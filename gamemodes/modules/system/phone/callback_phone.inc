#include <YSI\YSI_Coding\y_hooks>

forward OnPhoneLoadingFinish(playerid);
public OnPhoneLoadingFinish(playerid)
{
    DeletePVar(playerid, "PHONE_LOADING");

    for(new i = 0; i < PHONE_TEXTDRAW_COUNT; i++)
        TextDrawShowForPlayer(playerid, PhoneTD[i]);

    PhoneOpen[playerid] = true;
    SetPVarInt(playerid, "DIENTHOAI_", 1);
    UpdatePhoneTime(playerid);
    SelectTextDraw(playerid, 0xff78bcf0);
    pc_cmd_me(playerid, "da mo dien thoai cua minh len.");
    return 1;
}

hook OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    if(clickedid == Text:INVALID_TEXT_DRAW)
    {
        if(GetPVarInt(playerid, "DelayPhone") == 1)
        {
            DeletePVar(playerid, "DelayPhone");
            return 1;
        }
        if(GetPVarInt(playerid, "DIENTHOAI_") == 1)
        {
            Phone_Close(playerid);
            PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);
            CancelSelectTextDraw(playerid, 0xff78bcf0);
            pc_cmd_me(playerid, "da tat dien thoai.");
            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
            if(!IsPlayerInAnyVehicle(playerid)) ClearAnimations(playerid);
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
        }
        return 0;
    }

    if(!PhoneOpen[playerid]) return 0;

    if(clickedid == PhoneTD[18])
    {
        PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);
        pc_cmd_sacombank(playerid, "");
        return 1;
    }
    if(clickedid == PhoneTD[27])
    {
        PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);
        PC_EmulateCommand(playerid, "/timduong");
        return 1;
    }
    if(clickedid == PhoneTD[33])
    {
        PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);
        pc_cmd_time(playerid, "");
        return 1;
    }
    if(clickedid == PhoneTD[49])
    {
        PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);
        pc_cmd_dichvu(playerid, "taxi");
        return 1;
    }
    if(clickedid == PhoneTD[61])
    {
        PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);
        pc_cmd_danhbacuatoi(playerid, "");
        return 1;
    }
    if(clickedid == PhoneTD[59])
    {
        PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);
        pc_cmd_sms(playerid, "");
        return 1;
    }
    if(clickedid == PhoneTD[62])
    {
        PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);
        ShowPlayerDialog(playerid, DIALOG_DIENTHOAI, DIALOG_STYLE_LIST, "DIEN THOAI",
            "Goi bang so dien thoai\nGoi canh sat\nGoi cap cuu\nGoi tho sua xe\nGoi taxi",
            "Lua chon", "Huy bo");
        return 1;
    }
    if(clickedid == PhoneTD[30])
    {
        PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);
        PC_EmulateCommand(playerid, "/setting");
        return 1;
    }
    if(clickedid == PhoneTD[31])
    {
        PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);
        ShowPlayerDialog(playerid, DIALOG_DIENTHOAI, DIALOG_STYLE_LIST, "DIEN THOAI",
            "Goi bang so dien thoai\nGoi canh sat\nGoi cap cuu\nGoi tho sua xe\nGoi taxi",
            "Lua chon", "Huy bo");
        return 1;
    }
    if(clickedid == PhoneTD[54])
    {
        pc_cmd_selfie(playerid, "");
        return 1;
    }
    if(clickedid == PhoneTD[32])
    {
        ShowTDN(playerid, NOTIFICATION_WARNING, "Chuc nang dang duoc phat trien...");
        return 1;
    }
    if(clickedid == PhoneTD[20])
    {
        if(GetPVarInt(playerid, "DIENTHOAI_") == 1)
        {
            Phone_Close(playerid);
            PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);
            CancelSelectTextDraw(playerid, 0xff78bcf0);
            pc_cmd_me(playerid, "da tat dien thoai.");
            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
            if(!IsPlayerInAnyVehicle(playerid)) ClearAnimations(playerid);
            RemovePlayerAttachedObject(playerid, 9);
            SetPlayerChatBubble(playerid, "", COLOR_PURPLE, 1.0, 1000);
        }
        if(takingselfie[playerid] == 1)
        {
            TogglePlayerControllable(playerid,1);
            SetCameraBehindPlayer(playerid);
            takingselfie[playerid] = 0;
            ApplyAnimation(playerid, "PED", "ATM", 4.1, 0, 1, 1, 0, 1, 1);
        }
        return 1;
    }
    return 0;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	new string[1280];


	if(dialogid == DIALOG_SACOMBANK)
    {
        if(response){
            ShowPlayerDialog(playerid, DIALOG_THEBANK, DIALOG_STYLE_LIST, "{FFD700}NGAN HANG - DICH VU",
                "{00CFFF}[i] {ffffff}Quan ly the va tai khoan\n{00FF7F}[>] {ffffff}Chuyen tien\n{FF9900}[+] {ffffff}Mo tai khoan thanh toan {ffffff}",
                "Chon", "Dong");
		}
	}

	if(dialogid == DIALOG_THEBANK)
    {
        if(response)
        {
            if(listitem == 0)
                pc_cmd_sacombank(playerid, "");
            if(listitem == 1){
                if(PlayerInfo[playerid][pSoTaiKhoan] != 0){
                	ShowChuyenTienHihi(playerid);
                }else SendClientMessage(playerid, COLOR_YELLOW, "{FF6B6B}[!]{FFFFFF} Ban chua mo tai khoan thanh toan de su dung chuc nang nay.");
			}
			if(listitem == 2){
			    if(PlayerInfo[playerid][pSoTaiKhoan] == 0)
			    {
	                if(PlayerInfo[playerid][pCash] >= 5000){
	                    PlayerInfo[playerid][pCash] -= 5000;

	                    new sotaikhoan;
		    			sotaikhoan = 100000 + random(999999);
		    			PlayerInfo[playerid][pSoTaiKhoan] = sotaikhoan;
						//UpdateMoTaiKhoanMission(playerid);

						format(string, sizeof(string), "UPDATE accounts SET `SoTaiKhoan`=%d WHERE `UserName` = '%e'", PlayerInfo[playerid][pSoTaiKhoan], GetPlayerNameEx(playerid));
						mysql_function_query(MainPipeline, string, false, "OnQueryFinish", "ii", SENDDATA_THREAD, playerid);
		    			format(string, sizeof string, "{00FF7F}[+]{FFFFFF} Dang ky thanh cong!\n{AAAAAA}So tai khoan: {00CFFF}%d\n{FFD700}[BANK]{FFFFFF} Phi mo tai khoan: {FF6B6B}$5,000", sotaikhoan);
		    			ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "{FFD700}DANG KY THANH CONG", string, "Dong y", "");
	                }else SendClientMessage(playerid, COLOR_YELLOW, "{FF6B6B}[!]{FFFFFF} Ban can co it nhat {FFFF00}$5,000{FFFFFF} de mo tai khoan ngan hang.");
                }else SendClientMessage(playerid, COLOR_YELLOW, "{FF6B6B}[!]{FFFFFF} Ban da co tai khoan ngan hang roi.");
			}
		}
	}
	
	if(dialogid == DIALOG_CHUYENTIEN)
    {
        if(response) {
    		if(!IsNumeric(inputtext) || strval(inputtext) < 123) {
    		    new errstr[512];
    		    format(errstr, sizeof(errstr), "{FFD700}=== CHUYEN KHOAN ==={FFFFFF}\n{AAAAAA}So du hien tai: {00FF7F}$%s\n\n{FF6B6B}[!] So tai khoan khong hop le!\n{FFFFFF}Vui long nhap lai {00CFFF}so tai khoan{FFFFFF} nguoi nhan:", number_format(PlayerInfo[playerid][pAccount]));
    		    return ShowPlayerDialog(playerid, DIALOG_CHUYENTIEN, DIALOG_STYLE_INPUT, "{FFD700}CHUYEN TIEN", errstr, "Tiep tuc", "Thoat");
    		}
			new check;
			if(strval(inputtext) == PlayerInfo[playerid][pSoTaiKhoan]) {
			    new errstr[512];
			    format(errstr, sizeof(errstr), "{FFD700}=== CHUYEN KHOAN ==={FFFFFF}\n{AAAAAA}So du hien tai: {00FF7F}$%s\n\n{FF6B6B}[!] Ban khong the chuyen tien cho chinh minh!\n{FFFFFF}Nhap {00CFFF}so tai khoan{FFFFFF} nguoi khac:", number_format(PlayerInfo[playerid][pAccount]));
			    return ShowPlayerDialog(playerid, DIALOG_CHUYENTIEN, DIALOG_STYLE_INPUT, "{FFD700}CHUYEN TIEN", errstr, "Tiep tuc", "Thoat");
			}
			foreach(new i: Player)
			{
				if(PlayerInfo[i][pSoTaiKhoan] == strval(inputtext)) {
					SetPVarInt(playerid, "NguoiNhan", i);
					format(string, sizeof string, "{FFD700}=== THONG TIN NGUOI NHAN ==={FFFFFF}\n{AAAAAA}Ten chu the  : {00CFFF}%s\n{AAAAAA}So tai khoan : {00CFFF}%d\n\n{FFD700}=== GIAO DICH ==={FFFFFF}\n{FFFFFF}Nhap {00FF7F}so tien{FFFFFF} muon giao dich:", GetPlayerNameEx(i), strval(inputtext));
					ShowPlayerDialog(playerid, DIALOG_CHUYENTIEN1, DIALOG_STYLE_INPUT, "{FFD700}CHUYEN TIEN", string, "Tiep tuc", "Thoat");
			   	    check = 1;
				}
			}
			if(check == 0) {
				format(string, sizeof(string), "SELECT `UserName`,`SoTaiKhoan` from accounts WHERE SoTaiKhoan = '%d'", strval(inputtext));
				mysql_tquery(MainPipeline, string, "STK_check", "d", playerid);
			}
		}
		else {
			DeletePVar(playerid,"Offline");
		}
	}

	if(dialogid == DIALOG_CHUYENTIEN2)
    {
        if(response) {
    		new amount = GetPVarInt(playerid, "TienGD");
    		if(GetPVarInt(playerid, "Offline") == 1) {
				format(string, sizeof(string), "SELECT `UserName`,`SoTaiKhoan`,`Bank` from accounts WHERE SoTaiKhoan = '%d'", GetPVarInt(playerid, "STK"));
				mysql_tquery(MainPipeline, string, "UpdateTK", "d", playerid);
				new nguoinhan[24];
				GetPVarString(playerid,"_NguoiNhan",nguoinhan,24);
				new stk = GetPVarInt(playerid, "STK");
				DeletePVar(playerid,"Offline");
				new beforeAmount = PlayerInfo[playerid][pAccount];
				PlayerInfo[playerid][pAccount] -= amount;
				format(string, sizeof string, "{00FF7F}[>]{FFFFFF} Chuyen {00FF7F}$%s{FFFFFF} toi {00CFFF}%s (STK: %d){FFFFFF} | So du con: {FF6B6B}$%s", number_format(amount), nguoinhan, stk, number_format(PlayerInfo[playerid][pAccount]));
    		    SendClientMessage(playerid, COLOR_YELLOW2, string);

    		    new ip[32];
    		    GetPlayerIp(playerid, ip, sizeof(ip));
    		    format(string, sizeof string, "[CHUYEN KHOAN OFFLINE] %s (IP:%s) -> %s (STK:%d) | $%s | Truoc: $%s | Sau: $%s", GetPlayerNameEx(playerid), ip, nguoinhan, stk, number_format(amount), number_format(beforeAmount), number_format(PlayerInfo[playerid][pAccount]));
    		    Log("logs/bank.log", string);

    		    new DCC_Channel:channel = DCC_FindChannelById("1548216250415456376");
    		    new nofi[520], title[128];
    		    format(title, sizeof(title), "**%s (ID: %d) chuyen khoan qua dien thoai**", GetPlayerNameEx(playerid), playerid);
    		    format(nofi, sizeof(nofi), ":bank: **SO TIEN: $%s** -> %s (STK: %d) | [OFFLINE] | Truoc: $%s | Sau: $%s", number_format(amount), nguoinhan, stk, number_format(beforeAmount), number_format(PlayerInfo[playerid][pAccount]));
    		    DiscordLog(channel, title, nofi);
				return 1;
    		}
    		new i = GetPVarInt(playerid, "NguoiNhan");
    		PlayerInfo[playerid][pAccount] -= amount;
    		PlayerInfo[i][pAccount] += amount;
    		format(string, sizeof string, "{00CFFF}[>]{FFFFFF} Nhan {00FF7F}$%s{FFFFFF} tu {00CFFF}%s (STK: %d)", number_format(amount), GetPlayerNameEx(playerid), PlayerInfo[playerid][pSoTaiKhoan]);
    		SendClientMessage(i, COLOR_YELLOW2, string);
    		format(string, sizeof string, "{00FF7F}[>]{FFFFFF} Chuyen {00FF7F}$%s{FFFFFF} toi {00CFFF}%s (STK: %d){FFFFFF} | So du con: {FF6B6B}$%s", number_format(amount), GetPlayerNameEx(i), PlayerInfo[i][pSoTaiKhoan], number_format(PlayerInfo[playerid][pAccount]));
    		SendClientMessage(playerid, COLOR_YELLOW2, string);

    		new ip[32], ipex[32];
    		GetPlayerIp(playerid, ip, sizeof(ip));
    		GetPlayerIp(i, ipex, sizeof(ipex));
    		format(string, sizeof string, "[CHUYEN KHOAN ONLINE] %s (IP:%s) -> %s (IP:%s) | $%s", GetPlayerNameEx(playerid), ip, GetPlayerNameEx(i), ipex, number_format(amount));
    		Log("logs/bank.log", string);

    		new DCC_Channel:channel = DCC_FindChannelById("1548216250415456376");
    		new nofi[520], title[128];
    		format(title, sizeof(title), "**%s (ID: %d) chuyen khoan cho %s (ID: %d)**", GetPlayerNameEx(playerid), playerid, GetPlayerNameEx(i), i);
    		format(nofi, sizeof(nofi), ":bank: **SO TIEN: $%s** | TK nguoi gui con: $%s", number_format(amount), number_format(PlayerInfo[playerid][pAccount]));
    		DiscordLog(channel, title, nofi);
    	}
	}

	if(dialogid == DIALOG_CHUYENTIEN1)
    {
        if(response) {
   			if(!IsNumeric(inputtext) || strval(inputtext) < 1) {
   				new errstr[512];
   				if(GetPVarInt(playerid, "Offline") == 1) {
   					new namez[24];
   					GetPVarString(playerid, "_NguoiNhan", namez, 24);
   					format(errstr, sizeof errstr, "{FFD700}=== THONG TIN NGUOI NHAN ==={FFFFFF}\n{AAAAAA}Ten chu the  : {00CFFF}%s\n{AAAAAA}So tai khoan : {00CFFF}%d\n\n{FF6B6B}[!] So tien khong hop le!\n{FFFFFF}Nhap lai {00FF7F}so tien{FFFFFF} muon giao dich:", namez, GetPVarInt(playerid, "STK"));
   				} else {
   					new i = GetPVarInt(playerid, "NguoiNhan");
   					format(errstr, sizeof errstr, "{FFD700}=== THONG TIN NGUOI NHAN ==={FFFFFF}\n{AAAAAA}Ten chu the  : {00CFFF}%s\n{AAAAAA}So tai khoan : {00CFFF}%d\n\n{FF6B6B}[!] So tien khong hop le!\n{FFFFFF}Nhap lai {00FF7F}so tien{FFFFFF} muon giao dich:", GetPlayerNameEx(i), PlayerInfo[i][pSoTaiKhoan]);
   				}
   				return ShowPlayerDialog(playerid, DIALOG_CHUYENTIEN1, DIALOG_STYLE_INPUT, "{FFD700}CHUYEN TIEN", errstr, "Tiep tuc", "Thoat");
   			}
			if(PlayerInfo[playerid][pAccount] < strval(inputtext)) return SendClientMessageEx(playerid, COLOR_GREY,"Ban khong du tien trong vi khong the tiep tuc giao dich.");
			SetPVarInt(playerid, "TienGD", strval(inputtext));

			if(GetPVarInt(playerid, "Offline") == 1) {
				new namez[24];
				GetPVarString(playerid, "_NguoiNhan", namez, 24);
				format(string, sizeof string, "{FFD700}=== XAC NHAN GIAO DICH ==={FFFFFF}\n{AAAAAA}Nguoi nhan    : {00CFFF}%s\n{AAAAAA}So tai khoan  : {00CFFF}%d\n{AAAAAA}So tien       : {00FF7F}$%s\n{AAAAAA}Phi giao dich : {FFFFFF}$0\n{AAAAAA}So du con lai : {FF6B6B}$%s\n{FFD700}===============", namez, GetPVarInt(playerid, "STK"), number_format(strval(inputtext)), number_format(PlayerInfo[playerid][pAccount] - strval(inputtext)));
				ShowPlayerDialog(playerid, DIALOG_CHUYENTIEN2, DIALOG_STYLE_MSGBOX, "{FFD700}XAC NHAN GIAO DICH", string, "Xac nhan", "Huy bo");
		   	 	return 1;
			}
			new i = GetPVarInt(playerid, "NguoiNhan");
			format(string, sizeof string, "{FFD700}=== XAC NHAN GIAO DICH ==={FFFFFF}\n{AAAAAA}Nguoi nhan    : {00CFFF}%s\n{AAAAAA}So tai khoan  : {00CFFF}%d\n{AAAAAA}So tien       : {00FF7F}$%s\n{AAAAAA}Phi giao dich : {FFFFFF}$0\n{AAAAAA}So du con lai : {FF6B6B}$%s\n{FFD700}===============", GetPlayerNameEx(i), PlayerInfo[i][pSoTaiKhoan], number_format(strval(inputtext)), number_format(PlayerInfo[playerid][pAccount] - strval(inputtext)));
			ShowPlayerDialog(playerid, DIALOG_CHUYENTIEN2, DIALOG_STYLE_MSGBOX, "{FFD700}XAC NHAN GIAO DICH", string, "Xac nhan", "Huy bo");
		}
		else {
			DeletePVar(playerid,"Offline");
		}
	}

    if(dialogid == DIALOG_DIENTHOAI)
    {
        if(response)
        {
            if(listitem == 0)
            {
    			if(PlayerInfo[playerid][pPhoneBook] == 0)
        			return SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban chua mua sim cho dien thoai.");
                ShowPlayerDialog(playerid, DIALOG_CALL, DIALOG_STYLE_INPUT, "Ban muon goi dien cho ai?", "Ban vui long dien so dien thoai ban can goi vao o ben duoi nay:", "Dong y", "");
            }
            if(listitem == 1)
            {
                ShowOnlineCops(playerid);
            }
            if(listitem == 2)
            {
                ShowOnlineMedics(playerid);
            }
            if(listitem == 3) { //mechanic
				//cmd_goisdt(playerid, "");
				ShowTDN(playerid, NOTIFICATION_WARNING, "Chuc nang dang duoc phat trien...");
				//HienTextdraw(playerid, "Chuc nang dang phat trien...");
			}
            if(listitem == 4)
            {
				ShowOnlineTaxiDrivers(playerid);
            }
        }
	}
	if(dialogid == DIALOG_LIST_COPS || dialogid == DIALOG_LIST_MEDICS || dialogid == DIALOG_LIST_TAXI)
	{
		if(!response) return 1;
		
		new varname[32];
		format(varname, sizeof(varname), "SelectedPhone_%d", listitem);
		new phonenumber = GetPVarInt(playerid, varname);
		
		new total = GetPVarInt(playerid, "TotalPhones");
		
		for(new i = 0; i < total; i++)
		{
			new debugVarname[32];
			format(debugVarname, sizeof(debugVarname), "SelectedPhone_%d", i);
		}
		
		if(phonenumber > 0)
		{
			new phonestr[12];
			format(phonestr, sizeof(phonestr), "%d", phonenumber);
			pc_cmd_call(playerid, phonestr);
		}
		else
		{
			new name[MAX_PLAYER_NAME];
			GetPlayerName(playerid, name, sizeof(name));
				
			SendClientMessageEx(playerid, COLOR_GREY, "Vui long chon lai nguoi can goi.");
		}
		
		// Xóa tất cả PVar sau khi sử dụng
		for(new i = 0; i < total; i++)
		{
			format(varname, sizeof(varname), "SelectedPhone_%d", i);
			DeletePVar(playerid, varname);
		}
		DeletePVar(playerid, "TotalPhones");
		
		return 1;
	}
	if(dialogid == DIALOG_SMS_PHONE)
    {
            if(!response) return 1;
            
            new phonenumber = strval(inputtext);
            if(phonenumber == 0)
            {
                SendClientMessageEx(playerid, COLOR_GREY, "So dien thoai khong hop le!");
                ShowPlayerDialog(playerid, DIALOG_SMS_PHONE, DIALOG_STYLE_INPUT, 
                    "Gui tin nhan", "Nhap so dien thoai nguoi nhan:", "Tiep tuc", "Huy");
                return 1;
            }
            
            // Lưu số điện thoại và chuyển sang nhập nội dung
            SetPVarInt(playerid, "SMS_Number", phonenumber);
            ShowPlayerDialog(playerid, DIALOG_SMS_TEXT, DIALOG_STYLE_INPUT, 
                "Gui tin nhan", "Nhap noi dung tin nhan:", "Gui", "Quay lai");
            return 1;
        }
        
	if(dialogid == DIALOG_SMS_TEXT)
    {
            if(!response)
            {
                // Quay lại nhập số điện thoại
                DeletePVar(playerid, "SMS_Number");
                ShowPlayerDialog(playerid, DIALOG_SMS_PHONE, DIALOG_STYLE_INPUT, 
                    "Gui tin nhan", "Nhap so dien thoai nguoi nhan:", "Tiep tuc", "Huy");
                return 1;
            }
            
            if(strlen(inputtext) == 0)
            {
                SendClientMessageEx(playerid, COLOR_GREY, "Noi dung tin nhan khong duoc de trong!");
                ShowPlayerDialog(playerid, DIALOG_SMS_TEXT, DIALOG_STYLE_INPUT, 
                    "Gui tin nhan", "Nhap noi dung tin nhan:", "Gui", "Quay lai");
                return 1;
            }
            
            // Lấy số điện thoại đã lưu và gọi cmd gốc
            new phonenumber = GetPVarInt(playerid, "SMS_Number");
            new params[128];
            format(params, sizeof(params), "%d %s", phonenumber, inputtext);
            DeletePVar(playerid, "SMS_Number");
            pc_cmd_sms(playerid, params);
            return 1;
        }
	
	if(dialogid == DIALOG_CALL && response == 1)
	{
    	if(strfind(inputtext, "%", true) != -1) {
			return 1;
		}
		if(strfind(inputtext, "c", true) != -1) {
			return 1;
		}

		new phonenumb = strval(inputtext);
		//___________________________________________________________________________________________________//
		if (PlayerInfo[playerid][pJailTime] > 0)
		{
			SendClientMessageEx(playerid,COLOR_GREY,"Ban khong the su dung dien thoai khi dang o trong tu.");
			return 1;
		}
		if(PlayerTied[playerid] != 0 || PlayerCuffed[playerid] != 0)
		{
			SendClientMessageEx(playerid,COLOR_GREY,"Ban khong the su dung dien thoai.");
			return 1;
		}
		if(PlayerInfo[playerid][pPnumber] == 0)
		{
			SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong co dien thoai.");
			return 1;
		}
		if(PhoneOnline[playerid] > 0)
		{
			SendClientMessageEx(playerid, COLOR_GREY, "Dien thoai cua ban dang tat.");
			return 1;
		}
		if(!strcmp(inputtext, "*101#", true)) {
			pc_cmd_xemtiendt(playerid, "");
			return 1;
		}
		if(GetPVarType(playerid, "PlayerCuffed") || GetPVarType(playerid, "Injured") || GetPVarType(playerid, "IsFrozen")) {
	   		return SendClientMessage(playerid, COLOR_GRAD2, "Ban khong the lam dieu do vao luc nay!");
		}
		format(string, sizeof(string), "* %s lay dien thoai ra va goi cho ai do.", GetPlayerNameEx(playerid));
		ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
		/* if(phonenumb == 101)
		{
			cmd_xemtiendt(playerid, "");
			return 1;
		} */
		if(phonenumb == PlayerInfo[playerid][pPnumber])
		{
			SendClientMessageEx(playerid, COLOR_GRAD2, "  Dien thoai nguoi do dang ban...");
			return 1;
		}
		if(Mobile[playerid] != INVALID_PLAYER_ID)
		{
			SendClientMessageEx(playerid, COLOR_GRAD2, "  Ban dang co mot cuoc goi...");
			return 1;
		}

		foreach(new i: Player)
		{
			if(PlayerInfo[i][pPnumber] == phonenumb && phonenumb != 0)
			{
				new giveplayerid = i;
				Mobile[playerid] = giveplayerid; //caller connecting
				if(IsPlayerConnected(giveplayerid))
				{
					if(giveplayerid != INVALID_PLAYER_ID)
					{
						if(PhoneOnline[giveplayerid] > 0)
						{
							SendClientMessageEx(playerid, COLOR_GREY, "Dien thoai nguoi do dang tat.");
							Mobile[playerid] = INVALID_PLAYER_ID;
							return 1;
						}
						if(Mobile[giveplayerid] != INVALID_PLAYER_ID)
						{
							SendClientMessageEx(playerid, COLOR_GRAD2, "Dien thoai nguoi do dang ban...");
							Mobile[playerid] = INVALID_PLAYER_ID;
							return 1;
						}
						if(Spectating[giveplayerid]!=0)
						{
							SendClientMessageEx(playerid, COLOR_GRAD2, "Dien thoai nguoi do dang ban...");
							Mobile[playerid] = INVALID_PLAYER_ID;
							return 1;
						}
						if (Mobile[giveplayerid] == INVALID_PLAYER_ID)
						{
							format(string, sizeof(string), "Dien thoai di dong cua ban dang vang len - su dung /p de tra loi. [Nguoi goi: %s]", GetPlayerNameEx(playerid));
							PlayerPlaySound(playerid, 23000, 0.0, 0.0, 0.0);
							SendClientMessageEx(giveplayerid, COLOR_YELLOW, string);
							RingTone[giveplayerid] = 10;
							format(string, sizeof(string), "* %s's dien thoai dang do chuong.", GetPlayerNameEx(i));
							SendClientMessageEx(playerid, COLOR_WHITE, "HINT: Bay gio ban co the bam T de tra loi cuoc goi, hoac /h de tat cuoc goi.");
							ProxDetector(30.0, i, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
							new Float:rX, Float:rY, Float:rZ;
							GetPlayerPos(giveplayerid, rX, rY, rZ);
							CellTime[playerid] = 1;
							SetPlayerAttachedObject(playerid, 9, 330, 6);
							return SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);
						}
					}
				}
			}
		}
	}

	//________________________________________________________________________//
	if(dialogid == DIALOG_DANHBA)
    {
        if(response)
        {
            if(listitem == 0)
                ShowPlayerDialog(playerid, DANHBA_1, DIALOG_STYLE_LIST, "Danh ba dien thoai cua ban","Goi\nNhan tin\nSua\nXoa","Chon", "Dong");
            if(listitem == 1)
                ShowPlayerDialog(playerid, DANHBA_2, DIALOG_STYLE_LIST, "Danh ba dien thoai cua ban","Goi\nNhan tin\nSua\nXoa","Chon", "Dong");
            if(listitem == 2)
                ShowPlayerDialog(playerid, DANHBA_3, DIALOG_STYLE_LIST, "Danh ba dien thoai cua ban","Goi\nNhan tin\nSua\nXoa","Chon", "Dong");
            if(listitem == 3)
                ShowPlayerDialog(playerid, DANHBA_4, DIALOG_STYLE_LIST, "Danh ba dien thoai cua ban","Goi\nNhan tin\nSua\nXoa","Chon", "Dong");
            if(listitem == 4)
                ShowPlayerDialog(playerid, DANHBA_5, DIALOG_STYLE_LIST, "Danh ba dien thoai cua ban","Goi\nNhan tin\nSua\nXoa","Chon", "Dong");
            if(listitem == 5)
                ShowPlayerDialog(playerid, DANHBA_6, DIALOG_STYLE_LIST, "Danh ba dien thoai cua ban","Goi\nNhan tin\nSua\nXoa","Chon", "Dong");
            if(listitem == 6)
                ShowPlayerDialog(playerid, DANHBA_7, DIALOG_STYLE_LIST, "Danh ba dien thoai cua ban","Goi\nNhan tin\nSua\nXoa","Chon", "Dong");
            if(listitem == 7)
                ShowPlayerDialog(playerid, DANHBA_8, DIALOG_STYLE_LIST, "Danh ba dien thoai cua ban","Goi\nNhan tin\nSua\nXoa","Chon", "Dong");
            if(listitem == 8)
                ShowPlayerDialog(playerid, DANHBA_9, DIALOG_STYLE_LIST, "Danh ba dien thoai cua ban","Goi\nNhan tin\nSua\nXoa","Chon", "Dong");
        }
	}

	//________________________________________________________________________//
	if(dialogid == DANHBA_1)
    {
        if(response)
        {
            if(listitem == 0){ // GOI
				if(PlayerInfo[playerid][pSDT1] != 0){
				    new str[2460], str1[1280], str2[1280], str3[1280], str4[1280], name[1280];
					format(str1, sizeof(str), "THONG TIN CUOC GOI : ");
				    format(str2, sizeof(str), "    -> Ten nguoi nhan : %s", PlayerInfo[playerid][pDanhBa1]);
				    format(str3, sizeof(str), "    -> SDT : %d", PlayerInfo[playerid][pSDT1]);
				    format(str4, sizeof(str), "Neu ban muon goi dien cho nguoi nay, vui long nhan 'Goi ngay' de bat dau cuoc goi");
				    format(str, sizeof(str), "%s\n\n%s\n%s\n%s", str1,str2,str3,str4);
				    format(name, sizeof(name), "DIEN THOAI");
					ShowPlayerDialog(playerid, GOIDIEN_1, DIALOG_STYLE_MSGBOX, name, str, "Goi ngay", "Huy Bo");
					return 1;
				}else ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "DIEN THOAI","Hien tai danh ba nay van dang con trong , ban vui long them nguoi than vao danh ba slot nay de goi dien...","Dong y", "");
            }
            if(listitem == 1){ // NHAN TIN
				if(PlayerInfo[playerid][pSDT1] != 0){
				    ShowPlayerDialog(playerid, NHANTIN_1, DIALOG_STYLE_INPUT, "Nhan tin", "Vui long nhap tin nhan", "Gui", "Huy");
				    return 1;
				}else ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "DIEN THOAI","Hien tai danh ba nay van dang con trong , ban vui long them nguoi than vao danh ba slot nay de nhan tin...","Dong y", "");
            }
            if(listitem == 2){ // EDIT DANH BA
                ShowPlayerDialog(playerid, EDITDB_1, DIALOG_STYLE_INPUT, "Them/Sua so dien thoai", "Vui long dien ten hien thi", "Tiep Tuc", "Tro lai");
                return 1;
			}
			if(listitem == 3){ // XOA DANH BA
	            PlayerInfo[playerid][pSDT1] = 0;
	            format(PlayerInfo[playerid][pDanhBa1], 32, "None");
	            SendClientMessageEx(playerid, COLOR_YELLOW, "Ban da xoa danh ba thanh cong !.");
			}
        }
	}

	if(dialogid == GOIDIEN_1)
    {
        if(response){
            new phonenumb = PlayerInfo[playerid][pSDT1];
            if (PlayerInfo[playerid][pJailTime] > 0)
			{
				SendClientMessageEx(playerid,COLOR_GREY,"Ban khong the su dung dien thoai khi dang o trong tu.");
				return 1;
			}
			if(PlayerTied[playerid] != 0 || PlayerCuffed[playerid] != 0)
			{
				SendClientMessageEx(playerid,COLOR_GREY,"Ban khong the su dung dien thoai.");
				return 1;
			}
			if(PlayerInfo[playerid][pPnumber] == 0)
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong co dien thoai.");
				return 1;
			}
			if(PhoneOnline[playerid] > 0)
			{
				SendClientMessageEx(playerid, COLOR_GREY, "Dien thoai cua ban dang tat.");
				return 1;
			}
			if(GetPVarType(playerid, "PlayerCuffed") || GetPVarType(playerid, "Injured") || GetPVarType(playerid, "IsFrozen")) {
		   		return SendClientMessage(playerid, COLOR_GRAD2, "Ban khong the lam dieu do vao luc nay!");
			}
			format(string, sizeof(string), "* %s lay dien thoai di dong tu tui quan ra.", GetPlayerNameEx(playerid));
			ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
			if(phonenumb == PlayerInfo[playerid][pPnumber])
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "  Dien thoai nguoi do dang ban...");
				return 1;
			}
			if(Mobile[playerid] != INVALID_PLAYER_ID)
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "  Ban dang co mot cuoc goi...");
				return 1;
			}

			foreach(new i: Player)
			{
				if(PlayerInfo[i][pPnumber] == phonenumb && phonenumb != 0)
				{
					new giveplayerid = i;
					Mobile[playerid] = giveplayerid; //caller connecting
					if(IsPlayerConnected(giveplayerid))
					{
						if(giveplayerid != INVALID_PLAYER_ID)
						{
							if(PhoneOnline[giveplayerid] > 0)
							{
								SendClientMessageEx(playerid, COLOR_GREY, "Dien thoai nguoi do dang tat.");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if(Mobile[giveplayerid] != INVALID_PLAYER_ID)
							{
								SendClientMessageEx(playerid, COLOR_GRAD2, "Dien thoai nguoi do dang ban...");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if(Spectating[giveplayerid]!=0)
							{
								SendClientMessageEx(playerid, COLOR_GRAD2, "Dien thoai nguoi do dang ban...");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if (Mobile[giveplayerid] == INVALID_PLAYER_ID)
							{
								format(string, sizeof(string), "Dien thoai di dong cua ban dang vang len - su dung /p de tra loi. [Nguoi goi: %s]", GetPlayerNameEx(playerid));
								PlayerPlaySound(playerid, 23000, 0.0, 0.0, 0.0);
								SendClientMessageEx(giveplayerid, COLOR_YELLOW, string);
								RingTone[giveplayerid] = 10;
								format(string, sizeof(string), "* %s's dien thoai dang do chuong.", GetPlayerNameEx(i));
								SendClientMessageEx(playerid, COLOR_WHITE, "HINT: Bay gio ban co the bam T de tra loi cuoc goi, hoac /h de tat cuoc goi.");
								ProxDetector(30.0, i, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
								new Float:rX, Float:rY, Float:rZ;
								GetPlayerPos(giveplayerid, rX, rY, rZ);
								CellTime[playerid] = 1;
								SetPlayerAttachedObject(playerid, 9, 330, 6);
								return SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);
							}
						}
					}
				}
			}
        }
    }

    if(dialogid == NHANTIN_1)
    {
       if(response)
       {
			new phonenumb = PlayerInfo[playerid][pSDT1];
    	    if(Spectating[playerid] == 0)
			{
				format(string, sizeof(string), "* %s lay dien thoai di dong tu trong tui quan.", GetPlayerNameEx(playerid));
				ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
			}

			if(phonenumb == 555)
			{
				if ((strcmp("yes", inputtext, true, strlen(inputtext)) == 0) && (strlen(inputtext) == strlen("yes")))
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
					Mobile[playerid] = giveplayerid; //caller connecting
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
		       						format(string, sizeof(string), "(BE) %s SMS cho %s: %s", GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid), inputtext);
									SendClientMessageEx(u,COLOR_YELLOW, string);
								}
							}
							if(PlayerInfo[playerid][pPhonePrivacy] == 1)
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: Khong biet.", inputtext, GetPlayerNameEx(playerid));
							}
							else
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: %s (%d)", inputtext, GetPlayerNameEx(playerid), PlayerInfo[playerid][pPnumber]);
							}

							if(i != playerid)
							{
								if(PlayerInfo[i][pSmslog] > 0)
								{
									new query[384], ftext[128];
									mysql_escape_string(inputtext, ftext);
									if(PlayerInfo[playerid][pPhonePrivacy] == 1) format(query, sizeof(query), "INSERT INTO `sms` (`id`, `sender`, `senderid`, `sendernumber`, `receiver`, `receiverid`, `receivernumber`, `message`, `date`) VALUES (NULL, '%s', %d, 0, '%s', %d, %d, '%s', NOW())", GetPlayerNameExt(playerid), GetPlayerSQLId(playerid), GetPlayerNameExt(i), GetPlayerSQLId(i), phonenumb, ftext);
									else format(query, sizeof(query), "INSERT INTO `sms` (`id`, `sender`, `senderid`, `sendernumber`, `receiver`, `receiverid`, `receivernumber`, `message`, `date`) VALUES (NULL, '%s', %d, %d, '%s', %d, %d, '%s', NOW())", GetPlayerNameExt(playerid), GetPlayerSQLId(playerid), PlayerInfo[playerid][pPnumber], GetPlayerNameExt(i), GetPlayerSQLId(i), phonenumb, ftext);
									mysql_function_query(MainPipeline, query, false, "OnQueryFinish", "i", SENDDATA_THREAD);
								}
							}

							RingTone[giveplayerid] =20;
							SendClientMessageEx(giveplayerid, COLOR_YELLOW, string);
							SendClientMessageEx(playerid, COLOR_YELLOW, string);
							SendClientMessageEx(playerid, COLOR_WHITE, "Gui tin nhan van ban");
							
							Mobile[playerid] = INVALID_PLAYER_ID;

							if(strcmp(PlayerInfo[giveplayerid][pAutoTextReply], "Nothing", true) != 0)
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: %s [automated response] (%d)", PlayerInfo[giveplayerid][pAutoTextReply], GetPlayerNameEx(giveplayerid), PlayerInfo[giveplayerid][pPnumber]);
								SendClientMessageEx(playerid, COLOR_YELLOW, string);
							}

							return 1;
						}
					}
				}
			}
			SendClientMessageEx(playerid, COLOR_GRAD2, "  Gui tin nhan that bai...");
       }
    }

    if(dialogid == EDITDB_1)
    {
        if(response)
		{
			strmid(PlayerInfo[playerid][pDanhBa1], inputtext, 0, strlen(inputtext), 128);
			format(string, sizeof(string), "UPDATE `accounts` SET `DanhBa1`='%s' WHERE `id`=%d", inputtext, GetPlayerSQLId(playerid));
			mysql_function_query(MainPipeline, string, false, "OnQueryFinish", "i", SENDDATA_THREAD);
			ShowPlayerDialog(playerid, EDITDB_1_1, DIALOG_STYLE_INPUT, "Them/Sua so dien thoai", "Vui long dien so dien thoai", "Tiep Tuc", "Tro lai");
        }
	}

	if(dialogid == EDITDB_1_1)
    {
		if(response)
		{
			new sdtdanhba = strval(inputtext);

			if(~strfind(inputtext, ";") || ~strfind(inputtext, ",") || ~strfind(inputtext, "'") || ~strfind(inputtext, ":") ||
			~strfind(inputtext, "|") || ~strfind(inputtext, "{") || ~strfind(inputtext, "}") || ~strfind(inputtext, "+") ||
			~strfind(inputtext, "?") || ~strfind(inputtext, "/")  || ~strfind(inputtext, " ") ||
			~strfind(inputtext, "@") || ~strfind(inputtext, "[") || ~strfind(inputtext, "]") || ~strfind(inputtext, "<") ||
			~strfind(inputtext, ">") || ~strfind(inputtext, "!") || ~strfind(inputtext, "#") || ~strfind(inputtext, "^") ||
			~strfind(inputtext, "*") || ~strfind(inputtext, "&") || ~strfind(inputtext, "q") || ~strfind(inputtext, "w") ||
			~strfind(inputtext, "e") || ~strfind(inputtext, "r") || ~strfind(inputtext, "t") || ~strfind(inputtext, "y") ||
			~strfind(inputtext, "u") || ~strfind(inputtext, "i") || ~strfind(inputtext, "o") || ~strfind(inputtext, "p") ||
			~strfind(inputtext, "a") || ~strfind(inputtext, "s") || ~strfind(inputtext, "d") || ~strfind(inputtext, "f") ||
			~strfind(inputtext, "g") || ~strfind(inputtext, "h") || ~strfind(inputtext, "j") || ~strfind(inputtext, "k") ||
			~strfind(inputtext, "l") || ~strfind(inputtext, "z") || ~strfind(inputtext, "x") || ~strfind(inputtext, "c") ||
			~strfind(inputtext, "v") || ~strfind(inputtext, "b") || ~strfind(inputtext, "n") || ~strfind(inputtext, "m") ||
			~strfind(inputtext, "(") || ~strfind(inputtext, ")") || ~strfind(inputtext, ".") || ~strfind(inputtext, "%")) return ShowPlayerDialog(playerid, EDITDB_1_1, DIALOG_STYLE_INPUT,"{ff0000}[!]","{ff0000}Vui long nhap so dien thoai bang so.","Chap nhan","Thoat");

			PlayerInfo[playerid][pSDT1] = sdtdanhba;

			format(string, sizeof(string), "UPDATE `accounts` SET `SDT1`=%d WHERE `id` = %d",number_format(sdtdanhba), GetPlayerSQLId(playerid));
			mysql_function_query(MainPipeline, string, false, "OnQueryFinish", "ii", SENDDATA_THREAD, playerid);

			format(string, sizeof(string), "Ban da them thanh cong so dien thoai %d duoi ten %s.",PlayerInfo[playerid][pSDT1], PlayerInfo[playerid][pDanhBa1]);
			SendClientMessageEx(playerid, COLOR_YELLOW, string);
		}
    }

    //________________________________________________________________________//
	if(dialogid == DANHBA_2)
    {
        if(response)
        {
            if(listitem == 0){ // GOI
				if(PlayerInfo[playerid][pSDT2] != 0){
				    new str[2460], str1[1280], str2[1280], str3[1280], str4[1280], name[1280];
					format(str1, sizeof(str), "THONG TIN CUOC GOI : ");
				    format(str2, sizeof(str), "    -> Ten nguoi nhan : %s", PlayerInfo[playerid][pDanhBa2]);
				    format(str3, sizeof(str), "    -> SDT : %d", PlayerInfo[playerid][pSDT2]);
				    format(str4, sizeof(str), "Neu ban muon goi dien cho nguoi nay, vui long nhan 'Goi ngay' de bat dau cuoc goi");
				    format(str, sizeof(str), "%s\n\n%s\n%s\n%s", str1,str2,str3,str4);
				    format(name, sizeof(name), "DIEN THOAI");
					ShowPlayerDialog(playerid, GOIDIEN_2, DIALOG_STYLE_MSGBOX, name, str, "Goi ngay", "Huy Bo");
					return 1;
				}else ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "DIEN THOAI","Hien tai danh ba nay van dang con trong , ban vui long them nguoi than vao danh ba slot nay de goi dien...","Dong y", "");
            }
            if(listitem == 1){ // NHAN TIN
				if(PlayerInfo[playerid][pSDT2] != 0){
				    ShowPlayerDialog(playerid, NHANTIN_2, DIALOG_STYLE_INPUT, "Nhan tin", "Vui long nhap tin nhan", "Gui", "Huy");
				    return 1;
				}else ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "DIEN THOAI","Hien tai danh ba nay van dang con trong , ban vui long them nguoi than vao danh ba slot nay de nhan tin...","Dong y", "");
            }
            if(listitem == 2){ // EDIT DANH BA
                ShowPlayerDialog(playerid, EDITDB_2, DIALOG_STYLE_INPUT, "Them/Sua so dien thoai", "Vui long dien ten hien thi", "Tiep Tuc", "Tro lai");
                return 1;
			}
			if(listitem == 3){ // XOA DANH BA
	            PlayerInfo[playerid][pSDT2] = 0;
	            format(PlayerInfo[playerid][pDanhBa2], 32, "None");
	            SendClientMessageEx(playerid, COLOR_YELLOW, "Ban da xoa danh ba thanh cong !.");
			}
        }
	}

	if(dialogid == GOIDIEN_2)
    {
        if(response){
            new phonenumb = PlayerInfo[playerid][pSDT2];
            if (PlayerInfo[playerid][pJailTime] > 0)
			{
				SendClientMessageEx(playerid,COLOR_GREY,"Ban khong the su dung dien thoai khi dang o trong tu.");
				return 1;
			}
			if(PlayerTied[playerid] != 0 || PlayerCuffed[playerid] != 0)
			{
				SendClientMessageEx(playerid,COLOR_GREY,"Ban khong the su dung dien thoai.");
				return 1;
			}
			if(PlayerInfo[playerid][pPnumber] == 0)
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong co dien thoai.");
				return 1;
			}
			if(PhoneOnline[playerid] > 0)
			{
				SendClientMessageEx(playerid, COLOR_GREY, "Dien thoai cua ban dang tat.");
				return 1;
			}
			if(GetPVarType(playerid, "PlayerCuffed") || GetPVarType(playerid, "Injured") || GetPVarType(playerid, "IsFrozen")) {
		   		return SendClientMessage(playerid, COLOR_GRAD2, "Ban khong the lam dieu do vao luc nay!");
			}
			format(string, sizeof(string), "* %s lay dien thoai di dong tu tui quan ra.", GetPlayerNameEx(playerid));
			ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
			if(phonenumb == PlayerInfo[playerid][pPnumber])
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "  Dien thoai nguoi do dang ban...");
				return 1;
			}
			if(Mobile[playerid] != INVALID_PLAYER_ID)
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "  Ban dang co mot cuoc goi...");
				return 1;
			}

			foreach(new i: Player)
			{
				if(PlayerInfo[i][pPnumber] == phonenumb && phonenumb != 0)
				{
					new giveplayerid = i;
					Mobile[playerid] = giveplayerid; //caller connecting
					if(IsPlayerConnected(giveplayerid))
					{
						if(giveplayerid != INVALID_PLAYER_ID)
						{
							if(PhoneOnline[giveplayerid] > 0)
							{
								SendClientMessageEx(playerid, COLOR_GREY, "Dien thoai nguoi do dang tat.");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if(Mobile[giveplayerid] != INVALID_PLAYER_ID)
							{
								SendClientMessageEx(playerid, COLOR_GRAD2, "Dien thoai nguoi do dang ban...");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if(Spectating[giveplayerid]!=0)
							{
								SendClientMessageEx(playerid, COLOR_GRAD2, "Dien thoai nguoi do dang ban...");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if (Mobile[giveplayerid] == INVALID_PLAYER_ID)
							{
								format(string, sizeof(string), "Dien thoai di dong cua ban dang vang len - su dung /p de tra loi. [Nguoi goi: %s]", GetPlayerNameEx(playerid));
								PlayerPlaySound(playerid, 23000, 0.0, 0.0, 0.0);
								SendClientMessageEx(giveplayerid, COLOR_YELLOW, string);
								RingTone[giveplayerid] = 10;
								format(string, sizeof(string), "* %s's dien thoai dang do chuong.", GetPlayerNameEx(i));
								SendClientMessageEx(playerid, COLOR_WHITE, "HINT: Bay gio ban co the bam T de tra loi cuoc goi, hoac /h de tat cuoc goi.");
								ProxDetector(30.0, i, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
								new Float:rX, Float:rY, Float:rZ;
								GetPlayerPos(giveplayerid, rX, rY, rZ);
								CellTime[playerid] = 1;
								SetPlayerAttachedObject(playerid, 9, 330, 6);
								return SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);
							}
						}
					}
				}
			}
        }
    }

    if(dialogid == NHANTIN_2)
    {
       if(response)
       {
			new phonenumb = PlayerInfo[playerid][pSDT2];
    	    if(Spectating[playerid] == 0)
			{
				format(string, sizeof(string), "* %s lay dien thoai di dong tu trong tui quan.", GetPlayerNameEx(playerid));
				ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
			}
			if(phonenumb == 555)
			{
				if ((strcmp("yes", inputtext, true, strlen(inputtext)) == 0) && (strlen(inputtext) == strlen("yes")))
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
					Mobile[playerid] = giveplayerid; //caller connecting
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
		       						format(string, sizeof(string), "(BE) %s SMS cho %s: %s", GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid), inputtext);
									SendClientMessageEx(u,COLOR_YELLOW, string);
								}
							}
							if(PlayerInfo[playerid][pPhonePrivacy] == 1)
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: Khong biet.", inputtext, GetPlayerNameEx(playerid));
							}
							else
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: %s (%d)", inputtext, GetPlayerNameEx(playerid), PlayerInfo[playerid][pPnumber]);
							}

							if(i != playerid)
							{
								if(PlayerInfo[i][pSmslog] > 0)
								{
									new query[384], ftext[128];
									mysql_escape_string(inputtext, ftext);
									if(PlayerInfo[playerid][pPhonePrivacy] == 1) format(query, sizeof(query), "INSERT INTO `sms` (`id`, `sender`, `senderid`, `sendernumber`, `receiver`, `receiverid`, `receivernumber`, `message`, `date`) VALUES (NULL, '%s', %d, 0, '%s', %d, %d, '%s', NOW())", GetPlayerNameExt(playerid), GetPlayerSQLId(playerid), GetPlayerNameExt(i), GetPlayerSQLId(i), phonenumb, ftext);
									else format(query, sizeof(query), "INSERT INTO `sms` (`id`, `sender`, `senderid`, `sendernumber`, `receiver`, `receiverid`, `receivernumber`, `message`, `date`) VALUES (NULL, '%s', %d, %d, '%s', %d, %d, '%s', NOW())", GetPlayerNameExt(playerid), GetPlayerSQLId(playerid), PlayerInfo[playerid][pPnumber], GetPlayerNameExt(i), GetPlayerSQLId(i), phonenumb, ftext);
									mysql_function_query(MainPipeline, query, false, "OnQueryFinish", "i", SENDDATA_THREAD);
								}
							}

							RingTone[giveplayerid] =20;
							SendClientMessageEx(giveplayerid, COLOR_YELLOW, string);
							SendClientMessageEx(playerid, COLOR_YELLOW, string);
							SendClientMessageEx(playerid, COLOR_WHITE, "Gui tin nhan van ban");
							Mobile[playerid] = INVALID_PLAYER_ID;

							if(strcmp(PlayerInfo[giveplayerid][pAutoTextReply], "Nothing", true) != 0)
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: %s [automated response] (%d)", PlayerInfo[giveplayerid][pAutoTextReply], GetPlayerNameEx(giveplayerid), PlayerInfo[giveplayerid][pPnumber]);
								SendClientMessageEx(playerid, COLOR_YELLOW, string);
							}

							return 1;
						}
					}
				}
			}
			SendClientMessageEx(playerid, COLOR_GRAD2, "  Gui tin nhan that bai...");
       }
    }

    if(dialogid == EDITDB_2)
    {
        if(response)
		{
			strmid(PlayerInfo[playerid][pDanhBa2], inputtext, 0, strlen(inputtext), 128);
			format(string, sizeof(string), "UPDATE `accounts` SET `DanhBa2`='%s' WHERE `id`=%d", inputtext, GetPlayerSQLId(playerid));
			mysql_function_query(MainPipeline, string, false, "OnQueryFinish", "i", SENDDATA_THREAD);
			ShowPlayerDialog(playerid, EDITDB_2_2, DIALOG_STYLE_INPUT, "Them/Sua so dien thoai", "Vui long dien so dien thoai", "Tiep Tuc", "Tro lai");
        }
	}

	if(dialogid == EDITDB_2_2)
    {
		if(response)
		{
			new sdtdanhba = strval(inputtext);

			if(~strfind(inputtext, ";") || ~strfind(inputtext, ",") || ~strfind(inputtext, "'") || ~strfind(inputtext, ":") ||
			~strfind(inputtext, "|") || ~strfind(inputtext, "{") || ~strfind(inputtext, "}") || ~strfind(inputtext, "+") ||
			~strfind(inputtext, "?") || ~strfind(inputtext, "/")  || ~strfind(inputtext, " ") ||
			~strfind(inputtext, "@") || ~strfind(inputtext, "[") || ~strfind(inputtext, "]") || ~strfind(inputtext, "<") ||
			~strfind(inputtext, ">") || ~strfind(inputtext, "!") || ~strfind(inputtext, "#") || ~strfind(inputtext, "^") ||
			~strfind(inputtext, "*") || ~strfind(inputtext, "&") || ~strfind(inputtext, "q") || ~strfind(inputtext, "w") ||
			~strfind(inputtext, "e") || ~strfind(inputtext, "r") || ~strfind(inputtext, "t") || ~strfind(inputtext, "y") ||
			~strfind(inputtext, "u") || ~strfind(inputtext, "i") || ~strfind(inputtext, "o") || ~strfind(inputtext, "p") ||
			~strfind(inputtext, "a") || ~strfind(inputtext, "s") || ~strfind(inputtext, "d") || ~strfind(inputtext, "f") ||
			~strfind(inputtext, "g") || ~strfind(inputtext, "h") || ~strfind(inputtext, "j") || ~strfind(inputtext, "k") ||
			~strfind(inputtext, "l") || ~strfind(inputtext, "z") || ~strfind(inputtext, "x") || ~strfind(inputtext, "c") ||
			~strfind(inputtext, "v") || ~strfind(inputtext, "b") || ~strfind(inputtext, "n") || ~strfind(inputtext, "m") ||
			~strfind(inputtext, "(") || ~strfind(inputtext, ")") || ~strfind(inputtext, ".") || ~strfind(inputtext, "%")) return ShowPlayerDialog(playerid, EDITDB_2_2, DIALOG_STYLE_INPUT,"{ff0000}[!]","{ff0000}Vui long nhap so dien thoai bang so.","Chap nhan","Thoat");

			PlayerInfo[playerid][pSDT2] = sdtdanhba;

			format(string, sizeof(string), "UPDATE `accounts` SET `SDT2`=%d WHERE `id` = %d",number_format(sdtdanhba), GetPlayerSQLId(playerid));
			mysql_function_query(MainPipeline, string, false, "OnQueryFinish", "ii", SENDDATA_THREAD, playerid);

			format(string, sizeof(string), "Ban da them thanh cong so dien thoai %d duoi ten %s.",PlayerInfo[playerid][pSDT2], PlayerInfo[playerid][pDanhBa2]);
			SendClientMessageEx(playerid, COLOR_YELLOW, string);
		}
    }

    //________________________________________________________________________//
	if(dialogid == DANHBA_3)
    {
        if(response)
        {
            if(listitem == 0){ // GOI
				if(PlayerInfo[playerid][pSDT3] != 0){
				    new str[2460], str1[1280], str2[1280], str3[1280], str4[1280], name[1280];
					format(str1, sizeof(str), "THONG TIN CUOC GOI : ");
				    format(str2, sizeof(str), "    -> Ten nguoi nhan : %s", PlayerInfo[playerid][pDanhBa3]);
				    format(str3, sizeof(str), "    -> SDT : %d", PlayerInfo[playerid][pSDT3]);
				    format(str4, sizeof(str), "Neu ban muon goi dien cho nguoi nay, vui long nhan 'Goi ngay' de bat dau cuoc goi");
				    format(str, sizeof(str), "%s\n\n%s\n%s\n%s", str1,str2,str3,str4);
				    format(name, sizeof(name), "DIEN THOAI");
					ShowPlayerDialog(playerid, GOIDIEN_3, DIALOG_STYLE_MSGBOX, name, str, "Goi ngay", "Huy Bo");
					return 1;
				}else ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "DIEN THOAI","Hien tai danh ba nay van dang con trong , ban vui long them nguoi than vao danh ba slot nay de goi dien...","Dong y", "");
            }
            if(listitem == 1){ // NHAN TIN
				if(PlayerInfo[playerid][pSDT3] != 0){
				    ShowPlayerDialog(playerid, NHANTIN_3, DIALOG_STYLE_INPUT, "Nhan tin", "Vui long nhap tin nhan", "Gui", "Huy");
				    return 1;
				}else ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "DIEN THOAI","Hien tai danh ba nay van dang con trong , ban vui long them nguoi than vao danh ba slot nay de nhan tin...","Dong y", "");
            }
            if(listitem == 2){ // EDIT DANH BA
                ShowPlayerDialog(playerid, EDITDB_3, DIALOG_STYLE_INPUT, "Them/Sua so dien thoai", "Vui long dien ten hien thi", "Tiep Tuc", "Tro lai");
                return 1;
			}
			if(listitem == 3){ // XOA DANH BA
	            PlayerInfo[playerid][pSDT3] = 0;
	            format(PlayerInfo[playerid][pDanhBa3], 32, "None");
	            SendClientMessageEx(playerid, COLOR_YELLOW, "Ban da xoa danh ba thanh cong !.");
			}
        }
	}

	if(dialogid == GOIDIEN_3)
    {
        if(response){
            new phonenumb = PlayerInfo[playerid][pSDT3];
            if (PlayerInfo[playerid][pJailTime] > 0)
			{
				SendClientMessageEx(playerid,COLOR_GREY,"Ban khong the su dung dien thoai khi dang o trong tu.");
				return 1;
			}
			if(PlayerTied[playerid] != 0 || PlayerCuffed[playerid] != 0)
			{
				SendClientMessageEx(playerid,COLOR_GREY,"Ban khong the su dung dien thoai.");
				return 1;
			}
			if(PlayerInfo[playerid][pPnumber] == 0)
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong co dien thoai.");
				return 1;
			}
			if(PhoneOnline[playerid] > 0)
			{
				SendClientMessageEx(playerid, COLOR_GREY, "Dien thoai cua ban dang tat.");
				return 1;
			}
			if(GetPVarType(playerid, "PlayerCuffed") || GetPVarType(playerid, "Injured") || GetPVarType(playerid, "IsFrozen")) {
		   		return SendClientMessage(playerid, COLOR_GRAD2, "Ban khong the lam dieu do vao luc nay!");
			}
			format(string, sizeof(string), "* %s lay dien thoai di dong tu tui quan ra.", GetPlayerNameEx(playerid));
			ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
			if(phonenumb == PlayerInfo[playerid][pPnumber])
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "  Dien thoai nguoi do dang ban...");
				return 1;
			}
			if(Mobile[playerid] != INVALID_PLAYER_ID)
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "  Ban dang co mot cuoc goi...");
				return 1;
			}

			foreach(new i: Player)
			{
				if(PlayerInfo[i][pPnumber] == phonenumb && phonenumb != 0)
				{
					new giveplayerid = i;
					Mobile[playerid] = giveplayerid; //caller connecting
					if(IsPlayerConnected(giveplayerid))
					{
						if(giveplayerid != INVALID_PLAYER_ID)
						{
							if(PhoneOnline[giveplayerid] > 0)
							{
								SendClientMessageEx(playerid, COLOR_GREY, "Dien thoai nguoi do dang tat.");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if(Mobile[giveplayerid] != INVALID_PLAYER_ID)
							{
								SendClientMessageEx(playerid, COLOR_GRAD2, "Dien thoai nguoi do dang ban...");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if(Spectating[giveplayerid]!=0)
							{
								SendClientMessageEx(playerid, COLOR_GRAD2, "Dien thoai nguoi do dang ban...");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if (Mobile[giveplayerid] == INVALID_PLAYER_ID)
							{
								format(string, sizeof(string), "Dien thoai di dong cua ban dang vang len - su dung /p de tra loi. [Nguoi goi: %s]", GetPlayerNameEx(playerid));
								PlayerPlaySound(playerid, 23000, 0.0, 0.0, 0.0);
								SendClientMessageEx(giveplayerid, COLOR_YELLOW, string);
								RingTone[giveplayerid] = 10;
								format(string, sizeof(string), "* %s's dien thoai dang do chuong.", GetPlayerNameEx(i));
								SendClientMessageEx(playerid, COLOR_WHITE, "HINT: Bay gio ban co the bam T de tra loi cuoc goi, hoac /h de tat cuoc goi.");
								ProxDetector(30.0, i, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
								new Float:rX, Float:rY, Float:rZ;
								GetPlayerPos(giveplayerid, rX, rY, rZ);
								CellTime[playerid] = 1;
								SetPlayerAttachedObject(playerid, 9, 330, 6);
								return SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);
							}
						}
					}
				}
			}
        }
    }

    if(dialogid == NHANTIN_3)
    {
       if(response)
       {
			new phonenumb = PlayerInfo[playerid][pSDT3];
    	    if(Spectating[playerid] == 0)
			{
				format(string, sizeof(string), "* %s lay dien thoai di dong tu trong tui quan.", GetPlayerNameEx(playerid));
				ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
			}

			if(phonenumb == 555)
			{
				if ((strcmp("yes", inputtext, true, strlen(inputtext)) == 0) && (strlen(inputtext) == strlen("yes")))
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
					Mobile[playerid] = giveplayerid; //caller connecting
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
		       						format(string, sizeof(string), "(BE) %s SMS cho %s: %s", GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid), inputtext);
									SendClientMessageEx(u,COLOR_YELLOW, string);
								}
							}
							if(PlayerInfo[playerid][pPhonePrivacy] == 1)
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: Khong biet.", inputtext, GetPlayerNameEx(playerid));
							}
							else
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: %s (%d)", inputtext, GetPlayerNameEx(playerid), PlayerInfo[playerid][pPnumber]);
							}

							if(i != playerid)
							{
								if(PlayerInfo[i][pSmslog] > 0)
								{
									new query[384], ftext[128];
									mysql_escape_string(inputtext, ftext);
									if(PlayerInfo[playerid][pPhonePrivacy] == 1) format(query, sizeof(query), "INSERT INTO `sms` (`id`, `sender`, `senderid`, `sendernumber`, `receiver`, `receiverid`, `receivernumber`, `message`, `date`) VALUES (NULL, '%s', %d, 0, '%s', %d, %d, '%s', NOW())", GetPlayerNameExt(playerid), GetPlayerSQLId(playerid), GetPlayerNameExt(i), GetPlayerSQLId(i), phonenumb, ftext);
									else format(query, sizeof(query), "INSERT INTO `sms` (`id`, `sender`, `senderid`, `sendernumber`, `receiver`, `receiverid`, `receivernumber`, `message`, `date`) VALUES (NULL, '%s', %d, %d, '%s', %d, %d, '%s', NOW())", GetPlayerNameExt(playerid), GetPlayerSQLId(playerid), PlayerInfo[playerid][pPnumber], GetPlayerNameExt(i), GetPlayerSQLId(i), phonenumb, ftext);
									mysql_function_query(MainPipeline, query, false, "OnQueryFinish", "i", SENDDATA_THREAD);
								}
							}

							RingTone[giveplayerid] =20;
							SendClientMessageEx(giveplayerid, COLOR_YELLOW, string);
							SendClientMessageEx(playerid, COLOR_YELLOW, string);
							SendClientMessageEx(playerid, COLOR_WHITE, "Gui tin nhan van ban");
							Mobile[playerid] = INVALID_PLAYER_ID;

							if(strcmp(PlayerInfo[giveplayerid][pAutoTextReply], "Nothing", true) != 0)
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: %s [automated response] (%d)", PlayerInfo[giveplayerid][pAutoTextReply], GetPlayerNameEx(giveplayerid), PlayerInfo[giveplayerid][pPnumber]);
								SendClientMessageEx(playerid, COLOR_YELLOW, string);
							}

							return 1;
						}
					}
				}
			}
			SendClientMessageEx(playerid, COLOR_GRAD2, "  Gui tin nhan that bai...");
       }
    }

    if(dialogid == EDITDB_3)
    {
        if(response)
		{
			strmid(PlayerInfo[playerid][pDanhBa3], inputtext, 0, strlen(inputtext), 128);
			format(string, sizeof(string), "UPDATE `accounts` SET `DanhBa3`='%s' WHERE `id`=%d", inputtext, GetPlayerSQLId(playerid));
			mysql_function_query(MainPipeline, string, false, "OnQueryFinish", "i", SENDDATA_THREAD);
			ShowPlayerDialog(playerid, EDITDB_3_3, DIALOG_STYLE_INPUT, "Them/Sua so dien thoai", "Vui long dien so dien thoai", "Tiep Tuc", "Tro lai");
        }
	}

	if(dialogid == EDITDB_3_3)
    {
		if(response)
		{
			new sdtdanhba = strval(inputtext);

			if(~strfind(inputtext, ";") || ~strfind(inputtext, ",") || ~strfind(inputtext, "'") || ~strfind(inputtext, ":") ||
			~strfind(inputtext, "|") || ~strfind(inputtext, "{") || ~strfind(inputtext, "}") || ~strfind(inputtext, "+") ||
			~strfind(inputtext, "?") || ~strfind(inputtext, "/")  || ~strfind(inputtext, " ") ||
			~strfind(inputtext, "@") || ~strfind(inputtext, "[") || ~strfind(inputtext, "]") || ~strfind(inputtext, "<") ||
			~strfind(inputtext, ">") || ~strfind(inputtext, "!") || ~strfind(inputtext, "#") || ~strfind(inputtext, "^") ||
			~strfind(inputtext, "*") || ~strfind(inputtext, "&") || ~strfind(inputtext, "q") || ~strfind(inputtext, "w") ||
			~strfind(inputtext, "e") || ~strfind(inputtext, "r") || ~strfind(inputtext, "t") || ~strfind(inputtext, "y") ||
			~strfind(inputtext, "u") || ~strfind(inputtext, "i") || ~strfind(inputtext, "o") || ~strfind(inputtext, "p") ||
			~strfind(inputtext, "a") || ~strfind(inputtext, "s") || ~strfind(inputtext, "d") || ~strfind(inputtext, "f") ||
			~strfind(inputtext, "g") || ~strfind(inputtext, "h") || ~strfind(inputtext, "j") || ~strfind(inputtext, "k") ||
			~strfind(inputtext, "l") || ~strfind(inputtext, "z") || ~strfind(inputtext, "x") || ~strfind(inputtext, "c") ||
			~strfind(inputtext, "v") || ~strfind(inputtext, "b") || ~strfind(inputtext, "n") || ~strfind(inputtext, "m") ||
			~strfind(inputtext, "(") || ~strfind(inputtext, ")") || ~strfind(inputtext, ".") || ~strfind(inputtext, "%")) return ShowPlayerDialog(playerid, EDITDB_3_3, DIALOG_STYLE_INPUT,"{ff0000}[!]","{ff0000}Vui long nhap so dien thoai bang so.","Chap nhan","Thoat");

			PlayerInfo[playerid][pSDT3] = sdtdanhba;

			format(string, sizeof(string), "UPDATE `accounts` SET `SDT3`=%d WHERE `id` = %d",number_format(sdtdanhba), GetPlayerSQLId(playerid));
			mysql_function_query(MainPipeline, string, false, "OnQueryFinish", "ii", SENDDATA_THREAD, playerid);

			format(string, sizeof(string), "Ban da them thanh cong so dien thoai %d duoi ten %s.",PlayerInfo[playerid][pSDT3], PlayerInfo[playerid][pDanhBa3]);
			SendClientMessageEx(playerid, COLOR_YELLOW, string);
		}
    }

    //________________________________________________________________________//
	if(dialogid == DANHBA_4)
    {
        if(response)
        {
            if(listitem == 0){ // GOI
				if(PlayerInfo[playerid][pSDT4] != 0){
				    new str[2460], str1[1280], str2[1280], str3[1280], str4[1280], name[1280];
					format(str1, sizeof(str), "THONG TIN CUOC GOI : ");
				    format(str2, sizeof(str), "    -> Ten nguoi nhan : %s", PlayerInfo[playerid][pDanhBa4]);
				    format(str3, sizeof(str), "    -> SDT : %d", PlayerInfo[playerid][pSDT4]);
				    format(str4, sizeof(str), "Neu ban muon goi dien cho nguoi nay, vui long nhan 'Goi ngay' de bat dau cuoc goi");
				    format(str, sizeof(str), "%s\n\n%s\n%s\n%s", str1,str2,str3,str4);
				    format(name, sizeof(name), "DIEN THOAI");
					ShowPlayerDialog(playerid, GOIDIEN_4, DIALOG_STYLE_MSGBOX, name, str, "Goi ngay", "Huy Bo");
					return 1;
				}else ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "DIEN THOAI","Hien tai danh ba nay van dang con trong , ban vui long them nguoi than vao danh ba slot nay de goi dien...","Dong y", "");
            }
            if(listitem == 1){ // NHAN TIN
				if(PlayerInfo[playerid][pSDT4] != 0){
				    ShowPlayerDialog(playerid, NHANTIN_4, DIALOG_STYLE_INPUT, "Nhan tin", "Vui long nhap tin nhan", "Gui", "Huy");
				    return 1;
				}else ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "DIEN THOAI","Hien tai danh ba nay van dang con trong , ban vui long them nguoi than vao danh ba slot nay de nhan tin...","Dong y", "");
            }
            if(listitem == 2){ // EDIT DANH BA
                ShowPlayerDialog(playerid, EDITDB_4, DIALOG_STYLE_INPUT, "Them/Sua so dien thoai", "Vui long dien ten hien thi", "Tiep Tuc", "Tro lai");
                return 1;
			}
			if(listitem == 3){ // XOA DANH BA
	            PlayerInfo[playerid][pSDT4] = 0;
	            format(PlayerInfo[playerid][pDanhBa4], 32, "None");
	            SendClientMessageEx(playerid, COLOR_YELLOW, "Ban da xoa danh ba thanh cong !.");
			}
        }
	}

	if(dialogid == GOIDIEN_4)
    {
        if(response){
            new phonenumb = PlayerInfo[playerid][pSDT4];
            if (PlayerInfo[playerid][pJailTime] > 0)
			{
				SendClientMessageEx(playerid,COLOR_GREY,"Ban khong the su dung dien thoai khi dang o trong tu.");
				return 1;
			}
			if(PlayerTied[playerid] != 0 || PlayerCuffed[playerid] != 0)
			{
				SendClientMessageEx(playerid,COLOR_GREY,"Ban khong the su dung dien thoai.");
				return 1;
			}
			if(PlayerInfo[playerid][pPnumber] == 0)
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong co dien thoai.");
				return 1;
			}
			if(PhoneOnline[playerid] > 0)
			{
				SendClientMessageEx(playerid, COLOR_GREY, "Dien thoai cua ban dang tat.");
				return 1;
			}
			if(GetPVarType(playerid, "PlayerCuffed") || GetPVarType(playerid, "Injured") || GetPVarType(playerid, "IsFrozen")) {
		   		return SendClientMessage(playerid, COLOR_GRAD2, "Ban khong the lam dieu do vao luc nay!");
			}
			format(string, sizeof(string), "* %s lay dien thoai di dong tu tui quan ra.", GetPlayerNameEx(playerid));
			ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
			if(phonenumb == PlayerInfo[playerid][pPnumber])
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "  Dien thoai nguoi do dang ban...");
				return 1;
			}
			if(Mobile[playerid] != INVALID_PLAYER_ID)
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "  Ban dang co mot cuoc goi...");
				return 1;
			}

			foreach(new i: Player)
			{
				if(PlayerInfo[i][pPnumber] == phonenumb && phonenumb != 0)
				{
					new giveplayerid = i;
					Mobile[playerid] = giveplayerid; //caller connecting
					if(IsPlayerConnected(giveplayerid))
					{
						if(giveplayerid != INVALID_PLAYER_ID)
						{
							if(PhoneOnline[giveplayerid] > 0)
							{
								SendClientMessageEx(playerid, COLOR_GREY, "Dien thoai nguoi do dang tat.");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if(Mobile[giveplayerid] != INVALID_PLAYER_ID)
							{
								SendClientMessageEx(playerid, COLOR_GRAD2, "Dien thoai nguoi do dang ban...");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if(Spectating[giveplayerid]!=0)
							{
								SendClientMessageEx(playerid, COLOR_GRAD2, "Dien thoai nguoi do dang ban...");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if (Mobile[giveplayerid] == INVALID_PLAYER_ID)
							{
								format(string, sizeof(string), "Dien thoai di dong cua ban dang vang len - su dung /p de tra loi. [Nguoi goi: %s]", GetPlayerNameEx(playerid));
								PlayerPlaySound(playerid, 23000, 0.0, 0.0, 0.0);
								SendClientMessageEx(giveplayerid, COLOR_YELLOW, string);
								RingTone[giveplayerid] = 10;
								format(string, sizeof(string), "* %s's dien thoai dang do chuong.", GetPlayerNameEx(i));
								SendClientMessageEx(playerid, COLOR_WHITE, "HINT: Bay gio ban co the bam T de tra loi cuoc goi, hoac /h de tat cuoc goi.");
								ProxDetector(30.0, i, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
								new Float:rX, Float:rY, Float:rZ;
								GetPlayerPos(giveplayerid, rX, rY, rZ);
								CellTime[playerid] = 1;
								SetPlayerAttachedObject(playerid, 9, 330, 6);
								return SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);
							}
						}
					}
				}
			}
        }
    }

    if(dialogid == NHANTIN_4)
    {
       if(response)
       {
			new phonenumb = PlayerInfo[playerid][pSDT4];
    	    if(Spectating[playerid] == 0)
			{
				format(string, sizeof(string), "* %s lay dien thoai di dong tu trong tui quan.", GetPlayerNameEx(playerid));
				ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
			}
			if(phonenumb == 555)
			{
				if ((strcmp("yes", inputtext, true, strlen(inputtext)) == 0) && (strlen(inputtext) == strlen("yes")))
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
					Mobile[playerid] = giveplayerid; //caller connecting
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
		       						format(string, sizeof(string), "(BE) %s SMS cho %s: %s", GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid), inputtext);
									SendClientMessageEx(u,COLOR_YELLOW, string);
								}
							}
							if(PlayerInfo[playerid][pPhonePrivacy] == 1)
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: Khong biet.", inputtext, GetPlayerNameEx(playerid));
							}
							else
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: %s (%d)", inputtext, GetPlayerNameEx(playerid), PlayerInfo[playerid][pPnumber]);
							}

							if(i != playerid)
							{
								if(PlayerInfo[i][pSmslog] > 0)
								{
									new query[384], ftext[128];
									mysql_escape_string(inputtext, ftext);
									if(PlayerInfo[playerid][pPhonePrivacy] == 1) format(query, sizeof(query), "INSERT INTO `sms` (`id`, `sender`, `senderid`, `sendernumber`, `receiver`, `receiverid`, `receivernumber`, `message`, `date`) VALUES (NULL, '%s', %d, 0, '%s', %d, %d, '%s', NOW())", GetPlayerNameExt(playerid), GetPlayerSQLId(playerid), GetPlayerNameExt(i), GetPlayerSQLId(i), phonenumb, ftext);
									else format(query, sizeof(query), "INSERT INTO `sms` (`id`, `sender`, `senderid`, `sendernumber`, `receiver`, `receiverid`, `receivernumber`, `message`, `date`) VALUES (NULL, '%s', %d, %d, '%s', %d, %d, '%s', NOW())", GetPlayerNameExt(playerid), GetPlayerSQLId(playerid), PlayerInfo[playerid][pPnumber], GetPlayerNameExt(i), GetPlayerSQLId(i), phonenumb, ftext);
									mysql_function_query(MainPipeline, query, false, "OnQueryFinish", "i", SENDDATA_THREAD);
								}
							}

							RingTone[giveplayerid] =20;
							SendClientMessageEx(giveplayerid, COLOR_YELLOW, string);
							SendClientMessageEx(playerid, COLOR_YELLOW, string);
							SendClientMessageEx(playerid, COLOR_WHITE, "Gui tin nhan van ban");
							Mobile[playerid] = INVALID_PLAYER_ID;

							if(strcmp(PlayerInfo[giveplayerid][pAutoTextReply], "Nothing", true) != 0)
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: %s [automated response] (%d)", PlayerInfo[giveplayerid][pAutoTextReply], GetPlayerNameEx(giveplayerid), PlayerInfo[giveplayerid][pPnumber]);
								SendClientMessageEx(playerid, COLOR_YELLOW, string);
							}

							return 1;
						}
					}
				}
			}
			SendClientMessageEx(playerid, COLOR_GRAD2, "  Gui tin nhan that bai...");
       }
    }

    if(dialogid == EDITDB_4)
    {
        if(response)
		{
			strmid(PlayerInfo[playerid][pDanhBa4], inputtext, 0, strlen(inputtext), 128);
			format(string, sizeof(string), "UPDATE `accounts` SET `DanhBa4`='%s' WHERE `id`=%d", inputtext, GetPlayerSQLId(playerid));
			mysql_function_query(MainPipeline, string, false, "OnQueryFinish", "i", SENDDATA_THREAD);
			ShowPlayerDialog(playerid, EDITDB_4_4, DIALOG_STYLE_INPUT, "Them/Sua so dien thoai", "Vui long dien so dien thoai", "Tiep Tuc", "Tro lai");
        }
	}

	if(dialogid == EDITDB_4_4)
    {
		if(response)
		{
			new sdtdanhba = strval(inputtext);

			if(~strfind(inputtext, ";") || ~strfind(inputtext, ",") || ~strfind(inputtext, "'") || ~strfind(inputtext, ":") ||
			~strfind(inputtext, "|") || ~strfind(inputtext, "{") || ~strfind(inputtext, "}") || ~strfind(inputtext, "+") ||
			~strfind(inputtext, "?") || ~strfind(inputtext, "/")  || ~strfind(inputtext, " ") ||
			~strfind(inputtext, "@") || ~strfind(inputtext, "[") || ~strfind(inputtext, "]") || ~strfind(inputtext, "<") ||
			~strfind(inputtext, ">") || ~strfind(inputtext, "!") || ~strfind(inputtext, "#") || ~strfind(inputtext, "^") ||
			~strfind(inputtext, "*") || ~strfind(inputtext, "&") || ~strfind(inputtext, "q") || ~strfind(inputtext, "w") ||
			~strfind(inputtext, "e") || ~strfind(inputtext, "r") || ~strfind(inputtext, "t") || ~strfind(inputtext, "y") ||
			~strfind(inputtext, "u") || ~strfind(inputtext, "i") || ~strfind(inputtext, "o") || ~strfind(inputtext, "p") ||
			~strfind(inputtext, "a") || ~strfind(inputtext, "s") || ~strfind(inputtext, "d") || ~strfind(inputtext, "f") ||
			~strfind(inputtext, "g") || ~strfind(inputtext, "h") || ~strfind(inputtext, "j") || ~strfind(inputtext, "k") ||
			~strfind(inputtext, "l") || ~strfind(inputtext, "z") || ~strfind(inputtext, "x") || ~strfind(inputtext, "c") ||
			~strfind(inputtext, "v") || ~strfind(inputtext, "b") || ~strfind(inputtext, "n") || ~strfind(inputtext, "m") ||
			~strfind(inputtext, "(") || ~strfind(inputtext, ")") || ~strfind(inputtext, ".") || ~strfind(inputtext, "%")) return ShowPlayerDialog(playerid, EDITDB_4_4, DIALOG_STYLE_INPUT,"{ff0000}[!]","{ff0000}Vui long nhap so dien thoai bang so.","Chap nhan","Thoat");

			PlayerInfo[playerid][pSDT4] = sdtdanhba;

			format(string, sizeof(string), "UPDATE `accounts` SET `SDT4`=%d WHERE `id` = %d",number_format(sdtdanhba), GetPlayerSQLId(playerid));
			mysql_function_query(MainPipeline, string, false, "OnQueryFinish", "ii", SENDDATA_THREAD, playerid);

			format(string, sizeof(string), "Ban da them thanh cong so dien thoai %d duoi ten %s.",PlayerInfo[playerid][pSDT4], PlayerInfo[playerid][pDanhBa4]);
			SendClientMessageEx(playerid, COLOR_YELLOW, string);
		}
    }

    //________________________________________________________________________//
	if(dialogid == DANHBA_5)
    {
        if(response)
        {
            if(listitem == 0){ // GOI
				if(PlayerInfo[playerid][pSDT5] != 0){
				    new str[2460], str1[1280], str2[1280], str3[1280], str4[1280], name[1280];
					format(str1, sizeof(str), "THONG TIN CUOC GOI : ");
				    format(str2, sizeof(str), "    -> Ten nguoi nhan : %s", PlayerInfo[playerid][pDanhBa5]);
				    format(str3, sizeof(str), "    -> SDT : %d", PlayerInfo[playerid][pSDT5]);
				    format(str4, sizeof(str), "Neu ban muon goi dien cho nguoi nay, vui long nhan 'Goi ngay' de bat dau cuoc goi");
				    format(str, sizeof(str), "%s\n\n%s\n%s\n%s", str1,str2,str3,str4);
				    format(name, sizeof(name), "DIEN THOAI");
					ShowPlayerDialog(playerid, GOIDIEN_5, DIALOG_STYLE_MSGBOX, name, str, "Goi ngay", "Huy Bo");
					return 1;
				}else ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "DIEN THOAI","Hien tai danh ba nay van dang con trong , ban vui long them nguoi than vao danh ba slot nay de goi dien...","Dong y", "");
            }
            if(listitem == 1){ // NHAN TIN
				if(PlayerInfo[playerid][pSDT5] != 0){
				    ShowPlayerDialog(playerid, NHANTIN_5, DIALOG_STYLE_INPUT, "Nhan tin", "Vui long nhap tin nhan", "Gui", "Huy");
				    return 1;
				}else ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "DIEN THOAI","Hien tai danh ba nay van dang con trong , ban vui long them nguoi than vao danh ba slot nay de nhan tin...","Dong y", "");
            }
            if(listitem == 2){ // EDIT DANH BA
                ShowPlayerDialog(playerid, EDITDB_5, DIALOG_STYLE_INPUT, "Them/Sua so dien thoai", "Vui long dien ten hien thi", "Tiep Tuc", "Tro lai");
                return 1;
			}
			if(listitem == 3){ // XOA DANH BA
	            PlayerInfo[playerid][pSDT5] = 0;
	            format(PlayerInfo[playerid][pDanhBa5], 32, "None");
	            SendClientMessageEx(playerid, COLOR_YELLOW, "Ban da xoa danh ba thanh cong !.");
			}
        }
	}

	if(dialogid == GOIDIEN_5)
    {
        if(response){
            new phonenumb = PlayerInfo[playerid][pSDT5];
            if (PlayerInfo[playerid][pJailTime] > 0)
			{
				SendClientMessageEx(playerid,COLOR_GREY,"Ban khong the su dung dien thoai khi dang o trong tu.");
				return 1;
			}
			if(PlayerTied[playerid] != 0 || PlayerCuffed[playerid] != 0)
			{
				SendClientMessageEx(playerid,COLOR_GREY,"Ban khong the su dung dien thoai.");
				return 1;
			}
			if(PlayerInfo[playerid][pPnumber] == 0)
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong co dien thoai.");
				return 1;
			}
			if(PhoneOnline[playerid] > 0)
			{
				SendClientMessageEx(playerid, COLOR_GREY, "Dien thoai cua ban dang tat.");
				return 1;
			}
			if(GetPVarType(playerid, "PlayerCuffed") || GetPVarType(playerid, "Injured") || GetPVarType(playerid, "IsFrozen")) {
		   		return SendClientMessage(playerid, COLOR_GRAD2, "Ban khong the lam dieu do vao luc nay!");
			}
			format(string, sizeof(string), "* %s lay dien thoai di dong tu tui quan ra.", GetPlayerNameEx(playerid));
			ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
			if(phonenumb == PlayerInfo[playerid][pPnumber])
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "  Dien thoai nguoi do dang ban...");
				return 1;
			}
			if(Mobile[playerid] != INVALID_PLAYER_ID)
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "  Ban dang co mot cuoc goi...");
				return 1;
			}

			foreach(new i: Player)
			{
				if(PlayerInfo[i][pPnumber] == phonenumb && phonenumb != 0)
				{
					new giveplayerid = i;
					Mobile[playerid] = giveplayerid; //caller connecting
					if(IsPlayerConnected(giveplayerid))
					{
						if(giveplayerid != INVALID_PLAYER_ID)
						{
							if(PhoneOnline[giveplayerid] > 0)
							{
								SendClientMessageEx(playerid, COLOR_GREY, "Dien thoai nguoi do dang tat.");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if(Mobile[giveplayerid] != INVALID_PLAYER_ID)
							{
								SendClientMessageEx(playerid, COLOR_GRAD2, "Dien thoai nguoi do dang ban...");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if(Spectating[giveplayerid]!=0)
							{
								SendClientMessageEx(playerid, COLOR_GRAD2, "Dien thoai nguoi do dang ban...");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if (Mobile[giveplayerid] == INVALID_PLAYER_ID)
							{
								format(string, sizeof(string), "Dien thoai di dong cua ban dang vang len - su dung /p de tra loi. [Nguoi goi: %s]", GetPlayerNameEx(playerid));
								PlayerPlaySound(playerid, 23000, 0.0, 0.0, 0.0);
								SendClientMessageEx(giveplayerid, COLOR_YELLOW, string);
								RingTone[giveplayerid] = 10;
								format(string, sizeof(string), "* %s's dien thoai dang do chuong.", GetPlayerNameEx(i));
								SendClientMessageEx(playerid, COLOR_WHITE, "HINT: Bay gio ban co the bam T de tra loi cuoc goi, hoac /h de tat cuoc goi.");
								ProxDetector(30.0, i, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
								new Float:rX, Float:rY, Float:rZ;
								GetPlayerPos(giveplayerid, rX, rY, rZ);
								CellTime[playerid] = 1;
								SetPlayerAttachedObject(playerid, 9, 330, 6);
								return SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);
							}
						}
					}
				}
			}
        }
    }

    if(dialogid == NHANTIN_5)
    {
       if(response)
       {
			new phonenumb = PlayerInfo[playerid][pSDT5];
    	    if(Spectating[playerid] == 0)
			{
				format(string, sizeof(string), "* %s lay dien thoai di dong tu trong tui quan.", GetPlayerNameEx(playerid));
				ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
			}

			if(phonenumb == 555)
			{
				if ((strcmp("yes", inputtext, true, strlen(inputtext)) == 0) && (strlen(inputtext) == strlen("yes")))
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
					Mobile[playerid] = giveplayerid; //caller connecting
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
		       						format(string, sizeof(string), "(BE) %s SMS cho %s: %s", GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid), inputtext);
									SendClientMessageEx(u,COLOR_YELLOW, string);
								}
							}
							if(PlayerInfo[playerid][pPhonePrivacy] == 1)
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: Khong biet.", inputtext, GetPlayerNameEx(playerid));
							}
							else
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: %s (%d)", inputtext, GetPlayerNameEx(playerid), PlayerInfo[playerid][pPnumber]);
							}

							if(i != playerid)
							{
								if(PlayerInfo[i][pSmslog] > 0)
								{
									new query[384], ftext[128];
									mysql_escape_string(inputtext, ftext);
									if(PlayerInfo[playerid][pPhonePrivacy] == 1) format(query, sizeof(query), "INSERT INTO `sms` (`id`, `sender`, `senderid`, `sendernumber`, `receiver`, `receiverid`, `receivernumber`, `message`, `date`) VALUES (NULL, '%s', %d, 0, '%s', %d, %d, '%s', NOW())", GetPlayerNameExt(playerid), GetPlayerSQLId(playerid), GetPlayerNameExt(i), GetPlayerSQLId(i), phonenumb, ftext);
									else format(query, sizeof(query), "INSERT INTO `sms` (`id`, `sender`, `senderid`, `sendernumber`, `receiver`, `receiverid`, `receivernumber`, `message`, `date`) VALUES (NULL, '%s', %d, %d, '%s', %d, %d, '%s', NOW())", GetPlayerNameExt(playerid), GetPlayerSQLId(playerid), PlayerInfo[playerid][pPnumber], GetPlayerNameExt(i), GetPlayerSQLId(i), phonenumb, ftext);
									mysql_function_query(MainPipeline, query, false, "OnQueryFinish", "i", SENDDATA_THREAD);
								}
							}

							RingTone[giveplayerid] =20;
							SendClientMessageEx(giveplayerid, COLOR_YELLOW, string);
							SendClientMessageEx(playerid, COLOR_YELLOW, string);
							SendClientMessageEx(playerid, COLOR_WHITE, "Gui tin nhan van ban");
							Mobile[playerid] = INVALID_PLAYER_ID;

							if(strcmp(PlayerInfo[giveplayerid][pAutoTextReply], "Nothing", true) != 0)
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: %s [automated response] (%d)", PlayerInfo[giveplayerid][pAutoTextReply], GetPlayerNameEx(giveplayerid), PlayerInfo[giveplayerid][pPnumber]);
								SendClientMessageEx(playerid, COLOR_YELLOW, string);
							}

							return 1;
						}
					}
				}
			}
			SendClientMessageEx(playerid, COLOR_GRAD2, "  Gui tin nhan that bai...");
       }
    }

    if(dialogid == EDITDB_5)
    {
        if(response)
		{
			strmid(PlayerInfo[playerid][pDanhBa5], inputtext, 0, strlen(inputtext), 128);
			format(string, sizeof(string), "UPDATE `accounts` SET `DanhBa5`='%s' WHERE `id`=%d", inputtext, GetPlayerSQLId(playerid));
			mysql_function_query(MainPipeline, string, false, "OnQueryFinish", "i", SENDDATA_THREAD);
			ShowPlayerDialog(playerid, EDITDB_5_5, DIALOG_STYLE_INPUT, "Them/Sua so dien thoai", "Vui long dien so dien thoai", "Tiep Tuc", "Tro lai");
        }
	}

	if(dialogid == EDITDB_5_5)
    {
		if(response)
		{
			new sdtdanhba = strval(inputtext);

			if(~strfind(inputtext, ";") || ~strfind(inputtext, ",") || ~strfind(inputtext, "'") || ~strfind(inputtext, ":") ||
			~strfind(inputtext, "|") || ~strfind(inputtext, "{") || ~strfind(inputtext, "}") || ~strfind(inputtext, "+") ||
			~strfind(inputtext, "?") || ~strfind(inputtext, "/")  || ~strfind(inputtext, " ") ||
			~strfind(inputtext, "@") || ~strfind(inputtext, "[") || ~strfind(inputtext, "]") || ~strfind(inputtext, "<") ||
			~strfind(inputtext, ">") || ~strfind(inputtext, "!") || ~strfind(inputtext, "#") || ~strfind(inputtext, "^") ||
			~strfind(inputtext, "*") || ~strfind(inputtext, "&") || ~strfind(inputtext, "q") || ~strfind(inputtext, "w") ||
			~strfind(inputtext, "e") || ~strfind(inputtext, "r") || ~strfind(inputtext, "t") || ~strfind(inputtext, "y") ||
			~strfind(inputtext, "u") || ~strfind(inputtext, "i") || ~strfind(inputtext, "o") || ~strfind(inputtext, "p") ||
			~strfind(inputtext, "a") || ~strfind(inputtext, "s") || ~strfind(inputtext, "d") || ~strfind(inputtext, "f") ||
			~strfind(inputtext, "g") || ~strfind(inputtext, "h") || ~strfind(inputtext, "j") || ~strfind(inputtext, "k") ||
			~strfind(inputtext, "l") || ~strfind(inputtext, "z") || ~strfind(inputtext, "x") || ~strfind(inputtext, "c") ||
			~strfind(inputtext, "v") || ~strfind(inputtext, "b") || ~strfind(inputtext, "n") || ~strfind(inputtext, "m") ||
			~strfind(inputtext, "(") || ~strfind(inputtext, ")") || ~strfind(inputtext, ".") || ~strfind(inputtext, "%")) return ShowPlayerDialog(playerid, EDITDB_5_5, DIALOG_STYLE_INPUT,"{ff0000}[!]","{ff0000}Vui long nhap so dien thoai bang so.","Chap nhan","Thoat");

			PlayerInfo[playerid][pSDT5] = sdtdanhba;

			format(string, sizeof(string), "UPDATE `accounts` SET `SDT5`=%d WHERE `id` = %d",number_format(sdtdanhba), GetPlayerSQLId(playerid));
			mysql_function_query(MainPipeline, string, false, "OnQueryFinish", "ii", SENDDATA_THREAD, playerid);

			format(string, sizeof(string), "Ban da them thanh cong so dien thoai %d duoi ten %s.",PlayerInfo[playerid][pSDT5], PlayerInfo[playerid][pDanhBa5]);
			SendClientMessageEx(playerid, COLOR_YELLOW, string);
		}
    }

    //________________________________________________________________________//
	if(dialogid == DANHBA_6)
    {
        if(response)
        {
            if(listitem == 0){ // GOI
				if(PlayerInfo[playerid][pSDT6] != 0){
				    new str[2460], str1[1280], str2[1280], str3[1280], str4[1280], name[1280];
					format(str1, sizeof(str), "THONG TIN CUOC GOI : ");
				    format(str2, sizeof(str), "    -> Ten nguoi nhan : %s", PlayerInfo[playerid][pDanhBa6]);
				    format(str3, sizeof(str), "    -> SDT : %d", PlayerInfo[playerid][pSDT6]);
				    format(str4, sizeof(str), "Neu ban muon goi dien cho nguoi nay, vui long nhan 'Goi ngay' de bat dau cuoc goi");
				    format(str, sizeof(str), "%s\n\n%s\n%s\n%s", str1,str2,str3,str4);
				    format(name, sizeof(name), "DIEN THOAI");
					ShowPlayerDialog(playerid, GOIDIEN_6, DIALOG_STYLE_MSGBOX, name, str, "Goi ngay", "Huy Bo");
					return 1;
				}else ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "DIEN THOAI","Hien tai danh ba nay van dang con trong , ban vui long them nguoi than vao danh ba slot nay de goi dien...","Dong y", "");
            }
            if(listitem == 1){ // NHAN TIN
				if(PlayerInfo[playerid][pSDT6] != 0){
				    ShowPlayerDialog(playerid, NHANTIN_6, DIALOG_STYLE_INPUT, "Nhan tin", "Vui long nhap tin nhan", "Gui", "Huy");
				    return 1;
				}else ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "DIEN THOAI","Hien tai danh ba nay van dang con trong , ban vui long them nguoi than vao danh ba slot nay de nhan tin...","Dong y", "");
            }
            if(listitem == 2){ // EDIT DANH BA
                ShowPlayerDialog(playerid, EDITDB_6, DIALOG_STYLE_INPUT, "Them/Sua so dien thoai", "Vui long dien ten hien thi", "Tiep Tuc", "Tro lai");
                return 1;
			}
			if(listitem == 3){ // XOA DANH BA
	            PlayerInfo[playerid][pSDT6] = 0;
	            format(PlayerInfo[playerid][pDanhBa6], 32, "None");
	            SendClientMessageEx(playerid, COLOR_YELLOW, "Ban da xoa danh ba thanh cong !.");
			}
        }
	}

	if(dialogid == GOIDIEN_6)
    {
        if(response){
            new phonenumb = PlayerInfo[playerid][pSDT6];
            if (PlayerInfo[playerid][pJailTime] > 0)
			{
				SendClientMessageEx(playerid,COLOR_GREY,"Ban khong the su dung dien thoai khi dang o trong tu.");
				return 1;
			}
			if(PlayerTied[playerid] != 0 || PlayerCuffed[playerid] != 0)
			{
				SendClientMessageEx(playerid,COLOR_GREY,"Ban khong the su dung dien thoai.");
				return 1;
			}
			if(PlayerInfo[playerid][pPnumber] == 0)
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong co dien thoai.");
				return 1;
			}
			if(PhoneOnline[playerid] > 0)
			{
				SendClientMessageEx(playerid, COLOR_GREY, "Dien thoai cua ban dang tat.");
				return 1;
			}
			if(GetPVarType(playerid, "PlayerCuffed") || GetPVarType(playerid, "Injured") || GetPVarType(playerid, "IsFrozen")) {
		   		return SendClientMessage(playerid, COLOR_GRAD2, "Ban khong the lam dieu do vao luc nay!");
			}
			format(string, sizeof(string), "* %s lay dien thoai di dong tu tui quan ra.", GetPlayerNameEx(playerid));
			ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
			if(phonenumb == PlayerInfo[playerid][pPnumber])
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "  Dien thoai nguoi do dang ban...");
				return 1;
			}
			if(Mobile[playerid] != INVALID_PLAYER_ID)
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "  Ban dang co mot cuoc goi...");
				return 1;
			}

			foreach(new i: Player)
			{
				if(PlayerInfo[i][pPnumber] == phonenumb && phonenumb != 0)
				{
					new giveplayerid = i;
					Mobile[playerid] = giveplayerid; //caller connecting
					if(IsPlayerConnected(giveplayerid))
					{
						if(giveplayerid != INVALID_PLAYER_ID)
						{
							if(PhoneOnline[giveplayerid] > 0)
							{
								SendClientMessageEx(playerid, COLOR_GREY, "Dien thoai nguoi do dang tat.");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if(Mobile[giveplayerid] != INVALID_PLAYER_ID)
							{
								SendClientMessageEx(playerid, COLOR_GRAD2, "Dien thoai nguoi do dang ban...");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if(Spectating[giveplayerid]!=0)
							{
								SendClientMessageEx(playerid, COLOR_GRAD2, "Dien thoai nguoi do dang ban...");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if (Mobile[giveplayerid] == INVALID_PLAYER_ID)
							{
								format(string, sizeof(string), "Dien thoai di dong cua ban dang vang len - su dung /p de tra loi. [Nguoi goi: %s]", GetPlayerNameEx(playerid));
								PlayerPlaySound(playerid, 23000, 0.0, 0.0, 0.0);
								SendClientMessageEx(giveplayerid, COLOR_YELLOW, string);
								RingTone[giveplayerid] = 10;
								format(string, sizeof(string), "* %s's dien thoai dang do chuong.", GetPlayerNameEx(i));
								SendClientMessageEx(playerid, COLOR_WHITE, "HINT: Bay gio ban co the bam T de tra loi cuoc goi, hoac /h de tat cuoc goi.");
								ProxDetector(30.0, i, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
								new Float:rX, Float:rY, Float:rZ;
								GetPlayerPos(giveplayerid, rX, rY, rZ);
								CellTime[playerid] = 1;
								SetPlayerAttachedObject(playerid, 9, 330, 6);
								return SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);
							}
						}
					}
				}
			}
        }
    }

    if(dialogid == NHANTIN_6)
    {
       if(response)
       {
			new phonenumb = PlayerInfo[playerid][pSDT6];
    	    if(Spectating[playerid] == 0)
			{
				format(string, sizeof(string), "* %s lay dien thoai di dong tu trong tui quan.", GetPlayerNameEx(playerid));
				ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
			}

			if(phonenumb == 555)
			{
				if ((strcmp("yes", inputtext, true, strlen(inputtext)) == 0) && (strlen(inputtext) == strlen("yes")))
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
					Mobile[playerid] = giveplayerid; //caller connecting
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
		       						format(string, sizeof(string), "(BE) %s SMS cho %s: %s", GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid), inputtext);
									SendClientMessageEx(u,COLOR_YELLOW, string);
								}
							}
							if(PlayerInfo[playerid][pPhonePrivacy] == 1)
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: Khong biet.", inputtext, GetPlayerNameEx(playerid));
							}
							else
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: %s (%d)", inputtext, GetPlayerNameEx(playerid), PlayerInfo[playerid][pPnumber]);
							}

							if(i != playerid)
							{
								if(PlayerInfo[i][pSmslog] > 0)
								{
									new query[384], ftext[128];
									mysql_escape_string(inputtext, ftext);
									if(PlayerInfo[playerid][pPhonePrivacy] == 1) format(query, sizeof(query), "INSERT INTO `sms` (`id`, `sender`, `senderid`, `sendernumber`, `receiver`, `receiverid`, `receivernumber`, `message`, `date`) VALUES (NULL, '%s', %d, 0, '%s', %d, %d, '%s', NOW())", GetPlayerNameExt(playerid), GetPlayerSQLId(playerid), GetPlayerNameExt(i), GetPlayerSQLId(i), phonenumb, ftext);
									else format(query, sizeof(query), "INSERT INTO `sms` (`id`, `sender`, `senderid`, `sendernumber`, `receiver`, `receiverid`, `receivernumber`, `message`, `date`) VALUES (NULL, '%s', %d, %d, '%s', %d, %d, '%s', NOW())", GetPlayerNameExt(playerid), GetPlayerSQLId(playerid), PlayerInfo[playerid][pPnumber], GetPlayerNameExt(i), GetPlayerSQLId(i), phonenumb, ftext);
									mysql_function_query(MainPipeline, query, false, "OnQueryFinish", "i", SENDDATA_THREAD);
								}
							}

							RingTone[giveplayerid] =20;
							SendClientMessageEx(giveplayerid, COLOR_YELLOW, string);
							SendClientMessageEx(playerid, COLOR_YELLOW, string);
							SendClientMessageEx(playerid, COLOR_WHITE, "Gui tin nhan van ban");
							Mobile[playerid] = INVALID_PLAYER_ID;

							if(strcmp(PlayerInfo[giveplayerid][pAutoTextReply], "Nothing", true) != 0)
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: %s [automated response] (%d)", PlayerInfo[giveplayerid][pAutoTextReply], GetPlayerNameEx(giveplayerid), PlayerInfo[giveplayerid][pPnumber]);
								SendClientMessageEx(playerid, COLOR_YELLOW, string);
							}

							return 1;
						}
					}
				}
			}
			SendClientMessageEx(playerid, COLOR_GRAD2, "  Gui tin nhan that bai...");
       }
    }

    if(dialogid == EDITDB_6)
    {
        if(response)
		{
			strmid(PlayerInfo[playerid][pDanhBa6], inputtext, 0, strlen(inputtext), 128);
			format(string, sizeof(string), "UPDATE `accounts` SET `DanhBa6`='%s' WHERE `id`=%d", inputtext, GetPlayerSQLId(playerid));
			mysql_function_query(MainPipeline, string, false, "OnQueryFinish", "i", SENDDATA_THREAD);
			ShowPlayerDialog(playerid, EDITDB_6_6, DIALOG_STYLE_INPUT, "Them/Sua so dien thoai", "Vui long dien so dien thoai", "Tiep Tuc", "Tro lai");
        }
	}

	if(dialogid == EDITDB_6_6)
    {
		if(response)
		{
			new sdtdanhba = strval(inputtext);

			if(~strfind(inputtext, ";") || ~strfind(inputtext, ",") || ~strfind(inputtext, "'") || ~strfind(inputtext, ":") ||
			~strfind(inputtext, "|") || ~strfind(inputtext, "{") || ~strfind(inputtext, "}") || ~strfind(inputtext, "+") ||
			~strfind(inputtext, "?") || ~strfind(inputtext, "/")  || ~strfind(inputtext, " ") ||
			~strfind(inputtext, "@") || ~strfind(inputtext, "[") || ~strfind(inputtext, "]") || ~strfind(inputtext, "<") ||
			~strfind(inputtext, ">") || ~strfind(inputtext, "!") || ~strfind(inputtext, "#") || ~strfind(inputtext, "^") ||
			~strfind(inputtext, "*") || ~strfind(inputtext, "&") || ~strfind(inputtext, "q") || ~strfind(inputtext, "w") ||
			~strfind(inputtext, "e") || ~strfind(inputtext, "r") || ~strfind(inputtext, "t") || ~strfind(inputtext, "y") ||
			~strfind(inputtext, "u") || ~strfind(inputtext, "i") || ~strfind(inputtext, "o") || ~strfind(inputtext, "p") ||
			~strfind(inputtext, "a") || ~strfind(inputtext, "s") || ~strfind(inputtext, "d") || ~strfind(inputtext, "f") ||
			~strfind(inputtext, "g") || ~strfind(inputtext, "h") || ~strfind(inputtext, "j") || ~strfind(inputtext, "k") ||
			~strfind(inputtext, "l") || ~strfind(inputtext, "z") || ~strfind(inputtext, "x") || ~strfind(inputtext, "c") ||
			~strfind(inputtext, "v") || ~strfind(inputtext, "b") || ~strfind(inputtext, "n") || ~strfind(inputtext, "m") ||
			~strfind(inputtext, "(") || ~strfind(inputtext, ")") || ~strfind(inputtext, ".") || ~strfind(inputtext, "%")) return ShowPlayerDialog(playerid, EDITDB_6_6, DIALOG_STYLE_INPUT,"{ff0000}[!]","{ff0000}Vui long nhap so dien thoai bang so.","Chap nhan","Thoat");

			PlayerInfo[playerid][pSDT6] = sdtdanhba;

			format(string, sizeof(string), "UPDATE `accounts` SET `SDT6`=%d WHERE `id` = %d",number_format(sdtdanhba), GetPlayerSQLId(playerid));
			mysql_function_query(MainPipeline, string, false, "OnQueryFinish", "ii", SENDDATA_THREAD, playerid);

			format(string, sizeof(string), "Ban da them thanh cong so dien thoai %d duoi ten %s.",PlayerInfo[playerid][pSDT6], PlayerInfo[playerid][pDanhBa6]);
			SendClientMessageEx(playerid, COLOR_YELLOW, string);
		}
    }

    //________________________________________________________________________//
	if(dialogid == DANHBA_7)
    {
        if(response)
        {
            if(listitem == 0){ // GOI
				if(PlayerInfo[playerid][pSDT7] != 0){
				    new str[2460], str1[1280], str2[1280], str3[1280], str4[1280], name[1280];
					format(str1, sizeof(str), "THONG TIN CUOC GOI : ");
				    format(str2, sizeof(str), "    -> Ten nguoi nhan : %s", PlayerInfo[playerid][pDanhBa7]);
				    format(str3, sizeof(str), "    -> SDT : %d", PlayerInfo[playerid][pSDT7]);
				    format(str4, sizeof(str), "Neu ban muon goi dien cho nguoi nay, vui long nhan 'Goi ngay' de bat dau cuoc goi");
				    format(str, sizeof(str), "%s\n\n%s\n%s\n%s", str1,str2,str3,str4);
				    format(name, sizeof(name), "DIEN THOAI");
					ShowPlayerDialog(playerid, GOIDIEN_7, DIALOG_STYLE_MSGBOX, name, str, "Goi ngay", "Huy Bo");
					return 1;
				}else ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "DIEN THOAI","Hien tai danh ba nay van dang con trong , ban vui long them nguoi than vao danh ba slot nay de goi dien...","Dong y", "");
            }
            if(listitem == 1){ // NHAN TIN
				if(PlayerInfo[playerid][pSDT7] != 0){
				    ShowPlayerDialog(playerid, NHANTIN_7, DIALOG_STYLE_INPUT, "Nhan tin", "Vui long nhap tin nhan", "Gui", "Huy");
				    return 1;
				}else ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "DIEN THOAI","Hien tai danh ba nay van dang con trong , ban vui long them nguoi than vao danh ba slot nay de nhan tin...","Dong y", "");
            }
            if(listitem == 2){ // EDIT DANH BA
                ShowPlayerDialog(playerid, EDITDB_7, DIALOG_STYLE_INPUT, "Them/Sua so dien thoai", "Vui long dien ten hien thi", "Tiep Tuc", "Tro lai");
                return 1;
			}
			if(listitem == 3){ // XOA DANH BA
	            PlayerInfo[playerid][pSDT7] = 0;
	            format(PlayerInfo[playerid][pDanhBa7], 32, "None");
	            SendClientMessageEx(playerid, COLOR_YELLOW, "Ban da xoa danh ba thanh cong !.");
			}
        }
	}

	if(dialogid == GOIDIEN_7)
    {
        if(response){
            new phonenumb = PlayerInfo[playerid][pSDT7];
            if (PlayerInfo[playerid][pJailTime] > 0)
			{
				SendClientMessageEx(playerid,COLOR_GREY,"Ban khong the su dung dien thoai khi dang o trong tu.");
				return 1;
			}
			if(PlayerTied[playerid] != 0 || PlayerCuffed[playerid] != 0)
			{
				SendClientMessageEx(playerid,COLOR_GREY,"Ban khong the su dung dien thoai.");
				return 1;
			}
			if(PlayerInfo[playerid][pPnumber] == 0)
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong co dien thoai.");
				return 1;
			}
			if(PhoneOnline[playerid] > 0)
			{
				SendClientMessageEx(playerid, COLOR_GREY, "Dien thoai cua ban dang tat.");
				return 1;
			}
			if(GetPVarType(playerid, "PlayerCuffed") || GetPVarType(playerid, "Injured") || GetPVarType(playerid, "IsFrozen")) {
		   		return SendClientMessage(playerid, COLOR_GRAD2, "Ban khong the lam dieu do vao luc nay!");
			}
			format(string, sizeof(string), "* %s lay dien thoai di dong tu tui quan ra.", GetPlayerNameEx(playerid));
			ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
			if(phonenumb == PlayerInfo[playerid][pPnumber])
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "  Dien thoai nguoi do dang ban...");
				return 1;
			}
			if(Mobile[playerid] != INVALID_PLAYER_ID)
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "  Ban dang co mot cuoc goi...");
				return 1;
			}

			foreach(new i: Player)
			{
				if(PlayerInfo[i][pPnumber] == phonenumb && phonenumb != 0)
				{
					new giveplayerid = i;
					Mobile[playerid] = giveplayerid; //caller connecting
					if(IsPlayerConnected(giveplayerid))
					{
						if(giveplayerid != INVALID_PLAYER_ID)
						{
							if(PhoneOnline[giveplayerid] > 0)
							{
								SendClientMessageEx(playerid, COLOR_GREY, "Dien thoai nguoi do dang tat.");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if(Mobile[giveplayerid] != INVALID_PLAYER_ID)
							{
								SendClientMessageEx(playerid, COLOR_GRAD2, "Dien thoai nguoi do dang ban...");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if(Spectating[giveplayerid]!=0)
							{
								SendClientMessageEx(playerid, COLOR_GRAD2, "Dien thoai nguoi do dang ban...");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if (Mobile[giveplayerid] == INVALID_PLAYER_ID)
							{
								format(string, sizeof(string), "Dien thoai di dong cua ban dang vang len - su dung /p de tra loi. [Nguoi goi: %s]", GetPlayerNameEx(playerid));
								PlayerPlaySound(playerid, 23000, 0.0, 0.0, 0.0);
								SendClientMessageEx(giveplayerid, COLOR_YELLOW, string);
								RingTone[giveplayerid] = 10;
								format(string, sizeof(string), "* %s's dien thoai dang do chuong.", GetPlayerNameEx(i));
								SendClientMessageEx(playerid, COLOR_WHITE, "HINT: Bay gio ban co the bam T de tra loi cuoc goi, hoac /h de tat cuoc goi.");
								ProxDetector(30.0, i, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
								new Float:rX, Float:rY, Float:rZ;
								GetPlayerPos(giveplayerid, rX, rY, rZ);
								CellTime[playerid] = 1;
								SetPlayerAttachedObject(playerid, 9, 330, 6);
								return SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);
							}
						}
					}
				}
			}
        }
    }

    if(dialogid == NHANTIN_7)
    {
       if(response)
       {
			new phonenumb = PlayerInfo[playerid][pSDT7];
    	    if(Spectating[playerid] == 0)
			{
				format(string, sizeof(string), "* %s lay dien thoai di dong tu trong tui quan.", GetPlayerNameEx(playerid));
				ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
			}

			if(phonenumb == 555)
			{
				if ((strcmp("yes", inputtext, true, strlen(inputtext)) == 0) && (strlen(inputtext) == strlen("yes")))
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
					Mobile[playerid] = giveplayerid; //caller connecting
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
		       						format(string, sizeof(string), "(BE) %s SMS cho %s: %s", GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid), inputtext);
									SendClientMessageEx(u,COLOR_YELLOW, string);
								}
							}
							if(PlayerInfo[playerid][pPhonePrivacy] == 1)
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: Khong biet.", inputtext, GetPlayerNameEx(playerid));
							}
							else
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: %s (%d)", inputtext, GetPlayerNameEx(playerid), PlayerInfo[playerid][pPnumber]);
							}

							if(i != playerid)
							{
								if(PlayerInfo[i][pSmslog] > 0)
								{
									new query[384], ftext[128];
									mysql_escape_string(inputtext, ftext);
									if(PlayerInfo[playerid][pPhonePrivacy] == 1) format(query, sizeof(query), "INSERT INTO `sms` (`id`, `sender`, `senderid`, `sendernumber`, `receiver`, `receiverid`, `receivernumber`, `message`, `date`) VALUES (NULL, '%s', %d, 0, '%s', %d, %d, '%s', NOW())", GetPlayerNameExt(playerid), GetPlayerSQLId(playerid), GetPlayerNameExt(i), GetPlayerSQLId(i), phonenumb, ftext);
									else format(query, sizeof(query), "INSERT INTO `sms` (`id`, `sender`, `senderid`, `sendernumber`, `receiver`, `receiverid`, `receivernumber`, `message`, `date`) VALUES (NULL, '%s', %d, %d, '%s', %d, %d, '%s', NOW())", GetPlayerNameExt(playerid), GetPlayerSQLId(playerid), PlayerInfo[playerid][pPnumber], GetPlayerNameExt(i), GetPlayerSQLId(i), phonenumb, ftext);
									mysql_function_query(MainPipeline, query, false, "OnQueryFinish", "i", SENDDATA_THREAD);
								}
							}

							RingTone[giveplayerid] =20;
							SendClientMessageEx(giveplayerid, COLOR_YELLOW, string);
							SendClientMessageEx(playerid, COLOR_YELLOW, string);
							SendClientMessageEx(playerid, COLOR_WHITE, "Gui tin nhan van ban");
							Mobile[playerid] = INVALID_PLAYER_ID;

							if(strcmp(PlayerInfo[giveplayerid][pAutoTextReply], "Nothing", true) != 0)
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: %s [automated response] (%d)", PlayerInfo[giveplayerid][pAutoTextReply], GetPlayerNameEx(giveplayerid), PlayerInfo[giveplayerid][pPnumber]);
								SendClientMessageEx(playerid, COLOR_YELLOW, string);
							}

							return 1;
						}
					}
				}
			}
			SendClientMessageEx(playerid, COLOR_GRAD2, "  Gui tin nhan that bai...");
       }
    }

    if(dialogid == EDITDB_7)
    {
        if(response)
		{
			strmid(PlayerInfo[playerid][pDanhBa7], inputtext, 0, strlen(inputtext), 128);
			format(string, sizeof(string), "UPDATE `accounts` SET `DanhBa7`='%s' WHERE `id`=%d", inputtext, GetPlayerSQLId(playerid));
			mysql_function_query(MainPipeline, string, false, "OnQueryFinish", "i", SENDDATA_THREAD);
			ShowPlayerDialog(playerid, EDITDB_7_7, DIALOG_STYLE_INPUT, "Them/Sua so dien thoai", "Vui long dien so dien thoai", "Tiep Tuc", "Tro lai");
        }
	}

	if(dialogid == EDITDB_7_7)
    {
		if(response)
		{
			new sdtdanhba = strval(inputtext);

			if(~strfind(inputtext, ";") || ~strfind(inputtext, ",") || ~strfind(inputtext, "'") || ~strfind(inputtext, ":") ||
			~strfind(inputtext, "|") || ~strfind(inputtext, "{") || ~strfind(inputtext, "}") || ~strfind(inputtext, "+") ||
			~strfind(inputtext, "?") || ~strfind(inputtext, "/")  || ~strfind(inputtext, " ") ||
			~strfind(inputtext, "@") || ~strfind(inputtext, "[") || ~strfind(inputtext, "]") || ~strfind(inputtext, "<") ||
			~strfind(inputtext, ">") || ~strfind(inputtext, "!") || ~strfind(inputtext, "#") || ~strfind(inputtext, "^") ||
			~strfind(inputtext, "*") || ~strfind(inputtext, "&") || ~strfind(inputtext, "q") || ~strfind(inputtext, "w") ||
			~strfind(inputtext, "e") || ~strfind(inputtext, "r") || ~strfind(inputtext, "t") || ~strfind(inputtext, "y") ||
			~strfind(inputtext, "u") || ~strfind(inputtext, "i") || ~strfind(inputtext, "o") || ~strfind(inputtext, "p") ||
			~strfind(inputtext, "a") || ~strfind(inputtext, "s") || ~strfind(inputtext, "d") || ~strfind(inputtext, "f") ||
			~strfind(inputtext, "g") || ~strfind(inputtext, "h") || ~strfind(inputtext, "j") || ~strfind(inputtext, "k") ||
			~strfind(inputtext, "l") || ~strfind(inputtext, "z") || ~strfind(inputtext, "x") || ~strfind(inputtext, "c") ||
			~strfind(inputtext, "v") || ~strfind(inputtext, "b") || ~strfind(inputtext, "n") || ~strfind(inputtext, "m") ||
			~strfind(inputtext, "(") || ~strfind(inputtext, ")") || ~strfind(inputtext, ".") || ~strfind(inputtext, "%")) return ShowPlayerDialog(playerid, EDITDB_7_7, DIALOG_STYLE_INPUT,"{ff0000}[!]","{ff0000}Vui long nhap so dien thoai bang so.","Chap nhan","Thoat");

			PlayerInfo[playerid][pSDT7] = sdtdanhba;

			format(string, sizeof(string), "UPDATE `accounts` SET `SDT7`=%d WHERE `id` = %d",number_format(sdtdanhba), GetPlayerSQLId(playerid));
			mysql_function_query(MainPipeline, string, false, "OnQueryFinish", "ii", SENDDATA_THREAD, playerid);

			format(string, sizeof(string), "Ban da them thanh cong so dien thoai %d duoi ten %s.",PlayerInfo[playerid][pSDT7], PlayerInfo[playerid][pDanhBa7]);
			SendClientMessageEx(playerid, COLOR_YELLOW, string);
		}
    }

    //________________________________________________________________________//
	if(dialogid == DANHBA_8)
    {
        if(response)
        {
            if(listitem == 0){ // GOI
				if(PlayerInfo[playerid][pSDT8] != 0){
				    new str[2460], str1[1280], str2[1280], str3[1280], str4[1280], name[1280];
					format(str1, sizeof(str), "THONG TIN CUOC GOI : ");
				    format(str2, sizeof(str), "    -> Ten nguoi nhan : %s", PlayerInfo[playerid][pDanhBa8]);
				    format(str3, sizeof(str), "    -> SDT : %d", PlayerInfo[playerid][pSDT8]);
				    format(str4, sizeof(str), "Neu ban muon goi dien cho nguoi nay, vui long nhan 'Goi ngay' de bat dau cuoc goi");
				    format(str, sizeof(str), "%s\n\n%s\n%s\n%s", str1,str2,str3,str4);
				    format(name, sizeof(name), "DIEN THOAI");
					ShowPlayerDialog(playerid, GOIDIEN_8, DIALOG_STYLE_MSGBOX, name, str, "Goi ngay", "Huy Bo");
					return 1;
				}else ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "DIEN THOAI","Hien tai danh ba nay van dang con trong , ban vui long them nguoi than vao danh ba slot nay de goi dien...","Dong y", "");
            }
            if(listitem == 1){ // NHAN TIN
				if(PlayerInfo[playerid][pSDT8] != 0){
				    ShowPlayerDialog(playerid, NHANTIN_8, DIALOG_STYLE_INPUT, "Nhan tin", "Vui long nhap tin nhan", "Gui", "Huy");
				    return 1;
				}else ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "DIEN THOAI","Hien tai danh ba nay van dang con trong , ban vui long them nguoi than vao danh ba slot nay de nhan tin...","Dong y", "");
            }
            if(listitem == 2){ // EDIT DANH BA
                ShowPlayerDialog(playerid, EDITDB_8, DIALOG_STYLE_INPUT, "Them/Sua so dien thoai", "Vui long dien ten hien thi", "Tiep Tuc", "Tro lai");
                return 1;
			}
			if(listitem == 3){ // XOA DANH BA
	            PlayerInfo[playerid][pSDT8] = 0;
	            format(PlayerInfo[playerid][pDanhBa8], 32, "None");
	            SendClientMessageEx(playerid, COLOR_YELLOW, "Ban da xoa danh ba thanh cong !.");
			}
        }
	}

	if(dialogid == GOIDIEN_8)
    {
        if(response){
            new phonenumb = PlayerInfo[playerid][pSDT8];
            if (PlayerInfo[playerid][pJailTime] > 0)
			{
				SendClientMessageEx(playerid,COLOR_GREY,"Ban khong the su dung dien thoai khi dang o trong tu.");
				return 1;
			}
			if(PlayerTied[playerid] != 0 || PlayerCuffed[playerid] != 0)
			{
				SendClientMessageEx(playerid,COLOR_GREY,"Ban khong the su dung dien thoai.");
				return 1;
			}
			if(PlayerInfo[playerid][pPnumber] == 0)
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong co dien thoai.");
				return 1;
			}
			if(PhoneOnline[playerid] > 0)
			{
				SendClientMessageEx(playerid, COLOR_GREY, "Dien thoai cua ban dang tat.");
				return 1;
			}
			if(GetPVarType(playerid, "PlayerCuffed") || GetPVarType(playerid, "Injured") || GetPVarType(playerid, "IsFrozen")) {
		   		return SendClientMessage(playerid, COLOR_GRAD2, "Ban khong the lam dieu do vao luc nay!");
			}
			format(string, sizeof(string), "* %s lay dien thoai di dong tu tui quan ra.", GetPlayerNameEx(playerid));
			ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
			if(phonenumb == PlayerInfo[playerid][pPnumber])
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "  Dien thoai nguoi do dang ban...");
				return 1;
			}
			if(Mobile[playerid] != INVALID_PLAYER_ID)
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "  Ban dang co mot cuoc goi...");
				return 1;
			}

			foreach(new i: Player)
			{
				if(PlayerInfo[i][pPnumber] == phonenumb && phonenumb != 0)
				{
					new giveplayerid = i;
					Mobile[playerid] = giveplayerid; //caller connecting
					if(IsPlayerConnected(giveplayerid))
					{
						if(giveplayerid != INVALID_PLAYER_ID)
						{
							if(PhoneOnline[giveplayerid] > 0)
							{
								SendClientMessageEx(playerid, COLOR_GREY, "Dien thoai nguoi do dang tat.");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if(Mobile[giveplayerid] != INVALID_PLAYER_ID)
							{
								SendClientMessageEx(playerid, COLOR_GRAD2, "Dien thoai nguoi do dang ban...");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if(Spectating[giveplayerid]!=0)
							{
								SendClientMessageEx(playerid, COLOR_GRAD2, "Dien thoai nguoi do dang ban...");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if (Mobile[giveplayerid] == INVALID_PLAYER_ID)
							{
								format(string, sizeof(string), "Dien thoai di dong cua ban dang vang len - su dung /p de tra loi. [Nguoi goi: %s]", GetPlayerNameEx(playerid));
								PlayerPlaySound(playerid, 23000, 0.0, 0.0, 0.0);
								SendClientMessageEx(giveplayerid, COLOR_YELLOW, string);
								RingTone[giveplayerid] = 10;
								format(string, sizeof(string), "* %s's dien thoai dang do chuong.", GetPlayerNameEx(i));
								SendClientMessageEx(playerid, COLOR_WHITE, "HINT: Bay gio ban co the bam T de tra loi cuoc goi, hoac /h de tat cuoc goi.");
								ProxDetector(30.0, i, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
								new Float:rX, Float:rY, Float:rZ;
								GetPlayerPos(giveplayerid, rX, rY, rZ);
								CellTime[playerid] = 1;
								SetPlayerAttachedObject(playerid, 9, 330, 6);
								return SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);
							}
						}
					}
				}
			}
        }
    }

    if(dialogid == NHANTIN_8)
    {
       if(response)
       {
			new phonenumb = PlayerInfo[playerid][pSDT8];
    	    if(Spectating[playerid] == 0)
			{
				format(string, sizeof(string), "* %s lay dien thoai di dong tu trong tui quan.", GetPlayerNameEx(playerid));
				ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
			}

			if(phonenumb == 555)
			{
				if ((strcmp("yes", inputtext, true, strlen(inputtext)) == 0) && (strlen(inputtext) == strlen("yes")))
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
					Mobile[playerid] = giveplayerid; //caller connecting
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
		       						format(string, sizeof(string), "(BE) %s SMS cho %s: %s", GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid), inputtext);
									SendClientMessageEx(u,COLOR_YELLOW, string);
								}
							}
							if(PlayerInfo[playerid][pPhonePrivacy] == 1)
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: Khong biet.", inputtext, GetPlayerNameEx(playerid));
							}
							else
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: %s (%d)", inputtext, GetPlayerNameEx(playerid), PlayerInfo[playerid][pPnumber]);
							}

							if(i != playerid)
							{
								if(PlayerInfo[i][pSmslog] > 0)
								{
									new query[384], ftext[128];
									mysql_escape_string(inputtext, ftext);
									if(PlayerInfo[playerid][pPhonePrivacy] == 1) format(query, sizeof(query), "INSERT INTO `sms` (`id`, `sender`, `senderid`, `sendernumber`, `receiver`, `receiverid`, `receivernumber`, `message`, `date`) VALUES (NULL, '%s', %d, 0, '%s', %d, %d, '%s', NOW())", GetPlayerNameExt(playerid), GetPlayerSQLId(playerid), GetPlayerNameExt(i), GetPlayerSQLId(i), phonenumb, ftext);
									else format(query, sizeof(query), "INSERT INTO `sms` (`id`, `sender`, `senderid`, `sendernumber`, `receiver`, `receiverid`, `receivernumber`, `message`, `date`) VALUES (NULL, '%s', %d, %d, '%s', %d, %d, '%s', NOW())", GetPlayerNameExt(playerid), GetPlayerSQLId(playerid), PlayerInfo[playerid][pPnumber], GetPlayerNameExt(i), GetPlayerSQLId(i), phonenumb, ftext);
									mysql_function_query(MainPipeline, query, false, "OnQueryFinish", "i", SENDDATA_THREAD);
								}
							}

							RingTone[giveplayerid] =20;
							SendClientMessageEx(giveplayerid, COLOR_YELLOW, string);
							SendClientMessageEx(playerid, COLOR_YELLOW, string);
							SendClientMessageEx(playerid, COLOR_WHITE, "Gui tin nhan van ban");
							Mobile[playerid] = INVALID_PLAYER_ID;

							if(strcmp(PlayerInfo[giveplayerid][pAutoTextReply], "Nothing", true) != 0)
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: %s [automated response] (%d)", PlayerInfo[giveplayerid][pAutoTextReply], GetPlayerNameEx(giveplayerid), PlayerInfo[giveplayerid][pPnumber]);
								SendClientMessageEx(playerid, COLOR_YELLOW, string);
							}

							return 1;
						}
					}
				}
			}
			SendClientMessageEx(playerid, COLOR_GRAD2, "  Gui tin nhan that bai...");
       }
    }

    if(dialogid == EDITDB_8)
    {
        if(response)
		{
			strmid(PlayerInfo[playerid][pDanhBa8], inputtext, 0, strlen(inputtext), 128);
			format(string, sizeof(string), "UPDATE `accounts` SET `DanhBa8`='%s' WHERE `id`=%d", inputtext, GetPlayerSQLId(playerid));
			mysql_function_query(MainPipeline, string, false, "OnQueryFinish", "i", SENDDATA_THREAD);
			ShowPlayerDialog(playerid, EDITDB_8_8, DIALOG_STYLE_INPUT, "Them/Sua so dien thoai", "Vui long dien so dien thoai", "Tiep Tuc", "Tro lai");
        }
	}

	if(dialogid == EDITDB_8_8)
    {
		if(response)
		{
			new sdtdanhba = strval(inputtext);

			if(~strfind(inputtext, ";") || ~strfind(inputtext, ",") || ~strfind(inputtext, "'") || ~strfind(inputtext, ":") ||
			~strfind(inputtext, "|") || ~strfind(inputtext, "{") || ~strfind(inputtext, "}") || ~strfind(inputtext, "+") ||
			~strfind(inputtext, "?") || ~strfind(inputtext, "/")  || ~strfind(inputtext, " ") ||
			~strfind(inputtext, "@") || ~strfind(inputtext, "[") || ~strfind(inputtext, "]") || ~strfind(inputtext, "<") ||
			~strfind(inputtext, ">") || ~strfind(inputtext, "!") || ~strfind(inputtext, "#") || ~strfind(inputtext, "^") ||
			~strfind(inputtext, "*") || ~strfind(inputtext, "&") || ~strfind(inputtext, "q") || ~strfind(inputtext, "w") ||
			~strfind(inputtext, "e") || ~strfind(inputtext, "r") || ~strfind(inputtext, "t") || ~strfind(inputtext, "y") ||
			~strfind(inputtext, "u") || ~strfind(inputtext, "i") || ~strfind(inputtext, "o") || ~strfind(inputtext, "p") ||
			~strfind(inputtext, "a") || ~strfind(inputtext, "s") || ~strfind(inputtext, "d") || ~strfind(inputtext, "f") ||
			~strfind(inputtext, "g") || ~strfind(inputtext, "h") || ~strfind(inputtext, "j") || ~strfind(inputtext, "k") ||
			~strfind(inputtext, "l") || ~strfind(inputtext, "z") || ~strfind(inputtext, "x") || ~strfind(inputtext, "c") ||
			~strfind(inputtext, "v") || ~strfind(inputtext, "b") || ~strfind(inputtext, "n") || ~strfind(inputtext, "m") ||
			~strfind(inputtext, "(") || ~strfind(inputtext, ")") || ~strfind(inputtext, ".") || ~strfind(inputtext, "%")) return ShowPlayerDialog(playerid, EDITDB_8_8, DIALOG_STYLE_INPUT,"{ff0000}[!]","{ff0000}Vui long nhap so dien thoai bang so.","Chap nhan","Thoat");

			PlayerInfo[playerid][pSDT8] = sdtdanhba;

			format(string, sizeof(string), "UPDATE `accounts` SET `SDT8`=%d WHERE `id` = %d",number_format(sdtdanhba), GetPlayerSQLId(playerid));
			mysql_function_query(MainPipeline, string, false, "OnQueryFinish", "ii", SENDDATA_THREAD, playerid);

			format(string, sizeof(string), "Ban da them thanh cong so dien thoai %d duoi ten %s.",PlayerInfo[playerid][pSDT8], PlayerInfo[playerid][pDanhBa8]);
			SendClientMessageEx(playerid, COLOR_YELLOW, string);
		}
    }

    //________________________________________________________________________//
	if(dialogid == DANHBA_9)
    {
        if(response)
        {
            if(listitem == 0){ // GOI
				if(PlayerInfo[playerid][pSDT9] != 0){
				    new str[2460], str1[1280], str2[1280], str3[1280], str4[1280], name[1280];
					format(str1, sizeof(str), "THONG TIN CUOC GOI : ");
				    format(str2, sizeof(str), "    -> Ten nguoi nhan : %s", PlayerInfo[playerid][pDanhBa9]);
				    format(str3, sizeof(str), "    -> SDT : %d", PlayerInfo[playerid][pSDT9]);
				    format(str4, sizeof(str), "Neu ban muon goi dien cho nguoi nay, vui long nhan 'Goi ngay' de bat dau cuoc goi");
				    format(str, sizeof(str), "%s\n\n%s\n%s\n%s", str1,str2,str3,str4);
				    format(name, sizeof(name), "DIEN THOAI");
					ShowPlayerDialog(playerid, GOIDIEN_9, DIALOG_STYLE_MSGBOX, name, str, "Goi ngay", "Huy Bo");
					return 1;
				}else ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "DIEN THOAI","Hien tai danh ba nay van dang con trong , ban vui long them nguoi than vao danh ba slot nay de goi dien...","Dong y", "");
            }
            if(listitem == 1){ // NHAN TIN
				if(PlayerInfo[playerid][pSDT9] != 0){
				    ShowPlayerDialog(playerid, NHANTIN_9, DIALOG_STYLE_INPUT, "Nhan tin", "Vui long nhap tin nhan", "Gui", "Huy");
				    return 1;
				}else ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "DIEN THOAI","Hien tai danh ba nay van dang con trong , ban vui long them nguoi than vao danh ba slot nay de nhan tin...","Dong y", "");
            }
            if(listitem == 2){ // EDIT DANH BA
                ShowPlayerDialog(playerid, EDITDB_9, DIALOG_STYLE_INPUT, "Them/Sua so dien thoai", "Vui long dien ten hien thi", "Tiep Tuc", "Tro lai");
                return 1;
			}
			if(listitem == 3){ // XOA DANH BA
	            PlayerInfo[playerid][pSDT9] = 0;
	            format(PlayerInfo[playerid][pDanhBa9], 32, "None");
	            SendClientMessageEx(playerid, COLOR_YELLOW, "Ban da xoa danh ba thanh cong !.");
			}
        }
	}

	if(dialogid == GOIDIEN_9)
    {
        if(response){
            new phonenumb = PlayerInfo[playerid][pSDT9];
            if (PlayerInfo[playerid][pJailTime] > 0)
			{
				SendClientMessageEx(playerid,COLOR_GREY,"Ban khong the su dung dien thoai khi dang o trong tu.");
				return 1;
			}
			if(PlayerTied[playerid] != 0 || PlayerCuffed[playerid] != 0)
			{
				SendClientMessageEx(playerid,COLOR_GREY,"Ban khong the su dung dien thoai.");
				return 1;
			}
			if(PlayerInfo[playerid][pPnumber] == 0)
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong co dien thoai.");
				return 1;
			}
			if(PhoneOnline[playerid] > 0)
			{
				SendClientMessageEx(playerid, COLOR_GREY, "Dien thoai cua ban dang tat.");
				return 1;
			}
			if(GetPVarType(playerid, "PlayerCuffed") || GetPVarType(playerid, "Injured") || GetPVarType(playerid, "IsFrozen")) {
		   		return SendClientMessage(playerid, COLOR_GRAD2, "Ban khong the lam dieu do vao luc nay!");
			}
			format(string, sizeof(string), "* %s lay dien thoai di dong tu tui quan ra.", GetPlayerNameEx(playerid));
			ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
			if(phonenumb == PlayerInfo[playerid][pPnumber])
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "  Dien thoai nguoi do dang ban...");
				return 1;
			}
			if(Mobile[playerid] != INVALID_PLAYER_ID)
			{
				SendClientMessageEx(playerid, COLOR_GRAD2, "  Ban dang co mot cuoc goi...");
				return 1;
			}

			foreach(new i: Player)
			{
				if(PlayerInfo[i][pPnumber] == phonenumb && phonenumb != 0)
				{
					new giveplayerid = i;
					Mobile[playerid] = giveplayerid; //caller connecting
					if(IsPlayerConnected(giveplayerid))
					{
						if(giveplayerid != INVALID_PLAYER_ID)
						{
							if(PhoneOnline[giveplayerid] > 0)
							{
								SendClientMessageEx(playerid, COLOR_GREY, "Dien thoai nguoi do dang tat.");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if(Mobile[giveplayerid] != INVALID_PLAYER_ID)
							{
								SendClientMessageEx(playerid, COLOR_GRAD2, "Dien thoai nguoi do dang ban...");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if(Spectating[giveplayerid]!=0)
							{
								SendClientMessageEx(playerid, COLOR_GRAD2, "Dien thoai nguoi do dang ban...");
								Mobile[playerid] = INVALID_PLAYER_ID;
								return 1;
							}
							if (Mobile[giveplayerid] == INVALID_PLAYER_ID)
							{
								format(string, sizeof(string), "Dien thoai di dong cua ban dang vang len - su dung /p de tra loi. [Nguoi goi: %s]", GetPlayerNameEx(playerid));
								PlayerPlaySound(playerid, 23000, 0.0, 0.0, 0.0);
								SendClientMessageEx(giveplayerid, COLOR_YELLOW, string);
								RingTone[giveplayerid] = 10;
								format(string, sizeof(string), "* %s's dien thoai dang do chuong.", GetPlayerNameEx(i));
								SendClientMessageEx(playerid, COLOR_WHITE, "HINT: Bay gio ban co the bam T de tra loi cuoc goi, hoac /h de tat cuoc goi.");
								ProxDetector(30.0, i, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
								new Float:rX, Float:rY, Float:rZ;
								GetPlayerPos(giveplayerid, rX, rY, rZ);
								CellTime[playerid] = 1;
								SetPlayerAttachedObject(playerid, 9, 330, 6);
								return SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);
							}
						}
					}
				}
			}
        }
    }

    if(dialogid == NHANTIN_9)
    {
       if(response)
       {
			new phonenumb = PlayerInfo[playerid][pSDT9];
    	    if(Spectating[playerid] == 0)
			{
				format(string, sizeof(string), "* %s lay dien thoai di dong tu trong tui quan.", GetPlayerNameEx(playerid));
				ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
			}

			if(phonenumb == 555)
			{
				if ((strcmp("yes", inputtext, true, strlen(inputtext)) == 0) && (strlen(inputtext) == strlen("yes")))
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
					Mobile[playerid] = giveplayerid; //caller connecting
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
		       						format(string, sizeof(string), "(BE) %s SMS cho %s: %s", GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid), inputtext);
									SendClientMessageEx(u,COLOR_YELLOW, string);
								}
							}
							if(PlayerInfo[playerid][pPhonePrivacy] == 1)
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: Khong biet.", inputtext, GetPlayerNameEx(playerid));
							}
							else
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: %s (%d)", inputtext, GetPlayerNameEx(playerid), PlayerInfo[playerid][pPnumber]);
							}

							if(i != playerid)
							{
								if(PlayerInfo[i][pSmslog] > 0)
								{
									new query[384], ftext[128];
									mysql_escape_string(inputtext, ftext);
									if(PlayerInfo[playerid][pPhonePrivacy] == 1) format(query, sizeof(query), "INSERT INTO `sms` (`id`, `sender`, `senderid`, `sendernumber`, `receiver`, `receiverid`, `receivernumber`, `message`, `date`) VALUES (NULL, '%s', %d, 0, '%s', %d, %d, '%s', NOW())", GetPlayerNameExt(playerid), GetPlayerSQLId(playerid), GetPlayerNameExt(i), GetPlayerSQLId(i), phonenumb, ftext);
									else format(query, sizeof(query), "INSERT INTO `sms` (`id`, `sender`, `senderid`, `sendernumber`, `receiver`, `receiverid`, `receivernumber`, `message`, `date`) VALUES (NULL, '%s', %d, %d, '%s', %d, %d, '%s', NOW())", GetPlayerNameExt(playerid), GetPlayerSQLId(playerid), PlayerInfo[playerid][pPnumber], GetPlayerNameExt(i), GetPlayerSQLId(i), phonenumb, ftext);
									mysql_function_query(MainPipeline, query, false, "OnQueryFinish", "i", SENDDATA_THREAD);
								}
							}

							RingTone[giveplayerid] =20;
							SendClientMessageEx(giveplayerid, COLOR_YELLOW, string);
							SendClientMessageEx(playerid, COLOR_YELLOW, string);
							SendClientMessageEx(playerid, COLOR_WHITE, "Gui tin nhan van ban");
							Mobile[playerid] = INVALID_PLAYER_ID;

							if(strcmp(PlayerInfo[giveplayerid][pAutoTextReply], "Nothing", true) != 0)
							{
								format(string, sizeof(string), "SMS: %s, Nguoi gui: %s [automated response] (%d)", PlayerInfo[giveplayerid][pAutoTextReply], GetPlayerNameEx(giveplayerid), PlayerInfo[giveplayerid][pPnumber]);
								SendClientMessageEx(playerid, COLOR_YELLOW, string);
							}

							return 1;
						}
					}
				}
			}
			SendClientMessageEx(playerid, COLOR_GRAD2, "  Gui tin nhan that bai...");
       }
    }

    if(dialogid == EDITDB_9)
    {
        if(response)
		{
			strmid(PlayerInfo[playerid][pDanhBa9], inputtext, 0, strlen(inputtext), 128);
			format(string, sizeof(string), "UPDATE `accounts` SET `DanhBa9`='%s' WHERE `id`=%d", inputtext, GetPlayerSQLId(playerid));
			mysql_function_query(MainPipeline, string, false, "OnQueryFinish", "i", SENDDATA_THREAD);
			ShowPlayerDialog(playerid, EDITDB_9_9, DIALOG_STYLE_INPUT, "Them/Sua so dien thoai", "Vui long dien so dien thoai", "Tiep Tuc", "Tro lai");
        }
	}

	if(dialogid == EDITDB_9_9)
    {
		if(response)
		{
			new sdtdanhba = strval(inputtext);

			if(~strfind(inputtext, ";") || ~strfind(inputtext, ",") || ~strfind(inputtext, "'") || ~strfind(inputtext, ":") ||
			~strfind(inputtext, "|") || ~strfind(inputtext, "{") || ~strfind(inputtext, "}") || ~strfind(inputtext, "+") ||
			~strfind(inputtext, "?") || ~strfind(inputtext, "/")  || ~strfind(inputtext, " ") ||
			~strfind(inputtext, "@") || ~strfind(inputtext, "[") || ~strfind(inputtext, "]") || ~strfind(inputtext, "<") ||
			~strfind(inputtext, ">") || ~strfind(inputtext, "!") || ~strfind(inputtext, "#") || ~strfind(inputtext, "^") ||
			~strfind(inputtext, "*") || ~strfind(inputtext, "&") || ~strfind(inputtext, "q") || ~strfind(inputtext, "w") ||
			~strfind(inputtext, "e") || ~strfind(inputtext, "r") || ~strfind(inputtext, "t") || ~strfind(inputtext, "y") ||
			~strfind(inputtext, "u") || ~strfind(inputtext, "i") || ~strfind(inputtext, "o") || ~strfind(inputtext, "p") ||
			~strfind(inputtext, "a") || ~strfind(inputtext, "s") || ~strfind(inputtext, "d") || ~strfind(inputtext, "f") ||
			~strfind(inputtext, "g") || ~strfind(inputtext, "h") || ~strfind(inputtext, "j") || ~strfind(inputtext, "k") ||
			~strfind(inputtext, "l") || ~strfind(inputtext, "z") || ~strfind(inputtext, "x") || ~strfind(inputtext, "c") ||
			~strfind(inputtext, "v") || ~strfind(inputtext, "b") || ~strfind(inputtext, "n") || ~strfind(inputtext, "m") ||
			~strfind(inputtext, "(") || ~strfind(inputtext, ")") || ~strfind(inputtext, ".") || ~strfind(inputtext, "%")) return ShowPlayerDialog(playerid, EDITDB_9_9, DIALOG_STYLE_INPUT,"{ff0000}[!]","{ff0000}Vui long nhap so dien thoai bang so.","Chap nhan","Thoat");

			PlayerInfo[playerid][pSDT9] = sdtdanhba;

			format(string, sizeof(string), "UPDATE `accounts` SET `SDT9`=%d WHERE `id` = %d",number_format(sdtdanhba), GetPlayerSQLId(playerid));
			mysql_function_query(MainPipeline, string, false, "OnQueryFinish", "ii", SENDDATA_THREAD, playerid);

			format(string, sizeof(string), "Ban da them thanh cong so dien thoai %d duoi ten %s.",PlayerInfo[playerid][pSDT9], PlayerInfo[playerid][pDanhBa9]);
			SendClientMessageEx(playerid, COLOR_YELLOW, string);
		}
    }
	return 1;
}