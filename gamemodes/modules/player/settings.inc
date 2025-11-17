#include <YSI\YSI_Coding\y_hooks>

stock ShowPlayerSettings(playerid)
{
    new dialogInfo[4096], dccUsername[DCC_USERNAME_SIZE], email[128];
    if(!IsPlayerDiscordVerified(playerid)) {
        strcat(dccUsername, "{FFD700}Chua lien ket");
    }
    else {
        new DCC_User:dccUser = DCC_FindUserById(PlayerInfo[playerid][pDiscordID]);
        DCC_GetUserName(dccUser, dccUsername, sizeof(dccUsername));
        format(dccUsername, sizeof(dccUsername), "{00FF00}#%s", dccUsername);
    }
    
    if(GetPVarInt(playerid, "NullEmail")) {
        strcat(email, "{FFD700}Chua co email");
    }
    else {
        format(email, _, "{00FF00}%s", PlayerInfo[playerid][pEmail]);
    }

    format(dialogInfo, _, "\
    {FFFFFF}Discord lien ket\t\t\t%s\n\
    {FFFFFF}Email\t\t\t%s\n\
    {FFFFFF}Tro chuyen Family\t\t%s\n\
    {FFFFFF}Tro chuyen OOC\t\t%s\n\
    {FFFFFF}Kenh quang cao\t\t%s\n\
    {FFFFFF}Kenh Newbie\t\t\t%s\n\
    {FFFFFF}Tin nhan rieng tu\t\t%s\n\
    {FFFFFF}Dien thoai\t\t\t%s\n\
    {FFFFFF}Kenh radio\t\t\t%s\n",
    dccUsername, email,
    !gFam[playerid] ? ("{00FF00}Bat") : ("{FF0000}Tat"),
    !gOoc[playerid] ? ("{00FF00}Bat") : ("{FF0000}Tat"),
    !gNews[playerid] ? ("{00FF00}Bat") : ("{FF0000}Tat"),
    !gNewbie[playerid] ? ("{00FF00}Bat") : ("{FF0000}Tat"),
    !HidePM[playerid] ? ("{00FF00}Bat") : ("{FF0000}Tat"),
    !PhoneOnline[playerid] ? ("{00FF00}Bat") : ("{FF0000}Tat"),
    !GetPVarInt(playerid, "togRadio") ? ("{00FF00}Bat") : ("{FF0000}Tat"));

    ShowPlayerDialog(playerid, DIALOG_SETTING, DIALOG_STYLE_TABLIST, "{FFFFFF}Cai dat tai khoan", dialogInfo, "Chon", "Dong");
    return 1;
}

alias:caidat("setting")
CMD:caidat(playerid, params[])
{
    if(GetPVarInt(playerid, "IsInArena") >= 0) 
        return SendClientMessageEx(playerid, COLOR_WHITE, "Ban khong the lam dieu nay tai dau truong!");

    if(GetPVarType(playerid, "StreamPrep")) 
		return SendClientMessageEx(playerid, COLOR_WHITE, "Ban khong the lam dieu do vao luc nay, hay doi den khi khi ket thuc.");
	
    if(PlayerCuffed[playerid] >= 1) 
        return SendClientMessageEx(playerid, COLOR_WHITE, "Ban khong the lam dieu nay ngay bay gio.");

    ShowPlayerSettings(playerid);
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_SETTING && response)
    {
        switch(listitem) {
            case 0: { // verify discord
                ShowPlayerSettings(playerid);
                return 1;
            }
            case 1: { // email
                /* #if !defined BETA_TEST
                    if(GetPVarInt(playerid, "NullEmail")) {
                        ShowPlayerDialog(playerid, NULLEMAIL, DIALOG_STYLE_INPUT, "{FFFFFF}Cai dat > Email", "{FFFFFF}Tai khoan cua ban hien tai chua lien ket Email, viec lien ket\nEmail se giup tai khoan tro choi cua ban tro nen bao mat hon,\nhay nhap Email cua ban vao o ben duoi va lay ma xac thuc\n\n{F3E351}Luu y: Email la duy nhat va phai la mot Email hop le.", "Xac nhan", "Quay lai");
                        return 1;
                    }
                #else
                    SendClientMessageEx(playerid, COLOR_GREY, "Khong the su dung trong Beta Test.");
                #endif */
                ShowPlayerSettings(playerid);
                return 1;
            }
            case 2: { // tog family
                gFam[playerid] = !gFam[playerid];
            }
            case 3: { // tog ooc
                gOoc[playerid] = !gOoc[playerid];
            }
            case 4: { // tog news
                gNews[playerid] = !gNews[playerid];
            }
            case 5: { // tog newbie
                gNewbie[playerid] = !gNewbie[playerid];
            }
            case 6: { // tog hide pm
                HidePM[playerid] = !HidePM[playerid];
            }
            case 7: { // tog phone
                PhoneOnline[playerid] = !PhoneOnline[playerid];
            }
            case 8: { // tog radio
                SetPVarInt(playerid, "togRadio", !GetPVarInt(playerid, "togRadio"));
            }
        }

        ShowPlayerSettings(playerid);
    }
    return 1;
}