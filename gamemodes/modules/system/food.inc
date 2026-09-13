#include <YSI\YSI_Coding\y_hooks>

#define DIALOG_BUYFOOD              (9150)

// Dialog nhập số lượng cho từng món
#define DIALOG_BUYBANHMI            (9300)
#define DIALOG_BUYNUOCLOC           (9301)
#define DIALOG_BUYPIZZA             (9302)
#define DIALOG_BUYBUNRIEU           (9303)
#define DIALOG_BUYPHOBO             (9304)
#define DIALOG_BUYHAMBURGER         (9305)
#define DIALOG_BUYCAPHE             (9306)
#define DIALOG_BUYSUABO             (9307)
#define DIALOG_BUYTHITHUNKHOI       (9308)
#define DIALOG_BUYTRAICAY           (9309)
#define DIALOG_BUYNUOCEP            (9310)

#define MAX_FOOD_AMOUNT             (99999)
#define MIN_FOOD_AMOUNT             (1)

enum ItemFood_Data {
    NameFood[128],
    PriceFood,
    DialogID,
    InventoryVariable   // vị trí enum trong PlayerInfo (pBanhMi, pNuocLoc, ...)
}

new const ItemFood[][ItemFood_Data] = {
    {"Banh mi",       2000, DIALOG_BUYBANHMI,      pBanhMi},
    {"Nuoc\t",        2000, DIALOG_BUYNUOCLOC,     pNuocLoc},
    {"Pizza\t",       7000, DIALOG_BUYPIZZA,       pPizza},
    {"Bun rieu",      3000, DIALOG_BUYBUNRIEU,     pBunRieu},
    {"Pho bo",        4000, DIALOG_BUYPHOBO,       pPhoBo},
    {"Hamburger",     3000, DIALOG_BUYHAMBURGER,   pHamburger},
    {"Ca phe",        2500, DIALOG_BUYCAPHE,       pCaPhe},
    {"Sua bo",        3000, DIALOG_BUYSUABO,       pSuaBo},
    {"Thit hun khoi", 4000, DIALOG_BUYTHITHUNKHOI, pThit},
    {"Trai cay",      1500, DIALOG_BUYTRAICAY,     pTraiCay},
    {"Nuoc ep",       2500, DIALOG_BUYNUOCEP,      pNuocEp}
};

stock ShowBuyFood(playerid) {
    new string[2048];
    for(new i = 0; i < sizeof(ItemFood); i++) {
        format(string, sizeof(string), "%s\n{00ffff}>{ffffff} %s\t\t%s$", 
            string, 
            ItemFood[i][NameFood],
            FormatMoney(ItemFood[i][PriceFood])
        );
    }
    ShowPlayerDialog(playerid, DIALOG_BUYFOOD, DIALOG_STYLE_LIST, 
        "Ban muon an gi?", string, "Mua", "Huy");
    return 1;
}

CMD:muathucan(playerid, params[])
{
    if (!IsAtRestaurant(playerid))
    {
        SendClientMessageEx(playerid, COLOR_GRAD2, " Ban khong o trong nha hang!");
        return 0;
    }
    ShowBuyFood(playerid);
    return 1;
}

CMD:adminta(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1) return 1;
    ShowBuyFood(playerid);
    return 1;
}

/* hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(newkeys & KEY_YES)
    {
        if (IsAtRestaurant(playerid))
            return cmd_muathucan(playerid, "");
    }
    return 1;
} */

stock ProcessFoodPurchase(playerid, itemIndex, amount)
{
    // Kiểm tra số lượng hợp lệ
    if(amount < MIN_FOOD_AMOUNT || amount > MAX_FOOD_AMOUNT)
    {
        SendClientMessage(playerid, -1, "Loi, hay nhap lai so luong");
        return 0;
    }

    new totalCost = amount * ItemFood[itemIndex][PriceFood];

    // Kiểm tra tiền
    if(PlayerInfo[playerid][pCash] < totalCost)
    {
        new moneyNeeded = totalCost - PlayerInfo[playerid][pCash];
        new str[256];
        format(str, sizeof(str), "{FFFFFF}Ban con thieu {10ff94}%d {FFFFFF}Money de mua {10ff94}%d {FFFFFF}%s.",
            moneyNeeded, amount, ItemFood[itemIndex][NameFood]);
        SendClientMessage(playerid, -1, str);
        return 0;
    }

    // Cộng vào biến tồn kho tương ứng (pBanhMi, pNuocLoc, ...)
    new variable = ItemFood[itemIndex][InventoryVariable];
    PlayerInfo[playerid][variable] += amount;

    // Trừ tiền
    PlayerInfo[playerid][pCash] -= totalCost;

    // Thông báo
    new str[128];
    format(str, sizeof(str), "{00ff00}+%d {ffffff}%s", amount, ItemFood[itemIndex][NameFood]);
    SendClientMessageEx(playerid, -1, str);

    format(str, sizeof(str), "{ff0000}-%s {ffffff}Money", FormatMoney(totalCost));
    SendClientMessageEx(playerid, -1, str);

    format(str, sizeof(str), "Ban da mua %d %s", amount, ItemFood[itemIndex][NameFood]);
    ShowTDN(playerid, NOTIFICATION_SUCCESS, str);

    PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);

    // Cảnh báo khai thác
    if(amount > 9999999)
    {
        new str2[128], name[MAX_PLAYER_NAME];
        GetPlayerName(playerid, name, sizeof(name));
        format(str2, sizeof(str2), "Nguoi choi %s dang co the dang bug mua %s voi so luong %d",
            name, ItemFood[itemIndex][NameFood], amount);
        ABroadCast(COLOR_YELLOW, str2, 2);
    }

    return 1;
}

// Xử lý dialog mua đồ ăn
hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    // Bước 1: chọn món trong danh sách -> mở dialog nhập số lượng
    if(dialogid == DIALOG_BUYFOOD && response)
    {
        if(listitem < 0 || listitem >= sizeof(ItemFood)) return 1;

        new name[128];
        format(name, sizeof(name), "Ban muon mua bao nhieu %s?", ItemFood[listitem][NameFood]);
        ShowPlayerDialog(playerid, ItemFood[listitem][DialogID], DIALOG_STYLE_INPUT,
            name, "Nhap so luong muon mua vao ben duoi:", "Mua", "Huy Bo");
        return 1;
    }

    // Bước 2: nhập số lượng -> xử lý mua
    for(new i = 0; i < sizeof(ItemFood); i++)
    {
        if(dialogid == ItemFood[i][DialogID] && response)
        {
            // Chặn ký tự lạ
            if(strfind(inputtext, "%", true) != -1 || strfind(inputtext, "c", true) != -1)
            {
                return pc_cmd_muathucan(playerid, "");
            }

            new amount = strval(inputtext);
            ProcessFoodPurchase(playerid, i, amount);
            return 1;
        }
    }
    return 1;
}