const mysql = require('mysql2/promise');
const { EmbedBuilder } = require('discord.js');

const pool = mysql.createPool({
    host: process.env.DB_HOST,
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
    database: process.env.DB_DATABASE
});

module.exports = {
    name: 'online',
    description: 'Hiển thị top 20 người chơi có giờ chơi cao nhất',
    run: async (interaction) => {
        await interaction.deferReply();

        try {
            const connection = await pool.getConnection();

            const [results] = await connection.execute(`
                SELECT Username, Level, ConnectedTime, PayDay
                FROM accounts
                ORDER BY ConnectedTime DESC
                LIMIT 20
            `);

            connection.release();

            if (results.length === 0) {
                const embed = new EmbedBuilder()
                    .setColor('#FF0000')
                    .setTitle('❌ Không có dữ liệu')
                    .setDescription('Chưa có người chơi nào trong hệ thống.')
                    .setFooter({ text: 'GTA-HTB Roleplay' });

                return interaction.editReply({ embeds: [embed] });
            }

            let description = '```\n' +
                '#  Người chơi          Level  Giờ chơi        \n' +
                '══════════════════════════════════════════════════\n';

            results.forEach((player, index) => {
                let stt;
                if (index === 0) stt = '👑';
                else if (index === 1) stt = '🥈';
                else if (index === 2) stt = '🥉';
                else stt = `${index + 1}`.padStart(2);

                const name = player.Username.padEnd(18);
                const level = `${player.Level}`.padStart(3).padEnd(7);
                // ConnectedTime = pConnectHours (số giờ), PayDay = pConnectSeconds (0-3599)
                const h = player.ConnectedTime || 0;
                const m = Math.floor((player.PayDay || 0) / 60);
                const playTime = `${h} giờ ${String(m).padStart(2, '0')} phút`.padStart(16);

                description += `${stt} ${name}${level}${playTime}\n`;
            });

            description += '\n```';

            const embed = new EmbedBuilder()
                .setColor('#00BFFF')
                .setTitle('⏱️ Bảng xếp hạng Giờ chơi')
                .setDescription(description)
                .setFooter({ text: 'HTB Roleplay' })
                .setTimestamp();

            await interaction.editReply({ embeds: [embed] });
        } catch (error) {
            console.error('Lỗi truy vấn:', error);
            const errorEmbed = new EmbedBuilder()
                .setColor('#FF0000')
                .setTitle('❌ Lỗi hệ thống')
                .setDescription('Đã xảy ra lỗi. Vui lòng thử lại sau.')
                .setFooter({ text: 'GTA-HTB Roleplay' });

            return interaction.editReply({ embeds: [errorEmbed] });
        }
    }
};
