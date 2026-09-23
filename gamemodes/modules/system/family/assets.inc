// ===========================================================================
// gamemodes/modules/system/family/assets.inc
// Hệ thống két sắt, bank, nâng cấp gang
//
// TÍCH HỢP (OnGameModeInit trong main.pwn):
//   FamilyAssets_InitMySQL();
//   FamilyAssets_Load();
//
// Thêm vào OnDialogResponse:
//   if(FA_OnDialogResponse(playerid, dialogid, response, listitem, inputtext)) return 1;
//
// Thêm vào OnPlayerPickUpPickup (hoặc yom_buttons / streamer OnPlayerEnterDynamicArea):
//   FA_OnPlayerPickUpPickup(playerid, pickupid);
//
// QUAN TRỌNG: Hàm FA_AddFame(familyid, amount) được dùng bởi cả turfwars.inc
// và management.inc nên khai báo ở đây.
// ===========================================================================

// ---------------------------------------------------------------------------
// HẰNG SỐ CẤP ĐỘ KÉT SẮT
// ---------------------------------------------------------------------------
#define MAX_SAFE_LEVEL          8
#define SAFE_LEVEL_CASH_BASE    500000
#define SAFE_LEVEL_GOODS_BASE   10
#define SAFE_LEVEL_MATS_FACTOR  1000    // level * 1000 * 10
#define SAFE_LEVEL_GUN_BASE     2       // 3*level + 2
#define SAFE_LEVEL_MEM_FACTOR   5       // 5*level
#define SAFE_MOVE_COOLDOWN      259200  // 3 ngày (giây)

// Timestamp lần cuối đổi vị trí két của mỗi gang
new g_safeCooldown[MAX_FAMILY];

// Vị trí ngân hàng (chỉ nạp/rút bank tại đây)
#define BANK_POS_X              2308.73
#define BANK_POS_Y              (-11.01)
#define BANK_POS_Z              26.74
#define BANK_RADIUS             5.0

// Dialog IDs (tiếp theo turfwars.inc, bắt đầu từ 5320)
#define DIALOG_SAFE_MAIN        5320
#define DIALOG_SAFE_VIEW        5321
#define DIALOG_SAFE_DEPOSIT     5322
#define DIALOG_SAFE_DEPOSIT_AMT 5323
#define DIALOG_SAFE_WITHDRAW    5324
#define DIALOG_SAFE_WITHDRAW_AMT 5325
#define DIALOG_SAFE_LOGS        5326
#define DIALOG_SAFE_GUNS        5327
#define DIALOG_SAFE_GUN_STORE   5328
#define DIALOG_SAFE_GUN_GET     5329
#define DIALOG_BANK_MAIN        5330
#define DIALOG_BANK_DEPOSIT     5331
#define DIALOG_BANK_WITHDRAW    5332
#define DIALOG_SAFE_UPGRADE     5333
#define DIALOG_SAFE_ITEM_LIST   5334
#define DIALOG_SAFE_ITEM_ACT    5335

// ---------------------------------------------------------------------------
// HÀM TÍNH SỨC CHỨA THEO LEVEL
// ---------------------------------------------------------------------------

stock FA_GetMaxCash(level)
{
    if(level <= 1) return SAFE_LEVEL_CASH_BASE;
    return SAFE_LEVEL_CASH_BASE * (level + 1);
}

stock FA_GetMaxGoods(level)
{
    if(level <= 1) return SAFE_LEVEL_GOODS_BASE;
    return SAFE_LEVEL_GOODS_BASE * (level + 1);
}

stock FA_GetMaxMats(level)
{
    return level * SAFE_LEVEL_MATS_FACTOR * 10;
}

stock FA_GetMaxGunSlots(level)
{
    return 3 * level + 2;
}

stock FA_GetMaxMembers(level)
{
    return SAFE_LEVEL_MEM_FACTOR * level;
}

// Chi phí nâng cấp sang targetLevel (trừ tiền mặt từ két + VL)
stock FA_GetUpgradeCostCash(targetLevel)
{
    return FA_GetMaxCash(targetLevel) / 2;
}

stock FA_GetUpgradeCostMats(targetLevel)
{
    return FA_GetMaxMats(targetLevel) / 2;
}

// ---------------------------------------------------------------------------
// THÊM/TRỪ FAME (được gọi từ turfwars.inc và management.inc)
// ---------------------------------------------------------------------------

stock FA_AddFame(familyid, amount)
{
    if(familyid < 0 || familyid >= MAX_FAMILY) return;
    FamilyInfo[familyid][FamilyFame] += amount;
    if(FamilyInfo[familyid][FamilyFame] < 0) FamilyInfo[familyid][FamilyFame] = 0;
    SaveFamilyExt(familyid);
}

// ---------------------------------------------------------------------------
// MYSQL – Bảng mở rộng cho các field mới
// ---------------------------------------------------------------------------

stock FamilyAssets_InitMySQL()
{
    // Bảng gun slots mới (26 slot, kèm ammo)
    mysql_tquery(MainPipeline,
        "CREATE TABLE IF NOT EXISTS `family_guns_v2` (\
        `family_id` INT NOT NULL,\
        `slot` INT NOT NULL,\
        `weapon_id` INT DEFAULT 0,\
        `ammo` INT DEFAULT 0,\
        PRIMARY KEY(`family_id`,`slot`)\
        )",
        "", "");

    // Bảng lịch sử két
    mysql_tquery(MainPipeline,
        "CREATE TABLE IF NOT EXISTS `family_safe_logs` (\
        `id` INT AUTO_INCREMENT PRIMARY KEY,\
        `family_id` INT NOT NULL,\
        `player_name` VARCHAR(24) NOT NULL,\
        `action_type` ENUM('deposit','withdraw') NOT NULL,\
        `item_type` VARCHAR(20) NOT NULL,\
        `amount` INT NOT NULL,\
        `gun_id` INT DEFAULT 0,\
        `timestamp` INT NOT NULL,\
        INDEX(`family_id`)\
        )",
        "", "");
}

// Load toàn bộ family ext data (level, fame, cansa, tepcansa, guns)
stock FamilyAssets_Load()
{
    mysql_tquery(MainPipeline,
        "SELECT `id`,`level`,`fame`,`cansa`,`tepcansa`,`safe_cooldown` FROM `families`",
        "FA_OnLoadFamilies", "");
    mysql_tquery(MainPipeline,
        "SELECT `family_id`,`slot`,`weapon_id`,`ammo` FROM `family_guns_v2`",
        "FA_OnLoadGuns", "");
}

forward FA_OnLoadFamilies();
public FA_OnLoadFamilies()
{
    new rows = cache_num_rows();
    for(new i = 0; i < rows; i++)
    {
        new famid, lv, fame, cansa, tepcansa, cooldown;
        cache_get_value_name_int(i, "id",            famid);
        cache_get_value_name_int(i, "level",         lv);
        cache_get_value_name_int(i, "fame",          fame);
        cache_get_value_name_int(i, "cansa",         cansa);
        cache_get_value_name_int(i, "tepcansa",      tepcansa);
        cache_get_value_name_int(i, "safe_cooldown", cooldown);

        if(famid < 0 || famid >= MAX_FAMILY) continue;
        FamilyInfo[famid][FamilyLevel]    = lv    < 1 ? 1 : lv;
        FamilyInfo[famid][FamilyFame]     = fame;
        FamilyInfo[famid][FamilyCansa]    = cansa;
        FamilyInfo[famid][FamilyTepCansa] = tepcansa;
        g_safeCooldown[famid]             = cooldown;
    }
    // Tạo pickup + 3D text label cho két sắt của các gang đã có vị trí
    for(new fid = 0; fid < MAX_FAMILY; fid++)
    {
        if(!FamilyInfo[fid][FamilyTaken]) continue;
        new Float:sx = FamilyInfo[fid][FamilySafe][0];
        new Float:sy = FamilyInfo[fid][FamilySafe][1];
        new Float:sz = FamilyInfo[fid][FamilySafe][2];
        if(sx == 0.0 && sy == 0.0) continue;
        // Tạo pickup nếu chưa có (FamilyPickup == 0 nghĩa là chưa tạo)
        if(FamilyInfo[fid][FamilyPickup] == 0)
            FamilyInfo[fid][FamilyPickup] = CreateDynamicPickup(1239, 23, sx, sy, sz, -1);
        FA_UpdateSafeLabel(fid);
    }
}

forward FA_OnLoadGuns();
public FA_OnLoadGuns()
{
    new rows = cache_num_rows();
    for(new i = 0; i < rows; i++)
    {
        new famid, slot, wid, ammo;
        cache_get_value_name_int(i, "family_id", famid);
        cache_get_value_name_int(i, "slot",      slot);
        cache_get_value_name_int(i, "weapon_id", wid);
        cache_get_value_name_int(i, "ammo",      ammo);

        if(famid < 0 || famid >= MAX_FAMILY) continue;
        new maxSlots = FA_GetMaxGunSlots(FamilyInfo[famid][FamilyLevel]);
        if(slot < 0 || slot >= maxSlots || slot >= 26) continue;
        FamilyInfo[famid][FamilyNewGuns][slot]    = wid;
        FamilyInfo[famid][FamilyNewGunAmmo][slot] = ammo;
    }
}

// Lưu các field mở rộng của family (không đụng SaveFamily cũ)
stock SaveFamilyExt(familyid)
{
    if(familyid < 0 || familyid >= MAX_FAMILY) return;
    new q[192];
    mysql_format(MainPipeline, q, sizeof(q),
        "UPDATE `families` SET `level`=%d,`fame`=%d,`cansa`=%d,`tepcansa`=%d,`safe_cooldown`=%d WHERE `id`=%d",
        FamilyInfo[familyid][FamilyLevel],
        FamilyInfo[familyid][FamilyFame],
        FamilyInfo[familyid][FamilyCansa],
        FamilyInfo[familyid][FamilyTepCansa],
        g_safeCooldown[familyid],
        familyid
    );
    mysql_tquery(MainPipeline, q, "", "");
}

// Thông báo cho tất cả thành viên gang đang online (trừ người thực hiện)
static stock FA_NotifyGang(famid, playerid, const msg[])
{
    new pname[MAX_PLAYER_NAME];
    GetPlayerName(playerid, pname, sizeof(pname));
    new str[192];
    format(str, sizeof(str), "[KET SAC] %s: %s", pname, msg);
    foreach(new i : Player)
    {
        if(PlayerInfo[i][pFMember] == famid && i != playerid)
            SendClientMessageEx(i, COLOR_YELLOW, str);
    }
}

// Lưu 1 slot súng
stock FA_SaveGunSlot(familyid, slot)
{
    if(familyid < 0 || familyid >= MAX_FAMILY) return;
    if(slot < 0 || slot >= 26) return;
    new q[160];
    mysql_format(MainPipeline, q, sizeof(q),
        "INSERT INTO `family_guns_v2`(`family_id`,`slot`,`weapon_id`,`ammo`) VALUES(%d,%d,%d,%d)\
         ON DUPLICATE KEY UPDATE `weapon_id`=%d,`ammo`=%d",
        familyid, slot,
        FamilyInfo[familyid][FamilyNewGuns][slot],
        FamilyInfo[familyid][FamilyNewGunAmmo][slot],
        FamilyInfo[familyid][FamilyNewGuns][slot],
        FamilyInfo[familyid][FamilyNewGunAmmo][slot]
    );
    mysql_tquery(MainPipeline, q, "", "");
}

// Ghi log giao dịch két
stock FA_LogTransaction(familyid, playerid, const actionType[], const itemType[], amount, gunId = 0)
{
    new pname[MAX_PLAYER_NAME];
    GetPlayerName(playerid, pname, sizeof(pname));
    new q[320];
    mysql_format(MainPipeline, q, sizeof(q),
        "INSERT INTO `family_safe_logs`(`family_id`,`player_name`,`action_type`,`item_type`,`amount`,`gun_id`,`timestamp`)\
         VALUES(%d,'%s','%s','%s',%d,%d,%d)",
        familyid, pname, actionType, itemType, amount, gunId, gettime());
    mysql_tquery(MainPipeline, q, "", "");
}

// ---------------------------------------------------------------------------
// MỞ DIALOG KÉT SẮT (Gọi khi player tương tác pickup két)
// ---------------------------------------------------------------------------

stock FA_OpenSafeDialog(playerid)
{
    new fam = PlayerInfo[playerid][pFMember];
    if(fam == INVALID_FAMILY_ID)
        return SendClientMessageEx(playerid, COLOR_RED, "Ban chua vao gang nao."), 0;

    new rank = PlayerInfo[playerid][pRank];
    new lv = FamilyInfo[fam][FamilyLevel];
    new maxGun = FA_GetMaxGunSlots(lv);
    new usedGun = 0;
    for(new s = 0; s < maxGun; s++)
        if(FamilyInfo[fam][FamilyNewGuns][s] > 0) usedGun++;

    new menu[512];
    new h1[64], h2[96], h3[40];
    format(h1, sizeof(h1), "{FFD700}===== KET SAC: %s =====", FamilyInfo[fam][FamilyName]);
    format(h2, sizeof(h2), "{AAAAAA}   Level: {FFFF00}%d{AAAAAA}/%d    Fame: {FF8800}%d", lv, MAX_SAFE_LEVEL, FamilyInfo[fam][FamilyFame]);
    format(h3, sizeof(h3), "{FFD700}==============================");

    new actions[256];
    format(actions, sizeof(actions),
        "{00CFFF}[>] {FFFFFF}Xem thong tin ket sat\n\
{00CFFF}[>] {FFFFFF}Vat pham\n\
{00CFFF}[>] {FFFFFF}Vu khi  {AAAAAA}[%d/%d slot]",
        usedGun, maxGun);
    if(rank >= 5) strcat(actions, "\n{00CFFF}[>] {FFFFFF}Lich su ket sat {AAAAAA}(rank 5+)", sizeof(actions));

    format(menu, sizeof(menu), "%s\n%s\n%s\n%s", h1, h2, h3, actions);

    ShowPlayerDialog(playerid, DIALOG_SAFE_MAIN, DIALOG_STYLE_LIST, "Ket Sac Gang", menu, "Chon", "Dong");
    return 1;
}

// ---------------------------------------------------------------------------
// Per-player state cho các dialog két sắt
// ---------------------------------------------------------------------------

new g_faDepositItem[MAX_PLAYERS];
new g_faWithdrawItem[MAX_PLAYERS];
new g_faSafeAction[MAX_PLAYERS];
new g_faSafeGunSlot[MAX_PLAYERS];
new g_faGunWeps[MAX_PLAYERS][13];
static g_safeLabel[MAX_FAMILY] = {-1, ...};

// ---------------------------------------------------------------------------
// DIALOG – NỘI DUNG KÉT
// ---------------------------------------------------------------------------

static stock FA_ShowSafeView(playerid, famid)
{
    new lv = FamilyInfo[famid][FamilyLevel];
    new maxGun = FA_GetMaxGunSlots(lv);
    new usedGun = 0;
    for(new s = 0; s < maxGun; s++)
        if(FamilyInfo[famid][FamilyNewGuns][s] > 0) usedGun++;

    new str[1024];
    format(str, sizeof(str),
        "{FFD700}===== KET SAC: %s =====\n\n\
{AAAAAA}   Level ket:    {FFFF00}%d{AAAAAA}/%d\n\
{AAAAAA}   Fame:         {FF8800}%d\n\n\
{FFD700}--- Vat pham ---\n\
{AAAAAA}   Tien mat:     {FFFFFF}$%d {AAAAAA}/ $%d\n\
{AAAAAA}   Can sa:       {FFFFFF}%d {AAAAAA}/ %d\n\
{AAAAAA}   Crack:        {FFFFFF}%d {AAAAAA}/ %d\n\
{AAAAAA}   Can sa moi:   {FFFFFF}%d {AAAAAA}/ %d\n\
{AAAAAA}   Tep can sa:   {FFFFFF}%d {AAAAAA}/ %d\n\
{AAAAAA}   Vat lieu:     {FFFFFF}%d {AAAAAA}/ %d\n\n\
{FFD700}--- Vu khi ---\n\
{AAAAAA}   Slot su dung: {FFFFFF}%d {AAAAAA}/ %d",
        FamilyInfo[famid][FamilyName],
        lv, MAX_SAFE_LEVEL,
        FamilyInfo[famid][FamilyFame],
        FamilyInfo[famid][FamilyCash],      FA_GetMaxCash(lv),
        FamilyInfo[famid][FamilyPot],       FA_GetMaxGoods(lv),
        FamilyInfo[famid][FamilyCrack],     FA_GetMaxGoods(lv),
        FamilyInfo[famid][FamilyCansa],     FA_GetMaxGoods(lv),
        FamilyInfo[famid][FamilyTepCansa],  FA_GetMaxGoods(lv),
        FamilyInfo[famid][FamilyMats],      FA_GetMaxMats(lv),
        usedGun, maxGun);

    ShowPlayerDialog(playerid, DIALOG_SAFE_VIEW, DIALOG_STYLE_MSGBOX, "Thong Tin Ket Sac", str, "OK", "Quay Lai");
}

// Danh sách vật phẩm trong két với số lượng hiện tại
static stock FA_ShowItemList(playerid, famid)
{
    new lv = FamilyInfo[famid][FamilyLevel];
    new list[1024]; list[0] = 0;

    new line[128];
    format(line, sizeof(line), "{FFFF00}[>] {FFFFFF}Tien mat    {AAAAAA}$%d / $%d\n",
        FamilyInfo[famid][FamilyCash], FA_GetMaxCash(lv));
    strcat(list, line, sizeof(list));
    format(line, sizeof(line), "{44FF44}[>] {FFFFFF}Can sa      {AAAAAA}%d / %d\n",
        FamilyInfo[famid][FamilyPot], FA_GetMaxGoods(lv));
    strcat(list, line, sizeof(list));
    format(line, sizeof(line), "{FF8888}[>] {FFFFFF}Crack       {AAAAAA}%d / %d\n",
        FamilyInfo[famid][FamilyCrack], FA_GetMaxGoods(lv));
    strcat(list, line, sizeof(list));
    format(line, sizeof(line), "{88FF88}[>] {FFFFFF}Can sa moi  {AAAAAA}%d / %d\n",
        FamilyInfo[famid][FamilyCansa], FA_GetMaxGoods(lv));
    strcat(list, line, sizeof(list));
    format(line, sizeof(line), "{AAFFAA}[>] {FFFFFF}Tep can sa  {AAAAAA}%d / %d\n",
        FamilyInfo[famid][FamilyTepCansa], FA_GetMaxGoods(lv));
    strcat(list, line, sizeof(list));
    format(line, sizeof(line), "{00BFFF}[>] {FFFFFF}Vat lieu    {AAAAAA}%d / %d\n",
        FamilyInfo[famid][FamilyMats], FA_GetMaxMats(lv));
    strcat(list, line, sizeof(list));

    ShowPlayerDialog(playerid, DIALOG_SAFE_ITEM_LIST, DIALOG_STYLE_LIST,
        "Vat Pham Ket Sac", list, "Chon", "Quay Lai");
}

// Dialog chọn hành động (gửi / rút) với thông tin số lượng hiển thị
static stock FA_ShowItemAction(playerid, itemType)
{
    g_faSafeAction[playerid] = itemType;
    new fam  = PlayerInfo[playerid][pFMember];
    new rank = PlayerInfo[playerid][pRank];
    new lv   = FamilyInfo[fam][FamilyLevel];

    new itemLabel[6][] = {"Tien mat", "Can sa", "Crack", "Can sa moi", "Tep can sa", "Vat lieu"};

    new playerAmt;
    switch(itemType)
    {
        case 0: playerAmt = GetPlayerCash(playerid);
        case 1: playerAmt = PlayerInfo[playerid][pPot];
        case 2: playerAmt = PlayerInfo[playerid][pCrack];
        case 3: playerAmt = PlayerInfo[playerid][pCanSa];
        case 4: playerAmt = PlayerInfo[playerid][pTepCanSa];
        case 5: playerAmt = PlayerInfo[playerid][pMats];
    }

    new safeAmt, safeMax;
    switch(itemType)
    {
        case 0: { safeAmt = FamilyInfo[fam][FamilyCash]; safeMax = FA_GetMaxCash(lv); }
        case 1: { safeAmt = FamilyInfo[fam][FamilyPot];  safeMax = FA_GetMaxGoods(lv); }
        case 2: { safeAmt = FamilyInfo[fam][FamilyCrack]; safeMax = FA_GetMaxGoods(lv); }
        case 3: { safeAmt = FamilyInfo[fam][FamilyCansa]; safeMax = FA_GetMaxGoods(lv); }
        case 4: { safeAmt = FamilyInfo[fam][FamilyTepCansa]; safeMax = FA_GetMaxGoods(lv); }
        case 5: { safeAmt = FamilyInfo[fam][FamilyMats]; safeMax = FA_GetMaxMats(lv); }
    }

    new list[512]; list[0] = 0;
    new h1[64], h2[80], h3[80], h4[40];
    format(h1, sizeof(h1), "{FFD700}===== %s =====", itemLabel[itemType]);
    format(h2, sizeof(h2), "{AAAAAA}   Tren nguoi:  {FFFFFF}%d", playerAmt);
    format(h3, sizeof(h3), "{AAAAAA}   Trong ket:   {FFFFFF}%d {AAAAAA}/ %d", safeAmt, safeMax);
    format(h4, sizeof(h4), "{FFD700}==============================");
    new withdraw_line[80];
    if(rank >= 4)
        format(withdraw_line, sizeof(withdraw_line), "{FF6B6B}[-] {FFFFFF}Rut ra khoi ket");
    else
        format(withdraw_line, sizeof(withdraw_line), "{555555}[-] {AAAAAA}Rut ra khoi ket {FF4444}(can rank 4)");
    format(list, sizeof(list), "%s\n%s\n%s\n%s\n{00FF7F}[+] {FFFFFF}Gui vao ket\n%s",
        h1, h2, h3, h4, withdraw_line);

    ShowPlayerDialog(playerid, DIALOG_SAFE_ITEM_ACT, DIALOG_STYLE_LIST,
        "Gui / Rut Vat Pham", list, "Chon", "Quay Lai");
}

// ---------------------------------------------------------------------------
// DIALOG – GỬI / RÚT (INPUT với thông tin số lượng)
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// 3D TEXT LABEL KÉT SẮT
// ---------------------------------------------------------------------------

// Hủy pickup + label két sắt và reset tọa độ — gọi khi xóa gang
stock FA_ResetSafe(famid)
{
    if(famid < 0 || famid >= MAX_FAMILY) return;
    if(FamilyInfo[famid][FamilyPickup] != 0)
    {
        DestroyDynamicPickup(FamilyInfo[famid][FamilyPickup]);
        FamilyInfo[famid][FamilyPickup] = 0;
    }
    if(g_safeLabel[famid] != -1)
    {
        DestroyDynamic3DTextLabel(Text3D:g_safeLabel[famid]);
        g_safeLabel[famid] = -1;
    }
    FamilyInfo[famid][FamilySafe][0] = 0.0;
    FamilyInfo[famid][FamilySafe][1] = 0.0;
    FamilyInfo[famid][FamilySafe][2] = 0.0;
    g_safeCooldown[famid] = 0;
}

stock FA_UpdateSafeLabel(famid)
{
    if(famid < 0 || famid >= MAX_FAMILY) return;
    if(!FamilyInfo[famid][FamilyTaken]) return;

    if(g_safeLabel[famid] != -1)
    {
        DestroyDynamic3DTextLabel(Text3D:g_safeLabel[famid]);
        g_safeLabel[famid] = -1;
    }

    new Float:sx = FamilyInfo[famid][FamilySafe][0];
    new Float:sy = FamilyInfo[famid][FamilySafe][1];
    new Float:sz = FamilyInfo[famid][FamilySafe][2];
    if(sx == 0.0 && sy == 0.0) return; // Chưa đặt vị trí

    new lv = FamilyInfo[famid][FamilyLevel];
    new labelText[384];
    format(labelText, sizeof(labelText),
        "{FFAA00}[ KET SAC GANG ]\n\
{FFFFFF}Gang:  {FFAA00}%s{AAAAAA}(#%d)\n\
{FFFFFF}Level: {44FF44}%d{FFFFFF}/%d  |  Fame: {FFFF00}%d\n\
{FFFFFF}Tien:  {FFFFFF}$%d / $%d\n\
{FFFFFF}Hang:  {FFFFFF}%d / %d\n\
{FFFFFF}Dung\"{43F8A7} [/glocker]{FFFFFF}\" {ffffff}de mo ket",
        FamilyInfo[famid][FamilyName], famid + 1,
        lv, MAX_SAFE_LEVEL,
        FamilyInfo[famid][FamilyFame],
        FamilyInfo[famid][FamilyCash], FA_GetMaxCash(lv),
        FamilyInfo[famid][FamilyPot] + FamilyInfo[famid][FamilyCrack], FA_GetMaxGoods(lv));

    g_safeLabel[famid] = _:CreateDynamic3DTextLabel(labelText, 0xFFAA00FF, sx, sy, sz + 1.0, 10.0);
}

static stock FA_ShowDepositAmount(playerid, itemType)
{
    g_faDepositItem[playerid] = itemType;
    new fam = PlayerInfo[playerid][pFMember];
    new lv  = FamilyInfo[fam][FamilyLevel];
    new itemLabel[6][] = {"Tien mat", "Can sa", "Crack", "Can sa (moi)", "Tep can sa", "Vat lieu"};

    new playerAmt, safeAmt, safeMax;
    switch(itemType)
    {
        case 0: { playerAmt = GetPlayerCash(playerid); safeAmt = FamilyInfo[fam][FamilyCash]; safeMax = FA_GetMaxCash(lv); }
        case 1: { playerAmt = PlayerInfo[playerid][pPot]; safeAmt = FamilyInfo[fam][FamilyPot]; safeMax = FA_GetMaxGoods(lv); }
        case 2: { playerAmt = PlayerInfo[playerid][pCrack]; safeAmt = FamilyInfo[fam][FamilyCrack]; safeMax = FA_GetMaxGoods(lv); }
        case 3: { playerAmt = PlayerInfo[playerid][pCanSa]; safeAmt = FamilyInfo[fam][FamilyCansa]; safeMax = FA_GetMaxGoods(lv); }
        case 4: { playerAmt = PlayerInfo[playerid][pTepCanSa]; safeAmt = FamilyInfo[fam][FamilyTepCansa]; safeMax = FA_GetMaxGoods(lv); }
        case 5: { playerAmt = PlayerInfo[playerid][pMats]; safeAmt = FamilyInfo[fam][FamilyMats]; safeMax = FA_GetMaxMats(lv); }
    }

    new title[48];
    format(title, sizeof(title), "Gui Vao Ket: %s", itemLabel[itemType]);
    new body[192];
    format(body, sizeof(body),
        "{AAAAAA}Tren nguoi ban:  {FFFFFF}%d\n{AAAAAA}Trong ket gang:  {FFFFFF}%d {AAAAAA}/ %d\n\n{FFFFFF}Nhap so luong muon gui:",
        playerAmt, safeAmt, safeMax);
    ShowPlayerDialog(playerid, DIALOG_SAFE_DEPOSIT_AMT, DIALOG_STYLE_INPUT, title, body, "Gui", "Quay Lai");
}

static stock FA_ShowWithdrawAmount(playerid, itemType)
{
    g_faWithdrawItem[playerid] = itemType;
    new fam  = PlayerInfo[playerid][pFMember];
    new lv   = FamilyInfo[fam][FamilyLevel];
    new itemLabel[6][] = {"Tien mat", "Can sa", "Crack", "Can sa (moi)", "Tep can sa", "Vat lieu"};

    new playerAmt, safeAmt, safeMax;
    switch(itemType)
    {
        case 0: { playerAmt = GetPlayerCash(playerid); safeAmt = FamilyInfo[fam][FamilyCash]; safeMax = FA_GetMaxCash(lv); }
        case 1: { playerAmt = PlayerInfo[playerid][pPot]; safeAmt = FamilyInfo[fam][FamilyPot]; safeMax = FA_GetMaxGoods(lv); }
        case 2: { playerAmt = PlayerInfo[playerid][pCrack]; safeAmt = FamilyInfo[fam][FamilyCrack]; safeMax = FA_GetMaxGoods(lv); }
        case 3: { playerAmt = PlayerInfo[playerid][pCanSa]; safeAmt = FamilyInfo[fam][FamilyCansa]; safeMax = FA_GetMaxGoods(lv); }
        case 4: { playerAmt = PlayerInfo[playerid][pTepCanSa]; safeAmt = FamilyInfo[fam][FamilyTepCansa]; safeMax = FA_GetMaxGoods(lv); }
        case 5: { playerAmt = PlayerInfo[playerid][pMats]; safeAmt = FamilyInfo[fam][FamilyMats]; safeMax = FA_GetMaxMats(lv); }
    }

    new title[48];
    format(title, sizeof(title), "Rut Khoi Ket: %s", itemLabel[itemType]);
    new body[192];
    format(body, sizeof(body),
        "{AAAAAA}Tren nguoi ban:  {FFFFFF}%d\n{AAAAAA}Trong ket gang:  {FFFFFF}%d {AAAAAA}/ %d\n\n{FFFFFF}Nhap so luong muon rut:",
        playerAmt, safeAmt, safeMax);
    ShowPlayerDialog(playerid, DIALOG_SAFE_WITHDRAW_AMT, DIALOG_STYLE_INPUT, title, body, "Rut", "Quay Lai");
}

// ---------------------------------------------------------------------------
// DIALOG – LỊCH SỬ
// ---------------------------------------------------------------------------

// Per-player offset cho paging
new g_faLogOffset[MAX_PLAYERS];

stock FA_ShowLogs(playerid, famid)
{
    g_faLogOffset[playerid] = 0;
    new q[192];
    mysql_format(MainPipeline, q, sizeof(q),
        "SELECT `player_name`,`action_type`,`item_type`,`amount`,`timestamp` \
         FROM `family_safe_logs` WHERE `family_id`=%d ORDER BY `id` DESC LIMIT 20",
        famid);
    mysql_tquery(MainPipeline, q, "FA_OnLoadLogs", "i", playerid);
}

forward FA_OnLoadLogs(playerid);
public FA_OnLoadLogs(playerid)
{
    if(!IsPlayerConnected(playerid)) return;
    new rows = cache_num_rows();
    if(rows == 0)
    {
        ShowPlayerDialog(playerid, DIALOG_SAFE_LOGS, DIALOG_STYLE_MSGBOX,
            "Lich Su Ket", "(Chua co giao dich nao)", "OK", "");
        return;
    }

    new content[1024]; content[0] = 0;
    for(new i = 0; i < rows; i++)
    {
        new pname[25], atype[12], itype[22], amt;
        cache_get_value_name(i, "player_name", pname, sizeof(pname));
        cache_get_value_name(i, "action_type", atype, sizeof(atype));
        cache_get_value_name(i, "item_type",   itype, sizeof(itype));
        cache_get_value_name_int(i, "amount",  amt);

        new actionStr[32];
        if(atype[0] == 'd') format(actionStr, sizeof(actionStr), "{00FF7F}[+] Gui vao");
        else                format(actionStr, sizeof(actionStr), "{FF6B6B}[-] Rut ra");

        new itemStr[20];
        if(!strcmp(itype, "cash",     true)) format(itemStr, sizeof(itemStr), "Tien mat");
        else if(!strcmp(itype, "pot", true)) format(itemStr, sizeof(itemStr), "Can sa");
        else if(!strcmp(itype, "crack",true)) format(itemStr, sizeof(itemStr), "Crack");
        else if(!strcmp(itype, "cansa",true)) format(itemStr, sizeof(itemStr), "Can sa moi");
        else if(!strcmp(itype, "tepcansa",true)) format(itemStr, sizeof(itemStr), "Tep can sa");
        else if(!strcmp(itype, "mats",true)) format(itemStr, sizeof(itemStr), "Vat lieu");
        else if(!strcmp(itype, "gun", true)) format(itemStr, sizeof(itemStr), "Vu khi");
        else format(itemStr, sizeof(itemStr), "%s", itype);

        new line[128];
        format(line, sizeof(line), "%s {AAAAAA}%s  {FFFFFF}%s: {FFFF00}%d\n",
            actionStr, itemStr, pname, amt);
        strcat(content, line, sizeof(content));
    }
    ShowPlayerDialog(playerid, DIALOG_SAFE_LOGS, DIALOG_STYLE_MSGBOX, "Lich Su Ket (20 GD gan nhat)", content, "OK", "");
}

// ---------------------------------------------------------------------------
// DIALOG – QUẢN LÝ VŨ KHÍ
// ---------------------------------------------------------------------------

static stock FA_ShowGunDialog(playerid, famid)
{
    new lv       = FamilyInfo[famid][FamilyLevel];
    new maxSlots = FA_GetMaxGunSlots(lv);
    new usedSlots = 0;
    for(new s = 0; s < maxSlots; s++)
        if(FamilyInfo[famid][FamilyNewGuns][s] > 0) usedSlots++;

    new list[1800]; list[0] = 0;

    for(new s = 0; s < maxSlots; s++)
    {
        new line[96];
        new wid = FamilyInfo[famid][FamilyNewGuns][s];
        if(wid > 0)
        {
            new wname[24];
            GetWeaponName(wid, wname, sizeof(wname));
            format(line, sizeof(line), "{FF8800}[CO SUNG] {FFFFFF}Slot %d: {FFFF00}%s {AAAAAA}(%d dan)\n",
                s + 1, wname, FamilyInfo[famid][FamilyNewGunAmmo][s]);
        }
        else
            format(line, sizeof(line), "{555555}[TRONG]   {AAAAAA}Slot %d: {555555}---\n", s + 1);
        strcat(list, line, sizeof(list));
    }

    ShowPlayerDialog(playerid, DIALOG_SAFE_GUNS, DIALOG_STYLE_LIST, "Kho Vu Khi Gang", list, "Chon", "Quay Lai");
}

// Danh sách vũ khí trên người để gửi vào két
static stock FA_ShowGunStoreList(playerid)
{
    new list[1024]; list[0] = 0;
    new cnt = 0;
    for(new s = 0; s < 13; s++)
    {
        new wid, ammo;
        GetPlayerWeaponData(playerid, s, wid, ammo);
        if(wid <= 0 || wid == 1) continue; // bỏ qua fist và invalid
        g_faGunWeps[playerid][cnt] = s; // lưu slot để lấy lại sau
        new wname[24];
        GetWeaponName(wid, wname, sizeof(wname));
        new line[64];
        format(line, sizeof(line), "%s  (%d dan)\n", wname, ammo);
        strcat(list, line, sizeof(list));
        cnt++;
        if(cnt >= 13) break;
    }
    if(cnt == 0)
    {
        SendClientMessageEx(playerid, COLOR_RED, "Ban khong co vu khi nao de gui vao ket.");
        FA_ShowGunDialog(playerid, PlayerInfo[playerid][pFMember]);
        return;
    }
    ShowPlayerDialog(playerid, DIALOG_SAFE_GUN_STORE, DIALOG_STYLE_LIST,
        "Chon Vu Khi De Gui Vao Ket", list, "Gui", "Quay Lai");
}

// ---------------------------------------------------------------------------
// BANK COMMAND
// ---------------------------------------------------------------------------

CMD:fbank(playerid, params[])
{
    new fam = PlayerInfo[playerid][pFMember];
    if(fam == INVALID_FAMILY_ID)
        return SendClientMessageEx(playerid, COLOR_RED, "Ban chua vao gang nao."), 1;

    ShowPlayerDialog(playerid, DIALOG_BANK_MAIN, DIALOG_STYLE_LIST,
        "Ngan Hang Gang",
        "Xem so du\nNap tien vao bank\nRut tien khoi bank (rank 5+)",
        "Chon", "Dong");
    return 1;
}

// ---------------------------------------------------------------------------
// LỆNH /glocker – Mở két sắt gang
// ---------------------------------------------------------------------------

CMD:glocker(playerid, params[])
{
    #pragma unused params
    new fam = PlayerInfo[playerid][pFMember];
    if(fam == INVALID_FAMILY_ID)
        return SendClientMessageEx(playerid, COLOR_RED, "Ban chua vao gang nao.");

    new Float:sx = FamilyInfo[fam][FamilySafe][0];
    new Float:sy = FamilyInfo[fam][FamilySafe][1];
    new Float:sz = FamilyInfo[fam][FamilySafe][2];

    if(sx == 0.0 && sy == 0.0)
        return SendClientMessageEx(playerid, COLOR_RED, "Gang chua dat vi tri ket sac. Rank 5+ dung /gangs de dat."), 1;

    new Float:px, Float:py, Float:pz;
    GetPlayerPos(playerid, px, py, pz);
    new Float:d = floatsqroot((px-sx)*(px-sx)+(py-sy)*(py-sy)+(pz-sz)*(pz-sz));
    if(d > 3.0)
        return SendClientMessageEx(playerid, COLOR_RED, "Ban phai dung gan ket sac gang.");

    FA_OpenSafeDialog(playerid);
    return 1;
}

// ---------------------------------------------------------------------------
// NÂNG CẤP KÉT
// ---------------------------------------------------------------------------

stock FA_UpgradeSafe(playerid, famid)
{
    new lv = FamilyInfo[famid][FamilyLevel];
    if(lv >= MAX_SAFE_LEVEL)
        return SendClientMessageEx(playerid, COLOR_RED, "Ket sac da dat cap toi da (Level 8)."), 0;

    new nextLv    = lv + 1;
    new costCash  = FA_GetUpgradeCostCash(nextLv);
    new costMats  = FA_GetUpgradeCostMats(nextLv);

    if(GetPlayerCash(playerid) < costCash)
    {
        new str[96]; format(str, sizeof(str), "Ban can $%d tien mat tren nguoi de nang cap.", costCash);
        return SendClientMessageEx(playerid, COLOR_RED, str), 0;
    }
    if(PlayerInfo[playerid][pMats] < costMats)
    {
        new str[96]; format(str, sizeof(str), "Ban can %d vat lieu tren nguoi de nang cap.", costMats);
        return SendClientMessageEx(playerid, COLOR_RED, str), 0;
    }

    GivePlayerCash(playerid, -costCash);
    PlayerInfo[playerid][pMats] -= costMats;

    FamilyInfo[famid][FamilyLevel] = nextLv;
    SaveFamilyExt(famid);
    SaveFamily(famid);
    FA_UpdateSafeLabel(famid);

    new str[128];
    format(str, sizeof(str),
        "Nang cap ket sac thanh cong! Level %d -> %d. Suc chua tien: $%d | Hang: %d | VL: %d",
        lv, nextLv, FA_GetMaxCash(nextLv), FA_GetMaxGoods(nextLv), FA_GetMaxMats(nextLv));
    SendClientMessageEx(playerid, COLOR_GREEN, str);
    return 1;
}

// ---------------------------------------------------------------------------
// DIALOG RESPONSE
// ---------------------------------------------------------------------------

stock FA_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    new fam  = PlayerInfo[playerid][pFMember];
    new rank = PlayerInfo[playerid][pRank];

    switch(dialogid)
    {
        // ---- Main safe menu ----
        case DIALOG_SAFE_MAIN:
        {
            if(!response) return 1;
            if(listitem < 3) return 1;
            switch(listitem - 3)
            {
                case 0: FA_ShowSafeView(playerid, fam);
                case 1: FA_ShowItemList(playerid, fam);
                case 2: FA_ShowGunDialog(playerid, fam);
                case 3: { if(rank >= 5) FA_ShowLogs(playerid, fam); }
            }
            return 1;
        }

        case DIALOG_SAFE_VIEW:
        {
            if(!response) FA_OpenSafeDialog(playerid);
            return 1;
        }

        // ---- Danh sách vật phẩm ----
        case DIALOG_SAFE_ITEM_LIST:
        {
            if(!response) { FA_OpenSafeDialog(playerid); return 1; }
            if(listitem < 0 || listitem > 5) return 1;
            FA_ShowItemAction(playerid, listitem);
            return 1;
        }

        // ---- Chọn hành động (gửi / rút) ----
        case DIALOG_SAFE_ITEM_ACT:
        {
            if(!response) { FA_ShowItemList(playerid, fam); return 1; }
            if(listitem < 4) return 1;
            switch(listitem - 4)
            {
                case 0: FA_ShowDepositAmount(playerid, g_faSafeAction[playerid]);
                case 1:
                {
                    if(rank < 4)
                        return SendClientMessageEx(playerid, COLOR_RED, "Can rank 4+ de rut do."), 1;
                    FA_ShowWithdrawAmount(playerid, g_faSafeAction[playerid]);
                }
            }
            return 1;
        }

        // ---- Nhập số lượng gửi ----
        case DIALOG_SAFE_DEPOSIT_AMT:
        {
            if(!response) { FA_ShowItemAction(playerid, g_faDepositItem[playerid]); return 1; }
            if(fam == INVALID_FAMILY_ID) return 1;
            new amount = strval(inputtext);
            if(amount <= 0) return SendClientMessageEx(playerid, COLOR_RED, "So luong khong hop le."), 1;

            new lv  = FamilyInfo[fam][FamilyLevel];
            new itm = g_faDepositItem[playerid];
            new itemLabel[6][] = {"Tien mat", "Can sa", "Crack", "Can sa (moi)", "Tep can sa", "Vat lieu"};
            new itemKey[6][]   = {"cash", "pot", "crack", "cansa", "tepcansa", "mats"};

            switch(itm)
            {
                case 0:
                {
                    if(GetPlayerCash(playerid) < amount) return SendClientMessageEx(playerid, COLOR_RED, "Khong du tien."), 1;
                    if(FamilyInfo[fam][FamilyCash] + amount > FA_GetMaxCash(lv))
                        return SendClientMessageEx(playerid, COLOR_RED, "Ket day tien. Nang cap de tang suc chua."), 1;
                    GivePlayerCash(playerid, -amount);
                    FamilyInfo[fam][FamilyCash] += amount;
                }
                case 1:
                {
                    if(PlayerInfo[playerid][pPot] < amount) return SendClientMessageEx(playerid, COLOR_RED, "Khong du can sa."), 1;
                    if(FamilyInfo[fam][FamilyPot] + amount > FA_GetMaxGoods(lv))
                        return SendClientMessageEx(playerid, COLOR_RED, "Ket day can sa."), 1;
                    PlayerInfo[playerid][pPot] -= amount;
                    FamilyInfo[fam][FamilyPot] += amount;
                }
                case 2:
                {
                    if(PlayerInfo[playerid][pCrack] < amount) return SendClientMessageEx(playerid, COLOR_RED, "Khong du crack."), 1;
                    if(FamilyInfo[fam][FamilyCrack] + amount > FA_GetMaxGoods(lv))
                        return SendClientMessageEx(playerid, COLOR_RED, "Ket day crack."), 1;
                    PlayerInfo[playerid][pCrack] -= amount;
                    FamilyInfo[fam][FamilyCrack] += amount;
                }
                case 3:
                {
                    if(PlayerInfo[playerid][pCanSa] < amount) return SendClientMessageEx(playerid, COLOR_RED, "Khong du can sa (moi)."), 1;
                    if(FamilyInfo[fam][FamilyCansa] + amount > FA_GetMaxGoods(lv))
                        return SendClientMessageEx(playerid, COLOR_RED, "Ket day can sa (moi)."), 1;
                    PlayerInfo[playerid][pCanSa] -= amount;
                    FamilyInfo[fam][FamilyCansa] += amount;
                }
                case 4:
                {
                    if(PlayerInfo[playerid][pTepCanSa] < amount) return SendClientMessageEx(playerid, COLOR_RED, "Khong du tep can sa."), 1;
                    if(FamilyInfo[fam][FamilyTepCansa] + amount > FA_GetMaxGoods(lv))
                        return SendClientMessageEx(playerid, COLOR_RED, "Ket day tep can sa."), 1;
                    PlayerInfo[playerid][pTepCanSa] -= amount;
                    FamilyInfo[fam][FamilyTepCansa] += amount;
                }
                case 5:
                {
                    if(PlayerInfo[playerid][pMats] < amount) return SendClientMessageEx(playerid, COLOR_RED, "Khong du vat lieu."), 1;
                    if(FamilyInfo[fam][FamilyMats] + amount > FA_GetMaxMats(lv))
                        return SendClientMessageEx(playerid, COLOR_RED, "Ket day vat lieu."), 1;
                    PlayerInfo[playerid][pMats] -= amount;
                    FamilyInfo[fam][FamilyMats] += amount;
                }
            }

            FA_LogTransaction(fam, playerid, "deposit", itemKey[itm], amount);
            SaveFamily(fam);
            new str[96];
            format(str, sizeof(str), "Da gui %d %s vao ket sac gang.", amount, itemLabel[itm]);
            SendClientMessageEx(playerid, COLOR_GREEN, str);
            FA_NotifyGang(fam, playerid, str);
            return 1;
        }

        // ---- Nhập số lượng rút ----
        case DIALOG_SAFE_WITHDRAW_AMT:
        {
            if(!response) { FA_ShowItemAction(playerid, g_faWithdrawItem[playerid]); return 1; }
            if(rank < 4 || fam == INVALID_FAMILY_ID) return 1;
            new amount = strval(inputtext);
            if(amount <= 0) return SendClientMessageEx(playerid, COLOR_RED, "So luong khong hop le."), 1;

            new itm = g_faWithdrawItem[playerid];
            new itemLabel[6][] = {"Tien mat", "Can sa", "Crack", "Can sa (moi)", "Tep can sa", "Vat lieu"};
            new itemKey[6][]   = {"cash", "pot", "crack", "cansa", "tepcansa", "mats"};

            switch(itm)
            {
                case 0:
                {
                    if(FamilyInfo[fam][FamilyCash] < amount) return SendClientMessageEx(playerid, COLOR_RED, "Ket khong du tien."), 1;
                    FamilyInfo[fam][FamilyCash] -= amount;
                    GivePlayerCash(playerid, amount);
                }
                case 1:
                {
                    if(FamilyInfo[fam][FamilyPot] < amount) return SendClientMessageEx(playerid, COLOR_RED, "Ket khong du can sa."), 1;
                    FamilyInfo[fam][FamilyPot] -= amount;
                    PlayerInfo[playerid][pPot] += amount;
                }
                case 2:
                {
                    if(FamilyInfo[fam][FamilyCrack] < amount) return SendClientMessageEx(playerid, COLOR_RED, "Ket khong du crack."), 1;
                    FamilyInfo[fam][FamilyCrack] -= amount;
                    PlayerInfo[playerid][pCrack] += amount;
                }
                case 3:
                {
                    if(FamilyInfo[fam][FamilyCansa] < amount) return SendClientMessageEx(playerid, COLOR_RED, "Ket khong du can sa (moi)."), 1;
                    FamilyInfo[fam][FamilyCansa] -= amount;
                    PlayerInfo[playerid][pCanSa] += amount;
                }
                case 4:
                {
                    if(FamilyInfo[fam][FamilyTepCansa] < amount) return SendClientMessageEx(playerid, COLOR_RED, "Ket khong du tep can sa."), 1;
                    FamilyInfo[fam][FamilyTepCansa] -= amount;
                    PlayerInfo[playerid][pTepCanSa] += amount;
                }
                case 5:
                {
                    if(FamilyInfo[fam][FamilyMats] < amount) return SendClientMessageEx(playerid, COLOR_RED, "Ket khong du vat lieu."), 1;
                    FamilyInfo[fam][FamilyMats] -= amount;
                    PlayerInfo[playerid][pMats] += amount;
                }
            }

            FA_LogTransaction(fam, playerid, "withdraw", itemKey[itm], amount);
            SaveFamily(fam);
            new str[96];
            format(str, sizeof(str), "Da rut %d %s tu ket sac gang.", amount, itemLabel[itm]);
            SendClientMessageEx(playerid, COLOR_GREEN, str);
            FA_NotifyGang(fam, playerid, str);
            return 1;
        }

        // ---- Danh sách log ----
        case DIALOG_SAFE_LOGS: return 1;

        // ---- Kho vũ khí ----
        case DIALOG_SAFE_GUNS:
        {
            if(!response) { FA_OpenSafeDialog(playerid); return 1; }
            if(fam == INVALID_FAMILY_ID) return 1;
            new lv       = FamilyInfo[fam][FamilyLevel];
            new maxSlots = FA_GetMaxGunSlots(lv);
            if(listitem >= maxSlots) return 1;
            new slotIdx = listitem;

            g_faSafeGunSlot[playerid] = slotIdx;
            new wid = FamilyInfo[fam][FamilyNewGuns][slotIdx];

            if(wid == 0)
            {
                // Slot trống → hiện danh sách vũ khí trên người
                FA_ShowGunStoreList(playerid);
            }
            else
            {
                // Slot có súng → kiểm tra player đã có cùng loại chưa
                new playerHasSame = 0;
                new playerAmmoSame = 0;
                for(new s = 0; s < 13; s++)
                {
                    new pwid, pammo;
                    GetPlayerWeaponData(playerid, s, pwid, pammo);
                    if(pwid == wid) { playerHasSame = 1; playerAmmoSame = pammo; break; }
                }

                if(rank < 4) return SendClientMessageEx(playerid, COLOR_RED, "Can rank 4+ de lay vu khi."), 1;

                if(playerHasSame)
                {
                    new wname[24];
                    GetWeaponName(wid, wname, sizeof(wname));
                    new confirm[256];
                    format(confirm, sizeof(confirm),
                        "Canh bao: Ban da co {FFFF00}%s{FFFFFF} (%d dan) tren nguoi.\n\n\
Lay tu ket se cong them {44FF44}%d dan{FFFFFF} vao vu khi hien tai.\n\n\
Ban co muon lay ra khong?",
                        wname, playerAmmoSame, FamilyInfo[fam][FamilyNewGunAmmo][slotIdx]);
                    ShowPlayerDialog(playerid, DIALOG_SAFE_GUN_GET, DIALOG_STYLE_MSGBOX,
                        "Xac Nhan Lay Vu Khi", confirm, "Xac Nhan", "Huy");
                }
                else
                {
                    // Lấy trực tiếp
                    new ammo = FamilyInfo[fam][FamilyNewGunAmmo][slotIdx];
                    GivePlayerWeapon(playerid, wid, ammo);
                    FamilyInfo[fam][FamilyNewGuns][slotIdx]    = 0;
                    FamilyInfo[fam][FamilyNewGunAmmo][slotIdx] = 0;
                    FA_SaveGunSlot(fam, slotIdx);
                    FA_LogTransaction(fam, playerid, "withdraw", "gun", ammo, wid);

                    new wname[24]; GetWeaponName(wid, wname, sizeof(wname));
                    new str[96];
                    format(str, sizeof(str), "Da lay %s (%d dan) tu ket sac.", wname, ammo);
                    SendClientMessageEx(playerid, COLOR_GREEN, str);
                    FA_NotifyGang(fam, playerid, str);
                }
            }
            return 1;
        }

        // ---- Gửi vũ khí vào két ----
        case DIALOG_SAFE_GUN_STORE:
        {
            if(!response) { FA_ShowGunDialog(playerid, fam); return 1; }
            if(fam == INVALID_FAMILY_ID) return 1;
            new safeSlot = g_faSafeGunSlot[playerid];
            if(safeSlot < 0 || safeSlot >= 26) return 1;

            // Lấy weapon slot của player từ bảng đã scan
            new pSlot = g_faGunWeps[playerid][listitem];
            new wid, ammo;
            GetPlayerWeaponData(playerid, pSlot, wid, ammo);
            if(wid <= 0) return SendClientMessageEx(playerid, COLOR_RED, "Vu khi khong hop le."), 1;

            // Kiểm tra safe slot vẫn còn trống
            if(FamilyInfo[fam][FamilyNewGuns][safeSlot] != 0)
                return SendClientMessageEx(playerid, COLOR_RED, "Slot vu khi da co nguoi khac su dung. Thu slot khac."), 1;

            RemovePlayerWeapon(playerid, wid);
            FamilyInfo[fam][FamilyNewGuns][safeSlot]    = wid;
            FamilyInfo[fam][FamilyNewGunAmmo][safeSlot] = ammo;
            FA_SaveGunSlot(fam, safeSlot);
            FA_LogTransaction(fam, playerid, "deposit", "gun", ammo, wid);

            new wname[24]; GetWeaponName(wid, wname, sizeof(wname));
            new str[96];
            format(str, sizeof(str), "Da gui %s (%d dan) vao ket sac.", wname, ammo);
            SendClientMessageEx(playerid, COLOR_GREEN, str);
            FA_NotifyGang(fam, playerid, str);
            return 1;
        }

        // ---- Xác nhận lấy súng khi đã có cùng loại ----
        case DIALOG_SAFE_GUN_GET:
        {
            if(!response) { FA_ShowGunDialog(playerid, fam); return 1; }
            if(fam == INVALID_FAMILY_ID) return 1;
            new safeSlot = g_faSafeGunSlot[playerid];
            new wid  = FamilyInfo[fam][FamilyNewGuns][safeSlot];
            new ammo = FamilyInfo[fam][FamilyNewGunAmmo][safeSlot];
            if(wid == 0) return 1;

            GivePlayerWeapon(playerid, wid, ammo);
            FamilyInfo[fam][FamilyNewGuns][safeSlot]    = 0;
            FamilyInfo[fam][FamilyNewGunAmmo][safeSlot] = 0;
            FA_SaveGunSlot(fam, safeSlot);
            FA_LogTransaction(fam, playerid, "withdraw", "gun", ammo, wid);

            new wname[24]; GetWeaponName(wid, wname, sizeof(wname));
            new str[96];
            format(str, sizeof(str), "Da lay %s (%d dan) tu ket sac.", wname, ammo);
            SendClientMessageEx(playerid, COLOR_GREEN, str);
            FA_NotifyGang(fam, playerid, str);
            return 1;
        }

        // ---- Bank ----
        case DIALOG_BANK_MAIN:
        {
            if(!response) return 1;
            if(fam == INVALID_FAMILY_ID) return 1;
            switch(listitem)
            {
                case 0:
                {
                    new str[80];
                    format(str, sizeof(str), "So du bank gang [%s]: $%d",
                        FamilyInfo[fam][FamilyName], FamilyInfo[fam][FamilyBank]);
                    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, str);
                }
                case 1:
                {
                    new Float:bx, Float:by, Float:bz;
                    GetPlayerPos(playerid, bx, by, bz);
                    new Float:d = floatsqroot((bx-BANK_POS_X)*(bx-BANK_POS_X)+(by-BANK_POS_Y)*(by-BANK_POS_Y));
                    if(d > BANK_RADIUS)
                        return SendClientMessageEx(playerid, COLOR_RED, "Ban phai dung tai ngan hang de nap tien."), 1;
                    ShowPlayerDialog(playerid, DIALOG_BANK_DEPOSIT, DIALOG_STYLE_INPUT,
                        "Nap Tien Vao Bank Gang", "Nhap so tien muon nap:", "Nap", "Huy");
                }
                case 2:
                {
                    if(rank < 5) return SendClientMessageEx(playerid, COLOR_RED, "Can rank 5+ de rut tien bank."), 1;
                    new Float:bx, Float:by, Float:bz;
                    GetPlayerPos(playerid, bx, by, bz);
                    new Float:d = floatsqroot((bx-BANK_POS_X)*(bx-BANK_POS_X)+(by-BANK_POS_Y)*(by-BANK_POS_Y));
                    if(d > BANK_RADIUS)
                        return SendClientMessageEx(playerid, COLOR_RED, "Ban phai dung tai ngan hang de rut tien."), 1;
                    ShowPlayerDialog(playerid, DIALOG_BANK_WITHDRAW, DIALOG_STYLE_INPUT,
                        "Rut Tien Khoi Bank Gang", "Nhap so tien muon rut:", "Rut", "Huy");
                }
            }
            return 1;
        }

        case DIALOG_BANK_DEPOSIT:
        {
            if(!response) return 1;
            if(fam == INVALID_FAMILY_ID) return 1;
            new amount = strval(inputtext);
            if(amount <= 0) return SendClientMessageEx(playerid, COLOR_RED, "So tien khong hop le."), 1;
            if(GetPlayerCash(playerid) < amount) return SendClientMessageEx(playerid, COLOR_RED, "Khong du tien."), 1;
            GivePlayerCash(playerid, -amount);
            FamilyInfo[fam][FamilyBank] += amount;
            SaveFamily(fam);
            new str[64];
            format(str, sizeof(str), "Da nap $%d vao bank gang.", amount);
            SendClientMessageEx(playerid, COLOR_GREEN, str);
            return 1;
        }

        case DIALOG_BANK_WITHDRAW:
        {
            if(!response) return 1;
            if(rank < 5 || fam == INVALID_FAMILY_ID) return 1;
            new amount = strval(inputtext);
            if(amount <= 0) return SendClientMessageEx(playerid, COLOR_RED, "So tien khong hop le."), 1;
            if(FamilyInfo[fam][FamilyBank] < amount) return SendClientMessageEx(playerid, COLOR_RED, "Bank gang khong du tien."), 1;
            FamilyInfo[fam][FamilyBank] -= amount;
            GivePlayerCash(playerid, amount);
            SaveFamily(fam);
            new str[64];
            format(str, sizeof(str), "Da rut $%d tu bank gang.", amount);
            SendClientMessageEx(playerid, COLOR_GREEN, str);
            return 1;
        }
    }
    return 0;
}

// ---------------------------------------------------------------------------
// PICKUP HANDLER (gọi từ OnPlayerPickUpPickup)
// ---------------------------------------------------------------------------

stock FA_OnPlayerPickUpPickup(playerid, pickupid)
{
    // Két sắt không dùng auto-dialog nữa — dùng /glocker
    #pragma unused playerid, pickupid
    return 0;
}

// ---------------------------------------------------------------------------
// LỆNH GỬI SÚNG VÀO KÉT (/fstoregun2)
// ---------------------------------------------------------------------------

CMD:fstoregun2(playerid, params[])
{
    new fam = PlayerInfo[playerid][pFMember];
    if(fam == INVALID_FAMILY_ID)
        return SendClientMessageEx(playerid, COLOR_RED, "Ban chua vao gang."), 1;

    // Kiểm tra gần két
    new Float:px, Float:py, Float:pz;
    GetPlayerPos(playerid, px, py, pz);
    new Float:sx = FamilyInfo[fam][FamilySafe][0],
        Float:sy = FamilyInfo[fam][FamilySafe][1],
        Float:sz = FamilyInfo[fam][FamilySafe][2];
    new Float:d = floatsqroot((px-sx)*(px-sx)+(py-sy)*(py-sy)+(pz-sz)*(pz-sz));
    if(d > 3.0)
        return SendClientMessageEx(playerid, COLOR_RED, "Ban phai dung gan ket sac gang."), 1;

    // Lấy vũ khí đang cầm
    new wid = GetPlayerWeapon(playerid);
    if(wid <= 0 || wid == 1)
        return SendClientMessageEx(playerid, COLOR_RED, "Ban phai cam vu khi de cat vao ket."), 1;

    // Tìm slot trống
    new lv       = FamilyInfo[fam][FamilyLevel];
    new maxSlots = FA_GetMaxGunSlots(lv);
    new slot     = -1;
    for(new s = 0; s < maxSlots; s++)
    {
        if(FamilyInfo[fam][FamilyNewGuns][s] == 0) { slot = s; break; }
    }
    if(slot == -1)
        return SendClientMessageEx(playerid, COLOR_RED, "Khong con slot vu khi. Nang cap ket de them slot."), 1;

    new ammo = GetPlayerAmmo(playerid);
    RemovePlayerWeapon(playerid, wid);
    FamilyInfo[fam][FamilyNewGuns][slot]    = wid;
    FamilyInfo[fam][FamilyNewGunAmmo][slot] = ammo;
    FA_SaveGunSlot(fam, slot);

    new wname[24];
    GetWeaponName(wid, wname, sizeof(wname));
    new str[64];
    format(str, sizeof(str), "Da cat %s (%d dan) vao slot %d.", wname, ammo, slot+1);
    SendClientMessageEx(playerid, COLOR_GREEN, str);
    FA_LogTransaction(fam, playerid, "deposit", "gun", ammo, wid);
    return 1;
}
