
// PVar de luu thong tin thanh vien dang quan ly
// pMemberManage_Name - Ten thanh vien
// pMemberManage_ID - SQL ID cua thanh vien
// pMemberManage_Rank - Rank hien tai
// pMemberManage_Division - Division hien tai
// pMemberManage_IsOnline - 1 neu online, 0 neu offline

#define DIALOG_MEMBER_LIST 9250
#define DIALOG_MEMBER_MANAGE 9251
#define DIALOG_MEMBER_GIVERANK 9252
#define DIALOG_MEMBER_SETDIV 9253
#define DIALOG_MEMBER_KICK_CONFIRM 9254

// Forward declarations cho offline queries
forward OnOfflineRankUpdate(playerid, memberid, newrank, oldrank);
forward OnOfflineDivisionUpdate(playerid, memberid, newdiv, olddiv);
forward OnOfflineKickMember(playerid, memberid, oldrank);

// ==================== XU LY DIALOG QUAN LY THANH VIEN ====================

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_MEMBER_LIST)
	{
		if(!response) return 1;
		
		// Query lai de lay thong tin thanh vien duoc chon
		new query[256], iGroupID = PlayerInfo[playerid][pLeader];
		format(query, sizeof(query), 
			"SELECT `id`, `Username`, `Rank`, `Division`, `LastLogin` FROM `accounts` WHERE `Member` = %d ORDER BY `Rank` DESC, `Username` ASC LIMIT %d, 1", 
			iGroupID, listitem);
		
		SetPVarInt(playerid, "pMemberSelect_Listitem", listitem);
		mysql_function_query(MainPipeline, query, true, "OnSelectMember", "i", playerid);
		return 1;
	}
	
	if(dialogid == DIALOG_MEMBER_MANAGE)
	{
		if(!response) 
		{
			// Quay lai danh sach
			PC_EmulateCommand(playerid, "/thanhvien");
			return 1;
		}
		
		new szMemberName[MAX_PLAYER_NAME];
		GetPVarString(playerid, "pMemberManage_Name", szMemberName, sizeof(szMemberName));
		new iMemberID = GetPVarInt(playerid, "pMemberManage_ID");
		new iMemberRank = GetPVarInt(playerid, "pMemberManage_Rank");
		new iMemberDiv = GetPVarInt(playerid, "pMemberManage_Division");
		new bool:isOnline = bool:GetPVarInt(playerid, "pMemberManage_IsOnline");
		new iGroupID = PlayerInfo[playerid][pLeader];
		
		switch(listitem)
		{
			case 3: // Xem thong tin chi tiet
			{
				new szDialog[512];
				new szLastLogin[64], szDutyStatus[32];
				
				// Query de lay LastLogin
				new query[256];
				format(query, sizeof(query), "SELECT `LastLogin` FROM `accounts` WHERE `id` = %d", iMemberID);
				new Cache:result = mysql_query(MainPipeline, query);
				
				if(cache_num_rows())
				{
					cache_get_field_content(0, "LastLogin", szLastLogin, MainPipeline);
				}
				cache_delete(result);
				
				// Kiem tra on-duty status
				if(isOnline)
				{
					new targetid = INVALID_PLAYER_ID;
					foreach(new i: Player)
					{
						if(PlayerInfo[i][pId] == iMemberID)
						{
							targetid = i;
							break;
						}
					}
					
					if(targetid != INVALID_PLAYER_ID)
					{
						if(PlayerInfo[targetid][pDuty] == 1)
							format(szDutyStatus, sizeof(szDutyStatus), "{33FF33}On Duty");
						else
							format(szDutyStatus, sizeof(szDutyStatus), "{FF6347}Off Duty");
					}
					else
						format(szDutyStatus, sizeof(szDutyStatus), "{AAAAAA}N/A");
				}
				else
					format(szDutyStatus, sizeof(szDutyStatus), "{AAAAAA}Offline");
				
				format(szDialog, sizeof(szDialog),
					"{FFD700}=======================================\n\
					{FFFFFF}THONG TIN THANH VIEN\n\
					{FFD700}========================================\n\n\
					{FFD700}> {FFFFFF}Ten: {87CEEB}%s\n\
					{FFD700}> {FFFFFF}Rank: {87CEEB}%s (%d)\n\
					{FFD700}> {FFFFFF}Division: {87CEEB}%s\n\
					{FFD700}> {FFFFFF}Trang thai: %s\n\
					{FFD700}> {FFFFFF}On-Duty: %s\n\
					{FFD700}> {FFFFFF}Last Login: {87CEEB}%s\n\
					{FFD700}> {FFFFFF}SQL ID: {87CEEB}%d",
					szMemberName,
					arrGroupRanks[iGroupID][iMemberRank], iMemberRank,
					(iMemberDiv >= 0) ? arrGroupDivisions[iGroupID][iMemberDiv] : ("Khong co"),
					isOnline ? ("{33FF33}Online") : ("{AAAAAA}Offline"),
					szDutyStatus,
					szLastLogin,
					iMemberID
				);
				ShowPlayerDialog(playerid, DIALOG_MEMBER_MANAGE, DIALOG_STYLE_MSGBOX, 
					"{FFFFFF}THONG TIN THANH VIEN", szDialog, "Quay lai", "Dong");
			}
			
			case 4: // Thay doi rank
			{
				new szDialog[512];
				format(szDialog, sizeof(szDialog), 
					"{FFFFFF}Nhap rank moi cho {87CEEB}%s{FFFFFF}\n\n\
					{FFD700}> {FFFFFF}Rank hien tai: {87CEEB}%s (%d)\n\
					{FFD700}> {FFFFFF}Rank toi da: {87CEEB}%d\n\n\
					{AAAAAA}Nhap rank tu 0 den %d:",
					szMemberName, arrGroupRanks[iGroupID][iMemberRank], iMemberRank,
					Group_GetMaxRank(iGroupID), Group_GetMaxRank(iGroupID)
				);
				ShowPlayerDialog(playerid, DIALOG_MEMBER_GIVERANK, DIALOG_STYLE_INPUT,
					"{FFFFFF}THAY DOI RANK", szDialog, "Xac nhan", "Huy");
			}
			
			case 5: // Thay doi division
			{
				new szDialog[1024];
				format(szDialog, sizeof(szDialog),
					"{FFFFFF}Chon division moi cho {87CEEB}%s{FFFFFF}\n\n\
					{FFD700}> {FFFFFF}Division hien tai: {87CEEB}%s\n\n",
					szMemberName,
					(iMemberDiv >= 0) ? arrGroupDivisions[iGroupID][iMemberDiv] : ("Khong co")
				);
				
				// Option 0: Khong co division
				strcat(szDialog, "{FFFFFF}0. {AAAAAA}Khong co division\n");
				
				// Options 1+: Cac division co san
				for(new i; i < MAX_GROUP_DIVS; i++)
				{
					if(arrGroupDivisions[iGroupID][i][0])
					{
						new szLine[64];
						format(szLine, sizeof(szLine), "{FFFFFF}%d. {87CEEB}%s\n", i + 1, arrGroupDivisions[iGroupID][i]);
						strcat(szDialog, szLine);
					}
				}
				
				ShowPlayerDialog(playerid, DIALOG_MEMBER_SETDIV, DIALOG_STYLE_LIST,
					"{FFFFFF}THAY DOI DIVISION", szDialog, "Chon", "Huy");
			}
			
			case 6: // Kick khoi nhom
			{
				new szDialog[256];
				format(szDialog, sizeof(szDialog),
					"{FF6347}Ban co chac chan muon kick {FFFFFF}%s {FF6347}ra khoi nhom?\n\n\
					{FFD700}> {FFFFFF}Rank: {87CEEB}%s (%d)\n\
					{FFD700}> {FFFFFF}Division: {87CEEB}%s\n\n\
					{AAAAAA}Hanh dong nay khong the hoan tac!",
					szMemberName,
					arrGroupRanks[iGroupID][iMemberRank], iMemberRank,
					(iMemberDiv >= 0) ? arrGroupDivisions[iGroupID][iMemberDiv] : ("Khong co")
				);
				ShowPlayerDialog(playerid, DIALOG_MEMBER_KICK_CONFIRM, DIALOG_STYLE_MSGBOX,
					"{FFFFFF}XAC NHAN KICK THANH VIEN", szDialog, "Xac nhan", "Huy");
			}
		}
		return 1;
	}
	
	if(dialogid == DIALOG_MEMBER_GIVERANK)
	{
		if(!response)
		{
			// Quay lai menu quan ly
			ShowMemberManageDialog(playerid);
			return 1;
		}
		
		new iNewRank = strval(inputtext);
		new iGroupID = PlayerInfo[playerid][pLeader];
		new szMemberName[MAX_PLAYER_NAME];
		GetPVarString(playerid, "pMemberManage_Name", szMemberName, sizeof(szMemberName));
		new iMemberID = GetPVarInt(playerid, "pMemberManage_ID");
		new iMemberRank = GetPVarInt(playerid, "pMemberManage_Rank");
		new bool:isOnline = bool:GetPVarInt(playerid, "pMemberManage_IsOnline");
		
		// Kiem tra khong duoc tu chinh rank ban than
		if(iMemberID == PlayerInfo[playerid][pId])
		{
			SendClientMessageEx(playerid, COLOR_GRAD1, "Ban khong the thay doi rank cua chinh minh!");
			ShowMemberManageDialog(playerid);
			return 1;
		}
		
		// Kiem tra rank hop le
		if(!(0 <= iNewRank <= Group_GetMaxRank(iGroupID)))
		{
			new szError[128];
			format(szError, sizeof(szError), "Rank khong hop le! Phai tu 0 den %d", Group_GetMaxRank(iGroupID));
			SendClientMessageEx(playerid, COLOR_GRAD1, szError);
			ShowMemberManageDialog(playerid);
			return 1;
		}
		
		// Kiem tra quyen han
		if(iMemberRank >= PlayerInfo[playerid][pRank] && PlayerInfo[playerid][pRank] < Group_GetMaxRank(iGroupID))
		{
			SendClientMessageEx(playerid, COLOR_GRAD1, "Ban khong the thay doi rank cua nguoi co rank bang hoac cao hon!");
			ShowMemberManageDialog(playerid);
			return 1;
		}
		
		if(iNewRank == iMemberRank)
		{
			SendClientMessageEx(playerid, COLOR_GRAD1, "Nguoi nay da co rank do roi!");
			ShowMemberManageDialog(playerid);
			return 1;
		}
		
		// Cap nhat rank
		if(isOnline)
		{
			// Nguoi choi online - cap nhat truc tiep
			new targetid = INVALID_PLAYER_ID;
			foreach(new i: Player)
			{
				if(PlayerInfo[i][pId] == iMemberID)
				{
					targetid = i;
					break;
				}
			}
			
			if(targetid != INVALID_PLAYER_ID)
			{
				new szMessage[128];
				format(szMessage, sizeof(szMessage), "%s %s da %s ban rank %s.", 
					arrGroupRanks[iGroupID][PlayerInfo[playerid][pRank]], GetPlayerNameEx(playerid),
					(iNewRank > iMemberRank) ? ("thang") : ("ha"), arrGroupRanks[iGroupID][iNewRank]);
				SendClientMessageEx(targetid, COLOR_LIGHTBLUE, szMessage);
				
				PlayerInfo[targetid][pRank] = iNewRank;
				SetPVarInt(playerid, "pMemberManage_Rank", iNewRank);
				
				format(szMessage, sizeof(szMessage), "Ban da %s %s len rank %s.", 
					(iNewRank > iMemberRank) ? ("thang") : ("ha"), szMemberName, arrGroupRanks[iGroupID][iNewRank]);
				SendClientMessageEx(playerid, COLOR_LIGHTBLUE, szMessage);
			}
		}
		else
		{
			// Nguoi choi offline - cap nhat database voi callback
			new query[256];
			mysql_format(MainPipeline, query, sizeof(query), 
				"UPDATE `accounts` SET `Rank` = %d WHERE `id` = %d", 
				iNewRank, iMemberID);
			mysql_tquery(MainPipeline, query, "OnOfflineRankUpdate", "iiii", playerid, iMemberID, iNewRank, iMemberRank);
		}
		
		// Quay lai menu quan ly
		ShowMemberManageDialog(playerid);
		return 1;
	}
	
	if(dialogid == DIALOG_MEMBER_SETDIV)
	{
		if(!response)
		{
			// Quay lai menu quan ly
			ShowMemberManageDialog(playerid);
			return 1;
		}
		
		new iNewDiv = listitem - 3; 
		new iGroupID = PlayerInfo[playerid][pLeader];
		new szMemberName[MAX_PLAYER_NAME];
		GetPVarString(playerid, "pMemberManage_Name", szMemberName, sizeof(szMemberName));
		new iMemberID = GetPVarInt(playerid, "pMemberManage_ID");
		new iMemberDiv = GetPVarInt(playerid, "pMemberManage_Division");
		new iMemberRank = GetPVarInt(playerid, "pMemberManage_Rank");
		new bool:isOnline = bool:GetPVarInt(playerid, "pMemberManage_IsOnline");
		
		// Kiem tra quyen han
		if(iMemberRank > PlayerInfo[playerid][pRank] && PlayerInfo[playerid][pRank] < Group_GetMaxRank(iGroupID))
		{
			SendClientMessageEx(playerid, COLOR_GRAD1, "Ban khong the thay doi division cua nguoi co rank cao hon!");
			ShowMemberManageDialog(playerid);
			return 1;
		}
		
		if(iNewDiv == iMemberDiv)
		{
			SendClientMessageEx(playerid, COLOR_GRAD1, "Nguoi nay da o division do roi!");
			ShowMemberManageDialog(playerid);
			return 1;
		}
		
		// Cap nhat division
		if(isOnline)
		{
			// Nguoi choi online
			new targetid = INVALID_PLAYER_ID;
			foreach(new i: Player)
			{
				if(PlayerInfo[i][pId] == iMemberID)
				{
					targetid = i;
					break;
				}
			}
			
			if(targetid != INVALID_PLAYER_ID)
			{
				new szMessage[128];
				if(iNewDiv >= 0)
					format(szMessage, sizeof(szMessage), "%s %s da chuyen ban vao division %s.", 
						arrGroupRanks[iGroupID][PlayerInfo[playerid][pRank]], GetPlayerNameEx(playerid),
						arrGroupDivisions[iGroupID][iNewDiv]);
				else
					format(szMessage, sizeof(szMessage), "%s %s da loai ban ra khoi division.", 
						arrGroupRanks[iGroupID][PlayerInfo[playerid][pRank]], GetPlayerNameEx(playerid));
				
				SendClientMessageEx(targetid, COLOR_LIGHTBLUE, szMessage);
				PlayerInfo[targetid][pDivision] = iNewDiv;
				SetPVarInt(playerid, "pMemberManage_Division", iNewDiv);
				
				// Thong bao cho leader
				new szLeaderMsg[128];
				if(iNewDiv >= 0)
					format(szLeaderMsg, sizeof(szLeaderMsg), "{33FF33}[THANH CONG]{FFFFFF} Ban da chuyen %s vao division %s.", 
						szMemberName, arrGroupDivisions[iGroupID][iNewDiv]);
				else
					format(szLeaderMsg, sizeof(szLeaderMsg), "{33FF33}[THANH CONG]{FFFFFF} Ban da loai %s ra khoi division.", szMemberName);
				
				SendClientMessageEx(playerid, COLOR_LIGHTBLUE, szLeaderMsg);
			}
		}
		else
		{
			// Nguoi choi offline - cap nhat database voi callback
			new query[256];
			mysql_format(MainPipeline, query, sizeof(query), 
				"UPDATE `accounts` SET `Division` = %d WHERE `id` = %d", 
				iNewDiv, iMemberID);
			mysql_tquery(MainPipeline, query, "OnOfflineDivisionUpdate", "iiii", playerid, iMemberID, iNewDiv, iMemberDiv);
		}
		
		// Quay lai menu quan ly
		ShowMemberManageDialog(playerid);
		return 1;
	}
	
	if(dialogid == DIALOG_MEMBER_KICK_CONFIRM)
	{
		if(!response)
		{
			// Quay lai menu quan ly
			ShowMemberManageDialog(playerid);
			return 1;
		}
		
		new iGroupID = PlayerInfo[playerid][pLeader];
		new szMemberName[MAX_PLAYER_NAME];
		GetPVarString(playerid, "pMemberManage_Name", szMemberName, sizeof(szMemberName));
		new iMemberID = GetPVarInt(playerid, "pMemberManage_ID");
		new iMemberRank = GetPVarInt(playerid, "pMemberManage_Rank");
		new bool:isOnline = bool:GetPVarInt(playerid, "pMemberManage_IsOnline");
		
		// Kiem tra khong duoc tu kick ban than
		if(iMemberID == PlayerInfo[playerid][pId])
		{
			SendClientMessageEx(playerid, COLOR_GRAD1, "Ban khong the kick chinh minh ra khoi nhom!");
			ShowMemberManageDialog(playerid);
			return 1;
		}
		
		// Kiem tra quyen han
		if(iMemberRank >= PlayerInfo[playerid][pRank] && PlayerInfo[playerid][pRank] < Group_GetMaxRank(iGroupID))
		{
			SendClientMessageEx(playerid, COLOR_GRAD1, "Ban khong the kick nguoi co rank bang hoac cao hon!");
			ShowMemberManageDialog(playerid);
			return 1;
		}
		
		// Kick thanh vien
		if(isOnline)
		{
			// Nguoi choi online
			new targetid = INVALID_PLAYER_ID;
			foreach(new i: Player)
			{
				if(PlayerInfo[i][pId] == iMemberID)
				{
					targetid = i;
					break;
				}
			}
			
			if(targetid != INVALID_PLAYER_ID)
			{
				new szMessage[128];
				format(szMessage, sizeof(szMessage), "%s %s da kick ban ra khoi %s.", 
					arrGroupRanks[iGroupID][PlayerInfo[playerid][pRank]], GetPlayerNameEx(playerid),
					arrGroupData[iGroupID][g_szGroupName]);
				SendClientMessageEx(targetid, COLOR_LIGHTBLUE, szMessage);
				SendClientMessageEx(targetid, COLOR_WHITE, "Ban da tro thanh mot nguoi dan binh thuong.");
				
				PlayerInfo[targetid][pMember] = INVALID_GROUP_ID;
				PlayerInfo[targetid][pDivision] = -1;
				PlayerInfo[targetid][pLeader] = INVALID_GROUP_ID;
				PlayerInfo[targetid][pDuty] = 0;
				PlayerInfo[targetid][pPreDutyModel] = 0;
				PlayerInfo[targetid][pRank] = INVALID_RANK;
				PlayerInfo[targetid][pModel] = NOOB_SKIN;
				SetPlayerSkin(targetid, NOOB_SKIN);
				SetPlayerToTeamColor(targetid);
				pTazer{targetid} = 0;
			}
		}
		else
		{
			// Nguoi choi offline - cap nhat database voi callback
			new query[256];
			mysql_format(MainPipeline, query, sizeof(query), 
				"UPDATE `accounts` SET `Member` = %d, `Division` = -1, `Leader` = %d, `Rank` = %d, `Model` = %d WHERE `id` = %d",
				INVALID_GROUP_ID, INVALID_GROUP_ID, INVALID_RANK, NOOB_SKIN, iMemberID);
			mysql_tquery(MainPipeline, query, "OnOfflineKickMember", "iii", playerid, iMemberID, iMemberRank);
		}
		return 1;
	}
	
	return 0;
}

forward OnSelectMember(playerid);
public OnSelectMember(playerid)
{
	new rows, fields;
	cache_get_data(rows, fields, MainPipeline);
	
	if(rows == 0)
	{
		SendClientMessageEx(playerid, COLOR_GRAD1, "Khong tim thay thanh vien!");
		return 1;
	}
	
	new szUsername[MAX_PLAYER_NAME], szLastLogin[64];
	new iRank, iDivision, iSQLID;
	new bool:isOnline = false;
	
	iSQLID = cache_get_field_content_int(0, "id", MainPipeline);
	cache_get_field_content(0, "Username", szUsername, MainPipeline);
	cache_get_field_content(0, "LastLogin", szLastLogin, MainPipeline);
	iRank = cache_get_field_content_int(0, "Rank", MainPipeline);
	iDivision = cache_get_field_content_int(0, "Division", MainPipeline);
	
	// Kiem tra online
	foreach(new i: Player)
	{
		if(PlayerInfo[i][pId] == iSQLID)
		{
			isOnline = true;
			break;
		}
	}
	
	// Luu thong tin vao PVar
	SetPVarString(playerid, "pMemberManage_Name", szUsername);
	SetPVarInt(playerid, "pMemberManage_ID", iSQLID);
	SetPVarInt(playerid, "pMemberManage_Rank", iRank);
	SetPVarInt(playerid, "pMemberManage_Division", iDivision);
	SetPVarInt(playerid, "pMemberManage_IsOnline", isOnline ? 1 : 0);
	
	// Hien thi menu quan ly
	ShowMemberManageDialog(playerid);
	
	return 1;
}

stock ShowMemberManageDialog(playerid)
{
	new szMemberName[MAX_PLAYER_NAME];
	GetPVarString(playerid, "pMemberManage_Name", szMemberName, sizeof(szMemberName));
	new iMemberRank = GetPVarInt(playerid, "pMemberManage_Rank");
	new iMemberDiv = GetPVarInt(playerid, "pMemberManage_Division");
	new bool:isOnline = bool:GetPVarInt(playerid, "pMemberManage_IsOnline");
	new iGroupID = PlayerInfo[playerid][pLeader];
	
	new szDialog[512], szTitle[128];
	
	format(szDialog, sizeof(szDialog),
		"{FFD700} > Rank: {FFFFFF}%s (%d)\n\
		{FFD700} > Division: {FFFFFF}%s\n\
		{FFD700} > Trang thai: %s\n\n\
		{33FF33}0. Xem thong tin chi tiet\n\
		{FFD700}1. Thay doi rank\n\
		{87CEEB}2. Thay doi division\n\
		{FF6347}3. Kick khoi nhom",
		szMemberName,
		arrGroupRanks[iGroupID][iMemberRank], iMemberRank,
		(iMemberDiv >= 0) ? arrGroupDivisions[iGroupID][iMemberDiv] : ("Khong co"),
		isOnline ? ("{33FF33}Online") : ("{AAAAAA}Offline")
	);
	
	format(szTitle, sizeof(szTitle), "{FFFFFF}QUAN LY: %s", szMemberName);
	ShowPlayerDialog(playerid, DIALOG_MEMBER_MANAGE, DIALOG_STYLE_LIST, szTitle, szDialog, "Chon", "Quay lai");
	
	return 1;
}

// ==================== CALLBACK CHO OFFLINE QUERIES ====================

public OnOfflineRankUpdate(playerid, memberid, newrank, oldrank)
{
	if(!IsPlayerConnected(playerid)) return 1;
	
	new rows = cache_affected_rows();
	
	if(rows > 0)
	{
		// Cap nhat thanh cong
		new szMemberName[MAX_PLAYER_NAME];
		GetPVarString(playerid, "pMemberManage_Name", szMemberName, sizeof(szMemberName));
		new iGroupID = PlayerInfo[playerid][pLeader];
		
		SetPVarInt(playerid, "pMemberManage_Rank", newrank);
		
		new szMessage[128];
		format(szMessage, sizeof(szMessage), "{33FF33}[THANH CONG]{FFFFFF} Ban da %s %s (offline) len rank %s.", 
			(newrank > oldrank) ? ("thang") : ("ha"), szMemberName, arrGroupRanks[iGroupID][newrank]);
		SendClientMessageEx(playerid, COLOR_LIGHTBLUE, szMessage);
		SendClientMessageEx(playerid, COLOR_YELLOW, "* Thay doi da duoc cap nhat vao database.");
		
		// Log
		new szLog[256];
		format(szLog, sizeof(szLog), "%s %s da thay doi rank cua %s tu %d den %d trong %s", 
			arrGroupRanks[iGroupID][PlayerInfo[playerid][pRank]], GetPlayerNameEx(playerid),
			szMemberName, oldrank, newrank, arrGroupData[iGroupID][g_szGroupName]);
		Log("logs/group.log", szLog);
	}
	else
	{
		SendClientMessageEx(playerid, COLOR_GREY, "Loi: Khong the cap nhat rank trong database!");
	}
	
	// Quay lai menu quan ly
	ShowMemberManageDialog(playerid);
	return 1;
}

public OnOfflineDivisionUpdate(playerid, memberid, newdiv, olddiv)
{
	if(!IsPlayerConnected(playerid)) return 1;
	
	new rows = cache_affected_rows();
	
	if(rows > 0)
	{
		// Cap nhat thanh cong
		new szMemberName[MAX_PLAYER_NAME];
		GetPVarString(playerid, "pMemberManage_Name", szMemberName, sizeof(szMemberName));
		new iGroupID = PlayerInfo[playerid][pLeader];
		
		SetPVarInt(playerid, "pMemberManage_Division", newdiv);
		
		// Thong bao thanh cong
		new szOfflineMsg[128];
		if(newdiv >= 0)
			format(szOfflineMsg, sizeof(szOfflineMsg), "{33FF33}[THANH CONG]{FFFFFF} Ban da chuyen %s (offline) vao division %s.", 
				szMemberName, arrGroupDivisions[iGroupID][newdiv]);
		else
			format(szOfflineMsg, sizeof(szOfflineMsg), "{33FF33}[THANH CONG]{FFFFFF} Ban da loai %s (offline) ra khoi division.", szMemberName);
		
		SendClientMessageEx(playerid, COLOR_LIGHTBLUE, szOfflineMsg);
		SendClientMessageEx(playerid, COLOR_YELLOW, "* Thay doi da duoc cap nhat vao database.");
		
		// Log
		new szLog[256];
		format(szLog, sizeof(szLog), "%s %s da thay doi division cua %s thanh %s trong %s", 
			arrGroupRanks[iGroupID][PlayerInfo[playerid][pRank]], GetPlayerNameEx(playerid),
			szMemberName, (newdiv >= 0) ? arrGroupDivisions[iGroupID][newdiv] : "None",
			arrGroupData[iGroupID][g_szGroupName]);
		Log("logs/group.log", szLog);
	}
	else
	{
		SendClientMessageEx(playerid, COLOR_GREY, "Loi: Khong the cap nhat division trong database!");
	}
	
	// Quay lai menu quan ly
	ShowMemberManageDialog(playerid);
	return 1;
}

public OnOfflineKickMember(playerid, memberid, oldrank)
{
	if(!IsPlayerConnected(playerid)) return 1;
	
	new rows = cache_affected_rows();
	
	if(rows > 0)
	{
		// Kick thanh cong
		new szMemberName[MAX_PLAYER_NAME];
		GetPVarString(playerid, "pMemberManage_Name", szMemberName, sizeof(szMemberName));
		new iGroupID = PlayerInfo[playerid][pLeader];
		
		SendClientMessageEx(playerid, COLOR_YELLOW, "* Nguoi choi dang offline - da cap nhat database.");
		
		new szMessage[128];
		format(szMessage, sizeof(szMessage), "{33FF33}[THANH CONG]{FFFFFF} Ban da kick %s ra khoi nhom.", szMemberName);
		SendClientMessageEx(playerid, COLOR_LIGHTBLUE, szMessage);
		
		// Log
		new szLog[256];
		format(szLog, sizeof(szLog), "%s %s (rank %d) da kick %s (rank %d) ra khoi %s", 
			arrGroupRanks[iGroupID][PlayerInfo[playerid][pRank]], GetPlayerNameEx(playerid), PlayerInfo[playerid][pRank],
			szMemberName, oldrank, arrGroupData[iGroupID][g_szGroupName]);
		Log("logs/group.log", szLog);
		
		// Xoa cac PVar
		DeletePVar(playerid, "pMemberManage_Name");
		DeletePVar(playerid, "pMemberManage_ID");
		DeletePVar(playerid, "pMemberManage_Rank");
		DeletePVar(playerid, "pMemberManage_Division");
		DeletePVar(playerid, "pMemberManage_IsOnline");
		
		// Quay lai danh sach
        PC_EmulateCommand(playerid, "/thanhvien");
	}
	else
	{
		SendClientMessageEx(playerid, COLOR_GREY, "Loi: Khong the kick thanh vien khoi database!");
		ShowMemberManageDialog(playerid);
	}
	
	return 1;
}
