// ============================================================
//  Prison Labor System - IC Prisoner grass cutting
//  He thong don co giam thoi gian tu cho tu nhan IC
//  Created by Codyz
// ============================================================

#include <YSI\YSI_Coding\y_hooks>

// ============================================================
// DEFINES
// ============================================================
#define PRISON_LABOR_MAX_PLANTS     20
#define PRISON_LABOR_OBJECT_MODEL   2811
#define PRISON_LABOR_HARVEST_TIME   5           // 5 giay
#define PRISON_LABOR_RESPAWN_TIME   600000      // 10 phut (ms)
#define PRISON_LABOR_CHECK_RANGE    4.0
#define PRISON_LABOR_FAIL_RANGE     6.0

#define PRISON_LABOR_ZONE_MINX      (-2089.2999954223633)
#define PRISON_LABOR_ZONE_MINY      (-276.10009765625)
#define PRISON_LABOR_ZONE_MAXX      (-2019.2999954223633)
#define PRISON_LABOR_ZONE_MAXY      (-128.10009765625)
#define PRISON_LABOR_ZONE_COLOR     (0xFF0000FF)

// ============================================================
// VARIABLES
// ============================================================
new Float:gLaborX[PRISON_LABOR_MAX_PLANTS];
new Float:gLaborY[PRISON_LABOR_MAX_PLANTS];
new Float:gLaborZ[PRISON_LABOR_MAX_PLANTS];
new gLaborObj[PRISON_LABOR_MAX_PLANTS];
new bool:gLaborActive[PRISON_LABOR_MAX_PLANTS];
new bool:gLaborOccupied[PRISON_LABOR_MAX_PLANTS];
new gLaborPlayer[PRISON_LABOR_MAX_PLANTS];

new gPrisonLaborZone = -1;

new gPlayerLaborPlant[MAX_PLAYERS];

// ============================================================
// FORWARDS
// ============================================================
forward PrisonLabor_HarvestFinish(playerid);
forward PrisonLabor_RespawnPlant(plantIndex);

// ============================================================
// HELPERS
// ============================================================
static stock bool:IsICPrisoner(playerid)
{
    return (PlayerInfo[playerid][pJailTime] > 0 && strfind(PlayerInfo[playerid][pPrisonReason], "[IC]", true) != -1);
}

static stock PrisonLabor_FindNearestPlant(playerid)
{
    for(new i = 0; i < PRISON_LABOR_MAX_PLANTS; i++)
    {
        if(!gLaborActive[i] || gLaborOccupied[i]) continue;
        if(IsPlayerInRangeOfPoint(playerid, PRISON_LABOR_CHECK_RANGE, gLaborX[i], gLaborY[i], gLaborZ[i]))
            return i;
    }
    return -1;
}

static stock PrisonLabor_SpawnPlantAt(index, Float:x, Float:y, Float:z)
{
    gLaborX[index] = x;
    gLaborY[index] = y;
    gLaborZ[index] = z;
    gLaborObj[index] = CreateDynamicObject(PRISON_LABOR_OBJECT_MODEL, x, y, z - 1.0, 0.0, 0.0, 0.0, .worldid = 0, .streamdistance = 300.0);
    gLaborActive[index] = true;
    gLaborOccupied[index] = false;
    gLaborPlayer[index] = -1;
}

// ============================================================
// INIT
// ============================================================
PrisonLabor_Init()
{
    new count = 0, attempts = 0;
    new Float:x, Float:y, Float:z;

    for(new i = 0; i < PRISON_LABOR_MAX_PLANTS; i++)
    {
        gLaborActive[i]   = false;
        gLaborOccupied[i] = false;
        gLaborPlayer[i]   = -1;
        gLaborObj[i]      = -1;
    }

    gPrisonLaborZone = GangZoneCreate(
        PRISON_LABOR_ZONE_MINX, PRISON_LABOR_ZONE_MINY,
        PRISON_LABOR_ZONE_MAXX, PRISON_LABOR_ZONE_MAXY
    );

    new rangeX = floatround((PRISON_LABOR_ZONE_MAXX - PRISON_LABOR_ZONE_MINX) * 100.0);
    new rangeY = floatround((PRISON_LABOR_ZONE_MAXY - PRISON_LABOR_ZONE_MINY) * 100.0);

    while(count < PRISON_LABOR_MAX_PLANTS && attempts < 20000)
    {
        attempts++;
        x = PRISON_LABOR_ZONE_MINX + float(random(rangeX)) / 100.0;
        y = PRISON_LABOR_ZONE_MINY + float(random(rangeY)) / 100.0;
        if(!CA_FindZ_For2DCoord(x, y, z)) continue;
        PrisonLabor_SpawnPlantAt(count, x, y, z);
        count++;
    }

    printf("[PrisonLabor] %d/%d cay co da spawn.", count, PRISON_LABOR_MAX_PLANTS);
    return 1;
}

stock PrisonLabor_ShowZone(playerid)
{
    if(gPrisonLaborZone != -1)
        GangZoneShowForPlayer(playerid, gPrisonLaborZone, PRISON_LABOR_ZONE_COLOR);
}

stock PrisonLabor_HideZone(playerid)
{
    if(gPrisonLaborZone != -1)
        GangZoneHideForPlayer(playerid, gPrisonLaborZone);
}

// ============================================================
// HARVEST - Bat dau
// ============================================================
static stock PrisonLabor_StartHarvest(playerid, plantIndex)
{
    gLaborOccupied[plantIndex] = true;
    gLaborPlayer[plantIndex]   = playerid;
    gPlayerLaborPlant[playerid] = plantIndex;

    ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.1, 1, 0, 0, 1, 0, 1);
    SetLoading(playerid, PRISON_LABOR_HARVEST_TIME * 1000, "Dang don co...", "PrisonLabor_HarvestFinish");

    new string[128];
    format(string, sizeof(string), "{FF8000}* {C2A2DA}%s dang don co.", GetPlayerNameEx(playerid));
    ProxDetector(30.0, playerid, string, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
    return 1;
}

// ============================================================
// HARVEST - Hoan thanh
// ============================================================
public PrisonLabor_HarvestFinish(playerid)
{
    if(!IsPlayerConnected(playerid)) return 1;

    new plantIndex = gPlayerLaborPlant[playerid];
    gPlayerLaborPlant[playerid] = -1;

    if(plantIndex < 0 || plantIndex >= PRISON_LABOR_MAX_PLANTS) return 1;

    gLaborOccupied[plantIndex] = false;
    gLaborPlayer[plantIndex]   = -1;

    if(!IsICPrisoner(playerid))
    {
        ClearAnimations(playerid);
        return 1;
    }

    if(!IsPlayerInRangeOfPoint(playerid, PRISON_LABOR_FAIL_RANGE, gLaborX[plantIndex], gLaborY[plantIndex], gLaborZ[plantIndex]))
    {
        ClearAnimations(playerid);
        ShowTDN(playerid, NOTIFICATION_ERROR, "Don co that bai vi ban di qua xa.");
        SendClientMessageEx(playerid, COLOR_RED, "{FF6600}[Tu Nhan] {ffffff}Ban da di chuyen qua xa, don co that bai.");
        return 1;
    }

    if(gLaborObj[plantIndex] != -1)
    {
        DestroyDynamicObject(gLaborObj[plantIndex]);
        gLaborObj[plantIndex] = -1;
    }
    gLaborActive[plantIndex] = false;
    SetTimerEx("PrisonLabor_RespawnPlant", PRISON_LABOR_RESPAWN_TIME, false, "d", plantIndex);

    ClearAnimations(playerid);

    // Giam thoi gian tu: random 30-60 giay
    new reduce = random(31) + 30;
    if(PlayerInfo[playerid][pJailTime] > reduce)
        PlayerInfo[playerid][pJailTime] -= reduce;
    else
        PlayerInfo[playerid][pJailTime] = 1;

    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{FF6600}[Tu Nhan] {ffffff}Don co thanh cong! Giam {FFFF00}%d giay{ffffff} thoi gian tu. Con lai: {FF8000}%s",
        reduce, FormatTimeLeft(PlayerInfo[playerid][pJailTime]));
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Don co thanh cong!", reduce);

    new proxstr[128];
    format(proxstr, sizeof(proxstr), "{FF8000}* {C2A2DA}%s da don xong mot dam co.", GetPlayerNameEx(playerid));
    ProxDetector(30.0, playerid, proxstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
    return 1;
}

// ============================================================
// RESPAWN
// ============================================================
public PrisonLabor_RespawnPlant(plantIndex)
{
    if(plantIndex < 0 || plantIndex >= PRISON_LABOR_MAX_PLANTS) return 1;
    gLaborObj[plantIndex] = CreateDynamicObject(PRISON_LABOR_OBJECT_MODEL,
        gLaborX[plantIndex], gLaborY[plantIndex], gLaborZ[plantIndex] - 1.0,
        0.0, 0.0, 0.0, .worldid = 0, .streamdistance = 300.0);
    gLaborActive[plantIndex] = true;
    return 1;
}

// ============================================================
// HOOKS
// ============================================================
hook OnPlayerConnect(playerid)
{
    gPlayerLaborPlant[playerid] = -1;
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    new plantIndex = gPlayerLaborPlant[playerid];
    if(plantIndex >= 0 && plantIndex < PRISON_LABOR_MAX_PLANTS)
    {
        gLaborOccupied[plantIndex] = false;
        gLaborPlayer[plantIndex]   = -1;
    }
    gPlayerLaborPlant[playerid] = -1;
    return 1;
}

hook OnPlayerSpawn(playerid)
{
    if(IsICPrisoner(playerid))
        PrisonLabor_ShowZone(playerid);
    else
        PrisonLabor_HideZone(playerid);
    return 1;
}

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if(!PRESSED(KEY_YES)) return 1;
    if(!IsICPrisoner(playerid)) return 1;
    if(IsPlayerInAnyVehicle(playerid)) return 1;
    if(IsLoadingActive(playerid)) return 1;
    if(gPlayerLaborPlant[playerid] != -1) return 1;
    if(PlayerCuffed[playerid] != 0 || GetPVarInt(playerid, "Injured") != 0 || PlayerInfo[playerid][pHospital] != 0) return 1;

    new plant = PrisonLabor_FindNearestPlant(playerid);
    if(plant != -1)
    {
        PrisonLabor_StartHarvest(playerid, plant);
        return 1;
    }
    return 1;
}
