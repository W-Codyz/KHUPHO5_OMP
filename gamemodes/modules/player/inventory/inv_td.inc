#include <YSI\YSI_Coding\y_hooks>

#define INVTD_MAX_SLOTS 28
#define INV_WEAPON_OFFSET 1000 // InvTDSlot >= offset => weapon (slot = val - offset)

new Text:        iTD_g   [2];
new Text:        iTD_bar [INVTD_MAX_SLOTS]; // global — vị trí cố định, tiết kiệm per-player TD
new PlayerText:  iTD_box [MAX_PLAYERS][INVTD_MAX_SLOTS];
new PlayerText:  iTD_mdl [MAX_PLAYERS][INVTD_MAX_SLOTS];
new PlayerText:  iTD_name[MAX_PLAYERS][INVTD_MAX_SLOTS];
new PlayerText:  iTD_qty [MAX_PLAYERS][INVTD_MAX_SLOTS];
new PlayerText:  iTD_pstr[MAX_PLAYERS][2];
// [0]=BagModel  [1]=PlayerName

new InvTDSlot[MAX_PLAYERS][INVTD_MAX_SLOTS];
new bool: g_InvTDCreated[MAX_PLAYERS];
// InvTDOpen[MAX_PLAYERS] declared in header.inc

// ======================================================================
// 3D model mapping
// ======================================================================
static GetInvItemModel(itemVariable)
{
    if(itemVariable == pBanhMi)    return 19579;
    if(itemVariable == pNuocLoc)   return 19570;
    if(itemVariable == pTepCanSa)  return 1575;
    if(itemVariable == pCanSa)     return 19473;
    if(itemVariable == pPot)       return 1576;
    if(itemVariable == pCrack)     return 1580;
    if(itemVariable == pPaper)     return 19805;
    if(itemVariable == pPizza)     return 2702;
    if(itemVariable == pBunRieu)   return 19993;
    if(itemVariable == pPhoBo)     return 19567;
    if(itemVariable == pRadio)     return 19942;
    if(itemVariable == pDice)      return 1915;
    if(itemVariable == pCredits)   return 1212;
    if(itemVariable == pRope)      return 11747;
    if(itemVariable == pCigar)     return 19896;
    if(itemVariable == pMoiCau)    return 1603;
    if(itemVariable == pHamburger) return 2703;
    if(itemVariable == pCaPhe)     return 19835;
    if(itemVariable == pSuaBo)     return 19569;
    if(itemVariable == pThit)      return 2806;
    if(itemVariable == pTraiCay)   return 19574;
    if(itemVariable == pNuocEp)    return 19563;
    if(itemVariable == pMinSat)    return 3930;
    if(itemVariable == pMinDong)   return 3930;
    if(itemVariable == pMinBac)    return 3930;
    if(itemVariable == pMinVang)   return 3930;
    if(itemVariable == pMinKimCuong) return 3930;
    return 2663;
}

static GetInvItemModelColor(itemVariable)
{
    if(itemVariable == pMinBac)      return -1329275137;
    if(itemVariable == pMinDong)     return -626712321;
    if(itemVariable == pMinVang)     return -65281;
    if(itemVariable == pMinKimCuong) return 16777215;
    return -1;
}

static GetWeaponModel(weaponid)
{
    switch(weaponid)
    {
        case 1:  return 331; case 2:  return 333; case 3:  return 334;
        case 4:  return 335; case 5:  return 336; case 6:  return 337;
        case 7:  return 338; case 8:  return 339; case 9:  return 341;
        case 16: return 342; case 17: return 343; case 18: return 344;
        case 22: return 346; case 23: return 347; case 24: return 348;
        case 25: return 349; case 26: return 350; case 27: return 351;
        case 28: return 352; case 29: return 353; case 30: return 355;
        case 31: return 356; case 32: return 372; case 33: return 357;
        case 34: return 358; case 35: return 359; case 36: return 360;
        case 37: return 361; case 38: return 362; case 39: return 363;
        case 40: return 364; case 41: return 365; case 42: return 366;
        case 43: return 367; case 44: return 368; case 45: return 369;
        case 46: return 371;
    }
    return 2663;
}

// ======================================================================
// Global textdraws
// ======================================================================
static CreateInvTDGlobal()
{
    // Header bar
    iTD_g[0] = TextDrawCreate(183.000, 112.000, "LD_SPAC:white");
    TextDrawTextSize(iTD_g[0], 278.000, 16.000);
    TextDrawAlignment(iTD_g[0], 1);
    TextDrawColor(iTD_g[0], -1732209921);
    TextDrawSetShadow(iTD_g[0], 0);
    TextDrawSetOutline(iTD_g[0], 0);
    TextDrawBackgroundColor(iTD_g[0], 255);
    TextDrawFont(iTD_g[0], 4);
    TextDrawSetProportional(iTD_g[0], 1);

    // X close button
    iTD_g[1] = TextDrawCreate(444.000, 112.000, "LD_BEAT:cross");
    TextDrawTextSize(iTD_g[1], 16.000, 16.000);
    TextDrawAlignment(iTD_g[1], 1);
    TextDrawColor(iTD_g[1], -10197816);
    TextDrawSetShadow(iTD_g[1], 0);
    TextDrawSetOutline(iTD_g[1], 0);
    TextDrawBackgroundColor(iTD_g[1], 255);
    TextDrawFont(iTD_g[1], 4);
    TextDrawSetProportional(iTD_g[1], 1);
    TextDrawSetSelectable(iTD_g[1], 1);

    // Accent bars — global, cố định theo grid 4×7
    static const Float:g_barY_g[4] = {174.0, 225.0, 275.0, 326.0};
    for(new row = 0; row < 4; row++) {
        for(new col = 0; col < 7; col++) {
            new s = row * 7 + col;
            new Float:bx = 183.0 + col * 40.0;
            iTD_bar[s] = TextDrawCreate(bx, g_barY_g[row], "LD_SPAC:white");
            TextDrawTextSize(iTD_bar[s], 38.000, 3.000);
            TextDrawAlignment(iTD_bar[s], 1);
            TextDrawColor(iTD_bar[s], -1732209921);
            TextDrawSetShadow(iTD_bar[s], 0);
            TextDrawSetOutline(iTD_bar[s], 0);
            TextDrawBackgroundColor(iTD_bar[s], 255);
            TextDrawFont(iTD_bar[s], 4);
            TextDrawSetProportional(iTD_bar[s], 1);
        }
    }
}

// ======================================================================
// Per-player textdraws
// ======================================================================
static CreateInvTDPlayer(playerid)
{
    // Coordinate lookup tables — slot = row*7 + col, X = 183.0 + col*40.0
    static const Float:g_boxY [4] = {128.0, 179.0, 229.0, 280.0};
    static const Float:g_mdlY [4] = {133.0, 183.0, 233.0, 283.0};
    static const Float:g_nameY[4] = {167.0, 217.0, 268.0, 318.0};
    static const Float:g_qtyY [4] = {129.0, 179.0, 230.0, 280.0};

    for(new row = 0; row < 4; row++)
    {
        for(new col = 0; col < 7; col++)
        {
            new s      = row * 7 + col;
            new Float:bx = 183.0 + col * 40.0;
            new Float:nx = bx + 19.0; // name/qty X

            // Box
            iTD_box[playerid][s] = CreatePlayerTextDraw(playerid, bx, g_boxY[row], "LD_SPAC:white");
            PlayerTextDrawTextSize(playerid, iTD_box[playerid][s], 38.000, 49.000);
            PlayerTextDrawAlignment(playerid, iTD_box[playerid][s], 1);
            PlayerTextDrawColor(playerid, iTD_box[playerid][s], 135);
            PlayerTextDrawSetShadow(playerid, iTD_box[playerid][s], 0);
            PlayerTextDrawSetOutline(playerid, iTD_box[playerid][s], 0);
            PlayerTextDrawBackgroundColor(playerid, iTD_box[playerid][s], 255);
            PlayerTextDrawFont(playerid, iTD_box[playerid][s], 4);
            PlayerTextDrawSetProportional(playerid, iTD_box[playerid][s], 1);
            PlayerTextDrawSetSelectable(playerid, iTD_box[playerid][s], 1);

            // Model
            iTD_mdl[playerid][s] = CreatePlayerTextDraw(playerid, bx, g_mdlY[row], "_");
            PlayerTextDrawTextSize(playerid, iTD_mdl[playerid][s], 38.000, 39.000);
            PlayerTextDrawAlignment(playerid, iTD_mdl[playerid][s], 1);
            PlayerTextDrawColor(playerid, iTD_mdl[playerid][s], -1);
            PlayerTextDrawSetShadow(playerid, iTD_mdl[playerid][s], 0);
            PlayerTextDrawSetOutline(playerid, iTD_mdl[playerid][s], 0);
            PlayerTextDrawBackgroundColor(playerid, iTD_mdl[playerid][s], 0);
            PlayerTextDrawFont(playerid, iTD_mdl[playerid][s], 5);
            PlayerTextDrawSetProportional(playerid, iTD_mdl[playerid][s], 0);
            PlayerTextDrawSetPreviewModel(playerid, iTD_mdl[playerid][s], 2663);
            PlayerTextDrawSetPreviewRot(playerid, iTD_mdl[playerid][s], 0.000, 0.000, 0.000, 1.000);
            PlayerTextDrawSetPreviewVehCol(playerid, iTD_mdl[playerid][s], 0, 0);

            // Name
            iTD_name[playerid][s] = CreatePlayerTextDraw(playerid, nx, g_nameY[row], "Trong");
            PlayerTextDrawLetterSize(playerid, iTD_name[playerid][s], 0.159, 0.799);
            PlayerTextDrawAlignment(playerid, iTD_name[playerid][s], 2);
            PlayerTextDrawColor(playerid, iTD_name[playerid][s], -1);
            PlayerTextDrawSetShadow(playerid, iTD_name[playerid][s], 0);
            PlayerTextDrawSetOutline(playerid, iTD_name[playerid][s], 0);
            PlayerTextDrawBackgroundColor(playerid, iTD_name[playerid][s], 150);
            PlayerTextDrawFont(playerid, iTD_name[playerid][s], 1);
            PlayerTextDrawSetProportional(playerid, iTD_name[playerid][s], 1);

            // Qty
            iTD_qty[playerid][s] = CreatePlayerTextDraw(playerid, nx, g_qtyY[row], "0x");
            PlayerTextDrawLetterSize(playerid, iTD_qty[playerid][s], 0.108, 0.699);
            PlayerTextDrawAlignment(playerid, iTD_qty[playerid][s], 2);
            PlayerTextDrawColor(playerid, iTD_qty[playerid][s], -1);
            PlayerTextDrawSetShadow(playerid, iTD_qty[playerid][s], 0);
            PlayerTextDrawSetOutline(playerid, iTD_qty[playerid][s], 0);
            PlayerTextDrawBackgroundColor(playerid, iTD_qty[playerid][s], 150);
            PlayerTextDrawFont(playerid, iTD_qty[playerid][s], 1);
            PlayerTextDrawSetProportional(playerid, iTD_qty[playerid][s], 1);
        }
    }

    // Bag model preview
    iTD_pstr[playerid][0] = CreatePlayerTextDraw(playerid, 180.000, 103.000, "_");
    PlayerTextDrawTextSize(playerid, iTD_pstr[playerid][0], 25.000, 32.000);
    PlayerTextDrawAlignment(playerid, iTD_pstr[playerid][0], 1);
    PlayerTextDrawColor(playerid, iTD_pstr[playerid][0], -1);
    PlayerTextDrawSetShadow(playerid, iTD_pstr[playerid][0], 0);
    PlayerTextDrawSetOutline(playerid, iTD_pstr[playerid][0], 0);
    PlayerTextDrawBackgroundColor(playerid, iTD_pstr[playerid][0], 0);
    PlayerTextDrawFont(playerid, iTD_pstr[playerid][0], 5);
    PlayerTextDrawSetProportional(playerid, iTD_pstr[playerid][0], 0);
    PlayerTextDrawSetPreviewModel(playerid, iTD_pstr[playerid][0], 2919);
    PlayerTextDrawSetPreviewRot(playerid, iTD_pstr[playerid][0], 0.000, 0.000, 0.000, 1.000);
    PlayerTextDrawSetPreviewVehCol(playerid, iTD_pstr[playerid][0], 0, 0);

    // Player name
    iTD_pstr[playerid][1] = CreatePlayerTextDraw(playerid, 203.000, 116.000, "Player");
    PlayerTextDrawLetterSize(playerid, iTD_pstr[playerid][1], 0.170, 0.898);
    PlayerTextDrawAlignment(playerid, iTD_pstr[playerid][1], 1);
    PlayerTextDrawColor(playerid, iTD_pstr[playerid][1], -1);
    PlayerTextDrawSetShadow(playerid, iTD_pstr[playerid][1], 0);
    PlayerTextDrawSetOutline(playerid, iTD_pstr[playerid][1], 0);
    PlayerTextDrawBackgroundColor(playerid, iTD_pstr[playerid][1], 150);
    PlayerTextDrawFont(playerid, iTD_pstr[playerid][1], 1);
    PlayerTextDrawSetProportional(playerid, iTD_pstr[playerid][1], 1);
}

static DestroyInvTDPlayer(playerid)
{
    for(new i = 0; i < INVTD_MAX_SLOTS; i++)
    {
        if(iTD_box[playerid][i] != INVALID_PLAYER_TEXT_DRAW) {
            PlayerTextDrawDestroy(playerid, iTD_box[playerid][i]);
            PlayerTextDrawDestroy(playerid, iTD_mdl[playerid][i]);
            PlayerTextDrawDestroy(playerid, iTD_name[playerid][i]);
            PlayerTextDrawDestroy(playerid, iTD_qty[playerid][i]);
            iTD_box[playerid][i]  = INVALID_PLAYER_TEXT_DRAW;
            iTD_mdl[playerid][i]  = INVALID_PLAYER_TEXT_DRAW;
            iTD_name[playerid][i] = INVALID_PLAYER_TEXT_DRAW;
            iTD_qty[playerid][i]  = INVALID_PLAYER_TEXT_DRAW;
        }
    }
    for(new i = 0; i < 2; i++) {
        if(iTD_pstr[playerid][i] != INVALID_PLAYER_TEXT_DRAW) {
            PlayerTextDrawDestroy(playerid, iTD_pstr[playerid][i]);
            iTD_pstr[playerid][i] = INVALID_PLAYER_TEXT_DRAW;
        }
    }
}

// ======================================================================
// Helpers
// ======================================================================
static BuildInvTDSlotMap(playerid)
{
    for(new i = 0; i < INVTD_MAX_SLOTS; i++) InvTDSlot[playerid][i] = -1;
    new slot = 0;
    for(new i = 0; i < sizeof(ItemInventory) && slot < INVTD_MAX_SLOTS; i++)
    {
        if(PlayerInfo[playerid][ItemInventory[i][pVariable]] > 0)
            InvTDSlot[playerid][slot++] = i;
    }
    // Add weapons — same pGuns filter as dialog inventory
    new myweapons[13][2];
    for(new i = 0; i < 13 && slot < INVTD_MAX_SLOTS; i++)
    {
        GetPlayerWeaponData(playerid, i, myweapons[i][0], myweapons[i][1]);
        if(myweapons[i][0] > 0 && PlayerInfo[playerid][pGuns][i] == myweapons[i][0])
            InvTDSlot[playerid][slot++] = INV_WEAPON_OFFSET + i;
    }
}

static PopulateInvTDSlots(playerid)
{
    new str[32];
    new count = 0;
    for(new i = 0; i < INVTD_MAX_SLOTS; i++)
    {
        new slotVal = InvTDSlot[playerid][i];
        if(slotVal != -1)
        {
            count++;
            if(slotVal >= INV_WEAPON_OFFSET)
            {
                new wslot = slotVal - INV_WEAPON_OFFSET;
                new weaponid, ammo;
                GetPlayerWeaponData(playerid, wslot, weaponid, ammo);
                new weaponname[32];
                GetWeaponName(weaponid, weaponname, sizeof(weaponname));
                PlayerTextDrawSetPreviewModel(playerid, iTD_mdl[playerid][i], GetWeaponModel(weaponid));
                PlayerTextDrawShow(playerid, iTD_mdl[playerid][i]);
                PlayerTextDrawSetString(playerid, iTD_name[playerid][i], weaponname);
                PlayerTextDrawShow(playerid, iTD_name[playerid][i]);
                format(str, sizeof(str), "%dx", ammo);
                PlayerTextDrawSetString(playerid, iTD_qty[playerid][i], str);
                PlayerTextDrawShow(playerid, iTD_qty[playerid][i]);
            }
            else
            {
                new qty = PlayerInfo[playerid][ItemInventory[slotVal][pVariable]];
                new itemVar = _:ItemInventory[slotVal][pVariable];
                PlayerTextDrawSetPreviewModel(playerid, iTD_mdl[playerid][i], GetInvItemModel(itemVar));
                PlayerTextDrawColor(playerid, iTD_mdl[playerid][i], GetInvItemModelColor(itemVar));
                PlayerTextDrawShow(playerid, iTD_mdl[playerid][i]);
                PlayerTextDrawSetString(playerid, iTD_name[playerid][i], ItemInventory[slotVal][NameItem]);
                PlayerTextDrawShow(playerid, iTD_name[playerid][i]);
                format(str, sizeof(str), "%dx", qty);
                PlayerTextDrawSetString(playerid, iTD_qty[playerid][i], str);
                PlayerTextDrawShow(playerid, iTD_qty[playerid][i]);
            }
            PlayerTextDrawColor(playerid, iTD_box[playerid][i], 135);
        }
        else
        {
            PlayerTextDrawHide(playerid, iTD_mdl[playerid][i]);
            PlayerTextDrawSetString(playerid, iTD_name[playerid][i], "_");
            PlayerTextDrawShow(playerid, iTD_name[playerid][i]);
            PlayerTextDrawSetString(playerid, iTD_qty[playerid][i], "_");
            PlayerTextDrawHide(playerid, iTD_qty[playerid][i]);
            PlayerTextDrawColor(playerid, iTD_box[playerid][i], 80);
        }
        PlayerTextDrawShow(playerid, iTD_box[playerid][i]);
    }
    format(str, sizeof(str), "%s - %d/28", GetPlayerNameEx(playerid), count);
    PlayerTextDrawSetString(playerid, iTD_pstr[playerid][1], str);
}

// ======================================================================
// Public API
// ======================================================================
public ShowInvTD(playerid)
{
    if(!IsPlayerConnected(playerid)) return 0;

    if(!g_InvTDCreated[playerid]) {
        CreateInvTDPlayer(playerid);
        g_InvTDCreated[playerid] = true;
    }

    InvTDOpen[playerid] = true;
    BuildInvTDSlotMap(playerid);

    for(new i = 0; i < 2; i++)
        TextDrawShowForPlayer(playerid, iTD_g[i]);
    for(new i = 0; i < INVTD_MAX_SLOTS; i++)
        TextDrawShowForPlayer(playerid, iTD_bar[i]);

    PlayerTextDrawShow(playerid, iTD_pstr[playerid][0]);
    PlayerTextDrawSetString(playerid, iTD_pstr[playerid][1], GetPlayerNameEx(playerid));
    PlayerTextDrawShow(playerid, iTD_pstr[playerid][1]);

    PopulateInvTDSlots(playerid);
    PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);
    SelectTextDraw(playerid, 0xFFFFFF88);
    return 1;
}

public HideInvTD(playerid)
{
    if(!InvTDOpen[playerid]) return 0;
    InvTDOpen[playerid] = false;

    for(new i = 0; i < 2; i++)
        TextDrawHideForPlayer(playerid, iTD_g[i]);
    for(new i = 0; i < INVTD_MAX_SLOTS; i++)
        TextDrawHideForPlayer(playerid, iTD_bar[i]);

    for(new i = 0; i < INVTD_MAX_SLOTS; i++) {
        PlayerTextDrawHide(playerid, iTD_box[playerid][i]);
        PlayerTextDrawHide(playerid, iTD_mdl[playerid][i]);
        PlayerTextDrawHide(playerid, iTD_name[playerid][i]);
        PlayerTextDrawHide(playerid, iTD_qty[playerid][i]);
    }
    for(new i = 0; i < 2; i++)
        PlayerTextDrawHide(playerid, iTD_pstr[playerid][i]);

    DestroyInvTDPlayer(playerid);
    g_InvTDCreated[playerid] = false;
    PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);
    CancelSelectTextDraw(playerid);
    return 1;
}

stock RefreshInvTD(playerid)
{
    if(!InvTDOpen[playerid] || !g_InvTDCreated[playerid]) return 0;
    BuildInvTDSlotMap(playerid);
    PopulateInvTDSlots(playerid);
    PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);
    return 1;
}

// ======================================================================
// Hooks
// ======================================================================
hook OnGameModeInit()
{
    CreateInvTDGlobal();
    return 1;
}

hook OnPlayerConnect(playerid)
{
    for(new i = 0; i < INVTD_MAX_SLOTS; i++) {
        iTD_box[playerid][i]  = INVALID_PLAYER_TEXT_DRAW;
        iTD_mdl[playerid][i]  = INVALID_PLAYER_TEXT_DRAW;
        iTD_name[playerid][i] = INVALID_PLAYER_TEXT_DRAW;
        iTD_qty[playerid][i]  = INVALID_PLAYER_TEXT_DRAW;
    }
    for(new i = 0; i < 2; i++)
        iTD_pstr[playerid][i] = INVALID_PLAYER_TEXT_DRAW;
    InvTDOpen[playerid]      = false;
    g_InvTDCreated[playerid] = false;
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    if(g_InvTDCreated[playerid])
        DestroyInvTDPlayer(playerid);
    InvTDOpen[playerid]      = false;
    g_InvTDCreated[playerid] = false;
    return 1;
}

hook OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    if(!InvTDOpen[playerid]) return 0;
    if(clickedid == INVALID_TEXT_DRAW || clickedid == iTD_g[1])
    {
        if(GetPVarInt(playerid, "DelayInv") == 1)
        {
            DeletePVar(playerid, "DelayInv");
            return 1;
        }
        HideInvTD(playerid);
        return 1;
    }
    return 0;
}

hook OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
    if(!InvTDOpen[playerid]) return 0;
    for(new i = 0; i < INVTD_MAX_SLOTS; i++)
    {
        if(playertextid == iTD_box[playerid][i])
        {
            new slotVal = InvTDSlot[playerid][i];
            if(slotVal == -1) return 1;
            HideInvTD(playerid); // set InvTDOpen=false truoc CancelSelectTextDraw, tranh double-sound
            if(slotVal >= INV_WEAPON_OFFSET)
            {
                new wslot = slotVal - INV_WEAPON_OFFSET;
                new weaponid, ammo;
                GetPlayerWeaponData(playerid, wslot, weaponid, ammo);
                ShowWeaponOptions(playerid, weaponid, ammo);
            }
            else
            {
                ShowItemOptions(playerid, slotVal);
            }
            return 1;
        }
    }
    return 0;
}
