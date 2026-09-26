// ============================================================
//  Miner Craft - GTA-HTB Roleplay Created by Codyz
//  Che tao vu khi tu khoang san
// ============================================================

#include <YSI\YSI_Coding\y_hooks>

#define MAX_MELEE_WEAPONS 8
#define MAX_RANGED_WEAPONS 7
#define CRAFT_COOLDOWN 60

//-----DIALOG------
#define DIALOG_CRAFT              (9600)
#define DIALOG_CANCHIEN_LIST      (9601)
#define DIALOG_VUKHI_LIST         (9602)
#define DIALOG_CRAFT_INFO         (9604)
#define DIALOG_CRAFT_CONFIRM      (9605)

// Pos craft
new Float:gMeleeWeaponCraftPos[3] = { 1538.288208, -1475.621948, 9.500000}; //Can chien
new Float:gRangedWeaponCraftPos[3] = { 1512.567382, -1458.324951, 9.500000}; // Sung

// TEXT3D và Pickup
new Text3D:gMeleeWeaponCraftLabel;
new Text3D:gRangedWeaponCraftLabel;
new gMeleeWeaponCraftPickup;
new gRangedWeaponCraftPickup;


enum CraftingType {
    CRAFT_TYPE_MELEE,
    CRAFT_TYPE_RANGED
}


enum MeleeWeaponInfo {
    mwName[32],        // Tên vũ khí
    mwWeaponID,        // ID vũ khí trong game
    mwMaterialCost,    // Số vật liệu cần
    mwIronCost,        // Số sắt cần
    mwWoodCost,        // Số gỗ cần
    mwCraftTime,       // Thời gian chế tạo (giây)
    mwLevelRequired,   // Level nghề nghiệp yêu cầu
    mwXPGain           // Điểm kinh nghiệm khi chế tạo thành công
}

// Súng
enum RangedWeaponInfo {
    rwName[32],        // Tên súng
    rwWeaponID,        // ID súng trong game
    rwMaterialCost,    // Số vật liệu cần
    rwIronCost,        // Số sắt cần
    rwBronzeCost,      // Số đồng cần
    rwGoldCost,        // Số vàng cần
    rwCraftTime,       // Thời gian chế tạo (giây)
    rwLevelRequired,   // Level nghề nghiệp yêu cầu
    rwXPGain           // Điểm kinh nghiệm khi chế tạo thành công
}

// Biến lưu thông tin chế tạo
new bool:PlayerCrafting[MAX_PLAYERS];
new PlayerCraftingType[MAX_PLAYERS];
new PlayerCraftingItemID[MAX_PLAYERS];
new PlayerCraftingMaterialsCost[MAX_PLAYERS];
new PlayerCraftingIronCost[MAX_PLAYERS];
new PlayerCraftingWoodCost[MAX_PLAYERS];
new PlayerCraftingBronzeCost[MAX_PLAYERS];
new PlayerCraftingGoldCost[MAX_PLAYERS];

new MeleeWeapons[MAX_MELEE_WEAPONS][MeleeWeaponInfo] = {
    // Tên,             ID,     Vật liệu,   Sắt,    Gỗ,     Thời gian, Level yêu cầu, XP
    {"Brass Knuckles",  1,      5000,        20, 20,       30, 1, 10},
    {"Golf Club",       2,      8000,        30,   30,      40, 2, 15},
    {"Nightstick",      3,      7000,        20,   30,      35, 1, 12},
    {"Baseball Bat",    5,      6000,        10,   40,      30, 2, 10},
    {"Shovel",          6,      8000,        30, 20,      35, 1, 15},
    {"Pool Cue",        7,      5000,        15,   35,      30, 1, 10},
    {"Katana",          8,      10000,       50, 20,      60, 3, 25},
    {"Purple Dildo",    10,     1000,       0,      8000,   45, 1, 15}
};

new RangedWeapons[MAX_RANGED_WEAPONS][RangedWeaponInfo] = {
    // Tên,                 ID, Vật liệu, Sắt, Đồng, Vàng, Thời gian, Level yêu cầu, XP
    {"9mm Pistol",          22, 15000, 50, 30, 1, 90, 2, 30},
    {"Desert Eagle",        24, 50000, 80, 50, 2, 150, 3, 50},
    {"Shotgun",             25, 60000, 100, 40, 3, 180, 3, 60},
    {"MP5",                 29, 85000, 110, 60, 4, 420, 4, 80},
    {"AK-47",               30, 100000, 150, 80, 5, 240, 4, 90},
    {"Tec-9",               32, 70000, 90, 50, 2, 180, 3, 60},
    {"Country Rifle",       33, 90000, 120, 70, 4, 240, 4, 85}
};

forward OnPlayerCraftingComplete(playerid);

stock GetPlayerCraftingLevel(playerid)
{
    new level = 1;
    new skillPoints = PlayerInfo[playerid][pCraftSkill];

    if(skillPoints >= 0 && skillPoints < 50) level = 1;
    else if(skillPoints >= 50 && skillPoints < 100) level = 2;
    else if(skillPoints >= 100 && skillPoints < 200) level = 3;
    else if(skillPoints >= 200 && skillPoints < 400) level = 4;
    else if(skillPoints >= 400) level = 5;

    return level;
}

stock GetSuccessRateByLevel(level)
{
    new successRate;

    switch(level)
    {
        case 1: successRate = 40;
        case 2: successRate = 60;
        case 3: successRate = 70;
        case 4: successRate = 80;
        case 5: successRate = 100;
        default: successRate = 40;
    }

    return successRate;
}

stock IsPlayerAtCraftingLocation(playerid, type)
{
    switch(type)
    {
        case CRAFT_TYPE_MELEE:
        {
            if(IsPlayerInRangeOfPoint(playerid, 3.0, gMeleeWeaponCraftPos[0], gMeleeWeaponCraftPos[1], gMeleeWeaponCraftPos[2]))
                return 1;
        }
        case CRAFT_TYPE_RANGED:
        {
            if(IsPlayerInRangeOfPoint(playerid, 3.0, gRangedWeaponCraftPos[0], gRangedWeaponCraftPos[1], gRangedWeaponCraftPos[2]))
                return 1;
        }
    }

    return 0;
}

public OnPlayerCraftingComplete(playerid)
{
    if(!PlayerCrafting[playerid]) return 0;

    ClearAnimations(playerid);

    new itemid = PlayerCraftingItemID[playerid];
    new craftType = PlayerCraftingType[playerid];
    new string[128];
    new playerLevel = GetPlayerCraftingLevel(playerid);
    new successRate = GetSuccessRateByLevel(playerLevel);
    new randomChance = random(100) + 1;

    if(randomChance <= successRate)
    {
        switch(craftType)
        {
            case CRAFT_TYPE_MELEE:
            {
                GivePlayerValidWeapon(playerid, MeleeWeapons[itemid][mwWeaponID], 1);
                format(string, sizeof(string), "* Ban da che tao thanh cong %s!", MeleeWeapons[itemid][mwName]);
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, string);
                if(PlayerInfo[playerid][pCraftSkill] < 400)
                {
                    PlayerInfo[playerid][pCraftSkill] += MeleeWeapons[itemid][mwXPGain];
                    if(PlayerInfo[playerid][pCraftSkill] > 400) PlayerInfo[playerid][pCraftSkill] = 400;
                    SendClientMessageEx(playerid, COLOR_YELLOW, "* Diem kinh nghiem che tao cua ban tang len!");
                }
                SendClientMessageEx(playerid, COLOR_YELLOW, "* Ban da dat cap do cao nhat cua che tao nen khong cong them kinh nghiem nua.");
            }
            case CRAFT_TYPE_RANGED:
            {
                GivePlayerValidWeapon(playerid, RangedWeapons[itemid][rwWeaponID], 100);
                format(string, sizeof(string), "* Ban da che tao thanh cong %s voi 100 vien dan!", RangedWeapons[itemid][rwName]);
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, string);
                if(PlayerInfo[playerid][pCraftSkill] < 400)
                {
                    PlayerInfo[playerid][pCraftSkill] += RangedWeapons[itemid][rwXPGain];
                    if(PlayerInfo[playerid][pCraftSkill] > 400) PlayerInfo[playerid][pCraftSkill] = 400;
                    SendClientMessageEx(playerid, COLOR_YELLOW, "* Diem kinh nghiem che tao cua ban tang len!");
                }
            }
        }
    }
    else
    {
        SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* Rat tiec, ban da that bai trong qua trinh che tao!");
    }

    PlayerCrafting[playerid] = false;
    PlayerCraftingType[playerid] = 0;
    PlayerCraftingItemID[playerid] = 0;
    PlayerCraftingMaterialsCost[playerid] = 0;
    PlayerCraftingIronCost[playerid] = 0;
    PlayerCraftingWoodCost[playerid] = 0;
    PlayerCraftingBronzeCost[playerid] = 0;
    PlayerCraftingGoldCost[playerid] = 0;

    return 1;
}

stock StartCrafting(playerid, type, itemid)
{
    if(PlayerCrafting[playerid])
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Ban dang che tao mot vat pham khac.");
        return 0;
    }

    if(!IsPlayerAtCraftingLocation(playerid, type))
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Ban khong o khu vuc che tao!");
        return 0;
    }

    new playerLevel = GetPlayerCraftingLevel(playerid);
    new itemLevel = 1;
    new materialCost = 0, ironCost = 0, woodCost = 0, bronzeCost = 0, goldCost = 0;
    new craftTime = 0;
    new string[128];

    switch(type)
    {
        case CRAFT_TYPE_MELEE:
        {
            if(itemid < 0 || itemid >= MAX_MELEE_WEAPONS)
            {
                SendClientMessageEx(playerid, COLOR_GREY, "ID vu khi khong hop le!");
                return 0;
            }

            itemLevel = MeleeWeapons[itemid][mwLevelRequired];
            materialCost = MeleeWeapons[itemid][mwMaterialCost];
            ironCost = MeleeWeapons[itemid][mwIronCost];
            woodCost = MeleeWeapons[itemid][mwWoodCost];
            craftTime = MeleeWeapons[itemid][mwCraftTime];

            if(playerLevel < itemLevel)
            {
                format(string, sizeof(string), "Ban can level %d de che tao %s!", itemLevel, MeleeWeapons[itemid][mwName]);
                SendClientMessageEx(playerid, COLOR_GREY, string);
                return 0;
            }

            if(PlayerInfo[playerid][pMats] < materialCost)
            {
                format(string, sizeof(string), "Ban can %d vat lieu de che tao %s!", materialCost, MeleeWeapons[itemid][mwName]);
                SendClientMessageEx(playerid, COLOR_GREY, string);
                return 0;
            }

            if(PlayerInfo[playerid][pMinSat] < ironCost)
            {
                format(string, sizeof(string), "Ban can %d sat de che tao %s!", ironCost, MeleeWeapons[itemid][mwName]);
                SendClientMessageEx(playerid, COLOR_GREY, string);
                return 0;
            }

            if(PlayerInfo[playerid][pGoThuong] < woodCost)
            {
                format(string, sizeof(string), "Ban can %d go de che tao %s!", woodCost, MeleeWeapons[itemid][mwName]);
                SendClientMessageEx(playerid, COLOR_GREY, string);
                return 0;
            }

            PlayerInfo[playerid][pMats] -= materialCost;
            PlayerInfo[playerid][pMinSat] -= ironCost;
            PlayerInfo[playerid][pGoThuong] -= woodCost;

            PlayerCraftingMaterialsCost[playerid] = materialCost;
            PlayerCraftingIronCost[playerid] = ironCost;
            PlayerCraftingWoodCost[playerid] = woodCost;

            format(string, sizeof(string), "Dang che tao %s...", MeleeWeapons[itemid][mwName]);

            PlayerCrafting[playerid] = true;
            PlayerCraftingType[playerid] = CRAFT_TYPE_MELEE;
            PlayerCraftingItemID[playerid] = itemid;

            ApplyAnimation(playerid, "BD_FIRE", "wash_up", 4.1, 1, 0, 0, 0, 10000, 1);
            SetLoading(playerid, craftTime * 1000, string, "OnPlayerCraftingComplete");
            SendHintMessage(playerid, "Su dung /huychetao de huy qua trinh che tao.");

            return 1;
        }
        case CRAFT_TYPE_RANGED:
        {
            if(itemid < 0 || itemid >= MAX_RANGED_WEAPONS)
            {
                SendClientMessageEx(playerid, COLOR_GREY, "ID sung khong hop le!");
                return 0;
            }

            itemLevel = RangedWeapons[itemid][rwLevelRequired];
            materialCost = RangedWeapons[itemid][rwMaterialCost];
            ironCost = RangedWeapons[itemid][rwIronCost];
            bronzeCost = RangedWeapons[itemid][rwBronzeCost];
            goldCost = RangedWeapons[itemid][rwGoldCost];
            craftTime = RangedWeapons[itemid][rwCraftTime];

            if(playerLevel < itemLevel)
            {
                format(string, sizeof(string), "Ban can level %d de che tao %s!", itemLevel, RangedWeapons[itemid][rwName]);
                SendClientMessageEx(playerid, COLOR_GREY, string);
                return 0;
            }

            if(PlayerInfo[playerid][pMats] < materialCost)
            {
                format(string, sizeof(string), "Ban can %d vat lieu de che tao %s!", materialCost, RangedWeapons[itemid][rwName]);
                SendClientMessageEx(playerid, COLOR_GREY, string);
                return 0;
            }

            if(PlayerInfo[playerid][pMinSat] < ironCost)
            {
                format(string, sizeof(string), "Ban can %d sat de che tao %s!", ironCost, RangedWeapons[itemid][rwName]);
                SendClientMessageEx(playerid, COLOR_GREY, string);
                return 0;
            }

            if(PlayerInfo[playerid][pMinDong] < bronzeCost)
            {
                format(string, sizeof(string), "Ban can %d dong de che tao %s!", bronzeCost, RangedWeapons[itemid][rwName]);
                SendClientMessageEx(playerid, COLOR_GREY, string);
                return 0;
            }

            if(PlayerInfo[playerid][pMinVang] < goldCost)
            {
                format(string, sizeof(string), "Ban can %d vang de che tao %s!", goldCost, RangedWeapons[itemid][rwName]);
                SendClientMessageEx(playerid, COLOR_GREY, string);
                return 0;
            }

            PlayerInfo[playerid][pMats] -= materialCost;
            PlayerInfo[playerid][pMinSat] -= ironCost;
            PlayerInfo[playerid][pMinDong] -= bronzeCost;
            PlayerInfo[playerid][pMinVang] -= goldCost;

            PlayerCraftingMaterialsCost[playerid] = materialCost;
            PlayerCraftingIronCost[playerid] = ironCost;
            PlayerCraftingBronzeCost[playerid] = bronzeCost;
            PlayerCraftingGoldCost[playerid] = goldCost;

            format(string, sizeof(string), "Dang che tao %s...", RangedWeapons[itemid][rwName]);

            PlayerCrafting[playerid] = true;
            PlayerCraftingType[playerid] = CRAFT_TYPE_RANGED;
            PlayerCraftingItemID[playerid] = itemid;

            ApplyAnimation(playerid, "BD_FIRE", "wash_up", 4.1, 1, 0, 0, 0, 10000, 1);
            SetLoading(playerid, craftTime * 1000, string, "OnPlayerCraftingComplete");
            SendHintMessage(playerid, "Su dung /huychetao de huy qua trinh che tao.");

            return 1;
        }
    }

    return 0;
}

stock StopCrafting(playerid, bool:refundResources = true)
{
    if(!PlayerCrafting[playerid]) return 0;

    StopLoading(playerid);
    ClearAnimations(playerid);

    if(refundResources)
    {
        PlayerInfo[playerid][pMats] += PlayerCraftingMaterialsCost[playerid];
        PlayerInfo[playerid][pMinSat] += PlayerCraftingIronCost[playerid];
        PlayerInfo[playerid][pGoThuong] += PlayerCraftingWoodCost[playerid];

        if(PlayerCraftingType[playerid] == CRAFT_TYPE_RANGED)
        {
            PlayerInfo[playerid][pMinDong] += PlayerCraftingBronzeCost[playerid];
            PlayerInfo[playerid][pMinVang] += PlayerCraftingGoldCost[playerid];
        }

        SendClientMessageEx(playerid, COLOR_YELLOW, "* Qua trinh che tao da bi huy, tai nguyen da duoc hoan tra!");
    }

    PlayerCrafting[playerid] = false;
    PlayerCraftingType[playerid] = 0;
    PlayerCraftingItemID[playerid] = 0;
    PlayerCraftingMaterialsCost[playerid] = 0;
    PlayerCraftingIronCost[playerid] = 0;
    PlayerCraftingWoodCost[playerid] = 0;
    PlayerCraftingBronzeCost[playerid] = 0;
    PlayerCraftingGoldCost[playerid] = 0;

    return 1;
}

stock ShowItemDetailInfo(playerid, craftType, itemid)
{
    new string[512];
    new itemLevel = 1;
    new playerLevel = GetPlayerCraftingLevel(playerid);
    new canCraft = 1;
    new materialHave, ironHave, woodHave, bronzeHave = 0, goldHave = 0;
    new materialNeed, ironNeed, woodNeed, bronzeNeed = 0, goldNeed = 0;

    materialHave = PlayerInfo[playerid][pMats];
    ironHave = PlayerInfo[playerid][pMinSat];
    woodHave = PlayerInfo[playerid][pGoThuong];

    if(craftType == CRAFT_TYPE_RANGED)
    {
        bronzeHave = PlayerInfo[playerid][pMinDong];
        goldHave = PlayerInfo[playerid][pMinVang];
    }

    switch(craftType)
    {
        case CRAFT_TYPE_MELEE:
        {
            if(itemid < 0 || itemid >= MAX_MELEE_WEAPONS)
            {
                SendClientMessageEx(playerid, COLOR_GREY, "ID vu khi khong hop le!");
                return 0;
            }

            format(string, sizeof(string), "{FFFFFF}Thong tin chi tiet ve {FFFF00}%s{FFFFFF}:\n\n", MeleeWeapons[itemid][mwName]);

            itemLevel = MeleeWeapons[itemid][mwLevelRequired];
            materialNeed = MeleeWeapons[itemid][mwMaterialCost];
            ironNeed = MeleeWeapons[itemid][mwIronCost];
            woodNeed = MeleeWeapons[itemid][mwWoodCost];

            if(playerLevel < itemLevel)
            {
                format(string, sizeof(string), "%s{FF0000}Cap do yeu cau: %d (Ban dang o cap %d){FFFFFF}\n\n", string, itemLevel, playerLevel);
                canCraft = 0;
            }
            else
            {
                format(string, sizeof(string), "%s{00FF00}Cap do yeu cau: %d (Ban dang o cap %d){FFFFFF}\n\n", string, itemLevel, playerLevel);
            }

            format(string, sizeof(string), "%s{FFFF00}Vat lieu can thiet:{FFFFFF}\n", string);

            if(materialHave >= materialNeed)
                format(string, sizeof(string), "%s- Vat lieu: {00FF00}%d/%d{FFFFFF}\n", string, materialHave, materialNeed);
            else { format(string, sizeof(string), "%s- Vat lieu: {FF0000}%d/%d{FFFFFF}\n", string, materialHave, materialNeed); canCraft = 0; }

            if(ironHave >= ironNeed)
                format(string, sizeof(string), "%s- Sat: {00FF00}%d/%d{FFFFFF}\n", string, ironHave, ironNeed);
            else { format(string, sizeof(string), "%s- Sat: {FF0000}%d/%d{FFFFFF}\n", string, ironHave, ironNeed); canCraft = 0; }

            if(woodHave >= woodNeed)
                format(string, sizeof(string), "%s- Go: {00FF00}%d/%d{FFFFFF}\n\n", string, woodHave, woodNeed);
            else { format(string, sizeof(string), "%s- Go: {FF0000}%d/%d{FFFFFF}\n\n", string, woodHave, woodNeed); canCraft = 0; }

            format(string, sizeof(string), "%s{FFFF00}Thoi gian che tao: {FFFFFF}%d giay\n", string, MeleeWeapons[itemid][mwCraftTime]);
            format(string, sizeof(string), "%s{FFFF00}Ti le thanh cong: {FFFFFF}%d%%\n", string, GetSuccessRateByLevel(playerLevel));
        }
        case CRAFT_TYPE_RANGED:
        {
            if(itemid < 0 || itemid >= MAX_RANGED_WEAPONS)
            {
                SendClientMessageEx(playerid, COLOR_GREY, "ID sung khong hop le!");
                return 0;
            }

            format(string, sizeof(string), "{FFFFFF}Thong tin chi tiet ve {FFFF00}%s{FFFFFF}:\n\n", RangedWeapons[itemid][rwName]);

            itemLevel = RangedWeapons[itemid][rwLevelRequired];
            materialNeed = RangedWeapons[itemid][rwMaterialCost];
            ironNeed = RangedWeapons[itemid][rwIronCost];
            bronzeNeed = RangedWeapons[itemid][rwBronzeCost];
            goldNeed = RangedWeapons[itemid][rwGoldCost];

            if(playerLevel < itemLevel)
            {
                format(string, sizeof(string), "%s{FF0000}Cap do yeu cau: %d (Ban dang o cap %d){FFFFFF}\n\n", string, itemLevel, playerLevel);
                canCraft = 0;
            }
            else
            {
                format(string, sizeof(string), "%s{00FF00}Cap do yeu cau: %d (Ban dang o cap %d){FFFFFF}\n\n", string, itemLevel, playerLevel);
            }

            format(string, sizeof(string), "%s{FFFF00}Vat lieu can thiet:{FFFFFF}\n", string);

            if(materialHave >= materialNeed)
                format(string, sizeof(string), "%s- Vat lieu: {00FF00}%d/%d{FFFFFF}\n", string, materialHave, materialNeed);
            else { format(string, sizeof(string), "%s- Vat lieu: {FF0000}%d/%d{FFFFFF}\n", string, materialHave, materialNeed); canCraft = 0; }

            if(ironHave >= ironNeed)
                format(string, sizeof(string), "%s- Sat: {00FF00}%d/%d{FFFFFF}\n", string, ironHave, ironNeed);
            else { format(string, sizeof(string), "%s- Sat: {FF0000}%d/%d{FFFFFF}\n", string, ironHave, ironNeed); canCraft = 0; }

            if(bronzeHave >= bronzeNeed)
                format(string, sizeof(string), "%s- Dong: {00FF00}%d/%d{FFFFFF}\n", string, bronzeHave, bronzeNeed);
            else { format(string, sizeof(string), "%s- Dong: {FF0000}%d/%d{FFFFFF}\n", string, bronzeHave, bronzeNeed); canCraft = 0; }

            if(goldHave >= goldNeed)
                format(string, sizeof(string), "%s- Vang: {00FF00}%d/%d{FFFFFF}\n\n", string, goldHave, goldNeed);
            else { format(string, sizeof(string), "%s- Vang: {FF0000}%d/%d{FFFFFF}\n\n", string, goldHave, goldNeed); canCraft = 0; }

            format(string, sizeof(string), "%s{FFFF00}Thoi gian che tao: {FFFFFF}%d giay\n", string, RangedWeapons[itemid][rwCraftTime]);
            format(string, sizeof(string), "%s{FFFF00}Ti le thanh cong: {FFFFFF}%d%%\n", string, GetSuccessRateByLevel(playerLevel));
        }
    }

    SetPVarInt(playerid, "CraftingDialogType", craftType);
    SetPVarInt(playerid, "CraftingDialogItem", itemid);
    SetPVarInt(playerid, "CraftingCanCraft", canCraft);

    if(canCraft)
        ShowPlayerDialog(playerid, DIALOG_CRAFT_INFO, DIALOG_STYLE_MSGBOX, "Thong Tin Chi Tiet", string, "Che Tao", "Quay Lai");
    else
        ShowPlayerDialog(playerid, DIALOG_CRAFT_INFO, DIALOG_STYLE_MSGBOX, "Thong Tin Chi Tiet", string, "Quay Lai", "Dong");

    return 1;
}


//-------------CMD ADMIN--------------

CMD:testcraft(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 4) return 1;
    PlayerInfo[playerid][pCraftSkill] = 400;
    SuccesMsg(playerid, "Da set craft skill len cap 5!");
    return 1;
}

CMD:setcraftmaterial(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 4)
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Ban khong duoc phep su dung lenh nay!");
        return 1;
    }

    new targetid, materialtype[32], amount;

    if(sscanf(params, "us[32]i", targetid, materialtype, amount))
    {
        SendClientMessageEx(playerid, COLOR_WHITE, "USAGE: /setcraftmaterial [playerid/PartOfName] [loai] [so luong]");
        SendClientMessageEx(playerid, COLOR_WHITE, "Loai: vatlieu, sat, go, dong, vang");
        return 1;
    }

    if(!IsPlayerConnected(targetid))
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi khong ton tai!");
        return 1;
    }

    if(amount < 0 || amount > 1000000)
    {
        SendClientMessageEx(playerid, COLOR_GREY, "So luong khong hop le (0-1,000,000)!");
        return 1;
    }

    new string[128];
    if(strcmp(materialtype, "vatlieu", true) == 0)
    {
        PlayerInfo[targetid][pMats] = amount;
        format(string, sizeof(string), "Ban da thiet lap %s so vat lieu thanh %d.", GetPlayerNameEx(targetid), amount);
        SendClientMessageEx(playerid, COLOR_WHITE, string);
        format(string, sizeof(string), "Admin %s da thiet lap so vat lieu cua ban thanh %d.", GetPlayerNameEx(playerid), amount);
        SendClientMessageEx(targetid, COLOR_WHITE, string);
    }
    else if(strcmp(materialtype, "sat", true) == 0)
    {
        PlayerInfo[targetid][pMinSat] = amount;
        format(string, sizeof(string), "Ban da thiet lap %s so sat thanh %d.", GetPlayerNameEx(targetid), amount);
        SendClientMessageEx(playerid, COLOR_WHITE, string);
        format(string, sizeof(string), "Admin %s da thiet lap so sat cua ban thanh %d.", GetPlayerNameEx(playerid), amount);
        SendClientMessageEx(targetid, COLOR_WHITE, string);
    }
    else if(strcmp(materialtype, "go", true) == 0)
    {
        PlayerInfo[targetid][pGoThuong] = amount;
        format(string, sizeof(string), "Ban da thiet lap %s so go thanh %d.", GetPlayerNameEx(targetid), amount);
        SendClientMessageEx(playerid, COLOR_WHITE, string);
        format(string, sizeof(string), "Admin %s da thiet lap so go cua ban thanh %d.", GetPlayerNameEx(playerid), amount);
        SendClientMessageEx(targetid, COLOR_WHITE, string);
    }
    else if(strcmp(materialtype, "dong", true) == 0)
    {
        PlayerInfo[targetid][pMinDong] = amount;
        format(string, sizeof(string), "Ban da thiet lap %s so dong thanh %d.", GetPlayerNameEx(targetid), amount);
        SendClientMessageEx(playerid, COLOR_WHITE, string);
        format(string, sizeof(string), "Admin %s da thiet lap so dong cua ban thanh %d.", GetPlayerNameEx(playerid), amount);
        SendClientMessageEx(targetid, COLOR_WHITE, string);
    }
    else if(strcmp(materialtype, "vang", true) == 0)
    {
        PlayerInfo[targetid][pMinVang] = amount;
        format(string, sizeof(string), "Ban da thiet lap %s so vang thanh %d.", GetPlayerNameEx(targetid), amount);
        SendClientMessageEx(playerid, COLOR_WHITE, string);
        format(string, sizeof(string), "Admin %s da thiet lap so vang cua ban thanh %d.", GetPlayerNameEx(playerid), amount);
        SendClientMessageEx(targetid, COLOR_WHITE, string);
    }
    else
    {
        SendClientMessageEx(playerid, COLOR_WHITE, "USAGE: /setcraftmaterial [playerid/PartOfName] [loai] [so luong]");
        SendClientMessageEx(playerid, COLOR_WHITE, "Loai: vatlieu, sat, go, dong, vang");
        return 1;
    }

    return 1;
}

CMD:setcraftlevel(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 4)
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Ban khong duoc phep su dung lenh nay!");
        return 1;
    }

    new targetid, craftpoints;

    if(sscanf(params, "ui", targetid, craftpoints))
    {
        SendClientMessageEx(playerid, COLOR_WHITE, "USAGE: /setcraftlevel [playerid/PartOfName] [diem kinh nghiem]");
        return 1;
    }

    if(!IsPlayerConnected(targetid))
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi khong ton tai!");
        return 1;
    }

    if(craftpoints < 0 || craftpoints > 1000)
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Diem kinh nghiem khong hop le (0-1,000)!");
        return 1;
    }
    PlayerInfo[targetid][pCraftSkill] = craftpoints;

    new level = GetPlayerCraftingLevel(targetid);

    new string[128];
    format(string, sizeof(string), "Ban da thiet lap diem kinh nghiem che tao cua %s thanh %d (Cap do %d).", GetPlayerNameEx(targetid), craftpoints, level);
    SendClientMessageEx(playerid, COLOR_WHITE, string);
    format(string, sizeof(string), "Admin %s da thiet lap diem kinh nghiem che tao cua ban thanh %d (Cap do %d).", GetPlayerNameEx(playerid), craftpoints, level);
    SendClientMessageEx(targetid, COLOR_WHITE, string);

    return 1;
}

CMD:craftkit(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 4)
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Ban khong duoc phep su dung lenh nay!");
        return 1;
    }

    new targetid;

    if(sscanf(params, "u", targetid))
    {
        SendClientMessageEx(playerid, COLOR_WHITE, "USAGE: /craftkit [playerid/PartOfName]");
        return 1;
    }

    if(!IsPlayerConnected(targetid))
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi khong ton tai!");
        return 1;
    }

    PlayerInfo[targetid][pMats] = 50000;
    PlayerInfo[targetid][pMinSat] = 10000;
    PlayerInfo[targetid][pGoThuong] = 10000;
    PlayerInfo[targetid][pMinDong] = 10000;
    PlayerInfo[targetid][pMinVang] = 100;

    new string[128];
    format(string, sizeof(string), "Ban da cung cap day du vat lieu cho %s.", GetPlayerNameEx(targetid));
    SendClientMessageEx(playerid, COLOR_WHITE, string);
    format(string, sizeof(string), "Admin %s da cung cap day du vat lieu cho ban.", GetPlayerNameEx(playerid));
    SendClientMessageEx(targetid, COLOR_WHITE, string);
    SendClientMessageEx(targetid, COLOR_YELLOW, "Ban nhan duoc: 50,000 vat lieu, 10,000 sat, 10,000 go, 10,000 dong, 100 vang.");

    return 1;
}

CMD:checkcraft(playerid, params[])
{
    new targetid;

    if(sscanf(params, "u", targetid))
        targetid = playerid;

    if(targetid != playerid && PlayerInfo[playerid][pAdmin] < 2)
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Ban khong duoc phep xem thong tin nguoi khac!");
        return 1;
    }

    if(!IsPlayerConnected(targetid))
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi khong ton tai!");
        return 1;
    }

    new string[512];
    format(string, sizeof(string), "{FFFFFF}Thong tin vat lieu che tao cua {FFFF00}%s{FFFFFF}:\n\n", GetPlayerNameEx(targetid));
    format(string, sizeof(string), "%s{FFFF00}Vat lieu: {FFFFFF}%d\n", string, PlayerInfo[targetid][pMats]);
    format(string, sizeof(string), "%s{FFFF00}Sat: {FFFFFF}%d\n", string, PlayerInfo[targetid][pMinSat]);
    format(string, sizeof(string), "%s{FFFF00}Go: {FFFFFF}%d\n", string, PlayerInfo[targetid][pGoThuong]);
    format(string, sizeof(string), "%s{FFFF00}Dong: {FFFFFF}%d\n", string, PlayerInfo[targetid][pMinDong]);
    format(string, sizeof(string), "%s{FFFF00}Vang: {FFFFFF}%d\n\n", string, PlayerInfo[targetid][pMinVang]);

    new level = GetPlayerCraftingLevel(targetid);
    format(string, sizeof(string), "%s{FFFF00}Cap do che tao: {FFFFFF}%d\n", string, level);
    format(string, sizeof(string), "%s{FFFF00}Diem kinh nghiem: {FFFFFF}%d\n", string, PlayerInfo[targetid][pCraftSkill]);

    new successRate = GetSuccessRateByLevel(level);
    format(string, sizeof(string), "%s{FFFF00}Ti le thanh cong: {FFFFFF}%d%%\n", string, successRate);

    ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "Thong Tin Vat Lieu Che Tao", string, "Dong", "");

    return 1;
}

CMD:trogiupchetao(playerid, params[])
{
    SendClientMessageEx(playerid, COLOR_WHITE, "=== LENH CHE TAO ===");
    SendClientMessageEx(playerid, COLOR_WHITE, "/chetaocanchien - Che tao vu khi can chien");
    SendClientMessageEx(playerid, COLOR_WHITE, "/chetaovukhi - Che tao sung");
    SendClientMessageEx(playerid, COLOR_WHITE, "/huychetao - Huy qua trinh che tao hien tai");
    SendClientMessageEx(playerid, COLOR_WHITE, "/thongkechetao - Xem thong tin kinh nghiem che tao");
    SendClientMessageEx(playerid, COLOR_WHITE, "/checkcraft - Xem thong tin vat lieu");

    if(PlayerInfo[playerid][pAdmin] >= 4)
    {
        SendClientMessageEx(playerid, COLOR_YELLOW, "=== LENH ADMIN ===");
        SendClientMessageEx(playerid, COLOR_YELLOW, "/setcraftmaterial - Set vat lieu cho nguoi choi");
        SendClientMessageEx(playerid, COLOR_YELLOW, "/setcraftlevel - Set cap do che tao cho nguoi choi");
        SendClientMessageEx(playerid, COLOR_YELLOW, "/craftkit - Cung cap day du vat lieu cho nguoi choi");
    }

    return 1;
}


//-------------CMD--------------
CMD:chetaocanchien(playerid, params[])
{
    if(PlayerCrafting[playerid])
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Ban dang che tao mot vat pham khac.");
        return 1;
    }

    if(!IsPlayerAtCraftingLocation(playerid, CRAFT_TYPE_MELEE))
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Ban khong o khu vuc che tao vu khi can chien!");
        return 1;
    }

    new string[1024];
    string = "ID\tTen Vu Khi\tLevel\n";

    for(new i = 0; i < MAX_MELEE_WEAPONS; i++)
    {
        format(string, sizeof(string), "%s%d\t%s\t%d\n", string, i, MeleeWeapons[i][mwName], MeleeWeapons[i][mwLevelRequired]);
    }

    ShowPlayerDialog(playerid, DIALOG_CANCHIEN_LIST, DIALOG_STYLE_TABLIST_HEADERS, "Che Tao Vu Khi Can Chien", string, "Xem Chi Tiet", "Huy Bo");
    return 1;
}

CMD:chetaovukhi(playerid, params[])
{
    if(PlayerCrafting[playerid])
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Ban dang che tao mot vat pham khac.");
        return 1;
    }

    if(!IsPlayerAtCraftingLocation(playerid, CRAFT_TYPE_RANGED))
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Ban khong o khu vuc che tao sung!");
        return 1;
    }

    new string[1024];
    string = "ID\tTen Sung\tLevel\n";

    for(new i = 0; i < MAX_RANGED_WEAPONS; i++)
    {
        format(string, sizeof(string), "%s%d\t%s\t%d\n", string, i, RangedWeapons[i][rwName], RangedWeapons[i][rwLevelRequired]);
    }

    ShowPlayerDialog(playerid, DIALOG_VUKHI_LIST, DIALOG_STYLE_TABLIST_HEADERS, "Che Tao Sung", string, "Xem Chi Tiet", "Huy Bo");
    return 1;
}

CMD:huychetao(playerid, params[])
{
    if(!PlayerCrafting[playerid])
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Ban khong dang che tao gi ca!");
        return 1;
    }

    SendClientMessageEx(playerid, COLOR_WHITE, "Ban da huy qua trinh che tao, vat lieu da duoc hoan tra.");
    StopCrafting(playerid, true);
    return 1;
}

CMD:vitrichetao(playerid, params[])
{
    new type, string[128];

    if(sscanf(params, "i", type))
    {
        SendClientMessageEx(playerid, COLOR_WHITE, "USAGE: /vitrichetao [loai]");
        SendClientMessageEx(playerid, COLOR_WHITE, "Loai: 1 = Can Chien, 2 = Sung");
        return 1;
    }

    switch(type)
    {
        case 1:
        {
            SetPlayerCheckpoint(playerid, gMeleeWeaponCraftPos[0], gMeleeWeaponCraftPos[1], gMeleeWeaponCraftPos[2], 3.0);
            format(string, sizeof(string), "Da danh dau vi tri che tao vu khi can chien tren ban do.");
            SendClientMessageEx(playerid, COLOR_WHITE, string);
            CP[playerid] = 252000;
        }
        case 2:
        {
            SetPlayerCheckpoint(playerid, gRangedWeaponCraftPos[0], gRangedWeaponCraftPos[1], gRangedWeaponCraftPos[2], 3.0);
            format(string, sizeof(string), "Da danh dau vi tri che tao sung tren ban do.");
            SendClientMessageEx(playerid, COLOR_WHITE, string);
            CP[playerid] = 252000;
        }
        default:
        {
            SendClientMessageEx(playerid, COLOR_WHITE, "USAGE: /vitrichetao [loai]");
            SendClientMessageEx(playerid, COLOR_WHITE, "Loai: 1 = Can Chien, 2 = Sung");
            return 1;
        }
    }

    return 1;
}

CMD:thongkechetao(playerid, params[])
{
    new level = GetPlayerCraftingLevel(playerid);
    new successRate = GetSuccessRateByLevel(level);
    new string[512];

    format(string, sizeof(string), "{FFFFFF}Thong tin tho che tao:\n\n");
    format(string, sizeof(string), "%s{FFFF00}Cap do hien tai: {FFFFFF}%d\n", string, level);
    format(string, sizeof(string), "%s{FFFF00}Diem kinh nghiem: {FFFFFF}%d\n", string, PlayerInfo[playerid][pCraftSkill]);
    format(string, sizeof(string), "%s{FFFF00}Ti le thanh cong: {FFFFFF}%d%%\n\n", string, successRate);

    new pointsNextLevel;
    switch(level)
    {
        case 1: pointsNextLevel = 50 - PlayerInfo[playerid][pCraftSkill];
        case 2: pointsNextLevel = 100 - PlayerInfo[playerid][pCraftSkill];
        case 3: pointsNextLevel = 200 - PlayerInfo[playerid][pCraftSkill];
        case 4: pointsNextLevel = 400 - PlayerInfo[playerid][pCraftSkill];
        case 5: pointsNextLevel = 0;
    }

    if(level < 5)
        format(string, sizeof(string), "%s{FFFF00}Diem can thiet de len cap %d: {FFFFFF}%d\n", string, level + 1, pointsNextLevel);
    else
        format(string, sizeof(string), "%s{00FF00}Ban da dat cap do toi da!\n", string);

    format(string, sizeof(string), "%s\n{FFFF00}Ti le thanh cong theo cap do:\n", string);
    format(string, sizeof(string), "%s{FFFFFF}Cap 1: 40%%\n", string);
    format(string, sizeof(string), "%s{FFFFFF}Cap 2: 60%%\n", string);
    format(string, sizeof(string), "%s{FFFFFF}Cap 3: 70%%\n", string);
    format(string, sizeof(string), "%s{FFFFFF}Cap 4: 80%%\n", string);
    format(string, sizeof(string), "%s{FFFFFF}Cap 5: 100%%\n", string);

    ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "Thong Tin Tho Che Tao", string, "Dong", "");

    return 1;
}

CMD:huongdanchetao(playerid, params[])
{
    new string[2048];

    format(string, sizeof(string), "{FFFFFF}=== HUONG DAN CHE TAO ===\n\n");
    format(string, sizeof(string), "%s{FFFF00}1. Cap do va ti le thanh cong:{FFFFFF}\n", string);
    format(string, sizeof(string), "%s- Cap 1 (0-49 diem): 40%% thanh cong\n", string);
    format(string, sizeof(string), "%s- Cap 2 (50-99 diem): 60%% thanh cong\n", string);
    format(string, sizeof(string), "%s- Cap 3 (100-199 diem): 70%% thanh cong\n", string);
    format(string, sizeof(string), "%s- Cap 4 (200-399 diem): 80%% thanh cong\n", string);
    format(string, sizeof(string), "%s- Cap 5 (400+ diem): 100%% thanh cong\n\n", string);

    format(string, sizeof(string), "%s{FFFF00}2. Vat lieu can thiet:{FFFFFF}\n", string);
    format(string, sizeof(string), "%s- Vat lieu: Su dung cho tat ca cac loai vat pham\n", string);
    format(string, sizeof(string), "%s- Sat: Su dung cho hau het cac vat pham\n", string);
    format(string, sizeof(string), "%s- Go: Su dung cho mot so vu khi can chien\n", string);
    format(string, sizeof(string), "%s- Dong: Su dung cho viec che tao sung\n", string);
    format(string, sizeof(string), "%s- Vang: Nguyen lieu cao cap de che tao sung\n\n", string);

    format(string, sizeof(string), "%s{FFFF00}3. Cac lenh:{FFFFFF}\n", string);
    format(string, sizeof(string), "%s- /chetaocanchien: Mo menu che tao vu khi can chien\n", string);
    format(string, sizeof(string), "%s- /chetaovukhi: Mo menu che tao sung\n", string);
    format(string, sizeof(string), "%s- /huychetao: Huy qua trinh che tao va hoan tra vat lieu\n", string);
    format(string, sizeof(string), "%s- /thongkechetao: Xem thong tin ve cap do va diem kinh nghiem\n", string);
    format(string, sizeof(string), "%s- /checkcraft: Xem so luong vat lieu hien co\n", string);
    format(string, sizeof(string), "%s- /vitrichetao: Tim duong den khu vuc che tao\n\n", string);

    format(string, sizeof(string), "%s{FFFF00}4. Meo:{FFFFFF}\n", string);
    format(string, sizeof(string), "%s- Moi lan che tao du thanh cong hay that bai deu tang kinh nghiem\n", string);
    format(string, sizeof(string), "%s- Neu crash game trong qua trinh che tao, vat lieu se duoc hoan tra\n", string);

    ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "Huong Dan Nghe Che Tao", string, "Dong", "");

    return 1;
}

//---------------CALLBACK-------------

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid)
    {
        case DIALOG_CANCHIEN_LIST:
        {
            if(!response) return 1;
            ShowItemDetailInfo(playerid, CRAFT_TYPE_MELEE, listitem);
            return 1;
        }
        case DIALOG_VUKHI_LIST:
        {
            if(!response) return 1;
            ShowItemDetailInfo(playerid, CRAFT_TYPE_RANGED, listitem);
            return 1;
        }
        case DIALOG_CRAFT_INFO:
        {
            if(!response)
            {
                new craftType = GetPVarInt(playerid, "CraftingDialogType");
                switch(craftType)
                {
                    case CRAFT_TYPE_MELEE:  pc_cmd_chetaocanchien(playerid, "");
                    case CRAFT_TYPE_RANGED: pc_cmd_chetaovukhi(playerid, "");
                }
                DeletePVar(playerid, "CraftingDialogType");
                DeletePVar(playerid, "CraftingDialogItem");
                DeletePVar(playerid, "CraftingCanCraft");
                return 1;
            }

            if(GetPVarInt(playerid, "CraftingCanCraft") == 0)
            {
                new craftType = GetPVarInt(playerid, "CraftingDialogType");
                switch(craftType)
                {
                    case CRAFT_TYPE_MELEE:  pc_cmd_chetaocanchien(playerid, "");
                    case CRAFT_TYPE_RANGED: pc_cmd_chetaovukhi(playerid, "");
                }
                DeletePVar(playerid, "CraftingDialogType");
                DeletePVar(playerid, "CraftingDialogItem");
                DeletePVar(playerid, "CraftingCanCraft");
                return 1;
            }

            new craftType = GetPVarInt(playerid, "CraftingDialogType");
            new itemid = GetPVarInt(playerid, "CraftingDialogItem");

            if(StartCrafting(playerid, craftType, itemid))
                SetPVarInt(playerid, "CraftCooldown", gettime() + CRAFT_COOLDOWN);

            DeletePVar(playerid, "CraftingDialogType");
            DeletePVar(playerid, "CraftingDialogItem");
            DeletePVar(playerid, "CraftingCanCraft");

            return 1;
        }
    }

    return 0;
}

hook OnPlayerDisconnect(playerid, reason)
{
    if(PlayerCrafting[playerid])
    {
        if(reason == 0)
            StopCrafting(playerid, true);
        else
            StopCrafting(playerid, false);
    }
    return 1;
}

hook OnGameModeInit()
{   
    //NPC nghien cuu che tao sung
    CreateActor(70, 1520.324584, -1458.531616, 9.500000, 2.282761, 0);
    CreateDynamic3DTextLabel("<Tien si~ Gojo>\nTao dang nghien cuu che tao sung, nhin deo' gi`?", -1, 1520.324584, -1458.531616, 9.500000+1.3,5.0);

    //NPC che tao vu khi
    CreateActor(179, 1510.571166, -1457.613037, 9.500000, 270.010864, 0);
    CreateDynamic3DTextLabel("<Chu' Tu>\nNhin` cai' gi? Can hang` thi` chon nhanh roi bien'!!", -1, 1510.571166, -1457.613037, 9.500000+1.3,5.0);

    //NPC che tao can chien
    CreateActor(169, 1536.539428, -1478.990234, 9.500000, 9.153258, 0);
    CreateDynamic3DTextLabel("<Dinh Nguyen>\nSung dan on ao lam... Can thu gi do em ai hon dung khong?", -1, 1536.539428, -1478.990234, 9.500000+1.3,5.0);

    gMeleeWeaponCraftLabel = CreateDynamic3DTextLabel("{FF8C00}>> {FFFF00}KHU CHE TAO VU KHI CAN CHIEN {FF8C00}<<\n{FFFFFF}Lenh: {00FF00}/chetaocanchien\n{AAAAAA}Yeu cau: Vat lieu | Sat | Go", -1, gMeleeWeaponCraftPos[0], gMeleeWeaponCraftPos[1], gMeleeWeaponCraftPos[2] + 0.8, 12.0, .testlos = 1, .worldid = 0);
    gRangedWeaponCraftLabel = CreateDynamic3DTextLabel("{FF4444}>> {FFFF00}KHU CHE TAO SUNG {FF4444}<<\n{FFFFFF}Lenh: {00FF00}/chetaovukhi\n{AAAAAA}Yeu cau: Vat lieu | Sat | Dong | Vang", -1, gRangedWeaponCraftPos[0], gRangedWeaponCraftPos[1], gRangedWeaponCraftPos[2] + 0.8, 12.0, .testlos = 1, .worldid = 0);

    gMeleeWeaponCraftPickup = CreateDynamicPickup(1239, 1, gMeleeWeaponCraftPos[0], gMeleeWeaponCraftPos[1], gMeleeWeaponCraftPos[2], 0);
    gRangedWeaponCraftPickup = CreateDynamicPickup(1239, 1, gRangedWeaponCraftPos[0], gRangedWeaponCraftPos[1], gRangedWeaponCraftPos[2], 0);

    return 1;
}
