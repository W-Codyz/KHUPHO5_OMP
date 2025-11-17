#include <YSI\YSI_Coding\y_hooks>

Job_Init()
{
    new count;
    for(new i=1;i<MAX_JOB_TYPE;i++)
    {
        if(JobInfo[i][jobPosX] != 0.0)
        {
            new string[128];
            format(string, sizeof(string), "{F5F52F}%s\n\n{FFFFFF}Nhan \"{5FBC52}Y{FFFFFF}\" de tuong tac", JobInfo[i][jobName]);
            CreateDynamic3DTextLabel(string, 140, JobInfo[i][jobPosX], JobInfo[i][jobPosY], JobInfo[i][jobPosZ]+0.3, 10.0, _, _, 1, JobInfo[i][jobVW], JobInfo[i][jobInt]);
            CreateDynamicActor(JobInfo[i][jobSkin], JobInfo[i][jobPosX], JobInfo[i][jobPosY], JobInfo[i][jobPosZ], JobInfo[i][jobPosA], true, _, JobInfo[i][jobVW], JobInfo[i][jobInt]);
            count++;
        }
    }

    if(count) print("[Job] All Jobs Actor has been loaded.");
    else print("[Job] No Job Actor has been loaded.");

    return 1;
}

Job_KeyState(playerid, KEY:newkeys, KEY:oldkeys)
{
    if(PRESSED(KEY_YES))
    {
        for(new i=1;i<MAX_JOB_TYPE;i++)
        {
            if(JobInfo[i][jobPosX] != 0.0)
            {
                if(IsPlayerInRangeOfPoint(playerid, 2.5, JobInfo[i][jobPosX], JobInfo[i][jobPosY], JobInfo[i][jobPosZ]) && !IsPlayerInAnyVehicle(playerid))
                {
                    if(JobInfo[i][jobLevel] > PlayerInfo[playerid][pLevel])
                    {
                        ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "Thong tin cong viec", "{FFFFFF}Cong viec {E23636}yeu cau cap do %d{FFFFFF} tuy nhien cap do hien tai cua ban\nkhong dat yeu cau, vi the ban khong du dieu kien lam cong viec\nnay. Vui long quay lai sau neu ban da du dieu kien.", "Dong", "", JobInfo[i][jobLevel]);
                    }
                    else
                    {
                        new title[28];
                        format(title, _, "Cong viec > %s", JobInfo[i][jobName]);
                        ShowPlayerDialog(playerid, DIALOG_GETJOB, DIALOG_STYLE_MSGBOX, title, JobHelp[i], "Nhan viec", "Huy bo");
                        SetPVarInt(playerid, "GetJobID", i);
                    }
                    return 1;
                }
            }
        }
    }
    return 0;
}

stock GetJobName(jobId)
{
    new jobname[32];
    if(jobId < 0 || jobId >= MAX_JOB_TYPE) return jobname;

    strunpack(jobname, JobInfo[jobId][jobName], sizeof(jobname));
    return jobname;
}

stock GetSkillNextLevel(playerid, job)
{
	new jlevel;
	switch(job)
	{
		case JOB_TRUCKER:
		{
			new skilllevel = PlayerInfo[playerid][pTruckSkill];
			if(skilllevel <= 70) jlevel = 71 - skilllevel;
			else if(skilllevel <= 150) jlevel = 151 - skilllevel;
			else if(skilllevel <= 230) jlevel = 231 - skilllevel;
			else if(skilllevel <= 480) jlevel = 481 - skilllevel;
			else if(skilllevel >= 481) jlevel = 0;
		}
        case JOB_PORTER:
		{
			new skilllevel = PlayerInfo[playerid][pPorterSkill];
			if(skilllevel <= 90) jlevel = 91 - skilllevel;
			else if(skilllevel <= 200) jlevel = 201 - skilllevel;
			else if(skilllevel <= 300) jlevel = 301 - skilllevel;
			else if(skilllevel <= 500) jlevel = 501 - skilllevel;
			else if(skilllevel >= 501) jlevel = 0;
		}
        case JOB_PIZZABOY:
		{
			new skilllevel = PlayerInfo[playerid][pPizzaSkill];
			if(skilllevel <= 110) jlevel = 111 - skilllevel;
			else if(skilllevel <= 250) jlevel = 251 - skilllevel;
			else if(skilllevel <= 390) jlevel = 391 - skilllevel;
			else if(skilllevel <= 530) jlevel = 531 - skilllevel;
			else if(skilllevel >= 531) jlevel = 0;
		}
        case JOB_CONSTRUCTION:
		{
			new skilllevel = PlayerInfo[playerid][pConstructSkill];
			if(skilllevel <= 90) jlevel = 91 - skilllevel;
			else if(skilllevel <= 200) jlevel = 201 - skilllevel;
			else if(skilllevel <= 300) jlevel = 301 - skilllevel;
			else if(skilllevel <= 500) jlevel = 501 - skilllevel;
			else if(skilllevel >= 501) jlevel = 0;
		}
        case JOB_CRAFTER:
        {
            new skilllevel = PlayerInfo[playerid][pCraftSkill];
			if(skilllevel <= 40) jlevel = 41 - skilllevel;
			else if(skilllevel <= 110) jlevel = 111 - skilllevel;
			else if(skilllevel <= 180) jlevel = 181 - skilllevel;
			else if(skilllevel <= 250) jlevel = 251 - skilllevel;
			else if(skilllevel >= 251) jlevel = 0;
        }
	}
    return jlevel;
}

stock GetJobLevel(playerid, job)
{
	new jlevel;
	switch(job)
	{
		case JOB_TRUCKER:
		{
			new skilllevel = PlayerInfo[playerid][pTruckSkill];
			if(skilllevel >= 0 && skilllevel <= 70) jlevel = 1;
			else if(skilllevel >= 71 && skilllevel <= 150) jlevel = 2;
			else if(skilllevel >= 151 && skilllevel <= 230) jlevel = 3;
			else if(skilllevel >= 231 && skilllevel <= 480) jlevel = 4;
			else if(skilllevel >= 481) jlevel = 5;
		}
        case JOB_PORTER:
		{
			new skilllevel = PlayerInfo[playerid][pPorterSkill];
			if(skilllevel >= 0 && skilllevel <= 90) jlevel = 1;
			else if(skilllevel >= 91 && skilllevel <= 200) jlevel = 2;
			else if(skilllevel >= 201 && skilllevel <= 300) jlevel = 3;
			else if(skilllevel >= 301 && skilllevel <= 500) jlevel = 4;
			else if(skilllevel >= 501) jlevel = 5;
		}
        case JOB_PIZZABOY:
		{
			new skilllevel = PlayerInfo[playerid][pPizzaSkill];
			if(skilllevel >= 0 && skilllevel <= 110) jlevel = 1;
			else if(skilllevel >= 111 && skilllevel <= 250) jlevel = 2;
			else if(skilllevel >= 251 && skilllevel <= 390) jlevel = 3;
			else if(skilllevel >= 391 && skilllevel <= 530) jlevel = 4;
			else if(skilllevel >= 531) jlevel = 5;
		}
        case JOB_CONSTRUCTION:
		{
			new skilllevel = PlayerInfo[playerid][pConstructSkill];
			if(skilllevel >= 0 && skilllevel <= 90) jlevel = 1;
			else if(skilllevel >= 91 && skilllevel <= 200) jlevel = 2;
			else if(skilllevel >= 201 && skilllevel <= 300) jlevel = 3;
			else if(skilllevel >= 301 && skilllevel <= 500) jlevel = 4;
			else if(skilllevel >= 501) jlevel = 5;
		}
        case JOB_CRAFTER:
        {
            new skilllevel = PlayerInfo[playerid][pCraftSkill];
			if(skilllevel >= 0 && skilllevel <= 40) jlevel = 1;
			else if(skilllevel >= 41 && skilllevel <= 110) jlevel = 2;
			else if(skilllevel >= 111 && skilllevel <= 180) jlevel = 3;
			else if(skilllevel >= 181 && skilllevel <= 250) jlevel = 4;
			else if(skilllevel >= 251) jlevel = 5;
        }
	}
	return jlevel;
}

alias:quitjob("nghiviec", "thoatviec")
CMD:quitjob(playerid, params[])
{
	if(PlayerInfo[playerid][pDonateRank] >= 1 || PlayerInfo[playerid][pFamed] >= 1)
	{
		new jobid;
		if(sscanf(params, "d", jobid))
		{
			SendHelpMessage(playerid, "/nghiviec [jobid]");
			SendClientMessage(playerid, COLOR_GRAD2, "[1] Cong viec chinh: %s", GetJobName(PlayerInfo[playerid][pJob]));
		    SendClientMessage(playerid, COLOR_GRAD2, "[2] Cong viec phu: %s", GetJobName(PlayerInfo[playerid][pJob2]));
			return 1;
		}

		switch(jobid)
		{
		    case 1:
			{
                if(PlayerWorking[playerid] > 0 && PlayerInfo[playerid][pJob] == PlayerWorking[playerid])
                    return SendClientMessageEx(playerid, COLOR_GREY, "Ban dang lam viec, can dung lam viec truoc khi nghi viec.");

				SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* Ban da thoat cong viec chinh thanh cong.");
				PlayerInfo[playerid][pJob] = 0;
			}
		    case 2:
			{
                if(PlayerWorking[playerid] > 0 && PlayerInfo[playerid][pJob2] == PlayerWorking[playerid])
                    return SendClientMessageEx(playerid, COLOR_GREY, "Ban dang lam viec, can dung lam viec truoc khi nghi viec.");
				
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* Ban da thoat cong viec phu thanh cong.");
				PlayerInfo[playerid][pJob2] = 0;
			}
		    default:
			{
				SendHelpMessage(playerid, "/nghiviec [jobid]");
				SendClientMessageEx(playerid, COLOR_GREY, "Co san: 1, 2 (VIP/Famed)");
			}
		}
	}
	else
	{
		if(PlayerInfo[playerid][pJob] > 0 )
		{
            if(PlayerWorking[playerid] > 0 && PlayerInfo[playerid][pJob] == PlayerWorking[playerid])
                return SendClientMessageEx(playerid, COLOR_GREY, "Ban dang lam viec, can dung lam viec truoc khi nghi viec.");

			SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* Ban da thoat cong viec chinh thanh cong.");
			PlayerInfo[playerid][pJob] = 0;
		}
		else
		{
			SendClientMessageEx(playerid, COLOR_GREY, "   Ban khong co cong viec!");
		}
	}
	return 1;
}

CMD:lamviec(playerid, params[])
{
    if(PlayerInfo[playerid][pJob] == JOB_NONE && PlayerInfo[playerid][pJob2] == JOB_NONE)
        return SendClientMessage(playerid, COLOR_GREY, "Ban khong co cong viec nao de su dung lenh nay.");

    if(PlayerWorking[playerid])
        return SendClientMessage(playerid, COLOR_GREY, "Ban dang lam viec roi.");

    if(PlayerInfo[playerid][pJob] == JOB_PORTER || PlayerInfo[playerid][pJob2] == JOB_PORTER)
    {
        if(PlayerPorter[playerid] == 0)
        {
            PlayerWorking[playerid] = JOB_PORTER;
            SendClientMessage(playerid, 0xCEE276FF, "Ban da bat dau lam viec, go \"/dunglamviec\" neu muon nghi ngoi.");
            Porter_Start(playerid);
        }
        else {
            SendClientMessage(playerid, 0xCEE276FF, "Ban dang lam viec roi.");
            return 1;
        }
    }
    else if(PlayerInfo[playerid][pJob] == JOB_PIZZABOY || PlayerInfo[playerid][pJob2] == JOB_PIZZABOY)
    {
        if(PlayerPizzaBoy[playerid] == 0)
        {
            PlayerPizzaBoy[playerid] = 1;
            PlayerWorking[playerid] = JOB_PIZZABOY;
            SendClientMessage(playerid, 0xCEE276FF, "Ban da bat dau lam viec, go \"/dunglamviec\" neu muon nghi ngoi.");
            SetPlayerSkin(playerid, JobInfo[JOB_PIZZABOY][jobSkin]);
        }
        else {
            SendClientMessage(playerid, 0xCEE276FF, "Ban dang lam viec roi.");
            return 1;
        }
    }
    else if(PlayerInfo[playerid][pJob] == JOB_TAXIDRIVER || PlayerInfo[playerid][pJob2] == JOB_TAXIDRIVER)
    {
        if(PlayerWorking[playerid] == 0) {
            SendHintMessage(playerid, "Cong viec cua ban phai su dung lenh \"{46BE00}/fare{FFFFFF}\".");
        }
    }
    else if(PlayerInfo[playerid][pJob] == JOB_CONSTRUCTION || PlayerInfo[playerid][pJob2] == JOB_CONSTRUCTION)
    {
        if(GetPVarType(playerid, "ConstructionID") == PLAYER_VARTYPE_NONE)
        {
            PlayerWorking[playerid] = JOB_CONSTRUCTION;
            SendClientMessage(playerid, 0xCEE276FF, "Ban da bat dau lam viec, go \"/dunglamviec\" neu muon nghi ngoi.");
            SetPlayerSkin(playerid, JobInfo[JOB_CONSTRUCTION][jobSkin]);
            Construction_Start(playerid);
        }
        else {
            SendClientMessage(playerid, 0xCEE276FF, "Ban dang lam viec roi.");
            return 1;
        }
    }
    else {
        SendClientMessage(playerid, COLOR_GREY, "Cong viec cua ban khong the su dung lenh nay, go \"/jobhelp\" de biet them chi tiet.");
    }
    return 1;
}

CMD:dunglamviec(playerid, params[])
{
    if(PlayerInfo[playerid][pJob] == JOB_NONE && PlayerInfo[playerid][pJob2] == JOB_NONE)
        return SendClientMessage(playerid, COLOR_GREY, "Ban khong co cong viec nao de su dung lenh nay.");

    if(!PlayerWorking[playerid])
        return SendClientMessage(playerid, COLOR_GREY, "Ban chua bat dau lam viec.");

    if(PlayerInfo[playerid][pJob] == JOB_PORTER || PlayerInfo[playerid][pJob2] == JOB_PORTER)
    {
        if(PlayerPorter[playerid] == 1)
        {
            PlayerWorking[playerid] = 0;
            SendClientMessage(playerid, 0xCEE276FF, "Ban da dung lam viec, go \"/lamviec\" neu muon lam viec lai.");
            Porter_Stop(playerid);
        }
        else {
            SendClientMessage(playerid, 0xCEE276FF, "Ban chua bat dau lam viec, go \"/lamviec\" de bat dau.");
        }
    }
    else if(PlayerInfo[playerid][pJob] == JOB_PIZZABOY || PlayerInfo[playerid][pJob2] == JOB_PIZZABOY)
    {
        if(PlayerPizzaBoy[playerid] == 1)
        {
            DeletePVar(playerid, "Pizza");
            PlayerPizzaBoy[playerid] = 0;
            PlayerWorking[playerid] = 0;
            SendClientMessage(playerid, 0xCEE276FF, "Ban da dung lam viec, go \"/lamviec\" neu muon lam viec lai.");
            SetPlayerSkin(playerid, PlayerInfo[playerid][pModel]);
        }
        else {
            SendClientMessage(playerid, 0xCEE276FF, "Ban chua bat dau lam viec, go \"/lamviec\" de bat dau.");
        }
    }
    else if(PlayerInfo[playerid][pJob] == JOB_TAXIDRIVER || PlayerInfo[playerid][pJob2] == JOB_TAXIDRIVER)
    {
        if(PlayerWorking[playerid] == JOB_TAXIDRIVER) {
            PC_EmulateCommand(playerid, "/fare");
        }
    }
    else if(PlayerInfo[playerid][pJob] == JOB_CONSTRUCTION || PlayerInfo[playerid][pJob2] == JOB_CONSTRUCTION)
    {
        if(GetPVarType(playerid, "ConstructionID") == PLAYER_VARTYPE_INT)
        {
            Construction_Stop(playerid);
            PlayerWorking[playerid] = 0;
            SendClientMessage(playerid, 0xCEE276FF, "Ban da dung lam viec, go \"/lamviec\" neu muon lam viec lai.");
            SetPlayerSkin(playerid, PlayerInfo[playerid][pModel]);
        }
        else {
            SendClientMessage(playerid, 0xCEE276FF, "Ban chua bat dau lam viec, go \"/lamviec\" de bat dau.");
        }
    }
    else {
        SendClientMessage(playerid, COLOR_GREY, "Cong viec cua ban khong the su dung lenh nay, go \"/jobhelp\" de biet them chi tiet.");
    }
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_GETJOB)
	{
		if(!response) return 1;

		if(!IsPlayerDiscordVerified(playerid)) 
            return SendClientMessageEx(playerid, COLOR_GREY, "Ban phai lien ket tai khoan Discord de co the lam dieu nay.");
			
		if(PlayerInfo[playerid][pIDCard] == 0)
			return SendClientMessageEx(playerid, COLOR_GREY, "Ban can phai co Can cuoc cong dan de xin viec.");

		new jobId = GetPVarInt(playerid, "GetJobID");

		if(PlayerInfo[playerid][pTaxiLicense] == 0 && jobId == JOB_TAXIDRIVER)
			return SendClientMessageEx(playerid, COLOR_GREY, "Cong viec nay yeu cau co giay phep lai Taxi.");

		if(PlayerInfo[playerid][pJob] == 0)
		{
			PlayerInfo[playerid][pJob] = jobId;
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "Ban da xin cong viec chinh thanh cong, go \"/jobhelp\" de xem tro giup ve cong viec.");
		}
		else
		{
			if(PlayerInfo[playerid][pJob2] != 0)
				return SendClientMessage(playerid, COLOR_GREY, "Ban can phai nghi viec cong viec chinh hoac phu truoc.");

			if(PlayerInfo[playerid][pDonateRank] < 1 && PlayerInfo[playerid][pLevel] < 10)
				return SendClientMessage(playerid, COLOR_LIGHTRED, "Ban khong du dieu kien de nhan cong viec phu.");

			PlayerInfo[playerid][pJob2] = jobId;
			SendClientMessage(playerid, COLOR_LIGHTBLUE, "Ban da xin cong viec phu thanh cong, go \"/jobhelp\" de xem tro giup ve cong viec.");
		}

		DeletePVar(playerid, "GetJobID");
	}
    return 1;
}