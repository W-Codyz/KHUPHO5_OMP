#include "YSI\YSI_Coding\y_hooks"

#define MAX_PAINT_SPOTS 3   // 3 điểm sơn cho mỗi nhà
#define MAX_HOUSES 12       // Số lượng nhà sơn tối đa (giả định bạn có 10 set data)

#define DIALOG_MENU_PAINTER (9016)

new tempskinpaint[MAX_PLAYERS];
new paintertimer[MAX_PLAYERS];
new paintertime[MAX_PLAYERS];
new Float: PaintInfo[MAX_HOUSES][MAX_PAINT_SPOTS][3];
new antitelepainter[MAX_PLAYERS];
new PlayerJobHouseID[MAX_PLAYERS];     // ID của ngôi nhà đang sơn (0-9)
new PlayerJobPaintSpot[MAX_PLAYERS];   // Checkpoint đang cần sơn (0-2)
new PlayerPaintSkin[MAX_PLAYERS];
hook OnGameModeInit()
{
    CreateActor(8, -2456.8425,786.6091,35.1719,92.0);
    CreateDynamic3DTextLabel("<Minh Sang - Tho son ky cuu>\nAn 'Y' de tuong tac", -1, -2456.8425,786.6091,35.1719+1.3,10.0);
    // house 1
    PaintInfo[0][0][0] = -2840.2241; PaintInfo[0][0][1] = 863.6917; PaintInfo[0][0][2] = 44.0547;
    PaintInfo[0][1][0] = -2847.4602; PaintInfo[0][1][1] = 863.7072; PaintInfo[0][1][2] = 44.0547;
    PaintInfo[0][2][0] = -2844.2129; PaintInfo[0][2][1] = 890.4627; PaintInfo[0][2][2] = 44.0547;

    // house 2
    PaintInfo[1][0][0] = -2857.9944; PaintInfo[1][0][1] = 839.6362; PaintInfo[1][0][2] = 39.6065;
    PaintInfo[1][1][0] = -2863.7339; PaintInfo[1][1][1] = 844.1420; PaintInfo[1][1][2] = 39.5078;
    PaintInfo[1][2][0] = -2874.7332; PaintInfo[1][2][1] = 818.1967; PaintInfo[1][2][2] = 39.5078;

    // house 3
    PaintInfo[2][0][0] = -2880.3879; PaintInfo[2][0][1] = 801.5635; PaintInfo[2][0][2] = 35.1498;
    PaintInfo[2][1][0] = -2888.5117; PaintInfo[2][1][1] = 794.5801; PaintInfo[2][1][2] = 35.0313;
    PaintInfo[2][2][0] = -2885.9170; PaintInfo[2][2][1] = 774.7571; PaintInfo[2][2][2] = 35.0384;

    // house 4
    PaintInfo[3][0][0] = -2884.5149; PaintInfo[3][0][1] = 747.5179; PaintInfo[3][0][2] = 29.2062;
    PaintInfo[3][1][0] = -2891.7100; PaintInfo[3][1][1] = 746.8639; PaintInfo[3][1][2] = 29.1719;
    PaintInfo[3][2][0] = -2889.5933; PaintInfo[3][2][1] = 722.6680; PaintInfo[3][2][2] = 29.1719;

    // house 5
    PaintInfo[4][0][0] = -2874.0308; PaintInfo[4][0][1] = 700.8782; PaintInfo[4][0][2] = 23.4575;
    PaintInfo[4][1][0] = -2879.4846; PaintInfo[4][1][1] = 694.9000; PaintInfo[4][1][2] = 23.4531;
    PaintInfo[4][2][0] = -2865.9790; PaintInfo[4][2][1] = 674.9018; PaintInfo[4][2][2] = 23.4531;

    // house 6
    PaintInfo[5][0][0] = -2842.9500; PaintInfo[5][0][1] = 901.3760; PaintInfo[5][0][2] = 44.0547;
    PaintInfo[5][1][0] = -2850.5708; PaintInfo[5][1][1] = 905.6861; PaintInfo[5][1][2] = 44.0547;
    PaintInfo[5][2][0] = -2849.0415; PaintInfo[5][2][1] = 929.9394; PaintInfo[5][2][2] = 44.0547;

    // house 7
    PaintInfo[6][0][0] = -2851.9167; PaintInfo[6][0][1] = 944.7149; PaintInfo[6][0][2] = 44.0547;
    PaintInfo[6][1][0] = -2861.3093; PaintInfo[6][1][1] = 947.7372; PaintInfo[6][1][2] = 44.0547;
    PaintInfo[6][2][0] = -2867.2339; PaintInfo[6][2][1] = 967.0223; PaintInfo[6][2][2] = 44.0547;

    // house 8
    PaintInfo[7][0][0] = -2874.4875; PaintInfo[7][0][1] = 978.2903; PaintInfo[7][0][2] = 40.7188;
    PaintInfo[7][1][0] = -2883.5815; PaintInfo[7][1][1] = 979.6193; PaintInfo[7][1][2] = 40.7188;
    PaintInfo[7][2][0] = -2891.7761; PaintInfo[7][2][1] = 1001.8345; PaintInfo[7][2][2] = 40.7188;

    // house 9
    PaintInfo[8][0][0] = -2894.4641; PaintInfo[8][0][1] = 1013.8566; PaintInfo[8][0][2] = 36.8281;
    PaintInfo[8][1][0] = -2896.8147; PaintInfo[8][1][1] = 1020.3206; PaintInfo[8][1][2] = 36.8281;
    PaintInfo[8][2][0] = -2900.3862; PaintInfo[8][2][1] = 1030.1024; PaintInfo[8][2][2] = 36.8281;

    // house 10
    PaintInfo[9][0][0] = -2900.7146; PaintInfo[9][0][1] = 1053.5397; PaintInfo[9][0][2] = 32.1328;
    PaintInfo[9][1][0] = -2900.5940; PaintInfo[9][1][1] = 1064.1160; PaintInfo[9][1][2] = 32.1328;
    PaintInfo[9][2][0] = -2900.4854; PaintInfo[9][2][1] = 1077.4283; PaintInfo[9][2][2] = 32.1328;

    // house 11
    PaintInfo[10][0][0] = -2904.5994; PaintInfo[10][0][1] = 1097.8978; PaintInfo[10][0][2] = 27.0703;
    PaintInfo[10][1][0] = -2904.7017; PaintInfo[10][1][1] = 1122.6342; PaintInfo[10][1][2] = 27.0703;
    PaintInfo[10][2][0] = -2912.5920; PaintInfo[10][2][1] = 1121.8875; PaintInfo[10][2][2] = 27.0703;

    // house 12
    PaintInfo[11][0][0] = -2905.1121; PaintInfo[11][0][1] = 1151.5498; PaintInfo[11][0][2] = 13.6712;
    PaintInfo[11][1][0] = -2905.2610; PaintInfo[11][1][1] = 1161.7814; PaintInfo[11][1][2] = 13.6641;
    PaintInfo[11][2][0] = -2909.6631; PaintInfo[11][2][1] = 1180.6768; PaintInfo[11][2][2] = 13.6641;

    return 1;
}
ResetPaintVariable(playerid)
{
    PlayerJobHouseID[playerid] = -1;
    PlayerJobPaintSpot[playerid] = -1;
    PlayerPaintSkin[playerid] = 0;
    antitelepainter[playerid] = 0;
    return 1;
}
hook OnPlayerDisconnect(playerid, reason)
{
    if (PlayerPaintSkin[playerid] != 0)
        SetPlayerSkin(playerid, tempskinpaint[playerid]);
    if (paintertimer[playerid])
        KillTimer(paintertimer[playerid]);
    return 1;
}
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(newkeys & KEY_YES)
    {
        if(IsPlayerInRangeOfPoint(playerid, 5.0, -2456.8425,786.6091,35.1719))
        {
            new str[64];
            format(str, sizeof(str), "Do ben chai son ATM cua ban: %d/20", PlayerInfo[playerid][pBinhXit]);
            ShowPlayerDialog(playerid, DIALOG_MENU_PAINTER, DIALOG_STYLE_LIST, str, "Nhan viec / Ngung nhan viec\nMua chai son (15.000$)", ">", "X");
            return 1;
        }
    }
    return 1;
}
hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_MENU_PAINTER)
    {
        switch(listitem)
        {
            case 0:
            {
                if (PlayerPaintSkin[playerid] == 8)
                {
                    if (PlayerJobPaintSpot[playerid] != -1)
                        DisablePlayerCheckpoint(playerid);
                    ResetPaintVariable(playerid);
                    SetPlayerSkin(playerid, tempskinpaint[playerid]);

                    SendClientMessage(playerid, COLOR_GREY, "Ban da nghi viec!");
                }
                else
                {
                    PlayerPaintSkin[playerid] = 8;
                    tempskinpaint[playerid] = GetPlayerSkin(playerid);
                    SetPlayerSkin(playerid, PlayerPaintSkin[playerid]);
                    SendClientMessage(playerid, COLOR_GREY, "Hay di den CHECKPOINT de lam cong viec!");
                    antitelepainter[playerid] = 10+random(15);
                    PlayerJobHouseID[playerid] = Random(0, 11);
                    PlayerJobPaintSpot[playerid] = 0;
                    SetTimerEx("antiteleportpainter", 1000, false, "d", playerid);
                    new houseid = PlayerJobHouseID[playerid], spotid = PlayerJobPaintSpot[playerid];
                    SetPlayerCheckpoint(playerid, PaintInfo[houseid][spotid][0], PaintInfo[houseid][spotid][1], PaintInfo[houseid][spotid][2], 1.5);
                }
            }
            case 1:
            {
                if (PlayerInfo[playerid][pCash] < 15000)
                    return SendClientMessage(playerid, COLOR_GREY, "Khong du tien de mua chai son!");
                if (PlayerInfo[playerid][pBinhXit] >= 5)
                    return SendClientMessage(playerid, COLOR_GREY, "Chai son ATM cua ban con do ben nhieu hon 5 nen khong the mua them!");
                PlayerInfo[playerid][pBinhXit] = 20;
                GivePlayerCash(playerid, -15000);
                SendClientMessage(playerid, -1, "Ban da mua thanh cong {ff0000}chai son ATM (do ben 20){ffffff} voi gia 15.000$.");
                SendClientMessage(playerid, COLOR_GREY, "Neu nhu ban khong co CHECKPOINT de lam viec ban co the ngung va nhan cong viec lai.");
            }
        }
        return 1;
    }
    return 0;
}
hook OnPlayerEnterCheckpoint(playerid)
{
    if (PlayerJobHouseID[playerid] >= 0)
    {
        if (PlayerJobPaintSpot[playerid] >= 0)
        {
            new i = PlayerJobPaintSpot[playerid], j = PlayerJobHouseID[playerid];
            if (IsPlayerInRangeOfPoint(playerid, 10.0, PaintInfo[j][i][0], PaintInfo[j][i][1], PaintInfo[j][i][2]))
            {
                if (PlayerInfo[playerid][pBinhXit] <= 0)
                {
                    CP[playerid] = 252000;
                    SendClientMessageEx(playerid, COLOR_GREY, "Chai son cua ban da het hay di den CHECKPOINT de lay them son.");
                    DisableCheckPoint(playerid);
                    SetPlayerCheckpoint(playerid, -2456.8425,786.6091,35.1719, 3.0);
                    ResetPaintVariable(playerid);
                    return 1;
                }
                     
                if (i == 0 && antitelepainter[playerid] > 0 && PlayerInfo[playerid][pAdmin] == 0)
                {
                    new szMess[256];
                    format(szMess, sizeof(szMess), "%s da bi kick vi su dung hack Teleport Son tuong.", GetPlayerNameEx(playerid));
                    SetPlayerPos(playerid, LastPosF4T[playerid][0], LastPosF4T[playerid][1], LastPosF4T[playerid][2]);
                    ABroadCast(COLOR_YELLOW, szMess, 2);
                    discord_messHack(szMess);
                    SendClientMessageEx(playerid, COLOR_WHITE, "[{DD0000}Anti-F4T{FFFFFF}] Anti cheating Teleport, neu co hack that hay tat giup tuiiii...!");
                    SetTimerEx("KickEx", 2000, 0, "i", playerid);
                    return 1;
                }
                DisableCheckPoint(playerid);
                paintertime[playerid] = 15;
                paintertimer[playerid] = SetTimerEx("startpainter", 100, false, "d", playerid);
            }
            
        }
    }
    return 1;
}
forward startpainter(playerid);
public startpainter(playerid)
{
    if (paintertime[playerid] > 0)
    {
        new str[64];
        ApplyAnimation(playerid, "BOMBER", "BOM_Plant_Loop", 4.1, 1, 0, 0, 1, 0);
        format(str, sizeof(str), "~w~Ban dang son tuong~n~Vui long doi: ~r~%d", paintertime[playerid]);
        GameTextForPlayer(playerid, str, 1000, 5);
        ApplyAnimation(playerid, "SPRAYCAN", "spraycan_full", 4.0, 1, 1, 1, 0, 0, 1);
        paintertime[playerid]--;
        paintertimer[playerid] = SetTimerEx("startpainter", 1000, false, "d", playerid);
        SetPlayerAttachedObject(playerid, 9, 365, 6,0, 0.0,0.119,0.00, 0.00, 0.00,1.5, 1.5, 1);
    }
    else
    {
        new str[128], haomon = Random(0,2);
        PlayerInfo[playerid][pBinhXit] -= haomon;
        format(str,sizeof(str), "Ban da bi giam do ben: -%d | Do ben chai son ATM: %d/20", haomon,PlayerInfo[playerid][pBinhXit]);
        SendClientMessage(playerid, COLOR_GREY, str);
        RemovePlayerAttachedObject(playerid, 9);
        if (PlayerJobPaintSpot[playerid] < 2)
        {
            PlayerJobPaintSpot[playerid]++;
            new houseid = PlayerJobHouseID[playerid], spotid = PlayerJobPaintSpot[playerid];
            format(str,sizeof(str), "Ban da hoan thanh CHECKPOINT THU %d, hay tiep tuc cong viec de nhan tien.", PlayerJobPaintSpot[playerid]);
            SendClientMessage(playerid, COLOR_GREY, str);
            SetPlayerCheckpoint(playerid, PaintInfo[houseid][spotid][0], PaintInfo[houseid][spotid][1], PaintInfo[houseid][spotid][2], 1.5);
        }
        else
        {
            new cashjob = Random(13000, 17000);
            format(str, sizeof(str), "Ban da hoan thanh cong viec va nhan duoc %d$.", cashjob);
            SendClientMessage(playerid, COLOR_GREY, str);
            GivePlayerCash(playerid, cashjob);
            ResetPaintVariable(playerid);
        }
        ClearAnimations(playerid);
    }
    return 1;
}
forward antiteleportpainter(playerid);
public antiteleportpainter(playerid)
{
    if (antitelepainter[playerid] > 0)
    {
        antitelepainter[playerid]--;
        SetTimerEx("antiteleportpainter", 1000, false, "d", playerid);
    }
} 
