const mysql = require('mysql2/promise');
const { EmbedBuilder, ApplicationCommandOptionType } = require('discord.js');

// Tạo pool connection
const pool = mysql.createPool({
    host: process.env.DB_HOST,
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
    database: process.env.DB_DATABASE
});

module.exports = {
    name: 'thongtin',
    description: 'Xem thông tin chi tiết của người chơi',
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

        try {
            const playerName = interaction.options.getString('ingame');
            const connection = await pool.getConnection();

            const query = `
                SELECT
                    Username,
                    LastLogin,
                    RegiDate,
                    ConnectedTime,
                    PayDay,
                    Sex,
                    Age,
                    BirthDate,
                    Level,
                    pHealth,
                    pArmor,
                    DiscordID,
                    Online,
                    Model
                FROM accounts
                WHERE Username = ?
            `;

            const [results] = await connection.execute(query, [playerName]);
            connection.release();

            if (results.length === 0) {
                const notFoundEmbed = new EmbedBuilder()
                    .setColor('#FF0000')
                    .setTitle('❌ Không tìm thấy người chơi')
                    .setDescription(`Không tìm thấy thông tin của người chơi: **${playerName}**`)
                    .setFooter({ text: 'Vui lòng kiểm tra lại tên người chơi' });
                
                return interaction.editReply({ embeds: [notFoundEmbed] });
            }

            const player = results[0];
            
            // Xử lý giới tính (2 là Nữ, còn lại là Nam)
            const sexEmoji = player.Sex === 2 ? '👩' : '👨';
            const sexText = player.Sex === 2 ? 'Nữ' : 'Nam';
            
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

            // Xử lý thông tin Discord
            let discordInfo = "Chưa liên kết";
            let verifyStatus = `❌ Chưa xác thực`;
            
            if (player.DiscordID && player.DiscordID !== '0') {
                discordInfo = `<@${player.DiscordID}>`;
                verifyStatus = '✅ Đã xác thực';
            }

            // Xử lý trạng thái online (sửa lỗi thiếu cột Online)
            const onlineStatus = player.Online === 1 ? '🟢 Đang online' : '🔴 Đang offline';

            // ConnectedTime = pConnectHours (số giờ), PayDay = pConnectSeconds (0-3599s)
            const hours = player.ConnectedTime || 0;
            const minutes = Math.floor((player.PayDay || 0) / 60);
            const playTime = `${hours} giờ ${minutes} phút`;

            // Tính phần trăm máu và giáp
            const healthPercent = Math.floor(player.pHealth);
            const armorPercent = Math.floor(player.pArmor);

            const getHealthColor = (percent) => {
                if (percent >= 75) return '🟢';
                if (percent >= 35) return '🟡';
                return '🔴';
            };

            // Tạo embed
            const embed = new EmbedBuilder()
                .setColor('#4169E1')
                .setTitle(`🎮 Thông tin người chơi: ${player.Username}`)
                .setDescription('' + '═'.repeat(40) + '')
                .setThumbnail(`https://assets.open.mp/assets/images/skins/${player.Model || 0}.png`) // Thêm thumbnail skin
                .addFields(
                    { 
                        name: '👤 Thông tin cơ bản',
                        value: [
                            `**Tên nhân vật:** ${player.Username}`,
                            `**Trạng thái:** ${onlineStatus}`,
                            `**Giới tính:** ${sexEmoji} ${sexText}`,
                            `**Tuổi:** 🎂 ${player.Age}`,
                            `**Cấp độ:** ⭐ ${player.Level}`,
                            `**Xác thực:** ${verifyStatus}`,
                            `**Discord:** ${discordInfo}`,
                        ].join('\n'),
                        inline: false
                    },
                    {
                        name: '❤️ Chỉ số',
                        value: [
                            `**Máu:** ${getHealthColor(healthPercent)} ${healthPercent}%`,
                            `**Giáp:** 🛡️ ${armorPercent}%`,
                        ].join('\n'),
                        inline: true
                    },
                    {
                        name: '⏰ Thời gian',
                        value: [
                            `**Thời gian chơi:** ⌛ ${playTime}`,
                            `**Ngày sinh:** 📅 ${player.BirthDate && player.BirthDate !== '00-00-0000' ? player.BirthDate : 'Chưa có thông tin'}`,
                        ].join('\n'),
                        inline: true
                    },
                    {
                        name: '📝 Thông tin khác',
                        value: [
                            `**Ngày đăng ký:** ${formatDate(player.RegiDate)}`,
                            `**Lần cuối đăng nhập:** ${formatDate(player.LastLogin)}`,
                        ].join('\n'),
                        inline: false
                    }
                )
                .setTimestamp()
                .setFooter({ text: 'GTA-HTB Roleplay' });

            await interaction.editReply({ embeds: [embed] });
        } catch (error) {
            console.error('Lỗi:', error);
            const errorEmbed = new EmbedBuilder()
                .setColor('#FF0000')
                .setTitle('❌ Lỗi hệ thống')
                .setDescription('Đã xảy ra lỗi. Vui lòng thử lại sau.')
                .setFooter({ text: 'GTA-HTB Roleplay' });
            
            return interaction.editReply({ embeds: [errorEmbed] });
        }
    }
};