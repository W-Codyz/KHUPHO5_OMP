// ===========================================================================
// gamemodes/modules/system/family/turfwars.inc
// Hệ thống Turf Wars – chiếm địa bàn theo vùng pickup
//
// TÍCH HỢP: Thêm các lệnh sau vào cuối mỗi callback tương ứng trong callbacks.inc
//   OnPlayerDeath   → TW_OnPlayerDeath(playerid, killerid, reason);
//   OnPlayerPickUpPickup → TW_OnPlayerPickUpPickup(playerid, pickupid);
//   OnPlayerEnterCheckpoint → TW_OnPlayerEnterCheckpoint(playerid);
//
// Thêm vào OnDialogResponse (dialog/main.inc hoặc callbacks.inc):
//   if(TW_OnDialogResponse(playerid, dialogid, response, listitem, inputtext)) return 1;
//
// Gọi khi OnGameModeInit:
//   TurfWars_InitMySQL();
//   TurfWars_Load();
//   TurfSupplyPoints_InitMySQL();
//   TurfSupplyPoints_Load();
// ===========================================================================

// ---------------------------------------------------------------------------
// HẰNG SỐ
// ---------------------------------------------------------------------------
#define TURF_WAR_TIME               600     // Thời gian 1 ván war (giây)
#define TURF_SUPPLY_MAX             50      // Số lần giao hàng để hoàn thành
#define TURF_SUPPLY_COST            25000   // Chi phí mua 1 lô hàng
#define TURF_SUPPLY_REWARD          35000   // Thưởng khi giao thành công
#define TURF_SUPPLY_DEADLINE        172800  // Deadline 48 giờ (giây)
#define TURF_WIN_FAME               1       // Fame thưởng khi chiếm được turf
#define TURF_SUPPLY_DONE_FAME       5       // Fame khi hoàn thành 50 supply
#define TURF_SUPPLY_DONE_BANK       200000  // Tiền bank thưởng khi xong supply
#define TURF_SUPPLY_FAIL_FAME       2       // Fame bị trừ khi trễ deadline
#define TURF_WAR_RADIUS             100.0   // Bán kính tính quân khi war kết thúc
#define TURF_SUPPLY_WAIT            30      // Giây chờ tại điểm lấy hàng
#define TURF_SUPPLY_DELIVER_RADIUS  5.0     // Bán kính giao hàng tại turf
#define TURF_SUPPLY_COLLECT_RADIUS  3.0     // Bán kính pickup lấy hàng
#define TURF_ADMIN_LEVEL            4       // Admin level yêu cầu cho /turfadmin
#define TURF_WAR_MIN_MEMBERS        3       // Số thành viên online tối thiểu để random war

// Vị trí điểm lấy hàng cấm cố định
#define TW_SUPPLY_X                 2145.0
#define TW_SUPPLY_Y                 (-1672.0)
#define TW_SUPPLY_Z                 15.0

// Dialog IDs – bắt đầu từ 5300 để không đụng hàng ID cũ
#define DIALOG_TURFADMIN_MAIN       5300
#define DIALOG_TURFADMIN_LIST       5301
#define DIALOG_TURFADMIN_DELETE     5302
#define DIALOG_TURFADMIN_WARSEL     5303
#define DIALOG_TURFADMIN_SELGANG1   5304
#define DIALOG_TURFADMIN_SELGANG2   5305
#define DIALOG_TURFADMIN_WARCONFIRM 5306
#define DIALOG_TURFADMIN_LOCKLIST   5307
#define DIALOG_DIEMCHIEMDONG        5308
#define DIALOG_SUPPLY_CONFIRM       5309
#define DIALOG_SUPPLY_ADMIN_LIST    5310
#define DIALOG_SUPPLY_ADMIN_DEL     5311
#define DIALOG_TURF_LIST            5312

// ---------------------------------------------------------------------------
// CẤU TRÚC DỮ LIỆU
// ---------------------------------------------------------------------------
enum E_TURF_DATA {
    twId,               // ID trong database (AUTO_INCREMENT)
    twActive,           // 0 = bình thường, 1 = đang war
    twTimeLeft,         // Giây còn lại trong war
    twPickupId,         // ID pickup trên map
    twAttackGang,       // Gang tấn công (-1 = không ai)
    twDefendGang,       // Gang phòng thủ (-1 = không có chủ)
    twOwnerGang,        // Gang sở hữu sau khi chiếm (-1 = chưa ai)
    Float:twPosX,
    Float:twPosY,
    Float:twPosZ,
    twSupplyCount,      // Số chuyến hàng đã hoàn thành (0-50)
    twSupplyDeadline,   // Unix timestamp hạn chót 48h
    twLocked            // 1 = admin khóa, không cho war
};
new TurfData[MAX_TURFS][E_TURF_DATA];
new TotalTurfs = 0;

// Trạng thái per-player
new g_twAdminTurf[MAX_PLAYERS] = {-1, ...};       // Turf đang thao tác trong /turfadmin
new g_twAdminGang1[MAX_PLAYERS] = {-1, ...};      // Gang 1 đã chọn khi setup war
static g_twAdminSupplyPt[MAX_PLAYERS] = {-1, ...}; // Supply point đang chọn để xóa
new bool:g_twSupplyCollecting[MAX_PLAYERS];        // Đang đứng chờ lấy hàng
new bool:g_twSupplyCarrying[MAX_PLAYERS];          // Đang mang hàng về turf
new g_twSupplyTurf[MAX_PLAYERS] = {-1, ...};       // Turf đích giao hàng
new g_twSupplyPtIdx[MAX_PLAYERS] = {-1, ...};      // Supply point đang lấy hàng
new g_twSupplyCheckTimer[MAX_PLAYERS] = {-1, ...}; // Timer kiểm tra bán kính 10m
new g_twSupplyTimeLeft[MAX_PLAYERS];               // Giây còn lại

// Text3D labels cho mỗi turf (-1 = chưa tạo)
static g_twLabel[MAX_TURFS] = {-1, ...};

// Runtime flag: supply hoan thanh, dang cho /nhanthuongturf
static g_twSupplyDone[MAX_TURFS];

// Turf war zone mà player đang đứng trong (-1 = không ai)
new g_twInZone[MAX_PLAYERS] = {-1, ...};

// ---- Điểm lấy hàng cấm — đa điểm, quản lý bởi admin ----
#define MAX_SUPPLY_POINTS       10
#define SUPPLY_PICKUP_RADIUS    5.0

enum E_SUPPLY_PT {
    spDbId,
    spPickupId,
    spLabelId,
    Float:spPosX,
    Float:spPosY,
    Float:spPosZ
};
new g_SupplyPts[MAX_SUPPLY_POINTS][E_SUPPLY_PT];
new g_SupplyPtCount = 0;

// ---------------------------------------------------------------------------
// TEXT LABEL
// ---------------------------------------------------------------------------

static stock TW_UpdateLabel(turfid)
{
    if(turfid < 0 || turfid >= TotalTurfs) return;

    // Xóa label cũ
    if(g_twLabel[turfid] != -1)
    {
        DestroyDynamic3DTextLabel(Text3D:g_twLabel[turfid]);
        g_twLabel[turfid] = -1;
    }

    new ownerName[32];
    new ownerIdx = TurfData[turfid][twOwnerGang];
    if(ownerIdx >= 0 && ownerIdx < MAX_FAMILY)
        strmid(ownerName, FamilyInfo[ownerIdx][FamilyName], 0, 32, 32);
    else
        format(ownerName, 32, "Chua co chu");

    new statusLine[128];
    new labelColor;

    if(TurfData[turfid][twActive])
    {
        new aName[32], dName[32];
        new aGang = TurfData[turfid][twAttackGang];
        new dGang = TurfData[turfid][twDefendGang];
        if(aGang >= 0 && aGang < MAX_FAMILY) strmid(aName, FamilyInfo[aGang][FamilyName], 0, 32, 32);
        else format(aName, 32, "???");
        if(dGang >= 0 && dGang < MAX_FAMILY) strmid(dName, FamilyInfo[dGang][FamilyName], 0, 32, 32);
        else format(dName, 32, "_");
        new mins = TurfData[turfid][twTimeLeft] / 60;
        new secs = TurfData[turfid][twTimeLeft] % 60;
        format(statusLine, sizeof(statusLine),
            "{FF4444}[DANG CHIEM DONG]\n{FFFFFF}Gang 1: {FF8800}%s\n{FFFFFF}Gang 2: {4488FF}%s\n{FFFF00}Thoi gian: {FFFFFF}%02d:%02d",
            aName, dName, mins, secs);
        labelColor = 0xFF4444FF;
    }
    else if(TurfData[turfid][twLocked])
    {
        format(statusLine, sizeof(statusLine), "{888888}[DA KHOA]\n{AAAAAA}Admin khoa, khong the war.");
        labelColor = 0x888888FF;
    }
    else if(ownerIdx >= 0 && g_twSupplyDone[turfid])
    {
        format(statusLine, sizeof(statusLine),
            "{FF8C00}[PHAN THUONG SAN SANG]\n{FFFFFF}Rank 4+ dung {FFFF00}/nhanthuongturf");
        labelColor = 0xFF8C00FF;
    }
    else if(ownerIdx >= 0 && TurfData[turfid][twSupplyDeadline] > 0)
    {
        new hoursLeft = (TurfData[turfid][twSupplyDeadline] - gettime()) / 3600;
        if(hoursLeft < 0) hoursLeft = 0;
        format(statusLine, sizeof(statusLine),
            "{FFFF00}[HANG CAM DANG CHAY]\n{FFFFFF}Tien do: {FFFF00}%d{FFFFFF}/%d chuyen hang cam\n{FFFFFF}Het han: {FF8800}%dh",
            TurfData[turfid][twSupplyCount], TURF_SUPPLY_MAX, hoursLeft);
        labelColor = 0xFFFF00FF;
    }
    else if(ownerIdx >= 0)
    {
        format(statusLine, sizeof(statusLine), "{44FF44}[BINH THUONG]\n{AAFFAA}San sang chiem dong.");
        labelColor = 0xFFFFFFFF;
    }
    else
    {
        format(statusLine, sizeof(statusLine), "{44FFFF}[TRONG]\n{AAFFFF}Chua ai so huu - hay chiem ngay!");
        labelColor = 0xAAFFFFFF;
    }

    new labelText[384];
    format(labelText, sizeof(labelText),
        "{FFFFFF}[ DIEM CHIEM DONG #%d ]\n{FFAA00}So huu: {FFFFFF}%s\n%s",
        turfid + 1, ownerName, statusLine);

    g_twLabel[turfid] = _:CreateDynamic3DTextLabel(labelText, labelColor,
        TurfData[turfid][twPosX],
        TurfData[turfid][twPosY],
        TurfData[turfid][twPosZ] + 1.2,
        20.0);
}

// Cập nhật label cho điểm lấy hàng cấm
static stock TW_UpdateSupplyLabel(idx)
{
    if(idx < 0 || idx >= g_SupplyPtCount) return;

    if(g_SupplyPts[idx][spLabelId] != -1)
    {
        DestroyDynamic3DTextLabel(Text3D:g_SupplyPts[idx][spLabelId]);
        g_SupplyPts[idx][spLabelId] = -1;
    }

    new labelText[256];
    format(labelText, sizeof(labelText),
        "{FFFFFF}[ DIEM LAY HANG CAM #%d ]\n\
{FFAA00}Lenh: {FFFFFF}/layhangcam\n\
{FFAA00}Chi phi:   {FF4444}$%d\n\
{FFAA00}Phan thuong: {44FF44}$%d\n\
{888888}(Chu so huu diem chiem dong moi dung duoc)",
        idx + 1, TURF_SUPPLY_COST, TURF_SUPPLY_REWARD);

    g_SupplyPts[idx][spLabelId] = _:CreateDynamic3DTextLabel(labelText, 0xFFAA00FF,
        g_SupplyPts[idx][spPosX],
        g_SupplyPts[idx][spPosY],
        g_SupplyPts[idx][spPosZ] + 1.0,
        15.0);
}

// ---------------------------------------------------------------------------
// MYSQL
// ---------------------------------------------------------------------------

// Tạo bảng nếu chưa có
stock TurfWars_InitMySQL()
{
    mysql_tquery(MainPipeline,
        "CREATE TABLE IF NOT EXISTS `turfs` (\
        `id` INT AUTO_INCREMENT PRIMARY KEY,\
        `pos_x` FLOAT NOT NULL DEFAULT 0.0,\
        `pos_y` FLOAT NOT NULL DEFAULT 0.0,\
        `pos_z` FLOAT NOT NULL DEFAULT 0.0,\
        `owner_gang` INT DEFAULT -1,\
        `supply_count` INT DEFAULT 0,\
        `supply_deadline` INT DEFAULT 0,\
        `locked` TINYINT DEFAULT 0\
        )",
        "", "");
}

// Load toàn bộ turfs từ database
stock TurfWars_Load()
{
    mysql_tquery(MainPipeline, "SELECT * FROM `turfs` ORDER BY `id` ASC", "TW_OnLoad", "");
}

forward TW_OnLoad();
public TW_OnLoad()
{
    new rows = cache_num_rows();
    TotalTurfs = 0;

    for(new i = 0; i < rows && TotalTurfs < MAX_TURFS; i++)
    {
        new idx = TotalTurfs;
        cache_get_value_name_int(i, "id",             TurfData[idx][twId]);
        cache_get_value_name_float(i, "pos_x",        TurfData[idx][twPosX]);
        cache_get_value_name_float(i, "pos_y",        TurfData[idx][twPosY]);
        cache_get_value_name_float(i, "pos_z",        TurfData[idx][twPosZ]);
        cache_get_value_name_int(i, "owner_gang",     TurfData[idx][twOwnerGang]);
        cache_get_value_name_int(i, "supply_count",   TurfData[idx][twSupplyCount]);
        cache_get_value_name_int(i, "supply_deadline",TurfData[idx][twSupplyDeadline]);
        cache_get_value_name_int(i, "locked",         TurfData[idx][twLocked]);

        // Fields runtime: reset khi server khởi động
        TurfData[idx][twActive]     = 0;
        TurfData[idx][twTimeLeft]   = 0;
        TurfData[idx][twAttackGang] = -1;
        TurfData[idx][twDefendGang] = -1;

        TurfData[idx][twPickupId] = CreateDynamicPickup(1239, 23,
            TurfData[idx][twPosX], TurfData[idx][twPosY], TurfData[idx][twPosZ], -1);

        TotalTurfs++;
        TW_UpdateLabel(idx);
    }
    printf("[TurfWars] Da tai %d turf tu database.", TotalTurfs);
}

// Lưu 1 turf
stock TW_SaveTurf(turfid)
{
    if(turfid < 0 || turfid >= TotalTurfs) return;
    new q[256];
    mysql_format(MainPipeline, q, sizeof(q),
        "UPDATE `turfs` SET `owner_gang`=%d,`supply_count`=%d,`supply_deadline`=%d,`locked`=%d WHERE `id`=%d",
        TurfData[turfid][twOwnerGang],
        TurfData[turfid][twSupplyCount],
        TurfData[turfid][twSupplyDeadline],
        TurfData[turfid][twLocked],
        TurfData[turfid][twId]
    );
    mysql_tquery(MainPipeline, q, "", "");
}

// ---------------------------------------------------------------------------
// HÀM HỖ TRỢ
// ---------------------------------------------------------------------------

// Gửi message cho toàn bộ thành viên online của gang
static stock TW_FamilyMsg(familyid, color, const msg[])
{
    if(familyid < 0 || familyid >= MAX_FAMILY) return;
    foreach(new i : Player)
    {
        if(PlayerInfo[i][pFMember] == familyid)
            SendClientMessageEx(i, color, msg);
    }
}

// Đếm thành viên gang trong bán kính quanh tọa độ turf
static stock TW_CountNear(familyid, turfid)
{
    new count = 0;
    new Float:tx = TurfData[turfid][twPosX],
        Float:ty = TurfData[turfid][twPosY],
        Float:tz = TurfData[turfid][twPosZ];
    foreach(new i : Player)
    {
        if(PlayerInfo[i][pFMember] != familyid) continue;
        new Float:px, Float:py, Float:pz;
        GetPlayerPos(i, px, py, pz);
        new Float:d = floatsqroot((px-tx)*(px-tx)+(py-ty)*(py-ty)+(pz-tz)*(pz-tz));
        if(d <= TURF_WAR_RADIUS) count++;
    }
    return count;
}

// Đếm thành viên gang đang online
static stock TW_CountOnline(familyid)
{
    new count = 0;
    foreach(new i : Player) if(PlayerInfo[i][pFMember] == familyid) count++;
    return count;
}

// Lấy mã màu nhúng vào chat từ FamilyColor (0-14)
static stock TW_FamilyColorTag(familyid, out[], size)
{
    if(familyid < 0 || familyid >= MAX_FAMILY) { format(out, size, "{FFFFFF}"); return; }
    switch(FamilyInfo[familyid][FamilyColor])
    {
        case 1:  format(out, size, "{FFFFFF}");
        case 2:  format(out, size, "{FF0000}");
        case 3:  format(out, size, "{0000FF}");
        case 4:  format(out, size, "{FFFF00}");
        case 5:  format(out, size, "{800080}");
        case 6:  format(out, size, "{FF69B4}");
        case 7:  format(out, size, "{8B4513}");
        case 8:  format(out, size, "{808080}");
        case 9:  format(out, size, "{808000}");
        case 10: format(out, size, "{D2B48C}");
        case 11: format(out, size, "{00FFFF}");
        case 12: format(out, size, "{FF8C00}");
        case 13: format(out, size, "{007FFF}");
        case 14: format(out, size, "{008000}");
        default: format(out, size, "{000000}");
    }
}

// ---------------------------------------------------------------------------
// SUPPLY POINTS — MySQL persistence
// ---------------------------------------------------------------------------

stock TurfSupplyPoints_InitMySQL()
{
    mysql_tquery(MainPipeline,
        "CREATE TABLE IF NOT EXISTS `turf_supply_points` (\
        `id` INT AUTO_INCREMENT PRIMARY KEY,\
        `pos_x` FLOAT NOT NULL DEFAULT 0.0,\
        `pos_y` FLOAT NOT NULL DEFAULT 0.0,\
        `pos_z` FLOAT NOT NULL DEFAULT 0.0\
        )",
        "", "");
}

stock TurfSupplyPoints_Load()
{
    mysql_tquery(MainPipeline, "SELECT * FROM `turf_supply_points` ORDER BY `id` ASC", "TSP_OnLoad", "");
}

forward TSP_OnLoad();
public TSP_OnLoad()
{
    new rows = cache_num_rows();
    g_SupplyPtCount = 0;
    for(new i = 0; i < rows && g_SupplyPtCount < MAX_SUPPLY_POINTS; i++)
    {
        new idx = g_SupplyPtCount;
        cache_get_value_name_int(i,   "id",    g_SupplyPts[idx][spDbId]);
        cache_get_value_name_float(i, "pos_x", g_SupplyPts[idx][spPosX]);
        cache_get_value_name_float(i, "pos_y", g_SupplyPts[idx][spPosY]);
        cache_get_value_name_float(i, "pos_z", g_SupplyPts[idx][spPosZ]);
        g_SupplyPts[idx][spPickupId] = CreateDynamicPickup(1239, 23,
            g_SupplyPts[idx][spPosX], g_SupplyPts[idx][spPosY], g_SupplyPts[idx][spPosZ], -1);
        g_SupplyPts[idx][spLabelId] = -1;
        g_SupplyPtCount++;
        TW_UpdateSupplyLabel(idx);
    }
    printf("[TurfWars] Da tai %d diem lay hang cam.", g_SupplyPtCount);
}

static stock TW_AddSupplyPt(playerid, Float:x, Float:y, Float:z)
{
    if(g_SupplyPtCount >= MAX_SUPPLY_POINTS)
    {
        SendClientMessageEx(playerid, COLOR_RED, "Da dat gioi han diem lay hang cam toi da.");
        return;
    }
    new q[192];
    mysql_format(MainPipeline, q, sizeof(q),
        "INSERT INTO `turf_supply_points` (`pos_x`,`pos_y`,`pos_z`) VALUES (%f,%f,%f)",
        x, y, z);
    mysql_tquery(MainPipeline, q, "TSP_OnCreated", "i", playerid);
}

forward TSP_OnCreated(playerid);
public TSP_OnCreated(playerid)
{
    new idx = g_SupplyPtCount;
    g_SupplyPts[idx][spDbId] = cache_insert_id();
    GetPlayerPos(playerid, g_SupplyPts[idx][spPosX], g_SupplyPts[idx][spPosY], g_SupplyPts[idx][spPosZ]);
    g_SupplyPts[idx][spPickupId] = CreateDynamicPickup(1239, 23,
        g_SupplyPts[idx][spPosX], g_SupplyPts[idx][spPosY], g_SupplyPts[idx][spPosZ], -1);
    g_SupplyPts[idx][spLabelId] = -1;
    g_SupplyPtCount++;
    TW_UpdateSupplyLabel(idx);

    new str[96];
    format(str, sizeof(str), "Da them diem lay hang cam #%d tai vi tri hien tai.", idx + 1);
    SendClientMessageEx(playerid, COLOR_GREEN, str);
}

static stock TW_RemoveSupplyPt(idx)
{
    if(idx < 0 || idx >= g_SupplyPtCount) return;

    new q[64];
    mysql_format(MainPipeline, q, sizeof(q), "DELETE FROM `turf_supply_points` WHERE `id`=%d", g_SupplyPts[idx][spDbId]);
    mysql_tquery(MainPipeline, q, "", "");

    if(g_SupplyPts[idx][spPickupId] != -1)
        DestroyDynamicPickup(g_SupplyPts[idx][spPickupId]);
    if(g_SupplyPts[idx][spLabelId] != -1)
    {
        DestroyDynamic3DTextLabel(Text3D:g_SupplyPts[idx][spLabelId]);
    }

    for(new i = idx; i < g_SupplyPtCount - 1; i++)
        g_SupplyPts[i] = g_SupplyPts[i+1];

    new last = g_SupplyPtCount - 1;
    g_SupplyPts[last][spDbId]    = 0;
    g_SupplyPts[last][spPickupId] = -1;
    g_SupplyPts[last][spLabelId]  = -1;
    g_SupplyPts[last][spPosX]    = 0.0;
    g_SupplyPts[last][spPosY]    = 0.0;
    g_SupplyPts[last][spPosZ]    = 0.0;
    g_SupplyPtCount--;
}

// ---------------------------------------------------------------------------
// QUẢN LÝ WAR
// ---------------------------------------------------------------------------

// Bắt đầu war: attackGang tấn công turf (đang thuộc defendGang hoặc -1)
stock TW_StartWar(turfid, attackGang, defendGang)
{
    if(turfid < 0 || turfid >= TotalTurfs)        return;
    if(TurfData[turfid][twActive])                return;
    if(TurfData[turfid][twLocked])                return;

    TurfData[turfid][twActive]      = 1;
    TurfData[turfid][twTimeLeft]    = TURF_WAR_TIME;
    TurfData[turfid][twAttackGang]  = attackGang;
    TurfData[turfid][twDefendGang]  = defendGang;

    new str[256], aName[32], dName[32];
    if(attackGang >= 0) strmid(aName, FamilyInfo[attackGang][FamilyName], 0, 32, 32);
    else format(aName, 32, "???");
    if(defendGang >= 0) strmid(dName, FamilyInfo[defendGang][FamilyName], 0, 32, 32);
    else format(dName, 32, "_");

    format(str, sizeof(str),
        "[CHIEM DONG] %s va %s bat dau tranh gianh! (%.1f, %.1f) - Dung /diemchiemdong xem chi tiet.",
        aName, dName, TurfData[turfid][twPosX], TurfData[turfid][twPosY]);

    if(attackGang >= 0) TW_FamilyMsg(attackGang, COLOR_RED,    str);
    if(defendGang >= 0) TW_FamilyMsg(defendGang, COLOR_ORANGE, str);

    TW_UpdateLabel(turfid);
}

// Kết thúc war: đếm quân, trao turf cho bên thắng
static stock TW_EndWar(turfid)
{
    new aGang = TurfData[turfid][twAttackGang];
    new dGang = TurfData[turfid][twDefendGang];

    new aCnt = (aGang >= 0) ? TW_CountNear(aGang, turfid) : 0;
    new dCnt = (dGang >= 0) ? TW_CountNear(dGang, turfid) : 0;

    // Gang đông hơn thắng; nếu bằng → bên phòng thủ (hoặc attack nếu không có defend)
    new winner = (aCnt > dCnt) ? aGang : ((dGang >= 0) ? dGang : aGang);
    TurfData[turfid][twOwnerGang] = winner;

    // Thưởng fame cho bên thắng (hàm FA_AddFame định nghĩa trong assets.inc)
    if(winner >= 0) FA_AddFame(winner, TURF_WIN_FAME);

    new wName[32], str[256];
    if(winner >= 0) strmid(wName, FamilyInfo[winner][FamilyName], 0, 32, 32);
    else format(wName, 32, "Khong ai");

    format(str, sizeof(str),
        "[CHIEM DONG KET THUC] %s chien thang! (Gang 1: %d vs Gang 2: %d) +%d fame",
        wName,
        (winner == aGang) ? aCnt : dCnt,
        (winner == aGang) ? dCnt : aCnt,
        TURF_WIN_FAME);

    if(aGang >= 0) TW_FamilyMsg(aGang, (winner == aGang) ? COLOR_GREEN : COLOR_RED, str);
    if(dGang >= 0 && dGang != aGang) TW_FamilyMsg(dGang, (winner == dGang) ? COLOR_GREEN : COLOR_RED, str);

    // Bắt đầu chu kỳ supply 48h
    TurfData[turfid][twSupplyCount]    = 0;
    TurfData[turfid][twSupplyDeadline] = gettime() + TURF_SUPPLY_DEADLINE;

    // Reset trạng thái war
    TurfData[turfid][twActive]     = 0;
    TurfData[turfid][twTimeLeft]   = 0;
    TurfData[turfid][twAttackGang] = -1;
    TurfData[turfid][twDefendGang] = -1;

    foreach(new i : Player)
    {
        if(g_twInZone[i] == turfid) g_twInZone[i] = -1;
    }

    TW_UpdateLabel(turfid);
    TW_SaveTurf(turfid);
}

// ---------------------------------------------------------------------------
// SUPPLY RUN
// ---------------------------------------------------------------------------

// Bắt đầu countdown 30s lấy hàng (gọi khi player tương tác pickup supply)
static stock TW_StartSupplyCollect(playerid, turfid)
{
    if(g_twSupplyCollecting[playerid]) return;
    if(PlayerInfo[playerid][pFMember] != TurfData[turfid][twOwnerGang]) return;

    // Trừ tiền
    if(GetPlayerCash(playerid) < TURF_SUPPLY_COST)
    {
        SendClientMessageEx(playerid, COLOR_RED, "Ban khong du tien mua hang ($25,000).");
        return;
    }
    GivePlayerCash(playerid, -TURF_SUPPLY_COST);

    g_twSupplyCollecting[playerid]  = true;
    g_twSupplyTurf[playerid]        = turfid;
    g_twSupplyTimeLeft[playerid]    = TURF_SUPPLY_WAIT;
    g_twSupplyCheckTimer[playerid]  = SetTimerEx("TW_SupplyCheck", 1000, true, "i", playerid);

    // Thông báo cops online
    new str[128], zone[MAX_ZONE_NAME];
    GetPlayer2DZone(playerid, zone, MAX_ZONE_NAME);
    format(str, sizeof(str), "[THONG BAO] Co giao dich hang cam tai khu vuc: %s!", zone);
    foreach(new i : Player)
    {
        if(IsACop(i))
            SendClientMessageEx(i, COLOR_RED, str);
    }

    SendClientMessageEx(playerid, COLOR_YELLOW, "Ban dang mua hang cam... Cho 30 giay. Neu bi giet hoac roi khoi diem lay hang cam se mat tien!");
    SetLoading(playerid, TURF_SUPPLY_WAIT * 1000, "Dang lay hang cam...", "TW_OnSupplyLoaded");
}

forward TW_SupplyCheck(playerid);
public TW_SupplyCheck(playerid)
{
    if(!IsPlayerConnected(playerid) || !g_twSupplyCollecting[playerid])
    {
        KillTimer(g_twSupplyCheckTimer[playerid]);
        g_twSupplyCheckTimer[playerid] = -1;
        return;
    }

    new ptIdx = g_twSupplyPtIdx[playerid];
    if(ptIdx >= 0 && ptIdx < g_SupplyPtCount)
    {
        if(!IsPlayerInRangeOfPoint(playerid, 10.0,
            g_SupplyPts[ptIdx][spPosX],
            g_SupplyPts[ptIdx][spPosY],
            g_SupplyPts[ptIdx][spPosZ]))
        {
            KillTimer(g_twSupplyCheckTimer[playerid]);
            g_twSupplyCheckTimer[playerid] = -1;
            g_twSupplyPtIdx[playerid]      = -1;
            StopLoading(playerid);
            g_twSupplyCollecting[playerid] = false;
            GivePlayerCash(playerid, TURF_SUPPLY_COST);
            SendClientMessageEx(playerid, COLOR_RED, "Ban da roi khoi diem lay hang. Qua trinh huy, tien duoc hoan lai!");
            return;
        }
    }

    g_twSupplyTimeLeft[playerid]--;
    new timeStr[48];
    format(timeStr, sizeof(timeStr), "~y~Con lai: ~w~%d giay~n~~r~Khong roi khoi 10m!", g_twSupplyTimeLeft[playerid]);
    GameTextForPlayer(playerid, timeStr, 1200, 3);
}

forward TW_OnSupplyLoaded(playerid);
public TW_OnSupplyLoaded(playerid)
{
    if(!IsPlayerConnected(playerid) || !g_twSupplyCollecting[playerid]) return;

    KillTimer(g_twSupplyCheckTimer[playerid]);
    g_twSupplyCheckTimer[playerid] = -1;
    g_twSupplyPtIdx[playerid]      = -1;

    g_twSupplyCollecting[playerid] = false;
    g_twSupplyCarrying[playerid]   = true;

    new turfid = g_twSupplyTurf[playerid];
    SetPlayerCP(playerid, TurfData[turfid][twPosX], TurfData[turfid][twPosY], TurfData[turfid][twPosZ], 5.0);
    gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;

    SendClientMessageEx(playerid, COLOR_GREEN, "Ban da nhan hang cam! Giao den checkpoint (diem chiem dong).");
}

// Giao hàng thành công khi đến turf
static stock TW_DeliverSupply(playerid)
{
    new turfid = g_twSupplyTurf[playerid];
    g_twSupplyCarrying[playerid] = false;
    g_twSupplyTurf[playerid]     = -1;
    DisablePlayerCheckpoint(playerid);

    GivePlayerCash(playerid, TURF_SUPPLY_REWARD);
    TurfData[turfid][twSupplyCount]++;

    new str[128];
    format(str, sizeof(str), "Giao hang cam thanh cong! +$%d. Tong: %d/%d chuyen hang cam.",
        TURF_SUPPLY_REWARD, TurfData[turfid][twSupplyCount], TURF_SUPPLY_MAX);
    SendClientMessageEx(playerid, COLOR_GREEN, str);

    // Hoàn thành 50 chuyến → đánh dấu chờ nhận thưởng bằng /nhanthuongturf
    if(TurfData[turfid][twSupplyCount] >= TURF_SUPPLY_MAX)
    {
        new owner = TurfData[turfid][twOwnerGang];
        g_twSupplyDone[turfid]             = 1;
        TurfData[turfid][twSupplyDeadline] = 0;

        new gstr[160];
        format(gstr, sizeof(gstr),
            "[HANG CAM HOAN THANH] Gang da giao du %d chuyen hang cam! Rank 4+ dung /nhanthuongturf de nhan qua.",
            TURF_SUPPLY_MAX);
        if(owner >= 0) TW_FamilyMsg(owner, COLOR_GREEN, gstr);
    }

    TW_UpdateLabel(turfid);
    TW_SaveTurf(turfid);
}

// Huỷ supply khi player chết
static stock TW_CancelSupply(playerid)
{
    if(g_twSupplyCollecting[playerid])
    {
        if(g_twSupplyCheckTimer[playerid] != -1)
        {
            KillTimer(g_twSupplyCheckTimer[playerid]);
            g_twSupplyCheckTimer[playerid] = -1;
        }
        g_twSupplyPtIdx[playerid] = -1;
        StopLoading(playerid);
        g_twSupplyCollecting[playerid] = false;
        SendClientMessageEx(playerid, COLOR_RED, "Ban bi giet trong khi lay hang cam. Mat $25,000!");
    }
    if(g_twSupplyCarrying[playerid])
    {
        g_twSupplyCarrying[playerid] = false;
        DisablePlayerCheckpoint(playerid);
        SendClientMessageEx(playerid, COLOR_RED, "Ban bi giet khi mang hang cam. Hang cam mat, khong hoan tien!");
    }
    g_twSupplyTurf[playerid] = -1;
}

// ---------------------------------------------------------------------------
// THÔNG BÁO KILL GIỮA 2 GANG
// ---------------------------------------------------------------------------

// Gọi từ OnPlayerDeath trong callbacks.inc
stock TW_OnPlayerDeath(playerid, killerid, reason)
{
    #pragma unused reason
    TW_CancelSupply(playerid);

    if(!IsPlayerConnected(killerid)) return;

    new kFam = PlayerInfo[killerid][pFMember];
    new vFam = PlayerInfo[playerid][pFMember];

    // Chỉ thông báo khi cả 2 đều có gang và khác gang
    if(kFam == INVALID_FAMILY_ID || vFam == INVALID_FAMILY_ID) return;
    if(kFam == vFam) return;

    new kName[MAX_PLAYER_NAME], vName[MAX_PLAYER_NAME];
    GetPlayerName(killerid, kName, sizeof(kName));
    GetPlayerName(playerid, vName, sizeof(vName));

    new str[192];
    format(str, sizeof(str), "{FFD700}[Tin tuc]{FFFFFF} %s da ha guc %s.", kName, vName);

    // Gửi cho thành viên 2 gang liên quan đang online
    foreach(new i : Player)
    {
        new iFam = PlayerInfo[i][pFMember];
        if(iFam == kFam || iFam == vFam)
            SendClientMessageEx(i, COLOR_WHITE, str);
    }
}

// ---------------------------------------------------------------------------
// PLAYER COMMAND: /layhangcam
// ---------------------------------------------------------------------------

CMD:layhangcam(playerid, params[])
{
    #pragma unused params
    new fam = PlayerInfo[playerid][pFMember];
    if(fam == INVALID_FAMILY_ID)
        return SendClientMessageEx(playerid, COLOR_RED, "Ban chua co gang."), 1;

    if(g_twSupplyCollecting[playerid] || g_twSupplyCarrying[playerid])
        return SendClientMessageEx(playerid, COLOR_RED, "Ban dang trong qua trinh lay/mang hang roi."), 1;

    // Tìm supply point gần nhất
    new Float:px, Float:py, Float:pz;
    GetPlayerPos(playerid, px, py, pz);

    new nearIdx = -1;
    for(new i = 0; i < g_SupplyPtCount; i++)
    {
        new Float:d = floatsqroot(
            (px - g_SupplyPts[i][spPosX]) * (px - g_SupplyPts[i][spPosX]) +
            (py - g_SupplyPts[i][spPosY]) * (py - g_SupplyPts[i][spPosY]) +
            (pz - g_SupplyPts[i][spPosZ]) * (pz - g_SupplyPts[i][spPosZ]));
        if(d <= SUPPLY_PICKUP_RADIUS)
        {
            nearIdx = i;
            break;
        }
    }

    if(nearIdx == -1)
        return SendClientMessageEx(playerid, COLOR_RED, "Ban can dung gan diem lay hang cam de dung lenh nay."), 1;

    // Tìm turf của gang cần supply
    new turfid = -1;
    for(new i = 0; i < TotalTurfs; i++)
    {
        if(TurfData[i][twOwnerGang] == fam &&
           TurfData[i][twSupplyDeadline] > 0 &&
           TurfData[i][twSupplyCount] < TURF_SUPPLY_MAX)
        {
            turfid = i;
            break;
        }
    }

    if(turfid == -1)
        return SendClientMessageEx(playerid, COLOR_YELLOW, "Gang ban hien khong co nhiem vu supply nao."), 1;

    g_twAdminTurf[playerid]    = turfid;
    g_twSupplyPtIdx[playerid]  = nearIdx;
    new str[256];
    format(str, sizeof(str),
        "{ffffff}Ban co muon mua 1 lo hang cam gia ${1FFF35}%d?\n{ffffff}Neu bi giet trong %d giay se mat tien.",
        TURF_SUPPLY_COST, TURF_SUPPLY_WAIT,
        turfid + 1);
    ShowPlayerDialog(playerid, DIALOG_SUPPLY_CONFIRM, DIALOG_STYLE_MSGBOX, "Mua Hang Cam", str, "Mua", "Huy");
    return 1;
}

// ---------------------------------------------------------------------------
// ADMIN COMMAND: /turfadmin
// ---------------------------------------------------------------------------

CMD:turfadmin(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < TURF_ADMIN_LEVEL)
        return SendClientMessageEx(playerid, COLOR_RED, "Ban khong co quyen su dung lenh nay.");

    ShowPlayerDialog(playerid, DIALOG_TURFADMIN_MAIN, DIALOG_STYLE_LIST,
        "Quan Ly Turf Wars",
        "Tao turf moi tai vi tri hien tai\n\
        Xoa turf\n\
        Bat dau war (thu cong)\n\
        Random war (2 gang ngau nhien)\n\
        Lock / Unlock turf\n\
        Quan ly diem lay hang cam",
        "Chon", "Dong");
    return 1;
}

// ---------------------------------------------------------------------------
// PLAYER COMMAND: /diemchiemdong
// ---------------------------------------------------------------------------

static stock TW_ShowTurfList(playerid)
{
    if(TotalTurfs == 0)
        return SendClientMessageEx(playerid, COLOR_RED, "Chua co diem chiem dong nao.");

    new fam = PlayerInfo[playerid][pFMember];
    new list[4096]; list[0] = 0;

    strcat(list, "Dia ban\tGang 1\tGang 2\tTinh trang\n", sizeof(list));

    for(new i = 0; i < TotalTurfs; i++)
    {
        new zone[MAX_ZONE_NAME];
        Get3DZone(TurfData[i][twPosX], TurfData[i][twPosY], TurfData[i][twPosZ], zone, sizeof(zone));

        new gang1Name[32], gang2Name[32];
        new bool:playerInvolved = false;
        new aGang = TurfData[i][twAttackGang];
        new dGang = TurfData[i][twDefendGang];
        new ownerIdx = TurfData[i][twOwnerGang];

        if(TurfData[i][twActive])
        {
            if(aGang >= 0 && aGang < MAX_FAMILY) strmid(gang1Name, FamilyInfo[aGang][FamilyName], 0, 32, 32);
            else format(gang1Name, 32, "---");
            if(dGang >= 0 && dGang < MAX_FAMILY) strmid(gang2Name, FamilyInfo[dGang][FamilyName], 0, 32, 32);
            else format(gang2Name, 32, "---");
            playerInvolved = (fam >= 0 && (fam == aGang || fam == dGang));
        }
        else
        {
            if(ownerIdx >= 0 && ownerIdx < MAX_FAMILY) strmid(gang1Name, FamilyInfo[ownerIdx][FamilyName], 0, 32, 32);
            else format(gang1Name, 32, "Trong");
            format(gang2Name, 32, "---");
            playerInvolved = (fam >= 0 && fam == ownerIdx);
        }

        new status[56];
        if(TurfData[i][twActive])
            format(status, sizeof(status), playerInvolved ? "{FFFF00}CHIEN (CUA BAN)" : "{FF4444}CHIEN");
        else if(TurfData[i][twLocked])
            format(status, sizeof(status), "{888888}DONG");
        else if(TurfData[i][twSupplyDeadline] > 0 && TurfData[i][twSupplyCount] < TURF_SUPPLY_MAX)
            format(status, sizeof(status), "{00BFFF}HANG CAM");
        else
            format(status, sizeof(status), "{44FF44}MO");

        new line[256];
        if(playerInvolved)
            format(line, sizeof(line), "{00FF00}%s\t{FFFF00}%s\t{FFFF00}%s\t%s\n", zone, gang1Name, gang2Name, status);
        else
            format(line, sizeof(line), "%s\t{FFFF00}%s\t{FFFF00}%s\t%s\n", zone, gang1Name, gang2Name, status);
        strcat(list, line, sizeof(list));
    }

    ShowPlayerDialog(playerid, DIALOG_TURF_LIST, DIALOG_STYLE_TABLIST_HEADERS,
        "> Diem chiem dong", list, "Xem chi tiet", "Dong");
}

CMD:diemchiemdong(playerid, params[])
{
    #pragma unused params
    TW_ShowTurfList(playerid);
    return 1;
}

// ---------------------------------------------------------------------------
// PLAYER COMMAND: /nhanthuongturf
// ---------------------------------------------------------------------------

CMD:nhanthuongturf(playerid, params[])
{
    #pragma unused params
    new fam = PlayerInfo[playerid][pFMember];
    if(fam == INVALID_FAMILY_ID)
        return SendClientMessageEx(playerid, COLOR_RED, "Ban chua co gang."), 1;

    if(PlayerInfo[playerid][pRank] < 4)
        return SendClientMessageEx(playerid, COLOR_RED, "Ban can rank 4 tro len trong gang de nhan thuong."), 1;

    new turfid = -1;
    for(new i = 0; i < TotalTurfs; i++)
    {
        if(TurfData[i][twOwnerGang] == fam && g_twSupplyDone[i])
        {
            turfid = i;
            break;
        }
    }

    if(turfid == -1)
        return SendClientMessageEx(playerid, COLOR_YELLOW, "Gang ban hien khong co phan thuong supply nao cho nhan."), 1;

    GivePlayerCash(playerid, TURF_SUPPLY_DONE_BANK);
    FA_AddFame(fam, TURF_SUPPLY_DONE_FAME);
    PlayerInfo[playerid][pPot]   += 50;
    PlayerInfo[playerid][pCrack] += 30;
    PlayerInfo[playerid][pMats]  += 5000;

    g_twSupplyDone[turfid]             = 0;
    TurfData[turfid][twSupplyCount]    = 0;
    TurfData[turfid][twSupplyDeadline] = 0;
    TW_SaveTurf(turfid);
    TW_UpdateLabel(turfid);

    new str[192];
    format(str, sizeof(str),
        "[PHAN THUONG] %s da nhan thuong giao hang cam diem chiem dong #%d: +$%d tien mat, +%d fame, +50 Pot/+30 Crack/+5000 Vat lieu (/tuido).",
        GetPlayerNameEx(playerid), turfid + 1, TURF_SUPPLY_DONE_BANK, TURF_SUPPLY_DONE_FAME);
    TW_FamilyMsg(fam, COLOR_LIGHTGREEN, str);

    SendClientMessageEx(playerid, TEAM_CYAN_COLOR,
        "* Phan thuong supply da duoc chuyen truc tiep vao nguoi ban.");
    return 1;
}

// ---------------------------------------------------------------------------
// CMD: /testturf (admin debug)
// ---------------------------------------------------------------------------

CMD:testturf(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < TURF_ADMIN_LEVEL)
        return SendClientMessageEx(playerid, COLOR_RED, "Ban khong co quyen su dung lenh nay."), 1;

    // Tách sub-command bằng tay (không dùng sscanf "s[]" vì greedy trong codebase này)
    // Sau đó sscanf chỉ dùng cho phần số, bắt đầu từ params[argsStart]
    new sub[16], subEnd;
    while(params[subEnd] && params[subEnd] != ' ' && subEnd < 15)
    {
        sub[subEnd] = params[subEnd];
        subEnd++;
    }
    sub[subEnd] = '\0';
    while(params[subEnd] == ' ') subEnd++; // bỏ qua khoảng trắng giữa sub và args

    // Helper macro: turf ID nguoi dung nhap la 1-based, array la 0-based
    // Gang ID la 0-based index truc tiep vao FamilyInfo[]

    // /testturf war [turfid] [gangid_attack] [gangid_defend]
    if(!strcmp(sub, "war", true))
    {
        new tid, g1, g2;
        if(sscanf(params[subEnd], "iii", tid, g1, g2))
            return SendClientMessageEx(playerid, COLOR_RED, "Dung: /testturf war [turfid 1+] [gangid_attack] [gangid_defend]"), 1;
        tid--;
        if(tid < 0 || tid >= TotalTurfs)
            return SendClientMessageEx(playerid, COLOR_RED, "turfid khong hop le (nhap tu 1)."), 1;
        if(g1 < 0 || g1 >= MAX_FAMILY || !FamilyInfo[g1][FamilyTaken])
            return SendClientMessageEx(playerid, COLOR_RED, "gangid_attack khong hop le."), 1;
        if(g2 < -1 || (g2 >= 0 && (g2 >= MAX_FAMILY || !FamilyInfo[g2][FamilyTaken])))
            return SendClientMessageEx(playerid, COLOR_RED, "gangid_defend khong hop le (-1 = khong co chu)."), 1;
        TW_StartWar(tid, g1, g2);
        new str[96], g1name[42], g2name[42];
        if(g1 >= 0) format(g1name, sizeof(g1name), "%s", FamilyInfo[g1][FamilyName]);
        else format(g1name, sizeof(g1name), "???");
        if(g2 >= 0) format(g2name, sizeof(g2name), "%s", FamilyInfo[g2][FamilyName]);
        else format(g2name, sizeof(g2name), "Trong");
        format(str, sizeof(str), "[TEST] War turf #%d | Attack: %s (id:%d) vs Defend: %s (id:%d).",
            tid+1, g1name, g1, g2name, g2);
        SendClientMessageEx(playerid, COLOR_GREEN, str);
        return 1;
    }

    // /testturf endwar [turfid]
    if(!strcmp(sub, "endwar", true))
    {
        new tid;
        if(sscanf(params[subEnd], "i", tid))
            return SendClientMessageEx(playerid, COLOR_RED, "Dung: /testturf endwar [turfid 1+]"), 1;
        tid--;
        if(tid < 0 || tid >= TotalTurfs || !TurfData[tid][twActive])
            return SendClientMessageEx(playerid, COLOR_RED, "Turf khong hop le hoac khong dang war."), 1;
        TurfData[tid][twTimeLeft] = 0;
        new str[64];
        format(str, sizeof(str), "[TEST] Ket thuc war turf #%d ngay lap tuc.", tid+1);
        SendClientMessageEx(playerid, COLOR_GREEN, str);
        return 1;
    }

    // /testturf setowner [turfid] [gangid] (gangid = -1 de xoa chu)
    if(!strcmp(sub, "setowner", true))
    {
        new tid, gid;
        if(sscanf(params[subEnd], "ii", tid, gid))
            return SendClientMessageEx(playerid, COLOR_RED, "Dung: /testturf setowner [turfid 1+] [gangid | -1]"), 1;
        tid--;
        if(tid < 0 || tid >= TotalTurfs)
            return SendClientMessageEx(playerid, COLOR_RED, "turfid khong hop le (nhap tu 1)."), 1;
        if(gid != -1 && (gid < 0 || gid >= MAX_FAMILY || !FamilyInfo[gid][FamilyTaken]))
            return SendClientMessageEx(playerid, COLOR_RED, "gangid khong hop le (-1 = xoa chu so huu)."), 1;
        TurfData[tid][twOwnerGang] = gid;
        TW_SaveTurf(tid);
        TW_UpdateLabel(tid);
        new str[80], gname[42];
        if(gid >= 0) format(gname, sizeof(gname), "%s", FamilyInfo[gid][FamilyName]);
        else format(gname, sizeof(gname), "Khong ai");
        format(str, sizeof(str), "[TEST] Turf #%d -> chu: %s (id:%d).", tid+1, gname, gid);
        SendClientMessageEx(playerid, COLOR_GREEN, str);
        return 1;
    }

    // /testturf supply [turfid]
    if(!strcmp(sub, "supply", true))
    {
        new tid;
        if(sscanf(params[subEnd], "i", tid))
            return SendClientMessageEx(playerid, COLOR_RED, "Dung: /testturf supply [turfid 1+]"), 1;
        tid--;
        if(tid < 0 || tid >= TotalTurfs)
            return SendClientMessageEx(playerid, COLOR_RED, "turfid khong hop le (nhap tu 1)."), 1;
        TurfData[tid][twSupplyCount]    = 0;
        TurfData[tid][twSupplyDeadline] = gettime() + TURF_SUPPLY_DEADLINE;
        g_twSupplyDone[tid]             = 0;
        TW_SaveTurf(tid);
        TW_UpdateLabel(tid);
        new str[96];
        format(str, sizeof(str), "[TEST] Bat dau supply cho turf #%d (0/%d chuyen, han 48h).", tid+1, TURF_SUPPLY_MAX);
        SendClientMessageEx(playerid, COLOR_GREEN, str);
        return 1;
    }

    // /testturf addsupply [turfid] [amount]
    if(!strcmp(sub, "addsupply", true))
    {
        new tid, amount;
        if(sscanf(params[subEnd], "ii", tid, amount))
            return SendClientMessageEx(playerid, COLOR_RED, "Dung: /testturf addsupply [turfid 1+] [amount]"), 1;
        tid--;
        if(tid < 0 || tid >= TotalTurfs)
            return SendClientMessageEx(playerid, COLOR_RED, "turfid khong hop le (nhap tu 1)."), 1;
        if(TurfData[tid][twSupplyDeadline] == 0 && !g_twSupplyDone[tid])
            return SendClientMessageEx(playerid, COLOR_RED, "Turf nay chua co supply dang chay. Dung /testturf supply [turfid] truoc."), 1;

        TurfData[tid][twSupplyCount] += amount;
        if(TurfData[tid][twSupplyCount] > TURF_SUPPLY_MAX)
            TurfData[tid][twSupplyCount] = TURF_SUPPLY_MAX;

        new str[128];
        if(TurfData[tid][twSupplyCount] >= TURF_SUPPLY_MAX)
        {
            g_twSupplyDone[tid]             = 1;
            TurfData[tid][twSupplyDeadline] = 0;
            format(str, sizeof(str), "[TEST] Supply turf #%d da dat %d/%d - phan thuong san sang!", tid+1, TurfData[tid][twSupplyCount], TURF_SUPPLY_MAX);
        }
        else
        {
            format(str, sizeof(str), "[TEST] Supply turf #%d tang len %d/%d.", tid+1, TurfData[tid][twSupplyCount], TURF_SUPPLY_MAX);
        }
        TW_SaveTurf(tid);
        TW_UpdateLabel(tid);
        SendClientMessageEx(playerid, COLOR_GREEN, str);
        return 1;
    }

    // /testturf expiry [turfid]
    if(!strcmp(sub, "expiry", true))
    {
        new tid;
        if(sscanf(params[subEnd], "i", tid))
            return SendClientMessageEx(playerid, COLOR_RED, "Dung: /testturf expiry [turfid 1+]"), 1;
        tid--;
        if(tid < 0 || tid >= TotalTurfs)
            return SendClientMessageEx(playerid, COLOR_RED, "turfid khong hop le (nhap tu 1)."), 1;
        if(TurfData[tid][twSupplyDeadline] == 0 && !g_twSupplyDone[tid])
            return SendClientMessageEx(playerid, COLOR_RED, "Turf nay chua co supply dang chay."), 1;

        new owner = TurfData[tid][twOwnerGang];
        if(owner >= 0) FA_AddFame(owner, -TURF_SUPPLY_FAIL_FAME);

        new fstr[96];
        format(fstr, sizeof(fstr),
            "[HANG CAM] Gang that bai! Chi giao %d/%d chuyen hang cam trong 48h. -%d fame!",
            TurfData[tid][twSupplyCount], TURF_SUPPLY_MAX, TURF_SUPPLY_FAIL_FAME);
        if(owner >= 0) { TW_FamilyMsg(owner, COLOR_RED, fstr); SaveFamily(owner); }

        TurfData[tid][twSupplyCount]    = 0;
        TurfData[tid][twSupplyDeadline] = 0;
        TW_SaveTurf(tid);
        TW_UpdateLabel(tid);
        new str[64];
        format(str, sizeof(str), "[TEST] Het han supply turf #%d. Fame tru, supply reset.", tid+1);
        SendClientMessageEx(playerid, COLOR_YELLOW, str);
        return 1;
    }

    // /testturf info [turfid]
    if(!strcmp(sub, "info", true))
    {
        new tid;
        if(sscanf(params[subEnd], "i", tid))
            return SendClientMessageEx(playerid, COLOR_RED, "Dung: /testturf info [turfid 1+]"), 1;
        tid--;
        if(tid < 0 || tid >= TotalTurfs)
            return SendClientMessageEx(playerid, COLOR_RED, "turfid khong hop le (nhap tu 1)."), 1;
        new str[256];
        format(str, sizeof(str),
            "[TEST] Turf #%d | Owner:%d | Active:%d | Attack:%d | Defend:%d | Supply:%d/%d | Done:%d | Locked:%d",
            tid+1,
            TurfData[tid][twOwnerGang], TurfData[tid][twActive],
            TurfData[tid][twAttackGang], TurfData[tid][twDefendGang],
            TurfData[tid][twSupplyCount], TURF_SUPPLY_MAX,
            g_twSupplyDone[tid], TurfData[tid][twLocked]);
        SendClientMessageEx(playerid, COLOR_YELLOW, str);
        return 1;
    }

    // Help
    SendClientMessageEx(playerid, COLOR_WHITE, "--- /testturf ---");
    SendClientMessageEx(playerid, COLOR_WHITE, "/testturf war [turfid] [attack] [defend]  - Bat dau war thu cong");
    SendClientMessageEx(playerid, COLOR_WHITE, "/testturf endwar [turfid]                 - Ket thuc war ngay lap tuc");
    SendClientMessageEx(playerid, COLOR_WHITE, "/testturf setowner [turfid] [gangid]      - Set chu so huu turf");
    SendClientMessageEx(playerid, COLOR_WHITE, "/testturf supply [turfid]                 - Bat dau chu ky supply (reset ve 0)");
    SendClientMessageEx(playerid, COLOR_WHITE, "/testturf addsupply [turfid] [amount]     - Tang supply count len them X chuyen");
    SendClientMessageEx(playerid, COLOR_WHITE, "/testturf expiry [turfid]                 - Gia lap het han supply ngay lap tuc");
    SendClientMessageEx(playerid, COLOR_WHITE, "/testturf info [turfid]                   - Xem trang thai raw cua turf");
    return 1;
}

// ---------------------------------------------------------------------------
// DIALOG RESPONSE (gọi từ OnDialogResponse trong callbacks.inc)
// ---------------------------------------------------------------------------

// Trả về 1 nếu đã xử lý, 0 nếu không phải dialog của module này
stock TW_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    #pragma unused inputtext
    switch(dialogid)
    {
        // --- Main admin menu ---
        case DIALOG_TURFADMIN_MAIN:
        {
            if(!response) return 1;
            switch(listitem)
            {
                case 0: // Tạo turf mới
                {
                    new Float:px, Float:py, Float:pz;
                    GetPlayerPos(playerid, px, py, pz);
                    if(TotalTurfs >= MAX_TURFS)
                        return SendClientMessageEx(playerid, COLOR_RED, "Da dat gioi han turf toi da."), 1;

                    new q[256];
                    mysql_format(MainPipeline, q, sizeof(q),
                        "INSERT INTO `turfs` (`pos_x`,`pos_y`,`pos_z`) VALUES (%f,%f,%f)",
                        px, py, pz);
                    mysql_tquery(MainPipeline, q, "TW_OnTurfCreated", "i", playerid);
                }
                case 1: // Xóa turf
                {
                    if(TotalTurfs == 0)
                        return SendClientMessageEx(playerid, COLOR_RED, "Khong co turf nao."), 1;
                    new list[1024]; list[0] = 0;
                    for(new i = 0; i < TotalTurfs; i++)
                    {
                        new line[64], ownerName[32];
                        new ownerIdx = TurfData[i][twOwnerGang];
                        if(ownerIdx >= 0) strmid(ownerName, FamilyInfo[ownerIdx][FamilyName], 0, 32, 32);
                        else format(ownerName, 32, "Chua ai");
                        format(line, sizeof(line), "Turf #%d (%.0f, %.0f) Chu: %s\n",
                            i+1, TurfData[i][twPosX], TurfData[i][twPosY], ownerName);
                        strcat(list, line, sizeof(list));
                    }
                    ShowPlayerDialog(playerid, DIALOG_TURFADMIN_DELETE, DIALOG_STYLE_LIST, "Xoa Turf", list, "Xoa", "Huy");
                }
                case 2: // War thủ công
                {
                    if(TotalTurfs == 0)
                        return SendClientMessageEx(playerid, COLOR_RED, "Khong co turf nao."), 1;
                    new list[1024]; list[0] = 0;
                    for(new i = 0; i < TotalTurfs; i++)
                    {
                        new line[64];
                        format(line, sizeof(line), "Turf #%d (%.0f, %.0f) Lock:%d\n",
                            i+1, TurfData[i][twPosX], TurfData[i][twPosY], TurfData[i][twLocked]);
                        strcat(list, line, sizeof(list));
                    }
                    ShowPlayerDialog(playerid, DIALOG_TURFADMIN_WARSEL, DIALOG_STYLE_LIST, "Chon Turf War", list, "Chon", "Huy");
                }
                case 3: // Random war
                {
                    TW_TryRandomWar(playerid);
                }
                case 4: // Lock/Unlock
                {
                    new list[1024]; list[0] = 0;
                    for(new i = 0; i < TotalTurfs; i++)
                    {
                        new line[72];
                        format(line, sizeof(line), "Turf #%d (%.0f, %.0f) [%s]\n",
                            i+1, TurfData[i][twPosX], TurfData[i][twPosY],
                            TurfData[i][twLocked] ? "LOCKED" : "Binh thuong");
                        strcat(list, line, sizeof(list));
                    }
                    ShowPlayerDialog(playerid, DIALOG_TURFADMIN_LOCKLIST, DIALOG_STYLE_LIST, "Lock/Unlock Turf", list, "Doi trang thai", "Huy");
                }
                case 5: // Supply points
                {
                    new list[512]; list[0] = 0;
                    strcat(list, "[+] Them diem tai vi tri hien tai\n", sizeof(list));
                    for(new i = 0; i < g_SupplyPtCount; i++)
                    {
                        new line[72];
                        format(line, sizeof(line), "Diem #%d  (%.0f, %.0f, %.0f)\n",
                            i+1, g_SupplyPts[i][spPosX], g_SupplyPts[i][spPosY], g_SupplyPts[i][spPosZ]);
                        strcat(list, line, sizeof(list));
                    }
                    if(g_SupplyPtCount == 0) strcat(list, "(Chua co diem nao)\n", sizeof(list));
                    ShowPlayerDialog(playerid, DIALOG_SUPPLY_ADMIN_LIST, DIALOG_STYLE_LIST, "Diem Lay Hang Cam", list, "Chon", "Huy");
                }
            }
            return 1;
        }

        case DIALOG_TURFADMIN_DELETE:
        {
            if(!response) return 1;
            if(listitem < 0 || listitem >= TotalTurfs) return 1;

            new q[64];
            mysql_format(MainPipeline, q, sizeof(q), "DELETE FROM `turfs` WHERE `id`=%d", TurfData[listitem][twId]);
            mysql_tquery(MainPipeline, q, "", "");

            if(TurfData[listitem][twPickupId] != -1)
                DestroyDynamicPickup(TurfData[listitem][twPickupId]);

            // Xóa label
            if(g_twLabel[listitem] != -1)
            {
                DestroyDynamic3DTextLabel(Text3D:g_twLabel[listitem]);
                g_twLabel[listitem] = -1;
            }

            // Dịch mảng lên
            for(new i = listitem; i < TotalTurfs - 1; i++)
            {
                TurfData[i]    = TurfData[i+1];
                g_twLabel[i]   = g_twLabel[i+1];
            }
            g_twLabel[TotalTurfs - 1] = -1;
            TotalTurfs--;

            SendClientMessageEx(playerid, COLOR_GREEN, "Da xoa turf thanh cong.");
            return 1;
        }

        case DIALOG_TURFADMIN_WARSEL: // Chọn turf để war thủ công
        {
            if(!response) return 1;
            if(listitem < 0 || listitem >= TotalTurfs) return 1;
            g_twAdminTurf[playerid] = listitem;

            // Chọn gang tấn công
            new list[512]; list[0] = 0;
            for(new i = 0; i < MAX_FAMILY; i++)
            {
                if(!FamilyInfo[i][FamilyTaken]) continue;
                new line[48];
                format(line, sizeof(line), "%s\n", FamilyInfo[i][FamilyName]);
                strcat(list, line, sizeof(list));
            }
            ShowPlayerDialog(playerid, DIALOG_TURFADMIN_SELGANG1, DIALOG_STYLE_LIST, "Chon Gang 1", list, "Chon", "Huy");
            return 1;
        }

        case DIALOG_TURFADMIN_SELGANG1:
        {
            if(!response) return 1;
            // Ánh xạ listitem → family ID thực
            new cnt = -1, famid = -1;
            for(new i = 0; i < MAX_FAMILY; i++)
            {
                if(!FamilyInfo[i][FamilyTaken]) continue;
                cnt++;
                if(cnt == listitem) { famid = i; break; }
            }
            if(famid == -1) return 1;
            g_twAdminGang1[playerid] = famid;

            new list[512]; list[0] = 0;
            for(new i = 0; i < MAX_FAMILY; i++)
            {
                if(!FamilyInfo[i][FamilyTaken] || i == famid) continue;
                new line[48];
                format(line, sizeof(line), "%s\n", FamilyInfo[i][FamilyName]);
                strcat(list, line, sizeof(list));
            }
            ShowPlayerDialog(playerid, DIALOG_TURFADMIN_SELGANG2, DIALOG_STYLE_LIST, "Chon Gang 2", list, "Chon", "Huy");
            return 1;
        }

        case DIALOG_TURFADMIN_SELGANG2:
        {
            if(!response) return 1;
            new cnt = -1, famid = -1, aGang = g_twAdminGang1[playerid];
            for(new i = 0; i < MAX_FAMILY; i++)
            {
                if(!FamilyInfo[i][FamilyTaken] || i == aGang) continue;
                cnt++;
                if(cnt == listitem) { famid = i; break; }
            }
            if(famid == -1) return 1;

            TW_StartWar(g_twAdminTurf[playerid], aGang, famid);
            new str[128];
            format(str, sizeof(str), "Da bat dau war giua %s va %s tai turf #%d.",
                FamilyInfo[aGang][FamilyName], FamilyInfo[famid][FamilyName], g_twAdminTurf[playerid]+1);
            SendClientMessageEx(playerid, COLOR_GREEN, str);
            return 1;
        }

        case DIALOG_TURFADMIN_LOCKLIST:
        {
            if(!response) return 1;
            if(listitem < 0 || listitem >= TotalTurfs) return 1;
            TurfData[listitem][twLocked] = TurfData[listitem][twLocked] ? 0 : 1;
            TW_SaveTurf(listitem);
            TW_UpdateLabel(listitem);
            new str[64];
            format(str, sizeof(str), "Turf #%d da duoc %s.",
                listitem+1, TurfData[listitem][twLocked] ? "LOCK" : "UNLOCK");
            SendClientMessageEx(playerid, COLOR_GREEN, str);
            return 1;
        }

        // ---- Danh sách tất cả turf ----
        case DIALOG_TURF_LIST:
        {
            if(!response) return 1;
            new turfid = listitem;
            if(turfid >= TotalTurfs) return 1;
            g_twAdminTurf[playerid] = turfid;

            // Khu vuc
            new zone[MAX_ZONE_NAME];
            Get3DZone(TurfData[turfid][twPosX], TurfData[turfid][twPosY], TurfData[turfid][twPosZ], zone, sizeof(zone));

            // Chu so huu
            new ownerName[32];
            new ownerIdx = TurfData[turfid][twOwnerGang];
            if(ownerIdx >= 0 && ownerIdx < MAX_FAMILY)
                strmid(ownerName, FamilyInfo[ownerIdx][FamilyName], 0, 32, 32);
            else format(ownerName, 32, "_");

            // Gang 1 / Gang 2
            new gang1Name[32], gang2Name[32];
            new aGang = TurfData[turfid][twAttackGang];
            new dGang = TurfData[turfid][twDefendGang];
            if(aGang >= 0 && aGang < MAX_FAMILY) strmid(gang1Name, FamilyInfo[aGang][FamilyName], 0, 32, 32);
            else format(gang1Name, 32, "_");
            if(dGang >= 0 && dGang < MAX_FAMILY) strmid(gang2Name, FamilyInfo[dGang][FamilyName], 0, 32, 32);
            else format(gang2Name, 32, "_");

            // Trang thai
            new stateStr[48];
            if(TurfData[turfid][twActive])
                format(stateStr, sizeof(stateStr), "{FF4444}DANG CHIEM DONG");
            else if(TurfData[turfid][twLocked])
                format(stateStr, sizeof(stateStr), "{888888}DANG BI KHOA");
            else if(TurfData[turfid][twSupplyDeadline] > 0 && TurfData[turfid][twSupplyCount] < TURF_SUPPLY_MAX)
                format(stateStr, sizeof(stateStr), "{00BFFF}DANG GIAO HANG CAM");
            else if(g_twSupplyDone[turfid])
                format(stateStr, sizeof(stateStr), "{FF8C00}PHAN THUONG SAN SANG");
            else
                format(stateStr, sizeof(stateStr), "{44FF44}Binh thuong");

            // Thoi gian con lai
            new timeStr[24];
            if(TurfData[turfid][twActive])
            {
                new mins = TurfData[turfid][twTimeLeft] / 60;
                new secs = TurfData[turfid][twTimeLeft] % 60;
                format(timeStr, sizeof(timeStr), "{FFFF00}%02d:%02d", mins, secs);
            }
            else format(timeStr, sizeof(timeStr), "_");

            // Hang cam tien do
            new supplyStr[56], deadlineStr[24];
            if(g_twSupplyDone[turfid])
            {
                format(supplyStr, sizeof(supplyStr), "{44FF44}%d/%d - HOAN THANH", TURF_SUPPLY_MAX, TURF_SUPPLY_MAX);
                format(deadlineStr, sizeof(deadlineStr), "_");
            }
            else if(TurfData[turfid][twSupplyDeadline] > 0 && TurfData[turfid][twSupplyCount] < TURF_SUPPLY_MAX)
            {
                format(supplyStr, sizeof(supplyStr), "{FFFF00}%d{FFFFFF}/%d chuyen", TurfData[turfid][twSupplyCount], TURF_SUPPLY_MAX);
                new hoursLeft = (TurfData[turfid][twSupplyDeadline] - gettime()) / 3600;
                if(hoursLeft < 0) hoursLeft = 0;
                format(deadlineStr, sizeof(deadlineStr), "{FF8800}%dh", hoursLeft);
            }
            else
            {
                format(supplyStr, sizeof(supplyStr), "_");
                format(deadlineStr, sizeof(deadlineStr), "_");
            }

            new str[768];
            format(str, sizeof(str),
                "{FFD700}===== DIEM CHIEM DONG #%d =====\n\n\
{AAAAAA}   Khu vuc:     {FFFFFF}%s\n\
{AAAAAA}   Chu so huu:  {FFFFFF}%s\n\
{AAAAAA}   Gang 1:      {FFFF00}%s\n\
{AAAAAA}   Gang 2:      {FFFF00}%s\n\
{AAAAAA}   Trang thai:  %s\n\
{AAAAAA}   Thoi gian:   %s\n\
{AAAAAA}   Hang cam:    %s\n\
{AAAAAA}   Het han:     %s",
                turfid + 1, zone, ownerName, gang1Name, gang2Name,
                stateStr, timeStr, supplyStr, deadlineStr);

            ShowPlayerDialog(playerid, DIALOG_DIEMCHIEMDONG, DIALOG_STYLE_MSGBOX,
                "Chi Tiet Diem Chiem Dong", str, "Dieu Huong", "Quay Lai");
            return 1;
        }

        case DIALOG_DIEMCHIEMDONG:
        {
            new turfid = g_twAdminTurf[playerid];
            if(!response)
            {
                // Quay lại danh sách
                TW_ShowTurfList(playerid);
                return 1;
            }
            if(turfid < 0 || turfid >= TotalTurfs) return 1;
            SetPlayerCP(playerid, TurfData[turfid][twPosX], TurfData[turfid][twPosY], TurfData[turfid][twPosZ], 10.0);
            gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
            SendClientMessageEx(playerid, COLOR_YELLOW, "Da dat checkpoint den vi tri turf.");
            return 1;
        }

        case DIALOG_SUPPLY_CONFIRM:
        {
            if(!response) return 1;
            new turfid = g_twAdminTurf[playerid];
            if(turfid < 0 || turfid >= TotalTurfs) return 1;
            TW_StartSupplyCollect(playerid, turfid);
            return 1;
        }

        // ---- Supply point admin list ----
        case DIALOG_SUPPLY_ADMIN_LIST:
        {
            if(!response) return 1;
            if(listitem == 0) // Thêm điểm mới
            {
                new Float:px, Float:py, Float:pz;
                GetPlayerPos(playerid, px, py, pz);
                TW_AddSupplyPt(playerid, px, py, pz);
            }
            else // Chọn điểm để xóa (offset 1)
            {
                new idx = listitem - 1;
                if(idx < 0 || idx >= g_SupplyPtCount) return 1;
                g_twAdminSupplyPt[playerid] = idx;
                new str[96];
                format(str, sizeof(str), "Xoa diem lay hang cam #%d tai (%.0f, %.0f, %.0f)?",
                    idx+1, g_SupplyPts[idx][spPosX], g_SupplyPts[idx][spPosY], g_SupplyPts[idx][spPosZ]);
                ShowPlayerDialog(playerid, DIALOG_SUPPLY_ADMIN_DEL, DIALOG_STYLE_MSGBOX,
                    "Xac Nhan Xoa", str, "Xoa", "Huy");
            }
            return 1;
        }

        case DIALOG_SUPPLY_ADMIN_DEL:
        {
            if(!response) return 1;
            new idx = g_twAdminSupplyPt[playerid];
            if(idx < 0 || idx >= g_SupplyPtCount) return 1;
            TW_RemoveSupplyPt(idx);
            g_twAdminSupplyPt[playerid] = -1;
            SendClientMessageEx(playerid, COLOR_GREEN, "Da xoa diem lay hang cam.");
            return 1;
        }
    }
    return 0; // Không phải dialog của module này
}

// Callback khi tạo turf mới thành công trong DB
forward TW_OnTurfCreated(playerid);
public TW_OnTurfCreated(playerid)
{
    new newId = cache_insert_id();
    new Float:px, Float:py, Float:pz;
    GetPlayerPos(playerid, px, py, pz);

    new idx = TotalTurfs;
    TurfData[idx][twId]          = newId;
    TurfData[idx][twPosX]        = px;
    TurfData[idx][twPosY]        = py;
    TurfData[idx][twPosZ]        = pz;
    TurfData[idx][twOwnerGang]   = -1;
    TurfData[idx][twActive]      = 0;
    TurfData[idx][twAttackGang]  = -1;
    TurfData[idx][twDefendGang]  = -1;
    TurfData[idx][twSupplyCount] = 0;
    TurfData[idx][twSupplyDeadline] = 0;
    TurfData[idx][twLocked]      = 0;
    TurfData[idx][twPickupId]    = CreateDynamicPickup(1239, 23, px, py, pz, -1);
    TotalTurfs++;
    TW_UpdateLabel(idx);

    new str[96];
    format(str, sizeof(str), "Da tao turf moi #%d tai (%.1f, %.1f, %.1f). Tong: %d turf.", newId, px, py, pz, TotalTurfs);
    SendClientMessageEx(playerid, COLOR_GREEN, str);
}

// Thực hiện random war: tìm 2 gang có đủ thành viên online
static stock TW_TryRandomWar(adminid)
{
    // Thu thập danh sách gang có ít nhất 1 thành viên (không cần online)
    new eligible[MAX_FAMILY], eCount = 0;
    for(new i = 0; i < MAX_FAMILY; i++)
    {
        if(FamilyInfo[i][FamilyTaken] && FamilyInfo[i][FamilyMembers] > 0)
            eligible[eCount++] = i;
    }

    if(eCount < 1)
    {
        SendClientMessageEx(adminid, COLOR_RED, "Khong co gang nao co thanh vien de random war.");
        return 0;
    }

    // Chọn gang 1 ngẫu nhiên
    new g1idx = random(eCount);
    new gang1 = eligible[g1idx];

    // Lọc điểm chiếm đóng không bị khóa, không đang war, và không thuộc gang1
    new candidateTurfs[MAX_TURFS], cCount = 0;
    for(new i = 0; i < TotalTurfs; i++)
    {
        if(!TurfData[i][twActive] && !TurfData[i][twLocked] && TurfData[i][twOwnerGang] != gang1)
            candidateTurfs[cCount++] = i;
    }

    if(cCount == 0)
    {
        SendClientMessageEx(adminid, COLOR_RED, "Khong co diem chiem dong kha dung (tat ca deu thuoc gang duoc chon hoac dang war).");
        return 0;
    }

    new turfid = candidateTurfs[random(cCount)];

    // Gang 2: chủ sở hữu điểm nếu có trong eligible và khác gang1
    // Nếu không → gang2 = -1 (điểm trống, gang1 chỉ cần đứng hết giờ là thắng)
    new turfOwner = TurfData[turfid][twOwnerGang];
    new gang2 = -1;

    if(turfOwner >= 0 && turfOwner != gang1)
    {
        for(new j = 0; j < eCount; j++)
        {
            if(eligible[j] == turfOwner) { gang2 = turfOwner; break; }
        }
    }

    TW_StartWar(turfid, gang1, gang2);
    SendClientMessageEx(adminid, COLOR_GREEN, "Da bat dau random war thanh cong!");
    return 1;
}

// ---------------------------------------------------------------------------
// PICKUP & CHECKPOINT HANDLERS (gọi từ callbacks.inc)
// ---------------------------------------------------------------------------

// Gọi từ OnPlayerPickUpPickup
stock TW_OnPlayerPickUpPickup(playerid, pickupid)
{
    // Kiểm tra pickup có phải điểm lấy hàng cấm không
    for(new i = 0; i < g_SupplyPtCount; i++)
    {
        if(pickupid == g_SupplyPts[i][spPickupId])
        {
            SendClientMessageEx(playerid, COLOR_YELLOW, "Dung /layhangcam tai day de bat dau thu gom hang cam.");
            return 1;
        }
    }
    return 0;
}

// Gọi từ OnPlayerEnterCheckpoint
stock TW_OnPlayerEnterCheckpoint(playerid)
{
    if(!g_twSupplyCarrying[playerid]) return 0;

    new turfid = g_twSupplyTurf[playerid];
    if(turfid < 0 || turfid >= TotalTurfs) return 0;

    // Kiểm tra gần turf đích
    new Float:px, Float:py, Float:pz;
    GetPlayerPos(playerid, px, py, pz);
    new Float:d = floatsqroot(
        (px-TurfData[turfid][twPosX])*(px-TurfData[turfid][twPosX]) +
        (py-TurfData[turfid][twPosY])*(py-TurfData[turfid][twPosY]) +
        (pz-TurfData[turfid][twPosZ])*(pz-TurfData[turfid][twPosZ]));

    if(d <= TURF_SUPPLY_DELIVER_RADIUS)
    {
        TW_DeliverSupply(playerid);
        return 1;
    }
    return 0;
}

// ---------------------------------------------------------------------------
// TIMER: Đếm ngược war và kiểm tra supply deadline
// ---------------------------------------------------------------------------

// Đếm ngược war mỗi 1 giây
task TurfWarsNewUpdate[1000]()
{
    static zoneCheckTick = 0;
    zoneCheckTick++;

    for(new i = 0; i < TotalTurfs; i++)
    {
        if(!TurfData[i][twActive]) continue;

        if(TurfData[i][twTimeLeft] > 0)
        {
            TurfData[i][twTimeLeft]--;

            // Cảnh báo 60s và 30s còn lại
            if(TurfData[i][twTimeLeft] == 60 || TurfData[i][twTimeLeft] == 30)
            {
                new str[96];
                format(str, sizeof(str), "[WAR] Con %d giay! Chuan bi phan quyet!", TurfData[i][twTimeLeft]);
                new aG = TurfData[i][twAttackGang], dG = TurfData[i][twDefendGang];
                if(aG >= 0) TW_FamilyMsg(aG, COLOR_YELLOW, str);
                if(dG >= 0) TW_FamilyMsg(dG, COLOR_YELLOW, str);
            }

            // Cập nhật label countdown mỗi 10 giây
            if(TurfData[i][twTimeLeft] % 10 == 0)
                TW_UpdateLabel(i);
        }
        else
        {
            TW_EndWar(i);
        }
    }

    // Kiểm tra player vào/rời khu vực war mỗi 10 giây
    if(zoneCheckTick >= 10)
    {
        zoneCheckTick = 0;
        foreach(new p : Player)
        {
            new Float:ppx, Float:ppy, Float:ppz;
            GetPlayerPos(p, ppx, ppy, ppz);

            new newZone = -1;
            for(new t = 0; t < TotalTurfs; t++)
            {
                if(!TurfData[t][twActive]) continue;
                new Float:d = floatsqroot(
                    (ppx - TurfData[t][twPosX]) * (ppx - TurfData[t][twPosX]) +
                    (ppy - TurfData[t][twPosY]) * (ppy - TurfData[t][twPosY]) +
                    (ppz - TurfData[t][twPosZ]) * (ppz - TurfData[t][twPosZ]));
                if(d <= TURF_WAR_RADIUS) { newZone = t; break; }
            }

            new oldZone = g_twInZone[p];
            if(newZone == oldZone) continue;

            g_twInZone[p] = newZone;

            if(newZone != -1) // Vừa vào zone war
            {
                new pFam = PlayerInfo[p][pFMember];
                new aG = TurfData[newZone][twAttackGang];
                new dG = TurfData[newZone][twDefendGang];
                if(pFam == aG || pFam == dG)
                    SendClientMessageEx(p, COLOR_YELLOW, ">> Ban da vao khu vuc chien dau! Hay chien dau het minh!");
                else
                    SendClientMessageEx(p, COLOR_RED, "{FF4444}[CANH BAO] Co noi sung dang dien ra gan day! Hay can than va roi di ngay!");
            }
            else if(oldZone >= 0) // Vừa rời zone war (guard: chỉ notify nếu oldZone hợp lệ)
            {
                new pFam2 = PlayerInfo[p][pFMember];
                new aG2 = TurfData[oldZone][twAttackGang];
                new dG2 = TurfData[oldZone][twDefendGang];
                if(pFam2 == aG2 || pFam2 == dG2)
                    SendClientMessageEx(p, COLOR_YELLOW, ">> Ban da roi khu vuc chien dau.");
            }
        }
    }
}

// Kiểm tra supply deadline mỗi 5 phút
task TurfSupplyDeadlineCheck[300000]()
{
    new now = gettime();
    for(new i = 0; i < TotalTurfs; i++)
    {
        if(TurfData[i][twOwnerGang] < 0)           continue;
        if(TurfData[i][twSupplyDeadline] == 0)     continue;
        if(TurfData[i][twSupplyCount] >= TURF_SUPPLY_MAX) continue;
        if(now <= TurfData[i][twSupplyDeadline])   continue;

        // Hết hạn mà chưa đủ 50 chuyến
        new owner = TurfData[i][twOwnerGang];
        FA_AddFame(owner, -TURF_SUPPLY_FAIL_FAME);

        new str[96];
        format(str, sizeof(str),
            "[HANG CAM] Gang that bai! Chi giao %d/%d chuyen hang cam trong 48h. -%d fame!",
            TurfData[i][twSupplyCount], TURF_SUPPLY_MAX, TURF_SUPPLY_FAIL_FAME);
        TW_FamilyMsg(owner, COLOR_RED, str);
        SaveFamily(owner);

        TurfData[i][twSupplyCount]    = 0;
        TurfData[i][twSupplyDeadline] = 0;
        TW_SaveTurf(i);
    }
}
