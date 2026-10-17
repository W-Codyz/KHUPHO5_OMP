const { EmbedBuilder, ApplicationCommandOptionType } = require('discord.js');
const mysql = require('mysql2/promise');

// Tạo pool connection
const pool = mysql.createPool({
    host: process.env.DB_HOST,
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
    database: process.env.DB_DATABASE
});

// Hàm format tiền
function formatMoney(amount) {
    return new Intl.NumberFormat('en-US').format(amount);
}

module.exports = {
    name: 'playerinfo',
    description: 'Xem thông tin chi tiết của người chơi (Admin only)',
    options: [
        {
            name: "username",
            description: "Tên nhân vật trong game",
            type: ApplicationCommandOptionType.String,
            required: true
        }
    ],
    run: async (interaction) => {
        const ADMIN_ROLE_ID = process.env.ADMIN_ROLE_ID || '1403988721266856059';
        
        // Kiểm tra quyền admin
        const member = interaction.member;
        if (!member.roles.cache.has(ADMIN_ROLE_ID)) {
            const embed = new EmbedBuilder()
                .setColor('#FF0000')
                .setTitle('❌ Lỗi quyền hạn')
                .setDescription('Bạn không có quyền sử dụng lệnh này.')
                .setFooter({ text: 'KP5 Roleplay' });
            
            return interaction.reply({ embeds: [embed], ephemeral: true });
        }

        await interaction.deferReply();

        try {
            const username = interaction.options.getString('username');
            
            if (!username || username.trim() === '') {
                const embed = new EmbedBuilder()
                    .setColor('#FF0000')
                    .setTitle('❌ Sai cú pháp')
                    .setDescription('Vui lòng nhập tên tài khoản cần tra cứu.')
                    .setFooter({ text: 'KP5 Roleplay' });
                
                return interaction.editReply({ embeds: [embed] });
            }

            const connection = await pool.getConnection();

            // Query thông tin người chơi
            const [results] = await connection.execute(
                `SELECT
                    RegiDate, LastLogin, Username, Money, Bank,
                    pHealth, pArmor, Model, PhoneNr, DiscordID, DiscordCode,
                    Level, Sex, Age, ConnectedTime, PayDay, BirthDate, Online
                FROM accounts
                WHERE Username = ?
                LIMIT 1`,
                [username]
            );

            connection.release();

            if (results.length === 0) {
                const embed = new EmbedBuilder()
                    .setColor('#FF0000')
                    .setTitle('❌ Không tìm thấy')
                    .setDescription(`Không tìm thấy tài khoản với tên: ***${username}***`)
                    .setFooter({ text: 'KP5 Roleplay' });
                
                return interaction.editReply({ embeds: [embed] });
            }

            const player = results[0];

            // Format ngày tháng
            const formatDate = (date) => {
                if (!date) return 'Chưa có thông tin';
                return new Date(date).toLocaleString('vi-VN', {
                    year: 'numeric',
                    month: '2-digit',
                    day: '2-digit',
                    hour: '2-digit',
                    minute: '2-digit'
                });
            };

            // Xử lý Discord
            let discordTag = 'Chưa liên kết';
            let verifyStatus = `❌ Chưa xác thực (Code: ${player.DiscordCode})`;
            
            if (player.DiscordID && player.DiscordID !== '0') {
                discordTag = `<@${player.DiscordID}>`;
                verifyStatus = '✅ Đã xác thực';
            }

            // Xử lý giới tính
            const genderEmoji = player.Sex === 2 ? '👩' : '👨';
            const genderText = player.Sex === 2 ? 'Nữ' : 'Nam';

            // ConnectedTime = pConnectHours (số giờ), PayDay = pConnectSeconds (0-3599s)
            const hours = player.ConnectedTime || 0;
            const minutes = Math.floor((player.PayDay || 0) / 60);
            const playTime = `${hours} giờ ${minutes} phút`;
            const birthDate = player.BirthDate && player.BirthDate !== '00-00-0000' ? player.BirthDate : 'Chưa có thông tin';

            // Xử lý trạng thái online
            const onlineStatus = player.Online === 1 ? '🟢 Đang online' : '🔴 Đang offline';

            // Tạo embed
            const embed = new EmbedBuilder()
                .setColor('#3498DB')
                .setTitle('📋 THÔNG TIN NHÂN VẬT')
                .setDescription([
                    `**👤 Tên nhân vật:** ${player.Username}`,
                    `**📶 Trạng thái:** ${onlineStatus}`,
                    `**${genderEmoji} Giới tính:** ${genderText}`,
                    `**🎂 Tuổi:** ${player.Age}`,
                    `**⭐ Level:** ${player.Level}`,
                    `**⏱️ Thời gian chơi:** ${playTime}`,
                    `**🎂 Ngày sinh:** ${birthDate}`,
                    `**📅 Ngày tạo:** ${formatDate(player.RegiDate)}`,
                    `**🕐 Đăng nhập cuối:** ${formatDate(player.LastLogin)}`,
                    `**💵 Tiền mặt:** $${formatMoney(player.Money)}`,
                    `**🏦 Ngân hàng:** $${formatMoney(player.Bank)}`,
                    `**❤️ Máu:** ${player.pHealth}%`,
                    `**🛡️ Giáp:** ${player.pArmor}%`,
                    `**📱 Số điện thoại:** ${player.PhoneNr}`,
                    `**🎭 Skin ID:** ${player.Model}`,
                    `**💬 Discord:** ${discordTag}`,
                    `**✅ Xác minh:** ${verifyStatus}`
                ].join('\n'))
                .setThumbnail(`https://assets.open.mp/assets/images/skins/${player.Model}.png`)
                .setFooter({ text: 'GTA KP5 - Player Information' })
                .setTimestamp();

            return interaction.editReply({ embeds: [embed] });

        } catch (error) {
            console.error('Lỗi truy vấn:', error);
            const errorEmbed = new EmbedBuilder()
                .setColor('#FF0000')
                .setTitle('❌ Lỗi hệ thống')
                .setDescription('Đã xảy ra lỗi. Vui lòng thử lại sau.')
                .setFooter({ text: 'KP5 Roleplay' });
            
            return interaction.editReply({ embeds: [errorEmbed] });
        }
    }
};
