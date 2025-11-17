#include <YSI/YSI_Coding/y_hooks>

	//-------------------------------------------------------------------
	//					Project create by #nDP 						   //
	//					Fb.com/nguyenduyphuong.com  			       //
	//					Copyright by nDP" 							   //
	//-------------------------------------------------------------------

//____________________________________________________________________________//

CMD:fakename(playerid, params[]) {
	if(PlayerInfo[playerid][pAdmin] < 2) return 1;
	foreach(new i : Player) {
		if(gPlayerLogged{i} == 1 && PlayerInfo[i][pAdmin] == 0) {
			new name[24];
			GetPlayerName(i, name, sizeof(name));

			if(strcmp(name, GetPlayerNameSQLId(GetPlayerSQLId(i)), true) != 0) {
				new string[1280];
				format(string, sizeof(string), "[ANTI-CHEAT] %s da bi kick do ten nhan vat khong phu hop trong du lieu may chu.", GetPlayerNameEx(i));
				ABroadCast(COLOR_YELLOW, string, 2);
				Log("logs/AntiHackBynDP.log", string);
				discord_messHack(string);
				Kick(i);
				return 1;
			}
		}
	}
	return 1;
}

forward fakeOnline_weapon(playerid);
public fakeOnline_weapon(playerid) {
	if(gPlayerLogged{playerid} == 0) 
	{
		if(GetPlayerWeapon(playerid) > 0) {
			new string[1280];
			format(string, sizeof(string), "[ANTI-CHEAT] %s da bi kick ra khoi may chu do su dung Hack FakeConnect [1].", GetPlayerNameEx(playerid));
			ABroadCast(COLOR_YELLOW, string, 2);
			discord_messHack(string);

			Log("logs/AntiHackBynDP.log", string);
			Kick(playerid);
			return 1;
		}
	}
	return 1;
}

forward fakeOnline_move(playerid);
public fakeOnline_move(playerid) {
	/*
		T� i khoản của người chơi phải được đăng ký.
		Kiểm tra t� i khoản n� y có tồn tại trong CSDL không v�  nếu tốc độ người n� y lớn hơn 5
		> Kick.
	*/
	if(PlayerInfo[playerid][pTut] != 0) {
		if(GetPlayerSQLId(playerid) == -1 && GetSpeedPlayer(playerid) > 5 && !IsAHitman(playerid)) {
			new string[1280];
			format(string, sizeof(string), "[ANTI-CHEAT] %s da bi kick ra khoi may chu do su dung Hack FakeConnect [2].", GetPlayerNameEx(playerid));
			ABroadCast(COLOR_YELLOW, string, 2);
			discord_messHack(string);
			Log("logs/AntiHackBynDP.log", string);
			Kick(playerid);
			return 1;
		}
	}
	return 1;
}

forward fakeOnline_name(playerid);
public fakeOnline_name(playerid) {
	/*
		Người chơi n� y đã đâng nhập, v�  không phải l�  Admin v�  Hitman
		Kiểm tra tên người chơi v�  tên trong CSDL.
		> Kick.
	*/
	if(gPlayerLogged{playerid} == 1 && PlayerInfo[playerid][pAdmin] == 0 && !IsAHitman(playerid) && GetPVarInt(playerid, "useMasked") == 0) {
		new name[24];
		GetPlayerName(playerid, name, sizeof(name));

		if(strcmp(name, GetPlayerNameSQLId(GetPlayerSQLId(playerid)), true) != 0) 
		{
			new string[1280];
			format(string, sizeof(string), "[ANTI-CHEAT] %s da bi kick ra khoi may chu do su dung Hack FakeConnect [3].", GetPlayerNameEx(playerid));
			ABroadCast(COLOR_YELLOW, string, 2);
			discord_messHack(string);

			Log("logs/AntiHackBynDP.log", string);
			Kick(playerid);
			return 1;
		}
	}
	return 1;
}

stock GetPlayerNameSQLId(id) 
{
	new Cache:Result;
	new pNameget[120];
	new sql[1280];
	format(sql, sizeof(sql), "select `Username` from `accounts` where `id` = %d", id);
	Result = mysql_query(MainPipeline, sql);
	if(cache_num_rows())
	{
		for(new i = 0; i < cache_num_rows(); i++)
		{
			cache_get_field_content(i, "Username", pNameget);
		}
	}
	cache_delete(Result);
	return pNameget;
}

forward fakeOnline_speed(playerid);
public fakeOnline_speed(playerid) {
	if(GetPlayerSkin(playerid) == 311 && GetSpeedPlayer(playerid) > 20 || GetPlayerSkin(playerid) == 0 && GetSpeedPlayer(playerid) > 20) {
		if(PlayerInfo[playerid][pAdmin] == 0) {
			new string[1280];
			format(string, sizeof(string), "[ANTI-CHEAT] %s da bi kick ra khoi may chu do su dung Hack FakeConnect [4].", GetPlayerNameEx(playerid));
			ABroadCast(COLOR_YELLOW, string, 2);
			discord_messHack(string);

			Log("logs/AntiHackBynDP.log", string);
			Kick(playerid);
			return 1;
		}
	}

	return 1;
}
