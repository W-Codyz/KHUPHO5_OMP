#include <YSI/YSI_Coding/y_hooks>

	//-------------------------------------------------------------------
	//					Project create by #nDP 						   //
	//					Fb.com/nguyenduyphuong.com  			       //
	//					Copyright by nDP" 							   //
	//-------------------------------------------------------------------

//____________________________________________________________________________//
new countGiveDamageNSS[MAX_PLAYERS];

hook OnPlayerDisconnect(playerid, reason) {
	countGiveDamageNSS[playerid] = 0;
	return 1;
}

hook OnPlayerConnect(playerid) {
	countGiveDamageNSS[playerid] = 0;
	return 1;
}

task reset_countGiveDamageNSS[5000]() {
	foreach(new playerid : Player) {
		if(countGiveDamageNSS[playerid] > 0) {
			countGiveDamageNSS[playerid] = 0;
		}
	}
}

hook OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid) 
{
	if(gPlayerLogged{playerid} != 1) {
		new String[128];
        format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s, da bi kick ra khoi may chu do su dung Fake Connect Kill (OnPlayerGiveDamage).", GetPlayerNameEx(playerid));
        discord_messHack(String);
		ABroadCast( COLOR_LIGHTRED, String, 2 );
		Log("logs/AntiHackBynDP.log", String);
		Kick(playerid);
		return 1;
	}

	if(weaponid != 0) {
		if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
			new String[128];
	        format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s, da bi kick ra khoi may chu do su dung Hack Weapon On Car (OnPlayerGiveDamage).", GetPlayerNameEx(playerid));
	        discord_messHack(String);
			ABroadCast( COLOR_LIGHTRED, String, 2 );
			Log("logs/AntiHackBynDP.log", String);
			Kick(playerid);
			return 1;
		}
	}

	if(GetPlayerWeapon(playerid) != weaponid) {
		countGiveDamageNSS[playerid] += 1;
		if(countGiveDamageNSS[playerid] == 3) {
			new String[128];
	        format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s, da bi kick do OnPlayerGiveDamage (non server-side).", GetPlayerNameEx(playerid));
	        discord_messHack(String);
			ABroadCast( COLOR_LIGHTRED, String, 2 );
			Log("logs/AntiHackBynDP.log", String);
			Kick(playerid);
			return 1;
		}
	}

	Anti_killall_1(playerid, amount);
	Anti_killall_2(playerid, amount);
	Anti_killall_3(playerid, amount, weaponid);
	Anti_killall_4(playerid, damagedid);
	Anti_killall_5(playerid, weaponid);
	Anti_killall_6(playerid, amount, weaponid);
	return 1;
}

// -------------------------------------------------------------------

hook OnPlayerDeath(playerid, killerid, reason) {
	if(gPlayerLogged{playerid} != 1) {
		new String[128];
        format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s, da bi kick ra khoi may chu do su dung Fake Connect Death (OnPlayerDeath).", GetPlayerNameEx(playerid));
        discord_messHack(String);
		ABroadCast( COLOR_LIGHTRED, String, 2 );
		Log("logs/AntiHackBynDP.log", String);
		Kick(playerid);
		return 1;
	}
	return 1;
}

forward Anti_killall_1(playerid, Float:amount); 
public Anti_killall_1(playerid, Float:amount) 
{
	/*
		Nếu mặc trang phục [0 hoặc 311 > trở lên] mà gây hơn 0 sát thương.
		Nếu không phải Admin.
		Nếu không phải là người trong tổ chức.

		> Kick , Rcon banned
	*/

	if(GetPlayerSkin(playerid) == 0 && amount > 0 || GetPlayerSkin(playerid) >= 311 && amount > 0) 
	{
	    if(PlayerInfo[playerid][pAdmin] == 0 && PlayerInfo[playerid][pMember] == INVALID_GROUP_ID) {
            new String[128];
	        format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s, da bi kick ra khoi may chu do su dung (Kill All [1]).", GetPlayerNameEx(playerid));
	        discord_messHack(String);
			ABroadCast( COLOR_LIGHTRED, String, 2 );
			Log("logs/AntiHackBynDP.log", String);
			Kick(playerid);
			return 1;
		}
	}
	return 1;
}

forward Anti_killall_2(playerid, Float:amount);
public Anti_killall_2(playerid, Float:amount) 
{
	/*
		Nếu người chơi gây sát thương dưới 0dmg		
		> Kick
	*/
	if(amount < 0) {
        new String[128];
        format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s, da bi kick ra khoi may chu do su dung (Kill All [2]).", GetPlayerNameEx(playerid));
        discord_messHack(String);
		ABroadCast( COLOR_LIGHTRED, String, 2 );
		Log("logs/AntiHackBynDP.log", String);
		Kick(playerid);
		return 1;
	}
	return 1;
}

forward Anti_killall_3(playerid, Float:amount, weaponid);
public Anti_killall_3(playerid, Float:amount, weaponid)  
{
	/*
		Nếu người dùng, dùng tay không mà gây dmg trên 10dmg nếu không học võ.
		> Kick
	*/

	if(weaponid == 0 && amount > 10 && PlayerInfo[playerid][pFightStyle] == 0) {
		new String[128];
        format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s, da bi kick ra khoi may chu do su dung (Kill All [3]).", GetPlayerNameEx(playerid));
        discord_messHack(String);
		ABroadCast( COLOR_LIGHTRED, String, 2 );
		Log("logs/AntiHackBynDP.log", String);
		Kick(playerid);
		return 1;
	}
	return 1;
}

stock nearDoor(playerid) {
	new doorID = -1;
	for(new i = 0; i < sizeof(DDoorsInfo); i++) {
        if(IsPlayerInRangeOfPoint(playerid,10.0,DDoorsInfo[i][ddExteriorX], DDoorsInfo[i][ddExteriorY], DDoorsInfo[i][ddExteriorZ]) || IsPlayerInRangeOfPoint(playerid,10.0,DDoorsInfo[i][ddInteriorX], DDoorsInfo[i][ddInteriorY], DDoorsInfo[i][ddInteriorZ])) {
        	doorID = i;
        	break;
        }
    }
	return doorID;
}

stock nearHouse(playerid) {
	new houseID = -1;
	for(new i = 0; i <  sizeof(HouseInfo); i++) {
        if(IsPlayerInRangeOfPoint(playerid, 10.0, HouseInfo[i][hInteriorX], HouseInfo[i][hInteriorY], HouseInfo[i][hInteriorZ]) || IsPlayerInRangeOfPoint(playerid,10, HouseInfo[i][hExteriorX], HouseInfo[i][hExteriorY], HouseInfo[i][hExteriorZ])) {
        	houseID = i;
        	break;
        }
    }
	return houseID;
}

stock nearBiz(playerid) {
	new bizID = -1;
	for(new i = 0; i < sizeof(Businesses); i++) {
        if(IsPlayerInRangeOfPoint(playerid,3,Businesses[i][bIntPos][0], Businesses[i][bIntPos][1], Businesses[i][bIntPos][2]) || IsPlayerInRangeOfPoint(playerid, 10, Businesses[i][bExtPos][0], Businesses[i][bExtPos][1], Businesses[i][bExtPos][2])) {
        	bizID = i;
        	break;
        }
    }
	return bizID;
}

forward Anti_killall_4(playerid, damagedid)  ;
public Anti_killall_4(playerid, damagedid)  
{
	/*
		Nếu người dùng bị bug khoảng cách bắn lên.
		Nếu người dùng không phải Admin.
		Freeze (3s).

		> Kick

		ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "Bug Distance", "{ff0000}Chung toi da kiem tra thay ban dang bi Bug Distance khoang cach ban sung.\nVui long nhan 'Dong y' de chung toi kiem tra.", "Dong y", "");
		
		TogglePlayerControllable(playerid, false);
       	SetTimerEx("unFreezeDistance", 3000, false, "d", playerid);
	*/

	new Float:Distance = GetDistanceBetweenPlayers(damagedid, playerid);
	if(Distance > 400.0 && PlayerInfo[playerid][pAdmin] == 0) {
		if(nearDoor(playerid) != -1 && nearHouse(playerid) == -1 && nearBiz(playerid) == -1 && GetPVarInt(playerid, "IsInArena") == -1 && GetPVarInt( playerid, "EventToken" ) != 1 &&
			nearDoor(damagedid) != -1 && nearHouse(damagedid) == -1 && nearBiz(damagedid) == -1 && GetPVarInt(damagedid, "IsInArena") == -1 && GetPVarInt( damagedid, "EventToken" ) != 1) {
			/*new String[1280];
	        format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s, da bi kick ra khoi may chu do su dung (OnPlayerGiveDamage : Distance %.fm).", GetPlayerNameEx(playerid), Distance);
			ABroadCast( COLOR_LIGHTRED, String, 2 );
			Log("logs/AntiHackBynDP.log", String);
			Kick(playerid);*/

			TogglePlayerControllable(playerid, false);
       		SetTimerEx("unFreezeDistance", 2000, false, "d", playerid);
			return 1;
		}
	}
	return 1;
}

forward unFreezeDistance(playerid);
public unFreezeDistance(playerid)
{
	TogglePlayerControllable(playerid, true);
	return 1;
}

forward Anti_killall_5(playerid, weaponid);
public Anti_killall_5(playerid, weaponid)  
{
	/*
		Nếu người dùng sử dụng vũ khí [ID: 34].
		Không trong faction.
		Không phải admin.
		Không tham gia event PUBG.
		Không tham gia arena.
		Không tham gia Event.

		> Kick
	*/

	if(weaponid == 34 && PlayerInfo[playerid][pMember] == INVALID_GROUP_ID && PlayerInfo[playerid][pAdmin] == 0)
	{
		if(GetPVarInt(playerid, "IsInArena") == -1 && GetPVarInt( playerid, "EventToken" ) == 0) {
			new String[1280];
	        format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s, da bi kick ra khoi may chu do su dung (Kill All [4]).", GetPlayerNameEx(playerid));
			ABroadCast( COLOR_LIGHTRED, String, 2 );
			discord_messHack(String);
			Log("logs/AntiHackBynDP.log", String);
			Kick(playerid);
			return 1;
		}
	}
	return 1;
}

forward Anti_killall_6(playerid, Float:amount, weaponid) ;
public Anti_killall_6(playerid, Float:amount, weaponid)  
{
	/*
		Dưới cấp độ 2 , không trong tổ chức.
		Không tham gia arena, không tham gia sự kiện [Event,PUBG].

		> Kick
	*/

	if(PlayerInfo[playerid][pLevel] < 2 && PlayerInfo[playerid][pMember] == INVALID_GROUP_ID) 
	{
		if(GetPVarInt(playerid, "IsInArena") == -1 && GetPVarInt( playerid, "EventToken" ) == 0) { // && playerPUBG[playerid][pubg_player_join] == 0
			if(weaponid != 0 && amount > 1) { 
				new String[128];

				new WeaponName[1280];
				GetWeaponName(weaponid, WeaponName, sizeof( WeaponName ) );

		        format( String, sizeof( String ), "[ANTI-CHEAT] Nguoi choi %s, da bi kick ra khoi may chu do su dung ([Level : %d - %s]).", GetPlayerNameEx(playerid), PlayerInfo[playerid][pLevel], WeaponName);
				ABroadCast( COLOR_LIGHTRED, String, 2 );
				discord_messHack(String);
				Log("logs/AntiHackBynDP.log", String);
				Kick(playerid);
				return 1;
			}
		}
	}
	return 1;
}
