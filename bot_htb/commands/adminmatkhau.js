const { EmbedBuilder, ApplicationCommandOptionType } = require('discord.js');
const mysql = require('mysql2/promise');
const { whirlpool } = require('hash-wasm');

const pool = mysql.createPool({
    host: process.env.DB_HOST,
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
    database: process.env.DB_DATABASE
});

module.exports = {
    name: 'adminmatkhau',
    description: 'Đặt lại mật khẩu tài khoản game cho member khi quên mật khẩu (Admin only)',
    options: [
        {
            name: 'ingame',
            description: 'Tên nhân vật trong game',
            type: ApplicationCommandOptionType.String,
            required: true
        },
        {
            name: 'mat_khau_moi',
            description: 'Mật khẩu mới',
            type: ApplicationCommandOptionType.String,
            required: true
        }
    ],
    run: async (interaction) => {
        const ADMIN_ROLE_ID = process.env.ADMIN_ROLE_ID || '1544259847841644554';

        if (!interaction.member.roles.cache.has(ADMIN_ROLE_ID)) {
            return interaction.reply({
                embeds: [
                    new EmbedBuilder()
                        .setColor('#FF0000')
                        .setTitle('❌ Lỗi quyền hạn')
                        .setDescription('Bạn không có quyền sử dụng lệnh này.')
                        .setFooter({ text: 'KP5 Roleplay' })
                ],
                ephemeral: true
            });
        }

        await interaction.deferReply();

        const playerName = interaction.options.getString('ingame');
        const newPassword = interaction.options.getString('mat_khau_moi');

        // Whirlpool hash - cùng thuật toán với WP_Hash trong game
        const hashedPassword = await whirlpool(newPassword);

        let connection;
        try {
            connection = await pool.getConnection();

            const [accounts] = await connection.execute(
                'SELECT `id`, `Username`, `Online`, `AdminLevel`, `DiscordID` FROM `accounts` WHERE `Username` = ?',
                [playerName]
            );

            if (accounts.length === 0) {
                connection.release();
                return interaction.editReply({
                    embeds: [
                        new EmbedBuilder()
                            .setColor('#FF0000')
                            .setTitle('❌ Không tìm thấy tài khoản')
                            .setDescription(`Không tìm thấy tài khoản game với tên: **${playerName}**`)
                            .setFooter({ text: 'KP5 Roleplay' })
                    ]
                });
            }

            const account = accounts[0];

            if (account.Online === 1) {
                connection.release();
                return interaction.editReply({
                    embeds: [
                        new EmbedBuilder()
                            .setColor('#FF0000')
                            .setTitle('❌ Tài khoản đang online')
                            .setDescription([
                                `Tài khoản **${playerName}** hiện đang **online** trong game.`,
                                'Vui lòng yêu cầu người chơi **thoát game** trước khi đặt lại mật khẩu.'
                            ].join('\n'))
                            .setFooter({ text: 'KP5 Roleplay' })
                    ]
                });
            }

            // Không cho đổi mật khẩu tài khoản Admin (AdminLevel >= 2)
            if (account.AdminLevel >= 2) {
                connection.release();
                return interaction.editReply({
                    embeds: [
                        new EmbedBuilder()
                            .setColor('#FF0000')
                            .setTitle('❌ Không thể thực hiện')
                            .setDescription('Không thể đặt lại mật khẩu tài khoản Admin.')
                            .setFooter({ text: 'KP5 Roleplay' })
                    ]
                });
            }

            await connection.execute(
                'UPDATE `accounts` SET `Key` = ? WHERE `Username` = ? AND `AdminLevel` < 2',
                [hashedPassword, playerName]
            );

            connection.release();

            await interaction.editReply({
                embeds: [
                    new EmbedBuilder()
                        .setColor('#00FF00')
                        .setTitle('✅ Đặt lại mật khẩu thành công')
                        .setDescription([
                            `**Admin hỗ trợ:** ${interaction.user.tag}`,
                            `**Tài khoản game:** ${playerName}`,
                            '',
                            '✅ Mật khẩu đã được cập nhật thành công!'
                        ].join('\n'))
                        .setFooter({ text: 'KP5 Roleplay' })
                        .setTimestamp()
                ]
            });

            // Gửi DM cho member nếu có DiscordID liên kết
            if (account.DiscordID && account.DiscordID !== '0') {
                try {
                    const targetUser = await interaction.client.users.fetch(account.DiscordID);
                    await targetUser.send({
                        embeds: [
                            new EmbedBuilder()
                                .setColor('#FFA500')
                                .setTitle('🔑 Mật khẩu tài khoản đã được đặt lại')
                                .setDescription([
                                    `Mật khẩu tài khoản game **${playerName}** của bạn đã được Admin đặt lại.`,
                                    '',
                                    `**Mật khẩu mới:** \`${newPassword}\``,
                                    '',
                                    '⚠️ Vui lòng đổi lại mật khẩu sau khi đăng nhập!'
                                ].join('\n'))
                                .setFooter({ text: 'KP5 Roleplay' })
                                .setTimestamp()
                        ]
                    });
                } catch (dmError) {
                    console.error('Không thể gửi DM cho member:', dmError);
                }
            }

        } catch (error) {
            console.error('Lỗi adminmatkhau:', error);
            if (connection) connection.release();
            return interaction.editReply({
                embeds: [
                    new EmbedBuilder()
                        .setColor('#FF0000')
                        .setTitle('❌ Lỗi hệ thống')
                        .setDescription('Đã xảy ra lỗi. Vui lòng thử lại sau.')
                        .setFooter({ text: 'KP5 Roleplay' })
                ]
            });
        }
    }
};
