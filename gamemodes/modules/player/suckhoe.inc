#include <YSI\YSI_Coding\y_hooks>
#include <open.mp>

new PlayerText:Thrist[MAX_PLAYERS];
new PlayerText:Hunger[MAX_PLAYERS];
new PlayerText:Value_Hunger[MAX_PLAYERS];
new PlayerText:Value_Thrist[MAX_PLAYERS];

hook OnPlayerConnect(playerid)
{
    Thrist[playerid] = CreatePlayerTextDraw(playerid, 622.000, 5.000, "HUD:radar_centre");
    PlayerTextDrawTextSize(playerid, Thrist[playerid], 13.000, 11.000);
    PlayerTextDrawAlignment(playerid, Thrist[playerid], 1);
    PlayerTextDrawColor(playerid, Thrist[playerid], 512819199);
    PlayerTextDrawSetShadow(playerid, Thrist[playerid], 0);
    PlayerTextDrawSetOutline(playerid, Thrist[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, Thrist[playerid], 255);
    PlayerTextDrawFont(playerid, Thrist[playerid], 4);
    PlayerTextDrawSetProportional(playerid, Thrist[playerid], 1);

    Hunger[playerid] = CreatePlayerTextDraw(playerid, 606.000, 5.000, "HUD:radar_burgerShot");
    PlayerTextDrawTextSize(playerid, Hunger[playerid], 12.000, 12.000);
    PlayerTextDrawAlignment(playerid, Hunger[playerid], 1);
    PlayerTextDrawColor(playerid, Hunger[playerid], -1);
    PlayerTextDrawSetShadow(playerid, Hunger[playerid], 0);
    PlayerTextDrawSetOutline(playerid, Hunger[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, Hunger[playerid], 255);
    PlayerTextDrawFont(playerid, Hunger[playerid], 4);
    PlayerTextDrawSetProportional(playerid, Hunger[playerid], 1);

    Value_Hunger[playerid] = CreatePlayerTextDraw(playerid, 612.000, 15.000, "100");
    PlayerTextDrawLetterSize(playerid, Value_Hunger[playerid], 0.170, 1.299);
    PlayerTextDrawTextSize(playerid, Value_Hunger[playerid], 5.000, -2.000);
    PlayerTextDrawAlignment(playerid, Value_Hunger[playerid], 2);
    PlayerTextDrawColor(playerid, Value_Hunger[playerid], -1);
    PlayerTextDrawSetShadow(playerid, Value_Hunger[playerid], 0);
    PlayerTextDrawSetOutline(playerid, Value_Hunger[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, Value_Hunger[playerid], 150);
    PlayerTextDrawFont(playerid, Value_Hunger[playerid], 2);
    PlayerTextDrawSetProportional(playerid, Value_Hunger[playerid], 1);

    Value_Thrist[playerid] = CreatePlayerTextDraw(playerid, 629.000, 15.000, "100");
    PlayerTextDrawLetterSize(playerid, Value_Thrist[playerid], 0.170, 1.299);
    PlayerTextDrawTextSize(playerid, Value_Thrist[playerid], 5.000, -2.000);
    PlayerTextDrawAlignment(playerid, Value_Thrist[playerid], 2);
    PlayerTextDrawColor(playerid, Value_Thrist[playerid], -1);
    PlayerTextDrawSetShadow(playerid, Value_Thrist[playerid], 0);
    PlayerTextDrawSetOutline(playerid, Value_Thrist[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, Value_Thrist[playerid], 150);
    PlayerTextDrawFont(playerid, Value_Thrist[playerid], 2);
    PlayerTextDrawSetProportional(playerid, Value_Thrist[playerid], 1);

    return 1;
}

hook OnPlayerSpawn(playerid)
{
    ShowHungerDisplay(playerid);
    return 1;
}

stock ShowHungerDisplay(playerid) {
    PlayerTextDrawShow(playerid, Thrist[playerid]);
    PlayerTextDrawShow(playerid, Hunger[playerid]);
    PlayerTextDrawShow(playerid, Value_Hunger[playerid]);
    PlayerTextDrawShow(playerid, Value_Thrist[playerid]);
    return 1;
}

stock HideHungerDisplay(playerid) {
    PlayerTextDrawHide(playerid, Thrist[playerid]);
    PlayerTextDrawHide(playerid, Hunger[playerid]);
    PlayerTextDrawHide(playerid, Value_Hunger[playerid]);
    PlayerTextDrawHide(playerid, Value_Thrist[playerid]);
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
    new hungerStr[8], thirstStr[8];
    new Float:hungerValue = PlayerInfo[playerid][pThucAn];
    new Float:thirstValue = PlayerInfo[playerid][pNuoc];
    
    format(hungerStr, sizeof(hungerStr), "%d", floatround(hungerValue));
    format(thirstStr, sizeof(thirstStr), "%d", floatround(thirstValue));
        
    PlayerTextDrawSetString(playerid, Value_Hunger[playerid], hungerStr);
    PlayerTextDrawSetString(playerid, Value_Thrist[playerid], thirstStr);
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
                //SendClientMessageEx(i, COLOR_YELLOW, "Ban vua bi tru diem doi bung.");
                ShowTDN(i, NOTIFICATION_INFO, "Ban vua bi tru diem doi bung.");
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
                //SendClientMessageEx(i, COLOR_YELLOW, "Ban vua bi tru diem khat nuoc.");
                ShowTDN(i, NOTIFICATION_INFO, "Ban vua bi tru diem khat nuoc.");
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