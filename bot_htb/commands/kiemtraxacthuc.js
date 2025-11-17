// commands/kiemtraxacthuc.js
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
    name: 'kiemtraxacthuc',
    description: 'Kiểm tra trạng thái xác minh của tài khoản',
    options: [
        {
            name: "ingame",
            description: "Tên nhân vật trong game",
            type: ApplicationCommandOptionType.String,
            required: true
        }
    ],
    run: async (interaction) => {
        await interaction.deferReply();

        const playerName = interaction.options.getString('ingame');

        try {
            const connection = await pool.getConnection();

            // Kiểm tra thông tin trong database
            const [rows] = await connection.execute(
                'SELECT DiscordID, DiscordCode FROM accounts WHERE Username = ?',
                [playerName]
            );

            connection.release();

            const embed = new EmbedBuilder()
                .setTimestamp();

            if (rows.length === 0) {
                embed.setColor('#FF0000')
                    .setTitle('❌ Không tìm thấy tài khoản')
                    .setDescription([
                        `🎮 **Tài khoản game:** ${playerName}`,
                        '❌ Không tìm thấy tài khoản này trong database!'
                    ].join('\n'))
                    .setFooter({ text: 'GTA-HTB Roleplay' });
            } else {
                const account = rows[0];

                // Nếu có DiscordID, lấy thông tin user Discord
                let discordTag = 'Chưa liên kết';
                if (account.DiscordID && account.DiscordID !== '0') {
                    discordTag = `<@${account.DiscordID}>`;
                }

                if (account.DiscordID && account.DiscordID !== '0' && (!account.DiscordCode || account.DiscordCode === 0)) {
                    embed.setColor('#00FF00')
                        .setTitle('✅ Đã xác thực')
                        .setDescription([
                            `🎮 **Tài khoản game:** ${playerName}`,
                            `👤 **Discord:** ${discordTag}`,
                            '✅ Tài khoản đã được xác thực thành công!'
                        ].join('\n'))
                        .setFooter({ text: 'GTA-HTB Roleplay' });
                } else if (account.DiscordCode && account.DiscordCode !== 0) {
                    embed.setColor('#FFFF00')
                        .setTitle('⚠️ Chưa xác thực')
                        .setDescription([
                            `🎮 **Tài khoản game:** ${playerName}`,
                            `🔑 **Mã xác minh:** ${account.DiscordCode}`,
                            '❌ Tài khoản đã có mã xác minh nhưng chưa hoàn tất!',
                            '',
                            '**Để hoàn tất xác thực:**',
                            `1. Sử dụng lệnh \`/xacminh\` tại kênh <#${process.env.VERIFICATION_CHANNEL_ID}>`,
                            `2. Nhập mã: \`${account.DiscordCode}\``
                        ].join('\n'))
                        .setFooter({ text: 'GTA-HTB Roleplay' });
                } else {
                    embed.setColor('#FF0000')
                        .setTitle('❌ Chưa có mã xác minh')
                        .setDescription([
                            `🎮 **Tài khoản game:** ${playerName}`,
                            '❌ Tài khoản chưa tạo mã xác minh!',
                            '',
                            '**Hướng dẫn lấy mã xác minh:**',
                            '1. Vào game GTA-HTB',
                            '2. Sử dụng lệnh \`/discord\`',
                            '3. Hệ thống sẽ tạo mã 6 chữ số',
                            `4. Dùng \`/xacminh\` tại <#${process.env.VERIFICATION_CHANNEL_ID}> với mã đó`
                        ].join('\n'))
                        .setFooter({ text: 'GTA-HTB Roleplay' });
                }
            }

            await interaction.editReply({ embeds: [embed] });
        } catch (error) {
            console.error('Lỗi:', error);
            const errorEmbed = new EmbedBuilder()
                .setColor('#FF0000')
                .setTitle('❌ Lỗi hệ thống')
                .setDescription('Đã xảy ra lỗi khi kiểm tra thông tin xác thực. Vui lòng thử lại sau.')
                .setFooter({ text: 'GTA-HTB Roleplay' });

            await interaction.editReply({ embeds: [errorEmbed] });
        }
    }
};