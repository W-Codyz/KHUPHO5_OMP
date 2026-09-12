#include <YSI\YSI_Coding\y_hooks>

#define MAX_LOADING_BARS (10)

new bool:g_LoadingActive[MAX_PLAYERS];
new g_LoadingTimer[MAX_PLAYERS];
new g_LoadingBarCount[MAX_PLAYERS];
new g_LoadingFinishFunc[MAX_PLAYERS][64];

new PlayerText:g_LoadingTD[MAX_PLAYERS][3 + MAX_LOADING_BARS];
// [0] = viền đen ngoài
// [1] = viền đen trong
// [2] = khung nền xám
// [3..12] = 10 ô loading

hook OnPlayerConnect(playerid)
{
    g_LoadingActive[playerid]    = false;
    g_LoadingTimer[playerid]     = 0;
    g_LoadingBarCount[playerid]  = 0;
    g_LoadingFinishFunc[playerid][0] = EOS;

    // Viền đen ngoài
    g_LoadingTD[playerid][0] = CreatePlayerTextDraw(playerid, 260.000, 388.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, g_LoadingTD[playerid][0], 114.000, 23.000);
    PlayerTextDrawAlignment(playerid, g_LoadingTD[playerid][0], 1);
    PlayerTextDrawColour(playerid, g_LoadingTD[playerid][0], 255);
    PlayerTextDrawSetShadow(playerid, g_LoadingTD[playerid][0], 0);
    PlayerTextDrawSetOutline(playerid, g_LoadingTD[playerid][0], 0);
    PlayerTextDrawBackgroundColor(playerid, g_LoadingTD[playerid][0], 255);
    PlayerTextDrawFont(playerid, g_LoadingTD[playerid][0], 4);
    PlayerTextDrawSetProportional(playerid, g_LoadingTD[playerid][0], 1);

    // Viền đen trong
    g_LoadingTD[playerid][1] = CreatePlayerTextDraw(playerid, 260.000, 388.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, g_LoadingTD[playerid][1], 114.000, 23.000);
    PlayerTextDrawAlignment(playerid, g_LoadingTD[playerid][1], 1);
    PlayerTextDrawColour(playerid, g_LoadingTD[playerid][1], 255);
    PlayerTextDrawSetShadow(playerid, g_LoadingTD[playerid][1], 0);
    PlayerTextDrawSetOutline(playerid, g_LoadingTD[playerid][1], 0);
    PlayerTextDrawBackgroundColor(playerid, g_LoadingTD[playerid][1], 255);
    PlayerTextDrawFont(playerid, g_LoadingTD[playerid][1], 4);
    PlayerTextDrawSetProportional(playerid, g_LoadingTD[playerid][1], 1);

    // Khung nền xám
    g_LoadingTD[playerid][2] = CreatePlayerTextDraw(playerid, 262.000, 390.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, g_LoadingTD[playerid][2], 110.000, 19.000);
    PlayerTextDrawAlignment(playerid, g_LoadingTD[playerid][2], 1);
    PlayerTextDrawColour(playerid, g_LoadingTD[playerid][2], -1448498689);
    PlayerTextDrawSetShadow(playerid, g_LoadingTD[playerid][2], 0);
    PlayerTextDrawSetOutline(playerid, g_LoadingTD[playerid][2], 0);
    PlayerTextDrawBackgroundColor(playerid, g_LoadingTD[playerid][2], 255);
    PlayerTextDrawFont(playerid, g_LoadingTD[playerid][2], 4);
    PlayerTextDrawSetProportional(playerid, g_LoadingTD[playerid][2], 1);

    // 10 ô loading
    for(new i = 0; i < MAX_LOADING_BARS; i++)
    {
        g_LoadingTD[playerid][3 + i] = CreatePlayerTextDraw(playerid, 264.000 + (i * 12.0), 392.000, "LD_SPAC:white");
        PlayerTextDrawTextSize(playerid, g_LoadingTD[playerid][3 + i], 10.000, 15.000);
        PlayerTextDrawAlignment(playerid, g_LoadingTD[playerid][3 + i], 1);
        PlayerTextDrawColour(playerid, g_LoadingTD[playerid][3 + i], 579543807);
        PlayerTextDrawSetShadow(playerid, g_LoadingTD[playerid][3 + i], 0);
        PlayerTextDrawSetOutline(playerid, g_LoadingTD[playerid][3 + i], 0);
        PlayerTextDrawBackgroundColor(playerid, g_LoadingTD[playerid][3 + i], 255);
        PlayerTextDrawFont(playerid, g_LoadingTD[playerid][3 + i], 4);
        PlayerTextDrawSetProportional(playerid, g_LoadingTD[playerid][3 + i], 1);
    }
    return 1;
}

stock SetLoading(playerid, time, const text[], const function[]="", va_args<>)
{
    if(g_LoadingActive[playerid]) StopLoading(playerid);

    if(time < 500)  time = 500;
    if(time > 10000) time = 10000;

    // Ẩn toàn bộ trước khi show lại
    for(new i = 0; i < 3 + MAX_LOADING_BARS; i++)
        PlayerTextDrawHide(playerid, g_LoadingTD[playerid][i]);

    // Show khung
    PlayerTextDrawShow(playerid, g_LoadingTD[playerid][0]);
    PlayerTextDrawShow(playerid, g_LoadingTD[playerid][1]);
    PlayerTextDrawShow(playerid, g_LoadingTD[playerid][2]);

    g_LoadingBarCount[playerid] = 0;
    g_LoadingActive[playerid]   = true;
    format(g_LoadingFinishFunc[playerid], 64, "%s", function);

    // Mỗi ô chạy trong time/10 ms
    new interval = time / MAX_LOADING_BARS;
    if(interval < 100) interval = 100;

    g_LoadingTimer[playerid] = SetTimerEx("LoadingTick", interval, true, "i", playerid);
    return 1;
}

forward LoadingTick(playerid);
public LoadingTick(playerid)
{
    if(!g_LoadingActive[playerid])
    {
        if(g_LoadingTimer[playerid]) KillTimer(g_LoadingTimer[playerid]);
        g_LoadingTimer[playerid] = 0;
        return 0;
    }

    // Show ô tiếp theo
    if(g_LoadingBarCount[playerid] < MAX_LOADING_BARS)
    {
        PlayerTextDrawShow(playerid, g_LoadingTD[playerid][3 + g_LoadingBarCount[playerid]]);
        g_LoadingBarCount[playerid]++;
    }

    // Đủ 10 ô → kết thúc
    if(g_LoadingBarCount[playerid] >= MAX_LOADING_BARS)
    {
        if(g_LoadingTimer[playerid]) KillTimer(g_LoadingTimer[playerid]);
        g_LoadingTimer[playerid]   = 0;
        g_LoadingActive[playerid]  = false;
        g_LoadingBarCount[playerid] = 0;

        // Ẩn khung
        for(new i = 0; i < 3 + MAX_LOADING_BARS; i++)
            PlayerTextDrawHide(playerid, g_LoadingTD[playerid][i]);

        // Gọi callback
        if(strlen(g_LoadingFinishFunc[playerid]) > 0 && funcidx(g_LoadingFinishFunc[playerid]) != -1)
            CallLocalFunction(g_LoadingFinishFunc[playerid], "i", playerid);
        g_LoadingFinishFunc[playerid][0] = EOS;
        return 0;
    }
    return 1;
}

stock StopLoading(playerid)
{
    if(g_LoadingTimer[playerid]) {
        KillTimer(g_LoadingTimer[playerid]);
        g_LoadingTimer[playerid] = 0;
    }
    if(g_LoadingActive[playerid])
    {
        g_LoadingActive[playerid]   = false;
        g_LoadingBarCount[playerid] = 0;
        g_LoadingFinishFunc[playerid][0] = EOS;
        for(new i = 0; i < 3 + MAX_LOADING_BARS; i++)
            PlayerTextDrawHide(playerid, g_LoadingTD[playerid][i]);
        return 1;
    }
    return 0;
}

stock IsLoadingActive(playerid)
{
    return g_LoadingActive[playerid];
}

hook OnPlayerDisconnect(playerid, reason)
{
    StopLoading(playerid);
    return 1;
}

CMD:testloading(playerid, params[])
{
    SetLoading(playerid, 5000, "Dang tai du lieu...", "OnTestLoadingFinish");
    return 1;
}

forward OnTestLoadingFinish(playerid);
public OnTestLoadingFinish(playerid)
{
    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "Da tai xong du lieu!");
    return 1;
}