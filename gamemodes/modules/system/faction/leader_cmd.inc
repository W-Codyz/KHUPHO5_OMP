#include <YSI\YSI_Coding\y_hooks>

#include "./modules/system/faction/leader_dialog"

CMD:thanhvien(playerid,params[])
{
	if(PlayerInfo[playerid][pLeader] == INVALID_GROUP_ID) 
		return SendClientMessageEx(playerid, COLOR_GRAD1, "Chi co Leader moi co the su dung lenh nay.");
	
	new query[256], iGroupID = PlayerInfo[playerid][pLeader];
	
	format(query, sizeof(query), 
		"SELECT `id`, `Username`, `Rank`, `Division`, `LastLogin` FROM `accounts` WHERE `Member` = %d ORDER BY `Rank` DESC, `Username` ASC", 
		iGroupID);
	
	mysql_function_query(MainPipeline, query, true, "OnLoadMemberList", "i", playerid);
	SendClientMessageEx(playerid, COLOR_YELLOW, "Dang tai danh sach thanh vien...");
	return 1;
}

forward OnLoadMemberList(playerid);
public OnLoadMemberList(playerid)
{
	new rows, fields;
	cache_get_data(rows, fields, MainPipeline);
	
	if(rows == 0)
	{
		SendClientMessageEx(playerid, COLOR_GRAD1, "Khong co thanh vien nao trong nhom!");
		return 1;
	}
	
	new iGroupID = PlayerInfo[playerid][pLeader];
	new szDialog[2048], szUsername[MAX_PLAYER_NAME], szLastLogin[64], szLine[128];
	new iRank, iDivision, iSQLID;
	new bool:isOnline;
	
	strcat(szDialog, "{FFD700}STT\t{FFFFFF}Ten\t{87CEEB}Rank\t{33FF33}Trang thai\n");
	
	for(new i = 0; i < rows; i++)
	{
		iSQLID = cache_get_field_content_int(i, "id", MainPipeline);
		cache_get_field_content(i, "Username", szUsername, MainPipeline);
		cache_get_field_content(i, "LastLogin", szLastLogin, MainPipeline);
		iRank = cache_get_field_content_int(i, "Rank", MainPipeline);
		iDivision = cache_get_field_content_int(i, "Division", MainPipeline);
		
		// Kiem tra xem nguoi choi co online khong
		isOnline = false;
		foreach(new x: Player)
		{
			if(PlayerInfo[x][pId] == iSQLID)
			{
				isOnline = true;
				break;
			}
		}
		
		if(isOnline)
		{
			format(szLine, sizeof(szLine), "{FFFFFF}%d\t{FFFFFF}%s\t{87CEEB}%s (%d)\t{33FF33}Online\n", 
				i + 1, szUsername, arrGroupRanks[iGroupID][iRank], iRank);
		}
		else
		{
			format(szLine, sizeof(szLine), "{FFFFFF}%d\t{FFFFFF}%s\t{87CEEB}%s (%d)\t{AAAAAA}Offline\n", 
				i + 1, szUsername, arrGroupRanks[iGroupID][iRank], iRank);
		}
		
		strcat(szDialog, szLine);
	}
	
	new szTitle[128];
	format(szTitle, sizeof(szTitle), "{FFFFFF}DANH SACH THANH VIEN - %s", arrGroupData[iGroupID][g_szGroupName]);
	ShowPlayerDialog(playerid, DIALOG_MEMBER_LIST, DIALOG_STYLE_TABLIST_HEADERS, szTitle, szDialog, "Chon", "Dong");
	
	return 1;
}

CMD:online(playerid, params[]) {
    if(PlayerInfo[playerid][pLeader] >= 0)
	{

		new
			szDialog[1024];

		foreach(new i: Player)
		{
			if(IsATaxiDriver(playerid) && IsATaxiDriver(i)) switch(TransportDuty[i]) {
				case 1: format(szDialog, sizeof(szDialog), "%s\n* %s (on duty), %i calls accepted", szDialog, GetPlayerNameEx(i), PlayerInfo[i][pCallsAccepted]);
				default: format(szDialog, sizeof(szDialog), "%s\n* %s (off duty), %i calls accepted", szDialog, GetPlayerNameEx(i), PlayerInfo[i][pCallsAccepted]);
			}
			else if(IsAMedic(playerid) && IsAMedic(i) && (arrGroupData[PlayerInfo[playerid][pMember]][g_iAllegiance] == arrGroupData[PlayerInfo[i][pMember]][g_iAllegiance])) switch(PlayerInfo[i][pDuty]) {
				case 1: format(szDialog, sizeof(szDialog), "%s\n* %s (on duty), %i calls accepted, %i patients delivered.", szDialog, GetPlayerNameEx(i), PlayerInfo[i][pCallsAccepted], PlayerInfo[i][pPatientsDelivered]);
				default: format(szDialog, sizeof(szDialog), "%s\n* %s (off duty), %i calls accepted, %i patients delivered.", szDialog, GetPlayerNameEx(i), PlayerInfo[i][pCallsAccepted], PlayerInfo[i][pPatientsDelivered]);
			}
			else if(PlayerInfo[i][pMember] == PlayerInfo[playerid][pLeader]) switch(PlayerInfo[i][pDuty]) {
				case 1: format(szDialog, sizeof(szDialog), "%s\n* %s (on duty)", szDialog, GetPlayerNameEx(i));
				default: format(szDialog, sizeof(szDialog), "%s\n* %s (off duty)", szDialog, GetPlayerNameEx(i));
			}
		}
		if(!isnull(szDialog)) {
		    strdel(szDialog, 0, 1);
			ShowPlayerDialog(playerid, 0, DIALOG_STYLE_LIST, "Online Members", szDialog, "Lua chon", "Huy bo");
		}
		else SendClientMessageEx(playerid, COLOR_GREY, "Khong co thanh vien online luc nay.");

    }  else SendClientMessageEx(playerid, COLOR_GREY, "Chi co leader Group moi co the su dung duoc lenh nay.");
    return 1;
}

CMD:uninvite(playerid, params[]) {
	if(0 <= PlayerInfo[playerid][pLeader] < MAX_GROUPS) {

		new
			iTargetID,
			iGroupID = PlayerInfo[playerid][pLeader];

		if(sscanf(params, "u", iTargetID)) {
			SendClientMessageEx(playerid, COLOR_GREY, "SU DUNG: /uninvite [Player]");
		}
		else if(IsPlayerConnected(iTargetID)) {
			if(iGroupID == PlayerInfo[iTargetID][pMember]) {
				if(playerid == iTargetID) {
					SendClientMessageEx(playerid, COLOR_GREY, "Ban khong the duoi chinh minh.");
				}
				else if(PlayerInfo[playerid][pRank] > PlayerInfo[iTargetID][pRank] || PlayerInfo[playerid][pRank] >= Group_GetMaxRank(iGroupID)) {

					new
						szMessage[128],
						iRank = PlayerInfo[playerid][pRank];

					format(szMessage, sizeof szMessage, "%s %s da duoi ban ra khoi %s.", arrGroupRanks[iGroupID][iRank], GetPlayerNameEx(playerid), arrGroupData[iGroupID][g_szGroupName]);
					SendClientMessageEx(iTargetID, COLOR_LIGHTBLUE, szMessage);
					SendClientMessageEx(iTargetID, COLOR_WHITE, "Ban da tro thanh mot nguoi dan binh thuong.");

					format(szMessage, sizeof szMessage, "Ban da duoi %s ra khoi nhom.", GetPlayerNameEx(iTargetID));
					SendClientMessageEx(playerid, COLOR_LIGHTBLUE, szMessage);

					format(szMessage, sizeof szMessage, "%s %s (rank %i) da bi duoi %s (rank %i) tu %s (%i).", arrGroupRanks[iGroupID][iRank], GetPlayerNameEx(playerid), iRank, GetPlayerNameEx(iTargetID), PlayerInfo[iTargetID][pRank], arrGroupData[iGroupID][g_szGroupName], iGroupID + 1);
					Log("logs/group.log", szMessage);

					PlayerInfo[iTargetID][pMember] = INVALID_GROUP_ID;
					PlayerInfo[iTargetID][pDivision] = -1;
					PlayerInfo[iTargetID][pLeader] = INVALID_GROUP_ID;
					PlayerInfo[iTargetID][pDuty] = 0;
					PlayerInfo[iTargetID][pPreDutyModel] = 0;
					PlayerInfo[iTargetID][pRank] = INVALID_RANK;
					PlayerInfo[iTargetID][pModel] = NOOB_SKIN;
					SetPlayerSkin(iTargetID, NOOB_SKIN);

					SetPlayerToTeamColor(iTargetID);
					pTazer{iTargetID} = 0;
				}
				else SendClientMessageEx(playerid, COLOR_GREY, "Ban khong the lam dieu nay voi nguoi cung Rank hoac cao hon.");
			}
			else SendClientMessageEx(playerid, COLOR_GRAD1, "Nguoi do khong o trong nhom cua ban.");
		}
		else SendClientMessageEx(playerid, COLOR_GRAD1, "nguoi choi khong hop le.");
	}
	else if(PlayerInfo[playerid][pFMember] != INVALID_FAMILY_ID && PlayerInfo[playerid][pRank] >= 5)
	{
		new string[128], giveplayerid;
		new family = PlayerInfo[playerid][pFMember];
		if(sscanf(params, "u", giveplayerid)) return SendClientMessageEx(playerid, COLOR_GREY, "SU DUNG: /uninvite [Player]");
		if(IsPlayerConnected(giveplayerid))
		{
			if(PlayerInfo[giveplayerid][pFMember] != PlayerInfo[playerid][pFMember])
			{
				SendClientMessageEx(playerid, COLOR_GREY, "Nguoi do khong o trong family.");
				return 1;
			}
			if(PlayerInfo[giveplayerid][pRank] > PlayerInfo[playerid][pRank])
			{
				SendClientMessageEx(playerid, COLOR_GREY, "Ban khong the duoi nguoi co rank cao hon minh.");
				return 1;
			}
			new file[32], month, day, year ;
			getdate(year,month,day);
			format(string, sizeof(string), "* Ban da duoi %s ra khoi family.",GetPlayerNameEx(giveplayerid));
			SendClientMessageEx(playerid, COLOR_LIGHTBLUE, string);
			format(string, sizeof(string), "* Family leader %s da duoi ban ra khoi family.",GetPlayerNameEx(playerid));
			SendClientMessageEx(giveplayerid, COLOR_LIGHTBLUE, string);
			PlayerInfo[giveplayerid][pFMember] = INVALID_FAMILY_ID;
			PlayerInfo[giveplayerid][pRank] = INVALID_RANK;
			FamilyInfo[family][FamilyMembers] --;
			SaveFamilies();
			format(string, sizeof(string), "%s uninvited %s from %s.", GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid), FamilyInfo[family][FamilyName]);
			format(file, sizeof(file), "family_logs/%d/%d-%02d-%02d.log", family, year, month, day);
			Log(file, string);
			return 1;
		}
		else
		{
			SendClientMessageEx(playerid, COLOR_GREY, "nguoi choi khong hop le.");
			return 1;
		}
	}
	else SendClientMessageEx(playerid, COLOR_GRAD1, "Chi Leader moi co quyen su dung lenh nay.");
	return 1;
}

CMD:ouninvite(playerid, params[]) {
	if(0 <= PlayerInfo[playerid][pLeader] < MAX_GROUPS) {
		if(!isnull(params)) {

			if (IsPlayerConnected(ReturnUser(params)))
			{
				return SendClientMessageEx(playerid, COLOR_GREY, "Nguoi do dang online - su dung /uninvite.");
			}
			new
				szQuery[96],
				szName[MAX_PLAYER_NAME],
				iPos;

			mysql_escape_string(params, szName);
			format(szQuery, sizeof szQuery, "SELECT `Member`, `Rank`, `id` FROM `accounts` WHERE `Username` = '%s'", szName);
			mysql_function_query(MainPipeline, szQuery, true, "Group_QueryFinish", "ii", GROUP_QUERY_UNCHECK, playerid);

			while((iPos = strfind(szName, "_", false, iPos)) != -1) szName[iPos] = ' ';
			SetPVarString(playerid, "Group_Uninv", szName);

			format(szQuery, sizeof szQuery, "Dang co gang loai bo %s ra khoi nhom, xin vui long doi...", szName);
			SendClientMessageEx(playerid, COLOR_LIGHTBLUE, szQuery);
		}
		else SendClientMessageEx(playerid, COLOR_WHITE, "SU DUNG: /ouninvite [account name]");
	}
	else if(PlayerInfo[playerid][pFMember] != INVALID_FAMILY_ID && PlayerInfo[playerid][pRank] >= 5)
	{
		if(isnull(params)) {
		return SendClientMessageEx(playerid, COLOR_WHITE, "SU DUNG: /ouninvite [name]");
		}

		new query[512], tmpName[24];
		mysql_escape_string(params, tmpName);
		SetPVarString(playerid, "OnUninvite", tmpName);

		format(query,sizeof(query),"UPDATE `accounts` SET `FMember` = 255, `Rank` = %d,`Model` = %d WHERE `Username`='%s' \
			AND `GangModerator`=0 \
			AND `AdminLevel` < 4 \
			AND `FMember`=%d \
			AND `Rank` < %d",
			INVALID_RANK,
			CIV[random(sizeof(CIV))],
			tmpName,
			PlayerInfo[playerid][pFMember],
			PlayerInfo[playerid][pRank]
		);
		mysql_function_query(MainPipeline, query, false, "OnUninvite", "i", playerid);

		new string[128];
		format(string, sizeof(string), "Da duoi %s ra khoi family...", tmpName);
		SendClientMessageEx(playerid, COLOR_LIGHTBLUE, string);
	}
	else SendClientMessageEx(playerid, COLOR_GRAD1, "Chi co Leader moi co the su dung lenh nay.");
	return 1;
}

CMD:giverank(playerid, params[]) {
	if(0 <= PlayerInfo[playerid][pLeader] < MAX_GROUPS) {

		new
			iTargetID,
			iRank,
			iGroupID = PlayerInfo[playerid][pLeader],
            szMessage[128];

		if(sscanf(params, "ui", iTargetID, iRank)) {
			SendClientMessageEx(playerid, COLOR_GREY, "SU DUNG: /giverank [Player] [rank]");
		}
		else if(!(0 <= iRank <= Group_GetMaxRank(iGroupID))) {
		    format(szMessage, sizeof(szMessage), "Rank khong dung (phai tu 0 va %d)", Group_GetMaxRank(iGroupID));
			SendClientMessageEx(playerid, COLOR_GREY, szMessage);
		}
		else if(IsPlayerConnected(iTargetID)) {
			if(iGroupID == PlayerInfo[iTargetID][pMember]) {
				if(iRank == PlayerInfo[iTargetID][pRank]) {
					SendClientMessageEx(playerid, COLOR_GREY, "Nguoi do da co rank nay.");
				}
				else if(playerid == iTargetID) {
					SendClientMessageEx(playerid, COLOR_GREY, "Ban khong the thay doi rank cua ban!");
				}
				if(PlayerInfo[iTargetID][pRank] > PlayerInfo[playerid][pRank])
		    	{
			        SendClientMessageEx(playerid, COLOR_GRAD1, "Ban khong the thuc hien lenh nay voi nguoi rank cao hon ban!");
			        return 1;
		    	}
				else if(PlayerInfo[playerid][pRank] > PlayerInfo[iTargetID][pRank] || PlayerInfo[playerid][pRank] >= Group_GetMaxRank(iGroupID) || PlayerInfo[playerid][pAdmin] >= 4) {

					format(szMessage, sizeof szMessage, "%s %s co %s ban rank %s.", arrGroupRanks[iGroupID][PlayerInfo[playerid][pRank]], GetPlayerNameEx(playerid), ((iRank > PlayerInfo[iTargetID][pRank]) ? ("promoted") : ("demoted")), arrGroupRanks[iGroupID][iRank]);
					SendClientMessageEx(iTargetID, COLOR_LIGHTBLUE, szMessage);

					format(szMessage, sizeof szMessage, "Ban co %s %s de rank cua %s.", ((iRank > PlayerInfo[iTargetID][pRank]) ? ("promoted") : ("demoted")), GetPlayerNameEx(iTargetID), arrGroupRanks[iGroupID][iRank]);
					SendClientMessageEx(playerid, COLOR_LIGHTBLUE, szMessage);

					format(szMessage, sizeof szMessage, "%s %s (rank %i) da cho %s rank %i (%s) trong %s (%i).", arrGroupRanks[iGroupID][PlayerInfo[playerid][pRank]], GetPlayerNameEx(playerid), PlayerInfo[playerid][pRank], GetPlayerNameEx(iTargetID), iRank, arrGroupRanks[iGroupID][iRank], arrGroupData[iGroupID][g_szGroupName], iGroupID + 1);
					Log("logs/group.log", szMessage);

					PlayerInfo[iTargetID][pRank] = iRank;
				}
				else SendClientMessageEx(playerid, COLOR_GREY, "Ban khong the lam dieu nay voi nguoi co Rank bang ban hoac cao hon.");
			}
			else SendClientMessageEx(playerid, COLOR_GRAD1, "Nguoi do khong co trong nhom cua ban.");
		}
		else SendClientMessageEx(playerid, COLOR_GRAD1, "nguoi choi khong hop le.");
	}
	else if(PlayerInfo[playerid][pFMember] != INVALID_FAMILY_ID && PlayerInfo[playerid][pRank] >= 5) {
		new string[128], giveplayerid, rank;
		new family = PlayerInfo[playerid][pFMember];
		if(sscanf(params, "ud", giveplayerid, rank)) return SendClientMessageEx(playerid, COLOR_GREY, "SU DUNG: /giverank [Player] [Rank (1-6)]");

		if(PlayerInfo[playerid][pRank] == 6)
		{
			if(rank > 6 || rank < 0) { SendClientMessageEx(playerid, COLOR_GREY, "   Khong duoc duoi 0, hoac tren 6!"); return 1; }
		}
		else if(PlayerInfo[playerid][pRank] == 5)
		{
			if(rank > 5 || rank < 0) { SendClientMessageEx(playerid, COLOR_GREY, "   Khong duoc duoi 0, hoac tren 5!"); return 1; }
		}

		if(IsPlayerConnected(giveplayerid))
		{
		    if(PlayerInfo[giveplayerid][pRank] > PlayerInfo[playerid][pRank])
		    {
		        SendClientMessageEx(playerid, COLOR_GRAD1, "Ban khong the thuc hien dieu nay voi nguoi cung cap hoac cao hon!");
		        return 1;
		    }
			if(PlayerInfo[playerid][pFMember] != PlayerInfo[giveplayerid][pFMember])
			{
				SendClientMessageEx(playerid, COLOR_GRAD1, "   Nguoi nay khong trong Family cua ban!");
				return 1;
			}

			if(rank > PlayerInfo[giveplayerid][pRank])
			{
				format(string, sizeof(string), "   Ban da duoc thang len mot cap cao hon boi %s.", GetPlayerNameEx(playerid));
			}
			if(rank < PlayerInfo[giveplayerid][pRank])
			{
				format(string, sizeof(string), "   Ban da bi ha mot cap thap hon boi %s.", GetPlayerNameEx(playerid));
			}
			new file[32], month, day, year;
			getdate(year,month,day);
			format(string, sizeof(string), "* Ban cho %s rank %d.",GetPlayerNameEx(giveplayerid),rank);
			SendClientMessageEx(playerid, COLOR_LIGHTBLUE, string);
			format(string, sizeof(string), "* Family leader %s da cho ban rank %d.",GetPlayerNameEx(playerid),rank);
			SendClientMessageEx(giveplayerid, COLOR_LIGHTBLUE, string);
			new temprank = PlayerInfo[giveplayerid][pRank];
			PlayerInfo[giveplayerid][pRank] = rank;
			format(string, sizeof(string), "%s set %s rank from %d to %d in %s.", GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid), temprank, rank, FamilyInfo[family][FamilyName]);
			format(file, sizeof(file), "family_logs/%d/%d-%02d-%02d.log", family, year, month, day);
			Log(file, string);
			return 1;
		}
		else
		{
			SendClientMessageEx(playerid, COLOR_GREY, "nguoi choi khong hop le.");
			return 1;
		}
	}
	else SendClientMessageEx(playerid, COLOR_GRAD1, "Only group leaders may use this command.");
	return 1;
}

CMD:setdiv(playerid, params[]) {
	if(0 <= PlayerInfo[playerid][pLeader] < MAX_GROUPS) {

		new
			iTargetID,
			iDiv,
			iGroupID = PlayerInfo[playerid][pLeader],
			szMessage[128];

		if(sscanf(params, "ui", iTargetID, iDiv)) {
			SendClientMessageEx(playerid, COLOR_GREY, "SU DUNG: /setdiv [Player] [div]");
			format(szMessage, sizeof(szMessage), "%s", "0 (None), ");
			for(new i; i < MAX_GROUP_DIVS; i++)
			{
			    if(arrGroupDivisions[iGroupID][i][0]) format(szMessage, sizeof(szMessage), "%s%d (%s), ", szMessage, i+1, arrGroupDivisions[iGroupID][i]);
				if(strlen(szMessage) > 64 || i == (MAX_GROUP_DIVS -1) && strlen(szMessage)) { SendClientMessageEx(playerid, COLOR_GRAD2, szMessage); szMessage[0] = 0; }

			}
		}
		else if(!(0 <= iDiv <= Group_GetMaxDiv(iGroupID)+1)) {
		    format(szMessage, sizeof(szMessage), "DIV khong hop le (Phai tu 0 den %d)", Group_GetMaxDiv(iGroupID) + 1);
			SendClientMessageEx(playerid, COLOR_GREY, szMessage);
		}
		else if(IsPlayerConnected(iTargetID)) {
			if(iGroupID == PlayerInfo[iTargetID][pMember]) {
				if(iDiv - 1 == PlayerInfo[iTargetID][pDivision]) {
					if (iDiv == 0) SendClientMessageEx(playerid, COLOR_GREY, "Nguoi do khong co DIV.");
					else SendClientMessageEx(playerid, COLOR_GREY, "Nguoi do da co DIV nay roi.");
				}
				else if(PlayerInfo[playerid][pLeader] == iGroupID || PlayerInfo[playerid][pDivision] == PlayerInfo[iTargetID][pDivision] || PlayerInfo[playerid][pRank] >= (Group_GetMaxRank(iGroupID) - 3)) {

					if(iDiv == 0)
					{
						format(szMessage, sizeof(szMessage), "Ban da bi duoi ra khoi bo phan hien tai boi %s.", GetPlayerNameEx(playerid));
						SendClientMessageEx(iTargetID, COLOR_LIGHTBLUE, szMessage);
						format(szMessage, sizeof(szMessage), "Ban da duoi %s ra khoi bo phan hien tai.", GetPlayerNameEx(iTargetID));
						SendClientMessageEx(playerid, COLOR_LIGHTBLUE, szMessage);
						format(szMessage, sizeof szMessage, "%s %s da duoi %s ra khoi bo phan %s (%d).", arrGroupRanks[iGroupID][PlayerInfo[playerid][pRank]], GetPlayerNameEx(playerid), GetPlayerNameEx(iTargetID), arrGroupData[iGroupID][g_szGroupName], iGroupID + 1);
						Log("logs/group.log", szMessage);
					}
					else
					{
						format(szMessage, sizeof szMessage, "%s %s da thiet lap cho ban %s DIV.", arrGroupRanks[iGroupID][PlayerInfo[playerid][pRank]], GetPlayerNameEx(playerid), arrGroupDivisions[iGroupID][iDiv-1]);
						SendClientMessageEx(iTargetID, COLOR_LIGHTBLUE, szMessage);
						format(szMessage, sizeof szMessage, "Ban da dua %s vao bo phan %s DIV", GetPlayerNameEx(iTargetID), arrGroupDivisions[iGroupID][iDiv-1]);
						SendClientMessageEx(playerid, COLOR_LIGHTBLUE, szMessage);
						format(szMessage, sizeof szMessage, "%s %s da dua %s's DIV den %s trong %s (%d).", arrGroupRanks[iGroupID][PlayerInfo[playerid][pRank]], GetPlayerNameEx(playerid), GetPlayerNameEx(iTargetID), arrGroupDivisions[iGroupID][iDiv-1], arrGroupData[iGroupID][g_szGroupName], iGroupID + 1);
						Log("logs/group.log", szMessage);
					}
					PlayerInfo[iTargetID][pDivision] = iDiv-1;
				}
				else SendClientMessageEx(playerid, COLOR_GREY, "Ban khong du tham quyen de phan chi cac bo phan.");
			}
			else SendClientMessageEx(playerid, COLOR_GRAD1, "Nguoi do khong trong nhom cua ban.");
		}
		else SendClientMessageEx(playerid, COLOR_GRAD1, "nguoi choi khong hop le.");
	}
	else if(PlayerInfo[playerid][pFMember] != 0 && PlayerInfo[playerid][pRank] >= 5) //Gotta do it like this since we dont have  Gang Leader system
	{
	    new
			iFamily = PlayerInfo[playerid][pFMember],
			iDiv,
			targetid,
			szMessage[128];

	    if(sscanf(params, "ui", targetid, iDiv))
	    {
	        SendClientMessageEx(playerid, COLOR_GREY, "SU DUNG: /setdiv [Player] [division]");
	    } else {
	        if(IsPlayerConnected(targetid))
	        {
	            if(PlayerInfo[targetid][pFMember] == PlayerInfo[playerid][pFMember])
	            {
	                if(PlayerInfo[targetid][pRank] <= PlayerInfo[playerid][pRank])
	                {
	                    if(iDiv != PlayerInfo[targetid][pDivision])
	                    {
                            if(0 <= iDiv <= 5)
							{
								new file[32], month, day, year ;
								getdate(year,month,day);
       							format(szMessage, sizeof(szMessage), "Ban da duoc chuyen den bo phan %s boi %s.", FamilyDivisionInfo[iFamily][iDiv], GetPlayerNameEx(playerid));
              					SendClientMessageEx(targetid, COLOR_LIGHTBLUE, szMessage);
                   				format(szMessage, sizeof(szMessage), "Ban da chuyen %s de bo phan %s.", GetPlayerNameEx(targetid), FamilyDivisionInfo[iFamily][iDiv]);
                       			SendClientMessageEx(playerid, COLOR_LIGHTBLUE, szMessage);
                       			format(szMessage, sizeof(szMessage), "%s da di chuyen %s den bo phan %s.", GetPlayerNameEx(playerid), GetPlayerNameEx(targetid), FamilyDivisionInfo[iFamily][iDiv]);
								format(file, sizeof(file), "family_logs/%d/%d-%02d-%02d.log", iFamily, year, month, day);
								Log(file, szMessage);
								PlayerInfo[targetid][pDivision] = iDiv;
							}
					  		else return SendClientMessageEx(playerid, COLOR_GREY, "ID phan chia cac bo phan khong hop le, vui long chon giua 0-4");
  						}
						else return SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi nay dang trong bo phan do!");
					}
					else return SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi nay co Rank cao hon ban!");
				}
				else return SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi nay khong o trong family/gang!");
			}
			else return SendClientMessageEx(playerid, COLOR_GREY, "nguoi choi khong hop le.");
	    }
	}
	else
	    return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong duoc phep su dung lenh nay!");
	return 1;
}
// ==================== XU LY DIALOG QUAN LY THANH VIEN ====================

