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

static const Float:DrivingCheckpoint[19][3] = {
    {-1570.033813, 661.503051, 7.187500},
    {-1575.252929, 732.588867, 7.206319},
    {-1713.913818, 688.007141, 24.742170},
    {-1920.399414, 605.449645, 35.015625},
    {-2027.866943, 565.938720, 35.015625},
    {-2161.993652, 565.687011, 35.015625},
    {-2238.416503, 507.969268, 35.015625},
    {-2379.279541, 521.041137, 28.041351},
    {-2543.584960, 565.939025, 14.460937},
    {-2751.185058, 608.981384, 24.017543},
    {-2750.653808, 891.841552, 66.187164},
    {-2711.916015, 1099.200683, 45.647693},
    {-2453.592041, 1226.969970, 35.050312},
    {-2432.239257, 1373.091186, 7.039062},
    {-2024.658691, 1284.951049, 7.039062},
    {-1708.303588, 1286.323974, 7.046842},
    {-1610.385009, 1165.744018, 7.039062},
    {-1562.586059, 892.366882, 7.039062},
    {-1568.925659, 662.053100, 7.187500}
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
    "B. Dung xe va di bo", "C. Tang toc den tram sua xe", "D. Di chuyen xe vao le duong va goi cuu ho", 3}
};

new const Float:CarExamSpawn[2][4] =
{
    {-1576.632080, 673.534301, 6.844708, 169.846954},
    {-1582.242553, 673.802856, 6.844011, 169.846954}
};

static PlayerExamList[MAX_PLAYERS][MAX_CAR_EXAM];
static PlayerExamCorrect[MAX_PLAYERS];
static PlayerExamCurrent[MAX_PLAYERS];
static PlayerExamStep[MAX_PLAYERS];
static PlayerExamCheckpoint[MAX_PLAYERS];
static PlayerExamCar[MAX_PLAYERS];
static Text3D:PlayerExamCarLabel[MAX_PLAYERS];

// type 0: car license
// type 1: taxi license
stock ShowLicenseDialog(playerid, type)
{
    if(type == 0)
    {
        ShowPlayerDialog(playerid, DIALOG_LICENSE_CAR, DIALOG_STYLE_MSGBOX, "{FFFFFF}Sep Liem", "{FFFFFF}Xin chao ban, toi ten {6D9BC0}Liem{FFFFFF} la nhan vien cua Trung tam cap giay phep.\n\
                Toi la nguoi co quyen han quyet dinh viec cap giay phep lai xe cho ban, nhung\nde co the nhan giay phep lai xe ban can phai thuc hien viec thi {68FA47}ly thuyet {FFFFFF}va{68FA47} thuc\nhanh{FFFFFF} de chung minh ban du dieu kien de dieu khien phuong tien giao thong.\n\
                    Ban se tieu ton {F04040}$10,000{FFFFFF} cho viec nay.\n\n\n{7D78FF}Ban co muon thuc hien khong?", "Co", "Khong");
    }
    else if(type == 1)
    {
        ShowPlayerDialog(playerid, DIALOG_LICENSE_TAXI, DIALOG_STYLE_MSGBOX, "{FFFFFF}James Carter", "{FFFFFF}Xin chao ban, toi ten {6D9BC0}James Carter{FFFFFF} la nhan vien cua Trung tam cap giay phep.\n\
                Toi la nguoi co tham quyen quyet dinh cap giay phep Taxi, de co the lay duoc\nloai giay phep nay, truoc tien ban can co mot giay phep lai xe truoc va phi ban\n\
                    can phai tra de lay giay phep Taxi la {F04040}$35,000{FFFFFF}.\n\n\n{7D78FF}Ban co muon lay giay phep nay khong?", "Co", "Khong");
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
    PlayerExamCarLabel[playerid] = Text3D:INVALID_3DTEXT_ID;
    for(new i; i < MAX_CAR_EXAM; i++)
    {
        PlayerExamList[playerid][i] = 0;
    }
    return 1;
}

hook OnPlayerDisconnect(playerid)
{
    if(PlayerExamCarLabel[playerid] != Text3D:INVALID_3DTEXT_ID) {
        Delete3DTextLabel(PlayerExamCarLabel[playerid]);
        PlayerExamCarLabel[playerid] = Text3D:INVALID_3DTEXT_ID;
    }
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
    
        if(GetPlayerCash(playerid) < 35000)
            return SendClientMessage(playerid, COLOR_GREY, "Ban khong du tien de lay giay phep Taxi.");

        GivePlayerCash(playerid, -35000);
        SendClientMessage(playerid, COLOR_WHITE, "Ban da tra {F04E4E}$35,000{FFFFFF} de lay giay phep Taxi.");
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

        if(GetPlayerCash(playerid) < 10000)
            return SendClientMessage(playerid, COLOR_GREY, "Ban khong du tien de tra phi thi giay phep lai xe.");

        GivePlayerCash(playerid, -10000);
        SendClientMessage(playerid, COLOR_WHITE, "Ban da tra {F04E4E}$10,000{FFFFFF} de thi giay phep lai xe.");

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
        SetVehicleParamsEx(PlayerExamCar[playerid], VEHICLE_PARAMS_OFF, VEHICLE_PARAMS_OFF, VEHICLE_PARAMS_OFF, VEHICLE_PARAMS_OFF, VEHICLE_PARAMS_OFF, VEHICLE_PARAMS_OFF, VEHICLE_PARAMS_OFF);
        VehicleFuel[PlayerExamCar[playerid]] = GetVehicleFuelCapacity(PlayerExamCar[playerid]);

        new labelStr[128];
        format(labelStr, sizeof(labelStr), "{FFF34F}Thi Bang Lai Xe\n{FFFFFF}Thi sinh: {FFF34F}%s", GetPlayerNameEx(playerid));
        PlayerExamCarLabel[playerid] = Create3DTextLabel(labelStr, 0xFFF34FFF, 0.0, 0.0, 0.5, 12.0, 0);
        Attach3DTextLabelToVehicle(PlayerExamCarLabel[playerid], PlayerExamCar[playerid], 0.0, 0.0, 0.5);

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

            if(PlayerExamCheckpoint[playerid] == 19) {
                if(PlayerExamCarLabel[playerid] != Text3D:INVALID_3DTEXT_ID) {
                    Delete3DTextLabel(PlayerExamCarLabel[playerid]);
                    PlayerExamCarLabel[playerid] = Text3D:INVALID_3DTEXT_ID;
                }
                DestroyVehicle(PlayerExamCar[playerid]);
                PlayerExamCar[playerid] = 0;
                PlayerExamStep[playerid] = 0;
                PlayerExamCorrect[playerid] = 0;
                PlayerExamCurrent[playerid] = 0;
                PlayerExamCheckpoint[playerid] = 0;
                PlayerExamStep[playerid] = 0;
                DisablePlayerRaceCP(playerid);
                HienTextdraw(playerid, "~y~CHUC MUNG~n~DA THI DAUGIAY PHEP LAI XE");
                SendClientMessage(playerid, COLOR_WHITE, "{FFF34F}CHUC MUNG!{FFFFFF} Ban da hoan thanh phan thi thuc hanh xuat sac va nhan duoc giay phep lai xe!");
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
            if(PlayerExamCarLabel[playerid] != Text3D:INVALID_3DTEXT_ID) {
                Delete3DTextLabel(PlayerExamCarLabel[playerid]);
                PlayerExamCarLabel[playerid] = Text3D:INVALID_3DTEXT_ID;
            }
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
        if(PlayerExamCarLabel[playerid] != Text3D:INVALID_3DTEXT_ID) {
            Delete3DTextLabel(PlayerExamCarLabel[playerid]);
            PlayerExamCarLabel[playerid] = Text3D:INVALID_3DTEXT_ID;
        }
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