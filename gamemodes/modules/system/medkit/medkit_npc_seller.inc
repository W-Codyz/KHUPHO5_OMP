// ============================================================
//  NPC Ban Medkit - KP5 Roleplay Created by Codyz
//  Bac si nap medkit vao, nguoi choi mua ra
// ============================================================

#include <YSI\YSI_Coding\y_hooks>

// ============================================================
// DEFINES
// ============================================================
#define NPC_MEDKIT_X                149.878341
#define NPC_MEDKIT_Y                24.154684
#define NPC_MEDKIT_Z                3381.033203
#define NPC_MEDKIT_A                3.501515
#define NPC_MEDKIT_INT              1
#define NPC_MEDKIT_VW               0
#define NPC_MEDKIT_SKIN             276
#define NPC_MEDKIT_RANGE            3.0
#define NPC_MEDKIT_DEFAULT_PRICE    5000
#define NPC_MEDKIT_MAX_STOCK        9999
#define NPC_MEDKIT_MAX_LOG          50

#define DIALOG_NPCMK_BUY            5230
#define DIALOG_NPCMK_DOCTOR         5231
#define DIALOG_NPCMK_DEPOSIT        5232
#define DIALOG_NPCMK_TAKE           5233
#define DIALOG_NPCMK_HISTORY        5234
#define DIALOG_NPCMK_ADMIN          5235
#define DIALOG_NPCMK_ADMIN_PRICE    5236
#define DIALOG_NPCMK_ADMIN_STOCK    5237
#define DIALOG_NPCMK_MAIN           5238
#define DIALOG_NPCMK_SELL_HERB      5239
#define DIALOG_NPCMK_ADMIN_HPRICE   5240
#define DIALOG_NPCMK_ADMIN_HSTOCK   5241

#define NPC_HERB_DEFAULT_PRICE      800
#define NPC_HERB_MAX_STOCK          9999

// ============================================================
// VARIABLES
// ============================================================
new gNPCMedkitPrice                         = NPC_MEDKIT_DEFAULT_PRICE;
new gNPCMedkitStock                         = 0;
new bool:gNPCMedkitVisible                  = true;
new gNPCMedkitActor                         = INVALID_ACTOR_ID;
new Text3D:gNPCMedkitLabel                  = Text3D:INVALID_3DTEXT_ID;
new Text3D:gNPCMedkitSpeech                 = Text3D:INVALID_3DTEXT_ID;

new gNPCHerbPrice                           = NPC_HERB_DEFAULT_PRICE;
new gNPCHerbStock                           = 0;

new gNPCMedkitLog[NPC_MEDKIT_MAX_LOG][128];
new gNPCMedkitLogCount                      = 0;

// ============================================================
// SAVE / LOAD CONFIG
// ============================================================
stock SaveNPCMedkitConfig()
{
    new line[64], File:f = fopen("NPCMedkitSeller.cfg", io_write);
    if(f == File:0) return;
    format(line, sizeof(line), "%d|%d|%d|%d|%d\r\n", gNPCMedkitPrice, gNPCMedkitStock, _:gNPCMedkitVisible, gNPCHerbPrice, gNPCHerbStock);
    fwrite(f, line);
    fclose(f);
}

stock LoadNPCMedkitConfig()
{
    if(!fexist("NPCMedkitSeller.cfg")) { SaveNPCMedkitConfig(); return 1; }
    new line[64], File:f = fopen("NPCMedkitSeller.cfg", io_read);
    if(f == File:0) return 1;
    if(fread(f, line)) {
        new vis;
        sscanf(line, "p<|>ddddd", gNPCMedkitPrice, gNPCMedkitStock, vis, gNPCHerbPrice, gNPCHerbStock);
        gNPCMedkitVisible = bool:vis;
    }
    fclose(f);
    printf("[NPCMedkit] Loaded: price=%d stock=%d visible=%d", gNPCMedkitPrice, gNPCMedkitStock, _:gNPCMedkitVisible);
    return 1;
}

// ============================================================
// LOG
// ============================================================
stock LoadNPCMedkitLog()
{
    gNPCMedkitLogCount = 0;
    if(!fexist("NPCMedkitLog.cfg")) return 1;
    new File:f = fopen("NPCMedkitLog.cfg", io_read);
    if(f == File:0) return 1;
    new line[96];
    while(gNPCMedkitLogCount < NPC_MEDKIT_MAX_LOG && fread(f, line))
        strcopy(gNPCMedkitLog[gNPCMedkitLogCount++], line, 128);
    fclose(f);
    return 1;
}

static SaveNPCMedkitLog()
{
    new File:f = fopen("NPCMedkitLog.cfg", io_write);
    if(f == File:0) return;
    for(new i = 0; i < gNPCMedkitLogCount; i++)
        fwrite(f, gNPCMedkitLog[i]);
    fclose(f);
}

static AppendNPCMedkitLog(playerid, const action[], qty)
{
    new entry[128];
    new hh, mm, ss, day, month, year;
    gettime(hh, mm, ss);
    getdate(year, month, day);

    new color[8];
    if(!strcmp(action, "NAP", true))        strcopy(color, "FF6600", 8);
    else if(!strcmp(action, "LAY", true))  strcopy(color, "3399FF", 8);
    else if(!strcmp(action, "THUOC", true)) strcopy(color, "CC44FF", 8);
    else                                   strcopy(color, "00FF88", 8); // MUA

    format(entry, sizeof(entry), "{AAAAAA}[%02d/%02d/%04d %02d:%02d] {%s}[%s] {FFFFFF}%s {FFFF00}x%d\n",
        day, month, year, hh, mm, color, action, GetPlayerNameEx(playerid), qty);

    if(gNPCMedkitLogCount >= NPC_MEDKIT_MAX_LOG) {
        for(new i = 0; i < NPC_MEDKIT_MAX_LOG - 1; i++)
            strcopy(gNPCMedkitLog[i], gNPCMedkitLog[i + 1], 128);
        gNPCMedkitLogCount = NPC_MEDKIT_MAX_LOG - 1;
    }
    strcopy(gNPCMedkitLog[gNPCMedkitLogCount++], entry, 128);
    SaveNPCMedkitLog();
}

// ============================================================
// ACTOR & LABEL
// ============================================================
static UpdateNPCMedkitLabel()
{
    if(gNPCMedkitLabel != Text3D:INVALID_3DTEXT_ID) {
        DestroyDynamic3DTextLabel(gNPCMedkitLabel);
        gNPCMedkitLabel = Text3D:INVALID_3DTEXT_ID;
    }
    if(!gNPCMedkitVisible) return;
    new labelstr[256];
    format(labelstr, sizeof(labelstr),
        "{FF3300}[NPC Ban Medkit]\n{FFFFFF}Medkit: {FFFF00}$%d{FFFFFF}/cai - Con: {00FF88}%d\n{FFFFFF}Thu mua thao duoc: {CC44FF}$%d{FFFFFF}/cai - Co: {00FF88}%d\n{AAAAAA}Nhan {FFFF00}Y{AAAAAA} de tuong tac",
        gNPCMedkitPrice, gNPCMedkitStock, gNPCHerbPrice, gNPCHerbStock);
    gNPCMedkitLabel = CreateDynamic3DTextLabel(labelstr, 0xFFFFFFFF,
        NPC_MEDKIT_X, NPC_MEDKIT_Y, NPC_MEDKIT_Z + 0.2, 10.0,
        INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, NPC_MEDKIT_VW, NPC_MEDKIT_INT);
}

static SpawnNPCMedkitActor()
{
    if(gNPCMedkitActor != INVALID_ACTOR_ID) {
        DestroyActor(gNPCMedkitActor);
        gNPCMedkitActor = INVALID_ACTOR_ID;
    }
    if(gNPCMedkitSpeech != Text3D:INVALID_3DTEXT_ID) {
        DestroyDynamic3DTextLabel(gNPCMedkitSpeech);
        gNPCMedkitSpeech = Text3D:INVALID_3DTEXT_ID;
    }
    if(!gNPCMedkitVisible) return;
    gNPCMedkitSpeech = CreateDynamic3DTextLabel("<Bac si Thu Y>\nNha` toi co' ban' quan tai`, co' muon' dat hang` truoc' 1 cai' khong?", -1, NPC_MEDKIT_X, NPC_MEDKIT_Y, NPC_MEDKIT_Z + 1.3, 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, NPC_MEDKIT_VW, NPC_MEDKIT_INT);
    gNPCMedkitActor = CreateActor(NPC_MEDKIT_SKIN, NPC_MEDKIT_X, NPC_MEDKIT_Y, NPC_MEDKIT_Z, NPC_MEDKIT_A);
    SetActorVirtualWorld(gNPCMedkitActor, NPC_MEDKIT_VW);
}

stock RefreshNPCMedkit()
{
    SpawnNPCMedkitActor();
    UpdateNPCMedkitLabel();
}

// ============================================================
// HELPERS
// ============================================================
stock IsPlayerNearNPCMedkit(playerid)
{
    if(!gNPCMedkitVisible) return 0;
    if(GetPlayerVirtualWorld(playerid) != NPC_MEDKIT_VW) return 0;
    if(GetPlayerInterior(playerid) != NPC_MEDKIT_INT) return 0;
    return IsPlayerInRangeOfPoint(playerid, NPC_MEDKIT_RANGE, NPC_MEDKIT_X, NPC_MEDKIT_Y, NPC_MEDKIT_Z);
}

static ShowNPCMedkitMainDialog(playerid)
{
    new str[256];
    format(str, sizeof(str), "{FFFFFF}Mua Medkit {AAAAAA}(NPC con: {FFFF00}%d{AAAAAA}, gia {FFFF00}$%d{AAAAAA}/cai)\n{FFFFFF}Ban Thao duoc {AAAAAA}(NPC mua gia {CC44FF}$%d{AAAAAA}/cai)", gNPCMedkitStock, gNPCMedkitPrice, gNPCHerbPrice);
    ShowPlayerDialog(playerid, DIALOG_NPCMK_MAIN, DIALOG_STYLE_LIST,
        "{FF3300}[NPC Ban Medkit]{FFFFFF} - Giao dich", str, "Chon", "Dong");
}

static ShowNPCMedkitBuyDialog(playerid)
{
    new str[256];
    format(str, sizeof(str),
        "{ffffff}NPC con: {FFFF00}%d Medkit\n{ffffff}Gia: {FFFF00}$%d{ffffff}/cai\nSo du: {00FF88}$%d\n\n{AAAAAA}Nhap so luong muon mua:",
        gNPCMedkitStock, gNPCMedkitPrice, GetPlayerCash(playerid));
    ShowPlayerDialog(playerid, DIALOG_NPCMK_BUY, DIALOG_STYLE_INPUT,
        "{FF3300}[NPC Ban Medkit]{FFFFFF} - Mua Medkit", str, "Mua", "Dong");
}

static ShowNPCMedkitDoctorDialog(playerid)
{
    new str[192];
    format(str, sizeof(str), "{FF6600}[NAP]{FFFFFF} Nap Medkit vao NPC (NPC co: %d)\n{3399FF}[LAY]{FFFFFF} Lay Medkit tu NPC\n{FFFF00}[LICH SU]{FFFFFF} Xem lich su giao dich", gNPCMedkitStock);
    ShowPlayerDialog(playerid, DIALOG_NPCMK_DOCTOR, DIALOG_STYLE_LIST,
        "{FF3300}[NPC Medkit]{FFFFFF} - Bac si", str, "Chon", "Dong");
}

// ============================================================
// HOOKS
// ============================================================
hook OnGameModeInit()
{
    LoadNPCMedkitConfig();
    LoadNPCMedkitLog();
    RefreshNPCMedkit();
    return 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(!(newkeys & KEY_YES)) return 1;
    if(!IsPlayerNearNPCMedkit(playerid)) return 1;
    if(IsAMedic(playerid) || PlayerInfo[playerid][pAdmin] >= 1337)
        ShowNPCMedkitDoctorDialog(playerid);
    else
        ShowNPCMedkitMainDialog(playerid);
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid)
    {
        // ==================== MENU CHINH (nguoi choi) ====================
        case DIALOG_NPCMK_MAIN:
        {
            if(!response) return 1;
            switch(listitem)
            {
                case 0: ShowNPCMedkitBuyDialog(playerid);
                case 1: {
                    new str[256];
                    format(str, sizeof(str), "{ffffff}Thao duoc tren nguoi: {FFFF00}%d\n{ffffff}NPC mua gia: {CC44FF}$%d{ffffff}/cai\n\n{AAAAAA}Nhap so luong muon ban:", PlayerInfo[playerid][pHerb], gNPCHerbPrice);
                    ShowPlayerDialog(playerid, DIALOG_NPCMK_SELL_HERB, DIALOG_STYLE_INPUT,
                        "{FF3300}[NPC Ban Medkit]{FFFFFF} - Ban Thao Duoc", str, "Ban", "Quay lai");
                }
            }
        }

        // ==================== MUA (nguoi choi) ====================
        case DIALOG_NPCMK_BUY:
        {
            if(!response) return 1;
            if(strfind(inputtext, "%", true) != -1) return 1;
            new qty = strval(inputtext);
            if(qty < 1)
                return SendClientMessageEx(playerid, COLOR_GREY, "So luong khong hop le!");
            if(qty > gNPCMedkitStock) {
                new str[128];
                format(str, sizeof(str), "NPC chi con {FFFF00}%d Medkit{FFFFFF}!", gNPCMedkitStock);
                return SendClientMessageEx(playerid, COLOR_GREY, str);
            }
            new totalCost = qty * gNPCMedkitPrice;
            if(GetPlayerCash(playerid) < totalCost) {
                new str[128];
                format(str, sizeof(str), "Ban khong du tien! Can {FFFF00}$%d{FFFFFF}.", totalCost);
                return SendClientMessageEx(playerid, COLOR_GREY, str);
            }
            GivePlayerCash(playerid, -totalCost);
            PlayerInfo[playerid][pMedkit] += qty;
            gNPCMedkitStock -= qty;
            SaveNPCMedkitConfig();
            UpdateNPCMedkitLabel();
            AppendNPCMedkitLog(playerid, "MUA", qty);
            new str[128];
            format(str, sizeof(str), "{ff3300}[NPC Medkit]{ffffff} Mua thanh cong {FFFF00}%d Medkit{ffffff} (-{FFFF00}$%d{ffffff}).", qty, totalCost);
            SendClientMessageEx(playerid, COLOR_WHITE, str);
        }

        // ==================== DOCTOR MENU ====================
        case DIALOG_NPCMK_DOCTOR:
        {
            if(!response) return 1;
            switch(listitem)
            {
                case 0: {
                    new str[192];
                    format(str, sizeof(str),
                        "{ffffff}Medkit tren nguoi: {FFFF00}%d\n{ffffff}NPC dang co: {FFFF00}%d\n\n{AAAAAA}Nhap so luong muon nap vao NPC:",
                        PlayerInfo[playerid][pMedkit], gNPCMedkitStock);
                    ShowPlayerDialog(playerid, DIALOG_NPCMK_DEPOSIT, DIALOG_STYLE_INPUT,
                        "{FF3300}[NPC Medkit]{FFFFFF} - Nap Medkit", str, "Nap", "Quay lai");
                }
                case 1: {
                    new str[192];
                    format(str, sizeof(str),
                        "{ffffff}NPC dang co: {FFFF00}%d Medkit\n{ffffff}Tren nguoi: {FFFF00}%d\n\n{AAAAAA}Nhap so luong muon lay:",
                        gNPCMedkitStock, PlayerInfo[playerid][pMedkit]);
                    ShowPlayerDialog(playerid, DIALOG_NPCMK_TAKE, DIALOG_STYLE_INPUT,
                        "{FF3300}[NPC Medkit]{FFFFFF} - Lay Medkit", str, "Lay", "Quay lai");
                }
                case 2: {
                    new str[4096];
                    for(new i = gNPCMedkitLogCount - 1; i >= 0; i--)
                        strcat(str, gNPCMedkitLog[i], sizeof(str));
                    if(!gNPCMedkitLogCount) strcat(str, "{AAAAAA}Chua co giao dich nao.", sizeof(str));
                    ShowPlayerDialog(playerid, DIALOG_NPCMK_HISTORY, DIALOG_STYLE_MSGBOX,
                        "{FFFF00}LICH SU GIAO DICH MEDKIT", str, "Dong", "");
                }
            }
        }

        // ==================== NAP (bac si) ====================
        case DIALOG_NPCMK_DEPOSIT:
        {
            if(!response) { ShowNPCMedkitDoctorDialog(playerid); return 1; }
            if(strfind(inputtext, "%", true) != -1) return 1;
            new qty = strval(inputtext);
            if(qty < 1)
                return SendClientMessageEx(playerid, COLOR_GREY, "So luong khong hop le!");
            if(qty > PlayerInfo[playerid][pMedkit]) {
                new str[128];
                format(str, sizeof(str), "Ban chi co {FFFF00}%d Medkit{FFFFFF} tren nguoi!", PlayerInfo[playerid][pMedkit]);
                return SendClientMessageEx(playerid, COLOR_GREY, str);
            }
            if(gNPCMedkitStock + qty > NPC_MEDKIT_MAX_STOCK) {
                new str[128];
                format(str, sizeof(str), "NPC chi chua duoc toi da {FFFF00}%d Medkit{FFFFFF}!", NPC_MEDKIT_MAX_STOCK);
                return SendClientMessageEx(playerid, COLOR_GREY, str);
            }
            PlayerInfo[playerid][pMedkit] -= qty;
            gNPCMedkitStock += qty;
            SaveNPCMedkitConfig();
            UpdateNPCMedkitLabel();
            AppendNPCMedkitLog(playerid, "NAP", qty);
            new str[128];
            format(str, sizeof(str), "{ff3300}[NPC Medkit]{ffffff} Da nap {FFFF00}%d Medkit{ffffff}. NPC con: {FFFF00}%d{ffffff}.", qty, gNPCMedkitStock);
            SendClientMessageEx(playerid, COLOR_WHITE, str);
        }

        // ==================== LAY (bac si) ====================
        case DIALOG_NPCMK_TAKE:
        {
            if(!response) { ShowNPCMedkitDoctorDialog(playerid); return 1; }
            if(strfind(inputtext, "%", true) != -1) return 1;
            new qty = strval(inputtext);
            if(qty < 1)
                return SendClientMessageEx(playerid, COLOR_GREY, "So luong khong hop le!");
            if(qty > gNPCMedkitStock) {
                new str[128];
                format(str, sizeof(str), "NPC chi con {FFFF00}%d Medkit{FFFFFF}!", gNPCMedkitStock);
                return SendClientMessageEx(playerid, COLOR_GREY, str);
            }
            PlayerInfo[playerid][pMedkit] += qty;
            gNPCMedkitStock -= qty;
            SaveNPCMedkitConfig();
            UpdateNPCMedkitLabel();
            AppendNPCMedkitLog(playerid, "LAY", qty);
            new str[128];
            format(str, sizeof(str), "{ff3300}[NPC Medkit]{ffffff} Da lay {FFFF00}%d Medkit{ffffff}. NPC con: {FFFF00}%d{ffffff}.", qty, gNPCMedkitStock);
            SendClientMessageEx(playerid, COLOR_WHITE, str);
        }

        // ==================== BAN THAO DUOC (nguoi choi) ====================
        case DIALOG_NPCMK_SELL_HERB:
        {
            if(!response) { ShowNPCMedkitMainDialog(playerid); return 1; }
            if(strfind(inputtext, "%", true) != -1) return 1;
            new qty = strval(inputtext);
            if(qty < 1)
                return SendClientMessageEx(playerid, COLOR_GREY, "So luong khong hop le!");
            if(qty > PlayerInfo[playerid][pHerb]) {
                new str[128];
                format(str, sizeof(str), "Ban chi co {FFFF00}%d thao duoc{FFFFFF}!", PlayerInfo[playerid][pHerb]);
                return SendClientMessageEx(playerid, COLOR_GREY, str);
            }
            if(gNPCHerbStock + qty > NPC_HERB_MAX_STOCK) {
                new str[128];
                format(str, sizeof(str), "NPC chi cho duoc them {FFFF00}%d thao duoc{FFFFFF}!", NPC_HERB_MAX_STOCK - gNPCHerbStock);
                return SendClientMessageEx(playerid, COLOR_GREY, str);
            }
            new totalEarn = qty * gNPCHerbPrice;
            PlayerInfo[playerid][pHerb] -= qty;
            gNPCHerbStock += qty;
            GivePlayerCash(playerid, totalEarn);
            SaveNPCMedkitConfig();
            UpdateNPCMedkitLabel();
            AppendNPCMedkitLog(playerid, "THUOC", qty);

            new str[128];
            format(str, sizeof(str), "{ff3300}[NPC Medkit]{ffffff} Ban thanh cong {FFFF00}%d thao duoc{ffffff} (+{FFFF00}$%d{ffffff}).", qty, totalEarn);
            SendClientMessageEx(playerid, COLOR_WHITE, str);

            new dtitle[128], dlog[256];
            format(dtitle, sizeof(dtitle), "%s (ID: %d) - Ban thao duoc cho NPC", GetPlayerNameEx(playerid), playerid);
            format(dlog, sizeof(dlog), ":herb: **BAN THAO DUOC**\n:moneybag: **Nhan: $%d**\n:package: **So luong: x%d**\n:dollar: **Gia/cai: $%d**", totalEarn, qty, gNPCHerbPrice);
            DiscordLog(dclog_Trucker, dtitle, dlog);
        }

        // ==================== ADMIN MENU ====================
        case DIALOG_NPCMK_ADMIN:
        {
            if(!response) return 1;
            if(PlayerInfo[playerid][pAdmin] < 1337) return 1;
            switch(listitem)
            {
                case 0: {
                    new str[128];
                    format(str, sizeof(str), "{ffffff}Gia hien tai: {FFFF00}$%d\n\n{AAAAAA}Nhap gia ban moi:", gNPCMedkitPrice);
                    ShowPlayerDialog(playerid, DIALOG_NPCMK_ADMIN_PRICE, DIALOG_STYLE_INPUT,
                        "{FFFF00}Doi Gia Ban Medkit", str, "Luu", "Quay lai");
                }
                case 1: {
                    new str[128];
                    format(str, sizeof(str), "{ffffff}So luong hien tai: {FFFF00}%d\n\n{AAAAAA}Nhap so luong kho moi (0 - %d):", gNPCMedkitStock, NPC_MEDKIT_MAX_STOCK);
                    ShowPlayerDialog(playerid, DIALOG_NPCMK_ADMIN_STOCK, DIALOG_STYLE_INPUT,
                        "{FFFF00}Dat So Luong Kho NPC", str, "Luu", "Quay lai");
                }
                case 2: {
                    new str[128];
                    format(str, sizeof(str), "{ffffff}Gia thu mua hien tai: {CC44FF}$%d{ffffff}/cai\n\n{AAAAAA}Nhap gia thu mua thao duoc moi:", gNPCHerbPrice);
                    ShowPlayerDialog(playerid, DIALOG_NPCMK_ADMIN_HPRICE, DIALOG_STYLE_INPUT,
                        "{FFFF00}Doi Gia Thu Mua Thao Duoc", str, "Luu", "Quay lai");
                }
                case 3: {
                    new str[128];
                    format(str, sizeof(str), "{ffffff}Kho thao duoc hien tai: {FFFF00}%d\n\n{AAAAAA}Nhap so luong kho thao duoc moi (0 - %d):", gNPCHerbStock, NPC_HERB_MAX_STOCK);
                    ShowPlayerDialog(playerid, DIALOG_NPCMK_ADMIN_HSTOCK, DIALOG_STYLE_INPUT,
                        "{FFFF00}Dat Kho Thao Duoc NPC", str, "Luu", "Quay lai");
                }
                case 4: {
                    gNPCMedkitVisible = !gNPCMedkitVisible;
                    SaveNPCMedkitConfig();
                    RefreshNPCMedkit();
                    new str[64];
                    format(str, sizeof(str), "NPC Medkit: {FFFF00}%s{FFFFFF}.", gNPCMedkitVisible ? "Dang hien" : "Dang an");
                    SendClientMessageEx(playerid, COLOR_WHITE, str);
                    PC_EmulateCommand(playerid, "/anpcmedkit");
                }
            }
        }

        case DIALOG_NPCMK_ADMIN_PRICE:
        {
            if(!response) { PC_EmulateCommand(playerid, "/anpcmedkit"); return 1; }
            if(PlayerInfo[playerid][pAdmin] < 1337) return 1;
            if(strfind(inputtext, "%", true) != -1) return 1;
            new price = strval(inputtext);
            if(price < 1)
                return SendClientMessageEx(playerid, COLOR_GREY, "Gia khong hop le! (toi thieu $1)");
            gNPCMedkitPrice = price;
            SaveNPCMedkitConfig();
            UpdateNPCMedkitLabel();
            new str[128];
            format(str, sizeof(str), "Da doi gia medkit thanh {FFFF00}$%d{FFFFFF}/cai.", gNPCMedkitPrice);
            SendClientMessageEx(playerid, COLOR_WHITE, str);
            PC_EmulateCommand(playerid, "/anpcmedkit");
        }

        case DIALOG_NPCMK_ADMIN_STOCK:
        {
            if(!response) { PC_EmulateCommand(playerid, "/anpcmedkit"); return 1; }
            if(PlayerInfo[playerid][pAdmin] < 1337) return 1;
            if(strfind(inputtext, "%", true) != -1) return 1;
            new stock_qty = strval(inputtext);
            if(stock_qty < 0 || stock_qty > NPC_MEDKIT_MAX_STOCK) {
                new str[96];
                format(str, sizeof(str), "So luong khong hop le! (0 - %d)", NPC_MEDKIT_MAX_STOCK);
                return SendClientMessageEx(playerid, COLOR_GREY, str);
            }
            gNPCMedkitStock = stock_qty;
            SaveNPCMedkitConfig();
            UpdateNPCMedkitLabel();
            new str[128];
            format(str, sizeof(str), "Da dat kho NPC Medkit thanh {FFFF00}%d{FFFFFF}.", gNPCMedkitStock);
            SendClientMessageEx(playerid, COLOR_WHITE, str);
            PC_EmulateCommand(playerid, "/anpcmedkit");
        }

        case DIALOG_NPCMK_ADMIN_HPRICE:
        {
            if(!response) { PC_EmulateCommand(playerid, "/anpcmedkit"); return 1; }
            if(PlayerInfo[playerid][pAdmin] < 1337) return 1;
            if(strfind(inputtext, "%", true) != -1) return 1;
            new price = strval(inputtext);
            if(price < 1)
                return SendClientMessageEx(playerid, COLOR_GREY, "Gia khong hop le! (toi thieu $1)");
            gNPCHerbPrice = price;
            SaveNPCMedkitConfig();
            UpdateNPCMedkitLabel();
            new str[128];
            format(str, sizeof(str), "Da doi gia thu mua thao duoc thanh {CC44FF}$%d{FFFFFF}/cai.", gNPCHerbPrice);
            SendClientMessageEx(playerid, COLOR_WHITE, str);
            PC_EmulateCommand(playerid, "/anpcmedkit");
        }

        case DIALOG_NPCMK_ADMIN_HSTOCK:
        {
            if(!response) { PC_EmulateCommand(playerid, "/anpcmedkit"); return 1; }
            if(PlayerInfo[playerid][pAdmin] < 1337) return 1;
            if(strfind(inputtext, "%", true) != -1) return 1;
            new hstock = strval(inputtext);
            if(hstock < 0 || hstock > NPC_HERB_MAX_STOCK) {
                new str[96];
                format(str, sizeof(str), "So luong khong hop le! (0 - %d)", NPC_HERB_MAX_STOCK);
                return SendClientMessageEx(playerid, COLOR_GREY, str);
            }
            gNPCHerbStock = hstock;
            SaveNPCMedkitConfig();
            UpdateNPCMedkitLabel();
            new str[128];
            format(str, sizeof(str), "Da dat kho thao duoc NPC thanh {FFFF00}%d{FFFFFF}.", gNPCHerbStock);
            SendClientMessageEx(playerid, COLOR_WHITE, str);
            PC_EmulateCommand(playerid, "/anpcmedkit");
        }
    }
    return 1;
}

// ============================================================
// COMMANDS
// ============================================================
CMD:anpcmedkit(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1337)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong co quyen su dung lenh nay!");

    new str[384];
    format(str, sizeof(str), "{FFFF00}Doi gia ban medkit{FFFFFF} (hien: {FFFF00}$%d{FFFFFF})\n{FFFF00}Dat kho medkit{FFFFFF} (hien: {FFFF00}%d{FFFFFF})\n{FFFF00}Doi gia thu mua thao duoc{FFFFFF} (hien: {CC44FF}$%d{FFFFFF})\n{FFFF00}Dat kho thao duoc{FFFFFF} (hien: {FFFF00}%d{FFFFFF})\n{FFFF00}%s NPC", gNPCMedkitPrice, gNPCMedkitStock, gNPCHerbPrice, gNPCHerbStock, gNPCMedkitVisible ? "An" : "Hien");
    ShowPlayerDialog(playerid, DIALOG_NPCMK_ADMIN, DIALOG_STYLE_LIST,
        "{FFFF00}Admin - NPC Ban Medkit", str, "Chon", "Dong");
    return 1;
}
