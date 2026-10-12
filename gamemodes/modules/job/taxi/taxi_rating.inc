#include <YSI\YSI_Coding\y_hooks>

// ============================================================
// DEFINES
// ============================================================
#define DIALOG_RATING_MENU      15000
#define DIALOG_RATING_VIEW      15001
#define DIALOG_RATING_STARS     15002
#define DIALOG_RATING_COMMENT   15003
#define DIALOG_RATING_CONFIRM   15004
#define DIALOG_RATING_LIST      15005
#define DIALOG_RATING_MEMBERS   15006
#define DIALOG_RATING_DETAIL    15007

#define MAX_TAXI_RATINGS        5000

// Check ca tai xe faction (group type 7) lan tai xe tu do (pJob)
#define IsTaxiWorker(%0) (IsATaxiDriver(%0) || PlayerInfo[%0][pJob] == JOB_TAXIDRIVER || PlayerInfo[%0][pJob2] == JOB_TAXIDRIVER)

// ============================================================
// ENUMS
// ============================================================
enum E_TAXI_RATING {
    r_DriverID,
    r_DriverName[MAX_PLAYER_NAME],
    r_RaterID,
    r_RaterName[MAX_PLAYER_NAME],
    r_Stars,
    r_Comment[128],
    r_Date[32],
    bool:r_Valid
};

// ============================================================
// VARIABLES
// ============================================================
new TaxiRatings[MAX_TAXI_RATINGS][E_TAXI_RATING];
new TotalRatings = 0;

// ============================================================
// SAVE / LOAD - File based (tuong tu mechanic_repair.inc)
// ============================================================
stock SaveTaxiRating(ratingid)
{
    if(ratingid < 0 || ratingid >= MAX_TAXI_RATINGS) return 0;
    if(!TaxiRatings[ratingid][r_Valid]) return 0;

    // Loai bo ky tu | trong comment de tranh loi dinh dang file
    new safeComment[128];
    format(safeComment, sizeof(safeComment), "%s", TaxiRatings[ratingid][r_Comment]);
    for(new i = 0; safeComment[i] != EOS; i++) {
        if(safeComment[i] == '|') safeComment[i] = ' ';
    }

    new line[320], File:f = fopen("TaxiRatings.cfg", io_append);
    if(f == File:0) return 0;
    format(line, sizeof(line), "%d|%s|%d|%s|%d|%s|%s\r\n",
        TaxiRatings[ratingid][r_DriverID],
        TaxiRatings[ratingid][r_DriverName],
        TaxiRatings[ratingid][r_RaterID],
        TaxiRatings[ratingid][r_RaterName],
        TaxiRatings[ratingid][r_Stars],
        safeComment,
        TaxiRatings[ratingid][r_Date]);
    fwrite(f, line);
    fclose(f);
    return 1;
}

stock LoadAllTaxiRatings()
{
    for(new i = 0; i < MAX_TAXI_RATINGS; i++) {
        TaxiRatings[i][r_DriverID] = 0;
        TaxiRatings[i][r_DriverName][0] = EOS;
        TaxiRatings[i][r_RaterID] = 0;
        TaxiRatings[i][r_RaterName][0] = EOS;
        TaxiRatings[i][r_Stars] = 0;
        TaxiRatings[i][r_Comment][0] = EOS;
        TaxiRatings[i][r_Date][0] = EOS;
        TaxiRatings[i][r_Valid] = false;
    }
    TotalRatings = 0;

    if(!fexist("TaxiRatings.cfg")) {
        printf("[TaxiRatings] File TaxiRatings.cfg chua ton tai, se tao khi co danh gia dau tien.");
        return 1;
    }

    new line[320], File:f = fopen("TaxiRatings.cfg", io_read);
    if(f == File:0) return 1;

    while(TotalRatings < MAX_TAXI_RATINGS && fread(f, line)) {
        if(!sscanf(line, "p<|>ds[24]ds[24]ds[128]s[32]",
            TaxiRatings[TotalRatings][r_DriverID],
            TaxiRatings[TotalRatings][r_DriverName],
            TaxiRatings[TotalRatings][r_RaterID],
            TaxiRatings[TotalRatings][r_RaterName],
            TaxiRatings[TotalRatings][r_Stars],
            TaxiRatings[TotalRatings][r_Comment],
            TaxiRatings[TotalRatings][r_Date]))
        {
            TaxiRatings[TotalRatings][r_Valid] = true;
            TotalRatings++;
        }
    }
    fclose(f);
    printf("[TaxiRatings] Da tai %d danh gia tu file.", TotalRatings);
    return 1;
}

// ============================================================
// LOGIC
// ============================================================
stock CreateNewRating(playerid, driverid, stars, const comment[])
{
    if(TotalRatings >= MAX_TAXI_RATINGS) return -1;

    new ratingid = TotalRatings;

    TaxiRatings[ratingid][r_DriverID] = PlayerInfo[driverid][pId];
    format(TaxiRatings[ratingid][r_DriverName], MAX_PLAYER_NAME, "%s", GetPlayerNameEx(driverid));
    TaxiRatings[ratingid][r_RaterID] = PlayerInfo[playerid][pId];
    format(TaxiRatings[ratingid][r_RaterName], MAX_PLAYER_NAME, "%s", GetPlayerNameEx(playerid));
    TaxiRatings[ratingid][r_Stars] = stars;
    format(TaxiRatings[ratingid][r_Comment], 128, "%s", comment);

    new year, month, day, hour, minute;
    getdate(year, month, day);
    gettime(hour, minute);
    format(TaxiRatings[ratingid][r_Date], 32, "%02d/%02d/%d %02d:%02d", day, month, year, hour, minute);

    TaxiRatings[ratingid][r_Valid] = true;
    TotalRatings++;

    return ratingid;
}

stock ShowDriverRatings(playerid, targetid)
{
    new totalStars = 0, totalValid = 0;
    for(new i = 0; i < TotalRatings; i++) {
        if(!TaxiRatings[i][r_Valid]) continue;
        if(TaxiRatings[i][r_DriverID] == PlayerInfo[targetid][pId]) {
            totalStars += TaxiRatings[i][r_Stars];
            totalValid++;
        }
    }

    new Float:avg = (totalValid > 0) ? (float(totalStars) / float(totalValid)) : 0.0;

    if(totalValid == 0) {
        new string[256];
        format(string, sizeof(string), "Chua co danh gia nao cho tai xe %s.", GetPlayerNameEx(targetid));
        ShowPlayerDialog(playerid, DIALOG_RATING_VIEW, DIALOG_STYLE_MSGBOX, "Danh gia tai xe", string, "Dong", "");
        return 1;
    }

    new dialogString[2048] = "Nguoi danh gia\tSo sao\tNhan xet\n";
    new dialogCount = 0;

    for(new i = 0; i < TotalRatings; i++) {
        if(!TaxiRatings[i][r_Valid]) continue;
        if(TaxiRatings[i][r_DriverID] != PlayerInfo[targetid][pId]) continue;

        new shortComment[32];
        if(strlen(TaxiRatings[i][r_Comment]) > 20) {
            strmid(shortComment, TaxiRatings[i][r_Comment], 0, 20);
            strcat(shortComment, "...");
        } else {
            format(shortComment, sizeof(shortComment), "%s", TaxiRatings[i][r_Comment]);
        }

        format(dialogString, sizeof(dialogString), "%s%s\t%d/5\t%s\n",
            dialogString, TaxiRatings[i][r_RaterName], TaxiRatings[i][r_Stars], shortComment);
        SetPVarInt(playerid, sprintf("ViewRating_%d", dialogCount), i);
        dialogCount++;
    }

    SetPVarInt(playerid, "TotalViewRatings", dialogCount);

    new title[96];
    format(title, sizeof(title), "Danh gia cua %s (TB: %.1f/5 tu %d luot)",
        GetPlayerNameEx(targetid), avg, totalValid);
    ShowPlayerDialog(playerid, DIALOG_RATING_LIST, DIALOG_STYLE_TABLIST_HEADERS,
        title, dialogString, "Chi tiet", "Dong");
    return 1;
}

stock ShowRatingDetail(playerid, ratingid)
{
    if(ratingid < 0 || ratingid >= TotalRatings) return 0;
    if(!TaxiRatings[ratingid][r_Valid]) return 0;

    new string[512];
    format(string, sizeof(string),
        "{FFFFFF}Thong tin danh gia:\n\n\
Tai xe: %s\n\
Danh gia boi: %s\n\
So sao: %d/5\n\
Nhan xet: %s\n\
Ngay: %s",
        TaxiRatings[ratingid][r_DriverName],
        TaxiRatings[ratingid][r_RaterName],
        TaxiRatings[ratingid][r_Stars],
        TaxiRatings[ratingid][r_Comment],
        TaxiRatings[ratingid][r_Date]);
    ShowPlayerDialog(playerid, DIALOG_RATING_DETAIL, DIALOG_STYLE_MSGBOX, "Chi tiet danh gia", string, "Dong", "");
    return 1;
}

stock ShowTaxiMembers(playerid)
{
    new count = 0;
    new string[2048] = "Ten\tXep hang\n";

    foreach(new i: Player) {
        if(!IsTaxiWorker(i)) continue;

        new totalStars = 0, totalValid = 0;
        for(new j = 0; j < TotalRatings; j++) {
            if(!TaxiRatings[j][r_Valid]) continue;
            if(TaxiRatings[j][r_DriverID] == PlayerInfo[i][pId]) {
                totalStars += TaxiRatings[j][r_Stars];
                totalValid++;
            }
        }
        new Float:avg = (totalValid > 0) ? (float(totalStars) / float(totalValid)) : 0.0;
        format(string, sizeof(string), "%s%s\t%.1f/5 (%d luot)\n",
            string, GetPlayerNameEx(i), avg, totalValid);
        count++;
    }

    if(count == 0) {
        SendClientMessageEx(playerid, COLOR_GREY, "Khong co tai xe taxi nao online!");
        return 0;
    }

    ShowPlayerDialog(playerid, DIALOG_RATING_MEMBERS, DIALOG_STYLE_TABLIST_HEADERS,
        "Danh gia thanh vien taxi", string, "Xem", "Dong");
    return 1;
}

stock ShowAllDriverRatings(playerid, targetid)
{
    new count = 0;
    new string[4096] = "{ffffff}Nguoi danh gia\t{ffffff}Danh gia\t{ffffff}Thoi gian\n";

    for(new i = 0; i < TotalRatings; i++) {
        if(!TaxiRatings[i][r_Valid]) continue;
        if(TaxiRatings[i][r_DriverID] != PlayerInfo[targetid][pId]) continue;

        new shortComment[32];
        if(strlen(TaxiRatings[i][r_Comment]) > 15) {
            strmid(shortComment, TaxiRatings[i][r_Comment], 0, 15);
            strcat(shortComment, "...");
        } else {
            format(shortComment, sizeof(shortComment), "%s", TaxiRatings[i][r_Comment]);
        }

        format(string, sizeof(string), "%s%s\t%d/5 (%s)\t%s\n",
            string, TaxiRatings[i][r_RaterName], TaxiRatings[i][r_Stars],
            shortComment, TaxiRatings[i][r_Date]);
        SetPVarInt(playerid, sprintf("Rating_%d", count), i);
        count++;
    }

    if(count == 0) {
        SendClientMessageEx(playerid, COLOR_GREY, "Chua co danh gia nao cho tai xe nay!");
        return 0;
    }

    SetPVarInt(playerid, "TotalShownRatings", count);

    ShowPlayerDialog(playerid, DIALOG_RATING_LIST, DIALOG_STYLE_TABLIST_HEADERS,
        sprintf("Danh gia cua %s", GetPlayerNameEx(targetid)),
        string, "Chi tiet", "Dong");
    return 1;
}

// ============================================================
// HOOKS
// ============================================================
hook OnGameModeInit()
{
    LoadAllTaxiRatings();
    return 1;
}

// ============================================================
// COMMANDS
// ============================================================
CMD:danhgia(playerid, params[])
{
    // Truong hop 1: /danhgia (khong tham so) -> danh gia tai xe vua cho di
    if(isnull(params))
    {
        if(LastTaxiDriver[playerid] == INVALID_PLAYER_ID)
            return SendHelpMessage(playerid, "/danhgia [ID tai xe]");

        new targetid = LastTaxiDriver[playerid];
        if(!IsPlayerConnected(targetid))
            return SendClientMessageEx(playerid, COLOR_GREY, "Tai xe cua ban da roi server, khong the danh gia!");

        ShowPlayerDialog(playerid, DIALOG_RATING_MENU, DIALOG_STYLE_LIST,
            sprintf("Danh gia tai xe %s", GetPlayerNameEx(targetid)),
            "Danh gia tai xe\nXem danh gia gan day", "Chon", "Dong");
        return 1;
    }

    // Truong hop 2: /danhgia [ID] -> danh gia tai xe dang online gan do
    new targetid = strval(params);
    if(!IsPlayerConnected(targetid))
        return SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi khong ton tai!");
    if(targetid == playerid)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong the tu danh gia ban than!");
    if(!IsTaxiWorker(targetid))
        return SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi nay khong phai la tai xe taxi!");

    new Float:tx, Float:ty, Float:tz;
    GetPlayerPos(targetid, tx, ty, tz);
    if(!IsPlayerInRangeOfPoint(playerid, 5.0, tx, ty, tz))
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban can dung gan tai xe de co the danh gia!");

    LastTaxiDriver[playerid] = targetid;

    ShowPlayerDialog(playerid, DIALOG_RATING_MENU, DIALOG_STYLE_LIST,
        sprintf("Danh gia tai xe %s", GetPlayerNameEx(targetid)),
        "Danh gia tai xe\nXem danh gia gan day", "Chon", "Dong");
    return 1;
}

CMD:xemdanhgia(playerid, params[])
{
    new targetid;
    if(sscanf(params, "u", targetid)) return SendHelpMessage(playerid, "/xemdanhgia [ID tai xe]");
    if(!IsPlayerConnected(targetid)) return SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi khong ton tai!");
    if(!IsTaxiWorker(targetid)) return SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi nay khong phai tai xe taxi!");

    new Float:tx, Float:ty, Float:tz;
    GetPlayerPos(targetid, tx, ty, tz);
    if(!IsPlayerInRangeOfPoint(playerid, 5.0, tx, ty, tz))
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong dung gan tai xe nay!");

    ShowDriverRatings(playerid, targetid);
    return 1;
}

// ============================================================
// DIALOG RESPONSES
// ============================================================
hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid)
    {
        case DIALOG_RATING_MENU:
        {
            if(!response) return 1;
            switch(listitem) {
                case 0: {
                    ShowPlayerDialog(playerid, DIALOG_RATING_STARS, DIALOG_STYLE_LIST,
                        sprintf("Danh gia tai xe %s", GetPlayerNameEx(LastTaxiDriver[playerid])),
                        "1 sao - Rat te\n2 sao - Te\n3 sao - Binh thuong\n4 sao - Tot\n5 sao - Xuat sac",
                        "Chon", "Quay lai");
                }
                case 1: ShowDriverRatings(playerid, LastTaxiDriver[playerid]);
            }
        }

        case DIALOG_RATING_STARS:
        {
            if(!response) {
                pc_cmd_danhgia(playerid, "");
                return 1;
            }
            SetPVarInt(playerid, "Rating_Stars", listitem + 1);
            ShowPlayerDialog(playerid, DIALOG_RATING_COMMENT, DIALOG_STYLE_INPUT,
                sprintf("Danh gia tai xe %s", GetPlayerNameEx(LastTaxiDriver[playerid])),
                "Hay nhap nhan xet cho tai xe (toi da 128 ky tu):",
                "Gui", "Quay lai");
        }

        case DIALOG_RATING_COMMENT:
        {
            if(!response) {
                ShowPlayerDialog(playerid, DIALOG_RATING_STARS, DIALOG_STYLE_LIST,
                    sprintf("Danh gia tai xe %s", GetPlayerNameEx(LastTaxiDriver[playerid])),
                    "1 sao - Rat te\n2 sao - Te\n3 sao - Binh thuong\n4 sao - Tot\n5 sao - Xuat sac",
                    "Chon", "Quay lai");
                return 1;
            }
            if(strlen(inputtext) > 128) {
                SendClientMessageEx(playerid, COLOR_GREY, "Nhan xet qua dai (toi da 128 ky tu)!");
                ShowPlayerDialog(playerid, DIALOG_RATING_COMMENT, DIALOG_STYLE_INPUT,
                    sprintf("Danh gia tai xe %s", GetPlayerNameEx(LastTaxiDriver[playerid])),
                    "Hay nhap nhan xet cho tai xe (toi da 128 ky tu):",
                    "Gui", "Quay lai");
                return 1;
            }
            SetPVarString(playerid, "Rating_Comment", inputtext);

            new stars = GetPVarInt(playerid, "Rating_Stars");
            new comment[128];
            GetPVarString(playerid, "Rating_Comment", comment, sizeof(comment));

            new string[512];
            format(string, sizeof(string),
                "{FFFFFF}Xac nhan danh gia:\n\nTai xe: %s\nSo sao: %d/5\nNhan xet: %s\n\nBan co muon gui danh gia nay khong?",
                GetPlayerNameEx(LastTaxiDriver[playerid]), stars, comment);
            ShowPlayerDialog(playerid, DIALOG_RATING_CONFIRM, DIALOG_STYLE_MSGBOX,
                "Xac nhan danh gia", string, "Gui", "Huy");
        }

        case DIALOG_RATING_CONFIRM:
        {
            if(!response) {
                ShowPlayerDialog(playerid, DIALOG_RATING_COMMENT, DIALOG_STYLE_INPUT,
                    sprintf("Danh gia tai xe %s", GetPlayerNameEx(LastTaxiDriver[playerid])),
                    "Hay nhap nhan xet cho tai xe (toi da 128 ky tu):",
                    "Gui", "Quay lai");
                return 1;
            }

            new stars = GetPVarInt(playerid, "Rating_Stars");
            new comment[128];
            GetPVarString(playerid, "Rating_Comment", comment, sizeof(comment));

            new ratingid = CreateNewRating(playerid, LastTaxiDriver[playerid], stars, comment);
            if(ratingid == -1) {
                SendClientMessageEx(playerid, COLOR_RED, "Loi khi tao danh gia (da dat gioi han %d)!", MAX_TAXI_RATINGS);
                return 1;
            }

            SaveTaxiRating(ratingid);

            SendClientMessageEx(playerid, COLOR_YELLOW, "Cam on ban da danh gia! Danh gia cua ban da duoc luu.");

            if(IsPlayerConnected(LastTaxiDriver[playerid])) {
                SendClientMessageEx(LastTaxiDriver[playerid], COLOR_YELLOW,
                    "{FFFF00}%s {ffffff}da danh gia dich vu cua ban {FFFF00}%d/5 sao.", GetPlayerNameEx(playerid), stars);
            }

            LastTaxiDriver[playerid] = INVALID_PLAYER_ID;
            DeletePVar(playerid, "Rating_Stars");
            DeletePVar(playerid, "Rating_Comment");
        }

        case DIALOG_RATING_MEMBERS:
        {
            if(!response) return 1;

            new targetid = INVALID_PLAYER_ID, counter = 0;
            foreach(new i: Player) {
                if(IsTaxiWorker(i)) {
                    if(counter == listitem) { targetid = i; break; }
                    counter++;
                }
            }

            if(targetid == INVALID_PLAYER_ID) {
                SendClientMessageEx(playerid, COLOR_GREY, "Tai xe da khong con online!");
                return 1;
            }
            ShowAllDriverRatings(playerid, targetid);
        }

        case DIALOG_RATING_LIST:
        {
            if(!response) return 1;

            new ratingid = GetPVarInt(playerid, sprintf("ViewRating_%d", listitem));
            ShowRatingDetail(playerid, ratingid);

            new total = GetPVarInt(playerid, "TotalViewRatings");
            for(new i = 0; i < total; i++) DeletePVar(playerid, sprintf("ViewRating_%d", i));
            DeletePVar(playerid, "TotalViewRatings");
        }

        case DIALOG_RATING_VIEW:  return 1;
        case DIALOG_RATING_DETAIL: return 1;
    }
    return 1;
}
