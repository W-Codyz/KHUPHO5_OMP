#include <YSI\YSI_Coding\y_hooks>


#define 		DIALOG_BANK					   (5894)
#define 		DIALOG_RUTTIEN				   (5895)
#define 		DIALOG_GUITIEN				   (5896)
#define 		DIALOG_CHUYENTIENBANK		       (5897)
#define 		DIALOG_CHUYENTIENBANK2			   (5898)
#define         DIALOG_BANK_HISTORY_MENU    (5899)
#define         DIALOG_BANK_HISTORY_VIEW    (5900)
#define         MAX_BANK_LOG_ENTRIES        50

stock GetBankTimestamp(output[], size)
{
    new year, month, day, hour, minute, second;
    getdate(year, month, day);
    gettime(hour, minute, second);
    format(output, size, "[%02d/%02d/%04d %02d:%02d:%02d]", day, month, year, hour, minute, second);
}

stock BankLogTransaction(sqlid, const line[])
{
    new filename[64];
    format(filename, sizeof(filename), "log_bank/%d_bank.txt", sqlid);
    new File:f = fopen(filename, io_append);
    if(f == File:0) return;
    new fullline[512];
    format(fullline, sizeof(fullline), "%s\r\n", line);
    fwrite(f, fullline);
    fclose(f);
}

stock ShowBankHistory(playerid)
{
    new sqlid = GetPlayerSQLId(playerid);
    new filename[64];
    format(filename, sizeof(filename), "log_bank/%d_bank.txt", sqlid);
    new string[4096];
    string[0] = EOS;
    if(!fexist(filename)) {
        strcat(string, "{AAAAAA}Chua co giao dich nao.", sizeof(string));
    } else {
        static lines[MAX_BANK_LOG_ENTRIES][256];
        static line[256];
        new total = 0;
        new File:f = fopen(filename, io_read);
        if(f == File:0) {
            strcat(string, "{AAAAAA}Khong the mo file lich su.", sizeof(string));
        } else {
            while(fread(f, line)) {
                strcopy(lines[total % MAX_BANK_LOG_ENTRIES], line, 256);
                total++;
            }
            fclose(f);
            new count = (total > MAX_BANK_LOG_ENTRIES) ? MAX_BANK_LOG_ENTRIES : total;
            new start = (total > MAX_BANK_LOG_ENTRIES) ? (total % MAX_BANK_LOG_ENTRIES) : 0;
            for(new i = 0; i < count; i++) {
                new idx = (start + i) % MAX_BANK_LOG_ENTRIES;
                strcat(string, lines[idx], sizeof(string));
            }
        }
    }
    ShowPlayerDialog(playerid, DIALOG_BANK_HISTORY_VIEW, DIALOG_STYLE_MSGBOX,
        "________________LICH SU NGAN HANG________________",
        string, "Dong", "Quay lai");
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
	if(PRESSED(KEY_YES))
		if(IsPlayerInRangeOfPoint(playerid, 3.0, 1376.550537, -28.821121, 1000.872924)) return ShowBank(playerid);
	return 1;
}

CMD:bank(playerid, params[]) {
    if(IsPlayerInRangeOfPoint(playerid, 3.0, 1376.550537, -28.821121, 1000.872924)) return ShowBank(playerid);
	return 1;
}

stock ShowBank(playerid) {
    if(PlayerInfo[playerid][pSoTaiKhoan] > 0)
    {
		new str[2460], str5[1280], str6[1280], str7[1280], str8[1280], name[1280];
	    format(str5, sizeof(str), "{00FF7F}[+] {ffffff}Gui tien vao ngan hang");
	    format(str6, sizeof(str), "{FF6B6B}[-] {ffffff}Rut tien tu ngan hang");
	    format(str7, sizeof(str), "{00CFFF}[>] {ffffff}Chuyen khoan");
	    format(str8, sizeof(str), "{87CEEB}[?] {ffffff}Lich su giao dich");
	    format(str, sizeof(str), "%s\n%s\n%s\n%s", str5,str6,str7,str8);
	    format(name, sizeof(name), "{FFD700}NGAN HANG");
		ShowPlayerDialog(playerid, DIALOG_BANK, DIALOG_STYLE_LIST, name, str, "Chon", "");
	}else SendClientMessageEx(playerid, COLOR_YELLOW, "{FF6B6B}[!]{FFFFFF} Ban chua co tai khoan ngan hang. Su dung {FFFF00}/dangkybank{FFFFFF} de dang ky.");
	return 1;
}

CMD:dangkybank(playerid, params[])
{
	if(PlayerInfo[playerid][pSoTaiKhoan] == 0)
	{
	    if(IsPlayerInRangeOfPoint(playerid, 3.0, 1375.455322, -21.559141, 1000.872924))
	    {
	        if(PlayerInfo[playerid][pCash] >= 5000)
	        {
			    new string[1280], str[1280];
			    new rand = Random(100000000, 999999999);
			    PlayerInfo[playerid][pSoTaiKhoan] = rand;
				PlayerInfo[playerid][pCash] -= 5000;

			    format(string, sizeof(string), "{C2A2DA}%s da dang ky tao mot the ngan hang", GetPlayerNameEx(playerid));
				ProxDetectorWrap(playerid, string, 92, 30.0, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

				format(str, sizeof(str), "{00FF7F}[+]{FFFFFF} Dang ky thanh cong! So tai khoan: {00CFFF}%d", GetPlayerSQLId(playerid));
				SendClientMessageEx(playerid ,COLOR_WHITE, str);

				SendClientMessage(playerid, -1, "{FFD700}[BANK]{FFFFFF} Phi mo tai khoan: {FF6B6B}$5,000{FFFFFF}.");
				//UpdateMoTaiKhoanMission(playerid);

			}else SendClientMessage(playerid, -1, "{FF6B6B}[!]{FFFFFF} Ban can co it nhat {FFFF00}$5,000{FFFFFF} de mo tai khoan ngan hang.");
		}else SendClientMessage(playerid, -1, "{FF6B6B}[!]{FFFFFF} Ban khong o gan quay dang ky ngan hang.");

	}else SendClientMessage(playerid, -1, "{FF6B6B}[!]{FFFFFF} Ban da co tai khoan ngan hang roi.");
	return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) {
    if(dialogid == DIALOG_BANK)
    {
		if(response)
	    {
	        if(listitem == 0)
	        {
	            new str[2460], str1[1280], str2[1280], str3[1280], str4[1280], str5[1280], name[1280];
				format(str1, sizeof(str), "{FFD700}________NGAN HANG - GUI TIEN________{FFFFFF}");
			    format(str2, sizeof(str), "{AAAAAA}   So tai khoan : {00CFFF}%d", GetPlayerSQLId(playerid));
			    format(str3, sizeof(str), "{AAAAAA}   So du : {00FF7F}$%s  {AAAAAA}| Tien mat : {00FF7F}$%s", number_format(PlayerInfo[playerid][pAccount]), number_format(PlayerInfo[playerid][pCash]));
			    format(str4, sizeof(str), "{FFD700}___________________________________{ffffff}");
			    format(str5, sizeof(str), "{FFFFFF}Nhap so tien ban muon {00FF7F}gui vao ngan hang{FFFFFF}:");
			    format(str, sizeof(str), "%s\n%s\n%s\n\n%s\n\n%s", str1,str2,str3,str4,str5);
			    format(name, sizeof(name), "{FFD700}GUI TIEN");
				ShowPlayerDialog(playerid, DIALOG_GUITIEN, DIALOG_STYLE_INPUT, name, str, "Xac nhan", "Huy");
	        }
			if(listitem == 1)
   			{
	            new str[2460], str1[1280], str2[1280], str3[1280], str4[1280], str5[1280], name[1280];
				format(str1, sizeof(str), "{FFD700}________NGAN HANG - RUT TIEN________{FFFFFF}");
			    format(str2, sizeof(str), "{AAAAAA}   So tai khoan : {00CFFF}%d", GetPlayerSQLId(playerid));
			    format(str3, sizeof(str), "{AAAAAA}   So du : {00FF7F}$%s  {AAAAAA}| Tien mat : {00FF7F}$%s", number_format(PlayerInfo[playerid][pAccount]), number_format(PlayerInfo[playerid][pCash]));
			    format(str4, sizeof(str), "{FFD700}___________________________________{ffffff}");
			    format(str5, sizeof(str), "{FFFFFF}Nhap so tien ban muon {FF6B6B}rut tu ngan hang{FFFFFF}:");
			    format(str, sizeof(str), "%s\n%s\n%s\n\n%s\n\n%s", str1,str2,str3,str4,str5);
			    format(name, sizeof(name), "{FFD700}RUT TIEN");
				ShowPlayerDialog(playerid, DIALOG_RUTTIEN, DIALOG_STYLE_INPUT, name, str, "Xac nhan", "Huy");
			}
			if(listitem == 2)
			{
			    /*new str[2460], str1[1280], str2[1280], str3[1280], str4[1280], str5[1280], name[1280];
				format(str1, sizeof(str), "{b32d00}HE THONG NGAN HANG - THONG TIN{FFFFFF}", GetPlayerNameEx(playerid));
			    format(str2, sizeof(str), "   + SO TAI KHOAN NGAN HANG : %d", PlayerInfo[playerid][pSoTaiKhoan]);
			    format(str3, sizeof(str), "   + SO DU : %d", PlayerInfo[playerid][pAccount]);
			    format(str4, sizeof(str), "{b32d00}NGAN HANG - GIAO DICH{ffffff}.");
			    format(str5, sizeof(str), "Nhap nguoi ban can chuyen khoan : ");
			    format(str, sizeof(str), "%s\n%s\n%s\n%s\n%s", str1,str2,str3,str4,str5);
			    format(name, sizeof(name), "BANK");
				ShowPlayerDialog(playerid, DIALOG_CHUYENTIENBANK, DIALOG_STYLE_INPUT, name, str, "Ok", "");*/
				ShowChuyenTienHihi(playerid);
			}
			if(listitem == 3)
			{
				ShowBankHistory(playerid);
			}
		}
	}
	if(dialogid == DIALOG_CHUYENTIENBANK)
	{
		if(response)
        {
            /*new giveplayerid = strval(inputtext);
            if(!IsPlayerConnected(giveplayerid)) return SendClientMessage(playerid, COLOR_RED, "Loi: ID nguoi choi khong hop le!");
            SetPVarInt(playerid, "PlayerSelected", giveplayerid);
            ShowPlayerDialog(playerid, DIALOG_CHUYENTIENBANK2, DIALOG_STYLE_INPUT, "Chuyen khoan", "Nhap so tien ban muon chuyen", "Dong y", "Huy bo");*/
            ShowChuyenTienHihi(playerid);
        }
	}
	if(dialogid == DIALOG_CHUYENTIENBANK2)
    {
        if(response)
        {
            new amount = strval(inputtext);
            new giveplayerid = GetPVarInt(playerid, "PlayerSelected");
            new playermoney = PlayerInfo[playerid][pAccount] ;

            if (amount < 1 && playermoney < amount) return SendClientMessageEx(playerid, COLOR_GRAD1, "Vui long nhap dung , so tien khong duoc duoi 1$.");
			if(amount < 0 || amount > 1000000000) {
			    SendClientMessageEx(playerid, COLOR_GRAD1, "Vui long nhap dung , so tien.");
				return 1;
			}
			if(PlayerInfo[playerid][pAccount] < amount) {
			    SendClientMessageEx(playerid, COLOR_GRAD1, "Vui long nhap dung , so tien.");
				return 1;
			}
			GivePlayerCashEx(playerid, TYPE_BANK, -amount);
            GivePlayerCashEx(giveplayerid, TYPE_BANK, amount);

            PlayerInfo[playerid][pAccount] -= amount;
            PlayerInfo[giveplayerid][pAccount] += amount;

			new string[1280];
            format(string, sizeof(string), "Ban da chuyen $%s cho %s", number_format(amount), GetPlayerNameEx(giveplayerid),giveplayerid);
            PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
            SendClientMessageEx(playerid, COLOR_GRAD1, string);
            format(string, sizeof(string), "Ban dan nhan duoc $%s vao tai khoan cua ban %s.", number_format(amount), GetPlayerNameEx(playerid), playerid);
            SendClientMessageEx(giveplayerid, COLOR_GRAD1, string);
            new ip[32], ipex[32];
            GetPlayerIp(playerid, ip, sizeof(ip));
            GetPlayerIp(giveplayerid, ipex, sizeof(ipex));
            format(string, sizeof(string), "[NGAN HANG] %s (IP:%s) da chuyen $%s den %s (IP:%s).", GetPlayerNameEx(playerid), ip, number_format(amount), GetPlayerNameEx(giveplayerid), ipex);

            if(amount >= 500000)
            {
                ABroadCast(COLOR_YELLOW,string,2);
            }
            Log("logs/bank.log", string);
            PlayerPlaySound(giveplayerid, 1052, 0.0, 0.0, 0.0);
            SetPVarInt(playerid, "LastTransaction", gettime());

            new DCC_Channel:channel = DCC_FindChannelById("1548216250415456376");
            new nofi[520], title[128];
            format(title, sizeof(title), "**%s (ID: %d) chuyển khoản cho %s (ID: %d)**", GetPlayerNameEx(playerid), playerid, GetPlayerNameEx(giveplayerid), giveplayerid);
            format(nofi, sizeof(nofi), ":bank: **Số tiền chuyển: $%s** | Tài khoản người gửi còn: $%s", number_format(amount), number_format(PlayerInfo[playerid][pAccount]));
            DiscordLog(channel, title, nofi);
            return 1;
        }
    }
	if(dialogid == DIALOG_RUTTIEN && response == 1)
	{
		if(strfind(inputtext, "%", true) != -1) {
			return ShowPlayerDialog(playerid, DIALOG_RUTTIEN, DIALOG_STYLE_INPUT, "Rut Tien","Nhap so tien ban muon rut.","Dong y","Huy");
		}
		if(strfind(inputtext, "c", true) != -1) {
			return ShowPlayerDialog(playerid, DIALOG_RUTTIEN, DIALOG_STYLE_INPUT, "Rut Tien","Nhap so tien ban muon rut.","Dong y","Huy");
		}
		new amount = strval(inputtext);
		if (amount > PlayerInfo[playerid][pAccount] || amount < 1)
		{
			SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong co du tien!");
			return 1;
		}
		if(amount < 0 || amount > 1000000000) {
		    SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong co du tien!");
			return 1;
		}
		else {
			if(gettime()-GetPVarInt(playerid, "LastTransaction") < 10) return SendClientMessageEx(playerid, COLOR_GRAD2, "Vui long cho 10 giay moi co the giao dich lai!");
			SetPVarInt(playerid, "LastTransaction", gettime());
			PlayerInfo[playerid][pAccount]=PlayerInfo[playerid][pAccount]-amount;
			GivePlayerCash(playerid,amount);
			new string[1280];
			format(string, sizeof(string), "{FF6B6B}[-]{FFFFFF} Rut {FF6B6B}$%s{FFFFFF} | So du con lai: {00FF7F}$%s", number_format(amount), number_format(PlayerInfo[playerid][pAccount]));
			SendClientMessageEx(playerid, COLOR_YELLOW, string);
			OnPlayerStatsUpdate(playerid);

			new ts[32], logline[256];
			GetBankTimestamp(ts, sizeof(ts));
			format(logline, sizeof(logline), "{ffffff}%s {FF6B6B}[-] RUT{FFFFFF} {FF6B6B}$%s{FFFFFF} | Du: {00FF7F}$%s",
				ts, number_format(amount), number_format(PlayerInfo[playerid][pAccount]));
			BankLogTransaction(GetPlayerSQLId(playerid), logline);

			new ip[32];
			GetPlayerIp(playerid, ip, sizeof(ip));
			format(string, sizeof(string), "[NGAN HANG - RUT TIEN] %s (IP:%s) da rut $%s. So du con: $%s", GetPlayerNameEx(playerid), ip, number_format(amount), number_format(PlayerInfo[playerid][pAccount]));
			Log("logs/bank.log", string);

			new DCC_Channel:channel = DCC_FindChannelById("1548216250415456376");
			new nofi[520], title[128];
			format(title, sizeof(title), "**%s (ID: %d) rút tiền ngân hàng**", GetPlayerNameEx(playerid), playerid);
			format(nofi, sizeof(nofi), ":arrow_down: **Số tiền rút: $%s** | Số dư còn lại: $%s", number_format(amount), number_format(PlayerInfo[playerid][pAccount]));
			DiscordLog(channel, title, nofi);
		}
	}
	if(dialogid == DIALOG_GUITIEN && response == 1)
	{
		if(strfind(inputtext, "%", true) != -1) {
			return ShowPlayerDialog(playerid, DIALOG_GUITIEN, DIALOG_STYLE_INPUT, "Gui Tien","Nhap so tien ban muon gui.","Dong y","Huy");
		}
		if(strfind(inputtext, "c", true) != -1) {
			return ShowPlayerDialog(playerid, DIALOG_GUITIEN, DIALOG_STYLE_INPUT, "Gui Tien","Nhap so tien ban muon gui.","Dong y","Huy");
		}
		new amount = strval(inputtext);
		if (amount > GetPlayerCash(playerid) || amount < 1)
		{
			SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong co du tien.");
			return 1;
		}
		if(amount < 0 || amount > 1000000000) {
		    SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong co du tien!");
			return 1;
		}
		else {
			if(gettime()-GetPVarInt(playerid, "LastTransaction") < 10) return SendClientMessageEx(playerid, COLOR_GRAD2, "Vui long cho 10 giay moi co the giao dich lai!");
			SetPVarInt(playerid, "LastTransaction", gettime());

			PlayerInfo[playerid][pCash] -= amount;
 			new string[1280];
			PlayerInfo[playerid][pAccount]=amount+PlayerInfo[playerid][pAccount];
		    SendClientMessageEx(playerid, COLOR_GREEN,"_______________________________________");
			format(string, sizeof(string), "{00FF7F}[+]{FFFFFF} Gui {00FF7F}$%s{FFFFFF} | So du moi: {00FF7F}$%s", number_format(amount), number_format(PlayerInfo[playerid][pAccount]));
			SendClientMessageEx(playerid, COLOR_GRAD4, string);
		    SendClientMessageEx(playerid, COLOR_GREEN,"_______________________________________");
			OnPlayerStatsUpdate(playerid);

			new ts[32], logline[256];
			GetBankTimestamp(ts, sizeof(ts));
			format(logline, sizeof(logline), "{ffffff}%s {00FF7F}[+] GUI{FFFFFF} {00FF7F}$%s{FFFFFF} | Du: {00FF7F}$%s",
				ts, number_format(amount), number_format(PlayerInfo[playerid][pAccount]));
			BankLogTransaction(GetPlayerSQLId(playerid), logline);

			new ip[32];
			GetPlayerIp(playerid, ip, sizeof(ip));
			format(string, sizeof(string), "[NGAN HANG - GUI TIEN] %s (IP:%s) da gui $%s. So du moi: $%s", GetPlayerNameEx(playerid), ip, number_format(amount), number_format(PlayerInfo[playerid][pAccount]));
			Log("logs/bank.log", string);

			new DCC_Channel:channel = DCC_FindChannelById("1548216250415456376");
			new nofi[520], title[128];
			format(title, sizeof(title), "**%s (ID: %d) gửi tiền vào ngân hàng**", GetPlayerNameEx(playerid), playerid);
			format(nofi, sizeof(nofi), ":arrow_up: **Số tiền gửi: $%s** | Số dư mới: $%s", number_format(amount), number_format(PlayerInfo[playerid][pAccount]));
			DiscordLog(channel, title, nofi);
		}
	}
	if(dialogid == DIALOG_BANK_HISTORY_VIEW)
	{
		if(!response)
			ShowBank(playerid);
	}
	return 1;
}
