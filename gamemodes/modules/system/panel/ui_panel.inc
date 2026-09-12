#include <YSI\YSI_Coding\y_hooks>

new PlayerText: Panel_TextDraw[MAX_PLAYERS][80];

hook OnPlayerConnect(playerid)
{
    Panel_TextDraw[playerid][0] = CreatePlayerTextDraw(playerid, 0.000, -1.000, "LD_SPAC:black");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][0], 641.000, 451.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][0], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][0], -158);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][0], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][0], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][0], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][0], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][0], 1);

    Panel_TextDraw[playerid][1] = CreatePlayerTextDraw(playerid, 181.000, 277.000, "X");
    PlayerTextDrawLetterSize(playerid, Panel_TextDraw[playerid][1], 0.300, 1.500);
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][1], -2.000, 0.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][1], 2);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][1], -602653441);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][1], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][1], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][1], 150);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][1], 1);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][1], 1);

    Panel_TextDraw[playerid][2] = CreatePlayerTextDraw(playerid, 107.000, 181.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][2], 33.000, 38.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][2], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][2], -1697828097);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][2], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][2], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][2], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][2], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][2], 1);

    Panel_TextDraw[playerid][3] = CreatePlayerTextDraw(playerid, 148.000, 181.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][3], 33.000, 38.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][3], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][3], -1697828097);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][3], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][3], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][3], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][3], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][3], 1);

    Panel_TextDraw[playerid][4] = CreatePlayerTextDraw(playerid, 188.000, 181.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][4], 33.000, 38.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][4], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][4], -1697828097);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][4], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][4], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][4], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][4], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][4], 1);

    Panel_TextDraw[playerid][5] = CreatePlayerTextDraw(playerid, 108.000, 182.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][5], 31.000, 27.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][5], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][5], 2054847231);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][5], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][5], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][5], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][5], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][5], 1);
    PlayerTextDrawSetSelectable(playerid, Panel_TextDraw[playerid][5], 1);

    Panel_TextDraw[playerid][6] = CreatePlayerTextDraw(playerid, 228.000, 181.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][6], 33.000, 38.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][6], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][6], -1697828097);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][6], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][6], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][6], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][6], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][6], 1);

    Panel_TextDraw[playerid][7] = CreatePlayerTextDraw(playerid, 166.000, 229.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][7], 33.000, 38.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][7], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][7], -1697828097);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][7], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][7], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][7], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][7], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][7], 1);

    Panel_TextDraw[playerid][8] = CreatePlayerTextDraw(playerid, 158.000, 277.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][8], 46.000, 14.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][8], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][8], -1697828097);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][8], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][8], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][8], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][8], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][8], 1);

    Panel_TextDraw[playerid][9] = CreatePlayerTextDraw(playerid, 124.000, 229.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][9], 33.000, 38.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][9], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][9], -1697828097);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][9], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][9], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][9], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][9], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][9], 1);

    Panel_TextDraw[playerid][10] = CreatePlayerTextDraw(playerid, 159.000, 278.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][10], 44.000, 12.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][10], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][10], 2054847231);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][10], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][10], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][10], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][10], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][10], 1);
    PlayerTextDrawSetSelectable(playerid, Panel_TextDraw[playerid][10], 1);

    Panel_TextDraw[playerid][11] = CreatePlayerTextDraw(playerid, 207.000, 229.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][11], 33.000, 38.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][11], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][11], -1697828097);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][11], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][11], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][11], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][11], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][11], 1);

    Panel_TextDraw[playerid][12] = CreatePlayerTextDraw(playerid, 208.000, 230.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][12], 31.000, 27.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][12], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][12], 2054847231);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][12], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][12], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][12], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][12], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][12], 1);
    PlayerTextDrawSetSelectable(playerid, Panel_TextDraw[playerid][12], 1);

    Panel_TextDraw[playerid][13] = CreatePlayerTextDraw(playerid, 167.000, 230.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][13], 31.000, 27.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][13], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][13], 2054847231);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][13], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][13], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][13], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][13], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][13], 1);
    PlayerTextDrawSetSelectable(playerid, Panel_TextDraw[playerid][13], 1);

    Panel_TextDraw[playerid][14] = CreatePlayerTextDraw(playerid, 125.000, 230.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][14], 31.000, 27.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][14], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][14], 2054847231);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][14], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][14], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][14], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][14], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][14], 1);
    PlayerTextDrawSetSelectable(playerid, Panel_TextDraw[playerid][14], 1);

    Panel_TextDraw[playerid][15] = CreatePlayerTextDraw(playerid, 117.000, 183.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][15], 2.000, 23.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][15], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][15], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][15], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][15], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][15], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][15], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][15], 1);

    Panel_TextDraw[playerid][16] = CreatePlayerTextDraw(playerid, 229.000, 182.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][16], 31.000, 27.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][16], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][16], 2054847231);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][16], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][16], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][16], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][16], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][16], 1);
    PlayerTextDrawSetSelectable(playerid, Panel_TextDraw[playerid][16], 1);

    Panel_TextDraw[playerid][17] = CreatePlayerTextDraw(playerid, 117.000, 183.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][17], 14.000, 2.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][17], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][17], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][17], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][17], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][17], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][17], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][17], 1);

    Panel_TextDraw[playerid][18] = CreatePlayerTextDraw(playerid, 189.000, 182.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][18], 31.000, 27.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][18], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][18], 2054847231);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][18], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][18], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][18], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][18], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][18], 1);
    PlayerTextDrawSetSelectable(playerid, Panel_TextDraw[playerid][18], 1);

    Panel_TextDraw[playerid][19] = CreatePlayerTextDraw(playerid, 117.000, 205.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][19], 14.000, 2.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][19], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][19], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][19], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][19], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][19], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][19], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][19], 1);

    Panel_TextDraw[playerid][20] = CreatePlayerTextDraw(playerid, 149.000, 182.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][20], 31.000, 27.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][20], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][20], 2054847231);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][20], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][20], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][20], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][20], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][20], 1);
    PlayerTextDrawSetSelectable(playerid, Panel_TextDraw[playerid][20], 1);

    Panel_TextDraw[playerid][21] = CreatePlayerTextDraw(playerid, 210.000, 202.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][21], 3.000, 6.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][21], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][21], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][21], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][21], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][21], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][21], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][21], 1);

    Panel_TextDraw[playerid][22] = CreatePlayerTextDraw(playerid, 129.000, 183.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][22], 2.000, 23.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][22], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][22], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][22], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][22], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][22], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][22], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][22], 1);

    Panel_TextDraw[playerid][23] = CreatePlayerTextDraw(playerid, 120.000, 187.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][23], 8.000, 1.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][23], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][23], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][23], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][23], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][23], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][23], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][23], 1);

    Panel_TextDraw[playerid][24] = CreatePlayerTextDraw(playerid, 157.000, 193.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][24], 14.000, 11.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][24], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][24], 1320741631);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][24], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][24], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][24], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][24], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][24], 1);

    Panel_TextDraw[playerid][25] = CreatePlayerTextDraw(playerid, 158.000, 190.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][25], 12.000, 5.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][25], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][25], 1320741631);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][25], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][25], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][25], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][25], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][25], 1);

    Panel_TextDraw[playerid][26] = CreatePlayerTextDraw(playerid, 156.000, 193.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][26], 16.000, 9.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][26], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][26], 1320741631);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][26], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][26], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][26], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][26], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][26], 1);

    Panel_TextDraw[playerid][27] = CreatePlayerTextDraw(playerid, 155.000, 181.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][27], 18.000, 21.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][27], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][27], 1320741631);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][27], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][27], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][27], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][27], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][27], 1);

    Panel_TextDraw[playerid][28] = CreatePlayerTextDraw(playerid, 160.000, 193.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][28], 8.000, 9.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][28], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][28], 2128609279);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][28], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][28], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][28], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][28], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][28], 1);

    Panel_TextDraw[playerid][29] = CreatePlayerTextDraw(playerid, 160.000, 190.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][29], 8.000, 2.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][29], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][29], 2128609279);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][29], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][29], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][29], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][29], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][29], 1);

    Panel_TextDraw[playerid][30] = CreatePlayerTextDraw(playerid, 158.000, 186.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][30], 12.000, 7.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][30], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][30], 2128609279);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][30], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][30], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][30], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][30], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][30], 1);

    Panel_TextDraw[playerid][31] = CreatePlayerTextDraw(playerid, 161.000, 195.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][31], 6.000, 1.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][31], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][31], 1320741631);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][31], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][31], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][31], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][31], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][31], 1);

    Panel_TextDraw[playerid][32] = CreatePlayerTextDraw(playerid, 162.000, 195.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][32], -1.000, 3.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][32], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][32], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][32], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][32], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][32], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][32], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][32], 1);

    Panel_TextDraw[playerid][33] = CreatePlayerTextDraw(playerid, 160.000, 203.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][33], -1.000, 3.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][33], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][33], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][33], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][33], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][33], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][33], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][33], 1);

    Panel_TextDraw[playerid][34] = CreatePlayerTextDraw(playerid, 170.000, 203.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][34], -1.000, 3.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][34], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][34], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][34], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][34], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][34], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][34], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][34], 1);

    Panel_TextDraw[playerid][35] = CreatePlayerTextDraw(playerid, 171.000, 196.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][35], -1.000, 3.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][35], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][35], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][35], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][35], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][35], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][35], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][35], 1);

    Panel_TextDraw[playerid][36] = CreatePlayerTextDraw(playerid, 158.000, 196.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][36], -1.000, 3.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][36], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][36], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][36], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][36], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][36], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][36], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][36], 1);

    Panel_TextDraw[playerid][37] = CreatePlayerTextDraw(playerid, 195.000, 195.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][37], 20.000, 9.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][37], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][37], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][37], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][37], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][37], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][37], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][37], 1);

    Panel_TextDraw[playerid][38] = CreatePlayerTextDraw(playerid, 197.000, 186.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][38], 15.000, 2.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][38], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][38], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][38], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][38], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][38], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][38], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][38], 1);

    Panel_TextDraw[playerid][39] = CreatePlayerTextDraw(playerid, 197.000, 202.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][39], 3.000, 6.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][39], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][39], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][39], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][39], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][39], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][39], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][39], 1);

    Panel_TextDraw[playerid][40] = CreatePlayerTextDraw(playerid, 194.000, 183.000, "/");
    PlayerTextDrawLetterSize(playerid, Panel_TextDraw[playerid][40], 0.300, 1.500);
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][40], -2.000, 0.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][40], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][40], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][40], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][40], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][40], 150);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][40], 1);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][40], 1);

    Panel_TextDraw[playerid][41] = CreatePlayerTextDraw(playerid, 211.000, 183.000, "\\");
    PlayerTextDrawLetterSize(playerid, Panel_TextDraw[playerid][41], 0.300, 1.500);
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][41], -2.000, 0.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][41], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][41], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][41], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][41], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][41], 150);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][41], 1);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][41], 1);

    Panel_TextDraw[playerid][42] = CreatePlayerTextDraw(playerid, 197.000, 197.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][42], 4.000, 5.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][42], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][42], 163221759);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][42], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][42], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][42], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][42], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][42], 1);

    Panel_TextDraw[playerid][43] = CreatePlayerTextDraw(playerid, 208.000, 197.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][43], 4.000, 5.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][43], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][43], 163221759);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][43], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][43], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][43], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][43], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][43], 1);

    Panel_TextDraw[playerid][44] = CreatePlayerTextDraw(playerid, 214.000, 194.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][44], 4.000, 5.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][44], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][44], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][44], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][44], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][44], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][44], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][44], 1);

    Panel_TextDraw[playerid][45] = CreatePlayerTextDraw(playerid, 192.000, 194.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][45], 4.000, 5.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][45], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][45], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][45], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][45], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][45], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][45], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][45], 1);

    Panel_TextDraw[playerid][46] = CreatePlayerTextDraw(playerid, 233.000, 181.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][46], 23.000, 27.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][46], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][46], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][46], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][46], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][46], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][46], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][46], 1);

    Panel_TextDraw[playerid][47] = CreatePlayerTextDraw(playerid, 129.000, 246.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][47], 23.000, 10.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][47], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][47], 1687547391);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][47], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][47], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][47], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][47], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][47], 1);

    Panel_TextDraw[playerid][48] = CreatePlayerTextDraw(playerid, 243.000, 193.000, "3");
    PlayerTextDrawLetterSize(playerid, Panel_TextDraw[playerid][48], 0.189, 0.898);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][48], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][48], 1018393087);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][48], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][48], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][48], 150);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][48], 1);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][48], 1);

    Panel_TextDraw[playerid][49] = CreatePlayerTextDraw(playerid, 134.000, 229.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][49], 13.000, 14.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][49], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][49], -16776961);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][49], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][49], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][49], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][49], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][49], 1);

    Panel_TextDraw[playerid][50] = CreatePlayerTextDraw(playerid, 136.000, 235.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][50], 9.000, 13.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][50], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][50], -16776961);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][50], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][50], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][50], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][50], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][50], 1);

    Panel_TextDraw[playerid][51] = CreatePlayerTextDraw(playerid, 136.000, 232.000, "V");
    PlayerTextDrawLetterSize(playerid, Panel_TextDraw[playerid][51], 0.379, 2.598);
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][51], -2.000, 0.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][51], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][51], -16776961);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][51], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][51], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][51], 150);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][51], 1);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][51], 1);

    Panel_TextDraw[playerid][52] = CreatePlayerTextDraw(playerid, 175.000, 235.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][52], 17.000, 19.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][52], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][52], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][52], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][52], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][52], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][52], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][52], 1);

    Panel_TextDraw[playerid][53] = CreatePlayerTextDraw(playerid, 185.000, 233.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][53], 7.000, 2.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][53], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][53], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][53], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][53], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][53], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][53], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][53], 1);

    Panel_TextDraw[playerid][54] = CreatePlayerTextDraw(playerid, 184.000, 236.000, "$");
    PlayerTextDrawLetterSize(playerid, Panel_TextDraw[playerid][54], 0.300, 1.500);
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][54], -2.000, 0.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][54], 2);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][54], 163221759);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][54], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][54], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][54], 150);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][54], 1);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][54], 1);

    Panel_TextDraw[playerid][55] = CreatePlayerTextDraw(playerid, 177.000, 251.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][55], 13.000, 2.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][55], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][55], 163221759);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][55], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][55], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][55], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][55], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][55], 1);

    Panel_TextDraw[playerid][56] = CreatePlayerTextDraw(playerid, 186.000, 234.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][56], 5.000, 2.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][56], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][56], 163221759);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][56], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][56], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][56], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][56], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][56], 1);

    Panel_TextDraw[playerid][57] = CreatePlayerTextDraw(playerid, 213.000, 234.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][57], 11.000, 18.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][57], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][57], 489901055);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][57], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][57], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][57], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][57], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][57], 1);

    Panel_TextDraw[playerid][58] = CreatePlayerTextDraw(playerid, 223.000, 236.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][58], 11.000, 18.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][58], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][58], 489901055);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][58], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][58], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][58], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][58], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][58], 1);

    Panel_TextDraw[playerid][59] = CreatePlayerTextDraw(playerid, 214.000, 236.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][59], 9.000, 14.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][59], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][59], -2459137);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][59], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][59], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][59], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][59], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][59], 1);

    Panel_TextDraw[playerid][60] = CreatePlayerTextDraw(playerid, 224.000, 237.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][60], 9.000, 15.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][60], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][60], 16777215);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][60], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][60], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][60], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][60], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][60], 1);

    Panel_TextDraw[playerid][61] = CreatePlayerTextDraw(playerid, 216.000, 239.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][61], 6.000, 2.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][61], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][61], 163221759);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][61], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][61], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][61], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][61], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][61], 1);

    Panel_TextDraw[playerid][62] = CreatePlayerTextDraw(playerid, 216.000, 242.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][62], 6.000, 2.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][62], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][62], 163221759);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][62], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][62], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][62], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][62], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][62], 1);

    Panel_TextDraw[playerid][63] = CreatePlayerTextDraw(playerid, 216.000, 246.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][63], 6.000, 2.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][63], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][63], 163221759);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][63], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][63], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][63], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][63], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][63], 1);

    Panel_TextDraw[playerid][64] = CreatePlayerTextDraw(playerid, 226.000, 240.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][64], 6.000, 1.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][64], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][64], 163221759);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][64], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][64], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][64], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][64], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][64], 1);

    Panel_TextDraw[playerid][65] = CreatePlayerTextDraw(playerid, 226.000, 242.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][65], 6.000, 1.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][65], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][65], 163221759);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][65], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][65], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][65], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][65], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][65], 1);

    Panel_TextDraw[playerid][66] = CreatePlayerTextDraw(playerid, 226.000, 244.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][66], 6.000, 1.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][66], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][66], 163221759);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][66], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][66], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][66], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][66], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][66], 1);

    Panel_TextDraw[playerid][67] = CreatePlayerTextDraw(playerid, 226.000, 248.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][67], 6.000, 1.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][67], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][67], 163221759);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][67], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][67], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][67], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][67], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][67], 1);

    Panel_TextDraw[playerid][68] = CreatePlayerTextDraw(playerid, 141.000, 257.000, "Vi tri");
    PlayerTextDrawLetterSize(playerid, Panel_TextDraw[playerid][68], 0.200, 0.998);
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][68], -2.000, 27.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][68], 2);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][68], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][68], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][68], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][68], 150);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][68], 1);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][68], 1);

    Panel_TextDraw[playerid][69] = CreatePlayerTextDraw(playerid, 183.000, 257.000, "Hoa don");
    PlayerTextDrawLetterSize(playerid, Panel_TextDraw[playerid][69], 0.180, 0.898);
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][69], -2.000, 33.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][69], 2);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][69], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][69], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][69], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][69], 150);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][69], 1);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][69], 1);

    Panel_TextDraw[playerid][70] = CreatePlayerTextDraw(playerid, 224.000, 257.000, "Viec lam");
    PlayerTextDrawLetterSize(playerid, Panel_TextDraw[playerid][70], 0.189, 0.898);
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][70], -2.000, 37.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][70], 2);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][70], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][70], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][70], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][70], 150);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][70], 1);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][70], 1);

    Panel_TextDraw[playerid][71] = CreatePlayerTextDraw(playerid, 245.000, 209.000, "Hanh dong");
    PlayerTextDrawLetterSize(playerid, Panel_TextDraw[playerid][71], 0.180, 0.898);
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][71], 5.000, 39.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][71], 2);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][71], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][71], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][71], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][71], 150);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][71], 1);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][71], 1);

    Panel_TextDraw[playerid][72] = CreatePlayerTextDraw(playerid, 204.600, 209.000, "Phuong tien");
    PlayerTextDrawLetterSize(playerid, Panel_TextDraw[playerid][72], 0.158, 0.898);
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][72], 0.000, 48.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][72], 2);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][72], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][72], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][72], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][72], 150);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][72], 1);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][72], 1);

    Panel_TextDraw[playerid][73] = CreatePlayerTextDraw(playerid, 164.000, 209.000, "Tui do");
    PlayerTextDrawLetterSize(playerid, Panel_TextDraw[playerid][73], 0.200, 0.998);
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][73], 78.000, 30.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][73], 2);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][73], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][73], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][73], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][73], 150);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][73], 1);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][73], 1);

    Panel_TextDraw[playerid][74] = CreatePlayerTextDraw(playerid, 124.000, 209.000, "Dien thoai");
    PlayerTextDrawLetterSize(playerid, Panel_TextDraw[playerid][74], 0.170, 0.898);
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][74], 60.000, 39.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][74], 2);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][74], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][74], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][74], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][74], 150);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][74], 1);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][74], 1);

    Panel_TextDraw[playerid][75] = CreatePlayerTextDraw(playerid, 130.000, 186.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][75], 2.000, 4.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][75], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][75], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][75], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][75], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][75], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][75], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][75], 1);

    Panel_TextDraw[playerid][76] = CreatePlayerTextDraw(playerid, 130.000, 192.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][76], 2.000, 4.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][76], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][76], -1);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][76], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][76], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][76], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][76], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][76], 1);

    Panel_TextDraw[playerid][77] = CreatePlayerTextDraw(playerid, 239.000, 190.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][77], 4.000, 4.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][77], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][77], 1018393087);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][77], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][77], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][77], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][77], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][77], 1);

    Panel_TextDraw[playerid][78] = CreatePlayerTextDraw(playerid, 246.000, 190.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][78], 4.000, 4.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][78], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][78], 1018393087);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][78], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][78], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][78], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][78], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][78], 1);

    Panel_TextDraw[playerid][79] = CreatePlayerTextDraw(playerid, 137.000, 233.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Panel_TextDraw[playerid][79], 7.000, 7.000);
    PlayerTextDrawAlignment(playerid, Panel_TextDraw[playerid][79], 1);
    PlayerTextDrawColor(playerid, Panel_TextDraw[playerid][79], -2139062017);
    PlayerTextDrawSetShadow(playerid, Panel_TextDraw[playerid][79], 0);
    PlayerTextDrawSetOutline(playerid, Panel_TextDraw[playerid][79], 0);
    PlayerTextDrawBackgroundColor(playerid, Panel_TextDraw[playerid][79], 255);
    PlayerTextDrawFont(playerid, Panel_TextDraw[playerid][79], 4);
    PlayerTextDrawSetProportional(playerid, Panel_TextDraw[playerid][79], 1);
    return 1;
}