// gamemodes/modules/house/house.inc
// He thong nha cap nhat [HOUSE REWORK]
//
// === SQL CAN CHAY TRUOC KHI KHOI DONG SERVER ===
//   ALTER TABLE houses ADD COLUMN IF NOT EXISTS Name VARCHAR(32) DEFAULT '' AFTER Description;
//   CREATE TABLE IF NOT EXISTS house_items (
//     id INT AUTO_INCREMENT PRIMARY KEY,
//     house_id INT NOT NULL,
//     item_cat TINYINT NOT NULL,
//     item_type INT NOT NULL,
//     item_amt INT NOT NULL DEFAULT 0,
//     INDEX idx_house_id (house_id)
//   );

#include <YSI\YSI_Coding\y_hooks>

// ===================== DEFINES =====================

#define MAX_HOUSE_SLOTS         20
#define HOUSE_STORE_EMPTY       0
#define HOUSE_STORE_ITEM        1
#define HOUSE_STORE_GUN         2
#define HOUSE_STORE_FUELCAN     3   // pBinhXang: HouseStoreAmt = raw float bits of pBinhXang

// ===================== STORAGE ARRAYS =====================

// Per-house storage (loaded tu DB khi server khoi dong)
new HouseStoreCat[MAX_HOUSES][MAX_HOUSE_SLOTS];
new HouseStoreType[MAX_HOUSES][MAX_HOUSE_SLOTS];
new HouseStoreAmt[MAX_HOUSES][MAX_HOUSE_SLOTS];

// Per-player buffer hien thi dialog storage
new HsDispCat[MAX_PLAYERS][MAX_HOUSE_SLOTS];
new HsDispType[MAX_PLAYERS][MAX_HOUSE_SLOTS];
new HsDispAmt[MAX_PLAYERS][MAX_HOUSE_SLOTS];
new HsDispSlot[MAX_PLAYERS][MAX_HOUSE_SLOTS]; // dialog row -> slot index trong house
new HsDispCount[MAX_PLAYERS];
new HsDispHouseId[MAX_PLAYERS];

// Per-admin buffer cho checkhouse
new AdminCheckHouseList[MAX_PLAYERS][10];
new AdminCheckHouseCount[MAX_PLAYERS];

// Per-player buffer chon item de gui vao
new HsPutCat[MAX_PLAYERS][30];
new HsPutType[MAX_PLAYERS][30];
new HsPutCount[MAX_PLAYERS];

// Interior door pickup + label (vi tri cua ra trong nha)
new HouseIntPickupID[MAX_HOUSES];
new Text3D:HouseIntTextID[MAX_HOUSES];

// Sell offer state
new HouseSellTarget[MAX_PLAYERS];    // playerid nguoi mua
new HouseSellHouseIdx[MAX_PLAYERS];  // houseid dang duoc ban
new HouseSellPrice[MAX_PLAYERS];     // gia thoa thuan

// ===================== HELPERS =====================

stock FindClosestHouseExterior(playerid, Float:range = 3.0)
{
    for(new i = 0; i < MAX_HOUSES; i++)
    {
        if(HouseInfo[i][hExteriorX] == 0.0) continue;
        if(GetPlayerInterior(playerid) != HouseInfo[i][hExtIW]) continue;
        if(GetPlayerVirtualWorld(playerid) != HouseInfo[i][hExtVW]) continue;
        if(IsPlayerInRangeOfPoint(playerid, range, HouseInfo[i][hExteriorX], HouseInfo[i][hExteriorY], HouseInfo[i][hExteriorZ]))
            return i;
    }
    return INVALID_HOUSE_ID;
}

stock GetPlayerHouseInterior(playerid)
{
    for(new i = 0; i < MAX_HOUSES; i++)
    {
        if(HouseInfo[i][hExteriorX] == 0.0) continue;
        if(HouseInfo[i][hIntVW] == 0 && HouseInfo[i][hIntIW] == 0) continue;
        if(GetPlayerVirtualWorld(playerid) == HouseInfo[i][hIntVW] &&
           GetPlayerInterior(playerid) == HouseInfo[i][hIntIW])
            return i;
    }
    return INVALID_HOUSE_ID;
}

stock bool:IsHouseOwnerSQL(playerid, houseid)
{
    return bool:(HouseInfo[houseid][hOwnerID] == GetPlayerSQLId(playerid));
}

stock bool:CanEnterHouse(playerid, houseid)
{
    return bool:(HouseInfo[houseid][hLock] == 0);
}

stock CountPlayerHousesSQL(playerid)
{
    new count = 0, sqlid = GetPlayerSQLId(playerid);
    for(new i = 0; i < MAX_HOUSES; i++)
    {
        if(HouseInfo[i][hOwnerID] == sqlid) count++;
    }
    return count;
}

stock GetHouseDisplayName(houseid, output[], len)
{
    if(HouseInfo[houseid][hName][0])
        format(output, len, "%s", HouseInfo[houseid][hName]);
    else
        format(output, len, "Ngoi nha #%d", houseid);
}

// Tim slot co san cho cat+type (stack neu da co, hoac slot trong dau tien)
stock House_FindSlot(houseid, cat, itype)
{
    new firstEmpty = -1;
    for(new s = 0; s < MAX_HOUSE_SLOTS; s++)
    {
        if(HouseStoreCat[houseid][s] == HOUSE_STORE_EMPTY)
        {
            if(firstEmpty == -1) firstEmpty = s;
            continue;
        }
        if(HouseStoreCat[houseid][s] == cat && HouseStoreType[houseid][s] == itype)
            return s; // da co, stack vao day
    }
    return firstEmpty; // -1 neu het slot
}

// ===================== PICKUP / 3D LABEL =====================

stock House_ReloadPickupLabel(houseid)
{
    if(HouseInfo[houseid][hExteriorX] == 0.0) return;

    if(IsValidDynamicPickup(HouseInfo[houseid][hPickupID]))
        DestroyDynamicPickup(HouseInfo[houseid][hPickupID]);
    if(IsValidDynamic3DTextLabel(HouseInfo[houseid][hTextID]))
        DestroyDynamic3DTextLabel(HouseInfo[houseid][hTextID]);

    // Interior door marker
    if(IsValidDynamicPickup(HouseIntPickupID[houseid]))
        DestroyDynamicPickup(HouseIntPickupID[houseid]);
    if(IsValidDynamic3DTextLabel(HouseIntTextID[houseid]))
        DestroyDynamic3DTextLabel(HouseIntTextID[houseid]);

    new szText[300], szZone[MAX_ZONE_NAME], szHouseName[36];
    Get3DZone(HouseInfo[houseid][hExteriorX], HouseInfo[houseid][hExteriorY], HouseInfo[houseid][hExteriorZ], szZone, MAX_ZONE_NAME);
    GetHouseDisplayName(houseid, szHouseName, sizeof(szHouseName));

    if(HouseInfo[houseid][hOwned])
    {
        new szStatus[48];
        if(HouseInfo[houseid][hLock])
            format(szStatus, sizeof(szStatus), "{FF4444}Dang khoa");
        else
            format(szStatus, sizeof(szStatus), "{44FF44}Dang mo cua");

        format(szText, sizeof(szText),
            "{FFFF44}%s\n{AAAAAA}So nha: {FF8800}#%d\n{AAAAAA}Chu so huu: {FF8800}%s\n{AAAAAA}Khu vuc: {FFFFFF}%s\n{AAAAAA}Trang thai: %s\n{FFFFFF}Nhan {FF8000}[Y] {FFFFFF}de vao nha.",
            szHouseName, houseid,
            StripUnderscore(HouseInfo[houseid][hOwnerName]),
            szZone, szStatus
        );
        HouseInfo[houseid][hPickupID] = CreateDynamicPickup(19524, 23,
            HouseInfo[houseid][hExteriorX], HouseInfo[houseid][hExteriorY], HouseInfo[houseid][hExteriorZ],
            .worldid = HouseInfo[houseid][hExtVW], .interiorid = HouseInfo[houseid][hExtIW]);
        HouseInfo[houseid][hTextID] = CreateDynamic3DTextLabel(szText, 0xFFFFFFFF,
            HouseInfo[houseid][hExteriorX], HouseInfo[houseid][hExteriorY], HouseInfo[houseid][hExteriorZ] + 0.5,
            10.0, .testlos = 1, .worldid = HouseInfo[houseid][hExtVW], .interiorid = HouseInfo[houseid][hExtIW], .streamdistance = 10.0);
    }
    else
    {
        format(szText, sizeof(szText),
            "{FFFF00}Ngoi nha dang ban!\n{AAAAAA}So nha: {FF8800}#%d\n{AAAAAA}Gia ban: {44FF44}$%s\n{AAAAAA}Khu vuc: {FFFFFF}%s\n{AAAAAA}Level yeu cau: {FF8800}%d\n{FFFF00}/muanha {FFFFFF}de mua ngoi nha nay.",
            houseid, number_format(HouseInfo[houseid][hValue]), szZone, HouseInfo[houseid][hLevel]
        );
        HouseInfo[houseid][hPickupID] = CreateDynamicPickup(1273, 23,
            HouseInfo[houseid][hExteriorX], HouseInfo[houseid][hExteriorY], HouseInfo[houseid][hExteriorZ],
            .worldid = HouseInfo[houseid][hExtVW], .interiorid = HouseInfo[houseid][hExtIW]);
        HouseInfo[houseid][hTextID] = CreateDynamic3DTextLabel(szText, 0xFFFFFFFF,
            HouseInfo[houseid][hExteriorX], HouseInfo[houseid][hExteriorY], HouseInfo[houseid][hExteriorZ] + 0.5,
            10.0, .testlos = 1, .worldid = HouseInfo[houseid][hExtVW], .interiorid = HouseInfo[houseid][hExtIW], .streamdistance = 10.0);
    }

    // Interior door marker (hien thi vi tri cua ra trong nha)
    if(HouseInfo[houseid][hInteriorX] != 0.0)
    {
        HouseIntPickupID[houseid] = CreateDynamicPickup(1318, 23,
            HouseInfo[houseid][hInteriorX], HouseInfo[houseid][hInteriorY], HouseInfo[houseid][hInteriorZ],
            .worldid = HouseInfo[houseid][hIntVW], .interiorid = HouseInfo[houseid][hIntIW]);
        new szIntLock[48], szIntLabel[192];
        if(HouseInfo[houseid][hLock])
            format(szIntLock, sizeof(szIntLock), "{FF4444}Dang khoa");
        else
            format(szIntLock, sizeof(szIntLock), "{44FF44}Dang mo cua");
        format(szIntLabel, sizeof(szIntLabel), "{FFFF44}%s\n{AAAAAA}So nha: {FF8800}#%d\n{AAAAAA}Trang thai: %s\n{FFFFFF}Nhan {FF8000}[Y] {FFFFFF}de ra khoi nha.", szHouseName, houseid, szIntLock);
        HouseIntTextID[houseid] = CreateDynamic3DTextLabel(szIntLabel,
            0xFFFFFFFF,
            HouseInfo[houseid][hInteriorX], HouseInfo[houseid][hInteriorY], HouseInfo[houseid][hInteriorZ] + 0.5,
            10.0, .testlos = 0, .worldid = HouseInfo[houseid][hIntVW], .interiorid = HouseInfo[houseid][hIntIW], .streamdistance = 10.0);
    }

    HintArea_SetHouse(houseid);
}

// ===================== MYSQL =====================

stock House_LoadAllItems()
{
    printf("[House] Loading house_items from database...");
    mysql_function_query(MainPipeline, "SELECT house_id, item_cat, item_type, item_amt FROM house_items", true, "House_OnLoadAllItems", "i", 0);
}

forward House_OnLoadAllItems(dummy);
public House_OnLoadAllItems(dummy)
{
    new rows, fields, tmp[32];
    cache_get_data(rows, fields, MainPipeline);
    for(new row = 0; row < rows; row++)
    {
        cache_get_field_content(row, "house_id", tmp, MainPipeline);
        new hid = strval(tmp) - 1; // DB dung 1-based, array dung 0-based
        if(hid < 0 || hid >= MAX_HOUSES) continue;

        cache_get_field_content(row, "item_cat", tmp, MainPipeline);
        new cat = strval(tmp);
        cache_get_field_content(row, "item_type", tmp, MainPipeline);
        new itype = strval(tmp);
        cache_get_field_content(row, "item_amt", tmp, MainPipeline);
        new amt = strval(tmp);

        new slot = House_FindSlot(hid, cat, itype);
        if(slot == -1) continue;
        HouseStoreCat[hid][slot] = cat;
        HouseStoreType[hid][slot] = itype;
        HouseStoreAmt[hid][slot] = amt;
    }
    printf("[House] Loaded %d house item records.", rows);
}

stock House_SaveStorage(houseid)
{
    new query[256];
    format(query, sizeof(query), "DELETE FROM house_items WHERE house_id = %d", houseid + 1);
    mysql_function_query(MainPipeline, query, false, "OnQueryFinish", "i", SENDDATA_THREAD);

    for(new s = 0; s < MAX_HOUSE_SLOTS; s++)
    {
        if(HouseStoreCat[houseid][s] == HOUSE_STORE_EMPTY) continue;
        format(query, sizeof(query),
            "INSERT INTO house_items (house_id, item_cat, item_type, item_amt) VALUES (%d,%d,%d,%d)",
            houseid + 1,
            HouseStoreCat[houseid][s],
            HouseStoreType[houseid][s],
            HouseStoreAmt[houseid][s]);
        mysql_function_query(MainPipeline, query, false, "OnQueryFinish", "i", SENDDATA_THREAD);
    }
}

stock House_SaveName(houseid)
{
    new query[256];
    format(query, sizeof(query),
        "UPDATE houses SET Name='%s' WHERE id=%d",
        g_mysql_ReturnEscaped(HouseInfo[houseid][hName], MainPipeline),
        houseid + 1);
    mysql_function_query(MainPipeline, query, false, "OnQueryFinish", "i", SENDDATA_THREAD);
}

// ===================== ENTER / EXIT =====================

stock House_TryEnter(playerid)
{
    new h = FindClosestHouseExterior(playerid, 3.0);
    if(h == INVALID_HOUSE_ID) return 0;

    if(HouseInfo[h][hInteriorX] == 0.0)
        return SendClientMessageEx(playerid, COLOR_GRAD2, "Ngoi nha nay chua co noi that."), 1;

    if(!CanEnterHouse(playerid, h))
    {
        GameTextForPlayer(playerid, "~r~Khoa cua", 5000, 1);
        return 1;
    }

    SetPlayerInterior(playerid, HouseInfo[h][hIntIW]);
    SetPlayerVirtualWorld(playerid, HouseInfo[h][hIntVW]);
    SetPlayerPos(playerid, HouseInfo[h][hInteriorX], HouseInfo[h][hInteriorY], HouseInfo[h][hInteriorZ]);
    SetPlayerFacingAngle(playerid, HouseInfo[h][hInteriorA]);
    SetCameraBehindPlayer(playerid);
    PlayerInfo[playerid][pInt] = HouseInfo[h][hIntIW];
    PlayerInfo[playerid][pVW] = HouseInfo[h][hIntVW];

    if(IsHouseOwnerSQL(playerid, h))
        SendHintMessage(playerid, "Su dung {FFFF00}/house {FFFFFF}de quan ly can nha cua ban.");
    else
        GameTextForPlayer(playerid, "~w~Chao mung den ngoi nha nay", 5000, 1);

    new szProx[128];
    format(szProx, sizeof(szProx), "{FF8000}* {C2A2DA}%s buoc vao nha.", GetPlayerNameEx(playerid));
    ProxDetector(30.0, playerid, szProx, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

    if(HouseInfo[h][hCustomInterior] == 1)
        Player_StreamPrep(playerid, HouseInfo[h][hInteriorX], HouseInfo[h][hInteriorY], HouseInfo[h][hInteriorZ], FREEZE_TIME);

    return 1;
}

stock House_TryExit(playerid)
{
    new h = GetPlayerHouseInterior(playerid);
    if(h == INVALID_HOUSE_ID) return 0;

    if(!IsPlayerInRangeOfPoint(playerid, 3.0, HouseInfo[h][hInteriorX], HouseInfo[h][hInteriorY], HouseInfo[h][hInteriorZ]))
        return 0;

    if(HouseInfo[h][hLock])
    {
        GameTextForPlayer(playerid, "~r~Khoa cua", 5000, 1);
        return 1;
    }

    SetPlayerInterior(playerid, HouseInfo[h][hExtIW]);
    SetPlayerVirtualWorld(playerid, HouseInfo[h][hExtVW]);
    SetPlayerPos(playerid, HouseInfo[h][hExteriorX], HouseInfo[h][hExteriorY], HouseInfo[h][hExteriorZ]);
    SetPlayerFacingAngle(playerid, HouseInfo[h][hExteriorA]);
    SetCameraBehindPlayer(playerid);
    PlayerInfo[playerid][pInt] = HouseInfo[h][hExtIW];
    PlayerInfo[playerid][pVW] = HouseInfo[h][hExtVW];

    new szProx[128];
    format(szProx, sizeof(szProx), "{FF8000}* {C2A2DA}%s ra khoi nha.", GetPlayerNameEx(playerid));
    ProxDetector(30.0, playerid, szProx, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

    if(HouseInfo[h][hCustomExterior] == 1)
        Player_StreamPrep(playerid, HouseInfo[h][hExteriorX], HouseInfo[h][hExteriorY], HouseInfo[h][hExteriorZ], FREEZE_TIME);

    return 1;
}

// ALT key: khoa/mo cua nha tu ben ngoai (goi tu callbacks.inc)
stock House_TryALTLock(playerid)
{
    new h = FindClosestHouseExterior(playerid, 2.0);
    if(h == INVALID_HOUSE_ID)
    {
        h = GetPlayerHouseInterior(playerid);
        if(h != INVALID_HOUSE_ID && !IsPlayerInRangeOfPoint(playerid, 2.0, HouseInfo[h][hInteriorX], HouseInfo[h][hInteriorY], HouseInfo[h][hInteriorZ]))
            h = INVALID_HOUSE_ID;
    }
    if(h == INVALID_HOUSE_ID) return 0;
    if(!IsHouseOwnerSQL(playerid, h)) return 0;

    HouseInfo[h][hLock] = (HouseInfo[h][hLock] == 0) ? 1 : 0;
    House_ReloadPickupLabel(h);
    SaveHouse(h);
    HintKey_RefreshHouse(h);

    new Float:lx = HouseInfo[h][hExteriorX], Float:ly = HouseInfo[h][hExteriorY], Float:lz = HouseInfo[h][hExteriorZ];
    foreach(new i : Player)
    {
        if(IsPlayerInRangeOfPoint(i, 10.0, lx, ly, lz))
            PlayerPlaySound(i, 24600, lx, ly, lz);
    }

    new szProx[128];
    format(szProx, sizeof(szProx), "{FF8000}* {C2A2DA}%s %s nha.", GetPlayerNameEx(playerid), HouseInfo[h][hLock] ? "khoa cua" : "mo cua");
    ProxDetector(30.0, playerid, szProx, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

    if(HouseInfo[h][hLock])
        HienTextdraw(playerid, "~r~KHOA NHA");
    else
        HienTextdraw(playerid, "~g~MO NHA");

    return 1;
}

// ===================== HOOK: Enter/Exit (Y key) =====================

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if(PRESSED(KEY_YES))
    {
        if(House_TryEnter(playerid)) return 1;
        if(House_TryExit(playerid))  return 1;
    }
    if(PRESSED(KEY_WALK))
    {
        House_TryALTLock(playerid);
    }
    return 1;
}

// ===================== DIALOG SHOW FUNCTIONS =====================

stock ShowHouseMainDialog(playerid, houseid)
{
    new szTitle[64], szHouseName[36];
    GetHouseDisplayName(houseid, szHouseName, sizeof(szHouseName));
    format(szTitle, sizeof(szTitle), "Nha > %s", szHouseName);

    new szLock[64];
    if(HouseInfo[houseid][hLock])
        format(szLock, sizeof(szLock), "Trang thai: {FF4444}Dang khoa {AAAAAA}(nhan de mo)");
    else
        format(szLock, sizeof(szLock), "Trang thai: {44FF44}Dang mo cua {AAAAAA}(nhan de khoa)");

    new szList[512];
    format(szList, sizeof(szList), "{AAAAAA}So nha: {FFFFFF}#%d\nDoi ten nha\nTu do\n%s\nDuoi nguoi trong nha\nBan nha\n", houseid, szLock);

    SetPVarInt(playerid, "HouseManageId", houseid);
    ShowPlayerDialog(playerid, DIALOG_HOUSE_MAIN, DIALOG_STYLE_LIST, szTitle, szList, "Chon", "Dong");
}

stock ShowHouseStorageDialog(playerid, houseid)
{
    new szTitle[64], szList[1024], szRow[96], szHouseName[36];
    GetHouseDisplayName(houseid, szHouseName, sizeof(szHouseName));
    format(szTitle, sizeof(szTitle), "Tu do - %s", szHouseName);

    // TABLIST_HEADERS: dong dau la header
    format(szList, sizeof(szList), "Vat pham\tSo luong\n");

    HsDispCount[playerid] = 0;
    HsDispHouseId[playerid] = houseid;

    for(new s = 0; s < MAX_HOUSE_SLOTS; s++)
    {
        if(HouseStoreCat[houseid][s] == HOUSE_STORE_EMPTY) continue;
        new cat   = HouseStoreCat[houseid][s];
        new itype = HouseStoreType[houseid][s];
        new amt   = HouseStoreAmt[houseid][s];

        if(cat == HOUSE_STORE_GUN)
        {
            new szWep[32];
            GetWeaponName(itype, szWep, sizeof(szWep));
            format(szRow, sizeof(szRow), "{FF8000}[Sung] {FFFFFF}%s\t%d\n", szWep, amt);
        }
        else if(cat == HOUSE_STORE_FUELCAN)
        {
            if(itype < 0 || itype >= sizeof(ItemInventory)) continue;
            new Float:fLiters = Float:amt - FUELCAN_EMPTY_MARKER;
            format(szRow, sizeof(szRow), "{00FF88}[Vat pham] {FFFFFF}%s\t%.1f L\n", ItemInventory[itype][NameItem], fLiters);
        }
        else
        {
            if(itype < 0 || itype >= sizeof(ItemInventory)) continue;
            format(szRow, sizeof(szRow), "{00FF88}[Vat pham] {FFFFFF}%s\t%d\n", ItemInventory[itype][NameItem], amt);
        }

        strcat(szList, szRow, sizeof(szList));
        HsDispCat[playerid][HsDispCount[playerid]]  = cat;
        HsDispType[playerid][HsDispCount[playerid]] = itype;
        HsDispAmt[playerid][HsDispCount[playerid]]  = amt;
        HsDispSlot[playerid][HsDispCount[playerid]] = s;
        HsDispCount[playerid]++;
        if(HsDispCount[playerid] >= MAX_HOUSE_SLOTS) break;
    }

    // Dong cuoi: (+) Them
    strcat(szList, "{00FF00}(+) Them vat pham\t{AAAAAA}-\n", sizeof(szList));

    SetPVarInt(playerid, "HouseManageId", houseid);
    ShowPlayerDialog(playerid, DIALOG_HOUSE_STORAGE, DIALOG_STYLE_TABLIST_HEADERS, szTitle, szList, "Thao tac", "Dong");
}

stock ShowHouseStoragePutDialog(playerid)
{
    new szList[1024], szRow[96];
    HsPutCount[playerid] = 0;

    // Vu khi
    for(new s = 0; s < 12; s++)
    {
        if(PlayerInfo[playerid][pGuns][s] == 0) continue;
        new szWep[32];
        GetWeaponName(PlayerInfo[playerid][pGuns][s], szWep, sizeof(szWep));
        format(szRow, sizeof(szRow), "{FF8000}[Sung] {FFFFFF}%s\n", szWep);
        strcat(szList, szRow, sizeof(szList));
        HsPutCat[playerid][HsPutCount[playerid]]  = HOUSE_STORE_GUN;
        HsPutType[playerid][HsPutCount[playerid]] = PlayerInfo[playerid][pGuns][s];
        HsPutCount[playerid]++;
        if(HsPutCount[playerid] >= 30) break;
    }

    // Vat pham tu inventory
    for(new i = 0; i < sizeof(ItemInventory); i++)
    {
        if(ItemInventory[i][pVariable] == pIDCard) continue;
        if(ItemInventory[i][pVariable] == (_:pBinhXang))
        {
            if(!(PlayerInfo[playerid][pBinhXang] > 0.0)) continue;
            new Float:fLiters = PlayerInfo[playerid][pBinhXang] - FUELCAN_EMPTY_MARKER;
            format(szRow, sizeof(szRow), "{00FF88}[Vat pham] {FFFFFF}%s {AAAAAA}(%.1f L)\n", ItemInventory[i][NameItem], fLiters);
            strcat(szList, szRow, sizeof(szList));
            HsPutCat[playerid][HsPutCount[playerid]]  = HOUSE_STORE_FUELCAN;
            HsPutType[playerid][HsPutCount[playerid]] = i;
            HsPutCount[playerid]++;
            if(HsPutCount[playerid] >= 30) break;
            continue;
        }
        if(PlayerInfo[playerid][ItemInventory[i][pVariable]] <= 0) continue;
        format(szRow, sizeof(szRow), "{00FF88}[Vat pham] {FFFFFF}%s {AAAAAA}(x%d)\n",
            ItemInventory[i][NameItem],
            PlayerInfo[playerid][ItemInventory[i][pVariable]]);
        strcat(szList, szRow, sizeof(szList));
        HsPutCat[playerid][HsPutCount[playerid]]  = HOUSE_STORE_ITEM;
        HsPutType[playerid][HsPutCount[playerid]] = i;
        HsPutCount[playerid]++;
        if(HsPutCount[playerid] >= 30) break;
    }

    if(HsPutCount[playerid] == 0)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong co vat pham nao de cat vao tu do.");

    ShowPlayerDialog(playerid, DIALOG_HOUSE_STORAGE_PUT, DIALOG_STYLE_LIST, "Chon vat pham de cat vao", szList, "Chon", "Quay lai");
    return 1;
}

stock ShowHouseSellBuyerDialog(playerid, houseid)
{
    new szInfo[128], szHouseName[36];
    GetHouseDisplayName(houseid, szHouseName, sizeof(szHouseName));
    format(szInfo, sizeof(szInfo), "{ffffff}Nha: %s\n{ffffff}Nhap ID nguoi choi muon ban:", szHouseName);
    SetPVarInt(playerid, "HouseManageId", houseid);
    ShowPlayerDialog(playerid, DIALOG_HOUSE_SELL, DIALOG_STYLE_INPUT, "Ban nha - Chon nguoi mua", szInfo, "Tiep theo", "Huy");
}

stock ShowHouseSellPriceDialog(playerid, houseid, buyerid)
{
    new szInfo[160], szHouseName[36];
    GetHouseDisplayName(houseid, szHouseName, sizeof(szHouseName));
    format(szInfo, sizeof(szInfo), "{ffffff}Nha: %s\n{ffffff}Nguoi mua: %s (ID: %d)\nNhap gia ban:", szHouseName, StripUnderscore(GetPlayerNameEx(buyerid)), buyerid);
    SetPVarInt(playerid, "HouseManageId", houseid);
    SetPVarInt(playerid, "HouseSellBuyerID", buyerid);
    ShowPlayerDialog(playerid, DIALOG_HOUSE_SELL_PRICE, DIALOG_STYLE_INPUT, "Ban nha - Dat gia", szInfo, "Xac nhan", "Quay lai");
}

stock ShowHouseEvictDialog(playerid, houseid)
{
    new szList[512], szRow[64], count = 0;
    foreach(new i : Player)
    {
        if(i == playerid) continue;
        if(GetPlayerHouseInterior(i) != houseid) continue;
        format(szRow, sizeof(szRow), "%s\n", StripUnderscore(GetPlayerNameEx(i)));
        strcat(szList, szRow, sizeof(szList));
        count++;
    }
    if(count == 0)
        return SendClientMessageEx(playerid, COLOR_GREY, "Hien tai khong co ai dang o trong nha nay."), 1;

    SetPVarInt(playerid, "HouseManageId", houseid);
    ShowPlayerDialog(playerid, DIALOG_HOUSE_EVICT, DIALOG_STYLE_LIST, "Duoi nguoi thue", szList, "Duoi", "Huy");
    return 1;
}

stock ShowHouseShopDialog(playerid)
{
    new szList[2048], szRow[128], count = 0;
    new szZone[MAX_ZONE_NAME];
    format(szList, sizeof(szList), "So nha\tGia ban\tKhu vuc\tLevel\n");

    for(new i = 0; i < MAX_HOUSES; i++)
    {
        if(HouseInfo[i][hExteriorX] == 0.0) continue;
        if(HouseInfo[i][hOwned]) continue;
        Get3DZone(HouseInfo[i][hExteriorX], HouseInfo[i][hExteriorY], HouseInfo[i][hExteriorZ], szZone, MAX_ZONE_NAME);
        format(szRow, sizeof(szRow), "#%d\t$%s\t%s\tLv.%d\n", i, number_format(HouseInfo[i][hValue]), szZone, HouseInfo[i][hLevel]);
        strcat(szList, szRow, sizeof(szList));
        count++;
        if(count >= 50) break; // gioi han hien thi 50 nha
    }

    if(count == 0)
        return SendClientMessageEx(playerid, COLOR_GREY, "Hien tai khong co ngoi nha nao dang ban."), 1;

    ShowPlayerDialog(playerid, DIALOG_HOUSESHOP_NEW, DIALOG_STYLE_TABLIST_HEADERS, "Danh sach nha dang ban", szList, "Xem vi tri", "Dong");
    return 1;
}

stock ShowMyHouseDialog(playerid)
{
    new szList[512], szRow[128], count = 0;
    new szZone[MAX_ZONE_NAME], szHouseName[36];
    new sqlid = GetPlayerSQLId(playerid);

    format(szList, sizeof(szList), "Ten nha\tKhu vuc\tTrang thai\n");

    for(new i = 0; i < MAX_HOUSES; i++)
    {
        if(HouseInfo[i][hOwnerID] != sqlid) continue;
        Get3DZone(HouseInfo[i][hExteriorX], HouseInfo[i][hExteriorY], HouseInfo[i][hExteriorZ], szZone, MAX_ZONE_NAME);
        GetHouseDisplayName(i, szHouseName, sizeof(szHouseName));
        new szStatus[32];
        if(HouseInfo[i][hLock])
            format(szStatus, sizeof(szStatus), "{FF4444}Khoa");
        else
            format(szStatus, sizeof(szStatus), "{44FF44}Mo");
        format(szRow, sizeof(szRow), "%s\t%s\t%s\n", szHouseName, szZone, szStatus);
        strcat(szList, szRow, sizeof(szList));
        count++;
    }

    if(count == 0)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban hien khong co ngoi nha nao."), 1;

    ShowPlayerDialog(playerid, DIALOG_MYHOUSE, DIALOG_STYLE_TABLIST_HEADERS, "Nha cua ban", szList, "Dong", "");
    return 1;
}

// ===================== DIALOG RESPONSE =====================

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    new houseid = GetPVarInt(playerid, "HouseManageId");

    // --- /house main menu ---
    if(dialogid == DIALOG_HOUSE_MAIN)
    {
        if(!response) return 1;
        if(houseid < 0 || houseid >= MAX_HOUSES) return 1;

        switch(listitem)
        {
            case 0: // So nha (info row, re-show)
            {
                ShowHouseMainDialog(playerid, houseid);
            }
            case 1: // Doi ten nha
            {
                new szInfo[96], szCurrent[36];
                GetHouseDisplayName(houseid, szCurrent, sizeof(szCurrent));
                format(szInfo, sizeof(szInfo), "{ffffff}Ten hien tai: %s\n{ffffff}Nhap ten moi (toi da 32 ky tu):", szCurrent);
                ShowPlayerDialog(playerid, DIALOG_HOUSE_RENAME, DIALOG_STYLE_INPUT, "Doi ten nha", szInfo, "Luu", "Huy");
            }
            case 2: // Tu do
            {
                ShowHouseStorageDialog(playerid, houseid);
            }
            case 3: // Khoa / Mo khoa
            {
                HouseInfo[houseid][hLock] = (HouseInfo[houseid][hLock] == 0) ? 1 : 0;
                House_ReloadPickupLabel(houseid);
                SaveHouse(houseid);
                HintKey_RefreshHouse(houseid);
                new Float:lx = HouseInfo[houseid][hExteriorX], Float:ly = HouseInfo[houseid][hExteriorY], Float:lz = HouseInfo[houseid][hExteriorZ];
                foreach(new i : Player)
                {
                    if(IsPlayerInRangeOfPoint(i, 10.0, lx, ly, lz))
                        PlayerPlaySound(i, 24600, lx, ly, lz);
                }
                new szMsg[64], szProx[128];
                format(szMsg, sizeof(szMsg), "{FFFFFF}Ngoi nha da duoc %s.", HouseInfo[houseid][hLock] ? "{FF4444}khoa" : "{44FF44}mo khoa");
                SendClientMessageEx(playerid, COLOR_WHITE, szMsg);
                format(szProx, sizeof(szProx), "{FF8000}* {C2A2DA}%s %s nha.", GetPlayerNameEx(playerid), HouseInfo[houseid][hLock] ? "khoa cua" : "mo cua");
                ProxDetector(30.0, playerid, szProx, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
                ShowHouseMainDialog(playerid, houseid);
            }
            case 4: // Duoi nguoi trong nha
            {
                ShowHouseEvictDialog(playerid, houseid);
            }
            case 5: // Ban nha
            {
                ShowHouseSellBuyerDialog(playerid, houseid);
            }
        }
        return 1;
    }

    // --- Doi ten nha ---
    if(dialogid == DIALOG_HOUSE_RENAME)
    {
        if(!response) { ShowHouseMainDialog(playerid, houseid); return 1; }
        if(houseid < 0 || houseid >= MAX_HOUSES) return 1;

        new szTrimmed[33];
        strmid(szTrimmed, inputtext, 0, 32);
        if(!szTrimmed[0])
        {
            SendClientMessageEx(playerid, COLOR_GREY, "Ten nha khong duoc de trong.");
            ShowPlayerDialog(playerid, DIALOG_HOUSE_RENAME, DIALOG_STYLE_INPUT, "Doi ten nha", "Nhap ten moi:", "Luu", "Huy");
            return 1;
        }
        format(HouseInfo[houseid][hName], 32, "%s", szTrimmed);
        House_SaveName(houseid);
        House_ReloadPickupLabel(houseid);
        new szMsg[64], szProx[128];
        format(szMsg, sizeof(szMsg), "Ten nha da duoc doi thanh: {FFFF00}%s", HouseInfo[houseid][hName]);
        SendClientMessageEx(playerid, COLOR_WHITE, szMsg);
        format(szProx, sizeof(szProx), "{FF8000}* {C2A2DA}%s doi ten nha.", GetPlayerNameEx(playerid));
        ProxDetector(30.0, playerid, szProx, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
        ShowHouseMainDialog(playerid, houseid);
        return 1;
    }

    // --- Tu do (storage view) ---
    if(dialogid == DIALOG_HOUSE_STORAGE)
    {
        if(!response) return 1;
        new hid = HsDispHouseId[playerid];
        if(hid < 0 || hid >= MAX_HOUSES) return 1;

        // listitem 0..HsDispCount-1 = items; listitem HsDispCount = "(+) Them"
        if(listitem == HsDispCount[playerid])
        {
            // Them vat pham
            ShowHouseStoragePutDialog(playerid);
            return 1;
        }
        if(listitem < 0 || listitem >= HsDispCount[playerid]) return 1;

        new cat   = HsDispCat[playerid][listitem];
        new itype = HsDispType[playerid][listitem];
        new amt   = HsDispAmt[playerid][listitem];
        new slot  = HsDispSlot[playerid][listitem];

        // Binh xang: khoi phuc float chinh xac
        if(cat == HOUSE_STORE_FUELCAN)
        {
            if(PlayerInfo[playerid][pBinhXang] > 0.0)
                return SendClientMessageEx(playerid, COLOR_GREY, "Ban da co binh xang roi, khong the lay them."), 1;
            PlayerInfo[playerid][pBinhXang] = Float:amt; // khoi phuc raw float bits
            HouseStoreCat[hid][slot]  = HOUSE_STORE_EMPTY;
            HouseStoreType[hid][slot] = 0;
            HouseStoreAmt[hid][slot]  = 0;
            House_SaveStorage(hid);
            new Float:fLiters = PlayerInfo[playerid][pBinhXang] - FUELCAN_EMPTY_MARKER;
            new szMsg[80], szProx[128];
            format(szMsg, sizeof(szMsg), "Ban da lay {00FF88}Binh xang (%.1f L) {FFFFFF}tu tu do.", fLiters);
            SendClientMessageEx(playerid, COLOR_WHITE, szMsg);
            format(szProx, sizeof(szProx), "{FF8000}* {C2A2DA}%s lay Binh xang ra khoi tu nha.", GetPlayerNameEx(playerid));
            ProxDetector(30.0, playerid, szProx, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
            ShowHouseStorageDialog(playerid, hid);
            return 1;
        }

        // Vu khi: lay ngay
        if(cat == HOUSE_STORE_GUN)
        {
            GivePlayerWeapon(playerid, itype, 50);
            // Xoa slot
            HouseStoreCat[hid][slot]  = HOUSE_STORE_EMPTY;
            HouseStoreType[hid][slot] = 0;
            HouseStoreAmt[hid][slot]  = 0;
            House_SaveStorage(hid);
            new szWep[32], szMsg[64], szProx[128];
            GetWeaponName(itype, szWep, sizeof(szWep));
            format(szMsg, sizeof(szMsg), "Ban da lay {FF8000}%s {FFFFFF}tu tu do.", szWep);
            SendClientMessageEx(playerid, COLOR_WHITE, szMsg);
            format(szProx, sizeof(szProx), "{FF8000}* {C2A2DA}%s lay %s ra khoi tu nha.", GetPlayerNameEx(playerid), szWep);
            ProxDetector(30.0, playerid, szProx, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
            ShowHouseStorageDialog(playerid, hid);
            return 1;
        }

        // Vat pham: hoi so luong
        if(amt == 1)
        {
            // Lay ngay
            if(itype >= 0 && itype < sizeof(ItemInventory))
                PlayerInfo[playerid][ItemInventory[itype][pVariable]]++;
            HouseStoreCat[hid][slot]  = HOUSE_STORE_EMPTY;
            HouseStoreType[hid][slot] = 0;
            HouseStoreAmt[hid][slot]  = 0;
            House_SaveStorage(hid);
            new szProx[128];
            SendClientMessageEx(playerid, COLOR_WHITE, "Ban da lay vat pham ra khoi tu do.");
            format(szProx, sizeof(szProx), "{FF8000}* {C2A2DA}%s lay %s ra khoi tu nha.", GetPlayerNameEx(playerid), ItemInventory[itype][NameItem]);
            ProxDetector(30.0, playerid, szProx, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
            ShowHouseStorageDialog(playerid, hid);
        }
        else
        {
            // Hoi so luong
            new szInfo[128];
            format(szInfo, sizeof(szInfo), "Vat pham: %s\nSo luong trong tu: %d\nNhap so luong muon lay:",
                ItemInventory[itype][NameItem], amt);
            SetPVarInt(playerid, "HsWithdrawSlot",  slot);
            SetPVarInt(playerid, "HsWithdrawType",  itype);
            SetPVarInt(playerid, "HsWithdrawMax",   amt);
            SetPVarInt(playerid, "HouseManageId",   hid);
            ShowPlayerDialog(playerid, DIALOG_HOUSE_STORAGE_AMT, DIALOG_STYLE_INPUT, "Lay vat pham", szInfo, "Lay", "Huy");
        }
        return 1;
    }

    // --- Nhap so luong lay ra ---
    if(dialogid == DIALOG_HOUSE_STORAGE_AMT)
    {
        new hid = GetPVarInt(playerid, "HouseManageId");
        if(!response) { ShowHouseStorageDialog(playerid, hid); return 1; }

        new amt = strval(inputtext);
        new maxAmt = GetPVarInt(playerid, "HsWithdrawMax");
        new slot   = GetPVarInt(playerid, "HsWithdrawSlot");
        new itype  = GetPVarInt(playerid, "HsWithdrawType");

        if(amt <= 0 || amt > maxAmt)
        {
            new szInfo[64];
            format(szInfo, sizeof(szInfo), "So luong khong hop le (1 - %d).", maxAmt);
            return SendClientMessageEx(playerid, COLOR_GREY, szInfo), 1;
        }

        PlayerInfo[playerid][ItemInventory[itype][pVariable]] += amt;
        HouseStoreAmt[hid][slot] -= amt;
        if(HouseStoreAmt[hid][slot] <= 0)
        {
            HouseStoreCat[hid][slot]  = HOUSE_STORE_EMPTY;
            HouseStoreType[hid][slot] = 0;
            HouseStoreAmt[hid][slot]  = 0;
        }
        House_SaveStorage(hid);
        new szMsg[64], szProx[128];
        format(szMsg, sizeof(szMsg), "Ban da lay {00FF88}%d %s {FFFFFF}tu tu do.", amt, ItemInventory[itype][NameItem]);
        SendClientMessageEx(playerid, COLOR_WHITE, szMsg);
        format(szProx, sizeof(szProx), "{FF8000}* {C2A2DA}%s lay %s ra khoi tu nha.", GetPlayerNameEx(playerid), ItemInventory[itype][NameItem]);
        ProxDetector(30.0, playerid, szProx, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
        ShowHouseStorageDialog(playerid, hid);
        return 1;
    }

    // --- Chon vat pham de gui vao ---
    if(dialogid == DIALOG_HOUSE_STORAGE_PUT)
    {
        new hid = GetPVarInt(playerid, "HouseManageId");
        if(!response) { ShowHouseStorageDialog(playerid, hid); return 1; }
        if(listitem < 0 || listitem >= HsPutCount[playerid]) return 1;

        new cat   = HsPutCat[playerid][listitem];
        new itype = HsPutType[playerid][listitem];

        // Vu khi: gui ngay
        if(cat == HOUSE_STORE_GUN)
        {
            new slot = House_FindSlot(hid, HOUSE_STORE_GUN, itype);
            if(slot == -1)
                return SendClientMessageEx(playerid, COLOR_GREY, "Tu do da day, khong the cat them."), 1;

            RemovePlayerWeapon(playerid, itype);
            // Xoa vu khi khoi PlayerInfo
            for(new s = 0; s < 12; s++)
            {
                if(PlayerInfo[playerid][pGuns][s] == itype)
                {
                    PlayerInfo[playerid][pGuns][s] = 0;
                    break;
                }
            }
            if(HouseStoreCat[hid][slot] == HOUSE_STORE_EMPTY)
            {
                HouseStoreCat[hid][slot]  = HOUSE_STORE_GUN;
                HouseStoreType[hid][slot] = itype;
                HouseStoreAmt[hid][slot]  = 1;
            }
            else HouseStoreAmt[hid][slot]++;

            House_SaveStorage(hid);
            new szWep[32], szMsg[64], szProx[128];
            GetWeaponName(itype, szWep, sizeof(szWep));
            format(szMsg, sizeof(szMsg), "Ban da cat {FF8000}%s {FFFFFF}vao tu do.", szWep);
            SendClientMessageEx(playerid, COLOR_WHITE, szMsg);
            format(szProx, sizeof(szProx), "{FF8000}* {C2A2DA}%s cat %s vao tu nha.", GetPlayerNameEx(playerid), szWep);
            ProxDetector(30.0, playerid, szProx, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
            ShowHouseStorageDialog(playerid, hid);
            return 1;
        }

        // Binh xang: luu raw float bits
        if(cat == HOUSE_STORE_FUELCAN)
        {
            if(!(PlayerInfo[playerid][pBinhXang] > 0.0))
                return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong co binh xang."), 1;
            new slot = House_FindSlot(hid, HOUSE_STORE_FUELCAN, itype);
            if(slot == -1) return SendClientMessageEx(playerid, COLOR_GREY, "Tu do da day."), 1;
            if(HouseStoreCat[hid][slot] != HOUSE_STORE_EMPTY)
                return SendClientMessageEx(playerid, COLOR_GREY, "Tu do da co binh xang roi."), 1;
            HouseStoreCat[hid][slot]  = HOUSE_STORE_FUELCAN;
            HouseStoreType[hid][slot] = itype;
            HouseStoreAmt[hid][slot]  = _:PlayerInfo[playerid][pBinhXang]; // raw float bits
            PlayerInfo[playerid][pBinhXang] = 0.0;
            House_SaveStorage(hid);
            SendClientMessageEx(playerid, COLOR_WHITE, "Ban da cat {00FF88}Binh xang {FFFFFF}vao tu do.");
            new szProx[128];
            format(szProx, sizeof(szProx), "{FF8000}* {C2A2DA}%s cat Binh xang vao tu nha.", GetPlayerNameEx(playerid));
            ProxDetector(30.0, playerid, szProx, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
            ShowHouseStorageDialog(playerid, hid);
            return 1;
        }

        // Vat pham: hoi so luong
        if(itype < 0 || itype >= sizeof(ItemInventory)) return 1;
        new have = PlayerInfo[playerid][ItemInventory[itype][pVariable]];
        if(have <= 0)
            return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong co vat pham nay."), 1;

        if(have == 1)
        {
            // Cat ngay
            new slot = House_FindSlot(hid, HOUSE_STORE_ITEM, itype);
            if(slot == -1) return SendClientMessageEx(playerid, COLOR_GREY, "Tu do da day."), 1;
            PlayerInfo[playerid][ItemInventory[itype][pVariable]]--;
            if(HouseStoreCat[hid][slot] == HOUSE_STORE_EMPTY)
            {
                HouseStoreCat[hid][slot]  = HOUSE_STORE_ITEM;
                HouseStoreType[hid][slot] = itype;
                HouseStoreAmt[hid][slot]  = 1;
            }
            else HouseStoreAmt[hid][slot]++;
            House_SaveStorage(hid);
            new szProx[128];
            SendClientMessageEx(playerid, COLOR_WHITE, "Ban da cat vat pham vao tu do.");
            format(szProx, sizeof(szProx), "{FF8000}* {C2A2DA}%s cat %s vao tu nha.", GetPlayerNameEx(playerid), ItemInventory[itype][NameItem]);
            ProxDetector(30.0, playerid, szProx, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
            ShowHouseStorageDialog(playerid, hid);
        }
        else
        {
            new szInfo[128];
            format(szInfo, sizeof(szInfo), "Vat pham: %s\nBan co: %d\nNhap so luong muon cat:", ItemInventory[itype][NameItem], have);
            SetPVarInt(playerid, "HsPutItemType", itype);
            SetPVarInt(playerid, "HsPutItemMax",  have);
            SetPVarInt(playerid, "HouseManageId", hid);
            ShowPlayerDialog(playerid, DIALOG_HOUSE_STORAGE_PAMT, DIALOG_STYLE_INPUT, "Cat vat pham", szInfo, "Cat", "Huy");
        }
        return 1;
    }

    // --- Nhap so luong cat vao ---
    if(dialogid == DIALOG_HOUSE_STORAGE_PAMT)
    {
        new hid   = GetPVarInt(playerid, "HouseManageId");
        if(!response) { ShowHouseStorageDialog(playerid, hid); return 1; }

        new itype  = GetPVarInt(playerid, "HsPutItemType");
        new maxAmt = GetPVarInt(playerid, "HsPutItemMax");
        new amt    = strval(inputtext);

        if(amt <= 0 || amt > maxAmt)
            return SendClientMessageEx(playerid, COLOR_GREY, "So luong khong hop le."), 1;
        if(itype < 0 || itype >= sizeof(ItemInventory)) return 1;
        if(PlayerInfo[playerid][ItemInventory[itype][pVariable]] < amt)
            return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong du vat pham."), 1;

        new slot = House_FindSlot(hid, HOUSE_STORE_ITEM, itype);
        if(slot == -1) return SendClientMessageEx(playerid, COLOR_GREY, "Tu do da day."), 1;

        PlayerInfo[playerid][ItemInventory[itype][pVariable]] -= amt;
        if(HouseStoreCat[hid][slot] == HOUSE_STORE_EMPTY)
        {
            HouseStoreCat[hid][slot]  = HOUSE_STORE_ITEM;
            HouseStoreType[hid][slot] = itype;
            HouseStoreAmt[hid][slot]  = amt;
        }
        else HouseStoreAmt[hid][slot] += amt;

        House_SaveStorage(hid);
        new szMsg[64], szProx[128];
        format(szMsg, sizeof(szMsg), "Ban da cat {00FF88}%d %s {FFFFFF}vao tu do.", amt, ItemInventory[itype][NameItem]);
        SendClientMessageEx(playerid, COLOR_WHITE, szMsg);
        format(szProx, sizeof(szProx), "{FF8000}* {C2A2DA}%s cat %s vao tu nha.", GetPlayerNameEx(playerid), ItemInventory[itype][NameItem]);
        ProxDetector(30.0, playerid, szProx, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
        ShowHouseStorageDialog(playerid, hid);
        return 1;
    }

    // --- Buoc 1: Nhap ID nguoi mua ---
    if(dialogid == DIALOG_HOUSE_SELL)
    {
        if(!response) { ShowHouseMainDialog(playerid, houseid); return 1; }

        new buyerid = strval(inputtext);
        if(!IsPlayerConnected(buyerid))
        {
            SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi co ID nay khong ton tai hoac chua dang nhap.");
            ShowHouseSellBuyerDialog(playerid, houseid);
            return 1;
        }
        if(buyerid == playerid)
        {
            SendClientMessageEx(playerid, COLOR_GREY, "Ban khong the ban nha cho chinh minh.");
            ShowHouseSellBuyerDialog(playerid, houseid);
            return 1;
        }
        ShowHouseSellPriceDialog(playerid, houseid, buyerid);
        return 1;
    }

    // --- Buoc 2: Nhap gia ban ---
    if(dialogid == DIALOG_HOUSE_SELL_PRICE)
    {
        new buyerid = GetPVarInt(playerid, "HouseSellBuyerID");
        if(!response) { ShowHouseSellBuyerDialog(playerid, houseid); return 1; }

        new price = strval(inputtext);
        if(price <= 0)
        {
            SendClientMessageEx(playerid, COLOR_GREY, "Gia ban phai lon hon 0.");
            ShowHouseSellPriceDialog(playerid, houseid, buyerid);
            return 1;
        }
        if(!IsPlayerConnected(buyerid))
            return SendClientMessageEx(playerid, COLOR_GREY, "Nguoi mua vua roi khoi server."), 1;

        HouseSellTarget[playerid]   = buyerid;
        HouseSellHouseIdx[playerid] = houseid;
        HouseSellPrice[playerid]    = price;

        new szMsg[256], szHouseName[36];
        GetHouseDisplayName(houseid, szHouseName, sizeof(szHouseName));
        format(szMsg, sizeof(szMsg), "{00FF88}%s {FFFFFF}muon ban nha '%s' {FFFFFF}cho ban voi gia {00FF88}$%s. {FFFFFF}Dung {00FF88}/chapnhannha {FFFFFF}de chap nhan.",
            StripUnderscore(GetPlayerNameEx(playerid)), szHouseName, number_format(price));
        SendClientMessageEx(buyerid, COLOR_WHITE, szMsg);
        SendClientMessageEx(playerid, COLOR_WHITE, "Loi de nghi da duoc gui. Cho nguoi mua tra loi...");
        return 1;
    }

    // --- Duoi nguoi thue ---
    if(dialogid == DIALOG_HOUSE_EVICT)
    {
        if(!response) return 1;
        new count = 0, evictId = INVALID_PLAYER_ID;
        foreach(new i : Player)
        {
            if(i == playerid) continue;
            if(GetPlayerHouseInterior(i) != houseid) continue;
            if(count == listitem) { evictId = i; break; }
            count++;
        }
        if(evictId == INVALID_PLAYER_ID) return 1;

        // Teleport nguoi bi duoi ra ngoai
        SetPlayerInterior(evictId, HouseInfo[houseid][hExtIW]);
        SetPlayerVirtualWorld(evictId, HouseInfo[houseid][hExtVW]);
        SetPlayerPos(evictId, HouseInfo[houseid][hExteriorX], HouseInfo[houseid][hExteriorY], HouseInfo[houseid][hExteriorZ]);
        PlayerInfo[evictId][pInt] = HouseInfo[houseid][hExtIW];
        PlayerInfo[evictId][pVW]  = HouseInfo[houseid][hExtVW];

        SendClientMessageEx(evictId, COLOR_GRAD2, "Ban da bi duoi ra khoi ngoi nha.");
        new szMsg[64];
        format(szMsg, sizeof(szMsg), "{00FF88}Da duoi {FFFFFF}%s {00FF88}ra khoi nha.", StripUnderscore(GetPlayerNameEx(evictId)));
        SendClientMessageEx(playerid, COLOR_WHITE, szMsg);
        return 1;
    }

    // --- Houseshop: xem vi tri ---
    if(dialogid == DIALOG_HOUSESHOP_NEW)
    {
        if(!response) return 1;
        // Tim ngoi nha tuong ung voi listitem
        new count = 0;
        for(new i = 0; i < MAX_HOUSES; i++)
        {
            if(HouseInfo[i][hExteriorX] == 0.0) continue;
            if(HouseInfo[i][hOwned]) continue;
            if(count == listitem)
            {
                SetPlayerCP(playerid, HouseInfo[i][hExteriorX], HouseInfo[i][hExteriorY], HouseInfo[i][hExteriorZ], 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                new szMsg[64], szZone[MAX_ZONE_NAME];
                Get3DZone(HouseInfo[i][hExteriorX], HouseInfo[i][hExteriorY], HouseInfo[i][hExteriorZ], szZone, MAX_ZONE_NAME);
                format(szMsg, sizeof(szMsg), "Checkpoint da duoc dat den nha #%d (%s).", i, szZone);
                SendClientMessageEx(playerid, COLOR_WHITE, szMsg);
                break;
            }
            count++;
            if(count > 50) break;
        }
        return 1;
    }

    // --- Admin: danh sach nha cua nguoi choi ---
    if(dialogid == DIALOG_ADMIN_CHECKHOUSE)
    {
        if(!response) return 1;
        if(listitem < 0 || listitem >= AdminCheckHouseCount[playerid]) return 1;
        new hid = AdminCheckHouseList[playerid][listitem];
        ShowAdminCheckHouseStorage(playerid, hid);
        return 1;
    }

    // --- Admin: tu do cua nha ---
    if(dialogid == DIALOG_ADMIN_CHECKHOUSE_STORE)
    {
        // "Quay lai" -> hien lai danh sach nha
        new targetid = GetPVarInt(playerid, "AdminCheckTarget");
        if(IsPlayerConnected(targetid))
            ShowAdminCheckHouseList(playerid, targetid);
        return 1;
    }

    return 1;
}

// ===================== ADMIN CHECK FUNCTIONS =====================

stock ShowAdminCheckHouseList(adminid, targetid)
{
    new sqlid = GetPlayerSQLId(targetid);
    new szList[2048], szRow[128], count = 0;
    new szZone[MAX_ZONE_NAME];
    format(szList, sizeof(szList), "Nha\tKhu vuc\tTrang thai\tTu do\n");

    AdminCheckHouseCount[adminid] = 0;

    for(new i = 0; i < MAX_HOUSES; i++)
    {
        if(HouseInfo[i][hOwnerID] != sqlid) continue;
        if(AdminCheckHouseCount[adminid] >= 10) break;

        new szHouseName[36];
        GetHouseDisplayName(i, szHouseName, sizeof(szHouseName));
        Get3DZone(HouseInfo[i][hExteriorX], HouseInfo[i][hExteriorY], HouseInfo[i][hExteriorZ], szZone, MAX_ZONE_NAME);

        new storeCount = 0;
        for(new s = 0; s < MAX_HOUSE_SLOTS; s++)
            if(HouseStoreCat[i][s] != HOUSE_STORE_EMPTY) storeCount++;

        format(szRow, sizeof(szRow), "%s (#%d)\t%s\t%s\t%d/%d vat pham\n",
            szHouseName, i, szZone,
            HouseInfo[i][hLock] ? "{FF4444}Khoa" : "{44FF44}Mo",
            storeCount, MAX_HOUSE_SLOTS);
        strcat(szList, szRow, sizeof(szList));

        AdminCheckHouseList[adminid][AdminCheckHouseCount[adminid]] = i;
        AdminCheckHouseCount[adminid]++;
        count++;
    }

    new szTitle[64];
    format(szTitle, sizeof(szTitle), "Nha cua %s (ID: %d)", GetPlayerNameEx(targetid), targetid);

    if(count == 0)
    {
        ShowPlayerDialog(adminid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, szTitle, "{ffffff}Nguoi choi nay khong co ngoi nha nao.", "Dong", "");
        return;
    }

    SetPVarInt(adminid, "AdminCheckTarget", targetid);
    ShowPlayerDialog(adminid, DIALOG_ADMIN_CHECKHOUSE, DIALOG_STYLE_TABLIST_HEADERS, szTitle, szList, "Xem tu do", "Dong");
}

stock ShowAdminCheckHouseStorage(adminid, houseid)
{
    new szList[1024], szRow[96], szTitle[64], szHouseName[36];
    GetHouseDisplayName(houseid, szHouseName, sizeof(szHouseName));
    format(szTitle, sizeof(szTitle), "Tu do - %s (#%d)", szHouseName, houseid);
    format(szList, sizeof(szList), "Loai\tVat pham\tSo luong\n");

    new count = 0;
    for(new s = 0; s < MAX_HOUSE_SLOTS; s++)
    {
        if(HouseStoreCat[houseid][s] == HOUSE_STORE_EMPTY) continue;
        new cat   = HouseStoreCat[houseid][s];
        new itype = HouseStoreType[houseid][s];
        new amt   = HouseStoreAmt[houseid][s];

        if(cat == HOUSE_STORE_GUN)
        {
            new szWep[32];
            GetWeaponName(itype, szWep, sizeof(szWep));
            format(szRow, sizeof(szRow), "{FF8000}Sung\t{FFFFFF}%s\t%d dan\n", szWep, amt);
        }
        else if(cat == HOUSE_STORE_FUELCAN)
        {
            if(itype < 0 || itype >= sizeof(ItemInventory)) continue;
            new Float:fLiters = Float:amt - FUELCAN_EMPTY_MARKER;
            format(szRow, sizeof(szRow), "{00FF88}Vat pham\t{FFFFFF}%s\t%.1f L\n", ItemInventory[itype][NameItem], fLiters);
        }
        else
        {
            if(itype < 0 || itype >= sizeof(ItemInventory)) continue;
            format(szRow, sizeof(szRow), "{00FF88}Vat pham\t{FFFFFF}%s\t%d\n", ItemInventory[itype][NameItem], amt);
        }

        strcat(szList, szRow, sizeof(szList));
        count++;
    }

    SetPVarInt(adminid, "AdminCheckHouseId", houseid);

    if(count == 0)
    {
        ShowPlayerDialog(adminid, DIALOG_ADMIN_CHECKHOUSE_STORE, DIALOG_STYLE_MSGBOX, szTitle, "{ffffff}Tu do trong khong.", "Quay lai", "");
        return;
    }

    ShowPlayerDialog(adminid, DIALOG_ADMIN_CHECKHOUSE_STORE, DIALOG_STYLE_TABLIST_HEADERS, szTitle, szList, "Quay lai", "");
}

// ===================== COMMANDS =====================

CMD:house(playerid, params[])
{
    if(GetPVarInt(playerid, "IsInArena") >= 0)
        return SendClientMessageEx(playerid, COLOR_WHITE, "Ban khong the lam dieu nay tai day.");
    if(GetPVarInt(playerid, "Injured") != 0)
        return SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong the lam dieu nay vao luc nay.");

    new h = GetPlayerHouseInterior(playerid);
    if(h == INVALID_HOUSE_ID)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban phai o ben trong nha de su dung lenh nay.");
    if(!IsHouseOwnerSQL(playerid, h))
        return SendClientMessageEx(playerid, COLOR_GREY, "Day khong phai la nha cua ban.");

    ShowHouseMainDialog(playerid, h);
    return 1;
}

CMD:muanha(playerid, params[])
{
    if(GetPVarInt(playerid, "IsInArena") >= 0)
        return SendClientMessageEx(playerid, COLOR_WHITE, "Ban khong the lam dieu nay tai day.");
    if(IsPlayerInAnyVehicle(playerid))
        return SendClientMessageEx(playerid, COLOR_WHITE, "Ban khong the mua nha khi dang tren xe.");
    if(GetPVarInt(playerid, "Injured") != 0)
        return SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong the lam dieu nay vao luc nay.");

    new h = FindClosestHouseExterior(playerid, 2.0);
    if(h == INVALID_HOUSE_ID)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban phai dung gan ngoi nha muon mua.");
    if(HouseInfo[h][hOwned])
        return SendClientMessageEx(playerid, COLOR_GREY, "Ngoi nha nay da co chu.");
    if(PlayerInfo[playerid][pLevel] < HouseInfo[h][hLevel])
    {
        new szMsg[64];
        format(szMsg, sizeof(szMsg), "Ban can it nhat Level {FFFFFF}%d {FF0000}de mua ngoi nha nay.", HouseInfo[h][hLevel]);
        return SendClientMessageEx(playerid, COLOR_GRAD2, szMsg);
    }
    if(CountPlayerHousesSQL(playerid) >= MAX_OWNABLE_HOUSES)
    {
        new szMsg[64];
        format(szMsg, sizeof(szMsg), "Ban chi co the so huu toi da %d ngoi nha.", MAX_OWNABLE_HOUSES);
        return SendClientMessageEx(playerid, COLOR_GREY, szMsg);
    }
    if(PlayerInfo[playerid][pFreezeHouse])
        return SendClientMessageEx(playerid, COLOR_GREY, "Tai san nha cua ban dang bi dong bang.");
    if(GetPlayerCash(playerid) < HouseInfo[h][hValue])
    {
        new szMsg[64];
        format(szMsg, sizeof(szMsg), "Ban can {FFFFFF}$%s {FF0000}de mua ngoi nha nay.", number_format(HouseInfo[h][hValue]));
        return SendClientMessageEx(playerid, COLOR_GRAD2, szMsg);
    }

    // Thuc hien mua
    GivePlayerCash(playerid, -HouseInfo[h][hValue]);
    HouseInfo[h][hOwned]   = 1;
    HouseInfo[h][hOwnerID] = GetPlayerSQLId(playerid);
    format(HouseInfo[h][hOwnerName], MAX_PLAYER_NAME, "%s", GetPlayerNameEx(playerid));
    HouseInfo[h][hLock]    = 1; // mac dinh khoa khi mua

    if(PlayerInfo[playerid][pPhousekey] == INVALID_HOUSE_ID)
        PlayerInfo[playerid][pPhousekey] = h;
    else
        PlayerInfo[playerid][pPhousekey2] = h;

    SaveHouse(h);
    House_ReloadPickupLabel(h);

    // Teleport vao trong (neu co noi that)
    if(HouseInfo[h][hInteriorX] != 0.0)
    {
        SetPlayerInterior(playerid, HouseInfo[h][hIntIW]);
        SetPlayerVirtualWorld(playerid, HouseInfo[h][hIntVW]);
        SetPlayerPos(playerid, HouseInfo[h][hInteriorX], HouseInfo[h][hInteriorY], HouseInfo[h][hInteriorZ]);
        SetPlayerFacingAngle(playerid, HouseInfo[h][hInteriorA]);
        SetCameraBehindPlayer(playerid);
        PlayerInfo[playerid][pInt] = HouseInfo[h][hIntIW];
        PlayerInfo[playerid][pVW]  = HouseInfo[h][hIntVW];
        if(HouseInfo[h][hCustomInterior] == 1)
            Player_StreamPrep(playerid, HouseInfo[h][hInteriorX], HouseInfo[h][hInteriorY], HouseInfo[h][hInteriorZ], FREEZE_TIME);
    }

    SendHintMessage(playerid, "Chuc mung! Su dung {FFFF00}/house {FFFFFF}de quan ly ngoi nha moi cua ban.");
    new szMsg[128], szProx[128];
    format(szMsg, sizeof(szMsg), "Ban da mua nha #%d voi gia {FFFFFF}$%s{00FF88}.", h, number_format(HouseInfo[h][hValue]));
    SendClientMessageEx(playerid, COLOR_GREEN, szMsg);
    format(szProx, sizeof(szProx), "{FF8000}* {C2A2DA}%s mua ngoi nha nay.", GetPlayerNameEx(playerid));
    ProxDetector(30.0, playerid, szProx, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
    return 1;
}

CMD:houseshop(playerid, params[])
{
    ShowHouseShopDialog(playerid);
    return 1;
}

CMD:myhouse(playerid, params[])
{
    ShowMyHouseDialog(playerid);
    return 1;
}

CMD:chapnhannha(playerid, params[])
{
    new sellerid = -1;
    foreach(new i : Player)
    {
        if(HouseSellTarget[i] == playerid)
        {
            sellerid = i;
            break;
        }
    }
    if(sellerid == -1)
        return SendClientMessageEx(playerid, COLOR_GREY, "Khong co loi de nghi mua nha nao danh cho ban.");

    new h     = HouseSellHouseIdx[sellerid];
    new price = HouseSellPrice[sellerid];

    if(h < 0 || h >= MAX_HOUSES || !HouseInfo[h][hOwned])
    {
        HouseSellTarget[sellerid] = INVALID_PLAYER_ID;
        return SendClientMessageEx(playerid, COLOR_GREY, "Loi: Ngoi nha khong con kha dung."), 1;
    }
    if(!IsHouseOwnerSQL(sellerid, h))
    {
        HouseSellTarget[sellerid] = INVALID_PLAYER_ID;
        return SendClientMessageEx(playerid, COLOR_GREY, "Nguoi ban khong con la chu nha."), 1;
    }
    if(GetPlayerCash(playerid) < price)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong du tien de mua ngoi nha nay."), 1;
    if(CountPlayerHousesSQL(playerid) >= MAX_OWNABLE_HOUSES)
    {
        new szMsg[64];
        format(szMsg, sizeof(szMsg), "Ban da co toi da %d ngoi nha.", MAX_OWNABLE_HOUSES);
        return SendClientMessageEx(playerid, COLOR_GREY, szMsg);
    }
    if(PlayerInfo[playerid][pFreezeHouse])
        return SendClientMessageEx(playerid, COLOR_GREY, "Tai san nha cua ban dang bi dong bang.");
    if(PlayerInfo[playerid][pLevel] < HouseInfo[h][hLevel])
        return SendClientMessageEx(playerid, COLOR_GREY, "Level cua ban khong du de so huu ngoi nha nay.");

    // Thuc hien chuyen nhuong
    GivePlayerCash(playerid, -price);
    GivePlayerCash(sellerid, price);

    // Xoa key cu
    if(PlayerInfo[sellerid][pPhousekey] == h)
        PlayerInfo[sellerid][pPhousekey] = INVALID_HOUSE_ID;
    if(PlayerInfo[sellerid][pPhousekey2] == h)
        PlayerInfo[sellerid][pPhousekey2] = INVALID_HOUSE_ID;

    // Cap nhat thong tin nha
    HouseInfo[h][hOwnerID] = GetPlayerSQLId(playerid);
    format(HouseInfo[h][hOwnerName], MAX_PLAYER_NAME, "%s", GetPlayerNameEx(playerid));
    HouseInfo[h][hName][0] = '\0'; // reset ten nha
    HouseInfo[h][hLock] = 1;

    // Them key moi
    if(PlayerInfo[playerid][pPhousekey] == INVALID_HOUSE_ID)
        PlayerInfo[playerid][pPhousekey] = h;
    else
        PlayerInfo[playerid][pPhousekey2] = h;

    // Kick anyone inside
    foreach(new i : Player)
    {
        if(GetPlayerHouseInterior(i) == h)
        {
            SendClientMessageEx(i, COLOR_GRAD2, "Nha ban dang o da doi chu, ban bi duoi ra ngoai.");
            SetPlayerPos(i, HouseInfo[h][hExteriorX], HouseInfo[h][hExteriorY], HouseInfo[h][hExteriorZ]);
            SetPlayerInterior(i, HouseInfo[h][hExtIW]);
            SetPlayerVirtualWorld(i, HouseInfo[h][hExtVW]);
            PlayerInfo[i][pInt] = HouseInfo[h][hExtIW];
            PlayerInfo[i][pVW]  = HouseInfo[h][hExtVW];
        }
    }

    SaveHouse(h);
    House_ReloadPickupLabel(h);

    HouseSellTarget[sellerid] = INVALID_PLAYER_ID;

    new szMsg[128], szHouseName[36], szProx[128];
    GetHouseDisplayName(h, szHouseName, sizeof(szHouseName));
    format(szMsg, sizeof(szMsg), "Ban da ban nha '%s' cho {FFFFFF}%s {00FF88}voi gia $%s.", szHouseName, StripUnderscore(GetPlayerNameEx(playerid)), number_format(price));
    SendClientMessageEx(sellerid, COLOR_GREEN, szMsg);
    format(szMsg, sizeof(szMsg), "Ban da mua nha '%s' tu {FFFFFF}%s {00FF88}voi gia $%s.", szHouseName, StripUnderscore(GetPlayerNameEx(sellerid)), number_format(price));
    SendClientMessageEx(playerid, COLOR_GREEN, szMsg);
    format(szProx, sizeof(szProx), "{FF8000}* {C2A2DA}%s mua nha tu %s.", StripUnderscore(GetPlayerNameEx(playerid)), StripUnderscore(GetPlayerNameEx(sellerid)));
    ProxDetector(30.0, playerid, szProx, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
    return 1;
}

alias:househelp("trogiupnha")
CMD:househelp(playerid, params[])
{
    SendClientMessageEx(playerid, COLOR_WHITE, "=== TRO GIUP NHA ===");
    SendClientMessageEx(playerid, COLOR_GRAD1, "/house - Quan ly nha (doi ten, tu do, khoa, duoi nguoi, ban)");
    SendClientMessageEx(playerid, COLOR_GRAD1, "/muanha - Mua nha dang ban gan ban");
    SendClientMessageEx(playerid, COLOR_GRAD1, "/houseshop - Xem danh sach tat ca nha dang ban");
    SendClientMessageEx(playerid, COLOR_GRAD1, "/myhouse - Danh sach nha cua ban");
    SendClientMessageEx(playerid, COLOR_GRAD1, "/chapnhannha - Chap nhan mua nha tu nguoi ban");
    SendClientMessageEx(playerid, COLOR_GRAD2, "ALT (ngoai nha): Nhanh khoa/mo cua nha cua ban.");
    SendClientMessageEx(playerid, COLOR_GRAD2, "Phim Y: Vao/ra nha khi dung o gan pickup.");
    return 1;
}

CMD:checkhouse(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1) return SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong co quyen su dung lenh nay.");

    new targetid;
    if(sscanf(params, "u", targetid)) return SendClientMessageEx(playerid, COLOR_WHITE, "SYNTAX: /checkhouse [playerid/name]");
    if(!IsPlayerConnected(targetid)) return SendClientMessageEx(playerid, COLOR_GRAD2, "Nguoi choi khong ton tai.");

    ShowAdminCheckHouseList(playerid, targetid);

    new logstr[128];
    format(logstr, sizeof(logstr), "%s (ID:%d) da xem house cua %s (ID:%d)", GetPlayerNameEx(playerid), playerid, GetPlayerNameEx(targetid), targetid);
    Log("logs/admin.log", logstr);
    return 1;
}
