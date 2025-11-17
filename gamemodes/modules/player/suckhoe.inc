#include <YSI\YSI_Coding\y_hooks>
#include <open.mp>

new PlayerText:HungerIcon[MAX_PLAYERS];
new PlayerText:ThristIcon[MAX_PLAYERS];
new PlayerText:HungerInLine[MAX_PLAYERS];
new PlayerText:ThristInLine[MAX_PLAYERS];
new PlayerText:ThristOutLine[MAX_PLAYERS];
new PlayerText:HungerOutLine[MAX_PLAYERS];
new PlayerText:ThristValue[MAX_PLAYERS];
new PlayerText:HungerValue[MAX_PLAYERS];

hook OnPlayerConnect(playerid)
{
    
    HungerIcon[playerid] = CreatePlayerTextDraw(playerid, 241.000, 415.000, "HUD:radar_burgerShot");
    PlayerTextDrawTextSize(playerid, HungerIcon[playerid], 14.000, 15.000);
    PlayerTextDrawAlignment(playerid, HungerIcon[playerid], 1);
    PlayerTextDrawColor(playerid, HungerIcon[playerid], -1);
    PlayerTextDrawSetShadow(playerid, HungerIcon[playerid], 0);
    PlayerTextDrawSetOutline(playerid, HungerIcon[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, HungerIcon[playerid], 255);
    PlayerTextDrawFont(playerid, HungerIcon[playerid], 4);
    PlayerTextDrawSetProportional(playerid, HungerIcon[playerid], 1);

    ThristIcon[playerid] = CreatePlayerTextDraw(playerid, 342.000, 415.000, "HUD:radar_diner");
    PlayerTextDrawTextSize(playerid, ThristIcon[playerid], 14.000, 14.000);
    PlayerTextDrawAlignment(playerid, ThristIcon[playerid], 1);
    PlayerTextDrawColor(playerid, ThristIcon[playerid], -1);
    PlayerTextDrawSetShadow(playerid, ThristIcon[playerid], 0);
    PlayerTextDrawSetOutline(playerid, ThristIcon[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, ThristIcon[playerid], 255);
    PlayerTextDrawFont(playerid, ThristIcon[playerid], 4);
    PlayerTextDrawSetProportional(playerid, ThristIcon[playerid], 1);

    HungerInLine[playerid] = CreatePlayerTextDraw(playerid, 259.000, 414.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, HungerInLine[playerid], 69.000, 15.000);
    PlayerTextDrawAlignment(playerid, HungerInLine[playerid], 1);
    PlayerTextDrawColor(playerid, HungerInLine[playerid], -1);
    PlayerTextDrawSetShadow(playerid, HungerInLine[playerid], 0);
    PlayerTextDrawSetOutline(playerid, HungerInLine[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, HungerInLine[playerid], 255);
    PlayerTextDrawFont(playerid, HungerInLine[playerid], 4);
    PlayerTextDrawSetProportional(playerid, HungerInLine[playerid], 1);

    ThristInLine[playerid] = CreatePlayerTextDraw(playerid, 360.000, 414.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, ThristInLine[playerid], 69.000, 15.000);
    PlayerTextDrawAlignment(playerid, ThristInLine[playerid], 1);
    PlayerTextDrawColor(playerid, ThristInLine[playerid], -1);
    PlayerTextDrawSetShadow(playerid, ThristInLine[playerid], 0);
    PlayerTextDrawSetOutline(playerid, ThristInLine[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, ThristInLine[playerid], 255);
    PlayerTextDrawFont(playerid, ThristInLine[playerid], 4);
    PlayerTextDrawSetProportional(playerid, ThristInLine[playerid], 1);

    ThristOutLine[playerid] = CreatePlayerTextDraw(playerid, 362.000, 416.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, ThristOutLine[playerid], 65.000, 12.000);
    PlayerTextDrawAlignment(playerid, ThristOutLine[playerid], 1);
    PlayerTextDrawColor(playerid, ThristOutLine[playerid], -1061109505);
    PlayerTextDrawSetShadow(playerid, ThristOutLine[playerid], 0);
    PlayerTextDrawSetOutline(playerid, ThristOutLine[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, ThristOutLine[playerid], 255);
    PlayerTextDrawFont(playerid, ThristOutLine[playerid], 4);
    PlayerTextDrawSetProportional(playerid, ThristOutLine[playerid], 1);

    HungerOutLine[playerid] = CreatePlayerTextDraw(playerid, 261.000, 416.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, HungerOutLine[playerid], 65.000, 12.000);
    PlayerTextDrawAlignment(playerid, HungerOutLine[playerid], 1);
    PlayerTextDrawColor(playerid, HungerOutLine[playerid], -1061109505);
    PlayerTextDrawSetShadow(playerid, HungerOutLine[playerid], 0);
    PlayerTextDrawSetOutline(playerid, HungerOutLine[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, HungerOutLine[playerid], 255);
    PlayerTextDrawFont(playerid, HungerOutLine[playerid], 4);
    PlayerTextDrawSetProportional(playerid, HungerOutLine[playerid], 1);

    ThristValue[playerid] = CreatePlayerTextDraw(playerid, 362.000, 416.000, "LD_SPAC:white");
    //PlayerTextDrawTextSize(playerid, ThristValue[playerid], 65.000, 12.000);
    PlayerTextDrawAlignment(playerid, ThristValue[playerid], 1);
    PlayerTextDrawColor(playerid, ThristValue[playerid], 512819199);
    PlayerTextDrawSetShadow(playerid, ThristValue[playerid], 0);
    PlayerTextDrawSetOutline(playerid, ThristValue[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, ThristValue[playerid], 255);
    PlayerTextDrawFont(playerid, ThristValue[playerid], 4);
    PlayerTextDrawSetProportional(playerid, ThristValue[playerid], 1);

    HungerValue[playerid] = CreatePlayerTextDraw(playerid, 261.000, 416.000, "LD_SPAC:white");
    //PlayerTextDrawTextSize(playerid, HungerValue[playerid], 65.000, 12.000);
    PlayerTextDrawAlignment(playerid, HungerValue[playerid], 1);
    PlayerTextDrawColor(playerid, HungerValue[playerid], -7601921);
    PlayerTextDrawSetShadow(playerid, HungerValue[playerid], 0);
    PlayerTextDrawSetOutline(playerid, HungerValue[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, HungerValue[playerid], 255);
    PlayerTextDrawFont(playerid, HungerValue[playerid], 4);
    PlayerTextDrawSetProportional(playerid, HungerValue[playerid], 1);

    return 1;
}

hook OnPlayerSpawn(playerid)
{
    ShowHungerDisplay(playerid);
    return 1;
}

stock ShowHungerDisplay(playerid) {
    PlayerTextDrawShow(playerid, HungerIcon[playerid]);
    PlayerTextDrawShow(playerid, ThristIcon[playerid]);
    PlayerTextDrawShow(playerid, HungerInLine[playerid]);
    PlayerTextDrawShow(playerid, ThristInLine[playerid]);
    PlayerTextDrawShow(playerid, ThristOutLine[playerid]);
    PlayerTextDrawShow(playerid, HungerOutLine[playerid]);
    PlayerTextDrawShow(playerid, ThristValue[playerid]);
    PlayerTextDrawShow(playerid, HungerValue[playerid]);
    return 1;
}

stock HideHungerDisplay(playerid) {
    PlayerTextDrawHide(playerid, HungerIcon[playerid]);
    PlayerTextDrawHide(playerid, ThristIcon[playerid]);
    PlayerTextDrawHide(playerid, HungerInLine[playerid]);
    PlayerTextDrawHide(playerid, ThristInLine[playerid]);
    PlayerTextDrawHide(playerid, ThristOutLine[playerid]);
    PlayerTextDrawHide(playerid, HungerOutLine[playerid]);
    PlayerTextDrawHide(playerid, ThristValue[playerid]);
    PlayerTextDrawHide(playerid, HungerValue[playerid]);
    return 1;
}

stock UpdatePlayerHealthBar(playerid, bool:isHunger, Float:value) {
    new Float:barSize = value * 65.0 / 100.0;
    
    if(isHunger) {
        if(value >= 100.0) {
            PlayerTextDrawTextSize(playerid, HungerValue[playerid], 65.0, 12.0);
            PlayerInfo[playerid][pThucAn] = 100.0;
        }
        else if(value <= 0.0) {
            PlayerTextDrawTextSize(playerid, HungerValue[playerid], 0.0, 12.0);
            PlayerInfo[playerid][pThucAn] = 0.0;
        }
        else {
            PlayerTextDrawTextSize(playerid, HungerValue[playerid], barSize, 12.0);
        }
        PlayerTextDrawShow(playerid, HungerValue[playerid]);
    }
    else {
        if(value >= 100.0) {
            PlayerTextDrawTextSize(playerid, ThristValue[playerid], 65.0, 12.0);
            PlayerInfo[playerid][pNuoc] = 100.0;
        }
        else if(value <= 0.0) {
            PlayerTextDrawTextSize(playerid, ThristValue[playerid], 0.0, 12.0);
            PlayerInfo[playerid][pNuoc] = 0.0;
        }
        else {
            PlayerTextDrawTextSize(playerid, ThristValue[playerid], barSize, 12.0);
        }
        PlayerTextDrawShow(playerid, ThristValue[playerid]);
    }
    return 1;
}

stock UpdatePlayerHungerDisplay(playerid) {
    new Float:hungerValue = PlayerInfo[playerid][pThucAn];
    new Float:thirstValue = PlayerInfo[playerid][pNuoc];
    
    UpdatePlayerHealthBar(playerid, true, hungerValue);
    UpdatePlayerHealthBar(playerid, false, thirstValue);
    return 1;
}

hook OnPlayerDisconnect(playerid, reason) {
    HideHungerDisplay(playerid);
    return 1;
}

/* ====================== CMD ======================== */
CMD:testta(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] >= 99999)
    {
        
        PlayerInfo[playerid][pThucAn] = 50 + random(50);
        PlayerInfo[playerid][pNuoc] = 50 + random(50);
        SendClientMessage(playerid, COLOR_GREEN, "Thanh Cong!");
    }
	return 1;
}

CMD:suckhoe(playerid, params[]) {
    new string[128];
    format(string, sizeof(string), "{FFFFFF}[SUC KHOE] Doi bung: {ff8d11}%d{FFFFFF} | Nuoc: {16c2f7}%d {FFFFFF}", 
        floatround(PlayerInfo[playerid][pThucAn]), 
        floatround(PlayerInfo[playerid][pNuoc]));
    SendClientMessageEx(playerid, COLOR_WHITE, string);
    return 1;
}

/* ====================== TASKS ======================== */
task UpdateBar[100]() {
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
                SendClientMessageEx(i, COLOR_YELLOW, "Ban vua bi tru diem doi bung.");
            }
            else {
                new Float:health;
                GetPlayerHealth(i, health);
                SetPlayerHealth(i, health - 10.0);
                
                TogglePlayerControllable(i, 0);
                ApplyAnimation(i, "CRACK", "crckdeth2", 4.1, 1, 1, 1, 1, 1, 1);
                SendClientMessageEx(i, COLOR_YELLOW2, "Ban da nhin an rat lau, ban da bi ngat.");
                SendClientMessageEx(i, COLOR_YELLOW, "Co the cua ban dang rat doi.");
                
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
                SendClientMessageEx(i, COLOR_YELLOW, "Ban vua bi tru diem khat nuoc.");
            }
            else {
                new Float:health;
                GetPlayerHealth(i, health);
                SetPlayerHealth(i, health - 20.0);
                
                TogglePlayerControllable(i, 0);
                ApplyAnimation(i, "CRACK", "crckdeth2", 4.1, 1, 1, 1, 1, 1, 1);
                SendClientMessageEx(i, COLOR_YELLOW2, "Ban da nhin uong nuoc rat lau, ban da bi ngat.");
                SendClientMessageEx(i, COLOR_YELLOW, "Co the cua ban dang rat khat.");
                
                SetTimerEx("LoadPlayerZone", 15000, 0, "d", i);
            }
        }
    }
}

/* ====================== FORWARD ======================== */
forward LoadPlayerZone(playerid);
public LoadPlayerZone(playerid) {
    TogglePlayerControllable(playerid, true);
    ClearAnimations(playerid);
    return 1;
}