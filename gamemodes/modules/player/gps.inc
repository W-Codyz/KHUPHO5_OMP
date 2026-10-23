#include <YSI\YSI_Coding\y_hooks>

// ============================================================
// GPS System - KP5 Roleplay Created by Codyz
// ============================================================

#define MAX_GPS_POINTS       5
#define MAX_GPS_TIMDUONG     30
#define GPS_JOB_COUNT        11

#define TIMVIECLAM           (5210)
#define TIMDUONG             (5211)
#define MAP_MAIN             (5212)

#if !defined DIALOG_MAP_TPHAT
    #define DIALOG_MAP_TPHAT MAP_MAIN
#endif

#define DIALOG_GPS_TD_MANAGE (5390)
#define DIALOG_GPS_TD_ADD    (5391)

// ============================================================
// TIMDUONG - File-backed, admin-editable
// ============================================================
new gTimDuongName[MAX_GPS_TIMDUONG][48];
new Float:gTimDuongX[MAX_GPS_TIMDUONG];
new Float:gTimDuongY[MAX_GPS_TIMDUONG];
new Float:gTimDuongZ[MAX_GPS_TIMDUONG];
new gGPSTimDuongCount = 0;

// ============================================================
// TIMVIECLAM - Static arrays, edit in code
// ============================================================
new gJobName[GPS_JOB_COUNT][48] = {
    "Pizza Boy Delivery",
    "Trucker Delivery",
    "Boc Vac",
    "Xay Dung",
    "Tho Son Tuong",
    "Cau Ca",
    "Lumberjack",
    "Tho Sua Dien",
    "Tho Khai Thac",
    "Lai Xe Container",
    "Can Sa"
};									//PIZZA 	 TRUCKER		 BOC VAC	 XAY DUNG	   THO SON TUONG CAU CA       LUMBERJACK  THO SUA DIEN  THO KHAI THAC CONTAINER      CAN SA
new Float:gJobX[GPS_JOB_COUNT] = {-1720.962646, -1560.963867, -1710.878540,  -2071.5452,  -2456.8425,  -2975.5642,  -758.966003, -2520.9807,  584.901367, -1655.575195, -1001.113586};
new Float:gJobY[GPS_JOB_COUNT] = { 1364.456176,   127.491157,    22.986656,    227.5330,    786.6091,    480.8670,   -130.437088,  -624.4470,  867.991882,   57.622314, -1664.279541};
new Float:gJobZ[GPS_JOB_COUNT] = {    7.187500,     3.554687,    3.749292,     36.1192,     35.1719,      4.8925,    65.813842,   132.7828,   -42.497318,    3.554687, 76.367187};

// ============================================================
// STOCK FUNCTIONS
// ============================================================
stock SetPlayerRaceCP(playerid, CP_TYPE:type, Float:centreX, Float:centreY, Float:centreZ, Float:nextX, Float:nextY, Float:nextZ, Float:size)
{
    SetPlayerWaze(playerid, centreX, centreY, centreZ, 0xD24949FF);
    return SetPlayerRaceCheckpoint(playerid, type, centreX, centreY, centreZ, nextX, nextY, nextZ, size);
}

stock DisablePlayerRaceCP(playerid)
{
    if(IsValidWazeGPS(playerid)) StopWazeGPS(playerid);
    return DisablePlayerRaceCheckpoint(playerid);
}

stock SetPlayerCP(playerid, Float:centreX, Float:centreY, Float:centreZ, Float:size, bool:wavegps=true)
{
    if(IsValidPlayerPickup(playerid, gPlayerCPArrow[playerid])) DestroyPlayerPickup(playerid, gPlayerCPArrow[playerid]);
    if(wavegps) SetPlayerWaze(playerid, centreX, centreY, centreZ, 0xD24949FF);
    gPlayerCPArrow[playerid] = CreatePlayerPickup(playerid, 19133, 23, centreX, centreY, centreZ+size, GetPlayerVirtualWorld(playerid));
    return SetPlayerCheckpoint(playerid, centreX, centreY, centreZ, size);
}

stock DisablePlayerCP(playerid)
{
    if(IsValidPlayerPickup(playerid, gPlayerCPArrow[playerid])) DestroyPlayerPickup(playerid, gPlayerCPArrow[playerid]);
    if(IsValidWazeGPS(playerid)) StopWazeGPS(playerid);
    gPlayerCPArrow[playerid] = -1;
    return DisablePlayerCheckpoint(playerid);
}

// ============================================================
// SAVE / LOAD
// ============================================================
stock SaveGPSTimDuong()
{
    new line[192];
    new File:f = fopen("GPSTimDuong.cfg", io_write);
    for(new i = 0; i < gGPSTimDuongCount; i++)
    {
        format(line, sizeof(line), "%s|%f|%f|%f\r\n",
            gTimDuongName[i], gTimDuongX[i], gTimDuongY[i], gTimDuongZ[i]);
        fwrite(f, line);
    }
    return fclose(f);
}

/* static _GPS_LoadDefaults()
{
    new i = 0;
    #define _A(%0,%1,%2,%3) strcopy(gTimDuongName[i],%0,48);gTimDuongX[i]=%1;gTimDuongY[i]=%2;gTimDuongZ[i]=%3;i++

    _A("Ngan hang San Fierro",     -1581.2155,   902.8041,   7.6953);
    _A("Ngan hang Los Santos",      587.0106,  -1238.3374,  17.8049);
    _A("Benh vien San Fierro",    -2655.8972,   634.8754,   14.4531);
    _A("Benh vien Los Santos",     1172.0809, -1323.4167,   15.4031);
    _A("Thi bang lai xe",         -2765.2007,   375.8469,    6.3435);
    _A("Cua xe the thao",         -1647.9294,  1205.3108,    7.2500);
    _A("Cua hang xe moto",        -1969.7197,   293.8402,   35.1719);
    _A("Cua hang sieu xe VIP",    -2411.9282,     3.4309,   35.3203);
    _A("Cua hang quan ao",        -1695.2990,   950.6997,   24.8906);
    _A("Khu vuc cau ca",          -2975.9360,   506.9248,    2.4297);
    _A("Khu vuc ban ca",          -2927.2327,   507.2165,    4.9141);
    _A("Nha hang an uong",        -1722.6263,  1359.8776,    7.1875);
    _A("Khu VIP",                 -2435.4814,   500.9243,   29.9394);
    _A("Cua hang tien loi 24/7",  -2442.5369,   753.4180,   35.1719);
    _A("Cua hang sua xe",         -1904.2194,   274.2740,   41.0469);
    _A("Dau truong PvP",          -1492.1313,   920.1057,    7.1875);
    _A("Khu giai tri casino",     -2079.0525,  1413.5898,    7.2210);
    _A("Phong tap vo",            -2269.259277, -155.654510, 35.320312);
    _A("Black Market",             1523.093505,-1474.203125,  9.500000);

    #undef _A
    gGPSTimDuongCount = i;
} */

stock LoadGPSTimDuong()
{
    gGPSTimDuongCount = 0;
    if(!fexist("GPSTimDuong.cfg"))
    {
        //_GPS_LoadDefaults();
        SaveGPSTimDuong();
        return 1;
    }
    new line[192];
    new File:f = fopen("GPSTimDuong.cfg", io_read);
    while(gGPSTimDuongCount < MAX_GPS_TIMDUONG && fread(f, line))
    {
        if(!sscanf(line, "p<|>s[48]fff",
            gTimDuongName[gGPSTimDuongCount],
            gTimDuongX[gGPSTimDuongCount],
            gTimDuongY[gGPSTimDuongCount],
            gTimDuongZ[gGPSTimDuongCount]))
            gGPSTimDuongCount++;
    }
    fclose(f);
    return 1;
}

// ============================================================
// DIALOG HELPERS
// ============================================================
stock ShowGPSMainDialog(playerid)
{
    return ShowPlayerDialog(playerid, DIALOG_MAP_TPHAT, DIALOG_STYLE_LIST,
        "{ffff00}GPS{ffffff} - Ban muon di dau?",
        "Tim duong\nTim viec lam\nVi tri can sa\nVi tri khoang san\nVi tri Lumerjack",
        "Chon", "Huy");
}

stock ShowGPSTimDuong(playerid)
{
    new string[4096], line[128];
    new Float:px, Float:py, Float:pz;
    GetPlayerPos(playerid, px, py, pz);

    // Header row
    strcat(string, "{FFD700}STT\t{FFFFFF}Ten\t{87CEEB}Khu Vuc\t{00FF88}Khoang cach\n", sizeof(string));

    for(new i = 0; i < gGPSTimDuongCount; i++)
    {
        new zone[32];
        new dist = floatround(GetPlayerDistanceFromPoint(playerid, gTimDuongX[i], gTimDuongY[i], gTimDuongZ[i]));
        Get3DZone(gTimDuongX[i], gTimDuongY[i], gTimDuongZ[i], zone, sizeof(zone));

        format(line, sizeof(line), "{FFD700}%d\t{FFFFFF}%s\t{87CEEB}%s\t{00FF88}%dm\n",
            i + 1, gTimDuongName[i], zone, dist);
        strcat(string, line, sizeof(string));
    }

    if(!gGPSTimDuongCount)
        strcat(string, "{AAAAAA}Chua co dia diem nao.", sizeof(string));

    ShowPlayerDialog(playerid, TIMDUONG, DIALOG_STYLE_TABLIST_HEADERS,
        "{ffff00}GPS{ffffff} - Tim Duong", string, "Chon", "Dong");
}

stock ShowGPSTimViecLam(playerid)
{
    new string[1024], line[56];
    for(new i = 0; i < GPS_JOB_COUNT; i++)
    {
        format(line, sizeof(line), "%s\n", gJobName[i]);
        strcat(string, line, sizeof(string));
    }
    ShowPlayerDialog(playerid, TIMVIECLAM, DIALOG_STYLE_LIST, "Tim Viec Lam", string, "Chon", "Dong");
}

stock ShowGPSManage(playerid)
{
    new string[3072], line[112];
    strcat(string, "{FFD700}ID\t{FFFFFF}Ten\t{87CEEB}Toa do\n", sizeof(string));
    for(new i = 0; i < gGPSTimDuongCount; i++)
    {
        format(line, sizeof(line), "{FFD700}%d\t{FFFFFF}%s\t{87CEEB}%.1f, %.1f, %.1f\n",
            i, gTimDuongName[i], gTimDuongX[i], gTimDuongY[i], gTimDuongZ[i]);
        strcat(string, line, sizeof(string));
    }
    if(!gGPSTimDuongCount)
        strcat(string, "Chua co dia diem nao.", sizeof(string));

    ShowPlayerDialog(playerid, DIALOG_GPS_TD_MANAGE, DIALOG_STYLE_TABLIST_HEADERS,
        "{FFFF00}QUAN LY GPS - TIM DUONG", string, "Xoa", "Dong");
}

// ============================================================
// HOOKS
// ============================================================
hook OnGameModeInit()
{
    LoadGPSTimDuong();
    return 1;
}

// ============================================================
// COMMANDS
// ============================================================
alias:map("gps", "timduong")
CMD:map(playerid, params[])
{
    return ShowGPSMainDialog(playerid);
}

CMD:timvieclam(playerid, params[])
{
    return ShowGPSTimViecLam(playerid);
}

// /editgps delete | add | setpos [id] | reload
CMD:editgps(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1337)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong co quyen su dung lenh nay!");

    new action[16];
    new id = -1;
    if(sscanf(params, "s[16]D(-1)", action, id))
    {
        SendClientMessageEx(playerid, COLOR_WHITE, "SU DUNG: /editgps [tuy chon] [...]");
        SendClientMessageEx(playerid, COLOR_GREY, "Tuy chon: {FFFF00}delete, add, setpos [id], reload");
        return 1;
    }

    if(!strcmp(action, "delete", true))
    {
        ShowGPSManage(playerid);
    }
    else if(!strcmp(action, "add", true))
    {
        if(gGPSTimDuongCount >= MAX_GPS_TIMDUONG)
            return SendClientMessageEx(playerid, COLOR_GREY, "Da dat so luong dia diem toi da!");

        ShowPlayerDialog(playerid, DIALOG_GPS_TD_ADD, DIALOG_STYLE_INPUT,
            "{FFFF00}THEM DIA DIEM GPS",
            "{FFFFFF}Nhap ten dia diem moi:\n{AAAAAA}(Vi tri se duoc lay tu vi tri hien tai cua ban)",
            "Them", "Huy");
    }
    else if(!strcmp(action, "setpos", true))
    {
        if(id < 0)
            return SendClientMessageEx(playerid, COLOR_WHITE, "SU DUNG: /editgps setpos [id]");
        if(id >= gGPSTimDuongCount)
            return SendClientMessageEx(playerid, COLOR_GREY, "ID khong hop le! (0 - %d)", gGPSTimDuongCount - 1);

        GetPlayerPos(playerid, gTimDuongX[id], gTimDuongY[id], gTimDuongZ[id]);
        SaveGPSTimDuong();

        new msg[128];
        format(msg, sizeof(msg), "Da cap nhat vi tri '{FFFF00}%s{FFFFFF}' (ID %d).", gTimDuongName[id], id);
        SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
    }
    else if(!strcmp(action, "reload", true))
    {
        LoadGPSTimDuong();
        new msg[64];
        format(msg, sizeof(msg), "Da tai lai %d dia diem GPS.", gGPSTimDuongCount);
        SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
    }
    else
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Tuy chon: {FFFF00}delete, add, setpos [id], reload");
    }
    return 1;
}

// ============================================================
// DIALOG RESPONSES
// ============================================================
hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid)
    {
        case DIALOG_MAP_TPHAT:
        {
            if(!response) return 1;
            switch(listitem)
            {
                case 0: ShowGPSTimDuong(playerid);
                case 1: PC_EmulateCommand(playerid, "/timvieclam");
                case 2: ShowPlayerDialog(playerid, CANSA_LIST, DIALOG_STYLE_LIST,
                            "{ffff00}KP5{ffffff} - Can Sa",
                            "Tim duong can sa\nXem can sa",
                            "Chon", "Thoat");
                case 3: PC_EmulateCommand(playerid, "/vitriminer");
                case 4: PC_EmulateCommand(playerid, "/vitrilumber");
            }
        }

        case TIMDUONG:
        {
            if(!response) return 1;
            if(listitem < 0 || listitem >= gGPSTimDuongCount) return 1;
            PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
            SetPlayerCP(playerid, gTimDuongX[listitem], gTimDuongY[listitem], gTimDuongZ[listitem], 3.0);
            gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
        }

        case TIMVIECLAM:
        {
            if(!response) return 1;
            if(listitem < 0 || listitem >= GPS_JOB_COUNT) return 1;
            PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
            if(listitem == GPS_JOB_COUNT - 1) // Can Sa
            {
                ShowPlayerDialog(playerid, CANSA_LIST, DIALOG_STYLE_LIST,
                    "{ffff00}KP5{ffffff} - Can Sa",
                    "Tim duong can sa\nXem can sa",
                    "Chon", "Thoat");
                return 1;
            }
            SetPlayerCP(playerid, gJobX[listitem], gJobY[listitem], gJobZ[listitem], 3.0);
            gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
        }

        case CANSA_LIST:
        {
            if(!response) return 1;
            if(listitem == 0)
            {
                ShowPlayerDialog(playerid, TIMDUONGCANSA, DIALOG_STYLE_LIST,
                    "{ffff00}KP5{ffffff} - Can Sa",
                    "Vi tri can sa moc\nChe bien can sa\nBan can sa",
                    "Chon", "Thoat");
            }
            else if(listitem == 1)
            {
                new str[256];
                format(str, sizeof(str), "{ffffff}Cay can sa:\t %d\nTep can sa:\t %d\nPot:\t\t %d\nCrack:\t\t %d",
                    PlayerInfo[playerid][pCanSa], PlayerInfo[playerid][pTepCanSa],
                    PlayerInfo[playerid][pPot], PlayerInfo[playerid][pCrack]);
                ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX,
                    "{ffff00}KP5{ffffff} - Xem can sa", str, "Dong y", "");
            }
        }

        case TIMDUONGCANSA:
        {
            if(!response) return 1;
            PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
            switch(listitem)
            {
                case 0: { SetPlayerCheckpoint(playerid, -1001.113586, -1664.279541, 76.367187, 3.0);    CP[playerid] = 252000; }
                case 1: { SetPlayerCheckpoint(playerid, -837.228576, -1992.566406, 17.786930, 3.0);  CP[playerid] = 252000; }
                case 2: { SetPlayerCheckpoint(playerid, -772.280334, -1962.709472, 7.574229, 3.0);    CP[playerid] = 252000; }
            }
        }

        case DIALOG_GPS_TD_MANAGE:
        {
            if(!response) return 1;
            if(listitem < 0 || listitem >= gGPSTimDuongCount) return 1;
            if(PlayerInfo[playerid][pAdmin] < 1337) return 1;

            new delname[48];
            strcopy(delname, gTimDuongName[listitem], 48);
            for(new i = listitem; i < gGPSTimDuongCount - 1; i++)
            {
                strcopy(gTimDuongName[i], gTimDuongName[i+1], 48);
                gTimDuongX[i] = gTimDuongX[i+1];
                gTimDuongY[i] = gTimDuongY[i+1];
                gTimDuongZ[i] = gTimDuongZ[i+1];
            }
            gGPSTimDuongCount--;
            SaveGPSTimDuong();

            new msg[128];
            format(msg, sizeof(msg), "Da xoa '{FFFF00}%s{FFFFFF}'. Con lai: %d dia diem.", delname, gGPSTimDuongCount);
            SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
            ShowGPSManage(playerid);
        }

        case DIALOG_GPS_TD_ADD:
        {
            if(!response) return 1;
            new len = strlen(inputtext);
            if(len == 0 || len > 47)
            {
                SendClientMessageEx(playerid, COLOR_GREY, "Ten dia diem khong hop le! (1-47 ky tu)");
                return 1;
            }
            if(gGPSTimDuongCount >= MAX_GPS_TIMDUONG)
            {
                SendClientMessageEx(playerid, COLOR_GREY, "Da dat so luong dia diem toi da!");
                return 1;
            }
            new id = gGPSTimDuongCount;
            strcopy(gTimDuongName[id], inputtext, 48);
            GetPlayerPos(playerid, gTimDuongX[id], gTimDuongY[id], gTimDuongZ[id]);
            gGPSTimDuongCount++;
            SaveGPSTimDuong();

            new msg[128];
            format(msg, sizeof(msg), "Da them '{FFFF00}%s{FFFFFF}' (ID %d) thanh cong!", inputtext, id);
            SendClientMessageEx(playerid, COLOR_LIGHTBLUE, msg);
        }
    }
    return 1;
}
