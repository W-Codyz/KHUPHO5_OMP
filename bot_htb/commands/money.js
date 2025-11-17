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
    name: 'money',
    description: 'Hiển thị top 10 người chơi giàu nhất máy chủ',
    run: async (interaction) => {
        await interaction.deferReply();

        try {
            const connection = await pool.getConnection();

            const query = `
                SELECT 
                    Username, 
                    Money,
                    Bank,
                    (Money + Bank) as total
                FROM accounts 
                ORDER BY total DESC
                LIMIT 10
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

            const getRankEmoji = (index) => {
                if (index === 0) return '👑';
                if (index === 1) return '🥈';
                if (index === 2) return '🥉';
                return `${index + 1}.`;
            };

            const embed = new EmbedBuilder()
                .setColor('#FFD700')
                .setTitle('💰 Top 10 người chơi giàu nhất')
                .setDescription(results.map((player, index) => 
                    `${getRankEmoji(index)} **${player.Username}**\n` +
                    `💵 Tiền mặt: $${player.Money.toLocaleString()}\n` +
                    `🏦 Ngân hàng: $${player.Bank.toLocaleString()}\n` + 
                    `💰 Tổng tài sản: $${player.total.toLocaleString()}\n`
                ).join('\n'))
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