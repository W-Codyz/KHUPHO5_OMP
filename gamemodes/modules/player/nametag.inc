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
        PlayerNameTag[playerid] = INVALID_3DTEXT_ID;
    }
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
            new Float:fHP, Float:fARM, string[256], szNameColor[8] = "FFFFFF";
            if(0 <= PlayerInfo[playerid][pMember] < MAX_GROUPS && PlayerInfo[playerid][pDuty] == 1) {
                new gId = PlayerInfo[playerid][pMember];
                format(szNameColor, _, "%s", Group_NumToDialogHex(arrGroupData[gId][g_hDutyColour]));
                format(string, _, "{%s}%s\n", szNameColor, arrGroupData[gId][g_szGroupName]);
            }

            GetPlayerHealth(playerid, fHP);
            GetPlayerArmour(playerid, fARM);
            if(fARM <= 0) format(string, _, "%s{%s}%s (%d)\n%s", string, szNameColor, GetPlayerNameEx(playerid), playerid, GetHealthDot(fHP));
            else format(string, _, "%s{%s}%s (%d)\n%s\n%s", string, szNameColor, GetPlayerNameEx(playerid), playerid, GetARMDot(fARM), GetHealthDot(fHP));
            
            Update3DTextLabelText(PlayerNameTag[playerid], 0xFFFFFFFF, string);
        }
    }
}