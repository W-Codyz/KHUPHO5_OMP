const mysql = require('mysql2/promise');
const { EmbedBuilder } = require('discord.js');

// Tạo pool connection
const pool = mysql.createPool({
    host: process.env.DB_HOST,
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
    database: process.env.DB_DATABASE
});

module.exports = {
    name: 'level',
    description: 'Hiển thị top 20 người chơi có level cao nhất',
    run: async (interaction) => {
        await interaction.deferReply();

        try {
            const connection = await pool.getConnection();

            const query = `
                SELECT 
                    Username,
                    Level,
                    ConnectedTime
                FROM accounts 
                ORDER BY Level DESC
                LIMIT 20
            `;

            const [results] = await connection.execute(query);
            connection.release();

            if (results.length === 0) {
                const embed = new EmbedBuilder()
                    .setColor('#FF0000')
                    .setTitle('❌ Không có dữ liệu')
                    .setDescription('Chưa có người chơi nào trong hệ thống.')
                    .setFooter({ text: 'GTA-HTB Roleplay' });
                
                return interaction.editReply({ embeds: [embed] });
            }

            // Tạo header cho bảng
            let description = '```\n' +
                '#  Người chơi          Level     Giờ chơi\n' +
                '══════════════════════════════════════════════\n';

            // Thêm từng dòng dữ liệu
            results.forEach((player, index) => {
                // Format số thứ tự với icon medal cho top 3
                let stt;
                if (index === 0) stt = '👑';
                else if (index === 1) stt = '🥈';
                else if (index === 2) stt = '🥉';
                else stt = `${index + 1}`.padStart(2);

                const name = player.Username.padEnd(18);
                const level = `${player.Level}`.padStart(3).padEnd(9);
                
                // ConnectedTime tính bằng giây, chuyển sang giờ
                const hours = (player.ConnectedTime && player.ConnectedTime > 0) 
                    ? Math.floor(player.ConnectedTime / 3600).toLocaleString().padStart(6)
                    : "0".padStart(6);

                description += `${stt} ${name}${level}${hours}\n`;
            });

            description += '\n``` ';

            const embed = new EmbedBuilder()
                .setColor('#FFD700')
                .setTitle('🏆 Bảng xếp hạng Level')
                .setDescription(description)
                .setFooter({ text: 'GTA-HTB Roleplay • Cập nhật theo thời gian thực' })
                .setTimestamp();

            await interaction.editReply({ embeds: [embed] });
        } catch (error) {
            console.error('Lỗi truy vấn:', error);
            const errorEmbed = new EmbedBuilder()
                .setColor('#FF0000')
                .setTitle('❌ Lỗi hệ thống')
                .setDescription('Đã xảy ra lỗi khi truy vấn dữ liệu. Vui lòng thử lại sau.')
                .setFooter({ text: 'GTA-HTB Roleplay' });
            
            return interaction.editReply({ embeds: [errorEmbed] });
        }
    }
};