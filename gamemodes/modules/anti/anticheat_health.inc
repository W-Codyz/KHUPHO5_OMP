/*
    Anti-Cheat: Repair / SetHP / Godmode / Vehicle MaxHP
    GTA-HTB Roleplay
    Tich hop ABroadCast + DCC Discord log (giong ANTI-VPN)
*/

#if defined _anticheat_health_included
    #endinput
#endif
#define _anticheat_health_included

// ============================================================
// CONFIG
// ============================================================
#define AC_VEH_HEALTH_THRESHOLD     25.0
#define AC_PLAYER_HEALTH_THRESHOLD  12.0
#define AC_WARN_LIMIT               3
#define AC_WHITELIST_MS             3000
#define AC_PLAYER_DEFAULT_MAXHP     1000.0   // Xe ca nhan max HP
#define AC_CHANNEL_HACK             "1548013006141661205"

// ============================================================
// TRACKING ARRAYS
// ============================================================
new Float:acLastVehHealth[MAX_PLAYERS];
new Float:acLastPlayerHealth[MAX_PLAYERS];
new acVehWarn[MAX_PLAYERS];
new acHpWarn[MAX_PLAYERS];

new bool:gAC_VehRepairEnabled = true;
new gAC_DisabledBy[MAX_PLAYER_NAME];
new gAC_DisabledAt = 0;

// Whitelist theo VEHICLE (fix bug mechanic sua xe khong co tai xe)
new acVehRepairWhitelist[MAX_VEHICLES];

// ============================================================
// WHITELIST
// ============================================================
/* stock AC_WhitelistHeal(playerid)
{
    SetPVarInt(playerid, "AC_LegitHeal", GetTickCount() + AC_WHITELIST_MS);
} */

stock AC_WhitelistVehRepair(vehicleid)
{
    if(IsValidVehicle(vehicleid))
        acVehRepairWhitelist[vehicleid] = GetTickCount() + AC_WHITELIST_MS;
}

stock bool:AC_ConsumeHeal(playerid)
{
    new t = GetPVarInt(playerid, "AC_LegitHeal");
    if(t > GetTickCount()) {
        DeletePVar(playerid, "AC_LegitHeal");
        return true;
    }
    if(t != 0) DeletePVar(playerid, "AC_LegitHeal");
    return false;
}

stock bool:AC_ConsumeVehRepair(vehicleid)
{
    if(acVehRepairWhitelist[vehicleid] > GetTickCount()) {
        acVehRepairWhitelist[vehicleid] = 0;
        return true;
    }
    return false;
}

// Lay max HP cho phep cua xe
//  - Xe group/family: lay tu DynVehicleInfo
//  - Xe ca nhan    : 1000.0
stock Float:AC_GetVehicleMaxAllowedHP(vehicleid)
{
    new Float:hp = GetGroupVehicleMaxHealth(vehicleid);
    if(hp > 0.0) return hp;            // xe group
    return AC_PLAYER_DEFAULT_MAXHP;    // xe ca nhan
}

// ============================================================
// DISCORD LOG
// ============================================================
static AC_SendDiscordLog(const title[], const body[])
{
    new DCC_Channel:channel = DCC_FindChannelById(AC_CHANNEL_HACK);
    DiscordLog(channel, title, body);
}

// ============================================================
// CHECK VEHICLE HEALTH
// ============================================================
stock AC_CheckVehicleHealth(playerid, vehicleid, Float:fCurrentVehHealth)
{
    if(PlayerInfo[playerid][pAdmin] > 1) return 0;
    if(!gAC_VehRepairEnabled) return 0;
    if(!IsValidVehicle(vehicleid)) return 0;

    new Float:fMaxAllowed = AC_GetVehicleMaxAllowedHP(vehicleid);

    // --------- CHECK 1: HP VUOT NGUONG MAX (xe ca nhan > 1000) ---------
    if(fCurrentVehHealth > fMaxAllowed + 0.5)
    {
        new Float:fRestore = acLastVehHealth[playerid];
        if(fRestore <= 0.0 || fRestore > fMaxAllowed) fRestore = fMaxAllowed;
        SetVehicleHealth(vehicleid, fRestore);

        acVehWarn[playerid]++;

        new szMess[256], szTitle[128], szDiscord[768];
        format(szMess, sizeof(szMess),
            "%s da bi kick vi su dung hack SetVehicleHealth vuot gioi han (Veh:%d HP:%.1f > %.1f).",
            GetPlayerNameEx(playerid), vehicleid, fCurrentVehHealth, fMaxAllowed);

        if(acVehWarn[playerid] >= AC_WARN_LIMIT)
        {
            // ============ KICK ============
            format(szTitle, sizeof(szTitle),
                ":no_entry: ANTI-CHEAT | %s (ID: %d) bi kick",
                GetPlayerNameEx(playerid), playerid);
            format(szDiscord, sizeof(szDiscord),
                ":shield: **ANTI-CHEAT SYSTEM**\n\n\
                :bust_in_silhouette: **Nguoi choi:** %s (ID: %d)\n\
                :car: **Xe:** %d | **Model:** %d\n\
                :heart: **HP phat hien:** `%.1f` (gioi han `%.1f`)\n\
                :hammer: **Hanh dong:** Kick khoi may chu\n\
                :memo: **Ly do:** SetVehicleHealth vuot gioi han\n\
                :warning: **So lan canh bao:** `%d/%d`",
                GetPlayerNameEx(playerid), playerid,
                vehicleid, GetVehicleModel(vehicleid),
                fCurrentVehHealth, fMaxAllowed,
                acVehWarn[playerid], AC_WARN_LIMIT);

            ABroadCast(COLOR_YELLOW, szMess, 2);
            AC_SendDiscordLog(szTitle, szDiscord);
            SendClientMessageEx(playerid, COLOR_WHITE,
                "[{DD0000}HTB-AC{FFFFFF}] Anti cheating SetVehicleHealth, neu co hack that hay tat giup tuiiii...!");
            SetTimerEx("KickEx", 2000, 0, "i", playerid);
        }
        else
        {
            // ============ WARNING -> GUI LOG DISCORD ============
            /* new szWarn[160];
            format(szWarn, sizeof(szWarn),
                "[{DD0000}HTB-AC{FFFFFF}] Canh bao: Phat hien HP xe bat thuong (%d/%d).",
                acVehWarn[playerid], AC_WARN_LIMIT);
            SendClientMessageEx(playerid, COLOR_WHITE, szWarn); // gửi cho player */

            format(szMess, sizeof(szMess),
                "[{DD0000}HTB-AC{FFFFFF}] Nghi van %s su dung hack SetVehicleHealth vuot gioi han (Veh:%d HP:%.1f > %.1f) [Canh bao: %d/%d].",
                GetPlayerNameEx(playerid), vehicleid, fCurrentVehHealth, fMaxAllowed,
                acVehWarn[playerid], AC_WARN_LIMIT);
            ABroadCast(COLOR_YELLOW, szMess, 2);

            format(szTitle, sizeof(szTitle),
                ":warning: ANTI-CHEAT WARNING | %s (ID: %d)",
                GetPlayerNameEx(playerid), playerid);
            format(szDiscord, sizeof(szDiscord),
                ":shield: **ANTI-CHEAT SYSTEM - CANH BAO**\n\n\
                :bust_in_silhouette: **Nguoi choi:** %s (ID: %d)\n\
                :car: **Xe:** %d | **Model:** %d\n\
                :heart: **HP phat hien:** `%.1f` (gioi han `%.1f`)\n\
                :warning: **So lan canh bao:** `%d/%d`\n\
                :memo: **Ly do:** SetVehicleHealth vuot gioi han (nghi van CLEO)",
                GetPlayerNameEx(playerid), playerid,
                vehicleid, GetVehicleModel(vehicleid),
                fCurrentVehHealth, fMaxAllowed,
                acVehWarn[playerid], AC_WARN_LIMIT);

            AC_SendDiscordLog(szTitle, szDiscord);   // ⚠️ THIẾU DÒNG NÀY LÀ HỎNG
        }

        AC_LogToFile("ac_veh_maxhp.log", szMess);
        acLastVehHealth[playerid] = fRestore;
        return 1;
    }

    // --------- CHECK 2: HP TANG DOT NGOT ---------
    new Float:fLast = acLastVehHealth[playerid];
    new Float:fDiff = fCurrentVehHealth - fLast;

    if(fLast > 0.0 && fDiff > AC_VEH_HEALTH_THRESHOLD)
    {
        if(AC_ConsumeVehRepair(vehicleid)) {
            acLastVehHealth[playerid] = fCurrentVehHealth;
            return 0;
        }

        SetVehicleHealth(vehicleid, fLast);
        acVehWarn[playerid]++;

        new szMess[256], szTitle[128], szDiscord[768];
        format(szMess, sizeof(szMess),
            "%s da bi kick vi su dung hack Repair xe (Veh:%d +%.1f HP).",
            GetPlayerNameEx(playerid), vehicleid, fDiff);

        if(acVehWarn[playerid] >= AC_WARN_LIMIT)
        {
            // ============ KICK ============
            format(szTitle, sizeof(szTitle),
                ":no_entry: ANTI-CHEAT | %s (ID: %d) bi kick",
                GetPlayerNameEx(playerid), playerid);
            format(szDiscord, sizeof(szDiscord),
                ":shield: **ANTI-CHEAT SYSTEM**\n\n\
                :bust_in_silhouette: **Nguoi choi:** %s (ID: %d)\n\
                :car: **Xe:** %d | **Model:** %d\n\
                :heart: **HP tang dot ngot:** `+%.1f` (nguong `%.1f`)\n\
                :hammer: **Hanh dong:** Kick khoi may chu\n\
                :memo: **Ly do:** Nghi van CLEO repair xe\n\
                :warning: **So lan canh bao:** `%d/%d`",
                GetPlayerNameEx(playerid), playerid,
                vehicleid, GetVehicleModel(vehicleid),
                fDiff, AC_VEH_HEALTH_THRESHOLD,
                acVehWarn[playerid], AC_WARN_LIMIT);

            ABroadCast(COLOR_YELLOW, szMess, 2);
            AC_SendDiscordLog(szTitle, szDiscord);
            SendClientMessageEx(playerid, COLOR_WHITE,
                "[{DD0000}HTB-AC{FFFFFF}] Anti cheating Repair xe, neu co hack that hay tat giup tuiiii...!");
            SetTimerEx("KickEx", 2000, 0, "i", playerid);
        }
        else
        {
            // ============ WARNING -> GUI LOG DISCORD ============
            new szWarn[160];
            format(szWarn, sizeof(szWarn),
                "[{DD0000}HTB-AC{FFFFFF}] Canh bao: Phat hien repair xe bat thuong (%d/%d).",
                acVehWarn[playerid], AC_WARN_LIMIT);
            SendClientMessageEx(playerid, COLOR_WHITE, szWarn);

            format(szTitle, sizeof(szTitle),
                ":warning: ANTI-CHEAT WARNING | %s (ID: %d)",
                GetPlayerNameEx(playerid), playerid);
            format(szDiscord, sizeof(szDiscord),
                ":shield: **ANTI-CHEAT SYSTEM - CANH BAO**\n\n\
                :bust_in_silhouette: **Nguoi choi:** %s (ID: %d)\n\
                :car: **Xe:** %d | **Model:** %d\n\
                :heart: **HP tang dot ngot:** `+%.1f` (nguong `%.1f`)\n\
                :warning: **So lan canh bao:** `%d/%d`\n\
                :memo: **Ly do:** Nghi van CLEO repair xe",
                GetPlayerNameEx(playerid), playerid,
                vehicleid, GetVehicleModel(vehicleid),
                fDiff, AC_VEH_HEALTH_THRESHOLD,
                acVehWarn[playerid], AC_WARN_LIMIT);

            AC_SendDiscordLog(szTitle, szDiscord);
        }

        AC_LogToFile("ac_veh_repair.log", szMess);
        acLastVehHealth[playerid] = fLast;
        return 1;
    }

    acLastVehHealth[playerid] = fCurrentVehHealth;
    return 0;
}

// ============================================================
// CHECK PLAYER HEALTH
// ============================================================
/* stock AC_CheckPlayerHealth(playerid, Float:fCurrentHp)
{
    if(PlayerInfo[playerid][pAdmin] > 1) return 0;
    if(!gAC_PlayerHealthEnabled) return 0;

    new Float:fLast = acLastPlayerHealth[playerid];
    new Float:fDiff = fCurrentHp - fLast;

    if(fLast > 0.0 && fDiff > AC_PLAYER_HEALTH_THRESHOLD)
    {
        if(AC_ConsumeHeal(playerid)) {
            acLastPlayerHealth[playerid] = fCurrentHp;
            return 0;
        }

        SetPlayerHealth(playerid, fLast);
        acHpWarn[playerid]++;

        new szMess[256], szTitle[128], szDiscord[640];
        format(szMess, sizeof(szMess),
            "%s da bi kick vi su dung hack SetHP (+%.1f HP).",
            GetPlayerNameEx(playerid), fDiff);

        if(acHpWarn[playerid] >= AC_WARN_LIMIT)
        {
            format(szTitle, sizeof(szTitle),
                ":no_entry: ANTI-CHEAT | %s (ID: %d) bi kick",
                GetPlayerNameEx(playerid), playerid);
            format(szDiscord, sizeof(szDiscord),
                ":shield: **ANTI-CHEAT SYSTEM**\n\n\
                :bust_in_silhouette: **Nguoi choi:** %s (ID: %d)\n\
                :heart: **HP tang dot ngot:** `+%.1f` (nguong `%.1f`)\n\
                :hammer: **Hanh dong:** Kick khoi may chu\n\
                :memo: **Ly do:** Nghi van CLEO sethp / godmode",
                GetPlayerNameEx(playerid), playerid,
                fDiff, AC_PLAYER_HEALTH_THRESHOLD);

            ABroadCast(COLOR_YELLOW, szMess, 2);
            AC_SendDiscordLog(szTitle, szDiscord);
            SendClientMessageEx(playerid, COLOR_WHITE,
                "[{DD0000}HTB-AC{FFFFFF}] Anti cheating SetHP, neu co hack that hay tat giup tuiiii...!");
            SetTimerEx("KickEx", 2000, 0, "i", playerid);
        }
        else
        {
            new szWarn[160];
            format(szWarn, sizeof(szWarn),
                "[{DD0000}HTB-AC{FFFFFF}] Canh bao: Phat hien SetHP bat thuong (%d/%d).",
                acHpWarn[playerid], AC_WARN_LIMIT);
            SendClientMessageEx(playerid, COLOR_WHITE, szWarn);
        }

        AC_LogToFile("ac_sethp.log", szMess);
        acLastPlayerHealth[playerid] = fLast;
        return 1;
    }

    acLastPlayerHealth[playerid] = fCurrentHp;
    return 0;
} */

// ============================================================
// RESET
// ============================================================
stock AC_ResetTracking(playerid)
{
    acLastVehHealth[playerid]    = 0.0;
    acLastPlayerHealth[playerid] = 0.0;
    acVehWarn[playerid] = 0;
    acHpWarn[playerid]  = 0;
    DeletePVar(playerid, "AC_LegitHeal");
}

// ============================================================
// LOG FILE
// ============================================================
static AC_LogToFile(const filename[], const text[])
{
    new File:f = fopen(filename, io_append);
    if(f == File:0) return 0;
    new y, m, d, h, mi, s;
    getdate(y, m, d); gettime(h, mi, s);
    new line[320];
    format(line, sizeof(line), "[%02d/%02d/%04d %02d:%02d:%02d] %s\r\n",
        d, m, y, h, mi, s, text);
    fwrite(f, line);
    fclose(f);
    return 1;
}

// ============================================================
// HOOKS
// ============================================================
hook OnPlayerConnect(playerid) { AC_ResetTracking(playerid); return 1; }
hook OnPlayerDisconnect(playerid, reason) { AC_ResetTracking(playerid); return 1; }

// ============================================================
// CMD: /antirepair — Bat/tat Vehicle Repair Anti-Cheat khi gap loi
// ============================================================
CMD:antirepair(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1337)
        return SendUnAuth(playerid);

    // Không có tham số → hiện trạng thái
    new choice[16];
    if(sscanf(params, "s[16]", choice))
    {
        new str[256];
        format(str, sizeof(str),
            "{FFFF00}[HTB-AC] Vehicle Repair Anti-Cheat: %s",
            gAC_VehRepairEnabled ? "{00FF00}BAT" : "{FF0000}TAT");

        if(!gAC_VehRepairEnabled)
        {
            format(str, sizeof(str),
                "%s\n{AAAAAA}Nguoi tat: %s | Cach day: %d tick",
                str,
                (strlen(gAC_DisabledBy) > 0) ? gAC_DisabledBy : "?",
                (gAC_DisabledAt > 0) ? (GetTickCount() - gAC_DisabledAt) : 0);
        }

        SendClientMessageEx(playerid, COLOR_WHITE, "SU DUNG: /antirepair [on | off | status]");
        SendClientMessageEx(playerid, COLOR_LIGHTBLUE, str);
        return 1;
    }

    new bool:turnOn;
    if(!strcmp(choice, "on", true))
        turnOn = true;
    else if(!strcmp(choice, "off", true))
        turnOn = false;
    else if(!strcmp(choice, "status", true))
    {
        new str[256];
        format(str, sizeof(str),
            "{FFFF00}[HTB-AC] Vehicle Repair Anti-Cheat: %s",
            gAC_VehRepairEnabled ? "{00FF00}BAT" : "{FF0000}TAT");
        SendClientMessageEx(playerid, COLOR_LIGHTBLUE, str);
        return 1;
    }
    else
    {
        SendClientMessageEx(playerid, COLOR_WHITE, "SU DUNG: /antirepair [on | off | status]");
        return 1;
    }

    // Không có gì thay đổi
    if(turnOn == gAC_VehRepairEnabled)
    {
        new str[128];
        format(str, sizeof(str),
            "[{DD0000}HTB-AC{FFFFFF}] Vehicle Repair AC da %s tu truoc.",
            turnOn ? "{00FF00}BAT" : "{FF0000}TAT");
        SendClientMessageEx(playerid, COLOR_WHITE, str);
        return 1;
    }

    new bool:wasEnabled = gAC_VehRepairEnabled;
    gAC_VehRepairEnabled = turnOn;

    new szMsg[196], szLog[196];
    format(szMsg, sizeof(szMsg),
        "[{DD0000}HTB-AC{FFFFFF}] Vehicle Repair Anti-Cheat: %s{FFFFFF} boi admin %s.",
        gAC_VehRepairEnabled ? "{00FF00}DA BAT LAI" : "{FF0000}DA TAT",
        GetPlayerNameEx(playerid));
    format(szLog, sizeof(szLog),
        "Admin %s (ID %d) %s VehicleRepair AC.",
        GetPlayerNameEx(playerid), playerid,
        gAC_VehRepairEnabled ? "BAT" : "TAT");

    // Ghi nho ai tat
    if(!gAC_VehRepairEnabled)
    {
        strcopy(gAC_DisabledBy, GetPlayerNameEx(playerid), MAX_PLAYER_NAME);
        gAC_DisabledAt = GetTickCount();
    }
    else
    {
        gAC_DisabledBy[0] = EOS;
        gAC_DisabledAt = 0;
    }

    ABroadCast(COLOR_YELLOW, szMsg, 2);
    AC_LogToFile("ac_toggle.log", szLog);

    if(!wasEnabled && gAC_VehRepairEnabled)
    {
        foreach(new i : Player)
        {
            acLastVehHealth[i] = 0.0;
            acVehWarn[i] = 0;
        }
    }

    return 1;
}