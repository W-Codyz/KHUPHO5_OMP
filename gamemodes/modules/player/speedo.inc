/* stock ShowVehicleHUDForPlayer(playerid)
{
	for(new i; i < 5; i++) {
		PlayerTextDrawShow(playerid, SpeedoMeter_TD[playerid][i]);
	}
	_vhudVisible[playerid] = 1;
}

stock HideVehicleHUDForPlayer(playerid)
{
	for(new i; i < 5; i++) {
		PlayerTextDrawHide(playerid, SpeedoMeter_TD[playerid][i]);
	}
	_vhudVisible[playerid] = 0;
}

stock UpdateVehicleHUDForPlayer(playerid, fuel, speed)
{
	new str[128], vehicleid = GetPlayerVehicleID(playerid), szColor[4];
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid,engine,lights,alarm,doors,bonnet,boot,objective);
	switch(speed)
	{
	    case 0..40: szColor = "~w~";
	    case 41..60: szColor = "~y~";
	    default: szColor = "~r~";
	}

	if (IsVIPcar(vehicleid) || IsAdminSpawnedVehicle(vehicleid) || IsFamedVeh(vehicleid) || GetVehicleModel(vehicleid) == 481 || GetVehicleModel(vehicleid) == 509 || GetVehicleModel(vehicleid) == 510)
		format(str, sizeof(str), "~b~~h~U");
	else
		format(str, sizeof(str), "%0.1f%", float(fuel));

	PlayerTextDrawSetString(playerid, SpeedoMeter_TD[playerid][2], str);

	format(str, sizeof(str), "%s%03d", szColor, speed);
	PlayerTextDrawSetString(playerid, SpeedoMeter_TD[playerid][0], str);
	
	if(Seatbelt[playerid] == 0)
	{
		format(str, sizeof(str), "~r~~h~SB");
		PlayerTextDrawSetString(playerid, SpeedoMeter_TD[playerid][3], str);
	}
	else {
		format(str, sizeof(str), "~g~~h~SB");
		PlayerTextDrawSetString(playerid, SpeedoMeter_TD[playerid][3], str);
	}
	if(lights != VEHICLE_PARAMS_ON) {
		format(str, sizeof(str), "~r~~h~LIGHT");
		PlayerTextDrawSetString(playerid, SpeedoMeter_TD[playerid][4], str);	
	}
	else {
		format(str, sizeof(str), "~g~~h~LIGHT");
		PlayerTextDrawSetString(playerid, SpeedoMeter_TD[playerid][4], str);
	}
} */

new PlayerText: SpeedoDrpTD[MAX_PLAYERS][50];

//-------------------------------
stock GetVehicleSpeed(vehicleid)
{
    new
        Float:vel[3],
        Float:tempspeed;
    GetVehicleVelocity(vehicleid, vel[0], vel[1], vel[2]);
    tempspeed = floatsqroot((vel[0] * vel[0]) + (vel[1] * vel[1]) + (vel[2] * vel[2])) * 136.666667;
    return floatround(tempspeed, floatround_round);
}
//-------------------------------

stock CreateSpeedoTD(playerid)
{
	SpeedoDrpTD[playerid][0] = CreatePlayerTextDraw(playerid, 146.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][0], 50.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][0], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][0], 150);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][0], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][0], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][0], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][0], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][0], 1);

    SpeedoDrpTD[playerid][1] = CreatePlayerTextDraw(playerid, 146.000, 359.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][1], 15.000, 15.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][1], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][1], -1732209921);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][1], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][1], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][1], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][1], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][1], 1);

    SpeedoDrpTD[playerid][2] = CreatePlayerTextDraw(playerid, 198.000, 359.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][2], 4.000, 38.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][2], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][2], 255);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][2], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][2], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][2], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][2], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][2], 1);

    SpeedoDrpTD[playerid][3] = CreatePlayerTextDraw(playerid, 198.898, 396.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][3], 1.500, -35.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][3], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][3], -1);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][3], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][3], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][3], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][3], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][3], 1);

    SpeedoDrpTD[playerid][4] = CreatePlayerTextDraw(playerid, 198.000, 358.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][4], 5.000, -2.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][4], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][4], -1);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][4], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][4], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][4], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][4], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][4], 1);

    SpeedoDrpTD[playerid][5] = CreatePlayerTextDraw(playerid, 198.000, 358.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][5], 1.000, -9.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][5], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][5], -1);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][5], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][5], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][5], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][5], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][5], 1);

    SpeedoDrpTD[playerid][6] = CreatePlayerTextDraw(playerid, 201.000, 358.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][6], 1.000, -9.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][6], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][6], -1);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][6], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][6], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][6], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][6], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][6], 1);

    SpeedoDrpTD[playerid][7] = CreatePlayerTextDraw(playerid, 199.000, 353.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][7], 3.000, 5.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][7], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][7], -1);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][7], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][7], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][7], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][7], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][7], 1);

    SpeedoDrpTD[playerid][8] = CreatePlayerTextDraw(playerid, 198.000, 350.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][8], 4.000, -1.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][8], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][8], -1);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][8], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][8], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][8], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][8], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][8], 1);

    SpeedoDrpTD[playerid][9] = CreatePlayerTextDraw(playerid, 201.000, 349.000, ")");
    PlayerTextDrawLetterSize(playerid, SpeedoDrpTD[playerid][9], 0.259, 0.597);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][9], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][9], -1);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][9], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][9], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][9], 150);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][9], 0);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][9], 1);

    SpeedoDrpTD[playerid][10] = CreatePlayerTextDraw(playerid, 146.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][10], 2.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][10], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][10], -1732209921);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][10], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][10], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][10], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][10], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][10], 1);

    SpeedoDrpTD[playerid][11] = CreatePlayerTextDraw(playerid, 148.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][11], 2.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][11], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][11], -1664968961);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][11], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][11], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][11], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][11], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][11], 1);

    SpeedoDrpTD[playerid][12] = CreatePlayerTextDraw(playerid, 150.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][12], 2.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][12], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][12], -1597662209);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][12], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][12], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][12], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][12], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][12], 1);

    SpeedoDrpTD[playerid][13] = CreatePlayerTextDraw(playerid, 152.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][13], 2.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][13], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][13], -1530421249);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][13], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][13], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][13], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][13], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][13], 1);

    SpeedoDrpTD[playerid][14] = CreatePlayerTextDraw(playerid, 154.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][14], 2.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][14], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][14], -1446337281);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][14], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][14], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][14], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][14], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][14], 1);

    SpeedoDrpTD[playerid][15] = CreatePlayerTextDraw(playerid, 156.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][15], 2.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][15], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][15], -1379030785);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][15], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][15], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][15], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][15], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][15], 1);

    SpeedoDrpTD[playerid][16] = CreatePlayerTextDraw(playerid, 158.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][16], 2.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][16], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][16], -1311789569);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][16], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][16], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][16], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][16], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][16], 1);

    SpeedoDrpTD[playerid][17] = CreatePlayerTextDraw(playerid, 160.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][17], 2.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][17], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][17], -1227705857);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][17], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][17], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][17], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][17], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][17], 1);

    SpeedoDrpTD[playerid][18] = CreatePlayerTextDraw(playerid, 162.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][18], 2.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][18], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][18], -1160399105);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][18], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][18], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][18], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][18], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][18], 1);

    SpeedoDrpTD[playerid][19] = CreatePlayerTextDraw(playerid, 164.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][19], 2.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][19], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][19], -1093158145);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][19], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][19], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][19], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][19], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][19], 1);

    SpeedoDrpTD[playerid][20] = CreatePlayerTextDraw(playerid, 166.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][20], 2.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][20], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][20], -1025851393);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][20], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][20], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][20], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][20], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][20], 1);

    SpeedoDrpTD[playerid][21] = CreatePlayerTextDraw(playerid, 168.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][21], 2.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][21], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][21], -941833217);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][21], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][21], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][21], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][21], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][21], 1);

    SpeedoDrpTD[playerid][22] = CreatePlayerTextDraw(playerid, 170.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][22], 2.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][22], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][22], -874526465);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][22], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][22], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][22], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][22], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][22], 1);

    SpeedoDrpTD[playerid][23] = CreatePlayerTextDraw(playerid, 172.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][23], 2.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][23], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][23], -807219713);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][23], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][23], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][23], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][23], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][23], 1);

    SpeedoDrpTD[playerid][24] = CreatePlayerTextDraw(playerid, 174.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][24], 2.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][24], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][24], -723201537);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][24], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][24], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][24], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][24], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][24], 1);

    SpeedoDrpTD[playerid][25] = CreatePlayerTextDraw(playerid, 176.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][25], 2.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][25], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][25], -655894785);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][25], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][25], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][25], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][25], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][25], 1);

    SpeedoDrpTD[playerid][26] = CreatePlayerTextDraw(playerid, 178.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][26], 2.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][26], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][26], -588588289);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][26], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][26], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][26], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][26], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][26], 1);

    SpeedoDrpTD[playerid][27] = CreatePlayerTextDraw(playerid, 180.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][27], 2.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][27], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][27], -521347073);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][27], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][27], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][27], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][27], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][27], 1);

    SpeedoDrpTD[playerid][28] = CreatePlayerTextDraw(playerid, 182.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][28], 2.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][28], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][28], -437263361);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][28], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][28], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][28], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][28], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][28], 1);

    SpeedoDrpTD[playerid][29] = CreatePlayerTextDraw(playerid, 184.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][29], 2.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][29], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][29], -370022145);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][29], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][29], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][29], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][29], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][29], 1);

    SpeedoDrpTD[playerid][30] = CreatePlayerTextDraw(playerid, 186.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][30], 2.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][30], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][30], -302715649);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][30], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][30], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][30], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][30], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][30], 1);

    SpeedoDrpTD[playerid][31] = CreatePlayerTextDraw(playerid, 188.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][31], 2.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][31], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][31], -218631681);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][31], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][31], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][31], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][31], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][31], 1);

    SpeedoDrpTD[playerid][32] = CreatePlayerTextDraw(playerid, 190.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][32], 2.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][32], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][32], -151390721);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][32], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][32], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][32], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][32], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][32], 1);

    SpeedoDrpTD[playerid][33] = CreatePlayerTextDraw(playerid, 192.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][33], 2.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][33], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][33], -84083969);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][33], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][33], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][33], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][33], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][33], 1);

    SpeedoDrpTD[playerid][34] = CreatePlayerTextDraw(playerid, 194.000, 377.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][34], 2.000, 20.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][34], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][34], -1);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][34], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][34], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][34], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][34], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][34], 1);

    SpeedoDrpTD[playerid][35] = CreatePlayerTextDraw(playerid, 153.000, 362.000, "NW");
    PlayerTextDrawLetterSize(playerid, SpeedoDrpTD[playerid][35], 0.200, 1.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][35], 2);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][35], -1);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][35], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][35], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][35], 150);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][35], 1);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][35], 1);

    SpeedoDrpTD[playerid][36] = CreatePlayerTextDraw(playerid, 174.000, 364.000, "250");
    PlayerTextDrawLetterSize(playerid, SpeedoDrpTD[playerid][36], 0.200, 1.200);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][36], 2);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][36], -1);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][36], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][36], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][36], 150);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][36], 2);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][36], 1);

    SpeedoDrpTD[playerid][37] = CreatePlayerTextDraw(playerid, 186.000, 356.000, "KM/H");
    PlayerTextDrawLetterSize(playerid, SpeedoDrpTD[playerid][37], 0.100, 0.898);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][37], 2);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][37], -1);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][37], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][37], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][37], 150);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][37], 2);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][37], 1);

    SpeedoDrpTD[playerid][38] = CreatePlayerTextDraw(playerid, 146.000, 405.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][38], 8.000, 6.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][38], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][38], -1);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][38], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][38], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][38], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][38], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][38], 1);

    SpeedoDrpTD[playerid][39] = CreatePlayerTextDraw(playerid, 146.000, 399.000, "/");
    PlayerTextDrawLetterSize(playerid, SpeedoDrpTD[playerid][39], 0.160, 0.699);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][39], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][39], -1);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][39], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][39], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][39], 150);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][39], 1);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][39], 1);

    SpeedoDrpTD[playerid][40] = CreatePlayerTextDraw(playerid, 154.000, 399.000, "/");
    PlayerTextDrawLetterSize(playerid, SpeedoDrpTD[playerid][40], -0.169, 0.699);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][40], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][40], -1);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][40], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][40], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][40], 150);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][40], 1);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][40], 1);

    SpeedoDrpTD[playerid][41] = CreatePlayerTextDraw(playerid, 148.000, 400.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][41], 4.000, 2.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][41], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][41], -1);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][41], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][41], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][41], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][41], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][41], 1);

    SpeedoDrpTD[playerid][42] = CreatePlayerTextDraw(playerid, 153.000, 404.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][42], 3.000, 3.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][42], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][42], -1);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][42], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][42], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][42], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][42], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][42], 1);

    SpeedoDrpTD[playerid][43] = CreatePlayerTextDraw(playerid, 144.000, 404.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][43], 3.000, 3.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][43], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][43], -1);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][43], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][43], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][43], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][43], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][43], 1);

    SpeedoDrpTD[playerid][44] = CreatePlayerTextDraw(playerid, 147.000, 411.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][44], 2.000, 2.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][44], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][44], -1);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][44], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][44], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][44], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][44], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][44], 1);

    SpeedoDrpTD[playerid][45] = CreatePlayerTextDraw(playerid, 151.000, 411.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][45], 2.000, 2.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][45], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][45], -1);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][45], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][45], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][45], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][45], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][45], 1);

    SpeedoDrpTD[playerid][46] = CreatePlayerTextDraw(playerid, 151.000, 408.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][46], 2.000, 2.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][46], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][46], -1732209921);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][46], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][46], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][46], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][46], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][46], 1);

    SpeedoDrpTD[playerid][47] = CreatePlayerTextDraw(playerid, 147.000, 408.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][47], 2.000, 2.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][47], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][47], -1732209921);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][47], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][47], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][47], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][47], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][47], 1);

    SpeedoDrpTD[playerid][48] = CreatePlayerTextDraw(playerid, 157.000, 404.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][48], 39.000, 5.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][48], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][48], 255);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][48], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][48], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][48], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][48], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][48], 1);

    SpeedoDrpTD[playerid][49] = CreatePlayerTextDraw(playerid, 158.898, 407.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][49], 35.500, -2.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][49], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][49], -1);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][49], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][49], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][49], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][49], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][49], 1);
}

hook OnPlayerConnect(playerid)
{
	CreateSpeedoTD(playerid);
	return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
	HideSpeedoTD(playerid);
	return 1;
}

hook OnPlayerStateChange(playerid, newstate, oldstate)
{
    if(newstate == PLAYER_STATE_DRIVER || newstate == PLAYER_STATE_PASSENGER)
    {
        if(PlayerInfo[playerid][pSpeedo] == 1)
        {
            ShowSpeedoTD(playerid);
        }
    }
    else if(oldstate == PLAYER_STATE_DRIVER || oldstate == PLAYER_STATE_PASSENGER)
    {
        HideSpeedoTD(playerid);
    }
    return 1;
}

ShowSpeedoTD(playerid)
{
	for(new i = 0; i < 50; i++)
	{
		PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][i]);
	}
	return 1;
}

HideSpeedoTD(playerid)
{
	for(new i = 0; i < 50; i++)
	{
		PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][i]);
	}
	return 1;
}

stock PlayerCompassUpdate(playerid, vehid)
{
	static Float:X, Float:Y, Float:Z, Float:Ang;
	GetPlayerPos(playerid, X, Y, Z);

	if(IsPlayerInAnyVehicle(playerid) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		GetVehicleZAngle(vehid, Ang);
	}
	else
	{
		GetPlayerFacingAngle(playerid, Ang);
	}

	static frmtcompass[258];
	if (Ang >= 348.75 || Ang < 11.25) frmtcompass = "N";
	else if (Ang >= 258.75 && Ang < 281.25) frmtcompass = "E";
	else if (Ang >= 303.75 && Ang < 326.25) frmtcompass = "NE";
	else if (Ang >= 168.75 && Ang < 191.25) frmtcompass = "S";
	else if (Ang >= 213.75 && Ang < 236.25) frmtcompass = "SE";
	else if (Ang >= 78.75 && Ang < 101.25) frmtcompass = "W";
	else if (Ang >= 33.75 && Ang < 56.25) frmtcompass = "NW";
	else if (Ang >= 123.25 && Ang < 146.25) frmtcompass = "SW";
	PlayerTextDrawSetString(playerid, SpeedoDrpTD[playerid][35], sprintf("%s", frmtcompass));
	return 1;
}


hook OnPlayerUpdate(playerid)
{
	if(gPlayerLogged{playerid} != 1)
			return 0;

		new vehid = GetPlayerVehicleID(playerid);
		if(IsValidVehicle(vehid))
		{
			if(IsPlayerInAnyVehicle(playerid) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
			{
				new Float:fFuel, Float:fDamage;
				GetVehicleHealth(vehid, fDamage);

				fFuel = VehicleFuel[vehid];
				//GetVehicleHealth(vehid, fDamage);

				if(fFuel < 0) fFuel = 0;
				else if(fFuel > 100) fFuel = 100;

				//if(fDamage <= 350.0) fDamage = 0.0;
				//else if(fDamage > 100.0) fDamage = 100.0;
				new speed[16];
				valstr(speed, GetVehicleSpeed(GetPlayerVehicleID(playerid)));
				PlayerTextDrawSetString(playerid, SpeedoDrpTD[playerid][36], speed);

				new Float:FuelBar;
				FuelBar = fFuel * 1.5/100;
				PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][3], FuelBar, -35.0);
				/* new Float:FuelBar = (fFuel / 100.0) * 44.0; 
				PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][3], FuelBar, -35.0); */		
				PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][3]);

				new Float:HealthBar;
				HealthBar = fDamage * 35.5/1000;
				PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][49], HealthBar, -2.0);
				PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][49]);

				if(EVF::GetVehicleSpeed(vehid) > 1) PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][10]);
				if(EVF::GetVehicleSpeed(vehid) > 6) PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][11]);
				if(EVF::GetVehicleSpeed(vehid) > 10) PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][12]);
				if(EVF::GetVehicleSpeed(vehid) > 14) PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][13]);
				if(EVF::GetVehicleSpeed(vehid) > 17) PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][14]);
				if(EVF::GetVehicleSpeed(vehid) > 20) PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][15]);
				if(EVF::GetVehicleSpeed(vehid) > 24) PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][16]);
				if(EVF::GetVehicleSpeed(vehid) > 28) PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][17]);
				if(EVF::GetVehicleSpeed(vehid) > 32) PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][18]);
				if(EVF::GetVehicleSpeed(vehid) > 36) PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][19]);
				if(EVF::GetVehicleSpeed(vehid) > 40) PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][20]);
				if(EVF::GetVehicleSpeed(vehid) > 44) PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][21]);
				if(EVF::GetVehicleSpeed(vehid) > 48) PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][22]);
				if(EVF::GetVehicleSpeed(vehid) > 52) PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][23]);
				if(EVF::GetVehicleSpeed(vehid) > 56) PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][24]);
				if(EVF::GetVehicleSpeed(vehid) > 60) PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][25]);
				if(EVF::GetVehicleSpeed(vehid) > 64) PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][26]);
				if(EVF::GetVehicleSpeed(vehid) > 68) PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][27]);
				if(EVF::GetVehicleSpeed(vehid) > 72) PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][28]);
				if(EVF::GetVehicleSpeed(vehid) > 76) PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][29]);
				if(EVF::GetVehicleSpeed(vehid) > 80) PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][30]);
				if(EVF::GetVehicleSpeed(vehid) > 84) PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][31]);
				if(EVF::GetVehicleSpeed(vehid) > 87) PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][32]);
				if(EVF::GetVehicleSpeed(vehid) > 89) PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][33]);
				if(EVF::GetVehicleSpeed(vehid) > 90) PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][34]);

				if(EVF::GetVehicleSpeed(vehid) < 1) PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][10]);
				if(EVF::GetVehicleSpeed(vehid) < 6) PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][11]);
				if(EVF::GetVehicleSpeed(vehid) < 10) PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][12]);
				if(EVF::GetVehicleSpeed(vehid) < 14) PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][13]);
				if(EVF::GetVehicleSpeed(vehid) < 17) PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][14]);
				if(EVF::GetVehicleSpeed(vehid) < 20) PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][15]);
				if(EVF::GetVehicleSpeed(vehid) < 24) PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][16]);
				if(EVF::GetVehicleSpeed(vehid) < 28) PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][17]);
				if(EVF::GetVehicleSpeed(vehid) < 32) PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][18]);
				if(EVF::GetVehicleSpeed(vehid) < 36) PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][19]);
				if(EVF::GetVehicleSpeed(vehid) < 40) PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][20]);
				if(EVF::GetVehicleSpeed(vehid) < 44) PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][21]);
				if(EVF::GetVehicleSpeed(vehid) < 48) PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][22]);
				if(EVF::GetVehicleSpeed(vehid) < 52) PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][23]);
				if(EVF::GetVehicleSpeed(vehid) < 56) PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][24]);
				if(EVF::GetVehicleSpeed(vehid) < 60) PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][25]);
				if(EVF::GetVehicleSpeed(vehid) < 64) PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][26]);
				if(EVF::GetVehicleSpeed(vehid) < 68) PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][27]);
				if(EVF::GetVehicleSpeed(vehid) < 72) PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][28]);
				if(EVF::GetVehicleSpeed(vehid) < 76) PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][29]);
				if(EVF::GetVehicleSpeed(vehid) < 80) PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][30]);
				if(EVF::GetVehicleSpeed(vehid) < 84) PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][31]);
				if(EVF::GetVehicleSpeed(vehid) < 87) PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][32]);
				if(EVF::GetVehicleSpeed(vehid) < 89) PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][33]);
				if(EVF::GetVehicleSpeed(vehid) < 90) PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][34]);
				PlayerCompassUpdate(playerid, vehid);
			}
		}
	return 1;
}

CMD:speedo(playerid, params[])
{
	if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER && GetPlayerState(playerid) != PLAYER_STATE_PASSENGER )
	{
		SendClientMessageEx(playerid, COLOR_GREY, "Ban khong o tren xe.");
	}
	else if (!PlayerInfo[playerid][pSpeedo])
	{
		SendClientMessageEx(playerid, COLOR_WHITE, "Ban da bat dong ho toc do.");
		PlayerInfo[playerid][pSpeedo] = 1;
		//ShowVehicleHUDForPlayer(playerid);
		ShowSpeedoTD(playerid);
	}
	else
	{
		SendClientMessageEx(playerid, COLOR_WHITE, "Ban da vo hieu hoa dong ho toc do.");
		PlayerInfo[playerid][pSpeedo] = 0;
		//HideVehicleHUDForPlayer(playerid);
		HideSpeedoTD(playerid);
	}

	return 1;
} 