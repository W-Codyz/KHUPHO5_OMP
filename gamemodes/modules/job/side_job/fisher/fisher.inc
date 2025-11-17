#include <YSI\YSI_Coding\y_hooks>

stock SavePlayerFish(playerid)
{
    new query[428];
    for(new i; i < MAX_FISH_BAG; i++)
    {
        format(query, _, "%s`Fish%d`='%d', `FishWeight%d`='%d'%s", query, i, Fishes[playerid][pFid][i], i, Fishes[playerid][pWeight][i], i==MAX_FISH_BAG-1 ? "" : ", ");
    }

    mysql_format(MainPipeline, query, sizeof(query), "UPDATE `accounts` SET %s WHERE `id`='%d'", query, GetPlayerSQLId(playerid));
    mysql_tquery(MainPipeline, query, "", "");
    return 1;
}

stock CreateFishLabel()
{
	for(new i; i < sizeof(FishPosition); i++)
	{
		CreateDynamic3DTextLabel("Diem cau ca\n\n{FFFFFF}Go \"{E4FD2B}/cauca{FFFFFF}\" de cau ca", 0x49FDFAFF, FishPosition[i][0], FishPosition[i][1], FishPosition[i][2], 20.0, _, _, 1, 0, 0);
	}
	return 1;
}

stock IsAtFishPlace(playerid)
{
	if(IsPlayerConnected(playerid))
	{
		for(new i;i < sizeof(FishPosition); i++)
		{
			if(IsPlayerInRangeOfPoint(playerid, 1.5, FishPosition[i][0], FishPosition[i][1], FishPosition[i][2]))
			{
				return 1;
			}
		}
	}
	return 0;
}

stock GetFishCost(playerid, fish)
{
	if(IsPlayerConnected(playerid)) {
		new cost = 0;
		switch (fish)
		{
			case 1:
			{
				cost = 1;
			}
			case 2:
			{
				cost = 3;
			}
			case 3:
			{
				cost = 3;
			}
			case 5:
			{
				cost = 5;
			}
			case 6:
			{
				cost = 2;
			}
			case 8:
			{
				cost = 8;
			}
			case 9:
			{
				cost = 12;
			}
			case 11:
			{
				cost = 9;
			}
			case 12:
			{
				cost = 7;
			}
			case 14:
			{
				cost = 12;
			}
			case 15:
			{
				cost = 9;
			}
			case 16:
			{
				cost = 7;
			}
			case 17:
			{
				cost = 7;
			}
			case 18:
			{
				cost = 10;
			}
			case 19:
			{
				cost = 4;
			}
			case 21:
			{
				cost = 3;
			}
		}
		return cost;
	}
	return 0;
}

stock ClearFishes(playerid)
{
	if(IsPlayerConnected(playerid)) 
    {
        for(new i; i < MAX_FISH_BAG; i++)
        {
            Fishes[playerid][pFid][i] = 0;
            Fishes[playerid][pWeight][i] = 0;
        }
	}
	return 1;
}

stock ClearFishID(playerid, fish)
{
	if(IsPlayerConnected(playerid))
	{
        if(fish >= 0 && fish < MAX_FISH_BAG)
        {
            Fishes[playerid][pFid][fish] = 0;
            Fishes[playerid][pWeight][fish] = 0;
        }
	}
	return 1;
}

stock GetFisherLevel(playerid)
{
    new Level = PlayerInfo[playerid][pFishSkill];
    if(Level >= 0 && Level <= 50) return 1;
    else if(Level >= 51 && Level <= 100) return 2;
    else if(Level >= 101 && Level <= 200) return 3;
    else if(Level >= 201 && Level <= 400) return 4;
    else if(Level >= 401) return 5;
	
	return 0;
}

stock GetFisherLevelUp(playerid)
{
    new Level = PlayerInfo[playerid][pFishSkill];
    if(Level <= 50) return 51 - Level;
    else if(Level <= 100) return 101 - Level;
    else if(Level <= 200) return 201 - Level;
    else if(Level <= 400) return 401 - Level;
    else if(Level >= 401) return 0;

	return 0;
}

stock ShowFishermanDialog(playerid)
{
	ShowPlayerDialog(playerid, DIALOG_FISHERMAN, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Ethan Blake", "Mat hang\tGia\n{FFFFFF}Can cau ca\t{EFD639}$1,000\n{FFFFFF}Moi cau ca\t{EFD639}$10/moi cau", "Mua", "Thoat");	
	return 1;
}

alias:fishhelp("trogiupca")
CMD:fishhelp(playerid, params[])
{
    SendClientMessageEx(playerid, 0x63B8C5FF,"* [FISH]{FFFFFF} /cauca - de bat dau cau ca");
    SendClientMessageEx(playerid, 0x63B8C5FF,"* [FISH]{FFFFFF} /tuica - de xem ca trong tui (ban ca va vut ca)");
    return 1;
}

forward PlayerFishing(playerid);
public PlayerFishing(playerid)
{
	if(!IsPlayerConnected(playerid)) return 0;
	if(--FishingTime[playerid] > 0)
	{
		ApplyAnimation(playerid, "SAMP", "FishingIdle", 4.1, false, true, false, true, 0);
		SetTimerEx("PlayerFishing", 1000, false, "i", playerid);
		return 1;
	}

	FishingTime[playerid] = 0;
	IsFishing[playerid] = 0;
	TogglePlayerControllable(playerid, true);
	RemovePlayerAttachedObject(playerid, 9);
	ApplyAnimation(playerid, "CARRY", "crry_prtial", 2.0, false, false, false, false, 0);
	ClearAnimations(playerid);

	new Caught, rand;
	if(!IsPlayerInDynamicArea(playerid, FSpecialAreaID))
	{
		if(GetFisherLevel(playerid) == 1) { Caught = random(20)-7; }
		else if(GetFisherLevel(playerid) == 2) { Caught = random(50)-20; }
		else if(GetFisherLevel(playerid) == 3) { Caught = random(100)-50; }
		else if(GetFisherLevel(playerid) == 4) { Caught = random(160)-60; }
		else if(GetFisherLevel(playerid) == 5) { Caught = random(180)-70; }
	}
	else {
		if(GetFisherLevel(playerid) == 1) { Caught = random(20)-4; }
		else if(GetFisherLevel(playerid) == 2) { Caught = random(50)-13; }
		else if(GetFisherLevel(playerid) == 3) { Caught = random(100)-30; }
		else if(GetFisherLevel(playerid) == 4) { Caught = random(160)-40; }
		else if(GetFisherLevel(playerid) == 5) { Caught = random(180)-50; }
	}

	rand = random(sizeof(FishNames));
	if(Caught <= 0)
	{
		SendClientMessageEx(playerid, COLOR_GREY, "Khong cau duoc thu gi.");
		return 1;
	}
	else if(rand == 0)
	{
		SendClientMessageEx(playerid, COLOR_GREY, "Ban da cau duoc mot chiec ao va nem no di.");
		return 1;
	}
	else if(rand == 4)
	{
		SendClientMessageEx(playerid, COLOR_GREY, "Ban da cau duoc mot chiec quan dai va nem do di.");
		return 1;
	}
	else if(rand == 7)
	{
		SendClientMessageEx(playerid, COLOR_GREY, "Ban da cau duoc mot can xang va nem no di.");
		return 1;
	}
	else if(rand == 10)
	{
		SendClientMessageEx(playerid, COLOR_GREY, "Ban da cau duoc mot doi giay va nem no di.");
		return 1;
	}
	else if(rand == 13)
	{
		SendClientMessageEx(playerid, COLOR_GREY, "Ban da cau duoc mot tui rac va nem no di.");
		return 1;
	}
	else if(rand == 20)
	{
		new mrand = 1 + random(100);
		SendClientMessageEx(playerid, 0xB7F553FF, "Ban da cau duoc mot tui tien va co $%d.", mrand);
		GivePlayerCash(playerid, mrand);
		return 1;
	}

	new bool:hasFish;
	for(new i; i < MAX_FISH_BAG; i++)
	{
		if(Fishes[playerid][pWeight][i] == 0)
		{
			PlayerInfo[playerid][pFishes] += 1;

			new rand_xp = 0;
			if(DoubleXP) {
				rand_xp = Random(PlayerInfo[playerid][pLevel] * XP_RATE - 6, PlayerInfo[playerid][pLevel] * XP_RATE - 3);
				PlayerInfo[playerid][pFishSkill] += 2;
				SendClientMessageEx(playerid, COLOR_YELLOW, "Kinh nghiem nhan vat: +%d XP (%s/%s) | +2 Fisher Skill (DOUBLE XP)", rand_xp*2, number_format(PlayerInfo[playerid][pXP]+rand_xp*2), number_format(GetRequiredXP(playerid)));
				GivePlayerXP(playerid, rand_xp*2);
			}
			else if(PlayerInfo[playerid][pDoubleEXP] > 0 && !DoubleXP)
			{
				rand_xp = Random(PlayerInfo[playerid][pLevel] * XP_RATE - 6, PlayerInfo[playerid][pLevel] * XP_RATE - 3);
				PlayerInfo[playerid][pFishSkill] += 2;
				SendClientMessageEx(playerid, COLOR_YELLOW, "Kinh nghiem nhan vat: +%d XP (%s/%s) | +2 Fisher Skill (DOUBLE XP)", rand_xp*2, number_format(PlayerInfo[playerid][pXP]+rand_xp*2), number_format(GetRequiredXP(playerid)));
				GivePlayerXP(playerid, rand_xp*2);
			}
			else
			{
				rand_xp = Random(PlayerInfo[playerid][pLevel] * XP_RATE - 6, PlayerInfo[playerid][pLevel] * XP_RATE - 3);
				PlayerInfo[playerid][pFishSkill] += 1;
				SendClientMessageEx(playerid, COLOR_YELLOW, "Kinh nghiem nhan vat: +%d XP (%s/%s) | +1 Fisher Skill", rand_xp, number_format(PlayerInfo[playerid][pXP]+rand_xp), number_format(GetRequiredXP(playerid)));
				GivePlayerXP(playerid, rand_xp);
			}

			Fishes[playerid][pWeight][i] = Caught;
			SendClientMessageEx(playerid, 0xB7F553FF, "Ban da cau duoc mot con %s, can nang %d kilogram.", FishNames[rand], Caught);
			Fishes[playerid][pLastWeight] = Caught;
			Fishes[playerid][pLastFish] = i;
			Fishes[playerid][pFid][i] = rand;
			Fishes[playerid][pFishID] = rand;
			if(Caught > PlayerInfo[playerid][pBiggestFish])
			{
				SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* Ban vua dat ky luc moi khi cau duoc con ca nang %d kilogram (Ky luc truoc: %d kilogram).", Caught, PlayerInfo[playerid][pBiggestFish]);
				PlayerInfo[playerid][pBiggestFish] = Caught;
			}

			hasFish = true;
			break;
		}
	}

	if(hasFish == false) 
		return SendClientMessageEx(playerid, COLOR_GREY, "Tui ca cua ban da day, ban can ban mot so ca truoc khi bat them.");

	if(PlayerInfo[playerid][pFishSkill] == 50)
	{ SendClientMessageEx(playerid, COLOR_YELLOW, "* Ky nang cau ca cua ban dat cap do 2, ban co the bat duoc ca to hon."); }
	else if(PlayerInfo[playerid][pFishSkill] == 100)
	{ SendClientMessageEx(playerid, COLOR_YELLOW, "* Ky nang cau ca cua ban dat cap do 3, ban co the bat duoc ca to hon."); }
	else if(PlayerInfo[playerid][pFishSkill] == 200)
	{ SendClientMessageEx(playerid, COLOR_YELLOW, "* Ky nang cau ca cua ban dat cap do 4, ban co the bat duoc ca to hon."); }
	else if(PlayerInfo[playerid][pFishSkill] == 400)
	{ SendClientMessageEx(playerid, COLOR_YELLOW, "* Ky nang cau ca cua ban dat cap do 5, ban co the bat duoc ca to hon."); }
	return 1;
}

alias:fish("cauca")
CMD:fish(playerid, params[])
{
	if(!IsPlayerDiscordVerified(playerid)) 
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban phai lien ket tai khoan Discord de co the lam dieu nay.");

	if(IsFishing[playerid])
		return SendClientMessageEx(playerid, COLOR_GREY, "Ban dang cau ca, vui long cho den khi cau xong.");

	if(PlayerInfo[playerid][pFishRod] == 0)
		return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong co can cau ca de thuc hien dieu nay.");

	if(PlayerInfo[playerid][pFishes] > 10)
		return SendClientMessageEx(playerid, COLOR_GREY, "Ban da cau qua nhieu, ban can duoc nghi ngoi.");
	
	if(Fishes[playerid][pWeight][0] > 0 && Fishes[playerid][pWeight][1] > 0 && Fishes[playerid][pWeight][2] > 0 && Fishes[playerid][pWeight][3] > 0 && Fishes[playerid][pWeight][4] > 0)
		return SendClientMessageEx(playerid, COLOR_GREY, "Tui ca khong con cho trong, vui long ban no di.");

	if(IsPlayerInAnyVehicle(playerid))
		return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong the cau ca khi dang ngoi tren mot chiec xe.");
	
	new vehicleid = GetClosestCar(playerid, _, 3.0),
		Float:px, Float:py, Float:pz,
		Float:vx, Float:vy, Float:vz, bool:isBoat; 
	if(IsValidVehicle(vehicleid)) {
		GetVehiclePos(vehicleid, vx, vy, vz);
		GetPlayerPos(playerid, px, py, pz);
		if(vz < pz) isBoat = true;
	}

	if((IsAtFishPlace(playerid)) || (IsABoat(vehicleid) && isBoat))
	{
		new needBait = !IsPlayerInDynamicArea(playerid, FSpecialAreaID) ? 1 : 3;
		if(PlayerInfo[playerid][pBait] < needBait)
			return SendClientMessageEx(playerid, COLOR_GREY, "Ban can co nhieu hon %d moi cau de cau ca.", needBait);

		PlayerInfo[playerid][pBait] -= needBait;
		IsFishing[playerid] = 1;
		FishingTime[playerid] = Random(10, 15);
		TogglePlayerControllable(playerid, false);
		ApplyAnimation(playerid, "SWORD", "sword_block", 50.0, false, true, false, true, 1, SYNC_ALL);
		SetLoading(playerid, FishingTime[playerid]*1000, "Dang cau ca...");
		SetPlayerAttachedObject(playerid, 9, 18632, 6, 0.091999, 0.056999, 0.000000, -166.100051, 0.000000, 0.000000);
		SetTimerEx("PlayerFishing", 1200, false, "i", playerid);
	}
	else
	{
		SendClientMessageEx(playerid, COLOR_GREY, "Ban khong o diem cau ca hoac tren mot chiec thuyen danh ca.");
		return 1;
	}
	return 1;
}

alias:fishes("tuica")
CMD:fishes(playerid, params[])
{
    new dialogInfo[428];
    for(new i; i < MAX_FISH_BAG; i++)
    {
        if(Fishes[playerid][pWeight][i] > 0) 
        {
            format(dialogInfo, _, "%s%s: %d kg\n", dialogInfo, FishNames[Fishes[playerid][pFid][i]], Fishes[playerid][pWeight][i]);
        }
        else {
            strcat(dialogInfo, "Trong\n");
        }
    }

    ShowPlayerDialog(playerid, DIALOG_FISHES, DIALOG_STYLE_LIST, "Tui ca cua ban", dialogInfo, "Chon", "Dong");
	return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_FISHERMAN && response)
	{
		if(!IsPlayerDiscordVerified(playerid)) 
            return SendClientMessageEx(playerid, COLOR_GREY, "Ban phai lien ket tai khoan Discord de co the lam dieu nay.");

		if(listitem == 0)
		{
			if(PlayerInfo[playerid][pFishRod])
				return SendClientMessageEx(playerid, COLOR_GREY, "Ban da so huu can cau ca roi!");

			if(GetPlayerCash(playerid) < 1000)
				return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong du tien de mua can cau ca.");

			SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "Ban da mua can cau ca voi gia $1,000.");
			GivePlayerCash(playerid, -1000);
			PlayerInfo[playerid][pFishRod] = 1;
		}
		else if(listitem == 1)
		{
			ShowPlayerDialog(playerid, DIALOG_BUYBAIT, DIALOG_STYLE_INPUT, "Ethan Blake > Moi cau ca", "{FFFFFF}Nhap so luong moi cau ban muon mua:", "Mua", "Thoat");
		}

		return 1;
	}

	if(dialogid == DIALOG_BUYBAIT && response)
	{
		if(!IsNumeric(inputtext))
			return ShowPlayerDialog(playerid, DIALOG_BUYBAIT, DIALOG_STYLE_INPUT, "Ethan Blake > Moi cau ca", "{E32C2C} Ban can phai nhap mot con so!\n\n{FFFFFF}Nhap so luong moi cau ban muon mua:", "Mua", "Thoat");

		new amount = strval(inputtext);
		if(amount <= 0)
			return ShowPlayerDialog(playerid, DIALOG_BUYBAIT, DIALOG_STYLE_INPUT, "Ethan Blake > Moi cau ca", "{E32C2C} So luong khong hop le!\n\n{FFFFFF}Nhap so luong moi cau ban muon mua:", "Mua", "Thoat");
	
		if(GetPlayerCash(playerid) < amount * 10)
			return ShowPlayerDialog(playerid, DIALOG_BUYBAIT, DIALOG_STYLE_INPUT, "Ethan Blake > Moi cau ca", "{E32C2C} Ban khong du tien de mua %d moi cau!\n\n{FFFFFF}Nhap so luong moi cau ban muon mua:", "Mua", "Thoat", amount);
	
		if(PlayerInfo[playerid][pBait] + amount > 50)
			return ShowPlayerDialog(playerid, DIALOG_BUYBAIT, DIALOG_STYLE_INPUT, "Ethan Blake > Moi cau ca", "{E32C2C} Ban khong the so huu nhieu hon 50 moi cau!\n\n{FFFFFF}Nhap so luong moi cau ban muon mua:", "Mua", "Thoat", amount);
	
		SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "Ban da mua %d moi cau voi gia $%d.", amount, amount * 10);
		GivePlayerCash(playerid, -(amount * 10));
		PlayerInfo[playerid][pBait] += amount;
		return 1;
	}

    if(dialogid == DIALOG_FISHES && response)
    {
        if(Fishes[playerid][pWeight][listitem] <= 0)
            return PC_EmulateCommand(playerid, "/fishes");

        SetPVarInt(playerid, "FishID", listitem);
        ShowPlayerDialog(playerid, DIALOG_FISHESOPTIONS, DIALOG_STYLE_TABLIST_HEADERS, "Tui ca cua ban", "{EAE72F}%s\t{4DF8B1}%d kg\n{FFFFFF}Ban ca\n{D13A3A}Vut ca", "Chon", "Dong", FishNames[Fishes[playerid][pFid][listitem]], Fishes[playerid][pWeight][listitem]);
    }

    if(dialogid == DIALOG_FISHESOPTIONS)
    {
        if(!response) return PC_EmulateCommand(playerid, "/fishes");

		if(!IsPlayerDiscordVerified(playerid)) 
            return SendClientMessageEx(playerid, COLOR_GREY, "Ban phai lien ket tai khoan Discord de co the lam dieu nay.");
			
        new fishid = GetPVarInt(playerid, "FishID"), price;
        if(Fishes[playerid][pWeight][fishid] <= 0)
            return PC_EmulateCommand(playerid, "/fishes");

        if(listitem == 0) {
            if (!IsAt247(playerid))
            {
                SendClientMessageEx(playerid, COLOR_GREY, "Ban phai o trong Cua hang 24/7!");
                return 1;
            }

            price = GetFishCost(playerid, Fishes[playerid][pFid][fishid]);
            price = price * Fishes[playerid][pWeight][fishid];

            SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "Ban da ban con %s (%d KG) voi gia $%s.", FishNames[Fishes[playerid][pFid][fishid]], Fishes[playerid][pWeight][fishid], number_format(price));
            GivePlayerCash(playerid, price);
            ClearFishID(playerid, fishid);
            
            Fishes[playerid][pLastFish] = 0;
            Fishes[playerid][pFishID] = 0;
            PC_EmulateCommand(playerid, "/fishes");
        }
        else if(listitem == 1) {
            SendClientMessageEx(playerid, COLOR_GREY, "Ban da vut bo con \"%s\".", FishNames[Fishes[playerid][pFid][fishid]]);
            ClearFishID(playerid, fishid);
            Fishes[playerid][pLastFish] = 0;
            Fishes[playerid][pFishID] = 0;
            PC_EmulateCommand(playerid, "/fishes");
        }
    }
    return 1;
}

hook OnGameModeInit()
{
	FSpecialZoneID = GangZoneCreate(SpecialFishZone[0], SpecialFishZone[1], SpecialFishZone[2], SpecialFishZone[3]);
	FSpecialAreaID = CreateDynamicRectangle(SpecialFishZone[0], SpecialFishZone[1], SpecialFishZone[2], SpecialFishZone[3], 0, 0);
	return 1;
}

hook OnPlayerConnect(playerid)
{
	GangZoneShowForPlayer(playerid, FSpecialZoneID, 0x00ACB2FF);
	return 1;
}