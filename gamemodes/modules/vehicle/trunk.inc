// vehicle/trunk.inc — He thong cop xe moi (/copxe, /lucsoatcopxe)

#define MAX_TRUNK_SLOTS     10
#define TRUNK_CAT_EMPTY     0
#define TRUNK_CAT_GUN       1
#define TRUNK_CAT_ITEM      2

// Luu danh sach item khi mo dialog cat vao (per player)
new TrunkPutType[MAX_PLAYERS][30];   // weapon ID hoac ItemInventory index
new TrunkPutCat[MAX_PLAYERS][30];    // TRUNK_CAT_GUN hoac TRUNK_CAT_ITEM
new TrunkPutCount[MAX_PLAYERS];

// Mapping cho lucsoat dialog: dialog row -> trunk slot
new LucSoatMap[MAX_PLAYERS][MAX_TRUNK_SLOTS];
new LucSoatCount[MAX_PLAYERS];

// ===================== HELPER FUNCTIONS =====================

stock FindClosestOwnedVehicle(playerid, Float:range = 4.0)
{
    new Float:x, Float:y, Float:z;
    for(new d = 0; d < MAX_PLAYERVEHICLES; d++)
    {
        if(PlayerVehicleInfo[playerid][d][pvId] != INVALID_PLAYER_VEHICLE_ID)
        {
            GetVehiclePos(PlayerVehicleInfo[playerid][d][pvId], x, y, z);
            if(IsPlayerInRangeOfPoint(playerid, range, x, y, z))
                return d;
        }
    }
    return -1;
}

stock bool:IsTrunkOpenable(vehicleid)
{
    new model = GetVehicleModel(vehicleid);
    return (model != 481 && model != 509 && model != 510);
}

stock GetTrunkItemName(playerid, pvid, slot, output[], len)
{
    new cat = PlayerVehicleInfo[playerid][pvid][pvTrunkCat][slot];
    new type = PlayerVehicleInfo[playerid][pvid][pvTrunkType][slot];
    if(cat == TRUNK_CAT_GUN)
        GetWeaponName(type, output, len);
    else if(cat == TRUNK_CAT_ITEM && type >= 0 && type < sizeof(ItemInventory))
        format(output, len, "%s", ItemInventory[type][NameItem]);
    else
        format(output, len, "Trong");
}

// ===================== SHOW DIALOGS =====================

stock ShowTrunkDialog(playerid, pvid)
{
    new szList[640], szRow[64], szTitle[64];
    format(szTitle, sizeof(szTitle), "Cop xe - %s", GetVehicleName(PlayerVehicleInfo[playerid][pvid][pvId]));
    for(new s = 0; s < MAX_TRUNK_SLOTS; s++)
    {
        new cat = PlayerVehicleInfo[playerid][pvid][pvTrunkCat][s];
        new type = PlayerVehicleInfo[playerid][pvid][pvTrunkType][s];
        new amt  = PlayerVehicleInfo[playerid][pvid][pvTrunkAmt][s];
        if(cat == TRUNK_CAT_EMPTY)
            format(szRow, sizeof(szRow), "Slot %d: {AAAAAA}[Trong]\n", s + 1);
        else if(cat == TRUNK_CAT_GUN)
        {
            new szWep[32];
            GetWeaponName(type, szWep, sizeof(szWep));
            format(szRow, sizeof(szRow), "Slot %d: {FF8000}[Sung] %s\n", s + 1, szWep);
        }
        else
        {
            if(type >= 0 && type < sizeof(ItemInventory))
                format(szRow, sizeof(szRow), "Slot %d: {00FF88}[Item] %s {FFFFFF}x%d\n", s + 1, ItemInventory[type][NameItem], amt);
            else
                format(szRow, sizeof(szRow), "Slot %d: {AAAAAA}[Trong]\n", s + 1);
        }
        strcat(szList, szRow, sizeof(szList));
    }
    SetPVarInt(playerid, "TrunkPvid", pvid);
    ShowPlayerDialog(playerid, DIALOG_TRUNK_MAIN, DIALOG_STYLE_LIST, szTitle, szList, "Thao tac", "Dong");
}

stock ShowTrunkPutDialog(playerid)
{
    new szList[1024], szRow[80];
    TrunkPutCount[playerid] = 0;

    // Sung (tat ca sung player dang co tren nguoi)
    for(new s = 0; s < 12; s++)
    {
        if(PlayerInfo[playerid][pGuns][s] != 0)
        {
            new szWep[32];
            GetWeaponName(PlayerInfo[playerid][pGuns][s], szWep, sizeof(szWep));
            format(szRow, sizeof(szRow), "{FF8000}[Sung] {FFFFFF}%s\n", szWep);
            strcat(szList, szRow, sizeof(szList));
            TrunkPutType[playerid][TrunkPutCount[playerid]] = PlayerInfo[playerid][pGuns][s];
            TrunkPutCat[playerid][TrunkPutCount[playerid]] = TRUNK_CAT_GUN;
            TrunkPutCount[playerid]++;
            if(TrunkPutCount[playerid] >= 30) break;
        }
    }

    // Vat pham tu ItemInventory (tru CCCD)
    for(new i = 0; i < sizeof(ItemInventory); i++)
    {
        if(ItemInventory[i][pVariable] == pIDCard) continue;
        if(PlayerInfo[playerid][ItemInventory[i][pVariable]] > 0)
        {
            format(szRow, sizeof(szRow), "{00FF88}[Item] {FFFFFF}%s {AAAAAA}(x%d)\n", ItemInventory[i][NameItem], PlayerInfo[playerid][ItemInventory[i][pVariable]]);
            strcat(szList, szRow, sizeof(szList));
            TrunkPutType[playerid][TrunkPutCount[playerid]] = i;
            TrunkPutCat[playerid][TrunkPutCount[playerid]] = TRUNK_CAT_ITEM;
            TrunkPutCount[playerid]++;
            if(TrunkPutCount[playerid] >= 30) break;
        }
    }

    if(TrunkPutCount[playerid] == 0)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong co vat pham nao de cat vao cop xe.");

    ShowPlayerDialog(playerid, DIALOG_TRUNK_PUT, DIALOG_STYLE_LIST, "Chon vat pham de cat vao", szList, "Chon", "Quay lai");
    return 1;
}

// ===================== COMMANDS =====================

CMD:copxe(playerid, params[])
{
    if(IsPlayerInAnyVehicle(playerid))
        return SendClientMessageEx(playerid, COLOR_WHITE, "Ban khong the lam dieu nay khi dang trong xe!");
    if(GetPVarInt(playerid, "IsInArena") >= 0)
        return SendClientMessageEx(playerid, COLOR_WHITE, "Ban khong the lam dieu nay trong dau truong!");
    if(GetPVarInt(playerid, "EventToken") != 0)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong the lam dieu nay khi dang trong su kien.");

    new pvid = FindClosestOwnedVehicle(playerid, 4.0);
    if(pvid == -1)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong o gan xe nao cua ban.");

    if(!IsTrunkOpenable(PlayerVehicleInfo[playerid][pvid][pvId]))
        return SendClientMessageEx(playerid, COLOR_GREY, "Chiec xe nay khong co cop.");

    new engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(PlayerVehicleInfo[playerid][pvid][pvId], engine, lights, alarm, doors, bonnet, boot, objective);
    if(boot == VEHICLE_PARAMS_OFF || boot == VEHICLE_PARAMS_UNSET)
        return SendClientMessageEx(playerid, COLOR_GRAD3, "Cop xe chua mo! Su dung /car trunk de mo cop.");

    ShowTrunkDialog(playerid, pvid);
    return 1;
}

CMD:lucsoatcopxe(playerid, params[])
{
    if(!IsACop(playerid))
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong phai la nhan vien thuc thi phap luat.");
    if(GetPVarInt(playerid, "Injured") != 0)
        return SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong the lam dieu nay vao luc nay.");

    new carid   = GetPlayerVehicleID(playerid);
    new closest = GetClosestCar(playerid, carid);
    if(!IsPlayerInRangeOfVehicle(playerid, closest, 5.0))
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong o gan bat ki xe nao.");

    if(!IsTrunkOpenable(closest))
        return SendClientMessageEx(playerid, COLOR_GREY, "Chiec xe nay khong co cop.");

    new engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(closest, engine, lights, alarm, doors, bonnet, boot, objective);
    if(boot == VEHICLE_PARAMS_OFF || boot == VEHICLE_PARAMS_UNSET)
        return SendClientMessageEx(playerid, COLOR_GRAD1, "Cop xe phai mo moi co the luc soat.");

    // Tim chu xe
    new ownerid = -1, ownerpvid = -1;
    foreach(new i: Player)
    {
        new v = GetPlayerVehicle(i, closest);
        if(v != -1)
        {
            ownerid   = i;
            ownerpvid = v;
            break;
        }
    }

    if(ownerid == -1)
        return SendClientMessageEx(playerid, COLOR_GREY, "Khong tim thay chu xe truc tuyen.");

    // Kiem tra co item trong cop khong
    new bHasItem = false;
    for(new s = 0; s < MAX_TRUNK_SLOTS; s++)
    {
        if(PlayerVehicleInfo[ownerid][ownerpvid][pvTrunkCat][s] != TRUNK_CAT_EMPTY)
        {
            bHasItem = true;
            break;
        }
    }
    if(!bHasItem)
        return SendClientMessageEx(playerid, COLOR_WHITE, "Cop xe nay khong co gi.");

    // Build dialog list (chi slot co item)
    new szList[640], szRow[80];
    LucSoatCount[playerid] = 0;
    for(new s = 0; s < MAX_TRUNK_SLOTS; s++)
    {
        new cat = PlayerVehicleInfo[ownerid][ownerpvid][pvTrunkCat][s];
        new type = PlayerVehicleInfo[ownerid][ownerpvid][pvTrunkType][s];
        new amt  = PlayerVehicleInfo[ownerid][ownerpvid][pvTrunkAmt][s];
        if(cat == TRUNK_CAT_EMPTY) continue;
        if(cat == TRUNK_CAT_GUN)
        {
            new szWep[32];
            GetWeaponName(type, szWep, sizeof(szWep));
            format(szRow, sizeof(szRow), "{FF8000}[Sung] {FFFFFF}%s\n", szWep);
        }
        else
        {
            if(type >= 0 && type < sizeof(ItemInventory))
                format(szRow, sizeof(szRow), "{00FF88}[Item] {FFFFFF}%s {AAAAAA}x%d\n", ItemInventory[type][NameItem], amt);
            else continue;
        }
        strcat(szList, szRow, sizeof(szList));
        LucSoatMap[playerid][LucSoatCount[playerid]] = s;
        LucSoatCount[playerid]++;
    }

    SetPVarInt(playerid, "LucSoatOwner", ownerid);
    SetPVarInt(playerid, "LucSoatPvid",  ownerpvid);

    new szTitle[64];
    format(szTitle, sizeof(szTitle), "Luc soat cop xe - %s", GetPlayerNameEx(ownerid));
    ShowPlayerDialog(playerid, DIALOG_LUCSOAT_MAIN, DIALOG_STYLE_LIST, szTitle, szList, "Tich thu", "Dong");
    return 1;
}

// ===================== MYSQL SAVE/LOAD =====================

stock g_mysql_SaveTrunk(playerid, slotid)
{
    new vehicle_id = PlayerVehicleInfo[playerid][slotid][pvSlotId];
    if(vehicle_id <= 0) return;

    new query[128];
    format(query, sizeof(query), "DELETE FROM `vehicle_trunk` WHERE `vehicle_id` = %d", vehicle_id);
    mysql_function_query(MainPipeline, query, false, "OnQueryFinish", "i", SENDDATA_THREAD);

    for(new s = 0; s < MAX_TRUNK_SLOTS; s++)
    {
        if(PlayerVehicleInfo[playerid][slotid][pvTrunkCat][s] != TRUNK_CAT_EMPTY)
        {
            format(query, sizeof(query),
                "INSERT INTO `vehicle_trunk` (`vehicle_id`,`slot`,`item_type`,`item_cat`,`item_amt`) VALUES (%d,%d,%d,%d,%d)",
                vehicle_id, s,
                PlayerVehicleInfo[playerid][slotid][pvTrunkType][s],
                PlayerVehicleInfo[playerid][slotid][pvTrunkCat][s],
                PlayerVehicleInfo[playerid][slotid][pvTrunkAmt][s]);
            mysql_function_query(MainPipeline, query, false, "OnQueryFinish", "i", SENDDATA_THREAD);
        }
    }
}

stock g_mysql_LoadTrunk(playerid)
{
    new szIds[256];
    new bool:bFirst = true;
    for(new d = 0; d < MAX_PLAYERVEHICLES; d++)
    {
        if(PlayerVehicleInfo[playerid][d][pvSlotId] > 0)
        {
            if(bFirst) { format(szIds, sizeof(szIds), "%d", PlayerVehicleInfo[playerid][d][pvSlotId]); bFirst = false; }
            else        format(szIds, sizeof(szIds), "%s,%d", szIds, PlayerVehicleInfo[playerid][d][pvSlotId]);
        }
    }
    if(bFirst) return;

    new query[512];
    format(query, sizeof(query),
        "SELECT `vehicle_id`,`slot`,`item_type`,`item_cat`,`item_amt` FROM `vehicle_trunk` WHERE `vehicle_id` IN (%s)",
        szIds);
    mysql_function_query(MainPipeline, query, true, "OnQueryFinish", "iii", LOADTRUNK_THREAD, playerid, g_arrQueryHandle{playerid});
}

stock g_mysql_DeleteTrunk(vehicle_id)
{
    if(vehicle_id <= 0) return;
    new query[128];
    format(query, sizeof(query), "DELETE FROM `vehicle_trunk` WHERE `vehicle_id` = %d", vehicle_id);
    mysql_function_query(MainPipeline, query, false, "OnQueryFinish", "i", SENDDATA_THREAD);
}
