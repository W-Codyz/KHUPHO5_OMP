#include <open.mp>
#include <YSI\YSI_Coding\y_hooks>

// ==================================================================
// DEFINES
// ==================================================================
#define MAX_SZ 100 // MAX SAFE ZONE

// Dialog IDs
#define DIALOG_SAFEZONE_LIST        (8310)
#define DIALOG_SAFEZONE_MANAGE      (8311)
#define DIALOG_SAFEZONE_EDIT        (8312)
#define DIALOG_SAFEZONE_EDIT_SIZE   (8313)
#define DIALOG_SAFEZONE_CONFIRM_DEL (8314)

// ==================================================================
// ENUMS
// ==================================================================
enum szInfos
{
    Float:szPosX,
    Float:szPosY,
    Float:szPosZ,
    szSize,
    szPickupID,
    Text3D:szTextID
};

// ==================================================================
// VARIABLES
// ==================================================================
new SafeZoneInfo[MAX_SZ][szInfos];
new PlayerEditingSZ[MAX_PLAYERS]; // Luu ID safezone dang edit

// --- SafeZone Anti-Attack + TextDraw ---
new AntiSafeZone[MAX_PLAYERS];
new SoLanBK[MAX_PLAYERS];
new EnterSafeZones[MAX_PLAYERS];
new Text:fstd[3];

// ==================================================================
// STOCK FUNCTIONS - SAVE / LOAD
// ==================================================================
stock SaveSafeZones()
{
    new szFileStr[1024];
    new File:fHandle = fopen("SafeZones.cfg", io_write);

    for(new iIndex; iIndex < MAX_SZ; iIndex++)
    {
        format(szFileStr, sizeof(szFileStr), "%f|%f|%f|%d|%d\r\n",
            SafeZoneInfo[iIndex][szPosX],
            SafeZoneInfo[iIndex][szPosY],
            SafeZoneInfo[iIndex][szPosZ],
            SafeZoneInfo[iIndex][szSize],
            SafeZoneInfo[iIndex][szPickupID]
        );
        fwrite(fHandle, szFileStr);
    }
    return fclose(fHandle);
}

stock LoadSafeZones()
{
    if(!fexist("SafeZones.cfg")) return 1;

    new string[256];
    new szFileStr[128];
    new File:iFileHandle = fopen("SafeZones.cfg", io_read);
    new iIndex;

    while(iIndex < sizeof(SafeZoneInfo) && fread(iFileHandle, szFileStr))
    {
        sscanf(szFileStr, "p<|>fffii",
            SafeZoneInfo[iIndex][szPosX],
            SafeZoneInfo[iIndex][szPosY],
            SafeZoneInfo[iIndex][szPosZ],
            SafeZoneInfo[iIndex][szSize],
            SafeZoneInfo[iIndex][szPickupID]
        );

        format(string, sizeof(string),
            "{33FF33}[KHU AN TOAN]\n{FFFFFF}ID: {FFD700}%d\n{FFFFFF}Pham vi: {FF6347}%d met\n{DF0101}[!] {FFFF00}Cam KOS trong khu vuc nay!",
            iIndex, SafeZoneInfo[iIndex][szSize]
        );

        if(SafeZoneInfo[iIndex][szPosX] != 0.0)
        {
            SafeZoneInfo[iIndex][szPickupID] = CreateDynamicPickup(1314, 23,
                SafeZoneInfo[iIndex][szPosX],
                SafeZoneInfo[iIndex][szPosY],
                SafeZoneInfo[iIndex][szPosZ]
            );
            SafeZoneInfo[iIndex][szTextID] = CreateDynamic3DTextLabel(string, -1,
                SafeZoneInfo[iIndex][szPosX],
                SafeZoneInfo[iIndex][szPosY],
                SafeZoneInfo[iIndex][szPosZ] + 0.5, 30.0,
                INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 30.0
            );
        }
        ++iIndex;
    }
    return fclose(iFileHandle);
}

// ==================================================================
// STOCK FUNCTIONS - TEXTDRAW (SAFE ZONE UI)
// ==================================================================
stock TextDrawSafeZone()
{
    fstd[0] = TextDrawCreate(10.000, 225.000, "LD_SPAC:white");
    TextDrawTextSize(fstd[0], 71.000, 24.000);
    TextDrawAlignment(fstd[0], 1);
    TextDrawColor(fstd[0], 100);
    TextDrawSetShadow(fstd[0], 0);
    TextDrawSetOutline(fstd[0], 0);
    TextDrawBackgroundColor(fstd[0], 255);
    TextDrawFont(fstd[0], 4);
    TextDrawSetProportional(fstd[0], 1);

    fstd[1] = TextDrawCreate(18.000, 230.000, "SAFE ZONE");
    TextDrawLetterSize(fstd[1], 0.300, 1.500);
    TextDrawAlignment(fstd[1], 1);
    TextDrawColor(fstd[1], -1);
    TextDrawSetShadow(fstd[1], 1);
    TextDrawSetOutline(fstd[1], 1);
    TextDrawBackgroundColor(fstd[1], 150);
    TextDrawFont(fstd[1], 1);
    TextDrawSetProportional(fstd[1], 1);

    fstd[2] = TextDrawCreate(10.000, 225.000, "LD_SPAC:white");
    TextDrawTextSize(fstd[2], 3.000, 24.000);
    TextDrawAlignment(fstd[2], 1);
    TextDrawColor(fstd[2], -2686721);
    TextDrawSetShadow(fstd[2], 0);
    TextDrawSetOutline(fstd[2], 0);
    TextDrawBackgroundColor(fstd[2], 255);
    TextDrawFont(fstd[2], 4);
    TextDrawSetProportional(fstd[2], 1);
}

stock TDShow(playerid)
{
    for(new i = 0; i < 3; i++)
        TextDrawShowForPlayer(playerid, fstd[i]);
}

stock TDHide(playerid)
{
    for(new i = 0; i < 3; i++)
        TextDrawHideForPlayer(playerid, fstd[i]);
}

// ==================================================================
// HOOKS - CALLBACKS
// ==================================================================
/* hook OnGameModeInit()
{
    print("\n--------------------------------------");
    print("Safe Zone System - Loaded");
    print("--------------------------------------\n");
    LoadSafeZones();
    TextDrawSafeZone();
    return 1;
} */

hook OnPlayerConnect(playerid)
{
    // Khoi tao gia tri cho SafeZone anti-attack
    EnterSafeZones[playerid] = -1;
    AntiSafeZone[playerid]    = 0;
    SoLanBK[playerid]         = 0;
    PlayerEditingSZ[playerid] = -1;
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    if(EnterSafeZones[playerid] >= 0)
    {
        TDHide(playerid);
        EnterSafeZones[playerid] = -1;
    }
    return 1;
}

// ------------------------------------------------------------------
// OnPlayerUpdate: disarms cop-outside players + hien/an TextDraw
// ------------------------------------------------------------------
hook OnPlayerUpdate(playerid)
{
    new bool:inAnyZone = false;

    for(new Sz; Sz < MAX_SZ; Sz++)
    {
        if(SafeZoneInfo[Sz][szSize] <= 0) continue;
        if(SafeZoneInfo[Sz][szPosX] == 0.0 && SafeZoneInfo[Sz][szPosY] == 0.0) continue;

        if(IsPlayerInRangeOfPoint(playerid, SafeZoneInfo[Sz][szSize],
            SafeZoneInfo[Sz][szPosX],
            SafeZoneInfo[Sz][szPosY],
            SafeZoneInfo[Sz][szPosZ]))
        {
            inAnyZone = true;

            if(!IsACop(playerid) && PlayerInfo[playerid][pAdmin] < 2)
            {
                SetPlayerArmedWeapon(playerid, 0);
            }

            if(zombieevent == 0 && gPlayerLogged{playerid} == 1 && EnterSafeZones[playerid] != Sz)
            {
                EnterSafeZones[playerid] = Sz;
                TDShow(playerid);
            }
            break;
        }
    }

    if(!inAnyZone && EnterSafeZones[playerid] >= 0)
    {
        EnterSafeZones[playerid] = -1;
        TDHide(playerid);
    }
    return 1;
}

// ------------------------------------------------------------------
// OnPlayerTakeDamage: chan tan cong bang sung xe (weapon 49/50)
// ------------------------------------------------------------------
hook OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
    if(issuerid != INVALID_PLAYER_ID && zombieevent == 0)
    {
        for(new Sz; Sz < MAX_SZ; Sz++)
        {
            if(SafeZoneInfo[Sz][szSize] <= 0) continue;

            if(IsPlayerInRangeOfPoint(playerid, SafeZoneInfo[Sz][szSize],
                SafeZoneInfo[Sz][szPosX],
                SafeZoneInfo[Sz][szPosY],
                SafeZoneInfo[Sz][szPosZ]))
            {
                if(GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
                {
                    if(GetPlayerState(issuerid) == PLAYER_STATE_DRIVER)
                    {
                        if(weaponid == 49 || weaponid == 50)
                        {
                            new Float:x, Float:y, Float:z, string[128];
                            GetPlayerPos(issuerid, x, y, z);
                            SetPlayerPos(issuerid, x, y, z + 5);
                            SendClientMessageEx(issuerid, COLOR_YELLOW, "Khong duoc tan cong trong khu an toan.");
                            GameTextForPlayer(issuerid, "~r~KHONG CR NGUOI KHAC", 5000, 3);
                            ProxDetector(30.0, playerid, string, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 1;
}

// ------------------------------------------------------------------
// OnPlayerGiveDamage: chan sat thuong trong / vao safezone
// ------------------------------------------------------------------
hook OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid)
{
    for(new Sz; Sz < MAX_SZ; Sz++)
    {
        if(SafeZoneInfo[Sz][szSize] <= 0) continue;

        // ---- Attacker dang o trong SafeZone ----
        if(IsPlayerInRangeOfPoint(playerid, SafeZoneInfo[Sz][szSize],
            SafeZoneInfo[Sz][szPosX],
            SafeZoneInfo[Sz][szPosY],
            SafeZoneInfo[Sz][szPosZ]))
        {
            if(weaponid != 0)
            {
                if(!IsACop(playerid) && PlayerInfo[playerid][pAdmin] < 2)
                {
                    GameTextForPlayer(playerid, "~r~SAFEZONE", 2000, 3);
                    TogglePlayerControllable(playerid, 0);
                    SetTimerEx("LoadPlayerZone", 5000, 0, "d", playerid);

                    new Float:php, Float:ahp;
                    GetPlayerHealth(damagedid, php);
                    GetPlayerArmour(damagedid, ahp);

                    SetPlayerHealth(damagedid, php);
                    SetPlayerArmour(damagedid, ahp);

                    AntiSafeZone[playerid] += 1;
                    SendClientMessageEx(playerid, COLOR_GRAD2, "{00ffff}Ban dang no sung trong khu an toan, ban bi 1 lan canh cao.");
                    SendClientMessageEx(playerid, COLOR_GRAD2, "{00ffff}Neu ban no sung them lan nua, ban se bi chet!");

                    if(AntiSafeZone[playerid] == 3)
                    {
                        new attackercc[MAX_PLAYER_NAME];
                        GetPlayerName(damagedid, attackercc, sizeof(attackercc));

                        new safe[1280];
                        format(safe, sizeof(safe), "[SAFEZONE] Ban da chet khi co gang gay sat thuong cho %s.", attackercc);
                        SendClientMessageEx(playerid, COLOR_LIGHTRED, safe);

                        AntiSafeZone[playerid] = 0;
                        SetPlayerHealth(playerid, 0);
                    }
                }
            }
            else
            {
                if(!IsACop(playerid) && PlayerInfo[playerid][pAdmin] < 2)
                {
                    new Float:php, Float:ahp;
                    GetPlayerHealth(damagedid, php);
                    GetPlayerArmour(damagedid, ahp);

                    SetPlayerHealth(damagedid, php);
                    SetPlayerArmour(damagedid, ahp);

                    GameTextForPlayer(playerid, "~r~SAFEZONE", 2000, 3);
                    TogglePlayerControllable(playerid, 0);
                    SetTimerEx("LoadPlayerZone", 5000, 0, "d", playerid);
                }
            }
        }

        // ---- Attacker o NGOAI, nan nhan o TRONG SafeZone ----
        if(!IsPlayerInRangeOfPoint(playerid, SafeZoneInfo[Sz][szSize],
            SafeZoneInfo[Sz][szPosX],
            SafeZoneInfo[Sz][szPosY],
            SafeZoneInfo[Sz][szPosZ]))
        {
            if(IsPlayerInRangeOfPoint(damagedid, SafeZoneInfo[Sz][szSize],
                SafeZoneInfo[Sz][szPosX],
                SafeZoneInfo[Sz][szPosY],
                SafeZoneInfo[Sz][szPosZ]))
            {
                if(!IsACop(playerid) && !IsAMedic(playerid))
                {
                    new Float:php, Float:ahp;
                    GetPlayerHealth(damagedid, php);
                    GetPlayerArmour(damagedid, ahp);

                    SetPlayerHealth(damagedid, php);
                    SetPlayerArmour(damagedid, ahp);

                    GameTextForPlayer(playerid, "~r~SAFEZONE", 2000, 3);
                    TogglePlayerControllable(playerid, 0);
                    SetTimerEx("LoadPlayerZone", 5000, 0, "d", playerid);

                    if(SafeZoneInfo[Sz][szSize] > 50)
                    {
                        AntiSafeZone[playerid] += 1;
                        SendClientMessageEx(playerid, COLOR_GRAD2, "{00ffff}Ban dang no sung trong khu an toan, ban bi 1 lan canh cao.");
                        SendClientMessageEx(playerid, COLOR_GRAD2, "{00ffff}Neu ban no sung them lan nua, ban se bi chet!");

                        if(AntiSafeZone[playerid] == 3)
                        {
                            new attackercc[MAX_PLAYER_NAME];
                            GetPlayerName(damagedid, attackercc, sizeof(attackercc));

                            new safe[1280];
                            format(safe, sizeof(safe), "[SAFEZONE] Ban da chet khi co gang gay sat thuong cho %s.", attackercc);
                            SendClientMessageEx(playerid, COLOR_LIGHTRED, safe);

                            AntiSafeZone[playerid] = 0;
                            SetPlayerHealth(playerid, 0);
                        }
                    }
                }
            }
        }
    }
    return 1;
}

// ------------------------------------------------------------------
// OnPlayerDeath: xu ly BK chet trong SafeZone bang heli (weapon 50)
// ------------------------------------------------------------------
hook OnPlayerDeath(playerid, killerid, reason)
{
    for(new Sz; Sz < MAX_SZ; Sz++)
    {
        if(SafeZoneInfo[Sz][szSize] <= 0) continue;

        if(zombieevent == 0)
        {
            if(IsPlayerInRangeOfPoint(killerid, SafeZoneInfo[Sz][szSize],
                SafeZoneInfo[Sz][szPosX],
                SafeZoneInfo[Sz][szPosY],
                SafeZoneInfo[Sz][szPosZ]))
            {
                if(IsPlayerConnected(killerid)
                    && PlayerInfo[killerid][pAdmin] < 2
                    && GetPlayerState(killerid) == PLAYER_STATE_DRIVER)
                {
                    switch(reason)
                    {
                        case 50:
                        {
                            if(IsAHelicopter(GetPlayerVehicleID(killerid)))
                            {
                                SetPlayerHealth(killerid, 0);
                                SoLanBK[killerid]++;

                                new thongbao[1280];
                                format(thongbao, sizeof(thongbao),
                                    "[SAFE-ZONE] Ban vua moi BK chet (%s) trong khu an toan [%d/2]",
                                    GetPlayerNameEx(playerid), SoLanBK[killerid]);
                                SendClientMessageEx(killerid, COLOR_LIGHTBLUE, thongbao);
                                SendClientMessageEx(killerid, COLOR_LIGHTBLUE, "[SAFE-ZONE] Neu ban con BK tiep tuc, se di tu 30 phut");

                                if(SoLanBK[killerid] == 2)
                                {
                                    new minutes = 30;
                                    new string[1280];
                                    ResetPlayerWeaponsEx(killerid);
                                    format(string, sizeof(string),
                                        "[SAFE-ZONE] Nguoi choi %s (ID %d), da bi dua vao tu boi System, Ly do: BK Safezone",
                                        GetPlayerNameEx(killerid), killerid);
                                    SendClientMessageToAllEx(COLOR_LIGHTRED, string);
                                    StaffAccountCheck(killerid, GetPlayerIpEx(killerid));
                                    PhoneOnline[killerid] = 1;
                                    PlayerInfo[killerid][pJailTime] = minutes * 5;
                                    SetPlayerInterior(killerid, 1);
                                    SetPlayerHealth(killerid, 0x7FB00000);
                                    SetPlayerFacingAngle(killerid, 0);
                                    new rand = random(sizeof(OOCPrisonSpawns));
                                    SetPlayerPos(killerid, OOCPrisonSpawns[rand][0], OOCPrisonSpawns[rand][1], OOCPrisonSpawns[rand][2]);
                                    PlayerInfo[killerid][pVW] = 0;
                                    SetPlayerVirtualWorld(killerid, 0);
                                    format(string, sizeof(string), "Ban da bi dua vao tu %d phut, Ly do: BK Safezone [2/2]", minutes);
                                    SendClientMessageEx(killerid, COLOR_LIGHTBLUE, string);
                                    format(PlayerInfo[killerid][pPrisonedBy], 24, "SYSTEM SAFEZONE");
                                    format(PlayerInfo[killerid][pPrisonReason], 128, "[OOC][SJAIL] BK Safezone");
                                    SetPlayerColor(killerid, TEAM_APRISON_COLOR);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 1;
}

// ==================================================================
// COMMANDS
// ==================================================================
CMD:gotosz(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1337)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong co quyen su dung lenh nay!");

    new szid;
    if(sscanf(params, "d", szid))
        return SendClientMessageEx(playerid, COLOR_WHITE, "SU DUNG: /gotosz [ID Khu An Toan]");

    if(szid < 0 || szid >= MAX_SZ)
    {
        new str[128];
        format(str, sizeof(str), "ID Khu An Toan khong hop le! (0-%d)", MAX_SZ - 1);
        return SendClientMessageEx(playerid, COLOR_GREY, str);
    }

    if(SafeZoneInfo[szid][szPosX] == 0.0 && SafeZoneInfo[szid][szPosY] == 0.0)
        return SendClientMessageEx(playerid, COLOR_GREY, "Khu An Toan nay chua duoc tao!");

    SafeSetPlayerPos(playerid, SafeZoneInfo[szid][szPosX], SafeZoneInfo[szid][szPosY], SafeZoneInfo[szid][szPosZ]);
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);

    new str[128];
    format(str, sizeof(str), "Ban da dich chuyen den Khu An Toan ID %d", szid);
    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, str);
    return 1;
}

CMD:editsz(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1337)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong co quyen su dung lenh nay!");

    new string[256], choice[32], szid, amount;
    if(sscanf(params, "s[32]dD(10)", choice, szid, amount))
    {
        SendClientMessageEx(playerid, COLOR_WHITE, "SU DUNG: /editsz [Tuy chon] [ID] [Gia tri]");
        SendClientMessageEx(playerid, COLOR_GREY, "Tuy chon: {FFFF00}vitri, kichthuoc");
        SendClientMessageEx(playerid, COLOR_GREY, "Vi du: {FFFF00}/editsz vitri 0 {FFFFFF}| {FFFF00}/editsz kichthuoc 0 50");
        return 1;
    }

    if(szid < 0 || szid >= MAX_SZ)
    {
        new str[128];
        format(str, sizeof(str), "ID Khu An Toan khong hop le! (0-%d)", MAX_SZ - 1);
        return SendClientMessageEx(playerid, COLOR_GREY, str);
    }

    if(strcmp(choice, "vitri", true) == 0)
    {
        GetPlayerPos(playerid, SafeZoneInfo[szid][szPosX], SafeZoneInfo[szid][szPosY], SafeZoneInfo[szid][szPosZ]);

        if(SafeZoneInfo[szid][szSize] == 0)
            SafeZoneInfo[szid][szSize] = 10;

        if(IsValidDynamicPickup(SafeZoneInfo[szid][szPickupID]))
            DestroyDynamicPickup(SafeZoneInfo[szid][szPickupID]);
        if(IsValidDynamic3DTextLabel(SafeZoneInfo[szid][szTextID]))
            DestroyDynamic3DTextLabel(SafeZoneInfo[szid][szTextID]);

        format(string, sizeof(string),
            "{33FF33}[KHU AN TOAN]\n{FFFFFF}ID: {FFD700}%d\n{FFFFFF}Pham vi: {FF6347}%d met\n{DF0101}[!] {FFFF00}Cam KOS trong khu vuc nay!",
            szid, SafeZoneInfo[szid][szSize]
        );
        SafeZoneInfo[szid][szPickupID] = CreateDynamicPickup(1314, 23,
            SafeZoneInfo[szid][szPosX],
            SafeZoneInfo[szid][szPosY],
            SafeZoneInfo[szid][szPosZ]
        );
        SafeZoneInfo[szid][szTextID] = CreateDynamic3DTextLabel(string, -1,
            SafeZoneInfo[szid][szPosX],
            SafeZoneInfo[szid][szPosY],
            SafeZoneInfo[szid][szPosZ] + 0.5, 30.0,
            INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 30.0
        );

        SaveSafeZones();

        new msg1[128], msg2[128];
        format(msg1, sizeof(msg1), "Da cap nhat vi tri Khu An Toan ID %d tai vi tri hien tai!", szid);
        format(msg2, sizeof(msg2), "Kich thuoc: %d met | Su dung /editsz kichthuoc %d [so] de thay doi", SafeZoneInfo[szid][szSize], szid);
        SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg1);
        SendClientMessageEx(playerid, COLOR_GREY, msg2);
    }
    else if(strcmp(choice, "kichthuoc", true) == 0)
    {
        if(amount < 1 || amount > 500)
            return SendClientMessageEx(playerid, COLOR_GREY, "Kich thuoc phai tu 1 den 500 met!");

        if(SafeZoneInfo[szid][szPosX] == 0.0 && SafeZoneInfo[szid][szPosY] == 0.0)
        {
            new str[128];
            format(str, sizeof(str), "Hay tao vi tri truoc! Su dung: /editsz vitri %d", szid);
            return SendClientMessageEx(playerid, COLOR_GREY, str);
        }

        SafeZoneInfo[szid][szSize] = amount;

        if(IsValidDynamic3DTextLabel(SafeZoneInfo[szid][szTextID]))
            DestroyDynamic3DTextLabel(SafeZoneInfo[szid][szTextID]);

        format(string, sizeof(string),
            "{33FF33}[KHU AN TOAN]\n{FFFFFF}ID: {FFD700}%d\n{FFFFFF}Pham vi: {FF6347}%d met\n{DF0101}[!] {FFFF00}Cam KOS trong khu vuc nay!",
            szid, SafeZoneInfo[szid][szSize]
        );
        SafeZoneInfo[szid][szTextID] = CreateDynamic3DTextLabel(string, -1,
            SafeZoneInfo[szid][szPosX],
            SafeZoneInfo[szid][szPosY],
            SafeZoneInfo[szid][szPosZ] + 0.5, 30.0,
            INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 30.0
        );

        SaveSafeZones();

        new msg[128];
        format(msg, sizeof(msg), "Da cap nhat kich thuoc Khu An Toan ID %d thanh %d met!", szid, amount);
        SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
    }
    else
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Tuy chon khong hop le! Su dung: {FFFF00}vitri {FFFFFF}hoac {FFFF00}kichthuoc");
    }
    return 1;
}

CMD:xoasz(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1337)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong co quyen su dung lenh nay!");

    new szid, string[128];
    if(sscanf(params, "d", szid))
        return SendClientMessageEx(playerid, COLOR_WHITE, "SU DUNG: /xoasz [ID Khu An Toan]");

    if(szid < 0 || szid >= MAX_SZ)
    {
        new str[128];
        format(str, sizeof(str), "ID Khu An Toan khong hop le! (0-%d)", MAX_SZ - 1);
        return SendClientMessageEx(playerid, COLOR_GREY, str);
    }

    if(SafeZoneInfo[szid][szPosX] == 0.0 && SafeZoneInfo[szid][szPosY] == 0.0)
        return SendClientMessageEx(playerid, COLOR_GREY, "Khu An Toan nay chua duoc tao!");

    SafeZoneInfo[szid][szPosX] = 0.0;
    SafeZoneInfo[szid][szPosY] = 0.0;
    SafeZoneInfo[szid][szPosZ] = 0.0;
    SafeZoneInfo[szid][szSize] = 0;

    if(IsValidDynamicPickup(SafeZoneInfo[szid][szPickupID]))
        DestroyDynamicPickup(SafeZoneInfo[szid][szPickupID]);
    if(IsValidDynamic3DTextLabel(SafeZoneInfo[szid][szTextID]))
        DestroyDynamic3DTextLabel(SafeZoneInfo[szid][szTextID]);

    SaveSafeZones();

    format(string, sizeof(string), "Ban da xoa Khu An Toan ID %d thanh cong!", szid);
    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, string);
    return 1;
}

alias:listsz("danhsachsz")
CMD:listsz(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1337)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong co quyen su dung lenh nay!");

    ShowSafeZoneList(playerid);
    return 1;
}

// ==================================================================
// DIALOG HANDLERS
// ==================================================================
stock ShowSafeZoneList(playerid)
{
    new string[2048], line[128], count = 0;

    strcat(string, "{FFD700}ID\t{FFFFFF}Vi tri\t{FF6347}Pham vi\n");

    for(new i = 0; i < MAX_SZ; i++)
    {
        if(SafeZoneInfo[i][szPosX] != 0.0)
        {
            format(line, sizeof(line), "{FFD700}%d\t{87CEEB}%.1f, %.1f, %.1f\t{FF6347}%d met\n",
                i, SafeZoneInfo[i][szPosX], SafeZoneInfo[i][szPosY], SafeZoneInfo[i][szPosZ], SafeZoneInfo[i][szSize]);
            strcat(string, line);
            count++;
        }
    }

    if(count == 0)
        strcat(string, "Chua co Khu An Toan nao duoc tao!");

    ShowPlayerDialog(playerid, DIALOG_SAFEZONE_LIST, DIALOG_STYLE_TABLIST_HEADERS,
        "{33FF33}DANH SACH KHU AN TOAN", string, "Chon", "Dong");
    return 1;
}

stock ShowSafeZoneManage(playerid, szid)
{
    if(szid < 0 || szid >= MAX_SZ) return 0;
    if(SafeZoneInfo[szid][szPosX] == 0.0 && SafeZoneInfo[szid][szPosY] == 0.0) return 0;

    PlayerEditingSZ[playerid] = szid;

    new string[512], title[64];
    format(title, sizeof(title), "QUAN LY KHU AN TOAN - ID {33FF33}%d", szid);

    format(string, sizeof(string),
        "{FFFFFF}Vi tri: {87CEEB}%.2f, %.2f, %.2f\n\
        {FFFFFF}Pham vi: {FF6347}%d met\n\n\
        {FFD700}> {FFFFFF}Dich chuyen den khu vuc nay\n\
        {FFD700}> {FFFFFF}Cap nhat vi tri (tai vi tri hien tai)\n\
        {FFD700}> {FFFFFF}Thay doi kich thuoc\n\
        {FFD700}> {FF6347}Xoa khu an toan nay",
        SafeZoneInfo[szid][szPosX], SafeZoneInfo[szid][szPosY], SafeZoneInfo[szid][szPosZ],
        SafeZoneInfo[szid][szSize]
    );

    ShowPlayerDialog(playerid, DIALOG_SAFEZONE_MANAGE, DIALOG_STYLE_LIST, title, string, "Chon", "Quay lai");
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid)
    {
        case DIALOG_SAFEZONE_LIST:
        {
            if(!response) return 1;

            new count = 0, selectedID = -1;
            for(new i = 0; i < MAX_SZ; i++)
            {
                if(SafeZoneInfo[i][szPosX] != 0.0)
                {
                    if(count == listitem)
                    {
                        selectedID = i;
                        break;
                    }
                    count++;
                }
            }

            if(selectedID != -1)
                ShowSafeZoneManage(playerid, selectedID);
        }

        case DIALOG_SAFEZONE_MANAGE:
        {
            new szid = PlayerEditingSZ[playerid];

            if(!response)
            {
                ShowSafeZoneList(playerid);
                return 1;
            }

            switch(listitem)
            {
                case 2: // Dich chuyen den
                {
                    SafeSetPlayerPos(playerid, SafeZoneInfo[szid][szPosX], SafeZoneInfo[szid][szPosY], SafeZoneInfo[szid][szPosZ]);
                    SetPlayerInterior(playerid, 0);
                    SetPlayerVirtualWorld(playerid, 0);

                    new str[128];
                    format(str, sizeof(str), "Ban da dich chuyen den Khu An Toan ID %d", szid);
                    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, str);
                }

                case 3: // Cap nhat vi tri
                {
                    new string[256];
                    GetPlayerPos(playerid, SafeZoneInfo[szid][szPosX], SafeZoneInfo[szid][szPosY], SafeZoneInfo[szid][szPosZ]);

                    if(SafeZoneInfo[szid][szSize] == 0)
                        SafeZoneInfo[szid][szSize] = 10;

                    if(IsValidDynamicPickup(SafeZoneInfo[szid][szPickupID]))
                        DestroyDynamicPickup(SafeZoneInfo[szid][szPickupID]);
                    if(IsValidDynamic3DTextLabel(SafeZoneInfo[szid][szTextID]))
                        DestroyDynamic3DTextLabel(SafeZoneInfo[szid][szTextID]);

                    format(string, sizeof(string),
                        "{33FF33}[KHU AN TOAN]\n{FFFFFF}ID: {FFD700}%d\n{FFFFFF}Pham vi: {FF6347}%d met\n{DF0101}[!] {FFFF00}Cam KOS trong khu vuc nay!",
                        szid, SafeZoneInfo[szid][szSize]
                    );
                    SafeZoneInfo[szid][szPickupID] = CreateDynamicPickup(1314, 23,
                        SafeZoneInfo[szid][szPosX],
                        SafeZoneInfo[szid][szPosY],
                        SafeZoneInfo[szid][szPosZ]
                    );
                    SafeZoneInfo[szid][szTextID] = CreateDynamic3DTextLabel(string, -1,
                        SafeZoneInfo[szid][szPosX],
                        SafeZoneInfo[szid][szPosY],
                        SafeZoneInfo[szid][szPosZ] + 0.5, 30.0,
                        INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 30.0
                    );

                    SaveSafeZones();

                    new msg[128];
                    format(msg, sizeof(msg), "Da cap nhat vi tri Khu An Toan ID %d tai vi tri hien tai!", szid);
                    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
                    ShowSafeZoneManage(playerid, szid);
                }

                case 4: // Thay doi kich thuoc
                {
                    new str[256];
                    format(str, sizeof(str),
                        "{FFFFFF}Nhap kich thuoc moi cho Khu An Toan ID {FFD700}%d\n\n\
                        Kich thuoc hien tai: {FF6347}%d met\n\
                        Cho phep: {FFFFFF}1 - 500 met",
                        szid, SafeZoneInfo[szid][szSize]
                    );
                    ShowPlayerDialog(playerid, DIALOG_SAFEZONE_EDIT_SIZE, DIALOG_STYLE_INPUT,
                        "{33FF33}THAY DOI KICH THUOC", str, "Luu", "Huy");
                }

                case 5: // Xoa
                {
                    new str[256];
                    format(str, sizeof(str),
                        "{FF6347}Ban co chac chan muon xoa Khu An Toan ID %d?\n\n\
                        {FFFFFF}Vi tri: {87CEEB}%.2f, %.2f, %.2f\n\
                        {FFFFFF}Pham vi: {FF6347}%d met\n\n\
                        {FFFF00}Hanh dong nay khong the hoan tac!",
                        szid, SafeZoneInfo[szid][szPosX], SafeZoneInfo[szid][szPosY],
                        SafeZoneInfo[szid][szPosZ], SafeZoneInfo[szid][szSize]
                    );
                    ShowPlayerDialog(playerid, DIALOG_SAFEZONE_CONFIRM_DEL, DIALOG_STYLE_MSGBOX,
                        "{FF6347}XAC NHAN XOA KHU AN TOAN", str, "Xoa", "Huy");
                }
            }
        }

        case DIALOG_SAFEZONE_EDIT_SIZE:
        {
            new szid = PlayerEditingSZ[playerid];

            if(!response)
            {
                ShowSafeZoneManage(playerid, szid);
                return 1;
            }

            new amount = strval(inputtext);

            if(amount < 1 || amount > 500)
            {
                SendClientMessageEx(playerid, COLOR_GREY, "Kich thuoc phai tu 1 den 500 met!");
                ShowSafeZoneManage(playerid, szid);
                return 1;
            }

            SafeZoneInfo[szid][szSize] = amount;

            if(IsValidDynamic3DTextLabel(SafeZoneInfo[szid][szTextID]))
                DestroyDynamic3DTextLabel(SafeZoneInfo[szid][szTextID]);

            new string[256];
            format(string, sizeof(string),
                "{33FF33}[KHU AN TOAN]\n{FFFFFF}ID: {FFD700}%d\n{FFFFFF}Pham vi: {FF6347}%d met\n{DF0101}[!] {FFFF00}Cam KOS trong khu vuc nay!",
                szid, SafeZoneInfo[szid][szSize]
            );
            SafeZoneInfo[szid][szTextID] = CreateDynamic3DTextLabel(string, -1,
                SafeZoneInfo[szid][szPosX], SafeZoneInfo[szid][szPosY], SafeZoneInfo[szid][szPosZ] + 0.5, 30.0,
                INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1, -1, 30.0
            );

            SaveSafeZones();

            new msg[128];
            format(msg, sizeof(msg), "Da cap nhat kich thuoc Khu An Toan ID %d thanh %d met!", szid, amount);
            SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
            ShowSafeZoneManage(playerid, szid);
        }

        case DIALOG_SAFEZONE_CONFIRM_DEL:
        {
            new szid = PlayerEditingSZ[playerid];

            if(!response)
            {
                ShowSafeZoneManage(playerid, szid);
                return 1;
            }

            SafeZoneInfo[szid][szPosX] = 0.0;
            SafeZoneInfo[szid][szPosY] = 0.0;
            SafeZoneInfo[szid][szPosZ] = 0.0;
            SafeZoneInfo[szid][szSize] = 0;

            if(IsValidDynamicPickup(SafeZoneInfo[szid][szPickupID]))
                DestroyDynamicPickup(SafeZoneInfo[szid][szPickupID]);
            if(IsValidDynamic3DTextLabel(SafeZoneInfo[szid][szTextID]))
                DestroyDynamic3DTextLabel(SafeZoneInfo[szid][szTextID]);

            SaveSafeZones();

            new string[128];
            format(string, sizeof(string), "Da xoa Khu An Toan ID %d thanh cong!", szid);
            SendClientMessageEx(playerid, COLOR_LIGHTBLUE, string);
            ShowSafeZoneList(playerid);
        }
    }
    return 1;
}