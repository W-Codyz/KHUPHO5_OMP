#include <open.mp>
#include <YSI\YSI_Coding\y_hooks>

#define MAX_BAN_ROULETTE 20
#define KHOANG_THOI_GIAN_QUAY 70000
#define THOI_GIAN_HIEU_UNG_QUAY 5000
#define TEP_TIN_ROULETTE "Roulette_Tables.cfg"
#define TIEN_TO_TIN_NHAN "{FFFF00}GTAHTB{FFFFFF} "
#define KHOANG_THOI_GIAN_DEM_NGUOC 1000
#define GIOI_HAN_THANG_MOI_LAN 5000000
#define VAT_PHAN_TRAM 2

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
new CacSoRoulette[37] = {0,32,15,19,4,21,2,25,17,34,6,27,13,36,11,30,8,23,10,5,24,16,33,1,20,14,31,9,22,18,29,7,28,12,35,3,26};
new const cacSoDo[] = {1,3,5,7,9,12,14,16,18,19,21,23,25,27,30,32,34,36};
new AdminBuocKetQua[MAX_BAN_ROULETTE]; // -1 = ngau nhien, 0-36 = so bi ep

// RAM: reset moi session, khong can luu DB
new RoulCuocChuKy[MAX_PLAYERS];    // tong cuoc trong chu ky nha hien tai
new RoulThangChuKy[MAX_PLAYERS];   // tong loi nhuan da nha trong chu ky
new RoulThangVanTruoc[MAX_PLAYERS]; // loi nhuan van truoc (kiem tra thang lon)

static g_sAiDecision[512];                   // quyet dinh AI phien hien tai (temp)
new g_AiKetQua[MAX_BAN_ROULETTE] = {-1, ...}; // so AI chon cho moi ban (-1=random)
new g_sAiDecisionBan[MAX_BAN_ROULETTE][384];  // noi dung log AI moi ban

static const cacSoDen[] = {2,4,6,8,10,11,13,15,17,20,22,24,26,28,29,31,33,35};

#define DIALOG_ROULETTE_CHINH 9110
#define DIALOG_ROULETTE_LOAI_CUOC 9111
#define DIALOG_ROULETTE_CUOC_SO 9112
#define DIALOG_ROULETTE_SO_TIEN_CUOC 9113
#define DIALOG_ROULETTE_QUAY 9114
#define DIALOG_ROULETTE_LICH_SU 9115

forward DemNguocThoiGianQuay(idban);
forward HenGioQuayRoulette(idban);
forward OnRuaThoPhienCreated(idban);

// 1 message persistent moi ban, edit lien tuc thay vi spam message moi
new DCC_Message:BanPhienMsg[MAX_BAN_ROULETTE];
forward KetThucQuay(idban);
forward HenGioHieuUngQuay(idban);

stock LayMauRoulette(so)
{
    if(so == 0) return 2;
    for(new i = 0; i < sizeof(cacSoDo); i++)
    {
        if(so == cacSoDo[i]) return 0;
    }
    return 1;
}

stock LayTenMau(mau, ketqua[], dodai)
{
    switch(mau)
    {
        case 0: { format(ketqua, dodai, "Tho (Do)"); }
        case 1: { format(ketqua, dodai, "Rua (Den)"); }
        case 2: { format(ketqua, dodai, "Xanh"); }
        default: { format(ketqua, dodai, "Khong ro"); }
    }
}

stock SendRuaThoLogDC(const str[])
{
    new DCC_Embed:embed = DCC_CreateEmbed();
    DCC_SetEmbedTitle(embed, "🎲 RÙA THỎ - LOG");
    DCC_SetEmbedDescription(embed, str);
    DCC_SetEmbedColor(embed, 0xFFD700);
    
    // Add timestamp
    new year, month, day, hour, minute, second;
    getdate(year, month, day);
    gettime(hour, minute, second);
    new timeStr[64];
    format(timeStr, sizeof(timeStr), "%02d/%02d/%d - %02d:%02d:%02d", day, month, year, hour, minute, second);
    
    DCC_SetEmbedFooter(embed, timeStr);
    DCC_SendChannelEmbedMessage(ruathoChannel, embed);
    return 1;
}

// Gui log AI nha cai vao channel rieng
stock SendNhaCaiLogDC(const str[])
{
    if(nhaCaiChannel == DCC_INVALID_CHANNEL) return 0;
    new DCC_Embed:embed = DCC_CreateEmbed();
    DCC_SetEmbedTitle(embed, "🧠 NHÀ CÁI");
    DCC_SetEmbedDescription(embed, str);
    DCC_SetEmbedColor(embed, 0xFF4500);
    new year, month, day, hour, minute, second;
    getdate(year, month, day); gettime(hour, minute, second);
    new timeStr[64];
    format(timeStr, sizeof(timeStr), "%02d/%02d/%d - %02d:%02d:%02d", day, month, year, hour, minute, second);
    DCC_SetEmbedFooter(embed, timeStr);
    DCC_SendChannelEmbedMessage(nhaCaiChannel, embed);
    return 1;
}

// Edit hoac tao moi message phien cho ban idban
stock GuiHoacEditPhienMsg(idban, const str[])
{
    new DCC_Embed:embed = DCC_CreateEmbed();
    DCC_SetEmbedTitle(embed, "🎲 RÙA THỎ - BÀN #" );
    new title[32];
    format(title, sizeof(title), "🎲 RÙA THỎ - BÀN #%d", idban);
    DCC_SetEmbedTitle(embed, title);
    DCC_SetEmbedDescription(embed, str);
    DCC_SetEmbedColor(embed, 0xFFD700);
    new year, month, day, hour, minute, second;
    getdate(year, month, day); gettime(hour, minute, second);
    new timeStr[64];
    format(timeStr, sizeof(timeStr), "%02d/%02d/%d - %02d:%02d:%02d", day, month, year, hour, minute, second);
    DCC_SetEmbedFooter(embed, timeStr);

    if(BanPhienMsg[idban] != DCC_Message:0)
        DCC_EditMessage(BanPhienMsg[idban], "", embed);
    else
        DCC_SendChannelEmbedMessage(ruathoChannel, embed, "", "OnRuaThoPhienCreated", "i", idban);
}

public OnRuaThoPhienCreated(idban)
{
    BanPhienMsg[idban] = DCC_GetCreatedMessage();
    DCC_SetMessagePersistent(BanPhienMsg[idban], true);
}

stock KiemTraThoiGianHoatDong()
{
    return 1;
}

stock KiemTraThoiGianHoatDongChoNguoiChoi(playerid)
{
    if(PlayerInfo[playerid][pAdmin] >= 1) return 1;
    return KiemTraThoiGianHoatDong();
}

stock FormatThoiGianTruoc(timestamp, ketqua[], dodai)
{
    if(timestamp == 0)
    {
        format(ketqua, dodai, "Khong ro");
        return;
    }
    
    new hiennay = gettime();
    new chenh = hiennay - timestamp;
    
    if(chenh < 0) chenh = 0;
    
    if(chenh < 10)
    {
        format(ketqua, dodai, "{00FF00}Vua xong");
    }
    else if(chenh < 60)
    {
        format(ketqua, dodai, "%d giay truoc", chenh);
    }
    else if(chenh < 3600)
    {
        new phut = chenh / 60;
        format(ketqua, dodai, "%d phut truoc", phut);
    }
    else if(chenh < 86400)
    {
        new gio = chenh / 3600;
        format(ketqua, dodai, "%d gio truoc", gio);
    }
    else
    {
        new ngay = chenh / 86400;
        format(ketqua, dodai, "%d ngay truoc", ngay);
    }
}

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
    {
        format(nhanvanban, sizeof(nhanvanban), "{FFFF00}Ban Quay So (Rua,Tho) #%d\n{FF0000}DANG QUAY...", idban);
    }
    else
    {
        format(nhanvanban, sizeof(nhanvanban), "{FFFF00}Ban Quay So (Rua,Tho) #%d\n{FFFFFF}/choihtb de tham gia\n{00FF00}Quay sau: %d giay", idban, BanRoulette[idban][bGiayDemNguoc]);
    }
    if(BanRoulette[idban][bNhan] != Text3D:INVALID_3DTEXT_ID)
    {
        Update3DTextLabelText(BanRoulette[idban][bNhan], 0xFFFFFFFF, nhanvanban);
    }
    return 1;
}

stock LuuBanRoulette()
{
    new File:tep = fopen(TEP_TIN_ROULETTE, io_write);
    if(tep)
    {
        new chuoi[128];
        new count = 0;

        for(new i = 0; i < MAX_BAN_ROULETTE; i++)
        {
            if(BanRoulette[i][bHoatDong])
            {
                format(chuoi, sizeof(chuoi), "%.6f %.6f %.6f\r\n",
                    BanRoulette[i][bX], BanRoulette[i][bY], BanRoulette[i][bZ]);
                fwrite(tep, chuoi);
                count++;
            }
        }
        fclose(tep);
        printf("Saved %d roulette tables to file", count); // Debug
    }
    else
    {
        printf("Cannot create/write file: %s", TEP_TIN_ROULETTE); // Debug
    }
}
stock TaiBanRoulette()
{
    new File:tep = fopen(TEP_TIN_ROULETTE, io_read);
    if(tep)
    {
        new chuoi[128], Float:x, Float:y, Float:z;
        new idban = 0;

        while(fread(tep, chuoi) && idban < MAX_BAN_ROULETTE)
        {
            for(new i = 0; i < sizeof(chuoi); i++)
            {
                if(chuoi[i] == '\n' || chuoi[i] == '\r')
                {
                    chuoi[i] = '\0';
                    break;
                }
            }

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
            else
            {
                printf("Failed to parse line: %s", chuoi);
            }
        }
        fclose(tep);
        printf("Loaded %d roulette tables", idban);
    }
    else
    {
        printf("Cannot open file: %s", TEP_TIN_ROULETTE);
    }
}

stock HienThiDialogChinh(playerid)
{
    if(!NguoiChoiTrongRoulette[playerid] || BanCuaNguoiChoi[playerid] == -1) return 0;
    new idban = BanCuaNguoiChoi[playerid];
    new dialog[2048], temp[256];
    format(dialog, sizeof(dialog), "{FFFF00}======= SONG BAI RUA THO =======\n\n");
    
    if(BanRoulette[idban][bSoThangCuoi] == -1)
    {
        format(temp, sizeof(temp), "{FFFFFF}So vua trung thuong:\t{AAAAAA}Chua co\n\n");
        strcat(dialog, temp);
    }
    else
    {
        new tenmau[32], mauColor[10];
        new mau = LayMauRoulette(BanRoulette[idban][bSoThangCuoi]);
        LayTenMau(mau, tenmau, sizeof(tenmau));
        
        switch(mau)
        {
            case 0: mauColor = "{FF6347}"; // Do
            case 1: mauColor = "{A9A9A9}"; // Den
            case 2: mauColor = "{90EE90}"; // Xanh
        }
        
        format(temp, sizeof(temp), "{FFFFFF}So vua trung thuong:\t%s%d (%s)\n\n", mauColor, BanRoulette[idban][bSoThangCuoi], tenmau);
        strcat(dialog, temp);
    }

    if(BanRoulette[idban][bGiayDemNguoc] > 0 && !BanRoulette[idban][bDangQuay])
    {
        new mauThoiGian[10];
        if(BanRoulette[idban][bGiayDemNguoc] <= 10) mauThoiGian = "{FF0000}";
        else if(BanRoulette[idban][bGiayDemNguoc] <= 30) mauThoiGian = "{FFFF00}";
        else mauThoiGian = "{00FF00}";
        
        format(temp, sizeof(temp), "{FFFFFF}Thoi gian cuoc con:\t%s%d giay\n\n", mauThoiGian, BanRoulette[idban][bGiayDemNguoc]);
        strcat(dialog, temp);
    }
    else if(BanRoulette[idban][bDangQuay])
    {
        strcat(dialog, "{FF0000}Trang thai:\t\tDANG QUAY...\n\n");
    }
    else
    {
        strcat(dialog, "\n");
    }

    strcat(dialog, "{FFFF00}----------- HUONG DAN -----------\n\n");
    strcat(dialog, "{FFFFFF}Nhan nut '{00FF00}Dat Cuoc{FFFFFF}' de mo bang cuoc.\n");
    strcat(dialog, "{FFFFFF}Dung lenh {FFFF00}/lichsuhtb{FFFFFF} de xem 20 lan quay truoc.\n\n");
    strcat(dialog, "{CCCCCC}Cac loai cuoc pho bien:\n\n");
    strcat(dialog, "\t{FF6347}> Cuoc 1 So (0-36):\t{FFFF00}Ty le 1 an 35\n");
    strcat(dialog, "\t{FF6347}> Cuoc Tho (Do):\t{FFFF00}Ty le 1 an 1\n");
    strcat(dialog, "\t{A9A9A9}> Cuoc Rua (Den):\t{FFFF00}Ty le 1 an 1\n");
    strcat(dialog, "\t{90EE90}> Cuoc Xanh (so 0):\t{FFFF00}Ty le 1 an 35\n\n");
    
    // Hien thi tong cuoc hien tai
    new tongCuoc = 0;
    for(new i=0; i<37; i++) tongCuoc += CuocHienTai[idban][playerid][i];
    for(new i=0; i<3; i++) tongCuoc += CuocMau[idban][playerid][i];
    
    if(tongCuoc > 0)
    {
        format(temp, sizeof(temp), "{FFFFFF}Tong cuoc da dat:\t\t{FF6347}$%s\n", number_format(tongCuoc));
        strcat(dialog, temp);
    }
    
    format(temp, sizeof(temp), "{FFFFFF}Tien mat cua ban:\t\t{00FF00}$%s\n", number_format(PlayerInfo[playerid][pCash]));
    strcat(dialog, temp);
    
    ShowPlayerDialog(playerid, DIALOG_ROULETTE_CHINH, DIALOG_STYLE_MSGBOX, "{FFFF00}Ban Quay So Rua Tho", dialog, "Dat Cuoc", "Thoat");
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
    ShowPlayerDialog(playerid, DIALOG_ROULETTE_LOAI_CUOC, DIALOG_STYLE_LIST, "{FFFF00}Chon loai cuoc", dialog, "Chon", "Tro lai");
}

stock HienThiDialogQuay(playerid, idban, vitrihighlight, vitrithang)
{
    if(!NguoiChoiTrongRoulette[playerid] || BanCuaNguoiChoi[playerid] != idban) return 0;
    new dialog[2048], temp[64];
    strcat(dialog, "{FFFF00}========= VONG QUAY RUA THO =========\n\n");
    
    for(new i = 0; i < 37; i++)
    {
        new soHienTai = CacSoRoulette[i];
        if(vitrithang != -1 && i == vitrithang)
        {
            format(temp, sizeof(temp), "{00FF00}>> %d << ", soHienTai);
        }
        else if(vitrithang == -1 && i == vitrihighlight)
        {
            format(temp, sizeof(temp), "{FFFF00}[%d] ", soHienTai);
        }
        else
        {
            switch(LayMauRoulette(soHienTai))
            {
                case 0: { format(temp, sizeof(temp), "{FF6347}%d ", soHienTai); }
                case 1: { format(temp, sizeof(temp), "{A9A9A9}%d ", soHienTai); }
                case 2: { format(temp, sizeof(temp), "{90EE90}%d ", soHienTai); }
            }
        }
        strcat(dialog, temp);
        if((i + 1) % 12 == 0) strcat(dialog, "\n");
    }
    
    if(vitrithang != -1)
    {
        new soThang = CacSoRoulette[vitrithang];
        new tenmau[32], mauColor[10];
        new mau = LayMauRoulette(soThang);
        LayTenMau(mau, tenmau, sizeof(tenmau));
        
        switch(mau)
        {
            case 0: mauColor = "{FF6347}"; // Do
            case 1: mauColor = "{A9A9A9}"; // Den
            case 2: mauColor = "{90EE90}"; // Xanh
        }
        
        format(temp, sizeof(temp), "\n\n{FFFFFF}KET QUA: %s%d (%s){FFFFFF}", mauColor, soThang, tenmau);
        strcat(dialog, temp);
        ShowPlayerDialog(playerid, DIALOG_ROULETTE_QUAY, DIALOG_STYLE_MSGBOX, "{00FF00}Rua Tho - Ket Qua", dialog, "Dong", "");
    }
    else
    {
        strcat(dialog, "\n\n{FFFF00}Dang quay...");
        ShowPlayerDialog(playerid, DIALOG_ROULETTE_QUAY, DIALOG_STYLE_MSGBOX, "{FFFF00}Rua Tho - Dang Quay", dialog, "", "");
    }
    return 1;
}

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
        if(!BanRoulette[i][bHoatDong])
        {
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
            
            // Discord Log - Tao ban moi
            new logMsg[256], tempMsg[128];
            format(logMsg, sizeof(logMsg), "**:hammer_pick: TẠO BÀN MỚI**\n");
            format(tempMsg, sizeof(tempMsg), "**Admin:** `%s`\n", GetPlayerNameEx(playerid));
            strcat(logMsg, tempMsg);
            format(tempMsg, sizeof(tempMsg), "**ID Bàn:** `#%d`\n", i);
            strcat(logMsg, tempMsg);
            format(tempMsg, sizeof(tempMsg), "**Vị trí:** `%.2f, %.2f, %.2f`", x, y, z);
            strcat(logMsg, tempMsg);
            SendRuaThoLogDC(logMsg);
            
            return 1;
        }
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
    
    for(new i=0; i<MAX_PLAYERS; i++)
    {
        if(BanCuaNguoiChoi[i] == idban)
        {
            BanCuaNguoiChoi[i] = -1;
            NguoiChoiTrongRoulette[i] = false;
            SetCameraBehindPlayer(i);
            SendClientMessage(i, COLOR_LIGHTRED, TIEN_TO_TIN_NHAN "{FF0000}Ban quay so da bi xoa boi quan tri vien!");
        }
    }
    
    LuuBanRoulette();
    new msg[128];
    format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{00FF00}Da xoa ban quay so #{FFFF00}%d{00FF00}!", idban);
    SendClientMessage(playerid, -1, msg);
    
    // Discord Log - Xoa ban
    new logMsg[256], tempMsg[128];
    format(logMsg, sizeof(logMsg), "**:wastebasket: XÓA BÀN**\n");
    format(tempMsg, sizeof(tempMsg), "**Admin:** `%s`\n", GetPlayerNameEx(playerid));
    strcat(logMsg, tempMsg);
    format(tempMsg, sizeof(tempMsg), "**ID Bàn:** `#%d`\n", idban);
    strcat(logMsg, tempMsg);
    format(tempMsg, sizeof(tempMsg), "**Lý do:** `Admin xóa bàn`");
    strcat(logMsg, tempMsg);
    SendRuaThoLogDC(logMsg);
    
    return 1;
}

// /buockq [id_ban] [so 0-36 | tho | rua | xanh] — ep ket qua van tiep theo
CMD:buockq(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1)
    {
        SendClientMessage(playerid, COLOR_GRAD2, TIEN_TO_TIN_NHAN "{FF0000}Ban khong co quyen!");
        return 1;
    }

    new idban;
    new param2[32];
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
    {
        soEp = cacSoDo[random(sizeof(cacSoDo))];
    }
    else if(strcmp(param2, "rua", true) == 0)
    {
        new cacSoDen[18], countDen = 0;
        for(new i = 1; i <= 36; i++)
        {
            new laTho = false;
            for(new j = 0; j < sizeof(cacSoDo); j++)
                if(cacSoDo[j] == i) { laTho = true; break; }
            if(!laTho) cacSoDen[countDen++] = i;
        }
        soEp = cacSoDen[random(countDen)];
    }
    else if(strcmp(param2, "xanh", true) == 0)
    {
        soEp = 0;
    }
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
    switch(mau)
    {
        case 0: mauCode = "{FF6347}";
        case 1: mauCode = "{A9A9A9}";
        case 2: mauCode = "{90EE90}";
    }

    format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{FFFF00}[ADMIN] Ban #{FFFFFF}%d{FFFF00}: Ket qua tiep theo se bi ep la %s%d {FFFFFF}(%s){FFFF00}.", idban, mauCode, soEp, tenmau);
    SendClientMessage(playerid, -1, msg);

    return 1;
}

// /huybq [id_ban] — huy ep ket qua, tra ve ngau nhien
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

// /quayngay [id_ban] — buoc quay ngay lap tuc
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
        if(BanRoulette[i][bHoatDong] && IsPlayerInRangeOfPoint(playerid, 5.0, BanRoulette[i][bX], BanRoulette[i][bY], BanRoulette[i][bZ]))
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
    
    // Neu dang trong ban, su dung ban hien tai
    if(NguoiChoiTrongRoulette[playerid] && BanCuaNguoiChoi[playerid] != -1)
    {
        idban = BanCuaNguoiChoi[playerid];
    }
    else
    {
        // Tim ban gan nhat
        for(new i = 0; i < MAX_BAN_ROULETTE; i++)
        {
            if(BanRoulette[i][bHoatDong] && IsPlayerInRangeOfPoint(playerid, 10.0, BanRoulette[i][bX], BanRoulette[i][bY], BanRoulette[i][bZ]))
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
    
    // Dem thong ke
    new demTho = 0, demRua = 0, demXanh = 0;
    
    // Sap xep lich su tu moi nhat den cu nhat
    new lichSuSapXep[20], thoiGianSapXep[20], soLuong = 0;
    
    // Thu thap du lieu lich su
    for(new i = 0; i < 20; i++)
    {
        if(BanRoulette[idban][bLichSu][i] != -1)
        {
            lichSuSapXep[soLuong] = BanRoulette[idban][bLichSu][i];
            thoiGianSapXep[soLuong] = BanRoulette[idban][bLichSuThoiGian][i];
            soLuong++;
            
            // Dem thong ke
            new mau = LayMauRoulette(BanRoulette[idban][bLichSu][i]);
            switch(mau)
            {
                case 0: demTho++;
                case 1: demRua++;
                case 2: demXanh++;
            }
        }
    }
    
    // Sap xep theo thoi gian (bubble sort - moi nhat len dau)
    for(new i = 0; i < soLuong - 1; i++)
    {
        for(new j = 0; j < soLuong - i - 1; j++)
        {
            if(thoiGianSapXep[j] < thoiGianSapXep[j + 1])
            {
                // Swap thoi gian
                new tempTime = thoiGianSapXep[j];
                thoiGianSapXep[j] = thoiGianSapXep[j + 1];
                thoiGianSapXep[j + 1] = tempTime;
                
                // Swap ket qua
                new tempSo = lichSuSapXep[j];
                lichSuSapXep[j] = lichSuSapXep[j + 1];
                lichSuSapXep[j + 1] = tempSo;
            }
        }
    }
    
    // Xay dung dialog lich su
    new dialog[2048], temp[256];
    format(dialog, sizeof(dialog), "{FFFF00}======= LICH SU 20 LAN QUAY =======\n\n");
    format(temp, sizeof(temp), "{FFFFFF}Ban Quay So {FFFF00}#%d\n\n", idban);
    strcat(dialog, temp);
    
    // Hien thi thong ke
    if(soLuong > 0)
    {
        strcat(dialog, "{FFFF00}--- Thong Ke ---\n");
        format(temp, sizeof(temp), "{FF6347}Tho (Do): {FFFFFF}%d lan\n", demTho);
        strcat(dialog, temp);
        format(temp, sizeof(temp), "{A9A9A9}Rua (Den): {FFFFFF}%d lan\n", demRua);
        strcat(dialog, temp);
        format(temp, sizeof(temp), "{90EE90}Xanh (0): {FFFFFF}%d lan\n\n", demXanh);
        strcat(dialog, temp);
    }
    
    strcat(dialog, "{FFFF00}--- Lich Su Chi Tiet ---\n");
    strcat(dialog, "{AAAAAA}(Tu moi nhat den cu nhat)\n\n");
    
    // Hien thi lich su da sap xep
    if(soLuong == 0)
    {
        strcat(dialog, "{AAAAAA}Chua co lich su quay nao!\n");
    }
    else
    {
        for(new i = 0; i < soLuong; i++)
        {
            new so = lichSuSapXep[i];
            new mau = LayMauRoulette(so);
            new tenmau[32], mauCode[16], thoiGianText[64];
            LayTenMau(mau, tenmau, sizeof(tenmau));
            FormatThoiGianTruoc(thoiGianSapXep[i], thoiGianText, sizeof(thoiGianText));
            
            switch(mau)
            {
                case 0: mauCode = "{FF6347}"; // Do
                case 1: mauCode = "{A9A9A9}"; // Den
                case 2: mauCode = "{90EE90}"; // Xanh
            }
            
            format(temp, sizeof(temp), "{FFFFFF}%02d. %s%d {AAAAAA}(%s) - {CCCCCC}%s\n", 
                i + 1, mauCode, so, tenmau, thoiGianText);
            strcat(dialog, temp);
        }
    }
    
    strcat(dialog, "\n{FFFF00}===================================");
    ShowPlayerDialog(playerid, DIALOG_ROULETTE_LICH_SU, DIALOG_STYLE_MSGBOX, "{FFFF00}Lich Su Rua Tho", dialog, "Dong", "");
    return 1;
}

public DemNguocThoiGianQuay(idban)
{
    if(idban < 0 || idban >= MAX_BAN_ROULETTE || !BanRoulette[idban][bHoatDong] || BanRoulette[idban][bDangQuay]) return;
    
    BanRoulette[idban][bGiayDemNguoc]--;
    
    if(BanRoulette[idban][bGiayDemNguoc] <= 10 && BanRoulette[idban][bGiayDemNguoc] > 0)
    {
        for(new i = 0; i < MAX_PLAYERS; i++)
        {
            if(IsPlayerConnected(i) && BanCuaNguoiChoi[i] == idban)
            {
                new msg[128];
                if(BanRoulette[idban][bGiayDemNguoc] <= 3)
                {
                    format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{FF0000}Vong quay se bat dau trong {FFFF00}%d{FF0000} giay!", BanRoulette[idban][bGiayDemNguoc]);
                }
                else
                {
                    format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{FFA500}Vong quay se bat dau trong {FFFF00}%d{FFA500} giay!", BanRoulette[idban][bGiayDemNguoc]);
                }
                SendClientMessage(i, -1, msg);
            }
        }
    }
    CapNhatNhanRoulette(idban);
}

public HenGioQuayRoulette(idban)
{
    if(idban < 0 || idban >= MAX_BAN_ROULETTE || !BanRoulette[idban][bHoatDong] || BanRoulette[idban][bDangQuay]) return;
    
    BanRoulette[idban][bDangQuay] = true;
    BanRoulette[idban][bGiayDemNguoc] = 0;
    CapNhatNhanRoulette(idban);
    
    // Tinh AI truoc de hien thi trong log phien
    g_sAiDecisionBan[idban][0] = 0;
    g_AiKetQua[idban] = ChonKetQuaNhaCai(idban);
    if(strlen(g_sAiDecision) > 0)
        strcat(g_sAiDecisionBan[idban], g_sAiDecision);

    // Them nguoi thuong (non-target co cuoc) vao phan AI
    for(new pn = 0; pn < MAX_PLAYERS; pn++)
    {
        if(!IsPlayerConnected(pn)) continue;
        if(PlayerInfo[pn][pRouletteTarget] == 1) continue;
        new tongCuocPn = 0;
        for(new jn = 0; jn < 37; jn++) tongCuocPn += CuocHienTai[idban][pn][jn];
        for(new jn = 0; jn < 3; jn++) tongCuocPn += CuocMau[idban][pn][jn];
        if(tongCuocPn == 0) continue;
        new lineNt[128];
        format(lineNt, sizeof(lineNt), ":bust_in_silhouette: `%s` → Ngẫu nhiên\n", GetPlayerNameEx(pn));
        strcat(g_sAiDecisionBan[idban], lineNt);
    }

    // Discord Log - Phien moi bat dau
    new logMsg[1536], tempMsg[256];
    format(logMsg, sizeof(logMsg), "**:hourglass_flowing_sand: PHIÊN ĐANG QUAY**\n");
    format(tempMsg, sizeof(tempMsg), "**Bàn:** `#%d`\n", idban);
    strcat(logMsg, tempMsg);
    
    // Dem so nguoi choi dat cuoc
    new soNguoiDatCuoc = 0, tongTienCuoc = 0;
    for(new i = 0; i < MAX_PLAYERS; i++)
    {
        if(!IsPlayerConnected(i)) continue;
        new tongCuocNguoiChoi = 0;
        for(new j=0; j<37; j++) tongCuocNguoiChoi += CuocHienTai[idban][i][j];
        for(new j=0; j<3; j++) tongCuocNguoiChoi += CuocMau[idban][i][j];
        if(tongCuocNguoiChoi > 0) 
        {
            soNguoiDatCuoc++;
            tongTienCuoc += tongCuocNguoiChoi;
        }
    }
    
    format(tempMsg, sizeof(tempMsg), "**Số người chơi:** `%d`\n", soNguoiDatCuoc);
    strcat(logMsg, tempMsg);
    format(tempMsg, sizeof(tempMsg), "**Tổng tiền cược:** `$%s`\n\n", number_format(tongTienCuoc));
    strcat(logMsg, tempMsg);
    
    // Chi tiet nguoi choi dat cuoc
    if(soNguoiDatCuoc > 0)
    {
        strcat(logMsg, "**:moneybag: DANH SÁCH CƯỢC:**\n");
        for(new i = 0; i < MAX_PLAYERS; i++)
        {
            if(!IsPlayerConnected(i)) continue;
            new tongCuocNguoiChoi = 0;
            for(new j=0; j<37; j++) tongCuocNguoiChoi += CuocHienTai[idban][i][j];
            for(new j=0; j<3; j++) tongCuocNguoiChoi += CuocMau[idban][i][j];
            
            if(tongCuocNguoiChoi > 0)
            {
                new tagCuoc[24];
                format(tagCuoc, sizeof(tagCuoc), "%s", (PlayerInfo[i][pRouletteTarget] == 1) ? ":dart:" : ":bust_in_silhouette:");
                format(tempMsg, sizeof(tempMsg), "> %s `%s` - Tổng cược: `$%s`\n", tagCuoc, GetPlayerNameEx(i), number_format(tongCuocNguoiChoi));
                strcat(logMsg, tempMsg);
                
                // Chi tiet cuoc
                new chiTietCuoc[256] = "";
                for(new j=0; j<37; j++) 
                {
                    if(CuocHienTai[idban][i][j] > 0)
                    {
                        format(tempMsg, sizeof(tempMsg), "Số %d: $%s | ", j, number_format(CuocHienTai[idban][i][j]));
                        strcat(chiTietCuoc, tempMsg);
                    }
                }
                if(CuocMau[idban][i][0] > 0)
                {
                    format(tempMsg, sizeof(tempMsg), "Thỏ: $%s | ", number_format(CuocMau[idban][i][0]));
                    strcat(chiTietCuoc, tempMsg);
                }
                if(CuocMau[idban][i][1] > 0)
                {
                    format(tempMsg, sizeof(tempMsg), "Rùa: $%s | ", number_format(CuocMau[idban][i][1]));
                    strcat(chiTietCuoc, tempMsg);
                }
                if(CuocMau[idban][i][2] > 0)
                {
                    format(tempMsg, sizeof(tempMsg), "Xanh: $%s | ", number_format(CuocMau[idban][i][2]));
                    strcat(chiTietCuoc, tempMsg);
                }
                if(strlen(chiTietCuoc) > 0)
                {
                    format(tempMsg, sizeof(tempMsg), "  └ `%s`\n", chiTietCuoc);
                    strcat(logMsg, tempMsg);
                }
            }
        }
    }
    else
    {
        strcat(logMsg, "_Không có người chơi đặt cược trong phiên này._\n");
    }

    // AI decision (tinh truoc de hien thi ngay)
    if(strlen(g_sAiDecisionBan[idban]) > 0)
    {
        strcat(logMsg, "\n**:brain: NHÀ CÁI:**\n");
        strcat(logMsg, g_sAiDecisionBan[idban]);
    }

    GuiHoacEditPhienMsg(idban, logMsg);

    BanRoulette[idban][bViTriHieuUngQuay] = 0;
    if(BanRoulette[idban][bHenGioHieuUngQuay] != -1) KillTimer(BanRoulette[idban][bHenGioHieuUngQuay]);
    BanRoulette[idban][bHenGioHieuUngQuay] = SetTimerEx("HenGioHieuUngQuay", 100, true, "i", idban);
    
    for(new i = 0; i < MAX_PLAYERS; i++)
    {
        if(IsPlayerConnected(i) && BanCuaNguoiChoi[i] == idban)
        {
            SendClientMessage(i, -1, TIEN_TO_TIN_NHAN "{FFFF00}==============================");
            SendClientMessage(i, -1, TIEN_TO_TIN_NHAN "{00FF00}Vong quay bat dau! {FFFFFF}Chuc ban may man!");
            SendClientMessage(i, -1, TIEN_TO_TIN_NHAN "{FFFF00}==============================");
        }
    }
    SetTimerEx("KetThucQuay", THOI_GIAN_HIEU_UNG_QUAY, false, "i", idban);
}

public HenGioHieuUngQuay(idban)
{
    if(idban < 0 || idban >= MAX_BAN_ROULETTE || !BanRoulette[idban][bHoatDong])
    {
        if(BanRoulette[idban][bHenGioHieuUngQuay] != -1) KillTimer(BanRoulette[idban][bHenGioHieuUngQuay]);
        return;
    }
    BanRoulette[idban][bViTriHieuUngQuay]++;
    if(BanRoulette[idban][bViTriHieuUngQuay] >= 37)
    {
        BanRoulette[idban][bViTriHieuUngQuay] = 0;
    }
    for(new i = 0; i < MAX_PLAYERS; i++)
    {
        if(IsPlayerConnected(i) && BanCuaNguoiChoi[i] == idban)
        {
            HienThiDialogQuay(i, idban, BanRoulette[idban][bViTriHieuUngQuay], -1);
        }
    }
}

// Tra ve so ngau nhien thuoc mau chi dinh (0=Tho/Do, 1=Rua/Den)
// Khong xu ly Xanh vi khong bao gio ep Xanh
stock LaySoTheoMau(mau)
{
    if(mau == 0) return cacSoDo[random(18)];
    return cacSoDen[random(18)];
}

// Quyet dinh ket qua AI nha cai. Tra ve so 0-36 neu co ep, -1 neu de random.
stock ChonKetQuaNhaCai(idban)
{
    g_sAiDecision[0] = 0; // reset quyet dinh cu

    // Tim player muc tieu co cuoc tren ban, uu tien nguoi profit thap nhat
    new priorityPlayer = -1;
    new minProfit = 0x7FFFFFFF;

    for(new p = 0; p < MAX_PLAYERS; p++)
    {
        if(!IsPlayerConnected(p)) continue;
        if(PlayerInfo[p][pRouletteTarget] != 1) continue;

        new tongCuoc = 0;
        for(new i = 0; i < 37; i++) tongCuoc += CuocHienTai[idban][p][i];
        for(new i = 0; i < 3; i++) tongCuoc += CuocMau[idban][p][i];
        if(tongCuoc == 0) continue; // khong co cuoc tren ban nay → bo qua

        if(PlayerInfo[p][pRouletteProfit] < minProfit)
        {
            minProfit = PlayerInfo[p][pRouletteProfit];
            priorityPlayer = p;
        }
    }

    if(priorityPlayer == -1) return -1;

    new p = priorityPlayer;

    // Neu tai san thuc da xuong duoi 4m → ngung can thiep, ve random
    new taiSanThuc = GetPlayerCash(p) + PlayerInfo[p][pAccount];
    if(taiSanThuc <= 4000000)
    {
        new aiLog[256];
        format(aiLog, sizeof(aiLog),
            "**⛔ DỪNG** | `%s` | Bàn #%d\nTài sản: `$%s` ≤ 4m → Đã đạt mục tiêu, về random.",
            GetPlayerNameEx(p), idban, number_format(taiSanThuc));
        strcat(g_sAiDecision, aiLog);
        SendNhaCaiLogDC(aiLog);
        return -1;
    }

    new profit = PlayerInfo[p][pRouletteProfit];

    // Tinh tong cuoc mau (chi Tho/Rua) cua player
    new cuocTho = CuocMau[idban][p][0];
    new cuocRua = CuocMau[idban][p][1];
    new tongCuocVan = 0;
    for(new i = 0; i < 37; i++) tongCuocVan += CuocHienTai[idban][p][i];
    for(new i = 0; i < 3; i++) tongCuocVan += CuocMau[idban][p][i];

    // --- Xac dinh mode ---
    // 0=Random (khong can thiep), 1=MOI khan cap (<-5m), 2=NHA (0 den 1m), 3=TUA (>=1m)
    new mode;
    if(profit < -5000000)
        mode = 1; // MOI khan cap: thua qua sau, cho thang 1 lan de giu nguoi choi
    else if(profit < 0)
        mode = 0; // Random: dang thua, de RNG tu nhien (house edge 2.7%)
    else if(profit < 1000000)
        mode = 2; // NHA: vua bat dau thang, cho thang de giu chan
    else
        mode = 3; // TUA: thang qua nhieu, an lai

    new modeLyDo[64];
    if(mode == 1) format(modeLyDo, _, "thua sâu ($%s)", number_format(profit));
    else if(mode == 0) format(modeLyDo, _, "đang âm, để random ($%s)", number_format(profit));
    else if(mode == 2) format(modeLyDo, _, "vừa thắng ($%s)", number_format(profit));
    else format(modeLyDo, _, "thắng nhiều ($%s)", number_format(profit));

    // Kiem tra het chu ky nha → chuyen TUA
    if(mode == 2 && PlayerInfo[p][pRoulNguong] > 0 && PlayerInfo[p][pRoulVanThang] >= PlayerInfo[p][pRoulNguong])
    {
        mode = 3;
        format(modeLyDo, _, "hết chu kỳ (%d/%d ván)", PlayerInfo[p][pRoulVanThang], PlayerInfo[p][pRoulNguong]);
    }

    // Mode 0: de random, khong can thiep
    if(mode == 0) return -1;

    // --- MODE MOI: ep thang 1 van, bo qua budget ---
    if(mode == 1)
    {
        new mauNha = -1;
        if(cuocTho >= cuocRua && cuocTho > 0) mauNha = 0;
        else if(cuocRua > 0) mauNha = 1;
        if(mauNha == -1) return -1;
        new ketQua = LaySoTheoMau(mauNha);
        new aiLog[256];
        format(aiLog, sizeof(aiLog),
            "**🪝 MỒI** | `%s` | Bàn #%d\nLý do: `%s`\nCược: Thỏ $%s / Rùa $%s\n→ Ép thắng **%s** (số %d)",
            GetPlayerNameEx(p), idban, modeLyDo,
            number_format(cuocTho), number_format(cuocRua),
            (mauNha == 0) ? "Thỏ" : "Rùa", ketQua);
        strcat(g_sAiDecision, aiLog);
        SendNhaCaiLogDC(aiLog);
        return ketQua;
    }

    // --- MODE NHA: ep thang, kiem tra budget 30% ---
    if(mode == 2)
    {
        if(PlayerInfo[p][pRoulNguong] == 0)
        {
            PlayerInfo[p][pRoulNguong] = 2 + random(3);
            PlayerInfo[p][pRoulVanThang] = 0;
            RoulCuocChuKy[p] = 0;
            RoulThangChuKy[p] = 0;
        }

        new mauNha = -1;
        if(cuocTho >= cuocRua && cuocTho > 0) mauNha = 0;
        else if(cuocRua > 0) mauNha = 1;
        if(mauNha == -1) return -1;

        new ketQua = LaySoTheoMau(mauNha);
        new aiLog[384];
        format(aiLog, sizeof(aiLog),
            "**🟢 NHẢ** | `%s` | Bàn #%d\nLý do: `%s` | Ván %d/%d\nTài sản: $%s | Profit: $%s\n→ Ép thắng **%s** (số %d)",
            GetPlayerNameEx(p), idban, modeLyDo,
            PlayerInfo[p][pRoulVanThang] + 1, PlayerInfo[p][pRoulNguong],
            number_format(taiSanThuc), number_format(profit),
            (mauNha == 0) ? "Thỏ" : "Rùa", ketQua);
        strcat(g_sAiDecision, aiLog);
        SendNhaCaiLogDC(aiLog);
        return ketQua;
    }

    // --- MODE TUA: ep thua ---
    if(mode == 3)
    {
        new mauNhieu = -1;
        if(cuocTho > 0 || cuocRua > 0)
            mauNhieu = (cuocTho >= cuocRua) ? 0 : 1;

        if(mauNhieu == -1) return -1;

        new ketQua = LaySoTheoMau(1 - mauNhieu);
        new aiLog[256];
        format(aiLog, sizeof(aiLog),
            "**🔴 TUA** | `%s` | Bàn #%d\nLý do: `%s`\nTài sản: $%s | Cược: Thỏ $%s / Rùa $%s\n→ Ép thua, ra **%s** (số %d)",
            GetPlayerNameEx(p), idban, modeLyDo,
            number_format(taiSanThuc),
            number_format(cuocTho), number_format(cuocRua),
            (1 - mauNhieu == 0) ? "Thỏ" : "Rùa", ketQua);
        strcat(g_sAiDecision, aiLog);
        SendNhaCaiLogDC(aiLog);
        return ketQua;
    }

    return -1;
}

// Cap nhat trang thai AI sau moi ket qua quay
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
    for(new i = 0; i < 3; i++) tongCuoc += CuocMau[idban][playerid][i];
    if(tongCuoc == 0) return;

    new loinhuan = tongThang - tongCuoc; // co the am neu thua

    // Ap dung VAT 2% tren loi nhuan thuc (profit tracking dung so thuc nguoi choi nhan)
    if(loinhuan > 0)
    {
        new vat = loinhuan * VAT_PHAN_TRAM / 100;
        loinhuan -= vat;
    }

    // Xac dinh mode truoc khi cap nhat profit
    new profitTruoc = PlayerInfo[playerid][pRouletteProfit];
    new bModeNha = (profitTruoc >= 0 && profitTruoc < 1000000);

    // Cap nhat RAM budget chu ky
    RoulCuocChuKy[playerid] += tongCuoc;
    RoulThangVanTruoc[playerid] = (loinhuan > 0) ? loinhuan : 0;

    // Cap nhat profit tich luy
    PlayerInfo[playerid][pRouletteProfit] += loinhuan;
    new profit = PlayerInfo[playerid][pRouletteProfit];

    // Cap nhat dem van thang chu ky (chi khi dang NHA va thang mau)
    if(bModeNha && loinhuan > 0 && PlayerInfo[playerid][pRoulNguong] > 0)
    {
        RoulThangChuKy[playerid] += loinhuan;
        PlayerInfo[playerid][pRoulVanThang]++;
    }

    // Khi profit xuong duoi 0 (roi khoi vung NHA/TUA ve Random/MOI) → reset chu ky
    if(profit < 0 && profitTruoc >= 0)
    {
        PlayerInfo[playerid][pRoulNguong] = 0;
        PlayerInfo[playerid][pRoulVanThang] = 0;
        RoulCuocChuKy[playerid] = 0;
        RoulThangChuKy[playerid] = 0;
    }

    // Luu DB ngay lap tuc
    new szQuery[256];
    format(szQuery, sizeof(szQuery),
        "UPDATE `accounts` SET `RouletteProfit`=%d, `RoulVanThang`=%d, `RoulNguong`=%d WHERE `id`=%d",
        PlayerInfo[playerid][pRouletteProfit],
        PlayerInfo[playerid][pRoulVanThang],
        PlayerInfo[playerid][pRoulNguong],
        GetPlayerSQLId(playerid));
    mysql_function_query(MainPipeline, szQuery, false, "OnQueryFinish", "ii", SENDDATA_THREAD, INVALID_PLAYER_ID);
}

public KetThucQuay(idban)
{
    if(idban < 0 || idban >= MAX_BAN_ROULETTE || !BanRoulette[idban][bHoatDong]) return;
    
    if(BanRoulette[idban][bHenGioHieuUngQuay] != -1)
    {
        KillTimer(BanRoulette[idban][bHenGioHieuUngQuay]);
        BanRoulette[idban][bHenGioHieuUngQuay] = -1;
    }
    
    new soThang, vitriThang = -1;
    if(AdminBuocKetQua[idban] != -1)
    {
        soThang = AdminBuocKetQua[idban];
        AdminBuocKetQua[idban] = -1;
    }
    else if(g_AiKetQua[idban] != -1)
    {
        soThang = g_AiKetQua[idban];
    }
    else
    {
        new randSo = random(47);
        soThang = (randSo >= 37) ? 0 : randSo;
    }
    g_AiKetQua[idban] = -1; // reset cho phien sau
    for(new i=0; i<37; i++)
    {
        if(CacSoRoulette[i] == soThang)
        {
            vitriThang = i;
            break;
        }
    }
    
    BanRoulette[idban][bSoThangCuoi] = soThang;
    BanRoulette[idban][bDangQuay] = false;
    
    // Luu vao lich su
    BanRoulette[idban][bLichSu][BanRoulette[idban][bChiSoLichSu]] = soThang;
    BanRoulette[idban][bLichSuThoiGian][BanRoulette[idban][bChiSoLichSu]] = gettime();
    BanRoulette[idban][bChiSoLichSu]++;
    if(BanRoulette[idban][bChiSoLichSu] >= 20) BanRoulette[idban][bChiSoLichSu] = 0;
    
    TaoHenGioRoulette(idban);
    
    new mauThang = LayMauRoulette(soThang);
    new tenMauThang[32];
    LayTenMau(mauThang, tenMauThang, sizeof(tenMauThang));
    
    // Discord Log - Ket qua phien
    new logMsg[3072], tempMsg[256];
    format(logMsg, sizeof(logMsg), "**:trophy: KẾT QUẢ PHIÊN QUAY**\n");
    format(tempMsg, sizeof(tempMsg), "**Bàn:** `#%d`\n", idban);
    strcat(logMsg, tempMsg);

    new emojiMau[16];
    switch(mauThang)
    {
        case 0: emojiMau = ":red_circle:"; // Do
        case 1: emojiMau = ":black_circle:"; // Den
        case 2: emojiMau = ":green_circle:"; // Xanh
    }
    format(tempMsg, sizeof(tempMsg), "**Số trúng:** %s `%d` (%s)\n\n", emojiMau, soThang, tenMauThang);
    strcat(logMsg, tempMsg);

    // Danh sach cuoc dau phien (truoc khi reset)
    new dsCuocPhien[1024] = "";
    new coCuocPhien = 0;
    for(new pc = 0; pc < MAX_PLAYERS; pc++)
    {
        if(!IsPlayerConnected(pc)) continue;
        new tongCuocPC = 0;
        for(new jj = 0; jj < 37; jj++) tongCuocPC += CuocHienTai[idban][pc][jj];
        for(new jj = 0; jj < 3; jj++) tongCuocPC += CuocMau[idban][pc][jj];
        if(tongCuocPC == 0) continue;
        coCuocPhien = 1;
        format(tempMsg, sizeof(tempMsg), "> `%s` — `$%s`", GetPlayerNameEx(pc), number_format(tongCuocPC));
        strcat(dsCuocPhien, tempMsg);
        new chiTietMau[128] = "";
        if(CuocMau[idban][pc][0] > 0) { format(tempMsg, sizeof(tempMsg), " | Thỏ:$%s", number_format(CuocMau[idban][pc][0])); strcat(chiTietMau, tempMsg); }
        if(CuocMau[idban][pc][1] > 0) { format(tempMsg, sizeof(tempMsg), " | Rùa:$%s", number_format(CuocMau[idban][pc][1])); strcat(chiTietMau, tempMsg); }
        if(CuocMau[idban][pc][2] > 0) { format(tempMsg, sizeof(tempMsg), " | Xanh:$%s", number_format(CuocMau[idban][pc][2])); strcat(chiTietMau, tempMsg); }
        strcat(dsCuocPhien, chiTietMau);
        strcat(dsCuocPhien, "\n");
    }
    if(coCuocPhien)
    {
        strcat(logMsg, "**:notepad_spiral: DANH SÁCH CƯỢC:**\n");
        strcat(logMsg, dsCuocPhien);
        strcat(logMsg, "\n");
    }

    // Thong ke nguoi thang/thua
    new soNguoiThang = 0, soNguoiThua = 0, soNguoiHoa = 0;
    new tongTienThang = 0, tongTienThua = 0;
    
    new dsNguoiThang[1024] = "", dsNguoiThua[1024] = "", dsNguoiHoa[512] = "";
    
    for(new p = 0; p < MAX_PLAYERS; p++)
    {
        if(!IsPlayerConnected(p)) continue;
        
        new tongThang = 0, tongCuoc = 0;
        
        if(CuocHienTai[idban][p][soThang] > 0) 
            tongThang += CuocHienTai[idban][p][soThang] * 36;
            
        if(CuocMau[idban][p][mauThang] > 0) 
            tongThang += CuocMau[idban][p][mauThang] * ((mauThang == 2) ? 36 : 2);

        for(new i=0; i<37; i++) tongCuoc += CuocHienTai[idban][p][i];
        for(new i=0; i<3; i++) tongCuoc += CuocMau[idban][p][i];

        if(tongCuoc > 0)
        {
            if(tongThang > tongCuoc + GIOI_HAN_THANG_MOI_LAN) tongThang = tongCuoc + GIOI_HAN_THANG_MOI_LAN;
            if(tongThang > tongCuoc)
            {
                new loinhuan = tongThang - tongCuoc;
                soNguoiThang++;
                tongTienThang += loinhuan;
                format(tempMsg, sizeof(tempMsg), "> :white_check_mark: `%s` - Thắng: `+$%s` (Cược: $%s)\n",
                    GetPlayerNameEx(p), number_format(loinhuan), number_format(tongCuoc));
                strcat(dsNguoiThang, tempMsg);
            }
            else if(tongThang == tongCuoc)
            {
                soNguoiHoa++;
                format(tempMsg, sizeof(tempMsg), "> :arrows_counterclockwise: `%s` - Hòa: `$%s`\n", 
                    GetPlayerNameEx(p), number_format(tongCuoc));
                strcat(dsNguoiHoa, tempMsg);
            }
            else
            {
                new tienThua = tongCuoc - tongThang;
                soNguoiThua++;
                tongTienThua += tienThua;
                format(tempMsg, sizeof(tempMsg), "> :x: `%s` - Thua: `-$%s` (Cược: $%s)\n", 
                    GetPlayerNameEx(p), number_format(tienThua), number_format(tongCuoc));
                strcat(dsNguoiThua, tempMsg);
            }
        }
    }
    
    // Tong hop thong ke
    format(tempMsg, sizeof(tempMsg), "**:bar_chart: THỐNG KÊ:**\n");
    strcat(logMsg, tempMsg);
    format(tempMsg, sizeof(tempMsg), "Người thắng: `%d` - Tổng: `+$%s`\n", soNguoiThang, number_format(tongTienThang));
    strcat(logMsg, tempMsg);
    format(tempMsg, sizeof(tempMsg), "Người thua: `%d` - Tổng: `-$%s`\n", soNguoiThua, number_format(tongTienThua));
    strcat(logMsg, tempMsg);
    format(tempMsg, sizeof(tempMsg), "Người hòa: `%d`\n\n", soNguoiHoa);
    strcat(logMsg, tempMsg);
    
    // Chi tiet nguoi thang
    if(soNguoiThang > 0)
    {
        strcat(logMsg, "**:moneybag: NGƯỜI THẮNG:**\n");
        strcat(logMsg, dsNguoiThang);
        strcat(logMsg, "\n");
    }
    
    // Chi tiet nguoi thua
    if(soNguoiThua > 0)
    {
        strcat(logMsg, "**:money_with_wings: NGƯỜI THUA:**\n");
        strcat(logMsg, dsNguoiThua);
        strcat(logMsg, "\n");
    }
    
    // Chi tiet nguoi hoa
    if(soNguoiHoa > 0)
    {
        strcat(logMsg, "**:repeat: NGƯỜI HÒA:**\n");
        strcat(logMsg, dsNguoiHoa);
    }
    
    if(soNguoiThang == 0 && soNguoiThua == 0 && soNguoiHoa == 0)
    {
        strcat(logMsg, "_Không có người chơi trong phiên này._\n");
    }

    // AI decision (tinh tu dau phien)
    if(strlen(g_sAiDecisionBan[idban]) > 0)
    {
        strcat(logMsg, "\n**:brain: NHÀ CÁI:**\n");
        strcat(logMsg, g_sAiDecisionBan[idban]);
        g_sAiDecisionBan[idban][0] = 0;
    }

    // Edit message phien, sau khi hien thi ket qua se reset de phien moi tao lai
    GuiHoacEditPhienMsg(idban, logMsg);
    BanPhienMsg[idban] = DCC_Message:0;

    for(new p = 0; p < MAX_PLAYERS; p++)
    {
        if(!IsPlayerConnected(p)) continue;
        
        if(BanCuaNguoiChoi[p] == idban)
        {
            HienThiDialogQuay(p, idban, -1, vitriThang);
        }
        
        new tongThang = 0, tongCuoc = 0;
        
        if(CuocHienTai[idban][p][soThang] > 0) 
            tongThang += CuocHienTai[idban][p][soThang] * 36;
            
        if(CuocMau[idban][p][mauThang] > 0) 
            tongThang += CuocMau[idban][p][mauThang] * ((mauThang == 2) ? 36 : 2);

        for(new i=0; i<37; i++) tongCuoc += CuocHienTai[idban][p][i];
        for(new i=0; i<3; i++) tongCuoc += CuocMau[idban][p][i];

        if(tongCuoc > 0)
        {
            // Ap dung VAT 2% tren loi nhuan
            new vat = 0;
            if(tongThang > tongCuoc)
            {
                vat = (tongThang - tongCuoc) * VAT_PHAN_TRAM / 100;
                tongThang -= vat;
            }

            PlayerInfo[p][pCash] += tongThang;

            new msg[256], tenmau[32], mauCode[16];
            LayTenMau(mauThang, tenmau, sizeof(tenmau));

            // Get color code for the winning number
            switch(mauThang)
            {
                case 0: mauCode = "{FF6347}"; // Tho (Red)
                case 1: mauCode = "{A9A9A9}"; // Rua (Black)
                case 2: mauCode = "{90EE90}"; // Xanh (Green)
                default: mauCode = "{FFFFFF}";
            }

            if(tongThang > tongCuoc)
            {
                new loinhuan = tongThang - tongCuoc;
                format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{00FF00}==== CHIEN THANG! ====");
                SendClientMessage(p, -1, msg);
                format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{FFFFFF}Ket qua: %s%d (%s)", mauCode, soThang, tenmau);
                SendClientMessage(p, -1, msg);
                format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{00FF00}Ban thang: {FFFF00}+$%s", number_format(loinhuan));
                SendClientMessage(p, -1, msg);
                format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{AAAAAA}Phi VAT %d%%: {FFFF00}-$%s", VAT_PHAN_TRAM, number_format(vat));
                SendClientMessage(p, -1, msg);
                format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{00FF00}==================");
                SendClientMessage(p, -1, msg);
            }
            else if(tongThang == tongCuoc)
            {
                format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{FFA500}==== HOA ====");
                SendClientMessage(p, -1, msg);
                format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{FFFFFF}Ket qua: %s%d (%s)", mauCode, soThang, tenmau);
                SendClientMessage(p, -1, msg);
                format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{FFA500}Hoan tien: {FFFF00}$%s", number_format(tongCuoc));
                SendClientMessage(p, -1, msg);
                format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{FFA500}==================");
                SendClientMessage(p, -1, msg);
            }
            else
            {
                new thua = tongCuoc - tongThang;
                format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{FF0000}==== THUA CUOC ====");
                SendClientMessage(p, -1, msg);
                format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{FFFFFF}Ket qua: %s%d (%s)", mauCode, soThang, tenmau);
                SendClientMessage(p, -1, msg);
                format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{FF0000}Ban thua: {FFFF00}-$%s", number_format(thua));
                SendClientMessage(p, -1, msg);
                format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{FF0000}==================");
                SendClientMessage(p, -1, msg);
            }
        }
        
        CapNhatNhaCai(p, idban, soThang);

        for(new i=0; i<37; i++) CuocHienTai[idban][p][i] = 0;
        for(new i=0; i<3; i++) CuocMau[idban][p][i] = 0;
    }
}


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
        BanPhienMsg[i] = DCC_Message:0;
        for(new j = 0; j < 20; j++)
        {
            BanRoulette[i][bLichSu][j] = -1;
            BanRoulette[i][bLichSuThoiGian][j] = 0;
        }
    }
    TaiBanRoulette();
    print(">> [ZnX] He Thong Quay So Rua Tho da duoc tai lai!");
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
    RoulCuocChuKy[playerid] = 0;
    RoulThangChuKy[playerid] = 0;
    RoulThangVanTruoc[playerid] = 0;
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    new idban = BanCuaNguoiChoi[playerid];
    
    // Check if player had active bets and log to Discord
    if(idban != -1 && idban < MAX_BAN_ROULETTE && BanRoulette[idban][bHoatDong])
    {
        new tongCuoc = 0;
        for(new i=0; i<37; i++) tongCuoc += CuocHienTai[idban][playerid][i];
        for(new i=0; i<3; i++) tongCuoc += CuocMau[idban][playerid][i];
        
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
            strcat(logMsg, tempMsg);
            format(tempMsg, sizeof(tempMsg), "**Bàn:** `#%d`\n", idban);
            strcat(logMsg, tempMsg);
            format(tempMsg, sizeof(tempMsg), "**Lý do:** `%s`\n", reasonText);
            strcat(logMsg, tempMsg);
            format(tempMsg, sizeof(tempMsg), "**Tổng cược mất:** `$%s`\n", number_format(tongCuoc));
            strcat(logMsg, tempMsg);
            
            // Chi tiet cuoc
            new chiTietCuoc[256] = "";
            for(new j=0; j<37; j++) 
            {
                if(CuocHienTai[idban][playerid][j] > 0)
                {
                    format(tempMsg, sizeof(tempMsg), "Số %d: $%s | ", j, number_format(CuocHienTai[idban][playerid][j]));
                    strcat(chiTietCuoc, tempMsg);
                }
            }
            if(CuocMau[idban][playerid][0] > 0)
            {
                format(tempMsg, sizeof(tempMsg), "Thỏ: $%s | ", number_format(CuocMau[idban][playerid][0]));
                strcat(chiTietCuoc, tempMsg);
            }
            if(CuocMau[idban][playerid][1] > 0)
            {
                format(tempMsg, sizeof(tempMsg), "Rùa: $%s | ", number_format(CuocMau[idban][playerid][1]));
                strcat(chiTietCuoc, tempMsg);
            }
            if(CuocMau[idban][playerid][2] > 0)
            {
                format(tempMsg, sizeof(tempMsg), "Xanh: $%s | ", number_format(CuocMau[idban][playerid][2]));
                strcat(chiTietCuoc, tempMsg);
            }
            if(strlen(chiTietCuoc) > 0)
            {
                format(tempMsg, sizeof(tempMsg), "**Chi tiết:** `%s`\n", chiTietCuoc);
                strcat(logMsg, tempMsg);
            }
            
            if(BanRoulette[idban][bDangQuay])
            {
                strcat(logMsg, "_Phiên đang quay..._");
            }
            else
            {
                format(tempMsg, sizeof(tempMsg), "_Phiên còn %d giây_", BanRoulette[idban][bGiayDemNguoc]);
                strcat(logMsg, tempMsg);
            }
            
            SendRuaThoLogDC(logMsg);
        }
        
        // Clear player's bets on disconnect
        for(new i=0; i<37; i++) CuocHienTai[idban][playerid][i] = 0;
        for(new i=0; i<3; i++) CuocMau[idban][playerid][i] = 0;
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
                {
                    ShowPlayerDialog(playerid, DIALOG_ROULETTE_CUOC_SO, DIALOG_STYLE_INPUT, 
                        "{FFFF00}> {FFFFFF}Dat cuoc so", 
                        "{FFFFFF}Nhap so ban muon cuoc {FFFF00}(0-36){FFFFFF}:", 
                        "Tiep tuc", "Huy");
                }
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
            // Tinh tong cuoc hien tai cua nguoi choi tren ban nay
            new idbanCheck = BanCuaNguoiChoi[playerid];
            new tongCuocHienTai = 0;
            for(new jj = 0; jj < 37; jj++) tongCuocHienTai += CuocHienTai[idbanCheck][playerid][jj];
            for(new jj = 0; jj < 3; jj++) tongCuocHienTai += CuocMau[idbanCheck][playerid][jj];
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
                SendClientMessage(playerid, -1, TIEN_TO_TIN_NHAN "{FF0000}Ban khong du tien! {FFFFFF}Can: {FFFF00}$",number_format(sotien));
                return HienThiDialogLoaiCuoc(playerid);
            }
            new idban = BanCuaNguoiChoi[playerid];
            new loaiCuoc = DuLieuCuocCuaNguoiChoi[playerid];
            new msg[128];
            
            // Discord Log - Dat cuoc
            new logMsg[512], tempMsg[256], loaiCuocText[64];
            
            if(loaiCuoc >= 100)
            {
                new so = loaiCuoc - 100;
                CuocHienTai[idban][playerid][so] += sotien;
                format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{00FF00}Ban da dat cuoc {FFFF00}$%s {00FF00}vao so {FFFF00}%d{00FF00}!", number_format(sotien), so);
                format(loaiCuocText, sizeof(loaiCuocText), "So %d", so);
            }
            else
            {
                switch(loaiCuoc)
                {
                    case 1: 
                    { 
                        CuocMau[idban][playerid][0] += sotien; 
                        format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{00FF00}Ban da dat cuoc {FFFF00}$%s {00FF00}vao {FF6347}Tho{00FF00}!", number_format(sotien)); 
                        format(loaiCuocText, sizeof(loaiCuocText), "Tho (Do)");
                    }
                    case 2: 
                    { 
                        CuocMau[idban][playerid][1] += sotien; 
                        format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{00FF00}Ban da dat cuoc {FFFF00}$%s {00FF00}vao {A9A9A9}Rua{00FF00}!", number_format(sotien)); 
                        format(loaiCuocText, sizeof(loaiCuocText), "Rua (Den)");
                    }
                    case 3: 
                    { 
                        CuocMau[idban][playerid][2] += sotien; 
                        format(msg, sizeof(msg), TIEN_TO_TIN_NHAN "{00FF00}Ban da dat cuoc {FFFF00}$%s {00FF00}vao {90EE90}Xanh{00FF00}!", number_format(sotien)); 
                        format(loaiCuocText, sizeof(loaiCuocText), "Xanh (So 0)");
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
            {
                HienThiDialogChinh(playerid);
            }
        }
        case DIALOG_ROULETTE_LICH_SU:
        {
            // Khong can xu ly gi, chi dong dialog
            return 1;
        }
    }
    return 1;
}
