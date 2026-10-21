// ============================================================
//  Herb Harvest System - KP5 Roleplay Created by Codyz
//  He thong hai thao duoc trong gangzone
// ============================================================

#include <YSI\YSI_Coding\y_hooks>

// ============================================================
// DEFINES
// ============================================================
#define HERB_MAX_PLANTS         100
#define HERB_OBJECT_MODEL       2811
#define HERB_HARVEST_TIME       10      // 10 giay
#define HERB_RESPAWN_TIME       3600000 // 1 tieng (ms)
#define HERB_CHECK_RANGE        4.0     // Pham vi de hai
#define HERB_FAIL_RANGE         6.0     // Roi xa hon nay = that bai

// Zone thao duoc
#define HERB_ZONE_MINX          (-1847.8999)
#define HERB_ZONE_MINY          (-2541.2875)
#define HERB_ZONE_MAXX          (-1291.8999)
#define HERB_ZONE_MAXY          (-1801.2875)

// ============================================================
// VARIABLES
// ============================================================
new Float:gHerbX[HERB_MAX_PLANTS];
new Float:gHerbY[HERB_MAX_PLANTS];
new Float:gHerbZ[HERB_MAX_PLANTS];
new gHerbObj[HERB_MAX_PLANTS];
new bool:gHerbActive[HERB_MAX_PLANTS];
new bool:gHerbOccupied[HERB_MAX_PLANTS];
new gHerbPlayer[HERB_MAX_PLANTS];

// Player data
new gPlayerHarvestHerb[MAX_PLAYERS];    // index cay dang hai (-1 = khong)

// ============================================================
// FORWARDS
// ============================================================
forward Herb_HarvestFinish(playerid);
forward Herb_RespawnPlant(plantIndex);

// ============================================================
// HELPERS
// ============================================================
static stock Herb_FindNearestPlant(playerid)
{
    for(new i = 0; i < HERB_MAX_PLANTS; i++)
    {
        if(!gHerbActive[i] || gHerbOccupied[i]) continue;
        if(IsPlayerInRangeOfPoint(playerid, HERB_CHECK_RANGE, gHerbX[i], gHerbY[i], gHerbZ[i]))
            return i;
    }
    return -1;
}

static stock Herb_SpawnPlantAt(index, Float:x, Float:y, Float:z)
{
    gHerbX[index] = x;
    gHerbY[index] = y;
    gHerbZ[index] = z;
    gHerbObj[index] = CreateDynamicObject(HERB_OBJECT_MODEL, x, y, z - 0.5, 0.0, 0.0, 0.0, .worldid = 0, .streamdistance = 700.0);
    gHerbActive[index] = true;
    gHerbOccupied[index] = false;
    gHerbPlayer[index] = -1;
}

// ============================================================
// INIT - Spawn thao duoc ngau nhien trong zone
// ============================================================
Herb_Init()
{
    new count = 0, attempts = 0;
    new Float:x, Float:y, Float:z;

    for(new i = 0; i < HERB_MAX_PLANTS; i++)
    {
        gHerbActive[i]   = false;
        gHerbOccupied[i] = false;
        gHerbPlayer[i]   = -1;
        gHerbObj[i]       = -1;
    }

    new rangeX = floatround((HERB_ZONE_MAXX - HERB_ZONE_MINX) * 100.0);
    new rangeY = floatround((HERB_ZONE_MAXY - HERB_ZONE_MINY) * 100.0);

    while(count < HERB_MAX_PLANTS && attempts < 20000)
    {
        attempts++;
        x = HERB_ZONE_MINX + float(random(rangeX)) / 100.0;
        y = HERB_ZONE_MINY + float(random(rangeY)) / 100.0;
        if(!CA_FindZ_For2DCoord(x, y, z)) continue;
        Herb_SpawnPlantAt(count, x, y, z);
        count++;
    }

    printf("[HerbSystem] %d/%d thao duoc da spawn.", count, HERB_MAX_PLANTS);
    return 1;
}

// ============================================================
// HARVEST - Bat dau hai
// ============================================================
static stock Herb_StartHarvest(playerid, plantIndex)
{
    gHerbOccupied[plantIndex] = true;
    gHerbPlayer[plantIndex]   = playerid;
    gPlayerHarvestHerb[playerid] = plantIndex;

    ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.1, 1, 0, 0, 1, 0, 1);
    SetLoading(playerid, HERB_HARVEST_TIME * 1000, "Dang hai thao duoc...", "Herb_HarvestFinish");

    new string[128];
    format(string, sizeof(string), "{FF8000}* {C2A2DA}%s dang hai thao duoc.", GetPlayerNameEx(playerid));
    ProxDetector(30.0, playerid, string, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
    return 1;
}

// ============================================================
// HARVEST - Hoan thanh
// ============================================================
public Herb_HarvestFinish(playerid)
{
    if(!IsPlayerConnected(playerid)) return 1;

    new plantIndex = gPlayerHarvestHerb[playerid];
    gPlayerHarvestHerb[playerid] = -1;

    if(plantIndex < 0 || plantIndex >= HERB_MAX_PLANTS) return 1;

    // Giai phong cay
    gHerbOccupied[plantIndex] = false;
    gHerbPlayer[plantIndex]   = -1;

    // Check vi tri - neu roi xa thi that bai
    if(!IsPlayerInRangeOfPoint(playerid, HERB_FAIL_RANGE, gHerbX[plantIndex], gHerbY[plantIndex], gHerbZ[plantIndex]))
    {
        ClearAnimations(playerid);
        ShowTDN(playerid, NOTIFICATION_ERROR, "Hai thao duoc that bai vi ban di qua xa.");
        SendClientMessageEx(playerid, COLOR_RED, "{00CC00}[Thao Duoc] {ffffff}Ban da di chuyen qua xa, hai thao duoc that bai.");
        return 1;
    }

    // Xoa cay, dat timer respawn
    if(gHerbObj[plantIndex] != -1)
    {
        DestroyDynamicObject(gHerbObj[plantIndex]);
        gHerbObj[plantIndex] = -1;
    }
    gHerbActive[plantIndex] = false;
    SetTimerEx("Herb_RespawnPlant", HERB_RESPAWN_TIME, false, "d", plantIndex);

    // Cho player thao duoc
    ClearAnimations(playerid);
    new herbAmount = 1 + random(2); // random 1-2
    PlayerInfo[playerid][pHerb] += herbAmount;

    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{00CC00}[Thao Duoc] {ffffff}Ban da hai duoc %d thao duoc! (Tong: %d)", herbAmount, PlayerInfo[playerid][pHerb]);
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Hai thao duoc thanh cong.");

    // XP
    new rand_xp = Random(PlayerInfo[playerid][pLevel] * XP_RATE - 7, PlayerInfo[playerid][pLevel] * XP_RATE - 4);
    if(DoubleXP || PlayerInfo[playerid][pDoubleEXP] > 0) {
        SendClientMessageEx(playerid, COLOR_YELLOW, "Kinh nghiem nhan vat: +%d XP (%s/%s) (DOUBLE XP)",
            rand_xp * 2, number_format(PlayerInfo[playerid][pXP] + rand_xp * 2), number_format(GetRequiredXP(playerid)));
        GivePlayerXP(playerid, rand_xp * 2);
    } else {
        SendClientMessageEx(playerid, COLOR_YELLOW, "Kinh nghiem nhan vat: +%d XP (%s/%s)",
            rand_xp, number_format(PlayerInfo[playerid][pXP] + rand_xp), number_format(GetRequiredXP(playerid)));
        GivePlayerXP(playerid, rand_xp);
    }

    new proxstr[128];
    format(proxstr, sizeof(proxstr), "{FF8000}* {C2A2DA}%s da hai duoc mot cay thao duoc.", GetPlayerNameEx(playerid));
    ProxDetector(30.0, playerid, proxstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
    return 1;
}

// ============================================================
// RESPAWN
// ============================================================
public Herb_RespawnPlant(plantIndex)
{
    if(plantIndex < 0 || plantIndex >= HERB_MAX_PLANTS) return 1;
    gHerbObj[plantIndex] = CreateDynamicObject(HERB_OBJECT_MODEL,
        gHerbX[plantIndex], gHerbY[plantIndex], gHerbZ[plantIndex] - 0.5,
        0.0, 0.0, 0.0, .worldid = 0, .streamdistance = 700.0);
    gHerbActive[plantIndex] = true;
    return 1;
}

// ============================================================
// HOOKS
// ============================================================
hook OnPlayerConnect(playerid)
{
    gPlayerHarvestHerb[playerid] = -1;
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    // Giai phong cay neu dang hai
    new plantIndex = gPlayerHarvestHerb[playerid];
    if(plantIndex >= 0 && plantIndex < HERB_MAX_PLANTS)
    {
        gHerbOccupied[plantIndex] = false;
        gHerbPlayer[plantIndex]   = -1;
    }
    gPlayerHarvestHerb[playerid] = -1;
    return 1;
}

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if(!PRESSED(KEY_YES)) return 1;
    if(IsPlayerInAnyVehicle(playerid)) return 1;
    if(IsLoadingActive(playerid)) return 1;
    if(gPlayerHarvestHerb[playerid] != -1) return 1;

    // Check neu player dang bi cong tay / bi thuong
    if(PlayerCuffed[playerid] >= 1 || GetPVarInt(playerid, "Injured") == 1) return 1;

    // Tim thao duoc gan nhat
    new plant = Herb_FindNearestPlant(playerid);
    if(plant != -1)
    {
        Herb_StartHarvest(playerid, plant);
        return 1;
    }
    return 1;
}

// ============================================================
// COMMANDS
// ============================================================
CMD:thaoduoc(playerid, params[])
{
    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{00CC00}[Thao Duoc] {ffffff}Ban dang co: {FFFF00}%d{ffffff} thao duoc trong nguoi.", PlayerInfo[playerid][pHerb]);
    return 1;
}

// GPS den cay thao duoc con song
CMD:vitritd(playerid, params[])
{
    SetPlayerCP(playerid, -1710.738403, -2102.775878, 40.563346, 3.0);
    gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "{00CC00}[GPS] {ffffff}Dang dan duong den noi moc thao duoc.");
    return 1;
}
