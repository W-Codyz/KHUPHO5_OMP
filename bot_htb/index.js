const { Client, Collection, GatewayIntentBits } = require('discord.js');
const fs = require('fs');
const dotenv = require('dotenv');
const express = require('express');
const app = express();

dotenv.config();

const client = new Client({
    intents: [
        GatewayIntentBits.Guilds,
        GatewayIntentBits.GuildMessages
    ]
});

client.commands = new Collection();

// Load commands
const commandFiles = fs.readdirSync('./commands').filter(file => file.endsWith('.js'));
for (const file of commandFiles) {
    const command = require(`./commands/${file}`);
    client.commands.set(command.name, command);
    console.log(`Loaded command: ${command.name}`);
}

// Ready event
client.once('ready', async () => {
    console.log(`Bot is ready! Logged in as ${client.user.tag}`);

    // Register Slash Commands
    try {
        const commands = [];
        client.commands.forEach(command => {
            commands.push({
                name: command.name,
                description: command.description,
                options: command.options
            });
        });

        await client.application.commands.set(commands);
        console.log('Slash commands registered!');
    } catch (error) {
        console.error('Error registering slash commands:', error);
    }
});

// Handle Interactions
client.on('interactionCreate', async interaction => {
    if (!interaction.isCommand()) return;

    const command = client.commands.get(interaction.commandName);
    if (!command) return;

    try {
        await command.run(interaction);
    } catch (error) {
        console.error(error);
        if (interaction.deferred || interaction.replied) {
            await interaction.editReply({ 
                content: 'Đã xảy ra lỗi khi thực hiện lệnh!',
                embeds: [],
                components: []
            }).catch(console.error);
        } else {
            await interaction.reply({ 
                content: 'Đã xảy ra lỗi khi thực hiện lệnh!',
                ephemeral: true 
            }).catch(console.error);
        }
    }
});

app.get('/api/discord/addrole', async (req, res) => {
    const { userId, guildId } = req.query;

    if (!userId || !guildId) {
        return res.status(400).send('Thieu thong tin user hoac guild');
    }

    try {
        // Tạo một interaction giả để sử dụng command addrole
        const fakeInteraction = {
            internalRequest: true,
            userId,
            guildId,
            client: client  // bot client
        };

        const command = client.commands.get('addrole');
        await command.run(fakeInteraction);

        res.status(200).send('Them role thanh cong');
    } catch (error) {
        console.error('Loi:', error);
        res.status(500).send('Loi khi them role');
    }
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
    console.log(`API server dang chay tren port ${PORT}`);
});

client.login(process.env.BOT_TOKEN);

client.on('error', error => {
    console.error('Discord client error:', error);
});

process.on('unhandledRejection', error => {
    console.error('Unhandled promise rejection:', error);
});

process.on('uncaughtException', error => {
    console.error('Uncaught exception:', error);
    // Có thể thêm code để tự restart bot nếu cần
});