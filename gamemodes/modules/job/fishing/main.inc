// System created by W_Codyz


#include "YSI\YSI_Coding\y_hooks"

#define DIALOG_SELLFISH (9001)
#define DIALOG_FISHING_ROD (9002)
#define DIALOG_CAUCA_MENU (9003)
#define DIALOG_DELETE_ROD (9004)
#define DIALOG_FISHING_SHOP (9010)
#define DIALOG_BUY_FISHING_ROD (9011)
#define DIALOG_BUY_BAIT (9012)
#define DIALOG_CONFIRM_BUY_ROD (9013)
#define DIALOG_CONFIRM_BUY_BAIT (9014)
#define MAX_FISH_SPOTS 5
#define FISH_CATCH_TIME 10000 // 10 seconds
#define RESET_HOUR 6 // Gi? reset (6h s�ng)
#define ONE_HOUR 3600000 // 1 gi? = 3600000 ms

#define FISH_SELLER_X -832.286071
#define FISH_SELLER_Y 2098.040283
#define FISH_SELLER_Z 47.550109

#define FISHING_SHOP_X -831.001281
#define FISHING_SHOP_Y 2081.063720
#define FISHING_SHOP_Z 48.769889
// C?n c�u constants
#define MAX_FISHING_RODS 3
#define ROD_DURABILITY_DECREASE_CHANCE 30 // 30% co h?i gi?m d? b?n m?i l?n c�u
#define MAX_ROD_DURABILITY 100
#define MIN_DURABILITY_DECREASE 5 // Gi?m t?i thi?u 5 d? b?n
#define MAX_DURABILITY_DECREASE 10 // Gi?m t?i da 10 d? b?n

// Shop prices
#define FISHING_ROD_PRICE 5000      // Gi� c?n c�u
#define FISHING_BAIT_PRICE 20       // Gi� m?i c�u
#define MAX_BAIT_BUY 100            // T?i da mua 100 m?i/l?n

new LastResetDay = 0;
new LastResetMonth = 0;
new LastResetYear = 0;
new fishingactor;
// Timers
new ResetFishingTimer;

// Variables
new PlayerBaitToBuy[MAX_PLAYERS];

// Fish Types & Prices
enum eFishTypes {
    FISH_CARP,      // C� ch�p - ph? bi?n
    FISH_TILAPIA,   // C� r� phi - ph? bi?n
    FISH_BASS,      // C� l�c - trung b�nh
    FISH_CATFISH,   // C� tr� - trung b�nh
    FISH_SALMON,    // C� h?i - hi?m
    FISH_TUNA,      // C� ng? - hi?m
    FISH_SNAKEHEAD, // C� qu? - hi?m
    FISH_STURGEON,  // C� t?m - r?t hi?m
    FISH_KOI,       // C� koi - r?t hi?m
    FISH_GOLDEN,    // C� v�ng - c?c hi?m
    MAX_FISH_TYPES
}

new const FishNames[MAX_FISH_TYPES][] = {
    "Ca Chep",
    "Ca Ro Phi",
    "Ca Loc",
    "Ca Tre",
    "Ca Hoi",
    "Ca Ngu",
    "Ca Qua",
    "Ca Tam",
    "Ca Koi",
    "Ca Vang"
};

new const FishPrices[MAX_FISH_TYPES] = {
    520,
    580,
    670,
    730,
    850,
    1000,
    1450,
    2200,
    4150,
    7900
};

new const FishRarity[MAX_FISH_TYPES] = {
    300,    // C� ch�p (30.0%)
    250,    // C� r� phi (25.0%)
    150,    // C� l�c (15.0%)
    120,    // C� tr� (12.0%)
    70,     // C� h?i (7.0%)
    60,     // C� ng? (6.0%)
    30,     // C� qu? (3.0%)
    15,     // C� t?m (1.5%)
    4,      // C� koi (0.4%)
    1       // C� v�ng (0.1%)
};

new const Float:FishSizeMin[MAX_FISH_TYPES] = {
    0.5,    // C� ch�p
    0.3,    // C� r� phi
    0.7,    // C� l�c
    0.6,    // C� tr�
    1.0,    // C� h?i
    1.5,    // C� ng?
    0.8,    // C� qu?
    2.0,    // C� t?m
    0.8,    // C� koi
    0.3     // C� v�ng
};

new const Float:FishSizeMax[MAX_FISH_TYPES] = {
    3.0,    // C� ch�p
    1.5,    // C� r� phi
    4.0,    // C� l�c
    2.5,    // C� tr�
    5.0,    // C� h?i
    8.0,    // C� ng?
    3.0,    // C� qu?
    10.0,   // C� t?m
    2.5,    // C� koi
    1.0     // C� v�ng
};

new const FishTypeToPlayerInfo[MAX_FISH_TYPES] = {
    pFishCarp,
    pFishTilapia,
    pFishBass,
    pFishCatfish,
    pFishSalmon,
    pFishTuna,
    pFishSnakehead,
    pFishSturgeon,
    pFishKoi,
    pFishGolden
};

// Fishing Spots
enum eFishingSpots {
    Float:spotX,
    Float:spotY,
    Float:spotZ
}

new Float:FishingSpots[MAX_FISH_SPOTS][eFishingSpots] = {
    {-788.268981, 2129.115234, 47.677726},
    {-791.664794, 2129.493164, 47.677726},
    {-787.173767, 2131.230468, 47.677726},
    {-785.866333, 2139.330566, 47.677726},
    {-793.546142, 2139.651367, 47.677726}
};

// Variables
new bool:IsPlayerFishing[MAX_PLAYERS];
new PlayerFishingTimer[MAX_PLAYERS];
new PlayerCurrentSpot[MAX_PLAYERS];
new PlayerCurrentRod[MAX_PLAYERS]; // C?n c�u hi?n t?i dang s? d?ng
new PlayerSelectedRod[MAX_PLAYERS]; // C?n c�u du?c ch?n d? x�a

GetFishingLimit(playerid) {
    new limit;
    switch(GetFishingLevel(playerid)) {
        case 1: limit = 150;
        case 2: limit = 200;
        case 3: limit = 300;
        case 4: limit = 500;
    }
    return limit;
}

GetFishingExpToNextLevel(playerid) {
    new exp = PlayerInfo[playerid][pFishSkill];
    switch(GetFishingLevel(playerid)) {
        case 1: return 120 - exp;
        case 2: return 300 - exp;
        case 3: return 500 - exp;
        case 4: return 0; // Max level
    }
    return 0;
}

// H�m ki?m tra xem ngu?i choi c� c?n c�u kh? d?ng kh�ng
GetAvailableFishingRod(playerid) {
    for(new i = 0; i < MAX_FISHING_RODS; i++) {
        if(PlayerInfo[playerid][pCanCau][i] == 1 && PlayerInfo[playerid][pDoBenCan][i] > 0) {
            return i;
        }
    }
    return -1; // Kh�ng c� c?n c�u kh? d?ng
}

// H�m d?m s? lu?ng c?n c�u hi?n c�
GetTotalFishingRods(playerid) {
    new count = 0;
    for(new i = 0; i < MAX_FISHING_RODS; i++) {
        if(PlayerInfo[playerid][pCanCau][i] == 1) {
            count++;
        }
    }
    return count;
}

// H�m th�m c?n c�u m?i
AddFishingRod(playerid) {
    for(new i = 0; i < MAX_FISHING_RODS; i++) {
        if(PlayerInfo[playerid][pCanCau][i] == 0) {
            PlayerInfo[playerid][pCanCau][i] = 1;
            PlayerInfo[playerid][pDoBenCan][i] = MAX_ROD_DURABILITY;
            return i; // Tr? v? slot d� th�m
        }
    }
    return -1; // Kh�ng c�n slot tr?ng
}

// H�m hi?n th? menu c�u c� ch�nh
ShowFishingMenu(playerid) {
    new string[512];
    new totalFish = 0, totalRods = GetTotalFishingRods(playerid);

    // �?m t?ng s? c�
    for(new i = 0; i < MAX_FISH_TYPES; i++) {
        totalFish += PlayerInfo[playerid][FishTypeToPlayerInfo[i]];
    }

    new limit = GetFishingLimit(playerid);
    new level = GetFishingLevel(playerid);

    format(string, sizeof(string), "{FFFFFF}=== MENU CAU CA ===\n");
    format(string, sizeof(string), "%s{FFFFFF}Cap do: {00ff00}%d{FFFFFF}\n", string, level);
    format(string, sizeof(string), "%s{FFFFFF}So lan cau hom nay: {b1ff00}%d/%d{FFFFFF}\n", string, PlayerInfo[playerid][pSoLanCauCa], limit);
    format(string, sizeof(string), "%s{FFFFFF}Moi cau: {ffff00}%d{FFFFFF} | Can cau: {ffff00}%d/3{FFFFFF}\n", string, PlayerInfo[playerid][pMoiCau], totalRods);
    format(string, sizeof(string), "%s{FFFFFF}Tong so ca: {00ffff}%d{FFFFFF}\n--------------------\n", string, totalFish);

    strcat(string, "{FFFFFF}1. Xem thong tin ca\n");
    strcat(string, "{FFFFFF}2. Xem thong tin can cau\n");
    strcat(string, "{FFFFFF}3. Xem ky luc ca lon nhat\n");

    ShowPlayerDialog(playerid, DIALOG_CAUCA_MENU, DIALOG_STYLE_LIST, "Menu Cau Ca", string, "Chon", "Dong");
}

// H�m hi?n th? th�ng tin c?n c�u c?a ngu?i choi
ShowFishingRodInfo(playerid) {
    new string[512];
    new totalRods = GetTotalFishingRods(playerid);

    if(totalRods == 0) {
        SendClientMessage(playerid, COLOR_WHITE, "Ban khong co can cau nao!");
        ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong co can cau nao!");
        return 0;
    }

    format(string, sizeof(string), "{FFFFFF}So can cau dang co: %d/%d\n\n", totalRods, MAX_FISHING_RODS);
    strcat(string, "{FFFFFF}STT\t{FFFFFF}Tinh Trang\t{FFFFFF}Do Ben\n");

    for(new i = 0; i < MAX_FISHING_RODS; i++) {
        if(PlayerInfo[playerid][pCanCau][i] == 1) {
            new color[10];
            new status[20];

            if(PlayerInfo[playerid][pDoBenCan][i] <= 0) {
                color = "{FF0000}";
                format(status, sizeof(status), "Hong");
            } else if(PlayerInfo[playerid][pDoBenCan][i] <= 20) {
                color = "{FF0000}";
                format(status, sizeof(status), "Sap Hong");
            } else if(PlayerInfo[playerid][pDoBenCan][i] <= 50) {
                color = "{FFFF00}";
                format(status, sizeof(status), "Trung Binh");
            } else {
                color = "{00FF00}";
                format(status, sizeof(status), "Tot");
            }

            format(string, sizeof(string), "%s%d\t%s%s{FFFFFF}\t%s\t%d/%d{FFFFFF}\n",
                string, i+1, color, status, color, PlayerInfo[playerid][pDoBenCan][i], MAX_ROD_DURABILITY);
        }
    }

    strcat(string, "\n{FFFFFF}Chon can cau de xem chi tiet hoac xoa (neu do ben duoi 100)");

    ShowPlayerDialog(playerid, DIALOG_FISHING_ROD, DIALOG_STYLE_LIST,
        "Thong tin can cau", string, "Chon", "Dong");
    return 1;
}

// H�m x�a c?n c�u
DeleteFishingRod(playerid, rodIndex) {
    if(rodIndex < 0 || rodIndex >= MAX_FISHING_RODS) return 0;
    if(PlayerInfo[playerid][pCanCau][rodIndex] != 1) return 0;

    PlayerInfo[playerid][pCanCau][rodIndex] = 0;
    PlayerInfo[playerid][pDoBenCan][rodIndex] = 0;

    new string[128];
    format(string, sizeof(string), "Ban da vut bo can cau so %d!", rodIndex + 1);
    SendClientMessage(playerid, COLOR_YELLOW, string);
    return 1;
}

// H�m gi?m d? b?n c?n c�u
DecreaseFishingRodDurability(playerid, rodIndex) {
    if(rodIndex < 0 || rodIndex >= MAX_FISHING_RODS) return 0;
    if(PlayerInfo[playerid][pCanCau][rodIndex] != 1) return 0;

    // Ki?m tra co h?i gi?m d? b?n (30%)
    if(random(100) < ROD_DURABILITY_DECREASE_CHANCE) {
        new decrease = MIN_DURABILITY_DECREASE + random(MAX_DURABILITY_DECREASE - MIN_DURABILITY_DECREASE + 1);
        PlayerInfo[playerid][pDoBenCan][rodIndex] -= decrease;

        if(PlayerInfo[playerid][pDoBenCan][rodIndex] <= 0) {
            // C?n c�u b? h?ng
            PlayerInfo[playerid][pDoBenCan][rodIndex] = 0;
            PlayerInfo[playerid][pCanCau][rodIndex] = 0; // X�a c?n c�u

            new string[128];
            format(string, sizeof(string), "{FF0000}Can cau so %d cua ban da bi hong!", rodIndex + 1);
            SendClientMessage(playerid, COLOR_WHITE, string);

            // N?u kh�ng c�n c?n c�u n�o kh? d?ng
            if(GetAvailableFishingRod(playerid) == -1) {
                SendClientMessage(playerid, COLOR_WHITE, "Ban khong con can cau nao de su dung!");
                ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong con can cau nao de su dung!");
            }
        } else {
            // Th�ng b�o d? b?n gi?m
            if(PlayerInfo[playerid][pDoBenCan][rodIndex] <= 20) {
                new string[128];
                format(string, sizeof(string), "{FFFF00}Can cau so %d sap hong! Do ben con lai: %d/%d",
                    rodIndex + 1, PlayerInfo[playerid][pDoBenCan][rodIndex], MAX_ROD_DURABILITY);
                SendClientMessage(playerid, COLOR_WHITE, string);
            }
        }

        return 1; // �? b?n d� gi?m
    }

    return 0; // �? b?n kh�ng gi?m
}

// H�m d? l?y c?p d? k? nang c�u c� c?a ngu?i choi
GetFishingLevel(playerid) {
    if(PlayerInfo[playerid][pFishSkill] >= 0 && PlayerInfo[playerid][pFishSkill] < 120) {
        return 1;
    }
    else if(PlayerInfo[playerid][pFishSkill] >= 120 && PlayerInfo[playerid][pFishSkill] < 300) {
        return 2;
    }
    else if(PlayerInfo[playerid][pFishSkill] >= 300 && PlayerInfo[playerid][pFishSkill] < 500) {
        return 3;
    }
    else if(PlayerInfo[playerid][pFishSkill] >= 500) {
        return 4;
    }
    return 1; // M?c d?nh l� c?p 1
}
/*
// H�m d? l?y s? kinh nghi?m c?n thi?t cho c?p d? ti?p theo
GetNextLevelExp(playerid) {
    new level = GetFishingLevel(playerid);

    switch(level) {
        case 1: return 120; // C?n 120 exp d? l�n c?p 2
        case 2: return 300; // C?n 300 exp d? l�n c?p 3
        case 3: return 500; // C?n 500 exp d? l�n c?p 4
        case 4: return 900; // M?c ti?p theo (n?u c� c?p cao hon trong tuong lai)
    }

    return 0;
}
*/
// Set up daily reset timer
SetupDailyReset() {
    new
        hour,
        minute,
        second;

    gettime(hour, minute, second);

    // T�nh th?i gian d?n 6h s�ng ng�y h�m sau
    new timeUntilReset;
    if(hour >= RESET_HOUR) {
        // N?u d� qua 6h s�ng, d?i d?n 6h s�ng ng�y mai
        timeUntilReset = (24 - (hour - RESET_HOUR)) * ONE_HOUR;
    } else {
        // N?u chua t?i 6h s�ng, d?i d?n 6h s�ng h�m nay
        timeUntilReset = (RESET_HOUR - hour) * ONE_HOUR;
    }

    // Tr? di s? ph�t v� gi�y d� qua
    timeUntilReset -= (minute * 60000) + (second * 1000);

    // Set timer
    if(ResetFishingTimer) KillTimer(ResetFishingTimer);
    ResetFishingTimer = SetTimerEx("ResetDailyFishing", timeUntilReset, false, "");
}

// Reset fishing counts for all players
forward ResetDailyFishing();
public ResetDailyFishing() {
    new currentYear, currentMonth, currentDay;
    getdate(currentYear, currentMonth, currentDay);

    // T?o gi� tr? YYYYMMDD cho ng�y h�m nay
    new today = (currentYear * 10000) + (currentMonth * 100) + currentDay;

    // Reset s? l?n c�u c� cho t?t c? ngu?i choi online
    foreach(new i : Player) {
        if(IsPlayerConnected(i)) {
            PlayerInfo[i][pSoLanCauCa] = 0;
            PlayerInfo[i][pLastFishingReset] = today;

            new limit = GetFishingLimit(i);
            new string[128];
            format(string, sizeof(string), "So lan cau ca hom nay da duoc lam moi! Ban co the cau toi da %d lan hom nay.", limit);
            SendClientMessage(i, COLOR_YELLOW, string);
        }
    }

    // Luu th�ng tin reset
    LastResetDay = currentDay;
    LastResetMonth = currentMonth;
    LastResetYear = currentYear;
    SaveLastResetInfo();

    // Set up timer cho ng�y mai
    SetupDailyReset();
}

// Function to initialize fishing spots and timers
InitFishingSystem() {
    // T?i th�ng tin reset t? file
    LoadLastResetInfo();
    Load_FishingShop();

    CheckAndResetIfNeeded();

    SetTimer("RegularResetCheck", 300000, true); // 15 ph�t = 900000ms 5p = 300000

    printf("[FISHING] System initialized, last reset: %d/%d/%d", LastResetDay, LastResetMonth, LastResetYear);
}

SaveLastResetInfo() {
    new File:file = fopen("fishing_reset.dat", io_write);
    if(file) {
        new data[64];
        format(data, sizeof(data), "%d %d %d", LastResetDay, LastResetMonth, LastResetYear);
        fwrite(file, data);
        fclose(file);
        printf("[FISHING] Saved reset info: %s", data);
    }
}

// H�m t?i th�ng tin reset
LoadLastResetInfo() {
    if(fexist("fishing_reset.dat")) {
        new File:file = fopen("fishing_reset.dat", io_read);
        if(file) {
            new data[64];
            fread(file, data);
            fclose(file);

            // Ph�n t�ch d? li?u
            sscanf(data, "ddd", LastResetDay, LastResetMonth, LastResetYear);
            printf("[FISHING] Loaded reset info: %d/%d/%d", LastResetDay, LastResetMonth, LastResetYear);
        }
    } else {
        // N?u kh�ng c� file, l?y ng�y hi?n t?i l�m m?c
        new year, month, day;
        getdate(year, month, day);
        LastResetDay = day;
        LastResetMonth = month;
        LastResetYear = year;
        SaveLastResetInfo();
    }
}

// H�m ki?m tra v� reset n?u c?n
CheckAndResetIfNeeded() {
    new currentYear, currentMonth, currentDay, currentHour;
    getdate(currentYear, currentMonth, currentDay);
    gettime(currentHour, _, _);

    // T?o gi� tr? YYYYMMDD cho ng�y h�m nay v� ng�y reset cu?i c�ng
    new today = (currentYear * 10000) + (currentMonth * 100) + currentDay;
    new lastReset = (LastResetYear * 10000) + (LastResetMonth * 100) + LastResetDay;

    // Ki?m tra xem d� qua ng�y m?i v� d� qua gi? reset chua
    new bool:needReset = (today > lastReset && currentHour >= RESET_HOUR);

    // N?u c?n reset, th?c hi?n reset
    if(needReset) {
        printf("[FISHING] Reset needed! Current: %d/%d/%d %d:00, Last: %d/%d/%d",
            currentDay, currentMonth, currentYear, currentHour,
            LastResetDay, LastResetMonth, LastResetYear);

        ResetAllPlayersFishingCount();

        // C?p nh?t th�ng tin reset
        LastResetDay = currentDay;
        LastResetMonth = currentMonth;
        LastResetYear = currentYear;
        SaveLastResetInfo();
    } else {
        printf("[FISHING] No reset needed. Current: %d/%d/%d %d:00",
            currentDay, currentMonth, currentYear, currentHour);
    }
}

forward RegularResetCheck();
public RegularResetCheck() {
    CheckAndResetIfNeeded();
}

ResetAllPlayersFishingCount() {
    new count = 0;
    new currentYear, currentMonth, currentDay;
    getdate(currentYear, currentMonth, currentDay);

    // T?o gi� tr? YYYYMMDD cho ng�y h�m nay
    new today = (currentYear * 10000) + (currentMonth * 100) + currentDay;

    foreach(new i : Player) {
        if(IsPlayerConnected(i)) {
            PlayerInfo[i][pSoLanCauCa] = 0;
            PlayerInfo[i][pLastFishingReset] = today;
            SendClientMessage(i, COLOR_YELLOW, "Gioi han cau ca da duoc lam moi! Ban co the cau toi da 60 lan hom nay.");
            count++;
        }
    }

    printf("[FISHING] Reset completed for %d online players", count);
}

GetRandomFishType() {
    new randValue = random(1000);
    new cumulativeRarity = 0;

    for(new i = 0; i < MAX_FISH_TYPES; i++) {
        cumulativeRarity += FishRarity[i];
        if(randValue < cumulativeRarity) {
            return i;
        }
    }
    return FISH_CARP; // M?c d?nh n?u c� l?i
}

Float:GetRandomFishSize(fishType) {
    new Float:minSize = FishSizeMin[fishType];
    new Float:maxSize = FishSizeMax[fishType];

    // S? d?ng c�ng th?c d? t?o s? th?p ph�n ng?u nhi�n gi?a min v� max
    return minSize + ((maxSize - minSize) * (float(random(1000)) / 1000.0));
}

// Check if player is at fishing spot
IsPlayerAtFishingSpot(playerid) {
    if(!IsPlayerConnected(playerid)) return -1;

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    for(new i = 0; i < MAX_FISH_SPOTS; i++) {
        if(IsPlayerInRangeOfPoint(playerid, 1.0, FishingSpots[i][spotX], FishingSpots[i][spotY], FishingSpots[i][spotZ])) {
            return i;
        }
    }
    return -1;
}

stock Load_Fishing()
{
    for(new i = 0; i < MAX_FISH_SPOTS; i++) {
        CreateDynamicPickup(1239, 23, FishingSpots[i][spotX], FishingSpots[i][spotY], FishingSpots[i][spotZ]);
	    CreateDynamic3DTextLabel("An {ffff00}[Y]{FFFFFF}\nDe cau ca\nKP5", -1, FishingSpots[i][spotX], FishingSpots[i][spotY], FishingSpots[i][spotZ], 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1);
    }
    CreateDynamicPickup(1239, 23, FISH_SELLER_X, FISH_SELLER_Y, FISH_SELLER_Z);
	CreateDynamic3DTextLabel("{FFFFFF}--Noi ban ca KP5--\nAn{ffff00}[Y]{FFFFFF}\nDe ban ca", -1, FISH_SELLER_X, FISH_SELLER_Y, FISH_SELLER_Z, 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1);
}

// Start fishing
StartFishing(playerid) {
    if (PlayerInfo[playerid][pLevel] <= 1)
    {
        SendClientMessage(playerid, COLOR_GREY, "Ban can phai dat level 2 moi co the lam cong viec nay.");
        ShowTDN(playerid, NOTIFICATION_WARNING, "Ban can phai dat level 2 moi co the lam cong viec nay.");
        return 1;
    }
    if(IsPlayerFishing[playerid]) {
        StopFishing(playerid);
        return 1;
    }

    // Ki?m tra c?n c�u kh? d?ng
    new rodIndex = GetAvailableFishingRod(playerid);
    if(rodIndex == -1) {
        SendClientMessage(playerid, COLOR_WHITE, "Ban khong co can cau hoac tat ca can cau da hong!");
        ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong co can cau hoac tat ca can cau da hong!");
        return 0;
    }

    if(PlayerInfo[playerid][pMoiCau] <= 0) {
        SendClientMessage(playerid, COLOR_WHITE, "Ban khong co moi cau!");
        ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong co moi cau!");
        return 0;
    }

    // Ki?m tra t?ng s? c�
    new totalFish = PlayerInfo[playerid][pFishCarp] + PlayerInfo[playerid][pFishTuna] +
                    PlayerInfo[playerid][pFishSalmon] + PlayerInfo[playerid][pFishBass];

    if(totalFish >= 20) {
        SendClientMessage(playerid, COLOR_WHITE, "Tui do ca da day! Ban can di ban bot ca truoc khi tiep tuc cau.");
        ShowTDN(playerid, NOTIFICATION_WARNING, "Tui do ca da day!");
        return 0;
    }

    new spotid = IsPlayerAtFishingSpot(playerid);
    if(spotid == -1) {
        SendClientMessage(playerid, COLOR_WHITE, "Ban khong o dia diem cau ca!");
        ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong o dia diem cau ca!");
        return 0;
    }

    IsPlayerFishing[playerid] = true;
    PlayerCurrentSpot[playerid] = spotid;
    PlayerCurrentRod[playerid] = rodIndex;
    PlayerInfo[playerid][pMoiCau]--;

    TogglePlayerControllable(playerid, 0);
    ApplyAnimation(playerid, "SWORD", "sword_block", 50.0, 0, 1, 0, 1, 1);
    SetPlayerAttachedObject(playerid, 9, 18632, 6, 0.09, 0.01, 0.00, 183.0, -6.0, 0.0, 1.00, 1.00, 1.00);

    new string[128];
    format(string, sizeof(string), "Ban da bat dau cau ca voi can cau so {b1ff00}%d{ffffff}... Cho doi trong giay lat.", rodIndex + 1);
    SendClientMessage(playerid, COLOR_WHITE, string);
    SendClientMessage(playerid, COLOR_WHITE, "An {b1ff00}[Y] {ffffff}lan nua de ngung cau ca.");
    ShowTDN(playerid, NOTIFICATION_INFO, "An [Y] lan nua de ngung cau ca.");

    new ThoiGianCau = FISH_CATCH_TIME + Random(1000, 5000);
    PlayerFishingTimer[playerid] = SetTimerEx("CatchFish", ThoiGianCau, false, "i", playerid);
    return 1;
}

// Stop fishing
StopFishing(playerid) {
    if(!IsPlayerFishing[playerid]) return 0;

    IsPlayerFishing[playerid] = false;
    TogglePlayerControllable(playerid, 1);
    RemovePlayerAttachedObject(playerid, 9);
    ClearAnimations(playerid);
    KillTimer(PlayerFishingTimer[playerid]);
    PlayerInfo[playerid][pMoiCau]++;
    SendClientMessage(playerid, COLOR_WHITE, "Ban da ngung cau ca.");
    ShowTDN(playerid, NOTIFICATION_INFO, "Ban da ngung cau ca.");
    return 1;
}

// Catch fish timer callback
forward CatchFish(playerid);
public CatchFish(playerid) {
    if(!IsPlayerFishing[playerid]) return 0;

    new limit = GetFishingLimit(playerid);
    if(PlayerInfo[playerid][pSoLanCauCa] >= limit) {
        new string[128];
        format(string, sizeof(string), "Ban da dat gioi han %d lan cau ca hom nay!", limit);
        SendClientMessage(playerid, COLOR_WHITE, string);
        StopFishing(playerid);
        return 0;
    }

    new rodIndex = PlayerCurrentRod[playerid];

    // X? l� d? b?n c?n c�u
    DecreaseFishingRodDurability(playerid, rodIndex);

    // Ki?m tra xem c?n c�u c�n s? d?ng du?c kh�ng
    if(PlayerInfo[playerid][pDoBenCan][rodIndex] <= 0) {
        SendClientMessage(playerid, COLOR_WHITE, "Can cau cua ban da hong! Hay tim can cau khac.");
        ShowTDN(playerid, NOTIFICATION_WARNING, "Can cau cua ban da hong!");
        StopFishing(playerid);
        return 0;
    }

    // Ch?n lo?i c� ng?u nhi�n d?a tr�n d? hi?m
    new fishType = GetRandomFishType();

    // Sinh k�ch thu?c ng?u nhi�n
    new Float:fishSize = GetRandomFishSize(fishType);

    // L�m tr�n k�ch thu?c d?n 2 ch? s? th?p ph�n d? hi?n th?
    new Float:displaySize = float(floatround(fishSize * 100)) / 100.0;

    // Tang s? lu?ng c� theo lo?i
    PlayerInfo[playerid][FishTypeToPlayerInfo[fishType]]++;

    // C?p nh?t k? l?c c� l?n nh?t n?u c?n
    if(fishSize > PlayerInfo[playerid][pBiggestFishSize]) {
        PlayerInfo[playerid][pBiggestFishSize] = fishSize;
        PlayerInfo[playerid][pBiggestFishType] = fishType;
    }

    PlayerInfo[playerid][pSoLanCauCa]++;

    new oldLevel = GetFishingLevel(playerid);
    PlayerInfo[playerid][pFishSkill]++;
    new newLevel = GetFishingLevel(playerid);

    if(oldLevel < newLevel && oldLevel > 0) {
        // Th�ng b�o l�n c?p
        new levelUpMsg[128];
        format(levelUpMsg, sizeof(levelUpMsg), "~g~LEVEL UP!~w~ Ky nang cau ca cua ban da tang len cap %d!", newLevel);
        GameTextForPlayer(playerid, levelUpMsg, 5000, 3);

        new string[128];
        format(string, sizeof(string), "Ky nang cau ca cua ban da tang len cap %d! Ban co the cau toi da %d lan moi ngay.",
            newLevel, GetFishingLimit(playerid));
        SendClientMessage(playerid, COLOR_GREEN, string);
    }

    // Th�ng b�o v?i m�u d?a theo d? hi?m
    new string[256];
    new fishColor[10];

    if(fishType >= FISH_GOLDEN) fishColor = "{FFD700}"; // V�ng (c?c hi?m)
    else if(fishType >= FISH_KOI) fishColor = "{FF00FF}"; // T�m (r?t hi?m)
    else if(fishType >= FISH_STURGEON) fishColor = "{0000FF}"; // Xanh duong (hi?m)
    else if(fishType >= FISH_SALMON) fishColor = "{00FFFF}"; // Lam (trung b�nh-hi?m)
    else if(fishType >= FISH_CATFISH) fishColor = "{00FF00}"; // Xanh l� (trung b�nh)
    else fishColor = "{FFFFFF}"; // Tr?ng (ph? bi?n)

    format(string, sizeof(string), "Ban da cau duoc mot con %s%s{FFFFFF} nang %.2f kg! (So lan cau hom nay: {b1ff00}%d/%d{FFFFFF})",
        fishColor, FishNames[fishType], displaySize, PlayerInfo[playerid][pSoLanCauCa], limit);
    SendClientMessageEx(playerid, COLOR_WHITE, string);
    format(string, sizeof(string), "~w~Xin chuc mung~n~~p~Ban da cau duoc mot con~n~~g~ %s", FishNames[fishType]);
    GameTextForPlayer(playerid, string, 10000, 4);

    // Hi?u ?ng d?c bi?t cho c� hi?m
    if(fishType >= FISH_STURGEON) {
        new announce[128];
        format(announce, sizeof(announce), "~y~%s da cau duoc mot con %s quy hiem!", GetPlayerNameEx(playerid), FishNames[fishType]);
        foreach(new i : Player) {
            if(IsPlayerConnected(i) && GetPlayerVirtualWorld(i) == GetPlayerVirtualWorld(playerid)) {
                GameTextForPlayer(i, announce, 5000, 3);
            }
        }
    }

    // Reset tr?ng th�i c�u c�
    IsPlayerFishing[playerid] = false;
    TogglePlayerControllable(playerid, 1);
    RemovePlayerAttachedObject(playerid, 9);
    ClearAnimations(playerid);
    KillTimer(PlayerFishingTimer[playerid]);
    return 1;
}

// Sell fish
SellFish(playerid) {
    new totalFish = 0;

    // �?m t?ng s? c�
    for(new i = 0; i < MAX_FISH_TYPES; i++) {
        totalFish += PlayerInfo[playerid][FishTypeToPlayerInfo[i]];
    }

    if(totalFish <= 0) {
        SendClientMessage(playerid, COLOR_WHITE, "Ban khong co ca de ban!");
        ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong co ca de ban!");
        return 0;
    }

    new string[1024];
    strcat(string, "{FFFFFF}Loai Ca\t{FFFFFF}So Luong\t{FFFFFF}Gia Ban\n");

    // Hi?n th? th�ng tin t?ng lo?i c�
    for(new i = 0; i < MAX_FISH_TYPES; i++) {
        new count = PlayerInfo[playerid][FishTypeToPlayerInfo[i]];
        if(count > 0) {
            new fishColor[10];

            if(i >= FISH_GOLDEN) fishColor = "{FFD700}"; // V�ng
            else if(i >= FISH_KOI) fishColor = "{FF00FF}"; // T�m
            else if(i >= FISH_STURGEON) fishColor = "{0000FF}"; // Xanh duong
            else if(i >= FISH_SALMON) fishColor = "{00FFFF}"; // Lam
            else if(i >= FISH_CATFISH) fishColor = "{00FF00}"; // Xanh l�
            else fishColor = "{FFFFFF}"; // Tr?ng

            format(string, sizeof(string), "%s%s%s\t%d\t$%d/ 1 con\n",
                string, fishColor, FishNames[i], count, FishPrices[i]);
        }
    }

    strcat(string, "\n{FFFFFF}Ban co muon ban tat ca so ca?");

    ShowPlayerDialog(playerid, DIALOG_SELLFISH, DIALOG_STYLE_LIST, "Ban Ca", string, "Dong y", "Huy bo");
    return 1;
}

// Dialog response handler
hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) {
    if(dialogid == DIALOG_SELLFISH) {
        if(!response) return 1;

        new totalPrice = 0;
        new totalFishSold = 0;

        // T�nh to�n gi� tr? v� d?t l?i s? lu?ng c�
        for(new i = 0; i < MAX_FISH_TYPES; i++) {
            new count = PlayerInfo[playerid][FishTypeToPlayerInfo[i]];
            if(count > 0) {
                totalPrice += count * FishPrices[i];
                totalFishSold += count;
                PlayerInfo[playerid][FishTypeToPlayerInfo[i]] = 0;
            }
        }

        PlayerInfo[playerid][pCash] += totalPrice;

        new string[128];
        format(string, sizeof(string), "Ban da ban %d con ca va nhan duoc $%d!", totalFishSold, totalPrice);
        SendClientMessage(playerid, COLOR_GREEN, string);
        new title[64];
        format(title, sizeof(title), "%s (ID: %d) - Bán cá", GetPlayerNameEx(playerid), playerid);
        DiscordLog(dclog_Fishing, title, "**Số cá bán: %d con**\n**Tiền nhận được: $%d**", totalFishSold, totalPrice);
        return 1;
    }
    else if(dialogid == DIALOG_CAUCA_MENU) {
        if(!response) return 1;

        switch(listitem) {
            case 6: { // Xem th�ng tin c�
                new totalFish = 0;

                // �?m t?ng s? c�
                for(new i = 0; i < MAX_FISH_TYPES; i++) {
                    totalFish += PlayerInfo[playerid][FishTypeToPlayerInfo[i]];
                }

                if(totalFish <= 0) {
                    SendClientMessage(playerid, COLOR_WHITE, "Ban khong co ca nao!");
                    ShowTDN(playerid, NOTIFICATION_INFO, "Ban khong co ca nao!");
                    return 1;
                }

                SendClientMessage(playerid, COLOR_GREEN, "---- Ca cua ban ----");

                // Hi?n th? th�ng tin t?ng lo?i c�
                for(new i = 0; i < MAX_FISH_TYPES; i++) {
                    new count = PlayerInfo[playerid][FishTypeToPlayerInfo[i]];
                    if(count > 0) {
                        new string[128];
                        format(string, sizeof(string), "%s: %d con (Gia: $%d/con)", FishNames[i], count, FishPrices[i]);
                        SendClientMessage(playerid, COLOR_WHITE, string);
                    }
                }
            }
            case 7: { // Xem th�ng tin c?n c�u
                ShowFishingRodInfo(playerid);
            }
            case 8: { // Xem k? l?c c� l?n nh?t
                if(PlayerInfo[playerid][pBiggestFishSize] > 0.0) {
                    new string[128];
                    format(string, sizeof(string), "Ky luc ca lon nhat cua ban: %s (%.2f kg)",
                        FishNames[PlayerInfo[playerid][pBiggestFishType]],
                        PlayerInfo[playerid][pBiggestFishSize]);
                    SendClientMessage(playerid, COLOR_YELLOW, string);
                } else {
                    SendClientMessage(playerid, COLOR_WHITE, "Ban chua cau duoc con ca nao!");
                    ShowTDN(playerid, NOTIFICATION_INFO, "Ban chua cau duoc con ca nao!");
                }
            }
        }
        return 1;
    }
    else if(dialogid == DIALOG_FISHING_ROD) {
        if(!response) return 1;

        // V� s? d?ng DIALOG_STYLE_LIST v?i header, c�c c?n c�u b?t d?u t? index 2
        if(listitem < 2) return 1; // B? qua header

        // T�m c?n c�u du?c ch?n d?a tr�n listitem (tr? di 2 d�ng header)
        new selectedRod = -1;
        new currentItem = 0;
        new targetItem = listitem - 2; // Tr? di 2 d�ng header

        for(new i = 0; i < MAX_FISHING_RODS; i++) {
            if(PlayerInfo[playerid][pCanCau][i] == 1) {
                if(currentItem == targetItem) {
                    selectedRod = i;
                    break;
                }
                currentItem++;
            }
        }

        if(selectedRod == -1) return 1;

        PlayerSelectedRod[playerid] = selectedRod;

        // Ki?m tra d? b?n
        if(PlayerInfo[playerid][pDoBenCan][selectedRod] >= MAX_ROD_DURABILITY) {
            new string[128];
            format(string, sizeof(string), "Can cau so %d dang o tinh trang hoan hao (100/100 do ben).\nKhong the vut bo can cau nay!", selectedRod + 1);
            SendClientMessage(playerid, COLOR_WHITE, string);
            return 1;
        }

        // Hi?n th? dialog x�c nh?n x�a
        new string[256];
        format(string, sizeof(string), "{FFFFFF}Can cau so: {ffff00}%d{FFFFFF}\n", selectedRod + 1);
        format(string, sizeof(string), "%s{FFFFFF}Do ben hien tai: {ffff00}%d/%d{FFFFFF}\n\n", string, PlayerInfo[playerid][pDoBenCan][selectedRod], MAX_ROD_DURABILITY);
        strcat(string, "{FF0000}Ban co chac chan muon vut bo can cau nay khong?\n");
        strcat(string, "{FFFFFF}Hanh dong nay khong the hoan tac!");

        ShowPlayerDialog(playerid, DIALOG_DELETE_ROD, DIALOG_STYLE_MSGBOX, "Xac nhan vut bo can cau", string, "Vut bo", "Huy");
        return 1;
    }
    else if(dialogid == DIALOG_DELETE_ROD) {
        if(!response) return 1;

        new rodIndex = PlayerSelectedRod[playerid];
        DeleteFishingRod(playerid, rodIndex);
        return 1;
    }

    else if(dialogid == DIALOG_FISHING_SHOP) {
            if(!response) return 1;

            switch(listitem) {
                case 6: ShowBuyFishingRod(playerid);     // Mua c?n c�u
                case 7: ShowBuyBait(playerid);           // Mua m?i c�u
            }
        }

    else if(dialogid == DIALOG_CONFIRM_BUY_ROD) {
            if(!response) return 1;
            ProcessBuyFishingRod(playerid);
        }

    else if(dialogid == DIALOG_BUY_BAIT) {
            if(!response) return 1;

            new amount = 0;
            new money = PlayerInfo[playerid][pCash];
            new maxCanBuy = money / FISHING_BAIT_PRICE;
            if(maxCanBuy > MAX_BAIT_BUY) maxCanBuy = MAX_BAIT_BUY;

            switch(listitem) {
                case 7: amount = 1;           // 1 c�i
                case 8: amount = 5;           // 5 c�i
                case 9: amount = 10;          // 10 c�i
                case 10: amount = 25;          // 25 c�i
                case 11: amount = 50;          // 50 c�i
                case 12: amount = 100;         // 100 c�i
                default: return 1;
            }

            // Ki?m tra s? lu?ng h?p l?
            if(amount > maxCanBuy) amount = maxCanBuy;
            if(amount <= 0) {
                SendClientMessage(playerid, COLOR_WHITE, "So luong khong hop le!");
                ShowTDN(playerid, NOTIFICATION_WARNING, "So luong khong hop le!");
                return 1;
            }

            ShowConfirmBuyBait(playerid, amount);
        }

    else if(dialogid == DIALOG_CONFIRM_BUY_BAIT) {
        if(!response) return 1;
        ProcessBuyBait(playerid, PlayerBaitToBuy[playerid]);
    }
    return 0;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    if(newkeys & KEY_YES) {
        if(IsPlayerInRangeOfPoint(playerid, 3.0, FISH_SELLER_X, FISH_SELLER_Y, FISH_SELLER_Z)) {
            SellFish(playerid);
            return 1;
        }

        if(IsPlayerAtFishingSpot(playerid) != -1) {
            StartFishing(playerid);
            return 1;
        }

        if(IsPlayerInRangeOfPoint(playerid, 3.0, FISHING_SHOP_X, FISHING_SHOP_Y, FISHING_SHOP_Z)) {
            ShowFishingShop(playerid);
            return 1;
        }
    }
    return 0;
}

hook OnPlayerSpawn(playerid) {
    new currentYear, currentMonth, currentDay, currentHour;
    getdate(currentYear, currentMonth, currentDay);
    gettime(currentHour, _, _);

    // Chuy?n d?i ng�y hi?n t?i th�nh s? nguy�n YYYYMMDD d? d? so s�nh
    new today = (currentYear * 10000) + (currentMonth * 100) + currentDay;

    // Ki?m tra n?u d� qua ng�y m?i v� d� qua gi? reset
    if (currentHour >= RESET_HOUR && PlayerInfo[playerid][pLastFishingReset] < today) {
        // Reset cho ngu?i choi n�y
        PlayerInfo[playerid][pSoLanCauCa] = 0;
        PlayerInfo[playerid][pLastFishingReset] = today;
        SendClientMessage(playerid, COLOR_YELLOW, "So lan cau ca hom nay da duoc reset.");
    }

    return 1;
}

// H�m hi?n th? menu c?a h�ng c�u c�
ShowFishingShop(playerid) {
    if (PlayerInfo[playerid][pLevel] <= 1)
    {
        SendClientMessage(playerid, COLOR_GREY, "Ban can phai dat level 2 moi co the lam cong viec nay.");
        ShowTDN(playerid, NOTIFICATION_WARNING, "Ban can phai dat level 2 moi co the lam cong viec nay.");
        return 1;
    }
    new string[512];
    new money = PlayerInfo[playerid][pCash];
    new totalRods = GetTotalFishingRods(playerid);
    new currentBait = PlayerInfo[playerid][pMoiCau];

    format(string, sizeof(string), "{FFFFFF}=== CUA HANG CAU CA ===\n");
    format(string, sizeof(string), "%s{FFFFFF}Tien cua ban: {00ff00}$%s{FFFFFF}\n", string, number_format(money));
    format(string, sizeof(string), "%s{FFFFFF}Can cau hien tai: {ffff00}%d/3{FFFFFF}\n", string, totalRods);
    format(string, sizeof(string), "%s{FFFFFF}Moi cau hien tai: {ffff00}%d{FFFFFF}\n------------------\n", string, currentBait);

    strcat(string, "{FFFFFF}San pham ban:\n");
    format(string, sizeof(string), "%s{FFFFFF}1. Can cau - {00ff00}$%s{FFFFFF}\n", string, number_format(FISHING_ROD_PRICE));
    format(string, sizeof(string), "%s{FFFFFF}2. Moi cau - {00ff00}$%d{FFFFFF}/cai\n\n", string, FISHING_BAIT_PRICE);

    strcat(string, "{FFFF00}Chon san pham ban muon mua:");

    ShowPlayerDialog(playerid, DIALOG_FISHING_SHOP, DIALOG_STYLE_LIST, "Cua hang cau ca", string, "Chon", "Dong");
}

// H�m x? l� mua c?n c�u
ShowBuyFishingRod(playerid) {
    new totalRods = GetTotalFishingRods(playerid);
    new money = PlayerInfo[playerid][pCash];

    if(totalRods >= MAX_FISHING_RODS) {
        SendClientMessage(playerid, COLOR_WHITE, "Ban da co du 3 can cau roi! Khong the mua them.");
        ShowTDN(playerid, NOTIFICATION_WARNING, "Ban da co du 3 can cau roi!");
        return 0;
    }

    if(money < FISHING_ROD_PRICE) {
        new string[128];
        format(string, sizeof(string), "Ban khong du tien! Can $%s de mua can cau.", number_format(FISHING_ROD_PRICE));
        SendClientMessage(playerid, COLOR_WHITE, string);
        return 0;
    }

    new string[256];
    format(string, sizeof(string), "{FFFFFF}=== MUA CAN CAU ===\n");
    format(string, sizeof(string), "%s{FFFFFF}Gia: {00ff00}$%s{FFFFFF}\n", string, number_format(FISHING_ROD_PRICE));
    format(string, sizeof(string), "%s{FFFFFF}Do ben: {00ff00}%d/100{FFFFFF}\n", string, MAX_ROD_DURABILITY);
    format(string, sizeof(string), "%s{FFFFFF}Can cau hien tai: {ffff00}%d/3{FFFFFF}\n\n", string, totalRods);

    strcat(string, "{FFFFFF}Ban co muon mua can cau nay khong?");

    ShowPlayerDialog(playerid, DIALOG_CONFIRM_BUY_ROD, DIALOG_STYLE_MSGBOX, "Xac nhan mua can cau", string, "Mua", "Huy");
    return 1;
}

// H�m x? l� mua m?i c�u
ShowBuyBait(playerid) {
    new money = PlayerInfo[playerid][pCash];
    new maxCanBuy = money / FISHING_BAIT_PRICE;

    if(maxCanBuy <= 0) {
        new string[128];
        format(string, sizeof(string), "Ban khong du tien! Can it nhat $%d de mua 1 moi cau.", FISHING_BAIT_PRICE);
        SendClientMessage(playerid, COLOR_WHITE, string);
        return 0;
    }

    if(maxCanBuy > MAX_BAIT_BUY) maxCanBuy = MAX_BAIT_BUY;

    new string[512];
    format(string, sizeof(string), "{FFFFFF}=== MUA MOI CAU ===\n");
    format(string, sizeof(string), "%s{FFFFFF}Gia: {00ff00}$%d{FFFFFF}/cai\n", string, FISHING_BAIT_PRICE);
    format(string, sizeof(string), "%s{FFFFFF}Tien cua ban: {00ff00}$%s{FFFFFF}\n", string, number_format(money));
    format(string, sizeof(string), "%s{FFFFFF}Moi cau hien tai: {ffff00}%d{FFFFFF}\n", string, PlayerInfo[playerid][pMoiCau]);
    format(string, sizeof(string), "%s{FFFFFF}Toi da co the mua: {ffff00}%d{FFFFFF} cai\n----------------------\n", string, maxCanBuy);

    strcat(string, "{FFFFFF}Chon so luong muon mua:\n");
    strcat(string, "{FFFFFF}1. Mua 1 cai\n");
    strcat(string, "{FFFFFF}2. Mua 5 cai\n");
    strcat(string, "{FFFFFF}3. Mua 10 cai\n");
    strcat(string, "{FFFFFF}4. Mua 25 cai\n");
    strcat(string, "{FFFFFF}5. Mua 50 cai\n");
    if(maxCanBuy >= 100) {
        strcat(string, "{FFFFFF}6. Mua 100 cai\n");
    }

    ShowPlayerDialog(playerid, DIALOG_BUY_BAIT, DIALOG_STYLE_LIST, "Mua moi cau", string, "Chon", "Huy");
    return 1;
}

// H�m x�c nh?n mua m?i c�u
ShowConfirmBuyBait(playerid, amount) {
    new totalPrice = amount * FISHING_BAIT_PRICE;
    new money = PlayerInfo[playerid][pCash];

    if(totalPrice > money) {
        SendClientMessage(playerid, COLOR_WHITE, "Ban khong du tien de mua so luong nay!");
        ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong du tien de mua so luong nay!");
        return 0;
    }

    PlayerBaitToBuy[playerid] = amount;

    new string[256];
    format(string, sizeof(string), "{FFFFFF}=== XAC NHAN MUA MOI CAU ===\n");
    format(string, sizeof(string), "%s{FFFFFF}So luong: {ffff00}%d{FFFFFF} cai\n", string, amount);
    format(string, sizeof(string), "%s{FFFFFF}Gia moi cai: {00ff00}$%d{FFFFFF}\n", string, FISHING_BAIT_PRICE);
    format(string, sizeof(string), "%s{FFFFFF}Tong tien: {00ff00}$%s{FFFFFF}\n\n", string, number_format(totalPrice));

    strcat(string, "{FFFFFF}Ban co chac chan muon mua khong?");

    ShowPlayerDialog(playerid, DIALOG_CONFIRM_BUY_BAIT, DIALOG_STYLE_MSGBOX, "Xac nhan mua moi cau", string, "Mua", "Huy");
    return 1;
}

// H�m th?c hi?n mua c?n c�u
ProcessBuyFishingRod(playerid) {
    new totalRods = GetTotalFishingRods(playerid);
    new money = PlayerInfo[playerid][pCash];

    // Ki?m tra l?i di?u ki?n
    if(totalRods >= MAX_FISHING_RODS) {
        SendClientMessage(playerid, COLOR_WHITE, "Ban da co du 3 can cau roi!");
        ShowTDN(playerid, NOTIFICATION_WARNING, "Ban da co du 3 can cau roi!");
        return 0;
    }

    if(money < FISHING_ROD_PRICE) {
        SendClientMessage(playerid, COLOR_WHITE, "Ban khong du tien!");
        ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong du tien!");
        return 0;
    }

    // Th�m c?n c�u m?i
    new rodSlot = AddFishingRod(playerid);
    if(rodSlot == -1) {
        SendClientMessage(playerid, COLOR_WHITE, "Loi: Khong the them can cau!");
        ShowTDN(playerid, NOTIFICATION_WARNING, "Khong the them can cau!");
        return 0;
    }

    // Tr? ti?n
    PlayerInfo[playerid][pCash] -= FISHING_ROD_PRICE;

    // Th�ng b�o th�nh c�ng
    new string[128];
    format(string, sizeof(string), "Ban da mua thanh cong can cau so %d voi gia $%s!",
        rodSlot + 1, number_format(FISHING_ROD_PRICE));
    SendClientMessage(playerid, COLOR_GREEN, string);

    format(string, sizeof(string), "So du hien tai: $%s", number_format(PlayerInfo[playerid][pCash]));
    SendClientMessage(playerid, COLOR_YELLOW, string);

    return 1;
}

// H�m th?c hi?n mua m?i c�u
ProcessBuyBait(playerid, amount) {
    new totalPrice = amount * FISHING_BAIT_PRICE;
    new money = PlayerInfo[playerid][pCash];

    // Ki?m tra l?i di?u ki?n
    if(totalPrice > money) {
        SendClientMessage(playerid, COLOR_WHITE, "Ban khong du tien!");
        return 0;
    }

    // Th�m m?i c�u
    PlayerInfo[playerid][pMoiCau] += amount;

    // Tr? ti?n
    PlayerInfo[playerid][pCash] -= totalPrice;

    // Th�ng b�o th�nh c�ng
    new string[128];
    format(string, sizeof(string), "Ban da mua thanh cong %d moi cau voi gia $%s!",
        amount, number_format(totalPrice));
    SendClientMessage(playerid, COLOR_GREEN, string);

    format(string, sizeof(string), "Moi cau hien tai: %d | So du: $%s",
        PlayerInfo[playerid][pMoiCau], number_format(PlayerInfo[playerid][pCash]));
    SendClientMessage(playerid, COLOR_YELLOW, string);

    return 1;
}

// Load fishing shop
stock Load_FishingShop() {
    fishingactor = CreateActor(178, FISHING_SHOP_X, FISHING_SHOP_Y, FISHING_SHOP_Z + 1.0, 111.592590);
    if(fishingactor == INVALID_ACTOR_ID) printf("[FISHING] ERROR: CreateActor failed (INVALID_ACTOR_ID)");
    else {
        printf("[FISHING] Actor created ID: %d", fishingactor);
        ApplyActorAnimation(fishingactor, "PED", "IDLE_CHAT", 0, 1, 0, 0, 0, 1);
    }
    CreateDynamic3DTextLabel("{FFFFFF}--Cua hang cau ca--\n{00ff00}Can cau & Moi cau\n{FFFFFF}An {ffff00}[Y] {FFFFFF}hoac {ffff00}[/muacancau] {FFFFFF}de mua hang",
        -1, FISHING_SHOP_X, FISHING_SHOP_Y, FISHING_SHOP_Z, 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1);
}

// CMD

CMD:cauca(playerid, params[]) {
    ShowFishingMenu(playerid);
    return 1;
}

CMD:testreset(playerid, params[]) {
    if(PlayerInfo[playerid][pAdmin] < 4) return SendClientMessage(playerid, COLOR_GREY, "Ban khong duoc phep su dung lenh nay");

    new string[128];
    format(string, sizeof(string), "[TEST] Admin %s bat dau test reset ca.", GetPlayerNameEx(playerid));
    SendClientMessageToAll(COLOR_YELLOW, string);

    // Reset bi?n LastReset th�nh ng�y h�m qua
    new year, month, day;
    getdate(year, month, day);

    // Gi? l?p ng�y h�m qua
    if(day == 1) {
        // X? l� tru?ng h?p ng�y d?u th�ng
        if(month == 1) {
            LastResetYear = year - 1;
            LastResetMonth = 12;
            LastResetDay = 31;
        } else {
            LastResetYear = year;
            LastResetMonth = month - 1;
            // T�m ng�y cu?i c�ng c?a th�ng tru?c
            new lastDay;
            switch(month - 1) {
                case 4, 6, 9, 11: lastDay = 30;
                case 2: lastDay = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28;
                default: lastDay = 31;
            }
            LastResetDay = lastDay;
        }
    } else {
        LastResetYear = year;
        LastResetMonth = month;
        LastResetDay = day - 1;
    }

    // Luu th�ng tin gi? l?p
    SaveLastResetInfo();

    // Th?c hi?n ki?m tra reset
    CheckAndResetIfNeeded();

    format(string, sizeof(string), "[TEST] Reset info changed to: %d/%d/%d", LastResetDay, LastResetMonth, LastResetYear);
    SendClientMessage(playerid, COLOR_GREEN, string);
    return 1;
}

CMD:resetinfo(playerid, params[]) {
    if(PlayerInfo[playerid][pAdmin] < 1) return SendClientMessage(playerid, COLOR_GREY, "Ban khong duoc phep su dung lenh nay");

    new currentYear, currentMonth, currentDay, currentHour, currentMinute;
    getdate(currentYear, currentMonth, currentDay);
    gettime(currentHour, currentMinute, _);

    new string[256];
    format(string, sizeof(string), "Thoi gian hien tai: %d/%d/%d %02d:%02d",
        currentDay, currentMonth, currentYear, currentHour, currentMinute);
    SendClientMessage(playerid, COLOR_WHITE, string);

    format(string, sizeof(string), "Lan reset cuoi: %d/%d/%d",
        LastResetDay, LastResetMonth, LastResetYear);
    SendClientMessage(playerid, COLOR_WHITE, string);

    format(string, sizeof(string), "Gioi han cau ca cua ban: %d/60",
        PlayerInfo[playerid][pSoLanCauCa]);
    SendClientMessage(playerid, COLOR_WHITE, string);
    return 1;
}

CMD:xemca(playerid, params[]) {
    new totalFish = 0;

    // �?m t?ng s? c�
    for(new i = 0; i < MAX_FISH_TYPES; i++) {
        totalFish += PlayerInfo[playerid][FishTypeToPlayerInfo[i]];
    }

    if(totalFish <= 0) {
        SendClientMessage(playerid, COLOR_WHITE, "Ban khong co ca nao!");
        return 1;
    }

    SendClientMessage(playerid, COLOR_GREEN, "---- Ca cua ban ----");

    // Hi?n th? th�ng tin t?ng lo?i c�
    for(new i = 0; i < MAX_FISH_TYPES; i++) {
        new count = PlayerInfo[playerid][FishTypeToPlayerInfo[i]];
        if(count > 0) {
            new string[128];
            format(string, sizeof(string), "%s: %d con", FishNames[i], count);
            SendClientMessage(playerid, COLOR_WHITE, string);
        }
    }

    // Hi?n th? k? l?c c� l?n nh?t
    if(PlayerInfo[playerid][pBiggestFishSize] > 0.0) {
        new string[128];
        format(string, sizeof(string), "Ca lon nhat: %s (%.2f kg)",
            FishNames[PlayerInfo[playerid][pBiggestFishType]],
            PlayerInfo[playerid][pBiggestFishSize]);
        SendClientMessage(playerid, COLOR_YELLOW, string);
    }

    return 1;
}

CMD:xemcancau(playerid, params[]) {
    ShowFishingRodInfo(playerid);
    return 1;
}

CMD:muacancau(playerid, params[]) {
    ShowFishingShop(playerid);
    return 1;
}

CMD:testcancau(playerid, params[]) {
    if(PlayerInfo[playerid][pAdmin] < 1) return SendClientMessage(playerid, COLOR_GREY, "Ban khong duoc phep su dung lenh nay");

    new totalRods = GetTotalFishingRods(playerid);
    if(totalRods >= MAX_FISHING_RODS) {
        SendClientMessage(playerid, COLOR_WHITE, "Ban da co du 3 can cau roi!");
        return 1;
    }

    // T?ng c?n c�u test cho admin
    new rodSlot = AddFishingRod(playerid);
    if(rodSlot != -1) {
        new string[128];
        format(string, sizeof(string), "Ban da nhan duoc can cau so %d voi do ben %d/%d!",
            rodSlot + 1, MAX_ROD_DURABILITY, MAX_ROD_DURABILITY);
        SendClientMessage(playerid, COLOR_GREEN, string);
    } else {
        SendClientMessage(playerid, COLOR_WHITE, "Khong the them can cau!");
    }

    PlayerInfo[playerid][pMoiCau] += 10; // T?ng th�m m?i
    return 1;
}

CMD:testcauca(playerid, params[]) {
    SendClientMessage(playerid, COLOR_GREEN, "Thanh cong!");
    PlayerInfo[playerid][pMoiCau] += 10;
    return 1;
}

CMD:testcc1(playerid, params[]) {
    SendClientMessage(playerid, COLOR_GREEN, "Thanh cong!");
    PlayerInfo[playerid][pDoBenCan][0] -= 10;
    return 1;
}
CMD:testcc2(playerid, params[]) {
    SendClientMessage(playerid, COLOR_GREEN, "Thanh cong!");
    PlayerInfo[playerid][pDoBenCan][1] -= 10;
    return 1;
}
