#include <YSI\YSI_Coding\y_hooks>

const MAX_CAR_EXAM = 8;

enum E_CAR_EXAM
{
    cexam_Question[128],
    cexam_Answer1[64],
    cexam_Answer2[64],
    cexam_Answer3[64],
    cexam_Answer4[64],
    cexam_CorrectAnswer
};

static const Float:DrivingCheckpoint[17][3] = {
    {-1990.1835, 543.6481, 35.0156},
    {-2008.3058, 472.5847, 35.0156},
    {-2009.8352, 216.1990, 27.5391},
    {-2009.6008, -50.5634, 35.1650},
    {-2146.3672, -68.1673, 35.1719},
    {-2338.1194, -68.0654, 35.1641},
    {-2413.2891, -67.5774, 35.2106},
    {-2418.6011, 77.2842, 35.0156},
    {-2388.4937, 331.9492, 35.0156},
    {-2316.9705, 431.3434, 34.9070},
    {-2382.8403, 552.1824, 24.9008},
    {-2383.8459, 654.8113, 35.0156},
    {-2384.5872, 804.9805, 35.0284},
    {-2219.6182, 806.6862, 49.2980},
    {-2007.7246, 805.2345, 45.4473},
    {-2007.4810, 606.5001, 35.0156},
    {-1941.6078, 576.7798, 35.1719}
};

static const DrivingTheory[MAX_CAR_EXAM][E_CAR_EXAM] =
{
    {"Khi vuot xe khac tren duong, ban can lam gi truoc tien?",
    "A. Bam coi lien tuc de gianh quyen vuot", "B. Tang toc bat ngo de vuot nhanh",
    "C. Quan sat ky va dam bao khong co chuong ngai vat phia truoc", "D. Khong quan tam", 2},

    {"Tai nga tu khong co den giao thong, xe nao duoc uu tien di truoc?",
    "A. Xe re trai", "B. Xe re phai", "C. Xe di thang", "D. Xe co trong tai lon", 1},

    {"Khi lai xe duong tron truot, ban can lam gi?", "A. Tang toc de khong bi truot banh",
    "B. Giam toc do va giu khoang cach an toan", "C. Dung phanh gap khi dung xe",
    "D. Chi su dung den pha de de quan sat", 1},

    {"Khi dang lai xe ma ban bi canh sat yeu cau dung lai, ban can lam gi?",
    "A. Tang toc va bo tron", "B. Dung xe va chap hanh", "C. Khong quan tam", 
    "D. Doi xe khac de tranh bi truy duoi", 1},

    {"Khi di xe qua khu vuc dong nguoi, hanh dong nao la dung dan?",
    "A. Tang toc de nhanh chong roi khoi khu vuc", "B. Bam coi lien tuc de gianh quyen di",
    "C. Giam toc do va chu y quan sat", "D. Chay xe bua bai gay nao loan", 2},

    {"Khi gap nguoi di bo qua duong, ban can lam gi?", "A. Tang toc de nhanh chong vuot qua",
    "B. Tiep tuc chay ma khong quan tam", "C. Dung xe va nhuong duong cho nguoi di bo",
    "D. Bam coi lien tuc de ho tranh duong", 2},

    {"Khi dieu khien phuong tien vao troi toi, ban can lam gi?", "A. Tang toc de nhanh chong den dich",
    "B. Giam toc do va su dung den pha", "C. Khong quan tam den an toan giao thong",
    "D. Dung xe va nghi ngoi", 1},

    {"Khi dang di chuyen va xe bi hu hong, ban can lam gi?", "A. Tiep tuc chay ma khong quan tam",
    "B. Dung xe va di bo", "C. Tang toc den cham sua xe", "D. Di chuyen xe vao le duong va goi cuu ho", 3}
};

new const Float:CarExamSpawn[2][4] =
{
    {-1927.8352, 574.0799, 34.8263, 88.8340},
    {-1927.8687, 579.1791, 34.8243, 88.8340}
};

static PlayerExamList[MAX_PLAYERS][MAX_CAR_EXAM];
static PlayerExamCorrect[MAX_PLAYERS];
static PlayerExamCurrent[MAX_PLAYERS];
static PlayerExamStep[MAX_PLAYERS];
static PlayerExamCheckpoint[MAX_PLAYERS];
static PlayerExamCar[MAX_PLAYERS];

// type 0: car license
// type 1: taxi license
stock ShowLicenseDialog(playerid, type)
{
    if(type == 0)
    {
        ShowPlayerDialog(playerid, DIALOG_LICENSE_CAR, DIALOG_STYLE_MSGBOX, "{FFFFFF}William Brown", "{FFFFFF}Xin chao ban, toi ten {6D9BC0}William Brown{FFFFFF} la nhan vien cua Trung tam cap giay phep.\n\
                Toi la nguoi co quyen han quyet dinh viec cap giay phep lai xe cho ban, nhung\nde co the nhan giay phep lai xe ban can phai thuc hien viec thi {68FA47}ly thuyet {FFFFFF}va{68FA47} thuc\nhanh{FFFFFF} de chung minh ban du dieu kien de dieu khien phuong tien giao thong.\n\
                    Ban se tieu ton {F04040}$700{FFFFFF} cho viec nay.\n\n\n{7D78FF}Ban co muon thuc hien khong?", "Co", "Khong");
    }
    else if(type == 1)
    {
        ShowPlayerDialog(playerid, DIALOG_LICENSE_TAXI, DIALOG_STYLE_MSGBOX, "{FFFFFF}James Carter", "{FFFFFF}Xin chao ban, toi ten {6D9BC0}James Carter{FFFFFF} la nhan vien cua Trung tam cap giay phep.\n\
                Toi la nguoi co tham quyen quyet dinh cap giay phep Taxi, de co the lay duoc\nloai giay phep nay, truoc tien ban can co mot giay phep lai xe truoc va phi ban\n\
                    can phai tra de lay giay phep Taxi la {F04040}$1,000{FFFFFF}.\n\n\n{7D78FF}Ban co muon lay giay phep nay khong?", "Co", "Khong");
    }
    return 1;
}

stock ShowCarExam(playerid)
{
    new curExam = PlayerExamList[playerid][PlayerExamCurrent[playerid]];
    new dialogTitle[128], dialogInfo[428];

    strcat(dialogTitle, DrivingTheory[curExam][cexam_Question]);
    format(dialogInfo, _, "{FFFFFF}%s\n%s\n%s\n%s\n{E7EC5A}___________________________________________________________", DrivingTheory[curExam][cexam_Answer1], DrivingTheory[curExam][cexam_Answer2], DrivingTheory[curExam][cexam_Answer3], DrivingTheory[curExam][cexam_Answer4]);
    ShowPlayerDialog(playerid, DIALOG_EXAM_CAR, DIALOG_STYLE_LIST, dialogTitle, dialogInfo, "Chon", " ");
    return 1;
}

stock SetPlayerExamCheckpoint(playerid)
{
    new routeId = PlayerExamCheckpoint[playerid]-1;
    if(routeId+1 < sizeof(DrivingCheckpoint))
    {
        SetPlayerRaceCP(playerid, CP_TYPE_GROUND_NORMAL, DrivingCheckpoint[routeId][0], DrivingCheckpoint[routeId][1], DrivingCheckpoint[routeId][2], DrivingCheckpoint[routeId+1][0], DrivingCheckpoint[routeId+1][1], DrivingCheckpoint[routeId+1][2], 4.0);
    }
    else
    {
        SetPlayerRaceCP(playerid, CP_TYPE_GROUND_FINISH, DrivingCheckpoint[routeId][0], DrivingCheckpoint[routeId][1], DrivingCheckpoint[routeId][2], DrivingCheckpoint[routeId][0], DrivingCheckpoint[routeId][1], DrivingCheckpoint[routeId][2], 4.0);
    }
    return 1;
}

hook OnPlayerConnect(playerid)
{
    PlayerExamStep[playerid] = 0;
    PlayerExamCorrect[playerid] = 0;
    PlayerExamCurrent[playerid] = 0;
    PlayerExamCheckpoint[playerid] = 0;
    for(new i; i < MAX_CAR_EXAM; i++)
    {
        PlayerExamList[playerid][i] = 0;
    }
    return 1;
}

hook OnPlayerDisconnect(playerid)
{
    if(IsValidVehicle(PlayerExamCar[playerid]))
    {
        DestroyVehicle(PlayerExamCar[playerid]);
        PlayerExamCar[playerid] = 0;
    }
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_LICENSE_TAXI && response)
    {
        if(PlayerInfo[playerid][pTaxiLicense] == 1)
            return SendClientMessage(playerid, COLOR_GREY, "Ban da co giay phep Taxi roi.");

        if(PlayerInfo[playerid][pCarLic] == 0)
            return SendClientMessage(playerid, COLOR_GREY, "Ban can phai co giay phep lai xe truoc khi co the lay giay phep Taxi.");
    
        if(GetPlayerCash(playerid) < 1000)
            return SendClientMessage(playerid, COLOR_GREY, "Ban khong du tien de lay giay phep Taxi.");

        GivePlayerCash(playerid, -1000);
        SendClientMessage(playerid, COLOR_WHITE, "Ban da tra {F04E4E}$1,000{FFFFFF} de lay giay phep Taxi.");
        PlayerInfo[playerid][pTaxiLicense] = 1;
        UpdatePlayerInteger(playerid, "TaxiLicense", 1);
        return 1;
    }
    
    if(dialogid == DIALOG_LICENSE_CAR && response)
    {
        if(PlayerInfo[playerid][pIDCard] == 0)
            return SendClientMessage(playerid, COLOR_GREY, "Ban can phai co Can cuoc cong dan de lam dieu nay.");

        if(PlayerExamStep[playerid] == 1) 
            return SendClientMessage(playerid, COLOR_GREY, "Ban dang co mot bai kiem tra ly thuyet roi.");
        
        if(PlayerExamStep[playerid] == 2)
            return SendClientMessage(playerid, COLOR_GREY, "Ban da vuot qua ly thuyet, ban can phai thi thuc hanh.");

        if(PlayerInfo[playerid][pCarLic] != 0)
            return SendClientMessage(playerid, COLOR_GREY, "Ban da co giay phep lai xe roi.");

        if(gPlayerCheckpointStatus[playerid] != CHECKPOINT_NONE || IsPlayerCheckpointActive(playerid))
            return SendClientMessage(playerid, COLOR_GREY, "Ban dang co mot diem danh dau tren mini-map, hay xoa no.");

        if(GetPlayerCash(playerid) < 700)
            return SendClientMessage(playerid, COLOR_GREY, "Ban khong du tien de tra phi thi giay phep lai xe.");
            
        GivePlayerCash(playerid, -700);
        SendClientMessage(playerid, COLOR_WHITE, "Ban da tra {F04E4E}$700{FFFFFF} de thi giay phep lai xe.");

        PlayerExamStep[playerid] = 1;
        PlayerExamCorrect[playerid] = 0;
        PlayerExamCurrent[playerid] = 0;
        PlayerExamCheckpoint[playerid] = 0;

        for(new i; i < MAX_CAR_EXAM; i++)
        {
            PlayerExamList[playerid][i] = i;
        }

        for (new i = MAX_CAR_EXAM - 1; i > 0; i--)
        {
            new j = random(i + 1);
            new temp = PlayerExamList[playerid][i];
            PlayerExamList[playerid][i] = PlayerExamList[playerid][j];
            PlayerExamList[playerid][j] = temp;
        }

        ShowCarExam(playerid);
        return 1;
    }
 
    if(dialogid == DIALOG_EXAM_CAR)
    {
        if(!response) {
            PlayerExamStep[playerid] = 0;
            PlayerExamCorrect[playerid] = 0;
            PlayerExamCurrent[playerid] = 0;
            for(new i; i < MAX_CAR_EXAM; i++)
            {
                PlayerExamList[playerid][i] = 0;
            }
            SendClientMessage(playerid, COLOR_GREY, "Ban da that bai trong viec thi ly thuyet.");
            return 1;
        }

        if(listitem < 0 || listitem > 3) return ShowCarExam(playerid);

        new curExam = PlayerExamList[playerid][PlayerExamCurrent[playerid]];
        if(listitem == DrivingTheory[curExam][cexam_CorrectAnswer])
            PlayerExamCorrect[playerid]++;

        PlayerExamCurrent[playerid]++;
        if(PlayerExamCurrent[playerid] >= MAX_CAR_EXAM)
        {
            if(PlayerExamCorrect[playerid] >= MAX_CAR_EXAM-1)
            {
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_CAREXAM;
                PlayerExamStep[playerid] = 2;
                SetPlayerCP(playerid, CarExamSpawn[0][0], CarExamSpawn[0][1], CarExamSpawn[0][2], 5.0);
                SendClientMessage(playerid, COLOR_WHITE, "{FFF34F}CHUC MUNG!{FFFFFF} Ban da thi dau ly thuyet voi so diem {FFF34F}%d/%d{FFFFFF}.", PlayerExamCorrect[playerid], MAX_CAR_EXAM);
                SendClientMessage(playerid, COLOR_LIGHTBLUE, "Hay di ra ben ngoai de bat dau phan thi thuc hanh lai xe!");
            }
            else
            {
                PlayerExamStep[playerid] = 0;
                SendClientMessage(playerid, COLOR_WHITE, "{E03F3F}RAT TIEC!{FFFFFF} Ban da that bai trong viec thi ly thuyet voi so diem {E03F3F}%d/%d{FFFFFF}.", PlayerExamCorrect[playerid], MAX_CAR_EXAM);
            }

            PlayerExamCorrect[playerid] = 0;
            PlayerExamCurrent[playerid] = 0;
            for(new i; i < MAX_CAR_EXAM; i++)
            {
                PlayerExamList[playerid][i] = 0;
            }
            return 1;
        }
        ShowCarExam(playerid);
        return 1;
    }
    return 1;
}

hook OnPlayerEnterCheckpoint(playerid)
{
    if(gPlayerCheckpointStatus[playerid] == CHECKPOINT_CAREXAM && PlayerExamStep[playerid] == 2)
    {
        new rCar = random(2);
        gPlayerCheckpointStatus[playerid] = CHECKPOINT_NONE;
        DisablePlayerCP(playerid);
        SendClientMessage(playerid, COLOR_WHITE, "Duoc roi, chuc ban may man trong phan thi thuc hanh nay!");
        PlayerExamCar[playerid] = CreateVehicle(410, CarExamSpawn[rCar][0], CarExamSpawn[rCar][1], CarExamSpawn[rCar][2], CarExamSpawn[rCar][3], 1, 1, 120);
        SetVehicleNumberPlate(PlayerExamCar[playerid], "CLA %03d", playerid);
        PutPlayerInVehicle(playerid, PlayerExamCar[playerid], 0);
        PlayerExamCheckpoint[playerid] = 1;
        SetPlayerExamCheckpoint(playerid);
    }
    return 1;
}

hook OnPlayerEnterRaceCheckpoint(playerid)
{
    if(PlayerExamStep[playerid] == 2 && PlayerExamCheckpoint[playerid] > 0)
    {
        if(IsPlayerInAnyVehicle(playerid)) {
            if(GetPlayerVehicleID(playerid) != PlayerExamCar[playerid])
                return SendClientMessageEx(playerid, COLOR_GREY, "Ban phai ngoi tren chiec xe ma ban da duoc cap de thi lai xe.");

            if(PlayerExamCheckpoint[playerid] == 17) {
                DestroyVehicle(PlayerExamCar[playerid]);
                PlayerExamCar[playerid] = 0;
                PlayerExamStep[playerid] = 0;
                PlayerExamCorrect[playerid] = 0;
                PlayerExamCurrent[playerid] = 0;
                PlayerExamCheckpoint[playerid] = 0;
                PlayerExamStep[playerid] = 0;
                DisablePlayerRaceCP(playerid);
                GameTextForPlayer(playerid, "~y~PASS THE~n~DRIVING TEST", 5000, 0);
                SendClientMessage(playerid, COLOR_WHITE, "{FFF34F}CHUC MUNG!{FFFFFF} Ban da hoan thanh phan thi thuc hanh xuat sac!");
                PlayerPlaySound(playerid, 31205);
                PlayerInfo[playerid][pCarLic] = 1;
                UpdatePlayerInteger(playerid, "CarLic", 1);
                UpdateNewbieMission(playerid, 1);
                return 1;
            }

            PlayerExamCheckpoint[playerid]++;
            SetPlayerExamCheckpoint(playerid);
            PlayerPlaySound(playerid, 1052);
        }
    }
    return 1;
}

hook OnVehicleDeath(vehicleid)
{
    foreach(new playerid: Player)
    {
        if(vehicleid == PlayerExamCar[playerid])
        {
            DestroyVehicle(PlayerExamCar[playerid]);
            PlayerExamCar[playerid] = 0;
            PlayerExamStep[playerid] = 0;
            PlayerExamCorrect[playerid] = 0;
            PlayerExamCurrent[playerid] = 0;
            PlayerExamCheckpoint[playerid] = 0;
            PlayerExamStep[playerid] = 0;
            DisablePlayerRaceCP(playerid);
            SendClientMessage(playerid, COLOR_WHITE, "{E03F3F}RAT TIEC!{FFFFFF} Ban da that bai trong viec thi thuc hanh lai xe.");
            return 1;
        }
    }
    return 1;
}

hook OnPlayerDeath(playerid, killerid, reason)
{
    if(PlayerExamStep[playerid] > 0 && PlayerExamCheckpoint[playerid] > 0)
    {
        if(IsValidVehicle(PlayerExamCar[playerid])) 
            DestroyVehicle(PlayerExamCar[playerid]);

        PlayerExamCar[playerid] = 0;
        PlayerExamStep[playerid] = 0;
        PlayerExamCorrect[playerid] = 0;
        PlayerExamCurrent[playerid] = 0;
        PlayerExamCheckpoint[playerid] = 0;
        PlayerExamStep[playerid] = 0;
        DisablePlayerRaceCP(playerid);
        SendClientMessage(playerid, COLOR_WHITE, "{E03F3F}RAT TIEC!{FFFFFF} Ban da that bai trong viec thi thuc hanh lai xe.");
    }
    return 1;
}

hook OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
    if(!ispassenger)
    {
        new vehPlate[64];
        GetVehicleNumberPlate(vehicleid, vehPlate, sizeof(vehPlate));
        if(strfind(vehPlate, "CLA", true) != -1)
        {
            if(PlayerExamCar[playerid] != vehicleid)
            {
                new Float:slx, Float:sly, Float:slz;
				GetPlayerPos(playerid, slx, sly, slz);
				SetPlayerPos(playerid, slx, sly, slz+1.3);
				PlayerPlaySound(playerid, 1130, slx, sly, slz+1.3);
			    RemovePlayerFromVehicle(playerid);
			    defer NOPCheck(playerid);
                SendClientMessage(playerid, COLOR_GREY, "Phuong tien nay thuoc so huu cua CLA, ban khong the dieu khien!");
                return 1;
            }
        }
    }
    return 1;
}