const { EmbedBuilder } = require('discord.js');
const { queryServer } = require('../utils/serverQuery');

module.exports = {
    name: 'players',
    description: 'Danh sách người chơi đang online',
    run: async (interaction) => {
        await interaction.deferReply();

        if(!process.env.SAMP_IP)
            return interaction.editReply('Cảnh báo! Vẫn chưa cài đặt IP cho bot.');

        const ip = process.env.SAMP_IP.split(':');
        const options = {
            host: ip[0],
            port: parseInt(ip[1]) || 7777
        };

        const embed = new EmbedBuilder();

        try {
            const info = await queryServer(options.host, options.port);
            console.log('Server info:', info); // Debug log
            
            embed.setColor('#33ff00')
                .setTitle('Lệnh `/players` đang bảo trì nên sẽ hiển thị thông tin\nmáy chủ thay vì danh sách người chơi\n\nMáy chủ đang hoạt động !')
                .setDescription([
                    `**IP:** \`gtahtb.top\``,
                    `**Số người chơi:** ${info.players}/${info.maxplayers}`,
                    `**Gamemode:** ${info.gamemode}`,
                    `**Language:** ${info.language}`,
                    '\n*Copy IP và dán vào phần "Favorite" trong SA-MP để vào server*'
                ].join('\n'));

        } catch (error) {
            console.error('Server query error:', error);
            embed.setColor('#ff0000')
                .setTitle('Máy chủ đang bảo trì !')
                .setDescription(`**IP:** \`gtahtb.top\``);
        }

        return interaction.editReply({ embeds: [embed] });
    }
};