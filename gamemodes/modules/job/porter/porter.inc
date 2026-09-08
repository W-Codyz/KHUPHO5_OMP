#include <YSI\YSI_Coding\y_hooks>

// step 0: load
// step 1: unload
forward PorterAnimation(playerid, step);
public PorterAnimation(playerid, step)
{
    if(!step) {
        SetPlayerAttachedObject(playerid, 9, 2060, 1, 0.051000, 0.424000, -0.022000, -22.600000, -84.000000, -19.500000);
    	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
		gPlayerCheckpointStatus[playerid] = CHECKPOINT_PORTERUNLOAD;
		SetPlayerCP(playerid, PorterUnload[0], PorterUnload[1], PorterUnload[2], 2.0, false);
    }
    else {
        new randCp = random(3);
        SetPlayerCP(playerid, PorterLoad[randCp][0], PorterLoad[randCp][1], PorterLoad[randCp][2], 2.0, false);
        gPlayerCheckpointStatus[playerid] = CHECKPOINT_PORTERLOAD;  
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);

        new modelid;
        GetPlayerAttachedObject(playerid, 9, modelid);
        if(modelid == 2060) RemovePlayerAttachedObject(playerid, 9);

        new payment;
        switch(GetJobLevel(playerid, JOB_PORTER))
        {
            case 1: payment = 600;
            case 2: payment = 700;
            case 3: payment = 800;
            case 4: payment = 900;
            case 5: payment = 1000;
        }

        payment = Random(payment * 90 / 100, payment * 110 / 100);
        GivePlayerCash(playerid, payment);
        UpdateNewbieMission(playerid, 3);

        new rand_xp = 0;
        if(DoubleXP) {
            rand_xp = Random(PlayerInfo[playerid][pLevel] * XP_RATE - 5, PlayerInfo[playerid][pLevel] * XP_RATE - 2);
            SendClientMessageEx(playerid, COLOR_YELLOW, "Kinh nghiem nhan vat: +%d XP (%s/%s) | +2 Porter Skill (DOUBLE XP)", rand_xp*2, number_format(PlayerInfo[playerid][pXP]+rand_xp*2), number_format(GetRequiredXP(playerid)));
            GivePlayerXP(playerid, rand_xp*2);
            PlayerInfo[playerid][pPorterSkill] += 2;
        }
        else if(PlayerInfo[playerid][pDoubleEXP] > 0 && !DoubleXP)
        {
            rand_xp = Random(PlayerInfo[playerid][pLevel] * XP_RATE - 5, PlayerInfo[playerid][pLevel] * XP_RATE - 2);
            SendClientMessageEx(playerid, COLOR_YELLOW, "Kinh nghiem nhan vat: +%d XP (%s/%s) | +2 Porter Skill (DOUBLE XP)", rand_xp*2, number_format(PlayerInfo[playerid][pXP]+rand_xp*2), number_format(GetRequiredXP(playerid)));
            GivePlayerXP(playerid, rand_xp*2);
            PlayerInfo[playerid][pPorterSkill] += 2;
        }
        else
        {
            rand_xp = Random(PlayerInfo[playerid][pLevel] * XP_RATE - 5, PlayerInfo[playerid][pLevel] * XP_RATE - 2);
            SendClientMessageEx(playerid, COLOR_YELLOW, "Kinh nghiem nhan vat: +%d XP (%s/%s) | +1 Porter Skill", rand_xp, number_format(PlayerInfo[playerid][pXP]+rand_xp), number_format(GetRequiredXP(playerid)));
            GivePlayerXP(playerid, rand_xp);
            PlayerInfo[playerid][pPorterSkill] += 1;
        }

        new title[64];
        format(title, _, "%s (ID: %d) - bóc vác", GetPlayerNameEx(playerid), playerid);
        DiscordLog(dclog_Porter, title, "**Tiền nhận được: $%d**\n**Kinh nghiệm nhân vật: +%d XP**", payment, rand_xp);
    }
    return 1;
}

Porter_Start(playerid)
{
    new randCp = random(3);
    SetPlayerCP(playerid, PorterLoad[randCp][0], PorterLoad[randCp][1], PorterLoad[randCp][2], 2.0, false);
    gPlayerCheckpointStatus[playerid] = CHECKPOINT_PORTERLOAD;
    PlayerPorter[playerid] = 1;
    return 1;
}

Porter_Stop(playerid)
{
    if(PlayerPorter[playerid] == 0) return 0;

    DisablePlayerCP(playerid);
    gPlayerCheckpointStatus[playerid] = CHECKPOINT_NONE;
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
    PlayerPorter[playerid] = 0;

    new modelid;
    GetPlayerAttachedObject(playerid, 9, modelid);
    if(modelid == 2060) RemovePlayerAttachedObject(playerid, 9);
    return 1;
}

hook OnPlayerEnterCheckpoint(playerid)
{
    if(gPlayerCheckpointStatus[playerid] == CHECKPOINT_PORTERLOAD)
    {
        if(PlayerPorter[playerid] == 0) {
            new modelid;
            GetPlayerAttachedObject(playerid, 9, modelid);
            if(modelid == 2060) RemovePlayerAttachedObject(playerid, 9);

            DisablePlayerCP(playerid);
            gPlayerCheckpointStatus[playerid] = CHECKPOINT_NONE;
            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
            PlayerPorter[playerid] = 0;
            SendClientMessage(playerid, COLOR_RED, "Phat hien van de trong qua trinh van chuyen, vui long lien he voi quan tri vien.");
            return 1;
        }

        for(new i=0;i<3;i++) {
            if(IsPlayerInRangeOfPoint(playerid, 3.0, PorterLoad[i][0], PorterLoad[i][1], PorterLoad[i][2])) {
                ApplyAnimation(playerid, "CARRY", "liftup", 4.1, false, true, true, false, 0);
                SetTimerEx("PorterAnimation", 1200, false, "ii", playerid, 0);
                SetLoading(playerid, 1200, "Dang boc hang...");
                return 1;
            }
        }
    }
    if(gPlayerCheckpointStatus[playerid] == CHECKPOINT_PORTERUNLOAD)
    {
        new modelid;
        GetPlayerAttachedObject(playerid, 9, modelid);
        if(IsPlayerInRangeOfPoint(playerid, 3.0, PorterUnload[0], PorterUnload[1], PorterUnload[2]))
        {
            if(modelid != 2060) {
                Porter_Stop(playerid);
                SendClientMessage(playerid, COLOR_GREY, "Khong thay vat pham tren tay, hay thu lai!");
                return 1;
            }

            ApplyAnimation(playerid, "CARRY", "putdwn", 4.1, false, true, true, false, 0);
            SetTimerEx("PorterAnimation", 1200, false, "ii", playerid, 1);
            SetLoading(playerid, 1200, "Dang chat hang...");
            return 1;
        }
    }
    return 1;
}