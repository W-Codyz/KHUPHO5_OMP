// ===========================================================================
// gamemodes/modules/system/family/admin.inc
// Admin commands /fcreate và /fdelete dùng dialog
//
// TÍCH HỢP:
//   Thêm vào OnDialogResponse:
//     if(FAdmin_OnDialogResponse(playerid, dialogid, response, listitem, inputtext)) return 1;
// ===========================================================================

// Dialog IDs (tiếp theo management.inc kết thúc tại 5362)
#define DIALOG_FADMIN_CREATE_SLOT    5370
#define DIALOG_FADMIN_CREATE_PLAYER  5371
#define DIALOG_FADMIN_DELETE_LIST    5372
#define DIALOG_FADMIN_DELETE_CONFIRM 5373
#define DIALOG_FADMIN_SWITCH_LIST    5374
#define DIALOG_FADMIN_SWITCH_TARGET  5375
#define DIALOG_FADMIN_RENAME_LIST    5376
#define DIALOG_FADMIN_RENAME_INPUT   5377

// Per-player: slot family đang được chọn
new g_fadminSelectedFamily[MAX_PLAYERS] = {-1, ...};
new g_fadminSwitchTarget[MAX_PLAYERS]   = {-1, ...}; // Target player cho /switchgang

// ---------------------------------------------------------------------------
// INTERNAL HELPERS
// ---------------------------------------------------------------------------

static stock FAdmin_ShowCreateSlotDialog(playerid)
{
    new list[512]; list[0] = 0;
    new count = 0;
    for(new f = 1; f < MAX_FAMILY; f++)
    {
        if(FamilyInfo[f][FamilyTaken] == 0)
        {
            new line[32];
            format(line, sizeof(line), "Slot %d\n", f);
            strcat(list, line, sizeof(list));
            count++;
        }
    }
    if(count == 0)
    {
        SendClientMessageEx(playerid, COLOR_GREY, "   Khong con slot gang nao trong!");
        return;
    }
    ShowPlayerDialog(playerid, DIALOG_FADMIN_CREATE_SLOT, DIALOG_STYLE_LIST,
        "Tao Gang - Chon Slot Trong", list, "Chon", "Huy");
}

// Map listitem trong dialog list → family slot thực (lọc theo điều kiện)
static stock FAdmin_MapListItem(condition, listitem)
{
    // condition: 0 = tìm slot trống (FamilyTaken==0), 1 = tìm slot có gang (FamilyTaken==1)
    new cnt = -1;
    for(new f = 1; f < MAX_FAMILY; f++)
    {
        new taken = FamilyInfo[f][FamilyTaken];
        if((condition == 0 && taken == 0) || (condition == 1 && taken == 1))
        {
            cnt++;
            if(cnt == listitem) return f;
        }
    }
    return -1;
}

// ---------------------------------------------------------------------------
// LỆNH /fcreate - Tạo gang mới (dialog-based)
// ---------------------------------------------------------------------------

CMD:fcreate(playerid, params[])
{
    #pragma unused params
    if(PlayerInfo[playerid][pAdmin] <= 3 && PlayerInfo[playerid][pGangModerator] < 1)
        return 1;

    g_fadminSelectedFamily[playerid] = -1;
    FAdmin_ShowCreateSlotDialog(playerid);
    return 1;
}

// ---------------------------------------------------------------------------
// LỆNH /fdelete - Xóa gang (dialog-based)
// ---------------------------------------------------------------------------

CMD:fdelete(playerid, params[])
{
    #pragma unused params
    if(PlayerInfo[playerid][pAdmin] <= 3 && PlayerInfo[playerid][pGangModerator] < 1)
        return 1;

    new list[768]; list[0] = 0;
    new count = 0;
    for(new f = 1; f < MAX_FAMILY; f++)
    {
        if(FamilyInfo[f][FamilyTaken] == 1)
        {
            new line[56];
            format(line, sizeof(line), "Slot %d: %s\n", f, FamilyInfo[f][FamilyName]);
            strcat(list, line, sizeof(list));
            count++;
        }
    }

    if(count == 0)
    {
        SendClientMessageEx(playerid, COLOR_GREY, "   Khong co gang nao de xoa!");
        return 1;
    }

    g_fadminSelectedFamily[playerid] = -1;
    ShowPlayerDialog(playerid, DIALOG_FADMIN_DELETE_LIST, DIALOG_STYLE_LIST,
        "Xoa Gang - Chon Slot", list, "Tiep Tuc", "Huy");
    return 1;
}

// ---------------------------------------------------------------------------
// LỆNH /fname - Admin đổi tên gang (dialog-based)
// ---------------------------------------------------------------------------

CMD:fname(playerid, params[])
{
    #pragma unused params
    if(PlayerInfo[playerid][pAdmin] <= 3 && PlayerInfo[playerid][pGangModerator] < 1)
        return 1;

    new list[768]; list[0] = 0;
    new count = 0;
    for(new f = 1; f < MAX_FAMILY; f++)
    {
        if(FamilyInfo[f][FamilyTaken] == 1)
        {
            new line[64];
            format(line, sizeof(line), "Slot %d: %s\n", f, FamilyInfo[f][FamilyName]);
            strcat(list, line, sizeof(list));
            count++;
        }
    }

    if(count == 0)
    {
        SendClientMessageEx(playerid, COLOR_GREY, "   Khong co gang nao.");
        return 1;
    }

    g_fadminSelectedFamily[playerid] = -1;
    ShowPlayerDialog(playerid, DIALOG_FADMIN_RENAME_LIST, DIALOG_STYLE_LIST,
        "Doi Ten Gang - Chon Gang", list, "Chon", "Huy");
    return 1;
}

// ---------------------------------------------------------------------------
// LỆNH /switchgang - Chuyển player sang gang khác (dialog-based)
// ---------------------------------------------------------------------------

static stock FAdmin_ShowSwitchGangList(playerid)
{
    new list[768]; list[0] = 0;
    new count = 0;
    for(new f = 1; f < MAX_FAMILY; f++)
    {
        if(FamilyInfo[f][FamilyTaken] == 1)
        {
            new line[64];
            new onlineCnt = 0;
            foreach(new i : Player) if(PlayerInfo[i][pFMember] == f) onlineCnt++;
            format(line, sizeof(line), "{AAAAAA}#%d  {FFFFFF}%s  {44FF44}(%d online)\n",
                f, FamilyInfo[f][FamilyName], onlineCnt);
            strcat(list, line, sizeof(list));
            count++;
        }
    }
    if(count == 0)
    {
        SendClientMessageEx(playerid, COLOR_GREY, "   Khong co gang nao.");
        return;
    }
    ShowPlayerDialog(playerid, DIALOG_FADMIN_SWITCH_LIST, DIALOG_STYLE_LIST,
        "Switch Gang - Chon Gang", list, "Chon", "Huy");
}

CMD:switchgang(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] <= 3 && PlayerInfo[playerid][pGangModerator] < 1)
        return 1;

    new targetid;
    if(sscanf(params, "u", targetid)) return SendHelpMessage(playerid, "/switchgang [playerid]");
    if(!IsPlayerConnected(targetid))
        return SendClientMessageEx(playerid, COLOR_RED, "Nguoi choi khong online."), 1;

    g_fadminSwitchTarget[playerid] = targetid;
    FAdmin_ShowSwitchGangList(playerid);
    return 1;
}

// ---------------------------------------------------------------------------
// DIALOG RESPONSE
// ---------------------------------------------------------------------------

stock FAdmin_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid)
    {
        // ---- Bước 1 /fcreate: chọn slot trống ----
        case DIALOG_FADMIN_CREATE_SLOT:
        {
            if(!response) return 1;

            new family = FAdmin_MapListItem(0, listitem);
            if(family == -1)
            {
                SendClientMessageEx(playerid, COLOR_GREY, "   Slot khong hop le.");
                return 1;
            }

            g_fadminSelectedFamily[playerid] = family;

            new title[48];
            format(title, sizeof(title), "Tao Gang - Slot %d", family);
            ShowPlayerDialog(playerid, DIALOG_FADMIN_CREATE_PLAYER, DIALOG_STYLE_INPUT,
                title, "Nhap Player ID (so) de dat lam Leader:", "Tao", "Quay Lai");
            return 1;
        }

        // ---- Bước 2 /fcreate: nhập player ID → thực hiện tạo gang ----
        case DIALOG_FADMIN_CREATE_PLAYER:
        {
            if(!response)
            {
                // Quay về bước chọn slot
                FAdmin_ShowCreateSlotDialog(playerid);
                return 1;
            }

            new family = g_fadminSelectedFamily[playerid];
            g_fadminSelectedFamily[playerid] = -1;

            if(family < 1 || family >= MAX_FAMILY) return 1;

            if(FamilyInfo[family][FamilyTaken] == 1)
            {
                SendClientMessageEx(playerid, COLOR_GREY, "   Slot nay vua bi ai do chiem truoc!");
                return 1;
            }

            new giveplayerid = strval(inputtext);
            if(!IsPlayerConnected(giveplayerid))
            {
                SendClientMessageEx(playerid, COLOR_GREY, "   Nguoi choi khong hop le.");
                return 1;
            }

            // Thực hiện tạo gang
            new string[128];
            format(string, sizeof(string), "* Ban da tao %s thanh Leader cua Gang Slot %d.", GetPlayerNameEx(giveplayerid), family);
            SendClientMessageEx(playerid, COLOR_LIGHTBLUE, string);
            format(string, sizeof(string), "* Admin %s da dat ban lam Leader gang.", GetPlayerNameEx(playerid));
            SendClientMessageEx(giveplayerid, COLOR_LIGHTBLUE, string);

            new sendername[MAX_PLAYER_NAME];
            GetPlayerName(giveplayerid, sendername, sizeof(sendername));
            strmid(FamilyInfo[family][FamilyLeader], sendername, 0, strlen(sendername), 24);
            FamilyInfo[family][FamilyMembers]++;
            FamilyInfo[family][FamilyTaken] = 1;
            PlayerInfo[giveplayerid][pFMember]  = family;
            PlayerInfo[giveplayerid][pRank]     = 6;
            PlayerInfo[giveplayerid][pDivision] = 0;
            SaveFamily(family);
            return 1;
        }

        // ---- Bước 1 /fdelete: chọn gang cần xóa ----
        case DIALOG_FADMIN_DELETE_LIST:
        {
            if(!response) return 1;

            new family = FAdmin_MapListItem(1, listitem);
            if(family == -1)
            {
                SendClientMessageEx(playerid, COLOR_GREY, "   Slot khong hop le.");
                return 1;
            }

            g_fadminSelectedFamily[playerid] = family;

            new str[160];
            format(str, sizeof(str),
                "Xac nhan xoa gang?\n\n  Slot : %d\n  Ten  : %s\n  Leader: %s\n\nHanh dong nay KHONG THE hoan tac!",
                family,
                FamilyInfo[family][FamilyName],
                FamilyInfo[family][FamilyLeader]);
            ShowPlayerDialog(playerid, DIALOG_FADMIN_DELETE_CONFIRM, DIALOG_STYLE_MSGBOX,
                "Xac Nhan Xoa Gang", str, "Xoa Ngay", "Huy");
            return 1;
        }

        // ---- Bước 2 /fdelete: xác nhận xóa → thực hiện ----
        case DIALOG_FADMIN_DELETE_CONFIRM:
        {
            new family = g_fadminSelectedFamily[playerid];
            g_fadminSelectedFamily[playerid] = -1;

            if(!response) return 1;
            if(family < 1 || family >= MAX_FAMILY) return 1;
            if(FamilyInfo[family][FamilyTaken] != 1) return 1;

            // Thông báo cho tất cả thành viên online trước khi xóa
            new notify[96];
            format(notify, sizeof(notify), "   Gang Slot %d (%s) da bi Admin %s xoa.",
                family, FamilyInfo[family][FamilyName], GetPlayerNameEx(playerid));
            foreach(new i : Player)
            {
                if(PlayerInfo[i][pFMember] == family)
                    SendClientMessageEx(i, COLOR_RED, notify);
            }

            // Reset data két sắt (ClearFamily không cover)
            FA_ResetSafe(family);

            ClearFamily(family);

            // Reset các field mà ClearFamily bỏ sót
            FamilyInfo[family][FamilyPickup] = 0;
            FamilyInfo[family][FamilyLevel]  = 1;
            FamilyInfo[family][FamilyFame]   = 0;
            FamilyInfo[family][FamilyBank]   = 0;

            SaveFamily(family);

            new str[64];
            format(str, sizeof(str), "   Da xoa gang Slot %d thanh cong.", family);
            SendClientMessageEx(playerid, COLOR_GREEN, str);
            return 1;
        }
        // ---- /fname: chọn gang ----
        case DIALOG_FADMIN_RENAME_LIST:
        {
            if(!response) return 1;

            new family = FAdmin_MapListItem(1, listitem);
            if(family == -1)
            {
                SendClientMessageEx(playerid, COLOR_GREY, "   Gang khong hop le.");
                return 1;
            }

            g_fadminSelectedFamily[playerid] = family;

            new title[64];
            format(title, sizeof(title), "Doi Ten Gang - Slot %d: %s", family, FamilyInfo[family][FamilyName]);
            ShowPlayerDialog(playerid, DIALOG_FADMIN_RENAME_INPUT, DIALOG_STYLE_INPUT,
                title, "Nhap ten moi cho gang (toi da 40 ky tu):", "Doi Ten", "Quay Lai");
            return 1;
        }

        // ---- /fname: nhập tên mới → thực hiện đổi tên ----
        case DIALOG_FADMIN_RENAME_INPUT:
        {
            new family = g_fadminSelectedFamily[playerid];
            g_fadminSelectedFamily[playerid] = -1;

            if(!response)
            {
                // Quay về danh sách chọn gang
                new list[768]; list[0] = 0;
                new count = 0;
                for(new f = 1; f < MAX_FAMILY; f++)
                {
                    if(FamilyInfo[f][FamilyTaken] == 1)
                    {
                        new line[64];
                        format(line, sizeof(line), "Slot %d: %s\n", f, FamilyInfo[f][FamilyName]);
                        strcat(list, line, sizeof(list));
                        count++;
                    }
                }
                if(count > 0)
                {
                    ShowPlayerDialog(playerid, DIALOG_FADMIN_RENAME_LIST, DIALOG_STYLE_LIST,
                        "Doi Ten Gang - Chon Gang", list, "Chon", "Huy");
                }
                return 1;
            }

            if(family < 1 || family >= MAX_FAMILY) return 1;
            if(FamilyInfo[family][FamilyTaken] != 1) return 1;

            if(!inputtext[0])
            {
                SendClientMessageEx(playerid, COLOR_GREY, "   Ten gang khong duoc de trong.");
                return 1;
            }
            if(strfind(inputtext, "|", true) != -1)
            {
                SendClientMessageEx(playerid, COLOR_GREY, "   Ten gang khong duoc chua ky tu '|'.");
                return 1;
            }
            if(strlen(inputtext) > 40)
            {
                SendClientMessageEx(playerid, COLOR_GREY, "   Ten gang qua dai (toi da 40 ky tu).");
                return 1;
            }

            new oldName[48];
            strmid(oldName, FamilyInfo[family][FamilyName], 0, sizeof(oldName), sizeof(oldName));

            strmid(FamilyInfo[family][FamilyName], inputtext, 0, strlen(inputtext), 100);
            SaveFamilies();

            new str[128];
            format(str, sizeof(str), "   Da doi ten gang Slot %d: %s -> %s", family, oldName, FamilyInfo[family][FamilyName]);
            SendClientMessageEx(playerid, COLOR_GREEN, str);

            new logstr[160], logfile[48], month, day, year;
            getdate(year, month, day);
            format(logstr, sizeof(logstr), "%s doi ten gang Slot %d tu '%s' thanh '%s'",
                GetPlayerNameEx(playerid), family, oldName, FamilyInfo[family][FamilyName]);
            format(logfile, sizeof(logfile), "family_logs/%d/%d-%02d-%02d.log", family, year, month, day);
            Log(logfile, logstr);
            return 1;
        }

        // ---- /switchgang: chọn gang ----
        case DIALOG_FADMIN_SWITCH_LIST:
        {
            if(!response) { g_fadminSwitchTarget[playerid] = -1; return 1; }

            new family = FAdmin_MapListItem(1, listitem);
            if(family == -1)
            {
                SendClientMessageEx(playerid, COLOR_GREY, "   Gang khong hop le.");
                return 1;
            }
            g_fadminSelectedFamily[playerid] = family;

            new targetid = g_fadminSwitchTarget[playerid];
            if(!IsPlayerConnected(targetid))
            {
                SendClientMessageEx(playerid, COLOR_RED, "Nguoi choi da thoat.");
                g_fadminSwitchTarget[playerid] = -1;
                return 1;
            }

            new oldGangName[32];
            new oldFam = PlayerInfo[targetid][pFMember];
            if(oldFam != INVALID_FAMILY_ID)
                strmid(oldGangName, FamilyInfo[oldFam][FamilyName], 0, 32, 32);
            else
                format(oldGangName, sizeof(oldGangName), "Khong co");

            new str[256];
            format(str, sizeof(str),
                "{AAAAAA}Xac nhan chuyen {FFFFFF}%s{AAAAAA} vao gang?\n\n\
{AAAAAA}  Gang  : {FFFFFF}%s {AAAAAA}(Slot #%d)\n\
{AAAAAA}  Rank  : {FFFF00}6 {AAAAAA}(tu dong)\n\
{AAAAAA}  Gang cu: {FF6B6B}%s",
                GetPlayerNameEx(targetid),
                FamilyInfo[family][FamilyName], family,
                oldGangName);
            ShowPlayerDialog(playerid, DIALOG_FADMIN_SWITCH_TARGET, DIALOG_STYLE_MSGBOX,
                "Xac Nhan Switch Gang", str, "Chuyen", "Huy");
            return 1;
        }

        // ---- /switchgang: xác nhận ----
        case DIALOG_FADMIN_SWITCH_TARGET:
        {
            new family   = g_fadminSelectedFamily[playerid];
            new targetid = g_fadminSwitchTarget[playerid];
            g_fadminSelectedFamily[playerid] = -1;
            g_fadminSwitchTarget[playerid]   = -1;

            if(!response) return 1;
            if(family < 1 || family >= MAX_FAMILY) return 1;
            if(!IsPlayerConnected(targetid))
            {
                SendClientMessageEx(playerid, COLOR_RED, "Nguoi choi da thoat.");
                return 1;
            }

            PlayerInfo[targetid][pFMember]  = family;
            PlayerInfo[targetid][pRank]     = 6;
            PlayerInfo[targetid][pDivision] = 0;
            PlayerInfo[targetid][pMember]   = INVALID_GROUP_ID;
            PlayerInfo[targetid][pLeader]   = INVALID_GROUP_ID;

            new str[128];
            format(str, sizeof(str), "Ban da duoc Admin chuyen vao gang %s (Rank 6).", FamilyInfo[family][FamilyName]);
            SendClientMessageEx(targetid, COLOR_LIGHTBLUE, str);
            format(str, sizeof(str), "Da chuyen %s vao gang %s (Slot #%d).",
                GetPlayerNameEx(targetid), FamilyInfo[family][FamilyName], family);
            SendClientMessageEx(playerid, COLOR_GREEN, str);
            return 1;
        }
    }
    return 0;
}
