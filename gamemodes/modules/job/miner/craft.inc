// ============================================================
//  Miner Craft - GTA-HTB Roleplay
//  Che tao vu khi tu khoang san
// ============================================================

#include <YSI\YSI_Coding\y_hooks>

// Craft recipes: { sat, dong, bac, vang, kimcuong, money, weapon, ammo }
static const sCraftMelee[][8] = {
    {5,  0, 0, 0, 0, 5000,   5,  1},   // Bat (9mm - placeholder)
    {10, 2, 0, 0, 0, 15000,  8,  1},   // Dao (Knife)
    {0,  5, 3, 0, 0, 30000,  4,  1}    // Kinh (Chainsaw)
};

static const sCraftGun[][8] = {
    {0,  10, 5,  0, 0, 50000,   22, 50},  // Pistol
    {0,  20, 10, 2, 0, 150000,  25, 30},  // Shotgun
    {0,  30, 15, 5, 1, 500000,  31, 25}   // MP5
};

static const sMeleeNames[][32] = {"Gay (Bat)", "Dao (Knife)", "Cua may (Chainsaw)"};
static const sGunNames[][32]   = {"Sung ngan (Pistol)", "Sung hoa cai (Shotgun)", "MP5"};

// ============================================================
//  SHOW DIALOGS
// ============================================================

static stock Miner_ShowCraftMain(playerid)
{
    ShowPlayerDialog(playerid, DIALOG_MINER_CRAFT, DIALOG_STYLE_LIST,
        "{F5D000}Che Tao Vu Khi",
        "Vu khi co ban (chat the)\nSung",
        "Chon", "Huy");
}

static stock Miner_ShowCraftMelee(playerid)
{
    new dialog[512];
    new sat  = PlayerInfo[playerid][pMinSat];
    new dong = PlayerInfo[playerid][pMinDong];
    new bac  = PlayerInfo[playerid][pMinBac];
    new vang = PlayerInfo[playerid][pMinVang];

    for(new i = 0; i < sizeof(sCraftMelee); i++)
    {
        new line[128];
        format(line, sizeof(line), "%s  [{FFFF00}Sat:%d Dong:%d Bac:%d Vang:%d ${FF6B6B}%d{FFFFFF}]\n",
            sMeleeNames[i],
            sCraftMelee[i][0], sCraftMelee[i][1], sCraftMelee[i][2], sCraftMelee[i][3],
            sCraftMelee[i][5]);
        strcat(dialog, line, sizeof(dialog));
    }
    if(dialog[0] == '\n') strdel(dialog, 0, 1);

    format(dialog, sizeof(dialog), "{FFFFFF}Khoang san hien co: Sat={00FF7F}%d{FFFFFF} Dong={00FF7F}%d{FFFFFF} Bac={00FF7F}%d{FFFFFF} Vang={00FF7F}%d\n\n%s",
        sat, dong, bac, vang, dialog);

    ShowPlayerDialog(playerid, DIALOG_MINER_CRAFT_MELEE, DIALOG_STYLE_LIST,
        "{F5D000}Che Tao - Vu Khi Co Ban", dialog, "Che tao", "Quay lai");
}

static stock Miner_ShowCraftGun(playerid)
{
    new dialog[512];
    new dong = PlayerInfo[playerid][pMinDong];
    new bac  = PlayerInfo[playerid][pMinBac];
    new vang = PlayerInfo[playerid][pMinVang];
    new kc   = PlayerInfo[playerid][pMinKimCuong];

    for(new i = 0; i < sizeof(sCraftGun); i++)
    {
        new line[128];
        format(line, sizeof(line), "%s  [{FFFF00}Dong:%d Bac:%d Vang:%d KimCuong:%d ${FF6B6B}%d{FFFFFF}]\n",
            sGunNames[i],
            sCraftGun[i][1], sCraftGun[i][2], sCraftGun[i][3], sCraftGun[i][4],
            sCraftGun[i][5]);
        strcat(dialog, line, sizeof(dialog));
    }
    if(dialog[0] == '\n') strdel(dialog, 0, 1);

    format(dialog, sizeof(dialog), "{FFFFFF}Khoang san hien co: Dong={00FF7F}%d{FFFFFF} Bac={00FF7F}%d{FFFFFF} Vang={00FF7F}%d{FFFFFF} KC={00FF7F}%d\n\n%s",
        dong, bac, vang, kc, dialog);

    ShowPlayerDialog(playerid, DIALOG_MINER_CRAFT_GUN, DIALOG_STYLE_LIST,
        "{F5D000}Che Tao - Sung", dialog, "Che tao", "Quay lai");
}

// ============================================================
//  CRAFT LOGIC
// ============================================================

static stock Miner_DoCraftMelee(playerid, listitem)
{
    if(listitem < 0 || listitem >= sizeof(sCraftMelee)) return 0;

    new sat  = PlayerInfo[playerid][pMinSat];
    new dong = PlayerInfo[playerid][pMinDong];
    new bac  = PlayerInfo[playerid][pMinBac];
    new vang = PlayerInfo[playerid][pMinVang];
    new cost = sCraftMelee[listitem][5];

    if(sat  < sCraftMelee[listitem][0] || dong < sCraftMelee[listitem][1] ||
       bac  < sCraftMelee[listitem][2] || vang < sCraftMelee[listitem][3] ||
       GetPlayerMoney(playerid) < cost)
    {
        ShowTDN(playerid, NOTIFICATION_ERROR, "Khong du nguyen lieu hoac tien de che tao.");
        return 0;
    }

    PlayerInfo[playerid][pMinSat]  -= sCraftMelee[listitem][0];
    PlayerInfo[playerid][pMinDong] -= sCraftMelee[listitem][1];
    PlayerInfo[playerid][pMinBac]  -= sCraftMelee[listitem][2];
    PlayerInfo[playerid][pMinVang] -= sCraftMelee[listitem][3];
    GivePlayerMoney(playerid, -cost);
    GivePlayerWeapon(playerid, sCraftMelee[listitem][6], sCraftMelee[listitem][7]);

    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "[Che Tao] Thanh cong! Ban da che tao %s.", sMeleeNames[listitem]);
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Che tao vu khi thanh cong!");

    new pname[MAX_PLAYER_NAME];
    GetPlayerName(playerid, pname, sizeof(pname));
    DiscordLog(dclog_Miner,
        "THỢ KHAI THÁC - CHẾ TẠO",
        "**%s (ID: %d)** chế tạo **%s**\nChi phí: $%d",
        pname, playerid, sMeleeNames[listitem], cost);
    return 1;
}

static stock Miner_DoCraftGun(playerid, listitem)
{
    if(listitem < 0 || listitem >= sizeof(sCraftGun)) return 0;

    new dong = PlayerInfo[playerid][pMinDong];
    new bac  = PlayerInfo[playerid][pMinBac];
    new vang = PlayerInfo[playerid][pMinVang];
    new kc   = PlayerInfo[playerid][pMinKimCuong];
    new cost = sCraftGun[listitem][5];

    if(dong < sCraftGun[listitem][1] || bac  < sCraftGun[listitem][2] ||
       vang < sCraftGun[listitem][3] || kc   < sCraftGun[listitem][4] ||
       GetPlayerMoney(playerid) < cost)
    {
        ShowTDN(playerid, NOTIFICATION_ERROR, "Khong du nguyen lieu hoac tien de che tao.");
        return 0;
    }

    PlayerInfo[playerid][pMinDong]     -= sCraftGun[listitem][1];
    PlayerInfo[playerid][pMinBac]      -= sCraftGun[listitem][2];
    PlayerInfo[playerid][pMinVang]     -= sCraftGun[listitem][3];
    PlayerInfo[playerid][pMinKimCuong] -= sCraftGun[listitem][4];
    GivePlayerMoney(playerid, -cost);
    GivePlayerWeapon(playerid, sCraftGun[listitem][6], sCraftGun[listitem][7]);

    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "[Che Tao] Thanh cong! Ban da che tao %s.", sGunNames[listitem]);
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Che tao vu khi thanh cong!");

    new pname[MAX_PLAYER_NAME];
    GetPlayerName(playerid, pname, sizeof(pname));
    DiscordLog(dclog_Miner,
        "THỢ KHAI THÁC - CHẾ TẠO",
        "**%s (ID: %d)** chế tạo **%s**\nChi phí: $%d",
        pname, playerid, sGunNames[listitem], cost);
    return 1;
}

// ============================================================
//  COMMAND
// ============================================================

CMD:chebien(playerid, params[])
{
    if(PlayerInfo[playerid][pJob] != JOB_MINER && PlayerInfo[playerid][pJob2] != JOB_MINER)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban phai la Tho Khai Thac de su dung lenh nay.");

    if(!PlayerMinerData[playerid][MP_Working])
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban phai dang lam viec de su dung lenh nay.");

    Miner_ShowCraftMain(playerid);
    return 1;
}

// ============================================================
//  DIALOG RESPONSE
// ============================================================

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_MINER_CRAFT)
    {
        if(!response) return 1;
        switch(listitem)
        {
            case 0: Miner_ShowCraftMelee(playerid);
            case 1: Miner_ShowCraftGun(playerid);
        }
        return 1;
    }

    if(dialogid == DIALOG_MINER_CRAFT_MELEE)
    {
        if(!response) { Miner_ShowCraftMain(playerid); return 1; }
        Miner_DoCraftMelee(playerid, listitem);
        return 1;
    }

    if(dialogid == DIALOG_MINER_CRAFT_GUN)
    {
        if(!response) { Miner_ShowCraftMain(playerid); return 1; }
        Miner_DoCraftGun(playerid, listitem);
        return 1;
    }
    return 1;
}
