// ============================================================
//  Miner Job - GTA-HTB Roleplay
// ============================================================

#include <YSI\YSI_Coding\y_hooks>

// Vi tri da tren xe (5 slot)
static const Float:gMinerStonePosOnVeh[MINER_MAX_VEH_STONES][3] = {
    {-0.3, -2.0, 0.0},
	{-0.4, -1.0, 0.0},
	{0.4, -0.8, 0.0},
	{0.4, -1.9, 0.0},
	{0.0, -1.4, 0.6}
};

// ----- Price persistence -----

stock MinerPrice_Save()
{
    new File:f = fopen("minerConfig.ini", io_write);
    if(!f) return;
    new buf[64];
    format(buf, sizeof(buf), "Sat=%d\n",       gMinerPrice[0]); fwrite(f, buf);
    format(buf, sizeof(buf), "Dong=%d\n",      gMinerPrice[1]); fwrite(f, buf);
    format(buf, sizeof(buf), "Bac=%d\n",       gMinerPrice[2]); fwrite(f, buf);
    format(buf, sizeof(buf), "Vang=%d\n",      gMinerPrice[3]); fwrite(f, buf);
    format(buf, sizeof(buf), "KimCuong=%d\n",  gMinerPrice[4]); fwrite(f, buf);
    fclose(f);
}

stock MinerPrice_Load()
{
    new File:f = fopen("minerConfig.ini", io_read);
    if(!f) { MinerPrice_Save(); return; }
    new line[64];
    while(fread(f, line, sizeof(line)))
    {
        new pos = strfind(line, "=");
        if(pos < 0) continue;
        new key[32], val[16];
        strmid(key, line, 0, pos, sizeof(key));
        strmid(val, line, pos + 1, strlen(line), sizeof(val));
        if(!strcmp(key, "Sat"))           gMinerPrice[0] = strval(val);
        else if(!strcmp(key, "Dong"))     gMinerPrice[1] = strval(val);
        else if(!strcmp(key, "Bac"))      gMinerPrice[2] = strval(val);
        else if(!strcmp(key, "Vang"))     gMinerPrice[3] = strval(val);
        else if(!strcmp(key, "KimCuong")) gMinerPrice[4] = strval(val);
    }
    fclose(f);
}

// --- Forwards ---
forward Miner_MineFinish(playerid);
forward Miner_WashFinish(playerid);
forward Miner_RespawnStone(stoneIndex);

// --- Internal state ---
static gMinerWashTimer[MAX_PLAYERS];
static STREAMER_TAG_ACTOR:gMinerSellNPC;

// ============================================================
//  HELPER
// ============================================================

static stock Miner_RollMineral()
{
    // 0=nothing 16%, 1=sat 50%, 2=dong 20%, 3=bac 10%, 4=vang 3%, 5=kimcuong 1%
    new roll = random(100);
    if(roll < 16)  return 0;
    if(roll < 66)  return 1;
    if(roll < 86)  return 2;
    if(roll < 96)  return 3;
    if(roll < 99)  return 4;
    return 5;
}

static stock Miner_FindNearestRock(playerid)
{
    for(new i = 0; i < MINER_MAX_STONES; i++)
    {
        if(!gMinerStoneActive[i] || gMinerStoneOccupied[i]) continue;
        if(IsPlayerInRangeOfPoint(playerid, 3.0, gMinerStoneX[i], gMinerStoneY[i], gMinerStoneZ[i]))
            return i;
    }
    return -1;
}

// ============================================================
//  INIT
// ============================================================

Miner_Init()
{
    new count = 0, attempts = 0;
    new Float:x, Float:y, Float:z;
    new rangeX = floatround((MINER_ZONE_MAXX - MINER_ZONE_MINX) * 100.0);
    new rangeY = floatround((MINER_ZONE_MAXY - MINER_ZONE_MINY) * 100.0);

    for(new i = 0; i < MINER_MAX_STONES; i++)
    {
        gMinerStoneActive[i]   = false;
        gMinerStoneOccupied[i] = false;
        gMinerStonePlayer[i]   = -1;
        gMinerStoneObj[i]      = -1;
    }

    for(new v = 0; v < MAX_VEHICLES; v++)
    {
        gMinerVehStoneCount[v] = 0;
        for(new s = 0; s < MINER_MAX_VEH_STONES; s++)
            gMinerVehStoneObj[v][s] = -1;
    }

    while(count < MINER_MAX_STONES && attempts < 20000)
    {
        attempts++;
        x = MINER_ZONE_MINX + float(random(rangeX)) / 100.0;
        y = MINER_ZONE_MINY + float(random(rangeY)) / 100.0;
        if(!CA_FindZ_For2DCoord(x, y, z)) continue;

        gMinerStoneX[count]   = x;
        gMinerStoneY[count]   = y;
        gMinerStoneZ[count]   = z;
        gMinerStoneObj[count] = CreateDynamicObject(MINER_ROCK_MODEL, x, y, z, 0.0, 0.0, float(random(360)));
        gMinerStoneActive[count] = true;
        count++;
    }
    MinerPrice_Load();
    printf("[Miner] %d/%d rocks spawned (%d attempts).", count, MINER_MAX_STONES, attempts);

    // Sell NPC + label
    gMinerSellNPC = CreateDynamicActor(49, MINER_SELL_X, MINER_SELL_Y, MINER_SELL_Z, 202.849380, true);
    CreateDynamic3DTextLabel(
        "{F5F52F}Nguoi Mua Khoang San\n{FFFFFF}Nhan {5FBC52}[Y]{FFFFFF} de ban khoang san",
        -1, MINER_SELL_X, MINER_SELL_Y, MINER_SELL_Z + 0.3, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1);

    // Wash station label + pickup
    CreateDynamic3DTextLabel(
        "{00CFFF}Tram Rua Khoang San\n{FFFFFF}Nhan {5FBC52}[Y]{FFFFFF} khi co da tren xe",
        -1, MINER_WASH_X, MINER_WASH_Y, MINER_WASH_Z + 0.3, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1);
    CreateDynamicPickup(1239, 23, MINER_WASH_X, MINER_WASH_Y, MINER_WASH_Z, -1, -1, -1, 15.0);

    return 1;
}

// ============================================================
//  START / STOP
// ============================================================

Miner_Start(playerid)
{
    if(PlayerMinerData[playerid][MP_Working])
    {
        ShowTDN(playerid, NOTIFICATION_WARNING, "Ban dang lam viec roi.");
        return 1;
    }

    PlayerMinerData[playerid][MP_Working]    = true;
    PlayerMinerData[playerid][MP_VehicleID]  = INVALID_VEHICLE_ID;
    PlayerMinerData[playerid][MP_HasStone]   = false;
    PlayerMinerData[playerid][MP_AttackStone]= -1;
    PlayerMinerData[playerid][MP_MineTimer]  = -1;
    gMinerWashTimer[playerid]                = -1;
    PlayerWorking[playerid]                  = JOB_MINER;
    SetPlayerSkin(playerid, JobInfo[JOB_MINER][jobSkin]);

    // Spawn dedicated bobcat
    new vehicleid = CreateVehicle(422, 576.9451, 867.7784, -43.4240, 180.5633, -1, -1, -1);
    if(vehicleid == INVALID_VEHICLE_ID)
    {
        ShowTDN(playerid, NOTIFICATION_ERROR, "Loi: Khong the tao xe. Thu lai sau.");
        PlayerMinerData[playerid][MP_Working] = false;
        PlayerWorking[playerid] = 0;
        return 1;
    }
    LockStatus{vehicleid} = 0;
    SetVehicleHealth(vehicleid, 999.0);
    PutPlayerInVehicle(playerid, vehicleid, 0);

    PlayerMinerData[playerid][MP_VehicleID] = vehicleid;
    gMinerVehStoneCount[vehicleid] = 0;
    for(new s = 0; s < MINER_MAX_VEH_STONES; s++) gMinerVehStoneObj[vehicleid][s] = -1;

    new labelStr[128];
    format(labelStr, sizeof(labelStr), "{6699CC}Khai Thac Khoang San\n{FFFFFF}Chu xe: {6699CC}%s", GetPlayerNameEx(playerid));
    gMinerVehLabel[playerid] = Create3DTextLabel(labelStr, 0x6699CCFF, 0.0, 0.0, 0.0, 12.0, 0);
    Attach3DTextLabelToVehicle(gMinerVehLabel[playerid], vehicleid, 0.0, 0.0, 0.0);

    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{6699CC}[Khai Thac] {ffffff} Bat dau lam viec. Lai Bobcat vao khu mo de khai thac da.");
    //ShowTDN(playerid, NOTIFICATION_SUCCESS, "Bat dau lam viec. Lai Bobcat vao khu mo de khai thac da.");
    return 1;
}

Miner_Stop(playerid)
{
    if(!PlayerMinerData[playerid][MP_Working]) return SendClientMessageEx(playerid, COLOR_GREY, "Ban chua bat dau lam viec.");

    if(PlayerMinerData[playerid][MP_MineTimer] != -1)
    {
        KillTimer(PlayerMinerData[playerid][MP_MineTimer]);
        PlayerMinerData[playerid][MP_MineTimer] = -1;
    }
    if(gMinerWashTimer[playerid] != -1)
    {
        gMinerWashTimer[playerid] = -1;
    }

    new attackStone = PlayerMinerData[playerid][MP_AttackStone];
    if(attackStone >= 0 && attackStone < MINER_MAX_STONES)
    {
        gMinerStoneOccupied[attackStone] = false;
        gMinerStonePlayer[attackStone]   = -1;
    }

    if(PlayerMinerData[playerid][MP_HasStone])
    {
        RemovePlayerAttachedObject(playerid, MINER_STONE_CARRY_SLOT);
        PlayerMinerData[playerid][MP_HasStone] = false;
    }

    new vehicleid = PlayerMinerData[playerid][MP_VehicleID];
    if(vehicleid != INVALID_VEHICLE_ID && IsValidVehicle(vehicleid))
    {
        for(new s = 0; s < gMinerVehStoneCount[vehicleid]; s++)
        {
            if(gMinerVehStoneObj[vehicleid][s] != -1)
            {
                DestroyDynamicObject(gMinerVehStoneObj[vehicleid][s]);
                gMinerVehStoneObj[vehicleid][s] = -1;
            }
        }
        gMinerVehStoneCount[vehicleid] = 0;
        DestroyVehicle(vehicleid);
    }

    if(gMinerVehLabel[playerid] != Text3D:0)
    {
        Delete3DTextLabel(gMinerVehLabel[playerid]);
        gMinerVehLabel[playerid] = Text3D:0;
    }

    RemovePlayerAttachedObject(playerid, MINER_TOOL_SLOT);
    StopLoading(playerid);
    ClearAnimations(playerid);

    PlayerMinerData[playerid][MP_Working]    = false;
    PlayerMinerData[playerid][MP_VehicleID]  = INVALID_VEHICLE_ID;
    PlayerMinerData[playerid][MP_AttackStone]= -1;
    PlayerWorking[playerid]                  = 0;
    SetPlayerSkin(playerid, PlayerInfo[playerid][pModel]);

    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{6699CC}[Khai Thac] {ffffff} Ban da ngung lam viec.");
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Da ngung lam viec.");
    return 1;
}

// ============================================================
//  MINING
// ============================================================

static stock Miner_StartMine(playerid, stoneIndex)
{
    gMinerStoneOccupied[stoneIndex] = true;
    gMinerStonePlayer[stoneIndex]   = playerid;
    PlayerMinerData[playerid][MP_AttackStone] = stoneIndex;

    ApplyAnimation(playerid, "BASEBALL", "Bat_4", 4.1, 1, 0, 0, 1, 0, 1);
    SetPlayerAttachedObject(playerid, MINER_TOOL_SLOT, 2237, 6, 0.086999, -0.063999, 0.000000, 0.000000, -160.799942, 0.000000, 1.000000, 1.000000, 1.000000);
    SetLoading(playerid, MINER_MINE_TIME, "Dang khai thac...", "Miner_MineFinish");

    new string[128];
    format(string, sizeof(string), "{FF8000}* {C2A2DA}%s dang khai thac khoang san.", GetPlayerNameEx(playerid));
    ProxDetectorWrap(playerid, string, 92, 30.0, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
    return 1;
}

public Miner_MineFinish(playerid)
{
    new stoneIndex = PlayerMinerData[playerid][MP_AttackStone];
    PlayerMinerData[playerid][MP_AttackStone] = -1;

    if(stoneIndex < 0 || stoneIndex >= MINER_MAX_STONES) return 1;

    gMinerStoneOccupied[stoneIndex] = false;
    gMinerStonePlayer[stoneIndex]   = -1;

    RemovePlayerAttachedObject(playerid, MINER_TOOL_SLOT);

    if(!IsPlayerInRangeOfPoint(playerid, 5.0, gMinerStoneX[stoneIndex], gMinerStoneY[stoneIndex], gMinerStoneZ[stoneIndex]))
    {
        ClearAnimations(playerid);
        ShowTDN(playerid, NOTIFICATION_ERROR, "Khai thac that bai vi ban di qua xa.");
        SendClientMessageEx(playerid, COLOR_RED, "{6699CC}[Khai Thac] {ffffff} Ban da di chuyen qua xa, khai thac that bai.");
        return 1;
    }

    ApplyAnimation(playerid, "CARRY", "LIFTUP", 4.0999, 0, 0, 0, 0, 0);

    if(gMinerStoneObj[stoneIndex] != -1)
    {
        DestroyDynamicObject(gMinerStoneObj[stoneIndex]);
        gMinerStoneObj[stoneIndex] = -1;
    }
    gMinerStoneActive[stoneIndex] = false;
    SetTimerEx("Miner_RespawnStone", MINER_RESPAWN_TIME, false, "d", stoneIndex);

    PlayerMinerData[playerid][MP_HasStone] = true;
    SetPlayerAttachedObject(playerid, MINER_STONE_CARRY_SLOT, MINER_STONE_MODEL, 5, 0.219000, 0.000000, 0.145000, -82.599922, 0.000000, 102.000038, 1.000000, 1.000000, 1.000000, 0, 0);
    
    ApplyAnimation(playerid, "CARRY", "LIFTUP", 4.0, 0, 0, 0, 0, 0);
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);

    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{6699CC}[Khai Thac] {ffffff}Khai thac thanh cong! Den xe an Y de dat da len xe.");
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Khai thac thanh cong.");

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
    format(string, sizeof(string), "{FF8000}* {C2A2DA}%s da khai thac duoc mot vien da.", GetPlayerNameEx(playerid));
    ProxDetectorWrap(playerid, string, 92, 30.0, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
    return 1;
}

public Miner_RespawnStone(stoneIndex)
{
    if(stoneIndex < 0 || stoneIndex >= MINER_MAX_STONES) return 1;
    gMinerStoneObj[stoneIndex] = CreateDynamicObject(MINER_ROCK_MODEL,
        gMinerStoneX[stoneIndex], gMinerStoneY[stoneIndex], gMinerStoneZ[stoneIndex],
        0.0, 0.0, float(random(360)));
    gMinerStoneActive[stoneIndex] = true;
    return 1;
}

// ============================================================
//  DEPOSIT TO VEHICLE
// ============================================================

static stock Miner_DepositToVehicle(playerid)
{
    new vehicleid = PlayerMinerData[playerid][MP_VehicleID];
    if(vehicleid == INVALID_VEHICLE_ID || !IsValidVehicle(vehicleid))
    {
        ShowTDN(playerid, NOTIFICATION_ERROR, "Khong tim thay xe cua ban.");
        return 0;
    }
    if(gMinerVehStoneCount[vehicleid] >= MINER_MAX_VEH_STONES)
    {
        SendClientMessageEx(playerid, COLOR_YELLOW, "{6699CC}[Khai Thac] {ffffff}Xe cua ban da day da. Hay lai den Tram Rua de rua khoang san.");
        ShowTDN(playerid, NOTIFICATION_WARNING, "Xe khoang san da day da.");
        return 0;
    }

    new slot = gMinerVehStoneCount[vehicleid];
    gMinerVehStoneObj[vehicleid][slot] = CreateDynamicObject(MINER_STONE_MODEL, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    AttachDynamicObjectToVehicle(gMinerVehStoneObj[vehicleid][slot], vehicleid,
        gMinerStonePosOnVeh[slot][0], gMinerStonePosOnVeh[slot][1], gMinerStonePosOnVeh[slot][2],
        0.0, 0.0, 0.0);
    gMinerVehStoneCount[vehicleid]++;

    RemovePlayerAttachedObject(playerid, MINER_STONE_CARRY_SLOT);
    PlayerMinerData[playerid][MP_HasStone] = false;
    SetPlayerSpecialAction(playerid, 0);

    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{6699CC}[Khai Thac] {ffffff}Da dat khoang san len xe (%d/%d).",
        gMinerVehStoneCount[vehicleid], MINER_MAX_VEH_STONES);
    if(gMinerVehStoneCount[vehicleid] >= MINER_MAX_VEH_STONES)
        ShowTDN(playerid, NOTIFICATION_SUCCESS, "Xe day. Lai den Tram Rua de xu ly khoang san.");
    return 1;
}

// ============================================================
//  WASH STATION
// ============================================================

static stock Miner_StartWash(playerid)
{
    new vehicleid = PlayerMinerData[playerid][MP_VehicleID];
    if(vehicleid == INVALID_VEHICLE_ID || !IsValidVehicle(vehicleid) || gMinerVehStoneCount[vehicleid] <= 0)
    {
        ShowTDN(playerid, NOTIFICATION_WARNING, "Xe khong co da de rua.");
        return 0;
    }

    ApplyAnimation(playerid, "BD_FIRE", "wash_up", 4.1, 1, 0, 0, 0, 10000, 1);
    gMinerWashTimer[playerid] = SetTimerEx("Miner_WashFinish", 10000, false, "d", playerid);

    new string[128];
    format(string, sizeof(string), "{FF8000}* {C2A2DA}%s dang rua khoang san.", GetPlayerNameEx(playerid));
    ProxDetectorWrap(playerid, string, 92, 30.0, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
    return 1;
}

public Miner_WashFinish(playerid)
{
    gMinerWashTimer[playerid] = -1;
    ClearAnimations(playerid);

    new minerals[6];
    new pname[MAX_PLAYER_NAME];
    GetPlayerName(playerid, pname, sizeof(pname));

    // Single stone carried by player (/layda flow)
    if(PlayerMinerData[playerid][MP_HasStone])
    {
        new mType = Miner_RollMineral();
        if(mType > 0) minerals[mType] += random(5) + 1;
        PlayerInfo[playerid][pMinSat]      += minerals[1];
        PlayerInfo[playerid][pMinDong]     += minerals[2];
        PlayerInfo[playerid][pMinBac]      += minerals[3];
        PlayerInfo[playerid][pMinVang]     += minerals[4];
        PlayerInfo[playerid][pMinKimCuong] += minerals[5];

        PlayerMinerData[playerid][MP_HasStone] = false;
        RemovePlayerAttachedObject(playerid, MINER_STONE_CARRY_SLOT);
        SetPlayerSpecialAction(playerid, 0);
        
        new mMsg[96]; new mPart[24];
        if(minerals[1]) { format(mPart, sizeof(mPart), "nhan duoc x%d Sat ", minerals[1]);  strcat(mMsg, mPart); }
        if(minerals[2]) { format(mPart, sizeof(mPart), "nhan duoc x%d Dong ", minerals[2]); strcat(mMsg, mPart); }
        if(minerals[3]) { format(mPart, sizeof(mPart), "nhan duoc x%d Bac ", minerals[3]);  strcat(mMsg, mPart); }
        if(minerals[4]) { format(mPart, sizeof(mPart), "nhan duoc x%d Vang ", minerals[4]); strcat(mMsg, mPart); }
        if(minerals[5]) { format(mPart, sizeof(mPart), "nhan duoc x%d Kim cuong", minerals[5]);    strcat(mMsg, mPart); }
        if(!mMsg[0]) strcat(mMsg, "khong nhan duoc gi", sizeof(mMsg));
        SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{6699CC}[Khai Thac] {ffffff}Rua xong 1 vien da va %s", mMsg);
        ShowTDN(playerid, NOTIFICATION_SUCCESS, "Rua da xong.");
        
        new string[128];
        format(string, sizeof(string), "{FF8000}* {C2A2DA}%s da rua khoang san va nhan thu gi do.", GetPlayerNameEx(playerid));
        ProxDetectorWrap(playerid, string, 92, 30.0, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
        
        new dtitle_w[128], dstr_w[256];
        format(dtitle_w, sizeof(dtitle_w), "%s (ID: %d) - Rửa Đá (Thợ Khai Thác)", pname, playerid);
        format(dstr_w, sizeof(dstr_w), ":pick: **RỬA ĐÁ THÀNH CÔNG**\n:gem: **Khoáng sản nhận được:**\n- Sắt: %d | Đồng: %d | Bạc: %d | Vàng: %d | Kim Cương: %d",
            minerals[1], minerals[2], minerals[3], minerals[4], minerals[5]);
        DiscordLog(dclog_Miner, dtitle_w, dstr_w);
        return 1;
    }

    // All vehicle stones
    /* new vehicleid = PlayerMinerData[playerid][MP_VehicleID];
    if(vehicleid == INVALID_VEHICLE_ID || !IsValidVehicle(vehicleid) || gMinerVehStoneCount[vehicleid] <= 0) return 1;

    new count = gMinerVehStoneCount[vehicleid];
    for(new i = 0; i < count; i++)
    {
        new mType = Miner_RollMineral();
        if(mType > 0) minerals[mType] += random(5) + 1;
    }

    PlayerInfo[playerid][pMinSat]      += minerals[1];
    PlayerInfo[playerid][pMinDong]     += minerals[2];
    PlayerInfo[playerid][pMinBac]      += minerals[3];
    PlayerInfo[playerid][pMinVang]     += minerals[4];
    PlayerInfo[playerid][pMinKimCuong] += minerals[5];

    for(new s = 0; s < count; s++)
    {
        if(gMinerVehStoneObj[vehicleid][s] != -1)
        {
            DestroyDynamicObject(gMinerVehStoneObj[vehicleid][s]);
            gMinerVehStoneObj[vehicleid][s] = -1;
        }
    }
    gMinerVehStoneCount[vehicleid] = 0;

    new bMsg[96]; new bPart[24];
    if(minerals[1]) { format(bPart, sizeof(bPart), "Sat x%d ", minerals[1]);  strcat(bMsg, bPart); }
    if(minerals[2]) { format(bPart, sizeof(bPart), "Dong x%d ", minerals[2]); strcat(bMsg, bPart); }
    if(minerals[3]) { format(bPart, sizeof(bPart), "Bac x%d ", minerals[3]);  strcat(bMsg, bPart); }
    if(minerals[4]) { format(bPart, sizeof(bPart), "Vang x%d ", minerals[4]); strcat(bMsg, bPart); }
    if(minerals[5]) { format(bPart, sizeof(bPart), "KC x%d", minerals[5]);    strcat(bMsg, bPart); }
    if(!bMsg[0]) strcat(bMsg, "Khong ra khoang san nao", sizeof(bMsg));
    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{6699CC}[Khai Thac] {ffffff}Rua xong %d vien da! %s", count, bMsg);
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Rua da xong. Den NPC ban khoang san.");
    DiscordLog(dclog_Miner,
        "THỢ KHAI THÁC - RỬA ĐÁ",
        "**%s (ID: %d)** rửa %d viên đá.\nSắt: %d | Đồng: %d | Bạc: %d | Vàng: %d | Kim Cương: %d",
        pname, playerid, count, minerals[1], minerals[2], minerals[3], minerals[4], minerals[5]); */
    return 1;
}

// ============================================================
//  SELL NPC
// ============================================================

static stock Miner_OpenSellDialog(playerid)
{
    new sat = PlayerInfo[playerid][pMinSat];
    new dong = PlayerInfo[playerid][pMinDong];
    new bac = PlayerInfo[playerid][pMinBac];
    new vang = PlayerInfo[playerid][pMinVang];
    new kc = PlayerInfo[playerid][pMinKimCuong];

    if(sat + dong + bac + vang + kc == 0)
    {
        ShowTDN(playerid, NOTIFICATION_WARNING, "Ban chua co khoang san nao.");
        return 0;
    }

    new total = sat * gMinerPrice[0] + dong * gMinerPrice[1] + bac * gMinerPrice[2] +
                vang * gMinerPrice[3] + kc * gMinerPrice[4];

    new dialog[768], line[128];
    strcat(dialog, "{6699CC}Khoang San\t{FFFFFF}So Luong\t{FFFF00}Gia/Don Vi\t{00FF7F}Tong\n", sizeof(dialog));
    format(line, sizeof(line), "{B0C4DE}Sat\t{FFFFFF}%d\t{FFFF00}$%d\t{00FF7F}$%d\n",      sat,  gMinerPrice[0], sat  * gMinerPrice[0]); strcat(dialog, line, sizeof(dialog));
    format(line, sizeof(line), "{B87333}Dong\t{FFFFFF}%d\t{FFFF00}$%d\t{00FF7F}$%d\n",     dong, gMinerPrice[1], dong * gMinerPrice[1]); strcat(dialog, line, sizeof(dialog));
    format(line, sizeof(line), "{C0C0C0}Bac\t{FFFFFF}%d\t{FFFF00}$%d\t{00FF7F}$%d\n",      bac,  gMinerPrice[2], bac  * gMinerPrice[2]); strcat(dialog, line, sizeof(dialog));
    format(line, sizeof(line), "{FFD700}Vang\t{FFFFFF}%d\t{FFFF00}$%d\t{00FF7F}$%d\n",     vang, gMinerPrice[3], vang * gMinerPrice[3]); strcat(dialog, line, sizeof(dialog));
    format(line, sizeof(line), "{00FFFF}Kim Cuong\t{FFFFFF}%d\t{FFFF00}$%d\t{00FF7F}$%d\n",kc,   gMinerPrice[4], kc   * gMinerPrice[4]); strcat(dialog, line, sizeof(dialog));
    format(line, sizeof(line), "{FFFF00}[Ban tat ca]\t\t\t{00FF7F}$%d",                     total);                                       strcat(dialog, line, sizeof(dialog));

    ShowPlayerDialog(playerid, DIALOG_MINER_SELL, DIALOG_STYLE_TABLIST_HEADERS,
        "{F5D000}Nguoi Mua Khoang San", dialog, "Ban", "Huy");
    return 1;
}

static stock Miner_SellType(playerid, listitem)
{
    if(listitem == 5) { Miner_SellAll(playerid); return 1; }
    if(listitem < 0 || listitem > 4) return 0;

    static const sNames[][16]   = {"Sat", "Dong", "Bac", "Vang", "Kim Cuong"};
    static const sDcNames[][32] = {"Sắt", "Đồng", "Bạc", "Vàng", "Kim Cương"};

    new amount;
    switch(listitem)
    {
        case 0: amount = PlayerInfo[playerid][pMinSat];
        case 1: amount = PlayerInfo[playerid][pMinDong];
        case 2: amount = PlayerInfo[playerid][pMinBac];
        case 3: amount = PlayerInfo[playerid][pMinVang];
        case 4: amount = PlayerInfo[playerid][pMinKimCuong];
    }
    new price = gMinerPrice[listitem];

    if(amount <= 0)
    {
        ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong co loai khoang san nay.");
        Miner_OpenSellDialog(playerid);
        return 0;
    }

    new income = amount * price;
    GivePlayerCash(playerid, income);

    switch(listitem)
    {
        case 0: PlayerInfo[playerid][pMinSat]      = 0;
        case 1: PlayerInfo[playerid][pMinDong]     = 0;
        case 2: PlayerInfo[playerid][pMinBac]      = 0;
        case 3: PlayerInfo[playerid][pMinVang]     = 0;
        case 4: PlayerInfo[playerid][pMinKimCuong] = 0;
    }

    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{6699CC}[Khai Thac] {ffffff}Da ban %d %s, nhan ${00FF7F}%d.", amount, sNames[listitem], income);
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Ban khoang san thanh cong!");

    new pname[MAX_PLAYER_NAME];
    GetPlayerName(playerid, pname, sizeof(pname));
    new dtitle_s[128], dstr_s[256];
    format(dtitle_s, sizeof(dtitle_s), "%s (ID: %d) - Bán Khoáng Sản", pname, playerid);
    format(dstr_s, sizeof(dstr_s), ":pick: **BÁN KHOÁNG SẢN THÀNH CÔNG**\n:gem: **Loại: %s x%d**\n:moneybag: **Thu về: $%s**",
        sDcNames[listitem], amount, number_format(income));
    DiscordLog(dclog_Miner, dtitle_s, dstr_s);

    new remaining = PlayerInfo[playerid][pMinSat] + PlayerInfo[playerid][pMinDong] +
                    PlayerInfo[playerid][pMinBac]  + PlayerInfo[playerid][pMinVang] + PlayerInfo[playerid][pMinKimCuong];
    if(remaining > 0)
        Miner_OpenSellDialog(playerid);
    return 1;
}

static stock Miner_SellAll(playerid)
{
    new sat = PlayerInfo[playerid][pMinSat];
    new dong = PlayerInfo[playerid][pMinDong];
    new bac = PlayerInfo[playerid][pMinBac];
    new vang = PlayerInfo[playerid][pMinVang];
    new kc = PlayerInfo[playerid][pMinKimCuong];
    new total = sat * gMinerPrice[0] + dong * gMinerPrice[1] + bac * gMinerPrice[2] +
                vang * gMinerPrice[3] + kc * gMinerPrice[4];

    if(total <= 0) return 0;

    GivePlayerCash(playerid, total);
    PlayerInfo[playerid][pMinSat]      = 0;
    PlayerInfo[playerid][pMinDong]     = 0;
    PlayerInfo[playerid][pMinBac]      = 0;
    PlayerInfo[playerid][pMinVang]     = 0;
    PlayerInfo[playerid][pMinKimCuong] = 0;

    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{6699CC}[Khai Thac] {ffffff}Da ban khoang san, nhan ${00FF7F}%d.", total);
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Ban khoang san thanh cong!");

    new pname[MAX_PLAYER_NAME];
    GetPlayerName(playerid, pname, sizeof(pname));
    new dtitle_a[128], dstr_a[256];
    format(dtitle_a, sizeof(dtitle_a), "%s (ID: %d) - Bán Toàn Bộ Khoáng Sản", pname, playerid);
    format(dstr_a, sizeof(dstr_a), ":pick: **BÁN TOÀN BỘ THÀNH CÔNG**\n:gem: **Khoáng sản:**\n- Sắt: %d | Đồng: %d | Bạc: %d | Vàng: %d | Kim Cương: %d\n:moneybag: **Tổng: $%s**",
        sat, dong, bac, vang, kc, number_format(total));
    DiscordLog(dclog_Miner, dtitle_a, dstr_a);
    return 1;
}

// ============================================================
//  HOOKS
// ============================================================

hook OnPlayerConnect(playerid)
{
    PlayerMinerData[playerid][MP_Working]    = false;
    PlayerMinerData[playerid][MP_HasStone]   = false;
    PlayerMinerData[playerid][MP_VehicleID]  = INVALID_VEHICLE_ID;
    PlayerMinerData[playerid][MP_AttackStone]= -1;
    PlayerMinerData[playerid][MP_MineTimer]  = -1;
    gMinerWashTimer[playerid]                = -1;
    gMinerVehLabel[playerid]                 = Text3D:0;
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    if(PlayerMinerData[playerid][MP_Working])
        Miner_Stop(playerid);
    return 1;
}

// ============================================================
//  TEST COMMANDS (xoa sau khi test xong)
// ============================================================

CMD:testminer(playerid, params[])
{
    // Set job + start working
    PlayerInfo[playerid][pJob] = JOB_MINER;
    Miner_Start(playerid);

    // Teleport to mine zone entrance (near job NPC)
    SetPlayerPos(playerid, 625.0, 742.0, 14.5);
    SetPlayerFacingAngle(playerid, 180.0);

    SendClientMessageEx(playerid, COLOR_YELLOW, "--- [TEST MINER] FLOW HUONG DAN ---");
    SendClientMessageEx(playerid, COLOR_WHITE,  "1. Di bo den vien da (model xanh trong khu mo), nhan {FFFF00}[Y]{FFFFFF} de khai thac (10 giay)");
    SendClientMessageEx(playerid, COLOR_WHITE,  "2. Khi co da, den sat xe Bobcat, nhan {FFFF00}[Y]{FFFFFF} de dat len xe (toi da 5 vien)");
    SendClientMessageEx(playerid, COLOR_WHITE,  "3. Lai xe den Tram Rua (%0.1f, %0.1f, %0.1f), nhan {FFFF00}[Y]{FFFFFF} khi ra khoi xe (10 giay)",
        MINER_WASH_X, MINER_WASH_Y, MINER_WASH_Z);
    SendClientMessageEx(playerid, COLOR_WHITE,  "4. Den NPC ban khoang san (%0.1f, %0.1f, %0.1f), nhan {FFFF00}[Y]{FFFFFF} de ban",
        MINER_SELL_X, MINER_SELL_Y, MINER_SELL_Z);
    SendClientMessageEx(playerid, COLOR_YELLOW, "--- lenh test: /testminerfill /testminermin /testminerwash /testminerstop ---");
    return 1;
}

// Skip mining: fill vehicle with max stones instantly
CMD:testminerfill(playerid, params[])
{
    if(!PlayerMinerData[playerid][MP_Working])
        return SendClientMessageEx(playerid, COLOR_GREY, "[Test] Chua bat dau lam viec. Dung /testminer truoc.");

    new vehicleid = PlayerMinerData[playerid][MP_VehicleID];
    if(vehicleid == INVALID_VEHICLE_ID || !IsValidVehicle(vehicleid))
        return SendClientMessageEx(playerid, COLOR_GREY, "[Test] Khong co xe.");

    // Remove existing
    for(new s = 0; s < gMinerVehStoneCount[vehicleid]; s++)
    {
        if(gMinerVehStoneObj[vehicleid][s] != -1)
            DestroyDynamicObject(gMinerVehStoneObj[vehicleid][s]);
    }
    gMinerVehStoneCount[vehicleid] = 0;

    // Fill all 5 slots
    for(new s = 0; s < MINER_MAX_VEH_STONES; s++)
    {
        gMinerVehStoneObj[vehicleid][s] = CreateDynamicObject(MINER_STONE_MODEL, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
        AttachDynamicObjectToVehicle(gMinerVehStoneObj[vehicleid][s], vehicleid,
            gMinerStonePosOnVeh[s][0], gMinerStonePosOnVeh[s][1], gMinerStonePosOnVeh[s][2],
            0.0, 0.0, 0.0);
    }
    gMinerVehStoneCount[vehicleid] = MINER_MAX_VEH_STONES;

    // Clear any stone on player
    if(PlayerMinerData[playerid][MP_HasStone])
    {
        RemovePlayerAttachedObject(playerid, MINER_STONE_CARRY_SLOT);
        PlayerMinerData[playerid][MP_HasStone] = false;
    }

    new labelStr[64];
    format(labelStr, sizeof(labelStr), "Da: %d/%d", MINER_MAX_VEH_STONES, MINER_MAX_VEH_STONES);
    UpdateDynamic3DTextLabelText(gMinerVehLabel[playerid], 0xFFFF00FF, labelStr);

    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{6699CC}[Test] {ffffff}Da nap day %d da len xe. Lai den Tram Rua (%0.1f %0.1f %0.1f).",
        MINER_MAX_VEH_STONES, MINER_WASH_X, MINER_WASH_Y, MINER_WASH_Z);
    return 1;
}

// Skip wash: give minerals directly
CMD:testminermin(playerid, params[])
{
    PlayerInfo[playerid][pMinSat]      += 5;
    PlayerInfo[playerid][pMinDong]     += 3;
    PlayerInfo[playerid][pMinBac]      += 2;
    PlayerInfo[playerid][pMinVang]     += 1;
    PlayerInfo[playerid][pMinKimCuong] += 1;

    SendClientMessageEx(playerid, COLOR_LIGHTBLUE,
        "{6699CC}[Test] {ffffff}+5 Sat, +3 Dong, +2 Bac, +1 Vang, +1 Kim Cuong. Den NPC (%0.1f %0.1f %0.1f) nhan [Y] ban.",
        MINER_SELL_X, MINER_SELL_Y, MINER_SELL_Z);
    return 1;
}

// Teleport to wash station
CMD:testminerwash(playerid, params[])
{
    SetPlayerPos(playerid, MINER_WASH_X, MINER_WASH_Y, MINER_WASH_Z + 0.5);
    SetPlayerFacingAngle(playerid, 0.0);
    SendClientMessageEx(playerid, COLOR_LIGHTBLUE,
        "{6699CC}[Test] {ffffff}TP den Tram Rua. Ra khoi xe roi nhan {FFFF00}[Y]{FFFFFF} (xe phai o trong vong 25m va co da).");
    return 1;
}

// Stop and clean up
CMD:testminerstop(playerid, params[])
{
    Miner_Stop(playerid);
    PlayerInfo[playerid][pJob] = JOB_NONE;
    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "[Test] Da dung lam viec va xoa job miner.");
    return 1;
}

// Show stone stats
CMD:testminerstones(playerid, params[])
{
    new active = 0, occupied = 0;
    for(new i = 0; i < MINER_MAX_STONES; i++)
    {
        if(gMinerStoneActive[i])   active++;
        if(gMinerStoneOccupied[i]) occupied++;
    }
    SendClientMessageEx(playerid, COLOR_YELLOW, "[Test Miner] Da: %d/%d active | %d dang bi khai thac.",
        active, MINER_MAX_STONES, occupied);
    new vehicleid = PlayerMinerData[playerid][MP_VehicleID];
    if(vehicleid != INVALID_VEHICLE_ID && IsValidVehicle(vehicleid))
        SendClientMessageEx(playerid, COLOR_YELLOW, "[Test Miner] Xe ID %d co %d/%d da.",
            vehicleid, gMinerVehStoneCount[vehicleid], MINER_MAX_VEH_STONES);
    return 1;
}

CMD:layda(playerid, params[])
{
    if(!PlayerMinerData[playerid][MP_Working])
        return SendClientMessageEx(playerid, COLOR_GREY, "[Khai Thac] Ban phai dang lam viec de dung lenh nay.");

    if(IsPlayerInAnyVehicle(playerid))
        return SendClientMessageEx(playerid, COLOR_GREY, "[Khai Thac] Ra khoi xe truoc khi lay da.");

    if(PlayerMinerData[playerid][MP_HasStone])
        return SendClientMessageEx(playerid, COLOR_GREY, "[Khai Thac] Ban dang cam da roi. Mang den Tram Rua an [Y].");

    new vehicleid = PlayerMinerData[playerid][MP_VehicleID];
    if(vehicleid == INVALID_VEHICLE_ID || !IsValidVehicle(vehicleid))
        return SendClientMessageEx(playerid, COLOR_GREY, "[Khai Thac] Khong tim thay xe.");

    
    new Float:vx, Float:vy, Float:vz;
    GetVehiclePos(vehicleid, vx, vy, vz);
    if(!IsPlayerInRangeOfPoint(playerid, 5.0, vx, vy, vz))
    {
        ShowTDN(playerid, NOTIFICATION_WARNING, "Ban phai o gan xe de lay da.");
        return 1;
    }

    if(gMinerVehStoneCount[vehicleid] <= 0)
    {
        ShowTDN(playerid, NOTIFICATION_WARNING, "Xe khong co da.");
        return 1;
    }

    // Remove last stone from vehicle
    gMinerVehStoneCount[vehicleid]--;
    new slot = gMinerVehStoneCount[vehicleid];
    if(gMinerVehStoneObj[vehicleid][slot] != -1)
    {
        DestroyDynamicObject(gMinerVehStoneObj[vehicleid][slot]);
        gMinerVehStoneObj[vehicleid][slot] = -1;
    }

    // Attach stone to player (same as MineFinish)
    PlayerMinerData[playerid][MP_HasStone] = true;
    SetPlayerAttachedObject(playerid, MINER_STONE_CARRY_SLOT, MINER_STONE_MODEL, 5, 0.219000, 0.000000, 0.145000, -82.599922, 0.000000, 102.000038, 1.000000, 1.000000, 1.000000, 0, 0);
    ApplyAnimation(playerid, "CARRY", "LIFTUP", 4.0, 0, 0, 0, 0, 0);
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);

    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{6699CC}[Khai Thac] {ffffff}Lay 1 vien da ra khoi xe (%d con lai). Mang den Tram Rua an {FFFF00}[Y]{FFFFFF} de rua.",
        gMinerVehStoneCount[vehicleid]);
    return 1;
}

CMD:chinhgiaminer(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] == 0) return 1;

    static const sNames[][16] = {"Sat", "Dong", "Bac", "Vang", "Kim Cuong"};

    new type, value;
    if(sscanf(params, "ii", type, value))
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Su dung: /chinhgiaminer [Loai 0-4] [Gia]");
        SendClientMessageEx(playerid, COLOR_WHITE, "0=Sat | 1=Dong | 2=Bac | 3=Vang | 4=Kim Cuong");
        SendClientMessageEx(playerid, COLOR_WHITE, "Gia hien tai: Sat=$%d | Dong=$%d | Bac=$%d | Vang=$%d | KC=$%d",
            gMinerPrice[0], gMinerPrice[1], gMinerPrice[2], gMinerPrice[3], gMinerPrice[4]);
        return 1;
    }
    if(type < 0 || type > 4)
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Loai khong hop le (0-4)!"); return 1;
    }
    if(value < 1)
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Gia phai lon hon 0!"); return 1;
    }

    gMinerPrice[type] = value;
    MinerPrice_Save();

    SendClientMessageEx(playerid, COLOR_WHITE,
        "[Admin-Miner] Da doi gia {44ff00}%s{FFFFFF} thanh {5BFF89}$%d{FFFFFF}.", sNames[type], value);
    foreach(new i : Player)
    {
        if(PlayerInfo[i][pAdmin] > 0 && i != playerid)
            SendClientMessageEx(i, COLOR_RED,
                "[Admin-Miner] %s doi gia %s thanh $%d.", GetPlayerNameEx(playerid), sNames[type], value);
    }
    return 1;
}

// ============================================================
//  HOOKS
// ============================================================

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if(!PRESSED(KEY_YES)) return 1;
    //if(!PlayerMinerData[playerid][MP_Working]) return 1;
    if(IsPlayerInAnyVehicle(playerid)) return 1;

    // --- Sell NPC ---
    if(IsPlayerInRangeOfPoint(playerid, 3.0, MINER_SELL_X, MINER_SELL_Y, MINER_SELL_Z))
    {
        Miner_OpenSellDialog(playerid);
        return 1;
    }

    // --- Wash station ---
    if(IsPlayerInRangeOfPoint(playerid, 5.0, MINER_WASH_X, MINER_WASH_Y, MINER_WASH_Z))
    {
        if(gMinerWashTimer[playerid] != -1)
        {
            ShowTDN(playerid, NOTIFICATION_WARNING, "Dang rua da, vui long cho.");
            return 1;
        }
        // Single stone on player (/layda flow)
        if(PlayerMinerData[playerid][MP_HasStone])
        {
            ApplyAnimation(playerid, "BD_FIRE", "wash_up", 4.1, 1, 0, 0, 0, 10000, 1);
            gMinerWashTimer[playerid] = 1;
            SetLoading(playerid, 10000, "Dang rua khoang san...", "Miner_WashFinish");
            new string[128];
            format(string, sizeof(string), "{FF8000}* {C2A2DA}%s dang rua khoang san.", GetPlayerNameEx(playerid));
            ProxDetectorWrap(playerid, string, 92, 30.0, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
            return 1;
        }
        // All vehicle stones
        /* new vehicleid = PlayerMinerData[playerid][MP_VehicleID];
        if(vehicleid != INVALID_VEHICLE_ID && IsValidVehicle(vehicleid) && gMinerVehStoneCount[vehicleid] > 0)
        {
            Miner_StartWash(playerid);
            return 1;
        } */
        ShowTDN(playerid, NOTIFICATION_WARNING, "Khong co khoang san de rua.");
        return 1;
    }

    // --- Deposit to vehicle ---
    if(PlayerMinerData[playerid][MP_HasStone])
    {
        new vehicleid2 = PlayerMinerData[playerid][MP_VehicleID];
        if(vehicleid2 != INVALID_VEHICLE_ID && IsValidVehicle(vehicleid2))
        {
            new Float:vx, Float:vy, Float:vz;
            GetVehiclePos(vehicleid2, vx, vy, vz);
            if(IsPlayerInRangeOfPoint(playerid, 5.0, vx, vy, vz))
            {
                Miner_DepositToVehicle(playerid);
                return 1;
            }
        }
        return 1;
    }

    // --- Mine rock ---
    if(!IsLoadingActive(playerid))
    {
        new vehicleid3 = PlayerMinerData[playerid][MP_VehicleID];
        if(vehicleid3 != INVALID_VEHICLE_ID && IsValidVehicle(vehicleid3) && gMinerVehStoneCount[vehicleid3] >= MINER_MAX_VEH_STONES)
        {
            SendClientMessageEx(playerid, COLOR_YELLOW, "{6699CC}[Khai Thac] {ffffff}Xe cua ban da day da. Hay lai den Tram Rua va [/layda] de rua khoang san.");
            ShowTDN(playerid, NOTIFICATION_WARNING, "Khong the dao khoang san.");
            return 1;
        }
        new stone = Miner_FindNearestRock(playerid);
        if(stone != -1)
        {
            Miner_StartMine(playerid, stone);
            return 1;
        }
    }

    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_MINER_NPC)
    {
        if(!response) return 1;
        switch(listitem)
        {
            case 0: Miner_Start(playerid);
            case 1: Miner_Stop(playerid);
            case 2:
            {
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "[================================================================================]");
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "[Khai Thac] Buoc 1: Khai thac khoang san trong khu mo, dat len xe Bobcat (toi da %d vien).", MINER_MAX_VEH_STONES);
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "[Khai Thac] Buoc 2: Lai xe den Tram Rua Khoang san, nhan [Y] de rua da nhan khoang san.");
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "[Khai Thac] Buoc 3: Den NPC ban khoang san, nhan [Y] de ban va nhan tien.");
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "[================================================================================]");
            }
        }
        return 1;
    }

    if(dialogid == DIALOG_MINER_SELL)
    {
        if(!response) return 1;
        Miner_SellType(playerid, listitem);
        return 1;
    }
    return 1;
}
