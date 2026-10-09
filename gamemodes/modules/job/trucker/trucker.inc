#include <YSI\YSI_Coding\y_hooks>

forward TRUCKER_WELCOME_MSG(playerid);
public TRUCKER_WELCOME_MSG(playerid)
{
    if(IsPlayerConnected(playerid)) Trucker_ShowInstructions(playerid);
    return 1;
}

// ================================================================
//  TRUCKER JOB - MAIN LOGIC
//  Flow: Lay xe tai (SF Docks) -> Mua hang (on foot) -> Chat len xe
//        -> Lai den diem ban -> Xuat hang -> Quay ve lanh luong
// ================================================================

// ----- Utility stocks -----

stock IsATruckerCar(carid)
{
    foreach(new i : Player) {
        if(TruckerJobCar[i] == carid && carid != INVALID_VEHICLE_ID) return 1;
    }
    return 0;
}

stock IsTrucker(playerid)
{
    return (PlayerInfo[playerid][pJob] == JOB_TRUCKER || PlayerInfo[playerid][pJob2] == JOB_TRUCKER);
}

stock CountCopsOnDuty()
{
    new count = 0;
    foreach(new i : Player) {
        if(IsACop(i) && PlayerInfo[i][pDuty] == 1) count++;
    }
    return count;
}

stock Trucker_Cleanup(playerid)
{
    if(TruckerJobCar[playerid] != INVALID_VEHICLE_ID && IsValidVehicle(TruckerJobCar[playerid])) {
        DestroyVehicle(TruckerJobCar[playerid]);
    }
    TruckerJobCar[playerid] = INVALID_VEHICLE_ID;

    if(TruckerJobText[playerid] != Text3D:INVALID_3DTEXT_ID) {
        Delete3DTextLabel(TruckerJobText[playerid]);
        TruckerJobText[playerid] = Text3D:INVALID_3DTEXT_ID;
    }
    LuongHang[playerid]          = 0;
    lanhluong[playerid]          = 0;
    TruckerDeliveries[playerid]  = 0;
    PlayerSellingCargo[playerid] = false;

    for(new c = 0; c < 4; c++) {
        hangtrentay[playerid][c] = 0;
        hangtrenxe[playerid][c]  = 0;
    }

    tongXPTruck[playerid]        = 0;
    if(PlayerWorking[playerid] == JOB_TRUCKER) PlayerWorking[playerid] = 0;
    RemovePlayerAttachedObject(playerid, 6);
    SetPlayerSpecialAction(playerid, 0);
    ClearAnimations(playerid);
    DisablePlayerCheckpoint(playerid);
    return 1;
}

stock Trucker_ShowInstructions(playerid)
{
    SendClientMessageEx(playerid, COLOR_TRUCKER,
        "|---------- HUONG DAN TRUCKER -----------|");
    SendClientMessageEx(playerid, COLOR_WHITE,
        "{FFFF00}1. {FFFFFF}Lay xe o NPC cong viec{FFFFFF} tai San Fierro Docks (diem vang).");
    SendClientMessageEx(playerid, COLOR_WHITE,
        " {FFFF00}2. {FFFFFF}Di bo den diem LAY HANG{FFFFFF} (diem xanh, Bam [Y] de mo bang mua).");
    SendClientMessageEx(playerid, COLOR_WHITE,
        "{FFFF00}3. {FFFFFF}Di ve xe, ban Y gan xe de {44ff00}chat hang len xe{FFFFFF} (toi da 5 kien).");
    SendClientMessageEx(playerid, COLOR_WHITE,
        "{FFFF00}4. {FFFFFF}Lai xe den diem BAN HANG{FFFFFF} (dung /diemgiaohang de xem danh sach).");
    SendClientMessageEx(playerid, COLOR_WHITE,
        "{FFFF00}5. {FFFFFF}Dung xe, lay hang (/xemhang), di bo den diem ban, bam [Y] de ban.");
    SendClientMessageEx(playerid, COLOR_WHITE,
        "{FFFF00}6. {FFFFFF}Quay ve Ben Cang SF, bam [Y] tai diem {5BFF89}LANH LUONG{FFFFFF} de nhan tien.");
    SendClientMessageEx(playerid, COLOR_TRUCKER,
        "   Lenh: /diemgiaohang  /xemhang  /kiemtrahang  /dungviec");
    SendClientMessageEx(playerid, COLOR_TRUCKER,
        "|----------------------------------------|");
    return 1;
}

// ----- Price persistence -----

stock TruckPrice_Save()
{
    new File:f = fopen("truckConfig.ini", io_write);
    if(!f) return;
    new buf[64];
    format(buf, sizeof(buf), "ThucAn=%d\n",  giatruck[0]); fwrite(f, buf);
    format(buf, sizeof(buf), "VatLieu=%d\n", giatruck[1]); fwrite(f, buf);
    format(buf, sizeof(buf), "QuanAo=%d\n",  giatruck[2]); fwrite(f, buf);
    format(buf, sizeof(buf), "HangCam=%d\n", giatruck[3]); fwrite(f, buf);
    fclose(f);
}

stock TruckPrice_Load()
{
    new File:f = fopen("truckConfig.ini", io_read);
    if(!f) {
        // Default prices
        giatruck[0] = 5000; giatruck[1] = 5500; giatruck[2] = 4800; giatruck[3] = 15000;
        TruckPrice_Save();
        return;
    }
    new line[64];
    while(fread(f, line, sizeof(line))) {
        new pos = strfind(line, "=");
        if(pos < 0) continue;
        new key[32], val[16];
        strmid(key, line, 0, pos, sizeof(key));
        strmid(val, line, pos + 1, strlen(line), sizeof(val));
        if(!strcmp(key, "ThucAn"))       giatruck[0] = strval(val);
        else if(!strcmp(key, "VatLieu")) giatruck[1] = strval(val);
        else if(!strcmp(key, "QuanAo"))  giatruck[2] = strval(val);
        else if(!strcmp(key, "HangCam")) giatruck[3] = strval(val);
    }
    if(!giatruck[3]) giatruck[3] = 15000;
    fclose(f);
}

// ----- Timers -----

forward UpdateTruckerPrices();
public UpdateTruckerPrices()
{
    for(new i = 0; i < sizeof(ThuMuaTruck); i++) {
        new base = 0;
        switch(ThuMuaTruck[i][loaihang]) {
            case 1: base = giatruck[0];
            case 2: base = giatruck[1];
            case 3: base = giatruck[2];
        }
        ThuMuaTruck[i][giahang]       = base;
        ThuMuaTruck[i][giabanhientai] = base + random(9000);
    }
    SendClientMessageToAll(COLOR_WHITE,
        "{FFA900}> {FFFFFF}THONG BAO: Gia thu mua hang hoa Trucker vua duoc cap nhat!");
    return 1;
}

forward UpdateIllegalStock();
public UpdateIllegalStock()
{
    for(new i = 0; i < sizeof(IllegalAreaInfo); i++) {
        if(IllegalAreaInfo[i][buyStock] < 5) {
            IllegalAreaInfo[i][buyStock]++;
            new string[128];
            format(string, sizeof(string),
                "{ff1100}Khu vuc mua hang cam\n\n{FFFFFF}So luong hien co: %d/5\nBam {FFFF00}[ Y ]{FFFFFF} de mua",
                IllegalAreaInfo[i][buyStock]);
            UpdateDynamic3DTextLabelText(
                STREAMER_TAG_3D_TEXT_LABEL:IllegalAreaInfo[i][buyText3D],
                COLOR_WHITE, string);
        }
    }
    return 1;
}

forward TRUCKER_GMI_DELAY();
public TRUCKER_GMI_DELAY()
{
    TruckPrice_Load();
    UpdateTruckerPrices();
    return 1;
}

// ----- Game Mode Init -----

hook OnGameModeInit()
{
    // Labels tai 12 diem ban hang
    for(new i = 0; i < sizeof(ThuMuaTruck); i++) {
        new string[128], loaiStr[24];
        switch(ThuMuaTruck[i][loaihang]) {
            case 1: loaiStr = "Thuc An";
            case 2: loaiStr = "Vat Lieu";
            case 3: loaiStr = "Quan Ao";
        }
        format(string, sizeof(string),
            "{FFFFFF}< DIEM BAN HANG >\n\n{44ff00}Nhan hang: %s\n{FFFFFF}Bam {FFFF00}[ Y ]{FFFFFF} de ban",
            loaiStr);
        CreateDynamic3DTextLabel(string, COLOR_WHITE,
            KhuVucThuMua[i][0], KhuVucThuMua[i][1], KhuVucThuMua[i][2], 12.0);
        CreateDynamicPickup(1239, 23,
            KhuVucThuMua[i][0], KhuVucThuMua[i][1], KhuVucThuMua[i][2], -1);
    }

    // Labels tai 3 khu vuc mua hang cam
    for(new i = 0; i < sizeof(IllegalAreaInfo); i++) {
        new string[128];
        format(string, sizeof(string),
            "{ff1100}Khu vuc mua hang cam\n\n{FFFFFF}So luong hien co: %d/5\nBam {FFFF00}[ Y ]{FFFFFF} de mua",
            IllegalAreaInfo[i][buyStock]);
        IllegalAreaInfo[i][buyText3D] = _:CreateDynamic3DTextLabel(string, COLOR_RED,
            IllegalAreaInfo[i][buyX], IllegalAreaInfo[i][buyY], IllegalAreaInfo[i][buyZ],
            12.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, -1, -1);
        CreateDynamicPickup(1239, 23,
            IllegalAreaInfo[i][buyX], IllegalAreaInfo[i][buyY], IllegalAreaInfo[i][buyZ], -1);
    }

    // Diem ban hang cam
    CreateDynamic3DTextLabel(
        "{ff1100}< DIEM BAN HANG CAM >\n\n{FFFFFF}Bam {FFFF00}[ Y ]{FFFFFF} de ban hang cam",
        COLOR_RED,
        SellIllegalPosition[0], SellIllegalPosition[1], SellIllegalPosition[2], 12.0);
    CreateDynamicPickup(1239, 23,
        SellIllegalPosition[0], SellIllegalPosition[1], SellIllegalPosition[2], -1);

    // Diem lay hang (Ben Cang SF)
    CreateDynamic3DTextLabel(
        "{FFFFFF}< BEN CANG SF - DIEM LAY HANG >\n\n{44ff00}Cong ty Van chuyen\n{FFFFFF}Bam {FFFF00}[ Y ]{FFFFFF} de mua hang hoa",
        COLOR_WHITE, TRUCKER_BUY_POS, 10.0);
    CreateDynamicPickup(1239, 23, TRUCKER_BUY_POS, -1);

    // Diem lanh luong (Ben Cang SF)
    CreateDynamic3DTextLabel(
        "{FFFFFF}< DIEM LANH LUONG >\n\n{5BFF89}Nhan tien luong tai day\n{FFFFFF}Bam {FFFF00}[ Y ]{FFFFFF} de lanh luong",
        0x00FF7FFF, TRUCKER_PAYMENT_POS, 10.0);
    CreateDynamicPickup(1239, 23, TRUCKER_PAYMENT_POS, -1);

    // Timers
    SetTimer("UpdateTruckerPrices", 5400000, true); // 1h30phut
    SetTimer("UpdateIllegalStock",  5400000, true);
    SetTimerEx("TRUCKER_GMI_DELAY", 1200, false, "");
    return 1;
}

// ----- Player Callbacks -----

hook OnPlayerConnect(playerid)
{
    for(new c = 0; c < 4; c++) {
        hangtrentay[playerid][c] = 0;
        hangtrenxe[playerid][c]  = 0;
    }
    LuongHang[playerid]          = 0;
    lanhluong[playerid]          = 0;
    TruckerDeliveries[playerid]  = 0;
    tongXPTruck[playerid]        = 0;
    PlayerSellingCargo[playerid] = false;
    TruckerJobCar[playerid]      = INVALID_VEHICLE_ID;
    TruckerJobText[playerid]     = Text3D:INVALID_3DTEXT_ID;
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    Trucker_Cleanup(playerid);
    return 1;
}

// ----- Y Key Interactions -----

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if(!PRESSED(KEY_YES)) return 1;

    // ==================================================
    // 1. DIEM LANH LUONG
    // ==================================================
    if(IsPlayerInRangeOfPoint(playerid, 4.0, TRUCKER_PAYMENT_POS))
    {
        if(!IsTrucker(playerid))                    return 1;
        if(PlayerWorking[playerid] != JOB_TRUCKER)  return 1;
        if(IsPlayerInAnyVehicle(playerid))          return 1;

        if(lanhluong[playerid] <= 0) {
            SendClientMessageEx(playerid, COLOR_GREY,
                "[Trucker] Ban chua giao hang nao, khong co tien de lanh!");
            return 1;
        }

        new salary = lanhluong[playerid];
        GivePlayerCash(playerid, salary);

        new xp_give = tongXPTruck[playerid];
        if(DoubleXP || PlayerInfo[playerid][pDoubleEXP] > 0) xp_give *= 2;
        if(xp_give > 0) {
            GivePlayerXP(playerid, xp_give);
            SendClientMessageEx(playerid, COLOR_YELLOW, "Kinh nghiem: +%d XP%s",
                xp_give, (DoubleXP || PlayerInfo[playerid][pDoubleEXP] > 0) ? " [2X]" : "");
        }

        SendClientMessageEx(playerid, COLOR_TRUCKER,
            "{D5A30A}[Trucker] Ban da lanh {5BFF89}$%s{D5A30A} luong. Cam on da lam viec!",
            number_format(salary));
        ShowTDN(playerid, NOTIFICATION_SUCCESS, "Lanh luong thanh cong!");

        // Discord log
        new title[64];
        format(title, sizeof(title), "%s (ID: %d) - Lãnh lương Trucker", GetPlayerNameEx(playerid), playerid);
        DiscordLog(dclog_Trucker, title,
            ":moneybag: **Tiền lãnh: $%s**\n:package: **Kiện đã giao: %d**\n:star: **XP nhận: +%d%s**\n:briefcase: **Trucker Skill: %d**",
            number_format(salary), TruckerDeliveries[playerid],
            xp_give, (DoubleXP || PlayerInfo[playerid][pDoubleEXP] > 0) ? " [2X]" : "",
            PlayerInfo[playerid][pTruckSkill]);

        Trucker_Cleanup(playerid);
        return 1;
    }

    // ==================================================
    // 2. DIEM MUA HANG (Ben Cang SF, on foot)
    // ==================================================
    if(IsPlayerInRangeOfPoint(playerid, 3.0, TRUCKER_BUY_POS))
    {
        if(!IsTrucker(playerid))                    return 1;
        if(PlayerWorking[playerid] != JOB_TRUCKER)  return 1;
        if(IsPlayerInAnyVehicle(playerid)) {
            SendClientMessageEx(playerid, COLOR_GREY, "[Trucker] Xuong xe de lay hang.");
            return 1;
        }
        if(LuongHang[playerid] >= 5) {
            SendClientMessageEx(playerid, COLOR_GREY, "[Trucker] Xe da day 5 kien hang, khong the mua them!");
            return 1;
        }
        // Kiem tra dang cam hang
        for(new c = 0; c < 3; c++) {
            if(hangtrentay[playerid][c] == 1) {
                SendClientMessageEx(playerid, COLOR_GREY, "[Trucker] Ban dang cam hang, hay chat len xe truoc.");
                return 1;
            }
        }

        // Build dialog voi gia VIP
        new szDialog[1024], row[200];
        strcat(szDialog, "#\tHang Hoa\tGia\n");

        if(PlayerInfo[playerid][pDonateRank] == 2) {
            format(row, sizeof(row), "0\tThuc An\t{4dad2b}$%s {FFFF00}(-10%%)\n",  number_format(floatround(giatruck[0] * 0.9))); strcat(szDialog, row);
            format(row, sizeof(row), "1\tVat Lieu\t{4dad2b}$%s {FFFF00}(-10%%)\n", number_format(floatround(giatruck[1] * 0.9))); strcat(szDialog, row);
            format(row, sizeof(row), "2\tQuan Ao\t{4dad2b}$%s {FFFF00}(-10%%)\n",  number_format(floatround(giatruck[2] * 0.9))); strcat(szDialog, row);
        } else if(PlayerInfo[playerid][pDonateRank] >= 3) {
            format(row, sizeof(row), "0\tThuc An\t{4dad2b}$%s {FFFF00}(-30%%)\n",  number_format(floatround(giatruck[0] * 0.7))); strcat(szDialog, row);
            format(row, sizeof(row), "1\tVat Lieu\t{4dad2b}$%s {FFFF00}(-30%%)\n", number_format(floatround(giatruck[1] * 0.7))); strcat(szDialog, row);
            format(row, sizeof(row), "2\tQuan Ao\t{4dad2b}$%s {FFFF00}(-30%%)\n",  number_format(floatround(giatruck[2] * 0.7))); strcat(szDialog, row);
        } else {
            format(row, sizeof(row), "0\tThuc An\t{4dad2b}$%s\n",  number_format(giatruck[0])); strcat(szDialog, row);
            format(row, sizeof(row), "1\tVat Lieu\t{4dad2b}$%s\n", number_format(giatruck[1])); strcat(szDialog, row);
            format(row, sizeof(row), "2\tQuan Ao\t{4dad2b}$%s\n",  number_format(giatruck[2])); strcat(szDialog, row);
        }
        ShowPlayerDialog(playerid, DIALOG_TRUCKERGO, DIALOG_STYLE_TABLIST_HEADERS,
            "{FFFFFF}Mua hang hoa - Ben Cang SF", szDialog, "Mua", "Dong");
        return 1;
    }

    // ==================================================
    // 3. DIEM BAN HANG (12 diem, khop voi loai hang dang cam)
    // ==================================================
    for(new i = 0; i < sizeof(ThuMuaTruck); i++) {
        if(IsPlayerInRangeOfPoint(playerid, 2.5,
            KhuVucThuMua[i][0], KhuVucThuMua[i][1], KhuVucThuMua[i][2]))
        {
            new loaiIdx = GetPVarInt(playerid, "idloaihang"); // 1=ThucAn, 2=VatLieu, 3=QuanAo
            if(loaiIdx < 1 || loaiIdx > 3) {
                SendClientMessageEx(playerid, COLOR_GREY,
                    "[Trucker] Ban khong dang cam hang hoa nao.");
                return 1;
            }
            if(ThuMuaTruck[i][loaihang] != loaiIdx) {
                SendClientMessageEx(playerid, COLOR_GREY,
                    "[Trucker] Diem nay khong nhan loai hang ban dang mang.");
                return 1;
            }

            PlayerSellingCargo[playerid] = true;
            new string[128];
            format(string, sizeof(string),
                "{FFFFFF}Gia thu mua hien tai: {5BFF89}$%d{FFFFFF}\nBan co muon ban khong?",
                ThuMuaTruck[i][giabanhientai]);
            ShowPlayerDialog(playerid, DIALOG_BANHANGTRUCK, DIALOG_STYLE_MSGBOX,
                "> Ban Hang", string, "BAN LUON", "HUY");
            SetPVarInt(playerid, "giatientruck", ThuMuaTruck[i][giabanhientai]);
            return 1;
        }
    }

    // ==================================================
    // 4. CHAT HANG LEN XE (gan xe cua ban, dang cam hang)
    // ==================================================
    if(IsTrucker(playerid) && PlayerWorking[playerid] == JOB_TRUCKER && !IsPlayerInAnyVehicle(playerid))
    {
        new vid = TruckerJobCar[playerid];
        if(vid != INVALID_VEHICLE_ID && IsValidVehicle(vid)) {
            new Float:vx, Float:vy, Float:vz;
            GetVehiclePos(vid, vx, vy, vz);
            if(IsPlayerInRangeOfPoint(playerid, 4.0, vx, vy, vz)) {
                if(PlayerSellingCargo[playerid]) return 1;

                new loaded = false;
                for(new c = 0; c < 4; c++) {
                    if(hangtrentay[playerid][c] == 1) {
                        hangtrenxe[playerid][c] += 1;
                        hangtrentay[playerid][c]  = 0;
                        LuongHang[playerid]       += 1;
                        SetPVarInt(playerid, "idloaihang", 0);
                        ApplyAnimation(playerid, "CARRY", "PUTDWN", 4.0, 0, 0, 0, 0, 0);
                        RemovePlayerAttachedObject(playerid, 6);
                        SetPlayerSpecialAction(playerid, 0);

                        new loaiName[24];
                        switch(c) {
                            case 0: loaiName = "Thuc An";
                            case 1: loaiName = "Vat Lieu";
                            case 2: loaiName = "Quan Ao";
                            case 3: loaiName = "Hang Cam";
                        }
                        SendClientMessageEx(playerid, COLOR_WHITE,
                            "{D5A30A}[Trucker] Da chat {44ff00}%s{D5A30A} len xe (%d/5 kien).",
                            loaiName, LuongHang[playerid]);

                        if(LuongHang[playerid] == 1) {
                            DisablePlayerCheckpoint(playerid);
                            SendClientMessageEx(playerid, COLOR_WHITE,
                                "Hint: Dung {FFFF00}/diemgiaohang{FFFFFF} de xem va danh dau diem ban hang gan nhat.");
                        }
                        else if(LuongHang[playerid] == 5) {
                            new dtitle[128], dstr[256];
                            format(dtitle, sizeof(dtitle), "%s (ID: %d) - Chất đủ 5 kiện hàng", GetPlayerNameEx(playerid), playerid);
                            format(dstr, sizeof(dstr), ":truck: **XE ĐẦY HÀNG (5/5)**\n:package: **Kiện cuối: %s**\n**Tổng hàng trên xe:**\n- Thực Ăn: %d | Vật Liệu: %d | Quần Áo: %d | Hàng Cấm: %d\n:arrow_right: **Sẵn sàng giao hàng!**",
                                loaiName,
                                hangtrenxe[playerid][0], hangtrenxe[playerid][1],
                                hangtrenxe[playerid][2], hangtrenxe[playerid][3]);
                            DiscordLog(dclog_Trucker, dtitle, dstr);
                        }
                        loaded = true;
                        break;
                    }
                }
                if(!loaded) {
                    SendClientMessageEx(playerid, COLOR_GREY,
                        "[Trucker] Ban khong dang cam hang hoa nao de chat len xe.");
                }
                return 1;
            }
        }
    }


    // ==================================================
    // 5. KHU VUC MUA HANG CAM (3 diem illegal)
    // ==================================================
    for(new i = 0; i < sizeof(IllegalAreaInfo); i++) {
        if(IsPlayerInRangeOfPoint(playerid, 2.5,
            IllegalAreaInfo[i][buyX], IllegalAreaInfo[i][buyY], IllegalAreaInfo[i][buyZ]))
        {
            if(!IsTrucker(playerid))                   return 1;
            if(PlayerWorking[playerid] != JOB_TRUCKER) return 1;
            if(IsPlayerInAnyVehicle(playerid)) {
                SendClientMessageEx(playerid, COLOR_GREY, "[Trucker] Xuong xe de mua hang."); return 1;
            }
            if(IsACop(playerid)) {
                SendClientMessageEx(playerid, COLOR_WHITE, "[Trucker] Ban la nguoi nha nuoc, khong the mua hang cam!"); return 1;
            }
            if(CountCopsOnDuty() < 2) {
                SendClientMessageEx(playerid, COLOR_WHITE, "[Trucker] Can it nhat 2 canh sat dang on-duty de mua hang cam!"); return 1;
            }

            for(new c = 0; c < 4; c++) {
                if(hangtrentay[playerid][c] == 1) {
                    SendClientMessageEx(playerid, COLOR_WHITE, "[Trucker] Ban dang cam hang, khong the mua them!"); return 1;
                }
            }

            if(IllegalAreaInfo[i][buyStock] <= 0) {
                SendClientMessageEx(playerid, COLOR_WHITE, "[Trucker] Het hang cam tai khu vuc nay!"); return 1;
            }
            if(GetPlayerCash(playerid) < giatruck[3]) {
                SendClientMessageEx(playerid, COLOR_WHITE,
                    "[Trucker] Khong du tien! Can $%s.", number_format(giatruck[3])); return 1;
            }

            // Mua hang
            IllegalAreaInfo[i][buyStock]--;
            GivePlayerCash(playerid, -giatruck[3]);
            hangtrentay[playerid][HANG_CAM] = 1;

            new updateText[128];
            format(updateText, sizeof(updateText),
                "{ff1100}Khu vuc mua hang cam\n\n{FFFFFF}So luong: %d/5\nBam {FFFF00}[ Y ]{FFFFFF} de mua",
                IllegalAreaInfo[i][buyStock]);
            UpdateDynamic3DTextLabelText(
                STREAMER_TAG_3D_TEXT_LABEL:IllegalAreaInfo[i][buyText3D], COLOR_WHITE, updateText);

            ApplyAnimation(playerid, "CARRY", "LIFTUP", 4.0, 0, 0, 0, 0, 0);
            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
            SetPlayerAttachedObject(playerid, 6, 1271, 1,
                0.299822, 0.663485, -0.036337, 184.311355, 90.004089, 0.0, 1.0, 1.0, 1.0);
            SetPVarInt(playerid, "idloaihang", 4); // 4 = HangCam

            SendClientMessageEx(playerid, COLOR_WHITE,
                "[Trucker] Da mua 1 kien {FF0606}Hang Cam{FFFFFF} gia {FF0606}$%s{FFFFFF}.",
                number_format(giatruck[3]));
            SendHintMessage(playerid,"Di ve xe va bam [Y] gan xe de chat len. Diem ban hang cam co dau '!' tren map.");

            // Discord log
            new title[128], str[256];
            format(title, sizeof(title), "%s (ID: %d) - Mua hàng cấm", GetPlayerNameEx(playerid), playerid);
            format(str, sizeof(str), ":red_circle: **MUA HÀNG CẤM**\n:money_with_wings: **Giá mua: $%s**\n:cop: **Cảnh sát on-duty: %d**\n:package: **Tồn kho khu vực: %d/5**\n:truck: **Hàng trên xe: %d/5**",
                number_format(giatruck[3]), CountCopsOnDuty(),
                IllegalAreaInfo[i][buyStock], LuongHang[playerid]);
            DiscordLog(dclog_Trucker, title, str);
            return 1;
        }
    }

    // ==================================================
    // 6. DIEM BAN HANG CAM (1 diem cố dinh)
    // ==================================================
    if(IsPlayerInRangeOfPoint(playerid, 2.5,
        SellIllegalPosition[0], SellIllegalPosition[1], SellIllegalPosition[2]))
    {
        if(!IsTrucker(playerid))                   return 1;
        if(PlayerWorking[playerid] != JOB_TRUCKER) return 1;
        if(hangtrentay[playerid][HANG_CAM] == 0)   return 1;
        if(PlayerSellingCargo[playerid])            return 1;

        PlayerSellingCargo[playerid] = true;
        ApplyAnimation(playerid, "CARRY", "PUTDWN", 4.0, 0, 0, 0, 0, 0);
        RemovePlayerAttachedObject(playerid, 6);
        SetPlayerSpecialAction(playerid, 0);

        new price = giatruck[3] + 300 + random(100);
        GivePlayerCash(playerid, price);
        hangtrentay[playerid][HANG_CAM] = 0;
        SetPVarInt(playerid, "idloaihang", 0);

        // Thuong vu khi ngau nhien (model 1-15)
        new randomWeapon = 1 + random(15);
        if(PlayerInfo[playerid][pConnectHours] >= 2 && PlayerInfo[playerid][pWRestricted] <= 0) {
            GivePlayerValidWeapon(playerid, randomWeapon, 50);
            SendClientMessageEx(playerid, COLOR_WHITE,
                "[Trucker] Da ban hang cam. Nhan {5BFF89}$%s{FFFFFF} + vu khi {b1ff00}%s{FFFFFF}!",
                number_format(price), GetWeaponName(randomWeapon));
        } else {
            SendClientMessageEx(playerid, COLOR_WHITE,
                "[Trucker] Da ban hang cam. Nhan {5BFF89}$%s{FFFFFF}!", number_format(price));
        }
        ShowTDN(playerid, NOTIFICATION_SUCCESS, "Ban hang cam thanh cong!");

        lanhluong[playerid]         += price;
        TruckerDeliveries[playerid] += 1;
        PlayerSellingCargo[playerid] = false;
        tongXPTruck[playerid]       += 40 + random(21);
        PlayerInfo[playerid][pTruckSkill] += 1;

        // Discord log
        new title[128], str[256];
        format(title, sizeof(title), "%s (ID: %d) - Bán hàng cấm", GetPlayerNameEx(playerid), playerid);
        if(PlayerInfo[playerid][pConnectHours] >= 2 && PlayerInfo[playerid][pWRestricted] <= 0) {
            format(str, sizeof(str), ":red_circle: **BÁN HÀNG CẤM THÀNH CÔNG**\n:money_with_wings: **Nhận: $%s**\n:gun: **Vũ khí: %s (x50)**\n:truck: **Lần giao thứ: %d**\n:dollar: **Tổng tích lũy: $%s** (chưa lãnh)",
                number_format(price), GetWeaponName(randomWeapon),
                TruckerDeliveries[playerid], number_format(lanhluong[playerid]));
        } else {
            format(str, sizeof(str), ":red_circle: **BÁN HÀNG CẤM THÀNH CÔNG**\n:money_with_wings: **Nhận: $%s**\n:no_entry_sign: **Không nhận vũ khí** (chưa đủ giờ/bị hạn chế)\n:truck: **Lần giao thứ: %d**\n:dollar: **Tổng tích lũy: $%s** (chưa lãnh)",
                number_format(price), TruckerDeliveries[playerid], number_format(lanhluong[playerid]));
        }
        DiscordLog(dclog_Trucker, title, str);
        return 1;
    }

    return 1;
}

// ----- Dialog Responses -----

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    // ==================================================
    // MUA HANG (DIALOG_TRUCKERGO)
    // ==================================================
    if(dialogid == DIALOG_TRUCKERGO)
    {
        if(!response) return 1;
        if(listitem < 0 || listitem > 2) return 1;

        if(LuongHang[playerid] >= 5) {
            SendClientMessageEx(playerid, COLOR_GREY, "[Trucker] Xe da day 5 kien!"); return 1;
        }
        for(new c = 0; c < 3; c++) {
            if(hangtrentay[playerid][c] == 1) {
                SendClientMessageEx(playerid, COLOR_GREY, "[Trucker] Ban dang cam hang roi, chat len xe truoc."); return 1;
            }
        }

        new finalPrice = giatruck[listitem];
        new vipDiscount = 0;
        if(PlayerInfo[playerid][pDonateRank] == 2) {
            vipDiscount = 10;
            finalPrice  = floatround(finalPrice * 0.9);
        } else if(PlayerInfo[playerid][pDonateRank] >= 3) {
            vipDiscount = 30;
            finalPrice  = floatround(finalPrice * 0.7);
        }

        if(GetPlayerCash(playerid) < finalPrice) {
            SendClientMessageEx(playerid, COLOR_WHITE,
                "[Trucker] Khong du tien! Can $%s.", number_format(finalPrice)); return 1;
        }

        new loaiName[24];
        switch(listitem) {
            case HANG_THUCAN:  loaiName = "Thuc An";
            case HANG_VATLIEU: loaiName = "Vat Lieu";
            case HANG_QUANAO:  loaiName = "Quan Ao";
        }

        GivePlayerCash(playerid, -finalPrice);
        hangtrentay[playerid][listitem] = 1;
        SetPVarInt(playerid, "idloaihang", listitem + 1); // 1=ThucAn, 2=VatLieu, 3=QuanAo

        ApplyAnimation(playerid, "CARRY", "LIFTUP", 4.0, 0, 0, 0, 0, 0);
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
        SetPlayerAttachedObject(playerid, 6, 1220, 1,
            0.299822, 0.663485, -0.036337, 184.311355, 90.004089, 0.0, 1.0, 1.0, 1.0);

        if(vipDiscount > 0) {
            SendClientMessageEx(playerid, COLOR_WHITE,
                "[Trucker] Da mua 1 kien {44ff00}%s{FFFFFF} - {5BFF89}$%s {FFFF00}(-VIP %d%%){FFFFFF}.",
                loaiName, number_format(finalPrice), vipDiscount);
        } else {
            SendClientMessageEx(playerid, COLOR_WHITE,
                "[Trucker] Da mua 1 kien {44ff00}%s{FFFFFF} - {5BFF89}$%s{FFFFFF}.",
                loaiName, number_format(finalPrice));
        }
        SendHintMessage(playerid,"Di ve xe cua ban va bam {FFFF00}[Y]{FFFFFF} gan xe de chat hang len.");
        return 1;
    }

    // ==================================================
    // BAN HANG (DIALOG_BANHANGTRUCK)
    // ==================================================
    if(dialogid == DIALOG_BANHANGTRUCK)
    {
        if(!response) {
            PlayerSellingCargo[playerid] = false;
            return 1;
        }
        ApplyAnimation(playerid, "CARRY", "PUTDWN", 4.0, 0, 0, 0, 0, 0);
        RemovePlayerAttachedObject(playerid, 6);
        SetPlayerSpecialAction(playerid, 0);

        new earn    = GetPVarInt(playerid, "giatientruck");
        new loai    = GetPVarInt(playerid, "idloaihang");   // 1-3

        lanhluong[playerid]         += earn;
        TruckerDeliveries[playerid] += 1;
        tongXPTruck[playerid]       += 40 + random(21);
        PlayerInfo[playerid][pTruckSkill] += 1;

        if(loai >= 1 && loai <= 3) hangtrentay[playerid][loai - 1] = 0;

        SetPVarInt(playerid, "idloaihang",   0);
        SetPVarInt(playerid, "giatientruck", 0);
        PlayerSellingCargo[playerid] = false;

        SendClientMessageEx(playerid, COLOR_TRUCKER,
            "{D5A30A}[Trucker] Giao hang thanh cong! Tich luy {5BFF89}+$%s{D5A30A} (chua lanh).",
            number_format(earn));
        ShowTDN(playerid, NOTIFICATION_SUCCESS, "Giao hang thanh cong!");

        // Discord log
        {
            new loaiNameStr[24];
            switch(loai) {
                case 1: loaiNameStr = "Thuc An";
                case 2: loaiNameStr = "Vat Lieu";
                case 3: loaiNameStr = "Quan Ao";
                default: loaiNameStr = "Khac";
            }
            new dtitle[128], dstr[256];
            format(dtitle, sizeof(dtitle), "%s (ID: %d) - Giao hàng Trucker", GetPlayerNameEx(playerid), playerid);
            format(dstr, sizeof(dstr), ":handshake: **GIAO HÀNG THÀNH CÔNG**\n:package: **Loại hàng: %s**\n:moneybag: **Giá thu mua: $%s**\n:truck: **Lần giao thứ: %d**\n:dollar: **Tổng tích lũy: $%s** (chưa lãnh)",
                loaiNameStr, number_format(earn),
                TruckerDeliveries[playerid], number_format(lanhluong[playerid]));
            DiscordLog(dclog_Trucker, dtitle, dstr);
        }

        if(LuongHang[playerid] > 0) {
            SendClientMessageEx(playerid, COLOR_WHITE,
                "Hint: Xe con hang, dung {FFFF00}/xemhang{FFFFFF} de lay va giao tiep.");
        } else {
            SetPlayerCP(playerid, TRUCKER_PAYMENT_POS, 3.0);
            gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
            SendHintMessage(playerid,"Xe da het hang! Quay ve {FFFF00}Ben Cang SF{FFFFFF} (diem xanh) de lanh {5BFF89}$%s{FFFFFF}.",number_format(lanhluong[playerid]));
        }
        return 1;
    }

    // ==================================================
    // XEM HANG TREN XE (DIALOG_XEMHANG)
    // ==================================================
    if(dialogid == DIALOG_XEMHANG)
    {
        if(!response) return 1;
        if(IsPlayerInAnyVehicle(playerid)) return 1;
        if(PlayerSellingCargo[playerid])   return 1;

        new vid = TruckerJobCar[playerid];
        if(vid == INVALID_VEHICLE_ID || !IsValidVehicle(vid)) return 1;

        new validItems[4], itemCount = 0;
        for(new c = 0; c < 4; c++) {
            if(hangtrenxe[playerid][c] >= 1) validItems[itemCount++] = c;
        }

        if(listitem < 0 || listitem >= itemCount) return 1;

        new sel = validItems[listitem];
        hangtrenxe[playerid][sel] -= 1;
        hangtrentay[playerid][sel]  = 1;
        LuongHang[playerid]        -= 1;
        SetPVarInt(playerid, "idloaihang", sel + 1);

        ApplyAnimation(playerid, "CARRY", "LIFTUP", 4.0, 0, 0, 0, 0, 0);
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
        // Box khac nhau cho hang cam
        SetPlayerAttachedObject(playerid, 6,
            (sel == HANG_CAM) ? 1271 : 1220, 1,
            0.299822, 0.663485, -0.036337, 184.311355, 90.004089, 0.0, 1.0, 1.0, 1.0);

        new loaiName[24];
        switch(sel) {
            case HANG_THUCAN:  loaiName = "Thuc An";
            case HANG_VATLIEU: loaiName = "Vat Lieu";
            case HANG_QUANAO:  loaiName = "Quan Ao";
            case HANG_CAM:     loaiName = "Hang Cam";
        }
        SendClientMessageEx(playerid, COLOR_TRUCKER,
            "{D5A30A}[Trucker] Da lay 1 kien {44ff00}%s{D5A30A}. Di den diem ban hang!", loaiName);

        if(sel == HANG_CAM) {
            SendClientMessageEx(playerid, COLOR_RED,
                "CANH BAO: Ban dang mang hang cam, canh sat co the bat ban bat cu luc nao!");
        }
        return 1;
    }

    // ==================================================
    // DANH SACH DIEM GIAO HANG (DIALOG_DIEMGIAOHANG)
    // ==================================================
    if(dialogid == DIALOG_DIEMGIAOHANG)
    {
        if(response && listitem >= 0 && listitem < sizeof(ThuMuaTruck)) {
            SetPlayerCP(playerid, KhuVucThuMua[listitem][0], KhuVucThuMua[listitem][1], KhuVucThuMua[listitem][2], 3.0);
            gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
            SendClientMessageEx(playerid, COLOR_WHITE,
                "{FFFFFF}[Trucker] Da danh dau diem ban hang {D5A30A}#(%d)%s {FFFFFF}tren mini-map.", listitem + 1, ThuMuaTruck[listitem][tenhang]);
        }
        return 1;
    }

    // ==================================================
    // CANH SAT KIEM TRA XE (DIALOG_TRUCKER_KIEMTRA)
    // ==================================================
    if(dialogid == DIALOG_TRUCKER_KIEMTRA)
    {
        if(!response) return 1;

        new targetid = GetPVarInt(playerid, "KiemTraHang_TargetID");
        DeletePVar(playerid, "KiemTraHang_TargetID");

        if(!IsPlayerConnected(targetid) || !IsTrucker(targetid) || PlayerWorking[targetid] != JOB_TRUCKER) {
            SendClientMessageEx(playerid, COLOR_GREY, "[Trucker] Nguoi choi khong con hop le.");
            return 1;
        }

        new validItems[4], itemCount = 0;
        for(new c = 0; c < 4; c++) {
            if(hangtrenxe[targetid][c] >= 1) validItems[itemCount++] = c;
        }

        if(listitem < 0 || listitem >= itemCount) return 1;

        new sel = validItems[listitem];
        if(sel == HANG_CAM) {
            new confiscated = hangtrenxe[targetid][HANG_CAM];
            hangtrenxe[targetid][HANG_CAM] = 0;
            if(LuongHang[targetid] >= confiscated) LuongHang[targetid] -= confiscated;
            else LuongHang[targetid] = 0;

            new bonus = 100 + random(20);
            GivePlayerCash(playerid, bonus);

            new string[256];
            format(string, sizeof(string),
                "* %s da tich thu %d kien hang cam tu xe cua %s.",
                GetPlayerNameEx(playerid), confiscated, GetPlayerNameEx(targetid));
            ProxDetector(30.0, playerid, string,
                COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

            SendClientMessageEx(playerid, COLOR_GREEN,
                "[Trucker] Da tich thu %d kien hang cam. Nhan {5BFF89}$%d{FFFFFF} tien thuong.", confiscated, bonus);
            SendClientMessageEx(targetid, COLOR_RED,
                "[Trucker] %s da tich thu %d kien hang cam cua ban!", GetPlayerNameEx(playerid), confiscated);
            ApplyAnimation(playerid, "POLICE", "CopTraf_Come", 4.1, 0, 0, 0, 0, 0);
        } else {
            SendClientMessageEx(playerid, COLOR_GREY, "[Trucker] Chi co the tich thu hang cam.");
        }
        return 1;
    }

    // ==================================================
    // MENU NPC TRUCKER (DIALOG_TRUCKERNPC)
    // ==================================================
    if(dialogid == DIALOG_TRUCKERNPC)
    {
        if(!response) return 1;

        switch(listitem)
        {
            case 0: // Lay xe Mule
            {
                if(TruckerJobCar[playerid] != INVALID_VEHICLE_ID && IsValidVehicle(TruckerJobCar[playerid])) {
                    SendClientMessageEx(playerid, COLOR_GREY,
                        "[Trucker] Ban da co xe. Hay su dung xe cu hoac /dungviec truoc.");
                    return 1;
                }
                if(PlayerWorking[playerid] != 0 && PlayerWorking[playerid] != JOB_TRUCKER) {
                    SendClientMessageEx(playerid, COLOR_GREY,
                        "[Trucker] Ban dang lam cong viec khac, khong the lay xe trucker.");
                    return 1;
                }

                new vid = CreateVehicle(414, -1565.0, 113.0, 3.5547, 38.4066, -1, -1, -1);
                if(vid == INVALID_VEHICLE_ID) {
                    SendClientMessageEx(playerid, COLOR_GREY, "[Trucker] Loi: khong the tao xe. Thu lai sau.");
                    return 1;
                }
                LockStatus{vid} = 0;
                VehicleFuel[vid] = 100.0;
                SetVehicleHealth(vid, 999.0);
                PutPlayerInVehicle(playerid, vid, 0);
                SetPlayerSkin(playerid, JobInfo[JOB_TRUCKER][jobSkin]);

                new labelText[128];
                format(labelText, sizeof(labelText),
                    "{b7eced}Trucker Delivery\n{FFFFFF}Chu xe: {b7eced}%s", GetPlayerNameEx(playerid));
                new Text3D:lbl = Create3DTextLabel(labelText, 0xFFFF00FF, 0.0, 0.0, 0.5, 12.0, 0);
                Attach3DTextLabelToVehicle(lbl, vid, 0.0, 0.0, 0.5);

                TruckerJobCar[playerid]  = vid;
                TruckerJobText[playerid] = lbl;
                PlayerWorking[playerid]  = JOB_TRUCKER;

                SetPlayerCP(playerid, TRUCKER_BUY_POS, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                SendClientMessageEx(playerid, COLOR_TRUCKER,
                    "{D5A30A}[Trucker] Da lay xe Mule! Den {FFFF00}Diem lay hang{D5A30A} tai Ben Cang SF (checkpoint vang).");
                SendClientMessageEx(playerid, COLOR_WHITE,
                    "Hint: Xuong xe, di bo den diem vang va bam {FFFF00}[Y]{FFFFFF}. Dung /huongdan de biet them.");
            }

            case 1: // Diem giao hang
            {
                PC_EmulateCommand(playerid, "/diemgiaohang");
            }

            case 2: // Huong dan
            {
                Trucker_ShowInstructions(playerid);
            }
        }
        return 1;
    }

    return 1;
}

// ================================================================
//  COMMANDS
// ================================================================

CMD:huongdan(playerid, params[])
{
    if(!IsTrucker(playerid)) {
        SendClientMessageEx(playerid, COLOR_GREY, "Ban khong phai Trucker."); return 1;
    }
    Trucker_ShowInstructions(playerid);
    return 1;
}

CMD:diemgiaohang(playerid, params[])
{
    if(!IsTrucker(playerid)) {
        SendClientMessageEx(playerid, COLOR_GREY, "[Trucker] Ban khong phai Trucker."); return 1;
    }
    new szDialog[1024], string[128], zone[MAX_ZONE_NAME];
    strcat(szDialog, "#\tLoai\tKhu Vuc\tKhoang Cach\n");
    for(new i = 0; i < sizeof(ThuMuaTruck); i++) {
        Get3DZone(KhuVucThuMua[i][0], KhuVucThuMua[i][1], KhuVucThuMua[i][2], zone, sizeof(zone));
        format(string, sizeof(string), "%d\t%s\t%s\t%.0fm\n",
            ThuMuaTruck[i][idhang], ThuMuaTruck[i][tenhang], zone,
            GetPlayerDistanceFromPoint(playerid,
                KhuVucThuMua[i][0], KhuVucThuMua[i][1], KhuVucThuMua[i][2]));
        strcat(szDialog, string);
    }
    ShowPlayerDialog(playerid, DIALOG_DIEMGIAOHANG, DIALOG_STYLE_TABLIST_HEADERS,
        "Dia Diem Giao Hang", szDialog, "Den", "Huy");
    return 1;
}

alias:xemhang("viewcargo")
CMD:xemhang(playerid, params[])
{
    if(!IsTrucker(playerid))                   return 1;
    if(PlayerWorking[playerid] != JOB_TRUCKER) return 1;

    for(new c = 0; c < 4; c++) {
        if(hangtrentay[playerid][c] == 1) {
            SendClientMessageEx(playerid, COLOR_GREY, "[Trucker] Dang cam hang, hay giao truoc."); return 1;
        }
    }
    if(IsPlayerInAnyVehicle(playerid)) {
        SendClientMessageEx(playerid, COLOR_GREY, "[Trucker] Xuong xe truoc."); return 1;
    }

    new vid = TruckerJobCar[playerid];
    if(vid == INVALID_VEHICLE_ID) {
        SendClientMessageEx(playerid, COLOR_GREY, "[Trucker] Ban chua co xe."); return 1;
    }
    new Float:vx, Float:vy, Float:vz;
    GetVehiclePos(vid, vx, vy, vz);
    if(!IsPlayerInRangeOfPoint(playerid, 6.0, vx, vy, vz)) {
        SendClientMessageEx(playerid, COLOR_GREY, "[Trucker] Phai o gan xe cua ban (trong vong 6m)."); return 1;
    }

    new szDialog[512], string[128], itemCount = 0;
    strcat(szDialog, "#\tHang Hoa\tSo Luong\n");

    if(hangtrenxe[playerid][0] >= 1) { format(string, sizeof(string), "%d\tThuc An\t%d\n",  itemCount++, hangtrenxe[playerid][0]); strcat(szDialog, string); }
    if(hangtrenxe[playerid][1] >= 1) { format(string, sizeof(string), "%d\tVat Lieu\t%d\n", itemCount++, hangtrenxe[playerid][1]); strcat(szDialog, string); }
    if(hangtrenxe[playerid][2] >= 1) { format(string, sizeof(string), "%d\tQuan Ao\t%d\n",  itemCount++, hangtrenxe[playerid][2]); strcat(szDialog, string); }
    if(hangtrenxe[playerid][3] >= 1) { format(string, sizeof(string), "%d\t{FF0606}Hang Cam{FFFFFF}\t%d\n", itemCount++, hangtrenxe[playerid][3]); strcat(szDialog, string); }

    if(itemCount == 0) {
        SendClientMessageEx(playerid, COLOR_GREY, "[Trucker] Khong co hang tren xe."); return 1;
    }
    ShowPlayerDialog(playerid, DIALOG_XEMHANG, DIALOG_STYLE_TABLIST_HEADERS,
        "Hang Hoa Tren Xe", szDialog, "Lay Hang", "Huy");
    return 1;
}

alias:kiemtrahang("checkcargo")
CMD:kiemtrahang(playerid, params[])
{
    if(!IsACop(playerid)) {
        SendClientMessageEx(playerid, COLOR_GREY, "[Trucker] Lenh nay chi danh cho canh sat."); return 1;
    }
    if(PlayerInfo[playerid][pDuty] == 0) {
        SendClientMessageEx(playerid, COLOR_GREY, "[Trucker] Phai on-duty moi dung lenh nay."); return 1;
    }

    new targetid;
    if(sscanf(params, "u", targetid)) {
        SendClientMessageEx(playerid, COLOR_GREY, "Su dung: /kiemtrahang [id/ten]"); return 1;
    }
    if(!IsPlayerConnected(targetid) || targetid == playerid) {
        SendClientMessageEx(playerid, COLOR_GREY, "[Trucker] Nguoi choi khong ton tai."); return 1;
    }
    if(!IsTrucker(targetid)) {
        SendClientMessageEx(playerid, COLOR_GREY, "[Trucker] Nguoi do khong phai Trucker."); return 1;
    }

    new vid = TruckerJobCar[targetid];
    if(vid == INVALID_VEHICLE_ID) {
        SendClientMessageEx(playerid, COLOR_GREY, "[Trucker] Nguoi choi chua co xe trucker."); return 1;
    }
    new Float:vx, Float:vy, Float:vz;
    GetVehiclePos(vid, vx, vy, vz);
    if(!IsPlayerInRangeOfPoint(playerid, 6.0, vx, vy, vz)) {
        SendClientMessageEx(playerid, COLOR_GREY, "[Trucker] Phai o gan xe cua nguoi choi (trong vong 6m)."); return 1;
    }

    new szDialog[512], string[128], itemCount = 0;
    strcat(szDialog, "#\tHang Hoa\tSo Luong\tLoai\n");

    if(hangtrenxe[targetid][0] >= 1) { format(string, sizeof(string), "%d\tThuc An\t%d\t{44ff00}Hop phap\n",          itemCount++, hangtrenxe[targetid][0]); strcat(szDialog, string); }
    if(hangtrenxe[targetid][1] >= 1) { format(string, sizeof(string), "%d\tVat Lieu\t%d\t{44ff00}Hop phap\n",         itemCount++, hangtrenxe[targetid][1]); strcat(szDialog, string); }
    if(hangtrenxe[targetid][2] >= 1) { format(string, sizeof(string), "%d\tQuan Ao\t%d\t{44ff00}Hop phap\n",          itemCount++, hangtrenxe[targetid][2]); strcat(szDialog, string); }
    if(hangtrenxe[targetid][3] >= 1) { format(string, sizeof(string), "%d\tHang Cam\t%d\t{FF0606}Bat hop phap\n",     itemCount++, hangtrenxe[targetid][3]); strcat(szDialog, string); }

    if(itemCount == 0) {
        SendClientMessageEx(playerid, COLOR_GREY, "[Trucker] Xe khong co hang hoa nao."); return 1;
    }

    SetPVarInt(playerid, "KiemTraHang_TargetID", targetid);
    new title[64];
    format(title, sizeof(title), "Kiem tra xe cua %s", GetPlayerNameEx(targetid));
    ShowPlayerDialog(playerid, DIALOG_TRUCKER_KIEMTRA, DIALOG_STYLE_TABLIST_HEADERS,
        title, szDialog, "Tich thu", "Dong");
    SendClientMessageEx(targetid, COLOR_YELLOW,
        "[Trucker] Canh sat {FFFF00}%s{FFFFFF} dang kiem tra xe cua ban.", GetPlayerNameEx(playerid));
    return 1;
}

CMD:dungviec(playerid, params[])
{
    if(!IsTrucker(playerid))                   return 1;
    if(PlayerWorking[playerid] != JOB_TRUCKER) {
        SendClientMessageEx(playerid, COLOR_GREY, "[Trucker] Ban chua bat dau lam viec."); return 1;
    }

    if(lanhluong[playerid] > 0) {
        SendClientMessageEx(playerid, COLOR_RED,
            "[Trucker] Ban con {5BFF89}$%s{FFFFFF} chua lanh! Hay ve Ben Cang SF de lanh luong truoc.",
            number_format(lanhluong[playerid]));
        return 1;
    }

    Trucker_Cleanup(playerid);
    SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "[Trucker] Da dung lam viec. Cam on ban da phuc vu!");
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Da dung lam viec Trucker.");
    return 1;
}

// Admin commands
CMD:chinhgiatruck(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] == 0) return 1;

    new type, value;
    if(sscanf(params, "ii", type, value)) {
        SendClientMessageEx(playerid, COLOR_GREY, "Su dung: /chinhgiatruck [Loai 0-3] [Gia]");
        SendClientMessageEx(playerid, COLOR_WHITE, "0=Thuc An | 1=Vat Lieu | 2=Quan Ao | 3=Hang Cam");
        return 1;
    }
    if(type < 0 || type > 3) {
        SendClientMessageEx(playerid, COLOR_GREY, "Loai khong hop le (0-3)!"); return 1;
    }
    if(value < 1) {
        SendClientMessageEx(playerid, COLOR_GREY, "Gia phai lon hon 0!"); return 1;
    }

    giatruck[type] = value;
    new names[4][24] = {"Thuc An", "Vat Lieu", "Quan Ao", "Hang Cam"};
    TruckPrice_Save();
    UpdateTruckerPrices();

    SendClientMessageEx(playerid, COLOR_GREEN,
        "[Admin-Trucker] Da doi gia {44ff00}%s{FFFFFF} thanh {5BFF89}$%d{FFFFFF}.", names[type], value);
    foreach(new i : Player) {
        if(PlayerInfo[i][pAdmin] > 0 && i != playerid)
            SendClientMessageEx(i, COLOR_RED,
                "[Admin-Trucker] %s doi gia %s thanh $%d.", GetPlayerNameEx(playerid), names[type], value);
    }
    return 1;
}

CMD:checkgiahang(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1) return 1;

    new szDialog[1024], string[128];
    strcat(szDialog, "Loai hang\tGia mua\tGia ban cao nhat\n");

    new highPrices[4];
    for(new i = 0; i < sizeof(ThuMuaTruck); i++) {
        new l = ThuMuaTruck[i][loaihang];
        if(l >= 1 && l <= 3 && ThuMuaTruck[i][giabanhientai] > highPrices[l])
            highPrices[l] = ThuMuaTruck[i][giabanhientai];
    }
    format(string, sizeof(string), "Thuc An\t$%s\t$%s\n",  number_format(giatruck[0]), number_format(highPrices[1])); strcat(szDialog, string);
    format(string, sizeof(string), "Vat Lieu\t$%s\t$%s\n", number_format(giatruck[1]), number_format(highPrices[2])); strcat(szDialog, string);
    format(string, sizeof(string), "Quan Ao\t$%s\t$%s\n",  number_format(giatruck[2]), number_format(highPrices[3])); strcat(szDialog, string);
    format(string, sizeof(string), "Hang Cam\t$%s\t-\n",   number_format(giatruck[3])); strcat(szDialog, string);

    ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_TABLIST_HEADERS,
        "Gia Hang Hoa Trucker", szDialog, "Dong", "");
    return 1;
}

// VIP Platinum: xem gia thu mua cao nhat
CMD:giahang(playerid, params[])
{
    if(PlayerInfo[playerid][pDonateRank] < 3 && PlayerInfo[playerid][pAdmin] < 1338) return 1;

    new szDialog[2000], s[96], zone[MAX_ZONE_NAME];
    strcat(szDialog, "Diem\tLoai Hang\tKhu Vuc\tGia Thu Mua\n");
    for(new i = 0; i < sizeof(ThuMuaTruck); i++) {
        Get3DZone(KhuVucThuMua[i][0], KhuVucThuMua[i][1], KhuVucThuMua[i][2], zone, sizeof(zone));
        format(s, sizeof(s), "%d\t%s\t%s\t{4dad2b}$%s\n",
            ThuMuaTruck[i][idhang],
            ThuMuaTruck[i][tenhang],
            zone,
            number_format(ThuMuaTruck[i][giabanhientai]));
        strcat(szDialog, s);
    }

    ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_TABLIST_HEADERS,
        "Gia Thu Mua Tung Diem (VIP)", szDialog, "Dong", "");
    return 1;
}

// Admin: reload gia
CMD:reloadgiatruck(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] == 0) return 1;
    UpdateTruckerPrices();
    SendClientMessageEx(playerid, COLOR_GREEN, "[Admin-Trucker] Da reload gia hang hoa.");
    return 1;
}
