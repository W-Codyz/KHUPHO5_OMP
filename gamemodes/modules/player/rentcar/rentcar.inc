#include <YSI\YSI_Coding\y_hooks>

stock LoadPlayerRentalCar(playerid)
{
    new query[128];
    mysql_format(MainPipeline, query, sizeof(query), "SELECT * FROM `rentalcar` WHERE `OwnerID` = '%d'", GetPlayerSQLId(playerid));
    mysql_tquery(MainPipeline, query, "OnLoadPlayerRentalCar", "d", playerid);
    return 1;
}

forward OnLoadPlayerRentalCar(playerid);
public OnLoadPlayerRentalCar(playerid)
{
    if(!cache_num_rows()) return 0;

    new count = 0;
    for(new i=0; i < cache_num_rows();i++)
    {
        CarRentalInfo[playerid][count][rcSqlid] = cache_get_field_content_int(i, "id", MainPipeline);
        CarRentalInfo[playerid][count][rcModel] = cache_get_field_content_int(i, "Model", MainPipeline);
        CarRentalInfo[playerid][count][rcTime] = cache_get_field_content_int(i, "Time", MainPipeline);
        CarRentalInfo[playerid][count][rcPosX] = cache_get_field_content_float(i, "PosX", MainPipeline);
        CarRentalInfo[playerid][count][rcPosY] = cache_get_field_content_float(i, "PosY", MainPipeline);
        CarRentalInfo[playerid][count][rcPosZ] = cache_get_field_content_float(i, "PosZ", MainPipeline);
        CarRentalInfo[playerid][count][rcAngle] = cache_get_field_content_float(i, "Angle", MainPipeline);
        count++;
    }

    if(count) printf("Da tai %d chiec xe thue cho %s", count, GetPlayerNameExt(playerid));
    return 1;
}

stock SavePlayerRentalCar(playerid)
{
    for(new i; i < MAX_RENTALCAR_SLOT; i++)
    {
        if(CarRentalInfo[playerid][i][rcSqlid] == 0) continue;

        new query[256];
        mysql_format(MainPipeline, query, sizeof(query), "UPDATE `rentalcar` SET `Model` = '%d', `Time` = '%d', `PosX` = '%f', `PosY` = '%f', `PosZ` = '%f', `Angle` = '%f' WHERE `id` = '%d'", CarRentalInfo[playerid][i][rcModel], CarRentalInfo[playerid][i][rcTime], CarRentalInfo[playerid][i][rcPosX], CarRentalInfo[playerid][i][rcPosY], CarRentalInfo[playerid][i][rcPosZ], CarRentalInfo[playerid][i][rcAngle], CarRentalInfo[playerid][i][rcSqlid]);
        mysql_tquery(MainPipeline, query, "", "");

        if(CarRentalInfo[playerid][i][rcLabel] != INVALID_3DTEXT_ID) {
            Delete3DTextLabel(CarRentalInfo[playerid][i][rcLabel]);
            CarRentalInfo[playerid][i][rcLabel] = Text3D:INVALID_3DTEXT_ID;
        }

        if(IsValidVehicle(CarRentalInfo[playerid][i][rcId])) {
            DestroyVehicle(CarRentalInfo[playerid][i][rcId]);
            CarRentalInfo[playerid][i][rcId] = INVALID_VEHICLE_ID;
        }

        CarRentalInfo[playerid][i][rcSqlid] = 0;
        CarRentalInfo[playerid][i][rcModel] = 0;
        CarRentalInfo[playerid][i][rcTime] = 0;
        CarRentalInfo[playerid][i][rcPosX] = 0.0;
        CarRentalInfo[playerid][i][rcPosY] = 0.0;
        CarRentalInfo[playerid][i][rcPosZ] = 0.0;
        CarRentalInfo[playerid][i][rcAngle] = 0.0;
    }
    return 1;
}

forward ParkRentedVehicle(playerid, slotid, Float:X, Float:Y, Float:Z);
public ParkRentedVehicle(playerid, slotid, Float:X, Float:Y, Float:Z)
{
	if(IsPlayerInRangeOfPoint(playerid, 1.0, X, Y, Z))
	{
		if(!IsValidVehicle(CarRentalInfo[playerid][slotid][rcId]))
            return SendClientMessageEx(playerid, COLOR_GREY, "Xe thue cua ban khong ton tai.");

        new vehicleid = CarRentalInfo[playerid][slotid][rcId];

        new Float:x, Float:y, Float:z, Float:angle, Float:health, string[180], Float: oldfuel, arrDamage[4];
	    GetVehicleHealth(vehicleid, health);
     	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendClientMessageEx(playerid, COLOR_GREY, "Ban phai o ghe lai xe.");
     	if(health < 800) return SendClientMessageEx(playerid, COLOR_GREY, " Khong the dau xe khi xe cua ban hu hong qua nang.");

		GetVehiclePos(vehicleid, x, y, z);
		GetVehicleZAngle(vehicleid, angle);
		SurfingCheck(vehicleid);
		oldfuel = VehicleFuel[vehicleid];

		GetVehicleDamageStatus(vehicleid, arrDamage[0], arrDamage[1], arrDamage[2], arrDamage[3]);
		DestroyVehicle(vehicleid);
        CarRentalInfo[playerid][slotid][rcId] = CreateVehicle(CarRentalInfo[playerid][slotid][rcModel], x, y, z, angle, random(128), random(128), -1);
		Vehicle_ResetData(CarRentalInfo[playerid][slotid][rcId]);
		VehicleFuel[CarRentalInfo[playerid][slotid][rcId]] = oldfuel;
		SetVehicleHealth(CarRentalInfo[playerid][slotid][rcId], health);
		UpdateVehicleDamageStatus(CarRentalInfo[playerid][slotid][rcId], arrDamage[0], arrDamage[1], arrDamage[2], arrDamage[3]);
        CarRentalInfo[playerid][slotid][rcPosX] = x;
        CarRentalInfo[playerid][slotid][rcPosY] = y;
        CarRentalInfo[playerid][slotid][rcPosZ] = z;
        CarRentalInfo[playerid][slotid][rcAngle] = angle;
        UpdateRentalCarLabel(playerid, slotid);

		format(string, sizeof(string), "UPDATE `rentalcar` SET `PosX` = '%f', `PosY` = '%f', `PosZ` = '%f', `Angle` = '%f' WHERE `id` = '%d'", x, y, z, angle, CarRentalInfo[playerid][slotid][rcSqlid]);
        mysql_function_query(MainPipeline, string, false, "OnQueryFinish", "ii", SENDDATA_THREAD, playerid);

		IsPlayerEntering{playerid} = true;
		PutPlayerInVehicle(playerid, CarRentalInfo[playerid][slotid][rcId], 0);
		SetPlayerArmedWeapon(playerid, 0);
		format(string, sizeof(string), "* %s da dau mot chiec xe.", GetPlayerNameEx(playerid));
		ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);

	}
	else
	{
	    SendClientMessage(playerid, COLOR_WHITE, "Xe khong the dau khi ban di chuyen xe!");
	}
	return 1;
}

stock IsPlayerFreeRentalCar(playerid)
{
    for(new i; i < MAX_RENTALCAR_SLOT; i++)
    {
        if(CarRentalInfo[playerid][i][rcSqlid] == 0) return 1;
    }
    return 0;
}

stock CreatePlayerRentalCar(playerid, modelid, time, Float:x, Float:y, Float:z, Float:angle)
{
    for(new i; i < MAX_RENTALCAR_SLOT; i++)
    {
        if(CarRentalInfo[playerid][i][rcSqlid] == 0)
        {
            new query[256], Cache:cacheid;
            mysql_format(MainPipeline, query, sizeof(query), "INSERT INTO `rentalcar` (`OwnerID`, `Model`, `Time`, `PosX`, `PosY`, `PosZ`, `Angle`) VALUES ('%d', '%d', '%d', '%f', '%f', '%f', '%f')", GetPlayerSQLId(playerid), modelid, time, x, y, z, angle);
            cacheid = mysql_query(MainPipeline, query);

            CarRentalInfo[playerid][i][rcSqlid] = cache_insert_id();
            CarRentalInfo[playerid][i][rcModel] = modelid;
            CarRentalInfo[playerid][i][rcTime] = time;
            CarRentalInfo[playerid][i][rcPosX] = x;
            CarRentalInfo[playerid][i][rcPosY] = y;
            CarRentalInfo[playerid][i][rcPosZ] = z;
            CarRentalInfo[playerid][i][rcAngle] = angle;
            CarRentalInfo[playerid][i][rcId] = CreateVehicle(modelid, x, y, z, angle, random(128), random(128), -1);
            LockStatus{CarRentalInfo[playerid][i][rcId]} = 0;
            UpdateRentalCarLabel(playerid, i);

            cache_delete(cacheid);
            return 1;
        }
    }
    return 0;
}

stock RemovePlayerRentalCar(playerid, slotid)
{
    if(slotid < 0 || slotid >= MAX_RENTALCAR_SLOT) return 0;
    if(CarRentalInfo[playerid][slotid][rcSqlid] == 0) return 0;

    new query[256];
    mysql_format(MainPipeline, query, sizeof(query), "DELETE FROM `rentalcar` WHERE `id` = '%d'", CarRentalInfo[playerid][slotid][rcSqlid]);
    mysql_tquery(MainPipeline, query, "", "");

    if(CarRentalInfo[playerid][slotid][rcLabel] != INVALID_3DTEXT_ID) {
        Delete3DTextLabel(CarRentalInfo[playerid][slotid][rcLabel]);
        CarRentalInfo[playerid][slotid][rcLabel] = Text3D:INVALID_3DTEXT_ID;
    }

    if(IsValidVehicle(CarRentalInfo[playerid][slotid][rcId])) {
        DestroyVehicle(CarRentalInfo[playerid][slotid][rcId]);
        CarRentalInfo[playerid][slotid][rcId] = INVALID_VEHICLE_ID;
    }

    CarRentalInfo[playerid][slotid][rcSqlid] = 0;
    CarRentalInfo[playerid][slotid][rcModel] = 0;
    CarRentalInfo[playerid][slotid][rcTime] = 0;
    CarRentalInfo[playerid][slotid][rcPosX] = 0.0;
    CarRentalInfo[playerid][slotid][rcPosY] = 0.0;
    CarRentalInfo[playerid][slotid][rcPosZ] = 0.0;
    CarRentalInfo[playerid][slotid][rcAngle] = 0.0;
    return 1;
}

stock SpawnRentalCar(playerid, slotid)
{
    if(slotid < 0 || slotid >= MAX_RENTALCAR_SLOT) return 0;
    if(CarRentalInfo[playerid][slotid][rcSqlid] == 0) return 0;

    if(IsValidVehicle(CarRentalInfo[playerid][slotid][rcId])) {
        DestroyVehicle(CarRentalInfo[playerid][slotid][rcId]);
        CarRentalInfo[playerid][slotid][rcId] = INVALID_VEHICLE_ID;
    }

    CarRentalInfo[playerid][slotid][rcId] = CreateVehicle(CarRentalInfo[playerid][slotid][rcModel], CarRentalInfo[playerid][slotid][rcPosX], CarRentalInfo[playerid][slotid][rcPosY], CarRentalInfo[playerid][slotid][rcPosZ], CarRentalInfo[playerid][slotid][rcAngle], random(128), random(128), -1);
    LockStatus{CarRentalInfo[playerid][slotid][rcId]} = 0;
    UpdateRentalCarLabel(playerid, slotid);
    return 1;
}

stock DestroyRentalCar(playerid, slotid)
{
    if(slotid < 0 || slotid >= MAX_RENTALCAR_SLOT) return 0;
    if(CarRentalInfo[playerid][slotid][rcSqlid] == 0) return 0;

    if(CarRentalInfo[playerid][slotid][rcLabel] != INVALID_3DTEXT_ID) {
        Delete3DTextLabel(CarRentalInfo[playerid][slotid][rcLabel]);
        CarRentalInfo[playerid][slotid][rcLabel] = Text3D:INVALID_3DTEXT_ID;
    }

    if(IsValidVehicle(CarRentalInfo[playerid][slotid][rcId])) {
        new Float:x, Float:y, Float:z, Float:angle;
        GetVehiclePos(CarRentalInfo[playerid][slotid][rcId], x, y, z);
        GetVehicleZAngle(CarRentalInfo[playerid][slotid][rcId], angle);
        CarRentalInfo[playerid][slotid][rcPosX] = x;
        CarRentalInfo[playerid][slotid][rcPosY] = y;
        CarRentalInfo[playerid][slotid][rcPosZ] = z;
        CarRentalInfo[playerid][slotid][rcAngle] = angle;

        new query[256];
        mysql_format(MainPipeline, query, sizeof(query), "UPDATE `rentalcar` SET `PosX` = '%f', `PosY` = '%f', `PosZ` = '%f', `Angle` = '%f' WHERE `id` = '%d'", x, y, z, angle, CarRentalInfo[playerid][slotid][rcSqlid]);
        mysql_tquery(MainPipeline, query, "", "");

        DestroyVehicle(CarRentalInfo[playerid][slotid][rcId]);
        CarRentalInfo[playerid][slotid][rcId] = INVALID_VEHICLE_ID;
    }
    return 1;
}

stock UpdateRentalCarLabel(playerid, slotid)
{
    if(CarRentalInfo[playerid][slotid][rcLabel] != INVALID_3DTEXT_ID) {
        Delete3DTextLabel(CarRentalInfo[playerid][slotid][rcLabel]);
        CarRentalInfo[playerid][slotid][rcLabel] = Text3D:INVALID_3DTEXT_ID;
    }
    if(!IsValidVehicle(CarRentalInfo[playerid][slotid][rcId])) return;

    new labelStr[128];
    format(labelStr, sizeof(labelStr), "{DADA7E}Xe Thue - {FFFFFF}%s\n{FFFFFF}Con lai: {DADA7E}%s", GetPlayerNameExt(playerid), FormatTimeLeft(CarRentalInfo[playerid][slotid][rcTime]));
    CarRentalInfo[playerid][slotid][rcLabel] = Create3DTextLabel(labelStr, 0xDADA7EFF, 0.0, 0.0, 0.0, 15.0, 0);
    Attach3DTextLabelToVehicle(CarRentalInfo[playerid][slotid][rcLabel], CarRentalInfo[playerid][slotid][rcId], 0.0, 0.0, 0.2);
}

task RentalCarLabelUpdate[30000]()
{
    foreach(new i: Player) {
        for(new j; j < MAX_RENTALCAR_SLOT; j++) {
            if(IsValidVehicle(CarRentalInfo[i][j][rcId]))
                UpdateRentalCarLabel(i, j);
        }
    }
}

hook OnPlayerConnect(playerid)
{
    for(new i; i < MAX_RENTALCAR_SLOT; i++)
        CarRentalInfo[playerid][i][rcLabel] = Text3D:INVALID_3DTEXT_ID;
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    SavePlayerRentalCar(playerid);
    return 1;
}

stock ShowDialogRentCar(playerid)
{
    new dialogInfo[602];
    for(new i; i < sizeof(RentCarList); i++) {
        format(dialogInfo, _, "%s%d(0.0, 0.0, -50.0, 1.0)\t%s~n~~g~$%d\n", dialogInfo, RentCarList[i][0], Model_Name(RentCarList[i][0]), RentCarList[i][1]);
    }

    ShowPlayerDialog(playerid, DIALOG_RENTCAR, DIALOG_STYLE_PREVMODEL, "Hao Nam - Thue phuong tien", dialogInfo, "Thue", "Dong");
    return 1;
}

CMD:xethue(playerid, params[])
{
    new dialogInfo[428];
    strcat(dialogInfo, "Ten phuong tien\tThoi gian con lai\n");
    for(new i; i < MAX_RENTALCAR_SLOT; i++)
    {
        if(CarRentalInfo[playerid][i][rcSqlid] != 0) {
            format(dialogInfo, _, "%s{FFFFFF}%s\t{D8ED36}%s\n", dialogInfo, Model_Name(CarRentalInfo[playerid][i][rcModel]), FormatTimeLeft(CarRentalInfo[playerid][i][rcTime]));
        }
        else {
            strcat(dialogInfo, "{A5A5A5}Trong\n");
        }
    }

    ShowPlayerDialog(playerid, DIALOG_RENTALCAR, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Danh sach phuong tien thue", dialogInfo, "Chon", "Dong");
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_RENTALCAR && response)
    {
        if(listitem >= MAX_RENTALCAR_SLOT) return 0;
        if(!CarRentalInfo[playerid][listitem][rcSqlid])
            return PC_EmulateCommand(playerid, "/xethue");

        new title[128];
        format(title, _, "{FFFFFF}Ban muon lam gi voi {6694FE}%s{FFFFFF}?", Model_Name(CarRentalInfo[playerid][listitem][rcModel]));
        ShowPlayerDialog(playerid, DIALOG_RENTALCAR2, DIALOG_STYLE_LIST, title, "{FFFFFF}%s\n{C5CB6B}Tim phuong tien\n{E83A3A}Huy thue", "Chon", "Quay lai", (IsValidVehicle(CarRentalInfo[playerid][listitem][rcId]))? "Cat vao" : "Lay ra");
        SetPVarInt(playerid, "RentalCarSlot", listitem+1);
        return 1;
    }

    if(dialogid == DIALOG_RENTALCAR2)
    {
        if(!response) {
            return PC_EmulateCommand(playerid, "/xethue");
        }

        new slotid = GetPVarInt(playerid, "RentalCarSlot") - 1;
        if(slotid < 0 || slotid >= MAX_RENTALCAR_SLOT) return 0;

        switch(listitem)
        {
            case 0: {
                if(!IsValidVehicle(CarRentalInfo[playerid][slotid][rcId])) {
                    SendClientMessageEx(playerid, 0xD7DB56FF, "Ban da lay phuong tien thue ra.");
                    SpawnRentalCar(playerid, slotid);
                }
                else {
                    SendClientMessageEx(playerid, 0xD7DB56FF, "Ban cat phuong tien thue vao.");
                    DestroyRentalCar(playerid, slotid);
                }
            }
            case 1: {
                if(IsValidVehicle(CarRentalInfo[playerid][slotid][rcId])) {
                    new Float:x, Float:y, Float:z;
                    GetVehiclePos(CarRentalInfo[playerid][slotid][rcId], x, y, z);
                    SetPlayerCP(playerid, x, y, z, 5.0);
                    SendClientMessageEx(playerid, 0xD7DB56FF, "Phuong tien thue cua ban da danh dau tren mini-map.");
                    gPlayerCheckpointStatus[playerid] = CHECKPOINT_FINDCAR;
                }
                else {
                    SendClientMessageEx(playerid, COLOR_GREY, "Phuong tien thue cua ban chua duoc tao ra.");
                }
            }
            case 2: {
                if(IsValidVehicle(CarRentalInfo[playerid][slotid][rcId])) {
                    if(GetVehicleDriver(CarRentalInfo[playerid][slotid][rcId]) != INVALID_PLAYER_ID) {
                        SendClientMessageEx(playerid, COLOR_GREY, "Ban khong the huy thue neu phuong tien dang duoc dieu khien.");
                        return 1;
                    }
                }

                RemovePlayerRentalCar(playerid, slotid);
                SendClientMessageEx(playerid, 0xD7DB56FF, "Ban da huy thue phuong tien thanh cong.");
                return 1;
            }
        }
    }

    if(dialogid == DIALOG_RENTCAR && response)
    {
        if(!IsPlayerDiscordVerified(playerid))
            return SendClientMessageEx(playerid, COLOR_GREY, "Ban phai lien ket tai khoan Discord de co the lam dieu nay.");

        if(listitem < 0 || listitem >= sizeof(RentCarList)) return 0;
        if(GetPlayerCash(playerid) < RentCarList[listitem][1])
            return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong du tien de thue phuong tien.");

        if(!IsPlayerFreeRentalCar(playerid))
            return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong the thue them phuong tien nao nua.");

        if(CreatePlayerRentalCar(playerid, RentCarList[listitem][0], RentCarList[listitem][2], RentCarSpawn[0], RentCarSpawn[1], RentCarSpawn[2], RentCarSpawn[3]))
        {
            SendClientMessageEx(playerid, 0xD7DB56FF, "Ban da thue phuong tien thanh cong.");
            SendClientMessageEx(playerid, 0xD7DB56FF, "Su dung \"/xethue\" de tuong tac voi phuong tien.");
            GivePlayerCash(playerid, -RentCarList[listitem][1]);
        }
        else {
            SendClientMessageEx(playerid, COLOR_GREY, "Co loi xay ra khi thue phuong tien.");
        }
    }
    return 1;
}
