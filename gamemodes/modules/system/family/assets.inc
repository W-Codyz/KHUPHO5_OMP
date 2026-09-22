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
    // Thêm cột mới vào bảng families nếu chưa có
    mysql_tquery(MainPipeline, "ALTER TABLE `families` ADD COLUMN IF NOT EXISTS `level` INT DEFAULT 1", "", "");
    mysql_tquery(MainPipeline, "ALTER TABLE `families` ADD COLUMN IF NOT EXISTS `fame` INT DEFAULT 0", "", "");
    mysql_tquery(MainPipeline, "ALTER TABLE `families` ADD COLUMN IF NOT EXISTS `cansa` INT DEFAULT 0", "", "");
    mysql_tquery(MainPipeline, "ALTER TABLE `families` ADD COLUMN IF NOT EXISTS `tepcansa` INT DEFAULT 0", "", "");

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
        "SELECT `id`,`level`,`fame`,`cansa`,`tepcansa` FROM `families`",
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
        new famid, lv, fame, cansa, tepcansa;
        cache_get_value_name_int(i, "id",       famid);
        cache_get_value_name_int(i, "level",    lv);
        cache_get_value_name_int(i, "fame",     fame);
        cache_get_value_name_int(i, "cansa",    cansa);
        cache_get_value_name_int(i, "tepcansa", tepcansa);

        if(famid < 0 || famid >= MAX_FAMILY) continue;
        FamilyInfo[famid][FamilyLevel]    = lv    < 1 ? 1 : lv;
        FamilyInfo[famid][FamilyFame]     = fame;
        FamilyInfo[famid][FamilyCansa]    = cansa;
        FamilyInfo[famid][FamilyTepCansa] = tepcansa;
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
        "UPDATE `families` SET `level`=%d,`fame`=%d,`cansa`=%d,`tepcansa`=%d WHERE `id`=%d",
        FamilyInfo[familyid][FamilyLevel],
        FamilyInfo[familyid][FamilyFame],
        FamilyInfo[familyid][FamilyCansa],
        FamilyInfo[familyid][FamilyTepCansa],
        familyid
    );
    mysql_tquery(MainPipeline, q, "", "");
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

    // Build menu tùy theo rank
    new menu[256];
    format(menu, sizeof(menu), "Xem noi dung ket\nGui do vao ket (rank 1+)");
    if(rank >= 4) strcat(menu, "\nRut do khoi ket (rank 4+)", sizeof(menu));
    if(rank >= 5) strcat(menu, "\nLich su giao dich (rank 5+)", sizeof(menu));
    strcat(menu, "\nGui / Lay vu khi", sizeof(menu));

    ShowPlayerDialog(playerid, DIALOG_SAFE_MAIN, DIALOG_STYLE_LIST, "Ket Sac Gang", menu, "Chon", "Dong");
    return 1;
}

// ---------------------------------------------------------------------------
// DIALOG – NỘI DUNG KÉT
// ---------------------------------------------------------------------------

static stock FA_ShowSafeView(playerid, famid)
{
    new lv    = FamilyInfo[famid][FamilyLevel];
    new str[512];
    format(str, sizeof(str),
        "Tien mat:   %d / %d\nCan sa:     %d / %d\nCrack:      %d / %d\nCan sa moi: %d / %d\nTep can sa: %d / %d\nVat lieu:   %d / %d\nSlot vu khi: %d / %d\nLevel ket:   %d | Fame: %d",
        FamilyInfo[famid][FamilyCash],      FA_GetMaxCash(lv),
        FamilyInfo[famid][FamilyPot],       FA_GetMaxGoods(lv),
        FamilyInfo[famid][FamilyCrack],     FA_GetMaxGoods(lv),
        FamilyInfo[famid][FamilyCansa],     FA_GetMaxGoods(lv),
        FamilyInfo[famid][FamilyTepCansa],  FA_GetMaxGoods(lv),
        FamilyInfo[famid][FamilyMats],      FA_GetMaxMats(lv),
        FA_GetMaxGunSlots(lv), 26,
        lv,
        FamilyInfo[famid][FamilyFame]);

    ShowPlayerDialog(playerid, DIALOG_SAFE_VIEW, DIALOG_STYLE_MSGBOX, "Noi Dung Ket Sac", str, "OK", "");
}

// ---------------------------------------------------------------------------
// DIALOG – GỬI ĐỒ
// ---------------------------------------------------------------------------

static stock FA_ShowDepositMenu(playerid)
{
    ShowPlayerDialog(playerid, DIALOG_SAFE_DEPOSIT, DIALOG_STYLE_LIST,
        "Gui Do Vao Ket",
        "Tien mat\nCan sa\nCrack\nCan sa (moi)\nTep can sa\nVat lieu",
        "Chon", "Huy");
}

// Per-player: loại item đang deposit/withdraw
new g_faDepositItem[MAX_PLAYERS];
new g_faWithdrawItem[MAX_PLAYERS];

static stock FA_ShowDepositAmount(playerid, itemType)
{
    g_faDepositItem[playerid] = itemType;
    new titles[6][] = {"Tien mat", "Can sa", "Crack", "Can sa (moi)", "Tep can sa", "Vat lieu"};
    new title[40];
    format(title, sizeof(title), "Gui: %s", titles[itemType]);
    ShowPlayerDialog(playerid, DIALOG_SAFE_DEPOSIT_AMT, DIALOG_STYLE_INPUT, title,
        "Nhap so luong:", "Gui", "Huy");
}

// ---------------------------------------------------------------------------
// DIALOG – RÚT ĐỒ
// ---------------------------------------------------------------------------

static stock FA_ShowWithdrawMenu(playerid)
{
    ShowPlayerDialog(playerid, DIALOG_SAFE_WITHDRAW, DIALOG_STYLE_LIST,
        "Rut Do Khoi Ket",
        "Tien mat\nCan sa\nCrack\nCan sa (moi)\nTep can sa\nVat lieu",
        "Chon", "Huy");
}

static stock FA_ShowWithdrawAmount(playerid, itemType)
{
    g_faWithdrawItem[playerid] = itemType;
    new titles[6][] = {"Tien mat", "Can sa", "Crack", "Can sa (moi)", "Tep can sa", "Vat lieu"};
    new title[40];
    format(title, sizeof(title), "Rut: %s", titles[itemType]);
    ShowPlayerDialog(playerid, DIALOG_SAFE_WITHDRAW_AMT, DIALOG_STYLE_INPUT, title,
        "Nhap so luong:", "Rut", "Huy");
}

// ---------------------------------------------------------------------------
// DIALOG – LỊCH SỬ
// ---------------------------------------------------------------------------

// Per-player offset cho paging
new g_faLogOffset[MAX_PLAYERS];

static stock FA_ShowLogs(playerid, famid)
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
        new pname[25], atype[12], itype[22], amt, ts;
        cache_get_value_name(i, "player_name", pname, sizeof(pname));
        cache_get_value_name(i, "action_type", atype, sizeof(atype));
        cache_get_value_name(i, "item_type",   itype, sizeof(itype));
        cache_get_value_name_int(i, "amount",  amt);
        cache_get_value_name_int(i, "timestamp", ts);

        new line[100];
        format(line, sizeof(line), "%s [%s] %s: %d\n",
            atype, itype, pname, amt);
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
    new list[512]; list[0] = 0;
    for(new s = 0; s < maxSlots; s++)
    {
        new line[48];
        new wid = FamilyInfo[famid][FamilyNewGuns][s];
        if(wid > 0)
        {
            new wname[24];
            GetWeaponName(wid, wname, sizeof(wname));
            format(line, sizeof(line), "Slot %d: %s (%d dan)\n", s+1, wname, FamilyInfo[famid][FamilyNewGunAmmo][s]);
        }
        else
            format(line, sizeof(line), "Slot %d: (Trong)\n", s+1);
        strcat(list, line, sizeof(list));
    }
    ShowPlayerDialog(playerid, DIALOG_SAFE_GUNS, DIALOG_STYLE_LIST, "Kho Vu Khi Gang", list, "Lay", "Huy");
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

    new str[128];
    format(str, sizeof(str),
        "Nang cap ket sac thanh cong! Level %d → %d. Suc chua tien: $%d | Hang: %d | VL: %d",
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
            // Tính offset listitem thực → rank cao hơn có nhiều option hơn
            switch(listitem)
            {
                case 0: FA_ShowSafeView(playerid, fam);
                case 1: FA_ShowDepositMenu(playerid);
                case 2:
                {
                    if(rank >= 4) FA_ShowWithdrawMenu(playerid);
                    else if(rank >= 5) FA_ShowLogs(playerid, fam); // slot lịch sử
                    else FA_ShowGunDialog(playerid, fam);          // slot súng
                }
                case 3:
                {
                    if(rank >= 5) FA_ShowLogs(playerid, fam);
                    else FA_ShowGunDialog(playerid, fam);
                }
                case 4: FA_ShowGunDialog(playerid, fam);
            }
            return 1;
        }

        // ---- Chọn loại để deposit ----
        case DIALOG_SAFE_DEPOSIT:
        {
            if(!response) return 1;
            FA_ShowDepositAmount(playerid, listitem);
            return 1;
        }

        // ---- Nhập số lượng deposit ----
        case DIALOG_SAFE_DEPOSIT_AMT:
        {
            if(!response) return 1;
            if(fam == INVALID_FAMILY_ID) return 1;
            new amount = strval(inputtext);
            if(amount <= 0) return SendClientMessageEx(playerid, COLOR_RED, "So luong khong hop le."), 1;

            new lv  = FamilyInfo[fam][FamilyLevel];
            new itm = g_faDepositItem[playerid];
            new itemNames[6][] = {"cash","pot","crack","cansa","tepcansa","mats"};

            switch(itm)
            {
                case 0: // Tiền
                {
                    new maxC = FA_GetMaxCash(lv);
                    if(GetPlayerCash(playerid) < amount) return SendClientMessageEx(playerid, COLOR_RED, "Khong du tien."), 1;
                    if(FamilyInfo[fam][FamilyCash] + amount > maxC)
                        return SendClientMessageEx(playerid, COLOR_RED, "Ket day tien. Nang cap de tang suc chua."), 1;
                    GivePlayerCash(playerid, -amount);
                    FamilyInfo[fam][FamilyCash] += amount;
                }
                case 1: // Pot
                {
                    if(PlayerInfo[playerid][pPot] < amount) return SendClientMessageEx(playerid, COLOR_RED, "Khong du can sa."), 1;
                    if(FamilyInfo[fam][FamilyPot] + amount > FA_GetMaxGoods(lv))
                        return SendClientMessageEx(playerid, COLOR_RED, "Ket day can sa."), 1;
                    PlayerInfo[playerid][pPot] -= amount;
                    FamilyInfo[fam][FamilyPot] += amount;
                }
                case 2: // Crack
                {
                    if(PlayerInfo[playerid][pCrack] < amount) return SendClientMessageEx(playerid, COLOR_RED, "Khong du crack."), 1;
                    if(FamilyInfo[fam][FamilyCrack] + amount > FA_GetMaxGoods(lv))
                        return SendClientMessageEx(playerid, COLOR_RED, "Ket day crack."), 1;
                    PlayerInfo[playerid][pCrack] -= amount;
                    FamilyInfo[fam][FamilyCrack] += amount;
                }
                case 3: // Cần sa mới
                {
                    if(PlayerInfo[playerid][pCanSa] < amount) return SendClientMessageEx(playerid, COLOR_RED, "Khong du can sa."), 1;
                    if(FamilyInfo[fam][FamilyCansa] + amount > FA_GetMaxGoods(lv))
                        return SendClientMessageEx(playerid, COLOR_RED, "Ket day can sa."), 1;
                    PlayerInfo[playerid][pCanSa] -= amount;
                    FamilyInfo[fam][FamilyCansa] += amount;
                }
                case 4: // Tép cần sa
                {
                    if(PlayerInfo[playerid][pTepCanSa] < amount) return SendClientMessageEx(playerid, COLOR_RED, "Khong du tep can sa."), 1;
                    if(FamilyInfo[fam][FamilyTepCansa] + amount > FA_GetMaxGoods(lv))
                        return SendClientMessageEx(playerid, COLOR_RED, "Ket day tep can sa."), 1;
                    PlayerInfo[playerid][pTepCanSa] -= amount;
                    FamilyInfo[fam][FamilyTepCansa] += amount;
                }
                case 5: // Vật liệu
                {
                    if(PlayerInfo[playerid][pMats] < amount) return SendClientMessageEx(playerid, COLOR_RED, "Khong du vat lieu."), 1;
                    if(FamilyInfo[fam][FamilyMats] + amount > FA_GetMaxMats(lv))
                        return SendClientMessageEx(playerid, COLOR_RED, "Ket day vat lieu."), 1;
                    PlayerInfo[playerid][pMats] -= amount;
                    FamilyInfo[fam][FamilyMats] += amount;
                }
            }

            FA_LogTransaction(fam, playerid, "deposit", itemNames[itm], amount);
            SaveFamily(fam);
            new str[64];
            format(str, sizeof(str), "Da gui %d vao ket sac gang.", amount);
            SendClientMessageEx(playerid, COLOR_GREEN, str);
            return 1;
        }

        // ---- Chọn loại để withdraw ----
        case DIALOG_SAFE_WITHDRAW:
        {
            if(!response) return 1;
            if(rank < 4) return SendClientMessageEx(playerid, COLOR_RED, "Can rank 4+ de rut do."), 1;
            FA_ShowWithdrawAmount(playerid, listitem);
            return 1;
        }

        // ---- Nhập số lượng withdraw ----
        case DIALOG_SAFE_WITHDRAW_AMT:
        {
            if(!response) return 1;
            if(rank < 4) return 1;
            if(fam == INVALID_FAMILY_ID) return 1;
            new amount = strval(inputtext);
            if(amount <= 0) return SendClientMessageEx(playerid, COLOR_RED, "So luong khong hop le."), 1;

            new itm = g_faWithdrawItem[playerid];
            new itemNames[6][] = {"cash","pot","crack","cansa","tepcansa","mats"};

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
                    if(FamilyInfo[fam][FamilyCansa] < amount) return SendClientMessageEx(playerid, COLOR_RED, "Ket khong du can sa."), 1;
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

            FA_LogTransaction(fam, playerid, "withdraw", itemNames[itm], amount);
            SaveFamily(fam);
            new str[64];
            format(str, sizeof(str), "Da rut %d tu ket sac gang.", amount);
            SendClientMessageEx(playerid, COLOR_GREEN, str);
            return 1;
        }

        // ---- Danh sách log ----
        case DIALOG_SAFE_LOGS: return 1;

        // ---- Gun dialog: lấy súng ----
        case DIALOG_SAFE_GUNS:
        {
            if(!response) return 1;
            if(fam == INVALID_FAMILY_ID) return 1;
            new lv       = FamilyInfo[fam][FamilyLevel];
            new maxSlots = FA_GetMaxGunSlots(lv);
            if(listitem < 0 || listitem >= maxSlots) return 1;

            new wid = FamilyInfo[fam][FamilyNewGuns][listitem];
            if(wid == 0) return SendClientMessageEx(playerid, COLOR_RED, "Slot nay trong."), 1;

            if(rank < 4) return SendClientMessageEx(playerid, COLOR_RED, "Can rank 4+ de lay vu khi."), 1;

            new ammo = FamilyInfo[fam][FamilyNewGunAmmo][listitem];
            GivePlayerWeapon(playerid, wid, ammo);
            FamilyInfo[fam][FamilyNewGuns][listitem]    = 0;
            FamilyInfo[fam][FamilyNewGunAmmo][listitem] = 0;
            FA_SaveGunSlot(fam, listitem);

            new wname[24];
            GetWeaponName(wid, wname, sizeof(wname));
            new str[64];
            format(str, sizeof(str), "Da lay %s (%d dan) tu ket sac.", wname, ammo);
            SendClientMessageEx(playerid, COLOR_GREEN, str);
            FA_LogTransaction(fam, playerid, "withdraw", "gun", ammo, wid);
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
    new fam = PlayerInfo[playerid][pFMember];
    if(fam == INVALID_FAMILY_ID) return 0;

    // Kiểm tra pickup là két sắt của gang
    if(pickupid == FamilyInfo[fam][FamilyPickup])
    {
        FA_OpenSafeDialog(playerid);
        return 1;
    }
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
