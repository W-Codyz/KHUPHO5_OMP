const Float:NAMETAG_DISTANCE = 25.0;

new Text3D:PlayerNameTag[MAX_PLAYERS] = {INVALID_3DTEXT_ID, ...};

GetHealthDot(Float:HP)
{
    new strDot[128];
    if(HP >= 100) strDot = "{F32C2C}----------";
    else if(HP >= 90) strDot = "{F32C2C}---------{754D4D}-";
    else if(HP >= 80) strDot = "{F32C2C}--------{754D4D}--";
    else if(HP >= 70) strDot = "{F32C2C}-------{754D4D}---";
    else if(HP >= 60) strDot = "{F32C2C}------{754D4D}----";
    else if(HP >= 50) strDot = "{F32C2C}-----{754D4D}-----";
    else if(HP >= 40) strDot = "{F32C2C}----{754D4D}------";
    else if(HP >= 30) strDot = "{F32C2C}---{754D4D}-------";
    else if(HP >= 20) strDot = "{F32C2C}--{754D4D}--------";
    else if(HP >= 10) strDot = "{F32C2C}-{754D4D}---------";
    else strDot = "{754D4D}----------";

    return strDot;
}

GetARMDot(Float:ARM)
{
    new strDot[128];
    if(ARM >= 100) strDot = "{FFFFFF}----------";
    else if(ARM >= 90) strDot = "{FFFFFF}---------{5A5A5A}-";
    else if(ARM >= 80) strDot = "{FFFFFF}--------{5A5A5A}--";
    else if(ARM >= 70) strDot = "{FFFFFF}-------{5A5A5A}---";
    else if(ARM >= 60) strDot = "{FFFFFF}------{5A5A5A}----";
    else if(ARM >= 50) strDot = "{FFFFFF}-----{5A5A5A}-----";
    else if(ARM >= 40) strDot = "{FFFFFF}----{5A5A5A}------";
    else if(ARM >= 30) strDot = "{FFFFFF}---{5A5A5A}-------";
    else if(ARM >= 20) strDot = "{FFFFFF}--{5A5A5A}--------";
    else if(ARM >= 10) strDot = "{FFFFFF}-{5A5A5A}---------";
    else strDot = "{5A5A5A}----------";

    return strDot;
}

CreatePlayerNameTag(playerid)
{
    PlayerNameTag[playerid] = Create3DTextLabel(" ", -1, 0.0, 0.0, -1.0, NAMETAG_DISTANCE, 0, true);
    Attach3DTextLabelToPlayer(PlayerNameTag[playerid], playerid, 0.0, 0.0, 0.5);
}

DestroyPlayerNameTag(playerid)
{
    if(IsValid3DTextLabel(PlayerNameTag[playerid]))
    {
        Delete3DTextLabel(PlayerNameTag[playerid]);
    }
    PlayerNameTag[playerid] = INVALID_3DTEXT_ID;
}

forward UpdateNameTag(playerid);
public UpdateNameTag(playerid)
{
    if(gPlayerLogged{playerid} == 1)
    {
        if(!IsValid3DTextLabel(PlayerNameTag[playerid])) {
            printf("Player %s has not initialized the name tag.", GetPlayerNameExt(playerid));
            printf("Initialized name tag for %s. (dot '\xE2\x80\xA2')", GetPlayerNameExt(playerid));
            CreatePlayerNameTag(playerid);
        }

        if(IsValid3DTextLabel(PlayerNameTag[playerid]) && gPlayerLogged{playerid} == 1)
        {
            new Float:fHP, Float:fARM, string[512], szNameColor[8] = "FFFFFF";
            format(string, sizeof(string), "[%s {ffffff}- %s]\n",
                IsPCPlayer(playerid) ? "{52b1f8}PC" : "{52b1f8}Mobile",
                SvHasMicro(playerid) ? "{6feb61}Co Mic" : "{FF3045}Khong Mic");
            if(Aduty[playerid] == 1) {
                new szAdminRank[32];
                switch(PlayerInfo[playerid][pAdmin]) {
                    case 1:     szAdminRank = "Server Moderator";
                    case 2:     szAdminRank = "Junior Administrator";
                    case 3:     szAdminRank = "General Administrator";
                    case 4:     szAdminRank = "Senior Administrator";
                    case 1337:  szAdminRank = "Head Administrator";
                    case 1338:  szAdminRank = "Lead Head Administrator";
                    case 99999: szAdminRank = "Executive Administrator";
                    default:    szAdminRank = "Administrator";
                }
                format(string, sizeof(string), "%s{FF3045}[ADMIN - %s]\n", string, szAdminRank);
            }
            if(GetPVarInt(playerid, "AdvisorDuty") == 1) {
                new szAdvRank[24];
                switch(PlayerInfo[playerid][pHelper]) {
                    case 1: szAdvRank = "Helper";
                    case 2: szAdvRank = "Community Advisor";
                    case 3: szAdvRank = "Senior Advisor";
                    case 4: szAdvRank = "Chief Advisor";
                    default: szAdvRank = "Advisor";
                }
                format(string, sizeof(string), "%s{33CCFF}[CA - %s]\n", string, szAdvRank);
            }
            if(0 <= PlayerInfo[playerid][pMember] < MAX_GROUPS && PlayerInfo[playerid][pDuty] == 1) {
                new gId = PlayerInfo[playerid][pMember];
                format(szNameColor, _, "%s", Group_NumToDialogHex(arrGroupData[gId][g_hDutyColour]));
                format(string, sizeof(string), "%s{%s}TO CHUC - %s\n", string, szNameColor, arrGroupData[gId][g_szGroupName]);
            }
            if(g_gangBadge[playerid] && PlayerInfo[playerid][pFMember] != INVALID_FAMILY_ID) {
                format(string, sizeof(string), "%s{ffd966}GANGSTER - %s\n", string, FamilyInfo[PlayerInfo[playerid][pFMember]][FamilyName]);
            }

            GetPlayerHealth(playerid, fHP);
            GetPlayerArmour(playerid, fARM);
            if(fARM <= 0) format(string, sizeof(string), "%s{%s}%s (%d)\n%s", string, szNameColor, GetPlayerNameEx(playerid), playerid, GetHealthDot(fHP));
            else format(string, sizeof(string), "%s{%s}%s (%d)\n%s\n%s", string, szNameColor, GetPlayerNameEx(playerid), playerid, GetARMDot(fARM), GetHealthDot(fHP));

            Update3DTextLabelText(PlayerNameTag[playerid], 0xFFFFFFFF, string);
        }
    }
}