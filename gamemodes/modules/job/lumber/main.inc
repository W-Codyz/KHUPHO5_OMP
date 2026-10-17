// ============================================================
//  Lumber Job - KP5 Roleplay Created by Codyz
// ============================================================

#include <YSI\YSI_Coding\y_hooks>

// Vi tri log tren xe (offset: x, y, z, rx, ry, rz)
static const Float:gLumberLogPosOnVeh[LUMBER_MAX_VEH_LOGS][6] = {
    {-0.07150, -1.22530, 0.24970, -0.20800, 0.00000, 0.00000},
    {-0.07150, -1.74530, 0.24970, -0.20800, 0.00000, 0.00000},
    {-0.07150, -1.22530, 0.76970, -0.20800, 0.00000, 0.00000},
    {-0.07150, -1.84930, 0.76970, -0.20800, 0.00000, 0.00000},
    {-0.07150, -1.53730, 1.18570, -0.20800, 0.00000, 0.00000}
};

// --- Forwards ---
forward Lumber_ChopFinish(playerid);
forward Lumber_RespawnTree(treeIndex);

// ============================================================
//  HELPER
// ============================================================

static stock Lumber_FindNearestTree(playerid)
{
    for(new i = 0; i < LUMBER_MAX_TREES; i++)
    {
        if(!gLumberTreeActive[i] || gLumberTreeOccupied[i]) continue;
        if(IsPlayerInRangeOfPoint(playerid, 4.0, gLumberTreeX[i], gLumberTreeY[i], gLumberTreeZ[i]))
            return i;
    }
    return -1;
}

static stock Lumber_SpawnTreeAt(index, Float:x, Float:y, Float:z)
{
    gLumberTreeX[index] = x;
    gLumberTreeY[index] = y;
    gLumberTreeZ[index] = z;
    gLumberTreeObj[index] = CreateDynamicObject(LUMBER_TREE_MODEL, x, y, z - 0.5, 0.0, 0.0, float(random(360)));
    gLumberTreeActive[index] = true;
    gLumberTreeOccupied[index] = false;
    gLumberTreePlayer[index] = -1;
}

// ============================================================
//  INIT
// ============================================================

Lumber_Init()
{
    new count = 0, attempts = 0;
    new Float:x, Float:y, Float:z;

    for(new i = 0; i < LUMBER_MAX_TREES; i++)
    {
        gLumberTreeActive[i]   = false;
        gLumberTreeOccupied[i] = false;
        gLumberTreePlayer[i]   = -1;
        gLumberTreeObj[i]      = -1;
    }
    for(new v = 0; v < MAX_VEHICLES; v++)
    {
        gLumberVehLogCount[v] = 0;
        for(new s = 0; s < LUMBER_MAX_VEH_LOGS; s++)
            gLumberVehLogObj[v][s] = -1;
    }

    // Spawn cay: chia deu vao 2 zone
    new zone1Range = 15, zone2Range = LUMBER_MAX_TREES - 15;
    new rangeX1 = floatround((LUMBER_ZONE1_MAXX - LUMBER_ZONE1_MINX) * 100.0);
    new rangeY1 = floatround((LUMBER_ZONE1_MAXY - LUMBER_ZONE1_MINY) * 100.0);
    new rangeX2 = floatround((LUMBER_ZONE2_MAXX - LUMBER_ZONE2_MINX) * 100.0);
    new rangeY2 = floatround((LUMBER_ZONE2_MAXY - LUMBER_ZONE2_MINY) * 100.0);

    while(count < zone1Range && attempts < 10000)
    {
        attempts++;
        x = LUMBER_ZONE1_MINX + float(random(rangeX1)) / 100.0;
        y = LUMBER_ZONE1_MINY + float(random(rangeY1)) / 100.0;
        if(!CA_FindZ_For2DCoord(x, y, z)) continue;
        Lumber_SpawnTreeAt(count, x, y, z);
        count++;
    }
    attempts = 0;
    while(count < LUMBER_MAX_TREES && attempts < 10000)
    {
        attempts++;
        x = LUMBER_ZONE2_MINX + float(random(rangeX2)) / 100.0;
        y = LUMBER_ZONE2_MINY + float(random(rangeY2)) / 100.0;
        if(!CA_FindZ_For2DCoord(x, y, z)) continue;
        Lumber_SpawnTreeAt(count, x, y, z);
        count++;
    }

    printf("[Lumber] %d/%d trees spawned.", count, LUMBER_MAX_TREES);

    // NPC ban / doi go (NPC cong viec duoc tao boi Job_Init)
    CreateDynamicActor(22, LUMBER_SELL_X, LUMBER_SELL_Y, LUMBER_SELL_Z, LUMBER_SELL_A, true);
    CreateDynamic3DTextLabel(
        "{F5F52F}Thu Mua Go\n{FFFFFF}Nhan {5FBC52}[Y]{FFFFFF} de ban/doi go tho",
        -1, LUMBER_SELL_X, LUMBER_SELL_Y, LUMBER_SELL_Z + 0.3, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1);

    return 1;
}

// ============================================================
//  START / STOP
// ============================================================

Lumber_Start(playerid)
{
    if(PlayerLumberData[playerid][LP_Working])
    {
        ShowTDN(playerid, NOTIFICATION_WARNING, "Ban dang lam viec roi.");
        return 1;
    }

    PlayerLumberData[playerid][LP_Working]    = true;
    PlayerLumberData[playerid][LP_VehicleID]  = INVALID_VEHICLE_ID;
    PlayerLumberData[playerid][LP_HasLog]     = false;
    PlayerLumberData[playerid][LP_AttackTree] = -1;
    PlayerLumberData[playerid][LP_ChopTimer]  = -1;
    PlayerWorking[playerid]                   = JOB_LUMBER;
    SetPlayerSkin(playerid, JobInfo[JOB_LUMBER][jobSkin]);

    new vehicleid = CreateVehicle(478, LUMBER_VEH_X, LUMBER_VEH_Y, LUMBER_VEH_Z, LUMBER_VEH_A, -1, -1, -1);
    if(vehicleid == INVALID_VEHICLE_ID)
    {
        ShowTDN(playerid, NOTIFICATION_ERROR, "Loi: Khong the tao xe. Thu lai sau.");
        PlayerLumberData[playerid][LP_Working] = false;
        PlayerWorking[playerid] = 0;
        return 1;
    }
    LockStatus{vehicleid} = 0;
    VehicleFuel[vehicleid] = 100.0;
    SetVehicleHealth(vehicleid, 999.0);
    PutPlayerInVehicle(playerid, vehicleid, 0);

    PlayerLumberData[playerid][LP_VehicleID] = vehicleid;
    gLumberVehLogCount[vehicleid] = 0;
    for(new s = 0; s < LUMBER_MAX_VEH_LOGS; s++) gLumberVehLogObj[vehicleid][s] = -1;

    new labelStr[128];
    format(labelStr, sizeof(labelStr), "{5DBC52}Lumberjack\n{FFFFFF}Chu xe: {5DBC52}%s", GetPlayerNameEx(playerid));
    gLumberVehLabel[playerid] = Create3DTextLabel(labelStr, 0x5DBC52FF, 0.0, 0.0, 0.0, 12.0, 0);
    Attach3DTextLabelToVehicle(gLumberVehLabel[playerid], vehicleid, 0.0, 0.0, 0.0);

    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{5DBC52}[Lumberjack] {ffffff}Bat dau lam viec. Lai xe Flatbed den khu rung de chat cay.");
    return 1;
}

Lumber_Stop(playerid)
{
    if(!PlayerLumberData[playerid][LP_Working]) return SendClientMessageEx(playerid, COLOR_GREY, "Ban chua bat dau lam viec.");

    if(PlayerLumberData[playerid][LP_ChopTimer] != -1)
    {
        KillTimer(PlayerLumberData[playerid][LP_ChopTimer]);
        PlayerLumberData[playerid][LP_ChopTimer] = -1;
    }

    new attackTree = PlayerLumberData[playerid][LP_AttackTree];
    if(attackTree >= 0 && attackTree < LUMBER_MAX_TREES)
    {
        gLumberTreeOccupied[attackTree] = false;
        gLumberTreePlayer[attackTree]   = -1;
    }

    if(PlayerLumberData[playerid][LP_HasLog])
    {
        RemovePlayerAttachedObject(playerid, LUMBER_LOG_CARRY_SLOT);
        PlayerLumberData[playerid][LP_HasLog] = false;
    }

    new vehicleid = PlayerLumberData[playerid][LP_VehicleID];
    if(vehicleid != INVALID_VEHICLE_ID && IsValidVehicle(vehicleid))
    {
        for(new s = 0; s < gLumberVehLogCount[vehicleid]; s++)
        {
            if(gLumberVehLogObj[vehicleid][s] != -1)
            {
                DestroyDynamicObject(gLumberVehLogObj[vehicleid][s]);
                gLumberVehLogObj[vehicleid][s] = -1;
            }
        }
        gLumberVehLogCount[vehicleid] = 0;
        DestroyVehicle(vehicleid);
    }

    if(gLumberVehLabel[playerid] != Text3D:0)
    {
        Delete3DTextLabel(gLumberVehLabel[playerid]);
        gLumberVehLabel[playerid] = Text3D:0;
    }

    RemovePlayerAttachedObject(playerid, LUMBER_TOOL_SLOT);
    StopLoading(playerid);
    ClearAnimations(playerid);
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);

    PlayerLumberData[playerid][LP_Working]    = false;
    PlayerLumberData[playerid][LP_VehicleID]  = INVALID_VEHICLE_ID;
    PlayerLumberData[playerid][LP_AttackTree] = -1;
    PlayerWorking[playerid]                   = 0;
    SetPlayerSkin(playerid, PlayerInfo[playerid][pModel]);

    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{5DBC52}[Lumberjack] {ffffff}Ban da ngung lam viec.");
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Da ngung lam viec.");
    return 1;
}

// ============================================================
//  CHOPPING
// ============================================================

static stock Lumber_StartChop(playerid, treeIndex)
{
    gLumberTreeOccupied[treeIndex] = true;
    gLumberTreePlayer[treeIndex]   = playerid;
    PlayerLumberData[playerid][LP_AttackTree] = treeIndex;

    ApplyAnimation(playerid, "CHAINSAW", "WEAPON_csaw", 4.1, 1, 0, 0, 1, 0, 1);
    SetPlayerAttachedObject(playerid, LUMBER_TOOL_SLOT, 341, 6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
    SetLoading(playerid, LUMBER_CHOP_TIME, "Dang chat cay...", "Lumber_ChopFinish");

    new string[128];
    format(string, sizeof(string), "{FF8000}* {C2A2DA}%s dang chat cay.", GetPlayerNameEx(playerid));
    ProxDetectorWrap(playerid, string, 92, 30.0, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
    return 1;
}

public Lumber_ChopFinish(playerid)
{
    new treeIndex = PlayerLumberData[playerid][LP_AttackTree];
    PlayerLumberData[playerid][LP_AttackTree] = -1;

    if(treeIndex < 0 || treeIndex >= LUMBER_MAX_TREES) return 1;

    gLumberTreeOccupied[treeIndex] = false;
    gLumberTreePlayer[treeIndex]   = -1;

    RemovePlayerAttachedObject(playerid, LUMBER_TOOL_SLOT);

    if(!IsPlayerInRangeOfPoint(playerid, 6.0, gLumberTreeX[treeIndex], gLumberTreeY[treeIndex], gLumberTreeZ[treeIndex]))
    {
        ClearAnimations(playerid);
        ShowTDN(playerid, NOTIFICATION_ERROR, "Chat cay that bai vi ban di qua xa.");
        SendClientMessageEx(playerid, COLOR_RED, "{5DBC52}[Lumberjack] {ffffff}Ban da di chuyen qua xa, chat cay that bai.");
        return 1;
    }

    // Xoa cay, dat timer respawn
    if(gLumberTreeObj[treeIndex] != -1)
    {
        DestroyDynamicObject(gLumberTreeObj[treeIndex]);
        gLumberTreeObj[treeIndex] = -1;
    }
    gLumberTreeActive[treeIndex] = false;
    SetTimerEx("Lumber_RespawnTree", LUMBER_RESPAWN_TIME, false, "d", treeIndex);

    // Player cam khuc go
    PlayerLumberData[playerid][LP_HasLog] = true;
    SetPlayerAttachedObject(playerid, LUMBER_LOG_CARRY_SLOT, LUMBER_LOG_MODEL, 1,  0.1539, 0.5870, 0.0000, 0.0000, 84.7999, 0.0000, 0.8460, 0.4109, 0.4190, 0xFFFFFFFF, 0xFFFFFFFF);
    ApplyAnimation(playerid, "CARRY", "LIFTUP", 4.0, 0, 0, 0, 0, 0);
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);

    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{5DBC52}[Lumberjack] {ffffff}Chat cay thanh cong! Den sat xe Flatbed nhan {FFFF00}[Y]{FFFFFF} de dat go len xe.");
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Chat cay thanh cong.");

    new rand_xp = 0;
    if(DoubleXP) {
        rand_xp = Random(PlayerInfo[playerid][pLevel] * XP_RATE - 7, PlayerInfo[playerid][pLevel] * XP_RATE - 4);
        SendClientMessageEx(playerid, COLOR_YELLOW, "Kinh nghiem nhan vat: +%d XP (%s/%s) (DOUBLE XP)", rand_xp*2, number_format(PlayerInfo[playerid][pXP]+rand_xp*2), number_format(GetRequiredXP(playerid)));
        GivePlayerXP(playerid, rand_xp*2);
    }
    else if(PlayerInfo[playerid][pDoubleEXP] > 0 && !DoubleXP) {
        rand_xp = Random(PlayerInfo[playerid][pLevel] * XP_RATE - 7, PlayerInfo[playerid][pLevel] * XP_RATE - 4);
        SendClientMessageEx(playerid, COLOR_YELLOW, "Kinh nghiem nhan vat: +%d XP (%s/%s) (DOUBLE XP)", rand_xp*2, number_format(PlayerInfo[playerid][pXP]+rand_xp*2), number_format(GetRequiredXP(playerid)));
        GivePlayerXP(playerid, rand_xp*2);
    }
    else {
        rand_xp = Random(PlayerInfo[playerid][pLevel] * XP_RATE - 7, PlayerInfo[playerid][pLevel] * XP_RATE - 4);
        SendClientMessageEx(playerid, COLOR_YELLOW, "Kinh nghiem nhan vat: +%d XP (%s/%s)", rand_xp, number_format(PlayerInfo[playerid][pXP]+rand_xp), number_format(GetRequiredXP(playerid)));
        GivePlayerXP(playerid, rand_xp);
    }

    new string[128];
    format(string, sizeof(string), "{FF8000}* {C2A2DA}%s da chat duoc mot khuc go.", GetPlayerNameEx(playerid));
    ProxDetectorWrap(playerid, string, 92, 30.0, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
    return 1;
}

public Lumber_RespawnTree(treeIndex)
{
    if(treeIndex < 0 || treeIndex >= LUMBER_MAX_TREES) return 1;
    gLumberTreeObj[treeIndex] = CreateDynamicObject(LUMBER_TREE_MODEL,
        gLumberTreeX[treeIndex], gLumberTreeY[treeIndex], gLumberTreeZ[treeIndex] - 0.5,
        0.0, 0.0, float(random(360)));
    gLumberTreeActive[treeIndex] = true;
    return 1;
}

// ============================================================
//  DAT GO LEN XE
// ============================================================

static stock Lumber_DepositToVehicle(playerid)
{
    new vehicleid = PlayerLumberData[playerid][LP_VehicleID];
    if(vehicleid == INVALID_VEHICLE_ID || !IsValidVehicle(vehicleid))
    {
        ShowTDN(playerid, NOTIFICATION_ERROR, "Khong tim thay xe cua ban.");
        return 0;
    }
    if(gLumberVehLogCount[vehicleid] >= LUMBER_MAX_VEH_LOGS)
    {
        SendClientMessageEx(playerid, COLOR_YELLOW, "{5DBC52}[Lumberjack] {ffffff}Xe cua ban da day go. Hay lai den NPC de ban hoac doi go tho.");
        ShowTDN(playerid, NOTIFICATION_WARNING, "Xe day go.");
        return 0;
    }

    new slot = gLumberVehLogCount[vehicleid];
    gLumberVehLogObj[vehicleid][slot] = CreateDynamicObject(LUMBER_VEH_LOG_MODEL, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    AttachDynamicObjectToVehicle(gLumberVehLogObj[vehicleid][slot], vehicleid,
        gLumberLogPosOnVeh[slot][0], gLumberLogPosOnVeh[slot][1], gLumberLogPosOnVeh[slot][2],
        gLumberLogPosOnVeh[slot][3], gLumberLogPosOnVeh[slot][4], gLumberLogPosOnVeh[slot][5]);
    gLumberVehLogCount[vehicleid]++;

    RemovePlayerAttachedObject(playerid, LUMBER_LOG_CARRY_SLOT);
    PlayerLumberData[playerid][LP_HasLog] = false;
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);

    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{5DBC52}[Lumberjack] {ffffff}Da dat go len xe (%d/%d).",
        gLumberVehLogCount[vehicleid], LUMBER_MAX_VEH_LOGS);
    if(gLumberVehLogCount[vehicleid] >= LUMBER_MAX_VEH_LOGS)
        ShowTDN(playerid, NOTIFICATION_SUCCESS, "Xe day go. Lai den NPC ban/doi go.");
    return 1;
}

// ============================================================
//  BAN / DOI GO
// ============================================================

static stock Lumber_OpenSellDialog(playerid)
{
    if(!PlayerLumberData[playerid][LP_HasLog])
    {
        SendClientMessageEx(playerid, COLOR_YELLOW, "{5DBC52}[Lumberjack] {ffffff}Can cam theo 1 khuc go. Dung {FFFF00}/laygo{ffffff} lay go tu xe truoc.");
        ShowTDN(playerid, NOTIFICATION_WARNING, "Cam theo go de ban/doi.");
        return 0;
    }

    ShowPlayerDialog(playerid, DIALOG_LUMBER_SELL, DIALOG_STYLE_LIST,
        "{F5D000}Thu Mua Go",
        "{FFFFFF}Ban go ($1500)\n{C2A2DA}Doi go tho (1~2 go tho)",
        "Chon", "Huy");
    return 1;
}

static stock Lumber_SellLog(playerid)
{
    RemovePlayerAttachedObject(playerid, LUMBER_LOG_CARRY_SLOT);
    PlayerLumberData[playerid][LP_HasLog] = false;
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);

    GivePlayerCash(playerid, LUMBER_LOG_PRICE);
    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{5DBC52}[Lumberjack] {ffffff}Ban 1 khuc go, nhan ${00FF7F}%d.", LUMBER_LOG_PRICE);
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Ban go thanh cong!");

    new pname[MAX_PLAYER_NAME];
    GetPlayerName(playerid, pname, sizeof(pname));
    DiscordLog(dclog_Lumber, "BAN GO", "**%s (ID: %d)** bán 1 khúc gỗ. **Tiền: $%s**",
        pname, playerid, number_format(LUMBER_LOG_PRICE));
    return 1;
}

static stock Lumber_ConvertToRaw(playerid)
{
    RemovePlayerAttachedObject(playerid, LUMBER_LOG_CARRY_SLOT);
    PlayerLumberData[playerid][LP_HasLog] = false;
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);

    new amount = 1 + random(2); // 1 hoac 2
    PlayerInfo[playerid][pGoThuong] += amount;

    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{5DBC52}[Lumberjack] {ffffff}Doi 1 khuc go duoc {FFFF00}%d go tho{ffffff}.", amount);
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Doi go tho thanh cong!");

    new pname[MAX_PLAYER_NAME];
    GetPlayerName(playerid, pname, sizeof(pname));
    DiscordLog(dclog_Lumber, "DOI GO THO", "**%s (ID: %d)** đổi 1 khúc → %d gỗ thô. Tổng: %d",
        pname, playerid, amount, PlayerInfo[playerid][pGoThuong]);
    return 1;
}

// ============================================================
//  HOOKS
// ============================================================

hook OnPlayerConnect(playerid)
{
    PlayerLumberData[playerid][LP_Working]    = false;
    PlayerLumberData[playerid][LP_HasLog]     = false;
    PlayerLumberData[playerid][LP_VehicleID]  = INVALID_VEHICLE_ID;
    PlayerLumberData[playerid][LP_AttackTree] = -1;
    PlayerLumberData[playerid][LP_ChopTimer]  = -1;
    gLumberVehLabel[playerid]                 = Text3D:0;
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    if(PlayerLumberData[playerid][LP_Working])
        Lumber_Stop(playerid);
    return 1;
}

// Lumber_Init() is called explicitly in main.pwn OnGameModeInit after CA_Init()

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if(!PRESSED(KEY_YES)) return 1;
    if(IsPlayerInAnyVehicle(playerid)) return 1;

    // --- NPC ban / doi go ---
    if(IsPlayerInRangeOfPoint(playerid, 3.0, LUMBER_SELL_X, LUMBER_SELL_Y, LUMBER_SELL_Z))
    {
        Lumber_OpenSellDialog(playerid);
        return 1;
    }

    // --- Dat go len xe ---
    if(PlayerLumberData[playerid][LP_HasLog])
    {
        new vehicleid = PlayerLumberData[playerid][LP_VehicleID];
        if(vehicleid != INVALID_VEHICLE_ID && IsValidVehicle(vehicleid))
        {
            new Float:vx, Float:vy, Float:vz;
            GetVehiclePos(vehicleid, vx, vy, vz);
            if(IsPlayerInRangeOfPoint(playerid, 5.0, vx, vy, vz))
            {
                Lumber_DepositToVehicle(playerid);
                return 1;
            }
        }
        return 1;
    }

    // --- Chat cay ---
    if(!IsLoadingActive(playerid) && PlayerLumberData[playerid][LP_Working])
    {
        new vehicleid2 = PlayerLumberData[playerid][LP_VehicleID];
        if(vehicleid2 != INVALID_VEHICLE_ID && IsValidVehicle(vehicleid2) && gLumberVehLogCount[vehicleid2] >= LUMBER_MAX_VEH_LOGS)
        {
            SendClientMessageEx(playerid, COLOR_YELLOW, "{5DBC52}[Lumberjack] {ffffff}Xe cua ban da day go. Lai den NPC ban hoac /laygo de lay go ra.");
            ShowTDN(playerid, NOTIFICATION_WARNING, "Xe day go.");
            return 1;
        }
        new tree = Lumber_FindNearestTree(playerid);
        if(tree != -1)
        {
            Lumber_StartChop(playerid, tree);
            return 1;
        }
    }

    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_LUMBER_GPS)
    {
        if(!response) return 1;
        switch(listitem)
        {
            case 0:
            {
                PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
                SetPlayerCP(playerid, LUMBER_NPC_X, LUMBER_NPC_Y, LUMBER_NPC_Z, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{5DBC52}[GPS] {ffffff}Dang dan duong den NPC nhan viec Lumberjack.");
            }
            case 1:
            {
                PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
                SetPlayerCP(playerid, LUMBER_SELL_X, LUMBER_SELL_Y, LUMBER_SELL_Z, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{5DBC52}[GPS] {ffffff}Dang dan duong den NPC thu mua go.");
            }
            case 2:
            {
                PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
                SetPlayerCP(playerid, -676.235839, -127.256149, 60.928993, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{5DBC52}[GPS] {ffffff}Dang dan duong den khu rung.");
            }
        }
        return 1;
    }

    if(dialogid == DIALOG_LUMBER_NPC)
    {
        if(!response) return 1;
        switch(listitem)
        {
            case 0: Lumber_Start(playerid);
            case 1: Lumber_Stop(playerid);
            case 2:
            {
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "[================================================================================]");
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "[Lumberjack] Buoc 1: Chat cay trong khu rung (nhan [Y] gan cay), dat go len xe Flatbed (toi da %d khuc).", LUMBER_MAX_VEH_LOGS);
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "[Lumberjack] Buoc 2: Lai xe den NPC Thu Mua Go, nhan [Y] de ban go (1500$/khuc) hoac doi go tho vao tui.");
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "[Lumberjack] Meo: /laygo de lay 1 khuc go ra khoi xe; /chatgo de xem trang thai.");
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "[================================================================================]");
            }
        }
        return 1;
    }

    if(dialogid == DIALOG_LUMBER_SELL)
    {
        if(!response) return 1;
        switch(listitem)
        {
            case 0: Lumber_SellLog(playerid);
            case 1: Lumber_ConvertToRaw(playerid);
        }
        return 1;
    }
    return 1;
}

// ============================================================
//  COMMANDS
// ============================================================

CMD:laygo(playerid, params[])
{
    if(!PlayerLumberData[playerid][LP_Working])
        return SendClientMessageEx(playerid, COLOR_GREY, "[Lumberjack] Ban phai dang lam viec de dung lenh nay.");

    if(IsPlayerInAnyVehicle(playerid))
        return SendClientMessageEx(playerid, COLOR_GREY, "[Lumberjack] Ra khoi xe truoc khi lay go.");

    if(PlayerLumberData[playerid][LP_HasLog])
        return SendClientMessageEx(playerid, COLOR_GREY, "[Lumberjack] Ban dang cam go roi. Dat len xe truoc.");

    new vehicleid = PlayerLumberData[playerid][LP_VehicleID];
    if(vehicleid == INVALID_VEHICLE_ID || !IsValidVehicle(vehicleid))
        return SendClientMessageEx(playerid, COLOR_GREY, "[Lumberjack] Khong tim thay xe.");

    new Float:vx, Float:vy, Float:vz;
    GetVehiclePos(vehicleid, vx, vy, vz);
    if(!IsPlayerInRangeOfPoint(playerid, 5.0, vx, vy, vz))
    {
        ShowTDN(playerid, NOTIFICATION_WARNING, "Ban phai o gan xe de lay go.");
        return 1;
    }

    if(gLumberVehLogCount[vehicleid] <= 0)
    {
        ShowTDN(playerid, NOTIFICATION_WARNING, "Xe khong co go.");
        return 1;
    }

    gLumberVehLogCount[vehicleid]--;
    new slot = gLumberVehLogCount[vehicleid];
    if(gLumberVehLogObj[vehicleid][slot] != -1)
    {
        DestroyDynamicObject(gLumberVehLogObj[vehicleid][slot]);
        gLumberVehLogObj[vehicleid][slot] = -1;
    }

    PlayerLumberData[playerid][LP_HasLog] = true;
    SetPlayerAttachedObject(playerid, LUMBER_LOG_CARRY_SLOT, LUMBER_LOG_MODEL, 1,  0.1539, 0.5870, 0.0000, 0.0000, 84.7999, 0.0000, 0.8460, 0.4109, 0.4190, 0xFFFFFFFF, 0xFFFFFFFF);
    ApplyAnimation(playerid, "CARRY", "LIFTUP", 4.0, 0, 0, 0, 0, 0);
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);

    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{5DBC52}[Lumberjack] {ffffff}Lay 1 khuc go ra khoi xe (%d con lai). Den NPC nhan {FFFF00}[Y]{FFFFFF} de ban hoac doi.",
        gLumberVehLogCount[vehicleid]);
    return 1;
}

alias:vitrilumber("lumber_gps")
CMD:vitrilumber(playerid, params[])
{
    ShowPlayerDialog(playerid, DIALOG_LUMBER_GPS, DIALOG_STYLE_LIST,
        "{0f8d47}Lumberjack {FFFFFF}- Tim Duong",
        "{FFFF00}NPC Nhan Viec\n{5DBC52}NPC Thu Mua Go\n{87CEEB}Khu Rung",
        "Di den", "Huy");
    return 1;
}

CMD:chatgo(playerid, params[])
{
    if(!PlayerLumberData[playerid][LP_Working])
        return SendClientMessageEx(playerid, COLOR_GREY, "[Lumberjack] Ban chua bat dau lam viec.");

    new vehicleid = PlayerLumberData[playerid][LP_VehicleID];
    new logOnVeh = (vehicleid != INVALID_VEHICLE_ID && IsValidVehicle(vehicleid)) ? gLumberVehLogCount[vehicleid] : 0;

    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{5DBC52}[Lumberjack] {ffffff}Go tren xe: {FFFF00}%d/%d{ffffff} | Go tho (tui): {FFFF00}%d{ffffff} | Dang cam go: %s",
        logOnVeh, LUMBER_MAX_VEH_LOGS,
        PlayerInfo[playerid][pGoThuong],
        PlayerLumberData[playerid][LP_HasLog] ? "{00FF00}Co" : "{FF0000}Khong");
    return 1;
}
