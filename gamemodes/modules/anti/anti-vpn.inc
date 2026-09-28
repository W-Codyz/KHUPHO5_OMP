/*
    Anti VPN/Proxy Powered by https://proxycheck.io/
    https://github.com/Pawn-Hubs/antivpn
*/

#if defined iAntiVPN
    #endinput
#endif

#if !defined _INC_a_http
    #error adicione #include <a_http> em cima da #include <antivpn>
    #endinput
#endif

#define iAntiVPN

public OnPlayerConnect(playerid)
{
    new ip[16], string[59];
    GetPlayerIp(playerid, ip, sizeof(ip));
    format(string, sizeof(string), "proxycheck.io/v2/%s?vpn=1&asn=1", ip);
    HTTP(playerid, HTTP_GET, string, "", "AntiVPN");
    #if defined AntiVPN_OnPlayerConnect
        return AntiVPN_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect AntiVPN_OnPlayerConnect
#if defined AntiVPN_OnPlayerConnect
    forward AntiVPN_OnPlayerConnect(playerid);
#endif

forward AntiVPN(playerid, response_code, data[]);
public AntiVPN(playerid, response_code, data[])
{
    new nome[MAX_PLAYERS], string[144];
    GetPlayerName(playerid, nome, sizeof(nome));
    if(response_code == 200)
    {
        if(substr(data, "\"proxy\": \"yes\""))
        {
            format(string, sizeof(string), "{F70000}[ANTI-VPN]: {FFE700}%s (ID: %d) {FFFFFF}da bi {F70000}kick {FFFFFF}vi nghi ngo su dung {F70000}VPN/Proxy", nome, playerid);
            SendClientMessageToAll(-1, string);
            printf("[ANTI-VPN]: %s (ID: %d) bi phat hien su dung VPN/Proxy.", nome, playerid);

            new DCC_Channel:channel = DCC_FindChannelById("1548013006141661205");
            new nofi[520], title[128];
            format(title, sizeof(title), "**%s (ID: %d) bị phát hiện dùng VPN/Proxy**", GetPlayerNameEx(playerid), playerid);
            format(nofi, sizeof(nofi), ":shield: **ANTI-VPN**\n:no_entry: Người chơi **%s (ID: %d)** đã bị kick khỏi máy chủ vì nghi ngờ sử dụng VPN/Proxy.", GetPlayerNameEx(playerid), playerid);
            DiscordLog(channel, title, nofi);

            Kick(playerid);
        }
    }
    return 1;
}

substr(const source[], const find[])
{
    new src = strlen(source);
    new enc = strlen(find);
    for(new i = 0; i <= src - enc; i++)
    {
        new match = true;
        for(new j = 0; j < enc; j++)
        {
            if(source[i+j] != find[j])
            {
                match = false;
                break;
            }
        }
        if(match) return true;
    }
    return false;
}
