async function updateVerificationRole(client, userId, guildId) {
    try {
        const guild = await client.guilds.fetch(guildId);
        const member = await guild.members.fetch(userId);
        const role = await guild.roles.fetch(process.env.VERIFIED_ROLE_ID);
        
        await member.roles.add(role);
        return true;
    } catch (error) {
        console.error('Lỗi khi cập nhật role:', error);
        return false;
    }
}

module.exports = { updateVerificationRole };