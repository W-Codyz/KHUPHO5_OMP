// gamemodes/modules/system/help_button.inc
// He thong goi y phim tuong tac: house / door / vehicle / actor
// PlayerTextDraw: chi create khi player buoc vao area, destroy khi roi het.

#include <YSI\YSI_Coding\y_hooks>

#if !defined DynArea
    #define DynArea STREAMER_TAG_AREA
#endif
#if !defined INVALID_DYNAMIC_AREA_ID
    #define INVALID_DYNAMIC_AREA_ID -1
#endif

// ===================== KEY / ACTION INDICES =====================

#define HINT_KEY_Y      0
#define HINT_KEY_ALT    1

#define HINT_ACT_ENTER_HOUSE    0   // "de vao nha"
#define HINT_ACT_EXIT_HOUSE     1   // "de ra khoi nha"
#define HINT_ACT_LOCK_HOUSE     2   // "de mo/khoa nha"
#define HINT_ACT_TALK           3   // "de noi chuyen"
#define HINT_ACT_ENTER_DOOR     4   // "de vao"
#define HINT_ACT_EXIT_DOOR      5   // "de ra"
#define HINT_ACT_LOCK_CAR       6   // "de khoa/mo xe"

static const g_szHintKeys[2][]    = {"Y", "ALT"};
static const g_szHintActions[7][] = {"de vao nha", "de ra khoi nha", "de mo/khoa nha", "de noi chuyen", "de vao trong", "de ra ngoai", "de khoa/mo xe"};

// ===================== PER-PLAYER TEXTDRAWS (lazy) =====================

new PlayerText:g_HintBgOuter[MAX_PLAYERS];
new PlayerText:g_HintBgInner[MAX_PLAYERS];
new PlayerText:g_HintTDKey[MAX_PLAYERS];
new PlayerText:g_HintTDAction[MAX_PLAYERS];
new PlayerText:g_HintTDPress[MAX_PLAYERS];

new bool:g_HintCreated[MAX_PLAYERS]; // true khi TDs dang ton tai
new g_HintCurKey[MAX_PLAYERS];       // index dang hien, -1 = an
new g_HintCurAct[MAX_PLAYERS];
new g_HintHideTimer[MAX_PLAYERS];    // timer tu dong an sau 5s, -1 = khong co

static HintTD_Create(playerid)
{
    g_HintBgOuter[playerid] = CreatePlayerTextDraw(playerid, 300.000, 323.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid,       g_HintBgOuter[playerid], 35.000, 37.000);
    PlayerTextDrawAlignment(playerid,      g_HintBgOuter[playerid], 1);
    PlayerTextDrawColor(playerid,          g_HintBgOuter[playerid], -1061109610);
    PlayerTextDrawSetShadow(playerid,      g_HintBgOuter[playerid], 0);
    PlayerTextDrawSetOutline(playerid,     g_HintBgOuter[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid,g_HintBgOuter[playerid], 255);
    PlayerTextDrawFont(playerid,           g_HintBgOuter[playerid], 4);
    PlayerTextDrawSetProportional(playerid,g_HintBgOuter[playerid], 1);

    g_HintBgInner[playerid] = CreatePlayerTextDraw(playerid, 304.500, 326.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid,       g_HintBgInner[playerid], 26.000, 30.000);
    PlayerTextDrawAlignment(playerid,      g_HintBgInner[playerid], 1);
    PlayerTextDrawColor(playerid,          g_HintBgInner[playerid], -1061109560);
    PlayerTextDrawSetShadow(playerid,      g_HintBgInner[playerid], 0);
    PlayerTextDrawSetOutline(playerid,     g_HintBgInner[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid,g_HintBgInner[playerid], 255);
    PlayerTextDrawFont(playerid,           g_HintBgInner[playerid], 4);
    PlayerTextDrawSetProportional(playerid,g_HintBgInner[playerid], 1);

    g_HintTDKey[playerid] = CreatePlayerTextDraw(playerid, 318.000, 334.000, "Y");
    PlayerTextDrawLetterSize(playerid,     g_HintTDKey[playerid], 0.300, 1.500);
    PlayerTextDrawAlignment(playerid,      g_HintTDKey[playerid], 2);
    PlayerTextDrawColor(playerid,          g_HintTDKey[playerid], -1);
    PlayerTextDrawSetShadow(playerid,      g_HintTDKey[playerid], 1);
    PlayerTextDrawSetOutline(playerid,     g_HintTDKey[playerid], 1);
    PlayerTextDrawBackgroundColor(playerid,g_HintTDKey[playerid], 150);
    PlayerTextDrawFont(playerid,           g_HintTDKey[playerid], 1);
    PlayerTextDrawSetProportional(playerid,g_HintTDKey[playerid], 1);

    g_HintTDAction[playerid] = CreatePlayerTextDraw(playerid, 338.000, 334.000, "de tuong tac");
    PlayerTextDrawLetterSize(playerid,     g_HintTDAction[playerid], 0.300, 1.500);
    PlayerTextDrawAlignment(playerid,      g_HintTDAction[playerid], 1);
    PlayerTextDrawColor(playerid,          g_HintTDAction[playerid], -1);
    PlayerTextDrawSetShadow(playerid,      g_HintTDAction[playerid], 1);
    PlayerTextDrawSetOutline(playerid,     g_HintTDAction[playerid], 1);
    PlayerTextDrawBackgroundColor(playerid,g_HintTDAction[playerid], 150);
    PlayerTextDrawFont(playerid,           g_HintTDAction[playerid], 1);
    PlayerTextDrawSetProportional(playerid,g_HintTDAction[playerid], 1);

    g_HintTDPress[playerid] = CreatePlayerTextDraw(playerid, 296.000, 334.000, "Nhan");
    PlayerTextDrawLetterSize(playerid,     g_HintTDPress[playerid], 0.300, 1.500);
    PlayerTextDrawAlignment(playerid,      g_HintTDPress[playerid], 3);
    PlayerTextDrawColor(playerid,          g_HintTDPress[playerid], -1);
    PlayerTextDrawSetShadow(playerid,      g_HintTDPress[playerid], 1);
    PlayerTextDrawSetOutline(playerid,     g_HintTDPress[playerid], 1);
    PlayerTextDrawBackgroundColor(playerid,g_HintTDPress[playerid], 150);
    PlayerTextDrawFont(playerid,           g_HintTDPress[playerid], 1);
    PlayerTextDrawSetProportional(playerid,g_HintTDPress[playerid], 1);

    g_HintCreated[playerid] = true;
}

static HintTD_Destroy(playerid)
{
    PlayerTextDrawDestroy(playerid, g_HintBgOuter[playerid]);  g_HintBgOuter[playerid]  = PlayerText:INVALID_TEXT_DRAW;
    PlayerTextDrawDestroy(playerid, g_HintBgInner[playerid]);  g_HintBgInner[playerid]  = PlayerText:INVALID_TEXT_DRAW;
    PlayerTextDrawDestroy(playerid, g_HintTDKey[playerid]);    g_HintTDKey[playerid]    = PlayerText:INVALID_TEXT_DRAW;
    PlayerTextDrawDestroy(playerid, g_HintTDAction[playerid]); g_HintTDAction[playerid] = PlayerText:INVALID_TEXT_DRAW;
    PlayerTextDrawDestroy(playerid, g_HintTDPress[playerid]);  g_HintTDPress[playerid]  = PlayerText:INVALID_TEXT_DRAW;
    g_HintCreated[playerid] = false;
}

// ===================== SHOW / HIDE =====================

forward HintKey_AutoHide(playerid);
public HintKey_AutoHide(playerid)
{
    g_HintHideTimer[playerid] = -1; // timer da fire, xoa ID truoc
    HintKey_Hide(playerid);
}

stock HintKey_Show(playerid, keyIdx, actionIdx)
{
    if(!g_HintCreated[playerid])
        HintTD_Create(playerid);

    PlayerTextDrawSetString(playerid, g_HintTDKey[playerid],    g_szHintKeys[keyIdx]);
    PlayerTextDrawSetString(playerid, g_HintTDAction[playerid], g_szHintActions[actionIdx]);

    PlayerTextDrawShow(playerid, g_HintBgOuter[playerid]);
    PlayerTextDrawShow(playerid, g_HintBgInner[playerid]);
    PlayerTextDrawShow(playerid, g_HintTDKey[playerid]);
    PlayerTextDrawShow(playerid, g_HintTDAction[playerid]);
    PlayerTextDrawShow(playerid, g_HintTDPress[playerid]);

    g_HintCurKey[playerid] = keyIdx;
    g_HintCurAct[playerid] = actionIdx;

    // Khoi dong lai timer an 5 giay (huy timer cu neu dang chay)
    if(g_HintHideTimer[playerid] != -1) KillTimer(g_HintHideTimer[playerid]);
    g_HintHideTimer[playerid] = SetTimerEx("HintKey_AutoHide", 5000, false, "d", playerid);
}

stock HintKey_Hide(playerid)
{
    if(g_HintHideTimer[playerid] != -1)
    {
        KillTimer(g_HintHideTimer[playerid]);
        g_HintHideTimer[playerid] = -1;
    }
    if(!g_HintCreated[playerid]) return;
    HintTD_Destroy(playerid);
    g_HintCurKey[playerid] = -1;
    g_HintCurAct[playerid] = -1;
}

// ===================== AREA TRACKING =====================

new DynArea:g_AreaHouseExt[MAX_HOUSES];
new DynArea:g_AreaHouseInt[MAX_HOUSES];
new DynArea:g_AreaActor[MAX_SERVER_ACTORS];

new g_NearHouse[MAX_PLAYERS];
new g_NearHouseInt[MAX_PLAYERS];
new g_NearActor[MAX_PLAYERS];
new g_NearDoor[MAX_PLAYERS];     // door ID near exterior, -1 if none
new g_NearDoorInt[MAX_PLAYERS];  // door ID near interior, -1 if none
new g_NearVehicle[MAX_PLAYERS];  // PlayerVehicleInfo slot, -1 if none

// Compact door area list — avoids sparse MAX_DDOORS array
#define MAX_DOOR_HINT_AREAS 1024

static DynArea:s_DoorArea[MAX_DOOR_HINT_AREAS];
static s_DoorAreaDoorIdx[MAX_DOOR_HINT_AREAS];
static bool:s_DoorAreaIsInt[MAX_DOOR_HINT_AREAS];
static s_DoorAreaCount;

// ===================== DOOR AREA HELPERS =====================

static HintArea_FindDoorSlot(doorid, bool:isInt)
{
    for(new i = 0; i < s_DoorAreaCount; i++)
        if(s_DoorAreaDoorIdx[i] == doorid && s_DoorAreaIsInt[i] == isInt)
            return i;
    return -1;
}

static HintArea_RemoveDoorSlot(slotidx)
{
    if(slotidx < 0 || slotidx >= s_DoorAreaCount) return;
    DestroyDynamicArea(s_DoorArea[slotidx]);
    s_DoorAreaCount--;
    if(slotidx != s_DoorAreaCount)
    {
        s_DoorArea[slotidx]        = s_DoorArea[s_DoorAreaCount];
        s_DoorAreaDoorIdx[slotidx] = s_DoorAreaDoorIdx[s_DoorAreaCount];
        s_DoorAreaIsInt[slotidx]   = s_DoorAreaIsInt[s_DoorAreaCount];
    }
    s_DoorArea[s_DoorAreaCount] = DynArea:INVALID_DYNAMIC_AREA_ID;
}

// Tao/cap nhat area cho door — goi tu CreateDynamicDoor trong functions.inc
stock HintArea_SetDoor(doorid)
{
    if(doorid < 0 || doorid >= MAX_DDOORS) return;

    new extSlot = HintArea_FindDoorSlot(doorid, false);
    if(extSlot != -1)
    {
        foreach(new i : Player)
            if(g_NearDoor[i] == doorid) { g_NearDoor[i] = -1; HintKey_Recalc(i); }
        HintArea_RemoveDoorSlot(extSlot);
    }

    new intSlot = HintArea_FindDoorSlot(doorid, true);
    if(intSlot != -1)
    {
        foreach(new i : Player)
            if(g_NearDoorInt[i] == doorid) { g_NearDoorInt[i] = -1; HintKey_Recalc(i); }
        HintArea_RemoveDoorSlot(intSlot);
    }

    if(DDoorsInfo[doorid][ddExteriorX] == 0.0) return;

    if(s_DoorAreaCount < MAX_DOOR_HINT_AREAS)
    {
        s_DoorArea[s_DoorAreaCount]        = CreateDynamicSphere(
            DDoorsInfo[doorid][ddExteriorX], DDoorsInfo[doorid][ddExteriorY], DDoorsInfo[doorid][ddExteriorZ],
            1.0,
            .worldid    = DDoorsInfo[doorid][ddExteriorVW],
            .interiorid = DDoorsInfo[doorid][ddExteriorInt]);
        s_DoorAreaDoorIdx[s_DoorAreaCount] = doorid;
        s_DoorAreaIsInt[s_DoorAreaCount]   = false;
        s_DoorAreaCount++;
    }

    if(DDoorsInfo[doorid][ddInteriorX] != 0.0 && s_DoorAreaCount < MAX_DOOR_HINT_AREAS)
    {
        s_DoorArea[s_DoorAreaCount]        = CreateDynamicSphere(
            DDoorsInfo[doorid][ddInteriorX], DDoorsInfo[doorid][ddInteriorY], DDoorsInfo[doorid][ddInteriorZ],
            1.0,
            .worldid    = DDoorsInfo[doorid][ddInteriorVW],
            .interiorid = DDoorsInfo[doorid][ddInteriorInt]);
        s_DoorAreaDoorIdx[s_DoorAreaCount] = doorid;
        s_DoorAreaIsInt[s_DoorAreaCount]   = true;
        s_DoorAreaCount++;
    }
}

// Xoa area cho door — goi tu DestroyDynamicDoorObjects trong functions.inc
stock HintArea_ClearDoor(doorid)
{
    if(doorid < 0 || doorid >= MAX_DDOORS) return;

    foreach(new i : Player)
    {
        if(g_NearDoor[i] == doorid)    { g_NearDoor[i] = -1; HintKey_Recalc(i); }
        if(g_NearDoorInt[i] == doorid) { g_NearDoorInt[i] = -1; HintKey_Recalc(i); }
    }

    // Iterate backwards so swap-with-last removal stays correct
    for(new i = s_DoorAreaCount - 1; i >= 0; i--)
        if(s_DoorAreaDoorIdx[i] == doorid)
            HintArea_RemoveDoorSlot(i);
}

// ===================== RECALC (priority: actor > house ext > door ext > house int > door int > vehicle) =====================

stock HintKey_Recalc(playerid)
{
    /* if(g_NearActor[playerid] != -1)
    {
        HintKey_Show(playerid, HINT_KEY_Y, HINT_ACT_TALK);
        return;
    } */
    if(g_NearHouse[playerid] != -1)
    {
        new h = g_NearHouse[playerid];
        if(!HouseInfo[h][hLock])
        {
            HintKey_Show(playerid, HINT_KEY_Y, HINT_ACT_ENTER_HOUSE);
            return;
        }
        if(IsHouseOwnerSQL(playerid, h))
        {
            HintKey_Show(playerid, HINT_KEY_ALT, HINT_ACT_LOCK_HOUSE);
            return;
        }
    }
    if(g_NearDoor[playerid] != -1)
    {
        HintKey_Show(playerid, HINT_KEY_Y, HINT_ACT_ENTER_DOOR);
        return;
    }
    if(g_NearHouseInt[playerid] != -1)
    {
        new h = g_NearHouseInt[playerid];
        if(HouseInfo[h][hLock] && IsHouseOwnerSQL(playerid, h))
            HintKey_Show(playerid, HINT_KEY_ALT, HINT_ACT_LOCK_HOUSE);
        else
            HintKey_Show(playerid, HINT_KEY_Y, HINT_ACT_EXIT_HOUSE);
        return;
    }
    if(g_NearDoorInt[playerid] != -1)
    {
        HintKey_Show(playerid, HINT_KEY_Y, HINT_ACT_EXIT_DOOR);
        return;
    }
    if(g_NearVehicle[playerid] != -1)
    {
        HintKey_Show(playerid, HINT_KEY_Y, HINT_ACT_LOCK_CAR);
        return;
    }
    HintKey_Hide(playerid);
}

// Goi sau khi thay doi trang thai khoa de cap nhat hint cho player gan nha
stock HintKey_RefreshHouse(houseid)
{
    foreach(new i : Player)
        if(g_NearHouse[i] == houseid) HintKey_Recalc(i);
}

// Tao/cap nhat area cho house — goi tu House_ReloadPickupLabel trong house.inc
stock HintArea_SetHouse(houseid)
{
    if(houseid < 0 || houseid >= MAX_HOUSES) return;

    if(g_AreaHouseExt[houseid] != DynArea:INVALID_DYNAMIC_AREA_ID)
    {
        foreach(new i : Player)
            if(g_NearHouse[i] == houseid) { g_NearHouse[i] = -1; HintKey_Recalc(i); }
        DestroyDynamicArea(g_AreaHouseExt[houseid]);
        g_AreaHouseExt[houseid] = DynArea:INVALID_DYNAMIC_AREA_ID;
    }
    if(g_AreaHouseInt[houseid] != DynArea:INVALID_DYNAMIC_AREA_ID)
    {
        foreach(new i : Player)
            if(g_NearHouseInt[i] == houseid) { g_NearHouseInt[i] = -1; HintKey_Recalc(i); }
        DestroyDynamicArea(g_AreaHouseInt[houseid]);
        g_AreaHouseInt[houseid] = DynArea:INVALID_DYNAMIC_AREA_ID;
    }

    if(HouseInfo[houseid][hExteriorX] == 0.0) return;

    g_AreaHouseExt[houseid] = CreateDynamicSphere(
        HouseInfo[houseid][hExteriorX], HouseInfo[houseid][hExteriorY], HouseInfo[houseid][hExteriorZ],
        1.0,
        .worldid    = HouseInfo[houseid][hExtVW],
        .interiorid = HouseInfo[houseid][hExtIW]);

    if(HouseInfo[houseid][hInteriorX] != 0.0)
        g_AreaHouseInt[houseid] = CreateDynamicSphere(
            HouseInfo[houseid][hInteriorX], HouseInfo[houseid][hInteriorY], HouseInfo[houseid][hInteriorZ],
            1.0,
            .worldid    = HouseInfo[houseid][hIntVW],
            .interiorid = HouseInfo[houseid][hIntIW]);
}

/* static HintArea_CreateActors()
{
    for(new i = 0; i < MAX_SERVER_ACTORS; i++)
    {
        if(ActorInfo[i][actorX] == 1.0 && ActorInfo[i][actorY] == 2.0 && ActorInfo[i][actorZ] == 3.0) continue;
        g_AreaActor[i] = CreateDynamicSphere(
            ActorInfo[i][actorX], ActorInfo[i][actorY], ActorInfo[i][actorZ],
            1.0,
            .worldid    = ActorInfo[i][actorVW],
            .interiorid = ActorInfo[i][actorInt]);
    }
} */

// ===================== VEHICLE PROXIMITY TIMER =====================

task HintVehicleCheck[500]()
{
    new Float:vx, Float:vy, Float:vz;
    foreach(new playerid : Player)
    {
        if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT)
        {
            if(g_NearVehicle[playerid] != -1)
            {
                g_NearVehicle[playerid] = -1;
                HintKey_Recalc(playerid);
            }
            continue;
        }
        new nearSlot = -1;
        for(new d = 0; d < MAX_PLAYERVEHICLES; d++)
        {
            if(PlayerVehicleInfo[playerid][d][pvId] == INVALID_PLAYER_VEHICLE_ID) continue;
            if(!PlayerVehicleInfo[playerid][d][pvSpawned]) continue;
            GetVehiclePos(PlayerVehicleInfo[playerid][d][pvId], vx, vy, vz);
            if(IsPlayerInRangeOfPoint(playerid, 1.0, vx, vy, vz))
            {
                nearSlot = d;
                break;
            }
        }
        if(nearSlot != g_NearVehicle[playerid])
        {
            g_NearVehicle[playerid] = nearSlot;
            HintKey_Recalc(playerid);
        }
    }
}

// ===================== HOOKS =====================

hook OnGameModeInit()
{
    for(new i = 0; i < MAX_HOUSES; i++)
    {
        g_AreaHouseExt[i] = DynArea:INVALID_DYNAMIC_AREA_ID;
        g_AreaHouseInt[i] = DynArea:INVALID_DYNAMIC_AREA_ID;
    }
    for(new i = 0; i < MAX_SERVER_ACTORS; i++)
        g_AreaActor[i] = DynArea:INVALID_DYNAMIC_AREA_ID;
    for(new i = 0; i < MAX_DOOR_HINT_AREAS; i++)
        s_DoorArea[i] = DynArea:INVALID_DYNAMIC_AREA_ID;
    s_DoorAreaCount = 0;
    //HintArea_CreateActors();
    return 1;
}

hook OnPlayerConnect(playerid)
{
    g_NearHouse[playerid]    = -1;
    g_NearHouseInt[playerid] = -1;
    g_NearActor[playerid]    = -1;
    g_NearDoor[playerid]     = -1;
    g_NearDoorInt[playerid]  = -1;
    g_NearVehicle[playerid]  = -1;
    g_HintCreated[playerid]  = false;
    g_HintCurKey[playerid]   = -1;
    g_HintCurAct[playerid]   = -1;
    g_HintHideTimer[playerid] = -1;
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    if(g_HintHideTimer[playerid] != -1)
    {
        KillTimer(g_HintHideTimer[playerid]);
        g_HintHideTimer[playerid] = -1;
    }
    if(g_HintCreated[playerid])
        HintTD_Destroy(playerid);
    g_NearHouse[playerid]    = -1;
    g_NearHouseInt[playerid] = -1;
    g_NearActor[playerid]    = -1;
    g_NearDoor[playerid]     = -1;
    g_NearDoorInt[playerid]  = -1;
    g_NearVehicle[playerid]  = -1;
    g_HintCurKey[playerid]   = -1;
    g_HintCurAct[playerid]   = -1;
    return 1;
}

/* hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if((newkeys & KEY_WALK) && g_HintCurAct[playerid] == HINT_ACT_LOCK_CAR && g_NearVehicle[playerid] != -1)
    {
        new slot = g_NearVehicle[playerid];
        new vehicleid = PlayerVehicleInfo[playerid][slot][pvId];
        if(vehicleid != INVALID_PLAYER_VEHICLE_ID && PlayerVehicleInfo[playerid][slot][pvSpawned])
        {
            new Float:x, Float:y, Float:z;
            GetVehiclePos(vehicleid, x, y, z);
            if(PlayerVehicleInfo[playerid][slot][pvLocked] == 0)
            {
                HienTextdraw(playerid, "~r~DA KHOA!");
                PlayerVehicleInfo[playerid][slot][pvLocked] = 1;
                LockPlayerVehicle(playerid, vehicleid, 1);
            }
            else
            {
                HienTextdraw(playerid, "~g~MO KHOA!");
                PlayerVehicleInfo[playerid][slot][pvLocked] = 0;
                UnLockPlayerVehicle(playerid, vehicleid, 1);
            }
            foreach(new i : Player)
                if(IsPlayerInRangeOfPoint(i, 10.0, x, y, z))
                    PlayerPlaySound(i, 24600, x, y, z);
        }
    }
    return 1;
} */

hook OnPlayerEnterDynamicArea(playerid, DynArea:areaid)
{
    /* for(new i = 0; i < MAX_SERVER_ACTORS; i++)
    {
        if(g_AreaActor[i] == areaid)
        {
            g_NearActor[playerid] = i;
            HintKey_Recalc(playerid);
            return 1;
        }
    } */
    for(new i = 0; i < MAX_HOUSES; i++)
    {
        if(g_AreaHouseExt[i] == areaid)
        {
            g_NearHouse[playerid] = i;
            HintKey_Recalc(playerid);
            return 1;
        }
        if(g_AreaHouseInt[i] == areaid)
        {
            g_NearHouseInt[playerid] = i;
            HintKey_Recalc(playerid);
            return 1;
        }
    }
    for(new i = 0; i < s_DoorAreaCount; i++)
    {
        if(s_DoorArea[i] == areaid)
        {
            if(s_DoorAreaIsInt[i])
                g_NearDoorInt[playerid] = s_DoorAreaDoorIdx[i];
            else
                g_NearDoor[playerid] = s_DoorAreaDoorIdx[i];
            HintKey_Recalc(playerid);
            return 1;
        }
    }
    return 1;
}

hook OnPlayerLeaveDynamicArea(playerid, DynArea:areaid)
{
    for(new i = 0; i < MAX_SERVER_ACTORS; i++)
    {
        if(g_AreaActor[i] == areaid)
        {
            g_NearActor[playerid] = -1;
            HintKey_Recalc(playerid);
            return 1;
        }
    }
    for(new i = 0; i < MAX_HOUSES; i++)
    {
        if(g_AreaHouseExt[i] == areaid)
        {
            g_NearHouse[playerid] = -1;
            HintKey_Recalc(playerid);
            return 1;
        }
        if(g_AreaHouseInt[i] == areaid)
        {
            g_NearHouseInt[playerid] = -1;
            HintKey_Recalc(playerid);
            return 1;
        }
    }
    for(new i = 0; i < s_DoorAreaCount; i++)
    {
        if(s_DoorArea[i] == areaid)
        {
            if(s_DoorAreaIsInt[i])
                g_NearDoorInt[playerid] = -1;
            else
                g_NearDoor[playerid] = -1;
            HintKey_Recalc(playerid);
            return 1;
        }
    }
    return 1;
}
