#include <YSI\YSI_Coding\y_hooks>

static const NbMissionIntro[5][328] =
{
    "{FFFFFF}Chao mung den voi San Andreas, day la noi ma ban se bat dau cuoc\nhanh trinh cua minh.\
    Truoc khi bat dau, ban can phai so huu cho minh\nCan cuoc cong dan de chung minh ban than cua ban, no \ 
    vo cung quan\ntrong vi co no ban moi co quyen han cong dan tai San Andreas.\n\n\
    {E7FA3B}Su dung \"/gps > Dia diem San Andreas > City Hall\"",

    "{FFFFFF}Khi da co Can cuoc cong dan, bay gio thu quan trong ma ban can\nphai so huu chinh la giay phep lai xe \
    neu ban khong muon bi canh\nrinh rap bat ki luc nao. Giay phep lai xe la mot loai giay to rat huu\nich.\n\n \
    {E7FA3B}Su dung \"/gps > Dia diem San Andreas > Trung tam giay phep\"",

    "{FFFFFF}De co the trao doi lien lac qua cuoc goi hay tin nhan voi nguoi khac, toi\nkhuyen ban nen so huu co minh mot chiec dien thoai va danh ba.\n\n\
    {E7FA3B}Su dung \"/gps > Cua hang 24/7\"",

    "{FFFFFF}Tien la mot loai phuong tien dung de trao doi hang hoa, dich vu hay\nthanh toan cac mat hang. No la mot phan thiet yeu khong the thieu\n\
    khi sinh song tai San Andreas. Ban hay thu kiem mot chut tien bang\ncong viec Boc vac, hay thu boc 10 bao hang.\n\n\
    {E7FA3B}Su dung \"/gps > Cong viec > Boc vac\"",

    "{FFFFFF}De lam viec ban can phai co mot suc khoe tot va the luc tot, vi the\nban can phai cung cap thuc an cho ban than de hoi lai the luc va suc\nkhoe. Neu khong ban co the gap bac si bat cu luc nao.\n\n\
    {E7FA3B}Su dung \"/gps > Nha hang\""
};

ShowNewbieMission(playerid)
{
    new dialogInfo[428];
    strcat(dialogInfo, "Nhiem vu\tTien trinh\n");
    for(new i;i < 5;i++) {
        format(dialogInfo, _, "%s{FFFFFF}%s\t%s\n", dialogInfo, NbMissionInfo[i][nbm_Name], PlayerInfo[playerid][pNbMission][i] ? "{48C748}Hoan thanh" : "{D5C735}Chua thuc hien");
    }

    SetPVarInt(playerid, "NbMissionSelected", 0);
    ShowPlayerDialog(playerid, DIALOG_NEWBIE_MISSION, DIALOG_STYLE_TABLIST_HEADERS, "{FFFFFF}Michael Evans - Nhiem vu tan thu", dialogInfo, "Chon", "Dong");
    return 1;
}

UpdateNewbieMission(playerid, id, bool:checked=false)
{
    if(id < 0 || id > 4 || PlayerInfo[playerid][pNbMission][id]) 
        return 0;

    if(PlayerInfo[playerid][pNbCurrent] == id)
    {
        new doneMission = 0;
        switch(id)
        {
            case 0: { // id card
                if(PlayerInfo[playerid][pIDCard] != 0)
                {
                    doneMission = 1;
                    SendClientMessage(playerid, COLOR_YELLOW, "Ban da hoan thanh nhiem vu \"%s\".", NbMissionInfo[id][nbm_Name]);
                    SendClientMessage(playerid, COLOR_YELLOW, "Phan thuong: +%d XP va +$%s.", NbMissionInfo[id][nbm_XP], number_format(NbMissionInfo[id][nbm_Cash]));
                    GivePlayerCash(playerid, NbMissionInfo[id][nbm_Cash]);
                    GivePlayerXP(playerid, NbMissionInfo[id][nbm_XP]);
                }
            }
            case 1: { // car license
                if(PlayerInfo[playerid][pCarLic] != 0)
                {
                    doneMission = 1;
                    SendClientMessage(playerid, COLOR_YELLOW, "Ban da hoan thanh nhiem vu \"%s\".", NbMissionInfo[id][nbm_Name]);
                    SendClientMessage(playerid, COLOR_YELLOW, "Phan thuong: +%d XP va +$%s.", NbMissionInfo[id][nbm_XP], number_format(NbMissionInfo[id][nbm_Cash]));
                    GivePlayerCash(playerid, NbMissionInfo[id][nbm_Cash]);
                    GivePlayerXP(playerid, NbMissionInfo[id][nbm_XP]);
                }
            }
            case 2: { // phone book and phone number
                if(PlayerInfo[playerid][pPhoneBook] == 1 && PlayerInfo[playerid][pPnumber] != 0)
                {
                    doneMission = 1;
                    SendClientMessage(playerid, COLOR_YELLOW, "Ban da hoan thanh nhiem vu \"%s\".", NbMissionInfo[id][nbm_Name]);
                    SendClientMessage(playerid, COLOR_YELLOW, "Phan thuong: +%d XP va +$%s.", NbMissionInfo[id][nbm_XP], number_format(NbMissionInfo[id][nbm_Cash]));
                    GivePlayerCash(playerid, NbMissionInfo[id][nbm_Cash]);
                    GivePlayerXP(playerid, NbMissionInfo[id][nbm_XP]);
                }
            }
            case 3: { // job
                if(checked) return 1;
                if(++PlayerInfo[playerid][pNbProgress][id] >= 10)
                {
                    doneMission = 1;
                    SendClientMessage(playerid, COLOR_YELLOW, "Ban da hoan thanh nhiem vu \"%s\".", NbMissionInfo[id][nbm_Name]);
                    SendClientMessage(playerid, COLOR_YELLOW, "Phan thuong: +%d XP va +$%s.", NbMissionInfo[id][nbm_XP], number_format(NbMissionInfo[id][nbm_Cash]));
                    GivePlayerCash(playerid, NbMissionInfo[id][nbm_Cash]);
                    GivePlayerXP(playerid, NbMissionInfo[id][nbm_XP]);
                }
            }
            case 4: { // food
                if(checked) return 1;
                doneMission = 1;
                SendClientMessage(playerid, COLOR_YELLOW, "Ban da hoan thanh nhiem vu \"%s\".", NbMissionInfo[id][nbm_Name]);
                SendClientMessage(playerid, COLOR_YELLOW, "Phan thuong: +%d XP va +$%s.", NbMissionInfo[id][nbm_XP], number_format(NbMissionInfo[id][nbm_Cash]));
                GivePlayerCash(playerid, NbMissionInfo[id][nbm_Cash]);
                GivePlayerXP(playerid, NbMissionInfo[id][nbm_XP]);
            }
        }

        if(doneMission) {
            PlayerInfo[playerid][pNbMission][id] = 1;
            PlayerInfo[playerid][pNbProgress][id] = 1;
            PlayerInfo[playerid][pNbCurrent] = -1;
        }
    }
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_NEWBIE_MISSION && response) 
    {
        if(!IsPlayerDiscordVerified(playerid)) 
            return SendClientMessageEx(playerid, COLOR_GREY, "Ban phai lien ket tai khoan Discord de co the lam dieu nay.");
            
        switch(GetPVarInt(playerid, "NbMissionSelected"))
        {
            case 0:
            {
                if(listitem >= 5) return 1;
                if(PlayerInfo[playerid][pNbMission][listitem]) 
                    return SendClientMessage(playerid, COLOR_GREY, "Ban da hoan thanh nhiem vu nay roi.");

                if(listitem > 0 && !PlayerInfo[playerid][pNbMission][listitem-1])
                    return SendClientMessage(playerid, COLOR_GREY, "Ban phai hoan thanh nhiem vu thu %d truoc khi thuc hien nhiem vu nay.", listitem);
                
                if(PlayerInfo[playerid][pNbCurrent] != -1)
                    return SendClientMessage(playerid, COLOR_GREY, "Ban dang thuc hien mot nhiem vu khac.");

                ShowPlayerDialog(playerid, DIALOG_NEWBIE_MISSION, DIALOG_STYLE_MSGBOX, "{FFFFFF}Michael Evans - Nhiem vu tan thu", NbMissionIntro[listitem], "Dong y", "Dong");
                SetPVarInt(playerid, "NbMissionSelected", listitem+1);
            }
            case 1 .. 5:
            {
                new id = GetPVarInt(playerid, "NbMissionSelected") - 1;

                PlayerInfo[playerid][pNbProgress][id] = 0;
                PlayerInfo[playerid][pNbCurrent] = id;
                SendClientMessage(playerid, COLOR_YELLOW, "Ban da nhan nhiem vu \"%s\".", NbMissionInfo[id][nbm_Name]);
                SendHintMessage(playerid, "Su dung lenh \"{6CFF5F}/nhiemvu 1{FFFFFF}\" de xem lai huong dan nhiem vu.");
                UpdateNewbieMission(playerid, id, true);
                DeletePVar(playerid, "NbMissionSelected");
            }
        }
    }
    return 1;
}

CMD:nhiemvu(playerid, params[])
{
   if(isnull(params) || !IsNumeric(params)) {
        SendHelpMessage(playerid, "/nhiemvu [id]");
        SendClientMessageEx(playerid, COLOR_GRAD2, "| 1. Nhiem vu tan thu           2. Hang ngay |");  
        return 1;
   }

   switch(strval(params))
   {
       case 1: {
            if(PlayerInfo[playerid][pNbCurrent] == -1) 
                return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong dang thuc hien nhiem vu tan thu nao.");

            new curMission = PlayerInfo[playerid][pNbCurrent];
            ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "{FFFFFF}Nhiem vu > Nhiem vu tan thu", NbMissionIntro[curMission], "Dong", "");
       }
       case 2: SendClientMessageEx(playerid, COLOR_GREY, "Chuc nang dang duoc phat trien!");
       default: SendClientMessageEx(playerid, COLOR_GREY, "ID nhiem vu khong hop le!");
   }
   return 1;
}