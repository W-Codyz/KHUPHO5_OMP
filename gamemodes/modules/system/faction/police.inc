#include <YSI\YSI_Coding\y_hooks>

#define DIALOG_POLICE_INSPECT   (11100)
#define DIALOG_POLICE_TAKE_AMT  (11101)
#define DIALOG_POLICE_TAKE_WEP  (11102)

ShowPoliceInspect(playerid, targetid)
{
    new string[2048], title[96];
    format(title, sizeof(title), "Kham xet tui do: %s", GetPlayerNameEx(targetid));
    new count = 0;

    for(new i = 0; i < sizeof(ItemInventory); i++)
    {
        new variable = ItemInventory[i][pVariable];
        new quantity = PlayerInfo[targetid][variable];
        if(quantity > 0)
        {
            count++;
            format(string, sizeof(string), "%s%s\t%d\n", string, ItemInventory[i][NameItem], quantity);
        }
    }

    new myweapons[13][2];
    for(new i = 0; i < 13; i++)
    {
        GetPlayerWeaponData(targetid, i, myweapons[i][0], myweapons[i][1]);
        if(myweapons[i][0] > 0 && PlayerInfo[targetid][pGuns][i] == myweapons[i][0])
        {
            count++;
            new weaponname[32];
            GetWeaponName(myweapons[i][0], weaponname, sizeof(weaponname));
            format(string, sizeof(string), "%s[Vu khi] %s\t%d dan\n", string, weaponname, myweapons[i][1]);
        }
    }

    SetPVarInt(playerid, "InspectTarget", targetid);

    if(count > 0)
        return ShowPlayerDialog(playerid, DIALOG_POLICE_INSPECT, DIALOG_STYLE_TABLIST, title, string, "Tich thu", "Dong");

    ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, title,
        "{ffffff}Nguoi nay khong mang theo gi.", "Dong", "");
    return 1;
}

alias:take("tichthu")
CMD:take(playerid, params[])
{
    if(!IsACop(playerid))
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong phai la nhan vien thuc thi phap luat.");

    new giveplayerid;
    if(sscanf(params, "u", giveplayerid))
    {
        SendHelpMessage(playerid, "/tichthu [player] - Kham xet va tich thu vat pham cua nguoi choi");
        return 1;
    }

    if(giveplayerid == playerid)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong the kham xet chinh minh!");

    if(!IsPlayerConnected(giveplayerid))
        return SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi khong hop le.");

    if(!ProxDetectorS(8.0, playerid, giveplayerid))
        return SendClientMessageEx(playerid, COLOR_GREY, "Nguoi do khong o gan ban.");

    new action[128];
    format(action, sizeof(action), "* %s dang kham xet tui do cua %s.",
        GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid));
    ProxDetector(30.0, playerid, action, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);

    ShowPoliceInspect(playerid, giveplayerid);
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid)
    {
        case DIALOG_POLICE_INSPECT:
        {
            if(!response) return 1;

            new targetid = GetPVarInt(playerid, "InspectTarget");
            if(!IsPlayerConnected(targetid))
                return SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi da ngat ket noi.");

            new currentItem = 0;

            for(new i = 0; i < sizeof(ItemInventory); i++)
            {
                new variable = ItemInventory[i][pVariable];
                if(PlayerInfo[targetid][variable] > 0)
                {
                    if(currentItem == listitem)
                    {
                        SetPVarInt(playerid, "InspectItemType", 0);
                        SetPVarInt(playerid, "InspectItemIndex", i);

                        new string[256];
                        format(string, sizeof(string),
                            "%s dang co: {FFFF00}%d {ffffff}%s\nNhap so luong tich thu (0 = tat ca):",
                            GetPlayerNameEx(targetid),
                            PlayerInfo[targetid][variable],
                            ItemInventory[i][NameItem]);
                        ShowPlayerDialog(playerid, DIALOG_POLICE_TAKE_AMT, DIALOG_STYLE_INPUT,
                            "Tich thu vat pham", string, "Tich thu", "Quay lai");
                        return 1;
                    }
                    currentItem++;
                }
            }

            new myweapons[13][2];
            for(new i = 0; i < 13; i++)
            {
                GetPlayerWeaponData(targetid, i, myweapons[i][0], myweapons[i][1]);
                if(myweapons[i][0] > 0 && PlayerInfo[targetid][pGuns][i] == myweapons[i][0])
                {
                    if(currentItem == listitem)
                    {
                        SetPVarInt(playerid, "InspectItemType", 1);
                        SetPVarInt(playerid, "InspectItemIndex", i);
                        SetPVarInt(playerid, "InspectWeaponID", myweapons[i][0]);

                        new weaponname[32];
                        GetWeaponName(myweapons[i][0], weaponname, sizeof(weaponname));
                        new string[256];
                        format(string, sizeof(string),
                            "Tich thu vu khi {FFFF00}%s{ffffff} (%d dan) cua {FFFF00}%s{ffffff}?",
                            weaponname, myweapons[i][1], GetPlayerNameEx(targetid));
                        ShowPlayerDialog(playerid, DIALOG_POLICE_TAKE_WEP, DIALOG_STYLE_MSGBOX,
                            "Tich thu vu khi", string, "Tich thu", "Quay lai");
                        return 1;
                    }
                    currentItem++;
                }
            }
        }

        case DIALOG_POLICE_TAKE_WEP:
        {
            new targetid = GetPVarInt(playerid, "InspectTarget");

            if(!response)
            {
                if(IsPlayerConnected(targetid)) ShowPoliceInspect(playerid, targetid);
                return 1;
            }

            if(!IsPlayerConnected(targetid))
                return SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi da ngat ket noi.");

            new slot     = GetPVarInt(playerid, "InspectItemIndex");
            new weaponid = GetPVarInt(playerid, "InspectWeaponID");

            new weaponname[32];
            GetWeaponName(weaponid, weaponname, sizeof(weaponname));

            RemovePlayerWeapon(targetid, weaponid);
            PlayerInfo[targetid][pGuns][slot] = 0;

            new string[256];
            format(string, sizeof(string), "* %s da tich thu vu khi %s cua %s.",
                GetPlayerNameEx(playerid), weaponname, GetPlayerNameEx(targetid));
            ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);

            format(string, sizeof(string), "* Vien canh sat %s da tich thu vu khi %s cua ban.",
                GetPlayerNameEx(playerid), weaponname);
            SendClientMessageEx(targetid, COLOR_LIGHTBLUE, string);

            ShowPoliceInspect(playerid, targetid);
        }

        case DIALOG_POLICE_TAKE_AMT:
        {
            new targetid = GetPVarInt(playerid, "InspectTarget");

            if(!response)
            {
                if(IsPlayerConnected(targetid)) ShowPoliceInspect(playerid, targetid);
                return 1;
            }

            if(!IsPlayerConnected(targetid))
                return SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi da ngat ket noi.");

            new itemIndex = GetPVarInt(playerid, "InspectItemIndex");
            new variable  = ItemInventory[itemIndex][pVariable];
            new available = PlayerInfo[targetid][variable];
            new amount    = strval(inputtext);

            if(amount <= 0 || amount > available) amount = available;

            PlayerInfo[targetid][variable] -= amount;
            g_mysql_SaveAccount(targetid);

            new string[256];
            format(string, sizeof(string), "* %s da tich thu %d %s cua %s.",
                GetPlayerNameEx(playerid), amount, ItemInventory[itemIndex][NameItem], GetPlayerNameEx(targetid));
            ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);

            format(string, sizeof(string), "* Vien canh sat %s da tich thu %d %s cua ban.",
                GetPlayerNameEx(playerid), amount, ItemInventory[itemIndex][NameItem]);
            SendClientMessageEx(targetid, COLOR_LIGHTBLUE, string);

            ShowPoliceInspect(playerid, targetid);
        }
    }
    return 1;
}

CMD:lucsoat(playerid, params[])
{
	if(IsACop(playerid))
	{
		new string[128], storageid, giveplayerid;
		if(sscanf(params, "u", giveplayerid)) {
			SendHelpMessage(playerid, "/lucsoat [player]");
			return 1;
		}
		/*if(sscanf(params, "ud", giveplayerid, storageid)) {
		SendHelpMessage(playerid, "/lucsoat [player] [storageid]");
		SendClientMessageEx(playerid, COLOR_GREY, "StorageIDs: (0) Pocket - (1) Equipped Storage Device");
		return 1;
		}

		if(storageid < 0 || storageid > 1) {
			SendClientMessageEx(playerid, COLOR_WHITE, "SU DUNG: /lucsoat [player] [storageid]");
			SendClientMessageEx(playerid, COLOR_GREY, "StorageIDs: (0) Pocket - (1) Equipped Storage Device");
			return 1;
		}*/

		if(IsPlayerConnected(giveplayerid))
		{
			if (ProxDetectorS(8.0, playerid, giveplayerid))
			{
				if(giveplayerid == playerid) { SendClientMessageEx(playerid, COLOR_GREY, "Ban khong the luc soat chinh minh!"); return 1; }

				/*// Find the storageid of the storagedevice.
				if(storageid == 1) {
					new bool:itemEquipped = false;
					for(new i = 0; i < 3; i++)
					{
						if(StorageInfo[giveplayerid][i][sAttached] == 1) {
							storageid = i+1;
							itemEquipped = true;
						}
					}
					if(itemEquipped == false) return SendClientMessageEx(playerid, COLOR_WHITE, "That person doesn't have a storage device equipped!");
				}*/
				new packages = GetPVarInt(giveplayerid, "Packages");
				SendClientMessageEx(playerid, COLOR_GREEN, "_______________________________________");
				format(string, sizeof(string), "*** %s' items...  ***", GetPlayerNameEx(giveplayerid));
				SendClientMessageEx(playerid, COLOR_WHITE, string);
				if(PlayerInfo[giveplayerid][pPot] > 0)
				{
					format(string, sizeof(string), "(Tui) %d grams pot.", PlayerInfo[giveplayerid][pPot]);
					SendClientMessageEx(playerid, COLOR_GREY, string);
				}

				if(PlayerInfo[giveplayerid][pWSeeds] == 1)
				{
					SendClientMessageEx(playerid, COLOR_GREY, "(Tui) Hat giong can sa");
				}
				if(PlayerInfo[giveplayerid][pCrack] > 0)
				{
					format(string, sizeof(string), "(Tui) %d grams crack.", PlayerInfo[giveplayerid][pCrack]);
					SendClientMessageEx(playerid, COLOR_GREY, string);
				}
				if(PlayerInfo[giveplayerid][pMats] > 0)
				{
					format(string, sizeof(string), "(Tui) %d materials.", PlayerInfo[giveplayerid][pMats]);
					SendClientMessageEx(playerid, COLOR_GREY, string);
				}
				if(PlayerInfo[giveplayerid][pHeroin] > 0)
				{
					format(string, sizeof(string), "(Tui) %d grams heroin.", PlayerInfo[giveplayerid][pHeroin]);
					SendClientMessageEx(playerid, COLOR_GREY, string);
				}
				if(PlayerInfo[giveplayerid][pRawOpium] > 0)
				{
					format(string, sizeof(string), "(Tui) %d grams thuoc phien tho.", PlayerInfo[giveplayerid][pRawOpium]);
					SendClientMessageEx(playerid, COLOR_GREY, string);
				}
				if(PlayerInfo[giveplayerid][pSyringes] > 0)
				{
					format(string, sizeof(string), "(Tui) %d kim tiem.", PlayerInfo[giveplayerid][pSyringes]);
					SendClientMessageEx(playerid, COLOR_GREY, string);
				}
				if(PlayerInfo[giveplayerid][pOpiumSeeds] > 0)
				{
					format(string, sizeof(string), "(Tui) %d hat thuoc phien.", PlayerInfo[giveplayerid][pOpiumSeeds]);
					SendClientMessageEx(playerid, COLOR_GREY, string);
				}
                if(packages > 0)
				{
					format(string, sizeof(string), "(Tui) %d goi vat lieu.", packages);
					SendClientMessageEx(playerid, COLOR_GREY, string);
				}

				if(storageid > 0)
				{
					if(StorageInfo[giveplayerid][storageid-1][sPot] > 0)
					{
						format(string, sizeof(string), "(%s) %d grams pot.", storagetype[storageid], StorageInfo[giveplayerid][storageid-1][sPot]);
						SendClientMessageEx(playerid, COLOR_GREY, string);
					}
					if(StorageInfo[giveplayerid][storageid-1][sCrack] > 0)
					{
						format(string, sizeof(string), "(%s) %d grams crack.", storagetype[storageid], StorageInfo[giveplayerid][storageid-1][sCrack]);
						SendClientMessageEx(playerid, COLOR_GREY, string);
					}
					if(StorageInfo[giveplayerid][storageid-1][sMats] > 0)
					{
						format(string, sizeof(string), "(%s) %d materials.", storagetype[storageid], StorageInfo[giveplayerid][storageid-1][sMats]);
						SendClientMessageEx(playerid, COLOR_GREY, string);
					}
				}

				if(PlayerInfo[giveplayerid][pPhoneBook] > 0) SendClientMessageEx(playerid, COLOR_GREY, "Sim dien thoai.");
				if(PlayerInfo[giveplayerid][pCDPlayer] > 0) SendClientMessageEx(playerid, COLOR_GREY, "May ghe nhac.");
				new weaponname[50];
				format(string, sizeof(string), "*** %s' vu khi...  ***", GetPlayerNameEx(giveplayerid));
				SendClientMessageEx(playerid, COLOR_WHITE, string);
				for (new i = 0; i < 12; i++)
				{
					if(PlayerInfo[giveplayerid][pGuns][i] > 0)
					{
						GetWeaponName(PlayerInfo[giveplayerid][pGuns][i], weaponname, sizeof(weaponname));
						format(string, sizeof(string), "Vu khi: %s.", weaponname);
						SendClientMessageEx(playerid, COLOR_GRAD1, string);
					}
				}
				SendClientMessageEx(playerid, COLOR_GREEN, "_______________________________________");
				format(string, sizeof(string), "* %s da luc soat %s thay vat pham pham phap.", GetPlayerNameEx(playerid),GetPlayerNameEx(giveplayerid));
				ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
			}
			else
			{
				SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi do khong gan ban.");
			}

		}
		else
		{
			SendClientMessageEx(playerid, COLOR_GREY, "Nguoi khong hop le.");
			return 1;
		}
	}
	else
	{
		SendClientMessageEx(playerid, COLOR_GREY, "   Ban khong phai la mot nhan vien thuc thi phap luat, hoac mot ve si!");
		return 1;
	}
	return 1;
}

CMD:licenses(playerid, params[])
{
	if(PlayerInfo[playerid][pIDCard] == 0) 
		return SendClientMessage(playerid, COLOR_GREY, "Ban chua so huu Can cuoc cong dan!");

	new string[428], text1[32], text2[32], text3[32], text4[32];
	if(PlayerInfo[playerid][pCarLic]) { text1 = "{0EEF5D}Da so huu{FFFFFF}"; } else { text1 = "{C2A347}Chua so huu{FFFFFF}"; }
	if(PlayerInfo[playerid][pFlyLic]) { text4 = "{0EEF5D}Da so huu{FFFFFF}"; } else { text4 = "{C2A347}Chua so huu{FFFFFF}"; }
	if(PlayerInfo[playerid][pBoatLic]) { text2 = "{0EEF5D}Da so huu{FFFFFF}"; } else { text2 = "{C2A347}Chua so huu{FFFFFF}"; }
	if(PlayerInfo[playerid][pTaxiLicense]) { text3 = "{0EEF5D}Da so huu{FFFFFF}"; } else { text3 = "{C2A347}Chua so huu{FFFFFF}"; }

	format(string, _, "{FFFFFF}ID Card: {A3F15F}%d\n{FFFFFF}Ho va ten: %s\n___________________________________\nGiay phep lai xe: %s\nGiay phep Taxi: %s\nGiay phep thuyen: %s\nGiay phep may bay: %s", PlayerInfo[playerid][pIDCard], GetPlayerNameEx(playerid), text1, text3, text2, text4);
	ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "Thong tin cong dan", string, "Dong", "");
	return 1;
}

alias:choxemgiayphep("showid")
CMD:choxemgiayphep(playerid, params[])
{
	if(PlayerInfo[playerid][pIDCard] == 0)
		return SendClientMessage(playerid, COLOR_GREY, "Ban chua so huu Can cuoc cong dan!");

	new giveplayerid;
	if(sscanf(params, "u", giveplayerid)) return SendHelpMessage(playerid, "/choxemgiayphep [player]");

	if(!IsPlayerConnected(giveplayerid))
		return SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi khong hop le.");

	if(giveplayerid == playerid)
		return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong the hien giay phep cho chinh ban - su dung /licenses.");

	if(!ProxDetectorS(8.0, playerid, giveplayerid))
		return SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi do khong gan ban.");

	new proxstring[128];
	format(proxstring, sizeof(proxstring), "{FF8000}* {C2A2DA}%s dua giay to tuy than ra cho %s xem.", GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid));
	ProxDetectorWrap(playerid, proxstring, 92, 30.0, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

	new dlgbody[128];
	format(dlgbody, sizeof(dlgbody), "{FFFFFF}%s muon cho ban xem giay to tuy than cua ho.\n{FFFF00}Ban co muon xem khong?", GetPlayerNameEx(playerid));
	SetPVarInt(giveplayerid, "pCardViewFrom", playerid);
	ShowPlayerDialog(giveplayerid, DIALOG_CARD_VIEW_REQUEST, DIALOG_STYLE_MSGBOX, "{FFFFFF}Kiem tra giay to", dlgbody, "Xem ngay", "Tu choi");

	SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* Ban da gui yeu cau cho %s xem giay to cua ban.", GetPlayerNameEx(giveplayerid));
	return 1;
}
