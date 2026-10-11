/*
	Mechanic System
	Group type 8 = Mechanic (formerly Towman)
	Commands: /tow, /untow, /sualuaxe
*/

// Unique XY positions of Pay N' Spray blocker objects (from system/remove/garage.inc)
// Used by timers.inc anti-repair check — included before timers so accessible globally
new Float:g_PayNSprayPos[17][3] = {
    {1968.74219,  2162.49219,  12.09380},
    {2006.00000,  2303.72656,  11.31250},
    { 720.01563,  -462.52341,  16.85940},
    {-100.00000,  1111.41406,  21.14060},
    {-1420.52686, 2591.15747,  57.24220},
    {1843.36719,  -1856.32031, 13.87500},
    {-2716.35156,  217.47659,   5.38280},
    {-1935.85938,  239.53130,  35.35160},
    {-1904.53125,  277.89841,  42.45310},
    {-1786.81250, 1209.42188,  25.83590},
    {1798.68750,  -2146.73438, 14.00000},
    {2644.85938,  -2039.23438, 14.03910},
    {2071.47656,  -1831.42188, 14.56250},
    { 488.28131,  -1734.69531, 12.39060},
    {-2425.72656, 1027.99219,  51.78130},
    {2393.76563,   1483.68750, 12.21090},
    {2386.65625,   1043.60156, 11.59380}
};

CMD:tow(playerid, params[]) {
 	if(IsACop(playerid) || IsAMechanic(playerid)) {
   		if(IsPlayerInAnyVehicle(playerid))
     	{
			new
				carid = GetPlayerVehicleID(playerid);

			if(IsATowTruck(carid))
			{
   				new
					closestcar = GetClosestCar(playerid, carid);

				foreach(new i: Player)
				{
					if(arr_Towing[i] == closestcar || (GetPlayerVehicleID(i) == closestcar && GetPlayerState(i) == 2)) return SendClientMessageEx(playerid, COLOR_GREY, "Ban dang keo xe khac, ban khong co quyen keo them xe.");
				}

       			if(GetDistanceToCar(playerid,closestcar) <= 8 && !IsTrailerAttachedToVehicle(carid)) {
					foreach(new i: Player)
					{
					    if(IsAPlane(closestcar) || IsABike(closestcar) || IsASpawnedTrain(closestcar) || IsATrain(closestcar) || IsAHelicopter(closestcar)) {
							return SendClientMessageEx(playerid, COLOR_GRAD2, "ban khong theo keo xe nay.");
						}
						if(GetPlayerVehicle(i, closestcar) != -1) {

							new
								hKey;

							if(((hKey = PlayerInfo[i][pPhousekey]) != INVALID_HOUSE_ID) && IsPlayerInRangeOfPoint(playerid, 50.0, HouseInfo[hKey][hExteriorX], HouseInfo[hKey][hExteriorY], HouseInfo[hKey][hExteriorZ])
							||((hKey = PlayerInfo[i][pPhousekey2]) != INVALID_HOUSE_ID) && IsPlayerInRangeOfPoint(playerid, 50.0, HouseInfo[hKey][hExteriorX], HouseInfo[hKey][hExteriorY], HouseInfo[hKey][hExteriorZ])) {
								return SendClientMessageEx(playerid, COLOR_GREY, "Chiec xe nay khong can phai keo.");
							}

							arr_Towing[playerid] = closestcar;
							if(IsAMechanic(playerid))
								SendClientMessageEx(playerid, COLOR_GRAD2, "Xe da san sang de keo di sua chua.");
							else
							    SendClientMessageEx(playerid, COLOR_GRAD2, "Chiec xe da san sang de giam giu.");
      						return AttachTrailerToVehicle(closestcar,carid);
						}
					}
					if(IsAMechanic(playerid))
						SendClientMessageEx(playerid, COLOR_GRAD2, "Xe da san sang de keo di sua chua.");
					else
						SendClientMessageEx(playerid, COLOR_GRAD2, "Chiec xe nay khong co dang ky, no san sang de giam giu.");
					AttachTrailerToVehicle(closestcar,carid);
					arr_Towing[playerid] = closestcar;
					return 1;
     			}
			}
			else if(IsAAircraftTowTruck(carid))
			{
				new
				    closestcar = GetClosestCar(playerid, carid);

                foreach(new i: Player)
				{
					if(arr_Towing[i] == closestcar || (GetPlayerVehicleID(i) == closestcar && GetPlayerState(i) == 2)) return SendClientMessageEx(playerid, COLOR_GREY, "You can't tow a vehicle which is occupied, or in tow.");
				}

				if(GetDistanceToCar(playerid,closestcar) <= 8 && !IsTrailerAttachedToVehicle(carid))
				{
					foreach(new i: Player)
					{
						if(IsAPlane(closestcar))
						{
						    if(GetPlayerVehicle(i, closestcar) != -1)
						    {
						        arr_Towing[playerid] = closestcar;
								if(IsAMechanic(playerid))
									SendClientMessageEx(playerid, COLOR_GRAD2, "Xe da san sang de keo di sua chua.");
								else
								    SendClientMessageEx(playerid, COLOR_GRAD2, "Chiec xe da san sang de giam giu.");
			      				return AttachTrailerToVehicle(closestcar,carid);
			      			}
			      		}
						else return SendClientMessageEx(playerid, COLOR_GRAD2, "Ban chi co the keo may bay bang xe nay!");
			      	}
					if(IsAMechanic(playerid))
						SendClientMessageEx(playerid, COLOR_GRAD2, "Xe da san sang de keo di sua chua.");
					else
						SendClientMessageEx(playerid, COLOR_GRAD2, "Chiec xe nay khong co dang ky, no san sang de giam giu.");
					AttachTrailerToVehicle(closestcar,carid);
					arr_Towing[playerid] = closestcar;
	      		}
	      	}
			else SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong duoc phep keo xe nay.");
   		}
     	else SendClientMessageEx(playerid, COLOR_GRAD2, "ban phai o ben trong xe keo moi co the su dung lenh nay!");
	}
   	else SendUnAuth(playerid);
	return 1;
}

// /sualuaxe - mechanic repairs closest vehicle within 5m
// Sets MechanicRepair PVar on the driver so the anti-cheat timer allows the health increase
CMD:sualuaxe(playerid, params[])
{
	if(!IsAMechanic(playerid)) return SendUnAuth(playerid);
	if(IsPlayerInAnyVehicle(playerid)) return SendClientMessageEx(playerid, COLOR_GREY, "Ban phai ra khoi xe truoc khi sua xe.");

	new closestVeh = GetClosestCar(playerid, INVALID_VEHICLE_ID);
	if(closestVeh == INVALID_VEHICLE_ID || GetDistanceToCar(playerid, closestVeh) > 5.0)
		return SendClientMessageEx(playerid, COLOR_GREY, "Khong co xe nao gan ban trong pham vi 5m.");

	new Float:currentHealth;
	GetVehicleHealth(closestVeh, currentHealth);
	if(currentHealth >= 1000.0)
		return SendClientMessageEx(playerid, COLOR_GREY, "Chiec xe nay khong can sua chua.");

	// Allow the health increase for the driver of the target vehicle
	new driverid = GetVehicleDriver(closestVeh);
	if(driverid != INVALID_PLAYER_ID) SetPVarInt(driverid, "MechanicRepair", 1);

	SetVehicleHealth(closestVeh, 1000.0);
	SendClientMessageEx(playerid, COLOR_GRAD2, "Ban da sua xong chiec xe.");
	new str[80];
	format(str, sizeof(str), "Xe cua ban da duoc tho may [%s] sua chua.", GetPlayerNameEx(playerid));
	if(driverid != INVALID_PLAYER_ID) SendClientMessageEx(driverid, COLOR_GRAD2, str);
	return 1;
}

CMD:untow(playerid, params[])
{
 	if(IsACop(playerid) || IsAMechanic(playerid))
	{
   		if(IsTrailerAttachedToVehicle(GetPlayerVehicleID(playerid)))
     	{
      		SendClientMessageEx(playerid, COLOR_GRAD1,"Ban da tha chiec xe ma ban da keo.");
			arr_Towing[playerid] = INVALID_VEHICLE_ID;
			DetachTrailerFromVehicle(GetPlayerVehicleID(playerid));
     	}
      	else
       	{
        	SendClientMessageEx(playerid, COLOR_GRAD1,"Ban khong keo chiec xe nao.");
        }
  	}
   	else
	{
 		SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong du kha nang su dung lenh nay.");
	}
	return 1;
}
