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

// 46 static TDs — shape/color fixed, show/hide per-player via TextDrawShowForPlayer
new Text:        SpeedoStaticTD[46];
// 4 dynamic TDs — value changes per-player (fuel bar, compass, speed, health bar)
new PlayerText:  SpeedoDrpTD[MAX_PLAYERS][4];

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

// Speed bar colors — 25 segments [9]-[33] in SpeedoStaticTD
static const g_SpeedBarColors[25] = {
    16711935,    // 0
    872349951,   // 1
    872349951,   // 2
    -1711341313, // 3
    -1711341313, // 4
    -855703297,  // 5
    -855703297,  // 6
    -65281,      // 7
    -65281,      // 8
    -65281,      // 9
    -65281,      // 10
    -65281,      // 11
    -3407617,    // 12
    -3407617,    // 13
    -3407617,    // 14
    -3407617,    // 15
    -6749953,    // 16
    -6749953,    // 17
    -6749953,    // 18
    -10092289,   // 19
    -10092289,   // 20
    -13434625,   // 21
    -13434625,   // 22
    -16776961,   // 23
    -16776961    // 24
};

stock CreateSpeedoStaticTD()
{
    // [0] main background
    SpeedoStaticTD[0] = TextDrawCreate(146.000, 377.000, "LD_BUM:blkdot");
    TextDrawTextSize(SpeedoStaticTD[0], 50.000, 20.000);
    TextDrawAlignment(SpeedoStaticTD[0], 1);
    TextDrawColor(SpeedoStaticTD[0], 150);
    TextDrawSetShadow(SpeedoStaticTD[0], 0);
    TextDrawSetOutline(SpeedoStaticTD[0], 0);
    TextDrawBackgroundColor(SpeedoStaticTD[0], 255);
    TextDrawFont(SpeedoStaticTD[0], 4);
    TextDrawSetProportional(SpeedoStaticTD[0], 1);

    // [1] left top decoration
    SpeedoStaticTD[1] = TextDrawCreate(146.000, 359.000, "LD_BUM:blkdot");
    TextDrawTextSize(SpeedoStaticTD[1], 15.000, 15.000);
    TextDrawAlignment(SpeedoStaticTD[1], 1);
    TextDrawColor(SpeedoStaticTD[1], -1732209921);
    TextDrawSetShadow(SpeedoStaticTD[1], 0);
    TextDrawSetOutline(SpeedoStaticTD[1], 0);
    TextDrawBackgroundColor(SpeedoStaticTD[1], 255);
    TextDrawFont(SpeedoStaticTD[1], 4);
    TextDrawSetProportional(SpeedoStaticTD[1], 1);

    // [2] right vertical line
    SpeedoStaticTD[2] = TextDrawCreate(198.000, 359.000, "LD_BUM:blkdot");
    TextDrawTextSize(SpeedoStaticTD[2], 4.000, 38.000);
    TextDrawAlignment(SpeedoStaticTD[2], 1);
    TextDrawColor(SpeedoStaticTD[2], 255);
    TextDrawSetShadow(SpeedoStaticTD[2], 0);
    TextDrawSetOutline(SpeedoStaticTD[2], 0);
    TextDrawBackgroundColor(SpeedoStaticTD[2], 255);
    TextDrawFont(SpeedoStaticTD[2], 4);
    TextDrawSetProportional(SpeedoStaticTD[2], 1);

    // [3]-[8] decorations (was [4]-[9])
    SpeedoStaticTD[3] = TextDrawCreate(198.000, 358.000, "LD_BUM:blkdot");
    TextDrawTextSize(SpeedoStaticTD[3], 5.000, -2.000);
    TextDrawAlignment(SpeedoStaticTD[3], 1);
    TextDrawColor(SpeedoStaticTD[3], -1);
    TextDrawSetShadow(SpeedoStaticTD[3], 0);
    TextDrawSetOutline(SpeedoStaticTD[3], 0);
    TextDrawBackgroundColor(SpeedoStaticTD[3], 255);
    TextDrawFont(SpeedoStaticTD[3], 4);
    TextDrawSetProportional(SpeedoStaticTD[3], 1);

    SpeedoStaticTD[4] = TextDrawCreate(198.000, 358.000, "LD_BUM:blkdot");
    TextDrawTextSize(SpeedoStaticTD[4], 1.000, -9.000);
    TextDrawAlignment(SpeedoStaticTD[4], 1);
    TextDrawColor(SpeedoStaticTD[4], -1);
    TextDrawSetShadow(SpeedoStaticTD[4], 0);
    TextDrawSetOutline(SpeedoStaticTD[4], 0);
    TextDrawBackgroundColor(SpeedoStaticTD[4], 255);
    TextDrawFont(SpeedoStaticTD[4], 4);
    TextDrawSetProportional(SpeedoStaticTD[4], 1);

    SpeedoStaticTD[5] = TextDrawCreate(201.000, 358.000, "LD_BUM:blkdot");
    TextDrawTextSize(SpeedoStaticTD[5], 1.000, -9.000);
    TextDrawAlignment(SpeedoStaticTD[5], 1);
    TextDrawColor(SpeedoStaticTD[5], -1);
    TextDrawSetShadow(SpeedoStaticTD[5], 0);
    TextDrawSetOutline(SpeedoStaticTD[5], 0);
    TextDrawBackgroundColor(SpeedoStaticTD[5], 255);
    TextDrawFont(SpeedoStaticTD[5], 4);
    TextDrawSetProportional(SpeedoStaticTD[5], 1);

    SpeedoStaticTD[6] = TextDrawCreate(199.000, 353.000, "LD_BUM:blkdot");
    TextDrawTextSize(SpeedoStaticTD[6], 3.000, 5.000);
    TextDrawAlignment(SpeedoStaticTD[6], 1);
    TextDrawColor(SpeedoStaticTD[6], -1);
    TextDrawSetShadow(SpeedoStaticTD[6], 0);
    TextDrawSetOutline(SpeedoStaticTD[6], 0);
    TextDrawBackgroundColor(SpeedoStaticTD[6], 255);
    TextDrawFont(SpeedoStaticTD[6], 4);
    TextDrawSetProportional(SpeedoStaticTD[6], 1);

    SpeedoStaticTD[7] = TextDrawCreate(198.000, 350.000, "LD_BUM:blkdot");
    TextDrawTextSize(SpeedoStaticTD[7], 4.000, -1.000);
    TextDrawAlignment(SpeedoStaticTD[7], 1);
    TextDrawColor(SpeedoStaticTD[7], -1);
    TextDrawSetShadow(SpeedoStaticTD[7], 0);
    TextDrawSetOutline(SpeedoStaticTD[7], 0);
    TextDrawBackgroundColor(SpeedoStaticTD[7], 255);
    TextDrawFont(SpeedoStaticTD[7], 4);
    TextDrawSetProportional(SpeedoStaticTD[7], 1);

    SpeedoStaticTD[8] = TextDrawCreate(201.000, 349.000, ")");
    TextDrawLetterSize(SpeedoStaticTD[8], 0.259, 0.597);
    TextDrawAlignment(SpeedoStaticTD[8], 1);
    TextDrawColor(SpeedoStaticTD[8], -1);
    TextDrawSetShadow(SpeedoStaticTD[8], 0);
    TextDrawSetOutline(SpeedoStaticTD[8], 0);
    TextDrawBackgroundColor(SpeedoStaticTD[8], 150);
    TextDrawFont(SpeedoStaticTD[8], 0);
    TextDrawSetProportional(SpeedoStaticTD[8], 1);

    // [9]-[33]: speed bar segments (was [10]-[34]), X = 146+i*2
    for(new i = 0; i < 25; i++)
    {
        SpeedoStaticTD[9 + i] = TextDrawCreate(146.0 + i * 2.0, 377.000, "LD_BUM:blkdot");
        TextDrawTextSize(SpeedoStaticTD[9 + i], 2.000, 20.000);
        TextDrawAlignment(SpeedoStaticTD[9 + i], 1);
        TextDrawColor(SpeedoStaticTD[9 + i], g_SpeedBarColors[i]);
        TextDrawSetShadow(SpeedoStaticTD[9 + i], 0);
        TextDrawSetOutline(SpeedoStaticTD[9 + i], 0);
        TextDrawBackgroundColor(SpeedoStaticTD[9 + i], 255);
        TextDrawFont(SpeedoStaticTD[9 + i], 4);
        TextDrawSetProportional(SpeedoStaticTD[9 + i], 1);
    }

    // [34]: "KM/H" label (was [37])
    SpeedoStaticTD[34] = TextDrawCreate(186.000, 356.000, "KM/H");
    TextDrawLetterSize(SpeedoStaticTD[34], 0.100, 0.898);
    TextDrawAlignment(SpeedoStaticTD[34], 2);
    TextDrawColor(SpeedoStaticTD[34], -1);
    TextDrawSetShadow(SpeedoStaticTD[34], 0);
    TextDrawSetOutline(SpeedoStaticTD[34], 0);
    TextDrawBackgroundColor(SpeedoStaticTD[34], 150);
    TextDrawFont(SpeedoStaticTD[34], 2);
    TextDrawSetProportional(SpeedoStaticTD[34], 1);

    // [35]-[45]: bottom decorations (was [38]-[48])
    SpeedoStaticTD[35] = TextDrawCreate(146.000, 405.000, "LD_BUM:blkdot");
    TextDrawTextSize(SpeedoStaticTD[35], 8.000, 6.000);
    TextDrawAlignment(SpeedoStaticTD[35], 1);
    TextDrawColor(SpeedoStaticTD[35], -1);
    TextDrawSetShadow(SpeedoStaticTD[35], 0);
    TextDrawSetOutline(SpeedoStaticTD[35], 0);
    TextDrawBackgroundColor(SpeedoStaticTD[35], 255);
    TextDrawFont(SpeedoStaticTD[35], 4);
    TextDrawSetProportional(SpeedoStaticTD[35], 1);

    SpeedoStaticTD[36] = TextDrawCreate(146.000, 399.000, "/");
    TextDrawLetterSize(SpeedoStaticTD[36], 0.160, 0.699);
    TextDrawAlignment(SpeedoStaticTD[36], 1);
    TextDrawColor(SpeedoStaticTD[36], -1);
    TextDrawSetShadow(SpeedoStaticTD[36], 0);
    TextDrawSetOutline(SpeedoStaticTD[36], 0);
    TextDrawBackgroundColor(SpeedoStaticTD[36], 150);
    TextDrawFont(SpeedoStaticTD[36], 1);
    TextDrawSetProportional(SpeedoStaticTD[36], 1);

    SpeedoStaticTD[37] = TextDrawCreate(154.000, 399.000, "/");
    TextDrawLetterSize(SpeedoStaticTD[37], -0.169, 0.699);
    TextDrawAlignment(SpeedoStaticTD[37], 1);
    TextDrawColor(SpeedoStaticTD[37], -1);
    TextDrawSetShadow(SpeedoStaticTD[37], 0);
    TextDrawSetOutline(SpeedoStaticTD[37], 0);
    TextDrawBackgroundColor(SpeedoStaticTD[37], 150);
    TextDrawFont(SpeedoStaticTD[37], 1);
    TextDrawSetProportional(SpeedoStaticTD[37], 1);

    SpeedoStaticTD[38] = TextDrawCreate(148.000, 400.000, "LD_BUM:blkdot");
    TextDrawTextSize(SpeedoStaticTD[38], 4.000, 2.000);
    TextDrawAlignment(SpeedoStaticTD[38], 1);
    TextDrawColor(SpeedoStaticTD[38], -1);
    TextDrawSetShadow(SpeedoStaticTD[38], 0);
    TextDrawSetOutline(SpeedoStaticTD[38], 0);
    TextDrawBackgroundColor(SpeedoStaticTD[38], 255);
    TextDrawFont(SpeedoStaticTD[38], 4);
    TextDrawSetProportional(SpeedoStaticTD[38], 1);

    SpeedoStaticTD[39] = TextDrawCreate(153.000, 404.000, "LD_BUM:blkdot");
    TextDrawTextSize(SpeedoStaticTD[39], 3.000, 3.000);
    TextDrawAlignment(SpeedoStaticTD[39], 1);
    TextDrawColor(SpeedoStaticTD[39], -1);
    TextDrawSetShadow(SpeedoStaticTD[39], 0);
    TextDrawSetOutline(SpeedoStaticTD[39], 0);
    TextDrawBackgroundColor(SpeedoStaticTD[39], 255);
    TextDrawFont(SpeedoStaticTD[39], 4);
    TextDrawSetProportional(SpeedoStaticTD[39], 1);

    SpeedoStaticTD[40] = TextDrawCreate(144.000, 404.000, "LD_BUM:blkdot");
    TextDrawTextSize(SpeedoStaticTD[40], 3.000, 3.000);
    TextDrawAlignment(SpeedoStaticTD[40], 1);
    TextDrawColor(SpeedoStaticTD[40], -1);
    TextDrawSetShadow(SpeedoStaticTD[40], 0);
    TextDrawSetOutline(SpeedoStaticTD[40], 0);
    TextDrawBackgroundColor(SpeedoStaticTD[40], 255);
    TextDrawFont(SpeedoStaticTD[40], 4);
    TextDrawSetProportional(SpeedoStaticTD[40], 1);

    SpeedoStaticTD[41] = TextDrawCreate(147.000, 411.000, "LD_BUM:blkdot");
    TextDrawTextSize(SpeedoStaticTD[41], 2.000, 2.000);
    TextDrawAlignment(SpeedoStaticTD[41], 1);
    TextDrawColor(SpeedoStaticTD[41], -1);
    TextDrawSetShadow(SpeedoStaticTD[41], 0);
    TextDrawSetOutline(SpeedoStaticTD[41], 0);
    TextDrawBackgroundColor(SpeedoStaticTD[41], 255);
    TextDrawFont(SpeedoStaticTD[41], 4);
    TextDrawSetProportional(SpeedoStaticTD[41], 1);

    SpeedoStaticTD[42] = TextDrawCreate(151.000, 411.000, "LD_BUM:blkdot");
    TextDrawTextSize(SpeedoStaticTD[42], 2.000, 2.000);
    TextDrawAlignment(SpeedoStaticTD[42], 1);
    TextDrawColor(SpeedoStaticTD[42], -1);
    TextDrawSetShadow(SpeedoStaticTD[42], 0);
    TextDrawSetOutline(SpeedoStaticTD[42], 0);
    TextDrawBackgroundColor(SpeedoStaticTD[42], 255);
    TextDrawFont(SpeedoStaticTD[42], 4);
    TextDrawSetProportional(SpeedoStaticTD[42], 1);

    SpeedoStaticTD[43] = TextDrawCreate(151.000, 408.000, "LD_BUM:blkdot");
    TextDrawTextSize(SpeedoStaticTD[43], 2.000, 2.000);
    TextDrawAlignment(SpeedoStaticTD[43], 1);
    TextDrawColor(SpeedoStaticTD[43], -1732209921);
    TextDrawSetShadow(SpeedoStaticTD[43], 0);
    TextDrawSetOutline(SpeedoStaticTD[43], 0);
    TextDrawBackgroundColor(SpeedoStaticTD[43], 255);
    TextDrawFont(SpeedoStaticTD[43], 4);
    TextDrawSetProportional(SpeedoStaticTD[43], 1);

    SpeedoStaticTD[44] = TextDrawCreate(147.000, 408.000, "LD_BUM:blkdot");
    TextDrawTextSize(SpeedoStaticTD[44], 2.000, 2.000);
    TextDrawAlignment(SpeedoStaticTD[44], 1);
    TextDrawColor(SpeedoStaticTD[44], -1732209921);
    TextDrawSetShadow(SpeedoStaticTD[44], 0);
    TextDrawSetOutline(SpeedoStaticTD[44], 0);
    TextDrawBackgroundColor(SpeedoStaticTD[44], 255);
    TextDrawFont(SpeedoStaticTD[44], 4);
    TextDrawSetProportional(SpeedoStaticTD[44], 1);

    SpeedoStaticTD[45] = TextDrawCreate(157.000, 404.000, "LD_BUM:blkdot");
    TextDrawTextSize(SpeedoStaticTD[45], 39.000, 5.000);
    TextDrawAlignment(SpeedoStaticTD[45], 1);
    TextDrawColor(SpeedoStaticTD[45], 255);
    TextDrawSetShadow(SpeedoStaticTD[45], 0);
    TextDrawSetOutline(SpeedoStaticTD[45], 0);
    TextDrawBackgroundColor(SpeedoStaticTD[45], 255);
    TextDrawFont(SpeedoStaticTD[45], 4);
    TextDrawSetProportional(SpeedoStaticTD[45], 1);
}

stock CreateSpeedoTD(playerid)
{
    // [0] fuel bar (dynamic: TextSize changes with fuel level)
    SpeedoDrpTD[playerid][0] = CreatePlayerTextDraw(playerid, 198.898, 396.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][0], 1.500, -35.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][0], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][0], -1);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][0], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][0], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][0], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][0], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][0], 1);

    // [1] compass direction (dynamic: string changes per player heading)
    SpeedoDrpTD[playerid][1] = CreatePlayerTextDraw(playerid, 153.000, 362.000, "NW");
    PlayerTextDrawLetterSize(playerid, SpeedoDrpTD[playerid][1], 0.200, 1.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][1], 2);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][1], -1);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][1], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][1], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][1], 150);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][1], 1);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][1], 1);

    // [2] speed number (dynamic: string changes per player speed)
    SpeedoDrpTD[playerid][2] = CreatePlayerTextDraw(playerid, 174.000, 364.000, "250");
    PlayerTextDrawLetterSize(playerid, SpeedoDrpTD[playerid][2], 0.200, 1.200);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][2], 2);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][2], -1);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][2], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][2], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][2], 150);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][2], 2);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][2], 1);

    // [3] health bar (dynamic: TextSize changes with vehicle health)
    SpeedoDrpTD[playerid][3] = CreatePlayerTextDraw(playerid, 158.898, 407.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][3], 35.500, -2.000);
    PlayerTextDrawAlignment(playerid, SpeedoDrpTD[playerid][3], 1);
    PlayerTextDrawColor(playerid, SpeedoDrpTD[playerid][3], -1);
    PlayerTextDrawSetShadow(playerid, SpeedoDrpTD[playerid][3], 0);
    PlayerTextDrawSetOutline(playerid, SpeedoDrpTD[playerid][3], 0);
    PlayerTextDrawBackgroundColor(playerid, SpeedoDrpTD[playerid][3], 255);
    PlayerTextDrawFont(playerid, SpeedoDrpTD[playerid][3], 4);
    PlayerTextDrawSetProportional(playerid, SpeedoDrpTD[playerid][3], 1);
}

hook OnPlayerConnect(playerid)
{
    for(new i = 0; i < 4; i++)
        SpeedoDrpTD[playerid][i] = INVALID_PLAYER_TEXT_DRAW;
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

hook OnGameModeInit()
{
    CreateSpeedoStaticTD();
    return 1;
}

ShowSpeedoTD(playerid)
{
    // Show background/decoration TDs [0]-[8] and [34]-[45]; speed bar [9]-[33] driven by OnPlayerUpdate
    for(new i = 0; i < 9; i++)
        TextDrawShowForPlayer(playerid, SpeedoStaticTD[i]);
    for(new i = 34; i < 46; i++)
        TextDrawShowForPlayer(playerid, SpeedoStaticTD[i]);
    for(new i = 0; i < 4; i++)
        PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][i]);
    return 1;
}

HideSpeedoTD(playerid)
{
    for(new i = 0; i < 46; i++)
        TextDrawHideForPlayer(playerid, SpeedoStaticTD[i]);
    for(new i = 0; i < 4; i++)
        PlayerTextDrawHide(playerid, SpeedoDrpTD[playerid][i]);
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
    PlayerTextDrawSetString(playerid, SpeedoDrpTD[playerid][1], sprintf("%s", frmtcompass));
    return 1;
}


hook OnPlayerUpdate(playerid)
{
    if(gPlayerLogged{playerid} != 1)
        return 0;

    new vehid = GetPlayerVehicleID(playerid);
    if(IsValidVehicle(vehid))
    {
        if(IsPlayerInAnyVehicle(playerid) && (GetPlayerState(playerid) == PLAYER_STATE_DRIVER || GetPlayerState(playerid) == PLAYER_STATE_PASSENGER))
        {
            new Float:fFuel, Float:fDamage;
            GetVehicleHealth(vehid, fDamage);

            fFuel = VehicleFuel[vehid];

            if(fFuel < 0) fFuel = 0;
            else if(fFuel > 100) fFuel = 100;

            new vspeed = GetVehicleSpeed(GetPlayerVehicleID(playerid));
            new color[4];
            switch (vspeed) {
                case 0..50:  color = "~w~";
                case 51..70: color = "~g~";
                case 71..90: color = "~y~";
                case 91..100: color = "~p~";
                default: color = "~r~";
            }
            new speed[16];
            format(speed, sizeof(speed), "%s%d", color, vspeed);
            PlayerTextDrawSetString(playerid, SpeedoDrpTD[playerid][2], speed);

            new Float:FuelBar;
            FuelBar = fFuel * -35.0/100;
            PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][0], 1.5, FuelBar);
            PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][0]);

            new Float:HealthBar;
            HealthBar = fDamage * 35.5/1000;
            PlayerTextDrawTextSize(playerid, SpeedoDrpTD[playerid][3], HealthBar, -2.0);
            PlayerTextDrawShow(playerid, SpeedoDrpTD[playerid][3]);

            // Speed bar segments [9]-[33] in SpeedoStaticTD
            if(vspeed > 1)  TextDrawShowForPlayer(playerid, SpeedoStaticTD[9]);  else TextDrawHideForPlayer(playerid, SpeedoStaticTD[9]);
            if(vspeed > 6)  TextDrawShowForPlayer(playerid, SpeedoStaticTD[10]); else TextDrawHideForPlayer(playerid, SpeedoStaticTD[10]);
            if(vspeed > 10) TextDrawShowForPlayer(playerid, SpeedoStaticTD[11]); else TextDrawHideForPlayer(playerid, SpeedoStaticTD[11]);
            if(vspeed > 14) TextDrawShowForPlayer(playerid, SpeedoStaticTD[12]); else TextDrawHideForPlayer(playerid, SpeedoStaticTD[12]);
            if(vspeed > 17) TextDrawShowForPlayer(playerid, SpeedoStaticTD[13]); else TextDrawHideForPlayer(playerid, SpeedoStaticTD[13]);
            if(vspeed > 20) TextDrawShowForPlayer(playerid, SpeedoStaticTD[14]); else TextDrawHideForPlayer(playerid, SpeedoStaticTD[14]);
            if(vspeed > 24) TextDrawShowForPlayer(playerid, SpeedoStaticTD[15]); else TextDrawHideForPlayer(playerid, SpeedoStaticTD[15]);
            if(vspeed > 28) TextDrawShowForPlayer(playerid, SpeedoStaticTD[16]); else TextDrawHideForPlayer(playerid, SpeedoStaticTD[16]);
            if(vspeed > 32) TextDrawShowForPlayer(playerid, SpeedoStaticTD[17]); else TextDrawHideForPlayer(playerid, SpeedoStaticTD[17]);
            if(vspeed > 36) TextDrawShowForPlayer(playerid, SpeedoStaticTD[18]); else TextDrawHideForPlayer(playerid, SpeedoStaticTD[18]);
            if(vspeed > 40) TextDrawShowForPlayer(playerid, SpeedoStaticTD[19]); else TextDrawHideForPlayer(playerid, SpeedoStaticTD[19]);
            if(vspeed > 44) TextDrawShowForPlayer(playerid, SpeedoStaticTD[20]); else TextDrawHideForPlayer(playerid, SpeedoStaticTD[20]);
            if(vspeed > 48) TextDrawShowForPlayer(playerid, SpeedoStaticTD[21]); else TextDrawHideForPlayer(playerid, SpeedoStaticTD[21]);
            if(vspeed > 52) TextDrawShowForPlayer(playerid, SpeedoStaticTD[22]); else TextDrawHideForPlayer(playerid, SpeedoStaticTD[22]);
            if(vspeed > 56) TextDrawShowForPlayer(playerid, SpeedoStaticTD[23]); else TextDrawHideForPlayer(playerid, SpeedoStaticTD[23]);
            if(vspeed > 60) TextDrawShowForPlayer(playerid, SpeedoStaticTD[24]); else TextDrawHideForPlayer(playerid, SpeedoStaticTD[24]);
            if(vspeed > 64) TextDrawShowForPlayer(playerid, SpeedoStaticTD[25]); else TextDrawHideForPlayer(playerid, SpeedoStaticTD[25]);
            if(vspeed > 68) TextDrawShowForPlayer(playerid, SpeedoStaticTD[26]); else TextDrawHideForPlayer(playerid, SpeedoStaticTD[26]);
            if(vspeed > 72) TextDrawShowForPlayer(playerid, SpeedoStaticTD[27]); else TextDrawHideForPlayer(playerid, SpeedoStaticTD[27]);
            if(vspeed > 76) TextDrawShowForPlayer(playerid, SpeedoStaticTD[28]); else TextDrawHideForPlayer(playerid, SpeedoStaticTD[28]);
            if(vspeed > 80) TextDrawShowForPlayer(playerid, SpeedoStaticTD[29]); else TextDrawHideForPlayer(playerid, SpeedoStaticTD[29]);
            if(vspeed > 84) TextDrawShowForPlayer(playerid, SpeedoStaticTD[30]); else TextDrawHideForPlayer(playerid, SpeedoStaticTD[30]);
            if(vspeed > 87) TextDrawShowForPlayer(playerid, SpeedoStaticTD[31]); else TextDrawHideForPlayer(playerid, SpeedoStaticTD[31]);
            if(vspeed > 89) TextDrawShowForPlayer(playerid, SpeedoStaticTD[32]); else TextDrawHideForPlayer(playerid, SpeedoStaticTD[32]);
            if(vspeed > 90) TextDrawShowForPlayer(playerid, SpeedoStaticTD[33]); else TextDrawHideForPlayer(playerid, SpeedoStaticTD[33]);

            PlayerCompassUpdate(playerid, vehid);
        }
    }
    return 1;
}

CMD:speedo(playerid, params[])
{
    if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER && GetPlayerState(playerid) != PLAYER_STATE_PASSENGER)
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Ban khong o tren xe.");
    }
    else if (!PlayerInfo[playerid][pSpeedo])
    {
        SendClientMessageEx(playerid, COLOR_WHITE, "Ban da bat dong ho toc do.");
        PlayerInfo[playerid][pSpeedo] = 1;
        ShowSpeedoTD(playerid);
    }
    else
    {
        SendClientMessageEx(playerid, COLOR_WHITE, "Ban da vo hieu hoa dong ho toc do.");
        PlayerInfo[playerid][pSpeedo] = 0;
        HideSpeedoTD(playerid);
    }

    return 1;
}
