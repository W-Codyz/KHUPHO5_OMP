#include <YSI/YSI_Coding/y_hooks>

stock IsPlayerDiscordVerified(playerid)
{
    return (!isnull(PlayerInfo[playerid][pDiscordID]));
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
    DCC_SetEmbedColor(embed_log, 0xAA85FF);
    
    return DCC_SendChannelEmbedMessage(dclog_Join, embed_log);
}

Discord_Init()
{
    DCC_SetBotActivity("GTA-HTB Roleplay");

    mainGuild = DCC_FindGuildById("1399386558968238242");

    dcRole_Verified = DCC_FindRoleById("1430358438701174804");
    dcRole_Admin = DCC_FindRoleById("1403988721266856059");
    dcRole_Police = DCC_FindRoleById("1399386559031021716");
    dcRole_Medic = DCC_FindRoleById("1399386559031021714");
    dc_Verify = DCC_FindChannelById("1399386568468463663");

    ccChannel = DCC_FindChannelById("1399386568107495464");
    dclog_Money = DCC_FindChannelById("1337348377003687958");
    dclog_Setvip = DCC_FindChannelById("1337354392407511171");

    dclog_Join = DCC_FindChannelById("1439904079202615389");
    dclog_Pay = DCC_FindChannelById("1404061322043461663");

    dclog_Pizza = DCC_FindChannelById("");
    dclog_Sweeper = DCC_FindChannelById("");
    dclog_Porter = DCC_FindChannelById("");
    dclog_Trucker = DCC_FindChannelById("");
    dclog_Construction = DCC_FindChannelById("");

    //old server
    renameChannel = DCC_FindChannelById("1421517124035416135");
	banChannel = DCC_FindChannelById("1421517179572195378");
    ruathoChannel = DCC_FindChannelById("1437273293403984056");
	creditChannel = DCC_FindChannelById("1421517503418601633");
	logChannel = DCC_FindChannelById("1403894879209259170");
	logDCChannel = DCC_FindChannelById("1402483870913527838");
	givegunChannel = DCC_FindChannelById("1403901608559120404");
	moneyChannel = DCC_FindChannelById("1403902144435847309");
	weaponChannel = DCC_FindChannelById("1404059243014787092");
	giveChannel = DCC_FindChannelById("1404062766322679910");
	antiChannel = DCC_FindChannelById("1425846474352230492");
    
}