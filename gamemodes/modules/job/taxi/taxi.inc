#include <YSI\YSI_Coding\y_hooks>

#define COLOR_TAXI_JOB      0xFFFF00FF

// Vi tri spawn xe taxi (gan NPC xin viec)
#define TAXI_VEH_X          -1976.2864
#define TAXI_VEH_Y          100.7167
#define TAXI_VEH_Z          27.4658
#define TAXI_VEH_A          89.1263

// ============================================================
// START / STOP
// ============================================================
Taxi_Start(playerid)
{
    if(PlayerWorking[playerid] == JOB_TAXIDRIVER)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban dang lam viec roi.");

    new vehicleid = CreateVehicle(420, TAXI_VEH_X, TAXI_VEH_Y, TAXI_VEH_Z, TAXI_VEH_A, 6, 6, -1);
    if(vehicleid == INVALID_VEHICLE_ID) {
        ShowTDN(playerid, NOTIFICATION_ERROR, "Loi: Khong the tao xe. Thu lai sau.");
        return 1;
    }

    LockStatus{vehicleid} = 0;
    VehicleFuel[vehicleid] = 100.0;
    SetVehicleHealth(vehicleid, 1000.0);
    PutPlayerInVehicle(playerid, vehicleid, 0);

    TaxiJobVehicle[playerid] = vehicleid;
    PlayerWorking[playerid] = JOB_TAXIDRIVER;
    SetPlayerSkin(playerid, JobInfo[JOB_TAXIDRIVER][jobSkin]);
    SetPlayerColor(playerid, COLOR_TAXI_JOB);

    SendClientMessageEx(playerid, COLOR_TAXI_JOB, "{FFFF00}[Taxi]{FFFFFF} Xe da san sang! Dung {FFFF00}/fare [gia]{FFFFFF} de bat dau nhan hanh khach.");
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Xe Taxi da san sang!");
    return 1;
}

Taxi_Stop(playerid)
{
    if(TransportDuty[playerid] > 0)
        PC_EmulateCommand(playerid, "/fare");

    new vehicleid = TaxiJobVehicle[playerid];
    if(vehicleid != INVALID_VEHICLE_ID && IsValidVehicle(vehicleid))
        DestroyVehicle(vehicleid);
    TaxiJobVehicle[playerid] = INVALID_VEHICLE_ID;

    PlayerWorking[playerid] = 0;
    SetPlayerSkin(playerid, PlayerInfo[playerid][pModel]);
    SetPlayerToTeamColor(playerid);

    SendClientMessageEx(playerid, COLOR_TAXI_JOB, "{FFFF00}[Taxi]{FFFFFF} Ban da tra xe va ket thuc ca lam viec.");
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Da ket thuc ca lam viec Taxi.");
    return 1;
}

// ============================================================
// HOOKS
// ============================================================
hook OnPlayerConnect(playerid)
{
    TaxiJobVehicle[playerid] = INVALID_VEHICLE_ID;
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    if(PlayerWorking[playerid] == JOB_TAXIDRIVER)
        Taxi_Stop(playerid);
    return 1;
}

// DIALOG_TAXI_NPC response
hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_TAXI_NPC)
    {
        if(!response) return 1;
        switch(listitem)
        {
            case 0: Taxi_Start(playerid);
            case 1:
            {
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "[================================================================================]");
                SendClientMessageEx(playerid, COLOR_TAXI_JOB,  "{FFFF00}[Taxi - Huong Dan]");
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "Buoc 1: {FFFFFF}Nhan {FFFF00}[Y]{FFFFFF} tai NPC chon 'Lay xe Taxi' de nhan xe.");
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "Buoc 2: {FFFFFF}Dung {FFFF00}/fare [gia]{FFFFFF} de bat dau nhan hanh khach (VD: /fare 50).");
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "Buoc 3: {FFFFFF}Khi hanh khach goi, dung {FFFF00}/ataxi [ID]{FFFFFF} de chap nhan.");
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "Buoc 4: {FFFFFF}Hanh khach se dat diem den tren ban do cua ban.");
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "Buoc 5: {FFFFFF}Dua hanh khach den noi, ho roi xe ban se nhan tien cuoc.");
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "Ket thuc: {FFFFFF}Dung {FFFF00}/dunglamviec{FFFFFF} de tra xe va ket thuc.");
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "[================================================================================]");
            }
        }
        return 1;
    }
    return 1;
}

// ============================================================

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