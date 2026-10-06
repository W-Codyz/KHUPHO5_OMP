#include <open.mp>
#include <YSI\YSI_Coding\y_hooks>

// ============================================================
// CẤU HÌNH
// ============================================================
#define MAX_BAN_ROULETTE            20
#define KHOANG_THOI_GIAN_QUAY       70000
#define THOI_GIAN_HIEU_UNG_QUAY     5000
#define TEP_TIN_ROULETTE            "Roulette_Tables.cfg"
#define TIEN_TO_TIN_NHAN            "{FFFF00}GTAHTB{FFFFFF} "
#define KHOANG_THOI_GIAN_DEM_NGUOC  1000
#define GIOI_HAN_THANG_MOI_LAN      5000000
#define VAT_PHAN_TRAM               2

// ============================================================
// LOG BUFFER — tránh cấp phát mảng lớn trên stack
// ============================================================
#define LOG_BUF_SIZE        3800     // dưới 4096 của Discord embed
#define LOG_CHUNK_SIZE      512
#define DCC_SPLIT_LIMIT     3600
#define DCC_SPLIT_MAX       5

// ============================================================
// DIALOG IDS
// ============================================================
#define DIALOG_ROULETTE_CHINH        9110
#define DIALOG_ROULETTE_LOAI_CUOC    9111
#define DIALOG_ROULETTE_CUOC_SO      9112
#define DIALOG_ROULETTE_SO_TIEN_CUOC 9113
#define DIALOG_ROULETTE_QUAY         9114
#define DIALOG_ROULETTE_LICH_SU      9115
#define DIALOG_ROULETTE_NHACAI_INFO  9116

// ============================================================
// ENUM + BIẾN TOÀN CỤC
// ============================================================
enum DuLieuBanRoulette
{
    Float:bX,
    Float:bY,
    Float:bZ,
    bVatThe,
    Text3D:bNhan,
    bool:bHoatDong,
    bHenGio,
    bHenGioDemNguoc,
    bSoThangCuoi,
    bGiayDemNguoc,
    bool:bDangQuay,
    bHenGioHieuUngQuay,
    bViTriHieuUngQuay,
    bLichSu[20],
    bLichSuThoiGian[20],
    bChiSoLichSu
}

new BanRoulette[MAX_BAN_ROULETTE][DuLieuBanRoulette];
new CuocHienTai[MAX_BAN_ROULETTE][MAX_PLAYERS][37];
new CuocMau[MAX_BAN_ROULETTE][MAX_PLAYERS][3];
new BanCuaNguoiChoi[MAX_PLAYERS] = {-1, ...};
new DuLieuCuocCuaNguoiChoi[MAX_PLAYERS];
new bool:NguoiChoiTrongRoulette[MAX_PLAYERS];

new CacSoRoulette[37] = {
    0,32,15,19,4,21,2,25,17,34,6,27,13,36,11,30,
    8,23,10,5,24,16,33,1,20,14,31,9,22,18,29,7,28,12,35,3,26
};

new const cacSoDo[] = {1,3,5,7,9,12,14,16,18,19,21,23,25,27,30,32,34,36};
static const cacSoDen[] = {2,4,6,8,10,11,13,15,17,20,22,24,26,28,29,31,33,35};

new AdminBuocKetQua[MAX_BAN_ROULETTE];        // -1 = random, 0-36 = ép

// AI state
static g_sAiDecision[512];                    // scratch log phiên hiện tại
new g_AiKetQua[MAX_BAN_ROULETTE] = {-1, ...};
new g_sAiDecisionBan[MAX_BAN_ROULETTE][384];  // log AI theo bàn

// Log buffer toàn cục (tránh stack)
static g_sLogBuf[LOG_BUF_SIZE];
static g_sLogChunk[LOG_CHUNK_SIZE];

// ============================================================
// FORWARD
// ============================================================
forward DemNguocThoiGianQuay(idban);
forward HenGioQuayRoulette(idban);
forward KetThucQuay(idban);
forward HenGioHieuUngQuay(idban);

// ============================================================
// LOG BUFFER HELPERS
// ============================================================
stock LogReset()
{
    g_sLogBuf[0] = 0;
}

stock bool:LogAdd(const str[])
{
    if(strlen(g_sLogBuf) >= LOG_BUF_SIZE - 1) return false;
    strcat(g_sLogBuf, str, LOG_BUF_SIZE);
    return true;
}

stock LogLen()
{
    return strlen(g_sLogBuf);
}

// Tìm điểm cắt tự nhiên (sau '\n') gần `preferredLen`
stock FindSplitPoint(const content[], start, preferredLen)
{
    new total = strlen(content);
    new endPos = start + preferredLen;
    if(endPos >= total) return total - start;

    new searchStart = start + preferredLen - 400;
    if(searchStart < start) searchStart = start;

    for(new i = endPos; i > searchStart; i--)
        if(content[i] == '\n') return i - start + 1;

    return preferredLen; // không có '\n', cắt cứng
}

// Gửi log; nếu > DCC_SPLIT_LIMIT thì chia thành nhiều embed
stock SendLogSplit(const baseTitle[], const content[], color = 0x00FF00)
{
    new total = strlen(content);
    if(total == 0) return 0;

    new y, mo, d, h, mi, s;
    getdate(y, mo, d); gettime(h, mi, s);
    new ts[64];
    format(ts, sizeof(ts), "%02d/%02d/%d - %02d:%02d:%02d", d, mo, y, h, mi, s);

    // 1 embed duy nhất
    if(total <= DCC_SPLIT_LIMIT)
    {
        new DCC_Embed:embed = DCC_CreateEmbed();
        DCC_SetEmbedTitle(embed, baseTitle);
        DCC_SetEmbedDescription(embed, content);
        DCC_SetEmbedColor(embed, color);
        DCC_SetEmbedFooter(embed, ts);
        DCC_SendChannelEmbedMessage(ruathoChannel, embed);
        return 1;
    }

    // Chia nhiều phần
    new totalParts = (total + DCC_SPLIT_LIMIT - 1) / DCC_SPLIT_LIMIT;
    if(totalParts > DCC_SPLIT_MAX) totalParts = DCC_SPLIT_MAX;

    new start = 0, part = 0;
    new partBuf[DCC_SPLIT_LIMIT + 128];

    while(start < total && part < totalParts)
    {
        new partLen = FindSplitPoint(content, start, DCC_SPLIT_LIMIT);
        if(start + partLen > total) partLen = total - start;

        strmid(partBuf, content, start, start + partLen, sizeof(partBuf));

        new partTitle[128];
        format(partTitle, sizeof(partTitle), "%s [%d/%d]", baseTitle, part + 1, totalParts);

        new DCC_Embed:embed = DCC_CreateEmbed();
        DCC_SetEmbedTitle(embed, partTitle);
        DCC_SetEmbedDescription(embed, partBuf);
        DCC_SetEmbedColor(embed, color);
        DCC_SetEmbedFooter(embed, ts);
        DCC_SendChannelEmbedMessage(ruathoChannel, embed);

        start += partLen;
        part++;
    }
    return 1;
}

// ============================================================
// UTILITY — màu sắc, tên màu, thời gian
// ============================================================
stock LayMauRoulette(so)
{
    if(so == 0) return 2;
    for(new i = 0; i < sizeof(cacSoDo); i++)
        if(so == cacSoDo[i]) return 0;
    return 1;
}

stock LayTenMau(mau, ketqua[], dodai)
{
    switch(mau)
    {
        case 0: format(ketqua, dodai, "Tho (Do)");
        case 1: format(ketqua, dodai, "Rua (Den)");
        case 2: format(ketqua, dodai, "Xanh");
        default: format(ketqua, dodai, "Khong ro");
    }
}

stock MauCode(mau, out[], size)
{
    switch(mau)
    {
        case 0: format(out, size, "{FF6347}");
        case 1: format(out, size, "{A9A9A9}");
        case 2: format(out, size, "{90EE90}");
        default: format(out, size, "{FFFFFF}");
    }
}

stock FormatThoiGianTruoc(timestamp, ketqua[], dodai)
{
    if(timestamp == 0) { format(ketqua, dodai, "Khong ro"); return; }

    new chenh = gettime() - timestamp;
    if(chenh < 0) chenh = 0;

    if(chenh < 10)         format(ketqua, dodai, "{00FF00}Vua xong");
    else if(chenh < 60)    format(ketqua, dodai, "%d giay truoc", chenh);
    else if(chenh < 3600)  format(ketqua, dodai, "%d phut truoc", chenh / 60);
    else if(chenh < 86400) format(ketqua, dodai, "%d gio truoc", chenh / 3600);
    else                   format(ketqua, dodai, "%d ngay truoc", chenh / 86400);
}

// ============================================================
// DISCORD LOG — admin actions
// ============================================================
stock SendRuaThoLogDC(const str[])
{
    new DCC_Embed:embed = DCC_CreateEmbed();
    DCC_SetEmbedTitle(embed, "🎲 RÙA THỎ - LOG");
    DCC_SetEmbedDescription(embed, str);
    DCC_SetEmbedColor(embed, 0xFFD700);

    new y, mo, d, h, mi, s;
    getdate(y, mo, d); gettime(h, mi, s);
    new ts[64];
    format(ts, sizeof(ts), "%02d/%02d/%d - %02d:%02d:%02d", d, mo, y, h, mi, s);
    DCC_SetEmbedFooter(embed, ts);
    DCC_SendChannelEmbedMessage(ruathoChannel, embed);
    return 1;
}

stock SendNhaCaiLogDC(const str[])
{
    if(nhaCaiChannel == DCC_INVALID_CHANNEL) return 0;
    new DCC_Embed:embed = DCC_CreateEmbed();
    DCC_SetEmbedTitle(embed, "🧠 NHÀ CÁI");
    DCC_SetEmbedDescription(embed, str);
    DCC_SetEmbedColor(embed, 0xFF4500);

    new y, mo, d, h, mi, s;
    getdate(y, mo, d); gettime(h, mi, s);
    new ts[64];
    format(ts, sizeof(ts), "%02d/%02d/%d - %02d:%02d:%02d", d, mo, y, h, mi, s);
    DCC_SetEmbedFooter(embed, ts);
    DCC_SendChannelEmbedMessage(nhaCaiChannel, embed);
    return 1;
}

// ============================================================
// QUẢN LÝ BÀN
// ============================================================
stock TaoHenGioRoulette(idban)
{
    if(idban < 0 || idban >= MAX_BAN_ROULETTE || !BanRoulette[idban][bHoatDong]) return 0;
    if(BanRoulette[idban][bHenGio] != -1) KillTimer(BanRoulette[idban][bHenGio]);
    if(BanRoulette[idban][bHenGioDemNguoc] != -1) KillTimer(BanRoulette[idban][bHenGioDemNguoc]);

    BanRoulette[idban][bGiayDemNguoc] = 70;
    BanRoulette[idban][bHenGio] = SetTimerEx("HenGioQuayRoulette", KHOANG_THOI_GIAN_QUAY, false, "i", idban);
    BanRoulette[idban][bHenGioDemNguoc] = SetTimerEx("DemNguocThoiGianQuay", KHOANG_THOI_GIAN_DEM_NGUOC, true, "i", idban);
    CapNhatNhanRoulette(idban);
    return 1;
}

stock CapNhatNhanRoulette(idban)
{
    if(idban < 0 || idban >= MAX_BAN_ROULETTE || !BanRoulette[idban][bHoatDong]) return 0;

    new nhanvanban[256];
    if(BanRoulette[idban][bDangQuay])
        format(nhanvanban, sizeof(nhanvanban),
            "{FFFF00}Ban Quay So (Rua,Tho) #%d\n{FF0000}DANG QUAY...", idban);
    else
        format(nhanvanban, sizeof(nhanvanban),
            "{FFFF00}Ban Quay So (Rua,Tho) #%d\n{FFFFFF}/choihtb de tham gia\n{00FF00}Quay sau: %d giay",
            idban, BanRoulette[idban][bGiayDemNguoc]);

    if(BanRoulette[idban][bNhan] != Text3D:INVALID_3DTEXT_ID)
        Update3DTextLabelText(BanRoulette[idban][bNhan], 0xFFFFFFFF, nhanvanban);
    return 1;
}

stock LuuBanRoulette()
{
    new File:tep = fopen(TEP_TIN_ROULETTE, io_write);
    if(!tep) { printf("Cannot create/write file: %s", TEP_TIN_ROULETTE); return 0; }

    new chuoi[128], count = 0;
    for(new i = 0; i < MAX_BAN_ROULETTE; i++)
    {
        if(!BanRoulette[i][bHoatDong]) continue;
        format(chuoi, sizeof(chuoi), "%.6f %.6f %.6f\r\n",
            BanRoulette[i][bX], BanRoulette[i][bY], BanRoulette[i][bZ]);
        fwrite(tep, chuoi);
        count++;
    }
    fclose(tep);
    printf("Saved %d roulette tables to file", count);
    return 1;
}

stock TaiBanRoulette()
{
    new File:tep = fopen(TEP_TIN_ROULETTE, io_read);
    if(!tep) { printf("Cannot open file: %s", TEP_TIN_ROULETTE); return 0; }

    new chuoi[128], Float:x, Float:y, Float:z;
    new idban = 0;

    while(fread(tep, chuoi) && idban < MAX_BAN_ROULETTE)
    {
        for(new i = 0; i < sizeof(chuoi); i++)
            if(chuoi[i] == '\n' || chuoi[i] == '\r') { chuoi[i] = '\0'; break; }

        if(strlen(chuoi) < 5) continue;

        if(sscanf(chuoi, "fff", x, y, z) == 0)
        {
            BanRoulette[idban][bX] = x;
            BanRoulette[idban][bY] = y;
            BanRoulette[idban][bZ] = z;
            BanRoulette[idban][bHoatDong] = true;
            BanRoulette[idban][bVatThe] = CreateObject(1824, x, y, z - 0.95, 0.0, 0.0, 0.0);
            BanRoulette[idban][bNhan] = Create3DTextLabel("Dang tai...", 0xFFFFFFFF, x, y, z + 0.5, 10.0, 0, 0);
            TaoHenGioRoulette(idban);
            printf("Loaded table %d: %.2f %.2f %.2f", idban, x, y, z);
            idban++;
        }
        else printf("Failed to parse line: %s", chuoi);
    }
    fclose(tep);
    printf("Loaded %d roulette tables", idban);
    return 1;
}

// ============================================================
// DIALOG
// ============================================================
stock HienThiDialogChinh(playerid)
{
    if(!NguoiChoiTrongRoulette[playerid] || BanCuaNguoiChoi[playerid] == -1) return 0;
    new idban = BanCuaNguoiChoi[playerid];
    new dialog[2048], temp[256];

    format(dialog, sizeof(dialog), "{FFFF00}======= SONG BAI RUA THO =======\n\n");

    if(BanRoulette[idban][bSoThangCuoi] == -1)
        strcat(dialog, "{FFFFFF}So vua trung thuong:\t{AAAAAA}Chua co\n\n", sizeof(dialog));
    else
    {
        new tenmau[32], mauColor[16];
        new mau = LayMauRoulette(BanRoulette[idban][bSoThangCuoi]);
        LayTenMau(mau, tenmau, sizeof(tenmau));
        MauCode(mau, mauColor, sizeof(mauColor));
        format(temp, sizeof(temp), "{FFFFFF}So vua trung thuong:\t%s%d (%s)\n\n",
            mauColor, BanRoulette[idban][bSoThangCuoi], tenmau);
        strcat(dialog, temp, sizeof(dialog));
    }

    if(BanRoulette[idban][bGiayDemNguoc] > 0 && !BanRoulette[idban][bDangQuay])
    {
        new mauTG[16];
        if(BanRoulette[idban][bGiayDemNguoc] <= 10)     mauTG = "{FF0000}";
        else if(BanRoulette[idban][bGiayDemNguoc] <= 30) mauTG = "{FFFF00}";
        else                                             mauTG = "{00FF00}";
        format(temp, sizeof(temp), "{FFFFFF}Thoi gian cuoc con:\t%s%d giay\n\n",
            mauTG, BanRoulette[idban][bGiayDemNguoc]);
        strcat(dialog, temp, sizeof(dialog));
    }
    else if(BanRoulette[idban][bDangQuay])
        strcat(dialog, "{FF0000}Trang thai:\t\tDANG QUAY...\n\n", sizeof(dialog));
    else
        strcat(dialog, "\n", sizeof(dialog));

    strcat(dialog, "{FFFF00}----------- HUONG DAN -----------\n\n", sizeof(dialog));
    strcat(dialog, "{FFFFFF}Nhan nut '{00FF00}Dat Cuoc{FFFFFF}' de mo bang cuoc.\n", sizeof(dialog));
    strcat(dialog, "{FFFFFF}Dung lenh {FFFF00}/lichsuhtb{FFFFFF} de xem 20 lan quay truoc.\n\n", sizeof(dialog));
    strcat(dialog, "{CCCCCC}Cac loai cuoc pho bien:\n\n", sizeof(dialog));
    strcat(dialog, "\t{FF6347}> Cuoc 1 So (0-36):\t{FFFF00}Ty le 1 an 35\n", sizeof(dialog));
    strcat(dialog, "\t{FF6347}> Cuoc Tho (Do):\t{FFFF00}Ty le 1 an 1\n", sizeof(dialog));
    strcat(dialog, "\t{A9A9A9}> Cuoc Rua (Den):\t{FFFF00}Ty le 1 an 1\n", sizeof(dialog));
    strcat(dialog, "\t{90EE90}> Cuoc Xanh (so 0):\t{FFFF00}Ty le 1 an 35\n\n", sizeof(dialog));

    new tongCuoc = 0;
    for(new i = 0; i < 37; i++) tongCuoc += CuocHienTai[idban][playerid][i];
    for(new i = 0; i < 3; i++)  tongCuoc += CuocMau[idban][playerid][i];

    if(tongCuoc > 0)
    {
        format(temp, sizeof(temp), "{FFFFFF}Tong cuoc da dat:\t\t{FF6347}$%s\n", number_format(tongCuoc));
        strcat(dialog, temp, sizeof(dialog));
    }
    format(temp, sizeof(temp), "{FFFFFF}Tien mat cua ban:\t\t{00FF00}$%s\n", number_format(PlayerInfo[playerid][pCash]));
    strcat(dialog, temp, sizeof(dialog));

    ShowPlayerDialog(playerid, DIALOG_ROULETTE_CHINH, DIALOG_STYLE_MSGBOX,
        "{FFFF00}Ban Quay So Rua Tho", dialog, "Dat Cuoc", "Thoat");
    return 1;
}

stock HienThiDialogLoaiCuoc(playerid)
{
    new dialog[512];
    format(dialog, sizeof(dialog),
        "{FFFFFF}Cuoc 1 So {AAAAAA}(0-36)\t{FFFF00}Ty le 1:35\n\
        {FF6347}Cuoc Tho (Do)\t{FFFF00}Ty le 1:1\n\
        {A9A9A9}Cuoc Rua (Den)\t{FFFF00}Ty le 1:1\n\
        {90EE90}Cuoc Xanh (So 0)\t{FFFF00}Ty le 1:35");
    ShowPlayerDialog(playerid, DIALOG_ROULETTE_LOAI_CUOC, DIALOG_STYLE_LIST,
        "{FFFF00}Chon loai cuoc", dialog, "Chon", "Tro lai");
}

stock HienThiDialogQuay(playerid, idban, vitrihighlight, vitrithang)
{
    if(!NguoiChoiTrongRoulette[playerid] || BanCuaNguoiChoi[playerid] != idban) return 0;
    new dialog[2048], temp[64];
    strcat(dialog, "{FFFF00}========= VONG QUAY RUA THO =========\n\n", sizeof(dialog));

    for(new i = 0; i < 37; i++)
    {
        new soHienTai = CacSoRoulette[i];
        if(vitrithang != -1 && i == vitrithang)
            format(temp, sizeof(temp), "{00FF00}>> %d << ", soHienTai);
        else if(vitrithang == -1 && i == vitrihighlight)
            format(temp, sizeof(temp), "{FFFF00}[%d] ", soHienTai);
        else
        {
            new mc[16];
            MauCode(LayMauRoulette(soHienTai), mc, sizeof(mc));
            format(temp, sizeof(temp), "%s%d ", mc, soHienTai);
        }
        strcat(dialog, temp, sizeof(dialog));
        if((i + 1) % 12 == 0) strcat(dialog, "\n", sizeof(dialog));
    }

    if(vitrithang != -1)
    {
        new soThang = CacSoRoulette[vitrithang];
        new tenmau[32], mauColor[16];
        new mau = LayMauRoulette(soThang);
        LayTenMau(mau, tenmau, sizeof(tenmau));
        MauCode(mau, mauColor, sizeof(mauColor));

        format(temp, sizeof(temp), "\n\n{FFFFFF}KET QUA: %s%d (%s){FFFFFF}", mauColor, soThang, tenmau);
        strcat(dialog, temp, sizeof(dialog));
        ShowPlayerDialog(playerid, DIALOG_ROULETTE_QUAY, DIALOG_STYLE_MSGBOX,
            "{00FF00}Rua Tho - Ket Qua", dialog, "Dong", "");
    }
    else
    {
        strcat(dialog, "\n\n{FFFF00}Dang quay...", sizeof(dialog));
        ShowPlayerDialog(playerid, DIALOG_ROULETTE_QUAY, DIALOG_STYLE_MSGBOX,
            "{FFFF00}Rua Tho - Dang Quay", dialog, "", "");
    }
    return 1;
}

// ============================================================
// AI — PRECOMPUTE PAYOUT PER TARGET
// ============================================================
stock TinhTongPayout(idban, soKetQua, bool:chiTinhTarget)
{
    new tongPayout = 0;
    new mauKQ = LayMauRoulette(soKetQua);

    for(new p = 0; p < MAX_PLAYERS; p++)
    {
        if(!IsPlayerConnected(p)) continue;
        if(chiTinhTarget && PlayerInfo[p][pRouletteTarget] != 1) continue;

        if(CuocHienTai[idban][p][soKetQua] > 0)
            tongPayout += CuocHienTai[idban][p][soKetQua] * 36;
        if(CuocMau[idban][p][mauKQ] > 0)
            tongPayout += CuocMau[idban][p][mauKQ] * ((mauKQ == 2) ? 36 : 2);
    }
    return tongPayout;
}

// Precompute payout[0..36] cho 1 player
stock PrecomputePayoutPlayer(idban, p, payoutByNumber[37])
{
    for(new i = 0; i < 37; i++) payoutByNumber[i] = 0;

    for(new so = 0; so < 37; so++)
        if(CuocHienTai[idban][p][so] > 0)
            payoutByNumber[so] += CuocHienTai[idban][p][so] * 36;

    if(CuocMau[idban][p][0] > 0)
        for(new i = 0; i < sizeof(cacSoDo); i++)
            payoutByNumber[cacSoDo[i]] += CuocMau[idban][p][0] * 2;

    if(CuocMau[idban][p][1] > 0)
        for(new i = 0; i < sizeof(cacSoDen); i++)
            payoutByNumber[cacSoDen[i]] += CuocMau[idban][p][1] * 2;

    if(CuocMau[idban][p][2] > 0)
        payoutByNumber[0] += CuocMau[idban][p][2] * 36;
}

// Precompute payout toàn bàn (mọi player)
stock PrecomputePayoutAll(idban, payoutByNumber[37])
{
    for(new i = 0; i < 37; i++) payoutByNumber[i] = 0;

    for(new p = 0; p < MAX_PLAYERS; p++)
    {
        if(!IsPlayerConnected(p)) continue;
        for(new so = 0; so < 37; so++)
            if(CuocHienTai[idban][p][so] > 0)
                payoutByNumber[so] += CuocHienTai[idban][p][so] * 36;

        if(CuocMau[idban][p][0] > 0)
            for(new i = 0; i < sizeof(cacSoDo); i++)
                payoutByNumber[cacSoDo[i]] += CuocMau[idban][p][0] * 2;

        if(CuocMau[idban][p][1] > 0)
            for(new i = 0; i < sizeof(cacSoDen); i++)
                payoutByNumber[cacSoDen[i]] += CuocMau[idban][p][1] * 2;

        if(CuocMau[idban][p][2] > 0)
            payoutByNumber[0] += CuocMau[idban][p][2] * 36;
    }
}

stock TimSoPayoutThapNhat(idban)
{
    new payoutByNumber[37];
    PrecomputePayoutAll(idban, payoutByNumber);

    new candidates[37], count = 0;
    new payoutMin = 0x7FFFFFFF;
    for(new i = 0; i < 37; i++)
    {
        if(payoutByNumber[i] < payoutMin)
        {
            payoutMin = payoutByNumber[i];
            count = 0;
            candidates[count++] = i;
        }
        else if(payoutByNumber[i] == payoutMin)
            candidates[count++] = i;
    }
    if(count == 0) return random(37);
    return candidates[random(count)];
}

// Lấy số random thuộc màu (0=Đỏ, 1=Đen)
stock LaySoTheoMau(mau)
{
    if(mau == 0) return cacSoDo[random(sizeof(cacSoDo))];
    return cacSoDen[random(sizeof(cacSoDen))];
}

// Lấy số thuộc màu, ưu tiên số player cược nhiều nhất
stock LaySoChoThang(idban, playerid, mau)
{
    new bestNum = -1, bestBet = 0;
    for(new i = 1; i < 37; i++)
    {
        if(LayMauRoulette(i) != mau) continue;
        if(CuocHienTai[idban][playerid][i] > bestBet)
        {
            bestBet = CuocHienTai[idban][playerid][i];
            bestNum = i;
        }
    }
    if(bestNum != -1) return bestNum;
    return LaySoTheoMau(mau);
}

// Lấy số thuộc màu, tránh số player cược. Trả về -1 nếu không thể.
stock LaySoChoThua(idban, playerid, mau)
{
    new candidates[18], count = 0;
    for(new i = 1; i < 37; i++)
    {
        if(LayMauRoulette(i) != mau) continue;
        if(CuocHienTai[idban][playerid][i] == 0)
            candidates[count++] = i;
    }
    if(count > 0) return candidates[random(count)];
    return -1; // Caller phải xử lý
}

// ============================================================
// AI DECISION
// ============================================================
stock ChonKetQuaNhaCai(idban)
{
    g_sAiDecision[0] = 0;

    // ---- BƯỚC 1: Circuit breaker ----
    new tongCuocTargets = 0;
    for(new tp = 0; tp < MAX_PLAYERS; tp++)
    {
        if(!IsPlayerConnected(tp)) continue;
        if(PlayerInfo[tp][pRouletteTarget] != 1) continue;
        for(new i = 0; i < 37; i++) tongCuocTargets += CuocHienTai[idban][tp][i];
        for(new i = 0; i < 3; i++)  tongCuocTargets += CuocMau[idban][tp][i];
    }

    if(tongCuocTargets > 10000000)
    {
        new soEp = TimSoPayoutThapNhat(idban);
        new payoutSoEp = TinhTongPayout(idban, soEp, false);
        new tenMauEp[32];
        LayTenMau(LayMauRoulette(soEp), tenMauEp, sizeof(tenMauEp));

        new aiLog[512];
        if(payoutSoEp == 0)
            format(aiLog, sizeof(aiLog),
                "**🔌 CIRCUIT BREAKER** | Bàn #%d\nTổng cược target: `$%s` > $10m\n→ Không ai thắng nếu ra **%s** (số %d) → Nhà ăn 100%%",
                idban, number_format(tongCuocTargets), tenMauEp, soEp);
        else
            format(aiLog, sizeof(aiLog),
                "**🔌 CIRCUIT BREAKER** | Bàn #%d\nTổng cược target: `$%s` > $10m\n→ Payout thấp nhất: `$%s` khi ra **%s** (số %d)",
                idban, number_format(tongCuocTargets), number_format(payoutSoEp), tenMauEp, soEp);

        strcat(g_sAiDecision, aiLog, sizeof(g_sAiDecision));
        SendNhaCaiLogDC(aiLog);
        return soEp;
    }

    // ---- BƯỚC 2: Sort targets theo profit tăng dần ----
    new sortedTargets[MAX_PLAYERS], numTargets = 0;
    for(new p = 0; p < MAX_PLAYERS; p++)
    {
        if(!IsPlayerConnected(p)) continue;
        if(PlayerInfo[p][pRouletteTarget] != 1) continue;

        new tongCuoc = 0;
        for(new i = 0; i < 37; i++) tongCuoc += CuocHienTai[idban][p][i];
        for(new i = 0; i < 3; i++)  tongCuoc += CuocMau[idban][p][i];
        if(tongCuoc == 0) continue;

        sortedTargets[numTargets++] = p;
    }

    if(numTargets == 0) return -1;

    for(new i = 0; i < numTargets - 1; i++)
        for(new j = 0; j < numTargets - i - 1; j++)
            if(PlayerInfo[sortedTargets[j]][pRouletteProfit] > PlayerInfo[sortedTargets[j+1]][pRouletteProfit])
            {
                new tmp = sortedTargets[j];
                sortedTargets[j] = sortedTargets[j+1];
                sortedTargets[j+1] = tmp;
            }

    // ---- BƯỚC 3: Duyệt target ----
    for(new idx = 0; idx < numTargets; idx++)
    {
        new p = sortedTargets[idx];

        new taiSanThuc = GetPlayerCash(p) + PlayerInfo[p][pAccount];
        if(taiSanThuc <= 4000000)
        {
            new aiLog[256];
            format(aiLog, sizeof(aiLog),
                "**⛔ SKIP** | `%s` | Bàn #%d\nTài sản: `$%s` ≤ 4m → Đã đạt mục tiêu",
                GetPlayerNameEx(p), idban, number_format(taiSanThuc));
            strcat(g_sAiDecision, aiLog, sizeof(g_sAiDecision));
            strcat(g_sAiDecision, "\n", sizeof(g_sAiDecision));
            SendNhaCaiLogDC(aiLog);
            continue;
        }

        new profit = PlayerInfo[p][pRouletteProfit];
        new cuocTho = CuocMau[idban][p][0];
        new cuocRua = CuocMau[idban][p][1];
        new cuocXanh = CuocMau[idban][p][2];

        new mode;
        if(profit < -5000000) mode = 1;
        else if(profit < 0)   mode = 0;
        else if(profit < 1000000) mode = 2;
        else                  mode = 3;

        new modeLyDo[64];
        if(mode == 1)      format(modeLyDo, _, "thua sâu ($%s)", number_format(profit));
        else if(mode == 0) format(modeLyDo, _, "đang âm ($%s)", number_format(profit));
        else if(mode == 2) format(modeLyDo, _, "vừa thắng ($%s)", number_format(profit));
        else               format(modeLyDo, _, "thắng nhiều ($%s)", number_format(profit));

        if(mode == 2 && PlayerInfo[p][pRoulNguong] > 0 &&
           PlayerInfo[p][pRoulVanThang] >= PlayerInfo[p][pRoulNguong])
        {
            mode = 3;
            format(modeLyDo, _, "hết chu kỳ (%d/%d ván)",
                PlayerInfo[p][pRoulVanThang], PlayerInfo[p][pRoulNguong]);
        }

        if(mode == 0)
        {
            new aiLog[256];
            format(aiLog, sizeof(aiLog),
                "**⭐ SKIP** | `%s` | Bàn #%d\nLý do: `%s` → mode Random",
                GetPlayerNameEx(p), idban, modeLyDo);
            strcat(g_sAiDecision, aiLog, sizeof(g_sAiDecision));
            strcat(g_sAiDecision, "\n", sizeof(g_sAiDecision));
            SendNhaCaiLogDC(aiLog);
            continue;
        }

        // ============ MODE 1: MỒI ============
        if(mode == 1)
        {
            new tongCuocP = 0;
            for(new i = 0; i < 37; i++) tongCuocP += CuocHienTai[idban][p][i];
            for(new i = 0; i < 3; i++)  tongCuocP += CuocMau[idban][p][i];

            // Precompute payout
            new payoutP[37];
            PrecomputePayoutPlayer(idban, p, payoutP);

            new coTheMoi = false;
            for(new i = 0; i < 37; i++)
            {
                if(payoutP[i] > tongCuocP && payoutP[i] <= 10000000)
                {
                    coTheMoi = true;
                    break;
                }
            }

            if(!coTheMoi)
            {
                new aiLog[256];
                format(aiLog, sizeof(aiLog),
                    "**⭐ SKIP MỒI** | `%s` | Bàn #%d\nLý do: `%s`\nKhông có kết quả nào cho target thắng trong budget $10m",
                    GetPlayerNameEx(p), idban, modeLyDo);
                strcat(g_sAiDecision, aiLog, sizeof(g_sAiDecision));
                strcat(g_sAiDecision, "\n", sizeof(g_sAiDecision));
                SendNhaCaiLogDC(aiLog);
                continue;
            }

            new mauNha = -1;
            if(cuocTho > 0 || cuocRua > 0) {
                if(cuocTho > cuocRua)      mauNha = 0;
                else if(cuocRua > cuocTho) mauNha = 1;
                else                        mauNha = random(2);
            }

            new ketQua = -1, bestBet = 0;
            if(mauNha != -1)
                ketQua = LaySoChoThang(idban, p, mauNha);
            else if(cuocXanh > 0 && cuocTho == 0 && cuocRua == 0)
            {
                new aiLog[256];
                format(aiLog, sizeof(aiLog),
                    "**⭐ SKIP MỒI** | `%s` | Bàn #%d\nLý do: `%s`\nChi co cuoc Xanh → thu target khac",
                    GetPlayerNameEx(p), idban, modeLyDo);
                strcat(g_sAiDecision, aiLog, sizeof(g_sAiDecision));
                strcat(g_sAiDecision, "\n", sizeof(g_sAiDecision));
                SendNhaCaiLogDC(aiLog);
                continue;
            }
            else
            {
                new bestNum = -1;
                for(new i = 1; i < 37; i++)
                    if(CuocHienTai[idban][p][i] > bestBet) { bestBet = CuocHienTai[idban][p][i]; bestNum = i; }
                if(bestNum == -1) continue;
                ketQua = bestNum;
            }

            // Payout guard cuối cùng
            new tongPayoutMoi = TinhTongPayout(idban, ketQua, true);
            new aiLog[512];

            if(tongPayoutMoi > 10000000)
            {
                format(aiLog, sizeof(aiLog),
                    "**⭐ SKIP MỒI** | `%s` | Bàn #%d\nLý do: `%s`\nPayout target nếu ép: `$%s` > $10m → thử target khác",
                    GetPlayerNameEx(p), idban, modeLyDo, number_format(tongPayoutMoi));
                strcat(g_sAiDecision, aiLog, sizeof(g_sAiDecision));
                strcat(g_sAiDecision, "\n", sizeof(g_sAiDecision));
                SendNhaCaiLogDC(aiLog);
                continue;
            }

            if(mauNha != -1)
                format(aiLog, sizeof(aiLog),
                    "**🪝 MỒI** | `%s` | Bàn #%d\nLý do: `%s`\nCược: Thỏ $%s / Rùa $%s | Payout: $%s\n→ Ép thắng **%s** (số %d)",
                    GetPlayerNameEx(p), idban, modeLyDo,
                    number_format(cuocTho), number_format(cuocRua),
                    number_format(tongPayoutMoi),
                    (mauNha == 0) ? "Thỏ" : "Rùa", ketQua);
            else
                format(aiLog, sizeof(aiLog),
                    "**🪝 MỒI** | `%s` | Bàn #%d\nLý do: `%s`\nCược số: $%s | Payout: $%s\n→ Ép thắng **số %d**",
                    GetPlayerNameEx(p), idban, modeLyDo,
                    number_format(bestBet), number_format(tongPayoutMoi), ketQua);

            strcat(g_sAiDecision, aiLog, sizeof(g_sAiDecision));
            SendNhaCaiLogDC(aiLog);
            return ketQua;
        }

        // ============ MODE 2: NHẢ ============
        if(mode == 2)
        {
            if(PlayerInfo[p][pRoulNguong] == 0)
            {
                PlayerInfo[p][pRoulNguong] = 2 + random(3);
                PlayerInfo[p][pRoulVanThang] = 0;
            }

            new mauNha = -1;
            if(cuocTho > 0 || cuocRua > 0) {
                if(cuocTho > cuocRua)      mauNha = 0;
                else if(cuocRua > cuocTho) mauNha = 1;
                else                        mauNha = random(2);
            }

            new ketQua = -1;
            if(mauNha != -1)
                ketQua = LaySoChoThang(idban, p, mauNha);
            else
            {
                new bestNum = -1, bestBet = 0;
                for(new i = 1; i < 37; i++)
                    if(CuocHienTai[idban][p][i] > bestBet) { bestBet = CuocHienTai[idban][p][i]; bestNum = i; }
                if(bestNum == -1) continue;
                ketQua = bestNum;
            }

            new tongPayoutTarget = TinhTongPayout(idban, ketQua, true);
            new aiLog[512];

            if(tongPayoutTarget > 10000000)
            {
                format(aiLog, sizeof(aiLog),
                    "**⭐ SKIP NHẢ** | `%s` | Bàn #%d\nLý do: `%s`\nPayout target nếu ép: `$%s` > $10m → thử target khác",
                    GetPlayerNameEx(p), idban, modeLyDo, number_format(tongPayoutTarget));
                strcat(g_sAiDecision, aiLog, sizeof(g_sAiDecision));
                strcat(g_sAiDecision, "\n", sizeof(g_sAiDecision));
                SendNhaCaiLogDC(aiLog);
                continue;
            }

            if(mauNha != -1)
                format(aiLog, sizeof(aiLog),
                    "**🟢 NHẢ** | `%s` | Bàn #%d\nLý do: `%s` | Ván %d/%d\nTài sản: $%s | Profit: $%s | Payout: $%s\n→ Ép thắng **%s** (số %d)",
                    GetPlayerNameEx(p), idban, modeLyDo,
                    PlayerInfo[p][pRoulVanThang] + 1, PlayerInfo[p][pRoulNguong],
                    number_format(taiSanThuc), number_format(profit),
                    number_format(tongPayoutTarget),
                    (mauNha == 0) ? "Thỏ" : "Rùa", ketQua);
            else
                format(aiLog, sizeof(aiLog),
                    "**🟢 NHẢ** | `%s` | Bàn #%d\nLý do: `%s` | Ván %d/%d\nPayout: $%s\n→ Ép thắng **số %d**",
                    GetPlayerNameEx(p), idban, modeLyDo,
                    PlayerInfo[p][pRoulVanThang] + 1, PlayerInfo[p][pRoulNguong],
                    number_format(tongPayoutTarget), ketQua);

            strcat(g_sAiDecision, aiLog, sizeof(g_sAiDecision));
            SendNhaCaiLogDC(aiLog);
            return ketQua;
        }

        // ============ MODE 3: TUA ============
        if(mode == 3)
        {
            new mauNhieu = -1;
            if(cuocTho > 0 || cuocRua > 0) {
                if(cuocTho > cuocRua)      mauNhieu = 0;
                else if(cuocRua > cuocTho) mauNhieu = 1;
                else                        mauNhieu = random(2);
            }

            new ketQua = -1, aiLog[384];

            if(mauNhieu != -1)
            {
                ketQua = LaySoChoThua(idban, p, 1 - mauNhieu);
                if(ketQua == -1)
                {
                    format(aiLog, sizeof(aiLog),
                        "**⭐ SKIP TUA** | `%s` | Bàn #%d\nLý do: `%s`\nKhông còn số nào ở màu đối diện để ép thua → bỏ qua",
                        GetPlayerNameEx(p), idban, modeLyDo);
                    strcat(g_sAiDecision, aiLog, sizeof(g_sAiDecision));
                    strcat(g_sAiDecision, "\n", sizeof(g_sAiDecision));
                    SendNhaCaiLogDC(aiLog);
                    continue;
                }
                format(aiLog, sizeof(aiLog),
                    "**🔴 TUA** | `%s` | Bàn #%d\nLý do: `%s`\nTài sản: $%s | Cược: Thỏ $%s / Rùa $%s\n→ Ép thua, ra **%s** (số %d)",
                    GetPlayerNameEx(p), idban, modeLyDo,
                    number_format(taiSanThuc),
                    number_format(cuocTho), number_format(cuocRua),
                    (1 - mauNhieu == 0) ? "Thỏ" : "Rùa", ketQua);
            }
            else
            {
                new maxBet = 0, maxNum = -1;
                for(new i = 0; i < 37; i++)
                    if(CuocHienTai[idban][p][i] > maxBet) { maxBet = CuocHienTai[idban][p][i]; maxNum = i; }
                if(maxBet == 0 && cuocXanh == 0) continue;

                new candidates[37], count = 0;
                for(new i = 0; i < 37; i++)
                {
                    if(i == 0 && cuocXanh > 0) continue;
                    if(CuocHienTai[idban][p][i] == 0) candidates[count++] = i;
                }
                if(count == 0)
                {
                    format(aiLog, sizeof(aiLog),
                        "**⭐ SKIP TUA** | `%s` | Bàn #%d\nLý do: `%s`\nTarget cược hết mọi số → bỏ qua",
                        GetPlayerNameEx(p), idban, modeLyDo);
                    strcat(g_sAiDecision, aiLog, sizeof(g_sAiDecision));
                    strcat(g_sAiDecision, "\n", sizeof(g_sAiDecision));
                    SendNhaCaiLogDC(aiLog);
                    continue;
                }

                ketQua = candidates[random(count)];

                if(maxNum == -1)
                    format(aiLog, sizeof(aiLog),
                        "**🔴 TUA** | `%s` | Bàn #%d\nLý do: `%s`\nTài sản: $%s | Cược Xanh: $%s\n→ Ép thua, ra **số %d**",
                        GetPlayerNameEx(p), idban, modeLyDo,
                        number_format(taiSanThuc), number_format(cuocXanh), ketQua);
                else
                    format(aiLog, sizeof(aiLog),
                        "**🔴 TUA** | `%s` | Bàn #%d\nLý do: `%s`\nTài sản: $%s | Cược số %d: $%s\n→ Ép thua, ra **số %d**",
                        GetPlayerNameEx(p), idban, modeLyDo,
                        number_format(taiSanThuc), maxNum, number_format(maxBet), ketQua);
            }

            strcat(g_sAiDecision, aiLog, sizeof(g_sAiDecision));
            SendNhaCaiLogDC(aiLog);
            return ketQua;
        }
    }

    // ---- BƯỚC 4: Random ----
    new aiLog[256];
    format(aiLog, sizeof(aiLog),
        "**🎲 RANDOM** | Bàn #%d\nĐã duyệt %d target, không target nào phù hợp → random",
        idban, numTargets);
    strcat(g_sAiDecision, aiLog, sizeof(g_sAiDecision));
    SendNhaCaiLogDC(aiLog);
    return -1;
}

// Cập nhật profit AI sau mỗi kết quả
stock CapNhatNhaCai(playerid, idban, soThang)
{
    if(!IsPlayerConnected(playerid)) return;
    if(PlayerInfo[playerid][pRouletteTarget] != 1) return;

    new tongThang = 0, tongCuoc = 0;
    new mauThang = LayMauRoulette(soThang);

    if(CuocHienTai[idban][playerid][soThang] > 0)
        tongThang += CuocHienTai[idban][playerid][soThang] * 36;
    if(CuocMau[idban][playerid][mauThang] > 0)
        tongThang += CuocMau[idban][playerid][mauThang] * ((mauThang == 2) ? 36 : 2);
    for(new i = 0; i < 37; i++) tongCuoc += CuocHienTai[idban][playerid][i];
    for(new i = 0; i < 3; i++)  tongCuoc += CuocMau[idban][playerid][i];
    if(tongCuoc == 0) return;

    new loinhuan = tongThang - tongCuoc;
    if(loinhuan > 0)
    {
        new vat = loinhuan * VAT_PHAN_TRAM / 100;
        loinhuan -= vat;
    }

    new profitTruoc = PlayerInfo[playerid][pRouletteProfit];
    new bModeNha = (profitTruoc >= 0 && profitTruoc < 1000000);

    PlayerInfo[playerid][pRouletteProfit] += loinhuan;
    new profit = PlayerInfo[playerid][pRouletteProfit];

    if(bModeNha && loinhuan > 0 && PlayerInfo[playerid][pRoulNguong] > 0)
        PlayerInfo[playerid][pRoulVanThang]++;

    if(profit < 0 && profitTruoc >= 0)
    {
        PlayerInfo[playerid][pRoulNguong] = 0;
        PlayerInfo[playerid][pRoulVanThang] = 0;
    }
    else if(profit < 1000000 && profitTruoc >= 1000000)
    {
        PlayerInfo[playerid][pRoulNguong] = 0;
        PlayerInfo[playerid][pRoulVanThang] = 0;
    }
}

// ============================================================
// COMMANDS
// ============================================================
alias:taoroulette("taobanruatho")
CMD:taoroulette(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1)
    {
        SendClientMessage(playerid, COLOR_GRAD2, TIEN_TO_TIN_NHAN "{FF0000}Ban khong co quyen!");
        return 1;
    }

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    for(new i = 0; i < MAX_BAN_ROULETTE; i++)
    {
        if(BanRoulette[i][bHoatDong]) continue;

        BanRoulette[i][bX] = x;
        BanRoulette[i][bY] = y;
        BanRoulette[i][bZ] = z;
        BanRoulette[i][bHoatDong] = true;
        BanRoulette[i][bVatThe] = CreateObject(1824, x, y, z - 0.95, 0.0, 0.0, 0.0);
        BanRoulette[i][bNhan] = Create3DTextLabel("Dang tai...", 0xFFFFFFFF, x, y, z + 0.5, 10.0, 0, 0);
        BanRoulette[i][bSoThangCuoi] = -1;
        BanRoulette[i][bDangQuay] = false;
        TaoHenGioRoulette(i);
        LuuBanRoulette();

        new msg[128];
        format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{00FF00}Da tao ban quay so #{FFFF00}%d{00FF00} thanh cong!", i);
        SendClientMessage(playerid, -1, msg);

        new logMsg[512];
        format(logMsg, sizeof(logMsg),
            "**:hammer_pick: TẠO BÀN MỚI**\n**Admin:** `%s`\n**ID Bàn:** `#%d`\n**Vị trí:** `%.2f, %.2f, %.2f`",
            GetPlayerNameEx(playerid), i, x, y, z);
        SendRuaThoLogDC(logMsg);
        return 1;
    }
    SendClientMessage(playerid, COLOR_GRAD2, TIEN_TO_TIN_NHAN "{FF0000}Da dat so luong ban quay so toi da!");
    return 1;
}

alias:xoaroulette("xoabanruatho")
CMD:xoaroulette(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1)
    {
        SendClientMessage(playerid, COLOR_GRAD2, TIEN_TO_TIN_NHAN "{FF0000}Ban khong co quyen!");
        return 1;
    }

    new idban;
    if(sscanf(params, "i", idban))
    {
        SendClientMessage(playerid, COLOR_GRAD2, "{FFFFFF}SU DUNG: {FFFF00}/xoaroulette [id ban]");
        return 1;
    }

    if(idban < 0 || idban >= MAX_BAN_ROULETTE || !BanRoulette[idban][bHoatDong])
    {
        SendClientMessage(playerid, COLOR_GRAD2, TIEN_TO_TIN_NHAN "{FF0000}ID ban khong hop le!");
        return 1;
    }

    if(IsValidObject(BanRoulette[idban][bVatThe])) DestroyObject(BanRoulette[idban][bVatThe]);
    if(BanRoulette[idban][bNhan] != Text3D:INVALID_3DTEXT_ID) Delete3DTextLabel(BanRoulette[idban][bNhan]);
    if(BanRoulette[idban][bHenGio] != -1) KillTimer(BanRoulette[idban][bHenGio]);
    if(BanRoulette[idban][bHenGioDemNguoc] != -1) KillTimer(BanRoulette[idban][bHenGioDemNguoc]);
    if(BanRoulette[idban][bHenGioHieuUngQuay] != -1) KillTimer(BanRoulette[idban][bHenGioHieuUngQuay]);

    BanRoulette[idban][bHoatDong] = false;

    for(new i = 0; i < MAX_PLAYERS; i++)
    {
        if(BanCuaNguoiChoi[i] != idban) continue;
        BanCuaNguoiChoi[i] = -1;
        NguoiChoiTrongRoulette[i] = false;
        SetCameraBehindPlayer(i);
        SendClientMessage(i, COLOR_LIGHTRED, TIEN_TO_TIN_NHAN "{FF0000}Ban quay so da bi xoa boi quan tri vien!");
    }

    LuuBanRoulette();
    new msg[128];
    format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{00FF00}Da xoa ban quay so #{FFFF00}%d{00FF00}!", idban);
    SendClientMessage(playerid, -1, msg);

    new logMsg[512];
    format(logMsg, sizeof(logMsg),
        "**:wastebasket: XÓA BÀN**\n**Admin:** `%s`\n**ID Bàn:** `#%d`\n**Lý do:** `Admin xóa bàn`",
        GetPlayerNameEx(playerid), idban);
    SendRuaThoLogDC(logMsg);
    return 1;
}

CMD:buockq(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1)
    {
        SendClientMessage(playerid, COLOR_GRAD2, TIEN_TO_TIN_NHAN "{FF0000}Ban khong co quyen!");
        return 1;
    }

    new idban, param2[32];
    if(sscanf(params, "is[32]", idban, param2))
    {
        SendClientMessage(playerid, COLOR_GRAD2, "{FFFFFF}SU DUNG: {FFFF00}/buockq {FFFFFF}[id ban] [so 0-36 | tho | rua | xanh]");
        return 1;
    }

    if(idban < 0 || idban >= MAX_BAN_ROULETTE || !BanRoulette[idban][bHoatDong])
    {
        SendClientMessage(playerid, COLOR_GRAD2, TIEN_TO_TIN_NHAN "{FF0000}ID ban khong hop le!");
        return 1;
    }

    new soEp = -1;
    if(strcmp(param2, "tho", true) == 0)
        soEp = cacSoDo[random(sizeof(cacSoDo))];
    else if(strcmp(param2, "rua", true) == 0)
        soEp = cacSoDen[random(sizeof(cacSoDen))];
    else if(strcmp(param2, "xanh", true) == 0)
        soEp = 0;
    else
    {
        soEp = strval(param2);
        if(soEp < 0 || soEp > 36)
        {
            SendClientMessage(playerid, COLOR_GRAD2, TIEN_TO_TIN_NHAN "{FF0000}So khong hop le! Nhap 0-36, hoac: tho / rua / xanh");
            return 1;
        }
    }

    AdminBuocKetQua[idban] = soEp;

    new tenmau[32], mauCode[16], msg[256];
    new mau = LayMauRoulette(soEp);
    LayTenMau(mau, tenmau, sizeof(tenmau));
    MauCode(mau, mauCode, sizeof(mauCode));

    format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{FFFF00}[ADMIN] Ban #{FFFFFF}%d{FFFF00}: Ket qua tiep theo se bi ep la %s%d {FFFFFF}(%s){FFFF00}.",
        idban, mauCode, soEp, tenmau);
    SendClientMessage(playerid, -1, msg);
    return 1;
}

CMD:huybq(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1)
    {
        SendClientMessage(playerid, COLOR_GRAD2, TIEN_TO_TIN_NHAN "{FF0000}Ban khong co quyen!");
        return 1;
    }

    new idban;
    if(sscanf(params, "i", idban))
    {
        SendClientMessage(playerid, COLOR_GRAD2, "{FFFFFF}SU DUNG: {FFFF00}/huybq {FFFFFF}[id ban]");
        return 1;
    }

    if(idban < 0 || idban >= MAX_BAN_ROULETTE || !BanRoulette[idban][bHoatDong])
    {
        SendClientMessage(playerid, COLOR_GRAD2, TIEN_TO_TIN_NHAN "{FF0000}ID ban khong hop le!");
        return 1;
    }

    if(AdminBuocKetQua[idban] == -1)
    {
        SendClientMessage(playerid, COLOR_GRAD2, TIEN_TO_TIN_NHAN "{FFFF00}Ban nay dang o che do ngau nhien, khong co gi de huy.");
        return 1;
    }

    AdminBuocKetQua[idban] = -1;
    new msg[128];
    format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{FFFF00}[ADMIN] Ban #{FFFFFF}%d{FFFF00}: Da huy ep ket qua. Tra ve che do ngau nhien.", idban);
    SendClientMessage(playerid, -1, msg);
    return 1;
}

CMD:settarget(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1)
    {
        SendClientMessage(playerid, COLOR_GRAD2, TIEN_TO_TIN_NHAN "{FF0000}Ban khong co quyen!");
        return 1;
    }
    new targetid;
    if(sscanf(params, "u", targetid) || !IsPlayerConnected(targetid))
    {
        SendClientMessage(playerid, -1, "{FFFFFF}SU DUNG: {FFFF00}/settarget {FFFFFF}[playerid/ten]");
        return 1;
    }
    if(PlayerInfo[targetid][pRouletteTarget] == 1)
    {
        new msg[128];
        format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{FFFF00}%s da la muc tieu nha cai roi.", GetPlayerNameEx(targetid));
        SendClientMessage(playerid, -1, msg);
        return 1;
    }
    PlayerInfo[targetid][pRouletteTarget] = 1;
    new msg[128];
    format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{00FF00}[ADMIN] Da set {FFFF00}%s{00FF00} la muc tieu nha cai.", GetPlayerNameEx(targetid));
    SendClientMessage(playerid, -1, msg);
    return 1;
}

CMD:unsettarget(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1)
    {
        SendClientMessage(playerid, COLOR_GRAD2, TIEN_TO_TIN_NHAN "{FF0000}Ban khong co quyen!");
        return 1;
    }
    new targetid;
    if(sscanf(params, "u", targetid) || !IsPlayerConnected(targetid))
    {
        SendClientMessage(playerid, -1, "{FFFFFF}SU DUNG: {FFFF00}/unsettarget {FFFFFF}[playerid/ten]");
        return 1;
    }
    if(PlayerInfo[targetid][pRouletteTarget] != 1)
    {
        new msg[128];
        format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{FFFF00}%s khong phai muc tieu nha cai.", GetPlayerNameEx(targetid));
        SendClientMessage(playerid, -1, msg);
        return 1;
    }
    PlayerInfo[targetid][pRouletteTarget] = 0;
    PlayerInfo[targetid][pRouletteProfit] = 0;
    PlayerInfo[targetid][pRoulVanThang] = 0;
    PlayerInfo[targetid][pRoulNguong] = 0;
    new msg[128];
    format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{00FF00}[ADMIN] Da xoa {FFFF00}%s{00FF00} khoi danh sach muc tieu nha cai.", GetPlayerNameEx(targetid));
    SendClientMessage(playerid, -1, msg);
    return 1;
}

CMD:quayngay(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1)
    {
        SendClientMessage(playerid, COLOR_GRAD2, TIEN_TO_TIN_NHAN "{FF0000}Ban khong co quyen!");
        return 1;
    }

    new idban;
    if(sscanf(params, "i", idban))
    {
        SendClientMessage(playerid, COLOR_GRAD2, "{FFFFFF}SU DUNG: {FFFF00}/quayngay {FFFFFF}[id ban]");
        return 1;
    }

    if(idban < 0 || idban >= MAX_BAN_ROULETTE || !BanRoulette[idban][bHoatDong])
    {
        SendClientMessage(playerid, COLOR_GRAD2, TIEN_TO_TIN_NHAN "{FF0000}ID ban khong hop le!");
        return 1;
    }

    if(BanRoulette[idban][bDangQuay])
    {
        SendClientMessage(playerid, COLOR_GRAD2, TIEN_TO_TIN_NHAN "{FFFF00}Ban nay dang quay roi, khong the bat dau lai!");
        return 1;
    }

    if(BanRoulette[idban][bHenGio] != -1) KillTimer(BanRoulette[idban][bHenGio]);
    if(BanRoulette[idban][bHenGioDemNguoc] != -1) KillTimer(BanRoulette[idban][bHenGioDemNguoc]);
    BanRoulette[idban][bHenGio] = -1;
    BanRoulette[idban][bHenGioDemNguoc] = -1;

    HenGioQuayRoulette(idban);

    new msg[128];
    format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{FFFF00}[ADMIN] Ban #{FFFFFF}%d{FFFF00}: Da bat dau quay ngay!", idban);
    SendClientMessage(playerid, -1, msg);

    new logMsg[256];
    format(logMsg, sizeof(logMsg), "**:fast_forward: ADMIN QUAY NGAY**\n**Admin:** `%s`\n**Bàn:** `#%d`",
        GetPlayerNameEx(playerid), idban);
    SendRuaThoLogDC(logMsg);
    return 1;
}

CMD:choihtb(playerid, params[])
{
    if(NguoiChoiTrongRoulette[playerid])
    {
        SendClientMessage(playerid, COLOR_GRAD2, TIEN_TO_TIN_NHAN "{FFFF00}Ban da o trong ban quay so roi!");
        return 1;
    }

    new banGanNhat = -1;
    for(new i = 0; i < MAX_BAN_ROULETTE; i++)
    {
        if(BanRoulette[i][bHoatDong] &&
           IsPlayerInRangeOfPoint(playerid, 5.0, BanRoulette[i][bX], BanRoulette[i][bY], BanRoulette[i][bZ]))
        {
            banGanNhat = i;
            break;
        }
    }

    if(banGanNhat == -1)
    {
        SendClientMessage(playerid, COLOR_GRAD2, TIEN_TO_TIN_NHAN "{FF0000}Ban khong o gan ban quay so nao!");
        return 1;
    }

    if(BanRoulette[banGanNhat][bDangQuay])
    {
        SendClientMessage(playerid, COLOR_GRAD2, TIEN_TO_TIN_NHAN "{FFFF00}Ban quay so dang quay, vui long cho!");
        return 1;
    }

    BanCuaNguoiChoi[playerid] = banGanNhat;
    NguoiChoiTrongRoulette[playerid] = true;

    SetPlayerCameraPos(playerid, BanRoulette[banGanNhat][bX], BanRoulette[banGanNhat][bY], BanRoulette[banGanNhat][bZ] + 5.0);
    SetPlayerCameraLookAt(playerid, BanRoulette[banGanNhat][bX], BanRoulette[banGanNhat][bY], BanRoulette[banGanNhat][bZ], CAMERA_CUT);

    new msg[128];
    format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{00FF00}Chao mung den voi ban quay so {FFFF00}#%d{00FF00}!", banGanNhat);
    SendClientMessage(playerid, -1, msg);

    HienThiDialogChinh(playerid);
    return 1;
}

CMD:thoathtb(playerid, params[])
{
    if(!NguoiChoiTrongRoulette[playerid])
    {
        SendClientMessage(playerid, COLOR_GRAD2, TIEN_TO_TIN_NHAN "{FF0000}Ban khong dang choi quay so!");
        return 1;
    }

    BanCuaNguoiChoi[playerid] = -1;
    NguoiChoiTrongRoulette[playerid] = false;
    SetCameraBehindPlayer(playerid);

    SendClientMessage(playerid, -1, TIEN_TO_TIN_NHAN "{00FF00}Ban da roi khoi ban quay so!");
    return 1;
}

CMD:lichsuhtb(playerid, params[])
{
    new idban = -1;

    if(NguoiChoiTrongRoulette[playerid] && BanCuaNguoiChoi[playerid] != -1)
        idban = BanCuaNguoiChoi[playerid];
    else
    {
        for(new i = 0; i < MAX_BAN_ROULETTE; i++)
        {
            if(BanRoulette[i][bHoatDong] &&
               IsPlayerInRangeOfPoint(playerid, 10.0, BanRoulette[i][bX], BanRoulette[i][bY], BanRoulette[i][bZ]))
            {
                idban = i;
                break;
            }
        }
    }

    if(idban == -1)
    {
        SendClientMessage(playerid, COLOR_GRAD2, TIEN_TO_TIN_NHAN "{FF0000}Ban khong o gan ban quay so nao!");
        return 1;
    }

    new demTho = 0, demRua = 0, demXanh = 0;
    new lichSuSapXep[20], thoiGianSapXep[20], soLuong = 0;

    for(new i = 0; i < 20; i++)
    {
        if(BanRoulette[idban][bLichSu][i] == -1) continue;
        lichSuSapXep[soLuong] = BanRoulette[idban][bLichSu][i];
        thoiGianSapXep[soLuong] = BanRoulette[idban][bLichSuThoiGian][i];
        soLuong++;

        switch(LayMauRoulette(BanRoulette[idban][bLichSu][i]))
        {
            case 0: demTho++;
            case 1: demRua++;
            case 2: demXanh++;
        }
    }

    for(new i = 0; i < soLuong - 1; i++)
        for(new j = 0; j < soLuong - i - 1; j++)
            if(thoiGianSapXep[j] < thoiGianSapXep[j + 1])
            {
                new tT = thoiGianSapXep[j]; thoiGianSapXep[j] = thoiGianSapXep[j + 1]; thoiGianSapXep[j + 1] = tT;
                new tS = lichSuSapXep[j];   lichSuSapXep[j]   = lichSuSapXep[j + 1];   lichSuSapXep[j + 1]   = tS;
            }

    new dialog[2048], temp[256];
    format(dialog, sizeof(dialog), "{FFFF00}======= LICH SU 20 LAN QUAY =======\n\n");
    format(temp, sizeof(temp), "{FFFFFF}Ban Quay So {FFFF00}#%d\n\n", idban);
    strcat(dialog, temp, sizeof(dialog));

    if(soLuong > 0)
    {
        strcat(dialog, "{FFFF00}--- Thong Ke ---\n", sizeof(dialog));
        format(temp, sizeof(temp), "{FF6347}Tho (Do): {FFFFFF}%d lan\n", demTho);
        strcat(dialog, temp, sizeof(dialog));
        format(temp, sizeof(temp), "{A9A9A9}Rua (Den): {FFFFFF}%d lan\n", demRua);
        strcat(dialog, temp, sizeof(dialog));
        format(temp, sizeof(temp), "{90EE90}Xanh (0): {FFFFFF}%d lan\n\n", demXanh);
        strcat(dialog, temp, sizeof(dialog));
    }

    strcat(dialog, "{FFFF00}--- Lich Su Chi Tiet ---\n", sizeof(dialog));
    strcat(dialog, "{AAAAAA}(Tu moi nhat den cu nhat)\n\n", sizeof(dialog));

    if(soLuong == 0)
        strcat(dialog, "{AAAAAA}Chua co lich su quay nao!\n", sizeof(dialog));
    else
    {
        for(new i = 0; i < soLuong; i++)
        {
            new so = lichSuSapXep[i];
            new tenmau[32], mauCode[16], thoiGianText[64];
            LayTenMau(LayMauRoulette(so), tenmau, sizeof(tenmau));
            MauCode(LayMauRoulette(so), mauCode, sizeof(mauCode));
            FormatThoiGianTruoc(thoiGianSapXep[i], thoiGianText, sizeof(thoiGianText));

            format(temp, sizeof(temp), "{FFFFFF}%02d. %s%d {AAAAAA}(%s) - {CCCCCC}%s\n",
                i + 1, mauCode, so, tenmau, thoiGianText);
            strcat(dialog, temp, sizeof(dialog));
        }
    }

    strcat(dialog, "\n{FFFF00}===================================", sizeof(dialog));
    ShowPlayerDialog(playerid, DIALOG_ROULETTE_LICH_SU, DIALOG_STYLE_MSGBOX,
        "{FFFF00}Lich Su Rua Tho", dialog, "Dong", "");
    return 1;
}

CMD:checknhacai(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1)
    {
        SendClientMessage(playerid, COLOR_GRAD2, TIEN_TO_TIN_NHAN "{FF0000}Ban khong co quyen!");
        return 1;
    }

    new targetid;
    if(sscanf(params, "u", targetid) || !IsPlayerConnected(targetid))
    {
        SendClientMessage(playerid, -1, "{FFFFFF}SU DUNG: {FFFF00}/checknhacai {FFFFFF}[playerid/ten]");
        return 1;
    }

    new dialog[2048], temp[256];
    new profit    = PlayerInfo[targetid][pRouletteProfit];
    new nguong    = PlayerInfo[targetid][pRoulNguong];
    new vanThang  = PlayerInfo[targetid][pRoulVanThang];
    new isTarget  = PlayerInfo[targetid][pRouletteTarget];
    new taiSan    = PlayerInfo[targetid][pCash] + PlayerInfo[targetid][pAccount];

    new modeName[96];
    if(isTarget != 1)
        format(modeName, sizeof(modeName), "{AAAAAA}Khong theo doi");
    else if(taiSan <= 4000000)
        format(modeName, sizeof(modeName), "{00FF00}SKIP {FFFFFF}(tai san <= $4m - dat muc tieu)");
    else if(profit < -5000000)
        format(modeName, sizeof(modeName), "{FF6347}MOI {FFFFFF}(thua sau: -$%s)", number_format(-profit));
    else if(profit < 0)
        format(modeName, sizeof(modeName), "{FFFF00}RANDOM {FFFFFF}(dang am: -$%s)", number_format(-profit));
    else if(profit < 1000000)
    {
        if(nguong > 0 && vanThang >= nguong)
            format(modeName, sizeof(modeName), "{FF4500}TUA {FFFFFF}(het chu ky %d/%d van)", vanThang, nguong);
        else
            format(modeName, sizeof(modeName), "{00FF00}NHA {FFFFFF}(vua thang - van %d/%d)", vanThang, nguong);
    }
    else
        format(modeName, sizeof(modeName), "{FF0000}TUA {FFFFFF}(thang nhieu: +$%s)", number_format(profit));

    format(dialog, sizeof(dialog), "{FFFF00}========= THONG TIN NHA CAI =========\n\n");

    format(temp, sizeof(temp), "{FFFFFF}Nguoi choi:\t{FFFF00}%s {AAAAAA}(ID: %d)\n\n", GetPlayerNameEx(targetid), targetid);
    strcat(dialog, temp, sizeof(dialog));

    strcat(dialog, "{FFFF00}--- TRANG THAI AI ---\n", sizeof(dialog));
    if(isTarget == 1)
        strcat(dialog, "{FFFFFF}Muc tieu:\t\t{FF0000}CO - Dang theo doi\n", sizeof(dialog));
    else
        strcat(dialog, "{FFFFFF}Muc tieu:\t\t{00FF00}KHONG\n", sizeof(dialog));

    if(profit >= 0)
        format(temp, sizeof(temp), "{FFFFFF}Profit tich luy:\t{00FF00}+$%s\n", number_format(profit));
    else
        format(temp, sizeof(temp), "{FFFFFF}Profit tich luy:\t{FF0000}-$%s\n", number_format(-profit));
    strcat(dialog, temp, sizeof(dialog));

    format(temp, sizeof(temp), "{FFFFFF}Mode hien tai:\t%s\n\n", modeName);
    strcat(dialog, temp, sizeof(dialog));

    strcat(dialog, "{FFFF00}--- CHU KY NHA ---\n", sizeof(dialog));
    format(temp, sizeof(temp), "{FFFFFF}Nguong van:\t\t{FFFF00}%d van\n", nguong);
    strcat(dialog, temp, sizeof(dialog));
    format(temp, sizeof(temp), "{FFFFFF}Van thang HT:\t\t{FFFF00}%d / %d\n\n", vanThang, (nguong > 0 ? nguong : 0));
    strcat(dialog, temp, sizeof(dialog));

    strcat(dialog, "{FFFF00}--- TAI SAN ---\n", sizeof(dialog));
    format(temp, sizeof(temp), "{FFFFFF}Tien mat:\t\t{00FF00}$%s\n", number_format(PlayerInfo[targetid][pCash]));
    strcat(dialog, temp, sizeof(dialog));
    format(temp, sizeof(temp), "{FFFFFF}Ngan hang:\t\t{00FF00}$%s\n", number_format(PlayerInfo[targetid][pAccount]));
    strcat(dialog, temp, sizeof(dialog));
    format(temp, sizeof(temp), "{FFFFFF}Tong tai san:\t\t%s$%s\n\n",
        (taiSan <= 4000000) ? "{FF0000}" : "{FFFF00}", number_format(taiSan));
    strcat(dialog, temp, sizeof(dialog));

    new demBan = 0;
    for(new i = 0; i < MAX_BAN_ROULETTE; i++)
    {
        if(!BanRoulette[i][bHoatDong]) continue;
        new t = 0;
        for(new j = 0; j < 37; j++) t += CuocHienTai[i][targetid][j];
        for(new j = 0; j < 3; j++)  t += CuocMau[i][targetid][j];
        if(t == 0) continue;
        if(demBan == 0) strcat(dialog, "{FFFF00}--- CUOC DANG DAT ---\n", sizeof(dialog));
        format(temp, sizeof(temp), "{FFFFFF}Ban #{FFFF00}%d{FFFFFF}: {FF6347}$%s\n", i, number_format(t));
        strcat(dialog, temp, sizeof(dialog));
        demBan++;
    }
    if(demBan == 0)
        strcat(dialog, "{AAAAAA}(Khong co cuoc nao dang dat)\n", sizeof(dialog));

    strcat(dialog, "\n{FFFF00}====================================", sizeof(dialog));

    ShowPlayerDialog(playerid, DIALOG_ROULETTE_NHACAI_INFO, DIALOG_STYLE_MSGBOX,
        "{FFFF00}Thong Tin Nha Cai", dialog, "Dong", "");
    return 1;
}

CMD:setnhacai(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1)
    {
        SendClientMessage(playerid, COLOR_GRAD2, TIEN_TO_TIN_NHAN "{FF0000}Ban khong co quyen!");
        return 1;
    }

    new targetid, field[16], valueStr[32];
    if(sscanf(params, "us[16]s[32]", targetid, field, valueStr) || !IsPlayerConnected(targetid))
    {
        SendClientMessage(playerid, -1, "{FFFFFF}SU DUNG: {FFFF00}/setnhacai {FFFFFF}[playerid] [field] [gia tri]");
        SendClientMessage(playerid, -1, "{FFFF00}Fields: {FFFFFF}target{FFFF00}(0/1) | {FFFFFF}profit{FFFF00}(so) | {FFFFFF}nguong{FFFF00}(van) | {FFFFFF}vanthang{FFFF00}(van)");
        return 1;
    }

    new val = strval(valueStr);
    new msg[256];

    if(strcmp(field, "target", true) == 0)
    {
        if(val < 0 || val > 1)
        {
            SendClientMessage(playerid, -1, TIEN_TO_TIN_NHAN "{FF0000}Gia tri target phai la 0 hoac 1!");
            return 1;
        }
        PlayerInfo[targetid][pRouletteTarget] = val;
        if(val == 0)
        {
            PlayerInfo[targetid][pRouletteProfit]  = 0;
            PlayerInfo[targetid][pRoulVanThang]    = 0;
            PlayerInfo[targetid][pRoulNguong]      = 0;
        }
        format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{00FF00}[ADMIN] Set target {FFFF00}%s {00FF00}= {FFFF00}%d%s",
            GetPlayerNameEx(targetid), val, (val == 0) ? " {AAAAAA}(da reset profit/nguong/vanthang)" : "");
    }
    else if(strcmp(field, "profit", true) == 0)
    {
        PlayerInfo[targetid][pRouletteProfit] = val;
        new profitDisp[64];
        if(val >= 0) format(profitDisp, sizeof(profitDisp), "+$%s", number_format(val));
        else         format(profitDisp, sizeof(profitDisp), "-$%s", number_format(-val));
        format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{00FF00}[ADMIN] Set profit {FFFF00}%s {00FF00}= {FFFF00}%s",
            GetPlayerNameEx(targetid), profitDisp);
    }
    else if(strcmp(field, "nguong", true) == 0)
    {
        if(val < 0)
        {
            SendClientMessage(playerid, -1, TIEN_TO_TIN_NHAN "{FF0000}Nguong phai >= 0!");
            return 1;
        }
        PlayerInfo[targetid][pRoulNguong] = val;
        format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{00FF00}[ADMIN] Set nguong {FFFF00}%s {00FF00}= {FFFF00}%d van",
            GetPlayerNameEx(targetid), val);
    }
    else if(strcmp(field, "vanthang", true) == 0)
    {
        if(val < 0)
        {
            SendClientMessage(playerid, -1, TIEN_TO_TIN_NHAN "{FF0000}Van thang phai >= 0!");
            return 1;
        }
        PlayerInfo[targetid][pRoulVanThang] = val;
        format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{00FF00}[ADMIN] Set van thang {FFFF00}%s {00FF00}= {FFFF00}%d van",
            GetPlayerNameEx(targetid), val);
    }
    else
    {
        SendClientMessage(playerid, -1, "{FFFFFF}Fields hop le: {FFFF00}target{FFFFFF} | {FFFF00}profit{FFFFFF} | {FFFF00}nguong{FFFFFF} | {FFFF00}vanthang");
        return 1;
    }

    SendClientMessage(playerid, -1, msg);

    new dcMsg[512];
    format(dcMsg, sizeof(dcMsg),
        "**:pencil: ADMIN CHỈNH NHÀ CÁI**\n**Admin:** `%s`\n**Target:** `%s`\n**Field:** `%s` → `%s`",
        GetPlayerNameEx(playerid), GetPlayerNameEx(targetid), field, valueStr);
    SendNhaCaiLogDC(dcMsg);
    return 1;
}

// ============================================================
// TIMER — ĐẾM NGƯỢC
// ============================================================
public DemNguocThoiGianQuay(idban)
{
    if(idban < 0 || idban >= MAX_BAN_ROULETTE || !BanRoulette[idban][bHoatDong] || BanRoulette[idban][bDangQuay]) return;

    BanRoulette[idban][bGiayDemNguoc]--;

    if(BanRoulette[idban][bGiayDemNguoc] <= 10 && BanRoulette[idban][bGiayDemNguoc] > 0)
    {
        for(new i = 0; i < MAX_PLAYERS; i++)
        {
            if(!IsPlayerConnected(i) || BanCuaNguoiChoi[i] != idban) continue;
            new msg[128];
            if(BanRoulette[idban][bGiayDemNguoc] <= 3)
                format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{FF0000}Vong quay se bat dau trong {FFFF00}%d{FF0000} giay!", BanRoulette[idban][bGiayDemNguoc]);
            else
                format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{FFA500}Vong quay se bat dau trong {FFFF00}%d{FFA500} giay!", BanRoulette[idban][bGiayDemNguoc]);
            SendClientMessage(i, -1, msg);
        }
    }
    CapNhatNhanRoulette(idban);
}

// ============================================================
// TIMER — BẮT ĐẦU QUAY
// ============================================================
public HenGioQuayRoulette(idban)
{
    if(idban < 0 || idban >= MAX_BAN_ROULETTE || !BanRoulette[idban][bHoatDong] || BanRoulette[idban][bDangQuay]) return;

    BanRoulette[idban][bDangQuay] = true;
    BanRoulette[idban][bGiayDemNguoc] = 0;
    CapNhatNhanRoulette(idban);

    // AI decision
    g_sAiDecisionBan[idban][0] = 0;
    g_AiKetQua[idban] = ChonKetQuaNhaCai(idban);
    if(strlen(g_sAiDecision) > 0)
        strcat(g_sAiDecisionBan[idban], g_sAiDecision, sizeof(g_sAiDecisionBan[]));

    for(new pn = 0; pn < MAX_PLAYERS; pn++)
    {
        if(!IsPlayerConnected(pn)) continue;
        if(PlayerInfo[pn][pRouletteTarget] == 1) continue;
        new tongCuocPn = 0;
        for(new jn = 0; jn < 37; jn++) tongCuocPn += CuocHienTai[idban][pn][jn];
        for(new jn = 0; jn < 3; jn++)  tongCuocPn += CuocMau[idban][pn][jn];
        if(tongCuocPn == 0) continue;
        new lineNt[128];
        format(lineNt, sizeof(lineNt), ":bust_in_silhouette: `%s` → Ngẫu nhiên\n", GetPlayerNameEx(pn));
        strcat(g_sAiDecisionBan[idban], lineNt, sizeof(g_sAiDecisionBan[]));
    }

    // ===== Build log phần 1: header + stats + AI =====
    LogReset();
    new temp[LOG_CHUNK_SIZE];

    LogAdd("**:hourglass_flowing_sand: PHIÊN ĐANG QUAY**\n");
    format(temp, sizeof(temp), "**Bàn:** `#%d`\n", idban);
    LogAdd(temp);

    new soNguoiDatCuoc = 0, tongTienCuoc = 0;
    for(new i = 0; i < MAX_PLAYERS; i++)
    {
        if(!IsPlayerConnected(i)) continue;
        new t = 0;
        for(new j = 0; j < 37; j++) t += CuocHienTai[idban][i][j];
        for(new j = 0; j < 3; j++)  t += CuocMau[idban][i][j];
        if(t > 0) { soNguoiDatCuoc++; tongTienCuoc += t; }
    }

    format(temp, sizeof(temp), "**Số người chơi:** `%d`\n", soNguoiDatCuoc);
    LogAdd(temp);
    format(temp, sizeof(temp), "**Tổng tiền cược:** `$%s`\n", number_format(tongTienCuoc));
    LogAdd(temp);

    if(strlen(g_sAiDecisionBan[idban]) > 0)
    {
        LogAdd("\n**:brain: NHÀ CÁI:**\n");
        LogAdd(g_sAiDecisionBan[idban]);
    }

    if(soNguoiDatCuoc > 0)
    {
        new titleMain[64];
        format(titleMain, sizeof(titleMain), "🎲 RÙA THỎ - BÀN #%d", idban);
        SendLogSplit(titleMain, g_sLogBuf, 0xFFD700);
    }

    // ===== Build log phần 2: danh sách cược =====
    if(soNguoiDatCuoc > 0)
    {
        LogReset();
        LogAdd("**:moneybag: DANH SÁCH CƯỢC:**\n");

        new chiTietCuoc[600];
        for(new i = 0; i < MAX_PLAYERS; i++)
        {
            if(!IsPlayerConnected(i)) continue;
            new t = 0;
            for(new j = 0; j < 37; j++) t += CuocHienTai[idban][i][j];
            for(new j = 0; j < 3; j++)  t += CuocMau[idban][i][j];
            if(t == 0) continue;

            new tag[24];
            format(tag, sizeof(tag), "%s",
                (PlayerInfo[i][pRouletteTarget] == 1) ? ":dart:" : ":bust_in_silhouette:");
            format(temp, sizeof(temp), "> %s `%s` - Tổng: `$%s`\n",
                tag, GetPlayerNameEx(i), number_format(t));
            LogAdd(temp);

            chiTietCuoc[0] = 0;
            for(new j = 0; j < 37; j++)
            {
                if(CuocHienTai[idban][i][j] == 0) continue;
                format(temp, sizeof(temp), "Số %d:$%s|", j, number_format(CuocHienTai[idban][i][j]));
                strcat(chiTietCuoc, temp, sizeof(chiTietCuoc));
            }
            if(CuocMau[idban][i][0] > 0) {
                format(temp, sizeof(temp), "Thỏ:$%s|", number_format(CuocMau[idban][i][0]));
                strcat(chiTietCuoc, temp, sizeof(chiTietCuoc));
            }
            if(CuocMau[idban][i][1] > 0) {
                format(temp, sizeof(temp), "Rùa:$%s|", number_format(CuocMau[idban][i][1]));
                strcat(chiTietCuoc, temp, sizeof(chiTietCuoc));
            }
            if(CuocMau[idban][i][2] > 0) {
                format(temp, sizeof(temp), "Xanh:$%s|", number_format(CuocMau[idban][i][2]));
                strcat(chiTietCuoc, temp, sizeof(chiTietCuoc));
            }
            if(strlen(chiTietCuoc) > 0)
            {
                format(temp, sizeof(temp), "  └ `%s`\n", chiTietCuoc);
                LogAdd(temp);
            }
        }

        new titleBets[96];
        format(titleBets, sizeof(titleBets), "🎲 RÙA THỎ - BÀN #%d [CƯỢC]", idban);
        SendLogSplit(titleBets, g_sLogBuf, 0xFFD700);
    }

    // Hiệu ứng quay — 250ms thay vì 100ms để tránh dialog spam
    BanRoulette[idban][bViTriHieuUngQuay] = 0;
    if(BanRoulette[idban][bHenGioHieuUngQuay] != -1) KillTimer(BanRoulette[idban][bHenGioHieuUngQuay]);
    BanRoulette[idban][bHenGioHieuUngQuay] = SetTimerEx("HenGioHieuUngQuay", 250, true, "i", idban);

    for(new i = 0; i < MAX_PLAYERS; i++)
    {
        if(!IsPlayerConnected(i) || BanCuaNguoiChoi[i] != idban) continue;
        SendClientMessage(i, -1, TIEN_TO_TIN_NHAN "{FFFF00}==============================");
        SendClientMessage(i, -1, TIEN_TO_TIN_NHAN "{00FF00}Vong quay bat dau! {FFFFFF}Chuc ban may man!");
        SendClientMessage(i, -1, TIEN_TO_TIN_NHAN "{FFFF00}==============================");
    }
    SetTimerEx("KetThucQuay", THOI_GIAN_HIEU_UNG_QUAY, false, "i", idban);
}

public HenGioHieuUngQuay(idban)
{
    if(idban < 0 || idban >= MAX_BAN_ROULETTE || !BanRoulette[idban][bHoatDong])
    {
        if(BanRoulette[idban][bHenGioHieuUngQuay] != -1)
            KillTimer(BanRoulette[idban][bHenGioHieuUngQuay]);
        return;
    }

    BanRoulette[idban][bViTriHieuUngQuay]++;
    if(BanRoulette[idban][bViTriHieuUngQuay] >= 37)
        BanRoulette[idban][bViTriHieuUngQuay] = 0;

    for(new i = 0; i < MAX_PLAYERS; i++)
        if(IsPlayerConnected(i) && BanCuaNguoiChoi[i] == idban)
            HienThiDialogQuay(i, idban, BanRoulette[idban][bViTriHieuUngQuay], -1);
}

// ============================================================
// TIMER — KẾT THÚC QUAY
// ============================================================
public KetThucQuay(idban)
{
    if(idban < 0 || idban >= MAX_BAN_ROULETTE || !BanRoulette[idban][bHoatDong]) return;

    if(BanRoulette[idban][bHenGioHieuUngQuay] != -1)
    {
        KillTimer(BanRoulette[idban][bHenGioHieuUngQuay]);
        BanRoulette[idban][bHenGioHieuUngQuay] = -1;
    }

    // Xác định số thắng
    new soThang, vitriThang = -1;
    if(AdminBuocKetQua[idban] != -1)
    {
        soThang = AdminBuocKetQua[idban];
        AdminBuocKetQua[idban] = -1;
    }
    else if(g_AiKetQua[idban] != -1) soThang = g_AiKetQua[idban];
    else soThang = random(37);
    g_AiKetQua[idban] = -1;

    for(new i = 0; i < 37; i++)
        if(CacSoRoulette[i] == soThang) { vitriThang = i; break; }

    BanRoulette[idban][bSoThangCuoi] = soThang;
    BanRoulette[idban][bDangQuay] = false;
    BanRoulette[idban][bLichSu][BanRoulette[idban][bChiSoLichSu]] = soThang;
    BanRoulette[idban][bLichSuThoiGian][BanRoulette[idban][bChiSoLichSu]] = gettime();
    BanRoulette[idban][bChiSoLichSu]++;
    if(BanRoulette[idban][bChiSoLichSu] >= 20) BanRoulette[idban][bChiSoLichSu] = 0;

    TaoHenGioRoulette(idban);

    new mauThang = LayMauRoulette(soThang);
    new tenMauThang[32];
    LayTenMau(mauThang, tenMauThang, sizeof(tenMauThang));

    // ===== Build log phần 1: kết quả + thống kê =====
    LogReset();
    new temp[LOG_CHUNK_SIZE];
    new emojiMau[16];
    switch(mauThang)
    {
        case 0: emojiMau = ":red_circle:";
        case 1: emojiMau = ":black_circle:";
        case 2: emojiMau = ":green_circle:";
    }

    LogAdd("**:trophy: KẾT QUẢ PHIÊN QUAY**\n");
    format(temp, sizeof(temp), "**Bàn:** `#%d`\n", idban);
    LogAdd(temp);
    format(temp, sizeof(temp), "**Số trúng:** %s `%d` (%s)\n", emojiMau, soThang, tenMauThang);
    LogAdd(temp);

    // Thống kê tổng hợp
    new soNguoiThang = 0, soNguoiThua = 0, soNguoiHoa = 0;
    new tongTienThang = 0, tongTienThua = 0;

    for(new p = 0; p < MAX_PLAYERS; p++)
    {
        if(!IsPlayerConnected(p)) continue;
        new tongThang = 0, tongCuoc = 0;
        if(CuocHienTai[idban][p][soThang] > 0) tongThang += CuocHienTai[idban][p][soThang] * 36;
        if(CuocMau[idban][p][mauThang] > 0)    tongThang += CuocMau[idban][p][mauThang] * ((mauThang == 2) ? 36 : 2);
        for(new i = 0; i < 37; i++) tongCuoc += CuocHienTai[idban][p][i];
        for(new i = 0; i < 3; i++)  tongCuoc += CuocMau[idban][p][i];
        if(tongCuoc == 0) continue;

        if(tongThang > tongCuoc)       { soNguoiThang++; tongTienThang += tongThang - tongCuoc; }
        else if(tongThang == tongCuoc)   soNguoiHoa++;
        else                             { soNguoiThua++; tongTienThua += tongCuoc - tongThang; }
    }

    LogAdd("\n**:bar_chart: THỐNG KÊ:**\n");
    format(temp, sizeof(temp), "Người thắng: `%d` - Tổng: `+$%s`\n", soNguoiThang, number_format(tongTienThang));
    LogAdd(temp);
    format(temp, sizeof(temp), "Người thua: `%d` - Tổng: `-$%s`\n", soNguoiThua, number_format(tongTienThua));
    LogAdd(temp);
    format(temp, sizeof(temp), "Người hòa: `%d`\n", soNguoiHoa);
    LogAdd(temp);

    if(strlen(g_sAiDecisionBan[idban]) > 0)
    {
        LogAdd("\n**:brain: NHÀ CÁI:**\n");
        LogAdd(g_sAiDecisionBan[idban]);
        g_sAiDecisionBan[idban][0] = 0;
    }

    if(soNguoiThang > 0 || soNguoiThua > 0 || soNguoiHoa > 0)
    {
        new titleKQ[64];
        format(titleKQ, sizeof(titleKQ), "🎲 RÙA THỎ - BÀN #%d [KẾT THÚC]", idban);
        SendLogSplit(titleKQ, g_sLogBuf, 0x00FF00);
    }

    // ===== Build log phần 2: chi tiết winners / losers / ties =====
    if(soNguoiThang > 0 || soNguoiThua > 0 || soNguoiHoa > 0)
    {
        LogReset();

        if(soNguoiThang > 0)
        {
            LogAdd("**:moneybag: NGƯỜI THẮNG:**\n");
            for(new p = 0; p < MAX_PLAYERS; p++)
            {
                if(!IsPlayerConnected(p)) continue;
                new tongThang = 0, tongCuoc = 0;
                if(CuocHienTai[idban][p][soThang] > 0) tongThang += CuocHienTai[idban][p][soThang] * 36;
                if(CuocMau[idban][p][mauThang] > 0)    tongThang += CuocMau[idban][p][mauThang] * ((mauThang == 2) ? 36 : 2);
                for(new i = 0; i < 37; i++) tongCuoc += CuocHienTai[idban][p][i];
                for(new i = 0; i < 3; i++)  tongCuoc += CuocMau[idban][p][i];
                if(tongCuoc == 0 || tongThang <= tongCuoc) continue;
                format(temp, sizeof(temp), "> :white_check_mark: `%s` - Thắng: `+$%s` (Cược: $%s)\n",
                    GetPlayerNameEx(p), number_format(tongThang - tongCuoc), number_format(tongCuoc));
                LogAdd(temp);
            }
            LogAdd("\n");
        }

        if(soNguoiThua > 0)
        {
            LogAdd("**:money_with_wings: NGƯỜI THUA:**\n");
            for(new p = 0; p < MAX_PLAYERS; p++)
            {
                if(!IsPlayerConnected(p)) continue;
                new tongThang = 0, tongCuoc = 0;
                if(CuocHienTai[idban][p][soThang] > 0) tongThang += CuocHienTai[idban][p][soThang] * 36;
                if(CuocMau[idban][p][mauThang] > 0)    tongThang += CuocMau[idban][p][mauThang] * ((mauThang == 2) ? 36 : 2);
                for(new i = 0; i < 37; i++) tongCuoc += CuocHienTai[idban][p][i];
                for(new i = 0; i < 3; i++)  tongCuoc += CuocMau[idban][p][i];
                if(tongCuoc == 0 || tongThang >= tongCuoc) continue;
                format(temp, sizeof(temp), "> :x: `%s` - Thua: `-$%s` (Cược: $%s)\n",
                    GetPlayerNameEx(p), number_format(tongCuoc - tongThang), number_format(tongCuoc));
                LogAdd(temp);
            }
            LogAdd("\n");
        }

        if(soNguoiHoa > 0)
        {
            LogAdd("**:repeat: NGƯỜI HÒA:**\n");
            for(new p = 0; p < MAX_PLAYERS; p++)
            {
                if(!IsPlayerConnected(p)) continue;
                new tongThang = 0, tongCuoc = 0;
                if(CuocHienTai[idban][p][soThang] > 0) tongThang += CuocHienTai[idban][p][soThang] * 36;
                if(CuocMau[idban][p][mauThang] > 0)    tongThang += CuocMau[idban][p][mauThang] * ((mauThang == 2) ? 36 : 2);
                for(new i = 0; i < 37; i++) tongCuoc += CuocHienTai[idban][p][i];
                for(new i = 0; i < 3; i++)  tongCuoc += CuocMau[idban][p][i];
                if(tongCuoc == 0 || tongThang != tongCuoc) continue;
                format(temp, sizeof(temp), "> :arrows_counterclockwise: `%s` - Hòa: `$%s`\n",
                    GetPlayerNameEx(p), number_format(tongCuoc));
                LogAdd(temp);
            }
        }

        new titleDetails[96];
        format(titleDetails, sizeof(titleDetails), "🎲 RÙA THỎ - BÀN #%d [CHI TIẾT]", idban);
        SendLogSplit(titleDetails, g_sLogBuf, 0x00FF00);
    }

    // ===== Xử lý payout =====
    for(new p = 0; p < MAX_PLAYERS; p++)
    {
        if(!IsPlayerConnected(p)) continue;

        if(BanCuaNguoiChoi[p] == idban)
            HienThiDialogQuay(p, idban, -1, vitriThang);

        new tongThang = 0, tongCuoc = 0;
        if(CuocHienTai[idban][p][soThang] > 0) tongThang += CuocHienTai[idban][p][soThang] * 36;
        if(CuocMau[idban][p][mauThang] > 0)    tongThang += CuocMau[idban][p][mauThang] * ((mauThang == 2) ? 36 : 2);
        for(new i = 0; i < 37; i++) tongCuoc += CuocHienTai[idban][p][i];
        for(new i = 0; i < 3; i++)  tongCuoc += CuocMau[idban][p][i];

        if(tongCuoc > 0)
        {
            new vat = 0;
            if(tongThang > tongCuoc)
            {
                vat = (tongThang - tongCuoc) * VAT_PHAN_TRAM / 100;
                tongThang -= vat;
            }

            PlayerInfo[p][pCash] += tongThang;

            new msg[256], tenmau[32], mauCode[16];
            LayTenMau(mauThang, tenmau, sizeof(tenmau));
            MauCode(mauThang, mauCode, sizeof(mauCode));

            if(tongThang > tongCuoc)
            {
                new loinhuan = tongThang - tongCuoc;
                SendClientMessage(p, -1, TIEN_TO_TIN_NHAN "{00FF00}==== CHIEN THANG! ====");
                format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{FFFFFF}Ket qua: %s%d (%s)", mauCode, soThang, tenmau);
                SendClientMessage(p, -1, msg);
                format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{00FF00}Ban thang: {FFFF00}+$%s", number_format(loinhuan));
                SendClientMessage(p, -1, msg);
                format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{AAAAAA}Phi VAT %d%%: {FFFF00}-$%s", VAT_PHAN_TRAM, number_format(vat));
                SendClientMessage(p, -1, msg);
                SendClientMessage(p, -1, TIEN_TO_TIN_NHAN "{00FF00}==================");
            }
            else if(tongThang == tongCuoc)
            {
                SendClientMessage(p, -1, TIEN_TO_TIN_NHAN "{FFA500}==== HOA ====");
                format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{FFFFFF}Ket qua: %s%d (%s)", mauCode, soThang, tenmau);
                SendClientMessage(p, -1, msg);
                format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{FFA500}Hoan tien: {FFFF00}$%s", number_format(tongCuoc));
                SendClientMessage(p, -1, msg);
                SendClientMessage(p, -1, TIEN_TO_TIN_NHAN "{FFA500}==================");
            }
            else
            {
                new thua = tongCuoc - tongThang;
                SendClientMessage(p, -1, TIEN_TO_TIN_NHAN "{FF0000}==== THUA CUOC ====");
                format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{FFFFFF}Ket qua: %s%d (%s)", mauCode, soThang, tenmau);
                SendClientMessage(p, -1, msg);
                format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{FF0000}Ban thua: {FFFF00}-$%s", number_format(thua));
                SendClientMessage(p, -1, msg);
                SendClientMessage(p, -1, TIEN_TO_TIN_NHAN "{FF0000}==================");
            }
        }

        // CapNhatNhaCai cho target
        CapNhatNhaCai(p, idban, soThang);

        // Auto-target cho non-target có profit >= 3m
        if(PlayerInfo[p][pRouletteTarget] != 1 && tongCuoc > 0)
        {
            new loinhuanVan = tongThang - tongCuoc;
            if(loinhuanVan > 0)
                loinhuanVan -= loinhuanVan * VAT_PHAN_TRAM / 100;

            PlayerInfo[p][pRouletteProfit] += loinhuanVan;

            if(PlayerInfo[p][pRouletteProfit] >= 3000000)
            {
                PlayerInfo[p][pRouletteTarget] = 1;
                new autoMsg[256];
                format(autoMsg, sizeof(autoMsg),
                    TIEN_TO_TIN_NHAN "{FF8C00}[AI] %s vua duoc them vao danh sach theo doi nha cai (tong thang > $3,000,000).",
                    GetPlayerNameEx(p));
                for(new ad = 0; ad < MAX_PLAYERS; ad++)
                    if(IsPlayerConnected(ad) && PlayerInfo[ad][pAdmin] >= 2)
                        SendClientMessage(ad, -1, autoMsg);

                new dcMsg[256];
                format(dcMsg, sizeof(dcMsg),
                    "**[AUTO-TARGET]** `%s` tổng thắng > $3,000,000 → đã tự động thêm vào danh sách nhà cái.",
                    GetPlayerNameEx(p));
                SendNhaCaiLogDC(dcMsg);
            }
        }

        // Reset cược
        for(new i = 0; i < 37; i++) CuocHienTai[idban][p][i] = 0;
        for(new i = 0; i < 3; i++)  CuocMau[idban][p][i] = 0;
    }
}

// ============================================================
// HOOKS
// ============================================================
hook OnGameModeInit()
{
    for(new i = 0; i < MAX_BAN_ROULETTE; i++)
    {
        BanRoulette[i][bHoatDong] = false;
        BanRoulette[i][bHenGio] = -1;
        BanRoulette[i][bHenGioDemNguoc] = -1;
        BanRoulette[i][bHenGioHieuUngQuay] = -1;
        BanRoulette[i][bNhan] = Text3D:INVALID_3DTEXT_ID;
        BanRoulette[i][bVatThe] = INVALID_OBJECT_ID;
        BanRoulette[i][bSoThangCuoi] = -1;
        BanRoulette[i][bChiSoLichSu] = 0;
        AdminBuocKetQua[i] = -1;
        for(new j = 0; j < 20; j++)
        {
            BanRoulette[i][bLichSu][j] = -1;
            BanRoulette[i][bLichSuThoiGian][j] = 0;
        }
    }
    TaiBanRoulette();
    print(">> He Thong Quay So Rua Tho da duoc tai lai!");
    return 1;
}

hook OnGameModeExit()
{
    LuuBanRoulette();
    for(new i = 0; i < MAX_BAN_ROULETTE; i++)
    {
        if(BanRoulette[i][bHenGio] != -1) KillTimer(BanRoulette[i][bHenGio]);
        if(BanRoulette[i][bHenGioDemNguoc] != -1) KillTimer(BanRoulette[i][bHenGioDemNguoc]);
        if(BanRoulette[i][bHenGioHieuUngQuay] != -1) KillTimer(BanRoulette[i][bHenGioHieuUngQuay]);
    }
    return 1;
}

hook OnPlayerConnect(playerid)
{
    BanCuaNguoiChoi[playerid] = -1;
    NguoiChoiTrongRoulette[playerid] = false;
    DuLieuCuocCuaNguoiChoi[playerid] = 0;
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    new idban = BanCuaNguoiChoi[playerid];

    if(idban != -1 && idban < MAX_BAN_ROULETTE && BanRoulette[idban][bHoatDong])
    {
        new tongCuoc = 0;
        for(new i = 0; i < 37; i++) tongCuoc += CuocHienTai[idban][playerid][i];
        for(new i = 0; i < 3; i++)  tongCuoc += CuocMau[idban][playerid][i];

        if(tongCuoc > 0)
        {
            new logMsg[512], tempMsg[256], reasonText[32];
            switch(reason)
            {
                case 0: reasonText = "Timeout/Crash";
                case 1: reasonText = "Thoát bình thường";
                case 2: reasonText = "Kick/Ban";
                default: reasonText = "Không rõ";
            }

            format(logMsg, sizeof(logMsg), "**:warning: NGƯỜI CHƠI THOÁT KHI ĐANG CƯỢC**\n");
            format(tempMsg, sizeof(tempMsg), "**Người chơi:** `%s`\n", GetPlayerNameEx(playerid));
            strcat(logMsg, tempMsg, sizeof(logMsg));
            format(tempMsg, sizeof(tempMsg), "**Bàn:** `#%d`\n", idban);
            strcat(logMsg, tempMsg, sizeof(logMsg));
            format(tempMsg, sizeof(tempMsg), "**Lý do:** `%s`\n", reasonText);
            strcat(logMsg, tempMsg, sizeof(logMsg));
            format(tempMsg, sizeof(tempMsg), "**Tổng cược mất:** `$%s`\n", number_format(tongCuoc));
            strcat(logMsg, tempMsg, sizeof(logMsg));

            if(BanRoulette[idban][bDangQuay])
                strcat(logMsg, "_Phiên đang quay..._", sizeof(logMsg));
            else
            {
                format(tempMsg, sizeof(tempMsg), "_Phiên còn %d giây_", BanRoulette[idban][bGiayDemNguoc]);
                strcat(logMsg, tempMsg, sizeof(logMsg));
            }

            SendRuaThoLogDC(logMsg);
        }

        for(new i = 0; i < 37; i++) CuocHienTai[idban][playerid][i] = 0;
        for(new i = 0; i < 3; i++)  CuocMau[idban][playerid][i] = 0;
    }

    BanCuaNguoiChoi[playerid] = -1;
    NguoiChoiTrongRoulette[playerid] = false;
    DuLieuCuocCuaNguoiChoi[playerid] = 0;
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid)
    {
        case DIALOG_ROULETTE_CHINH:
        {
            if(!response) return PC_EmulateCommand(playerid, "/thoathtb");
            if(!NguoiChoiTrongRoulette[playerid] || BanCuaNguoiChoi[playerid] == -1) return 1;
            if(BanRoulette[BanCuaNguoiChoi[playerid]][bDangQuay])
            {
                SendClientMessage(playerid, -1, TIEN_TO_TIN_NHAN "{FFFF00}Vong quay dang quay, khong the dat cuoc!");
                return 1;
            }
            HienThiDialogLoaiCuoc(playerid);
        }
        case DIALOG_ROULETTE_LOAI_CUOC:
        {
            if(!response) return HienThiDialogChinh(playerid);
            if(!NguoiChoiTrongRoulette[playerid] || BanCuaNguoiChoi[playerid] == -1) return 1;
            switch(listitem)
            {
                case 0:
                    ShowPlayerDialog(playerid, DIALOG_ROULETTE_CUOC_SO, DIALOG_STYLE_INPUT,
                        "{FFFF00}> {FFFFFF}Dat cuoc so",
                        "{FFFFFF}Nhap so ban muon cuoc {FFFF00}(0-36){FFFFFF}:",
                        "Tiep tuc", "Huy");
                case 1..3:
                {
                    DuLieuCuocCuaNguoiChoi[playerid] = listitem;
                    ShowPlayerDialog(playerid, DIALOG_ROULETTE_SO_TIEN_CUOC, DIALOG_STYLE_INPUT,
                        "{FFFF00}> {FFFFFF}Dat cuoc",
                        "{FFFFFF}Nhap so tien ban muon cuoc:",
                        "Dat cuoc", "Huy");
                }
            }
        }
        case DIALOG_ROULETTE_CUOC_SO:
        {
            if(!response) return HienThiDialogLoaiCuoc(playerid);
            new so = strval(inputtext);
            if(so < 0 || so > 36)
            {
                SendClientMessage(playerid, -1, TIEN_TO_TIN_NHAN "{FF0000}So khong hop le! Vui long nhap {FFFF00}0-36");
                return HienThiDialogLoaiCuoc(playerid);
            }
            DuLieuCuocCuaNguoiChoi[playerid] = 100 + so;
            ShowPlayerDialog(playerid, DIALOG_ROULETTE_SO_TIEN_CUOC, DIALOG_STYLE_INPUT,
                "{FFFF00}> {FFFFFF}Dat cuoc so",
                "{FFFFFF}Nhap so tien ban muon cuoc:",
                "Dat cuoc", "Huy");
        }
        case DIALOG_ROULETTE_SO_TIEN_CUOC:
        {
            if(!response) return HienThiDialogLoaiCuoc(playerid);
            new sotien = strval(inputtext);
            if(sotien <= 0)
            {
                SendClientMessage(playerid, -1, TIEN_TO_TIN_NHAN "{FF0000}So tien khong hop le!");
                return HienThiDialogLoaiCuoc(playerid);
            }

            new idbanCheck = BanCuaNguoiChoi[playerid];
            new tongCuocHienTai = 0;
            for(new jj = 0; jj < 37; jj++) tongCuocHienTai += CuocHienTai[idbanCheck][playerid][jj];
            for(new jj = 0; jj < 3; jj++)  tongCuocHienTai += CuocMau[idbanCheck][playerid][jj];

            if(tongCuocHienTai + sotien > GIOI_HAN_THANG_MOI_LAN)
            {
                new conLai = GIOI_HAN_THANG_MOI_LAN - tongCuocHienTai;
                if(conLai <= 0)
                    SendClientMessage(playerid, -1, TIEN_TO_TIN_NHAN "{FF0000}Ban da dat toi da {FFFF00}$5,000,000 {FF0000}trong phien nay!");
                else
                {
                    new tmpMsg[128];
                    format(tmpMsg, sizeof(tmpMsg), TIEN_TO_TIN_NHAN "{FF0000}Tong cuoc khong duoc qua {FFFF00}$5,000,000{FF0000}. Con co the dat them: {FFFF00}$%s", number_format(conLai));
                    SendClientMessage(playerid, -1, tmpMsg);
                }
                return HienThiDialogLoaiCuoc(playerid);
            }
            if(sotien > PlayerInfo[playerid][pCash])
            {
                SendClientMessage(playerid, -1, TIEN_TO_TIN_NHAN "{FF0000}Ban khong du tien! {FFFFFF}Can: {FFFF00}$", number_format(sotien));
                return HienThiDialogLoaiCuoc(playerid);
            }

            new idban = BanCuaNguoiChoi[playerid];
            new loaiCuoc = DuLieuCuocCuaNguoiChoi[playerid];
            new msg[128];

            if(loaiCuoc >= 100)
            {
                new so = loaiCuoc - 100;
                if(CuocHienTai[idban][playerid][so] == 0)
                {
                    new demSoDaCuoc = 0;
                    for(new j = 0; j < 37; j++)
                        if(CuocHienTai[idban][playerid][j] > 0) demSoDaCuoc++;
                    if(demSoDaCuoc >= 10)
                    {
                        SendClientMessage(playerid, -1, TIEN_TO_TIN_NHAN "{FF0000}Ban chi duoc cuoc toi da {FFFF00}10 so khac nhau {FF0000}moi phien!");
                        SendClientMessage(playerid, -1, TIEN_TO_TIN_NHAN "{FFFFFF}(Co the cong them tien vao so da cuoc)");
                        return HienThiDialogLoaiCuoc(playerid);
                    }
                }
                CuocHienTai[idban][playerid][so] += sotien;
                format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{00FF00}Ban da dat cuoc {FFFF00}$%s {00FF00}vao so {FFFF00}%d{00FF00}!",
                    number_format(sotien), so);
            }
            else
            {
                switch(loaiCuoc)
                {
                    case 1:
                    {
                        CuocMau[idban][playerid][0] += sotien;
                        format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{00FF00}Ban da dat cuoc {FFFF00}$%s {00FF00}vao {FF6347}Tho{00FF00}!",
                            number_format(sotien));
                    }
                    case 2:
                    {
                        CuocMau[idban][playerid][1] += sotien;
                        format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{00FF00}Ban da dat cuoc {FFFF00}$%s {00FF00}vao {A9A9A9}Rua{00FF00}!",
                            number_format(sotien));
                    }
                    case 3:
                    {
                        CuocMau[idban][playerid][2] += sotien;
                        format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{00FF00}Ban da dat cuoc {FFFF00}$%s {00FF00}vao {90EE90}Xanh{00FF00}!",
                            number_format(sotien));
                    }
                }
            }

            PlayerInfo[playerid][pCash] -= sotien;
            SendClientMessage(playerid, -1, msg);
            HienThiDialogChinh(playerid);
        }
        case DIALOG_ROULETTE_QUAY:
        {
            if(NguoiChoiTrongRoulette[playerid] && BanCuaNguoiChoi[playerid] != -1)
                HienThiDialogChinh(playerid);
        }
        case DIALOG_ROULETTE_LICH_SU:
            return 1;
        case DIALOG_ROULETTE_NHACAI_INFO:
            return 1;
    }
    return 1;
}