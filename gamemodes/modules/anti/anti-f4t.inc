#include <YSI\YSI_Coding\y_hooks>
#include "./modules/anti/ac-znx.inc"


new TimerAntiSpawn[MAX_PLAYERS];
new TimerLastPos[MAX_PLAYERS];
hook OnPlayerConnect(playerid)
{
    TimerAntiSpawn[playerid] = SetTimerEx("AntiCheat", 500, 1, "i", playerid);
	TimerLastPos[playerid] = SetTimerEx("TimeLastPosF4T", 30000, 1, "i", playerid);
    SetPlayerVirtualWorld(playerid, 222);
	return 1;
}
hook OnPlayerDisconnect(playerid, reason)
{
    if (TimerAntiSpawn[playerid]) 
		KillTimer(TimerAntiSpawn[playerid]);
	KillTimer(TimerLastPos[playerid]);
	return 1;
}
hook OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart){
	if (PlayerInfo[playerid][pConnectHours] < 2)
	{
		if (weaponid != 0 && weaponid > 0 && weaponid < 36)
		{
	        new szMess[256];
	        RemovePlayerWeapon(playerid, weaponid);
	        format(szMess, sizeof(szMess), "%s da bi kick vi su dung hack Gun.", GetPlayerNameEx(playerid));
			discord_messHack(szMess);
	        ABroadCast(COLOR_YELLOW, szMess, 2);
	        SendClientMessageEx(playerid, COLOR_WHITE, "[{DD0000}Anti-F4T{FFFFFF}] Anti hack distance, neu co hack that hay tat giup tuiiii...!");
	        SetTimerEx("KickEx", 2000, 0, "i", playerid);
		}
	}
    if (weaponid == 38 && PlayerInfo[playerid][pAdmin] < 2)
    {
        new szMess[256];
        RemovePlayerWeapon(playerid, 38);
        format(szMess, sizeof(szMess), "%s da bi kick vi su dung hack Minigun.", GetPlayerNameEx(playerid));
        ABroadCast(COLOR_YELLOW, szMess, 2);
		discord_messHack(szMess);
        SendClientMessageEx(playerid, COLOR_WHITE, "[{DD0000}Anti-F4T{FFFFFF}] Anti cheating minigun, neu co hack that hay tat giup tuiiii...!");
        SetTimerEx("KickEx", 2000, 0, "i", playerid);
    }
	new Float: ax, Float: ay, Float: az;
	GetPlayerPos(playerid, ax,ay,az);
	if ((GetPlayerDistanceFromPoint(playerid, ax, ay, az) > 10 && weaponid == 0) || GetPlayerDistanceFromPoint(playerid, ax,ay,az) > 400)
	{
        new szMess[256];
        RemovePlayerWeapon(playerid, 38);
        format(szMess, sizeof(szMess), "%s da bi kick vi su dung hack Distance.", GetPlayerNameEx(playerid));
        ABroadCast(COLOR_YELLOW, szMess, 2);
		discord_messHack(szMess);
        SendClientMessageEx(playerid, COLOR_WHITE, "[{DD0000}Anti-F4T{FFFFFF}] Anti hack distance, neu co hack that hay tat giup tuiiii...!");
        SetTimerEx("KickEx", 2000, 0, "i", playerid);
	}
	if (amount > 30 && weaponid != 25 && weaponid != 26 && weaponid != 27 && weaponid != 24 && weaponid != 34)
	{
        new szMess[256];
        RemovePlayerWeapon(playerid, 38);
        format(szMess, sizeof(szMess), "%s da bi kick vi su dung hack Dame.", GetPlayerNameEx(playerid));
        ABroadCast(COLOR_YELLOW, szMess, 2);
		discord_messHack(szMess);
        SendClientMessageEx(playerid, COLOR_WHITE, "[{DD0000}Anti-F4T{FFFFFF}] Anti hack dame, neu co hack that hay tat giup tuiiii...!");
        SetTimerEx("KickEx", 2000, 0, "i", playerid);
	}
    return 1;
}
forward TimeLastPosF4T(playerid);
public TimeLastPosF4T(playerid)
{
	GetPlayerPos(playerid, LastPosF4T[playerid][0], LastPosF4T[playerid][1], LastPosF4T[playerid][2]);
	return 1;
}

forward AntiCheat(playerid);
public AntiCheat(playerid)
{
    if(gPlayerLogged{playerid} == 0)
    {
		// ANTI NDP SafeSetPlayerPos(playerid, 0,0,0);
		if (IsPlayerSpawned(playerid))
		{
			new szMess[256];
			format(szMess, sizeof(szMess), "%s da bi kick vi su dung hack Fakespawn.", GetPlayerNameEx(playerid));
			ABroadCast(COLOR_YELLOW, szMess, 2);
			discord_messHack(szMess);
			SendClientMessageEx(playerid, COLOR_WHITE, "[{DD0000}Anti-F4T{FFFFFF}] Anti cheating Fakespawn, neu co hack that hay tat giup tuiiii...!");
			SetTimerEx("KickEx", 2000, 0, "i", playerid);
		}
    }
    if (PlayerInfo[playerid][pAdmin] < 2)
    {
		if(GetPlayerWeapon(playerid) == 40)
	    {
	        new szMess[256];
	        format(szMess, sizeof(szMess), "%s da bi kick vi su dung hack Crashing.", GetPlayerNameEx(playerid));
	        ABroadCast(COLOR_YELLOW, szMess, 2);
	        RemovePlayerWeapon(playerid, 40);
			discord_messHack(szMess);
	        SendClientMessageEx(playerid, COLOR_WHITE, "[{DD0000}Anti-F4T{FFFFFF}] Anti cheating crashing, neu co hack that hay tat giup tuiiii...!");
	        SetTimerEx("KickEx", 2000, 0, "i", playerid);
	    }
  		if(GetPlayerWeapon(playerid) == 38)
	    {
	        new szMess[256];
	        format(szMess, sizeof(szMess), "%s da bi kick vi su dung hack Minigun.", GetPlayerNameEx(playerid));
	        ABroadCast(COLOR_YELLOW, szMess, 2);
	        RemovePlayerWeapon(playerid, 40);
			discord_messHack(szMess);
	        SendClientMessageEx(playerid, COLOR_WHITE, "[{DD0000}Anti-F4T{FFFFFF}] Anti cheating Minigun, neu co hack that hay tat giup tuiiii...!");
	        SetTimerEx("KickEx", 2000, 0, "i", playerid);
	    }
	}
	return 1;
}

task VehicleAntiNoFuel[1000]()
{
    static engine,lights,alarm,doors,bonnet,boot,objective, Float:healthv;
    for(new v = 0; v < MAX_VEHICLES; v++) if(GetVehicleModel(v) && GetVehicleModel(v) != 481 && GetVehicleModel(v) != 509 && GetVehicleModel(v) != 510) 
	{
		if (GetVehicleDriver(v) != INVALID_PLAYER_ID)
		{
			GetVehicleParamsEx(v,engine,lights,alarm,doors,bonnet,boot,objective);
			if(arr_Engine{v} == 0 && vehicle_get_speed(v) > 5) 
			{
				SlapAntiSlap(GetVehicleDriver(v));
				SendClientMessage(GetVehicleDriver(v), COLOR_LIGHTRED, "Ban chua bat dong co xe, vui long bam N de bat dong co xe va chay.");
			}
			GetVehicleHealth(v, healthv);
			if (healthv <= 252 && vehicle_get_speed(v) > 5)
			{
				SlapAntiSlap(GetVehicleDriver(v));
				SendClientMessage(GetVehicleDriver(v), COLOR_LIGHTRED, "Xe da hu hong, vui long sua chua de tiep tuc chay.");
			}
		}
	}
}
stock SlapAntiSlap(playerid)
{
	if (IsPlayerInAnyVehicle(playerid))
	{
		new VehicleAnti = GetPlayerVehicleID(playerid), Float:posC[3];
		GetPlayerPos(playerid, posC[0], posC[1], posC[2]);
		SetVehicleToRespawn(VehicleAnti);
		SafeSetVehiclePos(VehicleAnti, posC[0], posC[1], posC[2]);
	}
	else
	{
		new Float: posx, Float: posy, Float: posz, Float:poszsave;
		GetPlayerPos(playerid, posx, posy, posz);
		poszsave = posz+5;
		SafeSetPlayerPos(playerid, posx, posy, posz+5);
		PlayerPlaySound(playerid, 1130, posx, posy, posz+5);
		GetPlayerPos(playerid, posx, posy, posz);
		if (posz <= poszsave-2)
		{
	        new szMess[256];
	        format(szMess, sizeof(szMess), "%s da bi kick vi su dung hack Anti Slap / Bypass Car / Bypass Tele", GetPlayerNameEx(playerid));
	        ABroadCast(COLOR_YELLOW, szMess, 2);
	        RemovePlayerWeapon(playerid, 40);
			discord_messHack(szMess);
	        SendClientMessageEx(playerid, COLOR_WHITE, "[{DD0000}Anti-F4T{FFFFFF}] Anti cheating Anti Slap / Bypass Car / Bypass Tele, neu co hack that hay tat giup tuiiii...!");
	        SetTimerEx("KickEx", 2000, 0, "i", playerid);
		}
	}
	return 1;
}


/* stock IsPlayerSpawned(playerid)
{
	new pState = GetPlayerState(playerid);
	
	return 0 <= playerid < MAX_PLAYERS && pState != PLAYER_STATE_NONE && pState != PLAYER_STATE_WASTED && pState != PLAYER_STATE_SPECTATING;
} */
