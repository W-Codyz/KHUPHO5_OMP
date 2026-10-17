const { EmbedBuilder, ApplicationCommandOptionType } = require('discord.js');
const mysql = require('mysql2/promise');

// Tạo pool connection
const pool = mysql.createPool({
    host: process.env.DB_HOST,
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
    database: process.env.DB_DATABASE
});

module.exports = {
    name: 'xacminh',
    description: 'Liên kết tài khoản Discord với tài khoản in-game',
    options: [
        {
            name: "code",
            description: "Mã xác minh 6 chữ số",
            type: ApplicationCommandOptionType.String,
            required: true
        }
    ],
    run: async (interaction) => {
        // Chỉ cho phép sử dụng lệnh trong kênh xác thực
        const VERIFY_CHANNEL_ID = process.env.VERIFICATION_CHANNEL_ID || '1544221791369560155';
        const VERIFIED_ROLE_ID = process.env.VERIFIED_ROLE_ID || '1544225805565952030';
        
        if (interaction.channelId !== VERIFY_CHANNEL_ID) {
            const embed = new EmbedBuilder()
                .setColor('#FF0000')
                .setTitle('❌ Liên kết không thành công')
                .setDescription(`Câu lệnh chỉ có thể sử dụng tại kênh <#${VERIFY_CHANNEL_ID}>.`)
                .setFooter({ text: 'KP5 Roleplay' });
            
            return interaction.reply({ embeds: [embed], ephemeral: true });
        }

        await interaction.deferReply({ ephemeral: true });

        try {
            const discordId = interaction.user.id;
            const guild = interaction.guild;
            const member = await guild.members.fetch(discordId);
            
            // Kiểm tra đã có role verified chưa
            if (member.roles.cache.has(VERIFIED_ROLE_ID)) {
                const embed = new EmbedBuilder()
                    .setColor('#FF9900')
                    .setTitle('⚠️ Liên kết không thành công')
                    .setDescription('Bạn đã liên kết tài khoản rồi, không cần thực hiện lại.\n\n*Nếu có vấn đề gì, vui lòng liên hệ đội ngũ phát triển!*')
                    .setFooter({ text: 'KP5 Roleplay' });
                
                return interaction.editReply({ embeds: [embed] });
            }

            const code = interaction.options.getString('code');
            
            // Kiểm tra code phải là số và có 6 chữ số
            if (!/^\d{6}$/.test(code)) {
                const embed = new EmbedBuilder()
                    .setColor('#FF0000')
                    .setTitle('❌ Liên kết không thành công')
                    .setDescription('Mã liên kết không hợp lệ, vui lòng nhập đúng 6 chữ số.')
                    .setFooter({ text: 'KP5 Roleplay' });
                
                return interaction.editReply({ embeds: [embed] });
            }

            const realCode = parseInt(code);
            const connection = await pool.getConnection();

            // Tìm tài khoản với DiscordCode (chưa xác minh)
            const [accounts] = await connection.execute(
                'SELECT `id`, `Username`, `DiscordCode` FROM `accounts` WHERE `DiscordCode` = ? AND `DiscordCode` != 0',
                [realCode]
            );

            if (accounts.length === 0) {
                connection.release();
                const embed = new EmbedBuilder()
                    .setColor('#FF0000')
                    .setTitle('❌ Liên kết không thành công')
                    .setDescription('Không tìm thấy tài khoản với mã liên kết này.')
                    .setFooter({ text: 'KP5 Roleplay' });
                
                return interaction.editReply({ embeds: [embed] });
            }

            const account = accounts[0];
            const accountId = account.id;
            const accountName = account.Username;
            const discordCode = account.DiscordCode;

            // Kiểm tra code có khớp không
            if (discordCode === realCode) {
                // Update database
                await connection.execute(
                    'UPDATE `accounts` SET `DiscordID` = ? WHERE `id` = ?',
                    [discordId, accountId]
                );
                connection.release();

                // Thêm role verified
                try {
                    await member.roles.add(VERIFIED_ROLE_ID);
                } catch (roleError) {
                    console.error('Lỗi khi thêm role:', roleError);
                    // Vẫn tiếp tục, không làm gián đoạn
                }

                // 1. Tin nhắn riêng tư (ephemeral) cho người dùng
                const embed = new EmbedBuilder()
                    .setColor('#00FF00')
                    .setTitle('✅ Liên kết thành công')
                    .setDescription(`Tài khoản Discord của bạn đã được liên kết thành công đến tài khoản trò chơi ***${accountName}***\n\nBây giờ bạn có thể thực hiện các thao tác riêng tư/bảo mật với tài khoản của mình.\n\n***Chúc bạn có một trải nghiệm thật tuyệt tại KP5 Roleplay! 🎮***`)
                    .setFooter({ text: 'KP5 Roleplay' })
                    .setTimestamp();

                await interaction.editReply({ embeds: [embed] });

                // 2. Tin nhắn công khai cho mọi người trong kênh (kèm hướng dẫn)
                const publicEmbed = new EmbedBuilder()
                    .setColor('#00FF00')
                    .setTitle('✅ Xác minh thành công!')
                    .setDescription(`**${interaction.user.tag}** vừa xác minh tài khoản game **${accountName}**.`)
                    .addFields(
                        {
                            name: '📌 Hướng dẫn xác minh dành cho bạn',
                            value: `Nếu bạn chưa xác minh, hãy làm theo các bước sau:\n` +
                                `1️⃣ Trong game, lấy mã xác minh 6 chữ số (lệnh /code hoặc xem trong thông tin tài khoản).\n` +
                                `2️⃣ Vào kênh <#${process.env.VERIFICATION_CHANNEL_ID}> và dùng lệnh \`/xacminh <mã số>\` (ví dụ: \`/xacminh 123456\`).\n` +
                                `3️⃣ Sau khi xác minh, bạn sẽ được cấp role và có thể sử dụng đầy đủ các tính năng của server.`
                        }
                    )
                    .setFooter({ text: 'KP5 Roleplay' })
                    .setTimestamp();

                await interaction.channel.send({ embeds: [publicEmbed] });

                // 3. DM riêng (tùy chọn, vẫn giữ như cũ)
                try {
                    const dmEmbed = new EmbedBuilder()
                        .setColor('#00FF00')
                        .setTitle('✅ Tài khoản đã được xác minh')
                        .setDescription([
                            `Tài khoản Discord của bạn đã được liên kết thành công với tài khoản game **${accountName}**`,
                            '',
                            'Bây giờ bạn có thể sử dụng đầy đủ các tính năng của máy chủ!',
                            '',
                            '**Chúc bạn có trải nghiệm tuyệt vời tại KP5 Roleplay! 🎮**'
                        ].join('\n'))
                        .setFooter({ text: 'KP5 Roleplay' })
                        .setTimestamp();

                    await interaction.user.send({ embeds: [dmEmbed] });
                } catch (dmError) {
                    console.error('Không thể gửi DM:', dmError);
                }

                return;
            } else {
                connection.release();
                const embed = new EmbedBuilder()
                    .setColor('#FF0000')
                    .setTitle('❌ Liên kết không thành công')
                    .setDescription('Mã liên kết không hợp lệ.')
                    .setFooter({ text: 'KP5 Roleplay' });
                
                return interaction.editReply({ embeds: [embed] });
            }

        } catch (error) {
            console.error('Lỗi xác minh:', error);
            const errorEmbed = new EmbedBuilder()
                .setColor('#FF0000')
                .setTitle('❌ Lỗi hệ thống')
                .setDescription('Đã xảy ra lỗi trong quá trình xác minh. Vui lòng thử lại sau.')
                .setFooter({ text: 'KP5 Roleplay' });
            
            return interaction.editReply({ embeds: [errorEmbed] });
        }
    }
};
