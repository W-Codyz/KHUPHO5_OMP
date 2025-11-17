#include <a_samp>
#include <YSI/YSI_Coding/y_hooks>

enum acEnum
{
	Float:	acPosX,
	Float:	acPosY,
	Float:	acPosZ,
	Float:  acSpawnX,
	Float:  acSpawnY,
	Float:  acSpawnZ,
		    acImunidade
};

static TPInfo[MAX_PLAYERS][acEnum];

static TPInfo_RangeCheck(Float:x1, Float:y1, Float:z1, Float:radius, Float:x2, Float:y2, Float:z2)
{
	x1 -= x2;
	y1 -= y2;
	z1 -= z2;

	return ((x1 * x1) + (y1 * y1) + (z1 * z1)) < (radius * radius);
}


hook OnPlayerUpdate(playerid)
{
	if(GetPlayerState(playerid) != PLAYER_STATE_SPECTATING && GetPlayerState(playerid) != PLAYER_STATE_NONE && GetPlayerInterior(playerid) == 0 && gPlayerLogged{playerid} != 0)
	{
	 if(PlayerInfo[playerid][pAdmin] == 0)
    {
		if(gettime() > TPInfo[playerid][acImunidade])
		{
		  if(GetPVarInt(playerid, "ClickMap") > gettime())
		  {
		 	if(!IsPlayerInRangeOfPoint(playerid, 15.0, TPInfo[playerid][acPosX], TPInfo[playerid][acPosY], TPInfo[playerid][acPosZ]) && !IsPlayerInRangeOfPoint(playerid, 5.0, TPInfo[playerid][acSpawnX], TPInfo[playerid][acSpawnY], TPInfo[playerid][acSpawnZ]))
		 	{
			    new
					Float:x,
					Float:y,
					Float:z;

				GetPlayerPos(playerid, x, y, z);

				if(!TPInfo_RangeCheck(TPInfo[playerid][acPosX], TPInfo[playerid][acPosY], TPInfo[playerid][acPosZ], 3.0, TPInfo[playerid][acSpawnX], TPInfo[playerid][acSpawnY], TPInfo[playerid][acSpawnZ]) && x != 0.0 && y != 0.0 && z != 0.0 && GetPlayerState(playerid) != PLAYER_STATE_PASSENGER)
				{
   				 new CheaterName[MAX_PLAYER_NAME];
                    new string[128];
                    GetPlayerName(playerid, CheaterName, sizeof(CheaterName));
                    format( string, sizeof( string ), "Nghi van %s (%d) su dung teleport",CheaterName, playerid);
                    discord_messHack(string);
					ABroadCast(COLOR_YELLOW, string, 2); 
				   }
				  }
			    }
		    }
		}
	   GetPlayerPos(playerid, TPInfo[playerid][acPosX], TPInfo[playerid][acPosY], TPInfo[playerid][acPosZ]);
	}
    return 1;
}

hook OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
    SetPVarInt(playerid, "ClickMap", gettime() + 15);
    return 1;
}
