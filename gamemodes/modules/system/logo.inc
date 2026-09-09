#include <YSI\YSI_Coding\y_hooks>

new Text:LogoTD[5];

stock LoadTDLogo()
{
    LogoTD[0] = TextDrawCreate(565.000, 390.000, "GTA");
    TextDrawLetterSize(LogoTD[0], 0.439, 3.199);
    TextDrawAlignment(LogoTD[0], 1);
    TextDrawColor(LogoTD[0], -251662081);
    TextDrawSetShadow(LogoTD[0], 1);
    TextDrawSetOutline(LogoTD[0], 1);
    TextDrawBackgroundColor(LogoTD[0], -2139094785);
    TextDrawFont(LogoTD[0], 2);
    TextDrawSetProportional(LogoTD[0], 1);

    LogoTD[1] = TextDrawCreate(553.000, 414.000, "HTB");
    TextDrawLetterSize(LogoTD[1], 0.409, 2.000);
    TextDrawAlignment(LogoTD[1], 1);
    TextDrawColor(LogoTD[1], -65281);
    TextDrawSetShadow(LogoTD[1], 1);
    TextDrawSetOutline(LogoTD[1], 1);
    TextDrawBackgroundColor(LogoTD[1], -2139094785);
    TextDrawFont(LogoTD[1], 1);
    TextDrawSetProportional(LogoTD[1], 1);

    LogoTD[2] = TextDrawCreate(581.000, 414.000, "Roleplay");
    TextDrawLetterSize(LogoTD[2], 0.409, 2.000);
    TextDrawAlignment(LogoTD[2], 1);
    TextDrawColor(LogoTD[2], 852308735);
    TextDrawSetShadow(LogoTD[2], 1);
    TextDrawSetOutline(LogoTD[2], 1);
    TextDrawBackgroundColor(LogoTD[2], 8388863);
    TextDrawFont(LogoTD[2], 1);
    TextDrawSetProportional(LogoTD[2], 1);

    LogoTD[3] = TextDrawCreate(576.000, 434.000, "GTAHTB.ONLINE");
    TextDrawLetterSize(LogoTD[3], 0.220, 0.999);
    TextDrawTextSize(LogoTD[3], -1.000, 0.000);
    TextDrawAlignment(LogoTD[3], 1);
    TextDrawColor(LogoTD[3], -1);
    TextDrawSetShadow(LogoTD[3], 0);
    TextDrawSetOutline(LogoTD[3], 0);
    TextDrawBackgroundColor(LogoTD[3], 150);
    TextDrawFont(LogoTD[3], 1);
    TextDrawSetProportional(LogoTD[3], 1);

    LogoTD[4] = TextDrawCreate(4.000, 429.000, "22:10:60 - 01/11/2025");
    TextDrawLetterSize(LogoTD[4], 0.240, 1.500);
    TextDrawAlignment(LogoTD[4], 1);
    TextDrawColor(LogoTD[4], -251662081);
    TextDrawSetShadow(LogoTD[4], 1);
    TextDrawSetOutline(LogoTD[4], 1);
    TextDrawBackgroundColor(LogoTD[4], 1768516095);
    TextDrawFont(LogoTD[4], 2);
    TextDrawSetProportional(LogoTD[4], 1);

    SetTimer("UpdateRealTime", 1000, true);
	return 1;
}

hook OnPlayerSpawn(playerid)
{
    if(gPlayerLogged{playerid} == 1)
    {
		if (IsPlayerSpawned(playerid))
		{
            TextDrawShowForPlayer(playerid, LogoTD[0]);
            TextDrawShowForPlayer(playerid, LogoTD[1]);
            TextDrawShowForPlayer(playerid, LogoTD[2]);
            TextDrawShowForPlayer(playerid, LogoTD[3]);
            TextDrawShowForPlayer(playerid, LogoTD[4]);
        }
    }
	return 1;
}

forward UpdateRealTime();
public UpdateRealTime()
{
    new hour, minute, second, day, month, year;
    new timeString[40];

    gettime(hour, minute, second);
    getdate(year, month, day);

    format(timeString, sizeof(timeString), "%02d:%02d:%02d - %02d/%02d/%d", hour, minute, second, day, month, year);

    TextDrawSetString(LogoTD[4], timeString);
    return 1;
}
