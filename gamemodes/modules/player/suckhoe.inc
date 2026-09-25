#include <YSI\YSI_Coding\y_hooks>

// ===================== SIMPLE STYLE (Don gian - type 2) =====================
new PlayerText:Thrist[MAX_PLAYERS];
new PlayerText:Hunger[MAX_PLAYERS];
new PlayerText:Value_Hunger[MAX_PLAYERS];
new PlayerText:Value_Thrist[MAX_PLAYERS];

// ===================== MODERN STYLE (Hien dai - type 1) =====================
// (lay tu SucKhoe.pwn - thanh bar LD_SPAC:white + icon burger/diner)
new PlayerText:HungerIcon[MAX_PLAYERS];
new PlayerText:ThristIcon[MAX_PLAYERS];
new PlayerText:HungerInLine[MAX_PLAYERS];
new PlayerText:ThristInLine[MAX_PLAYERS];
new PlayerText:ThristOutLine[MAX_PLAYERS];
new PlayerText:HungerOutLine[MAX_PLAYERS];
new PlayerText:ThristValue[MAX_PLAYERS];
new PlayerText:HungerValue[MAX_PLAYERS];

#define AT_SLOT_SAE 6 

// pTypeHunger: 0 = Tat | 1 = Hien dai | 2 = Don gian
stock GetTypeHunger(playerid) {
    return PlayerInfo[playerid][pTypeHunger];
}

stock HideAllHungerTextdraws(playerid) {
    // --- Modern (Hien dai) ---
    PlayerTextDrawHide(playerid, HungerIcon[playerid]);
    PlayerTextDrawHide(playerid, ThristIcon[playerid]);
    PlayerTextDrawHide(playerid, HungerInLine[playerid]);
    PlayerTextDrawHide(playerid, ThristInLine[playerid]);
    PlayerTextDrawHide(playerid, ThristOutLine[playerid]);
    PlayerTextDrawHide(playerid, HungerOutLine[playerid]);
    PlayerTextDrawHide(playerid, ThristValue[playerid]);
    PlayerTextDrawHide(playerid, HungerValue[playerid]);

    // --- Simple (Don gian) ---
    PlayerTextDrawHide(playerid, Thrist[playerid]);
    PlayerTextDrawHide(playerid, Hunger[playerid]);
    PlayerTextDrawHide(playerid, Value_Hunger[playerid]);
    PlayerTextDrawHide(playerid, Value_Thrist[playerid]);

    /* // --- Circle (Vong tron) ---
    HideCircleHungerTextdraws(playerid); */
}

stock ShowHungerDisplay(playerid) {
    new type = GetTypeHunger(playerid);

    if (type == 1) {
        // ----- HIEN DAI -----
        PlayerTextDrawShow(playerid, HungerIcon[playerid]);
        PlayerTextDrawShow(playerid, ThristIcon[playerid]);
        PlayerTextDrawShow(playerid, HungerInLine[playerid]);
        PlayerTextDrawShow(playerid, ThristInLine[playerid]);
        PlayerTextDrawShow(playerid, ThristOutLine[playerid]);
        PlayerTextDrawShow(playerid, HungerOutLine[playerid]);
        PlayerTextDrawShow(playerid, ThristValue[playerid]);
        PlayerTextDrawShow(playerid, HungerValue[playerid]);
    }
    else if (type == 2) {
        // ----- DON GIAN -----
        PlayerTextDrawShow(playerid, Thrist[playerid]);
        PlayerTextDrawShow(playerid, Hunger[playerid]);
        PlayerTextDrawShow(playerid, Value_Hunger[playerid]);
        PlayerTextDrawShow(playerid, Value_Thrist[playerid]);
    }
    /* else if (type == 3) {
        // ----- VONG TRON -----  circles shown inside UpdatePlayerHungerDisplay
    } */
    else {
        // ----- TAT -----
        HideAllHungerTextdraws(playerid);
    }
}

stock UpdatePlayerHungerDisplay(playerid) {
    new type = GetTypeHunger(playerid);
    if (type == 0) return 1; // Tat -> khong update

    new Float:hungerValue = PlayerInfo[playerid][pThucAn];
    new Float:thirstValue = PlayerInfo[playerid][pNuoc];

    if (type == 1) {
        // Modern bar (LD_SPAC:white)
        UpdatePlayerHealthBar(playerid, true,  hungerValue);
        UpdatePlayerHealthBar(playerid, false, thirstValue);
    }
    /* else if (type == 3) {
        // Circle (Vong tron)
        ShowCircleHunger(playerid, floatround(hungerValue));
        ShowCircleThrist(playerid, floatround(thirstValue));
    } */
    else {
        // Simple text
        new hungerStr[8], thirstStr[8];
        format(hungerStr, sizeof(hungerStr), "%d", floatround(hungerValue));
        format(thirstStr, sizeof(thirstStr), "%d", floatround(thirstValue));

        PlayerTextDrawSetString(playerid, Value_Hunger[playerid], hungerStr);
        PlayerTextDrawSetString(playerid, Value_Thrist[playerid], thirstStr);
    }
    return 1;
}

hook OnPlayerConnect(playerid)
{
    // ==================== MODERN STYLE (Hien dai) ====================
    HungerIcon[playerid] = CreatePlayerTextDraw(playerid, 241.000, 415.000, "HUD:radar_burgerShot");
    PlayerTextDrawTextSize(playerid, HungerIcon[playerid], 14.000, 15.000);
    PlayerTextDrawAlignment(playerid, HungerIcon[playerid], 1);
    PlayerTextDrawColour(playerid, HungerIcon[playerid], -1);
    PlayerTextDrawSetShadow(playerid, HungerIcon[playerid], 0);
    PlayerTextDrawSetOutline(playerid, HungerIcon[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, HungerIcon[playerid], 255);
    PlayerTextDrawFont(playerid, HungerIcon[playerid], 4);
    PlayerTextDrawSetProportional(playerid, HungerIcon[playerid], 1);

    ThristIcon[playerid] = CreatePlayerTextDraw(playerid, 342.000, 415.000, "HUD:radar_diner");
    PlayerTextDrawTextSize(playerid, ThristIcon[playerid], 14.000, 14.000);
    PlayerTextDrawAlignment(playerid, ThristIcon[playerid], 1);
    PlayerTextDrawColour(playerid, ThristIcon[playerid], -1);
    PlayerTextDrawSetShadow(playerid, ThristIcon[playerid], 0);
    PlayerTextDrawSetOutline(playerid, ThristIcon[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, ThristIcon[playerid], 255);
    PlayerTextDrawFont(playerid, ThristIcon[playerid], 4);
    PlayerTextDrawSetProportional(playerid, ThristIcon[playerid], 1);

    HungerInLine[playerid] = CreatePlayerTextDraw(playerid, 259.000, 414.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, HungerInLine[playerid], 69.000, 15.000);
    PlayerTextDrawAlignment(playerid, HungerInLine[playerid], 1);
    PlayerTextDrawColour(playerid, HungerInLine[playerid], -1);
    PlayerTextDrawSetShadow(playerid, HungerInLine[playerid], 0);
    PlayerTextDrawSetOutline(playerid, HungerInLine[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, HungerInLine[playerid], 255);
    PlayerTextDrawFont(playerid, HungerInLine[playerid], 4);
    PlayerTextDrawSetProportional(playerid, HungerInLine[playerid], 1);

    ThristInLine[playerid] = CreatePlayerTextDraw(playerid, 360.000, 414.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, ThristInLine[playerid], 69.000, 15.000);
    PlayerTextDrawAlignment(playerid, ThristInLine[playerid], 1);
    PlayerTextDrawColour(playerid, ThristInLine[playerid], -1);
    PlayerTextDrawSetShadow(playerid, ThristInLine[playerid], 0);
    PlayerTextDrawSetOutline(playerid, ThristInLine[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, ThristInLine[playerid], 255);
    PlayerTextDrawFont(playerid, ThristInLine[playerid], 4);
    PlayerTextDrawSetProportional(playerid, ThristInLine[playerid], 1);

    ThristOutLine[playerid] = CreatePlayerTextDraw(playerid, 362.000, 416.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, ThristOutLine[playerid], 65.000, 12.000);
    PlayerTextDrawAlignment(playerid, ThristOutLine[playerid], 1);
    PlayerTextDrawColour(playerid, ThristOutLine[playerid], -1061109505);
    PlayerTextDrawSetShadow(playerid, ThristOutLine[playerid], 0);
    PlayerTextDrawSetOutline(playerid, ThristOutLine[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, ThristOutLine[playerid], 255);
    PlayerTextDrawFont(playerid, ThristOutLine[playerid], 4);
    PlayerTextDrawSetProportional(playerid, ThristOutLine[playerid], 1);

    HungerOutLine[playerid] = CreatePlayerTextDraw(playerid, 261.000, 416.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, HungerOutLine[playerid], 65.000, 12.000);
    PlayerTextDrawAlignment(playerid, HungerOutLine[playerid], 1);
    PlayerTextDrawColour(playerid, HungerOutLine[playerid], -1061109505);
    PlayerTextDrawSetShadow(playerid, HungerOutLine[playerid], 0);
    PlayerTextDrawSetOutline(playerid, HungerOutLine[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, HungerOutLine[playerid], 255);
    PlayerTextDrawFont(playerid, HungerOutLine[playerid], 4);
    PlayerTextDrawSetProportional(playerid, HungerOutLine[playerid], 1);

    ThristValue[playerid] = CreatePlayerTextDraw(playerid, 362.000, 416.000, "LD_SPAC:white");
    //PlayerTextDrawTextSize(playerid, ThristValue[playerid], 65.000, 12.000); // set trong UpdatePlayerHealthBar
    PlayerTextDrawAlignment(playerid, ThristValue[playerid], 1);
    PlayerTextDrawColour(playerid, ThristValue[playerid], 512819199);
    PlayerTextDrawSetShadow(playerid, ThristValue[playerid], 0);
    PlayerTextDrawSetOutline(playerid, ThristValue[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, ThristValue[playerid], 255);
    PlayerTextDrawFont(playerid, ThristValue[playerid], 4);
    PlayerTextDrawSetProportional(playerid, ThristValue[playerid], 1);

    HungerValue[playerid] = CreatePlayerTextDraw(playerid, 261.000, 416.000, "LD_SPAC:white");
    //PlayerTextDrawTextSize(playerid, HungerValue[playerid], 65.000, 12.000); // set trong UpdatePlayerHealthBar
    PlayerTextDrawAlignment(playerid, HungerValue[playerid], 1);
    PlayerTextDrawColour(playerid, HungerValue[playerid], -7601921);
    PlayerTextDrawSetShadow(playerid, HungerValue[playerid], 0);
    PlayerTextDrawSetOutline(playerid, HungerValue[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, HungerValue[playerid], 255);
    PlayerTextDrawFont(playerid, HungerValue[playerid], 4);
    PlayerTextDrawSetProportional(playerid, HungerValue[playerid], 1);

    // ==================== SIMPLE STYLE (Don gian) ====================
    Thrist[playerid] = CreatePlayerTextDraw(playerid, 622.000, 5.000, "HUD:radar_centre");
    PlayerTextDrawTextSize(playerid, Thrist[playerid], 13.000, 11.000);
    PlayerTextDrawAlignment(playerid, Thrist[playerid], 1);
    PlayerTextDrawColour(playerid, Thrist[playerid], 512819199);
    PlayerTextDrawSetShadow(playerid, Thrist[playerid], 0);
    PlayerTextDrawSetOutline(playerid, Thrist[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, Thrist[playerid], 255);
    PlayerTextDrawFont(playerid, Thrist[playerid], 4);
    PlayerTextDrawSetProportional(playerid, Thrist[playerid], 1);

    Hunger[playerid] = CreatePlayerTextDraw(playerid, 606.000, 5.000, "HUD:radar_burgerShot");
    PlayerTextDrawTextSize(playerid, Hunger[playerid], 12.000, 12.000);
    PlayerTextDrawAlignment(playerid, Hunger[playerid], 1);
    PlayerTextDrawColour(playerid, Hunger[playerid], -1);
    PlayerTextDrawSetShadow(playerid, Hunger[playerid], 0);
    PlayerTextDrawSetOutline(playerid, Hunger[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, Hunger[playerid], 255);
    PlayerTextDrawFont(playerid, Hunger[playerid], 4);
    PlayerTextDrawSetProportional(playerid, Hunger[playerid], 1);

    Value_Hunger[playerid] = CreatePlayerTextDraw(playerid, 612.000, 15.000, "100");
    PlayerTextDrawLetterSize(playerid, Value_Hunger[playerid], 0.170, 1.299);
    PlayerTextDrawTextSize(playerid, Value_Hunger[playerid], 5.000, -2.000);
    PlayerTextDrawAlignment(playerid, Value_Hunger[playerid], 2);
    PlayerTextDrawColour(playerid, Value_Hunger[playerid], -1);
    PlayerTextDrawSetShadow(playerid, Value_Hunger[playerid], 0);
    PlayerTextDrawSetOutline(playerid, Value_Hunger[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, Value_Hunger[playerid], 150);
    PlayerTextDrawFont(playerid, Value_Hunger[playerid], 2);
    PlayerTextDrawSetProportional(playerid, Value_Hunger[playerid], 1);

    Value_Thrist[playerid] = CreatePlayerTextDraw(playerid, 629.000, 15.000, "100");
    PlayerTextDrawLetterSize(playerid, Value_Thrist[playerid], 0.170, 1.299);
    PlayerTextDrawTextSize(playerid, Value_Thrist[playerid], 5.000, -2.000);
    PlayerTextDrawAlignment(playerid, Value_Thrist[playerid], 2);
    PlayerTextDrawColour(playerid, Value_Thrist[playerid], -1);
    PlayerTextDrawSetShadow(playerid, Value_Thrist[playerid], 0);
    PlayerTextDrawSetOutline(playerid, Value_Thrist[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, Value_Thrist[playerid], 150);
    PlayerTextDrawFont(playerid, Value_Thrist[playerid], 2);
    PlayerTextDrawSetProportional(playerid, Value_Thrist[playerid], 1);

    return 1;
}

stock UpdatePlayerHealthBar(playerid, bool:isHunger, Float:value) {
    // Chi hoat dong khi dang bat Hien dai
    if (GetTypeHunger(playerid) != 1) return 1;

    new Float:barSize = value * 65.0 / 100.0;

    if(isHunger) {
        if(value >= 100.0) {
            PlayerTextDrawTextSize(playerid, HungerValue[playerid], 65.0, 12.0);
            PlayerInfo[playerid][pThucAn] = 100;
        }
        else if(value <= 0.0) {
            PlayerTextDrawTextSize(playerid, HungerValue[playerid], 0.0, 12.0);
            PlayerInfo[playerid][pThucAn] = 0;
        }
        else {
            PlayerTextDrawTextSize(playerid, HungerValue[playerid], barSize, 12.0);
        }
        PlayerTextDrawShow(playerid, HungerValue[playerid]);
    }
    else {
        if(value >= 100.0) {
            PlayerTextDrawTextSize(playerid, ThristValue[playerid], 65.0, 12.0);
            PlayerInfo[playerid][pNuoc] = 100;
        }
        else if(value <= 0.0) {
            PlayerTextDrawTextSize(playerid, ThristValue[playerid], 0.0, 12.0);
            PlayerInfo[playerid][pNuoc] = 0;
        }
        else {
            PlayerTextDrawTextSize(playerid, ThristValue[playerid], barSize, 12.0);
        }
        PlayerTextDrawShow(playerid, ThristValue[playerid]);
    }
    return 1;
}

CMD:testta(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] >= 1337)
    {
        PlayerInfo[playerid][pThucAn] = 50 + random(50);
        PlayerInfo[playerid][pNuoc]   = 50 + random(50);
        ShowTDN(playerid, NOTIFICATION_SUCCESS, "Thanh Cong!");
    }
    return 1;
}

stock SucKhoe(playerid,targetid)
{
    if(IsPlayerConnected(targetid))
    {
        SendClientMessageEx(playerid, COLOR_WHITE, "{FFFFFF}[SUC KHOE] Doi bung: {ff8d11}%d{FFFFFF} | Nuoc: {16c2f7}%d {FFFFFF}",
            floatround(PlayerInfo[playerid][pThucAn]),
            floatround(PlayerInfo[playerid][pNuoc]));
    }
    return 1;
}

// ============================== TASKS ==============================
task UpdateBar[1000]() {
    foreach(new playerid: Player) {
        if(gPlayerLogged{playerid} == 1) {
            UpdatePlayerHungerDisplay(playerid);
            ShowHungerDisplay(playerid);
        }
    }
}

task UpdateDoiBung[900000]() {
    foreach(new i: Player) {
        if(IsPlayerConnected(i) && GetPVarInt(i, "Injured") == 0) {
            if(PlayerInfo[i][pThucAn] > 0) {
                PlayerInfo[i][pThucAn] -= 5 + random(10);
                ShowTDN(i, NOTIFICATION_INFO, "Ban vua bi tru diem doi bung.");
            }
            else {
                new Float:health;
                GetPlayerHealth(i, health);
                SetPlayerHealth(i, health - 10.0);

                TogglePlayerControllable(i, 0);
                ApplyAnimation(i, "CRACK", "crckdeth2", 4.1, 1, 1, 1, 1, 1, 1);
                SendClientMessageEx(i, COLOR_YELLOW2, "Ban da nhin an rat lau, ban da bi ngat.");
                ShowTDN(i, NOTIFICATION_WARNING, "Co the cua ban dang rat doi.");

                SetTimerEx("LoadPlayerZone", 15000, 0, "d", i);
            }
        }
    }
}

task UpdateKhatNuoc[800000]() {
    foreach(new i: Player) {
        if(IsPlayerConnected(i) && GetPVarInt(i, "Injured") == 0) {
            if(PlayerInfo[i][pNuoc] > 0) {
                PlayerInfo[i][pNuoc] -= 5 + random(10);
                ShowTDN(i, NOTIFICATION_INFO, "Ban vua bi tru diem khat nuoc.");
            }
            else {
                new Float:health;
                GetPlayerHealth(i, health);
                SetPlayerHealth(i, health - 20.0);

                TogglePlayerControllable(i, 0);
                ApplyAnimation(i, "CRACK", "crckdeth2", 4.1, 1, 1, 1, 1, 1, 1);
                SendClientMessageEx(i, COLOR_YELLOW2, "Ban da nhin uong nuoc rat lau, ban da bi ngat.");
                ShowTDN(i, NOTIFICATION_WARNING, "Co the cua ban dang rat khat.");

                SetTimerEx("LoadPlayerZone", 15000, 0, "d", i);
            }
        }
    }
}

// ============================== COMMANDS ==============================
CMD:suckhoe(playerid, params[]) {
    new string[1280];
    format(string, sizeof(string), "{FFFFFF}[SUC KHOE] Doi bung: {ff8d11}%d{FFFFFF} | Nuoc: {16c2f7}%d {FFFFFF}",
        floatround(PlayerInfo[playerid][pThucAn]),
        floatround(PlayerInfo[playerid][pNuoc]));
    SendClientMessageEx(playerid, COLOR_WHITE, string);
    return 1;
}

// Cycle 0 -> 1 -> 2 -> 3 -> 0
CMD:hungertype(playerid, params[]) {
    new type = PlayerInfo[playerid][pTypeHunger];
    type = (type + 1) % 4;
    PlayerInfo[playerid][pTypeHunger] = type;

    switch(type) {
        case 0: SendClientMessageEx(playerid, COLOR_WHITE, "HUD Suc khoe: {FF0000}Tat");
        case 1: SendClientMessageEx(playerid, COLOR_WHITE, "HUD Suc khoe: {00FF00}Hien dai");
        case 2: SendClientMessageEx(playerid, COLOR_WHITE, "HUD Suc khoe: {FFFF00}Don gian");
        //case 3: SendClientMessageEx(playerid, COLOR_WHITE, "HUD Suc khoe: {00BFFF}Vong tron");
    }

    HideAllHungerTextdraws(playerid);
    UpdatePlayerHungerDisplay(playerid);
    ShowHungerDisplay(playerid);
    return 1;
}

hook OnPlayerDisconnect(playerid, reason) {
    HideAllHungerTextdraws(playerid);
    //DestroyCircleHungerTextdraws(playerid);
    return 1;
}

// ============================== HELPER ==============================
forward LoadPlayerZone(playerid);
public LoadPlayerZone(playerid) {
    TogglePlayerControllable(playerid, true);
    ClearAnimations(playerid);
    return 1;
}