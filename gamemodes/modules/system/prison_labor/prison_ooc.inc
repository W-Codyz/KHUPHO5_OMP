// ============================================================
//  OOC Prison Labor - Boc hang giam thoi gian tu
//  Tu nhan OOC lam viec boc hang tren dao vang
//  Created by Codyz
// ============================================================

#include <YSI\YSI_Coding\y_hooks>

// ============================================================
// DEFINES
// ============================================================
#define PRISON_OOC_SPAWN_X          (1227.988037)
#define PRISON_OOC_SPAWN_Y          (-3907.316162)
#define PRISON_OOC_SPAWN_Z          (11.074675)
#define PRISON_OOC_SPAWN_A          (83.133552)

#define PRISON_OOC_CENTER_X         (1295.104492)
#define PRISON_OOC_CENTER_Y         (-3921.612792)
#define PRISON_OOC_CENTER_Z         (10.031217)
#define PRISON_OOC_BOUNDARY         (200.0)

#define PRISON_OOC_PICKUP_RANGE     (3.0)
#define PRISON_OOC_PICKUP_FAIL_RANGE (5.0)
#define PRISON_OOC_PICKUP_TIME      (15000) // 15 giay ms
#define PRISON_OOC_CP_RADIUS        (2.0)

// 4 diem giao hang
static const Float:gOOCDeliveryPoints[4][3] = {
    {1223.949462, -3882.640869, 10.844900},
    {1224.612426, -3876.994628, 10.844900},
    {1224.834960, -3871.796142, 10.844900},
    {1225.256835, -3866.480712, 11.038112}
};

// ============================================================
// VARIABLES
// ============================================================
new bool:gPlayerOOCCarrying[MAX_PLAYERS];
new bool:gPlayerOOCPickingUp[MAX_PLAYERS];
new gPlayerOOCDelivery[MAX_PLAYERS]; // -1 = khong co checkpoint

new Text3D:gOOCPickupLabel = INVALID_3DTEXT_ID;
new gOOCPickupID = -1;

// ============================================================
// FORWARDS
// ============================================================
forward OOCLabor_PickupFinish(playerid);
forward OOCLabor_Tick();

// ============================================================
// HELPERS
// ============================================================
static stock bool:IsOOCPrisoner(playerid)
{
    return (PlayerInfo[playerid][pJailTime] > 0 && strfind(PlayerInfo[playerid][pPrisonReason], "[OOC]", true) != -1);
}

stock OOCLabor_ClearCarry(playerid)
{
    if(gPlayerOOCCarrying[playerid])
    {
        RemovePlayerAttachedObject(playerid, 6);
        SetPlayerSpecialAction(playerid, 0);
        DisablePlayerCheckpoint(playerid);
        gPlayerOOCCarrying[playerid] = false;
        gPlayerOOCDelivery[playerid] = -1;
    }
    gPlayerOOCPickingUp[playerid] = false;
}

// ============================================================
// INIT
// ============================================================
OOCLabor_Init()
{
    for(new i = 0; i < MAX_PLAYERS; i++)
    {
        gPlayerOOCCarrying[i]  = false;
        gPlayerOOCPickingUp[i] = false;
        gPlayerOOCDelivery[i]  = -1;
    }

    new label[128];
    format(label, sizeof(label),
        "{FF6600}[Khu Vuc Boc Hang]\n{FFFFFF}Nhan {F6F08D}[Y]{FFFFFF} de bat dau boc hang\n{AAAAAA}Chi danh cho tu nhan");
    gOOCPickupLabel = CreateDynamic3DTextLabel(label, 0xFFFFFFFF,
        PRISON_OOC_SPAWN_X, PRISON_OOC_SPAWN_Y, PRISON_OOC_SPAWN_Z + 1.0, 10.0);
    gOOCPickupID = CreateDynamicPickup(1239, 23,
        PRISON_OOC_SPAWN_X, PRISON_OOC_SPAWN_Y, PRISON_OOC_SPAWN_Z);

    SetTimer("OOCLabor_Tick", 1000, true);
    printf("[OOCLabor] He thong lao dong tu OOC da khoi dong.");
    return 1;
}

// ============================================================
// TICK - boundary check + clear carry khi het tu
// ============================================================
public OOCLabor_Tick()
{
    foreach(new i: Player)
    {
        if(!IsPlayerConnected(i)) continue;

        // Clear carry neu khong con la tu nhan OOC
        if((gPlayerOOCCarrying[i] || gPlayerOOCPickingUp[i]) && !IsOOCPrisoner(i))
        {
            OOCLabor_ClearCarry(i);
            ClearAnimations(i);
            continue;
        }

        if(!IsOOCPrisoner(i)) continue;

        // Kiem tra bien gioi 200m
        if(!IsPlayerInRangeOfPoint(i, PRISON_OOC_BOUNDARY, PRISON_OOC_CENTER_X, PRISON_OOC_CENTER_Y, PRISON_OOC_CENTER_Z))
        {
            OOCLabor_ClearCarry(i);
            ClearAnimations(i);
            SetPlayerPos(i, PRISON_OOC_SPAWN_X, PRISON_OOC_SPAWN_Y, PRISON_OOC_SPAWN_Z);
            SetPlayerFacingAngle(i, PRISON_OOC_SPAWN_A);
            SendClientMessageEx(i, COLOR_RED, "{FF6600}[Tu Nhan] {ffffff}Khong the di chuyen qua xa khu vuc giam giu.");
        }
    }
}

// ============================================================
// PICKUP - Bat dau boc hang
// ============================================================
static stock OOCLabor_StartPickup(playerid)
{
    gPlayerOOCPickingUp[playerid] = true;

    ApplyAnimation(playerid, "BD_FIRE", "wash_up", 4.1, 1, 0, 0, 0, 16000, 1);
    SetLoading(playerid, PRISON_OOC_PICKUP_TIME, "Dang boc hang...", "OOCLabor_PickupFinish");

    new string[128];
    format(string, sizeof(string), "{FF8000}* {C2A2DA}%s dang boc hang.", GetPlayerNameEx(playerid));
    ProxDetector(30.0, playerid, string, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
}

public OOCLabor_PickupFinish(playerid)
{
    if(!IsPlayerConnected(playerid)) return 1;
    gPlayerOOCPickingUp[playerid] = false;

    if(!IsOOCPrisoner(playerid)) return 1;

    // Kiem tra vi tri khi hoan thanh
    if(!IsPlayerInRangeOfPoint(playerid, PRISON_OOC_PICKUP_FAIL_RANGE, PRISON_OOC_SPAWN_X, PRISON_OOC_SPAWN_Y, PRISON_OOC_SPAWN_Z))
    {
        ClearAnimations(playerid);
        ShowTDN(playerid, NOTIFICATION_ERROR, "Boc hang that bai vi ban di qua xa.");
        SendClientMessageEx(playerid, COLOR_RED, "{FF6600}[Tu Nhan] {ffffff}Ban da di chuyen qua xa, boc hang that bai.");
        return 1;
    }

    // Thanh cong - cam hang len
    ApplyAnimation(playerid, "CARRY", "LIFTUP", 4.0, 0, 0, 0, 0, 0);
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
    SetPlayerAttachedObject(playerid, 6, 1271, 1,
        0.299822, 0.663485, -0.036337, 184.311355, 90.004089, 0.0, 1.0, 1.0, 1.0);

    // Chon ngau nhien diem giao hang
    new deliveryIdx = random(sizeof(gOOCDeliveryPoints));
    gPlayerOOCCarrying[playerid]  = true;
    gPlayerOOCDelivery[playerid]  = deliveryIdx;
    SetPlayerCheckpoint(playerid,
        gOOCDeliveryPoints[deliveryIdx][0],
        gOOCDeliveryPoints[deliveryIdx][1],
        gOOCDeliveryPoints[deliveryIdx][2],
        PRISON_OOC_CP_RADIUS);

    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{FF6600}[Tu Nhan] {ffffff}Boc hang thanh cong! Di den diem giao hang tren man hinh.");
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Boc hang thanh cong! Di den checkpoint.");

    new proxstr[128];
    format(proxstr, sizeof(proxstr), "{FF8000}* {C2A2DA}%s da boc hang len vai.", GetPlayerNameEx(playerid));
    ProxDetector(30.0, playerid, proxstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
    return 1;
}

// ============================================================
// HOOKS
// ============================================================
hook OnPlayerConnect(playerid)
{
    gPlayerOOCCarrying[playerid]  = false;
    gPlayerOOCPickingUp[playerid] = false;
    gPlayerOOCDelivery[playerid]  = -1;
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    OOCLabor_ClearCarry(playerid);
    return 1;
}

hook OnPlayerSpawn(playerid)
{
    if(gPlayerOOCCarrying[playerid] || gPlayerOOCPickingUp[playerid])
        OOCLabor_ClearCarry(playerid);
    return 1;
}

hook OnPlayerEnterCheckpoint(playerid)
{
    if(!gPlayerOOCCarrying[playerid] || gPlayerOOCDelivery[playerid] == -1) return 1;

    if(!IsOOCPrisoner(playerid))
    {
        OOCLabor_ClearCarry(playerid);
        return 1;
    }

    DisablePlayerCheckpoint(playerid);
    gPlayerOOCCarrying[playerid] = false;
    gPlayerOOCDelivery[playerid] = -1;

    // Dat hang xuong
    ApplyAnimation(playerid, "CARRY", "PUTDWN", 4.0, 0, 0, 0, 0, 0);
    RemovePlayerAttachedObject(playerid, 6);
    SetPlayerSpecialAction(playerid, 0);

    // Giam thoi gian tu: random 30-120 giay
    new reduce = random(91) + 30;
    if(PlayerInfo[playerid][pJailTime] > reduce)
        PlayerInfo[playerid][pJailTime] -= reduce;
    else
        PlayerInfo[playerid][pJailTime] = 1;

    // Thuong tien: $500-$519
    new reward = 500 + random(20);
    GivePlayerCash(playerid, reward);

    /* new szTDN[64];
    format(szTDN, sizeof(szTDN), "Giao hang! -%d giay tu +$%d", reduce, reward); */
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Giao hang thanh cong!");
    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{FF6600}[Tu Nhan] {ffffff}Giao hang thanh cong! Giam {FFFF00}%d giay{ffffff} tu + {00FF00}$%d{ffffff}. Con lai: {FF8000}%s",
        reduce, reward, FormatTimeLeft(PlayerInfo[playerid][pJailTime]));

    new proxstr[128];
    format(proxstr, sizeof(proxstr), "{FF8000}* {C2A2DA}%s da giao hang xong.", GetPlayerNameEx(playerid));
    ProxDetector(30.0, playerid, proxstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
    return 1;
}

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if(!PRESSED(KEY_YES)) return 1;
    if(!IsOOCPrisoner(playerid)) return 1;
    if(IsPlayerInAnyVehicle(playerid)) return 1;
    if(IsLoadingActive(playerid)) return 1;
    if(gPlayerOOCPickingUp[playerid] || gPlayerOOCCarrying[playerid]) return 1;
    if(PlayerCuffed[playerid] != 0 || GetPVarInt(playerid, "Injured") != 0 || PlayerInfo[playerid][pHospital] != 0) return 1;

    if(IsPlayerInRangeOfPoint(playerid, PRISON_OOC_PICKUP_RANGE, PRISON_OOC_SPAWN_X, PRISON_OOC_SPAWN_Y, PRISON_OOC_SPAWN_Z))
    {
        OOCLabor_StartPickup(playerid);
        return 1;
    }
    return 1;
}
