#include <YSI\YSI_Coding\y_hooks>

stock IsAPizzaCar(carid)
{
	for (new v = 0; v < sizeof(PizzaVehicles); v++) {
	    if(carid == PizzaVehicles[v]) return 1;
	}
	return 0;
}

CMD:laybanh(playerid, params[]) {
	if(PlayerInfo[playerid][pJob] != JOB_PIZZABOY && PlayerInfo[playerid][pJob2] != JOB_PIZZABOY) {
		SendClientMessageEx(playerid, COLOR_GREY, "Ban khong phai la Pizza Boy!");
	}
    else if(!IsAPizzaCar(GetPlayerVehicleID(playerid))) {
	    SendClientMessageEx(playerid, COLOR_GREY, "Ban khong dieu khien xe cua Pizza!");
	}
	else if(GetPlayerSkin(playerid) != 155) {
	    SendClientMessageEx(playerid, COLOR_GREY, "Ban can phai mac ao Pizza Boy!");
	}
	else if(GetPVarType(playerid, "Pizza")) {
		SendClientMessageEx(playerid, COLOR_GREY, "Bang dang co don hang Pizza can giao!");
	}
	else if(!IsPlayerInRangeOfPoint(playerid, 3.0, -1713.961425, 1348.545166, 7.180452)) {
		SendClientMessageEx(playerid, COLOR_GREY, "Ban khong dang dung o vi tri lay banh!");
	}
	else {

		new rand = random(MAX_HOUSES - 1), i;

		while(!(HouseInfo[rand][hOwned] && HouseInfo[rand][hExteriorZ] <= 100 && HouseInfo[rand][hExteriorX] > -2802.389648 && HouseInfo[rand][hExteriorX] < -1400.710327 && HouseInfo[rand][hExteriorY] < 2800 && HouseInfo[rand][hExteriorY] > -216.298019)) {
			rand = random(MAX_HOUSES - rand);

			if(rand < 0) rand = 0;
			if (i++ > MAX_HOUSES) 
				return SendClientMessageEx(playerid, COLOR_GREY, "Khong tim thay don hang Pizza nao!");
		}

		new
			iDist = floatround(GetPlayerDistanceFromPoint(playerid, HouseInfo[rand][hExteriorX], HouseInfo[rand][hExteriorY], HouseInfo[rand][hExteriorZ]));

		SetPVarInt(playerid, "tpPizzaTimer", iDist / 50);
		SetPVarInt(playerid, "pizzaTotal", iDist / 10);
		SetPVarInt(playerid, "pizzaTimer", iDist / 10);
		SetPVarInt(playerid, "Pizza", rand+1);

		SetTimerEx("OtherTimerEx", 1000, false, "ii", playerid, TYPE_TPPIZZARUNTIMER);
		SetTimerEx("OtherTimerEx", 1000, false, "ii", playerid, TYPE_PIZZATIMER);

		SendClientMessageEx(playerid, COLOR_WHITE, "Ban co don hang giao den cho {B2F663}%s{FFFFFF}. Ban co %d giay de giao no!", StripUnderscore(HouseInfo[rand][hOwnerName]), iDist / 10);

		SetPlayerCP(playerid, HouseInfo[rand][hExteriorX], HouseInfo[rand][hExteriorY], HouseInfo[rand][hExteriorZ], 5);
	}
	return 1;
}

hook OnPlayerEnterCheckpoint(playerid)
{
	if(GetPVarInt(playerid, "Pizza") > 0 && IsPlayerInRangeOfPoint(playerid, 5.0, HouseInfo[GetPVarInt(playerid, "Pizza")-1][hExteriorX], HouseInfo[GetPVarInt(playerid, "Pizza")-1][hExteriorY], HouseInfo[GetPVarInt(playerid, "Pizza")-1][hExteriorZ]) && GetPlayerInterior(playerid) == HouseInfo[GetPVarInt(playerid, "Pizza")-1][hExtIW] && GetPlayerVirtualWorld(playerid) == HouseInfo[GetPVarInt(playerid, "Pizza")-1][hExtVW])
	{
	    new string[128], bool:isTeleport = false;
		if (GetPVarInt(playerid, "tpPizzaTimer") != 0)
		{
			format(string, sizeof(string), "{AA3333}AdmWarning{FFFF00}: %s (ID %d) nghi van su dung dich chuyen Pizza.", GetPlayerNameEx(playerid), playerid);
  			ABroadCast( COLOR_YELLOW, string, 2 );
    		// format(string, sizeof(string), "%s (ID %d) is possibly teleport pizzarunning.", GetPlayerNameEx(playerid), playerid);
	    	// Log("logs/hack.log", string);
			isTeleport = true;
		}

		Misc_Save();
		new payment;
        switch(GetJobLevel(playerid, JOB_PIZZABOY))
        {
			case 1: payment = 5000;
			case 2: payment = 5500;
			case 3: payment = 6000;
			case 4: payment = 6500;
			case 5: payment = 7000;
        }
		new timeBonus = GetPVarInt(playerid, "pizzaTimer") * 10;
		if(timeBonus > 1000) timeBonus = 1000;
		payment += timeBonus;
		GivePlayerCash(playerid, payment);
		SendClientMessageEx(playerid, COLOR_WHITE, "Ban nhan duoc {15C836}$%s{FFFFFF} sau khi giao chiec banh Pizza thanh cong.", number_format(payment));
		DeletePVar(playerid, "Pizza");
		DisablePlayerCP(playerid);

		new rand_xp = 0;
		if(DoubleXP) {
			rand_xp = Random(PlayerInfo[playerid][pLevel] * XP_RATE - 7, PlayerInfo[playerid][pLevel] * XP_RATE - 4);
			SendClientMessageEx(playerid, COLOR_YELLOW, "Kinh nghiem nhan vat: +%d XP (%s/%s) | +2 Pizza Skill (DOUBLE XP)", rand_xp*2, number_format(PlayerInfo[playerid][pXP]+rand_xp*2), number_format(GetRequiredXP(playerid)));
			GivePlayerXP(playerid, rand_xp*2);
			PlayerInfo[playerid][pPizzaSkill] += 2;
		}
		else if(PlayerInfo[playerid][pDoubleEXP] > 0 && !DoubleXP)
		{
			rand_xp = Random(PlayerInfo[playerid][pLevel] * XP_RATE - 7, PlayerInfo[playerid][pLevel] * XP_RATE - 4);
			SendClientMessageEx(playerid, COLOR_YELLOW, "Kinh nghiem nhan vat: +%d XP (%s/%s) | +2 Pizza Skill (DOUBLE XP)", rand_xp*2, number_format(PlayerInfo[playerid][pXP]+rand_xp*2), number_format(GetRequiredXP(playerid)));
			GivePlayerXP(playerid, rand_xp*2);
			PlayerInfo[playerid][pPizzaSkill] += 2;
		}
		else
		{
			rand_xp = Random(PlayerInfo[playerid][pLevel] * XP_RATE - 7, PlayerInfo[playerid][pLevel] * XP_RATE - 4);
			SendClientMessageEx(playerid, COLOR_YELLOW, "Kinh nghiem nhan vat: +%d XP (%s/%s) | +1 Pizza Skill", rand_xp, number_format(PlayerInfo[playerid][pXP]+rand_xp), number_format(GetRequiredXP(playerid)));
			GivePlayerXP(playerid, rand_xp);
			PlayerInfo[playerid][pPizzaSkill] += 1;
		}

		new title[64];
		format(title, _, "%s (ID: %d) - giao Pizza", GetPlayerNameEx(playerid), playerid);
		DiscordLog(dclog_Pizza, title, "**Tiền nhận được: $%s**\n**XP nhận được: %d XP**\n**Tổng thời gian phải giao: %d giây**\n**Thời gian còn lại sau khi hoàn thành: %d giây**%s", number_format(payment), rand_xp, GetPVarInt(playerid, "pizzaTotal"), GetPVarInt(playerid, "pizzaTimer"), (isTeleport) ? "\n\n:shield: ***NGHI VẤN NGƯỜI CHƠI ĐANG GIAN LẬN***" : "");
	}
	return 1;
}