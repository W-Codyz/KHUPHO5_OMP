#include <YSI/YSI_Coding/y_hooks>

	//-------------------------------------------------------------------
	//					Project create by #nDP 						   //
	//					Fb.com/nguyenduyphuong.com  			       //
	//					Copyright by nDP" 							   //
	//-------------------------------------------------------------------

//____________________________________________________________________________//

new gAntiSpeedEnabled = 1;
new countHackSpeedAmount[MAX_PLAYERS];
new VehicleSpeed[212 + 1] =
{
	160, 160, 200, 120, 150, 165, 110, 170, 110, 180, 160, 240, 160, 160, 140,
	230, 155, 200, 150, 160, 180, 180, 165, 145, 170, 200, 200, 170, 170, 200,
	190, 130,  80, 180, 200, 120, 160, 160, 160, 160, 160,  75, 150, 150, 110,
	165, 280, 200, 190, 150, 120, 240, 190, 190, 190, 140, 160, 160, 165, 160,
	200, 190, 190, 190,  75,  75, 160, 160, 190, 200, 170, 160, 190, 190, 160,
	160, 200, 200, 150, 165, 200, 120, 150, 120, 190, 160, 100, 200, 200, 170,
	170, 160, 160, 190, 220, 170, 200, 200, 140, 140, 160,  75, 220, 220, 160,
	170, 230, 165, 140, 120, 140, 200, 200, 200, 120, 120, 165, 165, 160, 330,
	330, 190, 190, 190, 110, 160, 160, 160, 170, 160,  60,  70, 140, 200, 160,
	160, 160, 110, 110, 150, 160, 230, 160, 165, 170, 160, 160, 160, 200, 160,
	160, 165, 160, 200, 170, 180, 110, 110, 200, 200, 200, 200, 200, 200,  75,
	200, 160, 160, 170, 110, 110,  90,  60, 110,  60, 160, 160, 200, 110, 160,
	165, 190, 160, 170, 120, 165, 190, 200, 140, 200, 110, 120, 200, 200,  60,
	190, 200, 200, 200, 160, 165, 110, 200, 200, 160, 165, 160, 160, 160, 140,
	160, 160
};
Float:GetVehicleMaxSpeed(modelid)
{
    if (modelid < 400 || modelid > 611) return 9999.0;
    return float(VehicleSpeed[modelid - 400]);
}

CMD:antispeed(playerid, params[]) {
	if(PlayerInfo[playerid][pAdmin] < 9999) return 1;
	if(gAntiSpeedEnabled == 0) {
		gAntiSpeedEnabled = 1;
		SendClientMessageEx(playerid, -1, "{ff0000}[!]{FFFFFF} Ban da bat he thong kiem tra gian lan Hack (Speed)");
	}
	else {
		gAntiSpeedEnabled = 0;
		SendClientMessageEx(playerid, -1, "{ff0000}[!]{FFFFFF} Ban da tat he thong kiem tra gian lan Hack (Speed)");
	}
	return 1;
}

public OnPlayerSpeedCheat(playerid, speedtype) 
{
	if(gAntiSpeedEnabled == 0) return 1;
	if(PlayerInfo[playerid][pAdmin] >= 2) return 1; 

	new string[1280];
	if(speedtype == 0) {
		new vehicleid = GetClosestCar(playerid, INVALID_VEHICLE_ID, 15.0);
		if(vehicleid == INVALID_VEHICLE_ID) {
			format(string, sizeof(string), "[ANTI-CHEAT] %s vua su dung %s (Speed : %d | Type : %d), vui long (/spec %d).", GetPlayerNameEx(playerid), "Hack Speed", GetSpeedPlayer(playerid), speedtype, playerid);
		}
		else return 1;
	}
	else {
		format(string, sizeof(string), "[ANTI-CHEAT] %s vua su dung %s (Speed : %d | Type : %d), vui long (/spec %d).", GetPlayerNameEx(playerid), "Hack Speed", GetSpeedKM(playerid), speedtype, playerid);
	}
	ABroadCast(COLOR_YELLOW, string, 2);

	if(AdminOnl() == 0) Kick(playerid);
	return 1;
}

hook OnPlayerConnect(playerid) {
	countHackSpeedAmount[playerid] = 0;
	return 1;
}

hook OnPlayerDisconnect(playerid, reason) {
	countHackSpeedAmount[playerid] = 0;
	return 1;
}
forward checkSpeedTPhat(playerid);
public checkSpeedTPhat(playerid)
{
	if (GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		new Float: velocity_x,
			Float: velocity_y,
			Float: velocity_z;

		GetVehicleVelocity(GetPlayerVehicleID(playerid), velocity_x, velocity_y, velocity_z);

		if(floatround(VectorSize(velocity_x, velocity_y, 0.0) * 100.0) >= GetVehicleMaxSpeed(GetVehicleModel(GetPlayerVehicleID(playerid))))
			SetVehicleVelocity(GetPlayerVehicleID(playerid), velocity_x*0.97, velocity_y*0.97, velocity_z*0.97);
	}
	return 1;
}
forward checkSpeed_2(playerid);
public checkSpeed_2(playerid) {
	/* if(GetSpeedPlayer(playerid) > 10) { 
		new string[1280];
		format(string, sizeof(string), "[ANTI-CHEAT] %s vua su dung %s (Speed : %d), vui long (/spec %d).", GetPlayerNameEx(playerid), "Hack Slap", GetSpeedPlayer(playerid), playerid);
		ABroadCast(COLOR_YELLOW, string, 2);
	} */

	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) 
	{
		if(GetSpeedKM(playerid) >= 21 && GetSpeedKM(playerid) <= 25 && GetPVarInt(playerid, "NotAndroid") != 0 && PlayerInfo[playerid][pHospital] == 0 && GetPlayerVirtualWorld(playerid) == 0 && GetPlayerInterior(playerid) == 0)
		{
			countHackSpeedAmount[playerid] += 1;
			if(countHackSpeedAmount[playerid] == 3) {

				new string[1280];
				format(string, sizeof(string), "[ANTI-CHEAT] %s vua su dung %s (Speed : %d), vui long (/spec %d).", GetPlayerNameEx(playerid), "Hack Speed Car", GetSpeedKM(playerid), playerid);
				ABroadCast(COLOR_YELLOW, string, 2);

				countHackSpeedAmount[playerid] = 0;

				if(AdminOnl() == 0) Kick(playerid);
			}
		}
		else {
			countHackSpeedAmount[playerid] = 0;
		}
	}

	if(GetSpeedPlayer(playerid) > 8000 && PlayerInfo[playerid][pHospital] == 0 && GetPlayerVirtualWorld(playerid) == 0 && GetPlayerInterior(playerid) == 0)
    {
    	new string[1280];
		format(string, sizeof(string), "[ANTI-CHEAT] %s da bi kick ra khoi may chu do su dung %s (Speed : %d), vui long (/spec %d).", GetPlayerNameEx(playerid), "Hack Speed", GetSpeedPlayer(playerid), playerid);
		ABroadCast(COLOR_YELLOW, string, 2);
		Log("logs/AntiHackBynDP.log", string);
		Kick(playerid);
		return 1;
    }

	if(GetSpeedPlayer(playerid) > 3000 && PlayerInfo[playerid][pHospital] == 0 && GetPlayerVirtualWorld(playerid) == 0 && GetPlayerInterior(playerid) == 0)
    {
    	new closestcar = GetClosestCar(playerid);
		if(!IsPlayerInRangeOfVehicle(playerid, closestcar, 40.0)) {
	    	if(GetPVarInt(playerid, "TB_AntiSlap2") == 0)
	    	{
				new string[1280];
				format(string, sizeof(string), "[ANTI-CHEAT] %s vua su dung %s (Speed : %d), vui long (/spec %d).", GetPlayerNameEx(playerid), "Hack Slap", GetSpeedPlayer(playerid), playerid);
				ABroadCast(COLOR_YELLOW, string, 2);
				Log("logs/AntiHackBynDP.log", string);

				SetPVarInt(playerid, "TB_AntiSlap2", 1);
				SetTimerEx("deleteTB_AntiSlap2", 3000, false, "i", playerid);
			}

			if(AdminOnl() == 0) Kick(playerid);
		}
    }
}

/*
forward checkPrison(playerid);
public checkPrison(playerid) {
	if(PlayerInfo[playerid][pJailTime] > 0 && gPlayerLogged{playerid} != 0 && PlayerInfo[playerid][pAdmin] == 0) {
		if(!IsPlayerInRangeOfPoint(playerid, 500.0, -3379.4836,423.2281,4.8390)) {
			SetPlayerVirtualWorld(playerid, 304);
			ResetPlayerWeaponsEx(playerid);
			PlayerInfo[playerid][pWantedLevel] = 0;
			SetPlayerWantedLevel(playerid, 0);
			PhoneOnline[playerid] = 1;
			new rand = random(sizeof(OOCPrisonSpawns));
			ac_Streamer_UpdateEx(playerid, OOCPrisonSpawns[rand][0], OOCPrisonSpawns[rand][1], OOCPrisonSpawns[rand][2]);
			ac_SetPlayerPos(playerid, OOCPrisonSpawns[rand][0], OOCPrisonSpawns[rand][1], OOCPrisonSpawns[rand][2]);
			SetPlayerSkin(playerid, 50);
			SetPlayerColor(playerid, TEAM_APRISON_COLOR);
			Player_StreamPrep(playerid, OOCPrisonSpawns[rand][0], OOCPrisonSpawns[rand][1], OOCPrisonSpawns[rand][2], 5000);
		}
	}
	return 1;
}*/

forward deleteTB_AntiSlap(playerid);
public deleteTB_AntiSlap(playerid) {
	DeletePVar(playerid, "TB_AntiSlap");
	return 1;
}

forward deleteTB_AntiSlap2(playerid);
public deleteTB_AntiSlap2(playerid) {
	DeletePVar(playerid, "TB_AntiSlap2");
	return 1;
}

forward deleteTB_AntiSpeed(playerid);
public deleteTB_AntiSpeed(playerid) {
	DeletePVar(playerid, "TB_AntiSpeed");
	return 1;
}

// ------------------------------------------------------------------------

forward checkSpeedVehicle(playerid);
public checkSpeedVehicle(playerid) {
	new newcar = GetPlayerVehicleID(playerid);
    if(IsPlayerInAnyVehicle(playerid) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER && !IsAPlane(newcar) && !IsABoat(newcar) && PlayerInfo[playerid][pAdmin] == 0)
	{
        if(float(GetSpeedKM(playerid)) > GetVehicleMaxSpeed(GetVehicleModel(newcar)) + 5.0)
        {
        	new string[1280];
        	format(string, sizeof(string), "[ANTI-CHEAT] %s vua su dung %s, vui long (/spec %d).", GetPlayerNameEx(playerid), "Speed Car", playerid);
        	if(GetPVarInt(playerid, "TB_AntiSpeedCar") == 0) {
				ABroadCast(COLOR_YELLOW, string, 2);
				SetPVarInt(playerid, "TB_AntiSpeedCar", 1);
				SetTimerEx("deleteTB_AntiSpeedCar", 3000, false, "i", playerid);
			}
			Log("logs/AntiHackBynDP.log", string);
			if(AdminOnl() == 0) Kick(playerid);
        }
    }
}

forward deleteTB_AntiSpeedCar(playerid);
public deleteTB_AntiSpeedCar(playerid) {
	DeletePVar(playerid, "TB_AntiSpeedCar");
	return 1;
} 
