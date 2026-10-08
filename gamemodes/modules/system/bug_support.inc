#include <YSI\YSI_Coding\y_hooks>

#define DIALOG_CHECKBUGGING 5416

new checkbugging[MAX_PLAYERS];

hook OnPlayerConnect(playerid)
{
    checkbugging[playerid] = 0;
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    checkbugging[playerid] = 0;
    return 1;
}

hook OnPlayerUpdate(playerid)
{
    if(checkbugging[playerid] == 1) return 1;

    // Bỏ qua nếu player đang trong interior/vw khác 0 (nhà, cửa hàng, ...)
    if(GetPlayerInterior(playerid) != 0 || GetPlayerVirtualWorld(playerid) != 0) return 1;

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    if(z < -20.0)
    {
        checkbugging[playerid] = 1;
        TogglePlayerControllable(playerid, 0);
        SetPVarFloat(playerid, "Posx", -1605.0);
        SetPVarFloat(playerid, "Posy",   720.0);
        SetPVarFloat(playerid, "Posz",    11.869579);
        ShowPlayerDialog(playerid, DIALOG_CHECKBUGGING, DIALOG_STYLE_MSGBOX, "{FF0000}[!] {ffffff}ANTI BUG", "{ffffff}He thong phat hien ban bi bug, vui long chon {FFAA5B}XU LY{ffffff} de len mat dat\nNeu day la nham lan xin hay bao cao voi Quan tri vien", "XU LY", "");
    }
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_CHECKBUGGING)
    {
        SetPlayerInterior(playerid, 0);
        SetPlayerVirtualWorld(playerid, 0);
        SetPlayerPos(playerid, GetPVarFloat(playerid, "Posx"), GetPVarFloat(playerid, "Posy"), GetPVarFloat(playerid, "Posz"));
        TogglePlayerControllable(playerid, true);
        checkbugging[playerid] = 0;
    }
    return 1;
}
