#include <YSI\YSI_Coding\y_hooks>

#define MAX_GPS_POINTS 5
#define			TIMVIECLAM					(5210)
#define         TIMDUONG                    (5211)

stock SetPlayerRaceCP(playerid, CP_TYPE:type, Float:centreX, Float:centreY, Float:centreZ, Float:nextX, Float:nextY, Float:nextZ, Float:size)
{
	SetPlayerWaze(playerid, centreX, centreY, centreZ, 0xD24949FF);
	return SetPlayerRaceCheckpoint(playerid, type, centreX, centreY, centreZ, nextX, nextY, nextZ, size);
}

stock DisablePlayerRaceCP(playerid)
{
	if(IsValidWazeGPS(playerid)) StopWazeGPS(playerid);
	return DisablePlayerRaceCheckpoint(playerid);
}

stock SetPlayerCP(playerid, Float:centreX, Float:centreY, Float:centreZ, Float:size, bool:wavegps=true)
{
	if(IsValidPlayerPickup(playerid, gPlayerCPArrow[playerid])) DestroyPlayerPickup(playerid, gPlayerCPArrow[playerid]);

	if(wavegps) SetPlayerWaze(playerid, centreX, centreY, centreZ, 0xD24949FF);
	gPlayerCPArrow[playerid] = CreatePlayerPickup(playerid, 19133, 23, centreX, centreY, centreZ+size, GetPlayerVirtualWorld(playerid));
	return SetPlayerCheckpoint(playerid, centreX, centreY, centreZ, size);
}

stock DisablePlayerCP(playerid)
{
	if(IsValidPlayerPickup(playerid, gPlayerCPArrow[playerid])) DestroyPlayerPickup(playerid, gPlayerCPArrow[playerid]);
	if(IsValidWazeGPS(playerid)) StopWazeGPS(playerid);

	gPlayerCPArrow[playerid] = -1;
	return DisablePlayerCheckpoint(playerid);
}

CMD:timduong(playerid, params[])
{
   ShowPlayerDialog(playerid, TIMDUONG, DIALOG_STYLE_LIST, "Tim Duong", \
   "Ngan Hang San Fierro\n\
   Ngan Hang Los Santos\n\
   Benh Vien San Fierro\n\
   Benh Vien Los Santos\n\
   Thi Bang Lai Xe\n\
   Cua Hang Xe The Thao\n\
   Cua Hang Xe Moto\n\
   Cua Hang Sieu Xe VIP\n\
   Cua Hang Quan Ao\n\
   Khu Vuc Cau Ca\n\
   Khu Vuc Ban Ca\n\
   Nha Hang An Uong\n\
   Khu VIP\n\
   Cua Hang Tien Loi\n\
   Cua Hang Sua Xe\n\
   Dau Truong PvP\n\
   Khu Giai Tri Casino\n\
   Phong Tap Vo", "Chon", "Dong");
   return 1;
}

CMD:timvieclam(playerid, params[]) 
{
   return ShowPlayerDialog(playerid, TIMVIECLAM, DIALOG_STYLE_LIST, "Tim Viec Lam", \
   "Tham Tu\n\
   Luat Su\n\
   Gai Diem\n\
   Ve Si Los Santos\n\
   Tho Sua Xe\n\
   Ban Vu Khi\n\
   Ve Si San Fierro\n\
   Tai Xe Taxi\n\
   Van Chuyen Thuoc\n\
   Tho Thu Cong\n\
   Nguoi Pha Ruou\n\
   Giao Banh Pizza\n\
   Tai Xe Trucker\n\
   Lai Thuyen Trucker\n\
   Nong Trai Can Sa\n\
   Ngu Dan Cau Ca\n\
   Tho Chat Go\n\
   Tho Son Tuong", "Chon", "Dong");
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == TIMDUONG) //toadotimduong
	{
	    if(response)
	    {
	       	if(listitem == 0) // Bank SF
		    {
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -1581.2155,902.8041,7.6953, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
				return 1;
			}
			if(listitem == 1) // Bank Rodeo
		    {
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, 587.0106,-1238.3374,17.8049, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
				return 1;
			}
			if(listitem == 2) // Benh vien Idlewood
		    {
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -2655.8972,634.8754,14.4531, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
				return 1;
			}
		    if(listitem == 3) // Benh vien All Saints
		    {
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, 1172.0809,-1323.4167,15.4031, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
			}
			if(listitem == 4) // City hall
		    {
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -2765.2007,375.8469,6.3435, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
            }
            if(listitem == 5) // ShopXe
		    {
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -1647.9294,1205.3108,7.2500, 3.0);
				SendClientMessage(playerid, COLOR_YELLOW, "Shop Xe SF1 !");
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
            }
            if(listitem == 6) // moto
		    {
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -1969.7197,293.8402,35.1719, 3.0);
				SendClientMessage(playerid, COLOR_YELLOW, "Shop Xe SF2 !");
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
             }
            if(listitem == 7) // moto
		    {
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -2411.9282,3.4309,35.3203, 3.0);
				SendClientMessage(playerid, COLOR_YELLOW, "Shop Xe VIP (SIEU XE) !");
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
             }
            if(listitem == 8) // moto
		    {
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -1695.2990,950.6997,24.8906, 3.0);
				SendClientMessage(playerid, COLOR_YELLOW, "Shop quan ao");
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
			}
			if(listitem == 9) // cau ca
		    {
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -2975.9360, 506.9248, 2.4297, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
			}
			if(listitem == 10) // ban ca
		    {
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -2927.2327,507.2165,4.9141, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
			}
			if(listitem == 11)
			{
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -1722.6263,1359.8776,7.1875, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
			}
			if(listitem == 12)
			{
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -2435.4814,500.9243,29.9394, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
			}
			if(listitem == 13)
			{
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -2442.5369,753.4180,35.1719, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
			}
			if(listitem == 14)
			{
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -1904.2194,274.2740,41.0469, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
			}
			if(listitem == 15)// -1492.1313,920.1057,7.1875
			{
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -1492.1313,920.1057,7.1875, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
			}
			if(listitem == 16)// -1492.1313,920.1057,7.1875
			{
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -2079.0525,1413.5898,7.2210, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
			}
 			if(listitem == 17)// -1492.1313,920.1057,7.1875
			{
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -1552.6379,1061.9465,7.1875, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
			}
    	}
    }
	if(dialogid == TIMVIECLAM)
	{
	    if(response)
	    {
	       	if(listitem == 0) // Tham Tu
		    {
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -1589.5458,717.1549,9.1181, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
				return 1;
			}
			if(listitem == 1) //Luat Su
		    {
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -1619.3232,716.7933,14.4121, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
				return 1;
			}
		    if(listitem == 2) //Gai Diem
		    {
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -1549.5161,1171.8917,7.1875, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
			}
			if(listitem == 3) //Ve si
		    {
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, 1099.73,-1504.67,15.800, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
			}
			if(listitem == 4) //Tho Sua Xe
		    {
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -2032.601928, 143.866592, 28.835937, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
			}
			if(listitem == 5) //Buon Vu khi
		    {
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -2623.333984, 209.235931, 4.684767, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
			}
			if(listitem == 6) //Vo si
		    {
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -2269.256103, -158.054321, 35.320312, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
			}
			if(listitem == 7) // Tai Xe Taxi
		    {
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -1981.144775, 133.063293, 27.687500, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
			}
			if(listitem == 8) // Van chuyen thuoc phien
		    {
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -382.0426,-1426.1294,26.0843, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
			}
			if(listitem == 9) // Tho Thu Cong
		    {
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -1356.7195,2065.3450,52.4677, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
			}
			if(listitem == 10) // Nguoi pha ruou
		    {
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -864.3550,1536.9703,22.5870, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
			}
			if(listitem == 11) //Pizza Boy
		    {
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -1720.962646, 1364.456176, 7.187500, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
			}
			if(listitem == 12) //Trucker 2139.3455,-81.7619,2.9146
			{
			    PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, -1560.963867, 127.491157, 3.554687, 3.0);
				gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
			}
			if(listitem == 13) //Trucker 2139.3455,-81.7619,2.9146
			{
			    PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCP(playerid, 2139.3455,-81.7619,2.9146, 3.0);
				gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
                return 1;
			}
			if(listitem == 14) //Trucker 	ShowPlayerDialog(playerid, CANSA_LIST, DIALOG_STYLE_LIST,"{ffff00}GTAHTB{ffffff} - Can Sa", "Tim duong can sa\nXem can sa", "Chon", "Thoat");
			{
			    PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				ShowPlayerDialog(playerid, CANSA_LIST, DIALOG_STYLE_LIST,"{ffff00}GTAHTB{ffffff} - Can Sa", "Tim duong can sa\nXem can sa", "Chon", "Thoat");
                return 1;
			}
			if(listitem == 15) //Trucker 	ShowPlayerDialog(playerid, CANSA_LIST, DIALOG_STYLE_LIST,"{ffff00}GTAHTB{ffffff} - Can Sa", "Tim duong can sa\nXem can sa", "Chon", "Thoat");
			{
			    PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
                SetPlayerCP(playerid, -2975.5642,480.8670,4.8925, 3.0);
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
				return 1;
			}
			if(listitem == 16) //
			{
			    PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
          		SetPlayerCP(playerid, -543.9225,-79.9980,62.9417, 3.0);
       		    gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
				return 1;	
			}
			if(listitem == 17) // son tuong
			{
			    PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
          		SetPlayerCP(playerid, -2456.8425,786.6091,35.1719, 3.0);
       		    gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
				return 1;	
			}
		}
	}
	if(dialogid == CANSA_LIST)
	{
		if(response)
		{
			if(listitem == 0)
			{
				ShowPlayerDialog(playerid, TIMDUONGCANSA, DIALOG_STYLE_LIST,"{ffff00}GTAHTB{ffffff} - Can Sa", "Vi tri can sa moc\nChe bien can sa\nBan can sa", "Chon", "Thoat");
				return 1;
			}
			if(listitem == 1)
			{
				new stringbx[128];
				format(stringbx, sizeof stringbx, "Cay can sa:\t %d\nTep can sa:\t %d\nPot:\t\t %d\nCrack:\t\t %d", PlayerInfo[playerid][pCanSa], PlayerInfo[playerid][pTepCanSa], PlayerInfo[playerid][pPot], PlayerInfo[playerid][pCrack]);
				ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "{ffff00}GTAHTB{ffffff} - Xem can sa", stringbx, "Dong y", "");
			}
		}
	}

	if(dialogid == TIMDUONGCANSA)
	{
		if(response)
		{
			if(listitem == 0)
			{
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCheckpoint(playerid, -1682.8035,-938.0014,66.8403, 3.0);
				CP[playerid] = 252000;
				return 1;
			}
			if(listitem == 1)
			{
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCheckpoint(playerid, -1431.5900,-1460.4982,101.6989, 3.0);
				CP[playerid] = 252000;
				return 1;
			}
			if(listitem == 2)
			{
				PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
				SetPlayerCheckpoint(playerid, -382.8928,-1426.2904,26.2979, 3.0);
				CP[playerid] = 252000;
				return 1;
			}
		}
	}
	return 1;
}