PayDay(i) {
	new
		string[128],
		interest,
		year,
		month,
		day;
		
	getdate(year, month, day);

 	if(PlayerInfo[i][pLevel] > 0) {
		if(GetPVarType(i, "debtMsg")) {
			if(GetPlayerCash(i) < 0 && PlayerInfo[i][pJailTime] < 1 && !IsACop(i) && PlayerInfo[i][pWantedLevel] < 6) {
				SendClientMessageEx(i, COLOR_LIGHTRED, "Ban no $%s - hay tim cach tra lai tien hoac ban se gap rac roi!", number_format(GetPlayerCash(i)));
			}
			else DeletePVar(i, "debtMsg");
		}

		if(0 <= PlayerInfo[i][pRenting] < sizeof HouseInfo) {
			if(HouseInfo[PlayerInfo[i][pRenting]][hRentFee] > PlayerInfo[i][pAccount]) {
				PlayerInfo[i][pRenting] = INVALID_HOUSE_ID;
				SendClientMessageEx(i, COLOR_WHITE, "Ban bi duoi, ban khong co du tien de nop phi thue nha.");
			}
			else {
				HouseInfo[PlayerInfo[i][pRenting]][hSafeMoney] += HouseInfo[PlayerInfo[i][pRenting]][hRentFee];
				PlayerInfo[i][pAccount] -= HouseInfo[PlayerInfo[i][pRenting]][hRentFee];
			}
		}
		if(PlayerInfo[i][pConnectSeconds] >= 3600) {
			if(GetPVarType(i, "AdvisorDuty")) {
				PlayerInfo[i][pDutyHours]++;
			}
			if(SpecTimer) AddSpecialToken(i);
			SendClientMessageEx(i, 0x16FC6FFF, "______________ THONG TIN NGAN HANG ______________");
			if(PlayerInfo[i][pNation] == 0)
			{
				SendClientMessageEx(i, COLOR_WHITE, "  Tien luong: $%s  |  Thue chinh phu: $%s (%d%%)", number_format(PlayerInfo[i][pPayCheck]), number_format((PlayerInfo[i][pPayCheck] / 100) * TaxValue), TaxValue);
				PlayerInfo[i][pAccount] -= (PlayerInfo[i][pPayCheck] / 100) * TaxValue;
				Tax += (PlayerInfo[i][pPayCheck] / 100) * TaxValue;
			}
			else if(PlayerInfo[i][pNation] == 1)
			{
				
				SendClientMessageEx(i , COLOR_WHITE, "  Tien luong: $%s  |  Thue chinh phu: $%s (%d%%)", number_format(PlayerInfo[i][pPayCheck]), number_format((PlayerInfo[i][pPayCheck] / 100) * TRTaxValue), TRTaxValue);
				PlayerInfo[i][pAccount] -= (PlayerInfo[i][pPayCheck] / 100) * TRTaxValue;
				TRTax += (PlayerInfo[i][pPayCheck] / 100) * TRTaxValue;				
			}

			interest = (PlayerInfo[i][pAccount] + 1) / 1000;

			switch(PlayerInfo[i][pDonateRank]) {
				case 0: {
					if(interest > 50000) interest = 50000;
					SendClientMessageEx(i, COLOR_WHITE, "  So du con lai: $%s  |  Lai suat: 0.1%% (Toi da 50k)", number_format(PlayerInfo[i][pAccount]));
				}
				case 1: {
					if(interest > 100000) interest = 100000;
					SendClientMessageEx(i, COLOR_WHITE, "  So du con lai: $%s  |  Lai suat: 0.1%% {FFFF00}(Bronze VIP: Toi da 100k)", number_format(PlayerInfo[i][pAccount]));
				}
				case 2:	{
					if(interest > 150000) interest = 150000;
					SendClientMessageEx(i, COLOR_WHITE, "  So du con lai: $%s  |  Lai suat: 0.1%% {FFFF00}(Silver VIP: Toi da 150k)", number_format(PlayerInfo[i][pAccount]));
				}
				case 3: {
					if(interest > 200000) interest = 200000;
					SendClientMessageEx(i, COLOR_WHITE, "  So du con lai: $%s  |  Lai suat: 0.1%% {FFFF00}(Gold VIP: Toi da 200k)", number_format(PlayerInfo[i][pAccount]));
				}
				case 4, 5: {
					if(interest > 250000) interest = 250000;
					SendClientMessageEx(i, COLOR_WHITE, "  So du con lai: $%s  |  Lai suat: 0.1%% {FFFF00}(Platinum VIP: Toi da 250k)", number_format(PlayerInfo[i][pAccount]));
				}
			}
			if(PlayerInfo[i][pTaxiLicense] == 1) {
				PlayerInfo[i][pAccount] -= (PlayerInfo[i][pPayCheck] / 100) * 5;
				Tax += (PlayerInfo[i][pPayCheck] / 100) * 5;
				SendClientMessageEx(i, 0xFFFC40FD, "  Phi giay phep Taxi (5%%): $%s", number_format((PlayerInfo[i][pPayCheck] / 100) * 5));
			}
			for(new iGroupID; iGroupID < MAX_GROUPS; iGroupID++)
			{
				if(PlayerInfo[i][pNation] == 0)
				{
					if(arrGroupData[iGroupID][g_iAllegiance] == 1)
					{
						if(arrGroupData[iGroupID][g_iGroupType] == 5)
						{
							new str[128], file[32];
							format(str, sizeof(str), "%s da tra $%s tien thue.", GetPlayerNameEx(i), number_format((PlayerInfo[i][pPayCheck] / 100) * TaxValue));
							format(file, sizeof(file), "grouppay/%d/%d-%d-%d.log", iGroupID, month, day, year);
							Log(file, str);
						}
					}
				}
				else if (PlayerInfo[i][pNation] == 1)
				{
					if(arrGroupData[iGroupID][g_iAllegiance] == 2)
					{
						if(arrGroupData[iGroupID][g_iGroupType] == 5)
						{
							new str[128], file[32];
							format(str, sizeof(str), "%s da tra $%s tien thue.", GetPlayerNameEx(i), number_format((PlayerInfo[i][pPayCheck] / 100) * TaxValue));
							format(file, sizeof(file), "grouppay/%d/%d-%d-%d.log", iGroupID, month, day, year);
							Log(file, str);
						}
					}
				}
			}
			PlayerInfo[i][pAccount] += interest;
			format(string, sizeof(string), "  Lai thu duoc: {19D700}$%s", number_format(interest));
			SendClientMessageEx(i, COLOR_WHITE, string);
			SendClientMessageEx(i, 0x16FC6FFF, "__________________________________________________");
			format(string, sizeof(string), "  Tien con lai: $%s  |  Tra thue: $%s", number_format(PlayerInfo[i][pAccount]), number_format((0 <= PlayerInfo[i][pRenting] < sizeof HouseInfo) ? (HouseInfo[PlayerInfo[i][pRenting]][hRentFee]) : (0)));
			SendClientMessageEx(i, COLOR_GRAD5, string);

			GivePlayerCash(i, PlayerInfo[i][pPayCheck]);
			
			/*if(month == 12 && day == 5)
			{
				if(++PlayerInfo[i][pFallIntoFun] == 5)
				{
					if(PlayerInfo[i][pReceivedPrize] == 0)
					{
						PlayerInfo[i][pGVIPExVoucher] += 1;
						SendClientMessageEx(i, COLOR_LIGHTBLUE, "You have receieved a 7 day Gold VIP voucher for playing 5 hours.");
						PlayerInfo[i][pReceivedPrize] = 1;
					}
					PlayerInfo[i][pFallIntoFun] = 0;
				}
			}*/
			
			// Fall Into Fun - 100 HP every 5 paychecks
			/*PlayerInfo[i][pFallIntoFun]++;
			
			if(PlayerInfo[i][pFallIntoFun] == 5)
			{	
				new Float: health;
				GetPlayerHealth(i, health);
				
				if(health == 100)
				{
					PlayerInfo[i][pFirstaid]++;
					SendClientMessageEx(i, COLOR_LIGHTBLUE, "You have played for 5 hours and received a firstaid kit due to having 100 percent health already.");
					PlayerInfo[i][pFallIntoFun] = 0;
				}
				else 
				{
					SetPlayerHealth(i, 100.0);
					SendClientMessageEx(i, COLOR_LIGHTBLUE, "You have played for 5 hours and received 100 percent HP.");
					PlayerInfo[i][pFallIntoFun] = 0;
				}
			}*/

			new
				iGroupID = PlayerInfo[i][pMember],
				iRank = PlayerInfo[i][pRank];

			if((0 <= iGroupID < MAX_GROUPS) && 0 <= iRank <= 9 && arrGroupData[iGroupID][g_iPaycheck][iRank] > 0) {
				if(arrGroupData[iGroupID][g_iAllegiance] == 1)
				{
					if(Tax > 0) {
						Tax -= arrGroupData[iGroupID][g_iPaycheck][iRank];
						GivePlayerCash(i, arrGroupData[iGroupID][g_iPaycheck][iRank]);
						format(string,sizeof(string),"  Luong chinh phu SA: {19D700}$%s", number_format(arrGroupData[iGroupID][g_iPaycheck][iRank]));
						SendClientMessageEx(i, COLOR_WHITE, string);
						for(new z; z < MAX_GROUPS; z++)
						{
							if(arrGroupData[z][g_iAllegiance] == 1)
							{
								if(arrGroupData[z][g_iGroupType] == 5)
								{
									new str[128], file[32];
       							    format(str, sizeof(str), "%s da duoc tra $%s luong Chinh phu.", GetPlayerNameEx(i), number_format(arrGroupData[iGroupID][g_iPaycheck][iRank]));
									format(file, sizeof(file), "grouppay/%d/%d-%d-%d.log", z, month, day, year);
									Log(file, str);
									break;
								}
							}
						}
					}
					else SendClientMessageEx(i, COLOR_RED, "[SA] Chinh phu no tien luong cua ban, ngan sach chinh phu khong co tien de chi tra.");
				}
				else if(arrGroupData[iGroupID][g_iAllegiance] == 2)
				{
					if(TRTax > 0) {
						TRTax -= arrGroupData[iGroupID][g_iPaycheck][iRank];
						GivePlayerCash(i, arrGroupData[iGroupID][g_iPaycheck][iRank]);
						format(string,sizeof(string),"  Luong chinh phu TR: {19D700}$%s", number_format(arrGroupData[iGroupID][g_iPaycheck][iRank]));
						SendClientMessageEx(i, COLOR_WHITE, string);
						for(new z; z < MAX_GROUPS; z++)
						{
							if(arrGroupData[z][g_iAllegiance] == 2)
							{
								if(arrGroupData[z][g_iGroupType] == 5)
								{
									new str[128], file[32];
									format(str, sizeof(str), "%s da duoc tra $%s luong chinh phu.", GetPlayerNameEx(i), number_format(arrGroupData[iGroupID][g_iPaycheck][iRank]));
									format(file, sizeof(file), "grouppay/%d/%d-%d-%d.log", z, month, day, year);
									Log(file, str);
									break;
								}
							}
						}
					}
					else SendClientMessageEx(i, COLOR_RED, "[TR] Chinh phu no tien luong cua ban; quy chinh phu khong co tien de chi tra.");
				}
			}
   			if (PlayerInfo[i][pBusiness] != INVALID_BUSINESS_ID) {
				if (Businesses[PlayerInfo[i][pBusiness]][bAutoPay] && PlayerInfo[i][pBusinessRank] >= 0 && PlayerInfo[i][pBusinessRank] < 5) {
				    if (Businesses[PlayerInfo[i][pBusiness]][bSafeBalance] < Businesses[PlayerInfo[i][pBusiness]][bRankPay][PlayerInfo[i][pBusinessRank]]) {
				    	SendClientMessageEx(i,COLOR_RED,"Doanh nghiep khong co du tien de chi tra cho ban.");
				    }
					else {
						GivePlayerCash(i, Businesses[PlayerInfo[i][pBusiness]][bRankPay][PlayerInfo[i][pBusinessRank]]);
						Businesses[PlayerInfo[i][pBusiness]][bSafeBalance] -= Businesses[PlayerInfo[i][pBusiness]][bRankPay][PlayerInfo[i][pBusinessRank]];
						SaveBusiness(PlayerInfo[i][pBusiness]);
						format(string,sizeof(string),"  Luong doanh nghiep: {FF5934}$%s", number_format(Businesses[PlayerInfo[i][pBusiness]][bRankPay][PlayerInfo[i][pBusinessRank]]));
						SendClientMessageEx(i, COLOR_WHITE, string);
					}
				}
			}
			
			HienTextdraw(i, "~y~PayDay~w~ Ban da nhan luong");
			//SendAudioToPlayer(i, 63, 100);
			PlayerInfo[i][pConnectSeconds] = 0;
			PlayerInfo[i][pPayCheck] = 0;
			if(++PlayerInfo[i][pConnectHours] == 2) {
				SendClientMessageEx(i, COLOR_LIGHTBLUE, "Ban da duoc phep su dung/so huu vu khi!");
			}
			if(PlayerInfo[i][pDonateRank] > 0 && ++PlayerInfo[i][pPayDayHad] >= 5) {
				PlayerInfo[i][pExp]++;
				PlayerInfo[i][pPayDayHad] = 0;
			}
			
			// Zombie Halloween
			if(month == 10 && day == 30)
			{
				if(PlayerInfo[i][pFallIntoFun] < 4)
				{
					PlayerInfo[i][pFallIntoFun]++;
				}
				else {
					 PlayerInfo[i][pFallIntoFun] = 0;
					 PlayerInfo[i][pVials] += 1;
				}
			}	

			if((month == 12 && day == 24) || (month == 10 && day == 31))
			{
				if(PlayerInfo[i][pTrickortreat] > 0)
				{
					PlayerInfo[i][pTrickortreat]--;
				}
			}

			//Weekday Madness for Fall Into Fun event; re-using Trickortreat variable to check connected time
			/*if(month == 10 && (day == 9 || day == 16))
			{
				PlayerInfo[i][pRewardDrawChance] += 2;
			}
			else if(month == 10 && day == 19)
			{
				PlayerInfo[i][pRewardDrawChance] += 3;
			}
			else PlayerInfo[i][pRewardDrawChance]++;
			
			if(PlayerInfo[i][pDonateRank] >= 3 && month == 10 && day == 13)
			{
				PlayerInfo[i][pRewardDrawChance] += 3;
			}*/
			Misc_Save();
			if(iRewardPlay) {
				PlayerInfo[i][pRewardHours]++;
				if(floatround(PlayerInfo[i][pRewardHours]) % 16 == 0) {
					PlayerInfo[i][pGoldBoxTokens]++;
					SendClientMessage(i, COLOR_LIGHTBLUE, "Ban da nhan duoc 1 Gold Giftbox token!  #Chuc mung");
				}
				format(string, sizeof(string), "Ban dang co %d gio thuong, hay kiem tra /rewards de biet them thong tin.", floatround(PlayerInfo[i][pRewardHours]));
				SendClientMessageEx(i, COLOR_YELLOW, string);
			}

			new exp = 0;
			if(DoubleXP) {
				exp = (PlayerInfo[i][pLevel] * XP_RATE * XP_RATE_HOURLY * 2);
				SendClientMessageEx(i, COLOR_YELLOW, "Kinh nghiem nhan vat: +%d XP (%s/%s) | DOUBLE XP", exp, number_format(PlayerInfo[i][pXP]+exp), number_format(GetRequiredXP(i)));
				PlayerInfo[i][pExp] += 2;
				GivePlayerXP(i, exp);
			}
			else if(PlayerInfo[i][pDoubleEXP] > 0 && !DoubleXP) {
				PlayerInfo[i][pDoubleEXP]--;
				exp = (PlayerInfo[i][pLevel] * XP_RATE * XP_RATE_HOURLY * 2);
				SendClientMessageEx(i, COLOR_YELLOW, "Kinh nghiem nhan vat: +%d XP (%s/%s) | DOUBLE XP", exp, number_format(PlayerInfo[i][pXP]+exp), number_format(GetRequiredXP(i)));
				SendClientMessageEx(i, COLOR_YELLOW, "Ban co %d gio con lai nhan doi EXP Token.", PlayerInfo[i][pDoubleEXP]);
				PlayerInfo[i][pExp] += 2;
				GivePlayerXP(i, exp);
			}
			else 
			{
				exp = (PlayerInfo[i][pLevel] * XP_RATE * XP_RATE_HOURLY);
				PlayerInfo[i][pExp]++;
				SendClientMessageEx(i, COLOR_YELLOW, "Kinh nghiem nhan vat: +%d XP (%s/%s)", exp, number_format(PlayerInfo[i][pXP]+exp), number_format(GetRequiredXP(i)));
				GivePlayerXP(i, exp);
			}

			if(PlayerInfo[i][pWRestricted] > 0 && --PlayerInfo[i][pWRestricted] == 0) {
				SendClientMessageEx(i, COLOR_LIGHTRED, "Bay gio vu khi cua ban khong con bi gioi han!");
			}
			
			if(PlayerInfo[i][pShopNotice] > 0) PlayerInfo[i][pShopNotice]--;
			if(ShopReminder == 1 && PlayerInfo[i][pShopNotice] == 0)
			{
				PlayerInfo[i][pShopCounter]++;
				if(PlayerInfo[i][pLevel] <= 5 && PlayerInfo[i][pShopCounter] == 5 || PlayerInfo[i][pLevel] > 5 && PlayerInfo[i][pShopCounter] == 10)
				{
					format(string, sizeof(string), "Hey,kiem tra nay, su dung: ~y~/mcshop");
					if(PlayerInfo[i][pConnectHours] >= 50)
					{
						strcat(string, "~w~~n~De vo hieu hoa thong bao nay trong vong 24 gio, su dung: ~y~/togshopnotice");
					}
					PlayerInfo[i][pShopCounter] = 0;
					PlayerTextDrawSetString(i, ShopNotice[i], string);
					PlayerTextDrawShow(i, ShopNotice[i]);
					SetTimerEx("HidePlayerTextDraw", 10000, false, "ii", i, _:ShopNotice[i]);
				}
			}
		}
		else SendClientMessageEx(i, COLOR_LIGHTRED, "* Ban da khong choi du lau de nhan paycheck.");
	}

	if (GetPVarType(i, "UnreadMails") && HasMailbox(i))
	{
		SendClientMessageEx(i, COLOR_YELLOW, "Ban co mot tin nhan chua doc trong Mailbox, vui long kiem tra mailbox");
	}
}


alias:payday("lanhluong")
CMD:payday(playerid, params)
{
	if(!IsPlayerDiscordVerified(playerid)) 
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban phai lien ket tai khoan Discord de co the lam dieu nay.");

	if(PlayerInfo[playerid][pPayDayCode] == 0)
		return SendClientMessage(playerid, COLOR_GREY, "Ban khong co ki lanh luong co the nhan.");

	ShowPlayerDialog(playerid, DIALOG_PAYDAY, DIALOG_STYLE_INPUT, "PayCheck", "{FFFFFF}Ma lanh luong cua ban: {FFFA77}%d\n\n{FFFFFF}Nhap ma lanh luong cua ban ben duoi de nhan luong:", "Gui", "Huy bo", PlayerInfo[playerid][pPayDayCode]);
	return 1;
}

alias:nextpaycheck("thoigianlanhluong")
CMD:nextpaycheck(playerid, params[])
{
	SendClientMessageEx(playerid, 0x43FF88FF, "Thoi gian Paycheck truoc: %02d phut truoc", floatround(PlayerInfo[playerid][pConnectSeconds]/60));
	SendClientMessageEx(playerid, 0x43FF88FF, "Thoi gian Paycheck tiep theo: %02d phut nua", floatround((3600-PlayerInfo[playerid][pConnectSeconds]) / 60));
	SendClientMessageEx(playerid, COLOR_GRAD2, "Luu y: Thoi gian nhan Paycheck cua ban khong duoc tinh khi AFK.");
	return 1;
}