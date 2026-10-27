/*
    Anti-Cheat: Repair / SetHP / Godmode / Vehicle MaxHP
    KP5 Roleplay
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
#define AC_PLAYER_DEFAULT_MAXHP     1000.0
#define AC_CHANNEL_HACK             "1548013006141661205"

// Dialog hien thi khi bi kick (chi co nut OK)
#define DIALOG_AC_KICK              8701

// Mode cua anti-cheat
#define AC_MODE_OFF     0   // Tat hoan toan
#define AC_MODE_WARN    1   // Canh bao N lan roi kick, chi gui cho admin
#define AC_MODE_KICK    2   // Kick ngay lap tuc khi phat hien

// ============================================================
// TRACKING ARRAYS
// ============================================================
new Float:acLastVehHealth[MAX_PLAYERS];
new Float:acLastPlayerHealth[MAX_PLAYERS];
new acVehWarn[MAX_PLAYERS];
new acHpWarn[MAX_PLAYERS];

new gAC_VehMode = AC_MODE_KICK;    // Default: kick ngay
new gAC_DisabledBy[MAX_PLAYER_NAME];
new gAC_DisabledAt = 0;

// Whitelist theo VEHICLE (fix bug mechanic sua xe khong co tai xe)
new acVehRepairWhitelist[MAX_VEHICLES];

// ============================================================
// WHITELIST
// ============================================================
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

stock Float:AC_GetVehicleMaxAllowedHP(vehicleid)
{
    new Float:hp = GetGroupVehicleMaxHealth(vehicleid);
    if(hp > 0.0) return hp;
    return AC_PLAYER_DEFAULT_MAXHP;
}

// Ten mode tuong ung voi AC_MODE_OFF / WARN / KICK
static const AC_MODE_NAMES[3][] = {"TAT", "CANH BAO", "KICK NGAY"};

// ============================================================
// DISCORD LOG
// ============================================================
static AC_SendDiscordLog(const title[], const body[])
{
    new DCC_Channel:channel = DCC_FindChannelById(AC_CHANNEL_HACK);
    DiscordLog(channel, title, body);
}

// ============================================================
// KICK + DIALOG
// ============================================================
static AC_DoKick(playerid, const reason[])
{
    new szBody[512];
    format(szBody, sizeof(szBody),
        "Ban da bi kick khoi may chu vi bi phat hien su dung hack/cheat.\n\nLy do: %s\n\nNeu day la nham lan, vui long bao cao voi Ban Quan Tri (BQT).",
        reason);
    ShowPlayerDialog(playerid, DIALOG_AC_KICK, DIALOG_STYLE_MSGBOX,
        "[ Khu Pho 5 Anti-Cheat ]", szBody, "Dong", "");
    SetTimerEx("KickEx", 1000, 0, "i", playerid);
}

// ============================================================
// CHECK VEHICLE HEALTH
// ============================================================
stock AC_CheckVehicleHealth(playerid, vehicleid, Float:fCurrentVehHealth)
{
    if(PlayerInfo[playerid][pAdmin] > 1) return 0;
    if(gAC_VehMode == AC_MODE_OFF) return 0;
    if(!IsValidVehicle(vehicleid)) return 0;

    new Float:fMaxAllowed = AC_GetVehicleMaxAllowedHP(vehicleid);

    // --------- CHECK 1: HP VUOT NGUONG MAX (xe ca nhan > 1000) ---------
    if(fCurrentVehHealth > fMaxAllowed + 0.5)
    {
        new Float:fRestore = acLastVehHealth[playerid];
        if(fRestore <= 0.0 || fRestore > fMaxAllowed) fRestore = fMaxAllowed;
        SetVehicleHealth(vehicleid, fRestore);

        new szMess[256], szTitle[128], szDiscord[1024];
        format(szMess, sizeof(szMess),
            "%s bi phat hien su dung hack SetVehicleHealth vuot gioi han (Veh:%d HP:%.1f > %.1f).",
            GetPlayerNameEx(playerid), vehicleid, fCurrentVehHealth, fMaxAllowed);

        if(gAC_VehMode == AC_MODE_KICK)
        {
            format(szTitle, sizeof(szTitle),
                ":no_entry: ANTI-CHEAT | %s (ID: %d) bi kick",
                GetPlayerNameEx(playerid), playerid);
            format(szDiscord, sizeof(szDiscord),
            ":shield: **ANTI-CHEAT SYSTEM**\n\n\
            :bust_in_silhouette: **Nguoi choi:** %s (ID: %d)\n\
            :red_car: **Xe:** %d | **Model:** %d\n\
            :heart: **HP phat hien:** `%.1f` (gioi han `%.1f`)\n\
            :hammer: **Hanh dong:** Kick khoi may chu\n\
            :memo: **Ly do:** SetVehicleHealth vuot gioi han\n\
            :zap: **Mode AC:** Kick ngay",
                GetPlayerNameEx(playerid), playerid,
                vehicleid, GetVehicleModel(vehicleid),
                fCurrentVehHealth, fMaxAllowed);

            ABroadCast(COLOR_YELLOW, szMess, 2);
            AC_SendDiscordLog(szTitle, szDiscord);
            AC_DoKick(playerid, "SetVehicleHealth vuot gioi han");
        }
        else // AC_MODE_WARN
        {
            acVehWarn[playerid]++;

            if(acVehWarn[playerid] >= AC_WARN_LIMIT)
            {
                // Da du so canh bao -> kick
                format(szTitle, sizeof(szTitle),
                    ":no_entry: ANTI-CHEAT | %s (ID: %d) bi kick",
                    GetPlayerNameEx(playerid), playerid);
                format(szDiscord, sizeof(szDiscord),
                ":shield: **ANTI-CHEAT SYSTEM**\n\n\
                :bust_in_silhouette: **Nguoi choi:** %s (ID: %d)\n\
                :red_car: **Xe:** %d | **Model:** %d\n\
                :heart: **HP phat hien:** `%.1f` (gioi han `%.1f`)\n\
                :hammer: **Hanh dong:** Kick sau %d canh bao\n\
                :memo: **Ly do:** SetVehicleHealth vuot gioi han\n\
                :warning: **So lan canh bao:** `%d/%d`",
                    GetPlayerNameEx(playerid), playerid,
                    vehicleid, GetVehicleModel(vehicleid),
                    fCurrentVehHealth, fMaxAllowed,
                    AC_WARN_LIMIT,
                    acVehWarn[playerid], AC_WARN_LIMIT);

                ABroadCast(COLOR_YELLOW, szMess, 2);
                AC_SendDiscordLog(szTitle, szDiscord);
                AC_DoKick(playerid, "SetVehicleHealth vuot gioi han");
            }
            else
            {
                // Con dang canh bao -> chi gui cho admin + discord, khong nhan cho player
                new szWarnMsg[320];
                format(szWarnMsg, sizeof(szWarnMsg),
                    "[{DD0000}KP5-AC{FFFFFF}] Nghi van %s su dung hack SetVehicleHealth (Veh:%d HP:%.1f > %.1f) [Canh bao: %d/%d].",
                    GetPlayerNameEx(playerid), vehicleid, fCurrentVehHealth, fMaxAllowed,
                    acVehWarn[playerid], AC_WARN_LIMIT);
                ABroadCast(COLOR_YELLOW, szWarnMsg, 2);

                format(szTitle, sizeof(szTitle),
                    ":warning: ANTI-CHEAT WARNING | %s (ID: %d)",
                    GetPlayerNameEx(playerid), playerid);
                format(szDiscord, sizeof(szDiscord),
                ":shield: **ANTI-CHEAT SYSTEM - CANH BAO**\n\n\
                :bust_in_silhouette: **Nguoi choi:** %s (ID: %d)\n\
                :red_car: **Xe:** %d | **Model:** %d\n\
                :heart: **HP phat hien:** `%.1f` (gioi han `%.1f`)\n\
                :warning: **So lan canh bao:** `%d/%d`\n\
                :memo: **Ly do:** SetVehicleHealth vuot gioi han",
                    GetPlayerNameEx(playerid), playerid,
                    vehicleid, GetVehicleModel(vehicleid),
                    fCurrentVehHealth, fMaxAllowed,
                    acVehWarn[playerid], AC_WARN_LIMIT);

                AC_SendDiscordLog(szTitle, szDiscord);
            }
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

        new szMess[256], szTitle[128], szDiscord[1024];
        format(szMess, sizeof(szMess),
            "%s bi phat hien su dung hack Repair xe (Veh:%d +%.1f HP).",
            GetPlayerNameEx(playerid), vehicleid, fDiff);

        if(gAC_VehMode == AC_MODE_KICK)
        {
            format(szTitle, sizeof(szTitle),
                ":no_entry: ANTI-CHEAT | %s (ID: %d) bi kick",
                GetPlayerNameEx(playerid), playerid);
            format(szDiscord, sizeof(szDiscord),
            ":shield: **ANTI-CHEAT SYSTEM**\n\n\
            :bust_in_silhouette: **Nguoi choi:** %s (ID: %d)\n\
            :red_car: **Xe:** %d | **Model:** %d\n\
            :heart: **HP tang dot ngot:** `+%.1f` (nguong `%.1f`)\n\
            :hammer: **Hanh dong:** Kick khoi may chu\n\
            :memo: **Ly do:** Nghi van CLEO repair xe\n\
            :zap: **Mode AC:** Kick ngay",
                GetPlayerNameEx(playerid), playerid,
                vehicleid, GetVehicleModel(vehicleid),
                fDiff, AC_VEH_HEALTH_THRESHOLD);

            ABroadCast(COLOR_YELLOW, szMess, 2);
            AC_SendDiscordLog(szTitle, szDiscord);
            AC_DoKick(playerid, "Nghi van CLEO repair xe");
        }
        else // AC_MODE_WARN
        {
            acVehWarn[playerid]++;

            if(acVehWarn[playerid] >= AC_WARN_LIMIT)
            {
                format(szTitle, sizeof(szTitle),
                    ":no_entry: ANTI-CHEAT | %s (ID: %d) bi kick",
                    GetPlayerNameEx(playerid), playerid);
                format(szDiscord, sizeof(szDiscord),
                ":shield: **ANTI-CHEAT SYSTEM**\n\n\
                :bust_in_silhouette: **Nguoi choi:** %s (ID: %d)\n\
                :red_car: **Xe:** %d | **Model:** %d\n\
                :heart: **HP tang dot ngot:** `+%.1f` (nguong `%.1f`)\n\
                :hammer: **Hanh dong:** Kick sau %d canh bao\n\
                :memo: **Ly do:** Nghi van CLEO repair xe\n\
                :warning: **So lan canh bao:** `%d/%d`",
                    GetPlayerNameEx(playerid), playerid,
                    vehicleid, GetVehicleModel(vehicleid),
                    fDiff, AC_VEH_HEALTH_THRESHOLD,
                    AC_WARN_LIMIT,
                    acVehWarn[playerid], AC_WARN_LIMIT);

                ABroadCast(COLOR_YELLOW, szMess, 2);
                AC_SendDiscordLog(szTitle, szDiscord);
                AC_DoKick(playerid, "Nghi van CLEO repair xe");
            }
            else
            {
                // Con dang canh bao -> chi gui cho admin + discord, khong nhan cho player
                new szWarnMsg[320];
                format(szWarnMsg, sizeof(szWarnMsg),
                    "[{DD0000}KP5-AC{FFFFFF}] Nghi van %s su dung hack Repair xe (Veh:%d +%.1f HP) [Canh bao: %d/%d].",
                    GetPlayerNameEx(playerid), vehicleid, fDiff,
                    acVehWarn[playerid], AC_WARN_LIMIT);
                ABroadCast(COLOR_YELLOW, szWarnMsg, 2);

                format(szTitle, sizeof(szTitle),
                    ":warning: ANTI-CHEAT WARNING | %s (ID: %d)",
                    GetPlayerNameEx(playerid), playerid);
                format(szDiscord, sizeof(szDiscord),
                ":shield: **ANTI-CHEAT SYSTEM - CANH BAO**\n\n\
                :bust_in_silhouette: **Nguoi choi:** %s (ID: %d)\n\
                :red_car: **Xe:** %d | **Model:** %d\n\
                :heart: **HP tang dot ngot:** `+%.1f` (nguong `%.1f`)\n\
                :warning: **So lan canh bao:** `%d/%d`\n\
                :memo: **Ly do:** Nghi van CLEO repair xe",
                    GetPlayerNameEx(playerid), playerid,
                    vehicleid, GetVehicleModel(vehicleid),
                    fDiff, AC_VEH_HEALTH_THRESHOLD,
                    acVehWarn[playerid], AC_WARN_LIMIT);

                AC_SendDiscordLog(szTitle, szDiscord);
            }
        }

        AC_LogToFile("ac_veh_repair.log", szMess);
        acLastVehHealth[playerid] = fLast;
        return 1;
    }

    acLastVehHealth[playerid] = fCurrentVehHealth;
    return 0;
}

// ============================================================
// CHECK PLAYER HEALTH (disabled)
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
                "[{DD0000}KP5-AC{FFFFFF}] Anti cheating SetHP, neu co hack that hay tat giup tuiiii...!");
            SetTimerEx("KickEx", 2000, 0, "i", playerid);
        }
        else
        {
            new szWarn[160];
            format(szWarn, sizeof(szWarn),
                "[{DD0000}KP5-AC{FFFFFF}] Canh bao: Phat hien SetHP bat thuong (%d/%d).",
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

// Reset khi roi xe: tranh so sanh HP xe cu vs HP xe moi gay false positive
hook OnPlayerStateChange(playerid, newstate, oldstate)
{
    if(oldstate == PLAYER_STATE_DRIVER || oldstate == PLAYER_STATE_PASSENGER)
        acLastVehHealth[playerid] = 0.0;
    return 1;
}

// ============================================================
// CMD: /antirepair [kick | warn | off | status]
// ============================================================
CMD:antirepair(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1337)
        return SendUnAuth(playerid);

    new choice[16];
    if(sscanf(params, "s[16]", choice))
    {
        // Hien thi trang thai + huong dan
        new szColor[12];
        if(gAC_VehMode == AC_MODE_OFF)       szColor = "{FF0000}";
        else if(gAC_VehMode == AC_MODE_WARN)  szColor = "{FFFF00}";
        else                                  szColor = "{00FF00}";

        new str[256];
        format(str, sizeof(str),
            "{FFFF00}[KP5-AC] Vehicle Repair AC: %s%s",
            szColor, AC_MODE_NAMES[gAC_VehMode]);

        if(gAC_VehMode == AC_MODE_OFF && strlen(gAC_DisabledBy) > 0)
        {
            format(str, sizeof(str), "%s\n{AAAAAA}Nguoi tat: %s | Cach day: %d tick",
                str, gAC_DisabledBy,
                (gAC_DisabledAt > 0) ? (GetTickCount() - gAC_DisabledAt) : 0);
        }

        SendClientMessageEx(playerid, COLOR_WHITE,  "SU DUNG: /antirepair [kick | warn | off | status]");
        SendClientMessageEx(playerid, COLOR_LIGHTBLUE, str);
        return 1;
    }

    // Xu ly tham so
    new newMode = -1;
    if     (!strcmp(choice, "kick",   true)) newMode = AC_MODE_KICK;
    else if(!strcmp(choice, "warn",   true)) newMode = AC_MODE_WARN;
    else if(!strcmp(choice, "off",    true)) newMode = AC_MODE_OFF;
    else if(!strcmp(choice, "status", true))
    {
        new szColor[12];
        if(gAC_VehMode == AC_MODE_OFF)       szColor = "{FF0000}";
        else if(gAC_VehMode == AC_MODE_WARN)  szColor = "{FFFF00}";
        else                                  szColor = "{00FF00}";

        new str[128];
        format(str, sizeof(str),
            "{FFFF00}[KP5-AC] Vehicle Repair AC: %s%s",
            szColor, AC_MODE_NAMES[gAC_VehMode]);
        SendClientMessageEx(playerid, COLOR_LIGHTBLUE, str);
        return 1;
    }
    else
    {
        SendClientMessageEx(playerid, COLOR_WHITE, "SU DUNG: /antirepair [kick | warn | off | status]");
        return 1;
    }

    // Khong co thay doi
    if(newMode == gAC_VehMode)
    {
        new szColor[12];
        if(gAC_VehMode == AC_MODE_OFF)       szColor = "{FF0000}";
        else if(gAC_VehMode == AC_MODE_WARN)  szColor = "{FFFF00}";
        else                                  szColor = "{00FF00}";

        new str[160];
        format(str, sizeof(str),
            "[{DD0000}KP5-AC{FFFFFF}] Vehicle Repair AC da o mode %s%s{FFFFFF} tu truoc.",
            szColor, AC_MODE_NAMES[newMode]);
        SendClientMessageEx(playerid, COLOR_WHITE, str);
        return 1;
    }

    new oldMode = gAC_VehMode;
    gAC_VehMode = newMode;

    // Ghi nho ai tat (khi chuyen sang OFF)
    if(gAC_VehMode == AC_MODE_OFF)
    {
        strcopy(gAC_DisabledBy, GetPlayerNameEx(playerid), MAX_PLAYER_NAME);
        gAC_DisabledAt = GetTickCount();
    }
    else
    {
        gAC_DisabledBy[0] = EOS;
        gAC_DisabledAt    = 0;
    }

    new szModeColor[12];
    if(gAC_VehMode == AC_MODE_OFF)       szModeColor = "{FF0000}";
    else if(gAC_VehMode == AC_MODE_WARN)  szModeColor = "{FFFF00}";
    else                                  szModeColor = "{00FF00}";

    new szMsg[256], szLog[196];
    format(szMsg, sizeof(szMsg),
        "[{DD0000}KP5-AC{FFFFFF}] Vehicle Repair AC chuyen sang mode %s%s{FFFFFF} boi admin %s.",
        szModeColor, AC_MODE_NAMES[gAC_VehMode], GetPlayerNameEx(playerid));
    format(szLog, sizeof(szLog),
        "Admin %s (ID %d) set VehicleRepair AC: %s -> %s.",
        GetPlayerNameEx(playerid), playerid,
        AC_MODE_NAMES[oldMode], AC_MODE_NAMES[gAC_VehMode]);

    ABroadCast(COLOR_YELLOW, szMsg, 2);
    AC_LogToFile("ac_toggle.log", szLog);

    // Reset tracking khi bat lai tu OFF
    if(oldMode == AC_MODE_OFF)
    {
        foreach(new i : Player)
        {
            acLastVehHealth[i] = 0.0;
            acVehWarn[i]       = 0;
        }
    }

    return 1;
}
