const mysql = require('mysql2/promise');
const { EmbedBuilder } = require('discord.js');

const pool = mysql.createPool({
    host: process.env.DB_HOST,
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
    database: process.env.DB_DATABASE
});

module.exports = {
    name: 'gangs',
    description: 'Hiển thị top 5 băng đảng hùng mạnh nhất theo độ nổi tiếng',
    run: async (interaction) => {
        await interaction.deferReply();

        try {
            const connection = await pool.getConnection();

            const query = `
                SELECT
                    f.Name,
                    f.fame,
                    f.Leader,
                    f.level,
                    COUNT(a.id) AS TotalMembers
                FROM families f
                LEFT JOIN accounts a ON a.FMember = f.ID
                WHERE f.Taken = 1
                GROUP BY f.ID
                ORDER BY f.fame DESC
                LIMIT 5
            `;

            const [results] = await connection.execute(query);
            connection.release();

            if (results.length === 0) {
                const embed = new EmbedBuilder()
                    .setColor('#ff3d3d')
                    .setTitle('❌ Không có dữ liệu')
                    .setDescription('Chưa có băng đảng nào trong hệ thống.')
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
                .setColor('#ff3d3d')
                .setTitle('🔫 Top 5 Băng Đảng Hùng Mạnh Nhất')
                .setDescription(results.map((gang, index) =>
                    `${getRankEmoji(index)} **${gang.Name}**\n` +
                    `⭐ Độ nổi tiếng: ${(gang.fame ?? 0).toLocaleString()}\n` +
                    `👤 Trùm: ${gang.Leader}\n` +
                    `👥 Thành viên: ${gang.TotalMembers}\n` +
                    `🏦 Level Gang: ${gang.level ?? 1}\n`
                ).join('\n'))
                .setFooter({ text: 'HTB Roleplay' })
                .setTimestamp();

            await interaction.editReply({ embeds: [embed] });
        } catch (error) {
            console.error('Lỗi truy vấn gangs:', error);
            const errorEmbed = new EmbedBuilder()
                .setColor('#ff3d3d')
                .setTitle('❌ Lỗi hệ thống')
                .setDescription('Đã xảy ra lỗi. Vui lòng thử lại sau.')
                .setFooter({ text: 'GTA-HTB Roleplay' });

            return interaction.editReply({ embeds: [errorEmbed] });
        }
    }
};
