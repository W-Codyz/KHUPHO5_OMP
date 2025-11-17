#include <YSI/YSI_Coding/y_hooks>

	//-------------------------------------------------------------------
	//					Project create by #nDP 						   //
	//					Fb.com/nguyenduyphuong.com  			       //
	//					Copyright by nDP" 							   //
	//-------------------------------------------------------------------

//____________________________________________________________________________//

enum telePlayer 
{
    tlTime, // Time teleport
    tlWarning,

    Float:tlPosX,
    Float:tlPosY,
    Float:tlPosZ,
};
new pTeleport[MAX_PLAYERS][telePlayer];

forward h_OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ);
public h_OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
    if(PlayerInfo[playerid][pAdmin] == 0) antiTeleport(playerid, fX, fY, fZ);
	if(PlayerInfo[playerid][pAdmin] >= 2)
	{
	    new zone[MAX_ZONE_NAME];
	    Get3DZone(fX,fY,fZ, zone, sizeof(zone));

		if(GetPlayerState(playerid) == 2) {
			new tmpcar = GetPlayerVehicleID(playerid);
			SetVehiclePos(tmpcar, fX, fY, fZ);
			LinkVehicleToInterior(tmpcar, 0);
			SetVehicleVirtualWorld(tmpcar, 0);
			fVehSpeed[playerid] = 0.0;
			new string[1280];
			format(string, sizeof(string), "Ban da duoc dich chuyen den {00ffff}%s", zone);
			SendClientMessageEx(playerid, -1, string);
		}
		else {
			ac_SetPlayerPos(playerid, fX, fY, fZ);
			new string[1280];
			format(string, sizeof(string), "Ban da duoc dich chuyen den {00ffff}%s", zone);
			SendClientMessageEx(playerid, -1, string);
		}
	}
	return 1;
}

forward antiTeleport(playerid, Float:X, Float:Y, Float:Z);
public antiTeleport(playerid, Float:X, Float:Y, Float:Z) 
{
    if(PlayerInfo[playerid][pAdmin] >= 2) return 1;
    if(X == 0 && Y == 0 && Z == 0) return 1;
    
    if(IsPlayerInRangeOfPoint(playerid, 50.0, X, Y, Z)) {
        return 1;
    }

    new iDist = floatround(GetPlayerDistanceFromPoint(playerid, X, Y, Z));
    new time;
    if(iDist > 100) time = iDist / 55; // 45
    else time = 3; // 10

    pTeleport[playerid][tlTime] = gettime() + time;

    pTeleport[playerid][tlPosX] = X;
    pTeleport[playerid][tlPosY] = Y;
    pTeleport[playerid][tlPosZ] = Z;
    return 1;
}

task checkTeleport[500]() {
    foreach(new playerid: Player) 
    {
        if(pTeleport[playerid][tlWarning] == 0) {
            if(IsPlayerInRangeOfPoint(playerid, 10.0, pTeleport[playerid][tlPosX], pTeleport[playerid][tlPosY], pTeleport[playerid][tlPosZ])) {
                if(pTeleport[playerid][tlTime] > gettime())
                {
                    SetPlayerPos(playerid, LastPosF4T[playerid][0], LastPosF4T[playerid][1], LastPosF4T[playerid][2]);
                    new String[128];
                    format( String, sizeof( String ), "[ANTI-CHEAT]: %s da bi kick do su dung Hack Teleport.", GetPlayerNameEx(playerid));
                    ABroadCast( COLOR_LIGHTRED, String, 2 );
                    discord_messHack(String);
                    Kick(playerid);
                }
            }
        }
    }
}
