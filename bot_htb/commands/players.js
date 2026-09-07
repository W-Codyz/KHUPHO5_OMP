const { EmbedBuilder } = require('discord.js');
const mysql = require('mysql2/promise');

const pool = mysql.createPool({
    host: process.env.DB_HOST,
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
    database: process.env.DB_DATABASE
});

module.exports = {
    name: 'players',
    description: 'Hiển thị danh sách người chơi đang online',
    run: async (interaction) => {
        // Không có flags => phản hồi công khai
        await interaction.deferReply();

        try {
            const connection = await pool.getConnection();

            // Chỉ lấy Username và Level
            const [rows] = await connection.execute(
                'SELECT Username, Level FROM accounts WHERE Online = 1 ORDER BY Username ASC'
            );
            connection.release();

            if (rows.length === 0) {
                const embed = new EmbedBuilder()
                    .setColor('#FFA500')
                    .setTitle('📡 Danh sách người chơi online')
                    .setDescription('Hiện không có người chơi nào đang online.')
                    .setFooter({ text: 'GTA-HTB Roleplay' })
                    .setTimestamp();
                return interaction.editReply({ embeds: [embed] });
            }

            const maxDisplay = 25;
            let playerList = rows.slice(0, maxDisplay).map((player, index) => {
                return `${index + 1}. **${player.Username}** (Lv.${player.Level})`;
            }).join('\n');

            if (rows.length > maxDisplay) {
                playerList += `\n... và ${rows.length - maxDisplay} người khác.`;
            }

            const embed = new EmbedBuilder()
                .setColor('#00FF00')
                .setTitle(`📡 Danh sách người chơi online (${rows.length})`)
                .setDescription(playerList)
                .setFooter({ text: 'GTA-HTB Roleplay' })
                .setTimestamp();

            return interaction.editReply({ embeds: [embed] });

        } catch (error) {
            console.error('Lỗi onlinelist:', error);
            const errorEmbed = new EmbedBuilder()
                .setColor('#FF0000')
                .setTitle('❌ Lỗi hệ thống')
                .setDescription('Không thể truy vấn dữ liệu. Vui lòng thử lại sau.')
                .setFooter({ text: 'GTA-HTB Roleplay' });
            return interaction.editReply({ embeds: [errorEmbed] });
        }
    }
};