#include <YSI\YSI_Coding\y_hooks>

new Text:LogoTD[3];
new PlayerText: UiPlayerInfoTD[MAX_PLAYERS][46];
new bool:UiPlayerInfoCreated[MAX_PLAYERS];

stock LoadTDLogo()
{
    LogoTD[0] = TextDrawCreate(526.000, 411.000, "KhuPho5");
    TextDrawLetterSize(LogoTD[0], 0.369, 2.299);
    TextDrawAlignment(LogoTD[0], 1);
    TextDrawColour(LogoTD[0], -12254977);
    TextDrawSetShadow(LogoTD[0], 1);
    TextDrawSetOutline(LogoTD[0], 1);
    TextDrawBackgroundColour(LogoTD[0], 150);
    TextDrawFont(LogoTD[0], 1);
    TextDrawSetProportional(LogoTD[0], 1);

    LogoTD[1] = TextDrawCreate(583.000, 410.000, "Roleplay");
    TextDrawLetterSize(LogoTD[1], 0.369, 2.299);
    TextDrawAlignment(LogoTD[1], 1);
    TextDrawColour(LogoTD[1], -2686721);
    TextDrawSetShadow(LogoTD[1], 1);
    TextDrawSetOutline(LogoTD[1], 1);
    TextDrawBackgroundColour(LogoTD[1], 8388863);
    TextDrawFont(LogoTD[1], 1);
    TextDrawSetProportional(LogoTD[1], 1);

    LogoTD[2] = TextDrawCreate(546.000, 430.000, "KHUPHO5.ONLINE");
    TextDrawLetterSize(LogoTD[2], 0.250, 1.399);
    TextDrawAlignment(LogoTD[2], 1);
    TextDrawColour(LogoTD[2], -1);
    TextDrawSetShadow(LogoTD[2], 1);
    TextDrawSetOutline(LogoTD[2], 1);
    TextDrawBackgroundColour(LogoTD[2], 255);
    TextDrawFont(LogoTD[2], 1);
    TextDrawSetProportional(LogoTD[2], 1);

	return 1;
}

stock CreateUiPlayerInfo(playerid)
{
    // [0]-[6]: model previews
    UiPlayerInfoTD[playerid][0] = CreatePlayerTextDraw(playerid, 551.000, 136.000, "_");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][0], 84.000, 25.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][0], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][0], -158);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][0], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][0], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][0], 120);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][0], 5);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][0], 0);
    PlayerTextDrawSetPreviewModel(playerid, UiPlayerInfoTD[playerid][0], 0);
    PlayerTextDrawSetPreviewRot(playerid, UiPlayerInfoTD[playerid][0], 0.000, 0.000, 0.000, -64.300);
    PlayerTextDrawSetPreviewVehCol(playerid, UiPlayerInfoTD[playerid][0], 0, 0);

    UiPlayerInfoTD[playerid][1] = CreatePlayerTextDraw(playerid, 461.000, 136.000, "_");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][1], 84.000, 25.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][1], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][1], -158);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][1], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][1], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][1], 120);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][1], 5);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][1], 0);
    PlayerTextDrawSetPreviewModel(playerid, UiPlayerInfoTD[playerid][1], 0);
    PlayerTextDrawSetPreviewRot(playerid, UiPlayerInfoTD[playerid][1], 0.000, 0.000, 0.000, -64.300);
    PlayerTextDrawSetPreviewVehCol(playerid, UiPlayerInfoTD[playerid][1], 0, 0);

    UiPlayerInfoTD[playerid][2] = CreatePlayerTextDraw(playerid, 568.000, 104.000, "_");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][2], 67.000, 25.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][2], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][2], -158);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][2], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][2], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][2], 120);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][2], 5);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][2], 0);
    PlayerTextDrawSetPreviewModel(playerid, UiPlayerInfoTD[playerid][2], 0);
    PlayerTextDrawSetPreviewRot(playerid, UiPlayerInfoTD[playerid][2], 0.000, 0.000, 0.000, -64.300);
    PlayerTextDrawSetPreviewVehCol(playerid, UiPlayerInfoTD[playerid][2], 0, 0);

    UiPlayerInfoTD[playerid][3] = CreatePlayerTextDraw(playerid, 495.000, 104.000, "_");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][3], 67.000, 25.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][3], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][3], -158);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][3], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][3], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][3], 120);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][3], 5);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][3], 0);
    PlayerTextDrawSetPreviewModel(playerid, UiPlayerInfoTD[playerid][3], 0);
    PlayerTextDrawSetPreviewRot(playerid, UiPlayerInfoTD[playerid][3], 0.000, 0.000, 0.000, -64.300);
    PlayerTextDrawSetPreviewVehCol(playerid, UiPlayerInfoTD[playerid][3], 0, 0);

    UiPlayerInfoTD[playerid][4] = CreatePlayerTextDraw(playerid, 568.000, 168.000, "_");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][4], 67.000, 25.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][4], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][4], -158);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][4], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][4], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][4], 120);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][4], 5);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][4], 0);
    PlayerTextDrawSetPreviewModel(playerid, UiPlayerInfoTD[playerid][4], 0);
    PlayerTextDrawSetPreviewRot(playerid, UiPlayerInfoTD[playerid][4], 0.000, 0.000, 0.000, -64.300);
    PlayerTextDrawSetPreviewVehCol(playerid, UiPlayerInfoTD[playerid][4], 0, 0);

    UiPlayerInfoTD[playerid][5] = CreatePlayerTextDraw(playerid, 495.000, 168.000, "_");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][5], 67.000, 25.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][5], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][5], -158);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][5], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][5], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][5], 120);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][5], 5);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][5], 0);
    PlayerTextDrawSetPreviewModel(playerid, UiPlayerInfoTD[playerid][5], 0);
    PlayerTextDrawSetPreviewRot(playerid, UiPlayerInfoTD[playerid][5], 0.000, 0.000, 0.000, -64.300);
    PlayerTextDrawSetPreviewVehCol(playerid, UiPlayerInfoTD[playerid][5], 0, 0);

    UiPlayerInfoTD[playerid][6] = CreatePlayerTextDraw(playerid, 568.000, 200.000, "_");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][6], 67.000, 25.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][6], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][6], -158);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][6], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][6], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][6], 100);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][6], 5);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][6], 0);
    PlayerTextDrawSetPreviewModel(playerid, UiPlayerInfoTD[playerid][6], 0);
    PlayerTextDrawSetPreviewRot(playerid, UiPlayerInfoTD[playerid][6], 0.000, 0.000, 0.000, -64.300);
    PlayerTextDrawSetPreviewVehCol(playerid, UiPlayerInfoTD[playerid][6], 0, 0);

    // [7]-[11]: decorative bars
    UiPlayerInfoTD[playerid][7] = CreatePlayerTextDraw(playerid, 575.000, 207.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][7], 1.000, 12.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][7], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][7], -1732209921);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][7], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][7], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][7], 255);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][7], 4);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][7], 1);

    UiPlayerInfoTD[playerid][8] = CreatePlayerTextDraw(playerid, 575.000, 207.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][8], 10.000, 4.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][8], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][8], -1732209921);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][8], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][8], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][8], 255);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][8], 4);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][8], 1);

    UiPlayerInfoTD[playerid][9] = CreatePlayerTextDraw(playerid, 575.000, 217.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][9], 10.000, 2.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][9], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][9], -1732209921);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][9], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][9], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][9], 255);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][9], 4);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][9], 1);

    UiPlayerInfoTD[playerid][10] = CreatePlayerTextDraw(playerid, 584.000, 207.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][10], 1.000, 12.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][10], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][10], -1732209921);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][10], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][10], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][10], 255);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][10], 4);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][10], 1);

    UiPlayerInfoTD[playerid][11] = CreatePlayerTextDraw(playerid, 578.000, 213.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][11], 4.000, 1.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][11], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][11], -1732209921);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][11], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][11], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][11], 255);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][11], 4);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][11], 1);

    // [12]-[15]: LD_BEAT:chit icons
    UiPlayerInfoTD[playerid][12] = CreatePlayerTextDraw(playerid, 501.000, 111.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][12], 5.000, 6.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][12], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][12], -1732209921);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][12], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][12], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][12], 255);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][12], 4);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][12], 1);

    UiPlayerInfoTD[playerid][13] = CreatePlayerTextDraw(playerid, 499.000, 116.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][13], 9.000, 6.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][13], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][13], -1732209921);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][13], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][13], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][13], 255);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][13], 4);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][13], 1);

    UiPlayerInfoTD[playerid][14] = CreatePlayerTextDraw(playerid, 509.000, 113.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][14], 5.000, 6.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][14], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][14], -1732209921);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][14], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][14], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][14], 255);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][14], 4);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][14], 1);

    UiPlayerInfoTD[playerid][15] = CreatePlayerTextDraw(playerid, 507.000, 118.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][15], 9.000, 6.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][15], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][15], -1732209921);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][15], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][15], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][15], 255);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][15], 4);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][15], 1);

    // [16]: "Online/ID:" label  [17]: online count (dynamic)
    UiPlayerInfoTD[playerid][16] = CreatePlayerTextDraw(playerid, 520.000, 108.000, "Online/ID:");
    PlayerTextDrawLetterSize(playerid, UiPlayerInfoTD[playerid][16], 0.136, 0.898);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][16], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][16], -1);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][16], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][16], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][16], 150);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][16], 1);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][16], 1);

    UiPlayerInfoTD[playerid][17] = CreatePlayerTextDraw(playerid, 520.000, 118.000, "100 - #29");
    PlayerTextDrawLetterSize(playerid, UiPlayerInfoTD[playerid][17], 0.148, 0.796);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][17], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][17], -1);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][17], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][17], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][17], 150);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][17], 1);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][17], 1);

    // [18]: "Thoi gian:" label  [19]: time (dynamic)
    UiPlayerInfoTD[playerid][18] = CreatePlayerTextDraw(playerid, 593.000, 108.000, "Thoi gian:");
    PlayerTextDrawLetterSize(playerid, UiPlayerInfoTD[playerid][18], 0.136, 0.898);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][18], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][18], -1);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][18], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][18], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][18], 150);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][18], 1);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][18], 1);

    UiPlayerInfoTD[playerid][19] = CreatePlayerTextDraw(playerid, 593.000, 118.000, "12:30");
    PlayerTextDrawLetterSize(playerid, UiPlayerInfoTD[playerid][19], 0.148, 0.796);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][19], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][19], -1);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][19], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][19], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][19], 150);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][19], 1);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][19], 1);

    // [20]-[24]: clock decoration
    UiPlayerInfoTD[playerid][20] = CreatePlayerTextDraw(playerid, 572.000, 108.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][20], 17.000, 18.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][20], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][20], -1732209921);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][20], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][20], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][20], 255);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][20], 4);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][20], 1);

    UiPlayerInfoTD[playerid][21] = CreatePlayerTextDraw(playerid, 573.000, 109.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][21], 15.000, 16.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][21], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][21], 255);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][21], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][21], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][21], 255);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][21], 4);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][21], 1);

    UiPlayerInfoTD[playerid][22] = CreatePlayerTextDraw(playerid, 575.000, 111.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][22], 11.000, 12.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][22], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][22], -1732209921);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][22], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][22], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][22], 255);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][22], 4);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][22], 1);

    UiPlayerInfoTD[playerid][23] = CreatePlayerTextDraw(playerid, 579.000, 112.000, "/");
    PlayerTextDrawLetterSize(playerid, UiPlayerInfoTD[playerid][23], 0.200, 0.497);
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][23], -6.000, -7.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][23], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][23], 255);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][23], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][23], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][23], 150);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][23], 1);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][23], 1);

    UiPlayerInfoTD[playerid][24] = CreatePlayerTextDraw(playerid, 583.000, 115.000, "/");
    PlayerTextDrawLetterSize(playerid, UiPlayerInfoTD[playerid][24], -0.289, 0.597);
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][24], -6.000, -7.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][24], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][24], 255);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][24], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][24], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][24], 150);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][24], 1);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][24], 1);

    // [25]-[28]: job section bars
    UiPlayerInfoTD[playerid][25] = CreatePlayerTextDraw(playerid, 468.000, 148.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][25], 11.000, 6.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][25], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][25], -1732209921);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][25], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][25], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][25], 255);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][25], 4);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][25], 1);

    UiPlayerInfoTD[playerid][26] = CreatePlayerTextDraw(playerid, 470.000, 148.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][26], 1.000, -4.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][26], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][26], -1732209921);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][26], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][26], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][26], 255);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][26], 4);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][26], 1);

    UiPlayerInfoTD[playerid][27] = CreatePlayerTextDraw(playerid, 476.000, 148.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][27], 1.000, -4.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][27], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][27], -1732209921);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][27], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][27], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][27], 255);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][27], 4);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][27], 1);

    UiPlayerInfoTD[playerid][28] = CreatePlayerTextDraw(playerid, 477.000, 145.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][28], -7.000, -1.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][28], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][28], -1732209921);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][28], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][28], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][28], 255);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][28], 4);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][28], 1);

    // [29]: "Cong viec:" label  [30]: job name (dynamic)
    UiPlayerInfoTD[playerid][29] = CreatePlayerTextDraw(playerid, 486.000, 140.000, "Cong viec:");
    PlayerTextDrawLetterSize(playerid, UiPlayerInfoTD[playerid][29], 0.136, 0.898);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][29], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][29], -1);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][29], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][29], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][29], 150);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][29], 1);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][29], 1);

    UiPlayerInfoTD[playerid][30] = CreatePlayerTextDraw(playerid, 486.000, 150.000, "Penambang");
    PlayerTextDrawLetterSize(playerid, UiPlayerInfoTD[playerid][30], 0.148, 0.796);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][30], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][30], -1);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][30], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][30], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][30], 150);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][30], 1);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][30], 1);

    // [31]-[32]: job icons
    UiPlayerInfoTD[playerid][31] = CreatePlayerTextDraw(playerid, 560.000, 143.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][31], 7.000, 6.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][31], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][31], -1732209921);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][31], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][31], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][31], 255);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][31], 4);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][31], 1);

    UiPlayerInfoTD[playerid][32] = CreatePlayerTextDraw(playerid, 557.000, 148.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][32], 13.000, 8.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][32], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][32], -1732209921);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][32], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][32], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][32], 255);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][32], 4);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][32], 1);

    // [33]: "Ten:" label  [34]: player name (dynamic)
    UiPlayerInfoTD[playerid][33] = CreatePlayerTextDraw(playerid, 576.000, 140.000, "Ten:");
    PlayerTextDrawLetterSize(playerid, UiPlayerInfoTD[playerid][33], 0.136, 0.898);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][33], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][33], -1);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][33], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][33], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][33], 150);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][33], 1);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][33], 1);

    UiPlayerInfoTD[playerid][34] = CreatePlayerTextDraw(playerid, 576.000, 150.000, "Alonsoo_Cortevaz");
    PlayerTextDrawLetterSize(playerid, UiPlayerInfoTD[playerid][34], 0.148, 0.796);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][34], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][34], -1);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][34], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][34], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][34], 150);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][34], 1);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][34], 1);

    // [35]-[37]: cash section
    UiPlayerInfoTD[playerid][35] = CreatePlayerTextDraw(playerid, 502.000, 177.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][35], 11.000, 9.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][35], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][35], -1732209921);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][35], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][35], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][35], 255);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][35], 4);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][35], 1);

    UiPlayerInfoTD[playerid][36] = CreatePlayerTextDraw(playerid, 505.000, 178.000, "$");
    PlayerTextDrawLetterSize(playerid, UiPlayerInfoTD[playerid][36], 0.216, 0.796);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][36], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][36], 255);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][36], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][36], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][36], 150);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][36], 1);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][36], 1);

    UiPlayerInfoTD[playerid][37] = CreatePlayerTextDraw(playerid, 520.000, 172.000, "Tien mat:");
    PlayerTextDrawLetterSize(playerid, UiPlayerInfoTD[playerid][37], 0.136, 0.898);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][37], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][37], -1);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][37], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][37], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][37], 150);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][37], 1);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][37], 1);

    // [38]: cash amount (dynamic)
    UiPlayerInfoTD[playerid][38] = CreatePlayerTextDraw(playerid, 520.000, 182.000, "$23400");
    PlayerTextDrawLetterSize(playerid, UiPlayerInfoTD[playerid][38], 0.148, 0.796);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][38], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][38], -1);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][38], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][38], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][38], 150);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][38], 1);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][38], 1);

    // [39]-[42]: bank section
    UiPlayerInfoTD[playerid][39] = CreatePlayerTextDraw(playerid, 574.000, 176.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][39], 13.000, 13.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][39], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][39], -1732209921);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][39], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][39], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][39], 255);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][39], 4);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][39], 1);

    UiPlayerInfoTD[playerid][40] = CreatePlayerTextDraw(playerid, 576.000, 174.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, UiPlayerInfoTD[playerid][40], 9.000, 11.000);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][40], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][40], -1732209921);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][40], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][40], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][40], 255);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][40], 4);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][40], 1);

    UiPlayerInfoTD[playerid][41] = CreatePlayerTextDraw(playerid, 579.000, 178.000, "$");
    PlayerTextDrawLetterSize(playerid, UiPlayerInfoTD[playerid][41], 0.170, 0.796);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][41], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][41], 255);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][41], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][41], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][41], 150);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][41], 1);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][41], 1);

    UiPlayerInfoTD[playerid][42] = CreatePlayerTextDraw(playerid, 593.000, 172.000, "Ngan hang:");
    PlayerTextDrawLetterSize(playerid, UiPlayerInfoTD[playerid][42], 0.136, 0.898);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][42], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][42], -1);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][42], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][42], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][42], 150);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][42], 1);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][42], 1);

    // [43]: bank amount (dynamic)
    UiPlayerInfoTD[playerid][43] = CreatePlayerTextDraw(playerid, 593.000, 182.000, "$23400");
    PlayerTextDrawLetterSize(playerid, UiPlayerInfoTD[playerid][43], 0.148, 0.796);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][43], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][43], -1);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][43], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][43], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][43], 150);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][43], 1);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][43], 1);

    // [44]: "Ngay:" label  [45]: date (dynamic)
    UiPlayerInfoTD[playerid][44] = CreatePlayerTextDraw(playerid, 593.000, 204.000, "Ngay:");
    PlayerTextDrawLetterSize(playerid, UiPlayerInfoTD[playerid][44], 0.136, 0.898);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][44], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][44], -1);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][44], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][44], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][44], 150);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][44], 1);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][44], 1);

    UiPlayerInfoTD[playerid][45] = CreatePlayerTextDraw(playerid, 593.000, 214.000, "7/11/2025");
    PlayerTextDrawLetterSize(playerid, UiPlayerInfoTD[playerid][45], 0.148, 0.796);
    PlayerTextDrawAlignment(playerid, UiPlayerInfoTD[playerid][45], 1);
    PlayerTextDrawColour(playerid, UiPlayerInfoTD[playerid][45], -1);
    PlayerTextDrawSetShadow(playerid, UiPlayerInfoTD[playerid][45], 0);
    PlayerTextDrawSetOutline(playerid, UiPlayerInfoTD[playerid][45], 0);
    PlayerTextDrawBackgroundColor(playerid, UiPlayerInfoTD[playerid][45], 150);
    PlayerTextDrawFont(playerid, UiPlayerInfoTD[playerid][45], 1);
    PlayerTextDrawSetProportional(playerid, UiPlayerInfoTD[playerid][45], 1);

    UiPlayerInfoCreated[playerid] = true;
    return 1;
}

stock DestroyUiPlayerInfo(playerid)
{
    if(!UiPlayerInfoCreated[playerid]) return 0;
    for(new i = 0; i < 46; i++) {
        if(UiPlayerInfoTD[playerid][i] != PlayerText:INVALID_TEXT_DRAW) {
            PlayerTextDrawDestroy(playerid, UiPlayerInfoTD[playerid][i]);
            UiPlayerInfoTD[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
        }
    }
    UiPlayerInfoCreated[playerid] = false;
    return 1;
}

stock GetAllPlayer()
{
    new count = 0;
    foreach(new i : Player)
    {
        count++;
    }
    return count;
}

stock ShowUiPlayerInfo(playerid)
{
    if(!UiPlayerInfoCreated[playerid]) {
        CreateUiPlayerInfo(playerid);
        PlayerInfo[playerid][pHUDInfo] = 1;
        UpdateHudUI(playerid);
    }

    for(new i = 0; i < 46; i++) {
        if(UiPlayerInfoTD[playerid][i] != PlayerText:INVALID_TEXT_DRAW)
            PlayerTextDrawShow(playerid, UiPlayerInfoTD[playerid][i]);
    }
    PlayerInfo[playerid][pHUDInfo] = 1;
    return 1;
}

stock HideUiPlayerInfo(playerid)
{
    if(UiPlayerInfoCreated[playerid]) {
        for(new i = 0; i < 46; i++) {
            if(UiPlayerInfoTD[playerid][i] != PlayerText:INVALID_TEXT_DRAW)
                PlayerTextDrawHide(playerid, UiPlayerInfoTD[playerid][i]);
        }
        DestroyUiPlayerInfo(playerid);
    }
    PlayerInfo[playerid][pHUDInfo] = 0;
    return 1;
}

stock UpdateHudUI(playerid)
{
    if(!PlayerInfo[playerid][pHUDInfo]) return 0;
    if(!UiPlayerInfoCreated[playerid]) return 0;

    new str[255];
    format(str, sizeof(str), "%d - #%d", GetAllPlayer(), playerid);
    PlayerTextDrawSetString(playerid, UiPlayerInfoTD[playerid][17], str);

    new hour, minute, second;
    gettime(hour, minute, second);
    format(str, sizeof(str), "%02d:%02d", hour, minute);
    PlayerTextDrawSetString(playerid, UiPlayerInfoTD[playerid][19], str);

    GetPlayerName(playerid, str, sizeof(str));
    PlayerTextDrawSetString(playerid, UiPlayerInfoTD[playerid][34], str);

    format(str, sizeof(str), "$%d", GetPlayerMoney(playerid));
    PlayerTextDrawSetString(playerid, UiPlayerInfoTD[playerid][38], str);

    format(str, sizeof(str), "$%d", PlayerInfo[playerid][pAccount]);
    PlayerTextDrawSetString(playerid, UiPlayerInfoTD[playerid][43], str);

    format(str, sizeof(str), "%s", GetJobName(PlayerInfo[playerid][pJob]));
    PlayerTextDrawSetString(playerid, UiPlayerInfoTD[playerid][30], str);

    new year, month, day;
    getdate(year, month, day);
    format(str, sizeof(str), "%d/%d/%d", day, month, year);
    PlayerTextDrawSetString(playerid, UiPlayerInfoTD[playerid][45], str);
    return 1;
}

hook OnPlayerSpawn(playerid)
{
    if(gPlayerLogged{playerid} == 1 && IsPlayerSpawned(playerid))
    {
        TextDrawShowForPlayer(playerid, LogoTD[0]);
        TextDrawShowForPlayer(playerid, LogoTD[1]);
        TextDrawShowForPlayer(playerid, LogoTD[2]);

        if(PlayerInfo[playerid][pHUDInfo])
            ShowUiPlayerInfo(playerid);
        else
            HideUiPlayerInfo(playerid);
    }
    return 1;
}

hook OnPlayerConnect(playerid)
{
    UiPlayerInfoCreated[playerid] = false;
    for(new i = 0; i < 46; i++)
        UiPlayerInfoTD[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
    return 1;
}

hook OnGameModeInit()
{
    LoadTDLogo();
    SetTimer("UpdateUiPlayerInfo", 1000, true);
    return 1;
}

forward UpdateUiPlayerInfo();
public UpdateUiPlayerInfo()
{
    for(new i = 0; i < MAX_PLAYERS; i++)
    {
        if(IsPlayerConnected(i)
            && PlayerInfo[i][pHUDInfo] == 1
            && UiPlayerInfoCreated[i])
            UpdateHudUI(i);
    }
    return 1;
}
