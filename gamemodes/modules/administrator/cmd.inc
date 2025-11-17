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
