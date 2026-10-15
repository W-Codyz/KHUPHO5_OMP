const { EmbedBuilder, ApplicationCommandOptionType } = require('discord.js');
const mysql = require('mysql2/promise');

const pool = mysql.createPool({
    host: process.env.DB_HOST,
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
    database: process.env.DB_DATABASE
});

module.exports = {
    name: 'checkip',
    description: 'Kiểm tra tất cả tài khoản sử dụng IP nhất định (Admin only)',
    options: [
        {
            name: 'ip',
            description: 'Địa chỉ IP cần kiểm tra (VD: 123.456.789.0)',
            type: ApplicationCommandOptionType.String,
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
            const ipInput = interaction.options.getString('ip').trim();

            const ipRegex = /^(\d{1,3}\.){3}\d{1,3}$/;
            if (!ipRegex.test(ipInput)) {
                const embed = new EmbedBuilder()
                    .setColor('#FF9900')
                    .setTitle('⚠️ Địa chỉ IP không hợp lệ')
                    .setDescription(`\`${ipInput}\` không phải định dạng IP hợp lệ.\nVui lòng nhập đúng định dạng: \`123.456.789.0\``)
                    .setFooter({ text: 'GTA-HTB Roleplay' });
                return interaction.editReply({ embeds: [embed] });
            }

            connection = await pool.getConnection();

            const [results] = await connection.execute(
                `SELECT
                    \`id\`, \`Username\`, \`Level\`, \`AdminLevel\`, \`Helper\`,
                    \`IP\`, \`SecureIP\`, \`DiscordID\`, \`Online\`,
                    \`LastLogin\`, \`RegiDate\`, \`ConnectedTime\`,
                    \`Band\`, \`PermBand\`, \`Warnings\`, \`WantedLevel\`,
                    \`Money\`, \`Bank\`, \`Credits\`,
                    \`VIPM\`, \`GVip\`, \`TempVIP\`, \`VIPExpire\`,
                    \`DonateRank\`, \`Developer\`, \`Scripter\`,
                    \`Jailed\`, \`JailTime\`, \`PayDay\`
                FROM \`accounts\`
                WHERE \`IP\` = ? OR \`SecureIP\` = ?
                ORDER BY \`LastLogin\` DESC
                LIMIT 30`,
                [ipInput, ipInput]
            );
            connection.release();

            if (results.length === 0) {
                const embed = new EmbedBuilder()
                    .setColor('#FF9900')
                    .setTitle('🔍 Không tìm thấy tài khoản')
                    .setDescription(`Không có tài khoản nào sử dụng IP: \`${ipInput}\``)
                    .setFooter({ text: 'GTA-HTB Roleplay' })
                    .setTimestamp();
                return interaction.editReply({ embeds: [embed] });
            }

            const formatMoney = (n) => Number(n).toLocaleString('vi-VN');
            const formatDate = (d) => d ? new Date(d).toLocaleString('vi-VN', { timeZone: 'Asia/Ho_Chi_Minh' }) : 'N/A';
            const onlineText = (o) => o === 1 ? '🟢' : '🔴';
            const banText = (band, perm) => {
                if (perm === 1) return '🚫 Perm Ban';
                if (band === 1) return '⛔ Temp Ban';
                return '✅ OK';
            };
            const vipText = (vipm, gvip, temp) => {
                if (gvip === 1) return '💎 Gold VIP';
                if (vipm === 1) return '⭐ VIP';
                if (temp === 1) return '⏳ Temp VIP';
                return 'Không';
            };
            const roleText = (acc) => {
                const roles = [];
                if (acc.AdminLevel > 0) roles.push(`Admin Lv${acc.AdminLevel}`);
                if (acc.Helper > 0) roles.push('Helper');
                if (acc.Developer > 0) roles.push('Dev');
                if (acc.Scripter > 0) roles.push('Scripter');
                return roles.length > 0 ? roles.join(', ') : 'Người chơi';
            };

            const lines = results.map((acc, i) => {
                const ipTag = acc.IP === ipInput && acc.SecureIP === ipInput
                    ? '(IP+Secure)' : acc.IP === ipInput ? '(IP)' : '(SecureIP)';
                // ConnectedTime = pConnectHours (số giờ), PayDay = pConnectSeconds (0-3599)
                const hours = acc.ConnectedTime || 0;
                const minutes = Math.floor((acc.PayDay || 0) / 60);
                const discordTag = acc.DiscordID && acc.DiscordID !== '0' ? `<@${acc.DiscordID}>` : '❌';

                return [
                    `**${i + 1}. ${acc.Username}** ${ipTag} ${onlineText(acc.Online)}`,
                    `> 🎯 Lv.**${acc.Level}** | ${roleText(acc)} | ${banText(acc.Band, acc.PermBand)}`,
                    `> 🎮 Discord: ${discordTag} | VIP: ${vipText(acc.VIPM, acc.GVip, acc.TempVIP)}`,
                    `> 💰 Tiền mặt: $${formatMoney(acc.Money)} | NH: $${formatMoney(acc.Bank)} | Credits: ${acc.Credits || 0}`,
                    `> ⚠️ Warns: ${acc.Warnings} | Wanted: ${acc.WantedLevel || 0} | Tù: ${acc.Jailed === 1 ? `${acc.JailTime}p` : 'Không'}`,
                    `> ⏱️ ${hours} giờ ${String(minutes).padStart(2, '0')} phút chơi | Đăng ký: ${formatDate(acc.RegiDate)}`,
                    `> 🕐 Đăng nhập cuối: ${formatDate(acc.LastLogin)}`
                ].join('\n');
            });

            const totalText = results.length === 30 ? ' *(tối đa 30, có thể còn hơn)*' : '';
            const headerLine = `Tìm thấy **${results.length}** tài khoản${totalText}:\n\n`;

            // Split description if too long
            const allLines = [...lines];
            const descChunks = [];
            let cur = headerLine;
            for (const line of allLines) {
                const next = (cur === headerLine ? '' : '\n\n') + line;
                if ((cur + next).length > 3900) {
                    descChunks.push(cur);
                    cur = line;
                } else {
                    cur += next;
                }
            }
            if (cur) descChunks.push(cur);

            const embeds = descChunks.map((chunk, idx) => new EmbedBuilder()
                .setColor('#00AAFF')
                .setTitle(idx === 0
                    ? `🔍 Tra cứu IP: \`${ipInput}\``
                    : `🔍 Tra cứu IP: \`${ipInput}\` (tiếp)`)
                .setDescription(chunk)
                .setFooter({ text: `GTA-HTB Roleplay • Admin: ${interaction.user.tag}` })
                .setTimestamp()
            );

            return interaction.editReply({ embeds: embeds.slice(0, 10) });

        } catch (error) {
            console.error('Lỗi checkip:', error);
            const errorEmbed = new EmbedBuilder()
                .setColor('#FF0000')
                .setTitle('❌ Lỗi hệ thống')
                .setDescription('Đã xảy ra lỗi khi tra cứu IP. Vui lòng thử lại sau.')
                .setFooter({ text: 'GTA-HTB Roleplay' });
            if (connection) connection.release();
            return interaction.editReply({ embeds: [errorEmbed] });
        }
    }
};
