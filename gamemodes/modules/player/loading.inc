#include <YSI\YSI_Coding\y_hooks>

stock createLoadingTD(playerid, const text[])
{
    Loader_TD[playerid][0] = CreatePlayerTextDraw(playerid, 229.000, 277.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Loader_TD[playerid][0], 174.000, 35.000);
    PlayerTextDrawAlignment(playerid, Loader_TD[playerid][0], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColour(playerid, Loader_TD[playerid][0], 421075455);
    PlayerTextDrawSetShadow(playerid, Loader_TD[playerid][0], 0);
    PlayerTextDrawSetOutline(playerid, Loader_TD[playerid][0], 0);
    PlayerTextDrawBackgroundColour(playerid, Loader_TD[playerid][0], 255);
    PlayerTextDrawFont(playerid, Loader_TD[playerid][0], TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawSetProportional(playerid, Loader_TD[playerid][0], true);

    Loader_TD[playerid][1] = CreatePlayerTextDraw(playerid, 229.000, 292.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Loader_TD[playerid][1], 0.000, 20.000);
    PlayerTextDrawAlignment(playerid, Loader_TD[playerid][1], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColour(playerid, Loader_TD[playerid][1], -1523963137);
    PlayerTextDrawSetShadow(playerid, Loader_TD[playerid][1], 0);
    PlayerTextDrawSetOutline(playerid, Loader_TD[playerid][1], 0);
    PlayerTextDrawBackgroundColour(playerid, Loader_TD[playerid][1], 255);
    PlayerTextDrawFont(playerid, Loader_TD[playerid][1], TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawSetProportional(playerid, Loader_TD[playerid][1], true);

    Loader_TD[playerid][2] = CreatePlayerTextDraw(playerid, 319.000, 279.000, text);
    PlayerTextDrawLetterSize(playerid, Loader_TD[playerid][2], 0.270, 1.099);
    PlayerTextDrawAlignment(playerid, Loader_TD[playerid][2], TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawColour(playerid, Loader_TD[playerid][2], -1);
    PlayerTextDrawSetShadow(playerid, Loader_TD[playerid][2], 1);
    PlayerTextDrawSetOutline(playerid, Loader_TD[playerid][2], 1);
    PlayerTextDrawBackgroundColour(playerid, Loader_TD[playerid][2], 150);
    PlayerTextDrawFont(playerid, Loader_TD[playerid][2], TEXT_DRAW_FONT_1);
    PlayerTextDrawSetProportional(playerid, Loader_TD[playerid][2], true);

    Loader_TD[playerid][3] = CreatePlayerTextDraw(playerid, 229.000, 311.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Loader_TD[playerid][3], 174.000, 1.000);
    PlayerTextDrawAlignment(playerid, Loader_TD[playerid][3], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColour(playerid, Loader_TD[playerid][3], -1523963137);
    PlayerTextDrawSetShadow(playerid, Loader_TD[playerid][3], 0);
    PlayerTextDrawSetOutline(playerid, Loader_TD[playerid][3], 0);
    PlayerTextDrawBackgroundColour(playerid, Loader_TD[playerid][3], 255);
    PlayerTextDrawFont(playerid, Loader_TD[playerid][3], TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawSetProportional(playerid, Loader_TD[playerid][3], true);

    Loader_TD[playerid][4] = CreatePlayerTextDraw(playerid, 229.000, 292.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Loader_TD[playerid][4], 174.000, 1.000);
    PlayerTextDrawAlignment(playerid, Loader_TD[playerid][4], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColour(playerid, Loader_TD[playerid][4], -1523963137);
    PlayerTextDrawSetShadow(playerid, Loader_TD[playerid][4], 0);
    PlayerTextDrawSetOutline(playerid, Loader_TD[playerid][4], 0);
    PlayerTextDrawBackgroundColour(playerid, Loader_TD[playerid][4], 255);
    PlayerTextDrawFont(playerid, Loader_TD[playerid][4], TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawSetProportional(playerid, Loader_TD[playerid][4], true);

    for(new i; i < 5; i++) {
        PlayerTextDrawShow(playerid, Loader_TD[playerid][i]);
    } 
    return 1;
}

stock SetLoading(playerid, time, const text[], const function[]="", va_args<>)
{
    new string[256];
    va_format(string, sizeof(string), text, va_start<4>);
    createLoadingTD(playerid, string);
    format(loadingFunction[playerid], 32, "%s", function);
    loadingAnimation[playerid] = PlayerText_MoveTextSize(playerid, Loader_TD[playerid][1], 174.000, time, EASE_IN_SINE);
    return 1;
}

stock StopLoading(playerid)
{
    if(loadingAnimation[playerid] != -1) {
        Animator_Destroy(loadingAnimation[playerid]);
        for(new i; i < 5; i++) {
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
        for(new i; i < 5; i++) {
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