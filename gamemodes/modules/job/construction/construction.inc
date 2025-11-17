#include <YSI\YSI_Coding\y_hooks>

Construction_Start(playerid, oldPoint=-1)
{
    new randCP = random(sizeof(ConstructionPos));
    for(new i=0; i < 1000 && oldPoint >= 0; i ++) {
        if(randCP != oldPoint) break;
        randCP = random(sizeof(ConstructionPos));
    }
    SetPlayerCP(playerid, ConstructionPos[randCP][0], ConstructionPos[randCP][1], ConstructionPos[randCP][2], 2.0, false);
    SetPVarInt(playerid, "ConstructionID", randCP);
    gPlayerCheckpointStatus[playerid] = CHECKPOINT_CONSTRUCTION;
    return 1;
}

Construction_Stop(playerid)
{
    if(IsPlayerAttachedObjectSlotUsed(playerid, 9)) {
        RemovePlayerAttachedObject(playerid, 9);
    }
    if(IsLoadingActive(playerid)) {
        StopLoading(playerid);
    }
    DisablePlayerCP(playerid);
    ClearAnimations(playerid);
    DeletePVar(playerid, "ConstructionID");
    return 1;
}

forward ConstructionLoader(playerid);
public ConstructionLoader(playerid)
{
    new consId = GetPVarInt(playerid, "ConstructionID");
    if(consId >= 0 && consId < sizeof(ConstructionPos)) {
        if(IsPlayerAttachedObjectSlotUsed(playerid, 9)) {
            RemovePlayerAttachedObject(playerid, 9);
        }

        TogglePlayerControllable(playerid, true);
        ClearAnimations(playerid, SYNC_ALL);

        new payment;
        switch(GetJobLevel(playerid, JOB_CONSTRUCTION))
        {
            case 1: payment = 25;
            case 2: payment = 35;
            case 3: payment = 48;
            case 4: payment = 60;
            case 5: payment = 73;
        }

        payment = Random(payment*2, payment*3);
        GivePlayerCash(playerid, payment);
        
        new rand_xp = 0;
        if(DoubleXP) {
            rand_xp = Random(PlayerInfo[playerid][pLevel] * XP_RATE - 5, PlayerInfo[playerid][pLevel] * XP_RATE - 2);
            SendClientMessageEx(playerid, COLOR_YELLOW, "Kinh nghiem nhan vat: +%d XP (%s/%s) | +2 Construction Skill (DOUBLE XP)", rand_xp*2, number_format(PlayerInfo[playerid][pXP]+rand_xp*2), number_format(GetRequiredXP(playerid)));
            GivePlayerXP(playerid, rand_xp*2);
            PlayerInfo[playerid][pConstructSkill] += 2;
         }
        else if(PlayerInfo[playerid][pDoubleEXP] > 0 && !DoubleXP)
        {
            rand_xp = Random(PlayerInfo[playerid][pLevel] * XP_RATE - 5, PlayerInfo[playerid][pLevel] * XP_RATE - 2);
            SendClientMessageEx(playerid, COLOR_YELLOW, "Kinh nghiem nhan vat: +%d XP (%s/%s) | +2 Construction Skill (DOUBLE XP)", rand_xp*2, number_format(PlayerInfo[playerid][pXP]+rand_xp*2), number_format(GetRequiredXP(playerid)));
            GivePlayerXP(playerid, rand_xp*2);
            PlayerInfo[playerid][pConstructSkill] += 2;
        }
        else
        {
            rand_xp = Random(PlayerInfo[playerid][pLevel] * XP_RATE - 5, PlayerInfo[playerid][pLevel] * XP_RATE - 2);
            SendClientMessageEx(playerid, COLOR_YELLOW, "Kinh nghiem nhan vat: +%d XP (%s/%s) | +1 Construction Skill", rand_xp, number_format(PlayerInfo[playerid][pXP]+rand_xp), number_format(GetRequiredXP(playerid)));
            GivePlayerXP(playerid, rand_xp);
            PlayerInfo[playerid][pConstructSkill] += 1;
        }

        Construction_Start(playerid, consId);

        new title[64];
        format(title, _, "%s (ID: %d) - Xây dựng", GetPlayerNameEx(playerid), playerid);
        DiscordLog(dclog_Construction, title, "**Tiền nhận được: $%d**\n**Kinh nghiệm nhân vật: +%d XP**", payment, rand_xp);
    }
    return 1;
}

hook OnPlayerEnterCheckpoint(playerid)
{
    if(GetPVarType(playerid, "ConstructionID") == PLAYER_VARTYPE_INT &&  gPlayerCheckpointStatus[playerid] == CHECKPOINT_CONSTRUCTION) {
        new consId = GetPVarInt(playerid, "ConstructionID");
        if(consId >= 0 && consId < sizeof(ConstructionPos)) {
            if(IsPlayerInRangeOfPoint(playerid, 2.8, ConstructionPos[consId][0], ConstructionPos[consId][1], ConstructionPos[consId][2])) {
                DisablePlayerCP(playerid);
                new randA = random(4);
                switch(randA) {
                    case 0: {
                        ApplyAnimation(playerid, "BASEBALL", "Bat_4", 4.1, true, false, false, true, 0, SYNC_ALL);
                        SetPlayerAttachedObject(playerid, 9, 19631, 6, 0.095000, -0.007999, 0.250000, -80.699989, -109.199996, 0.000000);
                    }
                    case 1: {
                        ApplyAnimation(playerid, "COP_AMBIENT", "Copbrowse_loop", 4.1, true, false, false, true, 0, SYNC_ALL);
                        SetPlayerAttachedObject(playerid, 9, 18635, 6);
                    }
                    case 2: {
                        ApplyAnimation(playerid, "HEIST9", "Use_SwipeCard", 4.1, true, false, false, true, 0, SYNC_ALL);
                        SetPlayerAttachedObject(playerid, 9, 18635, 6);
                    }
                    case 3: {
                        ApplyAnimation(playerid, "FIGHT_B", "FightB_G", 4.1, true, false, false, true, 0, SYNC_ALL);
                        SetPlayerAttachedObject(playerid, 9, 18635, 6);
                    }
                }
                TogglePlayerControllable(playerid, true);
                SetLoading(playerid, 10000, "Dang xay dung...", "ConstructionLoader");
                
                return 1;
            }
        }
    }
    return 1;
}