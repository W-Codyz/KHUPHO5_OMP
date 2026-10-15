/*==============================================================================
    hospital_spawn.inc
    He thong chuyen benh nhan va hoi phuc tren giuong benh
==============================================================================*/

// ============================================================
//  BIEN TOAN CUC
// ============================================================

static const Float:g_HospBedPos[14][4] =
{
    {155.791473, 57.609878,  3381.679443, 274.168945},
    {155.508697, 60.224601,  3381.679443, 270.933654},
    {155.645553, 63.071449,  3381.679443, 276.969238},
    {155.721740, 65.622497,  3381.679443, 273.522552},
    {163.067260, 65.740859,  3381.679443,  90.494880},
    {162.856536, 62.973827,  3381.679443,  87.863082},
    {162.989227, 60.313262,  3381.679443,  93.566154},
    {163.156799, 29.174312,  3381.679443,  89.872093},
    {162.983764, 26.425668,  3381.679443,  91.669570},
    {162.932327, 23.774274,  3381.679443,  91.231323},
    {155.838211, 23.759511,  3381.679443, 270.624389},
    {155.769607, 26.458150,  3381.679443, 264.733673},
    {155.705322, 29.203893,  3381.679443, 271.000396},
    {155.612609, 31.935873,  3381.679443, 268.493713}
};

new g_HospRecoveryTimer[MAX_PLAYERS] = {-1, ...};
new g_HospRecoveryTime[MAX_PLAYERS];
new bool:g_HospTreatFailed[MAX_PLAYERS];
new Float:g_HospBedX[MAX_PLAYERS],
    Float:g_HospBedY[MAX_PLAYERS],
    Float:g_HospBedZ[MAX_PLAYERS],
    Float:g_HospBedA[MAX_PLAYERS];

// ============================================================
//  HANG SO
// ============================================================

#define HOSP_DELIVER_X        -2695.5725
#define HOSP_DELIVER_Y         639.4147
#define HOSP_DELIVER_Z          14.4531
#define HOSP_DELIVER_RANGE       5.0

#define HOSP_EXIT_X           -2656.966064
#define HOSP_EXIT_Y            623.142883
#define HOSP_EXIT_Z             14.453125

#define HOSP_BED_INTERIOR        1
#define HOSP_BED_VW              0
#define HOSP_BED_COUNT          14
#define HOSP_BED_SNAP_DIST       3.0

// Delay mo khoa sau animation ngoi day (ms)
#define HOSP_STANDUP_DELAY    2000

#define HOSP_SUCCESS_CHANCE     50
#define HOSP_SUCCESS_TIME       30
#define HOSP_FAIL_TIME          60
#define HOSP_SUCCESS_FEE      3000
#define HOSP_FAIL_FEE        15000
#define HOSP_MEDIC_PAY        3000
#define HOSP_HEAL_HP           50.0

// ============================================================
//  FORWARD
// ============================================================

forward HospitalRecovery_Tick(playerid);
forward hosp_ConfirmBedPos(playerid);
forward hosp_CompleteBedExit(playerid);
forward hosp_PostSpawnBed(playerid);

// ============================================================
//  HAM HO TRO NOI BO
// ============================================================

stock hosp_StopRecovery(playerid)
{
    if(g_HospRecoveryTimer[playerid] != -1)
    {
        KillTimer(g_HospRecoveryTimer[playerid]);
        g_HospRecoveryTimer[playerid] = -1;
    }
    g_HospRecoveryTime[playerid] = 0;
    g_HospTreatFailed[playerid]  = false;
    g_HospBedX[playerid]         = 0.0;
    g_HospBedY[playerid]         = 0.0;
    g_HospBedZ[playerid]         = 0.0;
    g_HospBedA[playerid]         = 0.0;
    DeletePVar(playerid, "KhongHackWeaponInv");
}

// Dat benh nhan vao giuong — dung cho ca rescue va post-spawn
stock hosp_PlacePatientInBed(patientid, bool:treatFailed)
{
    // Kill timer cu truoc khi tao timer moi, tranh orphaned timer gay spam va tele 0/0/0
    if(g_HospRecoveryTimer[patientid] != -1)
        hosp_StopRecovery(patientid);

    new bedIdx = random(HOSP_BED_COUNT);

    g_HospBedX[patientid] = g_HospBedPos[bedIdx][0];
    g_HospBedY[patientid] = g_HospBedPos[bedIdx][1];
    g_HospBedZ[patientid] = g_HospBedPos[bedIdx][2];
    g_HospBedA[patientid] = g_HospBedPos[bedIdx][3];
    g_HospTreatFailed[patientid] = treatFailed;

    new recoveryTime = treatFailed ? HOSP_FAIL_TIME : HOSP_SUCCESS_TIME;
    g_HospRecoveryTime[patientid] = recoveryTime;

    RemovePlayerFromVehicle(patientid);
    SetPlayerHealth(patientid, 5.0);
    SetPlayerArmour(patientid, 0.0);
    DeletePVar(patientid, "HospitalTimer");

    SetPlayerInterior(patientid, HOSP_BED_INTERIOR);
    PlayerInfo[patientid][pInt] = HOSP_BED_INTERIOR;
    SetPlayerVirtualWorld(patientid, HOSP_BED_VW);
    PlayerInfo[patientid][pVW]  = HOSP_BED_VW;

    SetPlayerPos(patientid, g_HospBedX[patientid], g_HospBedY[patientid], g_HospBedZ[patientid]);
    SetPlayerFacingAngle(patientid, g_HospBedA[patientid]);
    Streamer_UpdateEx(patientid, g_HospBedX[patientid], g_HospBedY[patientid], g_HospBedZ[patientid]);

    // Tat anti-weapon check trong thoi gian nam vien de tranh false positive
    SetPVarInt(patientid, "KhongHackWeaponInv", 1);

    // Freeze ngay lap tuc truoc khi StreamPrep bat dau
    TogglePlayerControllable(patientid, false);

    // Xoa StreamPrep PVar cu neu co (tranh bi skip qua switch-case)
    DeletePVar(patientid, "StreamPrep");

    // StreamPrep: hien loading UI + xac nhan vi tri 2 lan trong FREEZE_TIME ms
    // (giong PlayerEnterInterior voi custom interior — ngan nguoi choi roi map anh)
    Player_StreamPrep(patientid, g_HospBedX[patientid], g_HospBedY[patientid], g_HospBedZ[patientid], FREEZE_TIME);

    SetLoading(patientid, recoveryTime * 1000, "Dang hoi phuc...");

    // Ap dung animation nam SAU khi StreamPrep hoan tat (FREEZE_TIME + 100ms)
    // StreamPrep ket thuc tai t=FREEZE_TIME, unfreeze player, hosp_ConfirmBedPos re-freeze + apply anim
    SetTimerEx("hosp_ConfirmBedPos", FREEZE_TIME + 100, false, "i", patientid);

    g_HospRecoveryTimer[patientid] = SetTimerEx("HospitalRecovery_Tick", 1000, true, "i", patientid);
}

// ============================================================
//  LENH /deliverpt (alias: /capcuu)
// ============================================================

alias:deliverpt("capcuu")
CMD:deliverpt(playerid, params[])
{
    if(!IsAMedic(playerid)) return 1;

    new giveplayerid;
    if(sscanf(params, "u", giveplayerid))
        return SendHelpMessage(playerid, "/capcuu [player]");

    new carid   = GetPlayerVehicleID(playerid);
    new caridex = GetPlayerVehicleID(giveplayerid);

    if(!IsAnAmbulance(carid))
        return SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong o tren xe FDSA.");

    if(carid != caridex)
        return SendClientMessageEx(playerid, COLOR_GRAD2, "Benh nhan phai nam tren xe cua ban.");

    if(playerid == giveplayerid)
        return SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong the tu dua chinh minh vao benh vien!");

    if(!IsPlayerInRangeOfPoint(playerid, HOSP_DELIVER_RANGE, HOSP_DELIVER_X, HOSP_DELIVER_Y, HOSP_DELIVER_Z))
        return SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong o diem giao benh nhan (Benh vien SF).");

    if(GetPVarInt(giveplayerid, "Injured") == 0)
        return SendClientMessageEx(playerid, COLOR_GRAD2, "Nguoi do khong bi thuong!");

    if(playerTabbed[giveplayerid] >= 1)
        return SendClientMessageEx(playerid, COLOR_GRAD2, "Nguoi choi dang tam dung ket noi, khong the cuu!");

    new bool:treatFailed = bool:(random(100) >= HOSP_SUCCESS_CHANCE);

    GivePlayerCash(playerid, HOSP_MEDIC_PAY);
    PlayerInfo[playerid][pPatientsDelivered]++;

    new string[144];

    if(!treatFailed)
    {
        SetPVarInt(giveplayerid, "HospFee", HOSP_SUCCESS_FEE);

        SendClientMessageEx(giveplayerid, TEAM_CYAN_COLOR,
            "Ca phau thuat thanh cong! Hoa don y te: $3,000. Chuc ban mot ngay tot lanh!");
        SendClientMessageEx(playerid, TEAM_CYAN_COLOR,
            "Cap cuu benh nhan thanh cong! Ban duoc benh vien tra cong $3,000!");
    }
    else
    {
        SetPVarInt(giveplayerid, "HospFee", HOSP_FAIL_FEE);

        ResetPlayerWeapons(giveplayerid);
        PlayerInfo[giveplayerid][pThucAn] = 50;
        PlayerInfo[giveplayerid][pNuoc]   = 50;

        SendClientMessageEx(giveplayerid, COLOR_RED,
            "Cap cuu that bai, bac si da co gang het suc! Hoa don y te: $15,000.");
        SendClientMessageEx(playerid, TEAM_CYAN_COLOR,
            "Cap cuu benh nhan that bai! Ban duoc benh vien tra cong $3,000.");
    }

    if(GetPVarInt(giveplayerid, "OnStretcher") == 1)
    {
        new mover = GetPVarInt(giveplayerid, "MovedByMedic");
        if(IsPlayerConnected(mover)) StopMoveEMS(mover, giveplayerid);
        else SetPVarInt(giveplayerid, "OnStretcher", 0);
    }
    KillEMSQueue(giveplayerid);
    SetPVarInt(giveplayerid, "MedicBill", 0);

    if(GetPVarType(giveplayerid, "STD"))
    {
        DeletePVar(giveplayerid, "STD");
        SendClientMessageEx(giveplayerid, COLOR_LIGHTBLUE, "* Benh vien da chua tri ban khoi STD!");
    }

    format(string, sizeof(string), "Bac si (%s) da cap cuu cho (%s). Tinh trang [%s].",
        GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid),
        treatFailed ? "That bai" : "Thanh cong");
    SendGroupMessage(3, TEAM_MED_COLOR, string);

    hosp_PlacePatientInBed(giveplayerid, treatFailed);
    return 1;
}

// ============================================================
//  TIMER: AP DUNG ANIMATION NAM SAU KHI STREAMPREP HOAN TAT
//  Chay tai FREEZE_TIME + 100ms — luc nay StreamPrep da xac nhan vi tri va mapping da load
// ============================================================

public hosp_ConfirmBedPos(playerid)
{
    if(!IsPlayerConnected(playerid) || g_HospRecoveryTimer[playerid] == -1) return;

    // StreamPrep vua ket thuc, dat lai vi tri chinh xac + re-freeze + apply animation nam
    SetPlayerPos(playerid, g_HospBedX[playerid], g_HospBedY[playerid], g_HospBedZ[playerid]);
    SetPlayerFacingAngle(playerid, g_HospBedA[playerid]);
    TogglePlayerControllable(playerid, false);
    ApplyAnimation(playerid, "SWAT", "gnstwall_injurd", 4.0, false, true, true, true, 0, 1);
}

// ============================================================
//  TIMER: HOI PHUC TUNG GIAY
// ============================================================

public HospitalRecovery_Tick(playerid)
{
    if(!IsPlayerConnected(playerid))
    {
        hosp_StopRecovery(playerid);
        return;
    }

    // Safety: neu timer nay la orphaned (hosp_StopRecovery da duoc goi boi timer khac),
    // thi g_HospRecoveryTimer da bi set -1 → thoat de tranh tele 0/0/0 va spam message.
    if(g_HospRecoveryTimer[playerid] == -1) return;

    // Anti-teleport: keo ve giuong neu bi dich chuyen ra ngoai
    if(g_HospBedX[playerid] != 0.0 &&
        !IsPlayerInRangeOfPoint(playerid, HOSP_BED_SNAP_DIST,
        g_HospBedX[playerid], g_HospBedY[playerid], g_HospBedZ[playerid]))
    {
        SetPlayerPos(playerid, g_HospBedX[playerid], g_HospBedY[playerid], g_HospBedZ[playerid]);
    }

    // Kiem tra animation index — neu khong phai animation nam (1508) thi apply lai
    // (xu ly cac truong hop sobeitCheck/StreamPrep hoac he thong khac clear animation)
    if(!GetPVarInt(playerid, "StreamPrep") && GetPlayerAnimationIndex(playerid) != 1508)
    {
        ClearAnimations(playerid);
        ApplyAnimation(playerid, "SWAT", "gnstwall_injurd", 4.0, false, true, true, true, 0, 1);
    }

    
    //TogglePlayerControllable(playerid, false); //bỏ đi vì freeze làm cho animation của player thành đứng thẳng

    new bubble[48];
    format(bubble, sizeof(bubble), "Dang hoi phuc... [%d giay]", g_HospRecoveryTime[playerid]);
    SetPlayerChatBubble(playerid, bubble, TEAM_CYAN_COLOR, 15.0, 1100);

    g_HospRecoveryTime[playerid]--;
    if(g_HospRecoveryTime[playerid] > 0) return;

    // ---- Hoan tat hoi phuc ----
    hosp_StopRecovery(playerid);

    // Animation ngoi day — player dung ngay tren giuong
    ApplyAnimation(playerid, "SUNBATHE", "Lay_Bac_out", 4.0, 0, 0, 0, 0, 0, 1);

    // Mo khoa sau khi animation ngoi day chay xong (~2 giay)
    SetTimerEx("hosp_CompleteBedExit", HOSP_STANDUP_DELAY, false, "i", playerid);
}

// ============================================================
//  TIMER: MO KHOA SAU ANIMATION NGOI DAY
// ============================================================

public hosp_CompleteBedExit(playerid)
{
    if(!IsPlayerConnected(playerid)) return;

    new fee = GetPVarInt(playerid, "HospFee");
    if(fee > 0)
    {
        GivePlayerCash(playerid, -fee);
        SendClientMessageEx(playerid, TEAM_CYAN_COLOR, "* Tien vien phi cua ban la: $%d.", fee);
        Tax += fee;
        DeletePVar(playerid, "HospFee");
    }

    TogglePlayerControllable(playerid, true);
    SetPlayerHealth(playerid, HOSP_HEAL_HP);
    PlayerInfo[playerid][pHoiPhuc] = (fee == HOSP_SUCCESS_FEE) ? 600 : 1200;
    SendClientMessageEx(playerid, TEAM_CYAN_COLOR, "* Ban da hoi phuc! Hay tu tim duong ra khoi benh vien.");
}

// ============================================================
//  TIMER: DAT VAO GIUONG SAU KHI SPAWN (death/disconnect case)
//  Chay 100ms sau OnPlayerSpawn de ghi de vi tri/vu khi cua SetPlayerSpawn
// ============================================================

public hosp_PostSpawnBed(playerid)
{
    if(!IsPlayerConnected(playerid)) return;

    // Vu khi co the da duoc khoi phuc boi SetPlayerWeapons trong OnPlayerSpawn
    // Reset lai de ap dung hinh phat chet-khi-bi-thuong hoac thoat-khi-bi-thuong
    ResetPlayerWeapons(playerid);
    for(new w = 0; w < 12; w++) PlayerInfo[playerid][pGuns][w] = 0;

    // treatFailed=false: hinh phat da ap dung truoc do, chi can giu trong giuong 15 giay
    hosp_PlacePatientInBed(playerid, false);
    SetPVarInt(playerid, "HospFee", HOSP_FAIL_FEE);
}

// ============================================================
//  CALLBACKS — EDGE CASES
//
//  Them vao callbacks.inc:
//    OnPlayerSpawn (TRUOC SetPlayerSpawn): HospitalSpawn_OnPlayerSpawn(playerid);
//    OnPlayerDisconnect:                  HospitalSpawn_OnPlayerDisconnect(playerid);
//    OnPlayerDeath (sau NPC check):       HospitalSpawn_OnPlayerDeath(playerid, killerid);
// ============================================================

// --- OnPlayerSpawn ---
// Goi TRUOC SetPlayerSpawn de intercept 2 truong hop:
//   (A) _HospDMDeath=1: chet khi dang bi thuong (bi DM kill)
//   (B) pHospital==5 va MedicBill==0: thoat game khi dang bi thuong, login lai
stock HospitalSpawn_OnPlayerSpawn(playerid)
{
    // Truong hop A: chet khi bi thuong
    if(GetPVarInt(playerid, "_HospDMDeath") == 1)
    {
        DeletePVar(playerid, "_HospDMDeath");

        // Xoa toan bo trang thai bi thuong (OnPlayerDeath co the set lai Injured=1)
        DeletePVar(playerid, "Injured");
        DeletePVar(playerid, "EMSAttempt");
        DeletePVar(playerid, "MedicCall");
        DeletePVar(playerid, "EMSWarns");
        DeletePVar(playerid, "InjuredTimer");

        // Dam bao SetPlayerSpawn chay duong binh thuong (khong phai hospital block cu)
        SetPVarInt(playerid, "MedicBill", 0);
        PlayerInfo[playerid][pHospital] = 0;

        // 100ms sau: ghi de ket qua cua SetPlayerSpawn/SetPlayerWeapons
        SetTimerEx("hosp_PostSpawnBed", 100, false, "i", playerid);
        return;
    }

    // Truong hop B: thoat game khi dang bi thuong → pHospital=5 da luu DB
    if(PlayerInfo[playerid][pHospital] == 5 && GetPVarInt(playerid, "MedicBill") == 0)
    {
        // Reset de SetPlayerSpawn spawn binh thuong tai vi tri cu (khong dung hospital block cu)
        PlayerInfo[playerid][pHospital] = 0;
        SetPVarInt(playerid, "MedicBill", 0);

        SetTimerEx("hosp_PostSpawnBed", 100, false, "i", playerid);
    }
}

// --- OnPlayerDisconnect ---
stock HospitalSpawn_OnPlayerDisconnect(playerid)
{
    if(g_HospRecoveryTimer[playerid] != -1)
        hosp_StopRecovery(playerid);

    if(GetPVarInt(playerid, "Injured") == 1)
    {
        ResetPlayerWeapons(playerid);
        for(new w = 0; w < 12; w++) PlayerInfo[playerid][pGuns][w] = 0;
        PlayerInfo[playerid][pThucAn] = 50;
        PlayerInfo[playerid][pNuoc]   = 50;
        KillEMSQueue(playerid);

        // Danh dau de login lai se dua vao benh vien (pHospital luu vao DB)
        PlayerInfo[playerid][pHospital] = 5;

        new string[128];
        format(string, sizeof(string),
            "[HOSPITAL LOG] %s (ID:%d) thoat game trong khi dang bi thuong.",
            GetPlayerNameEx(playerid), playerid);
        ABroadCast(COLOR_LIGHTRED, string, 2);
        Log("logs/hospital.log", string);
    }
}

// --- OnPlayerDeath ---
stock HospitalSpawn_OnPlayerDeath(playerid, killerid)
{
    if(g_HospRecoveryTimer[playerid] != -1)
        hosp_StopRecovery(playerid);

    if(GetPVarInt(playerid, "Injured") == 1)
    {
        // Ap dung hinh phat ngay trong OnPlayerDeath
        ResetPlayerWeapons(playerid);
        PlayerInfo[playerid][pThucAn] = 50;
        PlayerInfo[playerid][pNuoc]   = 50;

        // Danh dau truong hop "chet khi dang bi thuong"
        // HospitalSpawn_OnPlayerSpawn se xu ly phan con lai khi player click spawn
        // (khong goi KillEMSQueue o day vi OnPlayerDeath sau do co the set lai Injured=1)
        SetPVarInt(playerid, "_HospDMDeath", 1);

        if(killerid != INVALID_PLAYER_ID && IsPlayerConnected(killerid))
        {
            new string[128];
            format(string, sizeof(string),
                "[DEATH LOG] %s bi %s ban chet trong khi dang bi thuong.",
                GetPlayerNameEx(playerid), GetPlayerNameEx(killerid));
            ABroadCast(COLOR_LIGHTRED, string, 2);
            Log("logs/hospital.log", string);
        }
    }
}
