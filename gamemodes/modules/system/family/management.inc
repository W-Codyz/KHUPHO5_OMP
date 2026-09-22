// ===========================================================================
// gamemodes/modules/system/family/management.inc
// Lệnh /gangs – quản lý gang, thành viên, két sắt, bank
//
// TÍCH HỢP:
//   Thêm vào OnDialogResponse:
//     if(FM_OnDialogResponse(playerid, dialogid, response, listitem, inputtext)) return 1;
// ===========================================================================

// Dialog IDs (tiếp theo assets.inc, bắt đầu từ 5350)
#define DIALOG_GANGS_MAIN       5350
#define DIALOG_GANGS_INFO       5351
#define DIALOG_GANGS_MEMBERS    5352
#define DIALOG_GANGS_MEMBER_ACT 5353
#define DIALOG_GANGS_RANKUP     5354
#define DIALOG_GANGS_RANKDOWN   5355
#define DIALOG_GANGS_KICK       5356
#define DIALOG_GANGS_SAFE       5357
#define DIALOG_GANGS_SAFE_SETPOS 5358
#define DIALOG_GANGS_SAFE_UPGRADE 5359
#define DIALOG_GANGS_BANK       5360

// Per-player state cho management dialogs
new g_fmSelectedPlayer[MAX_PLAYERS] = {-1, ...}; // Player đang được chọn để xử lý

// ---------------------------------------------------------------------------
// LỆNH /gangs (rank 5+)
// ---------------------------------------------------------------------------

CMD:gangs(playerid, params[])
{
    new fam = PlayerInfo[playerid][pFMember];
    if(fam == INVALID_FAMILY_ID)
        return SendClientMessageEx(playerid, COLOR_RED, "Ban chua vao gang nao."), 1;
    if(PlayerInfo[playerid][pRank] < 5)
        return SendClientMessageEx(playerid, COLOR_RED, "Can rank 5+ de su dung lenh nay."), 1;

    ShowPlayerDialog(playerid, DIALOG_GANGS_MAIN, DIALOG_STYLE_LIST,
        "Quan Ly Gang",
        "Thong tin gang\nQuan ly thanh vien\nKet sac\nNgan hang gang",
        "Chon", "Dong");
    return 1;
}

// ---------------------------------------------------------------------------
// DIALOG HELPERS
// ---------------------------------------------------------------------------

static stock FM_ShowGangInfo(playerid, famid)
{
    new lv    = FamilyInfo[famid][FamilyLevel];
    new onlineCnt = 0;
    foreach(new i : Player) if(PlayerInfo[i][pFMember] == famid) onlineCnt++;

    // Danh sách turfs đang sở hữu
    new turfList[128]; turfList[0] = 0;
    for(new t = 0; t < TotalTurfs; t++)
    {
        if(TurfData[t][twOwnerGang] == famid)
        {
            new tline[32];
            format(tline, sizeof(tline), "  Turf #%d (%.0f,%.0f)\n", t+1, TurfData[t][twPosX], TurfData[t][twPosY]);
            strcat(turfList, tline, sizeof(turfList));
        }
    }
    if(strlen(turfList) == 0) format(turfList, sizeof(turfList), "  (Chua so huu turf nao)\n");

    new str[512];
    format(str, sizeof(str),
        "Ten gang:      %s\nLevel ket:     %d / %d\nFame:          %d\nSo thanh vien: %d / %d (Online: %d)\nTien bank:     $%d\n\nTurf dang so huu:\n%s",
        FamilyInfo[famid][FamilyName],
        lv, MAX_SAFE_LEVEL,
        FamilyInfo[famid][FamilyFame],
        FamilyInfo[famid][FamilyMembers], FA_GetMaxMembers(lv), onlineCnt,
        FamilyInfo[famid][FamilyBank],
        turfList);

    ShowPlayerDialog(playerid, DIALOG_GANGS_INFO, DIALOG_STYLE_MSGBOX, "Thong Tin Gang", str, "OK", "");
}

static stock FM_ShowMemberList(playerid, famid)
{
    new list[1024]; list[0] = 0;
    foreach(new i : Player)
    {
        if(PlayerInfo[i][pFMember] != famid) continue;
        new pname[MAX_PLAYER_NAME];
        GetPlayerName(i, pname, sizeof(pname));
        new line[56];
        format(line, sizeof(line), "[Online] %s (Rank %d)\n", pname, PlayerInfo[i][pRank]);
        strcat(list, line, sizeof(list));
    }

    // TODO: Nếu cần thêm thành viên offline, query database ở đây
    if(strlen(list) == 0) format(list, sizeof(list), "(Khong co thanh vien online)");

    ShowPlayerDialog(playerid, DIALOG_GANGS_MEMBERS, DIALOG_STYLE_LIST, "Danh Sach Thanh Vien (Online)", list, "Chon", "Quay Lai");
}

// Build danh sách player online của gang theo thứ tự; trả về ID player thứ idx
static stock FM_GetGangMemberByIdx(famid, idx)
{
    new cnt = -1;
    foreach(new i : Player)
    {
        if(PlayerInfo[i][pFMember] != famid) continue;
        cnt++;
        if(cnt == idx) return i;
    }
    return -1;
}

static stock FM_ShowMemberActions(playerid, targetid)
{
    if(targetid == -1 || !IsPlayerConnected(targetid)) return;
    new tname[MAX_PLAYER_NAME];
    GetPlayerName(targetid, tname, sizeof(tname));
    new title[48];
    format(title, sizeof(title), "Hanh Dong: %s (Rank %d)", tname, PlayerInfo[targetid][pRank]);

    ShowPlayerDialog(playerid, DIALOG_GANGS_MEMBER_ACT, DIALOG_STYLE_LIST,
        title,
        "Nang rank\nHa rank\nKick khoi gang",
        "Chon", "Huy");
}

// ---------------------------------------------------------------------------
// DIALOG RESPONSE
// ---------------------------------------------------------------------------

stock FM_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    #pragma unused inputtext
    new fam  = PlayerInfo[playerid][pFMember];
    new rank = PlayerInfo[playerid][pRank];

    switch(dialogid)
    {
        // ---- Main menu /gangs ----
        case DIALOG_GANGS_MAIN:
        {
            if(!response) return 1;
            switch(listitem)
            {
                case 0: FM_ShowGangInfo(playerid, fam);
                case 1: FM_ShowMemberList(playerid, fam);
                case 2: // Két sắt sub-menu
                {
                    ShowPlayerDialog(playerid, DIALOG_GANGS_SAFE, DIALOG_STYLE_LIST,
                        "Ket Sac Gang",
                        "Dat vi tri ket ve vi tri hien tai\nNang cap ket sac",
                        "Chon", "Quay Lai");
                }
                case 3: // Bank → mở dialog bank (dùng dialog của assets.inc)
                {
                    ShowPlayerDialog(playerid, DIALOG_BANK_MAIN, DIALOG_STYLE_LIST,
                        "Ngan Hang Gang",
                        "Xem so du\nNap tien vao bank\nRut tien khoi bank (rank 5+)",
                        "Chon", "Dong");
                }
            }
            return 1;
        }

        case DIALOG_GANGS_INFO: return 1; // MSGBOX → chỉ có OK

        // ---- Danh sách thành viên → chọn để xử lý ----
        case DIALOG_GANGS_MEMBERS:
        {
            if(!response)
            {
                // Quay lại main menu
                ShowPlayerDialog(playerid, DIALOG_GANGS_MAIN, DIALOG_STYLE_LIST,
                    "Quan Ly Gang",
                    "Thong tin gang\nQuan ly thanh vien\nKet sac\nNgan hang gang",
                    "Chon", "Dong");
                return 1;
            }
            new targetid = FM_GetGangMemberByIdx(fam, listitem);
            if(targetid == -1 || targetid == playerid)
                return SendClientMessageEx(playerid, COLOR_RED, "Khong the chon ban than."), 1;

            g_fmSelectedPlayer[playerid] = targetid;
            FM_ShowMemberActions(playerid, targetid);
            return 1;
        }

        // ---- Hành động với thành viên ----
        case DIALOG_GANGS_MEMBER_ACT:
        {
            if(!response) return 1;
            new targetid = g_fmSelectedPlayer[playerid];
            if(targetid == -1 || !IsPlayerConnected(targetid)) return 1;

            new tRank = PlayerInfo[targetid][pRank];

            switch(listitem)
            {
                case 0: // Nâng rank
                {
                    // Không nâng quá rank mình - 1
                    new newRank = tRank + 1;
                    if(newRank >= rank)
                        return SendClientMessageEx(playerid, COLOR_RED, "Khong the nang rank cao bang hoac hon ban."), 1;

                    PlayerInfo[targetid][pRank] = newRank;
                    new q[128];
                    mysql_format(MainPipeline, q, sizeof(q),
                        "UPDATE `accounts` SET `Rank`=%d WHERE `id`=%d", newRank, PlayerInfo[targetid][pId]);
                    mysql_tquery(MainPipeline, q, "", "");

                    new tname[MAX_PLAYER_NAME], str[80];
                    GetPlayerName(targetid, tname, sizeof(tname));
                    format(str, sizeof(str), "Da nang rank %s len Rank %d.", tname, newRank);
                    SendClientMessageEx(playerid, COLOR_GREEN, str);
                    format(str, sizeof(str), "Ban da duoc nang len Rank %d boi %s.", newRank, ReturnPlayerName(playerid));
                    SendClientMessageEx(targetid, COLOR_GREEN, str);
                }
                case 1: // Hạ rank
                {
                    new newRank = tRank - 1;
                    if(newRank < 1)
                        return SendClientMessageEx(playerid, COLOR_RED, "Rank thap nhat la 1."), 1;
                    if(tRank >= rank)
                        return SendClientMessageEx(playerid, COLOR_RED, "Khong the ha rank nguoi co rank cao hon ban."), 1;

                    PlayerInfo[targetid][pRank] = newRank;
                    new q2[128];
                    mysql_format(MainPipeline, q2, sizeof(q2),
                        "UPDATE `accounts` SET `Rank`=%d WHERE `id`=%d", newRank, PlayerInfo[targetid][pId]);
                    mysql_tquery(MainPipeline, q2, "", "");

                    new tname[MAX_PLAYER_NAME], str[80];
                    GetPlayerName(targetid, tname, sizeof(tname));
                    format(str, sizeof(str), "Da ha rank %s xuong Rank %d.", tname, newRank);
                    SendClientMessageEx(playerid, COLOR_GREEN, str);
                    format(str, sizeof(str), "Ban da bi ha xuong Rank %d boi %s.", newRank, ReturnPlayerName(playerid));
                    SendClientMessageEx(targetid, COLOR_RED, str);
                }
                case 2: // Kick
                {
                    // Rank 6 không bị kick; chỉ kick được rank thấp hơn mình
                    if(tRank >= 6)
                        return SendClientMessageEx(playerid, COLOR_RED, "Khong the kick thanh vien rank 6."), 1;
                    if(tRank >= rank)
                        return SendClientMessageEx(playerid, COLOR_RED, "Chi kick duoc nguoi co rank thap hon ban."), 1;

                    PlayerInfo[targetid][pFMember] = INVALID_FAMILY_ID;
                    PlayerInfo[targetid][pRank]    = 0;
                    FamilyInfo[fam][FamilyMembers]--;
                    new q3[128];
                    mysql_format(MainPipeline, q3, sizeof(q3),
                        "UPDATE `accounts` SET `FMember`=255,`Rank`=0 WHERE `id`=%d", PlayerInfo[targetid][pId]);
                    mysql_tquery(MainPipeline, q3, "", "");
                    SaveFamily(fam);

                    new tname[MAX_PLAYER_NAME], str[80];
                    GetPlayerName(targetid, tname, sizeof(tname));
                    format(str, sizeof(str), "Da kick %s khoi gang.", tname);
                    SendClientMessageEx(playerid, COLOR_GREEN, str);
                    SendClientMessageEx(targetid, COLOR_RED, "Ban da bi kick khoi gang.");
                }
            }
            return 1;
        }

        // ---- Két sắt sub-menu ----
        case DIALOG_GANGS_SAFE:
        {
            if(!response)
            {
                ShowPlayerDialog(playerid, DIALOG_GANGS_MAIN, DIALOG_STYLE_LIST,
                    "Quan Ly Gang",
                    "Thong tin gang\nQuan ly thanh vien\nKet sac\nNgan hang gang",
                    "Chon", "Dong");
                return 1;
            }
            switch(listitem)
            {
                case 0: // Đặt vị trí két về vị trí hiện tại
                {
                    new Float:px, Float:py, Float:pz;
                    GetPlayerPos(playerid, px, py, pz);
                    FamilyInfo[fam][FamilySafe][0] = px;
                    FamilyInfo[fam][FamilySafe][1] = py;
                    FamilyInfo[fam][FamilySafe][2] = pz;

                    // Hủy pickup cũ và tạo mới
                    if(FamilyInfo[fam][FamilyPickup] != -1)
                        DestroyPickup(FamilyInfo[fam][FamilyPickup]);
                    FamilyInfo[fam][FamilyPickup] = CreatePickup(1239, 23, px, py, pz, -1);

                    SaveFamily(fam);
                    SendClientMessageEx(playerid, COLOR_GREEN, "Da cap nhat vi tri ket sac gang.");
                }
                case 1: // Nâng cấp két
                {
                    new lv     = FamilyInfo[fam][FamilyLevel];
                    new nextLv = lv + 1;
                    if(nextLv > MAX_SAFE_LEVEL)
                        return SendClientMessageEx(playerid, COLOR_RED, "Ket sac da dat cap do toi da."), 1;

                    new str[256];
                    format(str, sizeof(str),
                        "Nang cap ket sac len Level %d?\n\nChi phi:\n  Tien mat (tren nguoi): $%d\n  Vat lieu (tren nguoi): %d\n\nKhi nang cap:\n  Tien max: $%d\n  Hang max: %d\n  VL max: %d\n  Slot VK: %d\n  Thanh vien max: %d",
                        nextLv,
                        FA_GetUpgradeCostCash(nextLv),
                        FA_GetUpgradeCostMats(nextLv),
                        FA_GetMaxCash(nextLv),
                        FA_GetMaxGoods(nextLv),
                        FA_GetMaxMats(nextLv),
                        FA_GetMaxGunSlots(nextLv),
                        FA_GetMaxMembers(nextLv));
                    ShowPlayerDialog(playerid, DIALOG_GANGS_SAFE_UPGRADE, DIALOG_STYLE_MSGBOX,
                        "Xac Nhan Nang Cap Ket", str, "Nang Cap", "Huy");
                }
            }
            return 1;
        }

        case DIALOG_GANGS_SAFE_UPGRADE:
        {
            if(!response) return 1;
            FA_UpgradeSafe(playerid, fam);
            return 1;
        }
    }
    return 0;
}
