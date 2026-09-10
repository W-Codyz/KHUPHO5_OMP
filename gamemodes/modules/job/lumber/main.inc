#include <YSI\YSI_Coding\y_hooks>
#define DIALOG_MENU_LUMBER (9005)
#define DIALOG_MENU_WOODSELL (9006)
#define DIALOG_MENU_WOODSELL1 (9007)
#define DIALOG_MENU_WOODSELL2 (9008)
#define DIALOG_MENU_WOODSELL3 (9009)
#define DIALOG_MENU_LUMBERMEM (9015)

const WoodSell1 = 1500;
const WoodSell2 = 2500;
const WoodSell3 = 4000;
#define GOTHO 30
new ChatCayTime[MAX_PLAYERS];
new XuLyTime[MAX_PLAYERS];
new lXuLyTimer[MAX_PLAYERS];
new lChatCayTimer[MAX_PLAYERS];
new DangCamKhucGo[MAX_PLAYERS];
new Float:chatcaypos_pos[GOTHO][] = {
    {-586.5910, -67.6849, 65.1055},
    {-598.7264, -62.6084, 63.5321},
    {-595.4039, -57.6008, 64.0453},
    {-588.1226, -57.7902, 64.6371},
    {-602.3700, -52.8173, 63.5130},
    {-610.7908, -52.6962, 62.8075},
    {-616.3765, -60.2545, 62.9431},
    {-626.4560, -68.5381, 64.5750},
    {-623.5648, -76.1417, 64.6829},
    {-616.7187, -83.3847, 64.2409},
    {-608.1622, -86.6179, 64.0397},
    {-603.2106, -94.6078, 64.6535},
    {-611.3782, -97.4317, 64.8887},
    {-620.4033, -91.6125, 65.1022},
    {-588.1523, -90.0992, 64.9138},
    {-579.3025, -77.6503, 64.8380},
    {-579.9860, -70.0960, 64.8515},
    {-580.4525, -61.3451, 64.9347},
    {-581.1863, -55.4273, 65.0097},
    {-580.3475, -44.2026, 65.0613},
    {-569.9053, -42.1805, 64.6419},
    {-565.5383, -36.9702, 64.4907},
    {-565.5189, -28.6861, 63.9583},
    {-557.3141, -26.9681, 63.6383},
    {-554.7582, -14.4269, 63.1836},
    {-543.4727, -18.1107, 62.9540},
    {-533.2223, -24.5771, 61.2050},
    {-523.7198, -35.2411, 61.4957},
    {-521.1165, -51.2911, 61.9243},
    {-535.5420, -51.0299, 62.8781}
};

randomtree()
{
	new 
		Tree_Object_ID[] = {708,691,616};
    new modelid = Tree_Object_ID[random(sizeof Tree_Object_ID)];
    return modelid;
}
enum WoodInfo {
    wObject,
    wTextLabel,
    wStatus
}
new WoodLumber[GOTHO][WoodInfo];
hook OnPlayerDisconnect(playerid, reason)
{
    if (lXuLyTimer[playerid])
        KillTimer(lXuLyTimer[playerid]);
    if (lChatCayTimer[playerid])
        KillTimer(lChatCayTimer[playerid]);
    return 1;
}
hook OnPlayerConnect(playerid)
{
    DangCamKhucGo[playerid] = 0;
    return 1;
}
hook OnGameModeInit()
{
    new string[64];
    CreateActor(22, -543.9225,-79.9980,62.9417, 180);
    CreateDynamic3DTextLabel("<Minh Man - Quan ly bai go>\nAn 'Y' de tuong tac", -1, -543.9225,-79.9980,62.9417+1.3,5.0);
    CreateActor(299, -911.7522,-490.6188,25.9609,319.7426);
    CreateDynamic3DTextLabel("<Tien Ngo - Thu mua go>\nAn 'Y' de tuong tac", -1, -911.7522,-490.6188,25.9609+1.3,5.0);
    CreateDynamicPickup(1274, 1, -547.7433,-181.8307,78.4063, 0);
    CreateDynamic3DTextLabel("<Xu ly go>\nAn 'Y' de xu ly go", -1, -547.7433,-181.8307,78.4063,10.0);

    for (new i = 0; i < GOTHO; i++)
    {
        WoodLumber[i][wStatus] = 0;
        WoodLumber[i][wObject] = CreateDynamicObject(randomtree(), chatcaypos_pos[i][0], chatcaypos_pos[i][1], chatcaypos_pos[i][2]-1, 0, 0, 0);
        format(string, sizeof(string), "Cay go #%d\n'Dam vao cay' hoac 'LMB' de chat cay", i);
        WoodLumber[i][wTextLabel] = CreateDynamic3DTextLabel(string, -1, chatcaypos_pos[i][0], chatcaypos_pos[i][1], chatcaypos_pos[i][2], 3.0, _, _, 0, 0, 0, _, 20.0);
    }
    return 1;
}
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    if(newkeys & KEY_YES) {
        if(IsPlayerInRangeOfPoint(playerid, 5.0, -543.9225,-79.9980,62.9417))
        {
            ShowPlayerDialog(playerid, DIALOG_MENU_LUMBER, DIALOG_STYLE_MSGBOX, "Quan ly bai go", "{ffffff}Ban co chac chan muon mua 1 cay riu voi gia 15.000$ khong ? \n{ff0000}Mot cay riu se co do ben la: 20{ffffff}", "Chon", "Dong");
            return 1;
        }
        if(IsPlayerInRangeOfPoint(playerid, 10.0, -547.7433,-181.8307,78.4063))
        {
            if (DangCamKhucGo[playerid] == 0)
            {
                SendClientMessage(playerid, COLOR_GREY, "Ban can phai chat mot khuc go sau do moi co the xu ly.");
                ShowTDN(playerid, NOTIFICATION_WARNING, "Khong co go de xu ly.");
                return 1;
            }
            if (PlayerInfo[playerid][pGoThuong]+PlayerInfo[playerid][pGoXin]+PlayerInfo[playerid][pGoHiem] >= 20)
            {
                SendClientMessage(playerid, COLOR_GREY, "Tui cua ban da day khong the xu ly them go, hay mang den CHECKPOINT de ban.");
                ShowTDN(playerid, NOTIFICATION_WARNING, "Tui cua ban da day.");
                DangCamKhucGo[playerid] = 0;
                ClearAnimations(playerid);
                SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
                RemovePlayerAttachedObject(playerid, 9);
                SetPlayerCheckpoint(playerid,  -911.7522,-490.6188,25.9609, 3.0);
                CP[playerid] = 252000;
                return 1;
            }
            if (XuLyTime[playerid] > 0)
                return 0;
            XuLyTime[playerid] = 20;
            lXuLyTimer[playerid] = SetTimerEx("XuLyTimer", 1000, 0, "i", playerid);
            return 1;
        }
        if(IsPlayerInRangeOfPoint(playerid, 5.0,  -911.7522,-490.6188,25.9609))
        {
            new str[128];
            format(str, sizeof(str), "{00ff00}$%d {ffffff}Go thuong: {ffff00}%d\n{00ff00}$%d {ffffff}Go xin: {ff00ff}%d\n{00ff00}$%d {ffffff}Go hiem: {ff0000}%d", WoodSell1, PlayerInfo[playerid][pGoThuong], WoodSell2, PlayerInfo[playerid][pGoXin], WoodSell3, PlayerInfo[playerid][pGoHiem]);
            ShowPlayerDialog(playerid, DIALOG_MENU_WOODSELL, DIALOG_STYLE_LIST, "Thu mua go", str, ">", "X");
            return 1;
        }
    }
    if(newkeys & KEY_FIRE) {
        for(new i = 0; i < GOTHO; i++)
        {
            if (IsPlayerInRangeOfPoint(playerid, 3.0, chatcaypos_pos[i][0], chatcaypos_pos[i][1], chatcaypos_pos[i][2]))
            {
                if (WoodLumber[i][wStatus] == 0)
                {
                    if (ChatCayTime[playerid])
                        return 0;
                    if (PlayerCuffed[playerid] >= 1 || PlayerInfo[playerid][pJailTime] > 0 || GetPVarInt(playerid, "Injured"))
                        return 0;
                    if (PlayerInfo[playerid][pAxe] <= 0)
                    {
                        SendClientMessage(playerid, COLOR_GREY, "Ban chua mua riu hoac riu cua ban da bi hong, hay mua riu moi.");
                        ShowTDN(playerid, NOTIFICATION_WARNING, "Ban chua mua riu hoac riu da bi hong.");
                        SetPlayerCheckpoint(playerid, -543.9225,-79.9980,62.9417, 3.0);
                        CP[playerid] = 252000;
                        return 1;
                    }
                    if (PlayerInfo[playerid][pGoThuong]+PlayerInfo[playerid][pGoXin]+PlayerInfo[playerid][pGoHiem] >= 20)
                    {
                        SendClientMessage(playerid, COLOR_GREY, "Tui cua ban da day khong the xu ly them go, hay mang den CHECKPOINT de ban.");
                        ShowTDN(playerid, NOTIFICATION_WARNING, "Tui cua ban da day.");
                        DangCamKhucGo[playerid] = 0;
                        ClearAnimations(playerid);
                        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
                        RemovePlayerAttachedObject(playerid, 9);
                        SetPlayerCheckpoint(playerid,  -911.7522,-490.6188,25.9609, 3.0);
                        CP[playerid] = 252000;
                        return 1;
                    }

                    if (DangCamKhucGo[playerid])
                    {
                        SendClientMessage(playerid, COLOR_GREY, "Hay mang khuc go den CHECKPOINT de xu ly go.");
                        ShowTDN(playerid, NOTIFICATION_INFO, "Hay mang khuc go di xu ly.");
                        SetPlayerCheckpoint(playerid,  -547.7433,-181.8307,78.4063, 3.0);
                        CP[playerid] = 252000;
                        return 1;
                    }
                    WoodLumber[i][wStatus] = 1;
                    lChatCayTimer[playerid] = SetTimerEx("ChatCayTimer", 1000, 0, "ii", playerid, i);
                    ChatCayTime[playerid] = 20;
                    return 1;
                }
            }
        }
    }
    return 0;
}
hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_MENU_LUMBERMEM)
    {
        if (response)
        {
            switch(listitem)
            {
                case 0:
                {
                    SetPlayerCheckpoint(playerid, -596.1323,-83.0602,64.0670, 3.0);
                    CP[playerid] = 252000;
                    SendClientMessage(playerid, COLOR_GREY, "Hay di đen CHECKPOINT de chat cay.");
                    ShowTDN(playerid, NOTIFICATION_INFO, "Hay di den CHECKPOINT de chat cay.");
                }
                case 1:
                {
                    SendClientMessage(playerid, COLOR_GREY, "Hay di đen CHECKPOINT de mua riu.");
                    ShowTDN(playerid, NOTIFICATION_INFO, "Hay di den CHECKPOINT de mua riu.");
                    SetPlayerCheckpoint(playerid, -543.9225,-79.9980,62.9417, 3.0);
                    CP[playerid] = 252000;
                }
                case 2:
                {
                    SetPlayerCheckpoint(playerid,  -911.7522,-490.6188,25.9609, 3.0);
                    CP[playerid] = 252000;
                    SendClientMessage(playerid, COLOR_GREY, "Hay di đen CHECKPOINT de ban go.");
                    ShowTDN(playerid, NOTIFICATION_INFO, "Hay di den CHECKPOINT de ban go.");
                }
            }
        }
        return 1;
    }
    if(dialogid == DIALOG_MENU_LUMBER)
    {
        if (response)
        {
            if (PlayerInfo[playerid][pAxe] > 0)
            {
                SendClientMessage(playerid, COLOR_GREY, "Ban da co 'riu chat cay' roi, hay xai den khi bi hong moi co the mua them.");
                ShowTDN(playerid, NOTIFICATION_WARNING, "Ban da co riu chat cay.");
                return 1;
            }
            if (PlayerInfo[playerid][pCash] < 15000)
            {
                SendClientMessage(playerid, COLOR_GREY, "Ban khong co du tien de mua 'riu chat cay'.");
                ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong co du tien.");
                return 1;
            }
            GivePlayerCash(playerid, -15000);
            SendClientMessage(playerid, COLOR_GREY, "Ban da mua thanh cong 'riu chat cay' voi 15.000$, do ben 20.");
            ShowTDN(playerid, NOTIFICATION_SUCCESS, "Ban da mua thanh cong.");
            PlayerInfo[playerid][pAxe] = 20;
            SetPlayerCheckpoint(playerid, -596.1323,-83.0602,64.0670, 3.0);
            CP[playerid] = 252000;
            SendClientMessage(playerid, COLOR_GREY, "Hay di đen CHECKPOINT de chat cay.");
            ShowTDN(playerid, NOTIFICATION_INFO, "Hay di den CHECKPOINT de chat cay.");
        }
        return 1;
    }
    if(dialogid == DIALOG_MENU_WOODSELL)
    {
        new str[128];
        if (response)
        {
            switch(listitem)
            {
                case 0:
                {
                    format(str, sizeof(str), "{ffffff}So luong ban co: {ff0000}%d{ffffff}\n > Nhap vao day so luong muon ban", PlayerInfo[playerid][pGoThuong]);
                    ShowPlayerDialog(playerid, DIALOG_MENU_WOODSELL1, DIALOG_STYLE_INPUT, "Go thuong", str, ">", "X");
                }
                case 1:
                {
                    format(str, sizeof(str), "{ffffff}So luong ban co: {ff0000}%d{ffffff}\n > Nhap vao day so luong muon ban", PlayerInfo[playerid][pGoXin]);
                    ShowPlayerDialog(playerid, DIALOG_MENU_WOODSELL2, DIALOG_STYLE_INPUT, "Go xin", str, ">", "X");
                }
                case 2:
                {
                    format(str, sizeof(str), "{ffffff}So luong ban co: {ff0000}%d{ffffff}\n > Nhap vao day so luong muon ban", PlayerInfo[playerid][pGoHiem]);
                    ShowPlayerDialog(playerid, DIALOG_MENU_WOODSELL3, DIALOG_STYLE_INPUT, "Go hiem", str, ">", "X");
                }
            }
        }
    }
    if(dialogid == DIALOG_MENU_WOODSELL1)
    {
        if (response)
        {
            new amount = strval(inputtext), str[128];
            if (amount <= 0 || amount > 100)
            {
                SendClientMessage(playerid, COLOR_GREY, "Vui long nhap so luong hop le!");
                ShowTDN(playerid, NOTIFICATION_WARNING, "Vui long nhap so luong hop le!");
                return 1;
            }
            if (amount > PlayerInfo[playerid][pGoThuong])
            {
                SendClientMessage(playerid, COLOR_GREY, "Ban khong co du so luong go de ban!");
                ShowTDN(playerid, NOTIFICATION_WARNING, "Ban khong co du so luong go de ban!");
                return 1;
            }
            format(str, sizeof(str), "Ban da ban %d go va nhan duoc $%d.", amount, amount*WoodSell1);
            SendClientMessage(playerid, -1, str);
            ShowTDN(playerid, NOTIFICATION_SUCCESS, str);
            PlayerInfo[playerid][pGoThuong] -= amount;
            GivePlayerCash(playerid, amount*WoodSell1);
        }
    }
    if(dialogid == DIALOG_MENU_WOODSELL2)
    {
        if (response)
        {
            new amount = strval(inputtext), str[128];
            if (amount <= 0 || amount > 100)
                return SendClientMessage(playerid, COLOR_GREY, "Vui long nhap so luong hop le!");
            if (amount > PlayerInfo[playerid][pGoXin])
                return SendClientMessage(playerid, COLOR_GREY, "Ban khong co du so luong go de ban!");
            format(str, sizeof(str), "Ban da ban %d go va nhan duoc $%d.", amount, amount*WoodSell2);
            PlayerInfo[playerid][pGoXin] -= amount;
            SendClientMessage(playerid, -1, str);
            GivePlayerCash(playerid, amount*WoodSell2);
        }
    }
    if(dialogid == DIALOG_MENU_WOODSELL3)
    {
        if (response)
        {
            new amount = strval(inputtext), str[128];
            if (amount <= 0 || amount > 100)
                return SendClientMessage(playerid, COLOR_GREY, "Vui long nhap so luong hop le!");
            if (amount > PlayerInfo[playerid][pGoHiem])
                return SendClientMessage(playerid, COLOR_GREY, "Ban khong co du so luong go de ban!");
            format(str, sizeof(str), "Ban da ban %d go va nhan duoc $%d.", amount, amount*WoodSell3);
            SendClientMessage(playerid, -1, str);
            GivePlayerCash(playerid, amount*WoodSell3);
            PlayerInfo[playerid][pGoHiem] -= amount;
        }
    }
    return 0;
}
forward ResetWood(index);
public ResetWood(index)
{
    new string[64];
    WoodLumber[index][wStatus] = 0;
    WoodLumber[index][wObject] = CreateDynamicObject(randomtree(), chatcaypos_pos[index][0], chatcaypos_pos[index][1], chatcaypos_pos[index][2]-1, 0, 0, 0);
    format(string, sizeof(string), "Cay go #%d\n'Dam vao cay' hoac 'LMB' de chat cay", index);
    WoodLumber[index][wTextLabel] = CreateDynamic3DTextLabel(string, -1, chatcaypos_pos[index][0], chatcaypos_pos[index][1], chatcaypos_pos[index][2], 3.0, _, _, 0, 0, 0, _, 20.0);
    printf("Cay go #%d respawned", index);
    return 1;
}
forward XuLyTimer(playerid);
public XuLyTimer(playerid)
{
    if (XuLyTime[playerid] > 0)
    {
        new str[64];
        ApplyAnimation(playerid, "BOMBER", "BOM_Plant_Loop", 4.1, 1, 0, 0, 1, 0);
        format(str, sizeof(str), "~w~Ban dang xu ly go~n~Vui long doi: ~r~%d", XuLyTime[playerid]);
        GameTextForPlayer(playerid, str, 1000, 5);
        lXuLyTimer[playerid] = SetTimerEx("XuLyTimer", 1000, 0, "i", playerid);
        XuLyTime[playerid]--;
    }
    else
    {
        new LoaiGo, str[128], gostr[32];
        switch(random(100))
        {
            case 0..70:
            {
                LoaiGo = 1;
                format(gostr, sizeof(gostr), "Go thuong");
                PlayerInfo[playerid][pGoThuong]++;
            }
            case 71..90:
            {
                LoaiGo = 2;
                format(gostr, sizeof(gostr), "Go xin");
                PlayerInfo[playerid][pGoXin]++;
            }
            case 91..100:
            {
                LoaiGo = 3;
                format(gostr, sizeof(gostr), "Go hiem");
                PlayerInfo[playerid][pGoHiem]++;
            }
        }
        DangCamKhucGo[playerid] = 0;
        ClearAnimations(playerid);
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
        RemovePlayerAttachedObject(playerid, 9);
        format(str, sizeof(str), "Ban da nhan duoc 1 %s.", gostr);
        SendClientMessage(playerid, -1, str);
        ClearAnimations(playerid);
    }
    return 1;
}
forward ChatCayTimer(playerid, index);
public ChatCayTimer(playerid, index)
{
    if(PlayerCuffed[playerid] >= 1 || PlayerInfo[playerid][pJailTime] > 0 || GetPVarInt(playerid, "Injured"))
        return 0;
    if (ChatCayTime[playerid] > 0)
    {
        new str[64];
        ChatCayTime[playerid]--;
        format(str, sizeof(str), "~w~Ban dang chat cay~n~Vui long doi: ~r~%d", ChatCayTime[playerid]);
        GameTextForPlayer(playerid, str, 1000, 5);
        lChatCayTimer[playerid] = SetTimerEx("ChatCayTimer", 1000, 0, "ii", playerid, index);
        ApplyAnimation(playerid, "SWORD", "SWORD_4", 4.1, 0, 1, 1, 1, 0, 1);
        SetPlayerAttachedObject(playerid, 9, 341, 6,0, 0.0,0.0,0.00, 0.00, 0.00,1, 1, 1);
    }
    else
    {
        new str[128], rand = Random(0, 2);
        SendClientMessage(playerid, COLOR_GREY, "Ban da chat cay thanh cong va nhan duoc 1 go tho.");
        format(str, sizeof(str), "Riu cua ban da bi giam %d do ben.", rand);
        SendClientMessage(playerid, COLOR_GREY, str);
        PlayerInfo[playerid][pAxe] -= rand;
        format(str, sizeof(str), "Do ben hien tai: %d/20", PlayerInfo[playerid][pAxe]);
        SendClientMessage(playerid, COLOR_GREY, str);
        if(IsValidDynamicObject(WoodLumber[index][wObject])) {
            DestroyDynamicObject(WoodLumber[index][wObject]);
        }
        if(IsValidDynamic3DTextLabel(WoodLumber[index][wTextLabel])) {
            DestroyDynamic3DTextLabel(WoodLumber[index][wTextLabel]);
        }
        RemovePlayerAttachedObject(playerid, 9);
        WoodLumber[index][wStatus] = 2;
        SetTimerEx("ResetWood", 600000, 0, "i", index);
        ClearAnimations(playerid);
        DangCamKhucGo[playerid] = 1;
        ApplyAnimation(playerid, "carry", "liftup", 1, 0, 1, 1, 0, 1000);
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
        SetPlayerAttachedObject(playerid, 9, 1463, 1,0.052999, 0.592999,0.0,0.00, 90.00, 0.00,1.45, 0.451, 0.66);
        SendClientMessage(playerid, COLOR_GREY, "Hay mang khuc go den CHECKPOINT de xu ly go.");
        SetPlayerCheckpoint(playerid,  -547.7433,-181.8307,78.4063, 3.0);
        CP[playerid] = 252000;
    }
    return 1;
}
CMD:chatgo(playerid, params[])
{
    new str[64];
    format(str, sizeof(str), "JOB CHAT GO | DO BEN RIU: %d", PlayerInfo[playerid][pAxe]);
    ShowPlayerDialog(playerid, DIALOG_MENU_LUMBERMEM, DIALOG_STYLE_LIST, str, "Chat go\nMua riu\nBan go", "Chon", "Dong");   
    return 1;
}