const { EmbedBuilder, ApplicationCommandOptionType, ActionRowBuilder, ButtonBuilder, ButtonStyle } = require('discord.js');
const mysql = require('mysql2/promise');

const pool = mysql.createPool({
    host: process.env.DB_HOST,
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
    database: process.env.DB_DATABASE
});

module.exports = {
    name: 'adminxacminh',
    description: 'Hỗ trợ xác minh tài khoản cho member (Admin only)',
    options: [
        {
            name: "ingame",
            description: "Tên nhân vật trong game",
            type: ApplicationCommandOptionType.String,
            required: true
        },
        {
            name: "member",
            description: "Tag Discord của người cần xác minh",
            type: ApplicationCommandOptionType.User,
            required: true
        }
    ],
    run: async (interaction) => {
        const ADMIN_ROLE_ID = process.env.ADMIN_ROLE_ID || '1403988721266856059';
        const VERIFIED_ROLE_ID = process.env.VERIFIED_ROLE_ID || '1430358438701174804';
        
        const member = interaction.member;
        if (!member.roles.cache.has(ADMIN_ROLE_ID)) {
            const embed = new EmbedBuilder()
                .setColor('#FF0000')
                .setTitle('❌ Lỗi quyền hạn')
                .setDescription('Bạn không có quyền sử dụng lệnh này.')
                .setFooter({ text: 'GTA-HTB Roleplay' });
            return interaction.reply({ embeds: [embed], ephemeral: true });
        }

        await interaction.deferReply();

        let connection;
        try {
            const playerName = interaction.options.getString('ingame');
            const targetUser = interaction.options.getUser('member');
            const targetMember = await interaction.guild.members.fetch(targetUser.id);
            
            connection = await pool.getConnection();

            // ---- Kiểm tra DiscordID đã liên kết chưa ----
            const [existingAccounts] = await connection.execute(
                'SELECT `id`, `Username` FROM `accounts` WHERE `DiscordID` = ? AND `DiscordID` != ?',
                [targetUser.id, '0']
            );

            if (existingAccounts.length > 0) {
                const existingAccount = existingAccounts[0];
                
                const row = new ActionRowBuilder()
                    .addComponents(
                        new ButtonBuilder()
                            .setCustomId(`confirm_${interaction.id}`)
                            .setLabel('Xác nhận chuyển đổi')
                            .setStyle(ButtonStyle.Danger),
                        new ButtonBuilder()
                            .setCustomId(`cancel_${interaction.id}`)
                            .setLabel('Hủy bỏ')
                            .setStyle(ButtonStyle.Secondary)
                    );

                const warningEmbed = new EmbedBuilder()
                    .setColor('#FF9900')
                    .setTitle('⚠️ Discord ID đã tồn tại')
                    .setDescription([
                        `Discord <@${targetUser.id}> đã được liên kết với tài khoản game: **${existingAccount.Username}**`,
                        '',
                        `**Bạn có muốn chuyển sang tài khoản mới:** **${playerName}**?`,
                        '',
                        '**Hành động này sẽ:**',
                        `• Xóa liên kết Discord khỏi tài khoản **${existingAccount.Username}**`,
                        `• Liên kết Discord với tài khoản **${playerName}**`,
                        '• Giữ nguyên nickname và role verified'
                    ].join('\n'))
                    .setFooter({ text: 'GTA-HTB Roleplay • Vui lòng xác nhận trong 30 giây' })
                    .setTimestamp();

                const response = await interaction.editReply({ 
                    embeds: [warningEmbed],
                    components: [row]
                });

                try {
                    const confirmation = await response.awaitMessageComponent({ 
                        filter: i => i.user.id === interaction.user.id,
                        time: 30000 
                    });

                    // Vô hiệu hóa nút ngay lập tức
                    await confirmation.update({
                        embeds: [warningEmbed],
                        components: []
                    });

                    if (confirmation.customId === `cancel_${interaction.id}`) {
                        const cancelEmbed = new EmbedBuilder()
                            .setColor('#FF0000')
                            .setTitle('❌ Đã hủy')
                            .setDescription('Thao tác xác minh đã bị hủy bỏ.')
                            .setFooter({ text: 'GTA-HTB Roleplay' });
                        await interaction.editReply({ embeds: [cancelEmbed], components: [] });
                        connection.release();
                        return;
                    }

                    // Xóa DiscordID khỏi tài khoản cũ
                    await connection.execute(
                        'UPDATE `accounts` SET `DiscordID` = ? WHERE `id` = ?',
                        ['0', existingAccount.id]
                    );

                } catch (error) {
                    // Timeout hoặc lỗi khác
                    const timeoutEmbed = new EmbedBuilder()
                        .setColor('#FF0000')
                        .setTitle('⏰ Hết thời gian hoặc lỗi')
                        .setDescription('Thao tác xác minh đã hết thời gian chờ hoặc bị lỗi.')
                        .setFooter({ text: 'GTA-HTB Roleplay' });
                    await interaction.editReply({ embeds: [timeoutEmbed], components: [] });
                    connection.release();
                    return;
                }
            }

            // ---- Lấy thông tin tài khoản mới (bao gồm Online) ----
            const [accounts] = await connection.execute(
                'SELECT `id`, `Username`, `DiscordID`, `DiscordCode`, `Online` FROM `accounts` WHERE `Username` = ?',
                [playerName]
            );

            if (accounts.length === 0) {
                const embed = new EmbedBuilder()
                    .setColor('#FF0000')
                    .setTitle('❌ Không tìm thấy tài khoản')
                    .setDescription(`Không tìm thấy tài khoản game với tên: **${playerName}**`)
                    .setFooter({ text: 'GTA-HTB Roleplay' });
                await interaction.editReply({ embeds: [embed], components: [] });
                connection.release();
                return;
            }

            const account = accounts[0];
            const accountId = account.id;

            // ---- Kiểm tra online ----
            if (account.Online === 1) {
                const embed = new EmbedBuilder()
                    .setColor('#FF0000')
                    .setTitle('❌ Tài khoản đang online')
                    .setDescription([
                        `Tài khoản **${playerName}** hiện đang **online** trong game.`,
                        'Vui lòng yêu cầu người chơi **thoát game** trước khi thực hiện xác minh.',
                        'Sau khi thoát, hãy thử lại lệnh này.'
                    ].join('\n'))
                    .setFooter({ text: 'GTA-HTB Roleplay' });
                await interaction.editReply({ embeds: [embed], components: [] });
                connection.release();
                return;
            }

            // ---- Cập nhật DiscordID ----
            await connection.execute(
                'UPDATE `accounts` SET `DiscordID` = ? WHERE `id` = ?',
                [targetUser.id, accountId]
            );

            connection.release();

            // ---- Thêm role verified ----
            try {
                await targetMember.roles.add(VERIFIED_ROLE_ID);
            } catch (roleError) {
                console.error('Lỗi khi thêm role:', roleError);
            }

            // ---- Thông báo thành công ----
            const successEmbed = new EmbedBuilder()
                .setColor('#00FF00')
                .setTitle('✅ Xác minh thành công')
                .setDescription([
                    `**Admin hỗ trợ:** ${interaction.user.tag}`,
                    `**Tài khoản game:** ${playerName}`,
                    `**Discord member:** <@${targetUser.id}>`,
                    '',
                    '✅ Đã liên kết tài khoản và cấp quyền xác minh thành công!'
                ].join('\n'))
                .setFooter({ text: 'GTA-HTB Roleplay' })
                .setTimestamp();
            
            await interaction.editReply({ embeds: [successEmbed], components: [] });

            // ---- DM cho member ----
            try {
                const dmEmbed = new EmbedBuilder()
                    .setColor('#00FF00')
                    .setTitle('✅ Tài khoản đã được xác minh')
                    .setDescription([
                        `Tài khoản Discord của bạn đã được Admin liên kết thành công với tài khoản game **${playerName}**`,
                        '',
                        'Bây giờ bạn có thể sử dụng đầy đủ các tính năng của máy chủ!',
                        '',
                        '**Chúc bạn có trải nghiệm tuyệt vời tại GTA-HTB Roleplay! 🎮**'
                    ].join('\n'))
                    .setFooter({ text: 'GTA-HTB Roleplay' })
                    .setTimestamp();
                await targetUser.send({ embeds: [dmEmbed] });
            } catch (dmError) {
                console.error('Không thể gửi DM:', dmError);
            }

        } catch (error) {
            console.error('Lỗi admin xác minh:', error);
            const errorEmbed = new EmbedBuilder()
                .setColor('#FF0000')
                .setTitle('❌ Lỗi hệ thống')
                .setDescription('Đã xảy ra lỗi trong quá trình xác minh. Vui lòng thử lại sau.')
                .setFooter({ text: 'GTA-HTB Roleplay' });
            if (connection) connection.release();
            return interaction.editReply({ embeds: [errorEmbed], components: [] });
        }
    }
};