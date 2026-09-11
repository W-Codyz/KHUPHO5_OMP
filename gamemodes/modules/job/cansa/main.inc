#include <YSI\YSI_Coding\y_hooks>

new dangchebien[MAX_PLAYERS];
new inputcansa[MAX_PLAYERS];
#define MONEY_SELL_CANSA 350 // money sell 1 tep can sa
#define THUNGRAC 21
#define GIAOCANSA 10
#define INVALID_FLOAT -1.0

#define LUCLOI_TIME 15
#define TIME_RESPAWN_THUNGRAC 120
#define CANSA_ANTI_TELEPORT_TIME 10
new antitelecansa[MAX_PLAYERS];
new NhanDiemCount[MAX_PLAYERS];
new GiaoCanSaTimer[MAX_PLAYERS];
enum thungrac_enum {
    thungrac_id,
    Text3D:thungrac_label,
    bool:thungrac_status,
    Float:thungrac_X,
    Float:thungrac_Y,
    Float:thungrac_Z,
}
new Float:giaocansa_pos[GIAOCANSA][] = {
    {-2161.4504,-2361.0203,30.7851},
    {-1986.3915,-2424.1895,30.6250},
    {-1570.1198,-2719.2937,48.7435},
    {-1059.8779,-1653.5802,76.9463},
    {-480.7922,-563.5710,25.2056},
    {-1031.5946,-587.6744,32.0078},
    {33.7156,-2655.7175,40.5119},
    {-122.3480,-232.8315,1.4219},
    {-2006.5918,-490.1169,35.5313},
    {-1924.7406,-200.4326,25.7704}
};
new Float:thungrac_pos[THUNGRAC][] = {
    {-1672.8477,-906.9116,63.9474},
    {-1672.1248,-909.6394,63.8610},
    {-1674.3303,-922.0483,64.7770},
    {-1680.5518,-923.2217,65.8264},
    {-1687.5667,-926.2748,67.0700},
    {-1689.0289,-934.1330,67.6509},
    {-1682.8035,-938.0014,66.8403},
    {-1676.5183,-941.1469,65.9881},
    {-1676.8756,-948.8638,66.9618},
    {-1682.8094,-950.8898,67.7234},
    {-1690.8478,-952.0093,68.5269},
    {-1695.1565,-945.6532,69.0609},
    {-1698.6744,-937.9604,69.3442},
    {-1698.0914,-929.1716,68.8607},
    {-1671.6605,-910.0318,63.8015},
    {-1689.6769,-959.6280,69.5031},
    {-1680.5039,-961.9857,69.0970},
    {-1675.1395,-973.6382,70.2621},
    {-1684.3187,-979.5501,71.2242},
    {-1694.6453,-985.7333,72.2816},
    {-1709.9215,-980.9931,73.5295}
};

new bool:MocRac[MAX_PLAYERS] = false;
new ThungRac[THUNGRAC][thungrac_enum];

stock IsPlayerNearThungRac(playerid) {
    for(new i = 0 ; i < THUNGRAC; i++) {
        if(ThungRac[i][thungrac_X] != INVALID_FLOAT && IsPlayerInRangeOfPoint(playerid, 4.0, ThungRac[i][thungrac_X],ThungRac[i][thungrac_Y],ThungRac[i][thungrac_Z])) {
            return i;
        }
    }
    return floatround(INVALID_FLOAT);
}

forward ThungRacCreate(iMar);
public ThungRacCreate(iMar) {
    ThungRac[iMar][thungrac_label] = CreateDynamic3DTextLabel("{00ff00}Cay Can Sa\n{FFFF00}[Y] de bat dau hai", -1, thungrac_pos[iMar][0],thungrac_pos[iMar][1],thungrac_pos[iMar][2]+0.5, 5);
    ThungRac[iMar][thungrac_id] = CreateDynamicObject(19473, thungrac_pos[iMar][0],thungrac_pos[iMar][1],thungrac_pos[iMar][2]-0.7, 0,0,0);
    ThungRac[iMar][thungrac_X] = thungrac_pos[iMar][0];
    ThungRac[iMar][thungrac_Y] = thungrac_pos[iMar][1];
    ThungRac[iMar][thungrac_Z] = thungrac_pos[iMar][2];
    ThungRac[iMar][thungrac_status] = false;
    printf("[Can Sa] Can Sa %d | X%f | Y%f | Z%f", iMar, ThungRac[iMar][thungrac_X],ThungRac[iMar][thungrac_Y],ThungRac[iMar][thungrac_Z]);
    return 1;
}

forward TienTrinhHaiCanSa(playerid);
public TienTrinhHaiCanSa(playerid)
{
    if(!MocRac[playerid]) return 1;

    new time = GetPVarInt(playerid, "ThoiGianHaiCanSa");
    new iMar = GetPVarInt(playerid, "IDCanSaDangHai");

    if(iMar < 0 || iMar >= THUNGRAC) return 1;

    if(!IsPlayerInRangeOfPoint(playerid, 3.0, ThungRac[iMar][thungrac_X], ThungRac[iMar][thungrac_Y], ThungRac[iMar][thungrac_Z]))
    {
        StopLoading(playerid);
        MocRac[playerid] = false;
        ThungRac[iMar][thungrac_status] = false;
        ClearAnimations(playerid);
        DeletePVar(playerid, "ThoiGianHaiCanSa");
        DeletePVar(playerid, "IDCanSaDangHai");
        SendClientMessage(playerid, -1, "{FFFF00}GTAHTB{FFFFFF} Ban da di qua xa, viec hai can sa da bi huy.");
        return 1;
    }

    ApplyAnimation(playerid, "BOMBER", "BOM_Plant_Loop", 4.1, 0, 0, 0, 0, 0);

    time--;
    SetPVarInt(playerid, "ThoiGianHaiCanSa", time);

    if(time > 0)
    {
        SetTimerEx("TienTrinhHaiCanSa", 1000, false, "d", playerid);
    }
    return 1;
}

forward OnLoadingHaiCanSa(playerid);
public OnLoadingHaiCanSa(playerid)
{
    if(!MocRac[playerid]) return 1;

    new iMar = GetPVarInt(playerid, "IDCanSaDangHai");
    if(iMar < 0 || iMar >= THUNGRAC || !IsPlayerInRangeOfPoint(playerid, 3.0, ThungRac[iMar][thungrac_X], ThungRac[iMar][thungrac_Y], ThungRac[iMar][thungrac_Z])) {
        MocRac[playerid] = false;
        if(iMar >= 0 && iMar < THUNGRAC) ThungRac[iMar][thungrac_status] = false;
        ClearAnimations(playerid);
        DeletePVar(playerid, "ThoiGianHaiCanSa");
        DeletePVar(playerid, "IDCanSaDangHai");
        return 1;
    }

    OnPlayerMocRac(playerid, iMar);
    return 1;
}

forward OnPlayerMocRac(playerid, iMar);
public OnPlayerMocRac(playerid, iMar)
{
    new string[324];
    ClearAnimations(playerid);
    MocRac[playerid] = false;

    DeletePVar(playerid, "ThoiGianHaiCanSa");
    DeletePVar(playerid, "IDCanSaDangHai");

    PlayerInfo[playerid][pCanSa]++;
    format(string, sizeof(string), "{FFFF00}GTAHTB{FFFFFF} Ban da hai duoc 1 cay can sa {00FF00} (/cansa de xem vi tri che bien)");
    SendClientMessage(playerid, -1, string);
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "+1 cay can sa (/tuido).");

    ThungRac[iMar][thungrac_X] = INVALID_FLOAT;
    DestroyDynamicObject(ThungRac[iMar][thungrac_id]);
    DestroyDynamic3DTextLabel(ThungRac[iMar][thungrac_label]);
    SetTimerEx("ThungRacCreate", TIME_RESPAWN_THUNGRAC*1000, false, "d", iMar);
    //printf("Pha huy can sa %d", iMar);
    return 1;
}

hook OnGameModeInit()
{
    for(new i = 0; i < GIAOCANSA; i++)
    {
        CreateActor(294, giaocansa_pos[i][0], giaocansa_pos[i][1], giaocansa_pos[i][2], 0);
        CreateDynamic3DTextLabel("<Mafia>\nEyyo thang cu! may nhin cai gi ?", -1, giaocansa_pos[i][0], giaocansa_pos[i][1], giaocansa_pos[i][2]+1.3,5.0);
    }
    new i = 0;
    while(i < THUNGRAC)
    {
        ThungRacCreate(i);
        i++;
    }
    new bancannsa = CreateActor(29, -382.8928,-1426.2904,26.2979, 0);
    CreateDynamic3DTextLabel("{009933}Thu mua can sa\n Bam [Y] de tuong tac", -1, -382.8928,-1426.2904,26.2979+0.6,18.0);
    new matuy = CreateActor(200, -1431.5900,-1460.4982,101.6989, 0);
    SetActorFacingAngle(matuy, 271.1635);
    SetActorFacingAngle(bancannsa, 268.3628);
    CreateDynamic3DTextLabel("{009933}Che bien ma tuy\n Bam [Y] de tuong tac", -1, -1431.5900,-1460.4982,101.6989+0.6,18.0);
    return 1;
}
hook OnPlayerConnect(playerid)
{
    inputcansa[playerid] = 0;
    NhanDiemCount[playerid] = 0;
    PlayerGiaoCanSa[playerid] = 0;
    gdCanSa[playerid] = INVALID_PLAYER_ID;
    return 1;
}
hook OnPlayerDisconnect(playerid, reason)
{
    if(GiaoCanSaTimer[playerid])
        KillTimer(GiaoCanSaTimer[playerid]);
    if(MocRac[playerid]) {
        StopLoading(playerid);
        new iMar = GetPVarInt(playerid, "IDCanSaDangHai");
        ThungRac[iMar][thungrac_status] = false;
        DeletePVar(playerid, "ThoiGianHaiCanSa");
        DeletePVar(playerid, "IDCanSaDangHai");
        MocRac[playerid] = false;
    }
    return 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(newkeys & KEY_YES)
    {
        new iThungRac = IsPlayerNearThungRac(playerid);
        if(iThungRac != floatround(INVALID_FLOAT))
        {
            if(IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, -1, "{FFFF00}GTAHTB{FFFFFF} Ban khong the lam dieu nay o tren xe!");
            if(PlayerCuffed[playerid] >= 1 || PlayerInfo[playerid][pJailTime] > 0 || GetPVarInt(playerid, "Injured")) return SendClientMessage(playerid, -1, "{FFFF00}GTAHTB{FFFFFF} Ban khong the lam dieu do luc nay....");
            if(PlayerInfo[playerid][pMember]!=INVALID_GROUP_ID) return  SendClientMessageEx(playerid,COLOR_GRAD1,"Ban dang trong 1 to chuc, khong the lam dieu nay!");
            if(!ThungRac[iThungRac][thungrac_status])
            {
                if(!MocRac[playerid])
                {
                    if(PlayerInfo[playerid][pCanSa] < 20)
                    {
                        MocRac[playerid] = true;
                        ThungRac[iThungRac][thungrac_status] = true;
                        ApplyAnimation(playerid, "BOMBER", "BOM_Plant_Loop", 4.1, 0, 0, 0, 0, 0);
                        SetPVarInt(playerid, "ThoiGianHaiCanSa", LUCLOI_TIME);
                        SetPVarInt(playerid, "IDCanSaDangHai", iThungRac);
                        SetTimerEx("TienTrinhHaiCanSa", 1000, false, "d", playerid);
                        SetLoading(playerid, LUCLOI_TIME * 1000, "Dang hai can sa...", "OnLoadingHaiCanSa");
                        SendClientMessage(playerid, -1, "{00FF00}Bat dau hai can sa...");
                    }
                    else
                    {
                        SendClientMessage(playerid, -1, "{FFFF00}GTAHTB{FFFFFF} Ban da hai du 20 cay can sa, hay di che bien no.");
                    }
                }
                else
                {
                    SendClientMessage(playerid, -1, "{FFFF00}GTAHTB{FFFFFF} Ban khong the thuc hien vao luc nay!");
                }
            }
            else
            {
                SendClientMessage(playerid, -1, "{FFFF00}GTAHTB{FFFFFF} Cay can sa nay dang co nguoi hai.");
            }
        }
    }

    if(newkeys & KEY_YES)
    {
        if(IsPlayerInRangeOfPoint(playerid, 5.0, -1431.5900,-1460.4982,101.6989))
        {
            ShowPlayerDialog(playerid, BANCANLIST, DIALOG_STYLE_LIST, "{ffff00}GTAHTB{ffffff} - Vui long chon...?","Che bien can sa\t1 Can sa/3 Tep can sa\nQuy doi Pot\t1 Can sa/1 Pot\nQuy doi Crack\t1 Can sa/1 Crack", "Chon", "Huy Bo");
        }
        else if(IsPlayerInRangeOfPoint(playerid, 5.0, -382.8928,-1426.2904,26.2979))
        {
            ShowPlayerDialog(playerid, DIALOG_UPGRADECANSA, DIALOG_STYLE_LIST, "{ffff00}GTAHTB{ffffff} - Vui long chon...?","{ffffff}Ban can sa (IT TIEN)\n{ff0000}Giao can sa theo yeu cau [NHIEU TIEN / HOT]{ffffff}", "Chon", "Huy Bo");
        }
    }
    return 1;
}

CMD:cansa(playerid, params[])
{
	ShowPlayerDialog(playerid, CANSA_LIST, DIALOG_STYLE_LIST,"{ffff00}GTAHTB{ffffff} - Can Sa", "Tim duong can sa\nXem can sa", "Chon", "Thoat");
	return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_UPGRADECANSA)
    {
        if(response)
        {
            if (listitem == 0)
            {
                new stringb[128];
                format(stringb, sizeof stringb, "{ffffff}Hien tai bang dang co %d tep can sa \n1 tep can sa/1700$", PlayerInfo[playerid][pTepCanSa]);
                ShowPlayerDialog(playerid, DIALOG_BANCANSA, DIALOG_STYLE_INPUT,"{ffff00}GTAHTB{ffffff} - Ban Can Sa", stringb, "Ban", "Thoat");
            }
            if (listitem == 1)
            {
                new stringb[128];
                format(stringb, sizeof stringb, "{ffffff}Hien tai bang dang co %d tep can sa \n1 tep can sa tu 1.500$ tro len, ban se duoc nhan them tuy vao chuyen giao xa hoac gan.", PlayerInfo[playerid][pTepCanSa]);
                ShowPlayerDialog(playerid, DIALOG_NHANDIEMGIAOHANG, DIALOG_STYLE_INPUT,"{ffff00}GTAHTB{ffffff} - Ban Can Sa", stringb, "Xac nhan", "Thoat");
            }
        }
    }
    if(dialogid == DIALOG_NHANDIEMGIAOHANG)
    {
        if(response)
        {
            new amount = strval(inputtext);
            if (amount > 0)
            {
                if (PlayerInfo[playerid][pTepCanSa] >= amount)
                {
                    if (NhanDiemCount[playerid] != 0)
                        return SendClientMessage(playerid, COLOR_GREY, "Ban dang co mot don hang giao can sa roi, hay hoan thanh no.");
                    GiaoCanSaTimer[playerid] = SetTimerEx("NhanDiemGiaoHangCanSa", 1000, 0, "d", playerid);
                    NhanDiemCount[playerid] = amount*5;
                    inputcansa[playerid] = amount;
                    antitelecansa[playerid] = CANSA_ANTI_TELEPORT_TIME;
                    SetTimerEx("antiteleportcansa", 1000, false, "d", playerid);
                }
                else
                {
                    SendClientMessage(playerid, COLOR_GREY, "Ban khong co du so luong can sa de van chuyen.");
                    ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong co du so luong can sa de van chuyen.");
                }
            }
            else
            {
                SendClientMessage(playerid, COLOR_GREY, "Ban khong the nhap so luong be hon 0.");
                ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong the nhap so luong be hon 0.");
            }
        }
    }
	if(dialogid == CANSA_LIST)
	{
		if(response)
		{
			if(listitem == 0)
			{
				ShowPlayerDialog(playerid, TIMDUONGCANSA, DIALOG_STYLE_LIST,"{ffff00}GTAHTB{ffffff} - Can Sa", "Vi tri can sa moc\nChe bien can sa\nBan can sa", "Chon", "Thoat");
				return 1;
			}
			if(listitem == 1)
			{
				new stringbx[128];
				format(stringbx, sizeof stringbx, "Cay can sa:\t %d\nTep can sa:\t %d\nPot:\t\t %d\nCrack:\t\t %d", PlayerInfo[playerid][pCanSa], PlayerInfo[playerid][pTepCanSa], PlayerInfo[playerid][pPot], PlayerInfo[playerid][pCrack]);
				ShowPlayerDialog(playerid, DIALOG_MSG, DIALOG_STYLE_MSGBOX, "{ffff00}GTAHTB{ffffff} - Xem can sa", stringbx, "Dong y", "");
			}
		}
	}

	if(dialogid == TIMDUONGCANSA)
	{
		if(response)
		{
			if(listitem == 0)
			{
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCheckpoint(playerid, -1682.8035,-938.0014,66.8403, 3.0);
				CP[playerid] = 252000;
				return 1;
			}
			if(listitem == 1)
			{
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCheckpoint(playerid, -1431.5900,-1460.4982,101.6989, 3.0);
				CP[playerid] = 252000;
				return 1;
			}
			if(listitem == 2)
			{
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCheckpoint(playerid, -382.8928,-1426.2904,26.2979, 3.0);
				CP[playerid] = 252000;
				return 1;
			}
		}
	}

	if(dialogid == DIALOG_BANCANSA) {
		if(response) {
			new amount = strval(inputtext);
			if(amount > 0) {
				if(PlayerInfo[playerid][pTepCanSa] >= amount) {
					new stringcs[128];
					PlayerInfo[playerid][pTepCanSa] -= amount;
					GivePlayerCash(playerid, amount*MONEY_SELL_CANSA);
					format(stringcs, sizeof(stringcs), "{ffff00}GTAHTB{ffffff} Ban da ban %d tui can sa va nhan duoc $%d", amount, amount*MONEY_SELL_CANSA);
					SendClientMessage(playerid, -1, stringcs);
				}
				else
					SendClientMessage(playerid, -1,"{ffff00}GTAHTB{ffffff} - So luong khong hop le! Hay kiem tra lai so luong cua ban.");
			}
			else
				SendClientMessage(playerid, -1,"{ffff00}GTAHTB{ffffff} - So luong can ban phai lon hon 0!");
		}
		return 1;
	}

	if(dialogid == DIALOG_POT) {
		if(response) {
			new amount = strval(inputtext);
			if(amount > 0) {
				if(PlayerInfo[playerid][pCanSa] >= amount) {
					new stringss[128];
					PlayerInfo[playerid][pCanSa] -= amount;
					PlayerInfo[playerid][pPot] += amount*1;
					format(stringss, sizeof(stringss), "{FFFF00}GTAHTB{FFFFFF} Ban da doi %d cay can sa va nhan duoc %d pot.", amount, amount*1);
					SendClientMessage(playerid, -1, stringss);
				}
				else
					SendClientMessage(playerid, -1,"{FFFF00}GTAHTB{FFFFFF} So luong khong hop le! Hay kiem tra lai so luong cua ban.");
			}
			else
				SendClientMessage(playerid, -1,"{FFFF00}GTAHTB{FFFFFF} So luong can doi phai lon hon 0!");
		}
		return 1;
	}

	if(dialogid == DIALOG_CRACK) {
		if(response) {
			new amount = strval(inputtext);
			if(amount > 0) {
				if(PlayerInfo[playerid][pCanSa] >= amount) {
					new strings[128];
					PlayerInfo[playerid][pCanSa] -= amount;
					PlayerInfo[playerid][pCrack] += amount*1;
					format(strings, sizeof(strings), "{FFFF00}GTAHTB{FFFFFF} Ban da doi %d cay can sa va nhan duoc %d crack.", amount, amount*1);
					SendClientMessage(playerid, -1, strings);
				}
				else
					SendClientMessage(playerid, -1,"{FFFF00}GTAHTB{FFFFFF} So luong khong hop le! Hay kiem tra lai so luong cua ban.");
			}
			else
				SendClientMessage(playerid, -1,"{FFFF00}GTAHTB{FFFFFF} So luong can doi phai lon hon 0!");
		}
		return 1;
	}

	if(dialogid == BANCANLIST)
	{
		if(response)
		{
			if(listitem == 0)
			{
                if(dangchebien[playerid] == 1)
                {
                    SendClientMessage(playerid, -1, "Ban dang che bien roi, khong the tiep tuc.");
                    ShowTDN(playerid, NOTIFICATION_WARNING, "Ban dang che bien roi.");
                    return 1;
                }
                if(PlayerInfo[playerid][pCanSa] < 1)
                {
                    SendClientMessage(playerid, -1, "Ban khong co cay can sa nao de che bien.");
                    ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong co cay can sa nao.");
                    return 1;
                }

				SetPVarInt(playerid, "CheBienTime", 15);
                ApplyAnimation(playerid, "BD_FIRE", "wash_up", 4.0, 1, 0, 0, 0, 0, 1);
                SetLoading(playerid, 15000, "Dang che bien can sa...", "OnLoadingCheBienFinish");
				dangchebien[playerid] = 1;
			}
			if(listitem == 1)
			{
				new stringz[128];
				format(stringz, sizeof stringz, "Hien tai ban dang co %d cay can sa.\n1 cay can sa / 1 pot.\nNhap so luong cay can sa ban muon doi:", PlayerInfo[playerid][pCanSa]);
				ShowPlayerDialog(playerid, DIALOG_POT, DIALOG_STYLE_INPUT,"{FFFF00}GTAHTB{FFFFFF} - Quy Doi Pot", stringz, "Doi", "Thoat");
			}
			if(listitem == 2)
			{
				new stringzz[128];
				format(stringzz, sizeof stringzz, "Hien tai ban dang co %d cay can sa.\n1 cay can sa / 1 crack.\nNhap so luong cay can sa ban muon doi:", PlayerInfo[playerid][pCanSa]);
				ShowPlayerDialog(playerid, DIALOG_CRACK, DIALOG_STYLE_INPUT,"{FFFF00}GTAHTB{FFFFFF} - Quy Doi Crack", stringzz, "Doi", "Thoat");
			}
		}
	}
	return 1;
}
hook OnPlayerEnterCheckpoint(playerid)
{
    if (PlayerGiaoCanSa[playerid] != 0)
    {
        for (new i = 0; i < 10; i++)
        {
            if (IsPlayerInRangeOfPoint(playerid, 10.0, giaocansa_pos[i][0], giaocansa_pos[i][1], giaocansa_pos[i][2]))
            {
                if (antitelecansa[playerid] > 0 )
                {
                    new szMess[256];
                    SetPlayerPos(playerid, LastPosF4T[playerid][0], LastPosF4T[playerid][1], LastPosF4T[playerid][2]);
                    format(szMess, sizeof(szMess), "%s da bi kick vi su dung hack Teleport Giao can sa.", GetPlayerNameEx(playerid));
                    ABroadCast(COLOR_YELLOW, szMess, 2);
                    discord_messHack(szMess);
                    SendClientMessageEx(playerid, COLOR_WHITE, "[{DD0000}Anti-F4T{FFFFFF}] Anti cheating Teleport Can sa, neu co hack that hay tat giup tuiiii...!");
                    SetTimerEx("KickEx", 2000, 0, "i", playerid);
                    return 1;
                }
                DisableCheckPoint(playerid);
                SendClientMessageEx(playerid, COLOR_WHITE, "[{DD0000}Anti-F4T{FFFFFF}] Dang kiem tra CLEO/CHEAT...");
                NhanTienCanSa(playerid);
            }
        }
    }
    return 1;
}
forward NhanDiemGiaoHangCanSa(playerid);
public NhanDiemGiaoHangCanSa(playerid)
{
    if (NhanDiemCount[playerid] > 0)
    {
        if (IsPlayerInRangeOfPoint(playerid, 30.0, -382.8928,-1426.2904,26.2979))
        {
            new str[256];
            NhanDiemCount[playerid]--;
            format(str, sizeof(str), "~r~Vui long doi ~w~%d ~r~giay nua de nhan duoc diem giao hang~n~Khong duoc di chuyen qua xa khu vuc nhan diem giao hang", NhanDiemCount[playerid]);
            GameTextForPlayer(playerid, str, 1000, 5);
            GiaoCanSaTimer[playerid] = SetTimerEx("NhanDiemGiaoHangCanSa", 1000, 0, "d", playerid);
        }
        else
        {
            SendClientMessage(playerid, COLOR_LIGHTRED, "Nhan diem giao hang that bai do ban da di chuyen ra khoi khu vuc nhan diem giao hang.");
            ShowTDN(playerid, NOTIFICATION_WARNING, "Nhan diem giao hang that bai.");
            NhanDiemCount[playerid] = 0;
            antitelecansa[playerid] = 0;
            inputcansa[playerid] = 0;
        }
    }
    else
    {
        new rand = Random(0, 9);
        new Float: x, Float: y, Float:z;
        new str[100], zone[MAX_ZONE_NAME];
        GetPlayerPos(playerid, x,y,z);
        PlayerGiaoCanSa[playerid] = GetDistanceBetweenPoints(x,y,z,giaocansa_pos[rand][0],giaocansa_pos[rand][1],giaocansa_pos[rand][2]);
        GameTextForPlayer(playerid, "~w~Hay di den ~r~Checkpoint~w~ de giao hang!", 10000, 5);
        Get3DZone(giaocansa_pos[rand][0],giaocansa_pos[rand][1],giaocansa_pos[rand][2],zone, sizeof(zone));
        format(str, sizeof(str), "Co mot doi tuong dang giao can sa huong ve {ffffff}%s{aa3333} hay den do va tieu diet.", zone);
        foreach(new i: Player)
        {
            if (IsACop(i) && PlayerInfo[i][pDuty] == 1)
                SendClientMessage(i , COLOR_RED, str);
        }
        SetPlayerCheckpoint(playerid, giaocansa_pos[rand][0],giaocansa_pos[rand][1],giaocansa_pos[rand][2], 3.0);
        CP[playerid] = 252000;
    }
    return 1;
}
forward CheBien(playerid);
public CheBien(playerid)
{
	new time = GetPVarInt(playerid, "CheBienTime") - 1;
	SetPVarInt(playerid, "CheBienTime", time);

	new string[128];
	format(string, sizeof(string), "~n~~n~~n~~n~~n~~n~~n~~n~~n~~w~%d Giay Con Lai", time);
	GameTextForPlayer(playerid, string, 1100, 3);
	ApplyAnimation(playerid, "BD_FIRE", "wash_up", 4.0, 1, 0, 0, 0, 0, 1);

	if(time > 0)
	{
		SetTimerEx("CheBien", 1000, 0, "d", playerid);
	}
	else
	{
		ClearAnimations(playerid);
		DeletePVar(playerid, "CheBienTime");

		dangchebien[playerid] = 0;

		PlayerInfo[playerid][pCanSa] -= 1;
		PlayerInfo[playerid][pTepCanSa] += 3;
		SendClientMessage(playerid, 0x33FF33FF, "{FFFF00}GTAHTB{FFFFFF} Ban da che bien thanh cong 1 cay can sa va nhan duoc 3 tep can sa.");
	}
	return 1;
}

forward OnLoadingCheBienFinish(playerid);
public OnLoadingCheBienFinish(playerid)
{
    if(!dangchebien[playerid]) return 1;

    DeletePVar(playerid, "CheBienTime");
    dangchebien[playerid] = 0;

    if(PlayerInfo[playerid][pCanSa] < 1) {
        SendClientMessage(playerid, -1, "Ban khong con cay can sa de che bien.");
        return 1;
    }

    ClearAnimations(playerid);
    PlayerInfo[playerid][pCanSa] -= 1;
    PlayerInfo[playerid][pTepCanSa] += 3;
    SendClientMessage(playerid, 0x33FF33FF, "{FFFF00}GTAHTB{FFFFFF} Ban da che bien thanh cong 1 cay can sa va nhan duoc 3 tep can sa.");
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Che bien thanh cong.");
    return 1;
}

hook OnPlayerDeath(playerid, killerid, reason) {
    if(IsLoadingActive(playerid)) {
        StopLoading(playerid);
    }
    if(MocRac[playerid]) {
        new iMar = GetPVarInt(playerid, "IDCanSaDangHai");
        if(iMar >= 0 && iMar < THUNGRAC) ThungRac[iMar][thungrac_status] = false;
        MocRac[playerid] = false;
        DeletePVar(playerid, "ThoiGianHaiCanSa");
        DeletePVar(playerid, "IDCanSaDangHai");
    }
    if(dangchebien[playerid]) {
        dangchebien[playerid] = 0;
        DeletePVar(playerid, "CheBienTime");
    }
	if(gPlayerLogged{playerid} == 1) {
        if (PlayerGiaoCanSa[playerid] != 0)
        {
            if (killerid != INVALID_PLAYER_ID && gPlayerLogged{killerid} == 1 && IsACop(killerid) && PlayerInfo[killerid][pDuty] == 1)
            {
                new string[128], reasons[56];
                PlayerInfo[playerid][pTepCanSa] -= inputcansa[playerid];
                format(string, sizeof(string), "Ban da that bai trong viec giao can sa va bi bat va mat %d tep.", inputcansa[playerid]);
                SendClientMessage(playerid, COLOR_RED, string);
                DisableCheckPoint(playerid);
                inputcansa[playerid] = 0;
                PlayerGiaoCanSa[playerid] = 0;
                SetPlayerArmedWeapon(playerid, 0);
                if(GetPVarInt(playerid, "Injured") == 1)
                {
                    KillEMSQueue(playerid);
                    ClearAnimations(playerid);
                }
                format(reasons, sizeof(reasons), "Giao can sa that bai");
                GameTextForPlayer(playerid, "~w~Welcome to ~n~~r~Fort DeMorgan", 5000, 3);
                ResetPlayerWeaponsEx(playerid);
                format(string, sizeof(string), "%s da bi di tu boi %s, ly do: %s", GetPlayerNameEx(playerid), GetPlayerNameEx(killerid), reasons);
                Log("logs/cansathatbai.log", string);
                format(string, sizeof(string), "%s da bi phat tu 10 phut boi %s, ly do: %s", GetPlayerNameEx(playerid), GetPlayerNameEx(killerid), reasons);
                SendClientMessageToAllEx(COLOR_LIGHTRED, string);
                StaffAccountCheck(playerid, GetPlayerIpEx(playerid));
                PlayerInfo[playerid][pWantedLevel] = 0;
                SetPlayerWantedLevel(playerid, 0);
                SafeSetPlayerHealth(playerid, 100);
                PlayerInfo[playerid][pJailTime] = 10*60;
                SetPVarInt(playerid, "_rAppeal", gettime()+60);			format(PlayerInfo[playerid][pPrisonReason], 128, "[IC][PRISON] %s", reasons);
                format(PlayerInfo[playerid][pPrisonedBy], MAX_PLAYER_NAME, "%s", GetPlayerNameEx(killerid));
                PhoneOnline[playerid] = 1;
                SetPlayerInterior(playerid, 0);
                PlayerInfo[playerid][pInt] = 0;
                new rand = random(sizeof(OOCPrisonSpawns));
                Streamer_UpdateEx(playerid, OOCPrisonSpawns[rand][0], OOCPrisonSpawns[rand][1], OOCPrisonSpawns[rand][2]);
                SafeSetPlayerPos(playerid, OOCPrisonSpawns[rand][0], OOCPrisonSpawns[rand][1], OOCPrisonSpawns[rand][2]);
                SetPlayerSkin(playerid, 50);
                SetPlayerColor(playerid, TEAM_APRISON_COLOR);
                Player_StreamPrep(playerid, OOCPrisonSpawns[rand][0], OOCPrisonSpawns[rand][1], OOCPrisonSpawns[rand][2], FREEZE_TIME);
            }
            else
            {
                new str[256];
                PlayerInfo[playerid][pTepCanSa] -= inputcansa[playerid];
                PlayerInfo[killerid][pTepCanSa] += inputcansa[playerid];
                format(str, sizeof(str), "Ban da that bai trong viec giao can sa va bi cuop di mat %d tep.", inputcansa[playerid]);
                SendClientMessage(playerid, COLOR_RED, str);
                format(str, sizeof(str), "Ban da thanh cong trong viec cuop can sa va da cuop duoc %d tep.", inputcansa[playerid]);
                SendClientMessage(killerid, COLOR_YELLOW, str);
                DisableCheckPoint(playerid);
                inputcansa[playerid] = 0;
                PlayerGiaoCanSa[playerid] = 0;
            }
        }
	}
	return 1;
}


forward antiteleportcansa(playerid);
public antiteleportcansa(playerid)
{
    if (antitelecansa[playerid] > 0)
    {
        antitelecansa[playerid]--;
        SetTimerEx("antiteleportcansa", 1000, false, "d", playerid);
    }
} 

NhanTienCanSa(playerid)
{

    new moneybonus = floatround(PlayerGiaoCanSa[playerid]*1.5);
    new moneytotal = inputcansa[playerid] * (900 + moneybonus);
    new str[128];
    format(str, sizeof(str), "Giao can sa thanh cong va nhan duoc %d$!", moneytotal);
    SendClientMessage(playerid, -1, str);
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Giao can sa thanh cong!");
    GivePlayerCash(playerid, moneytotal);
    PlayerInfo[playerid][pTepCanSa] -= inputcansa[playerid];
    inputcansa[playerid] = 0;
    PlayerGiaoCanSa[playerid] = 0;
}

CMD:bantepcansa(playerid, params[])
{
    new string[128], playa, tepcs, money;
    if(sscanf(params, "udd", playa, tepcs, money))
	{
        SendClientMessageEx(playerid, COLOR_GREY, "SU DUNG: /bantepcansa [Player] [so luong] [so tien]");
        return 1;
    }
    if (PlayerInfo[playerid][pTepCanSa] < tepcs)
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Ban khong co du so luong tep can sa.");
        ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong co du so luong tep can sa.");
    }
    if (money < 1000 || money > 1000000)
    {
        SendClientMessageEx(playerid, COLOR_GREY, "So tien chi co the tu 1.000$ den 1.000.000$.");
        ShowTDN(playerid, NOTIFICATION_WARNING, "Hay nhap so tien khac.");
    }
    if (tepcs < 10 || tepcs > 1000)
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Tep can sa chi co the dua tu 10 den 1000 tep.");
        ShowTDN(playerid, NOTIFICATION_WARNING, "Hay thu lai sau.");
    }
    if (ProxDetectorS(8.0, playerid, playa))
    {
        if(IsPlayerConnected(playa))
        {
            if(playa != INVALID_PLAYER_ID)
            {
                format(string, sizeof(string), "Ban de nghi %s mua %d tep can sa, cho nguoi do chap nhan.", GetPlayerNameEx(playa), tepcs);
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, string);
                format(string, sizeof(string), "%s da de nghi ban mua %d tep can sa voi gia %d$", GetPlayerNameEx(playerid), tepcs, money);
                SendClientMessageEx(playa, COLOR_LIGHTBLUE, string);
                SendClientMessageEx(playa, COLOR_LIGHTBLUE, "Su dung lenh /chapnhan tepcansa de chap nhan mua.");
                gdCanSa[playerid] = INVALID_PLAYER_ID;
                gdCanSa[playa] = playerid;
                slCanSa[playa] = tepcs;
                tienCanSa[playa] = money;
            }
        }
    }
    else
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi do khong o gan ban.");
        ShowTDN(playerid, NOTIFICATION_WARNING, "Nguoi choi do khong o gan ban.");
    }


    return 1;
}
CMD:settepcansa(playerid, params[])
{
    new string[128], playa, tepcs;
    if(sscanf(params, "ud", playa, tepcs))
	{
        SendClientMessageEx(playerid, COLOR_GREY, "SU DUNG: /settepcansa [Player] [so luong]");
        return 1;
    }
    if (PlayerInfo[playerid][pAdmin] >= 99999)
	{
        if(IsPlayerConnected(playa))
		{
            if(playa != INVALID_PLAYER_ID)
			{
                PlayerInfo[playerid][pTepCanSa] = tepcs;
                format(string, sizeof(string), "Ban da thiet lap tep can sa %s cho %d.", GetPlayerNameEx(playa), tepcs);
                SendClientMessageEx(playerid, COLOR_WHITE, string);
                ShowTDN(playerid, NOTIFICATION_SUCCESS, string);
            }
        }
    }
    else
	{
        SendClientMessageEx(playerid, COLOR_GRAD1, "Ban khong duoc phep su dung lenh nay.");
        ShowTDN(playerid, NOTIFICATION_ERROR, "Ban khong duoc phep su dung lenh nay.");
    }
    return 1;
}
CMD:checktepcansa(playerid, params[])
{
    new string[128], playa;
    if(sscanf(params, "u", playa))
	{
        SendClientMessageEx(playerid, COLOR_GREY, "SU DUNG: /checktepcansa [Player]");
        ShowTDN(playerid, NOTIFICATION_INFO, "SU DUNG: /checktepcansa [Player]");
        return 1;
    }
    if (PlayerInfo[playerid][pAdmin] >= 4)
	{
        if(IsPlayerConnected(playa))
		{
            if(playa != INVALID_PLAYER_ID)
			{
                format(string, sizeof(string), "So tep can sa cua %s la %d.", GetPlayerNameEx(playa), PlayerInfo[playa][pTepCanSa]);
                SendClientMessageEx(playerid, COLOR_WHITE, string);
            }
        }
    }
    else
	{
        SendClientMessageEx(playerid, COLOR_GRAD1, "Ban khong duoc phep su dung lenh nay.");
        ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong duoc phep su dung lenh nay.");
    }
    return 1;
}
