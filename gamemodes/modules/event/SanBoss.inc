#include <YSI/YSI_Coding/y_hooks>

enum bossinfo
    {BossID, BossTrangThai, BossMau}
new conboss[bossinfo];
new BossMoveTime;
new BossTimerMove;
new BossRangeTimer; 

new PlayerText:BossHP[MAX_PLAYERS][2];
new PlayerBar:BossHPBar[MAX_PLAYERS][1];
new bool:PlayerShowingBossHP[MAX_PLAYERS]; 

#define MAX_BOSS_HP 500000.0
#define BOSS_SHOW_RANGE 100.0

forward BossDiChuyen();
public BossDiChuyen()
{
    new Float:posX, Float:posY, Float:posZ;
    GetObjectPos(conboss[BossID], posX, posY, posZ);
    if (IsInRangeOfPoint(posX, posY, posZ, -1397.6338,812.9567,17.8403, 100.0))
    {
        BossMoveTime = MoveObject(conboss[BossID], -1397.6338, 1012.9567, 17.8403, 2.00);
    }
    else
    {
        BossMoveTime = MoveObject(conboss[BossID], -1397.6338,812.9567,17.8403, 2.00);
    }
    BossTimerMove = SetTimer("BossDiChuyen", BossMoveTime, false);
}

forward CheckBossRange();
public CheckBossRange()
{
    if(conboss[BossTrangThai] == 0) return 1;
    
    new Float:bossX, Float:bossY, Float:bossZ;
    GetObjectPos(conboss[BossID], bossX, bossY, bossZ);
    
    foreach(new i: Player)
    {
        if(gPlayerLogged{i} != 1 || !IsPlayerConnected(i)) continue;

        new bool:inRange = IsPlayerInRangeOfPoint(i, BOSS_SHOW_RANGE, bossX, bossY, bossZ);
        
        if(inRange && !PlayerShowingBossHP[i])
        {
            ShowBossHPToPlayer(i);
            PlayerShowingBossHP[i] = true;
        }
        else if(!inRange && PlayerShowingBossHP[i])
        {
            HideBossHPBarFromPlayer(i);
            PlayerShowingBossHP[i] = false;
        }
    }
    return 1;
}

CMD:taoboss(playerid, params[]) {
	if(PlayerInfo[playerid][pAdmin] >= 1338)
	{
		if(conboss[BossTrangThai] == 1) return SendClientMessage(playerid, COLOR_RED, "Boss da duoc spawn.");
		conboss[BossTrangThai] = 1;
		conboss[BossMau] = 500000;
		conboss[BossID] = CreateObject(8493, -1397.6338,812.9567,17.8403,0,0,0, 1000);
	    SendClientMessageToAll(-1, "{00FF00}[GTAHTB.TOP] {FFFFFF}BOSS con tau hai tac da duoc spawn.");
        
        foreach(new i: Player)
        {
            PlayerShowingBossHP[i] = false;
        }
        
  		BossMoveTime = MoveObject(conboss[BossID], -1397.6338, 1012.9567, 17.8403, 2.00);
	    BossTimerMove = SetTimer("BossDiChuyen", BossMoveTime, false);
	    
	    BossRangeTimer = SetTimer("CheckBossRange", 1000, true);

		DiscordLog(ccChannel, "⚔️ THÔNG BÁO: SĂN BOSS HẢI TẶC! 🏴‍☠️", "**⏰ Ngay bây giờ! Sự kiện săn Boss hải tặc chính thức bắt đầu!\nHạ gục Boss để nhận ngay phần thưởng ngẫu nhiên cực khủng!! (/bosshtb) 💰\nTham gia ngay, chiến đấu và rinh quà về thôi nào! 🔥**");
        DCC_SendChannelMessage(ccChannel, "@everyone");
    } else return SendClientMessageEx(playerid, COLOR_GRAD1, "Ban Khong Duoc Su Dung Lenh Nay");

	return 1;
}

CMD:setmauboss(playerid, params[]) {
    if(PlayerInfo[playerid][pAdmin] < 99999) return SendClientMessage(playerid, COLOR_RED, "Ban khong the dung lenh nay");
    conboss[BossMau] = 1000;
	return 1;
}

CMD:bosshtb(playerid, params[]) {
    SendClientMessage(playerid, -1, "Da dinh vi cho ban vi tri cua Boss HTB o tren ban do.");
    PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
    SetPlayerCheckpoint(playerid, -1435.2001,850.1367,7.1875, 3.0);
    CP[playerid] = 252000;
	return 1;
}

CMD:xoabosshtb(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 4) return SendClientMessage(playerid, COLOR_RED, "Ban khong the dung lenh nay");
	if(conboss[BossTrangThai] == 0) return SendClientMessage(playerid, COLOR_RED, "Boss Chua Duoc Tao");
 	DestroyObject(conboss[BossID]);
 	conboss[BossTrangThai] = 0;
 	
 	KillTimer(BossTimerMove);
 	KillTimer(BossRangeTimer);
 	
 	SendClientMessageToAll(COLOR_RED, "BOSS DA BI ADMIN NAO DO XOA");
    
    foreach(new i: Player)
	{
        HideBossHPBarFromPlayer(i);
        PlayerShowingBossHP[i] = false;
	}
	return 1;
}

hook OnPlayerSpawn(playerid)
{
    CreateBossHPTextdraws(playerid);
    PlayerShowingBossHP[playerid] = false;
	return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    PlayerShowingBossHP[playerid] = false;
    return 1;
}

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    h_AntiRapid(playerid, weaponid, hittype, hitid, fX, fY, fZ);
    if( hittype != BULLET_HIT_TYPE_NONE ) {
        if(!( -1000.0 <= fX <= 1000.0 ) || !( -1000.0 <= fY <= 1000.0 ) || !( -1000.0 <= fZ <= 1000.0)) {
            return 1;
        }
    }
    if(GetPlayerSkin(playerid) == 0 || GetPlayerSkin(playerid) == 311)
    {
        new string[1280];
        format(string, sizeof(string), "[ANTI-CHEAT] %s da bi kick nghi van su dung Hack Multiplayer.", GetPlayerNameEx(playerid));
        ABroadCast(COLOR_YELLOW, string, 2);
        Kick(playerid);
        return 1;
    }

    if(PlayerInfo[playerid][pAdmin] == 0 && weaponid >= 1 && weaponid < 39)
    {
        if(GetPVarInt(playerid, "IsInArena") == -1)
        {
            if(GetPVarInt( playerid, "EventToken") == 0 && GetPVarInt(playerid, "Injured") != 1)
            {
                if(PlayerInfo[playerid][pGuns][GetWeaponSlotEx(weaponid)] != weaponid) // && PlayerInfo[playerid][pAGuns][GetWeaponSlotEx(weaponid)] == 0
                {
                	check_weapon_null[playerid] += 1;
                	if(check_weapon_null[playerid] == 3) {
                		new string[1280];
	                    format(string, sizeof(string), "[ANTI-CHEAT] %s da bi kick nghi van su dung Hack vu khi (Null).", GetPlayerNameEx(playerid));
	                    ABroadCast(COLOR_YELLOW, string, 2);

	                    Kick(playerid);
	                    return 1;
                	}
                }
                else check_weapon_null[playerid] = 0;
            }
        }
    }

    if (hittype == 3)
    {
        if (conboss[BossTrangThai] == 1)
        {
            if (hitid == conboss[BossID])
            {
                new str[256];
                if (conboss[BossMau]-50 > 0)
                {
                    conboss[BossMau] -= 50;       
					foreach(new i: Player)
					{
                        if(PlayerShowingBossHP[i]) 
                        {
                            UpdateBossBar(i);
                        }
					}             
                }
                else
                {
					KillTimer(BossTimerMove);
					KillTimer(BossRangeTimer);
					
					foreach(new i: Player)
					{
                        HideBossHPBarFromPlayer(i);
                        PlayerShowingBossHP[i] = false;
					}
                    format(str, sizeof(str), "~p~Su kien dac biet cua GTAHTB.TOP~n~~w~Nguoi choi ~r~%s ~w~da kill duoc BOSS", GetPlayerNameEx(playerid));
                    GameTextForAll(str, 25000, 4);
                    format(str, sizeof(str), "Xin chuc mung, nguoi choi {ffff00}%s {ffffff}da kill duoc {FF00FF}BOSS TAU HAI TAC GTAHTB.TOP{ffffff}.", GetPlayerNameEx(playerid));
                    SendClientMessageToAll(-1, str);
                    DestroyObject(conboss[BossID]);
                    conboss[BossTrangThai] = 0;
                    switch(random(100))
                    {
                        case 0..24:
                        {
                            new randmat = Random(10000, 20000);
                            format(str, sizeof(str), "%s da nhan duoc: {ff0000}%d vat lieu", GetPlayerNameEx(playerid), randmat);
                            PlayerInfo[playerid][pMats] += randmat;
                        }
                        case 25..49:
                        {
                            new randmon = Random(200000, 500000);
                            format(str, sizeof(str), "%s da nhan duoc: {ff0000}%d$", GetPlayerNameEx(playerid), randmon);
                            GivePlayerCash(playerid, randmon);
                        }
                        case 50..69:
                        {
                            format(str, sizeof(str), "%s da nhan duoc: {ff0000}1 pack vu khi", GetPlayerNameEx(playerid));
                            GivePlayerValidWeapon(playerid,27,50000);
                            GivePlayerValidWeapon(playerid,24,50000);
                            GivePlayerValidWeapon(playerid,29,50000);
                            GivePlayerValidWeapon(playerid,30,50000);
                        }
                        case 70..89:
                        {
							PlayerInfo[playerid][pDonateRank] = 3;
							PlayerInfo[playerid][pTempVIP] = 0;
							PlayerInfo[playerid][pBuddyInvited] = 0;
							if (PlayerInfo[playerid][pVIPExpire] > gettime())
							{

								PlayerInfo[playerid][pVIPExpire] += (86400 * 1);
							}
							else
							{

								PlayerInfo[playerid][pVIPExpire] = gettime() + (86400 * 1);
							}


                            format(str, sizeof(str), "%s da nhan duoc: {ff0000}1 vip gold (1 ngay)", GetPlayerNameEx(playerid));
                            SendClientMessageEx(playerid, -1, "Ban da nhan duoc 1 Gold VIP");
                        }
                        case 90..100:
                        {
                            format(str, sizeof(str), "%s da nhan duoc: {ff0000}1 xe PCJ-600, BF-400, FCR-900 (chon 1 trong 3)", GetPlayerNameEx(playerid));
                            ShowPlayerDialog(playerid,DIALOG_NOTHING,DIALOG_STYLE_MSGBOX,"Voucher Motobike","{ffffff}Ban duoc phep chon 1 trong 3 chiec xe sau:\n\nBF-400, PCJ-600, FCR-900\n\n{ff0000}Ban phai chup lai man hinh nay va sau do vao discord server tao ticket de nhan xe.","Thoat","");
                        }
                    }
                    SendClientMessageToAll(-1, str);
                }
            }
        }
    }
    return 1;
}

stock ShowBossHPToPlayer(playerid)
{
    if(gPlayerLogged{playerid} != 1) return 0;

    new Float:hpboss = (conboss[BossMau] / MAX_BOSS_HP) * 100.0;
    new string[200];
    format(string, sizeof(string), "%d", floatround(conboss[BossMau]));

    PlayerTextDrawSetString(playerid, BossHP[playerid][0], string);
    PlayerTextDrawShow(playerid, BossHP[playerid][0]);
    PlayerTextDrawShow(playerid, BossHP[playerid][1]);
    
    SetPlayerProgressBarValue(playerid, BossHPBar[playerid][0], hpboss);
    ShowPlayerProgressBar(playerid, BossHPBar[playerid][0]);
    
    return 1;
}

stock UpdateBossBar(playerid)
{
    if(!PlayerShowingBossHP[playerid]) return 0;
    if(gPlayerLogged{playerid} != 1) return 0;
    
    new string[200];
    format(string, sizeof(string), "%d", floatround(conboss[BossMau]));
    new Float:hpboss = (conboss[BossMau] / MAX_BOSS_HP) * 100.0;

    PlayerTextDrawSetString(playerid, BossHP[playerid][0], string);
    SetPlayerProgressBarValue(playerid, BossHPBar[playerid][0], hpboss);
    
    return 1;
}

public HideBossHPBarFromPlayer(playerid)
{
    PlayerTextDrawHide(playerid, BossHP[playerid][0]);
    PlayerTextDrawHide(playerid, BossHP[playerid][1]);
    HidePlayerProgressBar(playerid, BossHPBar[playerid][0]);
    return 1;
}

stock CreateBossHPTextdraws(playerid)
{
    BossHP[playerid][0] = CreatePlayerTextDraw(playerid, 294.000000, 390.000000, "500000");
    PlayerTextDrawFont(playerid, BossHP[playerid][0], 2);
    PlayerTextDrawLetterSize(playerid, BossHP[playerid][0], 0.300000, 1.700000);
    PlayerTextDrawTextSize(playerid, BossHP[playerid][0], 400.000000, 17.000000);
    PlayerTextDrawSetOutline(playerid, BossHP[playerid][0], 1);
    PlayerTextDrawAlignment(playerid, BossHP[playerid][0], 1);
    PlayerTextDrawColour(playerid, BossHP[playerid][0], 0xFFFF00FF);
    PlayerTextDrawSetProportional(playerid, BossHP[playerid][0], 1);

    BossHP[playerid][1] = CreatePlayerTextDraw(playerid, 250.000000, 354.000000, "Boss Tau Hai Tac (GTAHTB)");
    PlayerTextDrawFont(playerid, BossHP[playerid][1], 2);
    PlayerTextDrawLetterSize(playerid, BossHP[playerid][1], 0.250000, 2.000000);
    PlayerTextDrawTextSize(playerid, BossHP[playerid][1], 15000.000000, 7.000000);
    PlayerTextDrawSetOutline(playerid, BossHP[playerid][1], 1);
    PlayerTextDrawAlignment(playerid, BossHP[playerid][1], 1);
    PlayerTextDrawColour(playerid, BossHP[playerid][1], 0xDD0000FF); 
    PlayerTextDrawSetProportional(playerid, BossHP[playerid][1], 1);

    BossHPBar[playerid][0] = CreatePlayerProgressBar(playerid, 263.000000, 380.000000, 122.000000, 6.500000, -16776961, 100.0, 100);
    
    return 1;
}

//debug hp
CMD:checkbosshp(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 4) return 1;
    
    new string[128];
    format(string, sizeof(string), "Boss Status: %s | HP: %d | Showing HP: %s", 
        conboss[BossTrangThai] ? "Active" : "Inactive",
        conboss[BossMau],
        PlayerShowingBossHP[playerid] ? "Yes" : "No"
    );
    SendClientMessage(playerid, -1, string);
    return 1;
}