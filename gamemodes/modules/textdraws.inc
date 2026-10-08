// Poker Table Textdraw - Credits to GhoulSlayer
CreatePokerGUI(playerid)
{
    PlayerPokerUI[playerid][0] = CreatePlayerTextDraw(playerid, 390.000000, 263.000000, " "); // Seat 2 (SEAT 1)
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][0], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][0], 100);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][0], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][0], 0.159998, 1.200001);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][0], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][0], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][0], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][0], 0);

	PlayerPokerUI[playerid][1] = CreatePlayerTextDraw(playerid, 389.000000, 273.000000, " ");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][1], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][1], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][1], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][1], 0.159998, 1.200001);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][1], 16711935);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][1], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][1], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][1], 0);

	PlayerPokerUI[playerid][2] = CreatePlayerTextDraw(playerid, 369.000000, 286.000000, " ");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][2], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][2], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][2], 4);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][2], 0.500000, 1.000000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][2], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][2], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][2], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][2], 1);
	PlayerTextDrawUseBox(playerid, PlayerPokerUI[playerid][2], true);
	PlayerTextDrawBoxColour(playerid, PlayerPokerUI[playerid][2], 255);
	PlayerTextDrawTextSize(playerid, PlayerPokerUI[playerid][2], 20.000000, 33.000000);

	PlayerPokerUI[playerid][3] = CreatePlayerTextDraw(playerid, 392.000000, 286.000000, " ");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][3], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][3], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][3], 4);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][3], 0.500000, 1.000000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][3], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][3], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][3], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][3], 1);
	PlayerTextDrawUseBox(playerid, PlayerPokerUI[playerid][3], true);
	PlayerTextDrawBoxColour(playerid, PlayerPokerUI[playerid][3], 255);
	PlayerTextDrawTextSize(playerid, PlayerPokerUI[playerid][3], 20.000000, 33.000000);

	PlayerPokerUI[playerid][4] = CreatePlayerTextDraw(playerid, 391.000000, 319.000000, " ");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][4], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][4], 100);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][4], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][4], 0.180000, 1.000000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][4], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][4], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][4], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][4], 0);

	PlayerPokerUI[playerid][5] = CreatePlayerTextDraw(playerid, 250.000000, 263.000000, " "); // Seat 1 (SEAT 2)
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][5], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][5], 100);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][5], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][5], 0.159999, 1.200001);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][5], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][5], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][5], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][5], 0);

	PlayerPokerUI[playerid][6] = CreatePlayerTextDraw(playerid, 250.000000, 273.000000, " ");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][6], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][6], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][6], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][6], 0.159999, 1.200001);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][6], 16711935);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][6], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][6], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][6], 0);

	PlayerPokerUI[playerid][7] = CreatePlayerTextDraw(playerid, 229.000000, 286.000000, " ");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][7], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][7], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][7], 4);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][7], 0.500000, 1.000000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][7], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][7], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][7], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][7], 1);
	PlayerTextDrawUseBox(playerid, PlayerPokerUI[playerid][7], true);
	PlayerTextDrawBoxColour(playerid, PlayerPokerUI[playerid][7], 255);
	PlayerTextDrawTextSize(playerid, PlayerPokerUI[playerid][7], 20.000000, 33.000000);

	PlayerPokerUI[playerid][8] = CreatePlayerTextDraw(playerid, 252.000000, 286.000000, " ");
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][8], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][8], 4);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][8], 0.500000, 1.000000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][8], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][8], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][8], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][8], 1);
	PlayerTextDrawUseBox(playerid, PlayerPokerUI[playerid][8], true);
	PlayerTextDrawBoxColour(playerid, PlayerPokerUI[playerid][8], 255);
	PlayerTextDrawTextSize(playerid, PlayerPokerUI[playerid][8], 20.000000, 33.000000);

	PlayerPokerUI[playerid][9] = CreatePlayerTextDraw(playerid, 250.000000, 319.000000, " ");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][9], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][9], 100);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][9], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][9], 0.180000, 1.000000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][9], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][9], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][9], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][9], 0);

	PlayerPokerUI[playerid][10] = CreatePlayerTextDraw(playerid, 199.000000, 190.000000, " "); // Seat 6 (SEAT 3)
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][10], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][10], 100);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][10], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][10], 0.159998, 1.200001);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][10], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][10], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][10], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][10], 0);

	PlayerPokerUI[playerid][11] = CreatePlayerTextDraw(playerid, 199.000000, 199.000000, " ");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][11], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][11], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][11], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][11], 0.159998, 1.200001);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][11], 16711935);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][11], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][11], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][11], 0);

	PlayerPokerUI[playerid][12] = CreatePlayerTextDraw(playerid, 179.000000, 212.000000, " ");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][12], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][12], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][12], 4);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][12], 0.500000, 1.000000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][12], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][12], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][12], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][12], 1);
	PlayerTextDrawUseBox(playerid, PlayerPokerUI[playerid][12], true);
	PlayerTextDrawBoxColour(playerid, PlayerPokerUI[playerid][12], 255);
	PlayerTextDrawTextSize(playerid, PlayerPokerUI[playerid][12], 20.000000, 33.000000);

	PlayerPokerUI[playerid][13] = CreatePlayerTextDraw(playerid, 202.000000, 212.000000, " ");
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][13], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][13], 4);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][13], 0.500000, 1.000000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][13], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][13], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][13], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][13], 1);
	PlayerTextDrawUseBox(playerid, PlayerPokerUI[playerid][13], true);
	PlayerTextDrawBoxColour(playerid, PlayerPokerUI[playerid][13], 255);
	PlayerTextDrawTextSize(playerid, PlayerPokerUI[playerid][13], 20.000000, 33.000000);

	PlayerPokerUI[playerid][14] = CreatePlayerTextDraw(playerid, 200.000000, 245.000000, " ");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][14], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][14], 100);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][14], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][14], 0.180000, 1.000000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][14], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][14], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][14], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][14], 0);

	PlayerPokerUI[playerid][15] = CreatePlayerTextDraw(playerid, 250.000000, 116.000000, " ");  // Seat 5 (SEAT 4)
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][15], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][15], 100);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][15], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][15], 0.159998, 1.200001);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][15], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][15], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][15], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][15], 0);

	PlayerPokerUI[playerid][16] = CreatePlayerTextDraw(playerid, 250.000000, 126.000000, " ");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][16], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][16], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][16], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][16], 0.159998, 1.200001);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][16], 16711935);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][16], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][16], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][16], 0);

	PlayerPokerUI[playerid][17] = CreatePlayerTextDraw(playerid, 229.000000, 139.000000, " ");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][17], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][17], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][17], 4);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][17], 0.500000, 1.000000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][17], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][17], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][17], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][17], 1);
	PlayerTextDrawUseBox(playerid, PlayerPokerUI[playerid][17], true);
	PlayerTextDrawBoxColour(playerid, PlayerPokerUI[playerid][17], 255);
	PlayerTextDrawTextSize(playerid, PlayerPokerUI[playerid][17], 20.000000, 33.000000);

	PlayerPokerUI[playerid][18] = CreatePlayerTextDraw(playerid, 252.000000, 139.000000, " ");
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][18], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][18], 4);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][18], 0.500000, 1.000000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][18], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][18], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][18], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][18], 1);
	PlayerTextDrawUseBox(playerid, PlayerPokerUI[playerid][18], true);
	PlayerTextDrawBoxColour(playerid, PlayerPokerUI[playerid][18], 255);
	PlayerTextDrawTextSize(playerid, PlayerPokerUI[playerid][18], 20.000000, 33.000000);

	PlayerPokerUI[playerid][19] = CreatePlayerTextDraw(playerid, 250.000000, 172.000000, " ");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][19], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][19], 100);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][19], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][19], 0.180000, 1.000000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][19], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][19], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][19], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][19], 0);

	PlayerPokerUI[playerid][20] = CreatePlayerTextDraw(playerid, 390.000000, 116.000000, " "); // Seat 4 (SEAT 5)
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][20], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][20], 100);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][20], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][20], 0.159997, 1.200001);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][20], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][20], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][20], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][20], 0);

	PlayerPokerUI[playerid][21] = CreatePlayerTextDraw(playerid, 389.000000, 126.000000, " ");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][21], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][21], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][21], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][21], 0.159997, 1.200001);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][21], 16711935);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][21], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][21], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][21], 0);

	PlayerPokerUI[playerid][22] = CreatePlayerTextDraw(playerid, 369.000000, 139.000000, " ");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][22], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][22], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][22], 4);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][22], 0.500000, 1.000000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][22], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][22], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][22], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][22], 1);
	PlayerTextDrawUseBox(playerid, PlayerPokerUI[playerid][22], true);
	PlayerTextDrawBoxColour(playerid, PlayerPokerUI[playerid][22], 255);
	PlayerTextDrawTextSize(playerid, PlayerPokerUI[playerid][22], 20.000000, 33.000000);

	PlayerPokerUI[playerid][23] = CreatePlayerTextDraw(playerid, 392.000000, 139.000000, " ");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][23], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][23], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][23], 4);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][23], 0.500000, 1.000000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][23], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][23], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][23], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][23], 1);
	PlayerTextDrawUseBox(playerid, PlayerPokerUI[playerid][23], true);
	PlayerTextDrawBoxColour(playerid, PlayerPokerUI[playerid][23], 255);
	PlayerTextDrawTextSize(playerid, PlayerPokerUI[playerid][23], 20.000000, 33.000000);

	PlayerPokerUI[playerid][24] = CreatePlayerTextDraw(playerid, 391.000000, 172.000000, " ");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][24], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][24], 100);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][24], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][24], 0.180000, 1.000000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][24], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][24], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][24], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][24], 0);

	PlayerPokerUI[playerid][25] = CreatePlayerTextDraw(playerid, 443.000000, 190.000000, " "); // Seat 3 (SEAT 6)
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][25], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][25], 100);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][25], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][25], 0.159998, 1.200001);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][25], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][25], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][25], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][25], 0);

	PlayerPokerUI[playerid][26] = CreatePlayerTextDraw(playerid, 442.000000, 199.000000, " ");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][26], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][26], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][26], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][26], 0.159998, 1.200001);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][26], 16711935);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][26], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][26], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][26], 0);

	PlayerPokerUI[playerid][27] = CreatePlayerTextDraw(playerid, 422.000000, 212.000000, " ");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][27], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][27], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][27], 4);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][27], 0.500000, 1.000000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][27], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][27], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][27], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][27], 1);
	PlayerTextDrawUseBox(playerid, PlayerPokerUI[playerid][27], true);
	PlayerTextDrawBoxColour(playerid, PlayerPokerUI[playerid][27], 255);
	PlayerTextDrawTextSize(playerid, PlayerPokerUI[playerid][27], 20.000000, 33.000000);

	PlayerPokerUI[playerid][28] = CreatePlayerTextDraw(playerid, 445.000000, 212.000000, " ");
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][28], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][28], 4);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][28], 0.500000, 1.000000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][28], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][28], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][28], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][28], 1);
	PlayerTextDrawUseBox(playerid, PlayerPokerUI[playerid][28], true);
	PlayerTextDrawBoxColour(playerid, PlayerPokerUI[playerid][28], 255);
	PlayerTextDrawTextSize(playerid, PlayerPokerUI[playerid][28], 20.000000, 33.000000);

	PlayerPokerUI[playerid][29] = CreatePlayerTextDraw(playerid, 444.000000, 245.000000, " ");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][29], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][29], 100);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][29], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][29], 0.180000, 1.000000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][29], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][29], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][29], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][29], 0);

	PlayerPokerUI[playerid][30] = CreatePlayerTextDraw(playerid, 265.000000, 205.000000, "New Textdraw"); // Community Card Box
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][30], 0);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][30], 1);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][30], 0.539999, 2.099998);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][30], 0);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][30], 1);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][30], true);
	PlayerTextDrawUseBox(playerid, PlayerPokerUI[playerid][30], true);
	PlayerTextDrawBoxColour(playerid, PlayerPokerUI[playerid][30], 100);
	PlayerTextDrawTextSize(playerid, PlayerPokerUI[playerid][30], 375.000000, 71.000000);

	PlayerPokerUI[playerid][31] = CreatePlayerTextDraw(playerid, 266.000000, 208.000000, "LD_CARD:cdback");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][31], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][31], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][31], 4);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][31], 0.500000, 1.000000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][31], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][31], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][31], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][31], 1);
	PlayerTextDrawUseBox(playerid, PlayerPokerUI[playerid][31], true);
	PlayerTextDrawBoxColour(playerid, PlayerPokerUI[playerid][31], 255);
	PlayerTextDrawTextSize(playerid, PlayerPokerUI[playerid][31], 20.000000, 33.000000);

	PlayerPokerUI[playerid][32] = CreatePlayerTextDraw(playerid, 288.000000, 208.000000, "LD_CARD:cdback");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][32], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][32], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][32], 4);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][32], 0.500000, 1.000000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][32], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][32], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][32], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][32], 1);
	PlayerTextDrawUseBox(playerid, PlayerPokerUI[playerid][32], true);
	PlayerTextDrawBoxColour(playerid, PlayerPokerUI[playerid][32], 255);
	PlayerTextDrawTextSize(playerid, PlayerPokerUI[playerid][32], 20.000000, 33.000000);

	PlayerPokerUI[playerid][33] = CreatePlayerTextDraw(playerid, 310.000000, 208.000000, "LD_CARD:cdback");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][33], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][33], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][33], 4);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][33], 0.500000, 1.000000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][33], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][33], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][33], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][33], 1);
	PlayerTextDrawUseBox(playerid, PlayerPokerUI[playerid][33], true);
	PlayerTextDrawBoxColour(playerid, PlayerPokerUI[playerid][33], 255);
	PlayerTextDrawTextSize(playerid, PlayerPokerUI[playerid][33], 20.000000, 33.000000);

	PlayerPokerUI[playerid][34] = CreatePlayerTextDraw(playerid, 332.000000, 208.000000, "LD_CARD:cdback");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][34], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][34], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][34], 4);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][34], 0.500000, 1.000000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][34], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][34], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][34], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][34], 1);
	PlayerTextDrawUseBox(playerid, PlayerPokerUI[playerid][34], true);
	PlayerTextDrawBoxColour(playerid, PlayerPokerUI[playerid][34], 255);
	PlayerTextDrawTextSize(playerid, PlayerPokerUI[playerid][34], 20.000000, 33.000000);

	PlayerPokerUI[playerid][35] = CreatePlayerTextDraw(playerid, 354.000000, 208.000000, "LD_CARD:cdback");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][35], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][35], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][35], 4);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][35], 0.500000, 1.000000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][35], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][35], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][35], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][35], 1);
	PlayerTextDrawUseBox(playerid, PlayerPokerUI[playerid][35], true);
	PlayerTextDrawBoxColour(playerid, PlayerPokerUI[playerid][35], 255);
	PlayerTextDrawTextSize(playerid, PlayerPokerUI[playerid][35], 20.000000, 33.000000);

	PlayerPokerUI[playerid][36] = CreatePlayerTextDraw(playerid, 320.000000, 193.000000, "New Textdraw");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][36], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][36], 0);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][36], 1);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][36], 0.500000, 0.399999);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][36], 0);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][36], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][36], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][36], 1);
	PlayerTextDrawUseBox(playerid, PlayerPokerUI[playerid][36], true);
	PlayerTextDrawBoxColour(playerid, PlayerPokerUI[playerid][36], 50);
	PlayerTextDrawTextSize(playerid, PlayerPokerUI[playerid][36], 390.000000, 110.000000);

	PlayerPokerUI[playerid][37] = CreatePlayerTextDraw(playerid, 318.000000, 191.000000, "Texas Holdem Poker");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][37], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][37], -1);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][37], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][37], 0.199999, 1.200000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][37], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][37], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][37], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][37], 0);

	PlayerPokerUI[playerid][38] = CreatePlayerTextDraw(playerid, 321.000000, 268.000000, " ");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][38], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][38], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][38], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][38], 0.189999, 1.200000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][38], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][38], 1);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][38], true);
	PlayerTextDrawUseBox(playerid, PlayerPokerUI[playerid][38], true);
	PlayerTextDrawBoxColour(playerid, PlayerPokerUI[playerid][38], 45);
	PlayerTextDrawTextSize(playerid, PlayerPokerUI[playerid][38], 10.000000, 26.000000);
	PlayerTextDrawSetSelectable(playerid, PlayerPokerUI[playerid][38], true);

	PlayerPokerUI[playerid][39] = CreatePlayerTextDraw(playerid, 321.000000, 284.000000, " ");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][39], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][39], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][39], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][39], 0.189999, 1.200000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][39], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][39], 1);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][39], true);
	PlayerTextDrawUseBox(playerid, PlayerPokerUI[playerid][39], true);
	PlayerTextDrawBoxColour(playerid, PlayerPokerUI[playerid][39], 45);
	PlayerTextDrawTextSize(playerid, PlayerPokerUI[playerid][39], 10.000000, 26.000000);
	PlayerTextDrawSetSelectable(playerid, PlayerPokerUI[playerid][39], true);

	PlayerPokerUI[playerid][40] = CreatePlayerTextDraw(playerid, 321.000000, 300.000000, " ");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][40], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][40], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][40], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][40], 0.189999, 1.200000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][40], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][40], 1);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][40], true);
	PlayerTextDrawUseBox(playerid, PlayerPokerUI[playerid][40], true);
	PlayerTextDrawBoxColour(playerid, PlayerPokerUI[playerid][40], 45);
	PlayerTextDrawTextSize(playerid, PlayerPokerUI[playerid][40], 10.000000, 26.000000);
	PlayerTextDrawSetSelectable(playerid, PlayerPokerUI[playerid][40], true);

	PlayerPokerUI[playerid][41] = CreatePlayerTextDraw(playerid, 318.000000, 120.000000, "THOAT");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][41], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][41], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][41], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][41], 0.189999, 1.200000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][41], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][41], 1);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][41], true);
	PlayerTextDrawUseBox(playerid, PlayerPokerUI[playerid][41], true);
	PlayerTextDrawBoxColour(playerid, PlayerPokerUI[playerid][41], 45);
	PlayerTextDrawTextSize(playerid, PlayerPokerUI[playerid][41], 10.000000, 36.000000);
	PlayerTextDrawSetSelectable(playerid, PlayerPokerUI[playerid][41], true);

	PlayerPokerUI[playerid][42] = CreatePlayerTextDraw(playerid, 590.000000, 400.000000, "Casino~n~Games");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][42], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][42], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][42], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][42], 0.500000, 2.000000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][42], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][42], 1);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][42], true);

	PlayerPokerUI[playerid][43] = CreatePlayerTextDraw(playerid, 589.000000, 396.000000, "gta-sa.vn");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][43], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][43], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][43], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][43], 0.180000, 0.800000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][43], 200);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][43], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][43], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][43], 0);

	PlayerPokerUI[playerid][44] = CreatePlayerTextDraw(playerid, 588.000000, 437.000000, "Phien ban Gay");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][44], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][44], 255);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][44], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][44], 0.180000, 0.800000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][44], 200);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][44], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][44], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][44], 0);

/*	PlayerPokerUI[playerid][45] = CreatePlayerTextDraw(playerid, 5.000000, 100.000000, "Debug:");
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][45], 0);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][45], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][45], 0.159999, 1.099999);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][45], 200);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][45], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][45], 1);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][45], 1); */

	PlayerPokerUI[playerid][46] = CreatePlayerTextDraw(playerid, 318.000000, 245.000000, "Texas Holdem Poker");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][46], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][46], -1);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][46], 2);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][46], 0.199999, 1.200000);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][46], -1);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][46], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][46], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][46], 0);

	PlayerPokerUI[playerid][47] = CreatePlayerTextDraw(playerid, 320.000000, 248.000000, "New Textdraw");
	PlayerTextDrawAlignment(playerid, PlayerPokerUI[playerid][47], 2);
	PlayerTextDrawBackgroundColour(playerid, PlayerPokerUI[playerid][47], 0);
	PlayerTextDrawFont(playerid, PlayerPokerUI[playerid][47], 1);
	PlayerTextDrawLetterSize(playerid, PlayerPokerUI[playerid][47], 0.500000, 0.399998);
	PlayerTextDrawColour(playerid, PlayerPokerUI[playerid][47], 0);
	PlayerTextDrawSetOutline(playerid, PlayerPokerUI[playerid][47], 0);
	PlayerTextDrawSetProportional(playerid, PlayerPokerUI[playerid][47], true);
	PlayerTextDrawSetShadow(playerid, PlayerPokerUI[playerid][47], 1);
	PlayerTextDrawUseBox(playerid, PlayerPokerUI[playerid][47], true);
	PlayerTextDrawBoxColour(playerid, PlayerPokerUI[playerid][47], 50);
	PlayerTextDrawTextSize(playerid, PlayerPokerUI[playerid][47], 390.000000, 110.000000);
}
 
forward LoadTextDraws();
public LoadTextDraws()
{
	ServerTime_TD = TextDrawCreate(635.000, 408.000, "hh:mm:ss - dd/mm/yyyy");
	TextDrawLetterSize(ServerTime_TD, 0.200, 1.199);
	TextDrawAlignment(ServerTime_TD, TEXT_DRAW_ALIGN_RIGHT);
	TextDrawColour(ServerTime_TD, -1);
	TextDrawSetShadow(ServerTime_TD, 1);
	TextDrawSetOutline(ServerTime_TD, 1);
	TextDrawBackgroundColour(ServerTime_TD, 150);
	TextDrawFont(ServerTime_TD, TEXT_DRAW_FONT_1);
	TextDrawSetProportional(ServerTime_TD, true);
	textdrawscount++;

	/* InvTD@Global[0] = TextDrawCreate(203.000, 133.000, "LD_SPAC:white");
	TextDrawTextSize(InvTD@Global[0], 259.000, 15.000);
	TextDrawAlignment(InvTD@Global[0], TEXT_DRAW_ALIGN_LEFT);
	TextDrawColour(InvTD@Global[0], 168430335);
	TextDrawSetShadow(InvTD@Global[0], 0);
	TextDrawSetOutline(InvTD@Global[0], 0);
	TextDrawBackgroundColour(InvTD@Global[0], 255);
	TextDrawFont(InvTD@Global[0], TEXT_DRAW_FONT_SPRITE_DRAW);
	TextDrawSetProportional(InvTD@Global[0], true);
	textdrawscount++;

	InvTD@Global[1] = TextDrawCreate(203.000, 148.000, "LD_SPAC:white");
	TextDrawTextSize(InvTD@Global[1], 259.000, 234.000);
	TextDrawAlignment(InvTD@Global[1], TEXT_DRAW_ALIGN_LEFT);
	TextDrawColour(InvTD@Global[1], 168430315);
	TextDrawSetShadow(InvTD@Global[1], 0);
	TextDrawSetOutline(InvTD@Global[1], 0);
	TextDrawBackgroundColour(InvTD@Global[1], 255);
	TextDrawFont(InvTD@Global[1], TEXT_DRAW_FONT_SPRITE_DRAW);
	TextDrawSetProportional(InvTD@Global[1], true);
	textdrawscount++;

	InvTD@Global[2] = TextDrawCreate(450.000, 132.000, "X");
	TextDrawLetterSize(InvTD@Global[2], 0.349, 1.800);
	TextDrawTextSize(InvTD@Global[2], 460.000, 11.000);
	TextDrawAlignment(InvTD@Global[2], TEXT_DRAW_ALIGN_LEFT);
	TextDrawColour(InvTD@Global[2], -1);
	TextDrawSetShadow(InvTD@Global[2], 1);
	TextDrawSetOutline(InvTD@Global[2], 1);
	TextDrawBackgroundColour(InvTD@Global[2], 150);
	TextDrawFont(InvTD@Global[2], TEXT_DRAW_FONT_2);
	TextDrawSetProportional(InvTD@Global[2], true);
	TextDrawSetSelectable(InvTD@Global[2], true);
	textdrawscount++;

	InvTD@Global[3] = TextDrawCreate(369.000, 339.000, "LD_BEAT:left");
	TextDrawTextSize(InvTD@Global[3], 16.000, 22.000);
	TextDrawAlignment(InvTD@Global[3], TEXT_DRAW_ALIGN_LEFT);
	TextDrawColour(InvTD@Global[3], -1);
	TextDrawSetShadow(InvTD@Global[3], 0);
	TextDrawSetOutline(InvTD@Global[3], 0);
	TextDrawBackgroundColour(InvTD@Global[3], 255);
	TextDrawFont(InvTD@Global[3], TEXT_DRAW_FONT_SPRITE_DRAW);
	TextDrawSetProportional(InvTD@Global[3], true);
	TextDrawSetSelectable(InvTD@Global[3], true);
	textdrawscount++;

	InvTD@Global[4] = TextDrawCreate(434.000, 339.000, "LD_BEAT:right");
	TextDrawTextSize(InvTD@Global[4], 16.000, 22.000);
	TextDrawAlignment(InvTD@Global[4], TEXT_DRAW_ALIGN_LEFT);
	TextDrawColour(InvTD@Global[4], -1);
	TextDrawSetShadow(InvTD@Global[4], 0);
	TextDrawSetOutline(InvTD@Global[4], 0);
	TextDrawBackgroundColour(InvTD@Global[4], 255);
	TextDrawFont(InvTD@Global[4], TEXT_DRAW_FONT_SPRITE_DRAW);
	TextDrawSetProportional(InvTD@Global[4], true);
	TextDrawSetSelectable(InvTD@Global[4], true);
	textdrawscount++; */

    noticeCommand[0] = TextDrawCreate(237.000, 276.000, "LD_SPAC:white");
    TextDrawTextSize(noticeCommand[0], 169.000, 66.000);
    TextDrawAlignment(noticeCommand[0], TEXT_DRAW_ALIGN_LEFT);
    TextDrawColour(noticeCommand[0], 180);
    TextDrawSetShadow(noticeCommand[0], 0);
    TextDrawSetOutline(noticeCommand[0], 0);
    TextDrawBackgroundColour(noticeCommand[0], 255);
    TextDrawFont(noticeCommand[0], TEXT_DRAW_FONT_SPRITE_DRAW);
    TextDrawSetProportional(noticeCommand[0], true);
	textdrawscount++;

    noticeCommand[1] = TextDrawCreate(321.000, 278.000, "(!) LENH KHONG CHINH XAC (!)");
    TextDrawLetterSize(noticeCommand[1], 0.200, 1.399);
    TextDrawAlignment(noticeCommand[1], TEXT_DRAW_ALIGN_CENTER);
    TextDrawColour(noticeCommand[1], -1306385665);
    TextDrawSetShadow(noticeCommand[1], 0);
    TextDrawSetOutline(noticeCommand[1], 1);
    TextDrawBackgroundColour(noticeCommand[1], 150);
    TextDrawFont(noticeCommand[1], TEXT_DRAW_FONT_2);
    TextDrawSetProportional(noticeCommand[1], true);
	textdrawscount++;

    noticeCommand[2] = TextDrawCreate(321.000, 296.000, "CAU LENH BAN NHAP VAO KHONG CHINH XAC~n~SU DUNG \"/HELP\" DE XEM THEM THONG TIN~n~SU DUNG \"/CMDS\" DE XEM CAC CAU LENH");
    TextDrawLetterSize(noticeCommand[2], 0.140, 1.099);
    TextDrawAlignment(noticeCommand[2], TEXT_DRAW_ALIGN_CENTER);
    TextDrawColour(noticeCommand[2], -1);
    TextDrawSetShadow(noticeCommand[2], 0);
    TextDrawSetOutline(noticeCommand[2], 1);
    TextDrawBackgroundColour(noticeCommand[2], 150);
    TextDrawFont(noticeCommand[2], TEXT_DRAW_FONT_2);
    TextDrawSetProportional(noticeCommand[2], true);
	textdrawscount++;

    // Blindfold
	BFText = TextDrawCreate(0.000000, -3.000000, " ");
	TextDrawBackgroundColour(BFText, 255);
	TextDrawFont(BFText, 1);
	TextDrawLetterSize(BFText, 50.000000, 50.000000);
	TextDrawColour(BFText, 0x000000FF);
	TextDrawSetOutline(BFText, 0);
	TextDrawSetProportional(BFText, true);
	TextDrawSetShadow(BFText, 1);
	TextDrawUseBox(BFText, true);
	TextDrawBoxColour(BFText, 0x000000FF);
	TextDrawTextSize(BFText, 689.000000, -53.000000);
	textdrawscount++;

	// WristWatch
	WristWatch = TextDrawCreate(577.000000, 50.000000, " ");
	TextDrawAlignment(WristWatch, 2);
	TextDrawBackgroundColour(WristWatch, 255);
	TextDrawFont(WristWatch, 3);
	TextDrawLetterSize(WristWatch, 0.709998, 2.100001);
	TextDrawColour(WristWatch, -1);
	TextDrawSetOutline(WristWatch, 1);
	TextDrawSetProportional(WristWatch, false);
	textdrawscount++;

	// Notice Box
	NoticeTxtdraw[0] = TextDrawCreate(174.000000, 181.000000, "Box");
	TextDrawBackgroundColour(NoticeTxtdraw[0], 0);
	TextDrawFont(NoticeTxtdraw[0], 1);
	TextDrawLetterSize(NoticeTxtdraw[0], 1.590000, 7.700005);
	TextDrawColour(NoticeTxtdraw[0], 0);
	TextDrawSetOutline(NoticeTxtdraw[0], 0);
	TextDrawSetProportional(NoticeTxtdraw[0], true);
	TextDrawSetShadow(NoticeTxtdraw[0], 1);
	TextDrawUseBox(NoticeTxtdraw[0], true);
	TextDrawBoxColour(NoticeTxtdraw[0], 100);
	TextDrawTextSize(NoticeTxtdraw[0], 454.000000, 18.000000);
	textdrawscount++;

	NoticeTxtdraw[1] = TextDrawCreate(317.000000, 174.000000, "HTB:RP - Notice");
	TextDrawAlignment(NoticeTxtdraw[1], 2);
	TextDrawBackgroundColour(NoticeTxtdraw[1], 255);
	TextDrawFont(NoticeTxtdraw[1], 2);
	TextDrawLetterSize(NoticeTxtdraw[1], 0.189999, 1.200000);
	TextDrawColour(NoticeTxtdraw[1], -1);
	TextDrawSetOutline(NoticeTxtdraw[1], 1);
	TextDrawSetProportional(NoticeTxtdraw[1], true);
	textdrawscount++;

	// Title
	NoticeTxtdraw[2] = TextDrawCreate(323.000000, 196.000000, " ");
	TextDrawAlignment(NoticeTxtdraw[2], 2);
	TextDrawBackgroundColour(NoticeTxtdraw[2], 255);
	TextDrawFont(NoticeTxtdraw[2], 2);
	TextDrawLetterSize(NoticeTxtdraw[2], 0.449999, 2.000000);
	//TextDrawLetterSize(NoticeTxtdraw[2], 0.529999, 2.800000);
	TextDrawColour(NoticeTxtdraw[2], -1);
	TextDrawSetOutline(NoticeTxtdraw[2], 1);
	TextDrawSetProportional(NoticeTxtdraw[2], true);
	textdrawscount++;

	// Description.
	NoticeTxtdraw[3] = TextDrawCreate(322.000000, 221.000000, " ");
	TextDrawAlignment(NoticeTxtdraw[3], 2);
	TextDrawBackgroundColour(NoticeTxtdraw[3], 255);
	TextDrawFont(NoticeTxtdraw[3], 1);
	TextDrawLetterSize(NoticeTxtdraw[3], 0.189999, 1.200000);
	TextDrawColour(NoticeTxtdraw[3], -1);
	TextDrawSetOutline(NoticeTxtdraw[3], 1);
	TextDrawSetProportional(NoticeTxtdraw[3], true);
	textdrawscount++;

	print("[Textdraws] Loaded textdraws.");
	
	return 1;
}

stock PrepTradeToysGUI(playerid, sellerid, price, object)
{
	new string[128];
	InsideTradeToys[playerid] = 1;
	
	// Background
	ttBackground[playerid] = CreatePlayerTextDraw(playerid, 21.000000, 120.000000, "Background"); // Leave the string there, it will fuck up the size of the background if not
	PlayerTextDrawBackgroundColour(playerid, ttBackground[playerid], 255);
	PlayerTextDrawFont(playerid, ttBackground[playerid], 0);
	PlayerTextDrawLetterSize(playerid, ttBackground[playerid], -0.019998, 34.600002);
	PlayerTextDrawColour(playerid, ttBackground[playerid], -256);
	PlayerTextDrawSetOutline(playerid, ttBackground[playerid], 0);
	PlayerTextDrawSetProportional(playerid, ttBackground[playerid], false);
	PlayerTextDrawSetShadow(playerid, ttBackground[playerid], 1);
	PlayerTextDrawUseBox(playerid, ttBackground[playerid], true);
	PlayerTextDrawBoxColour(playerid, ttBackground[playerid], 136);
	PlayerTextDrawTextSize(playerid, ttBackground[playerid], 618.000000, 164.000000);
	PlayerTextDrawSetSelectable(playerid, ttBackground[playerid], false);
	
	// Model
	ttModel[playerid] = CreatePlayerTextDraw(playerid, 382.000000, 201.000000, "Model");
	PlayerTextDrawBackgroundColour(playerid, ttModel[playerid], 255);
	PlayerTextDrawFont(playerid, ttModel[playerid], 5);
	PlayerTextDrawLetterSize(playerid, ttModel[playerid], -0.679998, 15.999996);
	PlayerTextDrawColour(playerid, ttModel[playerid], -1);
	PlayerTextDrawSetOutline(playerid, ttModel[playerid], 0);
	PlayerTextDrawSetProportional(playerid, ttModel[playerid], true);
	PlayerTextDrawSetShadow(playerid, ttModel[playerid], 1);
	PlayerTextDrawUseBox(playerid, ttModel[playerid], true);
	PlayerTextDrawBoxColour(playerid, ttModel[playerid], 136);
	PlayerTextDrawTextSize(playerid, ttModel[playerid], 171.000000, 145.000000);
	PlayerTextDrawSetSelectable(playerid, ttModel[playerid], false);
	PlayerTextDrawSetPreviewModel(playerid, ttModel[playerid], object);
	PlayerTextDrawSetPreviewRot(playerid, ttModel[playerid], 0.000000, 0.000000, -180.000000, 1.000000);
	
	// Object Info
	format(string, sizeof(string), "(Object ID %d)", object);
	ttObjectInfo[playerid] = CreatePlayerTextDraw(playerid, 400.000000, 182.000000, string);
	PlayerTextDrawBackgroundColour(playerid, ttObjectInfo[playerid], 255);
	PlayerTextDrawFont(playerid, ttObjectInfo[playerid], 1);
	PlayerTextDrawLetterSize(playerid, ttObjectInfo[playerid], 0.310000, 1.000000);
	PlayerTextDrawColour(playerid, ttObjectInfo[playerid], -1);
	PlayerTextDrawSetOutline(playerid, ttObjectInfo[playerid], 0);
	PlayerTextDrawSetProportional(playerid, ttObjectInfo[playerid], true);
	PlayerTextDrawSetShadow(playerid, ttObjectInfo[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, ttObjectInfo[playerid], false);
	
	// Logo
	ttLogo[playerid] = CreatePlayerTextDraw(playerid, 264.000000, 121.000000, "Buon ban do choi");
	PlayerTextDrawBackgroundColour(playerid, ttLogo[playerid], 255);
	PlayerTextDrawFont(playerid, ttLogo[playerid], 2);
	PlayerTextDrawLetterSize(playerid, ttLogo[playerid], 0.379999, 1.500000);
	PlayerTextDrawColour(playerid, ttLogo[playerid], 542758143);
	PlayerTextDrawSetOutline(playerid, ttLogo[playerid], 0);
	PlayerTextDrawSetProportional(playerid, ttLogo[playerid], true);
	PlayerTextDrawSetShadow(playerid, ttLogo[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, ttLogo[playerid], false);
	
	// Purchase
	ttPurchase[playerid] = CreatePlayerTextDraw(playerid, 91.000000, 295.000000, "Mua");
	PlayerTextDrawBackgroundColour(playerid, ttPurchase[playerid], 255);
	PlayerTextDrawFont(playerid, ttPurchase[playerid], 1);
	PlayerTextDrawLetterSize(playerid, ttPurchase[playerid], 0.500000, 1.700000);
	PlayerTextDrawColour(playerid, ttPurchase[playerid], 16711935);
	PlayerTextDrawSetOutline(playerid, ttPurchase[playerid], 1);
	PlayerTextDrawSetProportional(playerid, ttPurchase[playerid], true);
	PlayerTextDrawUseBox(playerid, ttPurchase[playerid], true);
	PlayerTextDrawBoxColour(playerid, ttPurchase[playerid], 1625166370);
	PlayerTextDrawTextSize(playerid, ttPurchase[playerid], 164.000000, 4.000000);
	PlayerTextDrawSetSelectable(playerid, ttPurchase[playerid], true);
	
	// Decline
	ttDecline[playerid] = CreatePlayerTextDraw(playerid, 254.000000, 295.000000, "Tu choi");
	PlayerTextDrawBackgroundColour(playerid, ttDecline[playerid], 255);
	PlayerTextDrawFont(playerid, ttDecline[playerid], 1);
	PlayerTextDrawLetterSize(playerid, ttDecline[playerid], 0.500000, 1.799999);
	PlayerTextDrawColour(playerid, ttDecline[playerid], -16776961);
	PlayerTextDrawSetOutline(playerid, ttDecline[playerid], 0);
	PlayerTextDrawSetProportional(playerid, ttDecline[playerid], true);
	PlayerTextDrawSetShadow(playerid, ttDecline[playerid], 1);
	PlayerTextDrawUseBox(playerid, ttDecline[playerid], true);
	PlayerTextDrawBoxColour(playerid, ttDecline[playerid], -234414302);
	PlayerTextDrawTextSize(playerid, ttDecline[playerid], 310.000000, 0.000000);
	PlayerTextDrawSetSelectable(playerid, ttDecline[playerid], false);
	
	// Offer
	format(string, sizeof(string), "%s da de nghi ban do choi nay voi gia $%s", GetPlayerNameEx(sellerid), number_format(price));
	ttOffer[playerid] = CreatePlayerTextDraw(playerid, 71.000000, 174.000000, string);
	PlayerTextDrawBackgroundColour(playerid, ttOffer[playerid], 255);
	PlayerTextDrawFont(playerid, ttOffer[playerid], 1);
	PlayerTextDrawLetterSize(playerid, ttOffer[playerid], 0.310000, 1.000000);
	PlayerTextDrawColour(playerid, ttOffer[playerid], -1);
	PlayerTextDrawSetOutline(playerid, ttOffer[playerid], 0);
	PlayerTextDrawSetProportional(playerid, ttOffer[playerid], true);
	PlayerTextDrawSetShadow(playerid, ttOffer[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, ttOffer[playerid], false);
	
	// Note
	ttNote[playerid] = CreatePlayerTextDraw(playerid, 74.000000, 221.000000, "Bam phim Y tu ban phim de dong y mua. Bam phim N tu ban phim de tu choi mua");
	PlayerTextDrawBackgroundColour(playerid, ttNote[playerid], 255);
	PlayerTextDrawFont(playerid, ttNote[playerid], 1);
	PlayerTextDrawLetterSize(playerid, ttNote[playerid], 0.150000, 1.000000);
	PlayerTextDrawColour(playerid, ttNote[playerid], -1);
	PlayerTextDrawSetOutline(playerid, ttNote[playerid], 0);
	PlayerTextDrawSetProportional(playerid, ttNote[playerid], true);
	PlayerTextDrawSetShadow(playerid, ttNote[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, ttNote[playerid], false);
	
	// Yes 
	ttYes[playerid] = CreatePlayerTextDraw(playerid, 117.000000, 275.000000, "'Y'");
	PlayerTextDrawBackgroundColour(playerid, ttYes[playerid], 255);
	PlayerTextDrawFont(playerid, ttYes[playerid], 1);
	PlayerTextDrawLetterSize(playerid, ttYes[playerid], 0.500000, 1.000000);
	PlayerTextDrawColour(playerid, ttYes[playerid], 16711935);
	PlayerTextDrawSetOutline(playerid, ttYes[playerid], 0);
	PlayerTextDrawSetProportional(playerid, ttYes[playerid], true);
	PlayerTextDrawSetShadow(playerid, ttYes[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, ttYes[playerid], false);

	// No 
	ttNo[playerid] = CreatePlayerTextDraw(playerid, 270.000000, 271.000000, "'N'");
	PlayerTextDrawBackgroundColour(playerid, ttNo[playerid], 255);
	PlayerTextDrawFont(playerid, ttNo[playerid], 1);
	PlayerTextDrawLetterSize(playerid, ttNo[playerid], 0.500000, 1.000000);
	PlayerTextDrawColour(playerid, ttNo[playerid], -16776961);
	PlayerTextDrawSetOutline(playerid, ttNo[playerid], 0);
	PlayerTextDrawSetProportional(playerid, ttNo[playerid], true);
	PlayerTextDrawSetShadow(playerid, ttNo[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, ttNo[playerid], false);
	
	PlayerTextDrawShow(playerid, ttBackground[playerid]);
	PlayerTextDrawShow(playerid, ttModel[playerid]);
	PlayerTextDrawShow(playerid, ttObjectInfo[playerid]);
	PlayerTextDrawShow(playerid, ttLogo[playerid]);
	PlayerTextDrawShow(playerid, ttPurchase[playerid]);
	PlayerTextDrawShow(playerid, ttDecline[playerid]);
	PlayerTextDrawShow(playerid, ttOffer[playerid]);
	PlayerTextDrawShow(playerid, ttNote[playerid]);
	PlayerTextDrawShow(playerid, ttYes[playerid]);
	PlayerTextDrawShow(playerid, ttNo[playerid]);
	return 1;
}

stock HideTradeToysGUI(playerid)
{
	InsideTradeToys[playerid] = 0;
	PlayerTextDrawHide(playerid, ttBackground[playerid]);
	PlayerTextDrawHide(playerid, ttModel[playerid]);
	PlayerTextDrawHide(playerid, ttObjectInfo[playerid]);
	PlayerTextDrawHide(playerid, ttLogo[playerid]);
	PlayerTextDrawHide(playerid, ttPurchase[playerid]);
	PlayerTextDrawHide(playerid, ttDecline[playerid]);
	PlayerTextDrawHide(playerid, ttOffer[playerid]);
	PlayerTextDrawHide(playerid, ttNote[playerid]);
	PlayerTextDrawHide(playerid, ttYes[playerid]);
	PlayerTextDrawHide(playerid, ttNo[playerid]);
	return 1;
}

stock CreateHungerGamesTextdraw(playerid)
{
	new string[128];
	
	// Create the Player Count Textdraw
	format(string, sizeof(string), "Players in event: %d", hgPlayerCount);
	HungerPlayerInfo[playerid][hgPlayerText] = CreatePlayerTextDraw(playerid, 41.000000, 433.000000, string);
	PlayerTextDrawBackgroundColour(playerid, HungerPlayerInfo[playerid][hgPlayerText], 255);
	PlayerTextDrawFont(playerid, HungerPlayerInfo[playerid][hgPlayerText], 1);
	PlayerTextDrawLetterSize(playerid, HungerPlayerInfo[playerid][hgPlayerText], 0.250000, 0.899999);
	PlayerTextDrawColour(playerid, HungerPlayerInfo[playerid][hgPlayerText], -1);
	PlayerTextDrawSetOutline(playerid, HungerPlayerInfo[playerid][hgPlayerText], 1);
	PlayerTextDrawSetProportional(playerid, HungerPlayerInfo[playerid][hgPlayerText], true);
	PlayerTextDrawSetSelectable(playerid, HungerPlayerInfo[playerid][hgPlayerText], false);
	
	// Create the loading textdraw
	HungerPlayerInfo[playerid][hgLoadingText] = CreatePlayerTextDraw(playerid, 549.000000, 423.000000, "Dang tai The Hunger Games...");
	PlayerTextDrawBackgroundColour(playerid, HungerPlayerInfo[playerid][hgLoadingText], 255);
	PlayerTextDrawFont(playerid, HungerPlayerInfo[playerid][hgLoadingText], 1);
	PlayerTextDrawLetterSize(playerid, HungerPlayerInfo[playerid][hgLoadingText], 0.160000, 0.699999);
	PlayerTextDrawColour(playerid, HungerPlayerInfo[playerid][hgLoadingText], -16776961);
	PlayerTextDrawSetOutline(playerid, HungerPlayerInfo[playerid][hgLoadingText], 1);
	PlayerTextDrawSetProportional(playerid, HungerPlayerInfo[playerid][hgLoadingText], true);
	PlayerTextDrawSetSelectable(playerid, HungerPlayerInfo[playerid][hgLoadingText], false);

	// Create the time left textdraw
	format(string, sizeof(string), "Time left until start: %d", hgCountdown);
	HungerPlayerInfo[playerid][hgTimeLeftText] = CreatePlayerTextDraw(playerid, 549.000000, 432.000000, string);
	PlayerTextDrawBackgroundColour(playerid, HungerPlayerInfo[playerid][hgTimeLeftText], 255);
	PlayerTextDrawFont(playerid, HungerPlayerInfo[playerid][hgTimeLeftText], 1);
	PlayerTextDrawLetterSize(playerid, HungerPlayerInfo[playerid][hgTimeLeftText], 0.210000, 1.000000);
	PlayerTextDrawColour(playerid, HungerPlayerInfo[playerid][hgTimeLeftText], -1);
	PlayerTextDrawSetOutline(playerid, HungerPlayerInfo[playerid][hgTimeLeftText], 1);
	PlayerTextDrawSetProportional(playerid, HungerPlayerInfo[playerid][hgTimeLeftText], true);
	PlayerTextDrawSetSelectable(playerid, HungerPlayerInfo[playerid][hgTimeLeftText], false);

	// Create the credits textdraw
	HungerPlayerInfo[playerid][hgCreditsText] = CreatePlayerTextDraw(playerid, 532.000000, 2.000000, "The Hunger Games, by Kids");
	PlayerTextDrawBackgroundColour(playerid, HungerPlayerInfo[playerid][hgCreditsText], 255);
	PlayerTextDrawFont(playerid, HungerPlayerInfo[playerid][hgCreditsText], 1);
	PlayerTextDrawLetterSize(playerid, HungerPlayerInfo[playerid][hgCreditsText], 0.190000, 0.699999);
	PlayerTextDrawColour(playerid, HungerPlayerInfo[playerid][hgCreditsText], -16776961);
	PlayerTextDrawSetOutline(playerid, HungerPlayerInfo[playerid][hgCreditsText], 1);
	PlayerTextDrawSetProportional(playerid, HungerPlayerInfo[playerid][hgCreditsText], true);
	PlayerTextDrawSetSelectable(playerid, HungerPlayerInfo[playerid][hgCreditsText], false);
	
	// Show the textdraws
	PlayerTextDrawShow(playerid, HungerPlayerInfo[playerid][hgPlayerText]);
	PlayerTextDrawShow(playerid, HungerPlayerInfo[playerid][hgLoadingText]);
	PlayerTextDrawShow(playerid, HungerPlayerInfo[playerid][hgTimeLeftText]);
	PlayerTextDrawShow(playerid, HungerPlayerInfo[playerid][hgCreditsText]);
	return true;
}

stock HideHungerGamesTextdraw(playerid)
{
	// Hide the textdraws
	PlayerTextDrawHide(playerid, HungerPlayerInfo[playerid][hgPlayerText]);
	PlayerTextDrawHide(playerid, HungerPlayerInfo[playerid][hgLoadingText]);
	PlayerTextDrawHide(playerid, HungerPlayerInfo[playerid][hgTimeLeftText]);
	PlayerTextDrawHide(playerid, HungerPlayerInfo[playerid][hgCreditsText]);
	return true;
}

stock ShowFPSCounter(playerid)
{
	new string[64];
	format(string, sizeof(string), "%d", pFPS[playerid]);
	pFPSCounter[playerid] = CreatePlayerTextDraw(playerid,617.000000, 2.000000, string);
	PlayerTextDrawBackgroundColour(playerid,pFPSCounter[playerid], 255);
	PlayerTextDrawFont(playerid,pFPSCounter[playerid], 3);
	PlayerTextDrawLetterSize(playerid,pFPSCounter[playerid], 0.370000, 1.200000);
	PlayerTextDrawColour(playerid,pFPSCounter[playerid], -65281);
	PlayerTextDrawSetOutline(playerid,pFPSCounter[playerid], 1);
	PlayerTextDrawSetProportional(playerid,pFPSCounter[playerid], true);
	PlayerTextDrawSetSelectable(playerid,pFPSCounter[playerid], false);
	
	PlayerTextDrawShow(playerid, pFPSCounter[playerid]);
	return true;
}

stock HideFPSCounter(playerid)
{
	PlayerTextDrawHide(playerid, pFPSCounter[playerid]);
	return true;
}

stock CreatePlayerTextDraws(playerid)
{
	MessageTD[playerid] = CreatePlayerTextDraw(playerid, 325.000, 349.000, "_");
	PlayerTextDrawLetterSize(playerid, MessageTD[playerid], 0.250, 1.399);
	PlayerTextDrawTextSize(playerid, MessageTD[playerid], 0.000, 250.000);
	PlayerTextDrawAlignment(playerid, MessageTD[playerid], TEXT_DRAW_ALIGN_CENTER);
	PlayerTextDrawColour(playerid, MessageTD[playerid], -1);
	PlayerTextDrawSetShadow(playerid, MessageTD[playerid], 1);
	PlayerTextDrawSetOutline(playerid, MessageTD[playerid], 1);
	PlayerTextDrawBackgroundColour(playerid, MessageTD[playerid], 150);
	PlayerTextDrawFont(playerid, MessageTD[playerid], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, MessageTD[playerid], true);
	
	/* XP_Textdraw[playerid][0] = CreatePlayerTextDraw(playerid, 0.000, 446.000, "LD_SPAC:white");
	PlayerTextDrawTextSize(playerid, XP_Textdraw[playerid][0], 640.000, 5.000);
	PlayerTextDrawAlignment(playerid, XP_Textdraw[playerid][0], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, XP_Textdraw[playerid][0], -100694529);
	PlayerTextDrawSetShadow(playerid, XP_Textdraw[playerid][0], 0);
	PlayerTextDrawSetOutline(playerid, XP_Textdraw[playerid][0], 0);
	PlayerTextDrawBackgroundColour(playerid, XP_Textdraw[playerid][0], 255);
	PlayerTextDrawFont(playerid, XP_Textdraw[playerid][0], TEXT_DRAW_FONT_SPRITE_DRAW);
	PlayerTextDrawSetProportional(playerid, XP_Textdraw[playerid][0], true);

	XP_Textdraw[playerid][1] = CreatePlayerTextDraw(playerid, 0.000, 446.000, "LD_SPAC:white");
	PlayerTextDrawTextSize(playerid, XP_Textdraw[playerid][1], 0.000, 5.000); // max 640.000
	PlayerTextDrawAlignment(playerid, XP_Textdraw[playerid][1], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, XP_Textdraw[playerid][1], -12254977);
	PlayerTextDrawSetShadow(playerid, XP_Textdraw[playerid][1], 0);
	PlayerTextDrawSetOutline(playerid, XP_Textdraw[playerid][1], 0);
	PlayerTextDrawBackgroundColour(playerid, XP_Textdraw[playerid][1], 255);
	PlayerTextDrawFont(playerid, XP_Textdraw[playerid][1], TEXT_DRAW_FONT_SPRITE_DRAW);
	PlayerTextDrawSetProportional(playerid, XP_Textdraw[playerid][1], true);

	XP_Textdraw[playerid][2] = CreatePlayerTextDraw(playerid, 635.000, 432.000, "Cap do: _ ~y~(_)");
	PlayerTextDrawLetterSize(playerid, XP_Textdraw[playerid][2], 0.200, 1.199);
	PlayerTextDrawAlignment(playerid, XP_Textdraw[playerid][2], TEXT_DRAW_ALIGN_RIGHT);
	PlayerTextDrawColour(playerid, XP_Textdraw[playerid][2], -1);
	PlayerTextDrawSetShadow(playerid, XP_Textdraw[playerid][2], 1);
	PlayerTextDrawSetOutline(playerid, XP_Textdraw[playerid][2], 1);
	PlayerTextDrawBackgroundColour(playerid, XP_Textdraw[playerid][2], 150);
	PlayerTextDrawFont(playerid, XP_Textdraw[playerid][2], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, XP_Textdraw[playerid][2], true); */

	/* GPS[playerid] = TextDrawCreate(95.000000, 319.000000, "Dang tai...");
	TextDrawAlignment(GPS[playerid], 2);
	TextDrawBackgroundColour(GPS[playerid], 255);
	TextDrawFont(GPS[playerid], 2);
	TextDrawLetterSize(GPS[playerid], 0.250000, 1.800000);
	TextDrawColour(GPS[playerid], -1);
	TextDrawSetOutline(GPS[playerid], 1);
	TextDrawSetProportional(GPS[playerid], true);
	TextDrawShowForPlayer(playerid, GPS[playerid]); */

	//GiveCash Notice
	GiveCashTD[playerid] = CreatePlayerTextDraw(playerid, 607.000, 108.000, "+$0");
	PlayerTextDrawLetterSize(playerid, GiveCashTD[playerid], 0.290, 1.600);
	PlayerTextDrawAlignment(playerid, GiveCashTD[playerid], TEXT_DRAW_ALIGN_RIGHT);
	PlayerTextDrawColour(playerid, GiveCashTD[playerid], 579543807);
	PlayerTextDrawSetShadow(playerid, GiveCashTD[playerid], 0);
	PlayerTextDrawSetOutline(playerid, GiveCashTD[playerid], 1);
	PlayerTextDrawBackgroundColour(playerid, GiveCashTD[playerid], 150);
	PlayerTextDrawFont(playerid, GiveCashTD[playerid], TEXT_DRAW_FONT_3);
	PlayerTextDrawSetProportional(playerid, GiveCashTD[playerid], true);
	
	//Shop Notice Textdraw
	ShopNotice[playerid] = CreatePlayerTextDraw(playerid,16.000000, 193.000000, "_");
	PlayerTextDrawBackgroundColour(playerid, ShopNotice[playerid], 255);
	PlayerTextDrawFont(playerid, ShopNotice[playerid], 1);
	PlayerTextDrawLetterSize(playerid, ShopNotice[playerid], 0.289999, 1.299999);
	PlayerTextDrawColour(playerid, ShopNotice[playerid], -1);
	PlayerTextDrawSetOutline(playerid, ShopNotice[playerid], 0);
	PlayerTextDrawSetProportional(playerid, ShopNotice[playerid], true);
	PlayerTextDrawSetShadow(playerid, ShopNotice[playerid], 1);
	PlayerTextDrawUseBox(playerid, ShopNotice[playerid], true);
	PlayerTextDrawBoxColour(playerid, ShopNotice[playerid], 75);
	PlayerTextDrawTextSize(playerid, ShopNotice[playerid], 186.000000, 63.000000);
	PlayerTextDrawSetSelectable(playerid, ShopNotice[playerid], false);
	
	//LEO Textdraws
	BackupText[playerid] = CreatePlayerTextDraw(playerid, 505 ,179 , "~r~Backup call active");
	PlayerTextDrawFont(playerid,BackupText[playerid] , 1);
	PlayerTextDrawLetterSize(playerid,BackupText[playerid] , 0.270000, 2.000000);
	PlayerTextDrawColour(playerid,BackupText[playerid] , -1);
	PlayerTextDrawSetOutline(playerid,BackupText[playerid] , true);
	PlayerTextDrawSetProportional(playerid,BackupText[playerid] , true);
	PlayerTextDrawSetShadow(playerid, BackupText[playerid] , 1);
	
	/* // hunger
	_hungerText[playerid] = CreatePlayerTextDraw(playerid, 635.000, 420.000, "Trang thai: ~w~Hai long");
	PlayerTextDrawLetterSize(playerid, _hungerText[playerid], 0.200, 1.199);
	PlayerTextDrawAlignment(playerid, _hungerText[playerid], TEXT_DRAW_ALIGN_RIGHT);
	PlayerTextDrawColour(playerid, _hungerText[playerid], -1);
	PlayerTextDrawSetShadow(playerid, _hungerText[playerid], 1);
	PlayerTextDrawSetOutline(playerid, _hungerText[playerid], 1);
	PlayerTextDrawBackgroundColour(playerid, _hungerText[playerid], 150);
	PlayerTextDrawFont(playerid, _hungerText[playerid], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, _hungerText[playerid], true); */
	
	//Vehicle Textdraws
	_vhudFlash[playerid] = CreatePlayerTextDraw(playerid, -20.000000, 2.000000,"|");
	PlayerTextDrawUseBox(playerid,_vhudFlash[playerid], true);
 	PlayerTextDrawBoxColour(playerid,_vhudFlash[playerid],0xffffff55);
  	PlayerTextDrawTextSize(playerid,_vhudFlash[playerid],660.000000,22.000000);
   	PlayerTextDrawAlignment(playerid,_vhudFlash[playerid],0);
   	PlayerTextDrawBackgroundColour(playerid,_vhudFlash[playerid],0x000000ff);
    PlayerTextDrawFont(playerid,_vhudFlash[playerid],3);
    PlayerTextDrawLetterSize(playerid,_vhudFlash[playerid],1.000000,52.200000);
    PlayerTextDrawColour(playerid,_vhudFlash[playerid],0xffffffff);
    PlayerTextDrawSetOutline(playerid,_vhudFlash[playerid], 1);
    PlayerTextDrawSetProportional(playerid,_vhudFlash[playerid], true);
    PlayerTextDrawSetShadow(playerid, _vhudFlash[playerid], 1);

	SpeedoMeter_TD[playerid][0] = CreatePlayerTextDraw(playerid, 477.000, 327.000, "000");
	PlayerTextDrawLetterSize(playerid, SpeedoMeter_TD[playerid][0], 0.399, 4.199);
	PlayerTextDrawAlignment(playerid, SpeedoMeter_TD[playerid][0], TEXT_DRAW_ALIGN_RIGHT);
	PlayerTextDrawColour(playerid, SpeedoMeter_TD[playerid][0], -1);
	PlayerTextDrawSetShadow(playerid, SpeedoMeter_TD[playerid][0], 1);
	PlayerTextDrawSetOutline(playerid, SpeedoMeter_TD[playerid][0], 1);
	PlayerTextDrawBackgroundColour(playerid, SpeedoMeter_TD[playerid][0], 150);
	PlayerTextDrawFont(playerid, SpeedoMeter_TD[playerid][0], TEXT_DRAW_FONT_2);
	PlayerTextDrawSetProportional(playerid, SpeedoMeter_TD[playerid][0], true);

	SpeedoMeter_TD[playerid][1] = CreatePlayerTextDraw(playerid, 499.000, 347.000, "KM/H");
	PlayerTextDrawLetterSize(playerid, SpeedoMeter_TD[playerid][1], 0.190, 1.500);
	PlayerTextDrawAlignment(playerid, SpeedoMeter_TD[playerid][1], TEXT_DRAW_ALIGN_RIGHT);
	PlayerTextDrawColour(playerid, SpeedoMeter_TD[playerid][1], -1);
	PlayerTextDrawSetShadow(playerid, SpeedoMeter_TD[playerid][1], 1);
	PlayerTextDrawSetOutline(playerid, SpeedoMeter_TD[playerid][1], 1);
	PlayerTextDrawBackgroundColour(playerid, SpeedoMeter_TD[playerid][1], 150);
	PlayerTextDrawFont(playerid, SpeedoMeter_TD[playerid][1], TEXT_DRAW_FONT_2);
	PlayerTextDrawSetProportional(playerid, SpeedoMeter_TD[playerid][1], true);

	SpeedoMeter_TD[playerid][2] = CreatePlayerTextDraw(playerid, 478.000, 335.000, "0.0%");
	PlayerTextDrawLetterSize(playerid, SpeedoMeter_TD[playerid][2], 0.190, 1.500);
	PlayerTextDrawAlignment(playerid, SpeedoMeter_TD[playerid][2], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, SpeedoMeter_TD[playerid][2], -626712321);
	PlayerTextDrawSetShadow(playerid, SpeedoMeter_TD[playerid][2], 1);
	PlayerTextDrawSetOutline(playerid, SpeedoMeter_TD[playerid][2], 1);
	PlayerTextDrawBackgroundColour(playerid, SpeedoMeter_TD[playerid][2], 150);
	PlayerTextDrawFont(playerid, SpeedoMeter_TD[playerid][2], TEXT_DRAW_FONT_2);
	PlayerTextDrawSetProportional(playerid, SpeedoMeter_TD[playerid][2], true);

	SpeedoMeter_TD[playerid][3] = CreatePlayerTextDraw(playerid, 452.000, 360.000, "SB");
	PlayerTextDrawLetterSize(playerid, SpeedoMeter_TD[playerid][3], 0.239, 2.099);
	PlayerTextDrawAlignment(playerid, SpeedoMeter_TD[playerid][3], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, SpeedoMeter_TD[playerid][3], -1);
	PlayerTextDrawSetShadow(playerid, SpeedoMeter_TD[playerid][3], 1);
	PlayerTextDrawSetOutline(playerid, SpeedoMeter_TD[playerid][3], 1);
	PlayerTextDrawBackgroundColour(playerid, SpeedoMeter_TD[playerid][3], 150);
	PlayerTextDrawFont(playerid, SpeedoMeter_TD[playerid][3], TEXT_DRAW_FONT_2);
	PlayerTextDrawSetProportional(playerid, SpeedoMeter_TD[playerid][3], true);

	SpeedoMeter_TD[playerid][4] = CreatePlayerTextDraw(playerid, 472.000, 360.000, "LIGHT");
	PlayerTextDrawLetterSize(playerid, SpeedoMeter_TD[playerid][4], 0.239, 2.099);
	PlayerTextDrawAlignment(playerid, SpeedoMeter_TD[playerid][4], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, SpeedoMeter_TD[playerid][4], -1);
	PlayerTextDrawSetShadow(playerid, SpeedoMeter_TD[playerid][4], 1);
	PlayerTextDrawSetOutline(playerid, SpeedoMeter_TD[playerid][4], 1);
	PlayerTextDrawBackgroundColour(playerid, SpeedoMeter_TD[playerid][4], 150);
	PlayerTextDrawFont(playerid, SpeedoMeter_TD[playerid][4], TEXT_DRAW_FONT_2);
	PlayerTextDrawSetProportional(playerid, SpeedoMeter_TD[playerid][4], true);
	
	_crTextTarget[playerid] = CreatePlayerTextDraw(playerid, 320.000000, 388.000000, "Target Vehicle: ~r~N/A");
	PlayerTextDrawAlignment(playerid, _crTextTarget[playerid], 2);
	PlayerTextDrawBackgroundColour(playerid, _crTextTarget[playerid], 255);
	PlayerTextDrawFont(playerid, _crTextTarget[playerid], 1);
	PlayerTextDrawLetterSize(playerid, _crTextTarget[playerid], 0.500000, 1.600000);
	PlayerTextDrawColour(playerid, _crTextTarget[playerid], -1);
	PlayerTextDrawSetOutline(playerid, _crTextTarget[playerid], 1);
	PlayerTextDrawSetProportional(playerid, _crTextTarget[playerid], true);

	_crTextSpeed[playerid] = CreatePlayerTextDraw(playerid, 190.000000, 410.000000, "Toc do: ~r~N/A");
	PlayerTextDrawAlignment(playerid, _crTextSpeed[playerid], 1);
	PlayerTextDrawBackgroundColour(playerid, _crTextSpeed[playerid], 255);
	PlayerTextDrawFont(playerid, _crTextSpeed[playerid], 1);
	PlayerTextDrawLetterSize(playerid, _crTextSpeed[playerid], 0.500000, 1.600000);
	PlayerTextDrawColour(playerid, _crTextSpeed[playerid], -1);
	PlayerTextDrawSetOutline(playerid, _crTextSpeed[playerid], 1);
	PlayerTextDrawSetProportional(playerid, _crTextSpeed[playerid], true);
	
	_crTickets[playerid] = CreatePlayerTextDraw(playerid, 340.000000, 410.000000, "Tickets: ~r~N/A");
	PlayerTextDrawAlignment(playerid, _crTickets[playerid], 1);
	PlayerTextDrawBackgroundColour(playerid, _crTickets[playerid], 255);
	PlayerTextDrawFont(playerid, _crTickets[playerid], 1);
	PlayerTextDrawLetterSize(playerid, _crTickets[playerid], 0.500000, 1.600000);
	PlayerTextDrawColour(playerid, _crTickets[playerid], -1);
	PlayerTextDrawSetOutline(playerid, _crTickets[playerid], 1);
	PlayerTextDrawSetProportional(playerid, _crTickets[playerid], true);
	
	_vhudVisible[playerid] = 0;
	return 1;
}

stock DestroyInventoryTextDraws(playerid)
{
	for(new i=0;i<MAX_ITEM_PER_PAGE;i++) 
	{
		if(IsValidPlayerTextDraw(playerid, InvTD@Slot[playerid][i])) 
			PlayerTextDrawDestroy(playerid, InvTD@Slot[playerid][i]);

		if(IsValidPlayerTextDraw(playerid, InvTD@ItemName[playerid][i])) 
			PlayerTextDrawDestroy(playerid, InvTD@ItemName[playerid][i]);

		InvTD@Slot[playerid][i] = INVALID_PLAYER_TEXT_DRAW;
		InvTD@ItemName[playerid][i] = INVALID_PLAYER_TEXT_DRAW;
	}

	if(IsValidPlayerTextDraw(playerid, InvTD@OwnerName[playerid]))
		PlayerTextDrawDestroy(playerid, InvTD@OwnerName[playerid]);
	
	if(IsValidPlayerTextDraw(playerid, InvTD@ItemInfo[playerid]))
		PlayerTextDrawDestroy(playerid, InvTD@ItemInfo[playerid]);

	if(IsValidPlayerTextDraw(playerid, InvTD@Page[playerid]))
		PlayerTextDrawDestroy(playerid, InvTD@Page[playerid]);

	InvTD@OwnerName[playerid] = INVALID_PLAYER_TEXT_DRAW;
	InvTD@ItemInfo[playerid] = INVALID_PLAYER_TEXT_DRAW;
	InvTD@Page[playerid] = INVALID_PLAYER_TEXT_DRAW;
	return 1;
}

stock CreateInventoryTextDraws(playerid)
{
	new Float:x=0.0, Float:y=105.0;
	for(new i=0;i<MAX_ITEM_PER_PAGE;i++) 
	{
		if(i % 5 == 0) {
			x = 211.0;
			y += 54.0;
		}
		else {
			x += 49.0;
		}

		InvTD@Slot[playerid][i] = CreatePlayerTextDraw(playerid, x, y, "_");
		PlayerTextDrawTextSize(playerid, InvTD@Slot[playerid][i], 46.000, 49.000);
		PlayerTextDrawAlignment(playerid, InvTD@Slot[playerid][i], TEXT_DRAW_ALIGN_LEFT);
		PlayerTextDrawColour(playerid, InvTD@Slot[playerid][i], -94);
		PlayerTextDrawSetShadow(playerid, InvTD@Slot[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, InvTD@Slot[playerid][i], 0);
		PlayerTextDrawBackgroundColour(playerid, InvTD@Slot[playerid][i], 1179010710);
		PlayerTextDrawFont(playerid, InvTD@Slot[playerid][i], TEXT_DRAW_FONT_MODEL_PREVIEW);
		PlayerTextDrawSetProportional(playerid, InvTD@Slot[playerid][i], false);
		PlayerTextDrawSetPreviewModel(playerid, InvTD@Slot[playerid][i], 19479);
		PlayerTextDrawSetPreviewRot(playerid, InvTD@Slot[playerid][i], 0.000, 0.000, 0.000, 1.000);
		PlayerTextDrawSetPreviewVehicleColours(playerid, InvTD@Slot[playerid][i], 0, 0);
		PlayerTextDrawSetSelectable(playerid, InvTD@Slot[playerid][i], true);

		InvTD@ItemName[playerid][i] = CreatePlayerTextDraw(playerid, x+23.0, y-1.0, "_");
		PlayerTextDrawLetterSize(playerid, InvTD@ItemName[playerid][i], 0.180, 0.899);
		PlayerTextDrawTextSize(playerid, InvTD@ItemName[playerid][i], 0.000, 47.000);
		PlayerTextDrawAlignment(playerid, InvTD@ItemName[playerid][i], TEXT_DRAW_ALIGN_CENTER);
		PlayerTextDrawColour(playerid, InvTD@ItemName[playerid][i], -1);
		PlayerTextDrawSetShadow(playerid, InvTD@ItemName[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, InvTD@ItemName[playerid][i], 0);
		PlayerTextDrawBackgroundColour(playerid, InvTD@ItemName[playerid][i], 150);
		PlayerTextDrawFont(playerid, InvTD@ItemName[playerid][i], TEXT_DRAW_FONT_1);
		PlayerTextDrawSetProportional(playerid, InvTD@ItemName[playerid][i], true);
	}

	InvTD@OwnerName[playerid] = CreatePlayerTextDraw(playerid, 208.000, 135.000, "Tui do cua _");
	PlayerTextDrawLetterSize(playerid, InvTD@OwnerName[playerid], 0.250, 1.299);
	PlayerTextDrawAlignment(playerid, InvTD@OwnerName[playerid], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, InvTD@OwnerName[playerid], -1);
	PlayerTextDrawSetShadow(playerid, InvTD@OwnerName[playerid], 1);
	PlayerTextDrawSetOutline(playerid, InvTD@OwnerName[playerid], 1);
	PlayerTextDrawBackgroundColour(playerid, InvTD@OwnerName[playerid], 150);
	PlayerTextDrawFont(playerid, InvTD@OwnerName[playerid], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, InvTD@OwnerName[playerid], true);

	InvTD@ItemInfo[playerid] = CreatePlayerTextDraw(playerid, 211.000, 328.000, "_");
	PlayerTextDrawLetterSize(playerid, InvTD@ItemInfo[playerid], 0.230, 1.199);
	PlayerTextDrawTextSize(playerid, InvTD@ItemInfo[playerid], 368.000, 0.000);
	PlayerTextDrawAlignment(playerid, InvTD@ItemInfo[playerid], TEXT_DRAW_ALIGN_LEFT);
	PlayerTextDrawColour(playerid, InvTD@ItemInfo[playerid], -1);
	PlayerTextDrawSetShadow(playerid, InvTD@ItemInfo[playerid], 0);
	PlayerTextDrawSetOutline(playerid, InvTD@ItemInfo[playerid], 0);
	PlayerTextDrawBackgroundColour(playerid, InvTD@ItemInfo[playerid], 150);
	PlayerTextDrawFont(playerid, InvTD@ItemInfo[playerid], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, InvTD@ItemInfo[playerid], true);

	InvTD@Page[playerid] = CreatePlayerTextDraw(playerid, 409.000, 341.000, "0/0");
	PlayerTextDrawLetterSize(playerid, InvTD@Page[playerid], 0.300, 1.500);
	PlayerTextDrawAlignment(playerid, InvTD@Page[playerid], TEXT_DRAW_ALIGN_CENTER);
	PlayerTextDrawColour(playerid, InvTD@Page[playerid], -1);
	PlayerTextDrawSetShadow(playerid, InvTD@Page[playerid], 0);
	PlayerTextDrawSetOutline(playerid, InvTD@Page[playerid], 0);
	PlayerTextDrawBackgroundColour(playerid, InvTD@Page[playerid], 150);
	PlayerTextDrawFont(playerid, InvTD@Page[playerid], TEXT_DRAW_FONT_1);
	PlayerTextDrawSetProportional(playerid, InvTD@Page[playerid], true);
	return 1;
}