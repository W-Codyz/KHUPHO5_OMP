#include <open.mp>
#include <YSI\YSI_Coding\y_hooks>

#define DIALOG_LOCATION_LIST      8500
#define DIALOG_LOCATION_OPTIONS   8501
#define DIALOG_LOCATION_DETAILS   8502

enum pLocationData {
    lSenderID,
    lSenderName[MAX_PLAYER_NAME],
    Float:lPosX,
    Float:lPosY,
    Float:lPosZ,
    lZone[MAX_ZONE_NAME],
    lSendTime
}

#define MAX_LOCATIONS 10

new PlayerLocations[MAX_PLAYERS][MAX_LOCATIONS][pLocationData];
new PlayerLastSentLocation[MAX_PLAYERS];


CMD:guivitri(playerid, params[])
{
    if(gettime() - PlayerLastSentLocation[playerid] < 60) { // 60 giây = 1 phút
        new timeLeft = 60 - (gettime() - PlayerLastSentLocation[playerid]);
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban phai doi %d giay nua moi co the gui vi tri tiep.", timeLeft);
    }
    
    new targetid;
    if(sscanf(params, "u", targetid))
        return SendClientMessageEx(playerid, COLOR_GREY, "SU DUNG: /guivitri [id/ten]");
    
    if(!IsPlayerConnected(targetid))
        return SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi khong ton tai hoac dang offline.");
    
    if(playerid == targetid)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong the gui vi tri cho chinh minh.");
    
    new bool:hasSpace = false;
    for(new i = 0; i < MAX_LOCATIONS; i++) {
        if(PlayerLocations[targetid][i][lSenderID] == INVALID_PLAYER_ID) {
            hasSpace = true;
            break;
        }
    }
    
    if(!hasSpace)
        return SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi nay da day vi tri, khong the gui them.");
    
    new Float:posX, Float:posY, Float:posZ;
    GetPlayerPos(playerid, posX, posY, posZ);
    
    new zone[MAX_ZONE_NAME];
    GetPlayer2DZone(playerid, zone, sizeof(zone));
    
    for(new i = 0; i < MAX_LOCATIONS; i++) {
        if(PlayerLocations[targetid][i][lSenderID] == INVALID_PLAYER_ID) {
            PlayerLocations[targetid][i][lSenderID] = playerid;
            GetPlayerName(playerid, PlayerLocations[targetid][i][lSenderName], MAX_PLAYER_NAME);
            PlayerLocations[targetid][i][lPosX] = posX;
            PlayerLocations[targetid][i][lPosY] = posY;
            PlayerLocations[targetid][i][lPosZ] = posZ;
            format(PlayerLocations[targetid][i][lZone], MAX_ZONE_NAME, "%s", zone);
            PlayerLocations[targetid][i][lSendTime] = gettime();
            
            PlayerLastSentLocation[playerid] = gettime();
            
            SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban da gui vi tri cua minh den %s.", GetPlayerNameEx(targetid));
            SendClientMessageEx(targetid, COLOR_WHITE, "{ff4400}>{ffffff} Ai do gui vi tri cho ban. Su dung /xemvitri de xem.");
            
            return 1;
        }
    }
    
    return 1;
}

CMD:xemvitri(playerid, params[])
{
    new bool:hasLocations = false;
    for(new i = 0; i < MAX_LOCATIONS; i++) {
        if(PlayerLocations[playerid][i][lSenderID] != INVALID_PLAYER_ID) {
            hasLocations = true;
            break;
        }
    }
    
    if(!hasLocations)
        return SendClientMessageEx(playerid, COLOR_GREY, "Khong co ai gui vi tri cho ban.");
    
    new string[1024], count = 0;
    strcat(string, "STT\tTen nguoi gui\tThoi gian\n");
    
    for(new i = 0; i < MAX_LOCATIONS; i++) {
        if(PlayerLocations[playerid][i][lSenderID] != INVALID_PLAYER_ID) {
            count++;
            
            new elapsedSeconds = gettime() - PlayerLocations[playerid][i][lSendTime];
            new minutes = elapsedSeconds / 60;
            new hours = minutes / 60;
            new days = hours / 24;
            
            new timeString[32];
            if(days > 0) {
                format(timeString, sizeof(timeString), "%d ngay truoc", days);
            } else if(hours > 0) {
                format(timeString, sizeof(timeString), "%d gio truoc", hours);
            } else if(minutes > 0) {
                format(timeString, sizeof(timeString), "%d phut truoc", minutes);
            } else {
                format(timeString, sizeof(timeString), "Vua xong");
            }
            
            format(string, sizeof(string), "%s%d\t%s\t%s\n", string, i+1, PlayerLocations[playerid][i][lSenderName], timeString);
        }
    }
    
    ShowPlayerDialog(playerid, DIALOG_LOCATION_LIST, DIALOG_STYLE_TABLIST_HEADERS, "Danh sach vi tri", string, "Chon", "Dong");
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch(dialogid) {
        case DIALOG_LOCATION_LIST: {
            if(!response) return 1;
            
            new idx = -1, count = 0;
            
            for(new i = 0; i < MAX_LOCATIONS; i++) {
                if(PlayerLocations[playerid][i][lSenderID] != INVALID_PLAYER_ID) {
                    if(count == listitem) {
                        idx = i;
                        break;
                    }
                    count++;
                }
            }
            
            if(idx == -1) return SendClientMessageEx(playerid, COLOR_GREY, "Da xay ra loi khi chon vi tri.");
            
            SetPVarInt(playerid, "SelectedLocation", idx);
            
            ShowPlayerDialog(playerid, DIALOG_LOCATION_OPTIONS, DIALOG_STYLE_LIST, "Tuy chon vi tri", 
                "Xem chi tiet\nXoa vi tri", "Chon", "Quay lai");
        }
        case DIALOG_LOCATION_OPTIONS: {
            if(!response) {
                pc_cmd_xemvitri(playerid, "");
                return 1;
            }
            
            new idx = GetPVarInt(playerid, "SelectedLocation");
            
            switch(listitem) {
                case 0: { 
                    new Float:playerX, Float:playerY, Float:playerZ;
                    GetPlayerPos(playerid, playerX, playerY, playerZ);
                    
                    new Float:distance = GetDistanceBetweenPoints(
                        playerX, playerY, playerZ,
                        PlayerLocations[playerid][idx][lPosX], 
                        PlayerLocations[playerid][idx][lPosY], 
                        PlayerLocations[playerid][idx][lPosZ]
                    );
                    
                    new string[1280];
                    format(string, sizeof(string), "{ff4400}>{ffffff} Nguoi gui: %s\n\n{ff4400}>{ffffff} Vi tri: %s\n\n{ff4400}>{ffffff} Toa do: %.4f, %.4f, %.4f\n\n{ff4400}>{ffffff} Khoang cach: %.2f met", 
                        PlayerLocations[playerid][idx][lSenderName],
                        PlayerLocations[playerid][idx][lZone],
                        PlayerLocations[playerid][idx][lPosX],
                        PlayerLocations[playerid][idx][lPosY],
                        PlayerLocations[playerid][idx][lPosZ],
                        distance
                    );
                    
                    ShowPlayerDialog(playerid, DIALOG_LOCATION_DETAILS, DIALOG_STYLE_MSGBOX, "Chi tiet vi tri", 
                        string, "Chi duong", "Dong");
                }
                case 1: { 
                    PlayerLocations[playerid][idx][lSenderID] = INVALID_PLAYER_ID;
                    PlayerLocations[playerid][idx][lSenderName][0] = 0;
                    PlayerLocations[playerid][idx][lPosX] = 0.0;
                    PlayerLocations[playerid][idx][lPosY] = 0.0;
                    PlayerLocations[playerid][idx][lPosZ] = 0.0;
                    PlayerLocations[playerid][idx][lZone][0] = 0;
                    PlayerLocations[playerid][idx][lSendTime] = 0;
                    
                    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Ban da xoa vi tri thanh cong.");
                    
                    pc_cmd_xemvitri(playerid, "");
                }
            }
        }
        case DIALOG_LOCATION_DETAILS: {
            if(!response) {
                ShowPlayerDialog(playerid, DIALOG_LOCATION_OPTIONS, DIALOG_STYLE_LIST, "Tuy chon vi tri", 
                    "Xem chi tiet\nXoa vi tri", "Chon", "Quay lai");
                return 1;
            }
            
            new idx = GetPVarInt(playerid, "SelectedLocation");
            
            SetPlayerCP(playerid, PlayerLocations[playerid][idx][lPosX], PlayerLocations[playerid][idx][lPosY], PlayerLocations[playerid][idx][lPosZ], 3.0);
            gPlayerCheckpointStatus[playerid] = CHECKPOINT_GPS;
            
            SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban da xem vi tri cua %s. Vi tri da duoc danh dau tren ban do.", 
                PlayerLocations[playerid][idx][lSenderName]);
        }
    }
    return 0;
}

hook OnPlayerDisconnect(playerid, reason)
{
    for(new i = 0; i < MAX_PLAYERS; i++) {
        for(new j = 0; j < MAX_LOCATIONS; j++) {
            if(PlayerLocations[i][j][lSenderID] == playerid) {
                PlayerLocations[i][j][lSenderID] = INVALID_PLAYER_ID;
                PlayerLocations[i][j][lSenderName][0] = 0;
                PlayerLocations[i][j][lPosX] = 0.0;
                PlayerLocations[i][j][lPosY] = 0.0;
                PlayerLocations[i][j][lPosZ] = 0.0;
                PlayerLocations[i][j][lZone][0] = 0;
                PlayerLocations[i][j][lSendTime] = 0;
            }
        }
    }
    
    for(new j = 0; j < MAX_LOCATIONS; j++) {
        PlayerLocations[playerid][j][lSenderID] = INVALID_PLAYER_ID;
        PlayerLocations[playerid][j][lSenderName][0] = 0;
        PlayerLocations[playerid][j][lPosX] = 0.0;
        PlayerLocations[playerid][j][lPosY] = 0.0;
        PlayerLocations[playerid][j][lPosZ] = 0.0;
        PlayerLocations[playerid][j][lZone][0] = 0;
        PlayerLocations[playerid][j][lSendTime] = 0;
    }
    
    PlayerLastSentLocation[playerid] = 0;
    
    return 1;
}

hook OnPlayerConnect(playerid)
{
    PlayerLastSentLocation[playerid] = 0;
    
    for(new j = 0; j < MAX_LOCATIONS; j++) {
        PlayerLocations[playerid][j][lSenderID] = INVALID_PLAYER_ID;
        PlayerLocations[playerid][j][lSenderName][0] = 0;
        PlayerLocations[playerid][j][lPosX] = 0.0;
        PlayerLocations[playerid][j][lPosY] = 0.0;
        PlayerLocations[playerid][j][lPosZ] = 0.0;
        PlayerLocations[playerid][j][lZone][0] = 0;
        PlayerLocations[playerid][j][lSendTime] = 0;
    }
    
    return 1;
}
