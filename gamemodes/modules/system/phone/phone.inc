#include <open.mp>
#include <YSI\YSI_Coding\y_hooks>

new PlayerText: Text_Player[MAX_PLAYERS][82];
new takingselfie[MAX_PLAYERS];
new Float:Degree[MAX_PLAYERS];
const Float: RadiusPhone = 1.4; //do not edit this
const Float: SpeedPhone  = 1.25; //do not edit this
const Float: HeightPhone = 1.0; // do not edit this
new Float:lX[MAX_PLAYERS];
new Float:lY[MAX_PLAYERS];
new Float:lZ[MAX_PLAYERS];


#define			DIALOG_DIENTHOAI        (5093)
#define         DIALOG_CALL             (5094)
#define         DIALOG_DANHBA           (5095)

#define                 DANHBA_1                (9100)
#define                 DANHBA_2                (9101)
#define                 DANHBA_3                (9102)
#define                 DANHBA_4                (9103)
#define                 DANHBA_5                (9104)
#define                 DANHBA_6                (9105)
#define                 DANHBA_7                (9106)
#define                 DANHBA_8                (9107)
#define                 DANHBA_9                (9108)

#define             	GOIDIEN_1               (9200)
#define             	NHANTIN_1               (9201)
#define             	EDITDB_1                (9202)
#define					EDITDB_1_1              (9203)

#define             	GOIDIEN_2               (9204)
#define             	NHANTIN_2               (9205)
#define             	EDITDB_2                (9206)
#define					EDITDB_2_2              (9207)

#define             	GOIDIEN_3               (9208)
#define             	NHANTIN_3               (9209)
#define             	EDITDB_3                (9210)
#define					EDITDB_3_3              (9211)

#define             	GOIDIEN_4               (9212)
#define             	NHANTIN_4               (9213)
#define             	EDITDB_4                (9214)
#define					EDITDB_4_4              (9215)

#define             	GOIDIEN_5               (9216)
#define             	NHANTIN_5               (9217)
#define             	EDITDB_5                (9218)
#define					EDITDB_5_5              (9219)

#define             	GOIDIEN_6               (9220)
#define             	NHANTIN_6               (9221)
#define             	EDITDB_6                (9222)
#define					EDITDB_6_6              (9223)

#define             	GOIDIEN_7               (9224)
#define             	NHANTIN_7               (9225)
#define             	EDITDB_7                (9226)
#define					EDITDB_7_7              (9227)

#define             	GOIDIEN_8               (9228)
#define             	NHANTIN_8               (9229)
#define             	EDITDB_8                (9230)
#define					EDITDB_8_8              (9231)

#define             	GOIDIEN_9               (9232)
#define             	NHANTIN_9               (9233)
#define             	EDITDB_9                (9234)
#define					EDITDB_9_9              (9235)

#define                 DIENTHOAI_MP3           (9300)

#define                 DIALOG_SACOMBANK        (9400)
#define                 DIALOG_THEBANK        	(9401)
#define 				DIALOG_CHUYENTIEN 		(9402)
#define 				DIALOG_CHUYENTIEN1 		(9403)
#define 				DIALOG_CHUYENTIEN2 		(9404)

#define 		DIALOG_LIST_COPS     (9450) 
#define 		DIALOG_LIST_MEDICS   (9451)
#define         DIALOG_LIST_TAXI     (9452)
#define 		DIALOG_SMS_PHONE 	(9453)
#define 		DIALOG_SMS_TEXT 	(9454)

CMD:phone(playerid, params[]){
    return pc_cmd_dienthoai(playerid, params);
}

CMD:dienthoai(playerid, params[])
{
	if(PlayerInfo[playerid][pPnumber] == 0) return SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban khong so huu hien thoai.");
    
    if(GetPVarInt(playerid, "DIENTHOAI_") == 1)
    {
        for(new i = 0; i < 82; i++)
        {
            PlayerTextDrawHide(playerid, Text_Player[playerid][i]);
        }
        PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);
        SetPVarInt(playerid, "DIENTHOAI_", 0);
    	CancelSelectTextDraw(playerid, 0xff78bcf0);
    	pc_cmd_me(playerid, "da tat dien thoai.");
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
        ClearAnimations(playerid);
		RemovePlayerAttachedObject(playerid, 9);
        SetPlayerChatBubble(playerid, "", COLOR_PURPLE, 1.0, 1000);
        if(takingselfie[playerid] == 1)
        {
            TogglePlayerControllable(playerid,1);
            SetCameraBehindPlayer(playerid);
            takingselfie[playerid] = 0;
            ApplyAnimation(playerid, "PED", "ATM", 4.1, 0, 1, 1, 0, 1, 1);
            return 1;
        }
        return 1;
    }

    if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER || GetPlayerState(playerid) == PLAYER_STATE_PASSENGER)
    {
        for(new i = 0; i < 82; i++)
        {
            PlayerTextDrawShow(playerid, Text_Player[playerid][i]);
        }
        PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);
        UpdatePhoneTime(playerid);
        SetPVarInt(playerid, "DIENTHOAI_", 1);
        SelectTextDraw(playerid, 0xff78bcf0);
        pc_cmd_me(playerid, "da mo dien thoai cua minh len.");
        SetPlayerChatBubble(playerid, "> Dang su dung dien thoai <", COLOR_PURPLE, 15.0, 5000);
    }
    else
    {
        for(new i = 0; i < 82; i++)
        {
            PlayerTextDrawShow(playerid, Text_Player[playerid][i]);
        }
        PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);
        UpdatePhoneTime(playerid);
        SetPVarInt(playerid, "DIENTHOAI_", 1);
        SelectTextDraw(playerid, 0xff78bcf0);
        pc_cmd_me(playerid, "da mo dien thoai cua minh len.");
        ApplyAnimation(playerid, "ped", "Jetpack_Idle", 4.1, 1, 0, 0, 1, 0, 1);
        SetPlayerAttachedObject(playerid, 9, 18866, 5, 0.135999, 0.017999, -0.053999, -91.299995, -88.799972, -2.199998, 1.000000, 1.000000, 1.000000);
        SetPlayerChatBubble(playerid, "> Dang su dung dien thoai <", COLOR_PURPLE, 15.0, 5000);
    }
	return 1;
}

CMD:selfie(playerid,params[])
{
	if(PlayerInfo[playerid][pPnumber] == 0) return SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban khong so huu hien thoai.");

	if(takingselfie[playerid] == 0)
	{
	    GetPlayerPos(playerid,lX[playerid],lY[playerid],lZ[playerid]);
		static Float: n1X, Float: n1Y;
		if(Degree[playerid] >= 360) Degree[playerid] = 0;
		Degree[playerid] += SpeedPhone;
		n1X = lX[playerid] + RadiusPhone * floatcos(Degree[playerid], degrees);
		n1Y = lY[playerid] + RadiusPhone * floatsin(Degree[playerid], degrees);
		SetPlayerCameraPos(playerid, n1X, n1Y, lZ[playerid] + HeightPhone);
		SetPlayerCameraLookAt(playerid, lX[playerid], lY[playerid], lZ[playerid]+1);
		SetPlayerFacingAngle(playerid, Degree[playerid] - 90.0);
		takingselfie[playerid] = 1;
		ApplyAnimation(playerid, "PED", "gang_gunstand", 4.1, 1, 1, 1, 1, 1, 1);
		return 1;
	}
    if(takingselfie[playerid] == 1)
	{
	    TogglePlayerControllable(playerid,1);
		SetCameraBehindPlayer(playerid);
	    takingselfie[playerid] = 0;
	    ApplyAnimation(playerid, "PED", "ATM", 4.1, 0, 1, 1, 0, 1, 1);
	    return 1;
	}
    return 1;
}

stock ShowChuyenTienHihi(playerid) {
	return ShowPlayerDialog(playerid, DIALOG_CHUYENTIEN, DIALOG_STYLE_INPUT, "Chuyen tien", "{ffffff}Vui long nhap so tai khoan de tiep tuc giao dich", "Tuy chon", "Thoat");
}

CMD:xemtiendt(playerid, params[])
{
    new str[2460], str1[1280], str2[1280], str3[1280], str4[1280], str5[1280], str6[1280], str7[1280], str8[1280], str9[1280], name[1280];
    format(str1, sizeof(str), "{f2f2f2}Nha mang {00ffff}Viettel");
    format(str2, sizeof(str), "{f2f2f2}THONG TIN CUA BAN :");
    format(str3, sizeof(str), "{f2f2f2}   -> Ten nguoi dung : {ff9900}%s", GetPlayerNameEx(playerid));
    format(str4, sizeof(str), "{f2f2f2}   -> Tai khoan chinh : {00ff00}%s$", number_format(PlayerInfo[playerid][pCash]));
    format(str5, sizeof(str), "{f2f2f2}   -> So dien thoai : {00ff00}%i", PlayerInfo[playerid][pPnumber]);
    format(str, sizeof(str), "%s\n\n%s\n%s\n%s\n%s", str1,str2,str3,str4,str5);
    format(name, sizeof(name), "{ffffff}THONG TIN DIEN THOAI");
	ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, name, str, "Dong y", "Huy Bo");
	return 1;
}

CMD:sacombank(playerid, params[])
{
	new thongtinthe[1280];
	if(PlayerInfo[playerid][pSoTaiKhoan] == 0)
    	format(thongtinthe, sizeof(thongtinthe), "Chua kich hoat");
	else
	    format(thongtinthe, sizeof(thongtinthe), "%d", PlayerInfo[playerid][pSoTaiKhoan]);

    new str[2460], str1[1280], str2[1280], str3[1280], str4[1280], str5[1280], str6[1280], str7[1280], str8[1280], str9[1280], str10[1280], name[1280];
    format(str1, sizeof(str), "{f2f2f2}Chao mung ban da den voi he thong {0066ff}Vietcombank");
    format(str2, sizeof(str), "{f2f2f2}THONG TIN CUA BAN :");
    format(str3, sizeof(str), "{f2f2f2}   -> Ten nguoi dung : {ff9900}%s", GetPlayerNameEx(playerid));
    format(str4, sizeof(str), "{f2f2f2}   -> So tai khoan : {00ff00}%s", thongtinthe);
    format(str5, sizeof(str), "{f2f2f2}   -> So du tai khoan : {00ff00}%d", PlayerInfo[playerid][pAccount]);
    format(str6, sizeof(str), "------------------------------------------------");
    format(str7, sizeof(str), "{f2f2f2}Hien tai ngan hang cho phep ban duoc su dung cac chuc nang sau:");
    format(str8, sizeof(str), "{1ac6ff}   1. Quan ly the va tai khoan.");
    format(str9, sizeof(str), "{1ac6ff}   2. Chuyen tien.");
    format(str10, sizeof(str), "{1ac6ff}   3. Mo tai khoan thanh toan (500$).");
    format(str, sizeof(str), "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s", str1,str2,str3,str4,str5,str6,str7,str8,str9,str10);
    format(name, sizeof(name), "{0066ff}Vietcombank Pay");
	ShowPlayerDialog(playerid, DIALOG_SACOMBANK, DIALOG_STYLE_MSGBOX, name, str, "Dong y", "Huy Bo");
	return 1;
}

forward UpdateTK(playerid);
public UpdateTK(playerid) {
	new namez[24],bank;
	new rows = cache_num_rows();
	if(rows)
    {
        for(new i=0; i<rows; i++)
        {
    	    cache_get_field_content(i, "UserName", namez, MainPipeline, 24);
    	    bank = cache_get_field_content_int(i, "Bank", MainPipeline);
            bank += GetPVarInt(playerid, "TienGD");
		    new string[129];
		    format(string, sizeof(string), "UPDATE accounts SET `Bank`=%d WHERE `SoTaiKhoan` = %d", bank, GetPVarInt(playerid, "STK"));
		    mysql_function_query(MainPipeline, string, false, "OnQueryFinish", "ii", SENDDATA_THREAD, playerid);
			return 1;
        }
    }
    else
    {
    	ShowPlayerDialog(playerid, DIALOG_CHUYENTIEN, DIALOG_STYLE_INPUT, "Chuyen tien", "So tai khoan khong ton tai !\nVui long nhap so tai khoan de tiep tuc giao dich", "Tuy chon", "Thoat");
    	return 1;
    }
    return 1;
}

forward STK_check(playerid);
public STK_check(playerid) {
	new namez[24],stk;
	new rows = cache_num_rows();
    if(rows)
    {
        for(new i=0; i<rows; i++)
        {
    	    cache_get_field_content(i, "UserName",  namez, MainPipeline, 24);
    	    stk = cache_get_field_content_int(i, "SoTaiKhoan", MainPipeline);
            SetPVarString(playerid, "_NguoiNhan", namez);
		    SetPVarInt(playerid, "Offline", 1);
		    SetPVarInt(playerid, "STK", stk);
		    new string[129];
		    format(string, sizeof string, "Ten chu the\t\t\t\t%s\nSo tai khoan\t\t\t\t%d\nBan hay nhap so tien muon giao dich o ben duoi", namez,stk);
		    ShowPlayerDialog(playerid, DIALOG_CHUYENTIEN1, DIALOG_STYLE_INPUT, "Chuyen tien", string, "Tuy chon", "Thoat");
		    return 1;
        }
    }
    else {
 		ShowPlayerDialog(playerid, DIALOG_CHUYENTIEN, DIALOG_STYLE_INPUT, "Chuyen tien", "So tai khoan khong ton tai !\nVui long nhap so tai khoan de tiep tuc giao dich", "Tuy chon", "Thoat");
 		return 1;
    }
    return 1;
}

CMD:danhbacuatoi(playerid, params[])
{
    if(PlayerInfo[playerid][pPnumber] == 0 ) 
        return SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban khong so huu dien thoai.");
    if(PlayerInfo[playerid][pPhoneBook] == 0)
        return SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban chua mua sim cho dien thoai.");
        
    new str[2460];
    str[0] = EOS; // Đảm bảo chuỗi rỗng ban đầu
    
    format(str, sizeof(str), "1. TEN: %s || SDT: %d\n", PlayerInfo[playerid][pDanhBa1], PlayerInfo[playerid][pSDT1]);
    format(str, sizeof(str), "%s2. TEN: %s || SDT: %d\n", str, PlayerInfo[playerid][pDanhBa2], PlayerInfo[playerid][pSDT2]);
    format(str, sizeof(str), "%s3. TEN: %s || SDT: %d\n", str, PlayerInfo[playerid][pDanhBa3], PlayerInfo[playerid][pSDT3]);
    format(str, sizeof(str), "%s4. TEN: %s || SDT: %d\n", str, PlayerInfo[playerid][pDanhBa4], PlayerInfo[playerid][pSDT4]);
    format(str, sizeof(str), "%s5. TEN: %s || SDT: %d\n", str, PlayerInfo[playerid][pDanhBa5], PlayerInfo[playerid][pSDT5]);
    format(str, sizeof(str), "%s6. TEN: %s || SDT: %d\n", str, PlayerInfo[playerid][pDanhBa6], PlayerInfo[playerid][pSDT6]);
    format(str, sizeof(str), "%s7. TEN: %s || SDT: %d\n", str, PlayerInfo[playerid][pDanhBa7], PlayerInfo[playerid][pSDT7]);
    format(str, sizeof(str), "%s8. TEN: %s || SDT: %d\n", str, PlayerInfo[playerid][pDanhBa8], PlayerInfo[playerid][pSDT8]);
    format(str, sizeof(str), "%s9. TEN: %s || SDT: %d", str, PlayerInfo[playerid][pDanhBa9], PlayerInfo[playerid][pSDT9]);
    
    ShowPlayerDialog(playerid, DIALOG_DANHBA, DIALOG_STYLE_LIST, "DANH BA CUA TOI", str, "Dong y", "Huy bo");
    return 1;
}

stock UpdatePhoneTime(playerid)
{	
	if(GetPVarInt(playerid, "DIENTHOAI_") == 1)
	{
		new hour, minute;
		gettime(hour, minute);
		
		new timeString[32];
		format(timeString, sizeof(timeString), "%02d:%02d", hour, minute);
		PlayerTextDrawSetString(playerid, Text_Player[playerid][81], timeString);
	}
	return 1;
}

stock ShowOnlineCops(playerid)
{
    new string[2048], count = 0;
    string[0] = 0;
    strcat(string, "Ten\tSo dien thoai\n");
    
    // Xóa tất cả PVar cũ
    new oldTotal = GetPVarInt(playerid, "TotalPhones");
    for(new i = 0; i < oldTotal; i++)
    {
        new oldVarname[32];
        format(oldVarname, sizeof(oldVarname), "SelectedPhone_%d", i);
        DeletePVar(playerid, oldVarname);
    }
    DeletePVar(playerid, "TotalPhones");

    foreach(new i: Player)
    {
        if(IsACop(i) && PlayerInfo[i][pDuty] == 1)
        {
            if(PlayerInfo[i][pPnumber] == 0) continue;
            
            // Lưu số điện thoại vào PVar với index
            new varname[32];
            format(varname, sizeof(varname), "SelectedPhone_%d", count);
            SetPVarInt(playerid, varname, PlayerInfo[i][pPnumber]);

            new tempstr[128];
            format(tempstr, sizeof(tempstr), "%s\t%d\n", GetPlayerNameEx(i), PlayerInfo[i][pPnumber]);
            strcat(string, tempstr);
            
            count++;
        }
    }
    
    // Lưu tổng số điện thoại
    SetPVarInt(playerid, "TotalPhones", count);
    
    if(count == 0)
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Khong co canh sat nao dang lam nhiem vu hoac ho chua so huu dien thoai.");
        return 0;
    }
    
    ShowPlayerDialog(playerid, DIALOG_LIST_COPS, DIALOG_STYLE_TABLIST_HEADERS, "Danh sach Canh sat dang lam nhiem vu", string, "Goi", "Huy");
    return 1;
}

stock ShowOnlineMedics(playerid)
{
    new string[2048], count = 0;
    string[0] = 0;
    strcat(string, "Ten\tSo dien thoai\n");
    
    // Xóa tất cả PVar cũ
    new oldTotal = GetPVarInt(playerid, "TotalPhones");
    for(new i = 0; i < oldTotal; i++)
    {
        new oldVarname[32];
        format(oldVarname, sizeof(oldVarname), "SelectedPhone_%d", i);
        DeletePVar(playerid, oldVarname);
    }
    DeletePVar(playerid, "TotalPhones");
    
    foreach(new i: Player)
    {
        if(IsAMedic(i) && PlayerInfo[i][pDuty] == 1)
        {
            if(PlayerInfo[i][pPnumber] == 0) continue;

            new varname[32];
            format(varname, sizeof(varname), "SelectedPhone_%d", count);
            SetPVarInt(playerid, varname, PlayerInfo[i][pPnumber]);
            
            new tempstr[128];
            format(tempstr, sizeof(tempstr), "%s\t%d\n", GetPlayerNameEx(i), PlayerInfo[i][pPnumber]);
            strcat(string, tempstr);
            
            count++;
        }
    }
    
    SetPVarInt(playerid, "TotalPhones", count);
    
    if(count == 0)
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Khong co bac si nao dang lam nhiem vu hoac ho chua so huu dien thoai.");
        return 0;
    }
    
    ShowPlayerDialog(playerid, DIALOG_LIST_MEDICS, DIALOG_STYLE_TABLIST_HEADERS, "Danh sach Bac si dang lam nhiem vu", string, "Goi", "Huy");
    return 1;
}

stock ShowOnlineTaxiDrivers(playerid)
{
    new string[2048], count = 0;
    string[0] = 0;
    strcat(string, "Ten\tSo dien thoai\n");
    
    // Xóa tất cả PVar cũ
    new oldTotal = GetPVarInt(playerid, "TotalPhones");
    for(new i = 0; i < oldTotal; i++)
    {
        new oldVarname[32];
        format(oldVarname, sizeof(oldVarname), "SelectedPhone_%d", i);
        DeletePVar(playerid, oldVarname);
    }
    DeletePVar(playerid, "TotalPhones");
    
    foreach(new i: Player)
    {
        if(IsATaxiDriver(i) && TransportDuty[i] > 0) // Kiểm tra tài xế taxi đang làm nhiệm vụ
        {
            if(PlayerInfo[i][pPnumber] == 0) continue; // Bỏ qua nếu không có số điện thoại

            new varname[32];
            format(varname, sizeof(varname), "SelectedPhone_%d", count);
            SetPVarInt(playerid, varname, PlayerInfo[i][pPnumber]);
            
            new tempstr[128];
            format(tempstr, sizeof(tempstr), "%s\t%d\n", GetPlayerNameEx(i), PlayerInfo[i][pPnumber]);
            strcat(string, tempstr);
            
            count++;
        }
    }
    
    SetPVarInt(playerid, "TotalPhones", count);
    
    if(count == 0)
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Khong co tai xe taxi nao dang lam nhiem vu hoac ho chua so huu dien thoai.");
        return 0;
    }
    
    ShowPlayerDialog(playerid, DIALOG_LIST_TAXI, DIALOG_STYLE_TABLIST_HEADERS, "Danh sach Tai xe taxi dang lam nhiem vu", string, "Goi", "Huy");
    return 1;
}

CMD:goisdt(playerid, params[])
{
	ShowPlayerDialog(playerid, DIALOG_DIENTHOAI, DIALOG_STYLE_LIST, "DIEN THOAI", "Goi bang so dien thoai\nGoi canh sat\nGoi cap cuu\nGoi tho sua xe\nGoi taxi", "Lua chon", "Huy bo");
    return 1;
}

stock ProcessSMS(playerid, phonenumb, text[])
{
    new string[128];
    
    if(Spectating[playerid] == 0)
    {
        format(string, sizeof(string), "* %s lay dien thoai di dong tu trong tui quan.", GetPlayerNameEx(playerid));
        ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
    }

    if(phonenumb == 555)
    {
        if ((strcmp("yes", text, true, strlen(text)) == 0) && (strlen(text) == strlen("yes")))
        {
            SendClientMessageEx(playerid, COLOR_WHITE, "Tin nhan van ban.");
            SendClientMessageEx(playerid, COLOR_YELLOW, "SMS: Toi dang theo doi ban, Sender: MOLE (555)");
            RingTone[playerid] = 20;
            return 1;
        }
        else
        {
            SendClientMessageEx(playerid, COLOR_YELLOW, "SMS: Toi dang theo doi ban, Sender: MOLE (555)");
            RingTone[playerid] = 20;
            return 1;
        }
    }
    
    foreach(new i: Player)
    {
        if(PlayerInfo[i][pPnumber] == phonenumb && phonenumb != 0)
        {
            new giveplayerid = i;
            if(Mobile[giveplayerid] != INVALID_PLAYER_ID)
            {
                SendClientMessageEx(playerid, COLOR_GREY, "Dien thoai cua nguoi do dang ban.");
                return 1;
            }
            Mobile[playerid] = giveplayerid;
            if(IsPlayerConnected(giveplayerid))
            {
                if(giveplayerid != INVALID_PLAYER_ID)
                {
                    if(PhoneOnline[giveplayerid] > 0)
                    {
                        SendClientMessageEx(playerid, COLOR_GREY, "Dien thoai cua nguoi do dang tat.");
                        Mobile[playerid] = INVALID_PLAYER_ID;
                        return 1;
                    }
                    foreach(new u: Player)
                    {
                        if(GetPVarInt(u, "BigEar") == 6 && (GetPVarInt(u, "BigEarPlayer") == playerid || GetPVarInt(u, "BigEarPlayer") == giveplayerid))
                        {
                            format(string, sizeof(string), "(BE) %s SMS cho %s: %s", GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid), text);
                            SendClientMessageEx(u,COLOR_YELLOW, string);
                        }
                    }
                    if(PlayerInfo[playerid][pPhonePrivacy] == 1)
                    {
                        format(string, sizeof(string), "SMS: %s, Nguoi gui: Khong biet.", text);
                    }
                    else
                    {
                        format(string, sizeof(string), "SMS: %s, Nguoi gui: %s (%d)", text, GetPlayerNameEx(playerid), PlayerInfo[playerid][pPnumber]);
                    }

                    if(i != playerid)
                    {
                        if(PlayerInfo[i][pSmslog] > 0)
                        {
                            new query[384], ftext[128];
                            mysql_escape_string(text, ftext);
                            if(PlayerInfo[playerid][pPhonePrivacy] == 1) 
                                format(query, sizeof(query), "INSERT INTO `sms` (`id`, `sender`, `senderid`, `sendernumber`, `receiver`, `receiverid`, `receivernumber`, `message`, `date`) VALUES (NULL, '%s', %d, 0, '%s', %d, %d, '%s', NOW())", 
                                    GetPlayerNameExt(playerid), GetPlayerSQLId(playerid), GetPlayerNameExt(i), GetPlayerSQLId(i), phonenumb, ftext);
                            else 
                                format(query, sizeof(query), "INSERT INTO `sms` (`id`, `sender`, `senderid`, `sendernumber`, `receiver`, `receiverid`, `receivernumber`, `message`, `date`) VALUES (NULL, '%s', %d, %d, '%s', %d, %d, '%s', NOW())", 
                                    GetPlayerNameExt(playerid), GetPlayerSQLId(playerid), PlayerInfo[playerid][pPnumber], GetPlayerNameExt(i), GetPlayerSQLId(i), phonenumb, ftext);
                            mysql_function_query(MainPipeline, query, false, "OnQueryFinish", "i", SENDDATA_THREAD);
                        }
                    }

                    RingTone[giveplayerid] = 20;
                    SendClientMessageEx(giveplayerid, COLOR_YELLOW, string);
                    SendClientMessageEx(playerid, COLOR_YELLOW, string);
                    SendClientMessageEx(playerid, COLOR_WHITE, "Gui tin nhan van ban");
                    format(string, sizeof(string), "~r~$-%d", 25);
                    GameTextForPlayer(playerid, string, 5000, 1);
                    GivePlayerCash(playerid,-25);
                    Mobile[playerid] = INVALID_PLAYER_ID;

                    if(strcmp(PlayerInfo[giveplayerid][pAutoTextReply], "Nothing", true) != 0)
                    {
                        format(string, sizeof(string), "SMS: %s, Nguoi gui: %s [automated response] (%d)", 
                            PlayerInfo[giveplayerid][pAutoTextReply], GetPlayerNameEx(giveplayerid), PlayerInfo[giveplayerid][pPnumber]);
                        SendClientMessageEx(playerid, COLOR_YELLOW, string);
                    }
                    return 1;
                }
            }
        }
    }
    SendClientMessageEx(playerid, COLOR_GRAD2, "Gui tin nhan that bai...");
    return 1;
}

