const { EmbedBuilder, ApplicationCommandOptionType, ActionRowBuilder, ButtonBuilder, ButtonStyle } = require('discord.js');
const mysql = require('mysql2/promise');

const pool = mysql.createPool({
    host: process.env.DB_HOST,
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
    database: process.env.DB_DATABASE
});

module.exports = {
    name: 'admintaocccd',
    description: 'Tạo căn cước công dân cho người chơi đã liên kết Discord (Admin only)',
    options: [
        {
            name: 'member',
            description: 'Người chơi Discord cần tạo CCCD',
            type: ApplicationCommandOptionType.User,
            required: true
        }
    ],
    run: async (interaction) => {
        // === Kiểm tra quyền Admin ===
        const ADMIN_ROLE_ID = process.env.ADMIN_ROLE_ID || '1403988721266856059';
        if (!interaction.member.roles.cache.has(ADMIN_ROLE_ID)) {
            const embed = new EmbedBuilder()
                .setColor('#FF0000')
                .setTitle('❌ Lỗi quyền hạn')
                .setDescription('Bạn không có quyền sử dụng lệnh này.')
                .setFooter({ text: 'GTA-HTB Roleplay' });
            return interaction.reply({ embeds: [embed], ephemeral: true });
        }

        await interaction.deferReply({ ephemeral: false });

        try {
            const targetUser = interaction.options.getUser('member');
            const targetMember = await interaction.guild.members.fetch(targetUser.id);

            const connection = await pool.getConnection();

            // 1. Tìm tài khoản game theo DiscordID
            const [accounts] = await connection.execute(
                'SELECT `id`, `Username`, `DiscordID`, `IDCard` FROM `accounts` WHERE `DiscordID` = ? AND `DiscordID` != ?',
                [targetUser.id, '0']
            );

            if (accounts.length === 0) {
                connection.release();
                const embed = new EmbedBuilder()
                    .setColor('#FF0000')
                    .setTitle('❌ Chưa liên kết Discord')
                    .setDescription(`Discord <@${targetUser.id}> chưa được liên kết với bất kỳ tài khoản game nào.\nVui lòng yêu cầu người chơi liên kết trước.`)
                    .setFooter({ text: 'GTA-HTB Roleplay' });
                return interaction.editReply({ embeds: [embed] });
            }

            const account = accounts[0];
            const accountId = account.id;
            const playerName = account.Username;
            const currentIDCard = account.IDCard;

            // 2. Kiểm tra đã có CCCD chưa
            if (currentIDCard !== 0 && currentIDCard !== null) {
                // Hỏi Admin có muốn tạo mới không
                const row = new ActionRowBuilder()
                    .addComponents(
                        new ButtonBuilder()
                            .setCustomId(`confirm_${interaction.id}`)
                            .setLabel('✅ Tạo mới CCCD')
                            .setStyle(ButtonStyle.Danger),
                        new ButtonBuilder()
                            .setCustomId(`cancel_${interaction.id}`)
                            .setLabel('❌ Hủy')
                            .setStyle(ButtonStyle.Secondary)
                    );

                const warningEmbed = new EmbedBuilder()
                    .setColor('#FF9900')
                    .setTitle('⚠️ CCCD đã tồn tại')
                    .setDescription([
                        `Tài khoản **${playerName}** đã có CCCD: **${currentIDCard}**.`,
                        '',
                        'Bạn có muốn **tạo mới** một CCCD khác không?',
                        '',
                        '⚠️ Lưu ý: CCCD cũ sẽ bị thay thế và không thể khôi phục.'
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

                    if (confirmation.customId === `cancel_${interaction.id}`) {
                        await confirmation.update({
                            embeds: [new EmbedBuilder()
                                .setColor('#FF0000')
                                .setTitle('❌ Đã hủy')
                                .setDescription('Thao tác tạo CCCD đã bị hủy.')
                                .setFooter({ text: 'GTA-HTB Roleplay' })
                            ],
                            components: []
                        });
                        connection.release();
                        return;
                    }

                    await confirmation.deferUpdate();
                } catch (error) {
                    await interaction.editReply({
                        embeds: [new EmbedBuilder()
                            .setColor('#FF0000')
                            .setTitle('⏰ Hết thời gian')
                            .setDescription('Thao tác tạo CCCD đã hết thời gian chờ.')
                            .setFooter({ text: 'GTA-HTB Roleplay' })
                        ],
                        components: []
                    });
                    connection.release();
                    return;
                }
            }

            // 3. Sinh số CCCD duy nhất (8 chữ số)
            let finalID = 0;
            let generated = false;
            let attempts = 0;
            while (!generated && attempts < 100) {
                const rand = Math.floor(Math.random() * 90000000) + 10000000; // từ 10.000.000 đến 99.999.999
                const [check] = await connection.execute(
                    'SELECT COUNT(*) as cnt FROM `accounts` WHERE `IDCard` = ? AND `id` != ?',
                    [rand, accountId]
                );
                if (check[0].cnt === 0) {
                    finalID = rand;
                    generated = true;
                }
                attempts++;
            }

            if (!generated) {
                connection.release();
                const embed = new EmbedBuilder()
                    .setColor('#FF0000')
                    .setTitle('❌ Lỗi tạo CCCD')
                    .setDescription('Không thể tìm được số CCCD duy nhất sau nhiều lần thử. Vui lòng thử lại sau.')
                    .setFooter({ text: 'GTA-HTB Roleplay' });
                return interaction.editReply({ embeds: [embed] });
            }

            // 4. Cập nhật IDCard vào database
            await connection.execute(
                'UPDATE `accounts` SET `IDCard` = ? WHERE `id` = ?',
                [finalID, accountId]
            );
            connection.release();

            // 5. Thông báo thành công (công khai) – KHÔNG DM
            const successEmbed = new EmbedBuilder()
                .setColor('#00FF00')
                .setTitle('✅ Tạo CCCD thành công')
                .setDescription([
                    `**Admin thực hiện:** ${interaction.user.tag}`,
                    `**Tài khoản game:** ${playerName}`,
                    `**Discord member:** <@${targetUser.id}>`,
                    '',
                    `**CCCD mới:** \`${finalID}\``,
                    '',
                    'Người chơi có thể sử dụng CCCD này trong game.'
                ].join('\n'))
                .setFooter({ text: 'GTA-HTB Roleplay' })
                .setTimestamp();

            await interaction.editReply({ embeds: [successEmbed], components: [] });

        } catch (error) {
            console.error('Lỗi tạo CCCD:', error);
            const errorEmbed = new EmbedBuilder()
                .setColor('#FF0000')
                .setTitle('❌ Lỗi hệ thống')
                .setDescription('Đã xảy ra lỗi khi tạo CCCD. Vui lòng thử lại sau.')
                .setFooter({ text: 'GTA-HTB Roleplay' });

            if (interaction.deferred) {
                return interaction.editReply({ embeds: [errorEmbed] });
            } else {
                return interaction.reply({ embeds: [errorEmbed], ephemeral: true });
            }
        }
    }
};