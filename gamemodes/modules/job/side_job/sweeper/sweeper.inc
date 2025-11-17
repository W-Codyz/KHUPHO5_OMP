#include <YSI\YSI_Coding\y_hooks>

#define SWEEPER_ROUTE_A 0
#define SWEEPER_ROUTE_B 1
#define SWEEPER_ROUTE_C 2

new Float:SweeperRouteA[20][3] = {
    {-1648.5590, 409.4576, 7.0313},
    {-1713.6903, 344.8164, 7.0313},
    {-1790.9781, 270.3780, 12.7863},
    {-1812.7262, 176.8745, 14.9609},
    {-1803.5540, 11.0145, 14.9609},
    {-1801.7496, -117.4557, 5.5065},
    {-1801.7560, -253.3311, 18.8995},
    {-1824.9771, -464.3425, 14.9609},
    {-1867.9905, -575.4459, 23.7957},
    {-1971.9730, -577.4283, 25.5671},
    {-2108.3359, -536.6776, 33.9454},
    {-2252.4436, -334.8280, 50.8672},
    {-2250.9077, -161.6850, 35.1719},
    {-2250.4082, 56.3145, 35.1797},
    {-2249.1448, 299.6953, 35.1641},
    {-2109.0188, 318.1260, 35.0156},
    {-1986.2338, 342.3407, 34.8243},
    {-1864.5197, 407.4524, 17.0078},
    {-1810.2549, 365.4421, 17.0156},
    {-1649.1750, 398.5131, 7.0313}
};

new Float:SweeperRouteB[21][3] = {
    {-1615.7169, 432.3584, 7.0313},
    {-1555.5553, 540.9693, 7.0313},
    {-1555.5076, 701.8250, 7.0391},
    {-1543.5144, 797.0373, 7.0391},
    {-1547.5730, 971.3673, 7.0391},
    {-1589.1042, 1130.0862, 7.0391},
    {-1675.7891, 1280.9482, 7.0391},
    {-1768.0303, 1365.4402, 7.0391},
    {-1934.4010, 1304.1180, 7.0391},
    {-2079.4485, 1278.8931, 11.8541},
    {-2236.2373, 1276.7675, 39.5333},
    {-2227.8176, 1173.4200, 55.5781},
    {-2047.7942, 1173.0751, 45.2969},
    {-1901.9127, 1171.4579, 45.2969},
    {-1797.8623, 1183.6255, 24.9766},
    {-1634.8492, 1182.1615, 7.1107},
    {-1607.5205, 1119.9386, 7.0391},
    {-1584.7802, 980.2532, 7.0391},
    {-1562.7550, 881.1519, 7.0391},
    {-1562.9272, 703.2056, 7.0391},
    {-1567.2583, 505.5810, 7.0313}
};

new Float:SweeperRouteC[21][3] = {
    {-1663.7869, 394.4616, 7.0313},
    {-1762.1444, 328.2949, 7.6999},
    {-1838.0850, 433.6278, 17.0480},
    {-1847.7417, 533.6640, 33.8586},
    {-1894.7029, 586.8787, 34.9360},
    {-1980.8997, 610.0426, 35.0234},
    {-2122.5366, 569.0649, 35.0156},
    {-2148.6895, 387.8988, 35.1719},
    {-2149.2620, 228.1281, 35.1719},
    {-2158.4617, 111.2250, 35.1719},
    {-2009.4979, 98.4445, 27.5314},
    {-2009.0559, -47.4374, 35.1719},
    {-2008.3217, -172.3845, 35.7109},
    {-1945.0963, -242.2948, 25.5625},
    {-1835.0265, -238.9423, 18.2266},
    {-1796.4088, -204.2495, 15.8599},
    {-1796.7770, -127.2251, 5.5473},
    {-1796.5428, -37.1220, 15.1250},
    {-1802.3478, 76.0816, 14.9609},
    {-1767.8997, 282.6899, 9.1791},
    {-1651.8574, 395.0475, 7.0313}
};

new PlayerGarbageObject[MAX_PLAYERS];
new PlayerSweeperRoute[MAX_PLAYERS] = {-1, ...};
new PlayerSweeperStep[MAX_PLAYERS];

stock IsVehicleSweeper(vehicleid)
{
    if(!IsValidVehicle(vehicleid)) return 0;

    for(new i=0; i < sizeof(SweeperVehicles); i++) {
        if(vehicleid == SweeperVehicles[i]) {
            return 1;
        }
    }
    return 0;
}

stock ResetSweeperVehicle(vehicleid)
{
    if(!IsVehicleSweeper(vehicleid)) return 0;

    SetVehicleToRespawn(vehicleid);
    return 1;
}

stock Float:GetDistanceRoute(route) 
{
    new Float:distance = 0;
    switch(route) {
        case SWEEPER_ROUTE_A: {
            for(new i=0; i < sizeof(SweeperRouteA) - 1; i++) {
                distance += GetDistanceBetweenPoints(SweeperRouteA[i][0], SweeperRouteA[i][1], SweeperRouteA[i][2], SweeperRouteA[i+1][0], SweeperRouteA[i+1][1], SweeperRouteA[i+1][2]);
            }
        }
        case SWEEPER_ROUTE_B: {
            for(new i=0; i < sizeof(SweeperRouteB) - 1; i++) {
                distance += GetDistanceBetweenPoints(SweeperRouteB[i][0], SweeperRouteB[i][1], SweeperRouteB[i][2], SweeperRouteB[i+1][0], SweeperRouteB[i+1][1], SweeperRouteB[i+1][2]);
            }
        }
        case SWEEPER_ROUTE_C: {
            for(new i=0; i < sizeof(SweeperRouteC) - 1; i++) {
                distance += GetDistanceBetweenPoints(SweeperRouteC[i][0], SweeperRouteC[i][1], SweeperRouteC[i][2], SweeperRouteC[i+1][0], SweeperRouteC[i+1][1], SweeperRouteC[i+1][2]);
            }
        }
    }
    return distance;
}

stock CreateGarbageObject(playerid, Float:x, Float:y, Float:z)
{
    if(IsValidDynamicObject(PlayerGarbageObject[playerid])) {
        DestroyDynamicObject(PlayerGarbageObject[playerid]);
        PlayerGarbageObject[playerid] = INVALID_OBJECT_ID;
    }

    new rand = random(4);
    switch(rand) {
        case 0: {
            PlayerGarbageObject[playerid] = CreateDynamicObject(2670, x, y, z-0.909985, 0.0, 0.0, 0.0, 0, 0);
        }
        case 1: {
            PlayerGarbageObject[playerid] = CreateDynamicObject(2671, x, y, z-0.999988, 0.0, 0.0, 0.0, 0, 0);
        }
        case 2: {
            PlayerGarbageObject[playerid] = CreateDynamicObject(2673, x, y, z-0.879986, 0.0, 0.0, 0.0, 0, 0);
        }
        case 3: {
            PlayerGarbageObject[playerid] = CreateDynamicObject(2674, x, y, z-0.879986, 0.0, 0.0, 0.0, 0, 0);
        }
    }
    return 1;
}

hook OnPlayerDisconnect(playerid, reason) 
{
    if(IsValidDynamicObject(PlayerGarbageObject[playerid])) {
        DestroyDynamicObject(PlayerGarbageObject[playerid]);
        PlayerGarbageObject[playerid] = INVALID_OBJECT_ID;
    }

    if(IsPlayerInAnyVehicle(playerid)) {
        new vehicleid = GetPlayerVehicleID(playerid);
        if(IsVehicleSweeper(vehicleid)) {
            ResetSweeperVehicle(vehicleid);
        }
    }

    PlayerSweeperRoute[playerid] = -1;
    PlayerSweeperStep[playerid] = 0;
    return 1;
}

hook OnPlayerStateChange(playerid, newstate, oldstate) 
{
    if(newstate == PLAYER_STATE_DRIVER && oldstate == PLAYER_STATE_ONFOOT) {
        new vehicleid = GetPlayerVehicleID(playerid);
        if(IsVehicleSweeper(vehicleid)) {
            ShowPlayerDialog(playerid, DIALOG_SWEEPER, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Chon tuyen duong quet don", "Tuyen duong\tQuang duong\n\
                {FFFFFF}Tuyen duong {D5D54A}A\t{566E8E}%0.1f km{FFFFFF}\nTuyen duong {D5D54A}B\t{566E8E}%0.1f km{FFFFFF}\nTuyen duong {D5D54A}C\t{566E8E}%0.1f km{FFFFFF}", "Chon", "Dong", 
                    GetDistanceRoute(SWEEPER_ROUTE_A) / 1000.0, GetDistanceRoute(SWEEPER_ROUTE_B) / 1000.0, GetDistanceRoute(SWEEPER_ROUTE_C) / 1000.0);
            TogglePlayerControllable(playerid, false);
        }
    }
    return 1;
}

hook OnPlayerExitVehicle(playerid, vehicleid) 
{
    if(IsVehicleSweeper(vehicleid)) {
        if(gPlayerCheckpointStatus[playerid] == CHECKPOINT_SWEEPER) {
            SendMessageTD(playerid, 4000, "Ban da roi khoi xe, cong viec cua ban da bi huy bo.");
            PlayerSweeperRoute[playerid] = -1;
            PlayerSweeperStep[playerid] = 0;
            gPlayerCheckpointStatus[playerid] = CHECKPOINT_NONE;
            ResetSweeperVehicle(vehicleid);
            DisablePlayerCP(playerid);
        }
    }
    return 1;
}

hook OnPlayerEnterCheckpoint(playerid) 
{
    if(gPlayerCheckpointStatus[playerid] == CHECKPOINT_SWEEPER) {
        if(PlayerSweeperRoute[playerid] != -1) {
            PlayerPlaySound(playerid, 1057);

            if(IsPlayerInRangeOfPoint(playerid, 7.0, SideJobInfo[0][sideJobPosX], SideJobInfo[0][sideJobPosY], SideJobInfo[0][sideJobPosZ]))
            {                
                SendMessageTD(playerid, 4000, "Ban da hoan thanh quet don tuyen duong");
                new payment = Random(PlayerSweeperStep[playerid] * 13, PlayerSweeperStep[playerid] * 18);
                GivePlayerCash(playerid, payment);
                SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* Ban da nhan duoc $%d tien luong tu viec quet don rac.", payment);

                new routeName[3][28] = {"Tuyến A", "Tuyến B", "Tuyến C"};
                new title[64];
                format(title, _, "%s (ID: %d) - dọn dẹp đường phố", GetPlayerNameEx(playerid), playerid);
                DiscordLog(dclog_Sweeper, title, "**Tiền nhận được: $%d**\n**Tuyến đường: %s**\n**Quãng đường: %0.1f km**", payment, routeName[PlayerSweeperRoute[playerid]], GetDistanceRoute(PlayerSweeperRoute[playerid]) / 1000.0);
                
                PlayerSweeperRoute[playerid] = -1;
                PlayerSweeperStep[playerid] = 0;
                gPlayerCheckpointStatus[playerid] = CHECKPOINT_NONE;
                ResetSweeperVehicle(GetPlayerVehicleID(playerid));
                DisablePlayerCP(playerid);
                return 1;
            }
            
            switch(PlayerSweeperRoute[playerid]) {
                case SWEEPER_ROUTE_A: {
                    if(PlayerSweeperStep[playerid] == sizeof(SweeperRouteA) - 1) {
                        if(IsValidDynamicObject(PlayerGarbageObject[playerid])) {
                            DestroyDynamicObject(PlayerGarbageObject[playerid]);
                            PlayerGarbageObject[playerid] = INVALID_OBJECT_ID;
                        }
                        PlayerSweeperStep[playerid]++;
                        SetPlayerCP(playerid, SideJobInfo[0][sideJobPosX], SideJobInfo[0][sideJobPosY], SideJobInfo[0][sideJobPosZ], 5.5);
                    } else {
                        PlayerSweeperStep[playerid]++;
                        SetPlayerCP(playerid, SweeperRouteA[PlayerSweeperStep[playerid]][0], SweeperRouteA[PlayerSweeperStep[playerid]][1], SweeperRouteA[PlayerSweeperStep[playerid]][2], 5.0);
                        CreateGarbageObject(playerid, SweeperRouteA[PlayerSweeperStep[playerid]][0], SweeperRouteA[PlayerSweeperStep[playerid]][1], SweeperRouteA[PlayerSweeperStep[playerid]][2]);
                    }
                    return 1;
                }
                case SWEEPER_ROUTE_B: {
                    if(PlayerSweeperStep[playerid] == sizeof(SweeperRouteB) - 1) {
                        if(IsValidDynamicObject(PlayerGarbageObject[playerid])) {
                            DestroyDynamicObject(PlayerGarbageObject[playerid]);
                            PlayerGarbageObject[playerid] = INVALID_OBJECT_ID;
                        }
                        PlayerSweeperStep[playerid]++;
                        SetPlayerCP(playerid, SideJobInfo[0][sideJobPosX], SideJobInfo[0][sideJobPosY], SideJobInfo[0][sideJobPosZ], 5.5);
                    } else {
                        PlayerSweeperStep[playerid]++;
                        SetPlayerCP(playerid, SweeperRouteB[PlayerSweeperStep[playerid]][0], SweeperRouteB[PlayerSweeperStep[playerid]][1], SweeperRouteB[PlayerSweeperStep[playerid]][2], 5.0);
                        CreateGarbageObject(playerid, SweeperRouteB[PlayerSweeperStep[playerid]][0], SweeperRouteB[PlayerSweeperStep[playerid]][1], SweeperRouteB[PlayerSweeperStep[playerid]][2]);
                    }
                    return 1;
                }
                case SWEEPER_ROUTE_C: {
                    if(PlayerSweeperStep[playerid] == sizeof(SweeperRouteC) - 1) {
                        if(IsValidDynamicObject(PlayerGarbageObject[playerid])) {
                            DestroyDynamicObject(PlayerGarbageObject[playerid]);
                            PlayerGarbageObject[playerid] = INVALID_OBJECT_ID;
                        }
                        PlayerSweeperStep[playerid]++;
                        SetPlayerCP(playerid, SideJobInfo[0][sideJobPosX], SideJobInfo[0][sideJobPosY], SideJobInfo[0][sideJobPosZ], 5.5);
                    } else {
                        PlayerSweeperStep[playerid]++;
                        SetPlayerCP(playerid, SweeperRouteC[PlayerSweeperStep[playerid]][0], SweeperRouteC[PlayerSweeperStep[playerid]][1], SweeperRouteC[PlayerSweeperStep[playerid]][2], 5.0);
                        CreateGarbageObject(playerid, SweeperRouteC[PlayerSweeperStep[playerid]][0], SweeperRouteC[PlayerSweeperStep[playerid]][1], SweeperRouteC[PlayerSweeperStep[playerid]][2]);
                    }
                    return 1;
                }
            }
        } 
    }
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext) 
{
    if(dialogid == DIALOG_SWEEPER) 
    {
        if(!response || gPlayerCheckpointStatus[playerid] != CHECKPOINT_NONE) {
            new Float:slx, Float:sly, Float:slz;
            GetPlayerPos(playerid, slx, sly, slz);
            SetPlayerPos(playerid, slx, sly, slz+1.3);
            PlayerPlaySound(playerid, 1130, slx, sly, slz+1.3);
            RemovePlayerFromVehicle(playerid);
            TogglePlayerControllable(playerid, true);

            if(gPlayerCheckpointStatus[playerid] != CHECKPOINT_NONE) {
                SendClientMessageEx(playerid, COLOR_GREY, "Ban can xoa diem danh dau tren mini-map truoc.");
            }
            return 1;
        }

        if(IsValidDynamicObject(PlayerGarbageObject[playerid])) {
            DestroyDynamicObject(PlayerGarbageObject[playerid]);
            PlayerGarbageObject[playerid] = INVALID_OBJECT_ID;
        }

        switch(listitem) {
            case 0: {
                PlayerSweeperStep[playerid] = 0;
                PlayerSweeperRoute[playerid] = SWEEPER_ROUTE_A;
                SetPlayerCP(playerid, SweeperRouteA[0][0], SweeperRouteA[0][1], SweeperRouteA[0][2], 5.0);
                CreateGarbageObject(playerid, SweeperRouteA[0][0], SweeperRouteA[0][1], SweeperRouteA[0][2]);
            }
            case 1: {
                PlayerSweeperStep[playerid] = 0;
                PlayerSweeperRoute[playerid] = SWEEPER_ROUTE_B;
                SetPlayerCP(playerid, SweeperRouteB[0][0], SweeperRouteB[0][1], SweeperRouteB[0][2], 5.0);
                CreateGarbageObject(playerid, SweeperRouteB[0][0], SweeperRouteB[0][1], SweeperRouteB[0][2]);
            }
            case 2: {
                PlayerSweeperStep[playerid] = 0;
                PlayerSweeperRoute[playerid] = SWEEPER_ROUTE_C;
                SetPlayerCP(playerid, SweeperRouteC[0][0], SweeperRouteC[0][1], SweeperRouteC[0][2], 5.0);
                CreateGarbageObject(playerid, SweeperRouteC[0][0], SweeperRouteC[0][1], SweeperRouteC[0][2]);
            }
        }

        new engine, lights, alarm, doors, bonnet, boot, objective;
        GetVehicleParamsEx(GetPlayerVehicleID(playerid), engine, lights, alarm, doors, bonnet, boot, objective);
        SetVehicleParamsEx(GetPlayerVehicleID(playerid), engine, lights, alarm, 1, bonnet, boot, objective);
        SendMessageTD(playerid, 4000, "Xe cua ban da duoc ~g~~h~khoa an toan");
        TogglePlayerControllable(playerid, true);
        gPlayerCheckpointStatus[playerid] = CHECKPOINT_SWEEPER;
        SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* Ban can hoan thanh het tuyen duong de nhan luong.");
        SendHintMessage(playerid, "Di theo diem danh dau mau do de quet don rac.");
        SendHintMessage(playerid, "Neu ban roi khoi xe, cong viec cua ban se bi huy bo.");
    }
    return 1;
}