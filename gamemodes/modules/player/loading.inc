#include <YSI\YSI_Coding\y_hooks>

new PlayerText:Loader_TD[MAX_PLAYERS][6];
new loadingAnimation[MAX_PLAYERS];
new loadingFunction[MAX_PLAYERS][32];

stock createLoadingTD(playerid, const text[])
{

    Loader_TD[playerid][0] = CreatePlayerTextDraw(playerid, 229.000, 344.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Loader_TD[playerid][0], 0.000, 20.000);
    PlayerTextDrawAlignment(playerid, Loader_TD[playerid][0], 1);
    PlayerTextDrawColor(playerid, Loader_TD[playerid][0], 16744447);
    PlayerTextDrawSetShadow(playerid, Loader_TD[playerid][0], 0);
    PlayerTextDrawSetOutline(playerid, Loader_TD[playerid][0], 0);
    PlayerTextDrawBackgroundColor(playerid, Loader_TD[playerid][0], 255);
    PlayerTextDrawFont(playerid, Loader_TD[playerid][0], 4);
    PlayerTextDrawSetProportional(playerid, Loader_TD[playerid][0], 1);

    Loader_TD[playerid][1] = CreatePlayerTextDraw(playerid, 229.000, 363.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Loader_TD[playerid][1], 2.000, -18.000);
    PlayerTextDrawAlignment(playerid, Loader_TD[playerid][1], 1);
    PlayerTextDrawColor(playerid, Loader_TD[playerid][1], -1448498689);
    PlayerTextDrawSetShadow(playerid, Loader_TD[playerid][1], 0);
    PlayerTextDrawSetOutline(playerid, Loader_TD[playerid][1], 0);
    PlayerTextDrawBackgroundColor(playerid, Loader_TD[playerid][1], 255);
    PlayerTextDrawFont(playerid, Loader_TD[playerid][1], 4);
    PlayerTextDrawSetProportional(playerid, Loader_TD[playerid][1], 1);

    Loader_TD[playerid][2] = CreatePlayerTextDraw(playerid, 319.000, 348.000, text);
    PlayerTextDrawLetterSize(playerid, Loader_TD[playerid][2], 0.270, 1.098);
    PlayerTextDrawAlignment(playerid, Loader_TD[playerid][2], 2);
    PlayerTextDrawColor(playerid, Loader_TD[playerid][2], -1);
    PlayerTextDrawSetShadow(playerid, Loader_TD[playerid][2], 1);
    PlayerTextDrawSetOutline(playerid, Loader_TD[playerid][2], 1);
    PlayerTextDrawBackgroundColor(playerid, Loader_TD[playerid][2], 150);
    PlayerTextDrawFont(playerid, Loader_TD[playerid][2], 1);
    PlayerTextDrawSetProportional(playerid, Loader_TD[playerid][2], 1);

    Loader_TD[playerid][3] = CreatePlayerTextDraw(playerid, 229.000, 363.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Loader_TD[playerid][3], 174.000, 2.000);
    PlayerTextDrawAlignment(playerid, Loader_TD[playerid][3], 1);
    PlayerTextDrawColor(playerid, Loader_TD[playerid][3], -1448498689);
    PlayerTextDrawSetShadow(playerid, Loader_TD[playerid][3], 0);
    PlayerTextDrawSetOutline(playerid, Loader_TD[playerid][3], 0);
    PlayerTextDrawBackgroundColor(playerid, Loader_TD[playerid][3], 255);
    PlayerTextDrawFont(playerid, Loader_TD[playerid][3], 4);
    PlayerTextDrawSetProportional(playerid, Loader_TD[playerid][3], 1);

    Loader_TD[playerid][4] = CreatePlayerTextDraw(playerid, 229.000, 344.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Loader_TD[playerid][4], 174.000, 2.000);
    PlayerTextDrawAlignment(playerid, Loader_TD[playerid][4], 1);
    PlayerTextDrawColor(playerid, Loader_TD[playerid][4], -1448498689);
    PlayerTextDrawSetShadow(playerid, Loader_TD[playerid][4], 0);
    PlayerTextDrawSetOutline(playerid, Loader_TD[playerid][4], 0);
    PlayerTextDrawBackgroundColor(playerid, Loader_TD[playerid][4], 255);
    PlayerTextDrawFont(playerid, Loader_TD[playerid][4], 4);
    PlayerTextDrawSetProportional(playerid, Loader_TD[playerid][4], 1);

    Loader_TD[playerid][5] = CreatePlayerTextDraw(playerid, 401.000, 363.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Loader_TD[playerid][5], 2.000, -18.000);
    PlayerTextDrawAlignment(playerid, Loader_TD[playerid][5], 1);
    PlayerTextDrawColor(playerid, Loader_TD[playerid][5], -1448498689);
    PlayerTextDrawSetShadow(playerid, Loader_TD[playerid][5], 0);
    PlayerTextDrawSetOutline(playerid, Loader_TD[playerid][5], 0);
    PlayerTextDrawBackgroundColor(playerid, Loader_TD[playerid][5], 255);
    PlayerTextDrawFont(playerid, Loader_TD[playerid][5], 4);
    PlayerTextDrawSetProportional(playerid, Loader_TD[playerid][5], 1);

    for(new i; i < 6; i++) {
        PlayerTextDrawShow(playerid, Loader_TD[playerid][i]);
    } 
    return 1;
}

stock SetLoading(playerid, time, const text[], const function[]="", va_args<>)
{
    if(IsLoadingActive(playerid)) {
        StopLoading(playerid);
    }

    new string[256];
    va_format(string, sizeof(string), text, va_start<4>);
    createLoadingTD(playerid, string);
    format(loadingFunction[playerid], 32, "%s", function);
    loadingAnimation[playerid] = PlayerText_MoveTextSize(playerid, Loader_TD[playerid][0], 174.000, time, EASE_IN_SINE);
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

stock StopLoading(playerid)
{
    if(loadingAnimation[playerid] != -1) {
        Animator_Destroy(loadingAnimation[playerid]);
        loadingAnimation[playerid] = -1;
        loadingFunction[playerid][0] = EOS;
        for(new i; i < 6; i++) {
            PlayerTextDrawDestroy(playerid, Loader_TD[playerid][i]);
        }
        return 1;
    }
    return 0;
}

stock IsLoadingActive(playerid)
{
    return (loadingAnimation[playerid] != -1);
}

public Animator_OnFinish(playerid, animator, eAnimatorTypes:type) 
{
    if(loadingAnimation[playerid] == animator && type == ANIMATOR_TEXT_SIZE)
    {
        for(new i; i < 6; i++) {
            PlayerTextDrawDestroy(playerid, Loader_TD[playerid][i]);
        }
        loadingAnimation[playerid] = -1;
        if(funcidx(loadingFunction[playerid]) == -1) {
            return 1;
        }

        CallLocalFunction(loadingFunction[playerid], "i", playerid); 
        loadingFunction[playerid][0] = EOS;
    }
    return 1;
}

hook OnPlayerConnect(playerid)
{
    loadingAnimation[playerid] = -1;
    loadingFunction[playerid][0] = EOS;
    return 1;
}