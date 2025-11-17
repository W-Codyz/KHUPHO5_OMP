#define SendUnAuth(%0) \
            SendClientMessageEx(%0, COLOR_LIGHTRED, "Ban khong duoc phep su dung lenh nay.")

#define SendHelpMessage(%0,%1) \
            SendClientMessageEx(%0, 0x3582FFFF, "SU DUNG:{FFFFFF} "%1)
		
#define SendHintMessage(%0,%1) \
			SendClientMessageEx(%0, 0xF08854FF, "[?]{FFFFFF} "%1)

stock ShopTechBroadCast(color, const string[])
{
	foreach(new i: Player)
	{
		if ((PlayerInfo[i][pShopTech] >= 1 || PlayerInfo[i][pAdmin] >= 1338) && PlayerInfo[i][pTogReports] == 0)
		{
			SendClientMessageEx(i, color, string);
		}
	}
	return 1;
}

stock ABroadCast(hColor, const szMessage[], iLevel, bool: bUndercover = false) {
	foreach(new i: Player)
	{
		if(PlayerInfo[i][pAdmin] >= iLevel && (bUndercover || !PlayerInfo[i][pTogReports])) {
			SendClientMessageEx(i, hColor, szMessage);
		}
	}
	return 1;
}

stock CBroadCast(color, const string[], level)
{
	foreach(new i: Player)
	{
		if (PlayerInfo[i][pHelper] >= level)
		{
			SendClientMessageEx(i, color, string);
			//printf("%s", string);
		}
	}
	return 1;
}

stock OOCOff(color, const string[])
{
	foreach(new i: Player)
	{
		if(!gOoc[i]) {
			SendClientMessageEx(i, color, string);
		}
	}
}

stock OOCNews(color, const string[])
{
	foreach(new i: Player)
	{
		if(!gNews[i]) {
			SendClientMessageEx(i, color, string);
		}
	}
}

stock SendGroupMessage(iGroupType, color, const string[], allegiance = 0)
{
	new iGroupID;
	foreach(new i: Player)
	{
	    iGroupID = PlayerInfo[i][pMember];
		if( iGroupType == -1 || ((0 <= iGroupID < MAX_GROUPS) && arrGroupData[iGroupID][g_iGroupType] == iGroupType) )
		{
		    if(allegiance == 0 || allegiance == arrGroupData[iGroupID][g_iAllegiance])
			{
				SendClientMessageEx(i, color, string);
			}
		}
	}
}

stock SendDivisionMessage(member, division, color, const string[])
{
	foreach(new i: Player)
	{
		if(PlayerInfo[i][pMember] == member && PlayerInfo[i][pDivision] == division) {
			SendClientMessageEx(i, color, string);
		}
	}
}

stock SendJobMessage(job, color, const string[])
{
	foreach(new i: Player)
	{
		if(((PlayerInfo[i][pJob] == job || PlayerInfo[i][pJob2] == job) && JobDuty[i] == 1) || ((PlayerInfo[i][pJob] == job || PlayerInfo[i][pJob2] == job))) {
			SendClientMessageEx(i, color, string);
		}
	}
}

stock SendNewFamilyMessage(family, color, const string[])
{
	foreach(new i: Player)
	{
		if(PlayerInfo[i][pFMember] == family) {
			if(!gFam[i]) {
				SendClientMessageEx(i, color, string);
			}
		}
		if(PlayerInfo[i][pAdmin] > 1 && GetPVarInt(i, "BigEarFamily") == family && GetPVarInt(i, "BigEar") == 5) {
			new szAntiprivacy[128];
			format(szAntiprivacy, sizeof(szAntiprivacy), "(BE) %s", string);
			SendClientMessageEx(i, color, szAntiprivacy);
		}
	}
}

stock SendFamilyMessage(family, color, const string[])
{
	foreach(new i: Player)
	{
		if(PlayerInfo[i][pMember] == family || PlayerInfo[i][pLeader] == family) {
			if(!gFam[i]) {
				SendClientMessageEx(i, color, string);
			}
		}
	}
}

stock RadioBroadCast(playerid, const string[])
{
	new MiscString[128], Float: aaaPositions[3];
	foreach(new i: Player)
	{
		if(PlayerInfo[i][pRadioFreq] == PlayerInfo[playerid][pRadioFreq] && PlayerInfo[i][pRadio] >= 1 && gRadio{i} != 0)
		{
			GetPlayerPos(i, aaaPositions[0], aaaPositions[1], aaaPositions[2]);
			format(MiscString, sizeof(MiscString), "** Radio (%d khz) ** %s: %s", PlayerInfo[playerid][pRadioFreq], GetPlayerNameEx(playerid), string);
			SendClientMessageEx(i, PUBLICRADIO_COLOR, MiscString);
			format(MiscString, sizeof(MiscString), "(radio) %s", string);
			SetPlayerChatBubble(playerid,MiscString,COLOR_WHITE,15.0,5000);
		}
	}
}

stock SendTeamBeepMessage(color, const string[])
{
	foreach(new i: Player)
	{
		if(IsACop(i))
		{
			SendClientMessageEx(i, color, string);
			RingTone[i] = 20;
		}
	}
}

stock SendVIPMessage(color, const string[])
{
	foreach(new i: Player)
	{
		if((PlayerInfo[i][pDonateRank] >= 1 || PlayerInfo[i][pAdmin] >= 2) && GetPVarType(i, "togVIP")) {
			SendClientMessageEx(i, color, string);
		}
	}
}

stock SendFamedMessage(color, const string[])
{
	foreach(new i: Player)
	{
		if((PlayerInfo[i][pFamed] >= 1 || PlayerInfo[i][pAdmin] >= 4) && GetPVarInt(i, "togFamed") == 1) {
			SendClientMessageEx(i, color, string);
		}
	}
}

stock SendAdvisorMessage(color, const string[])
{
	foreach(new i: Player)
	{
		if((PlayerInfo[i][pHelper] >= 2 || PlayerInfo[i][pDonateRank] == 5 || PlayerInfo[i][pWatchdog] == 1) && advisorchat[i])
		{
			SendClientMessageEx(i, color, string);
		}
		else
		{
			if(PlayerInfo[i][pAdmin] >= 1 && advisorchat[i])
			{
				SendClientMessageEx(i, color, string);
			}
		}
	}
}

stock SendDutyAdvisorMessage(color, const string[])
{
	foreach(new i: Player)
	{
		if(PlayerInfo[i][pHelper] >= 2 && GetPVarInt(i, "AdvisorDuty") == 1) {
			SendClientMessageEx(i, color, string);
		}
	}
}

forward DestroyMessageTD(playerid);
public DestroyMessageTD(playerid)
{
	if(IsPlayerTextDrawVisible(playerid, MessageTD[playerid]))
	{
		PlayerTextDrawHide(playerid, MessageTD[playerid]);
		DeletePVar(playerid, "MessageTD_Timer");
	}
	return 1;
}

stock SendMessageTD(playerid, time, const string[], va_args<>)
{
	if(GetPVarType(playerid, "MessageTD_Timer") == VARTYPE_INT && IsValidTimer(GetPVarInt(playerid, "MessageTD_Timer")))
	{
		KillTimer(GetPVarInt(playerid, "MessageTD_Timer"));
	}

	new szMsg[256];
	va_format(szMsg, sizeof(szMsg), string, va_start<3>);

	SetPVarInt(playerid, "MessageTD_Timer", SetTimerEx("DestroyMessageTD", time, false, "i", playerid));

	PlayerTextDrawSetString(playerid, MessageTD[playerid], szMsg);
	return PlayerTextDrawShow(playerid, MessageTD[playerid]);
}