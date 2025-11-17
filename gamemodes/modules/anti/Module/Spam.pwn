#include <YSI/YSI_Coding/y_hooks>

	//-------------------------------------------------------------------
	//					Project create by #nDP 						   //
	//					Fb.com/nguyenduyphuong.com  			       //
	//					Copyright by nDP" 							   //
	//-------------------------------------------------------------------

//____________________________________________________________________________//
enum eServer {
	name_server[128],
};

enum eServerText {
	text_nonrp[128],
};

new const AntiServer[][eServer] = {
	{"sv.rgame.network"},
	{"rgame"},
	{"server"},
	{"gtavn"},
	{"sagrp"},
	{"lvrp.net"},
	{"gtaviet"},
	{"hsgame"},
	{"gosamp"},
	{"gtaae"},
	{"aesamp"},
	{"lvrp"},
	{"GTAVN.NET"},
	{"sv.gtaviet.net"},
	{"sv.psa-rp.com"},
	{"gta-ae.pro"},
	{"Gvrp.vn"},
	{"HSGAME.PRO"},
	{"GOSAMP.NET"},
	{"00.000.00.00:0000"}
};

new const AntiTextNonrp[][eServerText] = {
	{"!"},
	{"@"},
	{"#"},
	{"$"},
	{"%"},
	{"^"},
	{"&"},
	{"*"},
	{"|"},
	{"`"}
};

stock SvSAMP(const text[]) {
	for (new i = 0; i < sizeof(AntiServer); i++) {
	if(strfind(text, AntiServer[i][name_server], true) != -1) {
			return 1;
		}
	}
	return 0;
}

stock SvNonrpText(const text[]) {
	for (new i = 0; i < sizeof(AntiTextNonrp); i++) {
	if(strfind(text, AntiTextNonrp[i][text_nonrp], true) != -1) {
			return 1;
		}
	}
	return 0;
}

stock check_textnonrp(const text[]) {
	if(SvNonrpText(text) == 1)
	{
		return 1;
	}
	return 0;
}

stock check_spamServer(const text[]) {
	if(strfind(text, "sa-rp.net", true) != -1 || strfind(text, "forum.sa-rp.net", true) != -1) return 0;
	if(strfind(text, ".com", true) != -1 || strfind(text, ".co", true) != -1 || strfind(text, ".info", true) != -1 || strfind(text, ".net", true) != -1 || strfind(text, ".org", true) != -1 || strfind(text, ".me", true) != -1 ||
	strfind(text, ".us", true) != -1 || strfind(text, ".biz", true) != -1 || strfind(text, ".com.co", true) != -1 || strfind(text, ".com.uk", true) != -1 || strfind(text, ".net.co", true) != -1 || strfind(text, ".top", true) != -1 ||
	strfind(text, ".in", true) != -1 || strfind(text, ".uk", true) != -1 || strfind(text, ".site", true) != -1 || strfind(text, ".net16.net", true) != -1 || strfind(text, ".tk", true) != -1 ||
	strfind(text, "join my server", true) != -1 || strfind(text, "come in my server", true) != -1 || strfind(text, "come on this server", true) != -1 || strfind(text, "comuv.com", true) != -1 || 
	SvSAMP(text) == 1)
	{
		return 1;
	}
	return 0;
}

hook OnPlayerText(playerid, text[]) {
	if(GetPlayerSkin(playerid) == 0 || GetPlayerSkin(playerid) >= 311) {
		Kick(playerid);
		return 1;
	}

	if(gPlayerLogged{playerid} != 1) {
		Kick(playerid);
		return 1;
	}
	return 1;
}

hook OnPlayerCommandText(playerid, cmdtext[]) {

	if(gPlayerLogged{playerid} != 1) {
		Kick(playerid);
		return 1;
	}

	if(GetPlayerSkin(playerid) == 0 || GetPlayerSkin(playerid) >= 311) {
		Kick(playerid);
		return 1;
	}

	// --------------------------------------------------------------------
	if(PlayerInfo[playerid][pAdmin] == 0) {
	    if (strcmp("/skema", cmdtext, true, 10) == 0)
		{
			Kick(playerid);
			return 1;
		}

		if (strcmp("/give.menu", cmdtext, true, 10) == 0)
		{
			Kick(playerid);
			return 1;
		}

		if (strcmp("/ct.up", cmdtext, true, 10) == 0)
		{
			Kick(playerid);
			return 1;
		}

		if (strcmp("/ptrll", cmdtext, true, 10) == 0)
		{
			SetTimerEx("KickEx", 5000, false, "i", playerid);
			return 1;
		}

		if (strcmp("/chp", cmdtext, true, 10) == 0)
		{
	        Kick(playerid);
			return 1;
		}
		if (strcmp("/tr", cmdtext, true, 10) == 0)
		{
	        Kick(playerid);
			return 1;
		}
		if (strcmp("/vb", cmdtext, true, 10) == 0)
		{
	        Kick(playerid);
			return 1;
		}
		if (strcmp("/cboom", cmdtext, true, 10) == 0)
		{
	        Kick(playerid);
			return 1;
		}
		if (strcmp("/hmo", cmdtext, true, 10) == 0)
		{
	        Kick(playerid);
			return 1;
		}
		if (strcmp("/invis", cmdtext, true, 10) == 0)
		{
	        Kick(playerid);
			return 1;
		}
		if (strcmp("/.killall", cmdtext, true, 10) == 0) {
	        if(PlayerInfo[playerid][pAdmin] == 0)
	        {
				new String[1280];
		        format( String, sizeof( String ), "AdmCmd: %s da bi khoa tai khoan, ly do: MultiHacking.", GetPlayerNameEx(playerid));
				ABroadCast( COLOR_LIGHTRED, String, 2 );
				discord_messHack(String);
				
				SendClientMessage(playerid, COLOR_LIGHTRED, String );
				SetPVarInt(playerid, "_HACK_WARNINGS", 0 );
				PlayerInfo[playerid][pBanned] = 3;
				new playerip[32];
				GetPlayerIp(playerid, playerip, sizeof(playerip));
				format( String, sizeof( String ), "AdmCmd: %s (IP:%s) da bi khoa tai khoan, ly do: MultiHacking", GetPlayerNameEx(playerid), playerip);
				PlayerInfo[playerid][pBanned] = 3;
				Log("logs/ban.log", String);
				SystemBan(playerid, "[System] (MultiHacking)");
				MySQLBan(GetPlayerSQLId(playerid), playerip, "MultiHacking", 1,"System");
				SetTimerEx("KickEx", 1000, false, "i", playerid);
				Kick(playerid);
				return 1;
			}
		}
	}
	return 1;
}
