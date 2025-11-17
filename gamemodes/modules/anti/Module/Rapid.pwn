#include <YSI/YSI_Coding/y_hooks>

	//-------------------------------------------------------------------
	//					Project create by #nDP 						   //
	//					Fb.com/nguyenduyphuong.com  			       //
	//					Copyright by nDP" 							   //
	//-------------------------------------------------------------------

//____________________________________________________________________________//

new SoLanRapid[MAX_PLAYERS];

hook OnPlayerConnect(playerid) {
	SoLanRapid[playerid] = 0;
	return 1;
}

hook OnPlayerDisconnect(playerid, reason) {
	SoLanRapid[playerid] = 0;
	return 1;
}

forward h_AntiRapid(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
public h_AntiRapid(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ) {
	if(PlayerInfo[playerid][pAdmin] > 2) return 1;
	
    if(weaponid == 33 && weaponid == 34) {
        if((gettime() - RapidTime_Sniper[playerid]) < 1)
        {
            RapidAmount_Sniper[playerid] += 1;
        }
        else
        {
            RapidAmount_Sniper[playerid] = 0;
        }
        if(RapidAmount_Sniper[playerid] >= 5)
        {
            if(GetPVarInt(playerid, "TB_AntiRapid") == 0) {
	            new String[128];
	            format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s , nghi van su dung Hack Rapid.", GetPlayerNameEx(playerid));
	            ABroadCast( COLOR_LIGHTRED, String, 2 );
	            discord_messHack(String);
	            
	            SetPVarInt(playerid, "TB_AntiRapid", 1);
				SetTimerEx("deleteTB_AntiRapid", 5000, false, "i", playerid);
            }
            
            SoLanRapid[playerid] += 1;
        	if(SoLanRapid[playerid] == 3) {
        		new String[128];
	            format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s , da bi kick do nghi van su dung Hack Rapid.", GetPlayerNameEx(playerid));
	            ABroadCast( COLOR_LIGHTRED, String, 2 );
	            discord_messHack(String);

	            Kick(playerid);
			}

            if(AdminOnl() == 0) Kick(playerid);
        }
        RapidTime_Sniper[playerid] = gettime();
    }

    if(weaponid == 27) {
        if((gettime() - RapidTime_Shotgun[playerid]) < 1)
        {
            RapidAmount_Shotgun[playerid] += 1;
        }
        else
        {
            RapidAmount_Shotgun[playerid]=0;
        }
        if(RapidAmount_Shotgun[playerid] >= 7)
        {
            if(GetPVarInt(playerid, "TB_AntiRapid") == 0) {
	            new String[128];
	            format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s , nghi van su dung Hack Rapid.", GetPlayerNameEx(playerid));
	            ABroadCast( COLOR_LIGHTRED, String, 2 );
	            discord_messHack(String);
	            
	            SetPVarInt(playerid, "TB_AntiRapid", 1);
				SetTimerEx("deleteTB_AntiRapid", 5000, false, "i", playerid);
            }
            
            SoLanRapid[playerid] += 1;
        	if(SoLanRapid[playerid] == 3) {
        		new String[128];
	            format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s , da bi kick do nghi van su dung Hack Rapid.", GetPlayerNameEx(playerid));
	            ABroadCast( COLOR_LIGHTRED, String, 2 );
	            discord_messHack(String);

	            Kick(playerid);
			}

            if(AdminOnl() == 0) Kick(playerid);
        }
        RapidTime_Shotgun[playerid] = gettime();
    }

    if(weaponid == 25) {
        if((gettime() - RapidTime_Shotgun[playerid]) < 1)
        {
            RapidAmount_Shotgun[playerid] += 1;
        }
        else
        {
            RapidAmount_Shotgun[playerid] = 0;
        }
        if(RapidAmount_Shotgun[playerid] >= 5)
        {
            if(GetPVarInt(playerid, "TB_AntiRapid") == 0) {
	            new String[128];
	            format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s , nghi van su dung Hack Rapid.", GetPlayerNameEx(playerid));
	            ABroadCast( COLOR_LIGHTRED, String, 2 );
	            discord_messHack(String);
	            
	            SetPVarInt(playerid, "TB_AntiRapid", 1);
				SetTimerEx("deleteTB_AntiRapid", 5000, false, "i", playerid);
            }
            
            SoLanRapid[playerid] += 1;
        	if(SoLanRapid[playerid] == 3) {
        		new String[128];
	            format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s , da bi kick do nghi van su dung Hack Rapid.", GetPlayerNameEx(playerid));
	            ABroadCast( COLOR_LIGHTRED, String, 2 );
	            discord_messHack(String);

	            Kick(playerid);
			}

            if(AdminOnl() == 0) Kick(playerid);
        }
        RapidTime_Shotgun[playerid] = gettime();
    }

    if(weaponid == 22) {
        if((gettime() - RapidTime_Deagle[playerid]) < 1)
        {
            RapidAmount_Deagle[playerid] += 1;
        }
        else
        {
            RapidAmount_Deagle[playerid]=0;
        }
        if(RapidAmount_Deagle[playerid] >= 5)
        {
            if(GetPVarInt(playerid, "TB_AntiRapid") == 0) {
	            new String[128];
	            format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s , nghi van su dung Hack Rapid.", GetPlayerNameEx(playerid));
	            ABroadCast( COLOR_LIGHTRED, String, 2 );
	            discord_messHack(String);

	            SetPVarInt(playerid, "TB_AntiRapid", 1);
				SetTimerEx("deleteTB_AntiRapid", 5000, false, "i", playerid);
            }
            
            SoLanRapid[playerid] += 1;
        	if(SoLanRapid[playerid] == 3) {
        		new String[128];
	            format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s , da bi kick do nghi van su dung Hack Rapid.", GetPlayerNameEx(playerid));
	            ABroadCast( COLOR_LIGHTRED, String, 2 );
	            discord_messHack(String);

	            Kick(playerid);
			}

            if(AdminOnl() == 0) Kick(playerid);
        }
        RapidTime_Deagle[playerid] = gettime();
    }

    if(weaponid == 24 || weaponid == 23) {
        if((gettime() - RapidTime_Deagle[playerid]) < 1)
        {
            RapidAmount_Deagle[playerid] += 1;
        }
        else
        {
            RapidAmount_Deagle[playerid] = 0;
        }
        if(RapidAmount_Deagle[playerid] >= 5)
        {
            if(GetPVarInt(playerid, "TB_AntiRapid") == 0) {
	            new String[128];
	            format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s , nghi van su dung Hack Rapid.", GetPlayerNameEx(playerid));
	            ABroadCast( COLOR_LIGHTRED, String, 2 );
	            discord_messHack(String);

	            SetPVarInt(playerid, "TB_AntiRapid", 1);
				SetTimerEx("deleteTB_AntiRapid", 5000, false, "i", playerid);
            }
            
            SoLanRapid[playerid] += 1;
        	if(SoLanRapid[playerid] == 3) {
        		new String[128];
	            format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s , da bi kick do nghi van su dung Hack Rapid.", GetPlayerNameEx(playerid));
	            ABroadCast( COLOR_LIGHTRED, String, 2 );
	            discord_messHack(String);

	            Kick(playerid);
			}

            if(AdminOnl() == 0) Kick(playerid);
        }
        RapidTime_Deagle[playerid] = gettime();
    }
    if(weaponid != 38 && weaponid != 28 && weaponid != 32 && weaponid != 24 && weaponid != 22 && weaponid != 23 && weaponid != 25 && weaponid != 26 && weaponid != 27 && weaponid != 33 && weaponid != 34)
    {
        if((gettime() - RapidTime[playerid]) < 1)
        {
            RapidAmount[playerid]+=1;
        }
        else
        {
            RapidAmount[playerid]=0;
        }
        if(RapidAmount[playerid] >= 15)
        {
            if(GetPVarInt(playerid, "TB_AntiRapid") == 0) {
	            new String[128];
	            format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s , nghi van su dung Hack Rapid.", GetPlayerNameEx(playerid));
	            ABroadCast( COLOR_LIGHTRED, String, 2 );
	            discord_messHack(String);

	            SetPVarInt(playerid, "TB_AntiRapid", 1);
				SetTimerEx("deleteTB_AntiRapid", 5000, false, "i", playerid);
            }
            
            SoLanRapid[playerid] += 1;
        	if(SoLanRapid[playerid] == 3) {
        		new String[128];
	            format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s , da bi kick do nghi van su dung Hack Rapid.", GetPlayerNameEx(playerid));
	            ABroadCast( COLOR_LIGHTRED, String, 2 );
	            discord_messHack(String);
	            
	            Kick(playerid);
			}

            if(AdminOnl() == 0) Kick(playerid);
        }
        RapidTime[playerid] = gettime();
    }
	return 1;
}

forward deleteTB_AntiRapid(playerid);
public deleteTB_AntiRapid(playerid) {
	DeletePVar(playerid, "TB_AntiRapid");

	TogglePlayerControllable(playerid, true);
	DeletePVar(playerid, "IsFrozen");
	return 1;
}
