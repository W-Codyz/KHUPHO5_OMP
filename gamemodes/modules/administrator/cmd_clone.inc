// Phien ban "im lang" cua cac lenh admin quan trong.
// Khong broadcast cho toan bo nguoi choi — chi gui xac nhan rieng cho admin va ghi log.

CMD:ssetmoney(playerid, params[])
{
    if (PlayerInfo[playerid][pAdmin] >= 1338)
    {
        new string[1024], giveplayerid, money, reason[64];
        if(sscanf(params, "uds[64]", giveplayerid, money, reason)) return SendHelpMessage(playerid, "/ssetmoney [player] [money] [reason]");
        if(IsPlayerConnected(giveplayerid))
        {
            ResetPlayerCash(giveplayerid);
            GivePlayerCash(giveplayerid, money);
            format(string, sizeof(string), "{FF8000}[SILENT SET TIEN] {FFFFFF}Da set tien {FFFF00}%s (ID:%d){FFFFFF} thanh {00FF00}$%s {FFFFFF}| Ly do: {FFFF00}%s",
                GetPlayerNameEx(giveplayerid), giveplayerid, number_format(money), reason);
            SendClientMessageEx(playerid, COLOR_WHITE, string);
            format(string, sizeof(string), "[SSETMONEY] Admin: %s | Player: %s | Money: $%s | Reason: %s",
                GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid), number_format(money), reason);
            Log("logs/stats.log", string);
        }
    }
    else SendUnAuth(playerid);
    return 1;
}

CMD:sgivemoney(playerid, params[])
{
    if (PlayerInfo[playerid][pAdmin] >= 4)
    {
        new string[1024], giveplayerid, money, reason[64];
        if(sscanf(params, "uds[64]", giveplayerid, money, reason)) return SendHelpMessage(playerid, "/sgivemoney [player] [money] [reason]");
        if(money < 1) return SendClientMessageEx(playerid, COLOR_GRAD1, "You cannot give less than $1!");
        if(IsPlayerConnected(giveplayerid))
        {
            GivePlayerCash(giveplayerid, money);
            format(string, sizeof(string), "{FF8000}[SILENT CHO TIEN] {FFFFFF}Da cho {FFFF00}%s (ID:%d){FFFFFF} so tien {00FF00}$%s {FFFFFF}| Ly do: {FFFF00}%s",
                GetPlayerNameEx(giveplayerid), giveplayerid, number_format(money), reason);
            SendClientMessageEx(playerid, COLOR_WHITE, string);
            format(string, sizeof(string), "[SGIVEMONEY] Admin: %s | Player: %s | Money: $%s | Reason: %s",
                GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid), number_format(money), reason);
            Log("logs/stats.log", string);
        }
    }
    else SendUnAuth(playerid);
    return 1;
}

CMD:ssetstat(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] >= 4)
    {
        new string[1024], giveplayerid, statcode, amount, reason[64];
        if(sscanf(params, "udds[64]", giveplayerid, statcode, amount, reason))
        {
            SendHelpMessage(playerid, "/ssetstat [player] [statcode] [amount] [reason]");
            SendClientMessageEx(playerid, COLOR_GRAD4, "|1 Level |2 ArmorUpgrade |3 UpgradePoints |4 Model |5 BankAccount |6 PhoneNumber |7 RespectPoints |8 House1 |9 House2");
            SendClientMessageEx(playerid, COLOR_GRAD2, "|11 FMember |12 Trucker |13 Materials |14 Pot |15 Crack |16 Fishing |17 Job |18 Rank |19 Packages |20 Insurance");
            SendClientMessageEx(playerid, COLOR_GRAD2, "|21 Warnings |22 Screwdriver |23 Gender |24 NMute |25 AdMute |26 Faction |27 Restricted Weapon Time |28 Gang Warns");
            SendClientMessageEx(playerid, COLOR_GRAD1, "|29 RMute |30 Reward Hours |31 Playing Hours |32 Gold Box Tokens |33 Computer Drawings |34 Papers |35 Business");
            SendClientMessageEx(playerid, COLOR_GRAD1, "|36 BusinessRank |37 Spraycan |38 Heroin |39 RawOpium |40 Syringes |41 Hunger |42 Fitness |43 Event Tokens |44 Modkit");
            return 1;
        }

        if(IsPlayerConnected(giveplayerid))
        {
            switch (statcode)
            {
            case 1:
                {
                    PlayerInfo[giveplayerid][pLevel] = amount;
                    PlayerInfo[giveplayerid][pXP] = 0;
                    format(string, sizeof(string), "   %s's Level has been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                    SetPlayerScore(giveplayerid, PlayerInfo[giveplayerid][pLevel]);
                }
            case 2:
                {
                    PlayerInfo[giveplayerid][pSHealth] = amount;
                    format(string, sizeof(string), "   %s's ArmorUpgrade has been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 3:
                {
                    PlayerInfo[giveplayerid][gPupgrade] = amount;
                    format(string, sizeof(string), "   %s's Upgrade Points has been set %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 4:
                {
                    PlayerInfo[giveplayerid][pModel] = amount;
                    format(string, sizeof(string), "   %s's Model has been set %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 5:
                {
                    PlayerInfo[giveplayerid][pAccount] = amount;
                    format(string, sizeof(string), "   %s's Bank Account has been set to $%d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 6:
                {
                    if(amount == 0)
                    {
                        PlayerInfo[giveplayerid][pPnumber] = amount;
                        format(string, sizeof(string), "   %s had their phone removed", GetPlayerNameEx(giveplayerid));
                    }
                    else
                    {
                        new query[128];
                        SetPVarInt(giveplayerid, "WantedPh", amount);
                        SetPVarInt(giveplayerid, "CurrentPh", PlayerInfo[giveplayerid][pPnumber]);
                        SetPVarInt(giveplayerid, "PhChangeCost", 50000);
                        SetPVarInt(giveplayerid, "PhChangerId", playerid);
                        format(query, sizeof(query), "SELECT `Username` FROM `accounts` WHERE `PhoneNr` = '%d'", amount);
                        mysql_function_query(MainPipeline, query, true, "OnPhoneNumberCheck", "ii", giveplayerid, 4);
                        return 1;
                    }
                }
            case 7:
                {
                    PlayerInfo[giveplayerid][pExp] = amount;
                    format(string, sizeof(string), "   %s's Respect Points have been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 8:
                {
                    PlayerInfo[giveplayerid][pPhousekey] = amount;
                    format(string, sizeof(string), "   %s's House 1 has been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 9:
                {
                    PlayerInfo[giveplayerid][pPhousekey2] = amount;
                    format(string, sizeof(string), "   %s's House 2 has been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 11:
                {
                    PlayerInfo[giveplayerid][pFMember] = amount;
                    format(string, sizeof(string), "   %s's Family Membership has been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 12:
                {
                    PlayerInfo[playerid][pTruckSkill] = amount;
                    format(string, sizeof(string), "   %s's Truck Skill has been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 13:
                {
                    PlayerInfo[giveplayerid][pMats] = amount;
                    format(string, sizeof(string), "   %s's Materials have been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 14:
                {
                    PlayerInfo[giveplayerid][pPot] = amount;
                    format(string, sizeof(string), "   %s's Pot has been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 15:
                {
                    PlayerInfo[giveplayerid][pCrack] = amount;
                    format(string, sizeof(string), "   %s's Crack has been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 16:
                {
                    PlayerInfo[giveplayerid][pFishSkill] = amount;
                    format(string, sizeof(string), "   %s's Fishing Skill has been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 17:
                {
                    PlayerInfo[giveplayerid][pJob] = amount;
                    format(string, sizeof(string), "   %s's Job has been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 18:
                {
                    PlayerInfo[giveplayerid][pRank] = amount;
                    format(string, sizeof(string), "   %s's Rank has been to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 19:
                {
                    SetPVarInt(playerid, "Packages", amount);
                    format(string, sizeof(string), "   %s's Materials Packages have been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 20:
                {
                    PlayerInfo[giveplayerid][pInsurance] = amount;
                    format(string, sizeof(string), "   %s's Insurance has been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 21:
                {
                    PlayerInfo[giveplayerid][pWarns] = amount;
                    format(string, sizeof(string), "   %s's Warnings have been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 22:
                {
                    PlayerInfo[giveplayerid][pScrewdriver] = amount;
                    format(string, sizeof(string), "   %s's Screwdrivers have been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 23:
                {
                    PlayerInfo[giveplayerid][pSex] = amount;
                    format(string, sizeof(string), "   %s's Gender has been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 24:
                {
                    PlayerInfo[giveplayerid][pNMuteTotal] = amount;
                    format(string, sizeof(string), "   %s's Total Newbie Mutes has been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 25:
                {
                    PlayerInfo[giveplayerid][pADMuteTotal] = amount;
                    format(string, sizeof(string), "   %s's Total Ad Mutes has been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 26:
                {
                    PlayerInfo[giveplayerid][pMember] = amount;
                    format(string, sizeof(string), "   %s's Faction has been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 27:
                {
                    if(PlayerInfo[giveplayerid][pConnectHours] >= 2) {
                        PlayerInfo[giveplayerid][pWRestricted] = amount;
                        format(string, sizeof(string), "   %s's Weapon Restricted Time has been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                    }
                    else return SendClientMessageEx(playerid, COLOR_GREY, "You cannot set this on a person who has under 2 playing hours.");
                }
            case 28:
                {
                    PlayerInfo[giveplayerid][pGangWarn] = amount;
                    format(string, sizeof(string), "   %s's Gang Warns have been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 29:
                {
                    PlayerInfo[giveplayerid][pRMutedTotal] = amount;
                    format(string, sizeof(string), "   %s's Report Mutes have been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 30:
                {
                    PlayerInfo[giveplayerid][pRewardHours] = amount;
                    format(string, sizeof(string), "   %s's Reward Playing Hours have been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 31:
                {
                    PlayerInfo[giveplayerid][pConnectHours] = amount;
                    format(string, sizeof(string), "   %s's Playing Hours have been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 32:
                {
                    PlayerInfo[giveplayerid][pGoldBoxTokens] = amount;
                    format(string, sizeof(string), "   %s's Box Tokens have been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 33:
                {
                    PlayerInfo[giveplayerid][pRewardDrawChance] = amount;
                    format(string, sizeof(string), "   %s's Computer Drawings have been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 34:
                {
                    PlayerInfo[giveplayerid][pPaper] = amount;
                    format(string, sizeof(string), "   %s's Papers have been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 35:
                {
                    if (amount < 0 || amount >= MAX_BUSINESSES) return 1;
                    PlayerInfo[giveplayerid][pBusiness] = amount;
                    format(string, sizeof(string), "   %s's Business have been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 36:
                {
                    if (amount < 0 || amount > 5) return 1;
                    PlayerInfo[giveplayerid][pBusinessRank] = amount;
                    format(string, sizeof(string), "   %s's Business Rank have been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 37:
                {
                    PlayerInfo[giveplayerid][pSpraycan] = amount;
                    format(string, sizeof(string), "   %s's Spraycans have been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 38:
                {
                    PlayerInfo[giveplayerid][pHeroin] = amount;
                    format(string, sizeof(string), "   %s's Heroin have been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 39:
                {
                    PlayerInfo[giveplayerid][pRawOpium] = amount;
                    format(string, sizeof(string), "   %s's raw opium have been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 40:
                {
                    PlayerInfo[giveplayerid][pSyringes] = amount;
                    format(string, sizeof(string), "   %s's Syringes have been set to %d.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 41:
                {
                    if (amount <= 0) amount = 1;
                    else if (amount > 100) amount = 100;
                    PlayerInfo[giveplayerid][pHunger] = amount;
                    format(string, sizeof(string), "   %s's Hunger has been set to %i.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 42:
                {
                    PlayerInfo[giveplayerid][pFitness] = amount;
                    format(string, sizeof(string), "   %s's Fitness has been set to %i.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 43:
                {
                    PlayerInfo[giveplayerid][pTrickortreat] = amount;
                    format(string, sizeof(string), "   %s's Event Tokens have been set to %i.", GetPlayerNameEx(giveplayerid), amount);
                }
            case 44:
                {
                    PlayerInfo[giveplayerid][pRimMod] = amount;
                    format(string, sizeof(string), "   %s's Rim Kits have been set to %i.", GetPlayerNameEx(giveplayerid), amount);
                }
            default:
                {
                    format(string, sizeof(string), "   Code khong hop le.");
                }
            }

            format(string, sizeof(string), "[SILENT] %s by %s | Reason: %s", string, GetPlayerNameEx(playerid), reason);
            Log("logs/stats.log", string);
            SendClientMessageEx(playerid, COLOR_GRAD1, string);
        }
    }
    return 1;
}

alias:screatecar("ssetcar")
CMD:screatecar(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] >= 4)
    {
        new iColors[2], iTargetID, iModelID, reason[64];
        if(sscanf(params, "uiiis[64]", iTargetID, iModelID, iColors[0], iColors[1], reason)) return SendHelpMessage(playerid, "/ssetcar [player] [model] [color1] [color2] [reason]");
        if(!(400 <= iModelID <= 611)) return SendClientMessageEx(playerid, COLOR_GRAD2, "Invalid model (400-611).");
        if(IsATrain(iModelID)) return SendClientMessageEx(playerid, COLOR_GREY, "Trains cannot be spawned.");
        if(!(0 <= iColors[0] <= 255 && 0 <= iColors[1] <= 255)) return SendClientMessageEx(playerid, COLOR_GRAD2, "Invalid color (0-255).");
        if(!vehicleCountCheck(iTargetID)) return SendClientMessageEx(playerid, COLOR_GREY, "That player owns too many vehicles.");
        if(!vehicleSpawnCountCheck(iTargetID)) return SendClientMessageEx(playerid, COLOR_GREY, "That player has too many vehicles spawned.");

        new Float: arr_fPlayerPos[4], szMessage[1024];
        GetPlayerPos(iTargetID, arr_fPlayerPos[0], arr_fPlayerPos[1], arr_fPlayerPos[2]);
        GetPlayerFacingAngle(iTargetID, arr_fPlayerPos[3]);
        CreatePlayerVehicle(iTargetID, GetPlayerFreeVehicleId(iTargetID), iModelID, arr_fPlayerPos[0], arr_fPlayerPos[1], arr_fPlayerPos[2], arr_fPlayerPos[3], iColors[0], iColors[1], 2000000, GetPlayerVirtualWorld(iTargetID), GetPlayerInterior(iTargetID));

        format(szMessage, sizeof(szMessage), "{FF8000}[SILENT TAO XE] {FFFFFF}Da tao xe {00FF00}%s{FFFFFF} cho {FFFF00}%s (ID:%d){FFFFFF} | Ly do: {FFFF00}%s",
            VehicleName[iModelID - 400], GetPlayerNameEx(iTargetID), iTargetID, reason);
        SendClientMessageEx(playerid, COLOR_WHITE, szMessage);

        format(szMessage, sizeof(szMessage), "[SCREATECAR] Admin: %s | Player: %s | Model: %s (%d) | Reason: %s",
            GetPlayerNameEx(playerid), GetPlayerNameEx(iTargetID), VehicleName[iModelID - 400], iModelID, reason);
        Log("logs/playervehicle.log", szMessage);
    }
    else SendClientMessageEx(playerid, COLOR_GREY, "You are not allowed to use this command.");
    return 1;
}

CMD:sremovepvehicle(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] >= 4)
    {
        new giveplayerid, reason[64];
        if(sscanf(params, "us[64]", giveplayerid, reason)) return SendHelpMessage(playerid, "/sremovepvehicle [player] [reason]");
        if(!IsPlayerConnected(giveplayerid)) return SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi khong ket noi.");

        new vstring[1024];
        for(new i, iModelID; i < GetPlayerVehicleSlots(giveplayerid); i++) {
            if((iModelID = PlayerVehicleInfo[giveplayerid][i][pvModelId] - 400) >= 0) {
                if(PlayerVehicleInfo[giveplayerid][i][pvImpounded])
                    format(vstring, sizeof(vstring), "%s\n%s (impounded)", vstring, VehicleName[iModelID]);
                else if(PlayerVehicleInfo[giveplayerid][i][pvDisabled])
                    format(vstring, sizeof(vstring), "%s\n%s (disabled)", vstring, VehicleName[iModelID]);
                else if(!PlayerVehicleInfo[giveplayerid][i][pvSpawned])
                    format(vstring, sizeof(vstring), "%s\n%s (stored)", vstring, VehicleName[iModelID]);
                else
                    format(vstring, sizeof(vstring), "%s\n%s (spawned)", vstring, VehicleName[iModelID]);
            }
            else strcat(vstring, "\nEmpty");
        }
        if(vstring[0] == '\n') strdel(vstring, 0, 1);

        new title[64];
        SetPVarInt(playerid, "vehcheck_giveplayerid", giveplayerid);
        SetPVarString(playerid, "vehcheck_reason", reason);
        SetPVarInt(playerid, "vehcheck_silent", 1);
        format(title, sizeof(title), "%s's Vehicle List", GetPlayerNameEx(giveplayerid));
        ShowPlayerDialog(playerid, ADMIN_VEHCHECK, DIALOG_STYLE_LIST, title, vstring, "Delete Car", "Cancel");
    }
    else SendClientMessageEx(playerid, COLOR_GREY, "You are not allowed to use this command.");
    return 1;
}

CMD:ssetcredits(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 99999 && PlayerInfo[playerid][pShopTech] != 3)
        return 0;

    new szMessage[1024], Player, Amount, reason[64];
    if(sscanf(params, "uds[64]", Player, Amount, reason)) return SendHelpMessage(playerid, "/ssetcredits [Player] [Amount] [Reason]");
    if(!IsPlayerConnected(Player)) return SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi khong hop le.");

    PlayerInfo[Player][pCredits] = Amount;

    format(szMessage, sizeof(szMessage), "UPDATE `accounts` SET `Credits`=%d WHERE `id` = %d", PlayerInfo[Player][pCredits], GetPlayerSQLId(Player));
    mysql_function_query(MainPipeline, szMessage, false, "OnQueryFinish", "ii", SENDDATA_THREAD, Player);

    format(szMessage, sizeof(szMessage), "[SSETCREDITS] Admin: %s | Player: %s (%d) | IP: %s | Credits: %s | Reason: %s",
        GetPlayerNameEx(playerid), GetPlayerNameEx(Player), GetPlayerSQLId(Player), GetPlayerIpEx(Player), number_format(PlayerInfo[Player][pCredits]), reason);
    Log("logs/credits.log", szMessage), print(szMessage);

    format(szMessage, sizeof(szMessage), "{FF8000}[SILENT SET CREDITS] {FFFFFF}Da set credits {FFFF00}%s (ID:%d){FFFFFF} thanh {00FF00}%s {FFFFFF}| Ly do: {FFFF00}%s",
        GetPlayerNameEx(Player), Player, number_format(PlayerInfo[Player][pCredits]), reason);
    SendClientMessageEx(playerid, COLOR_WHITE, szMessage);
    return 1;
}

CMD:ssetvip(playerid, params[])
{
    if (PlayerInfo[playerid][pAdmin] >= 1337 || PlayerInfo[playerid][pShopTech] >= 1)
    {
        new string[1024], giveplayerid, level, months, orderid[32], reason[64];
        if(sscanf(params, "udds[32]s[64]", giveplayerid, level, months, orderid, reason))
        {
            SendHelpMessage(playerid, "/ssetvip [player] [level] [months] [orderID] [reason]");
            SendClientMessageEx(playerid, COLOR_GRAD3, "Levels: |0| None |1| Bronze |2| Silver |4| Platinum |5| Moderator");
            return 1;
        }

        if(!IsPlayerConnected(giveplayerid)) return SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi khong ket noi.");
        if(level < 0 || level > 5) return SendClientMessageEx(playerid, COLOR_GRAD1, "VIP Level must be 0-5 (excluding 3)!");
        if(level == 3) return SendClientMessageEx(playerid, COLOR_GRAD1, "VIP Level 3 (Gold) must use /newgvip or /renewgvip.");

        new playerip[32];
        GetPlayerIp(giveplayerid, playerip, sizeof(playerip));

        PlayerInfo[giveplayerid][pDonateRank] = level;
        PlayerInfo[giveplayerid][pTempVIP] = 0;
        PlayerInfo[giveplayerid][pBuddyInvited] = 0;
        PlayerInfo[giveplayerid][pVIPSellable] = 0;
        LoadPlayerDisabledVehicles(giveplayerid);

        if(level == 0)
        {
            PlayerInfo[giveplayerid][pTokens] = 0;
            format(string, sizeof(string), "{FF8000}[SILENT VIP] {FFFFFF}Da set VIP {FFFF00}%s (ID:%d){FFFFFF} thanh {FF4444}None{FFFFFF} | Ly do: {FFFF00}%s",
                GetPlayerNameEx(giveplayerid), giveplayerid, reason);
            SendClientMessageEx(playerid, COLOR_WHITE, string);
            format(string, sizeof(string), "Your VIP level has been set to None by Admin %s.", GetPlayerNameEx(playerid));
            SendClientMessageEx(giveplayerid, COLOR_WHITE, string);
            format(string, sizeof(string), "[SSETVIP] Admin: %s | Player: %s (IP:%s) | VIP: None | Order: %s | Reason: %s",
                GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid), playerip, orderid, reason);
            Log("logs/setvip.log", string);
            Misc_Save();
            return 1;
        }

        if(PlayerInfo[giveplayerid][pVIPM] == 0)
        {
            PlayerInfo[giveplayerid][pVIPM] = VIPM;
            VIPM++;
        }
        PlayerInfo[giveplayerid][pVIPExpire] = gettime() + 2592000 * months;

        if(level == 1)
        {
            format(string, sizeof(string), "{FF8000}[SILENT VIP] {FFFFFF}Da set VIP {FFFF00}%s (ID:%d){FFFFFF} thanh {CD7F32}Bronze{FFFFFF} (%d thang) | Ly do: {FFFF00}%s",
                GetPlayerNameEx(giveplayerid), giveplayerid, months, reason);
            SendClientMessageEx(playerid, COLOR_WHITE, string);
            format(string, sizeof(string), "Your VIP level has been set to Bronze (%d months) by Admin %s.", months, GetPlayerNameEx(playerid));
            SendClientMessageEx(giveplayerid, COLOR_WHITE, string);
            format(string, sizeof(string), "[SSETVIP] Admin: %s | Player: %s (IP:%s) | VIP: Bronze (%d months) | Order: %s | Reason: %s",
                GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid), playerip, months, orderid, reason);
            Log("logs/setvip.log", string);
        }
        else if(level == 2)
        {
            format(string, sizeof(string), "{FF8000}[SILENT VIP] {FFFFFF}Da set VIP {FFFF00}%s (ID:%d){FFFFFF} thanh {C0C0C0}Silver{FFFFFF} (%d thang) | Ly do: {FFFF00}%s",
                GetPlayerNameEx(giveplayerid), giveplayerid, months, reason);
            SendClientMessageEx(playerid, COLOR_WHITE, string);
            format(string, sizeof(string), "Your VIP level has been set to Silver (%d months) by Admin %s.", months, GetPlayerNameEx(playerid));
            SendClientMessageEx(giveplayerid, COLOR_WHITE, string);
            format(string, sizeof(string), "[SSETVIP] Admin: %s | Player: %s (IP:%s) | VIP: Silver (%d months) | Order: %s | Reason: %s",
                GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid), playerip, months, orderid, reason);
            Log("logs/setvip.log", string);
        }
        else if(level == 4)
        {
            format(string, sizeof(string), "{FF8000}[SILENT VIP] {FFFFFF}Da set VIP {FFFF00}%s (ID:%d){FFFFFF} thanh {E5E4E2}Platinum{FFFFFF} (%d thang) | Ly do: {FFFF00}%s",
                GetPlayerNameEx(giveplayerid), giveplayerid, months, reason);
            SendClientMessageEx(playerid, COLOR_WHITE, string);
            format(string, sizeof(string), "Your VIP level has been set to Platinum (%d months) by Admin %s.", months, GetPlayerNameEx(playerid));
            SendClientMessageEx(giveplayerid, COLOR_WHITE, string);
            format(string, sizeof(string), "[SSETVIP] Admin: %s | Player: %s (IP:%s) | VIP: Platinum (%d months) | Order: %s | Reason: %s",
                GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid), playerip, months, orderid, reason);
            Log("logs/setvip.log", string);
        }
        else if(level == 5)
        {
            format(string, sizeof(string), "{FF8000}[SILENT VIP] {FFFFFF}Da set VIP {FFFF00}%s (ID:%d){FFFFFF} thanh {9B59B6}Moderator{FFFFFF} (%d thang) | Ly do: {FFFF00}%s",
                GetPlayerNameEx(giveplayerid), giveplayerid, months, reason);
            SendClientMessageEx(playerid, COLOR_WHITE, string);
            format(string, sizeof(string), "Your VIP level has been set to Moderator (%d months) by Admin %s.", months, GetPlayerNameEx(playerid));
            SendClientMessageEx(giveplayerid, COLOR_WHITE, string);
            format(string, sizeof(string), "[SSETVIP] Admin: %s | Player: %s (IP:%s) | VIP: Moderator (%d months) | Order: %s | Reason: %s",
                GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid), playerip, months, orderid, reason);
            Log("logs/setvip.log", string);
        }
        Misc_Save();
    }
    else SendUnAuth(playerid);
    return 1;
}
