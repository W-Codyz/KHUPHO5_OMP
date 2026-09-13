#include <YSI\YSI_Coding\y_hooks>

// step 0: load
// step 1: unload
forward PorterAnimation(playerid, step);
forward PorterLoadingFinish(playerid);

stock Porter_LoadingFailed(playerid)
{
    TogglePlayerControllable(playerid, true);
    ClearAnimations(playerid, SYNC_ALL);
    DeletePVar(playerid, "PorterLoadID");
    Porter_Stop(playerid);
    Porter_Start(playerid);
    SendClientMessage(playerid, COLOR_RED, "Boc hang that bai vi ban da roi khoi vi tri lam viec.");
    ShowTDN(playerid, NOTIFICATION_WARNING, "Boc hang that bai.");
    return 1;
}

public PorterLoadingFinish(playerid)
{
    new step = GetPVarInt(playerid, "PorterLoadingStep");
    new loadId = GetPVarInt(playerid, "PorterLoadID");
    new bool:isAtPosition;

    if(PlayerPorter[playerid] == 1)
    {
        if(!step && gPlayerCheckpointStatus[playerid] == CHECKPOINT_PORTERLOAD && loadId >= 0 && loadId < sizeof(PorterLoad)) {
            isAtPosition = IsPlayerInRangeOfPoint(playerid, 3.0, PorterLoad[loadId][0], PorterLoad[loadId][1], PorterLoad[loadId][2]);
        }
        else if(step && gPlayerCheckpointStatus[playerid] == CHECKPOINT_PORTERUNLOAD)
        {
            new unloadId = GetPVarInt(playerid, "PorterUnloadID");
            if(unloadId >= 0 && unloadId < sizeof(PorterUnload)) {
                isAtPosition = IsPlayerInRangeOfPoint(playerid, 3.0, PorterUnload[unloadId][0], PorterUnload[unloadId][1], PorterUnload[unloadId][2]);
            }
        }
    }

    DeletePVar(playerid, "PorterLoadingStep");
    if(!isAtPosition)
    {
        Porter_LoadingFailed(playerid);
        return 1;
    }

    TogglePlayerControllable(playerid, true);
    ClearAnimations(playerid, SYNC_ALL);
    DeletePVar(playerid, "PorterLoadID");
    PorterAnimation(playerid, step);
    return 1;
}

public PorterAnimation(playerid, step)
{
    if(!step) {
        SetPlayerAttachedObject(playerid, 9, 2060, 1, 0.051000, 0.424000, -0.022000, -22.600000, -84.000000, -19.500000);
    	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
		gPlayerCheckpointStatus[playerid] = CHECKPOINT_PORTERUNLOAD;
        new unloadId = random(sizeof(PorterUnload));
        SetPVarInt(playerid, "PorterUnloadID", unloadId);
		SetPlayerCP(playerid, PorterUnload[unloadId][0], PorterUnload[unloadId][1], PorterUnload[unloadId][2], 2.0, false);
        ApplyAnimation(playerid, "CARRY", "liftup", 4.1, false, true, true, false, 0);
        HienTextdraw(playerid, "Di den diem danh dau tren mini-map.");
    }
    else {
        new randCp = random(3);
        SetPlayerCP(playerid, PorterLoad[randCp][0], PorterLoad[randCp][1], PorterLoad[randCp][2], 2.0, false);
        gPlayerCheckpointStatus[playerid] = CHECKPOINT_PORTERLOAD;  
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
        ApplyAnimation(playerid, "CARRY", "liftup", 4.1, false, true, true, false, 0);

        new modelid;
        GetPlayerAttachedObject(playerid, 9, modelid);
        if(modelid == 2060) RemovePlayerAttachedObject(playerid, 9);

        new payment;
        switch(GetJobLevel(playerid, JOB_PORTER))
        {
            case 1: payment = 1500;
            case 2: payment = 2000;
            case 3: payment = 2500;
            case 4: payment = 3000;
            case 5: payment = 3500;
        }

        payment = Random(payment * 90 / 100, payment * 110 / 100);
        GivePlayerCash(playerid, payment);
        new strz[64];
        format(strz, sizeof(strz), "Ban da hoan thanh cong viec va nhan duoc $%d.", payment);
        ShowTDN(playerid, NOTIFICATION_SUCCESS, strz);
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
        format(title, _, "%s (ID: %d) - Bốc vác", GetPlayerNameEx(playerid), playerid);
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

    if(IsLoadingActive(playerid)) StopLoading(playerid);
    DisablePlayerCP(playerid);
    gPlayerCheckpointStatus[playerid] = CHECKPOINT_NONE;
    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
    TogglePlayerControllable(playerid, true);
    ClearAnimations(playerid, SYNC_ALL);
    DeletePVar(playerid, "PorterLoadingStep");
    DeletePVar(playerid, "PorterLoadID");
    DeletePVar(playerid, "PorterUnloadID");
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
            ShowTDN(playerid, NOTIFICATION_WARNING, "Co loi xay ra!!");
            return 1;
        }

        for(new i=0;i<3;i++) {
            if(IsPlayerInRangeOfPoint(playerid, 3.0, PorterLoad[i][0], PorterLoad[i][1], PorterLoad[i][2])) {
                SetPVarInt(playerid, "PorterLoadID", i);
                SetPVarInt(playerid, "PorterLoadingStep", 0);
                TogglePlayerControllable(playerid, false);
                ApplyAnimation(playerid, "BD_FIRE", "wash_up", 4.0, 1, 0, 0, 0, 0, 1);
                SetLoading(playerid, 5000, "Dang boc hang...", "PorterLoadingFinish");
                return 1;
            }
        }
    }
    if(gPlayerCheckpointStatus[playerid] == CHECKPOINT_PORTERUNLOAD)
    {
        new modelid;
        GetPlayerAttachedObject(playerid, 9, modelid);
        new unloadId = GetPVarInt(playerid, "PorterUnloadID");
        if(unloadId >= 0 && unloadId < sizeof(PorterUnload) && IsPlayerInRangeOfPoint(playerid, 3.0, PorterUnload[unloadId][0], PorterUnload[unloadId][1], PorterUnload[unloadId][2]))
        {
            if(modelid != 2060) {
                Porter_Stop(playerid);
                SendClientMessage(playerid, COLOR_GREY, "Khong thay vat pham tren tay, hay thu lai!");
                ShowTDN(playerid, NOTIFICATION_WARNING, "Khong thay vat pham tren tay, hay thu lai!");
                return 1;
            }

            SetPVarInt(playerid, "PorterLoadingStep", 1);
            TogglePlayerControllable(playerid, false);
            ApplyAnimation(playerid, "BD_FIRE", "wash_up", 4.0, 1, 0, 0, 0, 0, 1);
            SetLoading(playerid, 5000, "Dang chat hang...", "PorterLoadingFinish");
            return 1;
        }
    }
    return 1;
}