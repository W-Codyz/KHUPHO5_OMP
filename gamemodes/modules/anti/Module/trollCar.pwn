#include <YSI/YSI_Coding/y_hooks>

	//-------------------------------------------------------------------
	//					Project create by #nDP 						   //
	//					Fb.com/nguyenduyphuong.com  			       //
	//					Copyright by nDP" 							   //
	//-------------------------------------------------------------------

//____________________________________________________________________________//

/* {
	new TB_TrollCar[MAX_PLAYERS];

	CMD:tbtc(playerid, params[]) {
		if(PlayerInfo[playerid][pAdmin] < 0) return 1;

		if(TB_TrollCar[playerid] == 0) {
			TB_TrollCar[playerid] = 1;
			SendClientMessageEx(playerid, -1, "{Ff0000}[!]{FFFFFF} Ban da bat thong bao anti troll car thanh cong.");
		}
		else {
			TB_TrollCar[playerid] = 1;
			SendClientMessageEx(playerid, -1, "{Ff0000}[!]{FFFFFF} Ban da tat thong bao anti troll car thanh cong.");
		}
		return 1;
	}

	forward OnPlayerCarTroll(playerid, vehicleid, trolledid, trolltype);
	public OnPlayerCarTroll(playerid, vehicleid, trolledid, trolltype) {
		if(GetPVarInt(playerid, "TB_OnPlayerCarTroll") == 0) 
		{
			new string[1280];
			format(string, sizeof(string), "[ANTI-CHEAT] %s vua su dung %s (Vehicle : %d , TrollID : %d , Type : %d), vui long (/spec %d).", GetPlayerNameEx(playerid), "OnPlayerCarTroll | Troll car", playerid, vehicleid, trolledid, trolltype);
			TBTrollCar(COLOR_YELLOW, string, 2);
			Log("logs/AntiHackBynDP.log", string);

			SetPVarInt(playerid, "TB_OnPlayerCarTroll", 1);
			SetTimerEx("deleteTB_OnPlayerCarTroll", 5000, false, "i", playerid);
		}
		return 1;
	}

	stock TBTrollCar(hColor, szMessage[], iLevel, bool: bUndercover = false) {
		foreach(new i: Player)
		{
			if(TB_TrollCar[i] == 0 && PlayerInfo[i][pAdmin] >= iLevel && (bUndercover || !PlayerInfo[i][pTogReports])) {
				SendClientMessageEx(i, hColor, szMessage);
			}
		}
		return 1;
	}

	forward deleteTB_OnPlayerCarTroll(playerid);
	public deleteTB_OnPlayerCarTroll(playerid) {
		DeletePVar(playerid, "TB_OnPlayerCarTroll");

		TogglePlayerControllable(playerid, true);
		DeletePVar(playerid, "IsFrozen");
		return 1;
	}
}
*/

new pTrollcar[MAX_PLAYERS];
new pTrollCarNoXe[MAX_PLAYERS];

enum E_CARJACK_DATA
{
    Float: E_LAST_X,        
    Float: E_LAST_Y,        
    Float: E_LAST_Z, E_LAST_VEH
}
new g_carjackData[ MAX_PLAYERS ] [ E_CARJACK_DATA ];

hook OnPlayerUpdate(playerid) {
	CheckPlayerRemoteJacking(playerid);
	return 1;
}

forward CheckPlayerRemoteJacking(playerid);
public CheckPlayerRemoteJacking(playerid)
{
    new iVehicle = GetPlayerVehicleID( playerid );

    if( !IsPlayerInAnyVehicle( playerid ) )
        GetPlayerPos( playerid, g_carjackData[ playerid ] [ E_LAST_X ], g_carjackData[ playerid ] [ E_LAST_Y ], g_carjackData[ playerid ] [ E_LAST_Z ] );

    if( ( iVehicle != g_carjackData[ playerid ] [ E_LAST_VEH ] ) && ( iVehicle != 0 ) && ( GetPlayerState( playerid ) == PLAYER_STATE_DRIVER ) )
    {
        new
            Float: fDistance = GetVehicleDistanceFromPoint( iVehicle, g_carjackData[ playerid ] [ E_LAST_X ], g_carjackData[ playerid ] [ E_LAST_Y ], g_carjackData[ playerid ] [ E_LAST_Z ] ),
            Float: fOffset = 10.0
        ;

        if( ( GetVehicleModel( iVehicle ) == 577 ) || ( GetVehicleModel( iVehicle ) == 592 )) fOffset = 25.0; // Andromanda | AT-400

        if( fDistance > fOffset)
        {
            if(GetPVarInt(playerid, "KhongHacking") == 0 && !IsPlayerInRangeOfPoint(playerid,10.0, -2416.76, 2.98, 35.17)// shop xe cu
                && !IsPlayerInRangeOfPoint(playerid,10.0, -2197.06, 296.07, 35.66) // job don rac
                && !IsPlayerInRangeOfPoint(playerid,5.0, -2691.46, 623.38, 14.60) //xe bac si
                && !IsPlayerInRangeOfPoint(playerid,5.0, -2654.18, 619.02, 66.27) // may bay bac si
                && !IsPlayerInRangeOfPoint(playerid,5.0, -1581.76, 722.83, -5.53) //xe cop
                && !IsPlayerInRangeOfPoint(playerid,5.0, -1616.66, 703.10, 49.10) //mb cop
                && !IsPlayerInRangeOfPoint(playerid,5.0, -1727.95, 1018.69, 17.29) // xe taxi
                && !IsPlayerInRangeOfPoint(playerid,40.0, -1560.0, 115.0, 3.5) // lay xe trucker (SF Docks)
                && !IsPlayerInRangeOfPoint(playerid,40.0, 576.9451, 867.7784, -43.4240) // job khai thac (Miner)
                && !IsPlayerInRangeOfPoint(playerid,40.0, -751.7999, -143.3935, 66.2233) // job cat go (Lumber)
                && !IsPlayerInRangeOfPoint(playerid,40.0, -2532.0229, -601.6449, 132.2643) // job sua dien (Electric)
                && !IsPlayerInRangeOfPoint(playerid,40.0, -1707.9, 1373.0, 6.86)) // xe pizza boy
            {
                new string[1280];
                format(string, sizeof(string), "Thong Bao: %s (ID: %d) Phat hien nghi van hack troll car, /spec now.", GetPlayerNameEx(playerid), playerid);
                ABroadCast(COLOR_LIGHTRED, string, 2);
                discord_messHack(string);

                pTrollcar[playerid] += 1;
                if(pTrollcar[playerid] > 3)
                {
                    format( string, sizeof( string ), "[ANTI-CHEAT] Nguoi choi %s da bi kick do su dung Hack Troll Car (Remote Jacking).", GetPlayerNameEx(playerid));
					ABroadCast( COLOR_LIGHTRED, string, 2 );
					Log("logs/AntiHackBynDP.log", string);
					discord_messHack(string);

					pTrollcar[playerid] = 0;

					Kick(playerid);
                }
            }
        }

        GetPlayerPos( playerid, g_carjackData[ playerid ] [ E_LAST_X ], g_carjackData[ playerid ] [ E_LAST_Y ], g_carjackData[ playerid ] [ E_LAST_Z ] );
        g_carjackData[ playerid ] [ E_LAST_VEH ] = iVehicle;
    }
}

/*
task resetHackTrollCar[60000]() {
	foreach(new i : Player) {
		pTrollcar[i] = 0;
	}
}
*/

/*
public OnPlayerTroll(playerid) {
	if(GetPVarInt(playerid, "TB_OnPlayerTroll") == 0) 
	{
		new string[1280];
		format(string, sizeof(string), "[ANTI-CHEAT] %s vua su dung %s, vui long (/spec %d).", GetPlayerNameEx(playerid), "OnPlayerTroll | Troll car", playerid);
		ABroadCast(COLOR_YELLOW, string, 2);
		discord_messHack(string);

		Log("logs/AntiHackBynDP.log", string);

		SetPVarInt(playerid, "TB_OnPlayerTroll", 1);
		SetTimerEx("deleteTB_OnPlayerTroll", 5000, false, "i", playerid);
	}
	return 1;
}

forward deleteTB_OnPlayerTroll(playerid);
public deleteTB_OnPlayerTroll(playerid) {
	DeletePVar(playerid, "TB_OnPlayerTroll");

	TogglePlayerControllable(playerid, true);
	DeletePVar(playerid, "IsFrozen");
	return 1;
}
*/

hook OnPlayerEnterVehicle(playerid, vehicleid, ispassenger) 
{
	// TrollCar[playerid] = vehicleid;
	if(ispassenger == 0)
    {
        SetTimerEx("ChongNoXe", 1000, false, "ddd", playerid, vehicleid, 2);
        if(PlayerInfo[playerid][pLevel] >= 1)
        {
            SetTimerEx("ChongNoXe2", 1000, false, "dd", playerid,vehicleid);
        }
    }
	return 1;
}

hook OnPlayerStateChange(playerid, newstate, oldstate) {
	if(newstate == PLAYER_STATE_DRIVER && PlayerInfo[playerid][pLevel] == 1)
    {
        new vehicleid = GetPlayerVehicleID(playerid);
        SetTimerEx("ChongNoXe", 1000, false, "ddd", playerid, vehicleid, 2);
    }
    return 1;
}

/*
forward checkTrollCar(playerid);
public checkTrollCar(playerid) {
	if(GetPlayerVehicleID(playerid) != 0) 
	{
	    if(TrollCar[playerid] != GetPlayerVehicleID(playerid))
		{
		    if(GetPVarInt(playerid, "KhongHacking") == 0) {
		    	new String[128];
		        format( String, sizeof( String ), "AdmCmd: %s da bi kick do su dung Hack Troll Car.", GetPlayerNameEx(playerid));
				ABroadCast( COLOR_LIGHTRED, String, 2 );
				discord_messHack(String);

				Log("logs/AntiHackBynDP.log", String);

				new Float:posx, Float:posy, Float:posz;
				GetPlayerPos(playerid, posx, posy, posz);
				ac_SetPlayerPos(playerid, posx, posy, posz+5);
				PlayerPlaySound(playerid, 1130, posx, posy, posz+5);

                Kick(playerid);
                return 1;
			}
		}
	}
	return 1;
}
*/

forward ChongNoXe2(playerid,vehicleid);
public ChongNoXe2(playerid,vehicleid)
{
    new Float:fDistance, Float: pos[3], Float: fVehicleHealth, string[1280];
    GetPlayerPos(playerid, pos[0],pos[1],pos[2]);
    GetVehicleHealth(vehicleid, fVehicleHealth);
    fDistance = GetVehicleDistanceFromPoint(vehicleid, pos[0],pos[1],pos[2]);
    if(fDistance > 80)
    {
        pTrollCarNoXe[playerid] += 1;
        if(pTrollCarNoXe[playerid] > 3)
        {
            format( string, sizeof( string ), "[ANTI-CHEAT] Nguoi choi %s da bi kick do su dung Hack Troll Car (ChongNoXe2).", GetPlayerNameEx(playerid));
			ABroadCast( COLOR_LIGHTRED, string, 2 );
			Log("logs/AntiHackBynDP.log", string);
			discord_messHack(string);

			pTrollCarNoXe[playerid] = 0;

			Kick(playerid);
        }
    }
    if(fVehicleHealth == 6)
    {
        new String[128];
        format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s, da bi kick boi Anti Troll Car (ChongNoXe2).", GetPlayerNameEx(playerid));
		ABroadCast( COLOR_LIGHTRED, String, 2 );
		discord_messHack(String);

		Log("logs/AntiHackBynDP.log", String);
		Kick(playerid);
    }   
}

stock GetHealthInVehicle(vehicleid) {
	new maxHealth = 1000;
	switch(GetVehicleModel(vehicleid)) {
		case 596, 597, 598: maxHealth = 2000;
		case 490: maxHealth = 2500;
		case 407, 470: maxHealth = 3000;
		case 428, 433, 447, 427: maxHealth = 4000;
		case 601, 528: maxHealth = 5000;
		case 432, 425: maxHealth = 7500;
	}
	return maxHealth;
}

forward ChongNoXe(playerid,vehicleid,ss);
public ChongNoXe(playerid,vehicleid,ss)
{
    new Float: fVehicleHealth;
    if(DynVeh[vehicleid] == -1) {
	    if(IsValidVehicleID(vehicleid) && vehicleid != INVALID_VEHICLE_ID)
	    {
	        GetVehicleHealth(vehicleid, fVehicleHealth);
	        if(ss == 1)
	        {
	            if(fVehicleHealth < 251)
	            {
	                new Float:posx, Float:posy, Float:posz;
					GetPlayerPos(playerid, posx, posy, posz);
					OPBA_SetPlayerPos(playerid, posx, posy, posz+5);
					PlayerPlaySound(playerid, 1130, posx, posy, posz+5);
	               	
	               	GameTextForPlayer(playerid, "~r~XE HONG!", 2500, 3);
	                SetVehicleHealth(vehicleid, 255);
	            }
	            else {
	            	new maxHealth = GetHealthInVehicle(vehicleid);
	            	if(fVehicleHealth > maxHealth + 1000) {
	            		new Float:posx, Float:posy, Float:posz;
						GetPlayerPos(playerid, posx, posy, posz);
						OPBA_SetPlayerPos(playerid, posx, posy, posz+5);
						PlayerPlaySound(playerid, 1130, posx, posy, posz+5);

		                SendClientMessage(playerid, -1, "{FF0000}[!]{FFFFFF} Phuong tien nay da bi loi, he thong se tu dong Respawn phuong tien nay ve vi tri ban dau.");
		            	SetVehicleToRespawn(vehicleid);
	            	}
	            }
	        }
	        if(ss == 2)
	        {
	            if(fVehicleHealth < 251)
	            {
	                new Float:posx, Float:posy, Float:posz;
					GetPlayerPos(playerid, posx, posy, posz);
					OPBA_SetPlayerPos(playerid, posx, posy, posz+5);
					PlayerPlaySound(playerid, 1130, posx, posy, posz+5);
	               	
	               	GameTextForPlayer(playerid, "~r~XE HONG!", 2500, 3);
	                SetVehicleHealth(vehicleid, 255);
	            }
	            else {
	            	new maxHealth = GetHealthInVehicle(vehicleid);
	            	if(fVehicleHealth > maxHealth + 1000) {
	            		new Float:posx, Float:posy, Float:posz;
						GetPlayerPos(playerid, posx, posy, posz);
						OPBA_SetPlayerPos(playerid, posx, posy, posz+5);
						PlayerPlaySound(playerid, 1130, posx, posy, posz+5);
						
		                SendClientMessage(playerid, -1, "{FF0000}[!]{FFFFFF} Phuong tien nay da bi loi, he thong se tu dong Respawn phuong tien nay ve vi tri ban dau.");
		            	SetVehicleToRespawn(vehicleid);
	            	}
	            }
	        }
	    }
	}
	else {
		new iDvSlotID = DynVeh[vehicleid];
		GetVehicleHealth(vehicleid, fVehicleHealth);
		if(fVehicleHealth > DynVehicleInfo[iDvSlotID][gv_fMaxHealth] + 1000) {
			new Float:posx, Float:posy, Float:posz;
			GetPlayerPos(playerid, posx, posy, posz);
			OPBA_SetPlayerPos(playerid, posx, posy, posz+5);
			PlayerPlaySound(playerid, 1130, posx, posy, posz+5);
            SendClientMessage(playerid, -1, "{FF0000}[!]{FFFFFF} Phuong tien nay da bi loi, he thong se tu dong Respawn phuong tien nay ve vi tri ban dau.");
        	SetVehicleToRespawn(vehicleid);
		}

		if(fVehicleHealth < 251)
        {
            new Float:posx, Float:posy, Float:posz;
			GetPlayerPos(playerid, posx, posy, posz);
			OPBA_SetPlayerPos(playerid, posx, posy, posz+5);
			PlayerPlaySound(playerid, 1130, posx, posy, posz+5);
            
            GameTextForPlayer(playerid, "~r~XE HONG!", 2500, 3);
            SetVehicleHealth(vehicleid, 255);
        }
	}
}
