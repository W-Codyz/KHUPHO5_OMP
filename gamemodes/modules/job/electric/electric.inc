// ============================================================
//  Electric Job - GTA-HTB Roleplay
//  Cong viec sua dien, mini-game Stroop Effect
// ============================================================

#include <YSI\YSI_Coding\y_hooks>

// --- Defines ---
#define IsElectricanWorking(%0)         PlayerElectricJob[%0][E_EJ_Working]
#define SetElectricanWorking(%0,%1)     PlayerElectricJob[%0][E_EJ_Working] = %1
#define ELECTRIC_KIT_SLOT               8
#define ELECTRIC_MAX_POINTS             14

// --- Enum ---
enum E_ELECTRICJOB {
    E_EJ_GroupName[64],
    E_EJ_GroupID,
    E_EJ_PartnerID,
    E_EJ_VehicleID,
    bool:E_EJ_Locked,
    E_EJ_FixCount,
    E_EJ_LeaderID,
    bool:E_EJ_Working
};
new PlayerElectricJob[MAX_PLAYERS][E_ELECTRICJOB];

new STREAMER_TAG_CP:gElectricCP[MAX_PLAYERS][ELECTRIC_MAX_POINTS];
new STREAMER_TAG_MAP_ICON:gElectricIcon[MAX_PLAYERS][ELECTRIC_MAX_POINTS];
new STREAMER_TAG_RACE_CP:gElectricFinish[MAX_PLAYERS];
new bool:gElectricFixed[MAX_PLAYERS][ELECTRIC_MAX_POINTS];
new gToggleElectric = 0;
new Text3D:gElectricVehLabel[MAX_PLAYERS];

// --- Toa do 13 diem sua dien ---
static const Float:sElectricPoints[ELECTRIC_MAX_POINTS][3] = {
    {-2610.368652, -199.188369,   4.335937},  // 0
    {-2362.322753,  -79.038002,  35.320312},  // 1
    {-2013.339843,  119.651351,  27.687500},  // 2
    {-2258.612304,  329.607635,  35.320312},  // 3
    {-2598.649414,  392.402282,  10.223828},  // 4
    {-2614.532958,  576.747619,  14.601562},  // 5
    {-2421.941650,  718.472534,  35.171875},  // 6
    {-2246.216308,  796.350952,  49.445312},  // 7
    {-1907.464111,  829.003479,  35.171875},  // 8
    {-1544.866455,  903.848571,   7.265625},  // 9
    {-1520.063842,  939.793945,   7.187500},  // 10
    {-1721.241943, 1321.319946,   7.187500},  // 11
    {-1980.112304, 1167.926269,  45.445312},  // 12
    {-1650.144165, 425.219268, 7.179687}   // 13
};

// --- Dap an dung cho 13 thach thuc (theo mau chu, khong phai nghia tu) ---
// Bang mau: Do / Xanh / Lam / Trang / Vang / Tim / Den / Xam / Cam
static const sElectricAnswers[ELECTRIC_MAX_POINTS][3][16] = {
    {"Do",    "Xanh",  "Trang"},  // 0
    {"Xanh",  "Vang",  "Do"},     // 1
    {"Tim",   "Do",    "Vang"},   // 2
    {"Do",    "Vang",  "Xanh"},   // 3
    {"Vang",  "Tim",   "Do"},     // 4
    {"Tim",   "Tim",   "Do"},     // 5
    {"Do",    "Den",   "Xanh"},   // 6
    {"Lam",   "Tim",   "Xanh"},   // 7
    {"Lam",   "Do",    "Xanh"},   // 8
    {"Do",    "Xanh",  "Tim"},    // 9
    {"Vang",  "Vang",  "Do"},     // 10
    {"Xam",   "Xanh",  "Do"},     // 11
    {"Cam",   "Xanh",  "Tim"},    // 12
    {"Lam",   "Den",   "Vang"}    // 13
};

// --- Noi dung hien thi trong dialog (mau chu KHAC nghia tu - Stroop Effect) ---
// Chu thich: mau hex la mau THAT cua chu; tu trong ngoac [] la nghia SAI (danh lua)
static const sElectricDisplay[ELECTRIC_MAX_POINTS][72] = {
    "{FF0000}[VANG]   {00FF00}[LAM]   {FFFFFF}[DO]",    // 0:  Do    Xanh  Trang
    "{00FF00}[DO]   {FFFF00}[XANH]   {FF0000}[TIM]",    // 1:  Xanh  Vang  Do
    "{AA00FF}[XAM]   {FF0000}[VANG]   {FFFF00}[TIM]",   // 2:  Tim   Do    Vang
    "{FF0000}[XANH]   {FFFF00}[TIM]   {00FF00}[DO]",    // 3:  Do    Vang  Xanh
    "{FFFF00}[VANG]   {AA00FF}[TIM]   {FF0000}[VANG]",  // 4:  Vang  Tim   Do
    "{AA00FF}[VANG]   {AA00FF}[LAM]   {FF0000}[XANH]",  // 5:  Tim   Tim   Do
    "{FF0000}[VANG]   {222222}[LAM]   {00FF00}[DO]",    // 6:  Do    Den   Xanh
    "{0055FF}[XANH]   {AA00FF}[VANG]   {00FF00}[DO]",   // 7:  Lam   Tim   Xanh
    "{0055FF}[VANG]   {FF0000}[TIM]   {00FF00}[DO]",    // 8:  Lam   Do    Xanh
    "{FF0000}[VANG]   {00FF00}[LAM]   {AA00FF}[DO]",    // 9:  Do    Xanh  Tim
    "{FFFF00}[TIM]   {FFFF00}[LAM]   {FF0000}[CAM]",    // 10: Vang  Vang  Do
    "{C0C0C0}[XAM]   {00FF00}[LAM]   {FF0000}[VANG]",  // 11: Xam   Xanh  Do
    "{FF8000}[DO]   {00FF00}[CAM]   {AA00FF}[XAM]",     // 12: Cam   Xanh  Tim
    "{0055FF}[TIM]   {555555}[XANH]   {FFFF00}[DO]"    // 13: Lam   Den   Vang
};

// ============================================================
//  Helper: kiem tra chuoi co phai so nguyen khong
// ============================================================
static bool:Electric_IsNumeric(const str[])
{
    if(!str[0]) return false;
    for(new i = 0; str[i]; i++) {
        if(str[i] < '0' || str[i] > '9') return false;
    }
    return true;
}

// ============================================================
//  Reset bien cua player
// ============================================================
stock Electric_ResetVars(playerid)
{
    SetElectricanWorking(playerid, false);
    PlayerWorking[playerid] = 0;
    PlayerElectricJob[playerid][E_EJ_GroupName][0] = EOS;
    PlayerElectricJob[playerid][E_EJ_GroupID]  = -1;
    PlayerElectricJob[playerid][E_EJ_LeaderID] = INVALID_PLAYER_ID;
    PlayerElectricJob[playerid][E_EJ_PartnerID]= INVALID_PLAYER_ID;
    PlayerElectricJob[playerid][E_EJ_VehicleID]= INVALID_VEHICLE_ID;
    PlayerElectricJob[playerid][E_EJ_Locked]   = false;
    PlayerElectricJob[playerid][E_EJ_FixCount] = 0;

    for(new i = 0; i < ELECTRIC_MAX_POINTS; i++) {
        if(DestroyDynamicCP(gElectricCP[playerid][i]))
            gElectricCP[playerid][i] = STREAMER_TAG_CP:INVALID_STREAMER_ID;
        if(DestroyDynamicMapIcon(gElectricIcon[playerid][i]))
            gElectricIcon[playerid][i] = STREAMER_TAG_MAP_ICON:INVALID_STREAMER_ID;
        gElectricFixed[playerid][i] = false;
    }
    if(DestroyDynamicRaceCP(gElectricFinish[playerid]))
        gElectricFinish[playerid] = STREAMER_TAG_RACE_CP:INVALID_STREAMER_ID;
    if(gElectricVehLabel[playerid] != Text3D:INVALID_3DTEXT_ID) {
        Delete3DTextLabel(gElectricVehLabel[playerid]);
        gElectricVehLabel[playerid] = Text3D:INVALID_3DTEXT_ID;
    }
    return 1;
}

// ============================================================
//  Stop: dung lam viec (goi tu /dunglamviec hoac menu case 5)
// ============================================================
stock Electric_Stop(playerid)
{
    if(!IsElectricanWorking(playerid)) return 0;

    // Destroy xe cong tac truoc
    new vid = PlayerElectricJob[playerid][E_EJ_VehicleID];
    if(IsValidVehicle(vid))
    {
        DestroyVehicle(vid);
    }
    PlayerElectricJob[playerid][E_EJ_VehicleID] = INVALID_VEHICLE_ID;
    PlayerElectricJob[playerid][E_EJ_Locked]    = false;

    // Thong bao partner neu co
    new partner = PlayerElectricJob[playerid][E_EJ_PartnerID];
    if(partner != INVALID_PLAYER_ID && IsPlayerConnected(partner))
    {
        PlayerElectricJob[partner][E_EJ_VehicleID] = INVALID_VEHICLE_ID;
        PlayerElectricJob[partner][E_EJ_Locked]    = false;
        PlayerElectricJob[partner][E_EJ_PartnerID] = INVALID_PLAYER_ID;
        SendClientMessageEx(partner, COLOR_RED,
            "[Sua Dien] %s da roi nhom, xe cong tac bi thu hoi.", GetPlayerNameEx(playerid));
    }

    if(GetPVarInt(playerid, "EJ_HasKit"))
    {
        RemovePlayerAttachedObject(playerid, ELECTRIC_KIT_SLOT);
        DeletePVar(playerid, "EJ_HasKit");
    }

    SetPlayerSkin(playerid, PlayerInfo[playerid][pModel]);
    {
        new string[128];
        format(string, sizeof(string), "{FF8000}* {C2A2DA}%s da ngung lam viec Sua Dien.", GetPlayerNameEx(playerid));
        ProxDetectorWrap(playerid, string, 92, 30.0, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
    }
    Electric_ResetVars(playerid);
    ShowTDN(playerid, NOTIFICATION_INFO, "Ban da ngung lam viec Sua Dien.");
    return 1;
}

// ============================================================
//  Dat skin theo gioi tinh
// ============================================================
stock Electric_SetSkin(playerid)
{
    SetPlayerSkin(playerid, (PlayerInfo[playerid][pSex] == 1) ? 16 : 157);
    return 1;
}

// ============================================================
//  Hien thi dialog sua dien (theo chi so diem 0-29)
// ============================================================
stock Electric_ShowFixDialog(playerid, index)
{
    new body[256];
    format(body, sizeof(body), "Nhin MAU THAT SU cua tung chu roi nhap, cach nhau dau cach.\n\n%s\n\n{AAAAAA}Su dung {5CFF77}\"/xemday\" {AAAAAA}de biet chi tiet mau\n{FF4444}(Nhin mau chu - khong phai nghia tu trong ngoac [])", sElectricDisplay[index]);
    SetPVarInt(playerid, "EJ_FixIndex", index);
    ShowPlayerDialog(playerid, DIALOG_ELECTRIC_FIX,
        DIALOG_STYLE_INPUT, "{F5D000}GTA-HTB Roleplay {FFFFFF}- Sua Dien",
        body, "Nhap", "Huy");
    return 1;
}

// ============================================================
//  Callback: sau khi loading 10 giay, hoan tat sua dien
// ============================================================
forward Electric_FixFinish(playerid);
public Electric_FixFinish(playerid)
{
    if(!IsElectricanWorking(playerid)) return 1;

    new index = GetPVarInt(playerid, "EJ_FixIndex");
    if(index < 0 || index >= ELECTRIC_MAX_POINTS) return 1;

    new pay = 2000 + random(1001);
    if (PlayerElectricJob[playerid][E_EJ_PartnerID] != INVALID_PLAYER_ID) pay += 500; // thưởng thêm khi có partner
    GivePlayerCash(playerid, pay);
    PlayerPlaySound(playerid, 32000, 0.0, 0.0, 0.0);

    new msg[128];
    format(msg, sizeof(msg), "Sua dien thanh cong! Ban nhan duoc {3BFF6F}$%d{ffffff} o lan sua dien nay.", pay);
    SendClientMessageEx(playerid, COLOR_WHITE, msg);
    {
        new string[128];
        format(string, sizeof(string), "{FF8000}* {C2A2DA}%s sua dien thanh cong!", GetPlayerNameEx(playerid));
        ProxDetectorWrap(playerid, string, 92, 30.0, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
    }
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Sua dien thanh cong!");

    // XP
    new rand_xp;
    if(DoubleXP || PlayerInfo[playerid][pDoubleEXP] > 0) {
        rand_xp = Random(PlayerInfo[playerid][pLevel] * XP_RATE - 5, PlayerInfo[playerid][pLevel] * XP_RATE - 2);
        GivePlayerXP(playerid, rand_xp * 2);
        SendClientMessageEx(playerid, COLOR_YELLOW,
            "Kinh nghiem nhan vat: +%d XP (%s/%s) (DOUBLE XP)",
            rand_xp * 2,
            number_format(PlayerInfo[playerid][pXP]),
            number_format(GetRequiredXP(playerid)));
        rand_xp *= 2;
    } else {
        rand_xp = Random(PlayerInfo[playerid][pLevel] * XP_RATE - 5, PlayerInfo[playerid][pLevel] * XP_RATE - 2);
        GivePlayerXP(playerid, rand_xp);
        SendClientMessageEx(playerid, COLOR_YELLOW,
            "Kinh nghiem nhan vat: +%d XP (%s/%s)",
            rand_xp,
            number_format(PlayerInfo[playerid][pXP]),
            number_format(GetRequiredXP(playerid)));
    }

    PlayerElectricJob[playerid][E_EJ_FixCount]++;

    // Discord log
    {
        new etitle[128], estr[256];
        format(etitle, sizeof(etitle), "%s (ID: %d) - Sửa điện | Vị trí (#%d)", GetPlayerNameEx(playerid), playerid, index + 1);
        format(estr, sizeof(estr), ":wrench: **SỬA ĐIỆN THÀNH CÔNG**\n:map: **Vị trí #%d/%d**\n:moneybag: **Tiền nhận: $%d**\n:star: **XP nhận: +%d%s**\n:clipboard: **Tiến độ: %d/%d điểm**",
            index + 1, ELECTRIC_MAX_POINTS, pay,
            rand_xp, (DoubleXP || PlayerInfo[playerid][pDoubleEXP] > 0) ? " [2X]" : "",
            PlayerElectricJob[playerid][E_EJ_FixCount], ELECTRIC_MAX_POINTS);
        DiscordLog(dclog_Electric, etitle, estr);
    }

    if(DestroyDynamicCP(gElectricCP[playerid][index]))
        gElectricCP[playerid][index] = STREAMER_TAG_CP:INVALID_STREAMER_ID;
    if(DestroyDynamicMapIcon(gElectricIcon[playerid][index]))
        gElectricIcon[playerid][index] = STREAMER_TAG_MAP_ICON:INVALID_STREAMER_ID;

    if(PlayerElectricJob[playerid][E_EJ_FixCount] >= ELECTRIC_MAX_POINTS)
    {
        for(new i = 0; i < ELECTRIC_MAX_POINTS; i++) {
            if(DestroyDynamicCP(gElectricCP[playerid][i]))
                gElectricCP[playerid][i] = STREAMER_TAG_CP:INVALID_STREAMER_ID;
            if(DestroyDynamicMapIcon(gElectricIcon[playerid][i]))
                gElectricIcon[playerid][i] = STREAMER_TAG_MAP_ICON:INVALID_STREAMER_ID;
        }
        gElectricFinish[playerid] = CreateDynamicRaceCP(1,
            -2532.7063, -612.3835, 132.5625,
            0.0, 0.0, 0.0, 4.0, 0, 0, playerid, 1000.0, -1, 0);
        SendClientMessageEx(playerid, COLOR_YELLOW,
            "Ban da sua xong tat ca su co dien! Hay lai xe ve diem xuat phat de hoan thanh!");
    }
    return 1;
}

// ============================================================
//  Xu ly khi player tra loi dung (goi tu OnDialogResponse)
// ============================================================
stock Electric_OnFixResponse(playerid, inputtext[])
{
    if(!IsElectricanWorking(playerid)) return 1;

    new index = GetPVarInt(playerid, "EJ_FixIndex");
    if(index < 0 || index >= ELECTRIC_MAX_POINTS) return 1;

    new ans1[32], ans2[32], ans3[32];
    if(sscanf(inputtext, "s[32]s[32]s[32]", ans1, ans2, ans3)) {
        Electric_ShowFixDialog(playerid, index);
        return 1;
    }

    if(!strcmp(ans1, sElectricAnswers[index][0], true) &&
       !strcmp(ans2, sElectricAnswers[index][1], true) &&
       !strcmp(ans3, sElectricAnswers[index][2], true))
    {
        // Dung - bat dau loading sua dien 10 giay
        ApplyAnimation(playerid, "BD_FIRE", "wash_up", 4.1, 0, 0, 0, 0, 5000, 1);
        PlayerPlaySound(playerid, 32000, 0.0, 0.0, 0.0);

        gElectricFixed[playerid][index] = true;
        RemovePlayerAttachedObject(playerid, ELECTRIC_KIT_SLOT);
        DeletePVar(playerid, "EJ_HasKit");

        {
            new string[128];
            format(string, sizeof(string), "{FF8000}* {C2A2DA}%s dang sua dien...", GetPlayerNameEx(playerid));
            ProxDetectorWrap(playerid, string, 92, 30.0, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
        }
        ShowTDN(playerid, NOTIFICATION_INFO, "Dap an dung! Dang sua dien...");
        SetLoading(playerid, 10000, "Dang sua dien...", "Electric_FixFinish");
    }
    else
    {
        // Sai - nga xuong va mat mau (freeze=1 giu nguoi choi nam xuong den khi dung day)
        ApplyAnimation(playerid, "PED", "BIKE_fall_off", 4.1, 0, 1, 1, 1, 0);
        PlayerPlaySound(playerid, 6402, 0.0, 0.0, 0.0);
        {
            new string[128];
            format(string, sizeof(string), "{FF8000}* {C2A2DA}%s bi dien giat!", GetPlayerNameEx(playerid));
            ProxDetectorWrap(playerid, string, 92, 30.0, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
        }

        new Float:damage = float(10 + random(11)); // 10-20 HP
        new Float:health;
        GetPlayerHealth(playerid, health);
        SetPlayerHealth(playerid, health - damage);

        new dmgMsg[64];
        format(dmgMsg, sizeof(dmgMsg), "Chap dien! Ban bi dien giat va mat {FF4545}%.0f {FFFFFF}HP!", damage);
        SendClientMessageEx(playerid, COLOR_WHITE, dmgMsg);

        // Discord log
        {
            new etitle[128], estr[256];
            format(etitle, sizeof(etitle), "%s (ID: %d) - Bi dien giat diem #%d", GetPlayerNameEx(playerid), playerid, index + 1);
            format(estr, sizeof(estr), ":zap: **DIEN GIAT**\n:map: **Diem #%d**\n:drop_of_blood: **HP mat: %.0f**\n:clipboard: **Da sua: %d/%d diem**\n:busts_in_silhouette: **Nhom: %s**",
                index + 1, damage,
                PlayerElectricJob[playerid][E_EJ_FixCount], ELECTRIC_MAX_POINTS,
                PlayerElectricJob[playerid][E_EJ_GroupName]);
            DiscordLog(dclog_Electric, etitle, estr);
        }

        SetTimerEx("ElectricStandUp", 2500, false, "i", playerid);
    }
    return 1;
}

// ============================================================
//  Timer: dung day sau khi nga xuong
// ============================================================
forward ElectricStandUp(playerid);
public ElectricStandUp(playerid)
{
    if(!IsPlayerConnected(playerid)) return 1;
    if(GetPVarInt(playerid, "Injured") == 1) return 1;
    new Float:hp;
    GetPlayerHealth(playerid, hp);
    if(hp <= 0.0) return 1;
    ApplyAnimation(playerid, "SUNBATHE", "Lay_Bac_out", 4.0, 0, 0, 0, 0, 0, 1);
    return 1;
}

forward WakeUpElectric(playerid);
public WakeUpElectric(playerid)
{
    ClearAnimations(playerid);
    return 1;
}

// ============================================================
//  Hook: khoi tao
// ============================================================
hook OnGameModeInit()
{
    // NPC + 3DText duoc tao boi Job_Init() trong job/main.inc
    gToggleElectric = 0;
    return 1;
}

// ============================================================
//  Hook: connect
// ============================================================
hook OnPlayerConnect(playerid)
{
    gElectricVehLabel[playerid] = Text3D:INVALID_3DTEXT_ID;
    Electric_ResetVars(playerid);
    return 1;
}

// ============================================================
//  Hook: disconnect
// ============================================================
hook OnPlayerDisconnect(playerid, reason)
{
    if(IsElectricanWorking(playerid))
    {
        if(GetPVarInt(playerid, "EJ_HasKit"))
        {
            RemovePlayerAttachedObject(playerid, ELECTRIC_KIT_SLOT);
            DeletePVar(playerid, "EJ_HasKit");
        }

        if(PlayerElectricJob[playerid][E_EJ_PartnerID] != INVALID_PLAYER_ID &&
           PlayerElectricJob[playerid][E_EJ_LeaderID] == playerid)
        {
            new partner = PlayerElectricJob[playerid][E_EJ_PartnerID];
            PlayerElectricJob[partner][E_EJ_VehicleID] = PlayerElectricJob[playerid][E_EJ_VehicleID];
            PlayerElectricJob[partner][E_EJ_Locked]    = PlayerElectricJob[playerid][E_EJ_Locked];
            PlayerElectricJob[partner][E_EJ_PartnerID] = INVALID_PLAYER_ID;
            PlayerElectricJob[playerid][E_EJ_VehicleID]= INVALID_VEHICLE_ID;
            PlayerElectricJob[playerid][E_EJ_Locked]   = false;
            SendClientMessageEx(partner, COLOR_YELLOW,
                "Leader cua ban da thoat. Xe cong tac da duoc chuyen cho ban!");
        }
        else if(PlayerElectricJob[playerid][E_EJ_PartnerID] == INVALID_PLAYER_ID &&
                PlayerElectricJob[playerid][E_EJ_LeaderID] == playerid)
        {
            if(IsValidVehicle(PlayerElectricJob[playerid][E_EJ_VehicleID]))
                DestroyVehicle(PlayerElectricJob[playerid][E_EJ_VehicleID]);
        }
    }
    Electric_ResetVars(playerid);
    return 1;
}

// ============================================================
//  Hook: xe bi respawn (xu ly xe cong tac bi thu hoi)
// ============================================================
hook OnVehicleSpawn(vehicleid)
{
    foreach(new i : Player) if(IsPlayerConnected(i))
    {
        if(PlayerElectricJob[i][E_EJ_VehicleID] == vehicleid)
        {
            if(gElectricVehLabel[i] != Text3D:INVALID_3DTEXT_ID) {
                Delete3DTextLabel(gElectricVehLabel[i]);
                gElectricVehLabel[i] = Text3D:INVALID_3DTEXT_ID;
            }
            DestroyVehicle(PlayerElectricJob[i][E_EJ_VehicleID]);
            PlayerElectricJob[i][E_EJ_VehicleID] = INVALID_VEHICLE_ID;
            PlayerElectricJob[i][E_EJ_Locked]    = false;
        }
    }
    return 1;
}

// ============================================================
//  Hook: vao checkpoint de sua dien
// ============================================================
hook OnPlayerEnterDynamicCP(playerid, STREAMER_TAG_CP:checkpointid)
{
    if(!IsElectricanWorking(playerid)) return 1;
    if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return 1;
    if(!GetPVarInt(playerid, "EJ_HasKit")) {
        SendClientMessageEx(playerid, COLOR_WHITE, "{FF6B6B}[!]{FFFFFF}Ban chua co dung cu sua dien! Dung {76FF71}/takekit {FFFFFF}de lay tu xe.");
        ShowTDN(playerid, NOTIFICATION_WARNING, "Chua co dung cu sua dien!");
        return 1;
    }
    if(GetPVarInt(playerid, "Injured") == 1) return 1;

    for(new i = 0; i < ELECTRIC_MAX_POINTS; i++) {
        if(!gElectricFixed[playerid][i] && checkpointid == gElectricCP[playerid][i]) {
            {
                new string[128];
                format(string, sizeof(string), "{FF8000}* {C2A2DA}%s dang kiem tra su co dien...", GetPlayerNameEx(playerid));
                ProxDetectorWrap(playerid, string, 92, 30.0, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
            }
            Electric_ShowFixDialog(playerid, i);
            break;
        }
    }
    return 1;
}

// ============================================================
//  Hook: vao race checkpoint de hoan thanh cong viec
// ============================================================
hook OnPlayerEnterDynRaceCP(playerid, STREAMER_TAG_RACE_CP:checkpointid)
{
    if(!IsElectricanWorking(playerid)) return 1;

    if(checkpointid == gElectricFinish[playerid])
    {
        if(PlayerElectricJob[playerid][E_EJ_LeaderID] == playerid)
        {
            if(IsValidVehicle(PlayerElectricJob[playerid][E_EJ_VehicleID]))
                DestroyVehicle(PlayerElectricJob[playerid][E_EJ_VehicleID]);
            PlayerElectricJob[playerid][E_EJ_VehicleID] = INVALID_VEHICLE_ID;
            PlayerElectricJob[playerid][E_EJ_Locked]    = false;
        }
        if(GetPVarInt(playerid, "EJ_HasKit"))
        {
            RemovePlayerAttachedObject(playerid, ELECTRIC_KIT_SLOT);
            DeletePVar(playerid, "EJ_HasKit");
        }
        SetPlayerSkin(playerid, PlayerInfo[playerid][pModel]);

        // Luu truoc khi reset
        new savedGroupName[64], savedFixCount;
        savedFixCount = PlayerElectricJob[playerid][E_EJ_FixCount];
        format(savedGroupName, sizeof(savedGroupName), "%s", PlayerElectricJob[playerid][E_EJ_GroupName]);

        Electric_ResetVars(playerid);
        ShowTDN(playerid, NOTIFICATION_SUCCESS, "Cong viec Sua Dien hoan thanh!");

        // Discord log
        {
            new etitle[128], estr[256];
            format(etitle, sizeof(etitle), "%s (ID: %d) - Hoan thanh Sua Dien", GetPlayerNameEx(playerid), playerid);
            format(estr, sizeof(estr), ":trophy: **HOAN THANH CONG VIEC SUA DIEN**\n**Nhom:** %s\n**Da sua:** %d/%d diem\n:white_check_mark: **Cong viec ket thuc, ve diem xuat phat!**",
                savedGroupName, savedFixCount, ELECTRIC_MAX_POINTS);
            DiscordLog(dclog_Electric, etitle, estr);
        }
    }
    return 1;
}

// ============================================================
//  Lenh: /takekit - lay bo cong cu tu xe
// ============================================================
CMD:takekit(playerid, params[])
{
    if(!IsElectricanWorking(playerid))
        return ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong dang lam cong viec Sua Dien!");

    if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT)
        return ShowTDN(playerid, NOTIFICATION_WARNING, "Ban phai xuong xe truoc khi lay bo cong cu!");

    new Float:vx, Float:vy, Float:vz;
    GetVehiclePos(PlayerElectricJob[playerid][E_EJ_VehicleID], vx, vy, vz);
    if(!IsPlayerInRangeOfPoint(playerid, 4.0, vx, vy, vz))
        return ShowTDN(playerid, NOTIFICATION_WARNING, "Ban phai dung canh xe cong tac!");

    if(GetPVarInt(playerid, "EJ_HasKit"))
        return ShowTDN(playerid, NOTIFICATION_WARNING, "Ban da co bo cong cu roi!");

    SetPlayerAttachedObject(playerid, ELECTRIC_KIT_SLOT, 19921, 6,
        0.0, 0.0, 0.0, 87.2, -0.1, -86.9, 1.0, 1.0, 1.0);
    SetPVarInt(playerid, "EJ_HasKit", 1);
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Da lay bo cong cu sua dien!");
    return 1;
}

// ============================================================
//  Lenh: /egroup (accept | decline) - tra loi loi moi nhom
// ============================================================
CMD:egroup(playerid, params[])
{
    new type[16];
    if(sscanf(params, "s[16]", type))
        return SendHelpMessage(playerid, "/egroup (accept | decline)");

    if(!strcmp(type, "accept", true))
    {
        new groupid = GetPVarInt(playerid, "EJ_InviteGroupID");
        if(!groupid) return ShowTDN(playerid, NOTIFICATION_ERROR, "Ban khong co loi moi nao!");

        if(PlayerElectricJob[playerid][E_EJ_PartnerID] != INVALID_PLAYER_ID)
            return ShowTDN(playerid, NOTIFICATION_ERROR, "Ban da co trong nhom roi!");

        foreach(new i : Player) if(IsPlayerConnected(i))
        {
            if(PlayerElectricJob[i][E_EJ_GroupID] == groupid)
            {
                SendClientMessageEx(playerid, COLOR_WHITE,
                    "Ban da tham gia nhom {FFFF00}%s{FFFFFF} cua %s.",
                    PlayerElectricJob[i][E_EJ_GroupName], GetPlayerNameEx(i));
                SendClientMessageEx(i, COLOR_WHITE,
                    "{FFFF00}%s{FFFFFF} da chap nhan loi moi vao nhom.", GetPlayerNameEx(playerid));
                PlayerElectricJob[i][E_EJ_PartnerID]      = playerid;
                PlayerElectricJob[playerid][E_EJ_PartnerID]= i;
                format(PlayerElectricJob[playerid][E_EJ_GroupName], 64,
                    "%s", PlayerElectricJob[i][E_EJ_GroupName]);
                DeletePVar(playerid, "EJ_InviteGroupID");
                return 1;
            }
        }
        return ShowTDN(playerid, NOTIFICATION_ERROR, "Nhom khong con ton tai!");
    }
    else if(!strcmp(type, "decline", true))
    {
        DeletePVar(playerid, "EJ_InviteGroupID");
        ShowTDN(playerid, NOTIFICATION_INFO, "Ban da tu choi loi moi.");
    }
    else SendHelpMessage(playerid, "/egroup (accept | decline)");
    return 1;
}

// ============================================================
//  Dialog: DIALOG_ELECTRIC_MENU - menu chinh
// ============================================================
stock Electric_OnMenuResponse(playerid, response, listitem)
{
    if(!response) {
        SendClientMessageEx(playerid, COLOR_GREY, "Ban da huy chon.");
        return 1;
    }
    if(GetPVarInt(playerid, "Injured") == 1)
        return ShowTDN(playerid, NOTIFICATION_ERROR, "Ban dang bi thuong, khong the thao tac!");

    switch(listitem)
    {
        case 0: // Tao nhom
        {
            ShowPlayerDialog(playerid, DIALOG_ELECTRIC_GNAME, DIALOG_STYLE_INPUT,
                "{F5D000}GTA-HTB Roleplay {FFFFFF}- Tao Nhom",
                "Nhap ten nhom cua ban (toi thieu 7 ky tu, khong phai toan so):", "Tao", "Huy");
        }
        case 1: // Moi nguoi
        {
            if(PlayerElectricJob[playerid][E_EJ_GroupName][0] == EOS)
                return ShowTDN(playerid, NOTIFICATION_ERROR, "Ban phai tao nhom truoc!");
            ShowPlayerDialog(playerid, DIALOG_ELECTRIC_INVITE, DIALOG_STYLE_INPUT,
                "{F5D000}GTA-HTB Roleplay {FFFFFF}- Moi Vao Nhom",
                "Nhap player ID muon moi (phai dung canh ban):", "Moi", "Huy");
        }
        case 2: // Duoi nguoi
        {
            if(PlayerElectricJob[playerid][E_EJ_GroupName][0] == EOS)
                return ShowTDN(playerid, NOTIFICATION_ERROR, "Ban khong co nhom!");
            ShowPlayerDialog(playerid, DIALOG_ELECTRIC_KICK, DIALOG_STYLE_INPUT,
                "{F5D000}GTA-HTB Roleplay {FFFFFF}- Duoi Khoi Nhom",
                "Nhap player ID muon duoi khoi nhom:", "Duoi", "Huy");
        }
        case 3: // Tham gia nhom
        {
            ShowPlayerDialog(playerid, DIALOG_ELECTRIC_JOIN, DIALOG_STYLE_INPUT,
                "{F5D000}GTA-HTB Roleplay {FFFFFF}- Tham Gia Nhom",
                "Nhap Group ID muon tham gia:", "Tham gia", "Huy");
        }
        case 4: // Bat dau lam viec
        {
            if(gToggleElectric == 1)
                return ShowTDN(playerid, NOTIFICATION_ERROR, "Cong viec Sua Dien hien dang bi tam ngung!");
            if(PlayerInfo[playerid][pJob] != JOB_ELECTRIC && PlayerInfo[playerid][pJob2] != JOB_ELECTRIC)
                return ShowTDN(playerid, NOTIFICATION_ERROR, "Ban khong co cong viec Tho Sua Dien!");
            if(PlayerElectricJob[playerid][E_EJ_GroupName][0] == EOS)
                return ShowTDN(playerid, NOTIFICATION_ERROR, "Ban chua tao nhom!");
            if(IsElectricanWorking(playerid))
                return ShowTDN(playerid, NOTIFICATION_ERROR, "Ban da bat dau lam viec roi!");

            new partner = PlayerElectricJob[playerid][E_EJ_PartnerID];
            new vehid = CreateVehicle(552, -2532.0229, -601.6449, 132.2643, 177.5118, 1, 1, 60000);
            SetVehicleHealth(vehid, 2000.0);

            new lblText[128];
            format(lblText, sizeof(lblText), "{F5D000}Tho Sua Dien\n{FFFFFF}Nhom: {F5D000}%s", PlayerElectricJob[playerid][E_EJ_GroupName]);
            gElectricVehLabel[playerid] = Create3DTextLabel(lblText, 0xFFFFFFFF, 0.0, 0.0, 0.5, 15.0, 0);
            Attach3DTextLabelToVehicle(gElectricVehLabel[playerid], vehid, 0.0, 0.0, 0.5);

            PlayerElectricJob[playerid][E_EJ_VehicleID] = vehid;
            PlayerElectricJob[playerid][E_EJ_Locked]    = false;

            SetElectricanWorking(playerid, true);
            PlayerWorking[playerid] = JOB_ELECTRIC;
            Electric_SetSkin(playerid);
            PutPlayerInVehicle(playerid, vehid, 0);
            {
                new string[128];
                format(string, sizeof(string), "{FF8000}* {C2A2DA}%s bat dau lam viec Sua Dien!", GetPlayerNameEx(playerid));
                ProxDetectorWrap(playerid, string, 92, 30.0, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
            }

            if(partner != INVALID_PLAYER_ID)
            {
                PlayerElectricJob[partner][E_EJ_VehicleID] = vehid;
                SetElectricanWorking(partner, true);
                PlayerWorking[partner] = JOB_ELECTRIC;
                Electric_SetSkin(partner);
                PutPlayerInVehicle(partner, vehid, 1);
                {
                    new string[128];
                    format(string, sizeof(string), "{FF8000}* {C2A2DA}%s bat dau lam viec Sua Dien!", GetPlayerNameEx(partner));
                    ProxDetectorWrap(partner, string, 92, 30.0, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
                }

                for(new i = 0; i < ELECTRIC_MAX_POINTS; i++) {
                    gElectricCP[partner][i]   = CreateDynamicCP(sElectricPoints[i][0], sElectricPoints[i][1], sElectricPoints[i][2], 2.0, 0, 0, partner, 25.0);
                    gElectricIcon[partner][i] = CreateDynamicMapIcon(sElectricPoints[i][0], sElectricPoints[i][1], sElectricPoints[i][2], 0, 0xFF0000FF, 0, 0, partner, 5000.0);
                }
            }

            PlayerElectricJob[playerid][E_EJ_GroupID] = -1;
            for(new i = 0; i < ELECTRIC_MAX_POINTS; i++) {
                gElectricCP[playerid][i]   = CreateDynamicCP(sElectricPoints[i][0], sElectricPoints[i][1], sElectricPoints[i][2], 2.0, 0, 0, playerid, 25.0);
                gElectricIcon[playerid][i] = CreateDynamicMapIcon(sElectricPoints[i][0], sElectricPoints[i][1], sElectricPoints[i][2], 0, 0xFF0000FF, 0, 0, playerid, 5000.0);
            }

            SendClientMessageEx(playerid, COLOR_YELLOW,
                "Cong viec Sua Dien bat dau! Lay bo cong cu bang /takekit roi den cac diem do tren ban do.");

            // Discord log
            {
                new etitle[128], estr[256];
                format(etitle, sizeof(etitle), "%s (ID: %d) - Bat dau Sua Dien", GetPlayerNameEx(playerid), playerid);
                if(partner != INVALID_PLAYER_ID) {
                    format(estr, sizeof(estr), ":zap: **BAT DAU CONG VIEC SUA DIEN**\n**Nhom:** %s\n**Leader:** %s\n**Partner:** %s (ID: %d)\n**Diem can sua:** %d",
                        PlayerElectricJob[playerid][E_EJ_GroupName],
                        GetPlayerNameEx(playerid), GetPlayerNameEx(partner), partner,
                        ELECTRIC_MAX_POINTS);
                } else {
                    format(estr, sizeof(estr), ":zap: **BAT DAU CONG VIEC SUA DIEN**\n**Nhom:** %s\n**Leader:** %s\n**Partner:** (khong co)\n**Diem can sua:** %d",
                        PlayerElectricJob[playerid][E_EJ_GroupName],
                        GetPlayerNameEx(playerid), ELECTRIC_MAX_POINTS);
                }
                DiscordLog(dclog_Electric, etitle, estr);
            }
        }
        case 5: // Ngung lam viec
        {
            Electric_Stop(playerid);
        }
        case 6: // Xem bang mau day dien
        {
            Electric_ShowColorsDialog(playerid);
        }
    }
    return 1;
}

// ============================================================
//  Dialog: DIALOG_ELECTRIC_GNAME - tao ten nhom
// ============================================================
stock Electric_OnGroupNameResponse(playerid, response, inputtext[])
{
    if(!response) return 1;
    if(GetPVarInt(playerid, "Injured") == 1)
        return ShowTDN(playerid, NOTIFICATION_ERROR, "Ban dang bi thuong!");

    if(isnull(inputtext))
        return ShowPlayerDialog(playerid, DIALOG_ELECTRIC_GNAME, DIALOG_STYLE_INPUT,
            "{F5D000}GTA-HTB Roleplay {FFFFFF}- Tao Nhom",
            "{FF4444}Khong duoc de trong!\n\nNhap ten nhom (toi thieu 64 ky tu, khong phai toan so):", "Tao", "Huy");

    if(Electric_IsNumeric(inputtext))
        return ShowPlayerDialog(playerid, DIALOG_ELECTRIC_GNAME, DIALOG_STYLE_INPUT,
            "{F5D000}GTA-HTB Roleplay {FFFFFF}- Tao Nhom",
            "{FF4444}Khong duoc nhap toan so!\n\nNhap ten nhom (toi thieu 64 ky tu):", "Tao", "Huy");

    if(strlen(inputtext) < 7)
        return ShowPlayerDialog(playerid, DIALOG_ELECTRIC_GNAME, DIALOG_STYLE_INPUT,
            "{F5D000}GTA-HTB Roleplay {FFFFFF}- Tao Nhom",
            "{FF4444}Ten nhom phai tren 7 ky tu!\n\nNhap ten nhom:", "Tao", "Huy");

    PlayerElectricJob[playerid][E_EJ_LeaderID] = playerid;
    PlayerElectricJob[playerid][E_EJ_GroupID]  = random(99) + 1;
    format(PlayerElectricJob[playerid][E_EJ_GroupName], 64, "%s", inputtext);

    SendClientMessageEx(playerid, COLOR_WHITE,
        "Da tao nhom {FFFF00}%s{FFFFFF} (Group ID: {FFFF00}%d{FFFFFF}). Chia se ID nay cho nguoi khac!",
        inputtext, PlayerElectricJob[playerid][E_EJ_GroupID]);
    return 1;
}

// ============================================================
//  Dialog: DIALOG_ELECTRIC_INVITE - moi nguoi vao nhom
// ============================================================
stock Electric_OnInviteResponse(playerid, response, inputtext[])
{
    if(!response) return 1;
    if(PlayerElectricJob[playerid][E_EJ_GroupName][0] == EOS)
        return ShowTDN(playerid, NOTIFICATION_ERROR, "Ban chua co nhom!");

    if(isnull(inputtext) || !Electric_IsNumeric(inputtext))
        return ShowPlayerDialog(playerid, DIALOG_ELECTRIC_INVITE, DIALOG_STYLE_INPUT,
            "{F5D000}GTA-HTB Roleplay {FFFFFF}- Moi Vao Nhom",
            "{FF4444}Chi nhap so ID!\n\nNhap player ID muon moi:", "Moi", "Huy");

    new otherid = strval(inputtext);
    if(otherid == playerid)
        return ShowPlayerDialog(playerid, DIALOG_ELECTRIC_INVITE, DIALOG_STYLE_INPUT,
            "{F5D000}GTA-HTB Roleplay {FFFFFF}- Moi Vao Nhom",
            "{FF4444}Khong the moi chinh minh!\n\nNhap player ID muon moi:", "Moi", "Huy");

    if(otherid == INVALID_PLAYER_ID || !IsPlayerConnected(otherid))
        return ShowPlayerDialog(playerid, DIALOG_ELECTRIC_INVITE, DIALOG_STYLE_INPUT,
            "{F5D000}GTA-HTB Roleplay {FFFFFF}- Moi Vao Nhom",
            "{FF4444}Nguoi choi khong co mat!\n\nNhap player ID muon moi:", "Moi", "Huy");

    new Float:ox, Float:oy, Float:oz;
    GetPlayerPos(otherid, ox, oy, oz);
    if(!IsPlayerInRangeOfPoint(playerid, 5.0, ox, oy, oz))
        return ShowPlayerDialog(playerid, DIALOG_ELECTRIC_INVITE, DIALOG_STYLE_INPUT,
            "{F5D000}GTA-HTB Roleplay {FFFFFF}- Moi Vao Nhom",
            "{FF4444}Nguoi choi phai dung canh ban!\n\nNhap player ID muon moi:", "Moi", "Huy");

    SendClientMessageEx(playerid, COLOR_WHITE,
        "Ban da moi {FFFF00}%s{FFFFFF}. Cho nguoi do tra loi.", GetPlayerNameEx(otherid));
    SendClientMessageEx(otherid, COLOR_YELLOW,
        "{FFFF00}%s{FFFFFF} moi ban vao nhom Sua Dien. Dung {00FF00}/egroup accept{FFFFFF} de chap nhan!",
        GetPlayerNameEx(playerid));
    SetPVarInt(otherid, "EJ_InviteGroupID", PlayerElectricJob[playerid][E_EJ_GroupID]);
    return 1;
}

// ============================================================
//  Dialog: DIALOG_ELECTRIC_KICK - duoi nguoi khoi nhom
// ============================================================
stock Electric_OnKickResponse(playerid, response, inputtext[])
{
    if(!response) return 1;
    if(PlayerElectricJob[playerid][E_EJ_GroupName][0] == EOS)
        return ShowTDN(playerid, NOTIFICATION_ERROR, "Ban khong co nhom!");

    if(isnull(inputtext) || !Electric_IsNumeric(inputtext))
        return ShowPlayerDialog(playerid, DIALOG_ELECTRIC_KICK, DIALOG_STYLE_INPUT,
            "{F5D000}GTA-HTB Roleplay {FFFFFF}- Duoi Khoi Nhom",
            "{FF4444}Chi nhap so ID!\n\nNhap player ID muon duoi:", "Duoi", "Huy");

    new otherid = strval(inputtext);
    if(PlayerElectricJob[otherid][E_EJ_PartnerID] != playerid)
        return ShowPlayerDialog(playerid, DIALOG_ELECTRIC_KICK, DIALOG_STYLE_INPUT,
            "{F5D000}GTA-HTB Roleplay {FFFFFF}- Duoi Khoi Nhom",
            "{FF4444}Nguoi nay khong trong nhom ban!\n\nNhap player ID muon duoi:", "Duoi", "Huy");

    PlayerElectricJob[playerid][E_EJ_PartnerID] = INVALID_PLAYER_ID;
    PlayerElectricJob[otherid][E_EJ_PartnerID]  = INVALID_PLAYER_ID;
    SendClientMessageEx(playerid, COLOR_WHITE,
        "Da duoi {FFFF00}%s{FFFFFF} khoi nhom.", GetPlayerNameEx(otherid));
    SendClientMessageEx(otherid, COLOR_YELLOW,
        "Ban bi {FFFF00}%s{FFFFFF} duoi khoi nhom Sua Dien.", GetPlayerNameEx(playerid));
    return 1;
}

// ============================================================
//  Dialog: DIALOG_ELECTRIC_JOIN - tham gia nhom theo Group ID
// ============================================================
stock Electric_OnJoinResponse(playerid, response, inputtext[])
{
    if(!response) return 1;
    if(GetPVarInt(playerid, "Injured") == 1)
        return ShowTDN(playerid, NOTIFICATION_ERROR, "Ban dang bi thuong!");
    if(PlayerElectricJob[playerid][E_EJ_PartnerID] != INVALID_PLAYER_ID)
        return ShowTDN(playerid, NOTIFICATION_ERROR, "Ban da trong mot nhom roi!");

    if(isnull(inputtext) || !Electric_IsNumeric(inputtext))
        return ShowPlayerDialog(playerid, DIALOG_ELECTRIC_JOIN, DIALOG_STYLE_INPUT,
            "{F5D000}GTA-HTB Roleplay {FFFFFF}- Tham Gia Nhom",
            "{FF4444}Chi nhap so Group ID!\n\nNhap Group ID:", "Tham gia", "Huy");

    new bool:found = false;
    foreach(new i : Player) if(IsPlayerConnected(i))
    {
        if(PlayerElectricJob[i][E_EJ_GroupID] == strval(inputtext))
        {
            if(PlayerElectricJob[i][E_EJ_PartnerID] != INVALID_PLAYER_ID)
                return ShowTDN(playerid, NOTIFICATION_ERROR, "Nhom nay da du thanh vien!");

            PlayerElectricJob[i][E_EJ_PartnerID]       = playerid;
            PlayerElectricJob[playerid][E_EJ_PartnerID] = i;
            format(PlayerElectricJob[playerid][E_EJ_GroupName], 64, "%s", PlayerElectricJob[i][E_EJ_GroupName]);
            PlayerElectricJob[i][E_EJ_GroupID] = -1;
            found = true;

            SendClientMessageEx(playerid, COLOR_WHITE,
                "Da tham gia nhom {FFFF00}%s{FFFFFF} cua %s.",
                PlayerElectricJob[i][E_EJ_GroupName], GetPlayerNameEx(i));
            SendClientMessageEx(i, COLOR_WHITE,
                "{FFFF00}%s{FFFFFF} da tham gia nhom cua ban!", GetPlayerNameEx(playerid));
            break;
        }
    }

    if(!found) ShowTDN(playerid, NOTIFICATION_ERROR, "Khong tim thay Group ID nay!");
    return 1;
}

// ============================================================
//  Hien thi bang mau day dien
// ============================================================
stock Electric_ShowColorsDialog(playerid)
{
    ShowPlayerDialog(playerid, DIALOG_ELECTRIC_COLORS, DIALOG_STYLE_MSGBOX,
        "{F5D000}GTA-HTB Roleplay {FFFFFF}- Bang Mau Day Dien",
        "{FFFFFF}Cac mau co the xuat hien trong mini-game:\n\n{FF0000}DO          {00FF00}XANH        {0055FF}LAM\n\n{FFFFFF}TRANG       {FFFF00}VANG        {AA00FF}TIM\n\n{555555}DEN         {C0C0C0}XAM         {FF8000}CAM\n\n{AAAAAA}Luu y: Nhin MAU CHU, khong phai nghia tu trong []",
        "Dong", "");
    return 1;
}

// ============================================================
//  Lenh: /xemday - xem bang mau day dien
// ============================================================
CMD:xemday(playerid, params[])
{
    Electric_ShowColorsDialog(playerid);
    return 1;
}
