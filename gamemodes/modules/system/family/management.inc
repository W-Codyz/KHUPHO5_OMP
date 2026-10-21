// ===========================================================================
// gamemodes/modules/system/family/management.inc
// Lệnh /gangs – quản lý gang, thành viên, két sắt, bank
//
// TÍCH HỢP:
//   Thêm vào OnDialogResponse:
//     if(FM_OnDialogResponse(playerid, dialogid, response, listitem, inputtext)) return 1;
// ===========================================================================

// Dialog IDs (tiếp theo assets.inc, bắt đầu từ 5350)
#define DIALOG_GANGS_MAIN       5350
#define DIALOG_GANGS_INFO       5351
#define DIALOG_GANGS_MEMBERS    5352
#define DIALOG_GANGS_MEMBER_ACT 5353
#define DIALOG_GANGS_RANKUP     5354
#define DIALOG_GANGS_RANKDOWN   5355
#define DIALOG_GANGS_KICK       5356
#define DIALOG_GANGS_SAFE       5357
#define DIALOG_GANGS_SAFE_SETPOS 5358
#define DIALOG_GANGS_SAFE_UPGRADE 5359
#define DIALOG_GANGS_BANK       5360
#define DIALOG_GANGS_MOTD       5361
#define DIALOG_GANGS_MOTD_INPUT 5362

#if !defined DIALOG_STYLE_TABLIST_HEADERS
    #define DIALOG_STYLE_TABLIST_HEADERS 5
#endif

// Per-player state cho management dialogs
new g_fmSelectedPlayer[MAX_PLAYERS] = {-1, ...}; // Player đang được chọn để xử lý
new g_fmMotdSlot[MAX_PLAYERS];                   // Slot MOTD đang chỉnh (0-2)
new g_gangBadge[MAX_PLAYERS];                    // Gang badge toggle (0=tắt, 1=bật)

// Cache danh sách thành viên cho dialog tablist (single-owner, một lúc chỉ một commander xem)
#define MAX_GANG_MEMBER_DISPLAY 50
new g_fmListName    [MAX_GANG_MEMBER_DISPLAY][MAX_PLAYER_NAME];
new g_fmListRank    [MAX_GANG_MEMBER_DISPLAY];
new g_fmListDbId    [MAX_GANG_MEMBER_DISPLAY];
new g_fmListPlayerId[MAX_GANG_MEMBER_DISPLAY]; // playerid nếu online, -1 nếu offline
new g_fmListCount   = 0;

// Dữ liệu thành viên đang được chọn (dùng cho cả online lẫn offline)
new g_fmSelDbId [MAX_PLAYERS];
new g_fmSelRank [MAX_PLAYERS];
new g_fmSelName [MAX_PLAYERS][MAX_PLAYER_NAME];

forward UpdateNameTag(playerid);
forward OnGangMemberList(playerid);

// ---------------------------------------------------------------------------
// LỆNH /gangs (rank 5+)
// ---------------------------------------------------------------------------

CMD:gangs(playerid, params[])
{
    new fam = PlayerInfo[playerid][pFMember];
    if(fam == INVALID_FAMILY_ID)
        return SendClientMessageEx(playerid, COLOR_RED, "Ban chua vao gang nao."), 1;
    if(PlayerInfo[playerid][pRank] < 5)
        return SendClientMessageEx(playerid, COLOR_RED, "Can rank 5+ de su dung lenh nay."), 1;

    ShowPlayerDialog(playerid, DIALOG_GANGS_MAIN, DIALOG_STYLE_LIST,
        "Quan Ly Gang",
        "Thong tin gang\nCai dat MOTD\nQuan ly thanh vien\nKet sac\nLich su ket sac",
        "Chon", "Dong");
    return 1;
}

// ---------------------------------------------------------------------------
// LỆNH /gbadge – bật/tắt hiển thị tên gang trên đầu
// ---------------------------------------------------------------------------

CMD:gbadge(playerid, params[])
{
    #pragma unused params
    new fam = PlayerInfo[playerid][pFMember];
    if(fam == INVALID_FAMILY_ID)
        return SendClientMessageEx(playerid, COLOR_RED, "Ban chua vao gang nao."), 1;

    g_gangBadge[playerid] = !g_gangBadge[playerid];
    if(g_gangBadge[playerid])
        SendClientMessageEx(playerid, COLOR_GREEN, "Da bat gang badge. Ten gang se hien thi tren dau ban.");
    else
        SendClientMessageEx(playerid, COLOR_RED, "Da tat gang badge.");

    UpdateNameTag(playerid);
    return 1;
}

// ---------------------------------------------------------------------------
// DIALOG HELPERS
// ---------------------------------------------------------------------------

static stock FM_ShowGangInfo(playerid, famid)
{
    new lv    = FamilyInfo[famid][FamilyLevel];
    new onlineCnt = 0;
    foreach(new i : Player) if(PlayerInfo[i][pFMember] == famid) onlineCnt++;

    // Danh sách turfs đang sở hữu
    new turfList[128]; turfList[0] = 0;
    for(new t = 0; t < TotalTurfs; t++)
    {
        if(TurfData[t][twOwnerGang] == famid)
        {
            new tline[32];
            new float:X, float:Y, float:Z;
            X = TurfData[t][twPosX];
            Y = TurfData[t][twPosY];
            Z = TurfData[t][twPosZ];
            new zone[MAX_ZONE_NAME];
			Get3DZone(X, Y, Z, zone, sizeof(zone));
            format(tline, sizeof(tline), "  Turf #%d (%s)\n", t+1, zone);
            strcat(turfList, tline, sizeof(turfList));
        }
    }
    if(strlen(turfList) == 0) format(turfList, sizeof(turfList), "  (Chua so huu turf nao)\n");

    new str[512];
    format(str, sizeof(str),
        "{FFD700}===== %s =====\n\n\
        {AAAAAA}   Level ket:   {FFFF00}%d{AAAAAA}/%d\n\
        {AAAAAA}   Fame:        {FF8800}%d\n\
        {AAAAAA}   Thanh vien:  {FFFFFF}%d{AAAAAA}/%d  {44FF44}(Online: %d)\n\
        {AAAAAA}   Tien bank:   {FFFFFF}$%d\n\n\
        {FFD700}--- Turf dang so huu ---\n\
        {FFFFFF}%s",
        FamilyInfo[famid][FamilyName],
        lv, MAX_SAFE_LEVEL,
        FamilyInfo[famid][FamilyFame],
        FamilyInfo[famid][FamilyMembers], FA_GetMaxMembers(lv), onlineCnt,
        FamilyInfo[famid][FamilyBank],
        turfList);

    ShowPlayerDialog(playerid, DIALOG_GANGS_INFO, DIALOG_STYLE_MSGBOX, "Thong Tin Gang", str, "OK", "");
}

// Truy vấn MySQL để lấy toàn bộ thành viên (kể cả offline), kết quả xử lý trong OnGangMemberList
static stock FM_ShowMemberList(playerid, famid)
{
    new q[128];
    mysql_format(MainPipeline, q, sizeof(q),
        "SELECT `id`, `Username`, `Rank` FROM `accounts` WHERE `FMember`=%d ORDER BY `Rank` DESC",
        famid);
    mysql_tquery(MainPipeline, q, "OnGangMemberList", "i", playerid);
}

// Callback nhận kết quả danh sách thành viên từ MySQL
public OnGangMemberList(playerid)
{
    if(!IsPlayerConnected(playerid)) return 0;

    new rows = cache_num_rows();
    new cnt  = (rows < MAX_GANG_MEMBER_DISPLAY) ? rows : MAX_GANG_MEMBER_DISPLAY;

    g_fmListCount = 0;

    for(new r = 0; r < cnt; r++)
    {
        new dbid, rnk;
        new uname[MAX_PLAYER_NAME];
        cache_get_value_name_int(r, "id",       dbid);
        cache_get_value_name    (r, "Username", uname, MAX_PLAYER_NAME);
        cache_get_value_name_int(r, "Rank",     rnk);

        g_fmListDbId[r] = dbid;
        g_fmListRank[r] = rnk;
        format(g_fmListName[r], MAX_PLAYER_NAME, "%s", uname);

        // Tìm playerid nếu đang online
        new pid = -1;
        foreach(new i : Player)
        {
            if(PlayerInfo[i][pId] == dbid) { pid = i; break; }
        }
        g_fmListPlayerId[r] = pid;
        g_fmListCount++;
    }

    // Xây tablist: dòng đầu là header (dùng static tránh tốn stack)
    static list[3072];
    format(list, sizeof(list), "Ten\tTrang thai\tRank\n");

    for(new r = 0; r < g_fmListCount; r++)
    {
        new line[96];
        new statusStr[24];
        if(g_fmListPlayerId[r] != -1)
            format(statusStr, sizeof(statusStr), "{44FF44}Online");
        else
            format(statusStr, sizeof(statusStr), "{AAAAAA}Offline");

        format(line, sizeof(line), "%s\t%s\t{FFFF00}%d\n",
            g_fmListName[r],
            statusStr,
            g_fmListRank[r]);
        strcat(list, line, sizeof(list));
    }

    if(g_fmListCount == 0)
        strcat(list, "(Chua co thanh vien)\t\t\n", sizeof(list));

    ShowPlayerDialog(playerid, DIALOG_GANGS_MEMBERS, DIALOG_STYLE_TABLIST_HEADERS,
        "Danh Sach Thanh Vien",
        list,
        "Chon", "Quay Lai");
    return 1;
}

static stock FM_ShowMotdMenu(playerid, famid)
{
    new list[512];
    for(new i = 0; i < 3; i++)
    {
        new line[160];
        if(strlen(FamilyMOTD[famid][i]) > 0)
            format(line, sizeof(line), "{AAAAAA}[%d] {FFFFFF}%s\n", i + 1, FamilyMOTD[famid][i]);
        else
            format(line, sizeof(line), "{AAAAAA}[%d] {888888}(Chua co noi dung)\n", i + 1);
        strcat(list, line, sizeof(list));
    }
    ShowPlayerDialog(playerid, DIALOG_GANGS_MOTD, DIALOG_STYLE_LIST,
        "Cai Dat MOTD Gang", list, "Chinh sua", "Quay Lai");
}

// Dùng dữ liệu cache g_fmSel* nên không cần truyền targetid
static stock FM_ShowMemberActions(playerid)
{
    new title[64];
    format(title, sizeof(title), "Hanh Dong: %s (Rank %d)", g_fmSelName[playerid], g_fmSelRank[playerid]);

    ShowPlayerDialog(playerid, DIALOG_GANGS_MEMBER_ACT, DIALOG_STYLE_LIST,
        title,
        "{44FF44}Nang rank\n{FFAA00}Ha rank\n{FF4444}Kick khoi gang",
        "Chon", "Huy");
}

// ---------------------------------------------------------------------------
// DIALOG RESPONSE
// ---------------------------------------------------------------------------

stock FM_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    #pragma unused inputtext
    new fam  = PlayerInfo[playerid][pFMember];
    new rank = PlayerInfo[playerid][pRank];

    switch(dialogid)
    {
        // ---- Main menu /gangs ----
        case DIALOG_GANGS_MAIN:
        {
            if(!response) return 1;
            switch(listitem)
            {
                case 0: FM_ShowGangInfo(playerid, fam);
                case 1: // Cài đặt MOTD (rank 4+)
                {
                    if(rank < 4)
                        return SendClientMessageEx(playerid, COLOR_RED, "Can rank 4+ de cai dat MOTD."), 1;
                    FM_ShowMotdMenu(playerid, fam);
                }
                case 2: FM_ShowMemberList(playerid, fam);
                case 3: // Két sắt sub-menu
                {
                    new lv_s = FamilyInfo[fam][FamilyLevel];
                    new safeList[256];
                    format(safeList, sizeof(safeList),
                        "{FFD700}[ Di chuyen ket ]{AAAAAA}  Cooldown: 3 ngay\n\
                        {44FF44}[ Nang cap Level %d > %d ]{AAAAAA}  Chi phi: $%d + %d VL",
                        lv_s, lv_s + 1,
                        FA_GetUpgradeCostCash(lv_s + 1),
                        FA_GetUpgradeCostMats(lv_s + 1));
                    ShowPlayerDialog(playerid, DIALOG_GANGS_SAFE, DIALOG_STYLE_LIST,
                        "Ket Sac Gang", safeList, "Chon", "Quay Lai");
                }
                case 4: // Lịch sử két sắt (rank 5+ → gọi FA_ShowLogs)
                {
                    if(rank < 5)
                        return SendClientMessageEx(playerid, COLOR_RED, "Can rank 5+ de xem lich su ket sac."), 1;
                    FA_ShowLogs(playerid, fam);
                }
            }
            return 1;
        }

        case DIALOG_GANGS_INFO: return 1; // MSGBOX → chỉ có OK

        // ---- Danh sách thành viên → chọn để xử lý ----
        case DIALOG_GANGS_MEMBERS:
        {
            if(!response)
            {
                ShowPlayerDialog(playerid, DIALOG_GANGS_MAIN, DIALOG_STYLE_LIST,
                    "Quan Ly Gang",
                    "Thong tin gang\nCai dat MOTD\nQuan ly thanh vien\nKet sac\nLich su ket sac",
                    "Chon", "Dong");
                return 1;
            }

            if(listitem < 0 || listitem >= g_fmListCount)
                return SendClientMessageEx(playerid, COLOR_RED, "Lua chon khong hop le."), 1;

            // Không cho chọn bản thân
            if(g_fmListDbId[listitem] == PlayerInfo[playerid][pId])
                return SendClientMessageEx(playerid, COLOR_RED, "Khong the chon ban than."), 1;

            // Lưu thông tin thành viên được chọn vào cache per-player
            g_fmSelectedPlayer[playerid] = g_fmListPlayerId[listitem];
            g_fmSelDbId[playerid]        = g_fmListDbId[listitem];
            g_fmSelRank[playerid]        = g_fmListRank[listitem];
            format(g_fmSelName[playerid], MAX_PLAYER_NAME, "%s", g_fmListName[listitem]);

            FM_ShowMemberActions(playerid);
            return 1;
        }

        // ---- Hành động với thành viên (online hoặc offline) ----
        case DIALOG_GANGS_MEMBER_ACT:
        {
            if(!response) return 1;

            new targetid = g_fmSelectedPlayer[playerid];
            new isOnline = (targetid != -1 && IsPlayerConnected(targetid));
            new tRank    = g_fmSelRank[playerid];
            new tDbId    = g_fmSelDbId[playerid];
            new tname[MAX_PLAYER_NAME];
            format(tname, sizeof(tname), "%s", g_fmSelName[playerid]);

            switch(listitem)
            {
                case 0: // Nâng rank
                {
                    new newRank = tRank + 1;
                    if(newRank >= rank && PlayerInfo[playerid][pAdmin] <= 2)
                        return SendClientMessageEx(playerid, COLOR_RED, "Khong the nang rank cao bang hoac hon ban."), 1;

                    if(isOnline) PlayerInfo[targetid][pRank] = newRank;

                    new q[128];
                    mysql_format(MainPipeline, q, sizeof(q),
                        "UPDATE `accounts` SET `Rank`=%d WHERE `id`=%d", newRank, tDbId);
                    mysql_tquery(MainPipeline, q, "", "");

                    new str[80];
                    format(str, sizeof(str), "Da nang rank %s len Rank %d.", tname, newRank);
                    SendClientMessageEx(playerid, COLOR_GREEN, str);
                    if(isOnline)
                    {
                        format(str, sizeof(str), "Ban da duoc nang len Rank %d boi %s.", newRank, ReturnPlayerName(playerid));
                        SendClientMessageEx(targetid, COLOR_GREEN, str);
                    }
                }
                case 1: // Hạ rank
                {
                    new newRank = tRank - 1;
                    if(newRank < 1)
                        return SendClientMessageEx(playerid, COLOR_RED, "Rank thap nhat la 1."), 1;
                    if(tRank >= rank && PlayerInfo[playerid][pAdmin] <= 2)
                        return SendClientMessageEx(playerid, COLOR_RED, "Khong the ha rank nguoi co rank cao hon ban."), 1;

                    if(isOnline) PlayerInfo[targetid][pRank] = newRank;

                    new q2[128];
                    mysql_format(MainPipeline, q2, sizeof(q2),
                        "UPDATE `accounts` SET `Rank`=%d WHERE `id`=%d", newRank, tDbId);
                    mysql_tquery(MainPipeline, q2, "", "");

                    new str[80];
                    format(str, sizeof(str), "Da ha rank %s xuong Rank %d.", tname, newRank);
                    SendClientMessageEx(playerid, COLOR_GREEN, str);
                    if(isOnline)
                    {
                        format(str, sizeof(str), "Ban da bi ha xuong Rank %d boi %s.", newRank, ReturnPlayerName(playerid));
                        SendClientMessageEx(targetid, COLOR_RED, str);
                    }
                }
                case 2: // Kick (hỗ trợ cả offline)
                {
                    if(tRank >= 6 && PlayerInfo[playerid][pAdmin] <= 2)
                        return SendClientMessageEx(playerid, COLOR_RED, "Khong the kick thanh vien rank 6."), 1;
                    if(tRank >= rank && PlayerInfo[playerid][pAdmin] <= 2)
                        return SendClientMessageEx(playerid, COLOR_RED, "Chi kick duoc nguoi co rank thap hon ban."), 1;

                    if(isOnline)
                    {
                        PlayerInfo[targetid][pFMember] = INVALID_FAMILY_ID;
                        PlayerInfo[targetid][pRank]    = 0;
                        SendClientMessageEx(targetid, COLOR_RED, "Ban da bi kick khoi gang.");
                    }
                    FamilyInfo[fam][FamilyMembers]--;

                    new q3[128];
                    mysql_format(MainPipeline, q3, sizeof(q3),
                        "UPDATE `accounts` SET `FMember`=255,`Rank`=0 WHERE `id`=%d", tDbId);
                    mysql_tquery(MainPipeline, q3, "", "");
                    SaveFamily(fam);

                    new str[80];
                    format(str, sizeof(str), "Da kick %s khoi gang.", tname);
                    SendClientMessageEx(playerid, COLOR_GREEN, str);
                }
            }
            return 1;
        }

        // ---- Két sắt sub-menu ----
        case DIALOG_GANGS_SAFE:
        {
            if(!response)
            {
                ShowPlayerDialog(playerid, DIALOG_GANGS_MAIN, DIALOG_STYLE_LIST,
                    "Quan Ly Gang",
                    "Thong tin gang\nCai dat MOTD\nQuan ly thanh vien\nKet sac\nLich su ket sac",
                    "Chon", "Dong");
                return 1;
            }
            switch(listitem)
            {
                case 0: // Đặt vị trí két về vị trí hiện tại
                {
                    // Kiểm tra cooldown 3 ngày
                    new elapsed = gettime() - g_safeCooldown[fam];
                    if(g_safeCooldown[fam] != 0 && elapsed < SAFE_MOVE_COOLDOWN)
                    {
                        new dLeft = (SAFE_MOVE_COOLDOWN - elapsed) / 86400;
                        new hLeft = ((SAFE_MOVE_COOLDOWN - elapsed) % 86400) / 3600;
                        new cdStr[96];
                        format(cdStr, sizeof(cdStr), "Ket sac chi duoc doi vi tri sau %d ngay %d gio nua.", dLeft, hLeft);
                        return SendClientMessageEx(playerid, COLOR_RED, cdStr), 1;
                    }

                    new Float:px, Float:py, Float:pz;
                    GetPlayerPos(playerid, px, py, pz);
                    FamilyInfo[fam][FamilySafe][0] = px;
                    FamilyInfo[fam][FamilySafe][1] = py;
                    FamilyInfo[fam][FamilySafe][2] = pz;

                    // Xóa pickup cũ và tạo mới
                    if(FamilyInfo[fam][FamilyPickup] != 0)
                        DestroyDynamicPickup(FamilyInfo[fam][FamilyPickup]);
                    FamilyInfo[fam][FamilyPickup] = CreateDynamicPickup(1239, 23, px, py, pz, -1);

                    g_safeCooldown[fam] = gettime();
                    SaveFamily(fam);
                    SaveFamilyExt(fam);
                    FA_UpdateSafeLabel(fam);
                    SendClientMessageEx(playerid, COLOR_GREEN, "Da cap nhat vi tri ket sac gang. Dung /glocker tai day de mo ket. Cooldown: 3 ngay.");
                }
                case 1: // Nâng cấp két
                {
                    new lv     = FamilyInfo[fam][FamilyLevel];
                    new nextLv = lv + 1;
                    if(nextLv > MAX_SAFE_LEVEL)
                        return SendClientMessageEx(playerid, COLOR_RED, "Ket sac da dat cap do toi da."), 1;

                    new str[512];
                    format(str, sizeof(str),
                        "{FFD700}===== Nang Cap Ket Sac =====\n\n\
                    {AAAAAA}   Level hien tai:  {FFFF00}%d  {AAAAAA}->  {44FF44}%d\n\n\
                    {FFD700}--- Chi phi ---\n\
                    {AAAAAA}   Tien mat:        {FF6B6B}$%d\n\
                    {AAAAAA}   Vat lieu:        {FF6B6B}%d\n\n\
                    {FFD700}--- Sau khi nang cap ---\n\
                    {AAAAAA}   Tien max:        {FFFFFF}$%d\n\
                    {AAAAAA}   Hang max:        {FFFFFF}%d\n\
                    {AAAAAA}   VL max:          {FFFFFF}%d\n\
                    {AAAAAA}   Slot vu khi:     {FFFFFF}%d\n\
                    {AAAAAA}   Thanh vien max:  {FFFFFF}%d",
                        lv, nextLv,
                        FA_GetUpgradeCostCash(nextLv),
                        FA_GetUpgradeCostMats(nextLv),
                        FA_GetMaxCash(nextLv),
                        FA_GetMaxGoods(nextLv),
                        FA_GetMaxMats(nextLv),
                        FA_GetMaxGunSlots(nextLv),
                        FA_GetMaxMembers(nextLv));
                    ShowPlayerDialog(playerid, DIALOG_GANGS_SAFE_UPGRADE, DIALOG_STYLE_MSGBOX,
                        "Xac Nhan Nang Cap Ket", str, "Nang Cap", "Huy");
                }
            }
            return 1;
        }

        case DIALOG_GANGS_SAFE_UPGRADE:
        {
            if(!response) return 1;
            FA_UpgradeSafe(playerid, fam);
            return 1;
        }

        // ---- MOTD ----
        case DIALOG_GANGS_MOTD:
        {
            if(!response)
            {
                ShowPlayerDialog(playerid, DIALOG_GANGS_MAIN, DIALOG_STYLE_LIST,
                    "Quan Ly Gang",
                    "Thong tin gang\nCai dat MOTD\nQuan ly thanh vien\nKet sac\nLich su ket sac",
                    "Chon", "Dong");
                return 1;
            }
            if(listitem < 0 || listitem > 2) return 1;
            g_fmMotdSlot[playerid] = listitem;
            new title[32];
            format(title, sizeof(title), "Chinh Sua MOTD Slot %d", listitem + 1);
            new cur[128];
            format(cur, sizeof(cur), "%s", FamilyMOTD[fam][listitem]);
            ShowPlayerDialog(playerid, DIALOG_GANGS_MOTD_INPUT, DIALOG_STYLE_INPUT,
                title, "{ffffff}Nhap noi dung MOTD (de trong de xoa):", "Luu", "Huy");
            #pragma unused cur
            return 1;
        }

        case DIALOG_GANGS_MOTD_INPUT:
        {
            if(!response)
            {
                FM_ShowMotdMenu(playerid, fam);
                return 1;
            }
            new slot = g_fmMotdSlot[playerid];
            if(strlen(inputtext) > 128)
                return SendClientMessageEx(playerid, COLOR_RED, "MOTD toi da 128 ky tu."), 1;
            strmid(FamilyMOTD[fam][slot], inputtext, 0, strlen(inputtext), 128);
            SaveFamily(fam);
            new str[64];
            format(str, sizeof(str), "Da cap nhat MOTD slot %d.", slot + 1);
            SendClientMessageEx(playerid, COLOR_GREEN, str);
            FM_ShowMotdMenu(playerid, fam);
            return 1;
        }
    }
    return 0;
}
