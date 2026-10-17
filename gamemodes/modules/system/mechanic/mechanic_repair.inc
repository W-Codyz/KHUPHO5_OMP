/*
    Mechanic Repair, Crafting & Storage System
    KP5 Roleplay - Created by Codyz

    ============================================================
    CAN THEM VAO ENUM PLAYERINFO (enums.inc):
        pToolkitOto,        // int - so luong dung cu sua oto
        pToolkitXeMay,      // int - so luong dung cu sua xe may

    VA THEM SAVE/LOAD cho 2 bien nay trong player save system.
    ============================================================
    LUU Y: /sualuaxe cu trong mechanic.inc nen comment lai hoac xoa,
    vi /suaxe moi se thay the.
    ============================================================
*/

#include <YSI\YSI_Coding\y_hooks>

// ============================================================
// DEFINES
// ============================================================
#define MAX_REPAIR_SPOTS        20
#define MAX_CRAFT_SPOTS         10
#define MAX_STORAGE_SPOTS       10
#define MAX_MECH_LOG_ENTRIES    50

#define MECH_REPAIR_TYPE        0   // loai vi tri: sua xe
#define MECH_CRAFT_TYPE         1   // loai vi tri: che tao
#define MECH_STORAGE_TYPE       2   // loai vi tri: kho

// Dialog IDs
#define DIALOG_MECH_SUAXE       5400
#define DIALOG_MECH_CHETAO      5401
#define DIALOG_MECH_KHO         5402
#define DIALOG_MECH_KHO_TIEN_GUI    5403
#define DIALOG_MECH_KHO_TIEN_RUT    5404
#define DIALOG_MECH_KHO_DC_MENU     5405
#define DIALOG_MECH_KHO_DC_GUI      5406
#define DIALOG_MECH_KHO_DC_RUT      5407
#define DIALOG_MECH_KHO_LOG         5408
#define DIALOG_MECH_KHO_LOG_TIEN    5409

// Admin dialogs
#define DIALOG_ADMIN_REPAIR_LIST    5420
#define DIALOG_ADMIN_REPAIR_ADD     5421
#define DIALOG_ADMIN_CRAFT_LIST     5422
#define DIALOG_ADMIN_CRAFT_ADD      5423
#define DIALOG_ADMIN_STORAGE_LIST   5424
#define DIALOG_ADMIN_STORAGE_ADD    5425
#define DIALOG_ADMIN_STORAGE_GRP    5426
#define DIALOG_ADMIN_MECH_MENU      5430
#define DIALOG_ADMIN_MECH_ADD_GID   5431
#define DIALOG_ADMIN_MECH_SETPOS_ID 5432

#define DIALOG_MECH_KHO_LOG_DC      5433
#define DIALOG_MECH_KHO_DC_GUI_SL   5434
#define DIALOG_MECH_KHO_DC_RUT_SL   5435

// Craft costs
#define CRAFT_OTO_SAT           15
#define CRAFT_OTO_DONG          5
#define CRAFT_OTO_MATS          3000

#define CRAFT_XEMAY_SAT         20
#define CRAFT_XEMAY_DONG        4
#define CRAFT_XEMAY_MATS        2000

// ============================================================
// VARIABLES - Repair Spots
// ============================================================
new gRepairName[MAX_REPAIR_SPOTS][48];
new Float:gRepairX[MAX_REPAIR_SPOTS];
new Float:gRepairY[MAX_REPAIR_SPOTS];
new Float:gRepairZ[MAX_REPAIR_SPOTS];
new gRepairGroupID[MAX_REPAIR_SPOTS];
new Text3D:gRepairLabel[MAX_REPAIR_SPOTS] = {Text3D:INVALID_3DTEXT_ID, ...};
new gRepairPickup[MAX_REPAIR_SPOTS] = {-1, ...};
new gRepairCount = 0;

// ============================================================
// VARIABLES - Craft Spots
// ============================================================
new gCraftName[MAX_CRAFT_SPOTS][48];
new Float:gCraftX[MAX_CRAFT_SPOTS];
new Float:gCraftY[MAX_CRAFT_SPOTS];
new Float:gCraftZ[MAX_CRAFT_SPOTS];
new gCraftGroupID[MAX_CRAFT_SPOTS];
new Text3D:gCraftLabel[MAX_CRAFT_SPOTS] = {Text3D:INVALID_3DTEXT_ID, ...};
new gCraftPickup[MAX_CRAFT_SPOTS] = {-1, ...};
new gCraftCount = 0;

// ============================================================
// VARIABLES - Storage Spots
// ============================================================
new gStorageName[MAX_STORAGE_SPOTS][48];
new Float:gStorageX[MAX_STORAGE_SPOTS];
new Float:gStorageY[MAX_STORAGE_SPOTS];
new Float:gStorageZ[MAX_STORAGE_SPOTS];
new gStorageGroupID[MAX_STORAGE_SPOTS];
new Text3D:gStorageLabel[MAX_STORAGE_SPOTS] = {Text3D:INVALID_3DTEXT_ID, ...};
new gStoragePickup[MAX_STORAGE_SPOTS] = {-1, ...};
new gStorageCount = 0;

// ============================================================
// VARIABLES - Group Mechanic Data (per group, loaded on demand)
// ============================================================
// Luu tru tam trong RAM, save/load tu file
#define MAX_MECH_GROUPS 50
new gMechGroupMoney[MAX_MECH_GROUPS];
new gMechGroupToolOto[MAX_MECH_GROUPS];
new gMechGroupToolXeMay[MAX_MECH_GROUPS];
new bool:gMechGroupLoaded[MAX_MECH_GROUPS];

// ============================================================
// PLAYER TEMP STATE (PVars)
// ============================================================
// "MechRepairVeh"  - vehicleid dang sua
// "MechCraftType"  - 0=oto, 1=xemay
// "MechBusy"       - 1=dang sua/chetao

// ============================================================
// FORWARD DECLARATIONS
// ============================================================
forward OnMechanicRepairDone(playerid);
forward OnMechanicCraftDone(playerid);

// ============================================================
// HELPERS - 3DTextLabel & Pickup
// ============================================================
static CreateRepairWorldObjects(i)
{
    new label[256];
    format(label, sizeof(label),
        "{FF6600}[Tru Sua Xe]\n{FFFFFF}CONG TY: {%s}%s\n{FFFFFF}Lenh: {F6F08D}/suaxe",
        Group_NumToDialogHex(arrGroupData[gRepairGroupID[i]][g_hDutyColour]),
        gRepairName[i]);
    gRepairLabel[i] = CreateDynamic3DTextLabel(label, 0xFFFFFFFF,
        gRepairX[i], gRepairY[i], gRepairZ[i] + 0.5, 20.0);
    gRepairPickup[i] = CreateDynamicPickup(1239, 23,
        gRepairX[i], gRepairY[i], gRepairZ[i]);
}

static DestroRepairWorldObjects(i)
{
    if(gRepairLabel[i] != Text3D:INVALID_3DTEXT_ID) {
        DestroyDynamic3DTextLabel(gRepairLabel[i]);
        gRepairLabel[i] = Text3D:INVALID_3DTEXT_ID;
    }
    if(gRepairPickup[i] != -1) {
        DestroyDynamicPickup(gRepairPickup[i]);
        gRepairPickup[i] = -1;
    }
}

static CreateCraftWorldObjects(i)
{
    new label[256];
    format(label, sizeof(label),
        "{00CCFF}[Xuong Che Tao]\n{FFFFFF}CONG TY: {%s}%s\n{FFFFFF}Lenh: {F6F08D}/chetaodungcu",
        Group_NumToDialogHex(arrGroupData[gCraftGroupID[i]][g_hDutyColour]),
        gCraftName[i]);
    gCraftLabel[i] = CreateDynamic3DTextLabel(label, 0xFFFFFFFF,
        gCraftX[i], gCraftY[i], gCraftZ[i] + 0.5, 20.0);
    gCraftPickup[i] = CreateDynamicPickup(1239, 23,
        gCraftX[i], gCraftY[i], gCraftZ[i]);
}

static DestroyCraftWorldObjects(i)
{
    if(gCraftLabel[i] != Text3D:INVALID_3DTEXT_ID) {
        DestroyDynamic3DTextLabel(gCraftLabel[i]);
        gCraftLabel[i] = Text3D:INVALID_3DTEXT_ID;
    }
    if(gCraftPickup[i] != -1) {
        DestroyDynamicPickup(gCraftPickup[i]);
        gCraftPickup[i] = -1;
    }
}

static CreateStorageWorldObjects(i)
{
    new label[256];
    format(label, sizeof(label),
        "{00FF88}[Kho Mechanic]\n{FFFFFF}CONG TY: {%s}%s\n{FFFFFF}Lenh: {F6F08D}/dungcu",
        Group_NumToDialogHex(arrGroupData[gStorageGroupID[i]][g_hDutyColour]),
        gStorageName[i]);
    gStorageLabel[i] = CreateDynamic3DTextLabel(label, 0xFFFFFFFF,
        gStorageX[i], gStorageY[i], gStorageZ[i] + 0.5, 20.0);
    gStoragePickup[i] = CreateDynamicPickup(1239, 23,
        gStorageX[i], gStorageY[i], gStorageZ[i]);
}

static DestroyStorageWorldObjects(i)
{
    if(gStorageLabel[i] != Text3D:INVALID_3DTEXT_ID) {
        DestroyDynamic3DTextLabel(gStorageLabel[i]);
        gStorageLabel[i] = Text3D:INVALID_3DTEXT_ID;
    }
    if(gStoragePickup[i] != -1) {
        DestroyDynamicPickup(gStoragePickup[i]);
        gStoragePickup[i] = -1;
    }
}

// ============================================================
// SAVE / LOAD - Locations (file-based like gps.inc)
// ============================================================
stock SaveMechRepairSpots()
{
    new line[192], File:f = fopen("MechRepairSpots.cfg", io_write);
    if(f == File:0) return;
    for(new i = 0; i < gRepairCount; i++) {
        format(line, sizeof(line), "%s|%f|%f|%f|%d\r\n",
            gRepairName[i], gRepairX[i], gRepairY[i], gRepairZ[i], gRepairGroupID[i]);
        fwrite(f, line);
    }
    fclose(f);
}

stock LoadMechRepairSpots()
{
    for(new i = 0; i < gRepairCount; i++) DestroRepairWorldObjects(i);
    gRepairCount = 0;
    if(!fexist("MechRepairSpots.cfg")) { SaveMechRepairSpots(); return 1; }
    new line[192], File:f = fopen("MechRepairSpots.cfg", io_read);
    if(f == File:0) return 1;
    while(gRepairCount < MAX_REPAIR_SPOTS && fread(f, line)) {
        if(!sscanf(line, "p<|>s[48]fffd",
            gRepairName[gRepairCount], gRepairX[gRepairCount], gRepairY[gRepairCount],
            gRepairZ[gRepairCount], gRepairGroupID[gRepairCount]))
        {
            CreateRepairWorldObjects(gRepairCount);
            gRepairCount++;
        }
    }
    fclose(f);
    return 1;
}

stock SaveMechCraftSpots()
{
    new line[192], File:f = fopen("MechCraftSpots.cfg", io_write);
    if(f == File:0) return;
    for(new i = 0; i < gCraftCount; i++) {
        format(line, sizeof(line), "%s|%f|%f|%f|%d\r\n",
            gCraftName[i], gCraftX[i], gCraftY[i], gCraftZ[i], gCraftGroupID[i]);
        fwrite(f, line);
    }
    fclose(f);
}

stock LoadMechCraftSpots()
{
    for(new i = 0; i < gCraftCount; i++) DestroyCraftWorldObjects(i);
    gCraftCount = 0;
    if(!fexist("MechCraftSpots.cfg")) { SaveMechCraftSpots(); return 1; }
    new line[192], File:f = fopen("MechCraftSpots.cfg", io_read);
    if(f == File:0) return 1;
    while(gCraftCount < MAX_CRAFT_SPOTS && fread(f, line)) {
        if(!sscanf(line, "p<|>s[48]fffd",
            gCraftName[gCraftCount], gCraftX[gCraftCount], gCraftY[gCraftCount],
            gCraftZ[gCraftCount], gCraftGroupID[gCraftCount]))
        {
            CreateCraftWorldObjects(gCraftCount);
            gCraftCount++;
        }
    }
    fclose(f);
    return 1;
}

stock SaveMechStorageSpots()
{
    new line[192], File:f = fopen("MechStorageSpots.cfg", io_write);
    if(f == File:0) return;
    for(new i = 0; i < gStorageCount; i++) {
        format(line, sizeof(line), "%s|%f|%f|%f|%d\r\n",
            gStorageName[i], gStorageX[i], gStorageY[i], gStorageZ[i], gStorageGroupID[i]);
        fwrite(f, line);
    }
    fclose(f);
}

stock LoadMechStorageSpots()
{
    for(new i = 0; i < gStorageCount; i++) DestroyStorageWorldObjects(i);
    gStorageCount = 0;
    if(!fexist("MechStorageSpots.cfg")) { SaveMechStorageSpots(); return 1; }
    new line[192], File:f = fopen("MechStorageSpots.cfg", io_read);
    if(f == File:0) return 1;
    while(gStorageCount < MAX_STORAGE_SPOTS && fread(f, line)) {
        if(!sscanf(line, "p<|>s[48]fffd",
            gStorageName[gStorageCount], gStorageX[gStorageCount], gStorageY[gStorageCount],
            gStorageZ[gStorageCount], gStorageGroupID[gStorageCount]))
        {
            CreateStorageWorldObjects(gStorageCount);
            gStorageCount++;
        }
    }
    fclose(f);
    return 1;
}

// ============================================================
// SAVE / LOAD - Group Mechanic Data
// ============================================================
stock LoadMechGroupData(groupid)
{
    if(groupid < 0 || groupid >= MAX_MECH_GROUPS) return 0;
    gMechGroupMoney[groupid] = 0;
    gMechGroupToolOto[groupid] = 0;
    gMechGroupToolXeMay[groupid] = 0;
    gMechGroupLoaded[groupid] = true;

    new filename[64], line[128];
    format(filename, sizeof(filename), "MechGroup_%d.cfg", groupid);
    if(!fexist(filename)) return 1;

    new File:f = fopen(filename, io_read);
    if(f != File:0) {
        if(fread(f, line)) {
            sscanf(line, "p<|>ddd",
                gMechGroupMoney[groupid],
                gMechGroupToolOto[groupid],
                gMechGroupToolXeMay[groupid]);
        }
        fclose(f);
    }
    return 1;
}

stock SaveMechGroupData(groupid)
{
    if(groupid < 0 || groupid >= MAX_MECH_GROUPS) return 0;
    new filename[64], line[128];
    format(filename, sizeof(filename), "MechGroup_%d.cfg", groupid);
    new File:f = fopen(filename, io_write);
    if(f == File:0) return 0;
    format(line, sizeof(line), "%d|%d|%d\r\n",
        gMechGroupMoney[groupid],
        gMechGroupToolOto[groupid],
        gMechGroupToolXeMay[groupid]);
    fwrite(f, line);
    fclose(f);
    return 1;
}

stock EnsureMechGroupLoaded(groupid)
{
    if(groupid < 0 || groupid >= MAX_MECH_GROUPS) return 0;
    if(!gMechGroupLoaded[groupid]) LoadMechGroupData(groupid);
    return 1;
}

// ============================================================
// LOGGING
// ============================================================
stock MechLogMoney(groupid, const playername[], const action[], amount, balance)
{
    new filename[64], line[256];
    format(filename, sizeof(filename), "MechLog_%d_money.log", groupid);
    new File:f = fopen(filename, io_append);

    new year, month, day, hour, minute, second;
    getdate(year, month, day);
    gettime(hour, minute, second);

    new nfAmt[32], nfBal[32];
    format(nfAmt, sizeof(nfAmt), "%s", number_format(amount));
    format(nfBal, sizeof(nfBal), "%s", number_format(balance));
    format(line, sizeof(line), "[%02d/%02d/%04d %02d:%02d:%02d] %s - %s $%s | So du: $%s\r\n",
        day, month, year, hour, minute, second,
        playername, action, nfAmt, nfBal);
    fwrite(f, line);
    fclose(f);
}

stock MechLogTool(groupid, const playername[], const action[], const toolname[], amount, balance)
{
    new filename[64], line[256];
    format(filename, sizeof(filename), "MechLog_%d_tool.log", groupid);
    new File:f = fopen(filename, io_append);

    new year, month, day, hour, minute, second;
    getdate(year, month, day);
    gettime(hour, minute, second);

    format(line, sizeof(line), "[%02d/%02d/%04d %02d:%02d:%02d] %s - %s %s x%d | Ton kho: %d\r\n",
        day, month, year, hour, minute, second,
        playername, action, toolname, amount, balance);
    fwrite(f, line);
    fclose(f);
}

// ============================================================
// UTILITY
// ============================================================
stock IsPlayerNearRepairSpot(playerid, groupid)
{
    for(new i = 0; i < gRepairCount; i++) {
        if(gRepairGroupID[i] == groupid &&
           IsPlayerInRangeOfPoint(playerid, 10.0, gRepairX[i], gRepairY[i], gRepairZ[i]))
            return 1;
    }
    return 0;
}

stock IsPlayerNearCraftSpot(playerid, groupid)
{
    for(new i = 0; i < gCraftCount; i++) {
        if(gCraftGroupID[i] == groupid &&
           IsPlayerInRangeOfPoint(playerid, 5.0, gCraftX[i], gCraftY[i], gCraftZ[i]))
            return 1;
    }
    return 0;
}

stock GetPlayerStorageSpot(playerid, groupid)
{
    for(new i = 0; i < gStorageCount; i++) {
        if(gStorageGroupID[i] == groupid &&
           IsPlayerInRangeOfPoint(playerid, 5.0, gStorageX[i], gStorageY[i], gStorageZ[i]))
            return i;
    }
    return -1;
}

// Kiem tra xe co phai cua group khong, tra ve maxhealth neu co
stock Float:GetGroupVehicleMaxHealth(vehicleid)
{
    for(new i = 0; i < MAX_DYNAMIC_VEHICLES; i++) {
        if(DynVehicleInfo[i][gv_iSpawnedID] == vehicleid && DynVehicleInfo[i][gv_igID] >= 0) {
            new Float:hp = DynVehicleInfo[i][gv_fMaxHealth];
            return (hp > 0.0) ? hp : 1000.0;
        }
    }
    return 0.0; // khong phai xe group
}

// ============================================================
// COMMAND: /suaxe
// ============================================================
CMD:suaxe(playerid, params[])
{
    if(!IsAMechanic(playerid)) return SendUnAuth(playerid);
    if(IsPlayerInAnyVehicle(playerid))
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban phai ra khoi xe truoc khi sua xe.");
    if(GetPVarInt(playerid, "MechBusy") == 1)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban dang ban, khong the thuc hien.");

    new closestVeh = GetClosestCar(playerid, INVALID_VEHICLE_ID);
    if(closestVeh == INVALID_VEHICLE_ID || GetDistanceToCar(playerid, closestVeh) > 5.0)
        return SendClientMessageEx(playerid, COLOR_GREY, "Khong co xe nao gan ban trong pham vi 5m.");

    new Float:currentHealth;
    GetVehicleHealth(closestVeh, currentHealth);
    new Float:maxHP = GetGroupVehicleMaxHealth(closestVeh);
    if(maxHP <= 0.0) maxHP = 1000.0;
    if(currentHealth >= maxHP)
        return SendClientMessageEx(playerid, COLOR_GREY, "Chiec xe nay khong can sua chua.");

    new groupid = PlayerInfo[playerid][pMember];
    new bool:isBike = IsABike(closestVeh);

    if(isBike)
    {
        // Xe may: sua tai cho, can toolkit xe may
        if(PlayerInfo[playerid][pToolkitXeMay] <= 0)
            return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong co dung cu sua xe may. Hay che tao tai xuong.");

        // Bat dau sua
        SetPVarInt(playerid, "MechBusy", 1);
        SetPVarInt(playerid, "MechRepairVeh", closestVeh);
        SetPVarInt(playerid, "MechRepairBike", 1);

        new repairTime = 20 + random(11); // 20-30s
        new animCase = random(2);
        switch(animCase)
        {
            case 0: ApplyAnimation(playerid, "COP_AMBIENT", "Copbrowse_loop", 4.1, true, false, false, true, 0, SYNC_ALL);
            case 1: ApplyAnimation(playerid, "HEIST9", "Use_SwipeCard", 4.1, true, false, false, true, 0, SYNC_ALL);
        }
        SetLoading(playerid, repairTime * 1000, "Dang sua xe may...", "OnMechanicRepairDone");
        SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "Ban bat dau sua xe may... Vui long doi.");

        new proxstr[128];
        format(proxstr, sizeof(proxstr), "{FF8000}* {C2A2DA}%s bat dau sua chua xe may.", GetPlayerNameEx(playerid));
        ProxDetector(30.0, playerid, proxstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
    }
    else
    {
        // Oto: can toolkit oto + dung gan tru sua xe
        if(PlayerInfo[playerid][pToolkitOto] <= 0)
            return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong co dung cu sua oto. Hay che tao tai xuong.");

        if(!IsPlayerNearRepairSpot(playerid, groupid))
            return SendClientMessageEx(playerid, COLOR_GREY, "Ban phai o gan tru sua xe cua cong ty de sua oto!");

        new engine, lights, alarm, doors, bonnet, boot, objective;
        GetVehicleParamsEx(closestVeh, engine, lights, alarm, doors, bonnet, boot, objective);
        if(engine == VEHICLE_PARAMS_ON)
            return SendClientMessageEx(playerid, COLOR_GREY, "Xe phai tat dong co truoc khi sua! (/car engine)");

        SetPVarInt(playerid, "MechBusy", 1);
        SetPVarInt(playerid, "MechRepairVeh", closestVeh);
        SetPVarInt(playerid, "MechRepairBike", 0);

        new repairTime = 20 + random(11);
        new animCase = random(2);
        switch(animCase)
        {
            case 0: ApplyAnimation(playerid, "COP_AMBIENT", "Copbrowse_loop", 4.1, true, false, false, true, 0, SYNC_ALL);
            case 1: ApplyAnimation(playerid, "HEIST9", "Use_SwipeCard", 4.1, true, false, false, true, 0, SYNC_ALL);
        }
        SetPlayerAttachedObject(playerid, 9, 18635, 6);
        SetLoading(playerid, repairTime * 1000, "Dang sua oto...", "OnMechanicRepairDone");
        SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "Ban bat dau sua oto... Vui long doi.");

        new proxstr[128];
        format(proxstr, sizeof(proxstr), "{FF8000}* {C2A2DA}%s bat dau sua chua xe o to.", GetPlayerNameEx(playerid));
        ProxDetector(30.0, playerid, proxstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
    }
    return 1;
}

// ============================================================
// CALLBACK: Repair Done
// ============================================================
public OnMechanicRepairDone(playerid)
{
    if(!IsPlayerConnected(playerid)) return 0;
    SetPVarInt(playerid, "MechBusy", 0);
    ClearAnimations(playerid);
    RemovePlayerAttachedObject(playerid, 9);

    new vehicleid = GetPVarInt(playerid, "MechRepairVeh");
    new isBike = GetPVarInt(playerid, "MechRepairBike");

    // Kiem tra xe con ton tai va player con gan xe
    if(!IsValidVehicle(vehicleid) || GetDistanceToCar(playerid, vehicleid) > 10.0)
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Sua xe that bai - xe khong con o gan ban.");
        return 1;
    }

    // Repair visual first (RepairVehicle resets health to 1000 internally)
    RepairVehicle(vehicleid);

    // Set health AFTER RepairVehicle so it doesn't get overwritten
    new Float:groupMaxHP = GetGroupVehicleMaxHealth(vehicleid);
    if(groupMaxHP > 0.0)
        SetVehicleHealth(vehicleid, groupMaxHP);

    AC_WhitelistVehRepair(vehicleid);

    // Allow anti-cheat
    new driverid = GetVehicleDriver(vehicleid);
    if(driverid != INVALID_PLAYER_ID) SetPVarInt(driverid, "MechanicRepair", 1);

    // Tru dung cu
    if(isBike)
        PlayerInfo[playerid][pToolkitXeMay]--;
    else
        PlayerInfo[playerid][pToolkitOto]--;

    // Messages
    new str[128];
    if(isBike)
        format(str, sizeof(str), "Ban da sua xong xe may. (Con lai: %d dung cu xe may)", PlayerInfo[playerid][pToolkitXeMay]);
    else
        format(str, sizeof(str), "Ban da sua xong oto. (Con lai: %d dung cu oto)", PlayerInfo[playerid][pToolkitOto]);
    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, str);

    if(driverid != INVALID_PLAYER_ID && driverid != playerid) {
        format(str, sizeof(str), "Xe cua ban da duoc tho may {FFFF00}%s{FFFFFF} sua chua xong.", GetPlayerNameEx(playerid));
        SendClientMessageEx(driverid, COLOR_LIGHTBLUE, str);
    }

    new proxstr[128];
    format(proxstr, sizeof(proxstr), "{FF8000}* {C2A2DA}%s da sua chua xong chiec xe.", GetPlayerNameEx(playerid));
    ProxDetector(30.0, playerid, proxstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
    return 1;
}

// ============================================================
// COMMAND: /chetaodungcu
// ============================================================
CMD:chetaodungcu(playerid, params[])
{
    if(!IsAMechanic(playerid)) return SendUnAuth(playerid);
    if(GetPVarInt(playerid, "MechBusy") == 1)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban dang ban, khong the thuc hien.");

    new groupid = PlayerInfo[playerid][pMember];
    if(!IsPlayerNearCraftSpot(playerid, groupid))
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban phai o gan xuong che tao cua cong ty!");

    new str[512];
    format(str, sizeof(str), "{FF6600}Dung cu sua Oto{FFFFFF}\n  Sat x%d, Dong x%d, Vat lieu x%d\n  (Ban co: Sat %d, Dong %d, VL %d)\n\n{00CCFF}Dung cu sua Xe may{FFFFFF}\n  Sat x%d, Dong x%d, Vat lieu x%d\n  (Ban co: Sat %d, Dong %d, VL %d)",
        CRAFT_OTO_SAT, CRAFT_OTO_DONG, CRAFT_OTO_MATS,
        PlayerInfo[playerid][pMinSat], PlayerInfo[playerid][pMinDong], PlayerInfo[playerid][pMats],
        CRAFT_XEMAY_SAT, CRAFT_XEMAY_DONG, CRAFT_XEMAY_MATS,
        PlayerInfo[playerid][pMinSat], PlayerInfo[playerid][pMinDong], PlayerInfo[playerid][pMats]);

    ShowPlayerDialog(playerid, DIALOG_MECH_CHETAO, DIALOG_STYLE_LIST,
        "{FFFF00}Che Tao Dung Cu Sua Xe",
        "Dung cu sua Oto\nDung cu sua Xe may",
        "Che tao", "Huy");
    return 1;
}

// ============================================================
// CALLBACK: Craft Done
// ============================================================
public OnMechanicCraftDone(playerid)
{
    if(!IsPlayerConnected(playerid)) return 0;
    SetPVarInt(playerid, "MechBusy", 0);
    ClearAnimations(playerid);
    TogglePlayerControllable(playerid, true);

    new craftType = GetPVarInt(playerid, "MechCraftType");
    new str[128];

    if(craftType == 0) // Oto
    {
        PlayerInfo[playerid][pToolkitOto]++;
        format(str, sizeof(str), "Ban da che tao thanh cong {FF6600}Dung cu sua Oto{FFFFFF}! (Tong: %d)", PlayerInfo[playerid][pToolkitOto]);
    }
    else // Xe may
    {
        PlayerInfo[playerid][pToolkitXeMay]++;
        format(str, sizeof(str), "Ban da che tao thanh cong {00CCFF}Dung cu sua Xe may{FFFFFF}! (Tong: %d)", PlayerInfo[playerid][pToolkitXeMay]);
    }
    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, str);

    new proxstr[128];
    if(craftType == 0)
        format(proxstr, sizeof(proxstr), "{FF8000}* {C2A2DA}%s da che tao xong dung cu sua Oto.", GetPlayerNameEx(playerid));
    else
        format(proxstr, sizeof(proxstr), "{FF8000}* {C2A2DA}%s da che tao xong dung cu sua Xe may.", GetPlayerNameEx(playerid));
    ProxDetector(30.0, playerid, proxstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
    return 1;
}

// ============================================================
// COMMAND: /dungcu
// ============================================================
CMD:dungcu(playerid, params[])
{
    if(!IsAMechanic(playerid)) return SendUnAuth(playerid);

    new groupid = PlayerInfo[playerid][pMember];
    new spotIdx = GetPlayerStorageSpot(playerid, groupid);
    if(spotIdx == -1)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban phai o gan kho cua cong ty mechanic!");

    EnsureMechGroupLoaded(groupid);

    new str[256];
    format(str, sizeof(str),
        "{FFD700}Tien kho: $%s  |  {FF6600}Oto: %d  {00CCFF}XeMay: %d",
        number_format(gMechGroupMoney[groupid]),
        gMechGroupToolOto[groupid],
        gMechGroupToolXeMay[groupid]);

    ShowPlayerDialog(playerid, DIALOG_MECH_KHO, DIALOG_STYLE_LIST,
        str,
        "Gui tien\nRut tien\nGui/Rut dung cu\nXem log",
        "Chon", "Dong");
    return 1;
}

// ============================================================
// ADMIN COMMANDS
// ============================================================
// /editrepair add [groupid] | delete | setpos [id] | reload
CMD:editrepair(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1337)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong co quyen su dung lenh nay!");

    new action[16], id = -1;
    if(sscanf(params, "s[16]D(-1)", action, id)) {
        SendClientMessageEx(playerid, COLOR_WHITE, "SU DUNG: /editrepair [tuy chon] [...]");
        SendClientMessageEx(playerid, COLOR_GREY, "Tuy chon: {FFFF00}add [groupid], delete, setpos [id], reload");
        return 1;
    }

    if(!strcmp(action, "add", true))
    {
        if(gRepairCount >= MAX_REPAIR_SPOTS)
            return SendClientMessageEx(playerid, COLOR_GREY, "Da dat so luong toi da!");
        if(id < 1 || id > MAX_GROUPS)
            return SendClientMessageEx(playerid, COLOR_WHITE, "SU DUNG: /editrepair add [groupid] (so thu tu trong /grouplist)");
        if(!strlen(arrGroupData[id-1][g_szGroupName]))
            return SendClientMessageEx(playerid, COLOR_GREY, "Group so %d khong ton tai!", id);

        new idx = gRepairCount;
        strcopy(gRepairName[idx], arrGroupData[id-1][g_szGroupName], 48);
        GetPlayerPos(playerid, gRepairX[idx], gRepairY[idx], gRepairZ[idx]);
        gRepairGroupID[idx] = id - 1;
        CreateRepairWorldObjects(idx);
        gRepairCount++;
        SaveMechRepairSpots();
        LoadMechRepairSpots();

        new msg[128];
        format(msg, sizeof(msg), "Da them tru sua xe cho cong ty '{FFFF00}%s{FFFFFF}' (Group %d).", arrGroupData[id-1][g_szGroupName], id);
        SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
    }
    else if(!strcmp(action, "delete", true))
    {
        new string[2048], line[112];
        strcat(string, "{FFD700}ID\t{FFFFFF}Ten\t{87CEEB}Group\n", sizeof(string));
        for(new i = 0; i < gRepairCount; i++) {
            format(line, sizeof(line), "{FFD700}%d\t{FFFFFF}%s\t{87CEEB}%d\n", i, gRepairName[i], gRepairGroupID[i]);
            strcat(string, line, sizeof(string));
        }
        if(!gRepairCount) strcat(string, "Chua co tru nao.", sizeof(string));
        ShowPlayerDialog(playerid, DIALOG_ADMIN_REPAIR_LIST, DIALOG_STYLE_TABLIST_HEADERS,
            "{FFFF00}XOA TRU SUA XE", string, "Xoa", "Dong");
    }
    else if(!strcmp(action, "setpos", true))
    {
        if(id < 0) return SendClientMessageEx(playerid, COLOR_WHITE, "SU DUNG: /editrepair setpos [id]");
        if(id >= gRepairCount) return SendClientMessageEx(playerid, COLOR_GREY, "ID khong hop le!");

        DestroRepairWorldObjects(id);
        GetPlayerPos(playerid, gRepairX[id], gRepairY[id], gRepairZ[id]);
        CreateRepairWorldObjects(id);
        SaveMechRepairSpots();

        new msg[128];
        format(msg, sizeof(msg), "Da cap nhat vi tri tru sua xe '{FFFF00}%s{FFFFFF}' (ID %d).", gRepairName[id], id);
        SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
    }
    else if(!strcmp(action, "reload", true))
    {
        LoadMechRepairSpots();
        new msg[64];
        format(msg, sizeof(msg), "Da tai lai %d tru sua xe.", gRepairCount);
        SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
    }
    return 1;
}

// /editcraft add [groupid] | delete | setpos [id] | reload
CMD:editcraft(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1337)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong co quyen su dung lenh nay!");

    new action[16], id = -1;
    if(sscanf(params, "s[16]D(-1)", action, id)) {
        SendClientMessageEx(playerid, COLOR_WHITE, "SU DUNG: /editcraft [tuy chon] [...]");
        SendClientMessageEx(playerid, COLOR_GREY, "Tuy chon: {FFFF00}add [groupid], delete, setpos [id], reload");
        return 1;
    }

    if(!strcmp(action, "add", true))
    {
        if(gCraftCount >= MAX_CRAFT_SPOTS)
            return SendClientMessageEx(playerid, COLOR_GREY, "Da dat so luong toi da!");
        if(id < 1 || id > MAX_GROUPS)
            return SendClientMessageEx(playerid, COLOR_WHITE, "SU DUNG: /editcraft add [groupid] (so thu tu trong /grouplist)");
        if(!strlen(arrGroupData[id-1][g_szGroupName]))
            return SendClientMessageEx(playerid, COLOR_GREY, "Group so %d khong ton tai!", id);

        new idx = gCraftCount;
        strcopy(gCraftName[idx], arrGroupData[id-1][g_szGroupName], 48);
        GetPlayerPos(playerid, gCraftX[idx], gCraftY[idx], gCraftZ[idx]);
        gCraftGroupID[idx] = id - 1;
        CreateCraftWorldObjects(idx);
        gCraftCount++;
        SaveMechCraftSpots();
        LoadMechCraftSpots();

        new msg[128];
        format(msg, sizeof(msg), "Da them xuong che tao cho cong ty '{FFFF00}%s{FFFFFF}' (Group %d).", arrGroupData[id-1][g_szGroupName], id);
        SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
    }
    else if(!strcmp(action, "delete", true))
    {
        new string[2048], line[112];
        strcat(string, "{FFD700}ID\t{FFFFFF}Ten\t{87CEEB}Group\n", sizeof(string));
        for(new i = 0; i < gCraftCount; i++) {
            format(line, sizeof(line), "{FFD700}%d\t{FFFFFF}%s\t{87CEEB}%d\n", i, gCraftName[i], gCraftGroupID[i]);
            strcat(string, line, sizeof(string));
        }
        if(!gCraftCount) strcat(string, "Chua co xuong nao.", sizeof(string));
        ShowPlayerDialog(playerid, DIALOG_ADMIN_CRAFT_LIST, DIALOG_STYLE_TABLIST_HEADERS,
            "{FFFF00}XOA XUONG CHE TAO", string, "Xoa", "Dong");
    }
    else if(!strcmp(action, "setpos", true))
    {
        if(id < 0) return SendClientMessageEx(playerid, COLOR_WHITE, "SU DUNG: /editcraft setpos [id]");
        if(id >= gCraftCount) return SendClientMessageEx(playerid, COLOR_GREY, "ID khong hop le!");

        DestroyCraftWorldObjects(id);
        GetPlayerPos(playerid, gCraftX[id], gCraftY[id], gCraftZ[id]);
        CreateCraftWorldObjects(id);
        SaveMechCraftSpots();

        new msg[128];
        format(msg, sizeof(msg), "Da cap nhat vi tri xuong '{FFFF00}%s{FFFFFF}' (ID %d).", gCraftName[id], id);
        SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
    }
    else if(!strcmp(action, "reload", true))
    {
        LoadMechCraftSpots();
        new msg[64];
        format(msg, sizeof(msg), "Da tai lai %d xuong che tao.", gCraftCount);
        SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
    }
    return 1;
}

// /editstorage add [groupid] | delete | setpos [id] | reload
CMD:editstorage(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1337)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong co quyen su dung lenh nay!");

    new action[16], id = -1;
    if(sscanf(params, "s[16]D(-1)", action, id)) {
        SendClientMessageEx(playerid, COLOR_WHITE, "SU DUNG: /editstorage [tuy chon] [...]");
        SendClientMessageEx(playerid, COLOR_GREY, "Tuy chon: {FFFF00}add [groupid], delete, setpos [id], reload");
        return 1;
    }

    if(!strcmp(action, "add", true))
    {
        if(gStorageCount >= MAX_STORAGE_SPOTS)
            return SendClientMessageEx(playerid, COLOR_GREY, "Da dat so luong toi da!");
        if(id < 1 || id > MAX_GROUPS)
            return SendClientMessageEx(playerid, COLOR_WHITE, "SU DUNG: /editstorage add [groupid] (so thu tu trong /grouplist)");
        if(!strlen(arrGroupData[id-1][g_szGroupName]))
            return SendClientMessageEx(playerid, COLOR_GREY, "Group so %d khong ton tai!", id);

        new idx = gStorageCount;
        strcopy(gStorageName[idx], arrGroupData[id-1][g_szGroupName], 48);
        GetPlayerPos(playerid, gStorageX[idx], gStorageY[idx], gStorageZ[idx]);
        gStorageGroupID[idx] = id - 1;
        CreateStorageWorldObjects(idx);
        gStorageCount++;
        SaveMechStorageSpots();
        LoadMechStorageSpots();

        new msg[128];
        format(msg, sizeof(msg), "Da them kho cho cong ty '{FFFF00}%s{FFFFFF}' (Group %d).", arrGroupData[id-1][g_szGroupName], id);
        SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
    }
    else if(!strcmp(action, "delete", true))
    {
        new string[2048], line[112];
        strcat(string, "{FFD700}ID\t{FFFFFF}Ten\t{87CEEB}Group\n", sizeof(string));
        for(new i = 0; i < gStorageCount; i++) {
            format(line, sizeof(line), "{FFD700}%d\t{FFFFFF}%s\t{87CEEB}%d\n", i, gStorageName[i], gStorageGroupID[i]);
            strcat(string, line, sizeof(string));
        }
        if(!gStorageCount) strcat(string, "Chua co kho nao.", sizeof(string));
        ShowPlayerDialog(playerid, DIALOG_ADMIN_STORAGE_LIST, DIALOG_STYLE_TABLIST_HEADERS,
            "{FFFF00}XOA KHO MECHANIC", string, "Xoa", "Dong");
    }
    else if(!strcmp(action, "setpos", true))
    {
        if(id < 0) return SendClientMessageEx(playerid, COLOR_WHITE, "SU DUNG: /editstorage setpos [id]");
        if(id >= gStorageCount) return SendClientMessageEx(playerid, COLOR_GREY, "ID khong hop le!");

        DestroyStorageWorldObjects(id);
        GetPlayerPos(playerid, gStorageX[id], gStorageY[id], gStorageZ[id]);
        CreateStorageWorldObjects(id);
        SaveMechStorageSpots();

        new msg[128];
        format(msg, sizeof(msg), "Da cap nhat vi tri kho '{FFFF00}%s{FFFFFF}' (ID %d).", gStorageName[id], id);
        SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
    }
    else if(!strcmp(action, "reload", true))
    {
        LoadMechStorageSpots();
        new msg[64];
        format(msg, sizeof(msg), "Da tai lai %d kho mechanic.", gStorageCount);
        SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
    }
    return 1;
}

// ============================================================
// HOOK: OnGameModeInit
// ============================================================
hook OnGameModeInit()
{
    for(new i = 0; i < MAX_MECH_GROUPS; i++) gMechGroupLoaded[i] = false;
    return 1;
}

// Load spots sau khi MySQL load xong group (co du colour data)
forward OnAllGroupsLoaded();
public OnAllGroupsLoaded()
{
    LoadMechRepairSpots();
    LoadMechCraftSpots();
    LoadMechStorageSpots();
    return 1;
}

// ============================================================
// DIALOG RESPONSES
// ============================================================
hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid)
    {
        // ==================== CHE TAO ====================
        case DIALOG_MECH_CHETAO:
        {
            if(!response) return 1;
            new groupid = PlayerInfo[playerid][pMember];
            if(!IsPlayerNearCraftSpot(playerid, groupid)) {
                SendClientMessageEx(playerid, COLOR_GREY, "Ban da roi khoi xuong che tao!");
                return 1;
            }

            if(listitem == 0) // Oto
            {
                if(PlayerInfo[playerid][pMinSat] < CRAFT_OTO_SAT ||
                   PlayerInfo[playerid][pMinDong] < CRAFT_OTO_DONG ||
                   PlayerInfo[playerid][pMats] < CRAFT_OTO_MATS)
                {
                    SendClientMessageEx(playerid, COLOR_GREY, "Ban khong du nguyen lieu! Can: Sat x%d, Dong x%d, VL x%d",
                        CRAFT_OTO_SAT, CRAFT_OTO_DONG, CRAFT_OTO_MATS);
                    return 1;
                }

                PlayerInfo[playerid][pMinSat] -= CRAFT_OTO_SAT;
                PlayerInfo[playerid][pMinDong] -= CRAFT_OTO_DONG;
                PlayerInfo[playerid][pMats] -= CRAFT_OTO_MATS;
                SetPVarInt(playerid, "MechCraftType", 0);
            }
            else // Xe may
            {
                if(PlayerInfo[playerid][pMinSat] < CRAFT_XEMAY_SAT ||
                   PlayerInfo[playerid][pMinDong] < CRAFT_XEMAY_DONG ||
                   PlayerInfo[playerid][pMats] < CRAFT_XEMAY_MATS)
                {
                    SendClientMessageEx(playerid, COLOR_GREY, "Ban khong du nguyen lieu! Can: Sat x%d, Dong x%d, VL x%d",
                        CRAFT_XEMAY_SAT, CRAFT_XEMAY_DONG, CRAFT_XEMAY_MATS);
                    return 1;
                }

                PlayerInfo[playerid][pMinSat] -= CRAFT_XEMAY_SAT;
                PlayerInfo[playerid][pMinDong] -= CRAFT_XEMAY_DONG;
                PlayerInfo[playerid][pMats] -= CRAFT_XEMAY_MATS;
                SetPVarInt(playerid, "MechCraftType", 1);
            }

            SetPVarInt(playerid, "MechBusy", 1);
            TogglePlayerControllable(playerid, false);
            new craftTime = 15 + random(6); // 15-20s
            ApplyAnimation(playerid, "BD_FIRE", "wash_up", 4.1, true, false, false, true, 0, SYNC_ALL);
            SetLoading(playerid, craftTime * 1000, "Dang che tao dung cu...", "OnMechanicCraftDone");
            SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "Bat dau che tao dung cu... Vui long doi.");

            new proxstr[128];
            format(proxstr, sizeof(proxstr), "{FF8000}* {C2A2DA}%s bat dau che tao dung cu sua xe.", GetPlayerNameEx(playerid));
            ProxDetector(30.0, playerid, proxstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
        }

        // ==================== KHO MECHANIC ====================
        case DIALOG_MECH_KHO:
        {
            if(!response) return 1;
            new groupid = PlayerInfo[playerid][pMember];
            EnsureMechGroupLoaded(groupid);

            switch(listitem)
            {
                case 0: // Gui tien
                {
                    new str[128];
                    format(str, sizeof(str), "{FFFFFF}Tien mat cua ban: {FFD700}$%s\n{FFFFFF}Tien trong kho: {FFD700}$%s\n\n{FFFFFF}Nhap so tien muon gui:",
                        number_format(GetPlayerMoney(playerid)), number_format(gMechGroupMoney[groupid]));
                    ShowPlayerDialog(playerid, DIALOG_MECH_KHO_TIEN_GUI, DIALOG_STYLE_INPUT,
                        "{FFFF00}Gui Tien Vao Kho", str, "Gui", "Huy");
                }
                case 1: // Rut tien
                {
                    new str[128];
                    format(str, sizeof(str), "{FFFFFF}Tien trong kho: {FFD700}$%s\n\n{FFFFFF}Nhap so tien muon rut:",
                        number_format(gMechGroupMoney[groupid]));
                    ShowPlayerDialog(playerid, DIALOG_MECH_KHO_TIEN_RUT, DIALOG_STYLE_INPUT,
                        "{FFFF00}Rut Tien Tu Kho", str, "Rut", "Huy");
                }
                case 2: // Gui/Rut dung cu
                {
                    new str[128];
                    format(str, sizeof(str), "{FFFF00}Kho: {FF6600}Oto %d {00CCFF}XeMay %d {FFFFFF}| Ban: {FF6600}Oto %d {00CCFF}XeMay %d",
                        gMechGroupToolOto[groupid], gMechGroupToolXeMay[groupid],
                        PlayerInfo[playerid][pToolkitOto], PlayerInfo[playerid][pToolkitXeMay]);
                    ShowPlayerDialog(playerid, DIALOG_MECH_KHO_DC_MENU, DIALOG_STYLE_LIST,
                        str,
                        "Gui dung cu sua Oto\nGui dung cu sua Xe may\nRut dung cu sua Oto\nRut dung cu sua Xe may",
                        "Chon", "Quay lai");
                }
                case 3: // Xem log
                {
                    if(PlayerInfo[playerid][pRank] < 5 && !PlayerInfo[playerid][pLeader]) {
                        SendClientMessageEx(playerid, COLOR_GREY, "Ban can rank 5+ hoac leader de xem log!");
                        return 1;
                    }
                    ShowPlayerDialog(playerid, DIALOG_MECH_KHO_LOG, DIALOG_STYLE_LIST,
                        "{FFFF00}Xem Log Kho",
                        "Log tien (gui/rut)\nLog dung cu (cat/lay)",
                        "Xem", "Dong");
                }
            }
        }

        case DIALOG_MECH_KHO_TIEN_GUI:
        {
            if(!response) return PC_EmulateCommand(playerid, "/dungcu");
            new amount = strval(inputtext);
            new groupid = PlayerInfo[playerid][pMember];
            if(groupid < 0 || groupid >= MAX_MECH_GROUPS)
                return SendClientMessageEx(playerid, COLOR_GREY, "Group khong hop le!");
            EnsureMechGroupLoaded(groupid);

            if(amount <= 0)
                return SendClientMessageEx(playerid, COLOR_GREY, "So tien khong hop le!");
            if(GetPlayerMoney(playerid) < amount)
                return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong du tien!");

            GivePlayerMoney(playerid, -amount);
            gMechGroupMoney[groupid] += amount;
            SaveMechGroupData(groupid);
            MechLogMoney(groupid, GetPlayerNameEx(playerid), "GUI", amount, gMechGroupMoney[groupid]);

            new str[128], nfAmt[32], nfBal[32];
            format(nfAmt, sizeof(nfAmt), "%s", number_format(amount));
            format(nfBal, sizeof(nfBal), "%s", number_format(gMechGroupMoney[groupid]));
            format(str, sizeof(str), "Ban da gui {FFD700}$%s{FFFFFF} vao kho. So du kho: {FFD700}$%s", nfAmt, nfBal);
            SendClientMessageEx(playerid, COLOR_LIGHTBLUE, str);
        }

        case DIALOG_MECH_KHO_TIEN_RUT:
        {
            if(!response) return PC_EmulateCommand(playerid, "/dungcu");
            new amount = strval(inputtext);
            new groupid = PlayerInfo[playerid][pMember];
            if(groupid < 0 || groupid >= MAX_MECH_GROUPS)
                return SendClientMessageEx(playerid, COLOR_GREY, "Group khong hop le!");
            EnsureMechGroupLoaded(groupid);

            if(amount <= 0)
                return SendClientMessageEx(playerid, COLOR_GREY, "So tien khong hop le!");
            if(gMechGroupMoney[groupid] < amount)
                return SendClientMessageEx(playerid, COLOR_GREY, "Kho khong du tien!");

            GivePlayerMoney(playerid, amount);
            gMechGroupMoney[groupid] -= amount;
            SaveMechGroupData(groupid);
            MechLogMoney(groupid, GetPlayerNameEx(playerid), "RUT", amount, gMechGroupMoney[groupid]);

            new str[128], nfAmt[32], nfBal[32];
            format(nfAmt, sizeof(nfAmt), "%s", number_format(amount));
            format(nfBal, sizeof(nfBal), "%s", number_format(gMechGroupMoney[groupid]));
            format(str, sizeof(str), "Ban da rut {FFD700}$%s{FFFFFF} tu kho. So du kho: {FFD700}$%s", nfAmt, nfBal);
            SendClientMessageEx(playerid, COLOR_LIGHTBLUE, str);
        }

        case DIALOG_MECH_KHO_DC_MENU:
        {
            if(!response) return PC_EmulateCommand(playerid, "/dungcu");
            // 0=gui oto, 1=gui xemay, 2=rut oto, 3=rut xemay
            SetPVarInt(playerid, "MechKhoDCAction", listitem);

            new groupid = PlayerInfo[playerid][pMember];
            EnsureMechGroupLoaded(groupid);

            new title[64], body[128];
            switch(listitem) {
                case 0: {
                    title = "Gui Dung Cu Sua Oto";
                    format(body, sizeof(body), "{FFFFFF}Dung cu trong nguoi: {FF6600}%d\n{FFFFFF}Nhap so luong muon gui:", PlayerInfo[playerid][pToolkitOto]);
                }
                case 1: {
                    title = "Gui Dung Cu Sua Xe May";
                    format(body, sizeof(body), "{FFFFFF}Dung cu trong nguoi: {00CCFF}%d\n{FFFFFF}Nhap so luong muon gui:", PlayerInfo[playerid][pToolkitXeMay]);
                }
                case 2: {
                    title = "Rut Dung Cu Sua Oto";
                    format(body, sizeof(body), "{FFFFFF}Dung cu trong kho: {FF6600}%d\n{FFFFFF}Nhap so luong muon rut:", gMechGroupToolOto[groupid]);
                }
                case 3: {
                    title = "Rut Dung Cu Sua Xe May";
                    format(body, sizeof(body), "{FFFFFF}Dung cu trong kho: {00CCFF}%d\n{FFFFFF}Nhap so luong muon rut:", gMechGroupToolXeMay[groupid]);
                }
            }
            ShowPlayerDialog(playerid, DIALOG_MECH_KHO_DC_GUI_SL, DIALOG_STYLE_INPUT,
                title, body, "Xac nhan", "Huy");
        }

        case DIALOG_MECH_KHO_DC_GUI_SL:
        {
            if(!response) return PC_EmulateCommand(playerid, "/dungcu");
            new amount = strval(inputtext);
            if(amount <= 0) return SendClientMessageEx(playerid, COLOR_GREY, "So luong khong hop le!");

            new groupid = PlayerInfo[playerid][pMember];
            if(groupid < 0 || groupid >= MAX_MECH_GROUPS)
                return SendClientMessageEx(playerid, COLOR_GREY, "Group khong hop le!");
            new action = GetPVarInt(playerid, "MechKhoDCAction");
            EnsureMechGroupLoaded(groupid);

            new str[128];
            switch(action)
            {
                case 0: // Gui oto
                {
                    if(PlayerInfo[playerid][pToolkitOto] < amount)
                        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong du dung cu!");
                    PlayerInfo[playerid][pToolkitOto] -= amount;
                    gMechGroupToolOto[groupid] += amount;
                    SaveMechGroupData(groupid);
                    MechLogTool(groupid, GetPlayerNameEx(playerid), "GUI", "DC Oto", amount, gMechGroupToolOto[groupid]);
                    format(str, sizeof(str), "Da gui %d dung cu sua Oto vao kho. (Kho: %d)", amount, gMechGroupToolOto[groupid]);
                }
                case 1: // Gui xemay
                {
                    if(PlayerInfo[playerid][pToolkitXeMay] < amount)
                        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong du dung cu!");
                    PlayerInfo[playerid][pToolkitXeMay] -= amount;
                    gMechGroupToolXeMay[groupid] += amount;
                    SaveMechGroupData(groupid);
                    MechLogTool(groupid, GetPlayerNameEx(playerid), "GUI", "DC XeMay", amount, gMechGroupToolXeMay[groupid]);
                    format(str, sizeof(str), "Da gui %d dung cu sua Xe may vao kho. (Kho: %d)", amount, gMechGroupToolXeMay[groupid]);
                }
                case 2: // Rut oto
                {
                    if(gMechGroupToolOto[groupid] < amount)
                        return SendClientMessageEx(playerid, COLOR_GREY, "Kho khong du dung cu!");
                    PlayerInfo[playerid][pToolkitOto] += amount;
                    gMechGroupToolOto[groupid] -= amount;
                    SaveMechGroupData(groupid);
                    MechLogTool(groupid, GetPlayerNameEx(playerid), "RUT", "DC Oto", amount, gMechGroupToolOto[groupid]);
                    format(str, sizeof(str), "Da rut %d dung cu sua Oto tu kho. (Ban: %d)", amount, PlayerInfo[playerid][pToolkitOto]);
                }
                case 3: // Rut xemay
                {
                    if(gMechGroupToolXeMay[groupid] < amount)
                        return SendClientMessageEx(playerid, COLOR_GREY, "Kho khong du dung cu!");
                    PlayerInfo[playerid][pToolkitXeMay] += amount;
                    gMechGroupToolXeMay[groupid] -= amount;
                    SaveMechGroupData(groupid);
                    MechLogTool(groupid, GetPlayerNameEx(playerid), "RUT", "DC XeMay", amount, gMechGroupToolXeMay[groupid]);
                    format(str, sizeof(str), "Da rut %d dung cu sua Xe may tu kho. (Ban: %d)", amount, PlayerInfo[playerid][pToolkitXeMay]);
                }
            }
            SendClientMessageEx(playerid, COLOR_LIGHTBLUE, str);

            new proxstr[128];
            switch(action)
            {
                case 0, 1: format(proxstr, sizeof(proxstr), "{FF8000}* {C2A2DA}%s cat dung cu vao kho co khi.", GetPlayerNameEx(playerid));
                case 2, 3: format(proxstr, sizeof(proxstr), "{FF8000}* {C2A2DA}%s lay dung cu tu kho co khi.", GetPlayerNameEx(playerid));
            }
            ProxDetector(30.0, playerid, proxstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
        }

        case DIALOG_MECH_KHO_LOG:
        {
            if(!response) return 1;
            new groupid = PlayerInfo[playerid][pMember];
            if(PlayerInfo[playerid][pRank] < 5 && !PlayerInfo[playerid][pLeader]) return 1;

            new filename[64];
            static line[256], string[4096];
            string[0] = EOS;
            if(listitem == 0)
                format(filename, sizeof(filename), "MechLog_%d_money.log", groupid);
            else
                format(filename, sizeof(filename), "MechLog_%d_tool.log", groupid);

            if(!fexist(filename)) {
                strcat(string, "{AAAAAA}Chua co log nao.", sizeof(string));
            } else {
                // Doc 50 dong cuoi cung
                new File:f = fopen(filename, io_read);
                if(f == File:0) { strcat(string, "{AAAAAA}Khong the mo file log.", sizeof(string)); }
                else {
                static lines[MAX_MECH_LOG_ENTRIES][256];
                new count = 0, total = 0;
                while(fread(f, line)) {
                    strcopy(lines[total % MAX_MECH_LOG_ENTRIES], line, 256);
                    total++;
                }
                fclose(f);

                new start = (total > MAX_MECH_LOG_ENTRIES) ? (total % MAX_MECH_LOG_ENTRIES) : 0;
                count = (total > MAX_MECH_LOG_ENTRIES) ? MAX_MECH_LOG_ENTRIES : total;
                for(new i = 0; i < count; i++) {
                    new idx = (start + i) % MAX_MECH_LOG_ENTRIES;
                    strcat(string, lines[idx], sizeof(string));
                }
                } // end else fopen ok
            }

            new title[64];
            format(title, sizeof(title), "{FFFF00}Log %s - Group %d",
                (listitem == 0) ? "Tien" : "Dung Cu", groupid);
            ShowPlayerDialog(playerid, DIALOG_MECH_KHO_LOG_TIEN, DIALOG_STYLE_MSGBOX,
                title, string, "Dong", "");
        }

        // ==================== ADMIN DIALOGS ====================
        case DIALOG_ADMIN_REPAIR_ADD:
        {
            if(!response) return 1;
            new len = strlen(inputtext);
            if(len == 0 || len > 47) {
                SendClientMessageEx(playerid, COLOR_GREY, "Ten khong hop le! (1-47 ky tu)");
                return 1;
            }
            new id = gRepairCount;
            new gid = GetPVarInt(playerid, "MechAdminGroupID");
            strcopy(gRepairName[id], inputtext, 48);
            GetPlayerPos(playerid, gRepairX[id], gRepairY[id], gRepairZ[id]);
            gRepairGroupID[id] = gid;
            CreateRepairWorldObjects(id);
            gRepairCount++;
            SaveMechRepairSpots();

            new msg[128];
            format(msg, sizeof(msg), "Da them tru sua xe '{FFFF00}%s{FFFFFF}' (ID %d, Group %d).", inputtext, id, gid);
            SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
        }

        case DIALOG_ADMIN_REPAIR_LIST:
        {
            if(!response) return 1;
            if(listitem < 0 || listitem >= gRepairCount) return 1;
            if(PlayerInfo[playerid][pAdmin] < 1337) return 1;

            new delname[48];
            strcopy(delname, gRepairName[listitem], 48);
            DestroRepairWorldObjects(listitem);
            for(new i = listitem; i < gRepairCount - 1; i++) {
                strcopy(gRepairName[i], gRepairName[i+1], 48);
                gRepairX[i] = gRepairX[i+1]; gRepairY[i] = gRepairY[i+1]; gRepairZ[i] = gRepairZ[i+1];
                gRepairGroupID[i] = gRepairGroupID[i+1];
                gRepairLabel[i] = gRepairLabel[i+1]; gRepairPickup[i] = gRepairPickup[i+1];
            }
            gRepairCount--;
            SaveMechRepairSpots();
            new msg[128];
            format(msg, sizeof(msg), "Da xoa tru sua xe '{FFFF00}%s{FFFFFF}'.", delname);
            SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
        }

        case DIALOG_ADMIN_CRAFT_ADD:
        {
            if(!response) return 1;
            new len = strlen(inputtext);
            if(len == 0 || len > 47) {
                SendClientMessageEx(playerid, COLOR_GREY, "Ten khong hop le! (1-47 ky tu)");
                return 1;
            }
            new id = gCraftCount;
            new gid = GetPVarInt(playerid, "MechAdminGroupID");
            strcopy(gCraftName[id], inputtext, 48);
            GetPlayerPos(playerid, gCraftX[id], gCraftY[id], gCraftZ[id]);
            gCraftGroupID[id] = gid;
            CreateCraftWorldObjects(id);
            gCraftCount++;
            SaveMechCraftSpots();

            new msg[128];
            format(msg, sizeof(msg), "Da them xuong '{FFFF00}%s{FFFFFF}' (ID %d, Group %d).", inputtext, id, gid);
            SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
        }

        case DIALOG_ADMIN_CRAFT_LIST:
        {
            if(!response) return 1;
            if(listitem < 0 || listitem >= gCraftCount) return 1;
            if(PlayerInfo[playerid][pAdmin] < 1337) return 1;

            new delname[48];
            strcopy(delname, gCraftName[listitem], 48);
            DestroyCraftWorldObjects(listitem);
            for(new i = listitem; i < gCraftCount - 1; i++) {
                strcopy(gCraftName[i], gCraftName[i+1], 48);
                gCraftX[i] = gCraftX[i+1]; gCraftY[i] = gCraftY[i+1]; gCraftZ[i] = gCraftZ[i+1];
                gCraftGroupID[i] = gCraftGroupID[i+1];
                gCraftLabel[i] = gCraftLabel[i+1]; gCraftPickup[i] = gCraftPickup[i+1];
            }
            gCraftCount--;
            SaveMechCraftSpots();
            new msg[128];
            format(msg, sizeof(msg), "Da xoa xuong '{FFFF00}%s{FFFFFF}'.", delname);
            SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
        }

        case DIALOG_ADMIN_STORAGE_ADD:
        {
            if(!response) return 1;
            new len = strlen(inputtext);
            if(len == 0 || len > 47) {
                SendClientMessageEx(playerid, COLOR_GREY, "Ten khong hop le! (1-47 ky tu)");
                return 1;
            }
            new id = gStorageCount;
            new gid = GetPVarInt(playerid, "MechAdminGroupID");
            strcopy(gStorageName[id], inputtext, 48);
            GetPlayerPos(playerid, gStorageX[id], gStorageY[id], gStorageZ[id]);
            gStorageGroupID[id] = gid;
            CreateStorageWorldObjects(id);
            gStorageCount++;
            SaveMechStorageSpots();

            new msg[128];
            format(msg, sizeof(msg), "Da them kho '{FFFF00}%s{FFFFFF}' (ID %d, Group %d).", inputtext, id, gid);
            SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
        }

        case DIALOG_ADMIN_STORAGE_LIST:
        {
            if(!response) return 1;
            if(listitem < 0 || listitem >= gStorageCount) return 1;
            if(PlayerInfo[playerid][pAdmin] < 1337) return 1;

            new delname[48];
            strcopy(delname, gStorageName[listitem], 48);
            DestroyStorageWorldObjects(listitem);
            for(new i = listitem; i < gStorageCount - 1; i++) {
                strcopy(gStorageName[i], gStorageName[i+1], 48);
                gStorageX[i] = gStorageX[i+1]; gStorageY[i] = gStorageY[i+1]; gStorageZ[i] = gStorageZ[i+1];
                gStorageGroupID[i] = gStorageGroupID[i+1];
                gStorageLabel[i] = gStorageLabel[i+1]; gStoragePickup[i] = gStoragePickup[i+1];
            }
            gStorageCount--;
            SaveMechStorageSpots();
            new msg[128];
            format(msg, sizeof(msg), "Da xoa kho '{FFFF00}%s{FFFFFF}'.", delname);
            SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
        }

        // ==================== ADMIN MENU /amechanic ====================
        case DIALOG_ADMIN_MECH_MENU:
        {
            if(!response) return 1;
            if(PlayerInfo[playerid][pAdmin] < 1337) return 1;

            switch(listitem)
            {
                case 0:  { SetPVarInt(playerid, "MechAdminType", 0); ShowPlayerDialog(playerid, DIALOG_ADMIN_MECH_ADD_GID, DIALOG_STYLE_INPUT, "{FF6600}Them Tru Sua Xe", "{FFFFFF}Nhap Group ID:", "OK", "Huy"); }
                case 1:  PC_EmulateCommand(playerid, "/editrepair delete");
                case 2:  { SetPVarInt(playerid, "MechAdminType", 0); ShowPlayerDialog(playerid, DIALOG_ADMIN_MECH_SETPOS_ID, DIALOG_STYLE_INPUT, "{FF6600}Di Chuyen Tru Sua Xe", "{FFFFFF}Nhap ID tru sua xe:", "OK", "Huy"); }
                case 3:  PC_EmulateCommand(playerid, "/editrepair reload");
                case 4:  { SetPVarInt(playerid, "MechAdminType", 1); ShowPlayerDialog(playerid, DIALOG_ADMIN_MECH_ADD_GID, DIALOG_STYLE_INPUT, "{00CCFF}Them Xuong Che Tao", "{FFFFFF}Nhap Group ID:", "OK", "Huy"); }
                case 5:  PC_EmulateCommand(playerid, "/editcraft delete");
                case 6:  { SetPVarInt(playerid, "MechAdminType", 1); ShowPlayerDialog(playerid, DIALOG_ADMIN_MECH_SETPOS_ID, DIALOG_STYLE_INPUT, "{00CCFF}Di Chuyen Xuong Che Tao", "{FFFFFF}Nhap ID xuong:", "OK", "Huy"); }
                case 7:  PC_EmulateCommand(playerid, "/editcraft reload");
                case 8:  { SetPVarInt(playerid, "MechAdminType", 2); ShowPlayerDialog(playerid, DIALOG_ADMIN_MECH_ADD_GID, DIALOG_STYLE_INPUT, "{00FF88}Them Kho Mechanic", "{FFFFFF}Nhap Group ID:", "OK", "Huy"); }
                case 9:  PC_EmulateCommand(playerid, "/editstorage delete");
                case 10: { SetPVarInt(playerid, "MechAdminType", 2); ShowPlayerDialog(playerid, DIALOG_ADMIN_MECH_SETPOS_ID, DIALOG_STYLE_INPUT, "{00FF88}Di Chuyen Kho Mechanic", "{FFFFFF}Nhap ID kho:", "OK", "Huy"); }
                case 11: PC_EmulateCommand(playerid, "/editstorage reload");
            }
        }

        case DIALOG_ADMIN_MECH_ADD_GID:
        {
            if(!response) return PC_EmulateCommand(playerid, "/amechanic");
            if(PlayerInfo[playerid][pAdmin] < 1337) return 1;
            new gid = strval(inputtext);
            if(gid < 0) return SendClientMessageEx(playerid, COLOR_GREY, "Group ID khong hop le!");

            new cmd[40];
            switch(GetPVarInt(playerid, "MechAdminType"))
            {
                case 0: format(cmd, sizeof(cmd), "/editrepair add %d", gid);
                case 1: format(cmd, sizeof(cmd), "/editcraft add %d", gid);
                case 2: format(cmd, sizeof(cmd), "/editstorage add %d", gid);
            }
            PC_EmulateCommand(playerid, cmd);
        }

        case DIALOG_ADMIN_MECH_SETPOS_ID:
        {
            if(!response) return PC_EmulateCommand(playerid, "/amechanic");
            if(PlayerInfo[playerid][pAdmin] < 1337) return 1;
            new id = strval(inputtext);
            if(id < 0) return SendClientMessageEx(playerid, COLOR_GREY, "ID khong hop le!");

            new cmd[40];
            switch(GetPVarInt(playerid, "MechAdminType"))
            {
                case 0: format(cmd, sizeof(cmd), "/editrepair setpos %d", id);
                case 1: format(cmd, sizeof(cmd), "/editcraft setpos %d", id);
                case 2: format(cmd, sizeof(cmd), "/editstorage setpos %d", id);
            }
            PC_EmulateCommand(playerid, cmd);
        }
    }
    return 1;
}

// ============================================================
// COMMAND: /amechanic - Admin panel quan ly mechanic
// ============================================================
CMD:amechanic(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1337)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong co quyen su dung lenh nay!");

    new items[768];
    strcat(items, "{FF6600}[Tru Sua Xe]{FFFFFF} Them moi\n", sizeof(items));
    strcat(items, "{FF6600}[Tru Sua Xe]{FFFFFF} Xoa\n", sizeof(items));
    strcat(items, "{FF6600}[Tru Sua Xe]{FFFFFF} Di chuyen (setpos)\n", sizeof(items));
    strcat(items, "{FF6600}[Tru Sua Xe]{FFFFFF} Tai lai\n", sizeof(items));
    strcat(items, "{00CCFF}[Xuong Che Tao]{FFFFFF} Them moi\n", sizeof(items));
    strcat(items, "{00CCFF}[Xuong Che Tao]{FFFFFF} Xoa\n", sizeof(items));
    strcat(items, "{00CCFF}[Xuong Che Tao]{FFFFFF} Di chuyen (setpos)\n", sizeof(items));
    strcat(items, "{00CCFF}[Xuong Che Tao]{FFFFFF} Tai lai\n", sizeof(items));
    strcat(items, "{00FF88}[Kho Mechanic]{FFFFFF} Them moi\n", sizeof(items));
    strcat(items, "{00FF88}[Kho Mechanic]{FFFFFF} Xoa\n", sizeof(items));
    strcat(items, "{00FF88}[Kho Mechanic]{FFFFFF} Di chuyen (setpos)\n", sizeof(items));
    strcat(items, "{00FF88}[Kho Mechanic]{FFFFFF} Tai lai", sizeof(items));

    ShowPlayerDialog(playerid, DIALOG_ADMIN_MECH_MENU, DIALOG_STYLE_LIST,
        "{FFFF00}Admin - Quan Ly Mechanic", items, "Chon", "Dong");
    return 1;
}

// ============================================================
// Them vao GPS main dialog (gps.inc) - option "Vi tri sua xe"
// ============================================================
// Trong ShowGPSMainDialog, them item "Vi tri sua xe mechanic"
// va xu ly trong DIALOG_MAP_TPHAT response de goi:
// ShowMechRepairGPS(playerid)

stock ShowMechRepairGPS(playerid)
{
    new string[4096], line[128];
    strcat(string, "{FFD700}STT\t{FFFFFF}Ten\t{87CEEB}Group\t{00FF88}Khoang cach\n", sizeof(string));

    for(new i = 0; i < gRepairCount; i++) {
        new dist = floatround(GetPlayerDistanceFromPoint(playerid, gRepairX[i], gRepairY[i], gRepairZ[i]));
        format(line, sizeof(line), "{FFD700}%d\t{FFFFFF}%s\t{87CEEB}%d\t{00FF88}%dm\n",
            i + 1, gRepairName[i], gRepairGroupID[i], dist);
        strcat(string, line, sizeof(string));
    }
    if(!gRepairCount)
        strcat(string, "{AAAAAA}Chua co tru sua xe nao.", sizeof(string));

    // Hien dialog - xu ly response de SetPlayerCP den vi tri
    ShowPlayerDialog(playerid, DIALOG_MECH_SUAXE, DIALOG_STYLE_TABLIST_HEADERS,
        "{FFFF00}GPS - Tru Sua Xe", string, "Chon", "Dong");
}
