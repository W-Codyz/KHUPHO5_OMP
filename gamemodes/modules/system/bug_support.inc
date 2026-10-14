#include <YSI\YSI_Coding\y_hooks>

#define DIALOG_CHECKBUGGING 5416

new checkbugging[MAX_PLAYERS];
new bugCountdown[MAX_PLAYERS];

hook OnPlayerConnect(playerid)
{
    checkbugging[playerid] = 0;
    bugCountdown[playerid] = 0;
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    checkbugging[playerid] = 0;
    bugCountdown[playerid] = 0;
    return 1;
}

hook OnPlayerUpdate(playerid)
{
    if(checkbugging[playerid] == 1) return 1;
    if(PlayerInfo[playerid][pAdmin] != 0) return 1;

    if(GetPlayerInterior(playerid) == 0 && GetPlayerVirtualWorld(playerid) == 0)
    {
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);

        if(z < -95.0)
        {
            checkbugging[playerid] = 1;
            TogglePlayerControllable(playerid, 0);
            SetPVarFloat(playerid, "Posx", -1605.0);
            SetPVarFloat(playerid, "Posy",   720.0);
            SetPVarFloat(playerid, "Posz",    11.869579);
            SetPVarFloat(playerid, "BugX", x);
            SetPVarFloat(playerid, "BugY", y);
            SetPVarFloat(playerid, "BugZ", z);
            ShowPlayerDialog(playerid, DIALOG_CHECKBUGGING, DIALOG_STYLE_MSGBOX, "{FF0000}[!] {ffffff}ANTI BUG", "{ffffff}He thong phat hien ban bi bug, vui long chon {FFAA5B}XU LY{ffffff} de len mat dat\nNeu day la nham lan xin hay bao cao voi Quan tri vien", "XU LY", "Tu choi");

            new pName[MAX_PLAYER_NAME];
            GetPlayerName(playerid, pName, sizeof(pName));
            new hackMsg[512];
            format(hackMsg, sizeof(hackMsg), "⚠️ ANTI BUG - Phát hiện\n**Tên nhân vật:** %s\n**Tài khoản:** %s\n**ID:** %d\n**Tọa độ bug:** X: %.2f | Y: %.2f | Z: %.2f",
                GetPlayerNameEx(playerid), pName, playerid, x, y, z);
            discord_messHack(hackMsg);
        }
    }
    return 1;
}

forward BugCountdownTimer(playerid);
public BugCountdownTimer(playerid)
{
    if(!IsPlayerConnected(playerid)) return 0;

    bugCountdown[playerid]--;

    if(bugCountdown[playerid] <= 0)
    {
        TogglePlayerControllable(playerid, true);
        checkbugging[playerid] = 0;
        bugCountdown[playerid] = 0;
        SendMessageTD(playerid, 2000, "~g~Ban da duoc giai phong!");
    }
    else
    {
        SendMessageTD(playerid, 1100, "~r~Giai phong sau: ~w~%d ~r~giay", bugCountdown[playerid]);
        SetTimerEx("BugCountdownTimer", 1000, false, "i", playerid);
    }
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_CHECKBUGGING)
    {
        new pName[MAX_PLAYER_NAME];
        GetPlayerName(playerid, pName, sizeof(pName));

        new Float:bx = GetPVarFloat(playerid, "BugX");
        new Float:by = GetPVarFloat(playerid, "BugY");
        new Float:bz = GetPVarFloat(playerid, "BugZ");

        if(response) // XU LY
        {
            SetPlayerInterior(playerid, 0);
            SetPlayerVirtualWorld(playerid, 0);
            SetPlayerPos(playerid, GetPVarFloat(playerid, "Posx"), GetPVarFloat(playerid, "Posy"), GetPVarFloat(playerid, "Posz"));
            TogglePlayerControllable(playerid, true);
            checkbugging[playerid] = 0;
            new hackMsg2[512];
            format(hackMsg2, sizeof(hackMsg2), "✅ ANTI BUG - Xử lý\n**Tên nhân vật:** %s\n**Tài khoản:** %s\n**ID:** %d\n**Tọa độ bug:** X: %.2f | Y: %.2f | Z: %.2f\n**Kết quả:** Đã chọn xử lý, teleport về vị trí an toàn.",
                GetPlayerNameEx(playerid), pName, playerid, bx, by, bz);
            discord_messHack(hackMsg2);
        }
        else // TU CHOI
        {
            bugCountdown[playerid] = 5;
            SendMessageTD(playerid, 1100, "~r~Giai phong sau: ~w~5 ~r~giay");
            SetTimerEx("BugCountdownTimer", 1000, false, "i", playerid);
            new hackMsg3[512];
            format(hackMsg3, sizeof(hackMsg3), "⛔ ANTI BUG - Từ chối\n**Tên nhân vật:** %s\n**Tài khoản:** %s\n**ID:** %d\n**Tọa độ bug:** X: %.2f | Y: %.2f | Z: %.2f\n**Kết quả:** Từ chối xử lý, tự giải phóng sau 5 giây.",
                GetPlayerNameEx(playerid), pName, playerid, bx, by, bz);
            discord_messHack(hackMsg3);
        }
    }
    return 1;
}
