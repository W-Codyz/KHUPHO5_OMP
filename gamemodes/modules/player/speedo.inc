stock ShowVehicleHUDForPlayer(playerid)
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
}