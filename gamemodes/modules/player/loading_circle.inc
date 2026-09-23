#define MAX_CIRCLE_TD (52)

new PlayerText:CircleHunger[MAX_PLAYERS][MAX_CIRCLE_TD];
new PlayerText:CircleThrist[MAX_PLAYERS][MAX_CIRCLE_TD];

static stock CtdPos(Float:angle, Float:dist, Float:sx, Float:sy, &Float:x, &Float:y)
{
    x = sx + (dist * floatsin(-angle, degrees));
    y = sy + (dist * floatcos(angle, degrees));
}

stock ShowCircleHunger(playerid, value, Float:pos_x = 260.0, Float:pos_y = 422.0, color = 0xFF8D11FF, bg = 0x22222299, Float:size = 9.0, Float:thick = 0.3)
{
    if (IsPlayerNPC(playerid)) return 0;
    value = value < 0 ? 0 : (value > 100 ? 100 : value);

    new count = 0, percent = (60 * value) / 100, Float:x, Float:y;

    for (new i = 0; i < MAX_CIRCLE_TD; i++)
        PlayerTextDrawDestroy(playerid, CircleHunger[playerid][i]);

    for (new Float:i = 0.0; i < 360.0 && count < MAX_CIRCLE_TD; i += 7.0)
    {
        CtdPos(i + 180.0, size, pos_x, pos_y, x, y);
        CircleHunger[playerid][count] = CreatePlayerTextDraw(playerid, x, y, ".");
        PlayerTextDrawAlignment(playerid, CircleHunger[playerid][count], 2);
        PlayerTextDrawLetterSize(playerid, CircleHunger[playerid][count], thick, thick * 3.0);
        PlayerTextDrawSetShadow(playerid, CircleHunger[playerid][count], 0);
        PlayerTextDrawColor(playerid, CircleHunger[playerid][count], percent <= count ? bg : color);
        PlayerTextDrawShow(playerid, CircleHunger[playerid][count]);
        count++;
    }
    return 1;
}

stock ShowCircleThrist(playerid, value, Float:pos_x = 362.0, Float:pos_y = 422.0, color = 0x16C2F7FF, bg = 0x22222299, Float:size = 9.0, Float:thick = 0.3)
{
    if (IsPlayerNPC(playerid)) return 0;
    value = value < 0 ? 0 : (value > 100 ? 100 : value);

    new count = 0, percent = (60 * value) / 100, Float:x, Float:y;

    for (new i = 0; i < MAX_CIRCLE_TD; i++)
        PlayerTextDrawDestroy(playerid, CircleThrist[playerid][i]);

    for (new Float:i = 0.0; i < 360.0 && count < MAX_CIRCLE_TD; i += 7.0)
    {
        CtdPos(i + 180.0, size, pos_x, pos_y, x, y);
        CircleThrist[playerid][count] = CreatePlayerTextDraw(playerid, x, y, ".");
        PlayerTextDrawAlignment(playerid, CircleThrist[playerid][count], 2);
        PlayerTextDrawLetterSize(playerid, CircleThrist[playerid][count], thick, thick * 3.0);
        PlayerTextDrawSetShadow(playerid, CircleThrist[playerid][count], 0);
        PlayerTextDrawColor(playerid, CircleThrist[playerid][count], percent <= count ? bg : color);
        PlayerTextDrawShow(playerid, CircleThrist[playerid][count]);
        count++;
    }
    return 1;
}

stock HideCircleHungerTextdraws(playerid)
{
    for (new i = 0; i < MAX_CIRCLE_TD; i++)
    {
        PlayerTextDrawHide(playerid, CircleHunger[playerid][i]);
        PlayerTextDrawHide(playerid, CircleThrist[playerid][i]);
    }
    return 1;
}

stock DestroyCircleHungerTextdraws(playerid)
{
    for (new i = 0; i < MAX_CIRCLE_TD; i++)
    {
        PlayerTextDrawDestroy(playerid, CircleHunger[playerid][i]);
        CircleHunger[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
        PlayerTextDrawDestroy(playerid, CircleThrist[playerid][i]);
        CircleThrist[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
    }
    return 1;
}

hook OnPlayerConnect(playerid)
{
    for (new i = 0; i < MAX_CIRCLE_TD; i++)
    {
        CircleHunger[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
        CircleThrist[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
    }
    return 1;
}
