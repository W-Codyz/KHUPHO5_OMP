const { EmbedBuilder } = require('discord.js');
const mysql = require('mysql2/promise');

// -------------------- KIỂM TRA BIẾN MÔI TRƯỜNG --------------------
const requiredEnv = ['DB_HOST', 'DB_USER', 'DB_PASSWORD', 'DB_DATABASE'];
for (const key of requiredEnv) {
    if (!process.env[key]) {
        console.error(`❌ Thiếu biến môi trường: ${key}`);
        // Nếu thiếu, export 1 hàm báo lỗi để tránh crash module
        module.exports = {
            name: 'players',
            description: 'Hiển thị danh sách người chơi đang online',
            run: async (interaction) => {
                if (!interaction.replied && !interaction.deferred) {
                    await interaction.reply({ content: '⚠️ Bot bị cấu hình sai biến môi trường DB. Liên hệ Admin!', ephemeral: true });
                }
            }
        };
        return; // Thoát khỏi module
    }
}

// Tạo Pool với timeout kết nối và lấy connection
const pool = mysql.createPool({
    host: process.env.DB_HOST,
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
    database: process.env.DB_DATABASE,
    waitForConnections: true,
    connectionLimit: 10,
    acquireTimeout: 10000, // 10s chờ lấy connection từ pool
    connectTimeout: 10000,  // 10s chờ kết nối TCP tới DB
});

module.exports = {
    name: 'players',
    description: 'Hiển thị danh sách người chơi đang online',
    run: async (interaction) => {
        // -------------------- XỬ LÝ DEFER --------------------
        try {
            await interaction.deferReply();
        } catch (deferError) {
            // Nếu không defer được (ví dụ interaction hết hạn), thử reply trực tiếp
            console.error('Lỗi defer:', deferError);
            if (!interaction.replied) {
                return interaction.reply({ 
                    content: '⚠️ Bot xử lý quá chậm hoặc tương tác đã hết hạn. Vui lòng thử lại!', 
                    ephemeral: true 
                }).catch(() => {});
            }
            return;
        }

        try {
            // -------------------- LẤY KẾT NỐI DB (CÓ TIMEOUT) --------------------
            const connection = await Promise.race([
                pool.getConnection(),
                new Promise((_, reject) => 
                    setTimeout(() => reject(new Error('DB_CONNECTION_TIMEOUT')), 10000)
                )
            ]);

            // -------------------- THỰC HIỆN QUERY (CÓ TIMEOUT) --------------------
            const queryPromise = connection.execute(
                'SELECT Username, Level FROM accounts WHERE Online = 1 ORDER BY Level DESC'
            );
            
            const timeoutPromise = new Promise((_, reject) => 
                setTimeout(() => reject(new Error('DB_QUERY_TIMEOUT')), 10000)
            );

            const [rows] = await Promise.race([queryPromise, timeoutPromise]);
            
            connection.release(); // Giải phóng connection sau khi xong

            // -------------------- XỬ LÝ KẾT QUẢ --------------------
            if (rows.length === 0) {
                const embed = new EmbedBuilder()
                    .setColor('#FFA500')
                    .setTitle('📡 Danh sách người chơi online')
                    .setDescription('Hiện không có người chơi nào đang online.')
                    .setFooter({ text: 'GTA-HTB Roleplay' })
                    .setTimestamp();
                return interaction.editReply({ embeds: [embed] });
            }

            let description = '```\n' +
                '#  Người chơi          Level\n' +
                '══════════════════════════════\n';

            const maxDisplay = 25;
            rows.slice(0, maxDisplay).forEach((player, index) => {
                const stt = `${index + 1}`.padStart(2);

                const name = player.Username.padEnd(18);
                const level = `${player.Level}`.padStart(3);

                description += `${stt} ${name}${level}\n`;
            });

            if (rows.length > maxDisplay) {
                description += `... và ${rows.length - maxDisplay} người khác\n`;
            }

            description += '```';

            const embed = new EmbedBuilder()
                .setColor('#00FF00')
                .setTitle(`🟢 Người chơi đang online (${rows.length})`)
                .setDescription(description)
                .setFooter({ text: 'GTA-HTB Roleplay • Cập nhật theo thời gian thực' })
                .setTimestamp();

            return interaction.editReply({ embeds: [embed] });

        } catch (error) {
            // -------------------- BẮT TẤT CẢ LỖI (KỂ CẢ TIMEOUT) --------------------
            console.error('Lỗi onlinelist:', error);

            let errorMessage = 'Không thể truy vấn dữ liệu. Vui lòng thử lại sau.';
            if (error.message === 'DB_CONNECTION_TIMEOUT') {
                errorMessage = '⏰ Không thể truy vấn dữ liệu (Connect timeout). Vui lòng thử lại.';
            } else if (error.message === 'DB_QUERY_TIMEOUT') {
                errorMessage = '⏰ Không thể truy vấn dữ liệu (Query timeout). Vui lòng thử lại.';
            } else if (error.code === 'ECONNREFUSED') {
                errorMessage = '🔌 Không thể truy vấn dữ liệu (Connection refused). Vui lòng thử lại.';
            }

            const errorEmbed = new EmbedBuilder()
                .setColor('#FF0000')
                .setTitle('❌ Lỗi hệ thống')
                .setDescription(errorMessage)
                .setFooter({ text: 'GTA-HTB Roleplay' })
                .setTimestamp();

            // Kiểm tra nếu interaction vẫn còn hợp lệ để editReply
            try {
                return await interaction.editReply({ embeds: [errorEmbed] });
            } catch (editError) {
                // Nếu editReply lỗi (ví dụ interaction đã hết hạn), thử followUp
                console.error('Lỗi editReply trong catch:', editError);
                return interaction.followUp({ 
                    content: '⚠️ Có lỗi xảy ra nhưng bot không thể sửa tin nhắn cũ. Vui lòng thử lệnh lại.', 
                    ephemeral: true 
                }).catch(() => {});
            }
        }
    }
};