#include <YSI/YSI_Coding/y_hooks>

	//-------------------------------------------------------------------
	//					Project create by #nDP 						   //
	//					Fb.com/nguyenduyphuong.com  			       //
	//					Copyright by nDP" 							   //
	//-------------------------------------------------------------------

//____________________________________________________________________________//

task timeCheckWeapon[5000]() {
	foreach(new playerid : Player) 
	{		
		if(GetPVarInt(playerid, "IsInArena") == -1 && GetPVarInt( playerid, "EventToken") == 0 && GetPVarInt( playerid, "KhongHackWeaponInv") != 1 && GetPVarInt(playerid, "Injured") != 1) {
			hackWeapon(playerid);
		}
	}
}

forward AntiCrasherWeapon(playerid);
public AntiCrasherWeapon(playerid) {
	if (GetPlayerWeapon(playerid) == WEAPON_BOMB)
	{
		new String[128];
        format( String, sizeof( String ), "[ANTI-CHEAT BANNED] Nguoi choi %s, da bi BANNED, ly do: Hack Crasher Weapon.", GetPlayerNameEx(playerid));
		ABroadCast( COLOR_LIGHTRED, String, 2 );
		discord_messHack(String);
		PlayerInfo[playerid][pBanned] = 3;
		Log("logs/ban.log", String);
		new ip[32];
		GetPlayerIp(playerid, ip, sizeof(ip));
		SystemBan(playerid, "[F4T] (Hack Crasher Weapon)");
		MySQLBan(GetPlayerSQLId(playerid), ip, "Hack Crasher Weapon", 1, "System");
		TotalAutoBan++;
		Kick(playerid);
	}
	return 1;
}

forward hackWeapon(playerid);
public hackWeapon(playerid) 
{
	// Double check - neu nguoi choi dang injured thi khong check weapon
	if(GetPVarInt(playerid, "Injured") == 1) return 1;
	
	new string[1280], weapons[13][2], weaponname[50];
	for (new i = 0; i < 13; i++)
	{
		GetPlayerWeaponData(playerid, i, weapons[i][0], weapons[i][1]);
		if(weapons[i][0] > 0)
		{
			if(PlayerInfo[playerid][pGuns][i] != weapons[i][0])
			{
				if(PlayerInfo[playerid][pAGuns][i] >= 1) return 1;
				if (GetPlayerWeapon(playerid) == 3) return 1;
				if(weapons[i][0] == 46) return 1;
				
				if (GetPlayerWeapon(playerid) == WEAPON_MINIGUN)
				{
					//Ban if they have a minigun
					new String[128];
			        format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s, da bi kick ly do: Hack vu khi (hackWeapon).", GetPlayerNameEx(playerid));
					ABroadCast( COLOR_LIGHTRED, String, 2 );
					discord_messHack(String);
					Kick(playerid);
				}
				if (GetPlayerWeapon(playerid) == WEAPON_HEATSEEKER)
				{
					//Ban if they have a minigun
					new String[128];
			        format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s, da bi kick ly do: Hack vu khi (hackWeapon).", GetPlayerNameEx(playerid));
					ABroadCast( COLOR_LIGHTRED, String, 2 );
					discord_messHack(String);
					Kick(playerid);
				}
				if (GetPlayerWeapon(playerid) == WEAPON_FLAMETHROWER)
				{
					//Ban if they have a minigun
					new String[128];
			        format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s, da bi kick ly do: Hack vu khi (hackWeapon).", GetPlayerNameEx(playerid));
					ABroadCast( COLOR_LIGHTRED, String, 2 );
					discord_messHack(String);
					Kick(playerid);
				}
				if (GetPlayerWeapon(playerid) == WEAPON_ROCKETLAUNCHER)
				{
					//Ban if they have a minigun
					new String[128];
			        format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s, da bi kick ly do: Hack vu khi (hackWeapon).", GetPlayerNameEx(playerid));
					ABroadCast( COLOR_LIGHTRED, String, 2 );
					discord_messHack(String);
					Kick(playerid);
				}
				if (GetPlayerWeapon(playerid) == WEAPON_SATCHEL)
				{
					//Ban if they have a minigun
					new String[128];
			        format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s, da bi kick ly do: Hack vu khi (hackWeapon).", GetPlayerNameEx(playerid));
					ABroadCast( COLOR_LIGHTRED, String, 2 );
					discord_messHack(String);
					Kick(playerid);
				}
				if (GetPlayerWeapon(playerid) == WEAPON_BOMB)
				{
					new String[128];
			        format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s, da bi kick ly do: Hack vu khi (hackWeapon).", GetPlayerNameEx(playerid));
					ABroadCast( COLOR_LIGHTRED, String, 2 );
					discord_messHack(String);
					Kick(playerid);
				}
				if (GetPlayerWeapon(playerid) == WEAPON_SATCHEL)
				{
					//Ban if they have a minigun
					new String[128];
			        format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s, da bi kick ly do: Hack vu khi (hackWeapon).", GetPlayerNameEx(playerid));
					ABroadCast( COLOR_LIGHTRED, String, 2 );
					discord_messHack(String);
					Kick(playerid);
				}
				if (GetPlayerWeapon(playerid) == 44)
				{
					//Ban if they have a minigun
					new String[128];
			        format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s, da bi kick ly do: Hack vu khi (hackWeapon).", GetPlayerNameEx(playerid));
					ABroadCast( COLOR_LIGHTRED, String, 2 );
					discord_messHack(String);
					Kick(playerid);
				}
				if (GetPlayerWeapon(playerid) == 45)
				{
					//Ban if they have a minigun
					new String[128];
			        format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s, da bi kick ly do: Hack vu khi (hackWeapon).", GetPlayerNameEx(playerid));
					ABroadCast( COLOR_LIGHTRED, String, 2 );
					discord_messHack(String);
					Kick(playerid);
				}
				if(GetPVarInt(playerid, "Injured") == 1) return 1;
				GetWeaponName(weapons[i][0], weaponname, sizeof(weaponname));
				format(string, sizeof(string), "%s da bi kick do su dung hack/weapon %s (%d) (non server-side).", GetPlayerNameEx(playerid), weaponname, weapons[i][0]);
				ABroadCast( COLOR_LIGHTRED, string, 2 );
				discord_messHack(string);
				Kick(playerid);
				return 1;
			}
		}
	}
    return 1;
}
