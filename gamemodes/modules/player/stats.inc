stock ShowMainStats(playerid,targetid)
{
	if(IsPlayerConnected(targetid))
	{
		new resultline[1024], header[65], org[128], employer[GROUP_MAX_NAME_LEN], rank[GROUP_MAX_RANK_LEN], division[GROUP_MAX_DIV_LEN];
		new sext[16], std[20], nation[24], biz[128];
		if(PlayerInfo[targetid][pSex] == 1) { sext = "Nam"; } else { sext = "Nu"; }
		switch(GetPVarInt(targetid, "STD"))
		{
		    case 1: std = "Chlamydia";
		    case 2: std = "Gonorrhea";
		    case 3: std = "Syphilis";
		    default: std = "Khong co";
		}
		if(PlayerInfo[targetid][pMember] != INVALID_GROUP_ID)
		{
			GetPlayerGroupInfo(targetid, rank, division, employer);
			format(org, sizeof(org), "Faction: %s (%d)\nRank: %s (%d)\nDivision: %s (%d)\n", employer, PlayerInfo[targetid][pMember], rank, PlayerInfo[targetid][pRank], division, PlayerInfo[targetid][pDivision]);
		}
		else if(PlayerInfo[targetid][pFMember] < INVALID_FAMILY_ID)
		{
			if(0 <= PlayerInfo[targetid][pDivision] < 5) format(division, sizeof(division), "%s", FamilyDivisionInfo[PlayerInfo[targetid][pFMember]][PlayerInfo[targetid][pDivision]]);
			else division = "Khong co";
			format(org, sizeof(org), "Family:\t%s (%d)\nRank:\t%s (%d)\nDivision:\t%s (%d)\n", FamilyInfo[PlayerInfo[targetid][pFMember]][FamilyName], PlayerInfo[targetid][pFMember], FamilyRankInfo[PlayerInfo[targetid][pFMember]][PlayerInfo[targetid][pRank]], PlayerInfo[targetid][pRank], division, PlayerInfo[targetid][pDivision]);
		}
		else format(org, sizeof(org), "");
		if(PlayerInfo[targetid][pBusiness] != INVALID_BUSINESS_ID) format(biz, sizeof(biz), "Business:\t%s (%d)\nRank:\t%s (%d)\n", Businesses[PlayerInfo[targetid][pBusiness]][bName], PlayerInfo[targetid][pBusiness], GetBusinessRankName(PlayerInfo[targetid][pBusinessRank]), PlayerInfo[targetid][pBusinessRank]);
		else format(biz, sizeof(biz), "");
		switch(PlayerInfo[targetid][pNation])
		{
			case 0: nation = "San Andreas";
			case 1: nation = "Tierra Robada";
		}
		new insur[20];
		switch(PlayerInfo[targetid][pInsurance])
		{
			case 1: insur = "County General";
			case 2: insur = "All Saints";
			case 3: insur = "Montgomery";
			case 4: insur = "Fort Carson";
			case 5: insur = "San Fierro";
			case 6: insur = "Club VIP";
			case 7: insur = "Home Care";
			case 9: insur = "El Quebrados";
			case 10: insur = "SAAS Base Hospital";
			case 11: insur = "Las Venturas";
			case 12: insur = "Famed Room";
			case 13: insur = "DeMorgan";
			case 14: insur = "Bayside";
			default: insur = "Khong co";
		}
		new staffrank[64];
		if(PlayerInfo[targetid][pHelper] > 0 || PlayerInfo[targetid][pAdmin] == 1 || (PlayerInfo[targetid][pAdmin] > 1 && PlayerInfo[playerid][pAdmin] <= PlayerInfo[targetid][pAdmin])) format(staffrank, sizeof(staffrank), "%s\t \n", GetStaffRank(targetid));
		else staffrank = "";
		new drank[64];
		switch(PlayerInfo[targetid][pDonateRank])
		{
			case 1: drank = "{800080}Bronze VIP{FFFFFF}\n";
			case 2: drank = "{800080}Silver VIP{FFFFFF}\n";
			case 3: drank = "{FFD700}Gold VIP{FFFFFF}\n";
			case 4: drank = "{E5E4E2}Platinum VIP{FFFFFF}\n";
			case 5: drank = "{800080}VIP Moderator{FFFFFF}\n";
			default: drank = "{FFFFFF}Khong co{FFFFFF}\n";
		}


		new famedrank[64];
		if(PlayerInfo[targetid][pFamed] > 0)
		{
			switch(PlayerInfo[targetid][pFamed])
			{
				case 1: famedrank = "{228B22}Old-School{FFFFFF}\n";
				case 2: famedrank = "{FF7F00}Chartered Old-School{FFFFFF}\n";
				case 3: famedrank = "{ADFF2F}Famed{FFFFFF}\n";
				case 4: famedrank = "{8F00FF}Famed Commissioner{FFFFFF}\n";
				case 5: famedrank = "{8F00FF}Famed Moderator{FFFFFF}\n";
				case 6: famedrank = "{8F00FF}Famed Vice-Chairman{FFFFFF}\n";
				case 7: famedrank = "{8F00FF}Famed Chairman{FFFFFF}\n";
			}
		}
		if(PlayerInfo[targetid][pMarriedID] == -1) format(PlayerInfo[targetid][pMarriedName], MAX_PLAYER_NAME, "Khong co ai");
		new exp_levelup = GetRequiredXP(playerid);
		new Float:health, Float:armor;
		GetPlayerHealth(targetid, health);
		GetPlayerArmour(targetid, armor);
		new zone[MAX_ZONE_NAME];
		GetPlayer3DZone(targetid, zone, sizeof(zone));
		new pnumber[20];
		if(PlayerInfo[targetid][pPnumber] == 0) pnumber = "Khong co"; else format(pnumber, sizeof(pnumber), "%d", PlayerInfo[targetid][pPnumber]);

  		SetPVarInt(playerid, "ShowStats", targetid);
		format(header, sizeof(header), "Thong tin Nhan Vat > %s", GetPlayerNameEx(targetid));
		format(resultline, sizeof(resultline),"%s\
		Donate Member:\t%s\
		{FFFFFF}Cap do:\t%d\n\
		Gioi tinh:\t%s\n\
		Ngay sinh:\t%s\n\
		So dien thoai:\t%s\n\
		Vi tri hien tai:\t%s\n\
		Hon nhan:\t%s\n\
		Mau:\t%d%%\n\
		Giap:\t%d%%\n\
		Trang thai:\t%d%%\n\
		Fitness:\t%d\n\
		Gio da choi:\t%s\n\
		Diem nang cap:\t%s\n\
		XP Nhan vat:\t{FFF30F}%s/%s | Cap do tiep theo: %d{FFFFFF}\n\
		Quoc tich:\t%s\n\
		%s\
		%s\
		Cong viec: %s\t(Cap do: %d)\n\
		Cong viec 2: %s\t(Cap do: %d)\n\
		Bao hiem:\t%s",
		staffrank,
		drank,
		PlayerInfo[targetid][pLevel],
		sext,
		PlayerInfo[targetid][pBirthDate],
		pnumber,
		zone,
		PlayerInfo[targetid][pMarriedName],
		floatround(health),
		floatround(armor),
		PlayerInfo[targetid][pHunger],
		PlayerInfo[targetid][pFitness],
		number_format(PlayerInfo[targetid][pConnectHours]),
		number_format(PlayerInfo[targetid][gPupgrade]),
		number_format(PlayerInfo[targetid][pXP]),
		number_format(exp_levelup),
		PlayerInfo[targetid][pLevel] + 1,
		nation,
		org,
		biz,
		GetJobName(PlayerInfo[targetid][pJob]),
		GetJobLevel(targetid, PlayerInfo[targetid][pJob]),
		GetJobName(PlayerInfo[targetid][pJob2]),
		GetJobLevel(targetid, PlayerInfo[targetid][pJob2]),
		insur);
		ShowPlayerDialog(playerid, DISPLAY_STATS, DIALOG_STYLE_TABLIST, header, resultline, "Trang Sau", "Dong");
	}
	return 1;
}

stock ShowSecondaryStats(playerid, targetid)
{
    new resultline[1028], header[64], pnumber[20], pvtstring[128];
    if(PlayerInfo[targetid][pPnumber] == 0) pnumber = "Khong co"; else format(pnumber, sizeof(pnumber), "%d", PlayerInfo[targetid][pPnumber]);
    if (PlayerInfo[playerid][pAdmin] >= 2)
    {
        format(pvtstring, sizeof(pvtstring), "Gift Box Tokens:\t%s\n", number_format(PlayerInfo[targetid][pGoldBoxTokens]));
    }

    new totalwealth;
    totalwealth = PlayerInfo[targetid][pAccount] + GetPlayerCash(targetid);
    if(PlayerInfo[targetid][pPhousekey] != INVALID_HOUSE_ID && HouseInfo[PlayerInfo[targetid][pPhousekey]][hOwnerID] == GetPlayerSQLId(targetid)) totalwealth += HouseInfo[PlayerInfo[targetid][pPhousekey]][hSafeMoney];
    if(PlayerInfo[targetid][pPhousekey2] != INVALID_HOUSE_ID && HouseInfo[PlayerInfo[targetid][pPhousekey2]][hOwnerID] == GetPlayerSQLId(targetid)) totalwealth += HouseInfo[PlayerInfo[targetid][pPhousekey2]][hSafeMoney];

    SetPVarInt(playerid, "ShowSecStat", targetid);
    format(header, sizeof(header), "Thong tin Nhan Vat > %s", GetPlayerNameEx(targetid));
    format(resultline, sizeof(resultline),"{FFFFFF}Tong tien so huu:\t{22D20B}$%s\n\
    {FFFFFF}Tien:\t$%s\n\
    Tien ngan hang:\t$%s\n\
    So dien thoai:\t%s\n\
    Tan so Radio:\t%dkhz\n\
    VIP Tokens:\t%s\n\
    Paintball Tokens:\t%s\n\
    EXP Tokens:\t{E2F509}%s{FFFFFF}\n\
    EXP Gio choi:\t{E2F509}%s{FFFFFF}\n\
    The su kien:\t%s\n\
    SMSLog:\t%d\n\
    RC Cam:\t%d\n\
    Receiver:\t%d\n\
    Bug Sweep:\t%d\n\
    Phao bong:\t%d\n\
    Mailbox:\t%d\n\
    Metal Detector:\t%d\n\
    %s",
    number_format(totalwealth),
    number_format(GetPlayerCash(targetid)),
    number_format(PlayerInfo[targetid][pAccount]),
    pnumber,
    PlayerInfo[targetid][pRadioFreq],
    number_format(PlayerInfo[targetid][pTokens]),
    number_format(PlayerInfo[targetid][pPaintTokens]),
    number_format(PlayerInfo[targetid][pEXPToken]),
    number_format(PlayerInfo[targetid][pDoubleEXP]),
    number_format(PlayerInfo[targetid][pTrickortreat]),
    PlayerInfo[targetid][pSmslog],
    PlayerInfo[targetid][pRccam],
    PlayerInfo[targetid][pReceiver],
    PlayerInfo[targetid][pSweep],
    PlayerInfo[targetid][pFirework],
    PlayerInfo[targetid][pMailbox],
    PlayerInfo[targetid][pMetalDetector],
    pvtstring);

    ShowPlayerDialog(playerid, DISPLAY_SECSTATS, DIALOG_STYLE_TABLIST, header, resultline, "Trang sau", "Dong");
    return 1;
}

alias:stats("thongtin")
CMD:stats(playerid, params[]) 
{
    if(gPlayerLogged{playerid} == 0) return 1;

    if(isnull(params) || !IsNumeric(params)) {
        SendHelpMessage(playerid, "/thongtin [1/2]");
        SendClientMessageEx(playerid, COLOR_GRAD1, "Tuy chon: 1. Thong tin chinh | 2. Thong tin phu");
        return 1;
    }

    if(strval(params) == 1) ShowMainStats(playerid, playerid);
    else if(strval(params) == 2) ShowSecondaryStats(playerid, playerid);
	return 1;
}

CMD:netstats(playerid, params[]) {
	if(gPlayerLogged{playerid} != 0) {
		new strStats[401], szTitle[64];
		GetPlayerNetworkStats(playerid, strStats, sizeof(strStats));
		format(szTitle, sizeof(szTitle), "Network Stats (ID: %d) - %s", playerid, GetPlayerNameEx(playerid));
		ShowPlayerDialog(playerid, 0, DIALOG_STYLE_MSGBOX, szTitle, strStats, "Dong", "");
	}
	return 1;
}

CMD:anetstats(playerid, params[])
{
	if (PlayerInfo[playerid][pAdmin] < 2) {
		SendUnAuth(playerid);
		return 1;
	}

	new giveplayerid, strStats[401], szTitle[64];
	if(sscanf(params, "u", giveplayerid)) return SendHelpMessage(playerid, "/anetstats [player]");

	if(gPlayerLogged{giveplayerid} != 0) {
		GetPlayerNetworkStats(giveplayerid, strStats, sizeof(strStats));
		format(szTitle, sizeof(szTitle), "Network Stats (ID: %d) - %s", giveplayerid, GetPlayerNameEx(giveplayerid));
		ShowPlayerDialog(playerid, 0, DIALOG_STYLE_MSGBOX, szTitle, strStats, "Dong", "");
	}
	return 1;
}

alias:quickstats("qs", "thongke")
CMD:quickstats(playerid, params[])
{
	new string[128], Float: health, Float: armor;
	GetPlayerHealth(playerid, health);
	GetPlayerArmour(playerid, armor);

	format(string, sizeof(string), "---===== ** So lieu thong ke cua %s ** =====---", GetPlayerNameEx(playerid));
	SendClientMessageEx(playerid, COLOR_GREEN, string);
	format(string, sizeof(string), "** Level: %d | Bank: %s | Cash: %s | Tan so radio: %dkhz | Cao cao: %d", PlayerInfo[playerid][pLevel],
	number_format(PlayerInfo[playerid][pAccount]), number_format(PlayerInfo[playerid][pCash]), PlayerInfo[playerid][pRadioFreq], PlayerInfo[playerid][pWarns]);
	SendClientMessageEx(playerid, COLOR_GRAD1, string);
	format(string, sizeof(string), "** Health: %d | Armour: %d | Doi bung: %d | The luc: %d", floatround(health), floatround(armor), PlayerInfo[playerid][pHunger], PlayerInfo[playerid][pFitness]);
	SendClientMessageEx(playerid, COLOR_GRAD1, string);
	SendClientMessageEx(playerid, COLOR_GREEN, "--------------------------------------------------------------------------------------------------------------------");
	return 1;
}