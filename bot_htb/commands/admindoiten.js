const { EmbedBuilder, ApplicationCommandOptionType } = require('discord.js');
const mysql = require('mysql2/promise');

const pool = mysql.createPool({
    host: process.env.DB_HOST,
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
    database: process.env.DB_DATABASE
});

module.exports = {
    name: 'admindoiten',
    description: 'Đổi tên nhân vật trong game cho member (Admin only)',
    options: [
        {
            name: 'member',
            description: 'Tag Discord của người cần đổi tên',
            type: ApplicationCommandOptionType.User,
            required: true
        },
        {
            name: 'ten_moi',
            description: 'Tên mới (định dạng: HoDem_Ten)',
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

        const targetUser = interaction.options.getUser('member');
        const newName = interaction.options.getString('ten_moi');

        // Validate tên mới - giống quy tắc trong game
        if (newName.length > 20) {
            return interaction.editReply({
                embeds: [
                    new EmbedBuilder()
                        .setColor('#FF0000')
                        .setTitle('❌ Tên không hợp lệ')
                        .setDescription('Tên không được dài hơn **20 ký tự**.')
                        .setFooter({ text: 'KP5 Roleplay' })
                ]
            });
        }

        if (newName.includes(' ')) {
            return interaction.editReply({
                embeds: [
                    new EmbedBuilder()
                        .setColor('#FF0000')
                        .setTitle('❌ Tên không hợp lệ')
                        .setDescription('Tên không được có khoảng trắng. Sử dụng `_` thay vì dấu cách.')
                        .setFooter({ text: 'KP5 Roleplay' })
                ]
            });
        }

        if (!newName.includes('_')) {
            return interaction.editReply({
                embeds: [
                    new EmbedBuilder()
                        .setColor('#FF0000')
                        .setTitle('❌ Tên không hợp lệ')
                        .setDescription('Tên phải theo định dạng **HoDem_Ten** (bắt buộc có dấu `_`).')
                        .setFooter({ text: 'KP5 Roleplay' })
                ]
            });
        }

        if (!/^[a-zA-Z0-9_]+$/.test(newName)) {
            return interaction.editReply({
                embeds: [
                    new EmbedBuilder()
                        .setColor('#FF0000')
                        .setTitle('❌ Tên không hợp lệ')
                        .setDescription('Tên chỉ được chứa **chữ cái**, **số** và dấu `_`.')
                        .setFooter({ text: 'KP5 Roleplay' })
                ]
            });
        }

        let connection;
        try {
            connection = await pool.getConnection();

            // Tìm tài khoản qua DiscordID
            const [accounts] = await connection.execute(
                'SELECT `id`, `Username`, `Online`, `AdminLevel` FROM `accounts` WHERE `DiscordID` = ? AND `DiscordID` != ?',
                [targetUser.id, '0']
            );

            if (accounts.length === 0) {
                connection.release();
                return interaction.editReply({
                    embeds: [
                        new EmbedBuilder()
                            .setColor('#FF0000')
                            .setTitle('❌ Không tìm thấy tài khoản')
                            .setDescription(`<@${targetUser.id}> chưa liên kết tài khoản game với Discord.`)
                            .setFooter({ text: 'KP5 Roleplay' })
                    ]
                });
            }

            const account = accounts[0];
            const oldName = account.Username;

            if (account.Online === 1) {
                connection.release();
                return interaction.editReply({
                    embeds: [
                        new EmbedBuilder()
                            .setColor('#FF0000')
                            .setTitle('❌ Tài khoản đang online')
                            .setDescription([
                                `Tài khoản **${oldName}** hiện đang **online** trong game.`,
                                'Vui lòng yêu cầu người chơi **thoát game** trước khi đổi tên.'
                            ].join('\n'))
                            .setFooter({ text: 'KP5 Roleplay' })
                    ]
                });
            }

            // Kiểm tra tên mới đã tồn tại chưa
            const [existingName] = await connection.execute(
                'SELECT `id` FROM `accounts` WHERE `Username` = ?',
                [newName]
            );

            if (existingName.length > 0) {
                connection.release();
                return interaction.editReply({
                    embeds: [
                        new EmbedBuilder()
                            .setColor('#FF0000')
                            .setTitle('❌ Tên đã tồn tại')
                            .setDescription(`Tên **${newName}** đã được sử dụng bởi tài khoản khác.`)
                            .setFooter({ text: 'KP5 Roleplay' })
                    ]
                });
            }

            await connection.execute(
                'UPDATE `accounts` SET `Username` = ? WHERE `DiscordID` = ?',
                [newName, targetUser.id]
            );

            connection.release();

            await interaction.editReply({
                embeds: [
                    new EmbedBuilder()
                        .setColor('#00FF00')
                        .setTitle('✅ Đổi tên thành công')
                        .setDescription([
                            `**Admin hỗ trợ:** ${interaction.user.tag}`,
                            `**Member:** <@${targetUser.id}>`,
                            `**Tên cũ:** ${oldName}`,
                            `**Tên mới:** ${newName}`,
                            '',
                            '✅ Tên nhân vật đã được cập nhật thành công!'
                        ].join('\n'))
                        .setFooter({ text: 'KP5 Roleplay' })
                        .setTimestamp()
                ]
            });

            // Gửi DM thông báo cho member
            try {
                await targetUser.send({
                    embeds: [
                        new EmbedBuilder()
                            .setColor('#00FF00')
                            .setTitle('✅ Tên nhân vật đã được thay đổi')
                            .setDescription([
                                'Tên nhân vật game của bạn đã được Admin thay đổi thành công.',
                                '',
                                `**Tên cũ:** ${oldName}`,
                                `**Tên mới:** ${newName}`,
                                '',
                                '🎮 Chúc bạn chơi game vui vẻ!'
                            ].join('\n'))
                            .setFooter({ text: 'KP5 Roleplay' })
                            .setTimestamp()
                    ]
                });
            } catch (dmError) {
                console.error('Không thể gửi DM cho member:', dmError);
            }

        } catch (error) {
            console.error('Lỗi admindoiten:', error);
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
