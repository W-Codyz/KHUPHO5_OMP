const { EmbedBuilder, ApplicationCommandOptionType } = require('discord.js');
const mysql = require('mysql2/promise');

const pool = mysql.createPool({
    host: process.env.DB_HOST,
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
    database: process.env.DB_DATABASE
});

module.exports = {
    name: 'checkdiscord',
    description: 'Kiểm tra tài khoản game được liên kết với Discord @user (Admin only)',
    options: [
        {
            name: 'member',
            description: 'Discord member cần kiểm tra',
            type: ApplicationCommandOptionType.User,
            required: true
        }
    ],
    run: async (interaction) => {
        const ADMIN_ROLE_ID = process.env.ADMIN_ROLE_ID || '1544259847841644554';
        if (!interaction.member.roles.cache.has(ADMIN_ROLE_ID)) {
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
            const targetUser = interaction.options.getUser('member');

            connection = await pool.getConnection();

            const [results] = await connection.execute(
                `SELECT
                    \`id\`, \`Username\`, \`Level\`, \`AdminLevel\`, \`Helper\`,
                    \`Developer\`, \`Scripter\`, \`SeniorModerator\`,
                    \`IP\`, \`SecureIP\`, \`DiscordID\`, \`Online\`,
                    \`LastLogin\`, \`RegiDate\`, \`ConnectedTime\`,
                    \`Band\`, \`PermBand\`, \`Warnings\`, \`WantedLevel\`,
                    \`Money\`, \`Bank\`, \`Credits\`, \`TotalCredits\`, \`Tokens\`,
                    \`Sex\`, \`Age\`, \`BirthDate\`, \`PhoneNr\`, \`IDCard\`,
                    \`VIPM\`, \`GVip\`, \`TempVIP\`, \`VIPExpire\`, \`DonateRank\`,
                    \`CarLic\`, \`FlyLic\`, \`BoatLic\`, \`FishLic\`, \`GunLic\`, \`TaxiLicense\`,
                    \`Job\`, \`Job2\`, \`TruckSkill\`, \`FishSkill\`, \`PorterSkill\`, \`PizzaSkill\`,
                    \`Jailed\`, \`JailTime\`,
                    \`Married\`, \`MarriedTo\`, \`PayDay\`,
                    \`Leader\`, \`Member\`, \`Division\`, \`FMember\`,
                    \`Hunger\`, \`Hydration\`, \`Fitness\`,
                    \`Respect\`, \`XP\`,
                    \`Model\`
                FROM \`accounts\`
                WHERE \`DiscordID\` = ? AND \`DiscordID\` != ?`,
                [targetUser.id, '0']
            );
            connection.release();

            if (results.length === 0) {
                const embed = new EmbedBuilder()
                    .setColor('#FF9900')
                    .setTitle('🔍 Không tìm thấy liên kết')
                    .setDescription(`Discord <@${targetUser.id}> (**${targetUser.tag}**) chưa được liên kết với bất kỳ tài khoản game nào.`)
                    .setThumbnail(targetUser.displayAvatarURL({ dynamic: true }))
                    .setFooter({ text: 'GTA-HTB Roleplay' })
                    .setTimestamp();
                return interaction.editReply({ embeds: [embed] });
            }

            const formatMoney = (n) => Number(n).toLocaleString('vi-VN');
            const formatDate = (d) => d ? new Date(d).toLocaleString('vi-VN', { timeZone: 'Asia/Ho_Chi_Minh' }) : 'N/A';
            const sexText = (s) => s === 1 ? '👨 Nam' : '👩 Nữ';
            const onlineText = (o) => o === 1 ? '🟢 Online' : '🔴 Offline';
            const licText = (val) => val === 1 ? '✅' : '❌';
            const banText = (band, perm) => {
                if (perm === 1) return '🚫 Banned vĩnh viễn';
                if (band === 1) return '⛔ Tạm Banned';
                return '✅ Bình thường';
            };
            const vipText = (acc) => {
                if (acc.GVip === 1) return '💎 Gold VIP';
                if (acc.VIPM === 1) return `⭐ VIP (hết hạn: ${acc.VIPExpire > 0 ? new Date(acc.VIPExpire * 1000).toLocaleDateString('vi-VN') : 'N/A'})`;
                if (acc.TempVIP === 1) return '⏳ Temp VIP';
                return '❌ Không có';
            };
            const roleText = (acc) => {
                const roles = [];
                if (acc.AdminLevel > 0) roles.push(`🛡️ Admin Lv${acc.AdminLevel}`);
                if (acc.SeniorModerator > 0) roles.push('👮 Senior Mod');
                if (acc.Helper > 0) roles.push('🤝 Helper');
                if (acc.Developer > 0) roles.push('💻 Developer');
                if (acc.Scripter > 0) roles.push('📝 Scripter');
                return roles.length > 0 ? roles.join(' | ') : '🎮 Người chơi';
            };
            const jobNames = {
                0: 'Thất nghiệp', 1: 'Tài xế xe tải', 2: 'Thợ sửa xe',
                3: 'Cảnh sát', 4: 'Y tá', 5: 'Thợ cơ khí',
                6: 'Ngư dân', 7: 'Luật sư', 8: 'Phu hồ', 9: 'Giao pizza'
            };
            const jobText = (j) => jobNames[j] ?? `Job #${j}`;
            const barText = (val, max = 100) => {
                const pct = Math.min(100, Math.max(0, val || 0));
                const filled = Math.round(pct / 10);
                return `[${'█'.repeat(filled)}${'░'.repeat(10 - filled)}] ${pct}%`;
            };

            const embeds = results.map((acc, i) => {
                // ConnectedTime = pConnectHours (số giờ), PayDay = pConnectSeconds (0-3599)
                const hours = acc.ConnectedTime || 0;
                const minutes = Math.floor((acc.PayDay || 0) / 60);

                return new EmbedBuilder()
                    .setColor('#A855F7')
                    .setTitle(`🎮 Tài khoản #${i + 1}: **${acc.Username}**`)
                    .setThumbnail(`https://assets.open.mp/assets/images/skins/${acc.Model ?? 0}.png`)
                    .addFields(
                        {
                            name: '👤 Discord',
                            value: [
                                `**User:** <@${targetUser.id}> (${targetUser.tag})`,
                                `**ID:** \`${targetUser.id}\``
                            ].join('\n'),
                            inline: false
                        },
                        {
                            name: '📋 Thông tin cơ bản',
                            value: [
                                `**Tên:** \`${acc.Username}\`  |  **Level:** ${acc.Level}`,
                                `**Vai trò:** ${roleText(acc)}`,
                                `**Giới tính:** ${sexText(acc.Sex)}  |  **Tuổi:** ${acc.Age}`,
                                `**Ngày sinh:** ${acc.BirthDate}`,
                                `**CCCD:** ${acc.IDCard !== 0 ? `\`${acc.IDCard}\`` : '❌ Chưa có'}`,
                                `**SĐT:** ${acc.PhoneNr !== 0 ? `\`${acc.PhoneNr}\`` : '❌ Chưa có'}`
                            ].join('\n'),
                            inline: true
                        },
                        {
                            name: '🌐 Kết nối',
                            value: [
                                `**Trạng thái:** ${onlineText(acc.Online)}`,
                                `**IP:** \`${acc.IP}\``,
                                `**SecureIP:** \`${acc.SecureIP}\``,
                                `**Giờ chơi:** ${hours} giờ ${String(minutes).padStart(2, '0')} phút`
                            ].join('\n'),
                            inline: true
                        },
                        {
                            name: '💰 Tài chính',
                            value: [
                                `**Tiền mặt:** $${formatMoney(acc.Money)}`,
                                `**Ngân hàng:** $${formatMoney(acc.Bank)}`,
                                `**Credits:** ${acc.Credits || 0} (tổng: ${acc.TotalCredits || 0})`,
                                `**Tokens:** ${acc.Tokens || 0}`,
                                `**Donate Rank:** ${acc.DonateRank > 0 ? `⭐ Rank ${acc.DonateRank}` : 'Không'}`,
                                `**VIP:** ${vipText(acc)}`
                            ].join('\n'),
                            inline: false
                        },
                        {
                            name: '⚖️ Trạng thái & Hành chính',
                            value: [
                                `**Tình trạng:** ${banText(acc.Band, acc.PermBand)}`,
                                `**Cảnh cáo:** ${acc.Warnings}  |  **Wanted:** ⭐${acc.WantedLevel || 0}`,
                                `**Đang tù:** ${acc.Jailed === 1 ? `✅ còn ${acc.JailTime} phút` : '❌'}`,
                                `**Hôn nhân:** ${acc.Married === 1 ? `💍 ${acc.MarriedTo}` : '❌ Độc thân'}`
                            ].join('\n'),
                            inline: true
                        },
                        {
                            name: '📊 Chỉ số sinh tồn',
                            value: [
                                `**Đói:** ${barText(acc.Hunger)}`,
                                `**Nước:** ${barText(acc.Hydration)}`,
                                `**Thể lực:** ${barText(acc.Fitness)}`,
                                `**Respect:** ${acc.Respect || 0}  |  **XP:** ${acc.XP || 0}`
                            ].join('\n'),
                            inline: true
                        },
                        {
                            name: '🚗 Bằng lái',
                            value: [
                                `**Ô tô:** ${licText(acc.CarLic)}  **Bay:** ${licText(acc.FlyLic)}  **Tàu:** ${licText(acc.BoatLic)}`,
                                `**Câu cá:** ${licText(acc.FishLic)}  **Súng:** ${licText(acc.GunLic)}  **Taxi:** ${licText(acc.TaxiLicense)}`
                            ].join('\n'),
                            inline: true
                        },
                        {
                            name: '💼 Công việc & Kỹ năng',
                            value: [
                                `**Job 1:** ${jobText(acc.Job)}  |  **Job 2:** ${acc.Job2 !== 0 ? jobText(acc.Job2) : 'Không'}`,
                                `**Xe tải:** Lv${acc.TruckSkill || 0}  |  **Câu cá:** Lv${acc.FishSkill || 0}`,
                                `**Bốc vác:** Lv${acc.PorterSkill || 0}  |  **Pizza:** Lv${acc.PizzaSkill || 0}`
                            ].join('\n'),
                            inline: true
                        },
                        {
                            name: '🏢 Faction / Gang',
                            value: [
                                `**Faction Member:** ${acc.Member !== -1 ? `Faction #${acc.Member} (Div: ${acc.Division !== -1 ? acc.Division : 'N/A'})` : '❌'}`,
                                `**Faction Leader:** ${acc.Leader !== -1 ? `Faction #${acc.Leader}` : '❌'}`,
                                `**Gang:** ${acc.FMember !== 255 ? `Gang #${acc.FMember}` : '❌'}`
                            ].join('\n'),
                            inline: true
                        },
                        {
                            name: '📅 Thời gian',
                            value: [
                                `**Ngày đăng ký:** ${formatDate(acc.RegiDate)}`,
                                `**Đăng nhập cuối:** ${formatDate(acc.LastLogin)}`
                            ].join('\n'),
                            inline: false
                        }
                    )
                    .setFooter({ text: `GTA-HTB Roleplay • Admin: ${interaction.user.tag} • ID: ${acc.id}` })
                    .setTimestamp();
            });

            return interaction.editReply({ embeds: embeds.slice(0, 10) });

        } catch (error) {
            console.error('Lỗi checkdiscord:', error);
            const errorEmbed = new EmbedBuilder()
                .setColor('#FF0000')
                .setTitle('❌ Lỗi hệ thống')
                .setDescription('Đã xảy ra lỗi khi tra cứu Discord. Vui lòng thử lại sau.')
                .setFooter({ text: 'GTA-HTB Roleplay' });
            if (connection) connection.release();
            return interaction.editReply({ embeds: [errorEmbed] });
        }
    }
};
