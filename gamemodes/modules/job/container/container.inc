// ============================================================
//  Container Delivery Job - KP5 Roleplay
// ============================================================

#include <YSI\YSI_Coding\y_hooks>

// ============================================================
//  Du lieu tinh: kho hang va diem giao
// ============================================================

// Model trailer theo loai hang (lau = random trong 3 loai)
static const gCJTrailerModel[3] = {450, 584, 591};

// Toa do 3 vi tri spawn xe tai
static const Float:sCJTruckSpawn[3][4] = {
    {-1641.2336,  96.0517, 3.9461, 315.7069},
    {-1636.3193,  91.9577, 3.9534, 314.9186},
    {-1631.7323,  87.6626, 3.9319, 314.8746}
};

// Kho hang: [X, Y, Z] - pickup = vi tri CP (press Y tai day)
static const Float:sCJWarehousePos[CONTAINER_WAREHOUSE_COUNT][3] = {
    {-1730.718994, 159.652847, 3.554687},   // Diem lay hang 1
    {-1745.883300, 168.859573, 3.549555},   // Diem lay hang 2
    {-1730.159301, 175.208328, 3.554687}    // Diem lay hang 3
};

// Toa do diem giao
static const Float:sCJDeliveryPos[CONTAINER_DELIVERY_COUNT][3] = {
    {  -69.126403, -1130.504882,  1.078125},   // [0] Vat Lieu - Cong trinh LS
    {  586.958435,  1223.589355, 11.718750},   // [1] Vat Lieu - Cong trinh LV
    {  647.083251,  1698.590576,  6.992187},   // [2] Xang - Tram xang LV
    {  148.672439,  -167.256179,  1.578125},   // [3] Xang - Tram xang
    {  240.500289,   -56.941432,  1.578125},   // [4] Quan Ao
    {-2472.754150,  2301.515380,  4.984375},   // [5] Quan Ao
    {-1530.762207,  2630.576904, 55.835937},   // [6] Hang Lau 1
    { -299.213928,  1773.882812, 42.687500},   // [7] Hang Lau 2
    { 1198.196533,   249.226226, 19.554687}    // [8] Hang Lau 3
};

static const sCJDeliveryCargo[CONTAINER_DELIVERY_COUNT]        = {CARGO_VATLIEU, CARGO_VATLIEU, CARGO_XANG, CARGO_XANG, CARGO_QUANAO, CARGO_QUANAO, CARGO_LAU, CARGO_LAU, CARGO_LAU};
static const bool:sCJDeliveryIllegal[CONTAINER_DELIVERY_COUNT] = {false, false, false, false, false, false, true, true, true};

// ============================================================
//  Helper: kiem tra chuoi toan so
// ============================================================
static bool:CJ_IsNumeric(const str[])
{
    if(!str[0]) return false;
    for(new i = 0; str[i]; i++) {
        if(str[i] < '0' || str[i] > '9') return false;
    }
    return true;
}

// ============================================================
//  Helper: lay model xe theo skill
// ============================================================
stock Container_GetTruckModel(playerid)
{
    new s = PlayerInfo[playerid][pContainerSkill];
    if(s >= 600) return TRUCK_LV10;
    if(s >= 190) return TRUCK_LV5;
    return TRUCK_LV1;
}

// ============================================================
//  Helper: lay level job container
// ============================================================
stock Container_GetJobLevel(playerid)
{
    new s = PlayerInfo[playerid][pContainerSkill];
    if(s >= 600) return 10;
    if(s >= 475) return 9;
    if(s >= 365) return 8;
    if(s >= 270) return 7;
    if(s >= 190) return 6;
    if(s >= 125) return 5;
    if(s >= 75)  return 4;
    if(s >= 40)  return 3;
    if(s >= 15)  return 2;
    return 1;
}

// ============================================================
//  Helper: so diem con lai de len level tiep theo
// ============================================================
stock Container_GetSkillNextLevel(playerid)
{
    new s = PlayerInfo[playerid][pContainerSkill];
    static const thresholds[] = {15, 40, 75, 125, 190, 270, 365, 475, 600};
    for(new i = 0; i < sizeof(thresholds); i++) {
        if(s < thresholds[i]) return thresholds[i] - s;
    }
    return 0;
}

// ============================================================
//  Config: load gia tu file
// ============================================================
static CJ_LoadConfig()
{
    gCJPrice[CARGO_VATLIEU] = 800.0;
    gCJPrice[CARGO_XANG]    = 1200.0;
    gCJPrice[CARGO_QUANAO]  = 1000.0;
    gCJPrice[CARGO_LAU]     = 2500.0;

    if(!fexist(CONTAINER_CFG_FILE)) { CJ_SaveConfig(); return; }

    new File:f = fopen(CONTAINER_CFG_FILE, io_read);
    if(!f) return;
    new line[64], key[32], val[16];
    while(fread(f, line)) {
        if(sscanf(line, "p<=>s[32]s[16]", key, val)) continue;
        if(!strcmp(key, "PriceVatLieu", true)) gCJPrice[CARGO_VATLIEU] = floatstr(val);
        else if(!strcmp(key, "PriceXang",   true)) gCJPrice[CARGO_XANG]    = floatstr(val);
        else if(!strcmp(key, "PriceQuanAo", true)) gCJPrice[CARGO_QUANAO]  = floatstr(val);
        else if(!strcmp(key, "PriceLau",    true)) gCJPrice[CARGO_LAU]     = floatstr(val);
    }
    fclose(f);
}

static CJ_SaveConfig()
{
    new File:f = fopen(CONTAINER_CFG_FILE, io_write);
    if(!f) return;
    new line[64];
    format(line, sizeof(line), "PriceVatLieu=%.2f\n", gCJPrice[CARGO_VATLIEU]); fwrite(f, line);
    format(line, sizeof(line), "PriceXang=%.2f\n",    gCJPrice[CARGO_XANG]);    fwrite(f, line);
    format(line, sizeof(line), "PriceQuanAo=%.2f\n",  gCJPrice[CARGO_QUANAO]);  fwrite(f, line);
    format(line, sizeof(line), "PriceLau=%.2f\n",     gCJPrice[CARGO_LAU]);     fwrite(f, line);
    fclose(f);
}

// ============================================================
//  Reset bien player
// ============================================================
stock Container_ResetVars(playerid)
{
    if(IsValidTimer(PlayerCJ[playerid][E_CJ_CountdownTimer]))
        KillTimer(PlayerCJ[playerid][E_CJ_CountdownTimer]);

    DisablePlayerCP(playerid);
    if(gPlayerCheckpointStatus[playerid] >= CHECKPOINT_CONTAINER_WH
    && gPlayerCheckpointStatus[playerid] <= CHECKPOINT_CONTAINER_RET)
        gPlayerCheckpointStatus[playerid] = CHECKPOINT_NONE;

    if(CJVehLabel[playerid] != Text3D:INVALID_3DTEXT_ID) {
        Delete3DTextLabel(CJVehLabel[playerid]);
        CJVehLabel[playerid] = Text3D:INVALID_3DTEXT_ID;
    }
    if(CJTrailerLabel[playerid] != Text3D:INVALID_3DTEXT_ID) {
        Delete3DTextLabel(CJTrailerLabel[playerid]);
        CJTrailerLabel[playerid] = Text3D:INVALID_3DTEXT_ID;
    }
    if(PlayerCJ[playerid][E_CJ_HasCargo]) {
        RemovePlayerAttachedObject(playerid, CONTAINER_CARGO_SLOT);
        SetPlayerSpecialAction(playerid, 0);
    }

    PlayerCJ[playerid][E_CJ_GroupName][0]  = EOS;
    PlayerCJ[playerid][E_CJ_GroupID]       = -1;
    PlayerCJ[playerid][E_CJ_PartnerID]     = INVALID_PLAYER_ID;
    PlayerCJ[playerid][E_CJ_LeaderID]      = INVALID_PLAYER_ID;
    PlayerCJ[playerid][E_CJ_Working]       = false;
    PlayerCJ[playerid][E_CJ_TruckID]       = INVALID_VEHICLE_ID;
    PlayerCJ[playerid][E_CJ_TrailerID]     = INVALID_VEHICLE_ID;
    PlayerCJ[playerid][E_CJ_CargoType]     = -1;
    PlayerCJ[playerid][E_CJ_IsUrgent]      = false;
    PlayerCJ[playerid][E_CJ_CargoCount]    = 0;
    PlayerCJ[playerid][E_CJ_OriginalMax]   = 0;
    PlayerCJ[playerid][E_CJ_WarehouseIdx]  = -1;
    PlayerCJ[playerid][E_CJ_DeliveryIdx]   = -1;
    PlayerCJ[playerid][E_CJ_TimeTotal]     = 0;
    PlayerCJ[playerid][E_CJ_TimeLeft]      = 0;
    PlayerCJ[playerid][E_CJ_HasCargo]      = false;
    PlayerCJ[playerid][E_CJ_Returning]     = false;
    PlayerCJ[playerid][E_CJ_CountdownTimer]= -1;
    PlayerCJ[playerid][E_CJ_DeliveryDist]  = 0.0;
    PlayerWorking[playerid]                = 0;
    DeletePVar(playerid, "CJ_Unloading");
}

// ============================================================
//  Internal: tao checkpoint kho hang
// ============================================================
static CJ_ShowWarehouseCP(playerid, wIdx)
{
    SetPlayerCP(playerid, CJWarehouse[wIdx][CJW_X], CJWarehouse[wIdx][CJW_Y], CJWarehouse[wIdx][CJW_Z], 6.0);
    gPlayerCheckpointStatus[playerid] = CHECKPOINT_CONTAINER_WH;
    PlayerCJ[playerid][E_CJ_WarehouseIdx] = wIdx;
}

// ============================================================
//  Internal: hien CP diem giao hang (chi hang gap - hang thuong dung /diemgiaohang)
// ============================================================
static CJ_ShowDeliveryCP(playerid)
{
    new lid = PlayerCJ[playerid][E_CJ_LeaderID];
    if(lid == INVALID_PLAYER_ID || lid >= MAX_PLAYERS) return;
    new idx = PlayerCJ[lid][E_CJ_DeliveryIdx];
    if(idx < 0 || idx >= CONTAINER_DELIVERY_COUNT) return;

    SetPlayerCP(playerid, CJDelivery[idx][CJD_X], CJDelivery[idx][CJD_Y], CJDelivery[idx][CJD_Z], 8.0);
    gPlayerCheckpointStatus[playerid] = CHECKPOINT_CONTAINER_DEL;
}

// ============================================================
//  Internal: cap nhat label diem lay hang (hien ton kho)
// ============================================================
static CJ_UpdateWarehouseLabel(wIdx)
{
    new wlbl[128];
    format(wlbl, sizeof(wlbl),
        "{F5D000}Diem Lay Hang #%d\n{FFFFFF}An {46BE00}[Y]{FFFFFF} de boc hang\n{FF8800}Ton kho: {FFFFFF}%d kien",
        wIdx + 1, CJWarehouse[wIdx][CJW_Stock]);
    UpdateDynamic3DTextLabelText(CJWarehouseLabel[wIdx], 0xFFFFFFFF, wlbl);
}

// ============================================================
//  Internal: cap nhat 3D label tren trailer (so luong hang)
// ============================================================
static CJ_UpdateTrailerLabel(leaderID)
{
    new tid = PlayerCJ[leaderID][E_CJ_TrailerID];
    if(!IsValidVehicle(tid)) return;
    new maxCargo = (PlayerCJ[leaderID][E_CJ_PartnerID] != INVALID_PLAYER_ID) ? CONTAINER_TEAM_MAX : CONTAINER_SOLO_MAX;
    new lbl[128];
    format(lbl, sizeof(lbl), "{F5D000}Team: {FFFFFF}%s\n{FF8800}So luong: {FFFFFF}%d/%d kien hang",
        PlayerCJ[leaderID][E_CJ_GroupName], PlayerCJ[leaderID][E_CJ_CargoCount], maxCargo);
    if(CJTrailerLabel[leaderID] == Text3D:INVALID_3DTEXT_ID) {
        CJTrailerLabel[leaderID] = Create3DTextLabel(lbl, 0xFFFFFFFF, 0.0, 0.0, 0.0, 15.0, 0);
        Attach3DTextLabelToVehicle(CJTrailerLabel[leaderID], tid, 0.0, 0.0, 0.0);
    } else {
        Update3DTextLabelText(CJTrailerLabel[leaderID], 0xFFFFFFFF, lbl);
    }
}

// ============================================================
//  Internal: cap nhat 3D label tren xe
// ============================================================
static CJ_UpdateLabel(playerid)
{
    new vid = PlayerCJ[playerid][E_CJ_TruckID];
    if(!IsValidVehicle(vid)) return;
    if(CJVehLabel[playerid] != Text3D:INVALID_3DTEXT_ID) Delete3DTextLabel(CJVehLabel[playerid]);

    new lbl[128];
    format(lbl, sizeof(lbl), "{F5D000}Container Delivery\n{FFFFFF}Team: {F5D000}%s", PlayerCJ[playerid][E_CJ_GroupName]);
    CJVehLabel[playerid] = Create3DTextLabel(lbl, 0xFFFFFFFF, 0.0, 0.0, 0.6, 15.0, 0);
    Attach3DTextLabelToVehicle(CJVehLabel[playerid], vid, 0.0, 0.0, 0.6);
}

// ============================================================
//  Internal: chon kho hang ngau nhien con hang
// ============================================================
static CJ_PickWarehouse(minStock = 1)
{
    new avail[CONTAINER_WAREHOUSE_COUNT], count = 0;
    for(new i = 0; i < CONTAINER_WAREHOUSE_COUNT; i++)
        if(CJWarehouse[i][CJW_Stock] >= minStock) avail[count++] = i;
    if(!count) return -1;
    return avail[random(count)];
}

// ============================================================
//  Internal: chon diem giao ngau nhien theo loai hang
// ============================================================
static CJ_PickDelivery(cargoType)
{
    new avail[CONTAINER_DELIVERY_COUNT], count = 0;
    for(new i = 0; i < CONTAINER_DELIVERY_COUNT; i++)
        if(CJDelivery[i][CJD_CargoType] == cargoType) avail[count++] = i;
    if(!count) return -1;
    return avail[random(count)];
}

// ============================================================
//  Internal: khoi dong chuyen hang sau khi chon loai hang
// ============================================================
static CJ_StartDelivery(playerid, cargoType, bool:isUrgent)
{
    new bool:hasPartner = (PlayerCJ[playerid][E_CJ_PartnerID] != INVALID_PLAYER_ID && IsPlayerConnected(PlayerCJ[playerid][E_CJ_PartnerID]));
    new origMax = hasPartner ? CONTAINER_TEAM_MAX : CONTAINER_SOLO_MAX;
    new wIdx = CJ_PickWarehouse(isUrgent ? origMax : 1);
    if(wIdx == -1) {
        if(isUrgent) {
            new msg[128];
            format(msg, sizeof(msg), "Kho hang khong du %d kien cho hang gap! Thu thuong hoac cho kho hang.", origMax);
            ShowTDN(playerid, NOTIFICATION_ERROR, msg);
        } else {
            ShowTDN(playerid, NOTIFICATION_ERROR, "Tat ca kho hang deu het hang! Vui long cho kho phuc hoi.");
        }
        return;
    }

    // Hang gap: chon diem giao ngay. Hang thuong: player chon sau qua /diemgiaohang
    new dIdx = -1;
    if(isUrgent) {
        dIdx = CJ_PickDelivery(cargoType);
        if(dIdx == -1) { ShowTDN(playerid, NOTIFICATION_ERROR, "Khong tim thay diem giao."); return; }
    }

    // Spawn trailer phia sau xe
    new truckID = PlayerCJ[playerid][E_CJ_TruckID];
    new Float:tx, Float:ty, Float:tz, Float:ta;
    GetVehiclePos(truckID, tx, ty, tz);
    GetVehicleZAngle(truckID, ta);
    new trailerModel = (cargoType == CARGO_LAU) ? gCJTrailerModel[random(3)] : gCJTrailerModel[cargoType];
    new Float:ox = tx - (floatsin(ta, degrees) * 9.0);
    new Float:oy = ty - (floatcos(ta, degrees) * 9.0);
    new trailerID = CreateVehicle(trailerModel, ox, oy, tz + 1.0, ta, -1, -1, -1);
    AttachTrailerToVehicle(trailerID, truckID);
    CJTrailerLabel[playerid] = Text3D:INVALID_3DTEXT_ID;

    // Luu trang thai
    PlayerCJ[playerid][E_CJ_TrailerID]    = trailerID;
    PlayerCJ[playerid][E_CJ_CargoType]    = cargoType;
    PlayerCJ[playerid][E_CJ_IsUrgent]     = isUrgent;
    PlayerCJ[playerid][E_CJ_OriginalMax]  = isUrgent ? origMax : 0;
    PlayerCJ[playerid][E_CJ_CargoCount]   = 0;
    PlayerCJ[playerid][E_CJ_DeliveryIdx]  = dIdx;
    PlayerCJ[playerid][E_CJ_DeliveryDist] = 0.0;
    CJ_UpdateTrailerLabel(playerid);

    // Dong bo cho partner
    new partner = PlayerCJ[playerid][E_CJ_PartnerID];
    if(partner != INVALID_PLAYER_ID && IsPlayerConnected(partner)) {
        PlayerCJ[partner][E_CJ_TrailerID]   = trailerID;
        PlayerCJ[partner][E_CJ_CargoType]   = cargoType;
        PlayerCJ[partner][E_CJ_IsUrgent]    = isUrgent;
        PlayerCJ[partner][E_CJ_OriginalMax] = PlayerCJ[playerid][E_CJ_OriginalMax];
        PlayerCJ[partner][E_CJ_DeliveryIdx] = dIdx;
        if(isUrgent) CJ_ShowWarehouseCP(partner, wIdx);
    }
    if(isUrgent) CJ_ShowWarehouseCP(playerid, wIdx);

    // Thong bao
    new cargoName[16];
    switch(cargoType) {
        case CARGO_VATLIEU: cargoName = "Vat Lieu";
        case CARGO_XANG:    cargoName = "Xang Dau";
        case CARGO_QUANAO:  cargoName = "Quan Ao";
        default:            cargoName = "Hang Lau";
    }
    SendClientMessageEx(playerid, COLOR_YELLOW,
        "{F5D000}[Container]{ffffff} Trailer da san! Hang: {FFFF00}%s{FFFFFF} | %s",
        cargoName, isUrgent ? "{FF6B6B}HANG GAP" : "Giao Thuong");
    if(isUrgent)
        SendHintMessage(playerid, "Den kho hang, xuong xe, an {46BE00}[Y]{ffffff} gan diem boc hang de lay, roi an {46BE00}[Y]{ffffff} gan trailer de chat len.");
    else
        SendHintMessage(playerid, "Dung {46BE00}/diemgiaohang{ffffff} de chon diem giao TRUOC, roi den {46BE00}kho bat ky{ffffff} boc hang va an {46BE00}[Y]{ffffff}.");
}

// ============================================================
//  Internal: tinh toan tra tien
// ============================================================
static CJ_PayPlayer(playerid)
{
    new leaderID    = playerid;
    new cargoType   = PlayerCJ[leaderID][E_CJ_CargoType];
    new cargoCount  = PlayerCJ[leaderID][E_CJ_CargoCount];
    new bool:isUrgent = PlayerCJ[leaderID][E_CJ_IsUrgent];
    new partner     = PlayerCJ[leaderID][E_CJ_PartnerID];
    new timeLeft    = PlayerCJ[leaderID][E_CJ_TimeLeft];
    new bool:hasPartner = (partner != INVALID_PLAYER_ID && IsPlayerConnected(partner));

    new fullThreshold = (PlayerCJ[leaderID][E_CJ_OriginalMax] > 0) ? PlayerCJ[leaderID][E_CJ_OriginalMax] : CONTAINER_SOLO_MAX;
    new Float:pay = gCJPrice[cargoType] * float(cargoCount);
    if(cargoCount >= fullThreshold) pay *= 1.10;
    new bool:urgentBonus = (isUrgent && timeLeft > 0);
    if(urgentBonus) pay *= 1.10;
    if(hasPartner)  pay *= 1.15;
    new totalPay = floatround(pay);
    new eachPay  = hasPartner ? (totalPay / 2) : totalPay;

    // Bonus khoang cach: 5000 moi 1000m, doc lap
    new Float:delDist  = PlayerCJ[leaderID][E_CJ_DeliveryDist];
    new distBonus      = floatround((delDist / 1000.0) * 5000.0);

    GivePlayerCash(leaderID, eachPay + distBonus);
    if(hasPartner) GivePlayerCash(partner, eachPay + distBonus);

    // Ten hang
    new cargoName[16];
    switch(cargoType) {
        case CARGO_VATLIEU: cargoName = "Vat Lieu";
        case CARGO_XANG:    cargoName = "Xang Dau";
        case CARGO_QUANAO:  cargoName = "Quan Ao";
        default:            cargoName = "Hang Lau";
    }

    // XP & skill
    new xp = 1 + random(5);
    if(DoubleXP || PlayerInfo[leaderID][pDoubleEXP] > 0) xp *= 2;
    if(hasPartner) {
        xp *= 2;
        GivePlayerXP(partner, xp);
        PlayerInfo[partner][pContainerSkill]++;
        if(DoubleXP || PlayerInfo[partner][pDoubleEXP] > 0) PlayerInfo[partner][pContainerSkill]++;
        new pmsg[256], nfPartnerTotal[20];
        format(nfPartnerTotal, sizeof(nfPartnerTotal), "%s", number_format(eachPay + distBonus));
        format(pmsg, sizeof(pmsg),
            "{F5D000}[Container]{ffffff} Giao hang thanh cong! {3BFF6F}$%s{FFFFFF} | %s | %d kien%s%s%s",
            nfPartnerTotal, cargoName, cargoCount,
            (cargoCount >= fullThreshold) ? " | {F5D000}+Full 10%" : "",
            urgentBonus ? "{FFFFFF} | {FF6B6B}+Gap 10%" : "",
            "{FFFFFF} | {3BFF6F}+Team 15%");
        SendClientMessageEx(partner, COLOR_WHITE, pmsg);
        if(distBonus > 0)
            SendClientMessageEx(partner, COLOR_WHITE,
                "{F5D000}[Container]{ffffff} Chu hang bonus them {3BFF6F}$%s{FFFFFF} vi khoang cach {FFFF00}%dm{FFFFFF} (~%.1fkm)!",
                number_format(distBonus), floatround(delDist), delDist / 1000.0);
        SendClientMessageEx(partner, COLOR_YELLOW,
            "Ky nang: +1 | Level: %d | Con %d diem len cap",
            Container_GetJobLevel(partner), Container_GetSkillNextLevel(partner));
    }
    GivePlayerXP(leaderID, xp);
    PlayerInfo[leaderID][pContainerSkill]++;
    if(DoubleXP || PlayerInfo[leaderID][pDoubleEXP] > 0) PlayerInfo[leaderID][pContainerSkill]++;
    new msg[256];
    new nfLeaderTotal[20];
    format(nfLeaderTotal, sizeof(nfLeaderTotal), "%s", number_format(eachPay + distBonus));
    format(msg, sizeof(msg),
        "{F5D000}[Container]{ffffff} Giao hang thanh cong! {3BFF6F}$%s{FFFFFF} | %s | %d kien%s%s%s",
        nfLeaderTotal, cargoName, cargoCount,
        (cargoCount >= fullThreshold) ? " | {F5D000}+Full 10%" : "",
        urgentBonus ? "{FFFFFF} | {FF6B6B}+Gap 10%" : "",
        hasPartner ? "{FFFFFF} | {3BFF6F}+Team 15%" : "");
    SendClientMessageEx(leaderID, COLOR_WHITE, msg);
    if(distBonus > 0)
        SendClientMessageEx(leaderID, COLOR_WHITE,
            "{F5D000}[Container]{ffffff} Chu hang bonus them {3BFF6F}$%s{FFFFFF} vi khoang cach {FFFF00}%dm{FFFFFF} (~%.1fkm)!",
            number_format(distBonus), floatround(delDist), delDist / 1000.0);
    SendClientMessageEx(leaderID, COLOR_YELLOW,
        "Ky nang: +1 | Level: %d | Con %d diem len cap",
        Container_GetJobLevel(leaderID), Container_GetSkillNextLevel(leaderID));

    // Discord log chi tiet
    {
        new dIdx = PlayerCJ[leaderID][E_CJ_DeliveryIdx];

        // Tinh rieng tung khoan bonus de hien thi
        new Float:logBase        = gCJPrice[cargoType] * float(cargoCount);
        new Float:logAfterFull   = (cargoCount >= fullThreshold) ? (logBase   * 1.10) : logBase;
        new Float:logAfterUrgent = urgentBonus                   ? (logAfterFull * 1.10) : logAfterFull;
        new Float:logAfterTeam   = hasPartner                    ? (logAfterUrgent * 1.15) : logAfterUrgent;

        new bonusPart[256];
        bonusPart[0] = EOS;
        new tmp[96];
        new nfBuf[20]; // pre-stored number_format result per line
        if(cargoCount >= fullThreshold) {
            format(nfBuf, sizeof(nfBuf), "%s", number_format(floatround(logAfterFull - logBase)));
            format(tmp, sizeof(tmp), "\n  +Full hàng +10%%: $%s", nfBuf);
            strcat(bonusPart, tmp);
        }
        if(urgentBonus) {
            format(nfBuf, sizeof(nfBuf), "%s", number_format(floatround(logAfterUrgent - logAfterFull)));
            format(tmp, sizeof(tmp), "\n  +Hàng gấp +10%%: $%s", nfBuf);
            strcat(bonusPart, tmp);
        }
        if(hasPartner) {
            format(nfBuf, sizeof(nfBuf), "%s", number_format(floatround(logAfterTeam - logAfterUrgent)));
            format(tmp, sizeof(tmp), "\n  +Team +15%%: $%s", nfBuf);
            strcat(bonusPart, tmp);
        }
        if(distBonus > 0) {
            format(nfBuf, sizeof(nfBuf), "%s", number_format(distBonus));
            format(tmp, sizeof(tmp), "\n  +Khoảng cách (~%.1fkm): $%s", delDist / 1000.0, nfBuf);
            strcat(bonusPart, tmp);
        }
        if(bonusPart[0] == EOS) strcat(bonusPart, "\n  Không có bonus");

        new partnerStr[52];
        if(hasPartner) format(partnerStr, sizeof(partnerStr), " + %s (ID:%d)", GetPlayerNameEx(partner), partner);
        else           format(partnerStr, sizeof(partnerStr), " (Solo)");

        new dCargoName[20];
        switch(cargoType) {
            case CARGO_VATLIEU: dCargoName = "Vật Liệu";
            case CARGO_XANG:    dCargoName = "Xăng Dầu";
            case CARGO_QUANAO:  dCargoName = "Quần Áo";
            default:            dCargoName = "Hàng Lậu";
        }

        // Pre-store to avoid static buffer reuse when two number_format calls appear in one format()
        new nfBase2[20], nfTotal2[20];
        format(nfBase2, sizeof(nfBase2), "%s", number_format(floatround(logBase)));
        format(nfTotal2, sizeof(nfTotal2), "%s", number_format(eachPay + distBonus));

        new etitle[128], estr[512];
        format(etitle, sizeof(etitle), "Container | %s - %s", PlayerCJ[leaderID][E_CJ_GroupName], GetPlayerNameEx(leaderID));
        format(estr, sizeof(estr),
            ":truck: **GIAO HÀNG THÀNH CÔNG**\n**Nhóm:** %s\n**Lái xe:** %s (ID:%d)%s\n**Hàng:** %s | %d kiện\n**Điểm giao:** %s\n**Giá cơ bản:** $%s\n**Bonus:**%s\n**TỔNG/người:** $%s",
            PlayerCJ[leaderID][E_CJ_GroupName],
            GetPlayerNameEx(leaderID), leaderID, partnerStr,
            dCargoName, cargoCount,
            CJDelivery[dIdx][CJD_Name],
            nfBase2,
            bonusPart,
            nfTotal2);
        new cjY, cjMo, cjD, cjH, cjMi, cjS, cjDtStr[64];
        getdate(cjY, cjMo, cjD); gettime(cjH, cjMi, cjS);
        format(cjDtStr, sizeof(cjDtStr), "%02d/%02d/%d - %02d:%02d:%02d", cjD, cjMo, cjY, cjH, cjMi, cjS);
        new DCC_Embed:cjEmbed = DCC_CreateEmbed();
        DCC_SetEmbedTitle(cjEmbed, etitle);
        DCC_SetEmbedDescription(cjEmbed, estr);
        DCC_SetEmbedColor(cjEmbed, 0x85FF85);
        DCC_SetEmbedFooter(cjEmbed, cjDtStr);
        DCC_SendChannelEmbedMessage(dclog_Container, cjEmbed);
    }
}

// ============================================================
//  Timer: dem nguoc hang gap
// ============================================================
forward CJ_Countdown(playerid);
public CJ_Countdown(playerid)
{
    if(!IsPlayerConnected(playerid) || !IsContainerWorking(playerid)) return 1;
    if(PlayerCJ[playerid][E_CJ_TimeLeft] <= 0) {
        PlayerCJ[playerid][E_CJ_TimeLeft] = 0;
        KillTimer(PlayerCJ[playerid][E_CJ_CountdownTimer]);
        PlayerCJ[playerid][E_CJ_CountdownTimer] = -1;
        SendMessageTD(playerid, 4000, "~r~HET THOI GIAN! Mat bonus hang gap.");
        return 1;
    }
    PlayerCJ[playerid][E_CJ_TimeLeft]--;
    new mins = PlayerCJ[playerid][E_CJ_TimeLeft] / 60;
    new secs = PlayerCJ[playerid][E_CJ_TimeLeft] % 60;
    new cj_ptner = PlayerCJ[playerid][E_CJ_PartnerID];
    if(cj_ptner != INVALID_PLAYER_ID && IsPlayerConnected(cj_ptner))
        SendMessageTD(cj_ptner, 1100, "~y~HANG GAP ~w~- Con lai: ~r~%02d:%02d", mins, secs);
    else
        SendMessageTD(playerid, 1100, "~y~HANG GAP ~w~- Con lai: ~r~%02d:%02d", mins, secs);
    return 1;
}

// ============================================================
//  Timer: respawn kho hang
// ============================================================
forward CJ_WarehouseRespawn(warehouseIdx);
public CJ_WarehouseRespawn(warehouseIdx)
{
    CJWarehouse[warehouseIdx][CJW_Stock]   = CJWarehouse[warehouseIdx][CJW_MaxStock];
    CJWarehouse[warehouseIdx][CJW_Respawn] = false;
    CJ_UpdateWarehouseLabel(warehouseIdx);
    printf("[Container] Kho hang #%d da duoc cap lai (%d kien).", warehouseIdx + 1, CJWarehouse[warehouseIdx][CJW_MaxStock]);
    return 1;
}

// ============================================================
//  Timer: hoan tat do hang (sau animation 5 giay)
// ============================================================
forward CJ_DeliveryFinish(playerid);
public CJ_DeliveryFinish(playerid)
{
    if(!IsPlayerConnected(playerid) || !IsContainerWorking(playerid)) return 1;
    if(PlayerCJ[playerid][E_CJ_CargoCount] <= 0) return 1; // double-call guard

    new cj_partner = PlayerCJ[playerid][E_CJ_PartnerID];
    DeletePVar(playerid, "CJ_Unloading");
    if(cj_partner != INVALID_PLAYER_ID && IsPlayerConnected(cj_partner)) {
        DeletePVar(cj_partner, "CJ_Unloading");
        ClearAnimations(cj_partner);
    }
    ClearAnimations(playerid);

    // Dung countdown
    if(IsValidTimer(PlayerCJ[playerid][E_CJ_CountdownTimer])) {
        KillTimer(PlayerCJ[playerid][E_CJ_CountdownTimer]);
        PlayerCJ[playerid][E_CJ_CountdownTimer] = -1;
    }

    // Tra tien (chi tu leader)
    CJ_PayPlayer(playerid);

    // Destroy trailer label + trailer
    if(CJTrailerLabel[playerid] != Text3D:INVALID_3DTEXT_ID) {
        Delete3DTextLabel(CJTrailerLabel[playerid]);
        CJTrailerLabel[playerid] = Text3D:INVALID_3DTEXT_ID;
    }
    new tid = PlayerCJ[playerid][E_CJ_TrailerID];
    if(IsValidVehicle(tid)) DestroyVehicle(tid);
    PlayerCJ[playerid][E_CJ_TrailerID] = INVALID_VEHICLE_ID;

    // Set return CP cho leader; don dep CP giao hang
    DisablePlayerCP(playerid);
    SetPlayerCP(playerid, CJ_RETURN_X, CJ_RETURN_Y, CJ_RETURN_Z, 6.0);
    gPlayerCheckpointStatus[playerid] = CHECKPOINT_CONTAINER_RET;
    PlayerCJ[playerid][E_CJ_Returning]  = true;
    PlayerCJ[playerid][E_CJ_CargoCount] = 0;

    // Thong bao partner (partner theo leader lai xe ve, khong co return CP rieng)
    new partner = PlayerCJ[playerid][E_CJ_PartnerID];
    if(partner != INVALID_PLAYER_ID && IsPlayerConnected(partner)) {
        PlayerCJ[partner][E_CJ_TrailerID]  = INVALID_VEHICLE_ID;
        PlayerCJ[partner][E_CJ_Returning]  = true;
        PlayerCJ[partner][E_CJ_CargoCount] = 0;
        DisablePlayerCP(partner);
        if(gPlayerCheckpointStatus[partner] >= CHECKPOINT_CONTAINER_WH
        && gPlayerCheckpointStatus[partner] <= CHECKPOINT_CONTAINER_RET)
            gPlayerCheckpointStatus[partner] = CHECKPOINT_NONE;
        SendClientMessageEx(partner, COLOR_YELLOW,
            "{F5D000}[Container]{ffffff} Hang giao xong! Ngoi tren xe, leader se lai ve diem xuat phat.");
    }

    SendClientMessageEx(playerid, COLOR_YELLOW,
        "{F5D000}[Container]{ffffff} Hang giao xong! Lai xe ve diem xuat phat (checkpoint tren ban do).");
    SendHintMessage(playerid, "Lai xe ve checkpoint xuat phat de ket thuc chuyen hang.");
    return 1;
}

// ============================================================
//  Hook: khoi tao
// ============================================================
hook OnGameModeInit()
{
    for(new i = 0; i < CONTAINER_WAREHOUSE_COUNT; i++) {
        new Float:wx = sCJWarehousePos[i][0], Float:wy = sCJWarehousePos[i][1], Float:wz = sCJWarehousePos[i][2];
        CJWarehouse[i][CJW_X]        = wx;
        CJWarehouse[i][CJW_Y]        = wy;
        CJWarehouse[i][CJW_Z]        = wz;
        CJWarehouse[i][CJW_Stock]          = CONTAINER_STOCK_DEFAULT;
        CJWarehouse[i][CJW_MaxStock]       = CONTAINER_STOCK_DEFAULT;
        CJWarehouse[i][CJW_Respawn] = false;

        CJWarehousePickup[i] = CreateDynamicPickup(19134, 1, wx, wy, wz);
        new wlbl[128];
        format(wlbl, sizeof(wlbl),
            "{F5D000}Diem Lay Hang #%d\n{FFFFFF}An {46BE00}[Y]{FFFFFF} de boc hang\n{FF8800}Ton kho: {FFFFFF}%d kien",
            i + 1, CONTAINER_STOCK_DEFAULT);
        CJWarehouseLabel[i] = CreateDynamic3DTextLabel(wlbl, 0xFFFFFFFF, wx, wy, wz + 1.2, 12.0);
    }
    for(new i = 0; i < CONTAINER_DELIVERY_COUNT; i++) {
        CJDelivery[i][CJD_X]         = sCJDeliveryPos[i][0];
        CJDelivery[i][CJD_Y]         = sCJDeliveryPos[i][1];
        CJDelivery[i][CJD_Z]         = sCJDeliveryPos[i][2];
        CJDelivery[i][CJD_CargoType] = sCJDeliveryCargo[i];
        CJDelivery[i][CJD_IsIllegal] = sCJDeliveryIllegal[i];
        new cPrefix[12], szZone[32];
        switch(sCJDeliveryCargo[i]) {
            case CARGO_VATLIEU: cPrefix = "Vat Lieu";
            case CARGO_XANG:    cPrefix = "Xang Dau";
            case CARGO_QUANAO:  cPrefix = "Quan Ao";
            default:            cPrefix = "Hang Lau";
        }
        Get3DZone(sCJDeliveryPos[i][0], sCJDeliveryPos[i][1], sCJDeliveryPos[i][2], szZone, sizeof(szZone));
        format(CJDelivery[i][CJD_Name], 32, "%s - %s", cPrefix, szZone);
    }
    CJ_LoadConfig();
    gContainerLocked = 0;
    print("[Container] Job initialized.");
    return 1;
}

// ============================================================
//  Hook: connect
// ============================================================
hook OnPlayerConnect(playerid)
{
    CJVehLabel[playerid]    = Text3D:INVALID_3DTEXT_ID;
    CJTrailerLabel[playerid]= Text3D:INVALID_3DTEXT_ID;
    Container_ResetVars(playerid);
    return 1;
}

// ============================================================
//  Hook: disconnect
// ============================================================
hook OnPlayerDisconnect(playerid, reason)
{
    // Invitee thoat game khi dang co loi moi → unblock leader
    new invGroupID = GetPVarInt(playerid, "CJ_InviteGroupID");
    if(invGroupID) {
        foreach(new _li : Player) if(IsPlayerConnected(_li) && PlayerCJ[_li][E_CJ_GroupID] == invGroupID) {
            DeletePVar(_li, "CJ_PendingInvite");
            SendClientMessageEx(_li, COLOR_YELLOW, "{F5D000}[Container]{ffffff} Nguoi duoc moi da thoat game. Loi moi da bi huy.");
            break;
        }
    }

    if(!IsContainerWorking(playerid)) return 1;

    new partner = PlayerCJ[playerid][E_CJ_PartnerID];
    if(partner != INVALID_PLAYER_ID && IsPlayerConnected(partner)) {
        if(PlayerCJ[playerid][E_CJ_LeaderID] == playerid) {
            // LEADER DC → promote partner
            new vid = PlayerCJ[playerid][E_CJ_TruckID];
            PlayerCJ[partner][E_CJ_TruckID]  = vid;
            PlayerCJ[partner][E_CJ_LeaderID] = partner;
            PlayerCJ[partner][E_CJ_PartnerID]= INVALID_PLAYER_ID;
            PlayerCJ[playerid][E_CJ_TruckID] = INVALID_VEHICLE_ID;
            if(PlayerCJ[playerid][E_CJ_IsUrgent]) {
                if(IsValidTimer(PlayerCJ[playerid][E_CJ_CountdownTimer]))
                    KillTimer(PlayerCJ[playerid][E_CJ_CountdownTimer]);
                PlayerCJ[partner][E_CJ_IsUrgent] = false;
                SendClientMessageEx(partner, COLOR_YELLOW, "{F5D000}[Container]{ffffff} Leader thoat. Bonus hang gap bi huy.");
            }
            // Bug 3: neu leader thoat khi dang tro ve, cap return CP cho partner
            if(PlayerCJ[playerid][E_CJ_Returning]) {
                SetPlayerCP(partner, CJ_RETURN_X, CJ_RETURN_Y, CJ_RETURN_Z, 6.0);
                gPlayerCheckpointStatus[partner] = CHECKPOINT_CONTAINER_RET;
                SendClientMessageEx(partner, COLOR_YELLOW, "{F5D000}[Container]{ffffff} Leader thoat. Ban la leader moi. Lai xe ve checkpoint de ket thuc.");
            } else {
                SendClientMessageEx(partner, COLOR_YELLOW, "{F5D000}[Container]{ffffff} Leader thoat. Ban tro thanh leader.");
            }
        } else {
            // PARTNER (non-leader) DC → update leader
            PlayerCJ[partner][E_CJ_PartnerID] = INVALID_PLAYER_ID;
            // Bug 4: neu dang hang gap, huy urgent va chuyen sang giao binh thuong
            if(IsContainerWorking(partner) && PlayerCJ[partner][E_CJ_IsUrgent]) {
                PlayerCJ[partner][E_CJ_IsUrgent] = false;
                if(IsValidTimer(PlayerCJ[partner][E_CJ_CountdownTimer])) {
                    KillTimer(PlayerCJ[partner][E_CJ_CountdownTimer]);
                    PlayerCJ[partner][E_CJ_CountdownTimer] = -1;
                }
                new dIdx = PlayerCJ[partner][E_CJ_DeliveryIdx];
                if(PlayerCJ[partner][E_CJ_CargoCount] > 0 && dIdx >= 0) {
                    SetPlayerCP(partner, CJDelivery[dIdx][CJD_X], CJDelivery[dIdx][CJD_Y], CJDelivery[dIdx][CJD_Z], 8.0);
                    gPlayerCheckpointStatus[partner] = CHECKPOINT_CONTAINER_DEL;
                    SendClientMessageEx(partner, COLOR_YELLOW,
                        "{F5D000}[Container]{ffffff} Thanh vien thoat. Hang gap bi huy - giao %d kien binh thuong tai {FFFF00}%s{FFFFFF}!",
                        PlayerCJ[partner][E_CJ_CargoCount], CJDelivery[dIdx][CJD_Name]);
                } else {
                    SendClientMessageEx(partner, COLOR_YELLOW,
                        "{F5D000}[Container]{ffffff} Thanh vien thoat. Hang gap bi huy. Tiep tuc boc hang va dung /diemgiaohang de giao.");
                }
            } else {
                SendClientMessageEx(partner, COLOR_YELLOW, "{F5D000}[Container]{ffffff} Thanh vien nhom da thoat.");
            }
        }
    } else if(PlayerCJ[playerid][E_CJ_LeaderID] == playerid) {
        new vid = PlayerCJ[playerid][E_CJ_TruckID];
        if(IsValidVehicle(vid)) DestroyVehicle(vid);
        new tid = PlayerCJ[playerid][E_CJ_TrailerID];
        if(IsValidVehicle(tid)) DestroyVehicle(tid);
    }
    Container_ResetVars(playerid);
    return 1;
}

// ============================================================
//  Hook: xe bi respawn
// ============================================================
hook OnVehicleSpawn(vehicleid)
{
    foreach(new i : Player) if(IsPlayerConnected(i)) {
        if(PlayerCJ[i][E_CJ_TruckID] == vehicleid && PlayerCJ[i][E_CJ_LeaderID] == i) {
            if(CJVehLabel[i] != Text3D:INVALID_3DTEXT_ID) { Delete3DTextLabel(CJVehLabel[i]); CJVehLabel[i] = Text3D:INVALID_3DTEXT_ID; }
            DestroyVehicle(vehicleid);
            PlayerCJ[i][E_CJ_TruckID] = INVALID_VEHICLE_ID;
            SendClientMessageEx(i, COLOR_RED, "{F5D000}[Container]{ffffff} Xe cong viec bi thu hoi! Dung /dunglamviec roi bat dau lai.");
        }
    }
    return 1;
}

// ============================================================
//  Hook: nhan phim Y
// ============================================================
hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if(!PRESSED(KEY_YES)) return 1;
    if(!IsContainerWorking(playerid)) return 1;
    if(IsPlayerInAnyVehicle(playerid)) return 1;
    if(GetPVarInt(playerid, "Injured") == 1) return 1;

    new leaderID = PlayerCJ[playerid][E_CJ_LeaderID];
    if(leaderID == INVALID_PLAYER_ID) return 1;

    new cargoCount = PlayerCJ[leaderID][E_CJ_CargoCount];
    new maxCargo   = (PlayerCJ[leaderID][E_CJ_PartnerID] != INVALID_PLAYER_ID) ? CONTAINER_TEAM_MAX : CONTAINER_SOLO_MAX;

    // --- Y gan trailer: chat hang len ---
    new trailerID = PlayerCJ[leaderID][E_CJ_TrailerID];
    if(IsValidVehicle(trailerID) && PlayerCJ[playerid][E_CJ_HasCargo])
    {
        new Float:tvx, Float:tvy, Float:tvz;
        GetVehiclePos(trailerID, tvx, tvy, tvz);
        if(IsPlayerInRangeOfPoint(playerid, 5.0, tvx, tvy, tvz))
        {
            if(cargoCount >= maxCargo) {
                ShowTDN(playerid, NOTIFICATION_WARNING, "Xe da day hang!");
                return 1;
            }
            new wIdx = PlayerCJ[leaderID][E_CJ_WarehouseIdx];

            // Chat len trailer
            PlayerCJ[playerid][E_CJ_HasCargo] = false;
            RemovePlayerAttachedObject(playerid, CONTAINER_CARGO_SLOT);
            SetPlayerSpecialAction(playerid, 0);
            ApplyAnimation(playerid, "CARRY", "PUTDWN", 4.0, 0, 0, 0, 0, 0);
            CJWarehouse[wIdx][CJW_Stock]--;
            CJ_UpdateWarehouseLabel(wIdx);
            PlayerCJ[leaderID][E_CJ_CargoCount]++;
            CJ_UpdateTrailerLabel(leaderID);
            new newCount = PlayerCJ[leaderID][E_CJ_CargoCount];
            SendMessageTD(playerid, 2500, "~g~+1 kien hang - Xe: ~y~%d/%d", newCount, maxCargo);

            // Kich hoat respawn neu kho het (dedup: chi set 1 timer)
            if(CJWarehouse[wIdx][CJW_Stock] <= 0 && !CJWarehouse[wIdx][CJW_Respawn]) {
                CJWarehouse[wIdx][CJW_Respawn] = true;
                SetTimerEx("CJ_WarehouseRespawn", CONTAINER_RESPAWN_MS, false, "i", wIdx);
            }

            // Kiem tra dieu kien hien delivery CP
            new bool:isUrgent = PlayerCJ[leaderID][E_CJ_IsUrgent];
            new urgentThreshold = (PlayerCJ[leaderID][E_CJ_OriginalMax] > 0) ? PlayerCJ[leaderID][E_CJ_OriginalMax] : maxCargo;
            if(isUrgent && newCount >= urgentThreshold)
            {
                // Hang gap: day kien -> hien CP + bat dau dem nguoc
                CJ_ShowDeliveryCP(leaderID);
                if(leaderID != playerid) CJ_ShowDeliveryCP(playerid);

                new dIdx = PlayerCJ[leaderID][E_CJ_DeliveryIdx];
                new Float:tvx, Float:tvy, Float:tvz;
                GetVehiclePos(PlayerCJ[leaderID][E_CJ_TruckID], tvx, tvy, tvz);
                PlayerCJ[leaderID][E_CJ_DeliveryDist] = floatsqroot(
                    floatpower(tvx - CJDelivery[dIdx][CJD_X], 2.0) +
                    floatpower(tvy - CJDelivery[dIdx][CJD_Y], 2.0));
                new dist = floatround(PlayerCJ[leaderID][E_CJ_DeliveryDist]);
                new totalTime = dist / 10;
                PlayerCJ[leaderID][E_CJ_TimeTotal] = totalTime;
                PlayerCJ[leaderID][E_CJ_TimeLeft]  = totalTime;
                PlayerCJ[leaderID][E_CJ_CountdownTimer] = SetTimerEx("CJ_Countdown", 1000, true, "i", leaderID);

                {
                    new cj_hm = totalTime / 60, cj_hs = totalTime % 60;
                    SendClientMessageEx(leaderID, 0xF08854FF,
                        "[?]{FFFFFF} Day hang! Giao gap trong {FF6B6B}%02d:%02d{FFFFFF}. Len xe ngay! (Dem nguoc hien cho dong doi)", cj_hm, cj_hs);
                    new cj_ptnr = PlayerCJ[leaderID][E_CJ_PartnerID];
                    if(cj_ptnr != INVALID_PLAYER_ID && IsPlayerConnected(cj_ptnr))
                        SendClientMessageEx(cj_ptnr, 0xF08854FF,
                            "[?]{FFFFFF} Day hang! Dem nguoc {FF6B6B}%02d:%02d{FFFFFF} dang hien tren man hinh ban.", cj_hm, cj_hs);
                }
                {
                    new cname_u[20];
                    switch(PlayerCJ[leaderID][E_CJ_CargoType]) {
                        case CARGO_VATLIEU: cname_u = "Vật Liệu";
                        case CARGO_XANG:    cname_u = "Xăng Dầu";
                        case CARGO_QUANAO:  cname_u = "Quần Áo";
                        default:            cname_u = "Hàng Lậu";
                    }
                    new ptner_u = PlayerCJ[leaderID][E_CJ_PartnerID];
                    new ptnerName_u[MAX_PLAYER_NAME + 1] = "Solo";
                    if(ptner_u != INVALID_PLAYER_ID && IsPlayerConnected(ptner_u))
                        format(ptnerName_u, sizeof(ptnerName_u), "%s", GetPlayerNameEx(ptner_u));
                    new etitle_u[128], estr_u[384];
                    format(etitle_u, sizeof(etitle_u), "Container - Đầy hàng gấp | %s", PlayerCJ[leaderID][E_CJ_GroupName]);
                    format(estr_u, sizeof(estr_u),
                        ":package: **CONTAINER ĐẦY HÀNG (GẤP)**\n**Nhóm:** %s\n**Loại hàng:** %s | %d kiện\n**Điểm giao:** %s\n**Trưởng nhóm:** %s (ID:%d)\n**Thành viên:** %s\n**Thời gian giao:** %ds",
                        PlayerCJ[leaderID][E_CJ_GroupName], cname_u, newCount,
                        CJDelivery[dIdx][CJD_Name],
                        GetPlayerNameEx(leaderID), leaderID,
                        ptnerName_u, totalTime);
                    new uY, uMo, uD, uH, uMi, uS, uDtStr[64];
                    getdate(uY, uMo, uD); gettime(uH, uMi, uS);
                    format(uDtStr, sizeof(uDtStr), "%02d/%02d/%d - %02d:%02d:%02d", uD, uMo, uY, uH, uMi, uS);
                    new DCC_Embed:uEmbed = DCC_CreateEmbed();
                    DCC_SetEmbedTitle(uEmbed, etitle_u);
                    DCC_SetEmbedDescription(uEmbed, estr_u);
                    DCC_SetEmbedColor(uEmbed, 0x85FF85);
                    DCC_SetEmbedFooter(uEmbed, uDtStr);
                    DCC_SendChannelEmbedMessage(dclog_Container, uEmbed);
                }
            }
            else if(!isUrgent && newCount == maxCargo)
            {
                SendClientMessageEx(leaderID, COLOR_YELLOW,
                    "{F5D000}[Container]{ffffff} Xe day hang (%d/%d kien)! Len xe di giao ngay!", newCount, maxCargo);
                if(leaderID != playerid)
                    SendClientMessageEx(playerid, COLOR_YELLOW,
                        "{F5D000}[Container]{ffffff} Xe day hang! Leader len xe di giao!", maxCargo);
                {
                    new cname_r[20];
                    switch(PlayerCJ[leaderID][E_CJ_CargoType]) {
                        case CARGO_VATLIEU: cname_r = "Vật Liệu";
                        case CARGO_XANG:    cname_r = "Xăng Dầu";
                        case CARGO_QUANAO:  cname_r = "Quần Áo";
                        default:            cname_r = "Hàng Lậu";
                    }
                    new ptner_r = PlayerCJ[leaderID][E_CJ_PartnerID];
                    new rdIdx   = PlayerCJ[leaderID][E_CJ_DeliveryIdx];
                    new delName_r[32];
                    if(rdIdx >= 0) format(delName_r, sizeof(delName_r), "%s", CJDelivery[rdIdx][CJD_Name]);
                    else           delName_r = "Chưa chọn";
                    new ptnerName_r[MAX_PLAYER_NAME + 1] = "Solo";
                    if(ptner_r != INVALID_PLAYER_ID && IsPlayerConnected(ptner_r))
                        format(ptnerName_r, sizeof(ptnerName_r), "%s", GetPlayerNameEx(ptner_r));
                    new etitle_r[128], estr_r[384];
                    format(etitle_r, sizeof(etitle_r), "Container - Đầy hàng thường | %s", PlayerCJ[leaderID][E_CJ_GroupName]);
                    format(estr_r, sizeof(estr_r),
                        ":package: **CONTAINER ĐẦY HÀNG**\n**Nhóm:** %s\n**Loại hàng:** %s | %d/%d kiện\n**Điểm giao:** %s\n**Trưởng nhóm:** %s (ID:%d)\n**Thành viên:** %s",
                        PlayerCJ[leaderID][E_CJ_GroupName], cname_r, newCount, maxCargo,
                        delName_r, GetPlayerNameEx(leaderID), leaderID, ptnerName_r);
                    new rY, rMo, rD, rH, rMi, rS, rDtStr[64];
                    getdate(rY, rMo, rD); gettime(rH, rMi, rS);
                    format(rDtStr, sizeof(rDtStr), "%02d/%02d/%d - %02d:%02d:%02d", rD, rMo, rY, rH, rMi, rS);
                    new DCC_Embed:rEmbed = DCC_CreateEmbed();
                    DCC_SetEmbedTitle(rEmbed, etitle_r);
                    DCC_SetEmbedDescription(rEmbed, estr_r);
                    DCC_SetEmbedColor(rEmbed, 0x85FF85);
                    DCC_SetEmbedFooter(rEmbed, rDtStr);
                    DCC_SendChannelEmbedMessage(dclog_Container, rEmbed);
                }
            }
            else if(!isUrgent && newCount == 1)
            {
                // Hang thuong: thong bao player co the di giao (CP do /diemgiaohang quyet dinh)
                SendClientMessageEx(leaderID, COLOR_WHITE,
                    "{F5D000}[Container]{ffffff} Co hang roi (%d kien). Tiep tuc boc them hoac len xe di giao!", newCount);
                if(leaderID != playerid)
                    SendClientMessageEx(playerid, COLOR_WHITE, "{F5D000}[Container]{ffffff} Co hang roi. Leader len xe di giao!");
            }
            return 1;
        }
    }

    // --- Y gan diem boc hang kho: lay hang ---
    if(!PlayerCJ[playerid][E_CJ_HasCargo] && trailerID != INVALID_VEHICLE_ID)
    {
        if(cargoCount >= maxCargo) {
            ShowTDN(playerid, NOTIFICATION_WARNING, "Da chat day hang roi!");
            return 1;
        }
        // Hang thuong: can chon diem giao truoc moi duoc boc hang
        if(!PlayerCJ[leaderID][E_CJ_IsUrgent] && PlayerCJ[leaderID][E_CJ_DeliveryIdx] == -1) {
            ShowTDN(playerid, NOTIFICATION_WARNING, "Chon diem giao hang truoc! (/diemgiaohang)");
            return 1;
        }

        // Tim kho gan player: hang gap chi dung kho duoc chi dinh, hang thuong quet ca 3 kho
        new nearWIdx = -1;
        new assignedW = PlayerCJ[leaderID][E_CJ_WarehouseIdx];
        if(assignedW >= 0) {
            if(IsPlayerInRangeOfPoint(playerid, 3.0, CJWarehouse[assignedW][CJW_X], CJWarehouse[assignedW][CJW_Y], CJWarehouse[assignedW][CJW_Z]))
                nearWIdx = assignedW;
        } else {
            for(new _w = 0; _w < CONTAINER_WAREHOUSE_COUNT; _w++) {
                if(IsPlayerInRangeOfPoint(playerid, 3.0, CJWarehouse[_w][CJW_X], CJWarehouse[_w][CJW_Y], CJWarehouse[_w][CJW_Z])) {
                    nearWIdx = _w;
                    break;
                }
            }
        }

        if(nearWIdx >= 0)
        {
            if(CJWarehouse[nearWIdx][CJW_Stock] <= 0) {
                ShowTDN(playerid, NOTIFICATION_ERROR, "Kho nay het hang! Thu sang kho khac.");
                return 1;
            }
            // Cap nhat kho dang boc cho leader va partner
            PlayerCJ[leaderID][E_CJ_WarehouseIdx] = nearWIdx;
            new _p = PlayerCJ[leaderID][E_CJ_PartnerID];
            if(_p != INVALID_PLAYER_ID && IsPlayerConnected(_p))
                PlayerCJ[_p][E_CJ_WarehouseIdx] = nearWIdx;
            ApplyAnimation(playerid, "CARRY", "LIFTUP", 4.0, 0, 0, 0, 0, 0);
            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
            SetPlayerAttachedObject(playerid, CONTAINER_CARGO_SLOT, CONTAINER_CARGO_OBJ, 1,
                0.299822, 0.663485, -0.036337, 184.311355, 90.004089, 0.0, 1.0, 1.0, 1.0);
            PlayerCJ[playerid][E_CJ_HasCargo] = true;
            SendHintMessage(playerid, "Dang cam hang. Lai gan trailer va an {46BE00}[Y]{ffffff} de chat len.");
            return 1;
        }
    }
    return 1;
}

// ============================================================
//  Hook: vao checkpoint (thay the OnPlayerEnterDynamicCP)
// ============================================================
hook OnPlayerEnterCheckpoint(playerid)
{
    if(!IsContainerWorking(playerid)) return 1;
    new leaderID = PlayerCJ[playerid][E_CJ_LeaderID];
    if(leaderID == INVALID_PLAYER_ID) return 1;

    switch(gPlayerCheckpointStatus[playerid])
    {
        case CHECKPOINT_CONTAINER_WH:
        {
            new wIdx = PlayerCJ[leaderID][E_CJ_WarehouseIdx];
            if(wIdx < 0 || wIdx >= CONTAINER_WAREHOUSE_COUNT) return 1;
            new maxCargo = (PlayerCJ[leaderID][E_CJ_PartnerID] != INVALID_PLAYER_ID) ? CONTAINER_TEAM_MAX : CONTAINER_SOLO_MAX;
            SendClientMessageEx(playerid, COLOR_WHITE,
                "{F5D000}[Container]{ffffff} Kho #%d | Ton kho: {FFFF00}%d{FFFFFF} kien | Con boc: %d kien",
                wIdx + 1, CJWarehouse[wIdx][CJW_Stock], maxCargo - PlayerCJ[leaderID][E_CJ_CargoCount]);
            SendHintMessage(playerid, "Xuong xe, di den pickup (diem sang tren minimap) va an [Y] de boc hang.");
            DisablePlayerCP(playerid);
            gPlayerCheckpointStatus[playerid] = CHECKPOINT_NONE;
        }
        case CHECKPOINT_CONTAINER_DEL:
        {
            new cargoCount = PlayerCJ[leaderID][E_CJ_CargoCount];
            new bool:isUrgent = PlayerCJ[leaderID][E_CJ_IsUrgent];
            new urgentMax = (PlayerCJ[leaderID][E_CJ_OriginalMax] > 0) ? PlayerCJ[leaderID][E_CJ_OriginalMax]
                : ((PlayerCJ[leaderID][E_CJ_PartnerID] != INVALID_PLAYER_ID) ? CONTAINER_TEAM_MAX : CONTAINER_SOLO_MAX);
            if(isUrgent && cargoCount < urgentMax) {
                ShowTDN(playerid, NOTIFICATION_WARNING, "Hang gap can phai day xe (%d kien)!", urgentMax);
                return 1;
            }
            if(cargoCount <= 0) { ShowTDN(playerid, NOTIFICATION_WARNING, "Xe khong co hang!"); return 1; }
            // Dung countdown khi da toi diem giao (bonus van duoc giu neu con thoi gian)
            if(isUrgent && IsValidTimer(PlayerCJ[leaderID][E_CJ_CountdownTimer])) {
                KillTimer(PlayerCJ[leaderID][E_CJ_CountdownTimer]);
                PlayerCJ[leaderID][E_CJ_CountdownTimer] = -1;
                new cj_ptner = PlayerCJ[leaderID][E_CJ_PartnerID];
                if(cj_ptner != INVALID_PLAYER_ID && IsPlayerConnected(cj_ptner))
                    SendClientMessageEx(cj_ptner, COLOR_YELLOW, "{F5D000}[Container]{ffffff} Da toi diem giao! Thoi gian dung lai - bonus hang gap duoc giu.");
                SendClientMessageEx(leaderID, COLOR_YELLOW, "{F5D000}[Container]{ffffff} Da toi diem giao! Thoi gian dung lai - bonus hang gap duoc giu.");
            }
            new dIdx = PlayerCJ[leaderID][E_CJ_DeliveryIdx];
            SendClientMessageEx(playerid, COLOR_WHITE,
                "{F5D000}[Container]{ffffff} Da den: {FFFF00}%s{FFFFFF}. %d kien hang san sang giao.",
                CJDelivery[dIdx][CJD_Name], cargoCount);
            SendHintMessage(playerid, "Xuong xe va dung /xuonghang de do hang.");
            DisablePlayerCP(playerid);
            gPlayerCheckpointStatus[playerid] = CHECKPOINT_NONE;
        }
        case CHECKPOINT_CONTAINER_RET:
        {
            if(!PlayerCJ[playerid][E_CJ_Returning] || PlayerCJ[playerid][E_CJ_LeaderID] != playerid) return 1;

            DisablePlayerCP(playerid);
            gPlayerCheckpointStatus[playerid] = CHECKPOINT_NONE;

            new vid = PlayerCJ[playerid][E_CJ_TruckID];
            if(IsValidVehicle(vid)) {
                if(GetPlayerVehicleID(playerid) == vid) RemovePlayerFromVehicle(playerid);
                DestroyVehicle(vid);
            }

            // Luu group truoc khi reset
            new gname[64], gid = PlayerCJ[playerid][E_CJ_GroupID];
            format(gname, sizeof(gname), "%s", PlayerCJ[playerid][E_CJ_GroupName]);
            new savedPartner = PlayerCJ[playerid][E_CJ_PartnerID];

            // Reset partner truoc
            if(savedPartner != INVALID_PLAYER_ID && IsPlayerConnected(savedPartner)) {
                new p_gname[64], p_gid = PlayerCJ[savedPartner][E_CJ_GroupID];
                format(p_gname, sizeof(p_gname), "%s", PlayerCJ[savedPartner][E_CJ_GroupName]);
                Container_ResetVars(savedPartner);
                format(PlayerCJ[savedPartner][E_CJ_GroupName], 64, "%s", p_gname);
                PlayerCJ[savedPartner][E_CJ_GroupID]   = p_gid;
                PlayerCJ[savedPartner][E_CJ_LeaderID]  = playerid;
                PlayerCJ[savedPartner][E_CJ_PartnerID] = playerid;
                SetPlayerSkin(savedPartner, PlayerInfo[savedPartner][pModel]);
                ShowTDN(savedPartner, NOTIFICATION_SUCCESS, "Chuyen hang hoan thanh! San sang chuyen tiep.");
                SendClientMessageEx(savedPartner, COLOR_YELLOW, "{F5D000}[Container]{ffffff} Chuyen hang ket thuc. Vao NPC de bat dau chuyen tiep!");
            }

            // Reset leader
            Container_ResetVars(playerid);
            format(PlayerCJ[playerid][E_CJ_GroupName], 64, "%s", gname);
            PlayerCJ[playerid][E_CJ_GroupID]   = gid;
            PlayerCJ[playerid][E_CJ_LeaderID]  = playerid;
            if(savedPartner != INVALID_PLAYER_ID && IsPlayerConnected(savedPartner))
                PlayerCJ[playerid][E_CJ_PartnerID] = savedPartner;

            SetPlayerSkin(playerid, PlayerInfo[playerid][pModel]);
            ShowTDN(playerid, NOTIFICATION_SUCCESS, "Chuyen hang hoan thanh! Da tra xe.");
            SendClientMessageEx(playerid, COLOR_YELLOW, "{F5D000}[Container]{ffffff} Chuyen hang ket thuc. Vao NPC de bat dau chuyen tiep!");
        }
    }
    return 1;
}

// ============================================================
//  Lenh: /giaohang - chon loai giao hang
// ============================================================
CMD:giaohang(playerid, params[])
{
    if(!IsContainerWorking(playerid))
        return ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong dang lam viec Container!");
    if(PlayerCJ[playerid][E_CJ_LeaderID] != playerid)
        return ShowTDN(playerid, NOTIFICATION_WARNING, "Chi tai xe moi duoc chon hang!");
    if(!IsPlayerInAnyVehicle(playerid) || GetPlayerVehicleID(playerid) != PlayerCJ[playerid][E_CJ_TruckID])
        return ShowTDN(playerid, NOTIFICATION_WARNING, "Ban phai ngoi trong xe cong viec!");
    if(PlayerCJ[playerid][E_CJ_CargoType] != -1)
        return ShowTDN(playerid, NOTIFICATION_WARNING, "Dang co chuyen hang. Dung /dunglamviec truoc.");

    ShowPlayerDialog(playerid, DIALOG_CONTAINER_URGTYPE, DIALOG_STYLE_LIST,
        "{F5D000}KP5 Roleplay {FFFFFF}- Loai Giao Hang",
        "Giao Thuong (khong gioi han thoi gian)\nGiao Gap (+10 phan tram gia - can day 20 kien)",
        "Chon", "Huy");
    return 1;
}

// ============================================================
//  Lenh: /xuonghang - do hang tai diem giao
// ============================================================
CMD:xuonghang(playerid, params[])
{
    if(!IsContainerWorking(playerid))
        return ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong dang lam viec Container!");

    new leaderID = PlayerCJ[playerid][E_CJ_LeaderID];
    if(leaderID == INVALID_PLAYER_ID) return 1;

    new dIdx = PlayerCJ[leaderID][E_CJ_DeliveryIdx];
    if(dIdx < 0) return ShowTDN(playerid, NOTIFICATION_WARNING, "Ban chua co chuyen hang!");
    if(IsPlayerInAnyVehicle(playerid)) return ShowTDN(playerid, NOTIFICATION_WARNING, "Phai xuong xe truoc!");
    if(GetPVarInt(leaderID, "CJ_Unloading")) return ShowTDN(playerid, NOTIFICATION_WARNING, "Dang xuong hang, vui long cho...");

    new cargoCount = PlayerCJ[leaderID][E_CJ_CargoCount];
    if(cargoCount <= 0) return ShowTDN(playerid, NOTIFICATION_WARNING, "Xe khong co hang!");
    if(!IsValidVehicle(PlayerCJ[leaderID][E_CJ_TrailerID]))
        return ShowTDN(playerid, NOTIFICATION_ERROR, "Khong tim thay trailer hang!");
    new urgentThreshX = (PlayerCJ[leaderID][E_CJ_OriginalMax] > 0) ? PlayerCJ[leaderID][E_CJ_OriginalMax]
        : ((PlayerCJ[leaderID][E_CJ_PartnerID] != INVALID_PLAYER_ID) ? CONTAINER_TEAM_MAX : CONTAINER_SOLO_MAX);
    if(PlayerCJ[leaderID][E_CJ_IsUrgent] && cargoCount < urgentThreshX)
        return ShowTDN(playerid, NOTIFICATION_ERROR, "Hang gap can day %d kien moi giao duoc!", urgentThreshX);
    if(!IsPlayerInRangeOfPoint(playerid, 15.0, CJDelivery[dIdx][CJD_X], CJDelivery[dIdx][CJD_Y], CJDelivery[dIdx][CJD_Z]))
        return ShowTDN(playerid, NOTIFICATION_WARNING, "Ban chua o diem giao hang!");

    new cjOtherMember = PlayerCJ[leaderID][E_CJ_PartnerID];
    if(cjOtherMember != INVALID_PLAYER_ID && IsPlayerConnected(cjOtherMember))
    {
        new cjCheckID = (playerid == leaderID) ? cjOtherMember : leaderID;
        if(!IsPlayerInRangeOfPoint(cjCheckID, 10.0, CJDelivery[dIdx][CJD_X], CJDelivery[dIdx][CJD_Y], CJDelivery[dIdx][CJD_Z]))
            return ShowTDN(playerid, NOTIFICATION_WARNING, "Thanh vien nhom chua den diem giao hang!");
    }

    {
        new Float:px, Float:py, Float:pz;
        GetPlayerPos(playerid, px, py, pz);
        if(GetVehicleDistanceFromPoint(PlayerCJ[leaderID][E_CJ_TrailerID], px, py, pz) > 15.0)
            return ShowTDN(playerid, NOTIFICATION_ERROR, "Khong tin thay thung hang cua xe container.");
    }

    SetPVarInt(leaderID, "CJ_Unloading", 1);
    ApplyAnimation(playerid, "BD_FIRE", "wash_up", 4.1, true, false, false, true, 0, SYNC_ALL);
    SetLoading(playerid, 10000, "Dang xuong hang...");
    SetTimerEx("CJ_DeliveryFinish", 10000, false, "i", leaderID);

    new str[128];
    format(str, sizeof(str), "{FF8000}* {C2A2DA}%s dang xuong hang container...", GetPlayerNameEx(playerid));
    ProxDetectorWrap(playerid, str, 92, 30.0, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
    //SendMessageTD(playerid, 10500, "~y~Dang do hang...");
    return 1;
}

// ============================================================
//  Lenh: /vuthang - vut hang dang cam tren tay
// ============================================================
CMD:vuthang(playerid, params[])
{
    if(!IsContainerWorking(playerid))
        return ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong dang lam viec Container!");
    if(!PlayerCJ[playerid][E_CJ_HasCargo])
        return ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong dang cam hang!");

    PlayerCJ[playerid][E_CJ_HasCargo] = false;
    RemovePlayerAttachedObject(playerid, CONTAINER_CARGO_SLOT);
    SetPlayerSpecialAction(playerid, 0);
    ClearAnimations(playerid);
    ShowTDN(playerid, NOTIFICATION_INFO, "Da vut hang.");
    return 1;
}

// ============================================================
//  Lenh: /cgroup - quan ly nhom Container
// ============================================================
CMD:cgroup(playerid, params[])
{
    new type[16];
    if(sscanf(params, "s[16]", type))
        return SendHelpMessage(playerid, "/cgroup (chapnhan | tuchoi | thoat | huy)");

    if(!strcmp(type, "chapnhan", true))
    {
        new groupid = GetPVarInt(playerid, "CJ_InviteGroupID");
        if(!groupid) return ShowTDN(playerid, NOTIFICATION_ERROR, "Ban khong co loi moi nao!");
        if(PlayerCJ[playerid][E_CJ_PartnerID] != INVALID_PLAYER_ID) return ShowTDN(playerid, NOTIFICATION_ERROR, "Ban da trong nhom roi!");
        foreach(new i : Player) if(IsPlayerConnected(i)) {
            if(PlayerCJ[i][E_CJ_GroupID] == groupid) {
                if(PlayerCJ[i][E_CJ_PartnerID] != INVALID_PLAYER_ID) return ShowTDN(playerid, NOTIFICATION_ERROR, "Nhom nay da du thanh vien!");
                PlayerCJ[i][E_CJ_PartnerID]        = playerid;
                PlayerCJ[playerid][E_CJ_PartnerID] = i;
                PlayerCJ[playerid][E_CJ_LeaderID]  = i;
                format(PlayerCJ[playerid][E_CJ_GroupName], 64, "%s", PlayerCJ[i][E_CJ_GroupName]);
                PlayerCJ[i][E_CJ_GroupID] = -1;
                DeletePVar(playerid, "CJ_InviteGroupID");
                DeletePVar(i, "CJ_PendingInvite");
                SendClientMessageEx(playerid, COLOR_WHITE, "{F5D000}[Container]{ffffff} Da tham gia nhom {FFFF00}%s{FFFFFF}.", PlayerCJ[i][E_CJ_GroupName]);
                SendClientMessageEx(i, COLOR_WHITE, "{F5D000}[Container]{ffffff} {FFFF00}%s{FFFFFF} da tham gia nhom!", GetPlayerNameEx(playerid));
                {
                    new etitle[128], estr[256];
                    format(etitle, sizeof(etitle), "Container - Tham gia nhom | %s", PlayerCJ[i][E_CJ_GroupName]);
                    format(estr, sizeof(estr),
                        ":inbox_tray: **THAM GIA NHÓM CONTAINER**\n**Thành viên:** %s (ID:%d)\n**Nhóm:** %s\n**Trưởng nhóm:** %s (ID:%d)",
                        GetPlayerNameEx(playerid), playerid, PlayerCJ[i][E_CJ_GroupName], GetPlayerNameEx(i), i);
                    DiscordLog(dclog_Container, etitle, estr);
                }
                return 1;
            }
        }
        return ShowTDN(playerid, NOTIFICATION_ERROR, "Nhom khong ton tai!");
    }
    else if(!strcmp(type, "tuchoi", true))
    {
        new groupid = GetPVarInt(playerid, "CJ_InviteGroupID");
        if(!groupid) return ShowTDN(playerid, NOTIFICATION_ERROR, "Ban khong co loi moi nao!");
        foreach(new _i : Player) if(IsPlayerConnected(_i) && PlayerCJ[_i][E_CJ_GroupID] == groupid) {
            DeletePVar(_i, "CJ_PendingInvite");
            SendClientMessageEx(_i, COLOR_YELLOW, "{F5D000}[Container]{ffffff} %s da tu choi loi moi.", GetPlayerNameEx(playerid));
            break;
        }
        DeletePVar(playerid, "CJ_InviteGroupID");
        ShowTDN(playerid, NOTIFICATION_INFO, "Da tu choi loi moi.");
    }
    else if(!strcmp(type, "thoat", true))
    {
        if(PlayerCJ[playerid][E_CJ_GroupName][0] == EOS) return ShowTDN(playerid, NOTIFICATION_ERROR, "Ban khong trong nhom nao!");
        if(IsContainerWorking(playerid)) return ShowTDN(playerid, NOTIFICATION_WARNING, "Dung /dunglamviec truoc khi roi nhom!");
        // Huy loi moi dang cho neu co
        new pendingRaw = GetPVarInt(playerid, "CJ_PendingInvite");
        if(pendingRaw != 0) {
            new pendingID = pendingRaw - 1;
            if(IsPlayerConnected(pendingID)) {
                DeletePVar(pendingID, "CJ_InviteGroupID");
                SendClientMessageEx(pendingID, COLOR_YELLOW, "{F5D000}[Container]{ffffff} Loi moi da bi huy (nguoi gui roi nhom).");
            }
            DeletePVar(playerid, "CJ_PendingInvite");
        }
        new partner = PlayerCJ[playerid][E_CJ_PartnerID];
        if(partner != INVALID_PLAYER_ID && IsPlayerConnected(partner)) {
            PlayerCJ[partner][E_CJ_PartnerID] = INVALID_PLAYER_ID;
            SendClientMessageEx(partner, COLOR_YELLOW, "{F5D000}[Container]{ffffff} %s da roi nhom.", GetPlayerNameEx(playerid));
        }
        PlayerCJ[playerid][E_CJ_PartnerID]    = INVALID_PLAYER_ID;
        PlayerCJ[playerid][E_CJ_GroupName][0] = EOS;
        PlayerCJ[playerid][E_CJ_GroupID]      = -1;
        PlayerCJ[playerid][E_CJ_LeaderID]     = INVALID_PLAYER_ID;
        ShowTDN(playerid, NOTIFICATION_INFO, "Da roi nhom.");
    }
    else if(!strcmp(type, "huy", true))
    {
        new pendingRaw = GetPVarInt(playerid, "CJ_PendingInvite");
        if(!pendingRaw) return ShowTDN(playerid, NOTIFICATION_ERROR, "Ban khong co loi moi nao dang cho!");
        new pendingID = pendingRaw - 1;
        if(IsPlayerConnected(pendingID)) {
            DeletePVar(pendingID, "CJ_InviteGroupID");
            SendClientMessageEx(pendingID, COLOR_YELLOW, "{F5D000}[Container]{ffffff} Loi moi da bi huy boi nguoi gui.");
        }
        DeletePVar(playerid, "CJ_PendingInvite");
        ShowTDN(playerid, NOTIFICATION_INFO, "Da huy loi moi.");
    }
    else SendHelpMessage(playerid, "/cgroup (chapnhan | tuchoi | thoat | huy)");
    return 1;
}

// ============================================================
//  Lenh: /khoacontainer - admin khoa/mo job
// ============================================================
CMD:khoacontainer(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 2) return SendUnAuth(playerid);
    gContainerLocked = (gContainerLocked == 0) ? 1 : 0;
    new msg[64];
    format(msg, sizeof(msg), "{F5D000}[Container]{ffffff} Cong viec %s.", gContainerLocked ? "{FF4444}bi khoa" : "{44FF44}da mo");
    ABroadCast(COLOR_YELLOW, msg, 1);
    return 1;
}

// ============================================================
//  Lenh: /admcontainer - admin quan ly gia va kho
// ============================================================
CMD:admincontainer(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 2) return SendUnAuth(playerid);
    new action[16];
    if(sscanf(params, "s[16]", action)) {
        SendClientMessageEx(playerid, COLOR_WHITE, "[Admin Container] /admcontainer gia/kho/maxkho/xem");
        return 1;
    }
    if(!strcmp(action, "gia", true)) {
        new loai; new Float:gia;
        if(sscanf(params, "s[16]if", action, loai, gia) || loai < 0 || loai > 3)
            return SendClientMessageEx(playerid, COLOR_WHITE, "Su dung: /admcontainer gia [0=VL|1=Xang|2=QA|3=Lau] [gia]");
        gCJPrice[loai] = gia;
        CJ_SaveConfig();
        SendClientMessageEx(playerid, COLOR_GREEN, "[Admin] Gia hang %d = $%.2f/kien", loai, gia);
    }
    else if(!strcmp(action, "kho", true)) {
        new idx, amount;
        if(sscanf(params, "s[16]ii", action, idx, amount) || idx < 0 || idx >= CONTAINER_WAREHOUSE_COUNT)
            return SendClientMessageEx(playerid, COLOR_WHITE, "Su dung: /admcontainer kho [0-2] [so luong]");
        CJWarehouse[idx][CJW_Stock] = amount;
        CJ_UpdateWarehouseLabel(idx);
        SendClientMessageEx(playerid, COLOR_GREEN, "[Admin] Kho %d: ton kho = %d", idx + 1, amount);
    }
    else if(!strcmp(action, "maxkho", true)) {
        new idx, amount;
        if(sscanf(params, "s[16]ii", action, idx, amount) || idx < 0 || idx >= CONTAINER_WAREHOUSE_COUNT)
            return SendClientMessageEx(playerid, COLOR_WHITE, "Su dung: /admcontainer maxkho [0-2] [so luong]");
        CJWarehouse[idx][CJW_MaxStock] = amount;
        CJWarehouse[idx][CJW_Stock]    = amount;
        CJ_UpdateWarehouseLabel(idx);
        SendClientMessageEx(playerid, COLOR_GREEN, "[Admin] Kho %d: max = %d, da reset ton kho", idx + 1, amount);
    }
    else if(!strcmp(action, "xem", true)) {
        SendClientMessageEx(playerid, COLOR_YELLOW, "=== Container Config ===");
        SendClientMessageEx(playerid, COLOR_WHITE,
            "Gia: VL=$%.0f | Xang=$%.0f | QA=$%.0f | Lau=$%.0f",
            gCJPrice[0], gCJPrice[1], gCJPrice[2], gCJPrice[3]);
        for(new i = 0; i < CONTAINER_WAREHOUSE_COUNT; i++)
            SendClientMessageEx(playerid, COLOR_WHITE, "Kho %d: %d/%d kien", i+1, CJWarehouse[i][CJW_Stock], CJWarehouse[i][CJW_MaxStock]);
    }
    return 1;
}

// ============================================================
//  Dialog: menu chinh NPC
// ============================================================
stock Container_OnMenuResponse(playerid, response, listitem)
{
    if(!response) return 1;
    if(GetPVarInt(playerid, "Injured") == 1) return ShowTDN(playerid, NOTIFICATION_ERROR, "Ban dang bi thuong!");

    switch(listitem)
    {
        case 0: ShowPlayerDialog(playerid, DIALOG_CONTAINER_GNAME, DIALOG_STYLE_INPUT,
                    "{F5D000}KP5 Roleplay {FFFFFF}- Container | Tao Nhom",
                    "Nhap ten nhom (>= 7 ky tu, khong toan so):", "Tao", "Huy");
        case 1: {
            if(PlayerCJ[playerid][E_CJ_GroupName][0] == EOS) return ShowTDN(playerid, NOTIFICATION_ERROR, "Tao nhom truoc!");
            ShowPlayerDialog(playerid, DIALOG_CONTAINER_INVITE, DIALOG_STYLE_INPUT,
                "{F5D000}KP5 Roleplay {FFFFFF}- Container | Moi Vao Nhom",
                "Nhap player ID (phai dung canh ban):", "Moi", "Huy");
        }
        case 2: {
            if(PlayerCJ[playerid][E_CJ_PartnerID] == INVALID_PLAYER_ID) return ShowTDN(playerid, NOTIFICATION_ERROR, "Nhom khong co ai!");
            ShowPlayerDialog(playerid, DIALOG_CONTAINER_KICK, DIALOG_STYLE_INPUT,
                "{F5D000}KP5 Roleplay {FFFFFF}- Container | Duoi Khoi Nhom",
                "Nhap player ID muon duoi:", "Duoi", "Huy");
        }
        case 3: ShowPlayerDialog(playerid, DIALOG_CONTAINER_JOIN, DIALOG_STYLE_INPUT,
                    "{F5D000}KP5 Roleplay {FFFFFF}- Container | Tham Gia Nhom",
                    "Nhap Group ID muon tham gia:", "Tham gia", "Huy");
        case 4: {
            if(gContainerLocked) return ShowTDN(playerid, NOTIFICATION_ERROR, "Cong viec dang bi khoa!");
            if(PlayerInfo[playerid][pJob] != JOB_CONTAINER && PlayerInfo[playerid][pJob2] != JOB_CONTAINER)
                return ShowTDN(playerid, NOTIFICATION_ERROR, "Ban khong co cong viec Lai Xe Container!");
            if(IsContainerWorking(playerid)) return ShowTDN(playerid, NOTIFICATION_ERROR, "Ban da dang lam viec!");
            if(PlayerWorking[playerid] != 0) return ShowTDN(playerid, NOTIFICATION_ERROR, "Dung /dunglamviec de thoat cong viec hien tai truoc!");
            if(PlayerCJ[playerid][E_CJ_GroupName][0] == EOS) return ShowTDN(playerid, NOTIFICATION_ERROR, "Tao hoac tham gia nhom truoc!");
            {
                new pendingRaw = GetPVarInt(playerid, "CJ_PendingInvite");
                if(pendingRaw != 0) {
                    new pendingID = pendingRaw - 1;
                    if(IsPlayerConnected(pendingID))
                        return ShowTDN(playerid, NOTIFICATION_WARNING, "Dang cho %s tra loi loi moi! Hay doi hoac /cgroup hui de huy.", GetPlayerNameEx(pendingID));
                    // Invitee da thoat game, tu dong huy
                    DeletePVar(playerid, "CJ_PendingInvite");
                }
            }

            new spawnIdx = random(3);
            new vehid = CreateVehicle(Container_GetTruckModel(playerid),
                sCJTruckSpawn[spawnIdx][0], sCJTruckSpawn[spawnIdx][1],
                sCJTruckSpawn[spawnIdx][2], sCJTruckSpawn[spawnIdx][3], 1, 1, -1);
            VehicleFuel[vehid] = 100.0;
            SetVehicleHealth(vehid, 2000.0);

            PlayerCJ[playerid][E_CJ_TruckID]  = vehid;
            PlayerCJ[playerid][E_CJ_LeaderID] = playerid;
            PlayerCJ[playerid][E_CJ_Working]  = true;
            PlayerWorking[playerid]           = JOB_CONTAINER;
            SetPlayerSkin(playerid, 72);
            PutPlayerInVehicle(playerid, vehid, 0);

            new partner = PlayerCJ[playerid][E_CJ_PartnerID];
            if(partner != INVALID_PLAYER_ID && IsPlayerConnected(partner)) {
                PlayerCJ[partner][E_CJ_TruckID]  = vehid;
                PlayerCJ[partner][E_CJ_LeaderID] = playerid;
                PlayerCJ[partner][E_CJ_Working]  = true;
                PlayerWorking[partner]           = JOB_CONTAINER;
                SetPlayerSkin(partner, 72);
                PutPlayerInVehicle(partner, vehid, 1);
                SendClientMessageEx(partner, COLOR_YELLOW,
                    "{F5D000}[Container]{ffffff} %s bat dau lam viec! Dung /giaohang.", GetPlayerNameEx(playerid));
            }
            CJ_UpdateLabel(playerid);
            SendHintMessage(playerid, "Da len xe! Su dung {FF8000}/giaohang {FFFFFF}de chon loai hang.");
            new str[128];
            format(str, sizeof(str), "{FF8000}* {C2A2DA}%s bat dau lam viec Container!", GetPlayerNameEx(playerid));
            ProxDetectorWrap(playerid, str, 92, 30.0, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
        }
        case 5: Container_Stop(playerid);
    }
    return 1;
}

stock Container_OnGroupNameResponse(playerid, response, inputtext[])
{
    if(!response) return 1;
    if(isnull(inputtext) || strlen(inputtext) < 7 || CJ_IsNumeric(inputtext)) {
        new errmsg[128];
        format(errmsg, sizeof(errmsg), "{FF4444}%s\nNhap ten nhom (>= 7 ky tu, khong toan so):",
            isnull(inputtext) ? "Khong duoc de trong!" : (strlen(inputtext) < 7 ? "Ten qua ngan!" : "Khong duoc toan so!"));
        return ShowPlayerDialog(playerid, DIALOG_CONTAINER_GNAME, DIALOG_STYLE_INPUT,
            "{F5D000}KP5 Roleplay {FFFFFF}- Tao Nhom", errmsg, "Tao", "Huy");
    }
    PlayerCJ[playerid][E_CJ_LeaderID] = playerid;
    PlayerCJ[playerid][E_CJ_GroupID]  = random(99) + 1;
    format(PlayerCJ[playerid][E_CJ_GroupName], 64, "%s", inputtext);
    SendClientMessageEx(playerid, COLOR_WHITE,
        "{F5D000}[Container]{ffffff} Tao nhom {FFFF00}%s{FFFFFF} (Group ID: {FFFF00}%d{FFFFFF}). Chia se ID nay!",
        inputtext, PlayerCJ[playerid][E_CJ_GroupID]);
    {
        new etitle[128], estr[256];
        format(etitle, sizeof(etitle), "Container - Tạo nhóm | %s", inputtext);
        format(estr, sizeof(estr),
            ":busts_in_silhouette: **TẠO NHÓM CONTAINER**\n**Trưởng nhóm:** %s (ID:%d)\n**Tên nhóm:** %s\n**Group ID:** %d",
            GetPlayerNameEx(playerid), playerid, inputtext, PlayerCJ[playerid][E_CJ_GroupID]);
        DiscordLog(dclog_Container, etitle, estr);
    }
    return 1;
}

stock Container_OnInviteResponse(playerid, response, inputtext[])
{
    if(!response) return 1;
    if(!CJ_IsNumeric(inputtext) || isnull(inputtext))
        return ShowPlayerDialog(playerid, DIALOG_CONTAINER_INVITE, DIALOG_STYLE_INPUT,
            "{F5D000}KP5 Roleplay {FFFFFF}- Moi Vao Nhom", "{FF4444}Chi nhap so ID!\nNhap player ID:", "Moi", "Huy");
    new otherid = strval(inputtext);
    if(otherid == playerid || !IsPlayerConnected(otherid))
        return ShowPlayerDialog(playerid, DIALOG_CONTAINER_INVITE, DIALOG_STYLE_INPUT,
            "{F5D000}KP5 Roleplay {FFFFFF}- Moi Vao Nhom", "{FF4444}ID khong hop le!\nNhap player ID:", "Moi", "Huy");
    if(PlayerCJ[playerid][E_CJ_PartnerID] != INVALID_PLAYER_ID)
        return ShowTDN(playerid, NOTIFICATION_ERROR, "Nhom da du 2 nguoi!");
    if(IsContainerWorking(playerid))
        return ShowTDN(playerid, NOTIFICATION_ERROR, "Khong the moi khi da bat dau lam viec!");
    new Float:ox, Float:oy, Float:oz; GetPlayerPos(otherid, ox, oy, oz);
    if(!IsPlayerInRangeOfPoint(playerid, 5.0, ox, oy, oz))
        return ShowPlayerDialog(playerid, DIALOG_CONTAINER_INVITE, DIALOG_STYLE_INPUT,
            "{F5D000}KP5 Roleplay {FFFFFF}- Moi Vao Nhom", "{FF4444}Phai dung canh nhau!\nNhap player ID:", "Moi", "Huy");
    // Neu dang co loi moi cu, huy truoc
    new oldPending = GetPVarInt(playerid, "CJ_PendingInvite");
    if(oldPending != 0) {
        new oldID = oldPending - 1;
        if(IsPlayerConnected(oldID)) {
            DeletePVar(oldID, "CJ_InviteGroupID");
            SendClientMessageEx(oldID, COLOR_YELLOW, "{F5D000}[Container]{ffffff} Loi moi cua ban da bi huy (nguoi gui moi lai).");
        }
    }
    SetPVarInt(otherid, "CJ_InviteGroupID", PlayerCJ[playerid][E_CJ_GroupID]);
    SetPVarInt(playerid, "CJ_PendingInvite", otherid + 1);
    SendClientMessageEx(playerid, COLOR_WHITE, "{F5D000}[Container]{ffffff} Da moi %s. Cho tra loi.", GetPlayerNameEx(otherid));
    SendClientMessageEx(otherid, COLOR_YELLOW,
        "{F5D000}[Container]{ffffff} {FFFF00}%s{FFFFFF} moi ban. Dung {F5D000}/cgroup chapnhan{ffffff} hoac {F5D000}/cgroup tuchoi.", GetPlayerNameEx(playerid));
    return 1;
}

stock Container_OnKickResponse(playerid, response, inputtext[])
{
    if(!response) return 1;
    if(!CJ_IsNumeric(inputtext) || isnull(inputtext))
        return ShowPlayerDialog(playerid, DIALOG_CONTAINER_KICK, DIALOG_STYLE_INPUT,
            "{F5D000}KP5 Roleplay {FFFFFF}- Duoi Khoi Nhom", "{FF4444}Chi nhap so ID!\nNhap player ID:", "Duoi", "Huy");
    new otherid = strval(inputtext);
    if(otherid < 0 || otherid >= MAX_PLAYERS || !IsPlayerConnected(otherid))
        return ShowTDN(playerid, NOTIFICATION_ERROR, "ID khong hop le hoac nguoi choi da thoat!");
    if(PlayerCJ[otherid][E_CJ_PartnerID] != playerid)
        return ShowTDN(playerid, NOTIFICATION_ERROR, "Nguoi nay khong trong nhom!");
    PlayerCJ[playerid][E_CJ_PartnerID] = INVALID_PLAYER_ID;
    PlayerCJ[otherid][E_CJ_PartnerID]  = INVALID_PLAYER_ID;
    SendClientMessageEx(playerid, COLOR_WHITE, "{F5D000}[Container]{ffffff} Da duoi %s.", GetPlayerNameEx(otherid));
    SendClientMessageEx(otherid, COLOR_YELLOW, "{F5D000}[Container]{ffffff} Ban bi %s duoi khoi nhom.", GetPlayerNameEx(playerid));
    return 1;
}

stock Container_OnJoinResponse(playerid, response, inputtext[])
{
    if(!response) return 1;
    if(PlayerCJ[playerid][E_CJ_PartnerID] != INVALID_PLAYER_ID) return ShowTDN(playerid, NOTIFICATION_ERROR, "Ban da trong nhom roi!");
    if(!CJ_IsNumeric(inputtext) || isnull(inputtext))
        return ShowPlayerDialog(playerid, DIALOG_CONTAINER_JOIN, DIALOG_STYLE_INPUT,
            "{F5D000}KP5 Roleplay {FFFFFF}- Tham Gia Nhom", "{FF4444}Chi nhap Group ID!\nNhap Group ID:", "Tham gia", "Huy");
    new bool:found = false;
    foreach(new i : Player) if(IsPlayerConnected(i)) {
        if(PlayerCJ[i][E_CJ_GroupID] == strval(inputtext)) {
            if(PlayerCJ[i][E_CJ_PartnerID] != INVALID_PLAYER_ID) return ShowTDN(playerid, NOTIFICATION_ERROR, "Nhom da du thanh vien!");
            PlayerCJ[i][E_CJ_PartnerID]        = playerid;
            PlayerCJ[playerid][E_CJ_PartnerID] = i;
            PlayerCJ[playerid][E_CJ_LeaderID]  = i;
            format(PlayerCJ[playerid][E_CJ_GroupName], 64, "%s", PlayerCJ[i][E_CJ_GroupName]);
            PlayerCJ[i][E_CJ_GroupID] = -1;
            found = true;
            SendClientMessageEx(playerid, COLOR_WHITE, "{F5D000}[Container]{ffffff} Da tham gia nhom %s.", PlayerCJ[i][E_CJ_GroupName]);
            SendClientMessageEx(i, COLOR_WHITE, "{F5D000}[Container]{ffffff} %s da tham gia!", GetPlayerNameEx(playerid));
            {
                new etitle[128], estr[256];
                format(etitle, sizeof(etitle), "Container - Tham gia nhom | %s", PlayerCJ[i][E_CJ_GroupName]);
                format(estr, sizeof(estr),
                    ":inbox_tray: **THAM GIA NHÓM CONTAINER**\n**Thành viên:** %s (ID:%d)\n**Nhóm:** %s\n**Trưởng nhóm:** %s (ID:%d)",
                    GetPlayerNameEx(playerid), playerid, PlayerCJ[i][E_CJ_GroupName], GetPlayerNameEx(i), i);
                DiscordLog(dclog_Container, etitle, estr);
            }
            break;
        }
    }
    if(!found) ShowTDN(playerid, NOTIFICATION_ERROR, "Khong tim thay Group ID!");
    return 1;
}

stock Container_OnUrgentResponse(playerid, response, listitem)
{
    if(!response) return 1;
    SetPVarInt(playerid, "CJ_IsUrgent", listitem);
    new cargoList[256];
    new bool:isCop = IsACop(playerid);
    if(listitem == 0 && !isCop)
        cargoList = "Vat Lieu (Trailer 450)\nXang Dau (Trailer 584)\nQuan Ao (Trailer 591)\nHang Lau (x2 gia)";
    else
        cargoList = "Vat Lieu (Trailer 450)\nXang Dau (Trailer 584)\nQuan Ao (Trailer 591)";
    ShowPlayerDialog(playerid, DIALOG_CONTAINER_CARGO, DIALOG_STYLE_LIST,
        "{F5D000}KP5 Roleplay {FFFFFF}- Chon Loai Hang", cargoList, "Chon", "Huy");
    return 1;
}

stock Container_OnCargoResponse(playerid, response, listitem)
{
    if(!response) { DeletePVar(playerid, "CJ_IsUrgent"); return 1; }
    new bool:isUrgent = GetPVarInt(playerid, "CJ_IsUrgent") == 1;
    DeletePVar(playerid, "CJ_IsUrgent");
    if(isUrgent && listitem > CARGO_QUANAO) return 1;
    if(listitem == CARGO_LAU && IsACop(playerid)) {
        ShowTDN(playerid, NOTIFICATION_ERROR, "Nguoi thuc thi phap luat khong duoc phep van chuyen hang cam!");
        return 1;
    }
    if(!IsPlayerInAnyVehicle(playerid) || GetPlayerVehicleID(playerid) != PlayerCJ[playerid][E_CJ_TruckID]) {
        ShowTDN(playerid, NOTIFICATION_ERROR, "Phai ngoi trong xe!");
        return 1;
    }
    CJ_StartDelivery(playerid, listitem, isUrgent);
    return 1;
}

// ============================================================
//  Mo rong /kiemtrahang cho hang lau container
// ============================================================
stock Container_CheckIllegal(copid, targetid)
{
    if(PlayerInfo[copid][pDuty] != 1) return SendClientMessageEx(copid, COLOR_GREY, "{F5D000}[Container]{ffffff} Phai on-duty moi kiem tra!");
    if(!IsContainerWorking(targetid)) return 0; // Tra ve 0 de trucker handler xu ly tiep

    new leaderID = PlayerCJ[targetid][E_CJ_LeaderID];
    if(leaderID == INVALID_PLAYER_ID) return 0;
    if(PlayerCJ[leaderID][E_CJ_CargoType] != CARGO_LAU) {
        SendClientMessageEx(copid, COLOR_WHITE, "{F5D000}[Container]{ffffff} Xe nay khong mang hang lau.");
        return 1;
    }
    if(PlayerCJ[leaderID][E_CJ_CargoCount] <= 0) {
        SendClientMessageEx(copid, COLOR_WHITE, "{F5D000}[Container]{ffffff} Xe nay khong co hang.");
        return 1;
    }
    new Float:tvx, Float:tvy, Float:tvz;
    GetVehiclePos(PlayerCJ[leaderID][E_CJ_TruckID], tvx, tvy, tvz);
    if(!IsPlayerInRangeOfPoint(copid, 8.0, tvx, tvy, tvz))
        return SendClientMessageEx(copid, COLOR_GREY, "{F5D000}[Container]{ffffff} Phai dung gan xe container!"), 1;

    new cargoCount = PlayerCJ[leaderID][E_CJ_CargoCount];
    new bonus = 100 + random(21);
    GivePlayerCash(copid, bonus);
    PlayerCJ[leaderID][E_CJ_CargoCount] = 0;

    new msg[128];
    format(msg, sizeof(msg), "{F5D000}[Container]{ffffff} %s tich thu %d kien hang lau. Nhan {3BFF6F}$%d{FFFFFF} thuong.", GetPlayerNameEx(copid), cargoCount, bonus);
    SendClientMessageEx(copid, COLOR_WHITE, msg);
    format(msg, sizeof(msg), "{F5D000}[Container]{ffffff} %s da tich thu %d kien hang lau cua ban!", GetPlayerNameEx(copid), cargoCount);
    SendClientMessageEx(leaderID, COLOR_RED, msg);
    if(leaderID != targetid) SendClientMessageEx(targetid, COLOR_RED, msg);

    DisablePlayerCP(leaderID);
    if(gPlayerCheckpointStatus[leaderID] >= CHECKPOINT_CONTAINER_WH
    && gPlayerCheckpointStatus[leaderID] <= CHECKPOINT_CONTAINER_RET)
        gPlayerCheckpointStatus[leaderID] = CHECKPOINT_NONE;

    new bMsg[128];
    format(bMsg, sizeof(bMsg), "* %s tich thu hang lau tu container cua %s!", GetPlayerNameEx(copid), GetPlayerNameEx(leaderID));
    ProxDetectorWrap(copid, bMsg, 92, 30.0, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

    {
        new etitle[128], estr[256];
        format(etitle, sizeof(etitle), "Container - Tich thu hang lau | %s", GetPlayerNameEx(copid));
        format(estr, sizeof(estr), ":police_car: **TICH THU HANG LAU (CONTAINER)**\n**Cop:** %s\n**Lai xe:** %s\n**So kien:** %d\n**Thuong:** $%d",
            GetPlayerNameEx(copid), GetPlayerNameEx(leaderID), cargoCount, bonus);
        DiscordLog(dclog_Container, etitle, estr);
    }
    return 1;
}

// ============================================================
//  Hien danh sach diem giao hang de leader chon (hang thuong)
// ============================================================
stock CJ_ShowDeliveryListDialog(playerid)
{
    if(!IsContainerWorking(playerid) || PlayerCJ[playerid][E_CJ_LeaderID] != playerid) {
        ShowTDN(playerid, NOTIFICATION_WARNING, "Chi leader Container moi dung duoc lenh nay!");
        return 0;
    }
    if(PlayerCJ[playerid][E_CJ_IsUrgent]) {
        ShowTDN(playerid, NOTIFICATION_WARNING, "Hang gap da co diem giao tu dong!");
        return 0;
    }
    new cargoType = PlayerCJ[playerid][E_CJ_CargoType];
    if(cargoType == -1) {
        ShowTDN(playerid, NOTIFICATION_WARNING, "Chon loai hang truoc (/giaohang)!");
        return 0;
    }
    new szDialog[2048], line[128], zone[32];
    strcat(szDialog, "#\tTen Diem\tKhu Vuc\tKhoang Cach\n", sizeof(szDialog));
    new count = 0;
    for(new i = 0; i < CONTAINER_DELIVERY_COUNT; i++) {
        if(CJDelivery[i][CJD_CargoType] != cargoType) continue;
        Get3DZone(CJDelivery[i][CJD_X], CJDelivery[i][CJD_Y], CJDelivery[i][CJD_Z], zone, sizeof(zone));
        new dist = floatround(GetPlayerDistanceFromPoint(playerid, CJDelivery[i][CJD_X], CJDelivery[i][CJD_Y], CJDelivery[i][CJD_Z]));
        format(line, sizeof(line), "%d\t%s\t%s\t%dm\n", count + 1, CJDelivery[i][CJD_Name], zone, dist);
        strcat(szDialog, line, sizeof(szDialog));
        count++;
    }
    if(!count) { ShowTDN(playerid, NOTIFICATION_ERROR, "Khong co diem giao phu hop!"); return 0; }
    ShowPlayerDialog(playerid, DIALOG_CONTAINER_DELIVERYLIST, DIALOG_STYLE_TABLIST_HEADERS,
        "Chon Diem Giao Hang (Container)", szDialog, "Chon", "Huy");
    return 1;
}

// ============================================================
//  Dialog: chon diem giao hang (hang thuong)
// ============================================================
hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid != DIALOG_CONTAINER_DELIVERYLIST) return 1;
    if(!response) return 1;
    if(!IsContainerWorking(playerid) || PlayerCJ[playerid][E_CJ_LeaderID] != playerid) return 1;
    new cargoType = PlayerCJ[playerid][E_CJ_CargoType];
    new count = 0;
    for(new i = 0; i < CONTAINER_DELIVERY_COUNT; i++) {
        if(CJDelivery[i][CJD_CargoType] != cargoType) continue;
        if(count == listitem) {
            PlayerCJ[playerid][E_CJ_DeliveryIdx]  = i;
            PlayerCJ[playerid][E_CJ_DeliveryDist] = GetPlayerDistanceFromPoint(playerid,
                CJDelivery[i][CJD_X], CJDelivery[i][CJD_Y], CJDelivery[i][CJD_Z]);
            new partner = PlayerCJ[playerid][E_CJ_PartnerID];
            if(partner != INVALID_PLAYER_ID && IsPlayerConnected(partner)) {
                PlayerCJ[partner][E_CJ_DeliveryIdx] = i;
                SetPlayerCP(partner, CJDelivery[i][CJD_X], CJDelivery[i][CJD_Y], CJDelivery[i][CJD_Z], 8.0);
                gPlayerCheckpointStatus[partner] = CHECKPOINT_CONTAINER_DEL;
                SendClientMessageEx(partner, COLOR_YELLOW,
                    "{F5D000}[Container]{ffffff} Leader chon diem giao: {FFFF00}%s{FFFFFF}. Checkpoint da hien!", CJDelivery[i][CJD_Name]);
            }
            SetPlayerCP(playerid, CJDelivery[i][CJD_X], CJDelivery[i][CJD_Y], CJDelivery[i][CJD_Z], 8.0);
            gPlayerCheckpointStatus[playerid] = CHECKPOINT_CONTAINER_DEL;
            SendClientMessageEx(playerid, COLOR_WHITE,
                "{F5D000}[Container]{ffffff} Diem giao: {FFFF00}%s{FFFFFF} | Checkpoint da hien tren ban do!", CJDelivery[i][CJD_Name]);
            break;
        }
        count++;
    }
    return 1;
}

// ============================================================
//  Dung lam viec
// ============================================================
stock Container_Stop(playerid)
{
    if(!IsContainerWorking(playerid)) return 0;

    new gname[64], gid = PlayerCJ[playerid][E_CJ_GroupID];
    format(gname, sizeof(gname), "%s", PlayerCJ[playerid][E_CJ_GroupName]);

    if(PlayerCJ[playerid][E_CJ_LeaderID] == playerid) {
        new partner = PlayerCJ[playerid][E_CJ_PartnerID];
        if(partner != INVALID_PLAYER_ID && IsPlayerConnected(partner)) {
            SendClientMessageEx(partner, COLOR_RED, "{F5D000}[Container]{ffffff} %s da ngung lam viec.", GetPlayerNameEx(playerid));
            Container_ResetVars(partner);
        }
        new tid = PlayerCJ[playerid][E_CJ_TrailerID];
        if(IsValidVehicle(tid)) DestroyVehicle(tid);
        new vid = PlayerCJ[playerid][E_CJ_TruckID];
        if(IsValidVehicle(vid)) {
            if(GetPlayerVehicleID(playerid) == vid) RemovePlayerFromVehicle(playerid);
            DestroyVehicle(vid);
        }
    }
    SetPlayerSkin(playerid, PlayerInfo[playerid][pModel]);
    new str[128];
    format(str, sizeof(str), "{FF8000}* {C2A2DA}%s ngung lam viec Container.", GetPlayerNameEx(playerid));
    ProxDetectorWrap(playerid, str, 92, 30.0, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
    Container_ResetVars(playerid);
    // Giu lai ten nhom de bat dau lai nhanh
    format(PlayerCJ[playerid][E_CJ_GroupName], 64, "%s", gname);
    PlayerCJ[playerid][E_CJ_GroupID]  = gid;
    PlayerCJ[playerid][E_CJ_LeaderID] = playerid;
    ShowTDN(playerid, NOTIFICATION_INFO, "Da ngung lam viec Container Delivery.");
    return 1;
}
