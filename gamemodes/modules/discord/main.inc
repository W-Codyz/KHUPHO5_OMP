#include <YSI/YSI_Coding/y_hooks>

stock IsPlayerDiscordVerified(playerid)
{
    new id[32];
    strcat(id, PlayerInfo[playerid][pDiscordID], sizeof(id));
    if (strlen(id) == 0 || strcmp(id, "0") == 0) return 0;
    return 1;
}

stock IsDiscordCodeUsed(discordCode)
{
    new query[128], Cache:sqlCache;
    mysql_format(MainPipeline, query, sizeof(query), "SELECT COUNT(*) FROM `accounts` WHERE `DiscordCode` = '%d'", discordCode);
    sqlCache = mysql_query(MainPipeline, query);

    new result = cache_get_field_content_int(0, "COUNT(*)");
    cache_delete(sqlCache);
    if(result > 0) {
        return 1;
    }
    else {
        return 0;
    }
}

stock DiscordLog(DCC_Channel:channel, const title[], const string[], va_args<>)
{   
    if(channel == DCC_INVALID_CHANNEL) return 0;

    new szMsg[256], datetime[64];
    va_format(szMsg, sizeof(szMsg), string, va_start<3>);

    gettime(datetime[0], datetime[1], datetime[2]);
    getdate(datetime[3], datetime[4], datetime[5]);
    format(datetime, _, "%02d:%02d:%02d - %02d/%02d/%d", datetime[0], datetime[1], datetime[2], datetime[5], datetime[4], datetime[3]);
    
    new DCC_Embed:embed_log;
    embed_log = DCC_CreateEmbed(title, szMsg);
    DCC_SetEmbedFooter(embed_log, datetime);
    DCC_SetEmbedColor(embed_log, 0x85FF85);

    return DCC_SendChannelEmbedMessage(channel, embed_log);
}

stock SendInteractionEmbed(DCC_Interaction:interaction, DCC_User:user, color, const title[], const description[], va_args<>)
{
    new szMsg[1028];
    va_format(szMsg, sizeof(szMsg), description, va_start<5>);

    new userName[DCC_USERNAME_SIZE], footer[128];
    DCC_GetUserName(user, userName);
    format(footer, _, "Phản hồi này dành cho %s", userName);

    new DCC_Embed:embedmsg = DCC_CreateEmbed(title, szMsg);
    DCC_SetEmbedColor(embedmsg, color);
    DCC_SetEmbedFooter(embedmsg, footer);
    DCC_SetEmbedImage(embedmsg, "https://iili.io/F4IVMts.gif");

    return DCC_SendInteractionEmbed(interaction, embedmsg);
}

forward SendDiscordConnect(playerid);
public SendDiscordConnect(playerid)
{
    new szMsg[428], datetime[64];
    new DCC_Embed:embed_log;

    gettime(datetime[0], datetime[1], datetime[2]);
    getdate(datetime[3], datetime[4], datetime[5]);
    format(datetime, _, "%02d:%02d:%02d - %02d/%02d/%d", datetime[0], datetime[1], datetime[2], datetime[5], datetime[4], datetime[3]);
    format(szMsg, _, "Người chơi ***%s (%d)*** đã kết nối vào máy chủ.\n\n**IP Address: %s**\n**Thiết bị: %s**\n**Launcher: %s**", 
        GetPlayerNameExt(playerid), playerid, GetPlayerIpEx(playerid),
        IsPCPlayer(playerid) ? "Máy tính :desktop:" : "Điện thoại :mobile_phone:",
        IsPlayerUsingOmp(playerid) ? "open.mp Launcher" : "SA-MP Launcher");

    embed_log = DCC_CreateEmbed("PLAYER CONNECT", szMsg);
    DCC_SetEmbedFooter(embed_log, datetime);
    DCC_SetEmbedColor(embed_log, 0x02FF52);
    
    return DCC_SendChannelEmbedMessage(dclog_Join, embed_log);
}

forward SendDiscordDisconnect(playerid, reason);
public SendDiscordDisconnect(playerid, reason)
{
    new szMsg[428], datetime[64], szReason[32];
    new DCC_Embed:embed_log;

    // Lấy thời gian
    gettime(datetime[0], datetime[1], datetime[2]);
    getdate(datetime[3], datetime[4], datetime[5]);
    format(datetime, sizeof(datetime), "%02d:%02d:%02d - %02d/%02d/%d",
        datetime[0], datetime[1], datetime[2], datetime[5], datetime[4], datetime[3]);

    // Xác định chuỗi lý do dựa vào số reason
    switch (reason)
    {
        case 0: szReason = "Timeout (mất kết nối)";
        case 1: szReason = "Leaving (thoát chủ động)";
        case 2: szReason = "Kicked / Banned";
        default: szReason = "Không xác định";
    }

    format(szMsg, sizeof(szMsg),
        "Người chơi ***%s (%d)*** đã rời khỏi máy chủ.\n\n**Lý do: %s**",
        GetPlayerNameExt(playerid), playerid, szReason);

    embed_log = DCC_CreateEmbed("PLAYER DISCONNECT", szMsg);
    DCC_SetEmbedFooter(embed_log, datetime);
    DCC_SetEmbedColor(embed_log, 0xFF3D3D);
    
    return DCC_SendChannelEmbedMessage(dclog_Join, embed_log);
}

Discord_Init()
{
    DCC_SetBotActivity("GTA-HTB Roleplay");
    //Discord_SlashCommand();

    mainGuild = DCC_FindGuildById("1544209806024515614");

    dcRole_Verified = DCC_FindRoleById("1544225805565952030");
    dcRole_Admin = DCC_FindRoleById("1544259847841644554");
    dcRole_Police = DCC_FindRoleById("1548255570371551312");
    dcRole_Medic = DCC_FindRoleById("1548255508077613186");
    dc_Verify = DCC_FindChannelById("1544221791369560155");

    ccChannel = DCC_FindChannelById("1548216250415456376");
    dclog_Money = DCC_FindChannelById("1548216250415456376");
    dclog_Setvip = DCC_FindChannelById("1548216250415456376");
    dclog_AdminCmd = DCC_FindChannelById("1548216250415456376");

    dclog_Join = DCC_FindChannelById("1544235359196684319");
    dclog_Pay = DCC_FindChannelById("1548216250415456376");

    dclog_Pizza = DCC_FindChannelById("1548215799087366194");
    dclog_Sweeper = DCC_FindChannelById("1548215799087366194");
    dclog_Porter = DCC_FindChannelById("1548215799087366194");
    dclog_Trucker = DCC_FindChannelById("1548215799087366194");
    dclog_Construction = DCC_FindChannelById("1548215799087366194");
    dclog_CanSa = DCC_FindChannelById("1548215799087366194");
    dclog_Fishing = DCC_FindChannelById("1548215799087366194");
    dclog_Lumber = DCC_FindChannelById("1548215799087366194");
    dclog_Painter = DCC_FindChannelById("1548215799087366194");
    dclog_Taxi = DCC_FindChannelById("1548215799087366194");
    dclog_Electric = DCC_FindChannelById("1548215799087366194");
    dclog_Miner    = DCC_FindChannelById("1548215799087366194");

    //old server
    renameChannel = DCC_FindChannelById("");
	banChannel = DCC_FindChannelById("1548216250415456376");
    ruathoChannel = DCC_FindChannelById("1554072577255804939");
    nhaCaiChannel = DCC_FindChannelById("1554072577255804939"); //log AI nha cai
	creditChannel = DCC_FindChannelById("1548216250415456376");
	logChannel = DCC_FindChannelById("");
	logDCChannel = DCC_FindChannelById("");
	givegunChannel = DCC_FindChannelById("1548216250415456376");
	moneyChannel = DCC_FindChannelById("1548216250415456376");
	weaponChannel = DCC_FindChannelById("");
	giveChannel = DCC_FindChannelById("1548216250415456376");
	antiChannel = DCC_FindChannelById("1548013006141661205");
    
}

hook OnPlayerConnect(playerid)
{
    SendDiscordConnect(playerid);
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    SendDiscordDisconnect(playerid, reason);
    return 1;
}