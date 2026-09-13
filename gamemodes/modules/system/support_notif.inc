#include <open.mp>
#include <YSI\YSI_Coding\y_hooks>

new PlayerText:scm_PTD[MAX_PLAYERS];
new bool:scm_PTDCreated[MAX_PLAYERS];
new scm_Timer[MAX_PLAYERS];

hook OnPlayerConnect(playerid)
{
    // Chỉ reset state — KHÔNG tạo TD
    scm_PTDCreated[playerid] = false;
    scm_PTD[playerid]        = PlayerText:INVALID_TEXT_DRAW;
    scm_Timer[playerid]      = 0;
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    // Kill timer để tránh fire vào slot player mới
    if(scm_Timer[playerid] != 0) {
        KillTimer(scm_Timer[playerid]);
        scm_Timer[playerid] = 0;
    }
    // Destroy TD nếu đã tạo (dù SA-MP tự free, cho chắc)
    if(scm_PTDCreated[playerid]) {
        PlayerTextDrawDestroy(playerid, scm_PTD[playerid]);
        scm_PTD[playerid] = PlayerText:INVALID_TEXT_DRAW;
        scm_PTDCreated[playerid] = false;
    }
    return 1;
}

stock CreateSCMTextDraw(playerid)
{
    if(scm_PTDCreated[playerid]) return 0;

    scm_PTD[playerid] = CreatePlayerTextDraw(playerid, 321.0000, 357.4996, " ");
    PlayerTextDrawLetterSize(playerid, scm_PTD[playerid], 0.2128, 1.1390);
    PlayerTextDrawTextSize(playerid, scm_PTD[playerid], 0.0000, 388.0000);
    PlayerTextDrawAlignment(playerid, scm_PTD[playerid], 2);
    PlayerTextDrawColour(playerid, scm_PTD[playerid], -1);
    PlayerTextDrawSetOutline(playerid, scm_PTD[playerid], 1);
    PlayerTextDrawBackgroundColor(playerid, scm_PTD[playerid], 255);
    PlayerTextDrawFont(playerid, scm_PTD[playerid], 1);
    PlayerTextDrawSetProportional(playerid, scm_PTD[playerid], 1);
    PlayerTextDrawSetShadow(playerid, scm_PTD[playerid], 0);

    scm_PTDCreated[playerid] = true;
    return 1;
}

stock DestroySCMTextDraw(playerid)
{
    if(!scm_PTDCreated[playerid]) return 0;

    PlayerTextDrawDestroy(playerid, scm_PTD[playerid]);
    scm_PTD[playerid] = PlayerText:INVALID_TEXT_DRAW;
    scm_PTDCreated[playerid] = false;
    return 1;
}

stock HienTextdraw(playerid, string[])
{
    // Create on demand — tạo lần đầu, sau đó tái sử dụng
    if(!scm_PTDCreated[playerid])
        CreateSCMTextDraw(playerid);

    // Kill timer cũ trước khi đặt timer mới — fix bug 1
    if(scm_Timer[playerid] != 0) {
        KillTimer(scm_Timer[playerid]);
        scm_Timer[playerid] = 0;
    }

    PlayerTextDrawSetString(playerid, scm_PTD[playerid], string);
    PlayerTextDrawShow(playerid, scm_PTD[playerid]);

    scm_Timer[playerid] = SetTimerEx("XoaTextDraw", 5000, false, "i", playerid);
    return 1;
}

forward XoaTextDraw(playerid);
public XoaTextDraw(playerid)
{
    scm_Timer[playerid] = 0; // timer tự hết, reset id

    if(scm_PTDCreated[playerid]) {
        PlayerTextDrawSetString(playerid, scm_PTD[playerid], " ");
        PlayerTextDrawHide(playerid, scm_PTD[playerid]);
        // Nếu muốn tiết kiệm slot tối đa, bỏ comment dòng dưới:
        // DestroySCMTextDraw(playerid);
    }
    return 1;
}

CMD:tdn(playerid, params[])
{
    new string[256];
    if(sscanf(params, "s[256]", string)) return SendClientMessageEx(playerid, COLOR_RED, "Su dung: /tdn <noi dung>");
    HienTextdraw(playerid, string);
    return 1;
}