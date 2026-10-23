#include <YSI\YSI_Coding\y_hooks>

new dangchebien[MAX_PLAYERS];
new inputcansa[MAX_PLAYERS];
new LastCanSaAlert;
#define MONEY_SELL_CANSA 650 // money sell 1 tep can sa
#define THUNGRAC 29
#define GIAOCANSA 7
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
    {-2057.257324,-2508.715332,30.777992},
    {33.454772,-2652.849121,40.495742},
    {-1423.644653,-966.331359,200.798980},
    {-1032.349609,-587.748291,32.007812},
    {-478.559631,-563.545349,25.117458},
    {-347.718536,-1045.655517,59.812500},
    {-439.053924,-63.058433,58.934646}
};
new Float:thungrac_pos[THUNGRAC][] = {
    {-1013.942443,-1635.635131,76.367187},
    {-1010.599365,-1635.756103,76.367187},
    {-1009.950134,-1640.207031,76.367187},
    {-1012.749938,-1640.231323,76.367187},
    {-1013.858337,-1647.123901,76.367187},
    {-1008.373046,-1646.943481,76.367187},
    {-1001.890686,-1646.895019,76.367187},
    {-1002.384521,-1656.829833,76.367187},
    {-1011.037109,-1656.719604,76.373939},
    {-1012.429565,-1664.697509,76.367187},
    {-1006.000549,-1664.652709,76.367187},
    {-997.507080,-1664.699707,76.367187},
    {-988.740112,-1664.628173,76.367187},
    {-980.358459,-1664.631347,76.367187},
    {-975.485900,-1668.309814,76.367187},
    {-979.613342,-1669.516967,76.367187},
    {-986.913452,-1669.646118,76.367187},
    {-991.600646,-1671.229858,76.367187},
    {-999.734008,-1671.135131,76.367187},
    {-1005.169067,-1674.613037,76.367187},
    {-997.855102,-1674.661010,76.367187},
    {-989.313049,-1674.665283,76.367187},
    {-977.981140,-1674.566162,76.328369},
    {-976.256286,-1681.245971,76.196701},
    {-988.337402,-1682.628051,76.342582},
    {-996.619995,-1682.556518,76.367187},
    {-991.142700,-1689.334350,76.273544},
    {-984.491516,-1692.538085,76.129333},
    {-979.269836,-1696.863281,76.067863}
};

new bool:MocRac[MAX_PLAYERS] = false;
new ThungRac[THUNGRAC][thungrac_enum];

stock CanSa_AlertCops(playerid, const action[])
{
    if(gettime() < LastCanSaAlert) return;

    new bool:hasCop = false;
    foreach(new i: Player) { if(IsACop(i) && PlayerInfo[i][pDuty] == 1) { hasCop = true; break; } }

    if(!hasCop) return;

    LastCanSaAlert = gettime() + 60;

    new str[256], zone[MAX_ZONE_NAME], Float:px, Float:py, Float:pz;
    GetPlayerPos(playerid, px, py, pz);
    Get3DZone(px, py, pz, zone, sizeof(zone));
    format(str, sizeof(str), "[CANH BAO] Phat hien nghi can o{aa3333} %s - %s",action, zone);
    foreach(new i: Player) {
        if(IsACop(i) && PlayerInfo[i][pDuty] == 1)
            SendClientMessage(i, COLOR_RED, str);
    }
}

stock HasCopOnDuty()
{
    foreach(new i: Player) { if(IsACop(i) && PlayerInfo[i][pDuty] == 1) return true; }
    return false;
}

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
        SendClientMessage(playerid, -1, "{FFFF00}KP5{FFFFFF} Ban da di qua xa, viec hai can sa da bi huy.");
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
    format(string, sizeof(string), "{FFFF00}KP5{FFFFFF} Ban da hai duoc 1 cay can sa {00FF00} (/cansa de xem vi tri che bien)");
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
    new bancannsa = CreateActor(29, -772.280334,-1962.709472,7.574229, 0);
    CreateDynamic3DTextLabel("{009933}Thu mua can sa\n Bam [Y] de tuong tac", -1, -772.280334,-1962.709472,7.574229+0.6,18.0);
    new matuy = CreateActor(200, -778.561096, -1988.301757, 26.171636, 0);
    SetActorFacingAngle(matuy, 183.813156);
    SetActorFacingAngle(bancannsa, 268.3628);
    CreateDynamic3DTextLabel("{009933}Che bien ma tuy\n Bam [Y] de tuong tac", -1, -778.561096, -1988.301757, 26.171636+0.6,18.0);
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
        if(MocRac[playerid])
        {
            StopLoading(playerid);
            MocRac[playerid] = false;
            new iCancel = GetPVarInt(playerid, "IDCanSaDangHai");
            if(iCancel >= 0 && iCancel < THUNGRAC) ThungRac[iCancel][thungrac_status] = false;
            ClearAnimations(playerid);
            DeletePVar(playerid, "ThoiGianHaiCanSa");
            DeletePVar(playerid, "IDCanSaDangHai");
            SendClientMessage(playerid, -1, "{FFFF00}KP5{FFFFFF} Ban da huy viec hai can sa.");
            return 1;
        }

        new iThungRac = IsPlayerNearThungRac(playerid);
        if(iThungRac != floatround(INVALID_FLOAT))
        {
            if(IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, -1, "{FFFF00}KP5{FFFFFF} Ban khong the lam dieu nay o tren xe!");
            if(PlayerCuffed[playerid] >= 1 || PlayerInfo[playerid][pJailTime] > 0 || GetPVarInt(playerid, "Injured")) return SendClientMessage(playerid, -1, "{FFFF00}KP5{FFFFFF} Ban khong the lam dieu do luc nay....");
            if(PlayerInfo[playerid][pMember]!=INVALID_GROUP_ID) return  SendClientMessageEx(playerid,COLOR_GRAD1,"Ban dang trong 1 to chuc, khong the lam dieu nay!");
            if(!ThungRac[iThungRac][thungrac_status])
            {
                if(!MocRac[playerid])
                {
                    if(PlayerInfo[playerid][pCanSa] < 20)
                    {
                        new haiTime = 12 + random(7);
                        MocRac[playerid] = true;
                        ThungRac[iThungRac][thungrac_status] = true;
                        ApplyAnimation(playerid, "BOMBER", "BOM_Plant_Loop", 4.1, 0, 0, 0, 0, 0);
                        SetPVarInt(playerid, "ThoiGianHaiCanSa", haiTime);
                        SetPVarInt(playerid, "IDCanSaDangHai", iThungRac);
                        SetTimerEx("TienTrinhHaiCanSa", 1000, false, "d", playerid);
                        SetLoading(playerid, haiTime * 1000, "Dang hai can sa...", "OnLoadingHaiCanSa");
                        SendClientMessage(playerid, -1, "{00FF00}Bat dau hai can sa...");
                        SendHintMessage(playerid, "Nhan [Y] lan nua de huy viec hai can sa.");
                    }
                    else
                    {
                        SendClientMessage(playerid, -1, "{FFFF00}KP5{FFFFFF} Ban da hai du 20 cay can sa, hay di che bien no.");
                    }
                }
                else
                {
                    SendClientMessage(playerid, -1, "{FFFF00}KP5{FFFFFF} Ban khong the thuc hien vao luc nay!");
                }
            }
            else
            {
                SendClientMessage(playerid, -1, "{FFFF00}KP5{FFFFFF} Cay can sa nay dang co nguoi hai.");
            }
        }
    }

    if(newkeys & KEY_YES)
    {
        if(IsPlayerInRangeOfPoint(playerid, 5.0, -778.561096,-1988.301757,26.171636))
        {
            if(gettime() > GetPVarInt(playerid, "CanSaAreaCD")) {
                CanSa_AlertCops(playerid, "Khu vuc che bien");
                SetPVarInt(playerid, "CanSaAreaCD", gettime() + 30);
            }
            ShowPlayerDialog(playerid, BANCANLIST, DIALOG_STYLE_LIST, "{ffff00}KP5{ffffff} - Vui long chon...?","Che bien can sa\t1 Can sa/3 Tep can sa\nQuy doi Pot\t1 Can sa/1 Pot\nQuy doi Crack\t1 Can sa/1 Crack", "Chon", "Huy Bo");
        }
        else if(IsPlayerInRangeOfPoint(playerid, 5.0, -772.280334,-1962.709472,7.574229))
        {
            if(HasCopOnDuty() == false)
            {
                 SendClientMessage(playerid, -1, "Khong co canh sat nao dang lam nhiem vu, vui long quay lai sau.");
                 ShowTDN(playerid, NOTIFICATION_WARNING, "Khong co canh sat nao dang lam nhiem vu.");
                 return 1;
            }
            if(gettime() > GetPVarInt(playerid, "CanSaAreaCD")) {
                CanSa_AlertCops(playerid, "Khu vuc mua ban can sa");
                SetPVarInt(playerid, "CanSaAreaCD", gettime() + 30);
            }
            ShowPlayerDialog(playerid, DIALOG_UPGRADECANSA, DIALOG_STYLE_LIST, "fff00}KP5{ffffff} - Vui long chon...?","{ffffff}Ban can sa (IT TIEN)\n{ff0000}Giao can sa theo yeu cau [NHIEU TIEN / HOT]{ffffff}", "Chon", "Huy Bo");
        }
    }
    return 1;
}

CMD:cansa(playerid, params[])
{
	ShowPlayerDialog(playerid, CANSA_LIST, DIALOG_STYLE_LIST,"{ffff00}KP5{ffffff} - Can Sa", "Tim duong can sa\nXem can sa", "Chon", "Thoat");
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
                new stringb[1024];
                format(stringb, sizeof stringb, "{ffffff}Hien tai bang dang co %d tep can sa \n1 tep can sa/650$", PlayerInfo[playerid][pTepCanSa]);
                ShowPlayerDialog(playerid, DIALOG_BANCANSA, DIALOG_STYLE_INPUT,"{ffff00}KP5{ffffff} - Ban Can Sa", stringb, "Ban", "Thoat");
            }
            if (listitem == 1)
            {
                new stringb[128];
                format(stringb, sizeof stringb, "{ffffff}Hien tai bang dang co %d tep can sa \n1 tep can sa tu 1.500$ tro len, ban se duoc nhan them tuy vao chuyen giao xa hoac gan.", PlayerInfo[playerid][pTepCanSa]);
                ShowPlayerDialog(playerid, DIALOG_NHANDIEMGIAOHANG, DIALOG_STYLE_INPUT,"{ffff00}KP5{ffffff} - Ban Can Sa", stringb, "Xac nhan", "Thoat");
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
                    NhanDiemCount[playerid] = min(amount*5, 600);
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
				ShowPlayerDialog(playerid, TIMDUONGCANSA, DIALOG_STYLE_LIST,"{ffff00}KP5{ffffff} - Can Sa", "Vi tri can sa moc\nChe bien can sa\nBan can sa", "Chon", "Thoat");
				return 1;
			}
			if(listitem == 1)
			{
				new stringbx[128];
				format(stringbx, sizeof stringbx, "Cay can sa:\t %d\nTep can sa:\t %d\nPot:\t\t %d\nCrack:\t\t %d", PlayerInfo[playerid][pCanSa], PlayerInfo[playerid][pTepCanSa], PlayerInfo[playerid][pPot], PlayerInfo[playerid][pCrack]);
				ShowPlayerDialog(playerid, DIALOG_MSG, DIALOG_STYLE_MSGBOX, "{ffff00}KP5{ffffff} - Xem can sa", stringbx, "Dong y", "");
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
				SetPlayerCheckpoint(playerid, -1013.942443,-1635.635131,76.367187, 3.0);
				CP[playerid] = 252000;
				return 1;
			}
			if(listitem == 1)
			{
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCheckpoint(playerid, -778.561096,-1988.301757,26.171636, 3.0);
				CP[playerid] = 252000;
				return 1;
			}
			if(listitem == 2)
			{
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCheckpoint(playerid, -772.280334,-1962.709472,7.574229, 3.0);
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
					format(stringcs, sizeof(stringcs), "{ffff00}KP5{ffffff} Ban da ban %d tui can sa va nhan duoc $%d", amount, amount*MONEY_SELL_CANSA);
					SendClientMessage(playerid, -1, stringcs);
                    new title[64];
                    format(title, sizeof(title), "%s (ID: %d) - Bán cần sa", GetPlayerNameEx(playerid), playerid);
                    DiscordLog(dclog_CanSa, title, "**Số túi cần sa: %d**\n**Tiền nhận được: $%d**", amount, amount*MONEY_SELL_CANSA);
					CanSa_AlertCops(playerid, "Ban can sa cho mafia");
				}
				else
					SendClientMessage(playerid, -1,"{ffff00}KP5{ffffff} - So luong khong hop le! Hay kiem tra lai so luong cua ban.");
			}
			else
				SendClientMessage(playerid, -1,"{ffff00}KP5{ffffff} - So luong can ban phai lon hon 0!");
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
					format(stringss, sizeof(stringss), "{FFFF00}KP5{FFFFFF} Ban da doi %d cay can sa va nhan duoc %d pot.", amount, amount*1);
					SendClientMessage(playerid, -1, stringss);
					CanSa_AlertCops(playerid, "Trao doi can sa");
				}
				else
					SendClientMessage(playerid, -1,"{FFFF00}KP5{FFFFFF} So luong khong hop le! Hay kiem tra lai so luong cua ban.");
			}
			else
				SendClientMessage(playerid, -1,"{FFFF00}KP5{FFFFFF} So luong can doi phai lon hon 0!");
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
					format(strings, sizeof(strings), "{FFFF00}KP5{FFFFFF} Ban da doi %d cay can sa va nhan duoc %d crack.", amount, amount*1);
					SendClientMessage(playerid, -1, strings);
					CanSa_AlertCops(playerid, "Trao doi can sa");
				}
				else
					SendClientMessage(playerid, -1,"{FFFF00}KP5{FFFFFF} So luong khong hop le! Hay kiem tra lai so luong cua ban.");
			}
			else
				SendClientMessage(playerid, -1,"{FFFF00}KP5{FFFFFF} So luong can doi phai lon hon 0!");
		}
		return 1;
	}

	if(dialogid == BANCANLIST)
	{
		if(response)
		{
            if(HasCopOnDuty() == false)
            {
                SendClientMessage(playerid, -1, "Khong co canh sat nao dang lam nhiem vu, vui long quay lai sau.");
                ShowTDN(playerid, NOTIFICATION_WARNING, "Khong co canh sat nao dang lam nhiem vu.");
                return 1;
            }
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
				CanSa_AlertCops(playerid, "Khu che bien can sa");
			}
			if(listitem == 1)
			{
				new stringz[128];
				format(stringz, sizeof stringz, "Hien tai ban dang co %d cay can sa.\n1 cay can sa / 1 pot.\nNhap so luong cay can sa ban muon doi:", PlayerInfo[playerid][pCanSa]);
				ShowPlayerDialog(playerid, DIALOG_POT, DIALOG_STYLE_INPUT,"{FFFF00}KP5{FFFFFF} - Quy Doi Pot", stringz, "Doi", "Thoat");
			}
			if(listitem == 2)
			{
				new stringzz[128];
				format(stringzz, sizeof stringzz, "Hien tai ban dang co %d cay can sa.\n1 cay can sa / 1 crack.\nNhap so luong cay can sa ban muon doi:", PlayerInfo[playerid][pCanSa]);
				ShowPlayerDialog(playerid, DIALOG_CRACK, DIALOG_STYLE_INPUT,"{FFFF00}KP5{FFFFFF} - Quy Doi Crack", stringzz, "Doi", "Thoat");
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
        if (IsPlayerInRangeOfPoint(playerid, 30.0, -772.280334,-1962.709472,7.574229))
        {
            new str[256];
            NhanDiemCount[playerid]--;
            format(str, sizeof(str), "~r~Vui long doi ~w~%d ~r~giay nua de nhan duoc diem giao hang~n~Khong duoc di chuyen qua xa khu vuc nhan diem giao hang", NhanDiemCount[playerid]);
            SendMessageTD(playerid, 1000, str);
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
        SendMessageTD(playerid, 10000, "~w~Hay di den ~r~Checkpoint~w~ de giao hang!");
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
	SendMessageTD(playerid, 1100, string);
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
		SendClientMessage(playerid, 0x33FF33FF, "{FFFF00}KP5{FFFFFF} Ban da che bien thanh cong 1 cay can sa va nhan duoc 3 tep can sa.");
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
    SendClientMessage(playerid, 0x33FF33FF, "{FFFF00}KP5{FFFFFF} Ban da che bien thanh cong 1 cay can sa va nhan duoc 3 tep can sa.");
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Che bien thanh cong.");
    CanSa_AlertCops(playerid, "Khu che bien can sa");
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
                SendMessageTD(playerid, 5000, "~w~Welcome to ~n~~r~Fort DeMorgan");
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
    new title[64];
    format(title, sizeof(title), "%s (ID: %d) - Giao cần sa", GetPlayerNameEx(playerid), playerid);
    DiscordLog(dclog_CanSa, title, "**Số tép cần sa: %d**\n**Tiền nhận được: $%d**", inputcansa[playerid], moneytotal);
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
                CanSa_AlertCops(playerid, "Giao dich can sa.");
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
        ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong duoc phep su dung lenh nay.");
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
