#include <YSI\YSI_Coding\y_hooks>

#define DIALOG_BUYFOOD 9150

enum ItemFood_Data {
    NameFood[128],
    PriceFood
}

new const ItemFood[][ItemFood_Data] = {
    {"Banh mi", 2000},
    {"Nuoc\t", 2000},
    {"Pizza\t", 7000},
    {"Bun rieu", 3000},
    {"Pho bo", 4000},
    {"Hamburger", 3000},
    {"Ca phe", 2500},
    {"Sua bo", 3000},
    {"Thit hun khoi", 4000},
    {"Trai cay", 1500},
    {"Nuoc ep", 2500}
};

stock ShowBuyFood(playerid) {
    new string[15000];
    for(new i = 0; i < sizeof(ItemFood); i++) {
        format(string, sizeof(string), "%s\n{00ffff}>{ffffff} %s\t\t%s$", 
            string, 
            ItemFood[i][NameFood],
            FormatMoney(ItemFood[i][PriceFood])
        );
    }
    ShowPlayerDialog(playerid, DIALOG_BUYFOOD, DIALOG_STYLE_LIST, 
        "Ban muon an gi?", string, "Mua", "Huy");
    return 1;
}

CMD:muathucan(playerid, params[])
{
    if (!IsAtRestaurant(playerid))
	{
		SendClientMessageEx(playerid, COLOR_GRAD2, " Ban khong o trong nha hang!");
		return 0;
	}
	ShowBuyFood(playerid);
	return 1;
}

/* hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(newkeys & KEY_YES)
    {
        if (IsAtRestaurant(playerid))
			return cmd_muathucan(playerid, "");
    }
    return 1;
} */

// Xu ly dialog mua do an
hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_BUYFOOD)
    {
        if(!response) return 1;
        
        if(listitem < 0 || listitem >= sizeof(ItemFood)) return 1;
        
        new price = ItemFood[listitem][PriceFood];
        
        // Kiem tra tien
        if(PlayerInfo[playerid][pCash] < price)
        {
            SendClientMessageEx(playerid, COLOR_GREY, "Ban khong du tien de mua mon nay!");
            return 1;
        }
        
        // Tru tien
        PlayerInfo[playerid][pCash] -= price;
        
        new string[256], rand, rand1;
        
        // Xu ly cho tung mon an
        switch(listitem)
        {
            case 0: // Banh mi
            {
                if(PlayerInfo[playerid][pThucAn] >= 100)
                {
                    SendClientMessageEx(playerid, COLOR_GREY, "Ban da no roi, khong the an them!");
                    PlayerInfo[playerid][pCash] += price; // Hoan tien
                    return 1;
                }
                rand = 15 + random(10);
                PlayerInfo[playerid][pThucAn] += rand;
                if(PlayerInfo[playerid][pThucAn] > 100) PlayerInfo[playerid][pThucAn] = 100;
                format(string, sizeof(string), "Ban da mua {00FF00}Banh mi{FFFFFF} voi gia {00FF00}%s${FFFFFF}. Tang {FFA500}+%d{FFFFFF} do no.", 
                    FormatMoney(price), rand);
            }
            case 1: // Nuoc loc
            {
                if(PlayerInfo[playerid][pNuoc] >= 100)
                {
                    SendClientMessageEx(playerid, COLOR_GREY, "Ban da no nuoc roi, khong the uong them!");
                    PlayerInfo[playerid][pCash] += price; // Hoan tien
                    return 1;
                }
                rand = 15 + random(10);
                PlayerInfo[playerid][pNuoc] += rand;
                if(PlayerInfo[playerid][pNuoc] > 100) PlayerInfo[playerid][pNuoc] = 100;
                format(string, sizeof(string), "Ban da mua {00BFFF}Nuoc loc{FFFFFF} voi gia {00FF00}%s${FFFFFF}. Tang {00BFFF}+%d{FFFFFF} do khat.", 
                    FormatMoney(price), rand);
            }
            case 2: // Pizza
            {
                if(PlayerInfo[playerid][pThucAn] >= 100)
                {
                    SendClientMessageEx(playerid, COLOR_GREY, "Ban da no roi, khong the an them!");
                    PlayerInfo[playerid][pCash] += price; // Hoan tien
                    return 1;
                }
                rand = 60 + random(10);
                PlayerInfo[playerid][pThucAn] += rand;
                if(PlayerInfo[playerid][pThucAn] > 100) PlayerInfo[playerid][pThucAn] = 100;
                format(string, sizeof(string), "Ban da mua {00FF00}Pizza{FFFFFF} voi gia {00FF00}%s${FFFFFF}. Tang {FFA500}+%d{FFFFFF} do no.", 
                    FormatMoney(price), rand);
            }
            case 3: // Bun rieu
            {
                if(PlayerInfo[playerid][pThucAn] >= 100 && PlayerInfo[playerid][pNuoc] >= 100)
                {
                    SendClientMessageEx(playerid, COLOR_GREY, "Ban da no va khong con khat nuoc, khong the an them!");
                    PlayerInfo[playerid][pCash] += price; // Hoan tien
                    return 1;
                }
                rand = 35 + random(10);
                rand1 = 10 + random(10);
                PlayerInfo[playerid][pThucAn] += rand;
                PlayerInfo[playerid][pNuoc] += rand1;
                if(PlayerInfo[playerid][pThucAn] > 100) PlayerInfo[playerid][pThucAn] = 100;
                if(PlayerInfo[playerid][pNuoc] > 100) PlayerInfo[playerid][pNuoc] = 100;
                format(string, sizeof(string), "Ban da mua {00FF00}Bun rieu{FFFFFF} voi gia {00FF00}%s${FFFFFF}. Tang {FFA500}+%d{FFFFFF} do no, {00BFFF}+%d{FFFFFF} do khat.", 
                    FormatMoney(price), rand, rand1);
            }
            case 4: // Pho bo
            {
                if(PlayerInfo[playerid][pThucAn] >= 100 && PlayerInfo[playerid][pNuoc] >= 100)
                {
                    SendClientMessageEx(playerid, COLOR_GREY, "Ban da no va khong con khat nuoc, khong the an them!");
                    PlayerInfo[playerid][pCash] += price; // Hoan tien
                    return 1;
                }
                rand = 40 + random(10);
                rand1 = 10 + random(10);
                PlayerInfo[playerid][pThucAn] += rand;
                PlayerInfo[playerid][pNuoc] += rand1;
                if(PlayerInfo[playerid][pThucAn] > 100) PlayerInfo[playerid][pThucAn] = 100;
                if(PlayerInfo[playerid][pNuoc] > 100) PlayerInfo[playerid][pNuoc] = 100;
                format(string, sizeof(string), "Ban da mua {00FF00}Pho bo{FFFFFF} voi gia {00FF00}%s${FFFFFF}. Tang {FFA500}+%d{FFFFFF} do no, {00BFFF}+%d{FFFFFF} do khat.", 
                    FormatMoney(price), rand, rand1);
            }
            case 5: // Hamburger
            {
                if(PlayerInfo[playerid][pThucAn] >= 100 && PlayerInfo[playerid][pNuoc] >= 100)
                {
                    SendClientMessageEx(playerid, COLOR_GREY, "Ban da no va khong con khat nuoc, khong the an them!");
                    PlayerInfo[playerid][pCash] += price; // Hoan tien
                    return 1;
                }
                rand = 35 + random(10);
                rand1 = 10 + random(10);
                PlayerInfo[playerid][pThucAn] += rand;
                PlayerInfo[playerid][pNuoc] += rand1;
                if(PlayerInfo[playerid][pThucAn] > 100) PlayerInfo[playerid][pThucAn] = 100;
                if(PlayerInfo[playerid][pNuoc] > 100) PlayerInfo[playerid][pNuoc] = 100;
                format(string, sizeof(string), "Ban da mua {00FF00}Hamburger{FFFFFF} voi gia {00FF00}%s${FFFFFF}. Tang {FFA500}+%d{FFFFFF} do no, {00BFFF}+%d{FFFFFF} do khat.", 
                    FormatMoney(price), rand, rand1);
            }
            case 6: // Ca phe
            {
                if(PlayerInfo[playerid][pThucAn] >= 100 && PlayerInfo[playerid][pNuoc] >= 100)
                {
                    SendClientMessageEx(playerid, COLOR_GREY, "Ban da no va khong con khat nuoc, khong the uong them!");
                    PlayerInfo[playerid][pCash] += price; // Hoan tien
                    return 1;
                }
                rand = 10 + random(10);
                rand1 = 25 + random(10);
                PlayerInfo[playerid][pThucAn] += rand;
                PlayerInfo[playerid][pNuoc] += rand1;
                if(PlayerInfo[playerid][pThucAn] > 100) PlayerInfo[playerid][pThucAn] = 100;
                if(PlayerInfo[playerid][pNuoc] > 100) PlayerInfo[playerid][pNuoc] = 100;
                format(string, sizeof(string), "Ban da mua {00BFFF}Ca phe{FFFFFF} voi gia {00FF00}%s${FFFFFF}. Tang {FFA500}+%d{FFFFFF} do no, {00BFFF}+%d{FFFFFF} do khat.", 
                    FormatMoney(price), rand, rand1);
            }
            case 7: // Sua bo
            {
                if(PlayerInfo[playerid][pThucAn] >= 100 && PlayerInfo[playerid][pNuoc] >= 100)
                {
                    SendClientMessageEx(playerid, COLOR_GREY, "Ban da no va khong con khat nuoc, khong the uong them!");
                    PlayerInfo[playerid][pCash] += price; // Hoan tien
                    return 1;
                }
                rand = 10 + random(10);
                rand1 = 20 + random(10);
                PlayerInfo[playerid][pThucAn] += rand;
                PlayerInfo[playerid][pNuoc] += rand1;
                if(PlayerInfo[playerid][pThucAn] > 100) PlayerInfo[playerid][pThucAn] = 100;
                if(PlayerInfo[playerid][pNuoc] > 100) PlayerInfo[playerid][pNuoc] = 100;
                format(string, sizeof(string), "Ban da mua {00BFFF}Sua bo{FFFFFF} voi gia {00FF00}%s${FFFFFF}. Tang {FFA500}+%d{FFFFFF} do no, {00BFFF}+%d{FFFFFF} do khat.", 
                    FormatMoney(price), rand, rand1);
            }
            case 8: // Thit hun khoi
            {
                if(PlayerInfo[playerid][pThucAn] >= 100)
                {
                    SendClientMessageEx(playerid, COLOR_GREY, "Ban da no roi, khong the an them!");
                    PlayerInfo[playerid][pCash] += price; // Hoan tien
                    return 1;
                }
                rand = 40 + random(10);
                PlayerInfo[playerid][pThucAn] += rand;
                if(PlayerInfo[playerid][pThucAn] > 100) PlayerInfo[playerid][pThucAn] = 100;
                format(string, sizeof(string), "Ban da mua {00FF00}Thit hun khoi{FFFFFF} voi gia {00FF00}%s${FFFFFF}. Tang {FFA500}+%d{FFFFFF} do no.", 
                    FormatMoney(price), rand);
            }
            case 9: // Trai cay
            {
                if(PlayerInfo[playerid][pThucAn] >= 100 && PlayerInfo[playerid][pNuoc] >= 100)
                {
                    SendClientMessageEx(playerid, COLOR_GREY, "Ban da no va khong con khat nuoc, khong the an them!");
                    PlayerInfo[playerid][pCash] += price; // Hoan tien
                    return 1;
                }
                rand = 15 + random(10);
                rand1 = 10 + random(10);
                PlayerInfo[playerid][pThucAn] += rand;
                PlayerInfo[playerid][pNuoc] += rand1;
                if(PlayerInfo[playerid][pThucAn] > 100) PlayerInfo[playerid][pThucAn] = 100;
                if(PlayerInfo[playerid][pNuoc] > 100) PlayerInfo[playerid][pNuoc] = 100;
                format(string, sizeof(string), "Ban da mua {00FF00}Trai cay{FFFFFF} voi gia {00FF00}%s${FFFFFF}. Tang {FFA500}+%d{FFFFFF} do no, {00BFFF}+%d{FFFFFF} do khat.", 
                    FormatMoney(price), rand, rand1);
            }
            case 10: // Nuoc ep
            {
                if(PlayerInfo[playerid][pThucAn] >= 100 && PlayerInfo[playerid][pNuoc] >= 100)
                {
                    SendClientMessageEx(playerid, COLOR_GREY, "Ban da no va khong con khat nuoc, khong the uong them!");
                    PlayerInfo[playerid][pCash] += price; // Hoan tien
                    return 1;
                }
                rand = 10 + random(10);
                rand1 = 30 + random(10);
                PlayerInfo[playerid][pThucAn] += rand;
                PlayerInfo[playerid][pNuoc] += rand1;
                if(PlayerInfo[playerid][pThucAn] > 100) PlayerInfo[playerid][pThucAn] = 100;
                if(PlayerInfo[playerid][pNuoc] > 100) PlayerInfo[playerid][pNuoc] = 100;
                format(string, sizeof(string), "Ban da mua {00BFFF}Nuoc ep{FFFFFF} voi gia {00FF00}%s${FFFFFF}. Tang {FFA500}+%d{FFFFFF} do no, {00BFFF}+%d{FFFFFF} do khat.", 
                    FormatMoney(price), rand, rand1);
            }
        }
        
        SendClientMessageEx(playerid, COLOR_WHITE, string);
        PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
        return 1;
    }
    return 1;
}