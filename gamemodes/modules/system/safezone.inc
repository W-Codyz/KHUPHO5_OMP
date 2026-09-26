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
new Text:fstd[19];

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
    fstd[18] = TextDrawCreate(10.999992, 219.051804, "LD_SPAC:white");
    TextDrawLetterSize(fstd[18], 0.480000, 1.120000);
    TextDrawTextSize(fstd[18], 59.000000, 17.000000);
    TextDrawAlignment(fstd[18], 1);
    TextDrawColor(fstd[18], 0x2C193BFF);
    TextDrawUseBox(fstd[18], 0);
    TextDrawBoxColor(fstd[18], 0x80808080);
    TextDrawSetShadow(fstd[18], 2);
    TextDrawSetOutline(fstd[18], 0);
    TextDrawBackgroundColor(fstd[18], 0x000000FF);
    TextDrawFont(fstd[18], 4);
    TextDrawSetProportional(fstd[18], 0);
    TextDrawSetSelectable(fstd[18], 0);

    fstd[17] = TextDrawCreate(4.000000, 216.451767, "LD_BEAT:chit");
    TextDrawLetterSize(fstd[17], 0.480000, 1.120000);
    TextDrawTextSize(fstd[17], 14.000000, 17.000000);
    TextDrawAlignment(fstd[17], 1);
    TextDrawColor(fstd[17], 0x2C193BFF);
    TextDrawUseBox(fstd[17], 0);
    TextDrawBoxColor(fstd[17], 0x80808080);
    TextDrawSetShadow(fstd[17], 2);
    TextDrawSetOutline(fstd[17], 0);
    TextDrawBackgroundColor(fstd[17], 0x000000FF);
    TextDrawFont(fstd[17], 4);
    TextDrawSetProportional(fstd[17], 0);
    TextDrawSetSelectable(fstd[17], 0);

    fstd[16] = TextDrawCreate(63.199997, 216.451767, "LD_BEAT:chit");
    TextDrawLetterSize(fstd[16], 0.480000, 1.120000);
    TextDrawTextSize(fstd[16], 14.000000, 17.000000);
    TextDrawAlignment(fstd[16], 1);
    TextDrawColor(fstd[16], 0x2C193BFF);
    TextDrawUseBox(fstd[16], 0);
    TextDrawBoxColor(fstd[16], 0x80808080);
    TextDrawSetShadow(fstd[16], 2);
    TextDrawSetOutline(fstd[16], 0);
    TextDrawBackgroundColor(fstd[16], 0x000000FF);
    TextDrawFont(fstd[16], 4);
    TextDrawSetProportional(fstd[16], 0);
    TextDrawSetSelectable(fstd[16], 0);

    fstd[15] = TextDrawCreate(63.199997, 222.051682, "LD_BEAT:chit");
    TextDrawLetterSize(fstd[15], 0.480000, 1.120000);
    TextDrawTextSize(fstd[15], 14.000000, 17.000000);
    TextDrawAlignment(fstd[15], 1);
    TextDrawColor(fstd[15], 0x2C193BFF);
    TextDrawUseBox(fstd[15], 0);
    TextDrawBoxColor(fstd[15], 0x80808080);
    TextDrawSetShadow(fstd[15], 2);
    TextDrawSetOutline(fstd[15], 0);
    TextDrawBackgroundColor(fstd[15], 0x000000FF);
    TextDrawFont(fstd[15], 4);
    TextDrawSetProportional(fstd[15], 0);
    TextDrawSetSelectable(fstd[15], 0);

    fstd[14] = TextDrawCreate(71.000000, 225.051804, "LD_SPAC:white");
    TextDrawLetterSize(fstd[14], 0.480000, 1.120000);
    TextDrawTextSize(fstd[14], 4.000000, 6.399991);
    TextDrawAlignment(fstd[14], 1);
    TextDrawColor(fstd[14], 0x2C193BFF);
    TextDrawUseBox(fstd[14], 0);
    TextDrawBoxColor(fstd[14], 0x80808080);
    TextDrawSetShadow(fstd[14], 2);
    TextDrawSetOutline(fstd[14], 0);
    TextDrawBackgroundColor(fstd[14], 0x000000FF);
    TextDrawFont(fstd[14], 4);
    TextDrawSetProportional(fstd[14], 0);
    TextDrawSetSelectable(fstd[14], 0);

    fstd[13] = TextDrawCreate(6.299992, 224.751846, "LD_SPAC:white");
    TextDrawLetterSize(fstd[13], 0.480000, 1.120000);
    TextDrawTextSize(fstd[13], 5.000000, 11.499998);
    TextDrawAlignment(fstd[13], 1);
    TextDrawColor(fstd[13], 0x2C193BFF);
    TextDrawUseBox(fstd[13], 0);
    TextDrawBoxColor(fstd[13], 0x80808080);
    TextDrawSetShadow(fstd[13], 2);
    TextDrawSetOutline(fstd[13], 0);
    TextDrawBackgroundColor(fstd[13], 0x000000FF);
    TextDrawFont(fstd[13], 4);
    TextDrawSetProportional(fstd[13], 0);
    TextDrawSetSelectable(fstd[13], 0);

    fstd[12] = TextDrawCreate(36.000004, 222.651840, "SAFE ZONE");
    TextDrawLetterSize(fstd[12], 0.234660, 1.077329);
    TextDrawTextSize(fstd[12], 1280.000000, 1280.000000);
    TextDrawAlignment(fstd[12], 2);
    TextDrawColor(fstd[12], 0xFFFFFFFF);
    TextDrawUseBox(fstd[12], 0);
    TextDrawBoxColor(fstd[12], 0x80808080);
    TextDrawSetShadow(fstd[12], 0);
    TextDrawSetOutline(fstd[12], 0);
    TextDrawBackgroundColor(fstd[12], 0x000000FF);
    TextDrawFont(fstd[12], 1);
    TextDrawSetProportional(fstd[12], 1);
    TextDrawSetSelectable(fstd[12], 0);

    fstd[11] = TextDrawCreate(64.099998, 213.151810, "LD_BEAT:chit");
    TextDrawLetterSize(fstd[11], 0.480000, 1.120000);
    TextDrawTextSize(fstd[11], 14.000000, 17.000000);
    TextDrawAlignment(fstd[11], 1);
    TextDrawColor(fstd[11], 0x862BC2FF);
    TextDrawUseBox(fstd[11], 0);
    TextDrawBoxColor(fstd[11], 0x80808080);
    TextDrawSetShadow(fstd[11], 2);
    TextDrawSetOutline(fstd[11], 0);
    TextDrawBackgroundColor(fstd[11], 0x000000FF);
    TextDrawFont(fstd[11], 4);
    TextDrawSetProportional(fstd[11], 0);
    TextDrawSetSelectable(fstd[11], 0);

    fstd[10] = TextDrawCreate(74.800003, 213.151810, "LD_BEAT:chit");
    TextDrawLetterSize(fstd[10], 0.480000, 1.120000);
    TextDrawTextSize(fstd[10], 14.000000, 17.000000);
    TextDrawAlignment(fstd[10], 1);
    TextDrawColor(fstd[10], 0x862BC2FF);
    TextDrawUseBox(fstd[10], 0);
    TextDrawBoxColor(fstd[10], 0x80808080);
    TextDrawSetShadow(fstd[10], 2);
    TextDrawSetOutline(fstd[10], 0);
    TextDrawBackgroundColor(fstd[10], 0x000000FF);
    TextDrawFont(fstd[10], 4);
    TextDrawSetProportional(fstd[10], 0);
    TextDrawSetSelectable(fstd[10], 0);

    fstd[9] = TextDrawCreate(64.099998, 226.051804, "LD_BEAT:chit");
    TextDrawLetterSize(fstd[9], 0.480000, 1.120000);
    TextDrawTextSize(fstd[9], 14.000000, 17.000000);
    TextDrawAlignment(fstd[9], 1);
    TextDrawColor(fstd[9], 0x862BC2FF);
    TextDrawUseBox(fstd[9], 0);
    TextDrawBoxColor(fstd[9], 0x80808080);
    TextDrawSetShadow(fstd[9], 2);
    TextDrawSetOutline(fstd[9], 0);
    TextDrawBackgroundColor(fstd[9], 0x000000FF);
    TextDrawFont(fstd[9], 4);
    TextDrawSetProportional(fstd[9], 0);
    TextDrawSetSelectable(fstd[9], 0);

    fstd[8] = TextDrawCreate(74.700005, 226.051804, "LD_BEAT:chit");
    TextDrawLetterSize(fstd[8], 0.480000, 1.120000);
    TextDrawTextSize(fstd[8], 14.000000, 17.000000);
    TextDrawAlignment(fstd[8], 1);
    TextDrawColor(fstd[8], 0x862BC2FF);
    TextDrawUseBox(fstd[8], 0);
    TextDrawBoxColor(fstd[8], 0x80808080);
    TextDrawSetShadow(fstd[8], 2);
    TextDrawSetOutline(fstd[8], 0);
    TextDrawBackgroundColor(fstd[8], 0x000000FF);
    TextDrawFont(fstd[8], 4);
    TextDrawSetProportional(fstd[8], 0);
    TextDrawSetSelectable(fstd[8], 0);

    fstd[7] = TextDrawCreate(66.500008, 222.051804, "LD_SPAC:white");
    TextDrawLetterSize(fstd[7], 0.480000, 1.120000);
    TextDrawTextSize(fstd[7], 20.000000, 11.900007);
    TextDrawAlignment(fstd[7], 1);
    TextDrawColor(fstd[7], 0x862BC2FF);
    TextDrawUseBox(fstd[7], 0);
    TextDrawBoxColor(fstd[7], 0x80808080);
    TextDrawSetShadow(fstd[7], 2);
    TextDrawSetOutline(fstd[7], 0);
    TextDrawBackgroundColor(fstd[7], 0x000000FF);
    TextDrawFont(fstd[7], 4);
    TextDrawSetProportional(fstd[7], 0);
    TextDrawSetSelectable(fstd[7], 0);

    fstd[6] = TextDrawCreate(72.000000, 216.051804, "LD_SPAC:white");
    TextDrawLetterSize(fstd[6], 0.480000, 1.120000);
    TextDrawTextSize(fstd[6], 10.000000, 24.399998);
    TextDrawAlignment(fstd[6], 1);
    TextDrawColor(fstd[6], 0x862BC2FF);
    TextDrawUseBox(fstd[6], 0);
    TextDrawBoxColor(fstd[6], 0x80808080);
    TextDrawSetShadow(fstd[6], 2);
    TextDrawSetOutline(fstd[6], 0);
    TextDrawBackgroundColor(fstd[6], 0x000000FF);
    TextDrawFont(fstd[6], 4);
    TextDrawSetProportional(fstd[6], 0);
    TextDrawSetSelectable(fstd[6], 0);

    fstd[5] = TextDrawCreate(6.300007, 235.251816, "LD_SPAC:white");
    TextDrawLetterSize(fstd[5], 0.480000, 1.120000);
    TextDrawTextSize(fstd[5], 62.000000, 1.000000);
    TextDrawAlignment(fstd[5], 1);
    TextDrawColor(fstd[5], 0x862BC2FF);
    TextDrawUseBox(fstd[5], 0);
    TextDrawBoxColor(fstd[5], 0x80808080);
    TextDrawSetShadow(fstd[5], 2);
    TextDrawSetOutline(fstd[5], 0);
    TextDrawBackgroundColor(fstd[5], 0x000000FF);
    TextDrawFont(fstd[5], 4);
    TextDrawSetProportional(fstd[5], 0);
    TextDrawSetSelectable(fstd[5], 0);

    fstd[4] = TextDrawCreate(5.999998, 219.051804, "particle:lamp_shad_64");
    TextDrawLetterSize(fstd[4], 0.480000, 1.120000);
    TextDrawTextSize(fstd[4], 61.799988, 17.000000);
    TextDrawAlignment(fstd[4], 1);
    TextDrawColor(fstd[4], 0x862BC296);
    TextDrawUseBox(fstd[4], 0);
    TextDrawBoxColor(fstd[4], 0x80808080);
    TextDrawSetShadow(fstd[4], 2);
    TextDrawSetOutline(fstd[4], 0);
    TextDrawBackgroundColor(fstd[4], 0x000000FF);
    TextDrawFont(fstd[4], 4);
    TextDrawSetProportional(fstd[4], 0);
    TextDrawSetSelectable(fstd[4], 0);

    fstd[3] = TextDrawCreate(51.366673, 215.692871, "");
    TextDrawLetterSize(fstd[3], 0.480000, 1.120000);
    TextDrawTextSize(fstd[3], 49.800003, 13.000000);
    TextDrawAlignment(fstd[3], 1);
    TextDrawColor(fstd[3], 0xFFFFFFFF);
    TextDrawUseBox(fstd[3], 0);
    TextDrawBoxColor(fstd[3], 0x80808080);
    TextDrawSetShadow(fstd[3], 2);
    TextDrawSetOutline(fstd[3], 0);
    TextDrawBackgroundColor(fstd[3], 0x00000000);
    TextDrawFont(fstd[3], 5);
    TextDrawSetProportional(fstd[3], 0);
    TextDrawSetSelectable(fstd[3], 0);
    TextDrawSetPreviewModel(fstd[3], 19177);
    TextDrawSetPreviewRot(fstd[3], 0.000000, 0.000000, 0.000000, 1.000000);

    fstd[2] = TextDrawCreate(72.866661, 222.892700, "LD_SPAC:white");
    TextDrawLetterSize(fstd[2], 0.480000, 1.120000);
    TextDrawTextSize(fstd[2], 6.899999, 6.799999);
    TextDrawAlignment(fstd[2], 1);
    TextDrawColor(fstd[2], 0xFFFFFFFF);
    TextDrawUseBox(fstd[2], 0);
    TextDrawBoxColor(fstd[2], 0x80808080);
    TextDrawSetShadow(fstd[2], 2);
    TextDrawSetOutline(fstd[2], 0);
    TextDrawBackgroundColor(fstd[2], 0x000000FF);
    TextDrawFont(fstd[2], 4);
    TextDrawSetProportional(fstd[2], 0);
    TextDrawSetSelectable(fstd[2], 0);

    fstd[1] = TextDrawCreate(66.366653, 239.992462, "");
    TextDrawLetterSize(fstd[1], 0.480000, 1.120000);
    TextDrawTextSize(fstd[1], 19.900000, -11.000000);
    TextDrawAlignment(fstd[1], 1);
    TextDrawColor(fstd[1], 0xFFFFFFFF);
    TextDrawUseBox(fstd[1], 0);
    TextDrawBoxColor(fstd[1], 0x80808080);
    TextDrawSetShadow(fstd[1], 2);
    TextDrawSetOutline(fstd[1], 0);
    TextDrawBackgroundColor(fstd[1], 0x00000000);
    TextDrawFont(fstd[1], 5);
    TextDrawSetProportional(fstd[1], 0);
    TextDrawSetSelectable(fstd[1], 0);
    TextDrawSetPreviewModel(fstd[1], 19177);
    TextDrawSetPreviewRot(fstd[1], 0.000000, 0.000000, 0.000000, 1.000000);

    fstd[0] = TextDrawCreate(71.266655, 223.592560, "LD_BEAT:chit");
    TextDrawLetterSize(fstd[0], 0.480000, 1.120000);
    TextDrawTextSize(fstd[0], 10.100000, 12.799999);
    TextDrawAlignment(fstd[0], 1);
    TextDrawColor(fstd[0], 0xFFFFFFFF);
    TextDrawUseBox(fstd[0], 0);
    TextDrawBoxColor(fstd[0], 0x80808080);
    TextDrawSetShadow(fstd[0], 2);
    TextDrawSetOutline(fstd[0], 0);
    TextDrawBackgroundColor(fstd[0], 0x000000FF);
    TextDrawFont(fstd[0], 4);
    TextDrawSetProportional(fstd[0], 0);
    TextDrawSetSelectable(fstd[0], 0);
}

stock TDShow(playerid)
{
    for(new i = 0; i < 19; i++)
        TextDrawShowForPlayer(playerid, fstd[i]);
}

stock TDHide(playerid)
{
    for(new i = 0; i < 19; i++)
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