stock SendTaxiMessage(color, const string[])
{
	foreach(new i: Player)
	{
 		if(IsATaxiDriver(i) && PlayerInfo[i][pDuty] > 0) {
 		    SendClientMessageEx(i, color, string);
		}

		if(TransportDuty[i] > 0 && (PlayerInfo[i][pJob] == JOB_TAXIDRIVER || PlayerInfo[i][pJob2] == JOB_TAXIDRIVER || PlayerInfo[i][pTaxiLicense] == 1)) {
		    if(!IsATaxiDriver(i)) {
		    	SendClientMessageEx(i, color, string);
			}
		}
	}
}

CMD:fare(playerid, params[])
{
	if(!IsATaxiDriver(playerid) && PlayerInfo[playerid][pJob] != JOB_TAXIDRIVER && PlayerInfo[playerid][pJob2] != JOB_TAXIDRIVER)
	{
		SendClientMessageEx(playerid, COLOR_GREY, "Ban khong phai la tai xe Bus/Taxi.");
		ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong phai la tai xe Bus/Taxi.");
		return 1;
	}

	if(PlayerInfo[playerid][pTaxiLicense] == 0)
	{
		SendClientMessageEx(playerid, COLOR_GREY, "Ban khong co giay phep hoat dong Taxi/Bus.");
		ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong co giay phep hoat dong Taxi/Bus.");
		return 1;
	}
	
	new string[128], fare;

	if(PlayerInfo[playerid][pJailTime] > 0) 
	{
		SendClientMessageEx(playerid, COLOR_GREY, "Ban khong the lam dieu nay vao thoi diem nay.");
		ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong the lam dieu nay vao thoi diem nay.");
		return 1;
	}

	if(TransportDuty[playerid] > 0)
	{
		if(TransportDuty[playerid] == 1)
		{
			TaxiDrivers -= 1;
		}
		else if(TransportDuty[playerid] == 2)
		{
			BusDrivers -= 1;
		}

		if(IsValidDynamic3DTextLabel(TransportLabel[playerid])) 
			DestroyDynamic3DTextLabel(TransportLabel[playerid]);

		PlayerWorking[playerid] = 0;
		TransportLabel[playerid] = Text3D:0;
		TransportDuty[playerid] = 0;
		new paymentMessage[96];
		format(paymentMessage, sizeof(paymentMessage), "Ban da ket thuc ca lam viec va kiem duoc $%d.", TransportMoney[playerid]);
		SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* Ban da ket thuc ca lam viec va kiem duoc $%d.", TransportMoney[playerid]);
		ShowTDN(playerid, NOTIFICATION_SUCCESS, paymentMessage);
		GivePlayerCash(playerid, TransportMoney[playerid]);
		new title[64];
		format(title, sizeof(title), "%s (ID: %d) - Tài xế Taxi/Bus", GetPlayerNameEx(playerid), playerid);
		DiscordLog(dclog_Taxi, title, "**Loại hình: %s**\n**Tiền nhận được: $%d**", TransportDuty[playerid] == 1 ? "Taxi" : "Bus", TransportMoney[playerid]);
		TransportValue[playerid] = 0; TransportMoney[playerid] = 0;
		SetPlayerToTeamColor(playerid);
		return 1;
	}

	if(sscanf(params, "d", fare)) return SendHelpMessage(playerid, "/fare [price]");

	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) 
	{
		SendClientMessageEx(playerid, COLOR_GREY, "Ban khong dang dieu khien phuong tien.");
		ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong dang dieu khien phuong tien.");
		return 1;
	}

	if(fare < 1 || fare > 300) 
	{
		SendClientMessageEx(playerid, COLOR_GREY, "Gia cuoc xe phai trong khoang $1 den $300!");
		ShowTDN(playerid, NOTIFICATION_WARNING, "Gia cuoc khong hop le!");
		return 1;
	}
	
	new newcar = GetPlayerVehicleID(playerid);
	if(IsAnBus(newcar))
	{	
		if(IsValidDynamic3DTextLabel(TransportLabel[playerid])) 
			DestroyDynamic3DTextLabel(TransportLabel[playerid]);

		PlayerWorking[playerid] = JOB_TAXIDRIVER;
		BusDrivers += 1; TransportDuty[playerid] = 2; TransportValue[playerid] = fare;
		new fareMessage[64];
		format(fareMessage, sizeof(fareMessage), "Ban bat dau lam tai xe Bus, gia ve: $%d.", TransportValue[playerid]);
		SendClientMessageEx(playerid, COLOR_WHITE, "Ban bat dau lam tai xe Bus, gia ve: $%d.", TransportValue[playerid]);
		ShowTDN(playerid, NOTIFICATION_SUCCESS, fareMessage);
		format(string, _, "Bus Driver #%03d\n{FFFFFF}Gia cuoc: {12FF55}$%d", BusDrivers, fare);
		TransportLabel[playerid] = CreateDynamic3DTextLabel(string, 0xF0FF1AFF, 0.0, 0.0, 0.0, 15.0, _, newcar, 1, GetVehicleVirtualWorld(newcar), GetVehicleInterior(newcar));
	}
	else
	{
		if(IsValidDynamic3DTextLabel(TransportLabel[playerid])) 
			DestroyDynamic3DTextLabel(TransportLabel[playerid]);

		PlayerWorking[playerid] = JOB_TAXIDRIVER;
		TaxiDrivers += 1; TransportDuty[playerid] = 1; TransportValue[playerid] = fare;
		new fareMessage[64];
		format(fareMessage, sizeof(fareMessage), "Ban bat dau lam tai xe Taxi, gia ve: $%d.", TransportValue[playerid]);
		SendClientMessageEx(playerid, COLOR_WHITE, "Ban bat dau lam tai xe Taxi, gia ve: $%d.", TransportValue[playerid]);
		ShowTDN(playerid, NOTIFICATION_SUCCESS, fareMessage);
		format(string, _, "Taxi Driver #%03d\n{FFFFFF}Gia cuoc: {12FF55}$%d", TaxiDrivers, fare);
		TransportLabel[playerid] = CreateDynamic3DTextLabel(string, 0xF0FF1AFF, 0.0, 0.0, 0.0, 15.0, _, newcar, 1, GetVehicleVirtualWorld(newcar), GetVehicleInterior(newcar));
	}
	Streamer_Update(playerid, STREAMER_TYPE_3D_TEXT_LABEL);

	if(IsATaxiDriver(playerid)) SetPlayerColor(playerid, COLOR_TAXI); 
	else SetPlayerColor(playerid, TEAM_TAXI_COLOR);

	return 1;
}

alias:ataxi("abus")
CMD:ataxi(playerid, params[])
{
	if(!IsATaxiDriver(playerid) && PlayerInfo[playerid][pJob] != JOB_TAXIDRIVER && PlayerInfo[playerid][pJob2] != JOB_TAXIDRIVER && PlayerInfo[playerid][pTaxiLicense] != 1)
	{
		SendClientMessageEx(playerid, COLOR_GREY, "Ban khong phai tai xe Taxi/Bus!");
		ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong phai tai xe Taxi/Bus!");
		return 1;
	}
	
	if(TransportDuty[playerid] == 0) 
	{
		SendClientMessageEx(playerid, COLOR_GREY, "Ban hien dang khong trong gio lam viec.");
		ShowTDN(playerid, NOTIFICATION_WARNING, "Ban hien dang khong trong gio lam viec.");
		return 1;
	}

	new
		szMessage[128],
		iTarget;
	if(TransportDuty[playerid] == 1)
	{
		if(sscanf(params, "u", iTarget)) return SendHelpMessage(playerid, "/ataxi [playerid]");
		if(!IsPlayerConnected(iTarget)) { ShowTDN(playerid, NOTIFICATION_WARNING, "Nguoi choi khong ton tai."); return 1; }
		if(iTarget == playerid) { ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong the chap nhan cuoc goi taxi cua ban."); return 1; }
		if(TaxiCallTime[playerid] > 0) { ShowTDN(playerid, NOTIFICATION_WARNING, "Ban da chap nhan mot cuoc goi."); return 1; }
		if(!GetPVarType(iTarget, "TaxiCall")) { ShowTDN(playerid, NOTIFICATION_WARNING, "Nguoi do khong goi xe taxi."); return 1; }

		format(szMessage, sizeof(szMessage), "** Tai xe Taxi %s da chap nhan cuoc goi tu %s (%d)" , GetPlayerNameEx(playerid), GetPlayerNameEx(iTarget), iTarget);
		SendTaxiMessage(TEAM_AZTECAS_COLOR, szMessage);
		format(szMessage, sizeof(szMessage), "* Tai xe Taxi %s da chap nhan cuoc goi cua ban, xin vui long giu vi tri de ho toi don.", GetPlayerNameEx(playerid));
		SendClientMessageEx(iTarget, COLOR_LIGHTBLUE, szMessage);
		SendMessageTD(playerid, 5000, "~w~Tai xe Taxi~n~~r~Di toi diem danh dau.");
		TaxiCallTime[playerid] = 1;
		TaxiAccepted[playerid] = iTarget;
		DeletePVar(iTarget, "TaxiCall");
	}
	if(TransportDuty[playerid] == 2)
	{
		if(sscanf(params, "u", iTarget)) return SendHelpMessage(playerid, "/abus [playerid]");
		if(!IsPlayerConnected(iTarget)) { ShowTDN(playerid, NOTIFICATION_WARNING, "Nguoi choi khong hop le."); return 1; }
		if(iTarget == playerid) { ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong the chap nhan cuoc goi bus cua ban."); return 1; }
		if(BusCallTime[playerid] > 0) { ShowTDN(playerid, NOTIFICATION_WARNING, "Ban da chap nhan mot cuoc goi."); return 1; }
		if(!GetPVarType(iTarget, "BusCall")) { ShowTDN(playerid, NOTIFICATION_WARNING, "Nguoi do khong goi xe bus."); return 1; }

		format(szMessage, sizeof(szMessage), "** Bus Driver %s da chap nhan cuoc goi Bus tu %s(%d)" , GetPlayerNameEx(playerid), GetPlayerNameEx(iTarget), iTarget);
		SendTaxiMessage(TEAM_AZTECAS_COLOR, szMessage);
		format(szMessage, sizeof(szMessage), "* Bus Driver %s da chap nhan cuoc goi taxi cua ban, xin vui long giu vi tri de ho toi don.", GetPlayerNameEx(playerid));
		SendClientMessageEx(iTarget, COLOR_LIGHTBLUE, szMessage);
		SendMessageTD(playerid, 5000, "~w~Nguoi lai Bus~n~~r~Di toi diem danh dau.");
		BusCallTime[playerid] = 1;
		BusAccepted[playerid] = iTarget;
		DeletePVar(iTarget, "BusCall");
	}
	PlayerInfo[playerid][pCallsAccepted]++;
	return 1;
}