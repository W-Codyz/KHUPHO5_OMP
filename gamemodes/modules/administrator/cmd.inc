#include <YSI/YSI_Coding/y_hooks>
#define         DIALOG_THANHVIEN_ADMIN      (5197)
#define         DIALOG_ADMIN_KICK           (5198)
#define			DIALOG_THANHVIEN_ADVISOR    (5199)
#define         DIALOG_ADVISOR_KICK           (5200)

stock KickAdminOff(playerid, const playername[]) 
{
	new giveplayerid = ReturnUser(playername);
	if(IsPlayerConnected(giveplayerid))
	{
	    if(PlayerInfo[giveplayerid][pAdmin] != 0)
	    {
			new string[1280];
			format(string, sizeof(string), "[ADMIN KICK]: %s da huy bo chuc vu Admin cua %s thanh cong.", GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid));
			Log("logs/adminkickoff.log", string);

			SendClientMessageEx(playerid, COLOR_LIGHTRED, string);
			SendClientMessageEx(giveplayerid, COLOR_LIGHTRED, string);

			PlayerInfo[giveplayerid][pAdmin] = 0;
			return 1;
		}
	}
	else
	{
		new query[128], tmpName[24];
		mysql_escape_string(playername, tmpName);
		format(query, sizeof(query), "UPDATE `accounts` SET `AdminLevel`=0 WHERE `Username`='%s'", tmpName);
		mysql_function_query(MainPipeline, query, false, "AdminTakeOffline", "i", playerid);

        new string[1280];
		format(string, sizeof(string), "[ADMIN KICK OFFLINE]: %s da huy bo chuc vu Admin cua %s thanh cong.", GetPlayerNameEx(playerid), tmpName);
		Log("logs/adminkickoff.log", string);
        SendClientMessageEx(playerid, COLOR_LIGHTRED, string);
        return 1;
	}
	return 1;
}
stock KickAdvisorOff(playerid, const playername[]) 
{
    new giveplayerid = ReturnUser(playername);

    // --- Nếu người đó đang online ---
    if(IsPlayerConnected(giveplayerid))
    {
        if(PlayerInfo[giveplayerid][pHelper] != 0)
        {
            new string[1280];
            format(string, sizeof(string), 
                "[ADVISOR KICK]: %s da huy bo chuc vu Advisor cua %s thanh cong.",
                GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid));

            Log("logs/advkickoff.log", string);

            SendClientMessageEx(playerid, COLOR_LIGHTRED, string);
            SendClientMessageEx(giveplayerid, COLOR_LIGHTRED, string);

            PlayerInfo[giveplayerid][pHelper] = 0;
            return 1;
        }
        else
        {
            SendClientMessageEx(playerid, COLOR_GREY, "> Nguoi choi nay khong phai la Advisor.");
        }
    }

    // --- Nếu người đó offline ---
    else
    {
        new query[256], tmpName[24];
        mysql_escape_string(playername, tmpName);
        format(query, sizeof(query), 
            "UPDATE `accounts` SET `Helper` = 0 WHERE `Username` = '%s'", tmpName);
        
        mysql_function_query(MainPipeline, query, false, "AdvisorTakeOffline", "i", playerid);

        new string[1280];
        format(string, sizeof(string),
            "[ADV KICK OFFLINE]: %s da huy bo chuc vu Advisor cua %s thanh cong.",
            GetPlayerNameEx(playerid), tmpName);
        
        Log("logs/advkickoff.log", string);
        SendClientMessageEx(playerid, COLOR_LIGHTRED, string);
        return 1;
    }

    return 1;
}


hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_THANHVIEN_ADMIN && response == 1)
	{
		new Cache:Result,pNameget[128];
		new count;
		Result = mysql_query(MainPipeline, "SELECT `Username`, `AdminLevel` FROM `accounts` WHERE `AdminLevel` >= 2");
		if(cache_num_rows())
		{
			for(new i = 0; i < cache_num_rows(); i++)
			{
				if(i == listitem)
				{
					cache_get_field_content(i, "Username", pNameget);
					count = cache_get_field_content_int(i, "AdminLevel");
				}
			}
		}
		cache_delete(Result);
		SetPVarString(playerid, "AdminKickName", pNameget);
		new string[1280];
		format(string, sizeof(string), "{FFFFFF}> Ban co dong y huy bo chuc vu Administrator (%d) cua %s hay khong?", count, pNameget);
	 	ShowPlayerDialog(playerid, DIALOG_ADMIN_KICK, DIALOG_STYLE_MSGBOX, "Admin Kick", string, "Dong y", "{ff0000}Huy bo{FFFFFF}");
		return 1;
	}
	if(dialogid == DIALOG_THANHVIEN_ADVISOR && response == 1)
	{
		new Cache:Result, pNameget[128];
		new count;

		Result = mysql_query(MainPipeline, 
			"SELECT `Username`, `Helper` FROM `accounts` WHERE `Helper` >= 1");

		if(cache_num_rows())
		{
			for(new i = 0; i < cache_num_rows(); i++)
			{
				if(i == listitem)
				{
					cache_get_field_content(i, "Username", pNameget);
					count = cache_get_field_content_int(i, "Helper");
				}
			}
		}
		cache_delete(Result);

		SetPVarString(playerid, "AdvisorKickName", pNameget);

		new string[1280];
		format(string, sizeof(string),
			"{FFFFFF}> Ban co dong y huy bo chuc vu Advisor (%d) cua %s hay khong?",
			count, pNameget);

		ShowPlayerDialog(playerid, DIALOG_ADVISOR_KICK, DIALOG_STYLE_MSGBOX, 
			"Advisor Kick", string, "Dong y", "{ff0000}Huy bo{FFFFFF}");
		return 1;
	}

	if(dialogid == DIALOG_ADMIN_KICK && response == 1)
	{
		new name[128];
		GetPVarString(playerid, "AdminKickName", name, sizeof(name));
		KickAdminOff(playerid, name);
		return 1;
	}
	if(dialogid == DIALOG_ADVISOR_KICK && response == 1)
	{
		new name[128];
		GetPVarString(playerid, "AdvisorKickName", name, sizeof(name));
		KickAdvisorOff(playerid, name);
		return 1;
	}
	return 1;
}
CMD:dsadmin(playerid, params[]) {
	if(PlayerInfo[playerid][pAdmin] >= 99999)
	{
		new Cache:Result,pNameget[128],vzstr[50000];
		Result = mysql_query(MainPipeline, "SELECT `LastLogin`, `Username`, `Level`, `AdminLevel` FROM `accounts` WHERE `AdminLevel` >= 2");
		new count;
		new level;
		new LastLogin[255];
		if(cache_num_rows())
		{
			for(new i = 0; i < cache_num_rows(); i++)
			{
				cache_get_field_content(i, "LastLogin", LastLogin);
				cache_get_field_content(i, "Username", pNameget);
				count = cache_get_field_content_int(i, "AdminLevel");
				level = cache_get_field_content_int(i, "Level");
				format(vzstr, sizeof(vzstr), "%s%s\t%d\t%d\t%s\n", vzstr, pNameget, level, count, LastLogin);
			}
		}
		else
		{
			return SendClientMessage(playerid, COLOR_WHITE, "> Hien tai may chu chua co Administrator nao het.");
		}
		cache_delete(Result);
		new danhsach[50000];
		format(danhsach, sizeof(danhsach), "Quan Tri Vien\tCap Do\tChuc Vu\tThoi Gian\n%s", vzstr);
		ShowPlayerDialog(playerid, DIALOG_THANHVIEN_ADMIN, DIALOG_STYLE_TABLIST_HEADERS, "Ho So Admin", danhsach, "Dong y", "Huy bo");
	}
	return 1;
}
CMD:dsadvisor(playerid, params[]) {
    if(PlayerInfo[playerid][pAdmin] >= 1337)
    {
        new Cache:Result, pNameget[128], vzstr[50000];
        Result = mysql_query(MainPipeline, 
            "SELECT `LastLogin`, `Username`, `Level`, `Helper` FROM `accounts` WHERE `Helper` >= 1");
        
        new count, level;
        new LastLogin[255];
        if(cache_num_rows())
        {
            for(new i = 0; i < cache_num_rows(); i++)
            {
                cache_get_field_content(i, "LastLogin", LastLogin);
                cache_get_field_content(i, "Username", pNameget);
                count = cache_get_field_content_int(i, "Helper");
                level = cache_get_field_content_int(i, "Level");
                format(vzstr, sizeof(vzstr), "%s%s\t%d\t%d\t%s\n", 
                    vzstr, pNameget, level, count, LastLogin);
            }
        }
        else
        {
            return SendClientMessage(playerid, COLOR_WHITE, "> Hien tai may chu chua co Advisor nao het.");
        }

        cache_delete(Result);
        new danhsach[50000];
        format(danhsach, sizeof(danhsach), 
            "Quan Tri Vien\tCap Do\tChuc Vu\tThoi Gian\n%s", vzstr);
        
        ShowPlayerDialog(playerid, DIALOG_THANHVIEN_ADVISOR, DIALOG_STYLE_TABLIST_HEADERS, 
            "Ho So Advisor", danhsach, "Dong y", "Huy bo");
    }
    return 1;
}


CMD:kickallbaotri(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] == 99999)
	{
	    for(new i = 0; i < MAX_PLAYERS; i++){
	        SendClientMessageEx(i, COLOR_GRAD2, "{ff4400}>{ffffff} May chu dang bao tri. Xin loi vi su bat tien nay. <3");
			Kick(i);
		}
	}
	return 1;
}

CMD:baotri(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 99999) {
        SendClientMessageEx(playerid, COLOR_GRAD2, "{ff4400}>{ffffff} Ban khong duoc phep su dung lenh nay.");
        return 1;
    }

    SetTimer( "Maintenance", 30000, false );
    SendClientMessageToAllEx(COLOR_LIGHTBLUE, "*SERVER SE NGAT KET NOI SAU 30 GIAY DE BAO TRI, VUI LONG THOAT GAME");
    foreach(new i: Player)
	{
        GameTextForPlayer(i, "~y~Bao tri may chu", 5000, 6);
    }
    return 1;
}

//HELP CMD
CMD:mole1(playerid){
	pc_cmd_mole(playerid,"Ban muon kiem tien $?, ban that nghiep, (/map >> tim viec lam) se giup ban co 1 cong viec tot");
	return 1;
}
CMD:mole2(playerid){
	pc_cmd_mole(playerid,"Ban muon tro chuyen cung moi nguoi? (/map >> tim duong) 24/7 mua 1 radio lien lac de noi chuyen");
	return 1;
}

CMD:mole3(playerid){
	pc_cmd_mole(playerid,"Ban gap kho khan?, dung lo hay (/baocao) de goi ho tro vien ra ho tro ban bat cu luc nao");
	return 1;
}

CMD:mole4(playerid){
	pc_cmd_mole(playerid,"Ban muon kiem tien ?, khong lam cung co an => /map -> tim duong -> Song Bai");
	return 1;
}

CMD:1(playerid){
	pc_cmd_m(playerid,"Yeu Cau Xe Phia Truoc Tap Vao Le Now !!");
	return 1;
}

CMD:2(playerid){
	pc_cmd_m(playerid,"Yeu cau tat ca gio hai tay len dau (( /handsup )) bo sung xuong !");
	return 1;
}

CMD:3(playerid){
	pc_cmd_m(playerid,"Yeu Cau Anh Gio Tay Đau Hang (( /Handsup )) Va Hop Tac Now. Anh Co 5s De Lam Viec Do");
	return 1;
}

CMD:4(playerid){
	pc_cmd_m(playerid,"Yeu cau nguoi khong phan su khong duoc di theo khong chung toi se no sung");
	return 1;
}

CMD:5(playerid){
	pc_cmd_m(playerid,"Yeu Cau Ai Khong Phan Su Di Ra Khoi Khu Vuc Neu Khong Chung Toi Se Dung Bien Phap Manh Anh Co 5s Đe Lam Viec Do");
	return 1;
}