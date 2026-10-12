// ============================================================
//  Medkit Crafting & Usage System - GTA-HTB Roleplay Created by Codyz
//  Che tao medkit (Medic only) + Su dung medkit (all players)
// ============================================================

#include <YSI\YSI_Coding\y_hooks>

// ============================================================
// DEFINES
// ============================================================
#define MAX_MEDKIT_CRAFT_SPOTS      10
#define MEDKIT_CRAFT_HERBS          5       // 5 thao duoc = 1 medkit
#define MEDKIT_CRAFT_TIME_MIN       10      // 10s
#define MEDKIT_CRAFT_TIME_MAX       15      // 15s
#define MEDKIT_USE_TIME             10      // 10s su dung
#define MEDKIT_COOLDOWN             120     // 2 phut cooldown
#define MEDKIT_HEAL_AMOUNT          5.0     // Hoi 5 mau moi lan
#define MEDKIT_HEAL_TIMES           20      // 20 lan x 5 = 100 mau
#define MEDKIT_USE_MIN_HP           80.0    // Chi dung khi mau duoi 80

// Dialog IDs
#define DIALOG_MEDKIT_CRAFT         15010
#define DIALOG_MEDKIT_CRAFT_QTY     15011
#define DIALOG_ADMIN_MEDKIT_MENU    15012
#define DIALOG_ADMIN_MEDKIT_ADD_GRP 15013
#define DIALOG_ADMIN_MEDKIT_DEL     15014
#define DIALOG_ADMIN_MEDKIT_SETPOS  15015

// ============================================================
// VARIABLES - Craft Spots
// ============================================================
new Float:gMedCraftX[MAX_MEDKIT_CRAFT_SPOTS];
new Float:gMedCraftY[MAX_MEDKIT_CRAFT_SPOTS];
new Float:gMedCraftZ[MAX_MEDKIT_CRAFT_SPOTS];
new Text3D:gMedCraftLabel[MAX_MEDKIT_CRAFT_SPOTS] = {Text3D:INVALID_3DTEXT_ID, ...};
new gMedCraftPickup[MAX_MEDKIT_CRAFT_SPOTS] = {-1, ...};
new gMedCraftCount = 0;

// Player state
new PlayerMedkitCooldown[MAX_PLAYERS];

// ============================================================
// FORWARDS
// ============================================================
forward Medkit_CraftDone(playerid);
forward Medkit_FinishUse(playerid);
forward Medkit_HealTick(playerid, Float:amount, times);

// ============================================================
// HELPERS - 3DTextLabel & Pickup
// ============================================================
static CreateMedCraftWorldObjects(i)
{
    gMedCraftLabel[i] = CreateDynamic3DTextLabel("{FF3300}[Phong Thi Nghiem Thuoc]\n{FFFFFF}Lenh: {F6F08D}/chetaomedkit", 0xFFFFFFFF,
        gMedCraftX[i], gMedCraftY[i], gMedCraftZ[i] + 0.5, 20.0);
    gMedCraftPickup[i] = CreateDynamicPickup(1239, 23,
        gMedCraftX[i], gMedCraftY[i], gMedCraftZ[i]);
}

static DestroyMedCraftWorldObjects(i)
{
    if(gMedCraftLabel[i] != Text3D:INVALID_3DTEXT_ID) {
        DestroyDynamic3DTextLabel(gMedCraftLabel[i]);
        gMedCraftLabel[i] = Text3D:INVALID_3DTEXT_ID;
    }
    if(gMedCraftPickup[i] != -1) {
        DestroyDynamicPickup(gMedCraftPickup[i]);
        gMedCraftPickup[i] = -1;
    }
}

stock IsPlayerNearMedCraftSpot(playerid)
{
    for(new i = 0; i < gMedCraftCount; i++) {
        if(IsPlayerInRangeOfPoint(playerid, 5.0, gMedCraftX[i], gMedCraftY[i], gMedCraftZ[i]))
            return 1;
    }
    return 0;
}

// ============================================================
// SAVE / LOAD - Craft Spots (file-based)
// ============================================================
stock SaveMedkitCraftSpots()
{
    new line[96], File:f = fopen("MedkitCraftSpots.cfg", io_write);
    if(f == File:0) return;
    for(new i = 0; i < gMedCraftCount; i++) {
        format(line, sizeof(line), "%f|%f|%f\r\n",
            gMedCraftX[i], gMedCraftY[i], gMedCraftZ[i]);
        fwrite(f, line);
    }
    fclose(f);
}

stock LoadMedkitCraftSpots()
{
    for(new i = 0; i < gMedCraftCount; i++) DestroyMedCraftWorldObjects(i);
    gMedCraftCount = 0;
    if(!fexist("MedkitCraftSpots.cfg")) { SaveMedkitCraftSpots(); return 1; }
    new line[96], File:f = fopen("MedkitCraftSpots.cfg", io_read);
    if(f == File:0) return 1;
    while(gMedCraftCount < MAX_MEDKIT_CRAFT_SPOTS && fread(f, line)) {
        if(!sscanf(line, "p<|>fff",
            gMedCraftX[gMedCraftCount], gMedCraftY[gMedCraftCount], gMedCraftZ[gMedCraftCount]))
        {
            CreateMedCraftWorldObjects(gMedCraftCount);
            gMedCraftCount++;
        }
    }
    fclose(f);
    printf("[MedkitSystem] %d phong thi nghiem thuoc da load.", gMedCraftCount);
    return 1;
}

// ============================================================
// INIT - Goi trong OnGameModeInit (sau khi groups loaded)
// ============================================================
Medkit_Init()
{
    LoadMedkitCraftSpots();
    return 1;
}

// ============================================================
// COMMAND: /chetaomedkit - Chi Medic
// ============================================================
CMD:chetaomedkit(playerid, params[])
{
    if(!IsAMedic(playerid))
        return SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban khong phai la nhan vien Y te.");

    if(GetPVarInt(playerid, "MedkitBusy") == 1)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban dang ban, khong the thuc hien.");

    if(!IsPlayerNearMedCraftSpot(playerid))
        return SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban phai o gan Phong Thi Nghiem Thuoc!");

    new str[512];
    format(str, sizeof(str), "{ff3300}Los Santos Medic Department{FFFFFF}\n\n - Thao duoc hien co: {FFFF00}%d{FFFFFF}\n - Chi phi: {FFFF00}%d thao duoc{FFFFFF} / 1 Medkit\n - Chuc nang: Hoi phuc suc khoe nguoi su dung\n\nNhap so luong Medkit muon che tao:", PlayerInfo[playerid][pHerb], MEDKIT_CRAFT_HERBS);

    ShowPlayerDialog(playerid, DIALOG_MEDKIT_CRAFT, DIALOG_STYLE_INPUT,
        "{ffffff}Che Tao Medkit", str, "Che tao", "Huy");
    return 1;
}

// ============================================================
// CALLBACK: Craft Done
// ============================================================
public Medkit_CraftDone(playerid)
{
    if(!IsPlayerConnected(playerid)) return 0;
    SetPVarInt(playerid, "MedkitBusy", 0);
    ClearAnimations(playerid);
    TogglePlayerControllable(playerid, true);

    new qty = GetPVarInt(playerid, "MedkitCraftQty");
    PlayerInfo[playerid][pMedkit] += qty;

    new str[128];
    format(str, sizeof(str), "{ff3300}[Medkit] {ffffff}Ban da che tao thanh cong {FFFF00}%d Medkit{ffffff}. (Tong: %d)", qty, PlayerInfo[playerid][pMedkit]);
    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, str);
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Che tao Medkit thanh cong!");

    new proxstr[128];
    format(proxstr, sizeof(proxstr), "{FF8000}* {C2A2DA}%s da che tao xong %d Medkit.", GetPlayerNameEx(playerid), qty);
    ProxDetector(30.0, playerid, proxstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
    return 1;
}

// ============================================================
// COMMAND: /usemedkit - Tat ca player
// ============================================================
CMD:usemedkit(playerid, params[])
{
    if(PlayerInfo[playerid][pMedkit] <= 0)
        return SendClientMessageEx(playerid, COLOR_WHITE, "Ban khong co Medkit trong nguoi.");

    if(PlayerCuffed[playerid] >= 1 || GetPVarInt(playerid, "Injured") == 1)
        return SendClientMessageEx(playerid, COLOR_WHITE, "Ban khong the lam dieu nay bay gio.");

    if(GetPVarInt(playerid, "MedkitUsing") == 1)
        return SendClientMessageEx(playerid, COLOR_WHITE, "Ban dang su dung Medkit.");

    if(GetPVarInt(playerid, "MedkitHealing") == 1)
        return SendClientMessageEx(playerid, COLOR_WHITE, "Ban dang trong qua trinh hoi mau.");

    // Kiem tra cooldown
    if(gettime() < PlayerMedkitCooldown[playerid])
    {
        new timeleft = PlayerMedkitCooldown[playerid] - gettime();
        new str[128];
        format(str, sizeof(str), "Ban phai doi %d giay nua moi co the su dung medkit tiep.", timeleft);
        SendClientMessageEx(playerid, COLOR_WHITE, str);
        return 1;
    }

    // Kiem tra mau hien tai
    new Float:health;
    GetPlayerHealth(playerid, health);
    if(health > MEDKIT_USE_MIN_HP)
    {
        new str[128];
        format(str, sizeof(str), "Ban chi co the su dung medkit khi mau duoi %.0f!", MEDKIT_USE_MIN_HP);
        SendClientMessageEx(playerid, COLOR_WHITE, str);
        return 1;
    }

    // Bat dau su dung medkit
    SetPVarInt(playerid, "MedkitUsing", 1);
    PlayerMedkitCooldown[playerid] = gettime() + MEDKIT_COOLDOWN;
    new string[128];
    format(string, sizeof(string), "{C2A2DA}* %s dang su dung Medkit de hoi phuc suc khoe.", GetPlayerNameEx(playerid));
    ProxDetector(30.0, playerid, string, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

    ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.0, 1, 0, 0, 0, 0, 1);
    SetPlayerAttachedObject(playerid, 0, 11738, 6, 0.0, 0.0, 0.0);
    SetLoading(playerid, MEDKIT_USE_TIME * 1000, "Dang su dung medkit...", "Medkit_FinishUse");
    return 1;
}

// ============================================================
// CALLBACK: Finish Use - bat dau hoi mau
// ============================================================
public Medkit_FinishUse(playerid)
{
    if(!IsPlayerConnected(playerid)) return 1;

    // Kiem tra van dang su dung
    if(GetPVarInt(playerid, "MedkitUsing") != 1) return 1;

    ClearAnimations(playerid);
    RemovePlayerAttachedObject(playerid, 0);
    PlayerInfo[playerid][pMedkit]--;
    DeletePVar(playerid, "MedkitUsing");

    // Bat dau hoi mau tu tu
    SetPVarInt(playerid, "MedkitHealing", 1);
    Medkit_HealTick(playerid, MEDKIT_HEAL_AMOUNT, MEDKIT_HEAL_TIMES);

    HienTextdraw(playerid, "~g~Dang phuc suc khoe...");
    return 1;
}

// ============================================================
// HOI MAU TU TU - 5 HP moi giay x 20 lan = 100 HP
// ============================================================
public Medkit_HealTick(playerid, Float:amount, times)
{
    // Safety check
    if(!IsPlayerConnected(playerid))
    {
        DeletePVar(playerid, "MedkitHealing");
        return 1;
    }

    if(times <= 0)
    {
        DeletePVar(playerid, "MedkitHealing");
        SendClientMessageEx(playerid, COLOR_WHITE, "{ff3300}[Medkit] {ffffff}Su dung Medkit thanh cong! Da hoi phuc day du suc khoe.");
        ShowTDN(playerid, NOTIFICATION_SUCCESS, "Hoi mau hoan tat.");
        return 1;
    }

    new Float:health;
    GetPlayerHealth(playerid, health);
    if(health < 100.0)
    {
        new Float:newHP = health + amount;
        if(newHP > 100.0) newHP = 100.0;
        SetPlayerHealth(playerid, newHP);
    }

    SetTimerEx("Medkit_HealTick", 1000, false, "ifi", playerid, amount, times - 1);
    return 1;
}

// ============================================================
// HOOKS
// ============================================================
hook OnPlayerConnect(playerid)
{
    PlayerMedkitCooldown[playerid] = 0;
    DeletePVar(playerid, "MedkitUsing");
    DeletePVar(playerid, "MedkitHealing");
    DeletePVar(playerid, "MedkitBusy");
    return 1;
}

hook OnPlayerDeath(playerid, killerid, WEAPON:reason)
{
    // Reset trang thai khi chet
    DeletePVar(playerid, "MedkitUsing");
    DeletePVar(playerid, "MedkitHealing");
    DeletePVar(playerid, "MedkitBusy");
    return 1;
}

// ============================================================
// DIALOG RESPONSES
// ============================================================
hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid)
    {
        // ==================== CHE TAO MEDKIT ====================
        case DIALOG_MEDKIT_CRAFT:
        {
            if(!response) return 1;

            // Validate input
            if(strfind(inputtext, "%", true) != -1 || strfind(inputtext, "c", true) != -1)
                return PC_EmulateCommand(playerid, "/chetaomedkit");

            new qty = strval(inputtext);
            if(qty < 1 || qty > 100)
            {
                SendClientMessageEx(playerid, COLOR_GREY, "So luong khong hop le! (1-100)");
                return 1;
            }

            // Kiem tra van o gan xuong
            if(!IsPlayerNearMedCraftSpot(playerid))
            {
                SendClientMessageEx(playerid, COLOR_GREY, "Ban da roi khoi xuong che tao!");
                return 1;
            }

            // Kiem tra thao duoc
            new herbCost = qty * MEDKIT_CRAFT_HERBS;
            if(PlayerInfo[playerid][pHerb] < herbCost)
            {
                new str[128];
                format(str, sizeof(str), "Ban can %d thao duoc de che tao %d Medkit. Hien co: %d.",
                    herbCost, qty, PlayerInfo[playerid][pHerb]);
                SendClientMessageEx(playerid, COLOR_GREY, str);
                return 1;
            }

            // Tru nguyen lieu
            PlayerInfo[playerid][pHerb] -= herbCost;

            // Bat dau che tao
            SetPVarInt(playerid, "MedkitBusy", 1);
            SetPVarInt(playerid, "MedkitCraftQty", qty);
            TogglePlayerControllable(playerid, false);

            new craftTime = MEDKIT_CRAFT_TIME_MIN + random(MEDKIT_CRAFT_TIME_MAX - MEDKIT_CRAFT_TIME_MIN + 1);
            ApplyAnimation(playerid, "HEIST9", "Use_SwipeCard", 4.1, true, false, false, true, 0, SYNC_ALL);
            SetLoading(playerid, craftTime * 1000, "Dang che tao Medkit...", "Medkit_CraftDone");

            new str[128];
            format(str, sizeof(str), "{ff3300}[Medkit] {ffffff}Dang che tao %d Medkit (-%d thao duoc)... Vui long doi.", qty, herbCost);
            SendClientMessageEx(playerid, COLOR_LIGHTBLUE, str);

            new proxstr[128];
            format(proxstr, sizeof(proxstr), "{FF8000}* {C2A2DA}%s bat dau che tao Medkit.", GetPlayerNameEx(playerid));
            ProxDetector(30.0, playerid, proxstr, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
        }

        // ==================== ADMIN: MENU ====================
        case DIALOG_ADMIN_MEDKIT_MENU:
        {
            if(!response) return 1;
            if(PlayerInfo[playerid][pAdmin] < 1337) return 1;

            switch(listitem)
            {
                case 0: // Them phong
                {
                    if(gMedCraftCount >= MAX_MEDKIT_CRAFT_SPOTS)
                        return SendClientMessageEx(playerid, COLOR_GREY, "Da dat so luong vi tri toi da!");

                    new idx = gMedCraftCount;
                    GetPlayerPos(playerid, gMedCraftX[idx], gMedCraftY[idx], gMedCraftZ[idx]);
                    CreateMedCraftWorldObjects(idx);
                    gMedCraftCount++;
                    SaveMedkitCraftSpots();

                    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "Da them Phong Thi Nghiem Thuoc tai vi tri cua ban.");
                }
                case 1: // Xoa phong
                {
                    new string[1024], line[64];
                    strcat(string, "{FFD700}ID\t{FFFFFF}Toa do\n", sizeof(string));
                    for(new i = 0; i < gMedCraftCount; i++) {
                        format(line, sizeof(line), "{FFD700}%d\t{FFFFFF}%.1f, %.1f, %.1f\n",
                            i, gMedCraftX[i], gMedCraftY[i], gMedCraftZ[i]);
                        strcat(string, line, sizeof(string));
                    }
                    if(!gMedCraftCount) strcat(string, "{AAAAAA}Chua co phong nao.", sizeof(string));
                    ShowPlayerDialog(playerid, DIALOG_ADMIN_MEDKIT_DEL, DIALOG_STYLE_TABLIST_HEADERS,
                        "{FFFF00}XOA PHONG THI NGHIEM THUOC", string, "Xoa", "Dong");
                }
                case 2: // Di chuyen
                {
                    ShowPlayerDialog(playerid, DIALOG_ADMIN_MEDKIT_SETPOS, DIALOG_STYLE_INPUT,
                        "{FF3300}Di Chuyen Xuong Medkit",
                        "{FFFFFF}Nhap ID xuong can di chuyen:",
                        "OK", "Huy");
                }
                case 3: // Reload
                {
                    LoadMedkitCraftSpots();
                    new msg[64];
                    format(msg, sizeof(msg), "Da tai lai %d xuong che tao medkit.", gMedCraftCount);
                    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
                }
            }
        }

        case DIALOG_ADMIN_MEDKIT_DEL:
        {
            if(!response) return 1;
            if(listitem < 0 || listitem >= gMedCraftCount) return 1;
            if(PlayerInfo[playerid][pAdmin] < 1337) return 1;

            DestroyMedCraftWorldObjects(listitem);

            // Dich mang xuong
            for(new i = listitem; i < gMedCraftCount - 1; i++) {
                gMedCraftX[i] = gMedCraftX[i + 1];
                gMedCraftY[i] = gMedCraftY[i + 1];
                gMedCraftZ[i] = gMedCraftZ[i + 1];
                gMedCraftLabel[i] = gMedCraftLabel[i + 1];
                gMedCraftPickup[i] = gMedCraftPickup[i + 1];
            }
            gMedCraftCount--;
            SaveMedkitCraftSpots();

            new msg[64];
            format(msg, sizeof(msg), "Da xoa Phong Thi Nghiem Thuoc ID {FFFF00}%d{FFFFFF}.", listitem);
            SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
        }

        case DIALOG_ADMIN_MEDKIT_SETPOS:
        {
            if(!response) return PC_EmulateCommand(playerid, "/amedkit");
            if(PlayerInfo[playerid][pAdmin] < 1337) return 1;

            new id = strval(inputtext);
            if(id < 0 || id >= gMedCraftCount)
                return SendClientMessageEx(playerid, COLOR_GREY, "ID khong hop le!");

            DestroyMedCraftWorldObjects(id);
            GetPlayerPos(playerid, gMedCraftX[id], gMedCraftY[id], gMedCraftZ[id]);
            CreateMedCraftWorldObjects(id);
            SaveMedkitCraftSpots();

            new msg[64];
            format(msg, sizeof(msg), "Da cap nhat vi tri Phong Thi Nghiem Thuoc ID {FFFF00}%d{FFFFFF}.", id);
            SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
        }
    }
    return 1;
}

// ============================================================
// COMMAND: /amedkit - Admin panel
// ============================================================
CMD:amedkit(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1337)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong co quyen su dung lenh nay!");

    ShowPlayerDialog(playerid, DIALOG_ADMIN_MEDKIT_MENU, DIALOG_STYLE_LIST, "{FFFF00}Admin - Phong Thi Nghiem Thuoc", "{FF3300}[Phong TN]{FFFFFF} Them moi\n{FF3300}[Phong TN]{FFFFFF} Xoa\n{FF3300}[Phong TN]{FFFFFF} Di chuyen (setpos)\n{FF3300}[Phong TN]{FFFFFF} Tai lai", "Chon", "Dong");
    return 1;
}
