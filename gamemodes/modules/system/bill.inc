#include <YSI\YSI_Coding\y_hooks>

// ============================================================
//  Bill System
// ============================================================

#define MAX_BILLS               5000
#define DIALOG_BILL_MENU        (12000)
#define DIALOG_BILL_BUYER       (12001)
#define DIALOG_BILL_CONTENT     (12002)
#define DIALOG_BILL_AMOUNT      (12003)
#define DIALOG_BILL_CONFIRM     (12004)
#define DIALOG_BILL_LIST        (12005)
#define DIALOG_BILL_DETAIL      (12006)
#define DIALOG_BILL_PAY         (12007)
#define DIALOG_BILL_ALL         (12008)
#define DIALOG_BILL_OPTIONS     (12009)
#define DIALOG_BILL_DECLINE     (12010)
#define DIALOG_BILL_PAY_OPTIONS (12011)

enum BillData {
    billDBID,
    billID,
    billSQLPlayerID,
    billSeller[MAX_PLAYER_NAME],
    billBuyer[MAX_PLAYER_NAME],
    billContent[128],
    billAmount,
    billDate[32],
    billStatus
}

new BillInfo[MAX_BILLS][BillData];
new TotalBills = 0;

// ============================================================
// Load
// ============================================================

forward OnLoadBills();
public OnLoadBills()
{
    new rows, fields;
    cache_get_data(rows, fields, MainPipeline);

    for(new i = 0; i < MAX_BILLS; i++)
    {
        BillInfo[i][billDBID]        = 0;
        BillInfo[i][billID]          = 0;
        BillInfo[i][billSQLPlayerID] = 0;
        BillInfo[i][billSeller][0]   = EOS;
        BillInfo[i][billBuyer][0]    = EOS;
        BillInfo[i][billContent][0]  = EOS;
        BillInfo[i][billAmount]      = 0;
        BillInfo[i][billDate][0]     = EOS;
        BillInfo[i][billStatus]      = 0;
    }
    TotalBills = 0;

    if(rows > 0)
    {
        for(new i = 0; i < rows && TotalBills < MAX_BILLS; i++)
        {
            new temp[128];

            BillInfo[TotalBills][billDBID]        = cache_get_field_content_int(i, "ID",       MainPipeline);
            BillInfo[TotalBills][billID]          = cache_get_field_content_int(i, "BillID",   MainPipeline);
            BillInfo[TotalBills][billSQLPlayerID] = cache_get_field_content_int(i, "PlayerID", MainPipeline);
            BillInfo[TotalBills][billAmount]      = cache_get_field_content_int(i, "Amount",   MainPipeline);
            BillInfo[TotalBills][billStatus]      = cache_get_field_content_int(i, "Status",   MainPipeline);

            cache_get_field_content(i, "Seller",  temp, MainPipeline, MAX_PLAYER_NAME);
            format(BillInfo[TotalBills][billSeller],  MAX_PLAYER_NAME, "%s", temp);

            cache_get_field_content(i, "Buyer",   temp, MainPipeline, MAX_PLAYER_NAME);
            format(BillInfo[TotalBills][billBuyer],   MAX_PLAYER_NAME, "%s", temp);

            cache_get_field_content(i, "Content", temp, MainPipeline, 128);
            format(BillInfo[TotalBills][billContent], 128, "%s", temp);

            cache_get_field_content(i, "Date",    temp, MainPipeline, 32);
            format(BillInfo[TotalBills][billDate],    32,  "%s", temp);

            TotalBills++;
        }
    }

    printf("[Bills] Loaded %d bills.", TotalBills);
    return 1;
}

stock LoadAllBills()
{
    mysql_tquery(MainPipeline,
        "SELECT * FROM `bills` WHERE `Status` = 0",
        "OnLoadBills");
    return 1;
}

// ============================================================
// Save / Create
// ============================================================

forward OnBillCreated(billid);
public OnBillCreated(billid)
{
    BillInfo[billid][billDBID] = cache_insert_id();
    return 1;
}

stock Bill_Save(billid)
{
    new query[512], escaped[256];
    mysql_real_escape_string(BillInfo[billid][billContent], escaped);

    if(BillInfo[billid][billDBID] == 0)
    {
        format(query, sizeof(query),
            "INSERT INTO `bills` (`BillID`,`PlayerID`,`Seller`,`Buyer`,`Content`,`Amount`,`Date`,`Status`) VALUES ('%d','%d','%s','%s','%s','%d','%s','%d')",
            BillInfo[billid][billID],
            BillInfo[billid][billSQLPlayerID],
            BillInfo[billid][billSeller],
            BillInfo[billid][billBuyer],
            escaped,
            BillInfo[billid][billAmount],
            BillInfo[billid][billDate],
            BillInfo[billid][billStatus]);
        mysql_tquery(MainPipeline, query, "OnBillCreated", "i", billid);
    }
    else
    {
        format(query, sizeof(query),
            "UPDATE `bills` SET `Status`='%d', `Content`='%s' WHERE `ID`='%d'",
            BillInfo[billid][billStatus],
            escaped,
            BillInfo[billid][billDBID]);
        mysql_tquery(MainPipeline, query);
    }
    return 1;
}

stock GetBillByID(billid)
{
    for(new i = 0; i < TotalBills; i++)
    {
        if(BillInfo[i][billID] == billid)
            return i;
    }
    return -1;
}

stock CreateNewBill(playerid, targetid, const content[], amount)
{
    if(TotalBills >= MAX_BILLS) return -1;

    new idx = TotalBills;
    new id;
    do { id = random(999999) + 1; } while(GetBillByID(id) != -1);

    BillInfo[idx][billID]          = id;
    BillInfo[idx][billSQLPlayerID] = PlayerInfo[playerid][pId];
    format(BillInfo[idx][billSeller],  MAX_PLAYER_NAME, "%s", GetPlayerNameEx(playerid));
    format(BillInfo[idx][billBuyer],   MAX_PLAYER_NAME, "%s", GetPlayerNameEx(targetid));
    format(BillInfo[idx][billContent], 128, "%s", content);
    BillInfo[idx][billAmount]      = amount;
    BillInfo[idx][billStatus]      = 0;
    BillInfo[idx][billDBID]        = 0;

    new year, month, day, hour, minute;
    getdate(year, month, day);
    gettime(hour, minute);
    format(BillInfo[idx][billDate], 32, "%02d/%02d/%d %02d:%02d", day, month, year, hour, minute);

    TotalBills++;
    return idx;
}

// ============================================================
// Show dialogs
// ============================================================

stock ShowAllBills(playerid)
{
    if(TotalBills == 0)
    {
        SendClientMessage(playerid, COLOR_RED, "Khong co hoa don nao trong he thong!");
        return 0;
    }

    new string[8192], count;
    strcat(string, "ID\tNgay\tNoi dung\tTrang thai\n", sizeof(string));

    for(new i = 0; i < TotalBills; i++)
    {
        if(BillInfo[i][billDBID] == 0) continue;
        if(strlen(string) > 7800) break;

        new szStatus[32];
        if(BillInfo[i][billStatus] == 1)      strcat(szStatus, "{00FF00}Da TT",   sizeof(szStatus));
        else if(BillInfo[i][billStatus] == 2) strcat(szStatus, "{FF9900}Tu choi", sizeof(szStatus));
        else                                   strcat(szStatus, "{FF0000}Chua TT", sizeof(szStatus));

        new row[256];
        format(row, sizeof(row), "%06d\t%s\t%s->%s $%s\t%s\n",
            BillInfo[i][billID], BillInfo[i][billDate],
            BillInfo[i][billSeller], BillInfo[i][billBuyer],
            number_format(BillInfo[i][billAmount]), szStatus);
        strcat(string, row, sizeof(string));
        count++;
    }

    if(count == 0)
    {
        SendClientMessage(playerid, COLOR_RED, "Khong co hoa don nao!");
        return 0;
    }

    ShowPlayerDialog(playerid, DIALOG_BILL_ALL, DIALOG_STYLE_TABLIST_HEADERS,
        "Tat ca hoa don (Admin)", string, "Chi tiet", "Dong");
    return 1;
}

stock ShowPlayerAllBills(playerid)
{
    new string[4096], count;
    strcat(string, "ID\tNgay\tNguoi giao dich\tTrang thai\n", sizeof(string));

    new pName[MAX_PLAYER_NAME];
    format(pName, sizeof(pName), "%s", GetPlayerNameEx(playerid));

    for(new i = 0; i < TotalBills; i++)
    {
        if(BillInfo[i][billDBID] == 0) continue;
        if(strlen(string) > 3800) break;
        if(strcmp(BillInfo[i][billSeller], pName, true) != 0 &&
           strcmp(BillInfo[i][billBuyer],  pName, true) != 0) continue;

        new szStatus[32], szTrader[MAX_PLAYER_NAME];

        if(!strcmp(BillInfo[i][billSeller], pName, true))
        {
            format(szTrader, sizeof(szTrader), "%s", BillInfo[i][billBuyer]);
            if(BillInfo[i][billStatus] == 1)      strcat(szStatus, "{00FF00}Da TT",   sizeof(szStatus));
            else if(BillInfo[i][billStatus] == 2) strcat(szStatus, "{FF9900}Tu choi", sizeof(szStatus));
            else                                   strcat(szStatus, "{FF0000}Chua TT", sizeof(szStatus));
        }
        else
        {
            format(szTrader, sizeof(szTrader), "%s", BillInfo[i][billSeller]);
            if(BillInfo[i][billStatus] == 1)      strcat(szStatus, "{00FF00}Da TT",   sizeof(szStatus));
            else if(BillInfo[i][billStatus] == 2) strcat(szStatus, "{FF9900}Tu choi", sizeof(szStatus));
            else                                   strcat(szStatus, "{FF0000}Can TT",  sizeof(szStatus));
        }

        new row[256];
        format(row, sizeof(row), "%06d\t%s\t%s\t%s\n",
            BillInfo[i][billID], BillInfo[i][billDate], szTrader, szStatus);
        strcat(string, row, sizeof(string));
        count++;
    }

    if(count == 0)
    {
        SendClientMessage(playerid, COLOR_RED, "Ban khong co hoa don nao!");
        return 0;
    }

    ShowPlayerDialog(playerid, DIALOG_BILL_LIST, DIALOG_STYLE_TABLIST_HEADERS,
        "Tat ca hoa don cua ban", string, "Chi tiet", "Dong");
    return 1;
}

stock ShowPlayerBills(playerid, bool:created)
{
    new string[2048], count;
    if(created) strcat(string, "ID\tNgay\tSo tien\tTrang thai\tNguoi mua\n", sizeof(string));
    else        strcat(string, "ID\tNgay\tSo tien\tTrang thai\tNguoi ban\n", sizeof(string));

    new pName[MAX_PLAYER_NAME];
    format(pName, sizeof(pName), "%s", GetPlayerNameEx(playerid));

    for(new i = 0; i < TotalBills; i++)
    {
        if(strlen(string) > 1800) break;
        new row[256];

        if(created)
        {
            if(strcmp(BillInfo[i][billSeller], pName, true) != 0) continue;

            new szStatus[32];
            if(BillInfo[i][billStatus] == 1)      strcat(szStatus, "{00FF00}Da TT",   sizeof(szStatus));
            else if(BillInfo[i][billStatus] == 2) strcat(szStatus, "{FF9900}Tu choi", sizeof(szStatus));
            else                                   strcat(szStatus, "{FF0000}Chua TT", sizeof(szStatus));

            format(row, sizeof(row), "%06d\t%s\t$%s\t%s\t%s\n",
                BillInfo[i][billID], BillInfo[i][billDate],
                number_format(BillInfo[i][billAmount]), szStatus, BillInfo[i][billBuyer]);
        }
        else
        {
            if(strcmp(BillInfo[i][billBuyer], pName, true) != 0) continue;
            if(BillInfo[i][billStatus] != 0) continue;

            format(row, sizeof(row), "%06d\t%s\t$%s\t{FF0000}Can TT\t%s\n",
                BillInfo[i][billID], BillInfo[i][billDate],
                number_format(BillInfo[i][billAmount]), BillInfo[i][billSeller]);
        }

        strcat(string, row, sizeof(string));
        count++;
    }

    if(count == 0)
    {
        if(created) SendClientMessage(playerid, COLOR_RED, "Ban chua tao hoa don nao!");
        else        SendClientMessage(playerid, COLOR_RED, "Ban khong co hoa don can thanh toan!");
        return 0;
    }

    new title[64];
    if(created) strcat(title, "Hoa don da tao", sizeof(title));
    else        strcat(title, "Hoa don can thanh toan", sizeof(title));

    ShowPlayerDialog(playerid, DIALOG_BILL_LIST, DIALOG_STYLE_TABLIST_HEADERS,
        title, string, "Chi tiet", "Dong");
    return 1;
}

// ============================================================
// Commands
// ============================================================

CMD:hoadon(playerid, params[]) { return pc_cmd_bill(playerid, ""); }

CMD:bill(playerid, params[]) { 
    if(PlayerInfo[playerid][pAdmin] >= 1337)
    {
        ShowPlayerDialog(playerid, DIALOG_BILL_MENU, DIALOG_STYLE_LIST, "Menu Hoa Don",
            "Tao hoa don moi\nXem hoa don da tao\nXem hoa don can thanh toan\nXem tat ca hoa don cua ban\nXem tat ca hoa don (Admin)",
            "Chon", "Dong");
    }
    else
    {
        ShowPlayerDialog(playerid, DIALOG_BILL_MENU, DIALOG_STYLE_LIST, "Menu Hoa Don",
            "Tao hoa don moi\nXem hoa don da tao\nXem hoa don can thanh toan\nXem tat ca hoa don cua ban",
            "Chon", "Dong");
    }
    return 1;
 }

// ============================================================
// Dialog handler
// ============================================================

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_BILL_MENU)
    {
        if(!response) return 1;
        switch(listitem)
        {
            case 0:
            {
                if(PlayerInfo[playerid][pPaper] <= 0)
                {
                    SendClientMessage(playerid, COLOR_RED, "Ban khong co giay de viet hoa don!");
                    HienTextdraw(playerid, "Ban khong co giay nen khong the viet hoa don.");
                    return 1;
                }
                ShowPlayerDialog(playerid, DIALOG_BILL_BUYER, DIALOG_STYLE_INPUT,
                    "Tao Hoa Don - Nguoi Mua", "Nhap ID nguoi mua:", "Tiep", "Quay lai");
            }
            case 1: ShowPlayerBills(playerid, true);
            case 2: ShowPlayerBills(playerid, false);
            case 3: ShowPlayerAllBills(playerid);
            case 4:
            {
                if(PlayerInfo[playerid][pAdmin] < 1337)
                {
                    SendClientMessage(playerid, COLOR_RED, "Ban khong co quyen!");
                    return 1;
                }
                ShowAllBills(playerid);
            }
        }
        return 1;
    }

    if(dialogid == DIALOG_BILL_BUYER)
    {
        if(!response) { pc_cmd_bill(playerid, ""); return 1; }

        new targetid = strval(inputtext);
        if(!IsPlayerConnected(targetid))
        {
            SendClientMessage(playerid, COLOR_RED, "Nguoi choi khong ton tai hoac da offline!");
            ShowPlayerDialog(playerid, DIALOG_BILL_BUYER, DIALOG_STYLE_INPUT,
                "Tao Hoa Don - Nguoi Mua", "Nhap ID nguoi mua:", "Tiep", "Quay lai");
            return 1;
        }
        if(targetid == playerid)
        {
            SendClientMessage(playerid, COLOR_RED, "Ban khong the tao hoa don cho chinh minh!");
            ShowPlayerDialog(playerid, DIALOG_BILL_BUYER, DIALOG_STYLE_INPUT,
                "Tao Hoa Don - Nguoi Mua", "Nhap ID nguoi mua:", "Tiep", "Quay lai");
            return 1;
        }
        SetPVarInt(playerid, "Bill_TargetID", targetid);
        ShowPlayerDialog(playerid, DIALOG_BILL_CONTENT, DIALOG_STYLE_INPUT,
            "Tao Hoa Don - Noi Dung", "Nhap noi dung hoa don:", "Tiep", "Quay lai");
        return 1;
    }

    if(dialogid == DIALOG_BILL_CONTENT)
    {
        if(!response)
        {
            ShowPlayerDialog(playerid, DIALOG_BILL_BUYER, DIALOG_STYLE_INPUT,
                "Tao Hoa Don - Nguoi Mua", "Nhap ID nguoi mua:", "Tiep", "Quay lai");
            return 1;
        }
        if(strlen(inputtext) < 1 || strlen(inputtext) > 128)
        {
            SendClientMessage(playerid, COLOR_RED, "Noi dung khong hop le (1-128 ky tu)!");
            ShowPlayerDialog(playerid, DIALOG_BILL_CONTENT, DIALOG_STYLE_INPUT,
                "Tao Hoa Don - Noi Dung", "Nhap noi dung hoa don:", "Tiep", "Quay lai");
            return 1;
        }
        SetPVarString(playerid, "Bill_Content", inputtext);
        ShowPlayerDialog(playerid, DIALOG_BILL_AMOUNT, DIALOG_STYLE_INPUT,
            "Tao Hoa Don - So Tien", "Nhap so tien:", "Tiep", "Quay lai");
        return 1;
    }

    if(dialogid == DIALOG_BILL_AMOUNT)
    {
        if(!response)
        {
            ShowPlayerDialog(playerid, DIALOG_BILL_CONTENT, DIALOG_STYLE_INPUT,
                "Tao Hoa Don - Noi Dung", "Nhap noi dung hoa don:", "Tiep", "Quay lai");
            return 1;
        }
        if(PlayerInfo[playerid][pPaper] <= 0)
        {
            SendClientMessage(playerid, COLOR_RED, "Ban khong co giay de viet hoa don!");
            HienTextdraw(playerid, "Ban khong co giay nen khong the viet hoa don.");
            pc_cmd_bill(playerid, "");
            return 1;
        }
        new amount = strval(inputtext);
        if(amount <= 0 || amount > 999999999)
        {
            SendClientMessage(playerid, COLOR_RED, "So tien khong hop le (1 - 999,999,999)!");
            ShowPlayerDialog(playerid, DIALOG_BILL_AMOUNT, DIALOG_STYLE_INPUT,
                "Tao Hoa Don - So Tien", "Nhap so tien:", "Tiep", "Quay lai");
            return 1;
        }

        new targetid = GetPVarInt(playerid, "Bill_TargetID");
        new content[128];
        GetPVarString(playerid, "Bill_Content", content, sizeof(content));

        new billid = CreateNewBill(playerid, targetid, content, amount);
        if(billid == -1)
        {
            SendClientMessage(playerid, COLOR_RED, "Khong the tao hoa don (dat gioi han)!");
            return 1;
        }

        Bill_Save(billid);
        DeletePVar(playerid, "Bill_TargetID");
        DeletePVar(playerid, "Bill_Content");
        PlayerInfo[playerid][pPaper]--;

        new string[128];
        format(string, sizeof(string), "Ban da tao hoa don #%06d cho %s voi so tien $%s.",
            BillInfo[billid][billID], GetPlayerNameEx(targetid), number_format(amount));
        SendClientMessage(playerid, COLOR_YELLOW, string);

        if(IsPlayerConnected(targetid))
        {
            format(string, sizeof(string), "%s da tao hoa don #%06d cho ban voi so tien $%s.",
                GetPlayerNameEx(playerid), BillInfo[billid][billID], number_format(amount));
            SendClientMessage(targetid, COLOR_YELLOW, string);
        }
        return 1;
    }

    if(dialogid == DIALOG_BILL_LIST)
    {
        if(!response) { pc_cmd_bill(playerid, ""); return 1; }

        new billid = strval(inputtext);
        new bill_idx = GetBillByID(billid);
        if(bill_idx == -1)
        {
            SendClientMessage(playerid, COLOR_RED, "Hoa don khong ton tai!");
            return 1;
        }

        SetPVarInt(playerid, "Selected_Bill", bill_idx);

        new pName[MAX_PLAYER_NAME];
        format(pName, sizeof(pName), "%s", GetPlayerNameEx(playerid));

        new string[512], szStatus[48];
        if(BillInfo[bill_idx][billStatus] == 1)      format(szStatus, sizeof(szStatus), "{00FF00}Da thanh toan");
        else if(BillInfo[bill_idx][billStatus] == 2) format(szStatus, sizeof(szStatus), "{FF9900}Da tu choi");
        else                                          format(szStatus, sizeof(szStatus), "{FF0000}Chua thanh toan");

        format(string, sizeof(string),
            "{FFFFFF}ID: #%06d\nNguoi ban: %s\nNguoi mua: %s\nNoi dung: %s\nSo tien: $%s\nNgay: %s\nTrang thai: %s",
            BillInfo[bill_idx][billID],
            BillInfo[bill_idx][billSeller],
            BillInfo[bill_idx][billBuyer],
            BillInfo[bill_idx][billContent],
            number_format(BillInfo[bill_idx][billAmount]),
            BillInfo[bill_idx][billDate],
            szStatus);

        if(!strcmp(BillInfo[bill_idx][billBuyer], pName, true) && BillInfo[bill_idx][billStatus] == 0)
        {
            ShowPlayerDialog(playerid, DIALOG_BILL_OPTIONS, DIALOG_STYLE_MSGBOX,
                "Chi tiet hoa don", string, "Lua chon", "Quay lai");
        }
        else
        {
            ShowPlayerDialog(playerid, DIALOG_BILL_DETAIL, DIALOG_STYLE_MSGBOX,
                "Chi tiet hoa don", string, "Dong", "Quay lai");
        }
        return 1;
    }

    if(dialogid == DIALOG_BILL_ALL)
    {
        if(!response) { pc_cmd_bill(playerid, ""); return 1; }

        new billid = strval(inputtext);
        new bill_idx = GetBillByID(billid);
        if(bill_idx == -1)
        {
            SendClientMessage(playerid, COLOR_RED, "Hoa don khong ton tai!");
            return 1;
        }

        new string[512], szStatus[48];
        if(BillInfo[bill_idx][billStatus] == 1)      format(szStatus, sizeof(szStatus), "{00FF00}Da thanh toan");
        else if(BillInfo[bill_idx][billStatus] == 2) format(szStatus, sizeof(szStatus), "{FF9900}Da tu choi");
        else                                          format(szStatus, sizeof(szStatus), "{FF0000}Chua thanh toan");

        format(string, sizeof(string),
            "{FFFFFF}ID: #%06d\nNguoi ban: %s\nNguoi mua: %s\nNoi dung: %s\nSo tien: $%s\nNgay: %s\nTrang thai: %s",
            BillInfo[bill_idx][billID],
            BillInfo[bill_idx][billSeller],
            BillInfo[bill_idx][billBuyer],
            BillInfo[bill_idx][billContent],
            number_format(BillInfo[bill_idx][billAmount]),
            BillInfo[bill_idx][billDate],
            szStatus);

        ShowPlayerDialog(playerid, DIALOG_BILL_DETAIL, DIALOG_STYLE_MSGBOX,
            "Chi tiet hoa don", string, "Dong", "Quay lai");
        return 1;
    }

    if(dialogid == DIALOG_BILL_DETAIL)
    {
        if(!response) ShowPlayerAllBills(playerid);
        return 1;
    }

    if(dialogid == DIALOG_BILL_OPTIONS)
    {
        if(!response) { ShowPlayerBills(playerid, false); return 1; }
        ShowPlayerDialog(playerid, DIALOG_BILL_PAY_OPTIONS, DIALOG_STYLE_LIST,
            "Lua chon", "Thanh toan hoa don\nTu choi hoa don", "Chon", "Quay lai");
        return 1;
    }

    if(dialogid == DIALOG_BILL_PAY_OPTIONS)
    {
        new bill_idx = GetPVarInt(playerid, "Selected_Bill");

        if(!response)
        {
            new string[512];
            format(string, sizeof(string),
                "{FFFFFF}ID: #%06d\nNguoi ban: %s\nNguoi mua: %s\nNoi dung: %s\nSo tien: $%s\nNgay: %s\nTrang thai: {FF0000}Chua thanh toan",
                BillInfo[bill_idx][billID],
                BillInfo[bill_idx][billSeller],
                BillInfo[bill_idx][billBuyer],
                BillInfo[bill_idx][billContent],
                number_format(BillInfo[bill_idx][billAmount]),
                BillInfo[bill_idx][billDate]);
            ShowPlayerDialog(playerid, DIALOG_BILL_OPTIONS, DIALOG_STYLE_MSGBOX,
                "Chi tiet hoa don", string, "Lua chon", "Quay lai");
            return 1;
        }

        new string[512];
        if(listitem == 0)
        {
            format(string, sizeof(string),
                "{FFFFFF}ID: #%06d\nNguoi ban: %s\nNguoi mua: %s\nNoi dung: %s\nSo tien: $%s\nNgay: %s\nTrang thai: {FF0000}Chua thanh toan\n\n{ffffff}Ban co chac muon {00FF00}THANH TOAN{FFFFFF}?",
                BillInfo[bill_idx][billID],
                BillInfo[bill_idx][billSeller],
                BillInfo[bill_idx][billBuyer],
                BillInfo[bill_idx][billContent],
                number_format(BillInfo[bill_idx][billAmount]),
                BillInfo[bill_idx][billDate]);
            ShowPlayerDialog(playerid, DIALOG_BILL_PAY, DIALOG_STYLE_MSGBOX,
                "Xac nhan thanh toan", string, "Thanh toan", "Quay lai");
        }
        else
        {
            format(string, sizeof(string),
                "{FFFFFF}ID: #%06d\nNguoi ban: %s\nNguoi mua: %s\nNoi dung: %s\nSo tien: $%s\nNgay: %s\nTrang thai: {FF0000}Chua thanh toan\n\n{ffffff}Ban co chac muon {FF0000}TU CHOI{FFFFFF}?",
                BillInfo[bill_idx][billID],
                BillInfo[bill_idx][billSeller],
                BillInfo[bill_idx][billBuyer],
                BillInfo[bill_idx][billContent],
                number_format(BillInfo[bill_idx][billAmount]),
                BillInfo[bill_idx][billDate]);
            ShowPlayerDialog(playerid, DIALOG_BILL_DECLINE, DIALOG_STYLE_MSGBOX,
                "Xac nhan tu choi", string, "Tu choi", "Quay lai");
        }
        return 1;
    }

    if(dialogid == DIALOG_BILL_DECLINE)
    {
        if(!response)
        {
            ShowPlayerDialog(playerid, DIALOG_BILL_PAY_OPTIONS, DIALOG_STYLE_LIST,
                "Lua chon", "Thanh toan hoa don\nTu choi hoa don", "Chon", "Quay lai");
            return 1;
        }

        new bill_idx = GetPVarInt(playerid, "Selected_Bill");
        BillInfo[bill_idx][billStatus] = 2;
        Bill_Save(bill_idx);

        new string[128];
        format(string, sizeof(string), "Ban da tu choi hoa don #%06d.", BillInfo[bill_idx][billID]);
        SendClientMessage(playerid, COLOR_YELLOW, string);

        for(new i = 0; i < MAX_PLAYERS; i++)
        {
            if(!IsPlayerConnected(i)) continue;
            if(strcmp(GetPlayerNameEx(i), BillInfo[bill_idx][billSeller], true) != 0) continue;
            format(string, sizeof(string), "%s da tu choi thanh toan hoa don #%06d.",
                GetPlayerNameEx(playerid), BillInfo[bill_idx][billID]);
            SendClientMessage(i, COLOR_YELLOW, string);
            break;
        }
        ShowPlayerBills(playerid, false);
        return 1;
    }

    if(dialogid == DIALOG_BILL_PAY)
    {
        if(!response)
        {
            ShowPlayerDialog(playerid, DIALOG_BILL_PAY_OPTIONS, DIALOG_STYLE_LIST,
                "Lua chon", "Thanh toan hoa don\nTu choi hoa don", "Chon", "Quay lai");
            return 1;
        }

        new bill_idx = GetPVarInt(playerid, "Selected_Bill");

        if(BillInfo[bill_idx][billStatus] != 0)
        {
            SendClientMessage(playerid, COLOR_RED, "Hoa don nay da duoc xu ly!");
            return 1;
        }
        if(BillInfo[bill_idx][billAmount] <= 0 || BillInfo[bill_idx][billAmount] > 999999999)
        {
            SendClientMessage(playerid, COLOR_RED, "Loi: so tien khong hop le!");
            return 1;
        }
        if(PlayerInfo[playerid][pCash] < BillInfo[bill_idx][billAmount])
        {
            SendClientMessage(playerid, COLOR_RED, "Ban khong du tien de thanh toan!");
            return 1;
        }

        new sellerID = INVALID_PLAYER_ID;
        for(new i = 0; i < MAX_PLAYERS; i++)
        {
            if(!IsPlayerConnected(i)) continue;
            if(!strcmp(GetPlayerNameEx(i), BillInfo[bill_idx][billSeller], true))
            {
                sellerID = i;
                break;
            }
        }

        if(sellerID == INVALID_PLAYER_ID)
        {
            SendClientMessage(playerid, COLOR_RED, "Nguoi ban chua online, khong the thanh toan!");
            return 1;
        }

        PlayerInfo[playerid][pCash]   -= BillInfo[bill_idx][billAmount];
        PlayerInfo[sellerID][pCash]   += BillInfo[bill_idx][billAmount];
        BillInfo[bill_idx][billStatus] = 1;
        Bill_Save(bill_idx);

        new string[128];
        format(string, sizeof(string), "Ban da thanh toan hoa don #%06d voi so tien $%s.",
            BillInfo[bill_idx][billID], number_format(BillInfo[bill_idx][billAmount]));
        SendClientMessage(playerid, COLOR_GREEN, string);

        format(string, sizeof(string), "%s da thanh toan hoa don #%06d voi so tien $%s.",
            GetPlayerNameEx(playerid), BillInfo[bill_idx][billID],
            number_format(BillInfo[bill_idx][billAmount]));
        SendClientMessage(sellerID, COLOR_GREEN, string);
        return 1;
    }

    return 1;
}
