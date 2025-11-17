const dgram = require('dgram');

function queryServer(ip, port) {
    return new Promise((resolve, reject) => {
        const client = dgram.createSocket('udp4');
        const packet = Buffer.from([
            'S'.charCodeAt(0), 'A'.charCodeAt(0), 'M'.charCodeAt(0), 'P'.charCodeAt(0),
            ...ip.split('.').map(n => parseInt(n)),
            port & 0xFF, (port >> 8) & 0xFF,
            'i'.charCodeAt(0)
        ]);

        let timeout = setTimeout(() => {
            client.close();
            reject(new Error('Query timeout'));
        }, 3000);

        client.on('message', (response) => {
            clearTimeout(timeout);
            try {
                console.log('Raw response:', response);
                
                // Đọc từ offset 11
                let offset = 11;
                
                // Bỏ qua password byte
                offset++;
                
                // Đọc số người chơi và max players
                const players = response.readUInt16LE(offset);
                const maxplayers = response.readUInt16LE(offset + 2);
                offset += 4;

                // Đọc hostname
                const hostnameLength = response.readUInt32LE(offset);
                offset += 4;
                const hostname = response.toString('utf8', offset, offset + hostnameLength);
                offset += hostnameLength;

                // Đọc gamemode
                const gamemodeLength = response.readUInt32LE(offset);
                offset += 4;
                const gamemode = response.toString('utf8', offset, offset + gamemodeLength);
                offset += gamemodeLength;

                // Đọc language
                const languageLength = response.readUInt32LE(offset);
                offset += 4;
                const language = response.toString('utf8', offset, offset + languageLength);

                const info = {
                    hostname,
                    players,
                    maxplayers,
                    gamemode,
                    language
                };

                console.log('Parsed info:', info);

                client.close();
                resolve(info);
            } catch (err) {
                client.close();
                reject(err);
            }
        });

        client.on('error', (err) => {
            clearTimeout(timeout);
            client.close();
            reject(err);
        });

        client.send(packet, 0, packet.length, port, ip);
    });
}

module.exports = { queryServer };