#include <YSI\YSI_Coding\y_hooks>

stock IsPlayerInRangeActor(playerid, actorid, Float:range)
{
    if(!IsValidDynamicActor(actorid)) return 0;

    new Float:actorPos[3];
    GetDynamicActorPos(actorid, actorPos[0], actorPos[1], actorPos[2]);
    return (IsPlayerInRangeOfPoint(playerid, range, actorPos[0], actorPos[1], actorPos[2]));
}

forward LoadStreamerActors();
public LoadStreamerActors()
{
    for(new i=0; i < MAX_SERVER_ACTORS; i++)
    {
        CreateDynamicActor(ActorInfo[i][actorModelID], ActorInfo[i][actorX], ActorInfo[i][actorY], ActorInfo[i][actorZ], ActorInfo[i][actorRot], _, _, ActorInfo[i][actorVW], ActorInfo[i][actorInt]);
        
        new string[128];
        format(string, sizeof(string), "{4B86C2}%s\n\n{FFFFFF}Nhan \"{5FBC52}Y{FFFFFF}\" de tuong tac", ActorInfo[i][actorName]);
        CreateDynamic3DTextLabel(string, 140, ActorInfo[i][actorX], ActorInfo[i][actorY], ActorInfo[i][actorZ]+0.3, 10.0, _, _, 1, ActorInfo[i][actorVW], ActorInfo[i][actorInt]);
    }

    print("[Streamer] Actors has been loaded.");
    return 1;
}

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if(PRESSED(KEY_YES))
    {
        for(new i=0; i < MAX_SERVER_ACTORS; i++)
        {
            if(IsPlayerInRangeOfPoint(playerid, 3.0, ActorInfo[i][actorX], ActorInfo[i][actorY], ActorInfo[i][actorZ]))
            {
                switch(i)
                {
                    case ACTOR_CITYHALL: ShowCityHallDialog(playerid);
                    case ACTOR_CARLICENSE: ShowLicenseDialog(playerid, 0);
                    case ACTOR_TAXILICENSE: ShowLicenseDialog(playerid, 1);
                    case ACTOR_NEWBIEMISSION: ShowNewbieMission(playerid);
                    case ACTOR_FISHERMAN: /* ShowFishermanDialog(playerid); */ return 1;
                    case ACTOR_RENTCAR: ShowDialogRentCar(playerid);
                }
                return 1;
            }
        }
    }
    return 1;
}