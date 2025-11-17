#include <YSI\YSI_Coding\y_hooks>

stock IsATruckerCar(carid)
{
	for(new v = 0; v < sizeof(TruckerVehicles); v++) {
	    if(carid == TruckerVehicles[v] && IsValidVehicle(TruckerVehicles[v])) return 1;
	}
	return 0;
}

stock IsAtTruckDeliveryPoint(playerid)
{
	for(new i = 0; i < sizeof(Dropoffs_Legal); i++)
	{
		if(IsPlayerInRangeOfPoint(playerid, 6.0, Dropoffs_Legal[i][PosX], Dropoffs_Legal[i][PosY], Dropoffs_Legal[i][PosZ])) {
		    return 1;
		}
	}

	for(new i = 0; i < sizeof(Dropoffs_Illegal); i++)
	{
		if(IsPlayerInRangeOfPoint(playerid, 6.0, Dropoffs_Illegal[i][PosX], Dropoffs_Illegal[i][PosY], Dropoffs_Illegal[i][PosZ])) {
		    return 1;
		}
	}
	return 0;
}

stock CancelTruckDelivery(playerid)
{
    new vehicleid = GetPlayerVehicleID(playerid);
	if(TruckDeliveringTo[TruckUsed[playerid]] != INVALID_BUSINESS_ID)
	{
	    if(TruckDeliveringTo[TruckUsed[playerid]] == BUSINESS_TYPE_GASSTATION)
	    {
	        DestroyVehicle(GetPVarInt(playerid, "Gas_TrailerID"));
	        DeletePVar(playerid, "Gas_TrailerID");
	    }
	    Businesses[TruckDeliveringTo[TruckUsed[playerid]]][bOrderState] = 1;
		SaveBusiness(TruckDeliveringTo[TruckUsed[playerid]]);
	}
	if(1 <= TruckUsed[playerid] <= MAX_VEHICLES){
		TruckDeliveringTo[TruckUsed[playerid]] = INVALID_BUSINESS_ID, TruckContents{TruckUsed[playerid]} = 0;
	}
	if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
		if(IsATruckerCar(vehicleid)) SetVehicleToRespawn(vehicleid);
	}
	gPlayerCheckpointStatus[playerid] = CHECKPOINT_NONE;
	TruckUsed[playerid] = INVALID_VEHICLE_ID;
 	DisablePlayerCP(playerid);
 	DeletePVar(playerid, "TruckDeliver");
	return 1;
}

// type 0: kiểm tra giửa business và dropoff
// type 1: chỉ business
// type 2: chỉ dropoff
stock DisplayOrders(playerid, type=0)
{
	new szDialog[2048];
	if(type != 2)
	{
		for (new i, j; i < MAX_BUSINESSES; i++)
		{
			if (Businesses[i][bOrderState] == 1)
			{
				if(Businesses[i][bType] > 0)
				{
					format(szDialog, sizeof(szDialog), "%s%s\t%s\n", szDialog, Businesses[i][bName], GetInventoryType(i));
					ListItemTrackId[playerid][j++] = i;
				}
			}
		}
	}

	if (!szDialog[0])
	{

		/*ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "Error", "No jobs available right now. Try again later.", "OK", "");
		TogglePlayerControllable(playerid, true);
		DeletePVar(playerid, "IsFrozen"); */
		if(GetVehicleModel(GetPlayerVehicleID(playerid)) == 456 || GetVehicleModel(GetPlayerVehicleID(playerid)) == 414)
		{
			ShowPlayerDialog(playerid, DIALOG_LOADTRUCKOLD, DIALOG_STYLE_LIST, "Ban muon van chuyen gi?", "{D5D822}[YEU CAU CAP DO TRUCKER 1]\n{00F70C}Hang hoa hop phap {FFFFFF}(hop phap va khong co tien thuong)\n \n{D5D822}[YEU CAU CAP DO TRUCKER 2]\n{FF0606}Hang hoa pham phap {FFFFFF}(nguy co bi canh sat bat va co tien thuong cao)", "Chon", "Dong");
		}
		else
		{
			ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "Loi", "Hien tai cong viec chua co san cho loai xe tai nay.", "Dong", "");
			TogglePlayerControllable(playerid, true);
			DeletePVar(playerid, "IsFrozen");
		}
	}
	else
	{
		if(type == 0)
		{
			ShowPlayerDialog(playerid, DIALOG_LOADTRUCKC, DIALOG_STYLE_MSGBOX, "{FFFFFF}Truck Delivery", "{FFFFFF}Co mot so doanh nghiep da dat don hang den Cong ty Van chuyen\nban co muon xem danh sach don hang khong?\n\n\nNhan \"{2BD822}Co{FFFFFF}\" de xem danh sach don hang, nhan \"{D82222}Khong{FFFFFF}\" de xem\ndanh sach hang hoa van chuyen khac.", "Co", "Khong");
			return 1;
		}
		
	    ShowPlayerDialog(playerid, DIALOG_LOADTRUCK, DIALOG_STYLE_LIST, "Don dat hang co san", szDialog, "Co", "Dong lai");
	}
	return 1;
}

forward HijackTruck(playerid);
public HijackTruck(playerid)
{
    new vehicleid = GetPlayerVehicleID(playerid);
  	new business = TruckDeliveringTo[vehicleid];

	SetPVarInt(playerid, "LoadTruckTime", GetPVarInt(playerid, "LoadTruckTime")-1);
	new string[128];
	SendMessageTD(playerid, 1200, "Con ~y~~h~%d~w~ giay nua de hoan thanh vu cuop", GetPVarInt(playerid, "LoadTruckTime"));
	if(GetPVarInt(playerid, "LoadTruckTime") > 0) SetTimerEx("HijackTruck", 1000, false, "d", playerid);

	if(GetPVarInt(playerid, "LoadTruckTime") <= 0)
	{
		DeletePVar(playerid, "IsFrozen");
		TogglePlayerControllable(playerid, true);
  		DeletePVar(playerid, "LoadTruckTime");

        if(!IsPlayerInVehicle(playerid, vehicleid))
        {
			TruckUsed[playerid] = INVALID_VEHICLE_ID;
			gPlayerCheckpointStatus[playerid] = CHECKPOINT_NONE;
 			DisablePlayerCP(playerid);
            SendClientMessageEx(playerid, COLOR_LIGHTBLUE,"* Ban khong co quyen chiem lo hang.");
			return 1;
        }


		foreach(new i: Player)
		{
  			if(TruckUsed[i] == vehicleid)
  			{
				DeletePVar(i, "LoadTruckTime");
				TruckUsed[i] = INVALID_VEHICLE_ID;
				DisablePlayerCP(i);
				gPlayerCheckpointStatus[i] = CHECKPOINT_NONE;
 				SendClientMessageEx(i, COLOR_WHITE, "Lo hang da bi cuop, vi vay ban khong the giao lo hang nay.");
			}
		}

  		TruckUsed[playerid] = vehicleid;

		new route = TruckRoute[vehicleid];
		SetPVarInt(playerid, "TruckDeliver", TruckContents{vehicleid});
		switch(TruckContents{vehicleid}) {
			case 0: {
				if(business != INVALID_BUSINESS_ID)
				{
					format(string, sizeof(string), "Ban da cuop lo hang %s", GetInventoryType(TruckDeliveringTo[vehicleid]));
					SendClientMessageEx(playerid, COLOR_LIGHTBLUE, string);

					SetPlayerCP(playerid, Businesses[business][bSupplyPos][0], Businesses[business][bSupplyPos][1], Businesses[business][bSupplyPos][2], 10.0);
				}
			}
			case 1: {
				SendClientMessageEx(playerid, COLOR_LIGHTBLUE,"Ban da cuop lo hang thuc pham va do uong");
				SetPlayerCP(playerid, Dropoffs_Legal[route][PosX], Dropoffs_Legal[route][PosY], Dropoffs_Legal[route][PosZ], 5);
			}
			case 2:
			{
				SendClientMessageEx(playerid, COLOR_LIGHTBLUE,"Ban da cuop lo hang quan ao");
				SetPlayerCP(playerid, Dropoffs_Legal[route][PosX], Dropoffs_Legal[route][PosY], Dropoffs_Legal[route][PosZ], 5);
			}
			case 3:
			{
				SendClientMessageEx(playerid, COLOR_LIGHTBLUE,"Ban da cuop lo hang vat lieu.");
				SetPlayerCP(playerid, Dropoffs_Legal[route][PosX], Dropoffs_Legal[route][PosY], Dropoffs_Legal[route][PosZ], 5);
			}
			case 4:
			{
				SendClientMessageEx(playerid, COLOR_LIGHTBLUE,"* Ban da cuop lo hang vat pham 24/7.");
				SetPlayerCP(playerid, Dropoffs_Legal[route][PosX], Dropoffs_Legal[route][PosY], Dropoffs_Legal[route][PosZ], 5);
			}
			case 5:
			{
				SendClientMessageEx(playerid, COLOR_LIGHTBLUE,"* Ban da cuop lo hang vu khi.");
				SetPlayerCP(playerid, Dropoffs_Illegal[route][PosX], Dropoffs_Illegal[route][PosY], Dropoffs_Illegal[route][PosZ], 5);
			}
			case 6:
			{
				SendClientMessageEx(playerid, COLOR_LIGHTBLUE,"Ban bi cuop lo hang thuoc.");
				SetPlayerCP(playerid, Dropoffs_Illegal[route][PosX], Dropoffs_Illegal[route][PosY], Dropoffs_Illegal[route][PosZ], 5);
			}
			case 7:
			{
				SendClientMessageEx(playerid, COLOR_LIGHTBLUE,"Ban bi cuop lo hang vat lieu pham phap.");
				SetPlayerCP(playerid, Dropoffs_Illegal[route][PosX], Dropoffs_Illegal[route][PosY], Dropoffs_Illegal[route][PosZ], 5);
			}
		}

		SendClientMessageEx(playerid, COLOR_REALRED, "CANH BAO: Coi chung nhung ten cuop, chung co the lay hang hoa cua ban va tau thoat....");
		SendClientMessageEx(playerid, COLOR_WHITE, "Giao hang den vi tri quy dinh (di theo diem danh dau tren mini-map).");
	}
	return 1;
}

forward LoadTruckOld(playerid);
public LoadTruckOld(playerid)
{
    SetPVarInt(playerid, "LoadTruckTime", GetPVarInt(playerid, "LoadTruckTime")-1);
	new string[128];
	format(string, _, "~n~~n~~n~~n~~n~~n~~n~~n~~n~~w~%d giay con lai", GetPVarInt(playerid, "LoadTruckTime"));
	GameTextForPlayer(playerid, string, 1100, 3);
	if(GetPVarInt(playerid, "LoadTruckTime") > 0) SetTimerEx("LoadTruckOld", 1000, false, "d", playerid);

	if(GetPVarInt(playerid, "LoadTruckTime") <= 0)
	{
		DeletePVar(playerid, "LoadTruckTime");
		DeletePVar(playerid, "IsFrozen");
		TogglePlayerControllable(playerid, true);

  		new vehicleid = GetPlayerVehicleID(playerid);
  		new truckdeliver = GetPVarInt(playerid, "TruckDeliver");
  		TruckContents{vehicleid} = truckdeliver;
  		TruckUsed[playerid] = vehicleid;

		new route;
		if(truckdeliver > 0 && truckdeliver < 5) route = Random((truckdeliver-1)*4, truckdeliver*4-1);
		else route = Random((truckdeliver-5)*5, (truckdeliver-4)*5-1);

		TruckRoute[vehicleid] = route;
		// 1 = food and bev
		// 2 = clothing
		// 3 = legal mats
		// 4 = 24/7 items
		// 5 = weapons
		// 6 = illegal drugs
		// 7 = illegal materials
		if(truckdeliver == 1)
		{
			SendClientMessageEx(playerid, COLOR_LIGHTBLUE,"* Xe cua ban da chat day thuc pham va do uong.");
			SetPlayerCP(playerid, Dropoffs_Legal[route][PosX], Dropoffs_Legal[route][PosY], Dropoffs_Legal[route][PosZ], 5);
		}
		else if(truckdeliver == 2)
		{
			SendClientMessageEx(playerid, COLOR_LIGHTBLUE,"* Xe cua ban da chat day quan ao.");
			SetPlayerCP(playerid, Dropoffs_Legal[route][PosX], Dropoffs_Legal[route][PosY], Dropoffs_Legal[route][PosZ], 5);
		}
		else if(truckdeliver == 3)
		{
			SendClientMessageEx(playerid, COLOR_LIGHTBLUE,"* Xe cua ban da chat day vat lieu.");
			SetPlayerCP(playerid, Dropoffs_Legal[route][PosX], Dropoffs_Legal[route][PosY], Dropoffs_Legal[route][PosZ], 5);
		}
		else if(truckdeliver == 4)
		{
			SendClientMessageEx(playerid, COLOR_LIGHTBLUE,"* Xe cua ban da chat day vat pham 24/7.");
			SetPlayerCP(playerid, Dropoffs_Legal[route][PosX], Dropoffs_Legal[route][PosY], Dropoffs_Legal[route][PosZ], 5);
		}
		else if(truckdeliver == 5)
		{
			SendClientMessageEx(playerid, COLOR_LIGHTBLUE,"* Xe cua ban da chat day vu khi.");
			SetPlayerCP(playerid, Dropoffs_Illegal[route][PosX], Dropoffs_Illegal[route][PosY], Dropoffs_Illegal[route][PosZ], 5);
		}
		else if(truckdeliver == 6)
		{
			SendClientMessageEx(playerid, COLOR_LIGHTBLUE,"* Xe cua ban da chat day thuoc.");
			SetPlayerCP(playerid, Dropoffs_Illegal[route][PosX], Dropoffs_Illegal[route][PosY], Dropoffs_Illegal[route][PosZ], 5);
		}
		else if(truckdeliver == 7)
		{
			SendClientMessageEx(playerid, COLOR_LIGHTBLUE,"* Xe cua ban da chat day vat lieu pham phap.");
			SetPlayerCP(playerid, Dropoffs_Illegal[route][PosX], Dropoffs_Illegal[route][PosY], Dropoffs_Illegal[route][PosZ], 5);
		}

		SendClientMessageEx(playerid, COLOR_REALRED, "CANH BAO: Coi chung nhung ten cuop, chung co the lay hang hoa cua ban va tau thoat...");
		new Float:distance = 0.0;
		if(truckdeliver >= 5)
		{
			distance = GetPlayerDistanceFromPoint(playerid, Dropoffs_Illegal[route][PosX], Dropoffs_Illegal[route][PosY], Dropoffs_Illegal[route][PosZ]);
			SendClientMessageEx(playerid, COLOR_REALRED, "CANH BAO #2: Ban dang dua hang pham phap, hay coi trung nhan vien thuc thi phap luat.");
		}
		else {
			distance = GetPlayerDistanceFromPoint(playerid, Dropoffs_Legal[route][PosX], Dropoffs_Legal[route][PosY], Dropoffs_Legal[route][PosZ]);
		}

		SendClientMessageEx(playerid, COLOR_WHITE, "HUONG DAN: Dua hang hoa yeu cau den dia diem nhan hang (di theo diem danh dau tren mini-map).");
		SendClientMessageEx(playerid, 0x4FF3A6FF, "GHI CHU: Khoang cach giao hang la %0.1f km.", distance / 1000);
		
		SetPVarInt(playerid, "tpTruckRunTimer", 30);
		SetTimerEx("OtherTimerEx", 1000, false, "ii", playerid, TYPE_TPTRUCKRUNTIMER);
	}
	return 1;
}

forward LoadTruck(playerid);
public LoadTruck(playerid)
{
    SetPVarInt(playerid, "LoadTruckTime", GetPVarInt(playerid, "LoadTruckTime")-1);
	new string[128];
	format(string, _, "~n~~n~~n~~n~~n~~n~~n~~n~~n~~w~%d giay con lai", GetPVarInt(playerid, "LoadTruckTime"));
	GameTextForPlayer(playerid, string, 1100, 3);
	if(GetPVarInt(playerid, "LoadTruckTime") > 0) SetTimerEx("LoadTruck", 1000, false, "d", playerid);

	if(GetPVarInt(playerid, "LoadTruckTime") <= 0)
	{
		DeletePVar(playerid, "LoadTruckTime");
		DeletePVar(playerid, "IsFrozen");
		TogglePlayerControllable(playerid, true);

  		new vehicleid = GetPlayerVehicleID(playerid);
  		new business = TruckDeliveringTo[vehicleid];
  		TruckUsed[playerid] = vehicleid;


		gPlayerCheckpointStatus[playerid] = CHECKPOINT_DELIVERY;

		format(string, sizeof(string), "* Xe tai cua ban da day %s", GetInventoryType(business));
		SendClientMessageEx(playerid, COLOR_LIGHTBLUE, string);

        SetPlayerCP(playerid, Businesses[business][bSupplyPos][0], Businesses[business][bSupplyPos][1], Businesses[business][bSupplyPos][2], 10.0);
		new Float:distance = GetPlayerDistanceFromPoint(playerid, Businesses[business][bSupplyPos][0], Businesses[business][bSupplyPos][1], Businesses[business][bSupplyPos][2]);
		
		SendClientMessageEx(playerid, COLOR_WHITE, "HUONG DAN: Dua hang hoa yeu cau den dia diem nhan hang (di theo diem danh dau tren mini-map).");
		SendClientMessageEx(playerid, COLOR_REALRED, "CANH BAO: Coi chung nhung ten cuop, chung co the lay hang hoa cua ban va tau thoat...");
		SendClientMessageEx(playerid, 0x4FF3A6FF, "GHI CHU: Khoang cach giao hang la %0.1f km.", distance / 1000);

		if (Businesses[business][bType] == BUSINESS_TYPE_GUNSHOP)
		{
			SendClientMessageEx(playerid, COLOR_REALRED, "CANH BAO #2: Ban dang dua hang pham phap, hay coi chung nhan vien thuc thi phap luat.");
		}
		else if (Businesses[business][bType] == BUSINESS_TYPE_GASSTATION)
		{
		  	new Float:x, Float:y, Float:z, Float:ang;
		  	SetVehiclePos(vehicleid, -1570.9833,96.7547,4.1442);
		  	SetVehicleZAngle(vehicleid, 136.18);
		    GetPlayerPos(playerid, x, y, z);
		    GetVehicleZAngle(vehicleid, ang);
		    new iTrailer = CreateVehicle(584, x, y, z+1, ang, -1, -1, 1000);
		    SetPVarInt(playerid, "Gas_TrailerID", iTrailer);
			SetTimerEx("AttachGasTrailer", 500, false, "ii", iTrailer, vehicleid);
		}
		/*else if (Businesses[business][bType] == BUSINESS_TYPE_NEWCARDEALERSHIP)
		{
			new iModel, iSlot;
		    for (new i; i < MAX_BUSINESS_DEALERSHIP_VEHICLES; i++)
		    {
			    if (Businesses[business][DealershipVehOrder][i]) {
					iModel = Businesses[business][bModel][i];
					iSlot = i;
	 			}
		    }
			new Float: fVehPos[4];
			GetVehiclePos(vehicleid, fVehPos[0], fVehPos[1], fVehPos[2]);
			GetVehicleZAngle(vehicleid, fVehPos[3]);
			new iDeliveredVeh = CreateVehicle(iModel, fVehPos[0], fVehPos[1], fVehPos[2] + 3, fVehPos[3], 1, 1, -1);
			SetVehicleZAngle(iDeliveredVeh, fVehPos[3]);
			vehicle_lock_doors(iDeliveredVeh);

			SetPVarInt(playerid, "CarryingVehicle", iDeliveredVeh);
			SetPVarInt(playerid, "CarryingSlot", iSlot);
		} */
		SetPVarInt(playerid, "tpTruckRunTimer", floatround(GetPlayerDistanceFromPoint(playerid, Businesses[business][bSupplyPos][0], Businesses[business][bSupplyPos][1], Businesses[business][bSupplyPos][2]) / 100));
		SetTimerEx("OtherTimerEx", 1000, false, "ii", playerid, TYPE_TPTRUCKRUNTIMER);
	}
	return 1;
}

alias:kiemtrahang("checkcargo")
CMD:kiemtrahang(playerid, params[])
{
	if(PlayerInfo[playerid][pJob] != JOB_TRUCKER && PlayerInfo[playerid][pJob2] != JOB_TRUCKER && !IsACop(playerid))
	{
        SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong phai chu lo hang hoac canh sat!");
        return 1;
	}

	new carid = GetPlayerVehicleID(playerid);
 	new closestcar = GetClosestCar(playerid, carid);
  	if(IsPlayerInRangeOfVehicle(playerid, closestcar, 6.0) && IsATruckerCar(closestcar))
	{
		if(IsPlayerInAnyVehicle(playerid))
		{
			SendClientMessageEx(playerid, COLOR_GRAD1, "Ban khong the kiem tra lo hang khi dang trong xe.");
			return 1;
		}
		new engine, lights, alarm, doors, bonnet, boot, objective;
		GetVehicleParamsEx(closestcar,engine,lights,alarm,doors,bonnet,boot,objective);
		if(boot == VEHICLE_PARAMS_OFF || boot == VEHICLE_PARAMS_UNSET)
		{
			SendClientMessageEx(playerid, COLOR_GRAD1, "Than xe phai duoc mo ra moi co the kiem tra.");
			return 1;
		}

		new string[128];

     	SendClientMessageEx(playerid, COLOR_GREEN,"_______________ LO HANG _______________");
		if(IsPlayerInVehicle(playerid, closestcar))
  		{
    		SendClientMessageEx(playerid, COLOR_WHITE, "Co mot nguoi dang dieu khien xe, nguoi do phai xuong xe moi co the tiep tuc kiem tra lo hang.");
      		return 1;
		}
		new iTruckContents = TruckContents{closestcar};
		new truckcontentname[50];
		if(iTruckContents == 1)
		{ format(truckcontentname, sizeof(truckcontentname), "{00F70C}Thuc an & Do uong");}
		else if(iTruckContents == 2)
		{ format(truckcontentname, sizeof(truckcontentname), "{00F70C}Quan ao"); }
		else if(iTruckContents == 3)
		{ format(truckcontentname, sizeof(truckcontentname), "{00F70C}Vat lieu hop phap"); }
		else if(iTruckContents == 4)
		{ format(truckcontentname, sizeof(truckcontentname), "{00F70C}Vat pham 24/7"); }
		else if(iTruckContents == 5)
		{ format(truckcontentname, sizeof(truckcontentname), "{FF0606}Vu khi"); }
		else if(iTruckContents == 6)
		{ format(truckcontentname, sizeof(truckcontentname), "{FF0606}Ma tuy"); }
		else if(iTruckContents == 7)
		{ format(truckcontentname, sizeof(truckcontentname), "{FF0606}Vat lieu pham phap"); }
		format(string, sizeof(string), "Giay to xe: %s (%d)", GetVehicleName(closestcar), closestcar);
		SendClientMessageEx(playerid, COLOR_WHITE, string);
		if(iTruckContents == 0)
		{ format(truckcontentname, sizeof(truckcontentname), "%s",  GetInventoryType(TruckDeliveringTo[closestcar])); }
		format(string, sizeof(string), "Hang hoa: %s", truckcontentname);
		SendClientMessageEx(playerid, COLOR_WHITE, string);

		if(IsACop(playerid))
		{
			SendClientMessageEx(playerid, COLOR_YELLOW, "De loai bo hang hoa pham phap, su dung /clearcargo gan xe tai.");
		}

		if(PlayerInfo[playerid][pJob] == JOB_TRUCKER || PlayerInfo[playerid][pJob2] == JOB_TRUCKER)
		{
			if(TruckDeliveringTo[closestcar] > 0 && TruckUsed[playerid] == INVALID_VEHICLE_ID)
			{
				SendClientMessageEx(playerid, COLOR_YELLOW, "De cuop hang hoa, su dung /cuophang.");
			}
			else if(TruckUsed[playerid] == INVALID_VEHICLE_ID)
			{
				SendClientMessageEx(playerid, COLOR_YELLOW, "De cung cap hang hoa, su dung /layhang cho tai xe.");
			}
			else if(TruckUsed[playerid] == closestcar && gPlayerCheckpointStatus[playerid] == CHECKPOINT_RETURNTRUCK)
			{
				SendClientMessageEx(playerid, COLOR_YELLOW, "Ban chua tra xe lai cho ben cang, luong cua ban khong duoc tra.");
			}
			else if(TruckUsed[playerid] == closestcar)
			{
				SendClientMessageEx(playerid, COLOR_YELLOW, "Day la xe cua ban, ban da khong giao hang dung hen.");
			}
			else if(TruckUsed[playerid] != INVALID_VEHICLE_ID)
			{
				SendClientMessageEx(playerid, COLOR_YELLOW, "Ban da co cong viec khac. Su dung /huybo giaohang de huy bo viec giao hang.");
			}
		}
     	SendClientMessageEx(playerid, COLOR_GREEN,"_________________________________________________________");
    }
	else
	{
 		SendClientMessageEx(playerid, COLOR_GRAD1, "Ban khong dung gan xe van chuyen hang hoa.");
 	}
    return 1;
}

CMD:clearcargo(playerid, params[])
{
	if(!IsACop(playerid))
	{
        SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong phai nhan vien thuc thi phap luat!");
        return 1;
	}

	new carid = GetPlayerVehicleID(playerid);
 	new closestcar = GetClosestCar(playerid, carid);
  	if(IsPlayerInRangeOfVehicle(playerid, closestcar, 6.0) && IsATruckerCar(closestcar))
	{
		if(IsPlayerInAnyVehicle(playerid))
		{
		    SendClientMessageEx(playerid, COLOR_GRAD1, "Ban khong xoa hang hoa khi dang ben trong mot chiec xe.");
		    return 1;
		}
		if(TruckContents{closestcar} == 0)
		{
		 	if(TruckDeliveringTo[closestcar] != INVALID_BUSINESS_ID && (Businesses[TruckDeliveringTo[closestcar]][bType] != BUSINESS_TYPE_GASSTATION || Businesses[TruckDeliveringTo[closestcar]][bType] == BUSINESS_TYPE_NEWCARDEALERSHIP))
			{
				SendClientMessageEx(playerid, COLOR_WHITE, "Ban khong the lay hang hoa di.");
				return 1;
			}
		}
		new truckcontentname[50];
		new iTruckContents = TruckContents{closestcar};
		if(iTruckContents >= 0 && iTruckContents < 5)
		{
			SendClientMessageEx(playerid, COLOR_WHITE, "Khong co vat pham pham phap tren xe.");
			return 1;
		}
		else if(TruckDeliveringTo[closestcar] == INVALID_BUSINESS_ID && iTruckContents == 0)
		{
			SendClientMessageEx(playerid, COLOR_WHITE, "Khong co vat pham pham phap tren xe.");
			return 1;
		}
		if(iTruckContents == 5)
		{ format(truckcontentname, sizeof(truckcontentname), "{FF0606}Vu khi pham phap"); }
		else if(iTruckContents == 6)
		{ format(truckcontentname, sizeof(truckcontentname), "{FF0606}Ma tuy pham phap"); }
		else if(iTruckContents == 7)
		{ format(truckcontentname, sizeof(truckcontentname), "{FF0606}Vat lieu pham phap"); }
		else format(truckcontentname, sizeof(truckcontentname), "{FF0606}Vat lieu pham phap");
 		foreach(new i: Player)
		{
  			if(TruckUsed[i] == closestcar)
     		{
				TruckUsed[i] = INVALID_VEHICLE_ID;
				TruckDeliveringTo[closestcar] = INVALID_BUSINESS_ID;
				TruckContents{closestcar} = 0;
				TruckRoute[closestcar] = 0;
				DisablePlayerCP(i);
				gPlayerCheckpointStatus[i] = CHECKPOINT_NONE;
				DeletePVar(i, "TruckDeliver");
 				SendClientMessageEx(i, COLOR_WHITE, "Viec giao hang cua ban that bai, mot nhan vien canh sat da tich thu lo hang cua ban.");
       		}
		}
		new string[128];
		format(string, sizeof(string), "Ban da loai bo %s {FFFFFF}khoi chiec xe.", truckcontentname);
		SendClientMessageEx(playerid, COLOR_WHITE, string);
		format(string, sizeof(string), "* %s da lay vat pham pham phap ra khoi xe.", GetPlayerNameEx(playerid));
		ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
    }
   	else
	{
 		SendClientMessageEx(playerid, COLOR_GRAD1, "Ban khong dung gan mot chiec xe giao hang.");
 	}
    return 1;
}

CMD:cuophang(playerid, params[])
{
	if(PlayerInfo[playerid][pJob] == JOB_TRUCKER || PlayerInfo[playerid][pJob2] == JOB_TRUCKER)
	{
	    new vehicleid = GetPlayerVehicleID(playerid);
	    if(IsATruckerCar(vehicleid) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	    {
     		if(!CheckPointCheck(playerid))
	        {
         		if(GetPVarInt(playerid, "LoadTruckTime") > 0)
	            {
	                SendClientMessageEx(playerid, COLOR_WHITE, "Ban dang doi van chuyen hang hoa len xe tai cua ban!");
					return 1;
	            }
	            if(TruckUsed[playerid] != INVALID_VEHICLE_ID)
	            {
	                SendClientMessageEx(playerid, COLOR_WHITE, "Ban da co mot chuyen hang truoc do, su dung \"{50FFCD}/huybo giaohang\"{FFFFFF} de huy bo don dat hang hien tai.");
					return 1;
	            }
				if(TruckDeliveringTo[vehicleid] == 0 && TruckContents{vehicleid} == 0)
				{
				    SendClientMessageEx(playerid, COLOR_WHITE, "Xe tai nay khong co bat ki hang hoa nao!");
				    return 1;
				}
				if(IsPlayerInRangeOfPoint(playerid, 65.0, LoadTruckPosition[0], LoadTruckPosition[1], LoadTruckPosition[2]))
				{
				    SendClientMessageEx(playerid, COLOR_WHITE, "Ban khong the cuop hang khi xe dang o ben cang San Fierro!");
					return 1;
				}
			

				SendClientMessageEx(playerid, COLOR_LIGHTBLUE,"* Ban dang co gang cuop mot lo hang tu xe tai, xin vui long cho...");
				TogglePlayerControllable(playerid, false);
				SetPVarInt(playerid, "IsFrozen", 1);

				SetPVarInt(playerid, "LoadTruckTime", 10);
				SetTimerEx("HijackTruck", 1000, false, "d", playerid);
	        }
	        else SendClientMessageEx(playerid, COLOR_WHITE, "Xin hay chac chan rang cac diem danh dau cua ban da bi xoa bo.");
	    }
	    else SendClientMessageEx(playerid, COLOR_GREY, "Ban khong lai xe van chuyen hang hoa!");
	}
	else SendClientMessageEx(playerid, COLOR_GREY, "Ban khong phai la nguoi van chuyen hang hoa");
	return 1;
}

alias:layhang("loadshipment")
CMD:layhang(playerid, params[])
{
	if(PlayerInfo[playerid][pJob] == JOB_TRUCKER || PlayerInfo[playerid][pJob2] == JOB_TRUCKER)
	{
	    new vehicleid = GetPlayerVehicleID(playerid);
	    if(IsATruckerCar(vehicleid) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	    {
	        if(!CheckPointCheck(playerid))
	        {
	            if(GetPVarInt(playerid, "LoadTruckTime") > 0)
	            {
	                SendClientMessageEx(playerid, COLOR_WHITE, "Ban dang dua hang len xe roi!");
					return 1;
	            }
	            if(TruckUsed[playerid] != INVALID_VEHICLE_ID)
	            {
	                SendClientMessageEx(playerid, COLOR_WHITE, "Ban dang giao mat hang khac, su dung \"{50FFCD}/huybo giaohang\"{FFFFFF} de huy bo giao hang.");
					return 1;
	            }
				if(TruckContents{vehicleid} != 0)
				{
				    return SendClientMessageEx(playerid, COLOR_GRAD2, "Da dua hang len xe thanh cong.");
				}
				if(TruckDeliveringTo[vehicleid] != INVALID_BUSINESS_ID && TruckContents{vehicleid} == 0)
				{
				    return SendClientMessageEx(playerid, COLOR_GRAD2, "Da dua hang len xe thanh cong.");
				}

	            gPlayerCheckpointStatus[playerid] = CHECKPOINT_LOADTRUCK;
				//SetPlayerCP(playerid, -1572.767822, 81.137527, 3.554687, 5.0); old checkpoint
				SetPlayerCP(playerid, LoadTruckPosition[0], LoadTruckPosition[1], LoadTruckPosition[2], 8.0);
				SendMessageTD(playerid, 5000, "Dia diem ~r~~h~~h~Ben cang San Fierro");
				SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* Chon hang hoa de van chuyen tai ben cang San Fierro (Diem danh dau).");

	        }
	        else return SendClientMessageEx(playerid, COLOR_WHITE, "Xin hay chac chan rang cac diem checkpoint cua ban da bi xoa bo.");
	    }
	    else return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong lai chiec xe tai cho hang hoa yeu cau!");
	}
	else return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong phai Nguoi dua hang!");
	return 1;
}

hook OnPlayerEnterCheckpoint(playerid)
{
	switch(gPlayerCheckpointStatus[playerid])
	{
		case CHECKPOINT_LOADTRUCK:
		{
			if(IsPlayerInRangeOfPoint(playerid, 10.0, LoadTruckPosition[0], LoadTruckPosition[1], LoadTruckPosition[2]))
			{
				new vehicleid = GetPlayerVehicleID(playerid);
				if(IsATruckerCar(vehicleid) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
				{
					PlayerPlaySound(playerid, 1058, 0.0, 0.0, 0.0);
					DisablePlayerCP(playerid);
					gPlayerCheckpointStatus[playerid] = CHECKPOINT_NONE;
					TogglePlayerControllable(playerid, false);
					SetPVarInt(playerid, "IsFrozen", 1);
					DisplayOrders(playerid);
				}
				else return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong lai dung xe tai hoac tau truck!");
			}
		}
		case CHECKPOINT_RETURNTRUCK:
		{
			if(!IsPlayerInRangeOfPoint(playerid, 6.0, -1548.087524, 123.590423, 3.554687))
			{
				new vehicleid = GetPlayerVehicleID(playerid);
				if(IsATruckerCar(vehicleid) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
				{
					PlayerPlaySound(playerid, 1058, 0.0, 0.0, 0.0);
					DisablePlayerCP(playerid);
					gPlayerCheckpointStatus[playerid] = CHECKPOINT_NONE;
				}// In the case the person finds a way to exploit the checkpoint to different location
				CancelTruckDelivery(playerid);
				SendClientMessageEx(playerid, COLOR_REALRED, "ERROR: Wrong checkpoint entered. Truck delivery canceled completely.");
				return 1;
			}

			new bool:isTeleporting = false;
			if(GetPVarInt(playerid, "tpTruckRunTimer") != 0)
			{
				new string[128];
				format(string, sizeof(string), "{AA3333}AdmWarning{FFFF00}: %s (ID %d) is possibly teleport truck/boat running.", GetPlayerNameEx(playerid), playerid);
				ABroadCast( COLOR_YELLOW, string, 2 );
				isTeleporting = true;
				// format(string, sizeof(string), "%s (ID %d) is possibly teleport truckrunning.", GetPlayerNameEx(playerid), playerid);
				// Log("logs/hack.log", string);
			}
			new vehicleid = GetPlayerVehicleID(playerid);
			if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
			{
				SendClientMessageEx(playerid, COLOR_WHITE, "Ban khong o trong xe van chuyen hang!");
				return 1;
			}
			if(TruckUsed[playerid] != INVALID_VEHICLE_ID && vehicleid != TruckUsed[playerid])
			{
				SendClientMessageEx(playerid, COLOR_WHITE, "Day khong phai la chiec xe cua ban van chuyen hang, vui long su dung dung xe cua ban de nhan thu lao!");
				return 1;
			}

			PlayerPlaySound(playerid, 1058, 0.0, 0.0, 0.0);
			DisablePlayerCP(playerid);
			gPlayerCheckpointStatus[playerid] = CHECKPOINT_NONE;
			new route = TruckRoute[vehicleid];
			new string[128], payment;

			switch(GetJobLevel(playerid, JOB_TRUCKER))
			{
				case 1: payment = 200;
				case 2: payment = 600;
				case 3: payment = 950;
				case 4: payment = 1400;
				case 5: payment = 2700;
			}

			new reward[128];
			new truckdeliver = GetPVarInt(playerid, "TruckDeliver");
			strcat(reward, "Không có");
			if(TruckDeliveringTo[vehicleid] != INVALID_BUSINESS_ID) {
				new iBusiness = TruckDeliveringTo[vehicleid];
				new Float: iDist = GetPlayerDistanceFromPoint(playerid, Businesses[iBusiness][bSupplyPos][0], Businesses[iBusiness][bSupplyPos][1], Businesses[iBusiness][bSupplyPos][2]);

				payment = floatround(iDist / 2000 * payment);
				if (payment > 12000) payment = 12000;

				GivePlayerCash(playerid, payment);
				format(string, sizeof(string), "* Ban duoc tra {5BFF89}$%s{FFFFFF} cho viec van chuyen hang hoa va tra lai xe.", number_format(payment));

				SendClientMessageEx(playerid, COLOR_WHITE, string);
				SetVehicleToRespawn(vehicleid);
			}
			else {
				DeletePVar(playerid, "LoadType");
				TruckContents{vehicleid} = 0;

				if(truckdeliver >= 1 && truckdeliver < 5)
				{
					new Float:distancepay;
					distancepay = floatmul(GetDistance(LoadTruckPosition[0], LoadTruckPosition[1], LoadTruckPosition[2], Dropoffs_Legal[route][PosX], Dropoffs_Legal[route][PosY], Dropoffs_Legal[route][PosZ]), 0.3);
					payment += floatround(distancepay);
					GivePlayerCash(playerid, payment);
					format(string, sizeof(string), "* Ban duoc tra {5BFF89}$%s{FFFFFF} cho viec van chuyen hang hoa va tra lai xe.", number_format(payment));
				}
				else if(truckdeliver >= 5 && truckdeliver <= 7)
				{
					new Float:distancepay;
					distancepay = floatmul(GetDistance(LoadTruckPosition[0], LoadTruckPosition[1], LoadTruckPosition[2], Dropoffs_Illegal[route][PosX], Dropoffs_Illegal[route][PosY], Dropoffs_Illegal[route][PosZ]), 0.3);
					payment += floatround(distancepay);
					payment = floatround(payment * 1.10);
					GivePlayerCash(playerid, payment);
					format(string, sizeof(string), "* Ban duoc tra {5BFF89}$%s{FFFFFF} cho viec van chuyen hang hoa va tra lai xe.", number_format(payment));
					SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* Ban nhan duoc 10%% tien thuong do rui ro cua hang hoa.");

				}
				SendClientMessageEx(playerid, COLOR_WHITE, string);
				if(truckdeliver == 5) // Weapons
				{
					if(PlayerInfo[playerid][pConnectHours] >= 2 && PlayerInfo[playerid][pWRestricted] <= 0)
					{
						if(GetJobLevel(playerid, JOB_TRUCKER) == 2)
						{
							format(reward, _, "9MM");
							SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* Ban nhan duoc mien phi 9mm cho viec van chuyen vu khi trai phep.");
							GivePlayerValidWeapon(playerid, 22, 60000);
						}
						else if(GetJobLevel(playerid, JOB_TRUCKER) == 3)
						{
							format(reward, _, "Shotgun");
							SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* Ban nhan duoc mien phi Shotgun cho viec van chuyen vu khi trai phep.");
							GivePlayerValidWeapon(playerid, 25, 60000);
						}
						else if(GetJobLevel(playerid, JOB_TRUCKER) == 4)
						{
							format(reward, _, "MP5");
							SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* Ban nhan duoc mien phi MP5 cho viec van chuyen vu khi trai phep.");
							GivePlayerValidWeapon(playerid, 29, 60000);
						}
						else if(GetJobLevel(playerid, JOB_TRUCKER) == 5)
						{
							format(reward, _, "Desert Eagle");
							SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* Ban nhan duoc mien phi Desert Eagle cho viec van chuyen vu khi trai phep.");
							GivePlayerValidWeapon(playerid, 24, 60000);
						}
					}
					else
					{
						SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* Ban khong nhan duoc vu khi do ban co mot lenh gioi han vu khi.");
					}
				}
				if(truckdeliver == 6) // Drugs
				{

					if(GetJobLevel(playerid, JOB_TRUCKER) == 2)
					{
						format(reward, _, "2 Pot và 1 Crack");
						SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* Ban da nhan duoc 2 pot & 1 cho viec van chuyen ma tuy bat hop phap.");
						PlayerInfo[playerid][pPot] += 2;
						PlayerInfo[playerid][pCrack] += 1;
					}
					else if(GetJobLevel(playerid, JOB_TRUCKER) == 3)
					{
						format(reward, _, "4 Pot và 2 Crack");
						SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* Ban da nhan duoc 4 pot & 2 cho viec van chuyen ma tuy bat hop phap.");
						PlayerInfo[playerid][pPot] += 4;
						PlayerInfo[playerid][pCrack] += 2;
					}
					else if(GetJobLevel(playerid, JOB_TRUCKER) == 4)
					{
						format(reward, _, "6 Pot và 3 Crack");
						SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* Ban da nhan duoc 6 pot & 3 cho viec van chuyen ma tuy bat hop phap.");
						PlayerInfo[playerid][pPot] += 6;
						PlayerInfo[playerid][pCrack] += 3;
					}
					else if(GetJobLevel(playerid, JOB_TRUCKER) == 5)
					{
						format(reward, _, "8 Pot và 4 Crack");
						SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* Ban da nhan duoc 8 pot & 4 cho viec van chuyen ma tuy bat hop phap.");
						PlayerInfo[playerid][pPot] += 8;
						PlayerInfo[playerid][pCrack] += 4;
					}
				}
				if(truckdeliver == 7) // Illegal materials
				{
					if(GetJobLevel(playerid, JOB_TRUCKER) == 2)
					{
						format(reward, _, "100 Vật liệu");
						SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* Ban da nhan duoc 100 cho viec van chuyen vat lieu bat hop phap.");
						PlayerInfo[playerid][pMats] += 100;
					}
					else if(GetJobLevel(playerid, JOB_TRUCKER) == 3)
					{
						format(reward, _, "200 Vật liệu");
						SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* Ban da nhan duoc 200 cho viec van chuyen vat lieu bat hop phap.");
						PlayerInfo[playerid][pMats] += 200;
					}
					else if(GetJobLevel(playerid, JOB_TRUCKER) == 4)
					{
						format(reward, _, "400 Vật liệu");
						SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* Ban da nhan duoc 400 cho viec van chuyen vat lieu bat hop phap.");
						PlayerInfo[playerid][pMats] += 400;
					}
					else if(GetJobLevel(playerid, JOB_TRUCKER) == 5)
					{
						format(reward, _, "600 Vật liệu");
						SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* Ban da nhan duoc 600 cho viec van chuyen vat lieu bat hop phap.");
						PlayerInfo[playerid][pMats] += 600;
					}
				}
				SetVehicleToRespawn(vehicleid);
			}

			new rand_xp = 0;
			if(DoubleXP) {
				rand_xp = Random(PlayerInfo[playerid][pLevel] * XP_RATE - 3, PlayerInfo[playerid][pLevel] * XP_RATE + 3);
				PlayerInfo[playerid][pTruckSkill] += 2;
				SendClientMessageEx(playerid, COLOR_YELLOW, "Kinh nghiem nhan vat: +%d XP (%s/%s) | +2 Trucker Skill (DOUBLE XP)", rand_xp*2, number_format(PlayerInfo[playerid][pXP]+rand_xp*2), number_format(GetRequiredXP(playerid)));
				GivePlayerXP(playerid, rand_xp*2);
			}
			else
			if(PlayerInfo[playerid][pDoubleEXP] > 0 && !DoubleXP)
			{
				rand_xp = Random(PlayerInfo[playerid][pLevel] * XP_RATE - 3, PlayerInfo[playerid][pLevel] * XP_RATE + 3);
				PlayerInfo[playerid][pTruckSkill] += 2;
				SendClientMessageEx(playerid, COLOR_YELLOW, "Kinh nghiem nhan vat: +%d XP (%s/%s) | +2 Trucker Skill (DOUBLE XP)", rand_xp*2, number_format(PlayerInfo[playerid][pXP]+rand_xp*2), number_format(GetRequiredXP(playerid)));
				SendClientMessageEx(playerid, COLOR_YELLOW, "Ban co %d gio con lai de nhan doi EXP Token.", PlayerInfo[playerid][pDoubleEXP]);
				GivePlayerXP(playerid, rand_xp*2);
			}
			else
			{
				rand_xp = Random(PlayerInfo[playerid][pLevel] * XP_RATE - 3, PlayerInfo[playerid][pLevel] * XP_RATE + 3);
				PlayerInfo[playerid][pTruckSkill] += 1;
				SendClientMessageEx(playerid, COLOR_YELLOW, "Kinh nghiem nhan vat: +%d XP (%s/%s) | +1 Trucker Skill", rand_xp, number_format(PlayerInfo[playerid][pXP]+rand_xp), number_format(GetRequiredXP(playerid)));
				GivePlayerXP(playerid, rand_xp);
			}

			new title[64];
			format(title, _, "%s (ID: %d) - Giao hàng", GetPlayerNameEx(playerid), playerid);
			DiscordLog(dclog_Trucker, title, "**Tiền nhận được: $%s**\n**Kinh nghiệm nhân vật: +%d XP**\n**Loại hàng: %s**\n**Phần thưởng: %s**%s", number_format(payment), rand_xp, (truckdeliver < 5) ? ("HỢP PHÁP") : ("BẤT HỢP PHÁP"), reward, (isTeleporting) ? ("\n\n:shield: ***NGHI VẤN NGƯỜI CHƠI ĐANG GIAN LẬN***") : (""));

			TruckUsed[playerid] = INVALID_VEHICLE_ID;
			DeletePVar(playerid, "TruckDeliver");

			new mypoint = -1;
			for (new i=0; i<MAX_POINTS; i++)
			{
				if(strcmp(Points[i][Name], "San Fierro Docks", true) == 0)
				{
					mypoint = i;
				}
			}
			for(new i = 0; i < sizeof(FamilyInfo); i++)
			{
				if(mypoint != -1 && strcmp(Points[mypoint][Owner], FamilyInfo[i][FamilyName], true) == 0)
				{
					Misc_Save();
					FamilyInfo[i][FamilyBank] = FamilyInfo[i][FamilyBank]+(200);
				}
			}
		}
	}
	return 1;
}