#include <YSI\YSI_Coding\y_hooks>

#define DIALOG_EMOTE (8700)

new bool:g_PlayerTextDrawShown[MAX_PLAYERS];
new bool:g_PlayerInTextDrawMode[MAX_PLAYERS];

ShowMainMenuTextdraws(playerid)
{
    if(g_PlayerTextDrawShown[playerid])
        return 0;

    for(new i = 0; i < 80; i++)
    {
        if(IsValidDynamicPlayerTextDraw(playerid, Panel_TextDraw[playerid][i]))
            PlayerTextDrawShow(playerid, Panel_TextDraw[playerid][i]);
    }
    SelectTextDraw(playerid, 0xff78bcf0);
    PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);

    g_PlayerTextDrawShown[playerid] = true;
    g_PlayerInTextDrawMode[playerid] = true;
    return 1;
}

HideMainMenuTextdraws(playerid)
{
    if(!g_PlayerTextDrawShown[playerid])
        return 0;

    for(new i = 0; i < 80; i++)
    {
        if(IsValidDynamicPlayerTextDraw(playerid, Panel_TextDraw[playerid][i]))
            PlayerTextDrawHide(playerid, Panel_TextDraw[playerid][i]);
    }
    CancelSelectTextDraw(playerid);
    PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);

    g_PlayerTextDrawShown[playerid] = false;
    g_PlayerInTextDrawMode[playerid] = false;
    return 1;
}

hook OnClickDynamicTextDraw(playerid, Text:clickedid)
{
    if(clickedid == Text:INVALID_TEXT_DRAW) 
    {
        if(g_PlayerTextDrawShown[playerid])
        {
            HideMainMenuTextdraws(playerid);
            return 1;
        }
    }
    return 0;
}

hook OnClickDynamicPlayerTextDraw(playerid, PlayerText:playertextid)
{
    if(!g_PlayerInTextDrawMode[playerid])
        return 0;
    
    if(playertextid == Panel_TextDraw[playerid][5])
    {
        HideMainMenuTextdraws(playerid);
        PC_EmulateCommand(playerid, "/dienthoai");
        SetPVarInt(playerid, "DelayPhone", 1);
        return 1;
    }
    else if(playertextid == Panel_TextDraw[playerid][10])
    {
        HideMainMenuTextdraws(playerid);
        return 1;
    }
    else if(playertextid == Panel_TextDraw[playerid][20])
    {
        HideMainMenuTextdraws(playerid);
        PC_EmulateCommand(playerid, "/tuido");
        return 1;
    }
    else if(playertextid == Panel_TextDraw[playerid][18])
    {
        HideMainMenuTextdraws(playerid);
        PC_EmulateCommand(playerid, "/chinhxe");
        return 1;
    }
    else if(playertextid == Panel_TextDraw[playerid][16])
    {
        HideMainMenuTextdraws(playerid);
        ShowPlayerDialog(playerid, DIALOG_EMOTE, DIALOG_STYLE_LIST, "Hanh dong", "Gio tay dau hang\nChi tay ve phia truoc\nVay tay\nNoi chuyen\nChap tay thu thai\n{00FFAA}>{ffffff}Danh sach hanh dong\n{ff4400}X{ffffff}Dung hanh dong", "Chon", "Huy");
        return 1;
    }
    else if(playertextid == Panel_TextDraw[playerid][14])
    {
        HideMainMenuTextdraws(playerid);
        PC_EmulateCommand(playerid, "/timduong");
        return 1;
    }
    else if(playertextid == Panel_TextDraw[playerid][13])
    {
        HideMainMenuTextdraws(playerid);
        //PC_EmulateCommand(playerid, "/hoadon");
        ShowTDN(playerid, NOTIFICATION_WARNING, "Chuc nang dang phat trien.");
        return 1;
    }
    else if(playertextid == Panel_TextDraw[playerid][12])
    {
        HideMainMenuTextdraws(playerid);
        PC_EmulateCommand(playerid, "/timvieclam");
        return 1;
    }
    
    return 0;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) 
{
	if(newkeys & KEY_NO)
    {
		if(!IsPlayerInAnyVehicle(playerid))
		{
        	PC_EmulateCommand(playerid, "/menu");
		}
    }
    return 1;
}


CMD:menu(playerid, params[])
{
    if(g_PlayerTextDrawShown[playerid])
        return SendClientMessage(playerid, COLOR_GREY, "Menu da duoc hien thi.");
    
    ShowMainMenuTextdraws(playerid);
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    g_PlayerTextDrawShown[playerid] = false;
    g_PlayerInTextDrawMode[playerid] = false;
    
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_EMOTE)
    {
        if(response)
        {
            switch(listitem)
            {
                case 0: // Gio tay
                {
                    if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || GetPVarInt(playerid, "Injured") != 0 || PlayerCuffed[playerid] != 0 || PlayerInfo[playerid][pHospital] != 0) 
                        return SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong the lam dieu nay vao luc nay.");
                    ClearAnimations(playerid);
                    ApplyAnimation(playerid, "SHOP", "SHP_HandsUp_Scr", 4.1, 0, 0, 0, 1, 0);
                }
                case 1: // Chi tay
                {
                    if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || GetPVarInt(playerid, "Injured") != 0 || PlayerCuffed[playerid] != 0 || PlayerInfo[playerid][pHospital] != 0) 
                        return SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong the lam dieu nay vao luc nay.");
                    ClearAnimations(playerid);
                    ApplyAnimation(playerid, "ON_LOOKERS", "point_loop", 4.1, 1, 0, 0, 0, 0, 1);
                }
                case 2: // Vay tay
                {
                    if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || GetPVarInt(playerid, "Injured") != 0 || PlayerCuffed[playerid] != 0 || PlayerInfo[playerid][pHospital] != 0) 
                        return SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong the lam dieu nay vao luc nay.");
                    ClearAnimations(playerid);
                    ApplyAnimation(playerid, "ON_LOOKERS", "wave_loop", 4.1, 1, 0, 0, 0, 0, 1);
                }
                case 3: // Noi chuyen
                {
                    if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || GetPVarInt(playerid, "Injured") != 0 || PlayerCuffed[playerid] != 0 || PlayerInfo[playerid][pHospital] != 0) 
                        return SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong the lam dieu nay vao luc nay.");
                    ClearAnimations(playerid);
                    ApplyAnimation(playerid, "GANGS", "prtial_gngtlkD", 4.1, 1, 1, 1, 1, 1, 1);
                }
                case 4: // Bo tay
                {
                    if(GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || GetPVarInt(playerid, "Injured") != 0 || PlayerCuffed[playerid] != 0 || PlayerInfo[playerid][pHospital] != 0) 
                        return SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong the lam dieu nay vao luc nay.");
                    ClearAnimations(playerid);
                    ApplyAnimation(playerid, "DEALER", "DEALER_IDLE", 4.1, 1, 0, 0, 0, 0, 1);
                }
                case 5: // Danh sach hanh dong
                {
                    PC_EmulateCommand(playerid, "/hanhdong");
                }
                case 6: // Dung hanh dong
                {
                    ClearAnimations(playerid);
                }
            }
        }
    }
    return 1;
}

hook OnPlayerConnect(playerid)
{
    g_PlayerTextDrawShown[playerid] = false;
    g_PlayerInTextDrawMode[playerid] = false;
    return 1;
}