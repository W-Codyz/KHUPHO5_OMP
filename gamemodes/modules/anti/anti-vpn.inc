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

#include <YSI\YSI_Coding\y_hooks>

// ==================================================================
// DEFINES
// ==================================================================
#define MAX_VPN_WHITELIST       200
#define VPN_WHITELIST_FILE      "AntiVPNWhitelist.txt"

#define DIALOG_ANTIVPN_MAIN     (8320)
#define DIALOG_ANTIVPN_ADD      (8321)
#define DIALOG_ANTIVPN_LIST     (8322)
#define DIALOG_ANTIVPN_CONFIRM  (8323)

// ==================================================================
// VARIABLES
// ==================================================================
new g_VPNWhitelist[MAX_VPN_WHITELIST][16];
new g_VPNWhitelistCount;
new g_AdminSelectedWL[MAX_PLAYERS];
new g_AntiVPNKickIP[MAX_PLAYERS][16];

// ==================================================================
// WHITELIST SAVE / LOAD
// ==================================================================
stock SaveVPNWhitelist()
{
    new File:fHandle = fopen(VPN_WHITELIST_FILE, io_write);
    for(new i = 0; i < g_VPNWhitelistCount; i++)
    {
        new line[18];
        format(line, sizeof(line), "%s\r\n", g_VPNWhitelist[i]);
        fwrite(fHandle, line);
    }
    return fclose(fHandle);
}

stock LoadVPNWhitelist()
{
    g_VPNWhitelistCount = 0;
    if(!fexist(VPN_WHITELIST_FILE)) return 1;

    new File:fHandle = fopen(VPN_WHITELIST_FILE, io_read);
    new line[32];
    while(g_VPNWhitelistCount < MAX_VPN_WHITELIST && fread(fHandle, line))
    {
        new len = strlen(line);
        while(len > 0 && (line[len-1] == '\r' || line[len-1] == '\n' || line[len-1] == ' '))
            line[--len] = '\0';

        if(len > 0)
        {
            format(g_VPNWhitelist[g_VPNWhitelistCount], 16, "%s", line);
            g_VPNWhitelistCount++;
        }
    }
    return fclose(fHandle);
}

stock bool:IsIPWhitelisted(const ip[])
{
    for(new i = 0; i < g_VPNWhitelistCount; i++)
    {
        if(strcmp(g_VPNWhitelist[i], ip, true) == 0)
            return true;
    }
    return false;
}

// ==================================================================
// HOOKS
// ==================================================================
hook OnGameModeInit()
{
    LoadVPNWhitelist();
    printf("[ANTI-VPN] Whitelist loaded: %d IP(s).", g_VPNWhitelistCount);
    return 1;
}

public OnPlayerConnect(playerid)
{
    new ip[16], string[59];
    GetPlayerIp(playerid, ip, sizeof(ip));

    if(!IsIPWhitelisted(ip))
    {
        format(string, sizeof(string), "proxycheck.io/v2/%s?vpn=1&asn=1", ip);
        HTTP(playerid, HTTP_GET, string, "", "AntiVPN");
    }

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
forward AntiVPNKick(playerid);
public AntiVPN(playerid, response_code, data[])
{
    if(PlayerInfo[playerid][pAdmin] >= 2) return 1;

    new ip[16];
    GetPlayerIp(playerid, ip, sizeof(ip));
    if(IsIPWhitelisted(ip)) return 1;

    new nome[MAX_PLAYER_NAME], string[256];
    GetPlayerName(playerid, nome, sizeof(nome));
    if(response_code == 200)
    {
        if(substr(data, "\"proxy\": \"yes\""))
        {
            // Thong bao broadcast cho staff
            format(string, sizeof(string), "{F70000}[ANTI-VPN]: {FFE700}%s (ID: %d) {FFFFFF}da bi {F70000}kick {FFFFFF}vi nghi ngo su dung {F70000}VPN/Proxy", nome, playerid);
            ABroadCast(COLOR_YELLOW, string, 2);
            printf("[ANTI-VPN]: %s (ID: %d) IP: %s - bi phat hien su dung VPN/Proxy.", nome, playerid, ip);

            // Tin nhan cho nguoi choi truoc khi kick
            SendClientMessage(playerid, 0xFF0000FF, "___________________________________________");
            SendClientMessage(playerid, 0xFF4444FF, "  [ANTI-VPN] Ban da bi kick khoi may chu!");
            SendClientMessage(playerid, 0xFFFFFFFF, "  Ly do: He thong phat hien ban dang su dung VPN/Proxy.");
            SendClientMessage(playerid, 0xFFFFFFFF, "  Neu ban KHONG su dung VPN, hay lien he Admin de duoc ho tro.");
            SendClientMessage(playerid, 0xFF0000FF, "___________________________________________");

            // Discord log chi tiet
            new DCC_Channel:channel = DCC_FindChannelById("1548013006141661205");
            new nofi[640], title[128];
            format(title, sizeof(title), ":no_entry: ANTI-VPN | %s (ID: %d) bị kick", GetPlayerNameEx(playerid), playerid);
            format(nofi, sizeof(nofi),
                ":shield: **ANTI-VPN SYSTEM**\n\n\
                :bust_in_silhouette: **Người chơi:** %s (ID: %d)\n\
                :computer: **IP:** `%s`\n\
                :globe_with_meridians: **Lý do:** Phát hiện sử dụng VPN/Proxy\n\
                :hammer: **Hành động:** Kick khỏi máy chủ",
                GetPlayerNameEx(playerid), playerid, ip
            );
            DiscordLog(channel, title, nofi);

            GetPlayerIp(playerid, g_AntiVPNKickIP[playerid], 16);
            SetTimerEx("AntiVPNKick", 1500, false, "d", playerid);
        }
    }
    return 1;
}

public AntiVPNKick(playerid)
{
    if(!IsPlayerConnected(playerid)) return 1;

    new currentIP[16];
    GetPlayerIp(playerid, currentIP, sizeof(currentIP));
    if(strcmp(currentIP, g_AntiVPNKickIP[playerid]) != 0) return 1; // playerid bị tái sử dụng

    Kick(playerid);
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

// ==================================================================
// COMMANDS
// ==================================================================
CMD:antivpn(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 2)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong co quyen su dung lenh nay!");

    ShowAVPNMainDialog(playerid);
    return 1;
}

// ==================================================================
// DIALOG STOCKS
// ==================================================================
stock ShowAVPNMainDialog(playerid)
{
    new string[256];
    format(string, sizeof(string),
        "{FFFFFF}Tong so IP trong whitelist: {FFD700}%d/%d\n\
        {FFD700}> {FFFFFF}Them IP vao whitelist\n\
        {FFD700}> {FF6347}Xoa IP khoi whitelist",
        g_VPNWhitelistCount, MAX_VPN_WHITELIST
    );
    ShowPlayerDialog(playerid, DIALOG_ANTIVPN_MAIN, DIALOG_STYLE_LIST,
        "{33FF33}QUAN LY ANTI-VPN WHITELIST", string, "Chon", "Dong");
}

stock ShowAVPNListDialog(playerid)
{
    new string[4096], line[64];

    if(g_VPNWhitelistCount == 0)
    {
        ShowPlayerDialog(playerid, DIALOG_ANTIVPN_LIST, DIALOG_STYLE_MSGBOX,
            "{FF6347}WHITELIST RONG",
            "{FFFFFF}Chua co IP nao trong whitelist.\n\nSu dung menu chinh de them IP.",
            "Quay lai", "");
        return;
    }

    strcat(string, "{FFD700}STT\t{FFFFFF}Dia chi IP\n");
    for(new i = 0; i < g_VPNWhitelistCount; i++)
    {
        format(line, sizeof(line), "{FFD700}%d\t{FFFFFF}%s\n", i + 1, g_VPNWhitelist[i]);
        strcat(string, line);
    }

    ShowPlayerDialog(playerid, DIALOG_ANTIVPN_LIST, DIALOG_STYLE_TABLIST_HEADERS,
        "{FF6347}CHON IP DE XOA", string, "Xoa IP nay", "Quay lai");
}

// ==================================================================
// DIALOG HANDLER
// ==================================================================
hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid)
    {
        case DIALOG_ANTIVPN_MAIN:
        {
            if(!response) return 1;
            switch(listitem)
            {
                case 1: // Them IP
                {
                    ShowPlayerDialog(playerid, DIALOG_ANTIVPN_ADD, DIALOG_STYLE_INPUT,
                        "{33FF33}THEM IP VAO WHITELIST",
                        "{FFFFFF}Nhap dia chi IP can them vao whitelist:\n{AAAAAA}Vi du: 103.10.20.30",
                        "Them", "Huy");
                }
                case 2: ShowAVPNListDialog(playerid); // Xoa IP
            }
        }

        case DIALOG_ANTIVPN_ADD:
        {
            if(!response)
            {
                ShowAVPNMainDialog(playerid);
                return 1;
            }

            if(isnull(inputtext) || strlen(inputtext) < 7 || strlen(inputtext) > 15)
            {
                SendClientMessageEx(playerid, COLOR_GREY, "[ANTI-VPN] Dia chi IP khong hop le! (vi du: 1.2.3.4)");
                ShowAVPNMainDialog(playerid);
                return 1;
            }

            if(g_VPNWhitelistCount >= MAX_VPN_WHITELIST)
            {
                SendClientMessageEx(playerid, COLOR_GREY, "[ANTI-VPN] Whitelist da day!");
                return 1;
            }

            if(IsIPWhitelisted(inputtext))
            {
                SendClientMessageEx(playerid, COLOR_GREY, "[ANTI-VPN] IP nay da co trong whitelist!");
                ShowAVPNMainDialog(playerid);
                return 1;
            }

            format(g_VPNWhitelist[g_VPNWhitelistCount], 16, "%s", inputtext);
            g_VPNWhitelistCount++;
            SaveVPNWhitelist();

            new msg[128];
            format(msg, sizeof(msg), "[ANTI-VPN] {ffffff}Da them IP {FFD700}%s {FFFFFF}vao whitelist.", inputtext);
            SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
            ShowAVPNMainDialog(playerid);
        }

        case DIALOG_ANTIVPN_LIST:
        {
            if(!response)
            {
                ShowAVPNMainDialog(playerid);
                return 1;
            }

            if(listitem < 0 || listitem >= g_VPNWhitelistCount) return 1;
            g_AdminSelectedWL[playerid] = listitem;

            new str[256];
            format(str, sizeof(str),
                "{FF6347}Ban co chac chan muon xoa IP nay khoi whitelist?\n\n\
                {FFFFFF}IP: {FFD700}%s\n\n\
                {FFFF00}Hanh dong nay khong the hoan tac!",
                g_VPNWhitelist[listitem]
            );
            ShowPlayerDialog(playerid, DIALOG_ANTIVPN_CONFIRM, DIALOG_STYLE_MSGBOX,
                "{FF6347}XAC NHAN XOA IP", str, "Xoa", "Huy");
        }

        case DIALOG_ANTIVPN_CONFIRM:
        {
            if(!response)
            {
                ShowAVPNListDialog(playerid);
                return 1;
            }

            new idx = g_AdminSelectedWL[playerid];
            if(idx < 0 || idx >= g_VPNWhitelistCount) return 1;

            new removedIP[16];
            format(removedIP, sizeof(removedIP), "%s", g_VPNWhitelist[idx]);

            for(new i = idx; i < g_VPNWhitelistCount - 1; i++)
                format(g_VPNWhitelist[i], 16, "%s", g_VPNWhitelist[i + 1]);
            g_VPNWhitelistCount--;
            g_VPNWhitelist[g_VPNWhitelistCount][0] = '\0';

            SaveVPNWhitelist();

            new msg[128];
            format(msg, sizeof(msg), "[ANTI-VPN] {ffffff}Da xoa IP {FFD700}%s {FFFFFF}khoi whitelist.", removedIP);
            SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
            ShowAVPNMainDialog(playerid);
        }
    }
    return 1;
}
