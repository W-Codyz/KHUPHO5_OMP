#include <YSI\YSI_Coding\y_hooks>

#define MAX_GPS_POINTS 5

new GPSInfo[MAX_GPS_POINTS][E_GPS_INFO] = 
{
	{"Newbie Spawn", -1754.3822, 949.6694, 24.7422},
	{"City Hall", -2020.1469, -858.9731, 32.1878},
	{"San Fierro VIP", -2434.6670, 497.8742, 29.9372},
	{"Trung tam giay phep", -1941.5658, 577.1724, 35.1719},
	{"Benh vien San Fierro", -2665.1887, 633.8109, 14.4531}
};

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

stock GPS_ShowLocation(playerid)
{
	new dialogInfo[1028];
	strcat(dialogInfo, "Ten dia diem\tKhoang cach\n");
	for(new i;i < MAX_GPS_POINTS;i++)
	{
		format(dialogInfo, _, "%s{FFFFFF}%s\t{619EFF}%0.1f km\n", dialogInfo, GPSInfo[i][gpsName], GetPlayerDistanceFromPoint(playerid, GPSInfo[i][gpsX], GPSInfo[i][gpsY], GPSInfo[i][gpsZ]) / 1000.0);
	}

	ShowPlayerDialog(playerid, DIALOG_FINDLOCATION, DIALOG_STYLE_TABLIST_HEADERS, "GPS Navigation > Dia diem San Andreas", dialogInfo, "Chon", "Quay lai");
	return 1;
}

stock GPS_ShowJob(playerid)
{
	new dialogInfo[1028];
	strcat(dialogInfo, "Ten cong viec\tLoai cong viec\tKhoang cach\n");
	for(new i=1;i<MAX_JOB_TYPE;i++)
	{
		new Float:distance = GetPlayerDistanceFromPoint(playerid, JobInfo[i][jobPosX], JobInfo[i][jobPosY], JobInfo[i][jobPosZ]);
		format(dialogInfo, _, "%s{FFFFFF}%s\t{86C757}Can xin viec\t{619EFF}%0.1f km\n", dialogInfo, JobInfo[i][jobName], distance / 1000.0);
	}

	for(new i=0; i < sizeof(SideJobInfo); i++) 
	{
		new Float:distance = GetPlayerDistanceFromPoint(playerid, SideJobInfo[i][sideJobPosX], SideJobInfo[i][sideJobPosY], SideJobInfo[i][sideJobPosZ]);
		format(dialogInfo, _, "%s{FFFFFF}%s\t{C7C757}Khong can xin viec\t{619EFF}%0.1f km\n", dialogInfo, SideJobInfo[i][sideJobName], distance / 1000.0);
	}

	ShowPlayerDialog(playerid, DIALOG_FINDJOB, DIALOG_STYLE_TABLIST_HEADERS, "GPS Navigation > Cong viec", dialogInfo, "Chon", "Quay lai");
	return 1;
}

stock GPS_Show247Store(playerid)
{
	new dialogInfo[1028], count;
	strcat(dialogInfo, "Ten cua hang\tKhoang cach\n");
	for(new i;i<MAX_BUSINESSES;i++)
	{
		if(Businesses[i][bType] == BUSINESS_TYPE_STORE && Businesses[i][bExtPos][0] != 0.0)
		{
			new Float:distance = GetPlayerDistanceFromPoint(playerid, Businesses[i][bExtPos][0], Businesses[i][bExtPos][1], Businesses[i][bExtPos][2]);
			format(dialogInfo, _, "%s{FFFFFF}%s\t{619EFF}%0.1f km\n", dialogInfo, Businesses[i][bName], distance / 1000.0);
			count++;
		}
	}

	if(count == 0) strcat(dialogInfo, "Khong co cua hang 24/7 nao tren ban do.");

	ShowPlayerDialog(playerid, DIALOG_FINDSTORE, DIALOG_STYLE_TABLIST_HEADERS, "GPS Navigation > Cua hang 24/7", dialogInfo, "Chon", "Quay lai");
	return 1;
}

stock GPS_ShowRestaurant(playerid)
{
	new dialogInfo[1028], count;
	strcat(dialogInfo, "Ten cua hang\tKhoang cach\n");
	for(new i;i<MAX_BUSINESSES;i++)
	{
		if(Businesses[i][bType] == BUSINESS_TYPE_RESTAURANT && Businesses[i][bExtPos][0] != 0.0)
		{
			new Float:distance = GetPlayerDistanceFromPoint(playerid, Businesses[i][bExtPos][0], Businesses[i][bExtPos][1], Businesses[i][bExtPos][2]);
			format(dialogInfo, _, "%s{FFFFFF}%s\t{619EFF}%0.1f km\n", dialogInfo, Businesses[i][bName], distance / 1000.0);
			count++;
		}
	}

	if(count == 0) strcat(dialogInfo, "Khong co nha hang nao tren ban do.");

	ShowPlayerDialog(playerid, DIALOG_FINDRESTAURANT, DIALOG_STYLE_TABLIST_HEADERS, "GPS Navigation > Nha hang", dialogInfo, "Chon", "Quay lai");
	return 1;
}

stock GPS_ShowBusinesses(playerid)
{
	new count;
	AddDialogListitem(playerid, "Ten doanh nghiep\tKhoang cach");
	for(new i;i<MAX_BUSINESSES;i++)
	{
		if(Businesses[i][bType] != BUSINESS_TYPE_RESTAURANT && Businesses[i][bType] != BUSINESS_TYPE_STORE && Businesses[i][bExtPos][0] != 0.0)
		{
			new Float:distance = GetPlayerDistanceFromPoint(playerid, Businesses[i][bExtPos][0], Businesses[i][bExtPos][1], Businesses[i][bExtPos][2]);
			AddDialogListitem(playerid, "{FFFFFF}%s\t{619EFF}%0.1f km", Businesses[i][bName], distance / 1000.0);
			count++;
		}
	}

	if(count == 0) AddDialogListitem(playerid, "Khong co doanh nghiep nao tren ban do.");

	Dialog_ShowPages(playerid, DIALOG_FINDBUSINESSES, DIALOG_STYLE_TABLIST_HEADERS, "GPS Navigation > Nha hang", "Chon", "Quay lai", 15, "{54B530}Trang tiep theo >", "{D5D358}< Trang truoc");
	return 1;
}

DialogPages:DIALOG_FINDBUSINESSES(playerid, response, listitem, inputtext[])
{
	if(!response) return PC_EmulateCommand(playerid, "/gps");

	if(gPlayerCheckpointStatus[playerid] != CHECKPOINT_NONE)
		return SendClientMessage(playerid, COLOR_GREY, "Ban can xoa diem danh dau tren mini-map truoc.");
	
	new count = -1;
	for(new i;i<MAX_BUSINESSES;i++)
	{
		if(Businesses[i][bType] != BUSINESS_TYPE_RESTAURANT && Businesses[i][bType] != BUSINESS_TYPE_STORE && Businesses[i][bExtPos][0] != 0.0)
		{
			if(++count != listitem) continue;

			SetPlayerCP(playerid, Businesses[i][bExtPos][0], Businesses[i][bExtPos][1], Businesses[i][bExtPos][2], 5.0);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "____________ GPS Nagavition ____________");
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "Dia diem:{FFFFFF} %s", Businesses[i][bName]);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "Khoang cach:{FFFFFF} %0.1f km", GetPlayerDistanceFromPoint(playerid, Businesses[i][bExtPos][0], Businesses[i][bExtPos][1], Businesses[i][bExtPos][2]) / 1000.0);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "______________________________________");
			gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
			break;
		}
	}
	return 1;
}

CMD:gps(playerid, params[])
{
	ShowPlayerDialog(playerid, DIALOG_GPS, DIALOG_STYLE_TABLIST, "Navigation GPS", "\
            {FFFFFF}Dia diem San Andreas\t{BFC26E}Tim kiem cac dia diem co san tren San Andreas\n\
            {FFFFFF}Cong viec\t{BFC26E}Tim kiem cac viec lam tren San Andreas\n\
            {FFFFFF}Cua hang 24/7\t{BFC26E}Tim kiem cac cua hang 24/7 tren San Andreas\n\
            {FFFFFF}Nha hang\t{BFC26E}Tim kiem cac nha hang tren San Andreas\n\
            {FFFFFF}Doanh nghiep\t{BFC26E}Tim kiem cac doanh nghiep khac tren San Andreas\n\
            {FFFFFF}Xoa diem danh dau\t{BFC26E}Xoa diem danh dau ton tai tren mini-map", "Chon", "Dong");
	return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_GPS)
	{
		if(!response) return 1;

		switch(listitem) 
		{
			case 0: GPS_ShowLocation(playerid);
			case 1: GPS_ShowJob(playerid);
			case 2: GPS_Show247Store(playerid);
			case 3: GPS_ShowRestaurant(playerid);
			case 4: GPS_ShowBusinesses(playerid);
			case 5: PC_EmulateCommand(playerid, "/killcheckpoint");
		}
	}

	if(dialogid == DIALOG_FINDJOB)
	{
		if(!response) 
			return PC_EmulateCommand(playerid, "/gps");

		if(gPlayerCheckpointStatus[playerid] != CHECKPOINT_NONE)
			return SendClientMessage(playerid, COLOR_GREY, "Ban can xoa diem danh dau tren mini-map truoc.");

		new jobId = listitem+1;
		if(jobId < MAX_JOB_TYPE) {
			SetPlayerCP(playerid, JobInfo[jobId][jobPosX], JobInfo[jobId][jobPosY], JobInfo[jobId][jobPosZ], 5.0);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "____________ GPS Nagavition ____________");
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "Dia diem:{FFFFFF} Cong viec %s", JobInfo[jobId][jobName]);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "Khoang cach:{FFFFFF} %0.1f km", GetPlayerDistanceFromPoint(playerid, JobInfo[jobId][jobPosX], JobInfo[jobId][jobPosY], JobInfo[jobId][jobPosZ]) / 1000.0);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "______________________________________");
			gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
		}
		else {
			new sideJobId = jobId - MAX_JOB_TYPE;
			SetPlayerCP(playerid, SideJobInfo[sideJobId][sideJobPosX], SideJobInfo[sideJobId][sideJobPosY], SideJobInfo[sideJobId][sideJobPosZ], 5.0);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "____________ GPS Nagavition ____________");
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "Dia diem:{FFFFFF} Cong viec %s", SideJobInfo[sideJobId][sideJobName]);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "Khoang cach:{FFFFFF} %0.1f km", GetPlayerDistanceFromPoint(playerid, SideJobInfo[sideJobId][sideJobPosX], SideJobInfo[sideJobId][sideJobPosY], SideJobInfo[sideJobId][sideJobPosZ]) / 1000.0);
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "______________________________________");
			gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
		}
	}

	if(dialogid == DIALOG_FINDLOCATION)
	{
		if(!response) 
			return PC_EmulateCommand(playerid, "/gps");
		
		if(gPlayerCheckpointStatus[playerid] != CHECKPOINT_NONE)
			return SendClientMessage(playerid, COLOR_GREY, "Ban can xoa diem danh dau tren mini-map truoc.");
	
		SetPlayerCP(playerid, GPSInfo[listitem][gpsX], GPSInfo[listitem][gpsY], GPSInfo[listitem][gpsZ], 5.0);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "____________ GPS Nagavition ____________");
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "Dia diem:{FFFFFF} %s", GPSInfo[listitem][gpsName]);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "Khoang cach:{FFFFFF} %0.1f km", GetPlayerDistanceFromPoint(playerid, GPSInfo[listitem][gpsX], GPSInfo[listitem][gpsY], GPSInfo[listitem][gpsZ]) / 1000.0);
		SendClientMessage(playerid, COLOR_LIGHTBLUE, "______________________________________");
		gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
	}

	if(dialogid == DIALOG_FINDSTORE)
	{
		if(!response) 
			return PC_EmulateCommand(playerid, "/gps");
		
		if(gPlayerCheckpointStatus[playerid] != CHECKPOINT_NONE)
			return SendClientMessage(playerid, COLOR_GREY, "Ban can xoa diem danh dau tren mini-map truoc.");
		
		new count = -1;
		for(new i;i<MAX_BUSINESSES;i++)
		{
			if(Businesses[i][bType] == BUSINESS_TYPE_STORE && Businesses[i][bExtPos][0] != 0.0)
			{
				if(++count != listitem) continue;

				SetPlayerCP(playerid, Businesses[i][bExtPos][0], Businesses[i][bExtPos][1], Businesses[i][bExtPos][2], 5.0);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, "____________ GPS Nagavition ____________");
				SendClientMessage(playerid, COLOR_LIGHTBLUE, "Dia diem:{FFFFFF} %s (Cua hang 24/7)", Businesses[i][bName]);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, "Khoang cach:{FFFFFF} %0.1f km", GetPlayerDistanceFromPoint(playerid, Businesses[i][bExtPos][0], Businesses[i][bExtPos][1], Businesses[i][bExtPos][2]) / 1000.0);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, "______________________________________");
				gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
				return 1;
			}
		}
	}

	if(dialogid == DIALOG_FINDRESTAURANT)
	{
		if(!response) 
			return PC_EmulateCommand(playerid, "/gps");
		
		if(gPlayerCheckpointStatus[playerid] != CHECKPOINT_NONE)
			return SendClientMessage(playerid, COLOR_GREY, "Ban can xoa diem danh dau tren mini-map truoc.");
		
		new count = -1;
		for(new i;i<MAX_BUSINESSES;i++)
		{
			if(Businesses[i][bType] == BUSINESS_TYPE_RESTAURANT && Businesses[i][bExtPos][0] != 0.0)
			{
				if(++count != listitem) continue;

				SetPlayerCP(playerid, Businesses[i][bExtPos][0], Businesses[i][bExtPos][1], Businesses[i][bExtPos][2], 5.0);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, "____________ GPS Nagavition ____________");
				SendClientMessage(playerid, COLOR_LIGHTBLUE, "Dia diem:{FFFFFF} %s (Nha hang)", Businesses[i][bName]);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, "Khoang cach:{FFFFFF} %0.1f km", GetPlayerDistanceFromPoint(playerid, Businesses[i][bExtPos][0], Businesses[i][bExtPos][1], Businesses[i][bExtPos][2]) / 1000.0);
				SendClientMessage(playerid, COLOR_LIGHTBLUE, "______________________________________");
				gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
				return 1;
			}
		}
	}
	return 1;
}