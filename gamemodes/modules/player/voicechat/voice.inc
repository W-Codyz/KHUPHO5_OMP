#include <open.mp>
#include <YSI\YSI_Coding\y_hooks>

#define KEY_Z 0x5A
#define KEY_B 0x42

public SV_VOID:OnPlayerActivationKeyPress(SV_UINT:playerid, SV_UINT:keyid)
{
    if (bMicEnabled[playerid]) return;
    if ((keyid == KEY_Z || keyid == KEY_B) && lstream[playerid])
        SvAttachSpeakerToStream(lstream[playerid], playerid);
}

public SV_VOID:OnPlayerActivationKeyRelease(SV_UINT:playerid, SV_UINT:keyid)
{
    if (bMicEnabled[playerid]) return;
    if ((keyid == KEY_Z || keyid == KEY_B) && lstream[playerid])
        SvDetachSpeakerFromStream(lstream[playerid], playerid);
}

hook OnPlayerConnect(playerid)
{
    bMicEnabled[playerid] = false;
    lstream[playerid] = SV_NULL;

    if (SvGetVersion(playerid) == SV_NULL)
    {
        SendClientMessage(playerid, 0xFF4444AA, "[VOICE] Ban chua cai sampvoice, truy cap discord: dsc.gg/khupho5 de cai dat.");
        return 1;
    }

    if ((lstream[playerid] = SvCreateDLStreamAtPlayer(40.0, SV_INFINITY, playerid, 0xff0000ff, "Local")))
    {
        if (SvHasMicro(playerid) != SV_FALSE)
        {
            SvAddKey(playerid, KEY_Z);
            SvAddKey(playerid, KEY_B);
        }
    }
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    if (bMicEnabled[playerid])
    {
        SvStopRecord(playerid);
        SvDetachSpeakerFromStream(lstream[playerid], playerid);
    }
    if (lstream[playerid])
    {
        SvDeleteStream(lstream[playerid]);
        lstream[playerid] = SV_NULL;
    }
    bMicEnabled[playerid] = false;
    return 1;
}

CMD:mic(playerid, params[])
{
    if (!lstream[playerid] || SvHasMicro(playerid) == SV_FALSE)
        return SendClientMessage(playerid, 0xFF4444AA, "[VOICE] Ban khong the su dung lenh nay.");

    bMicEnabled[playerid] = !bMicEnabled[playerid];

    if (bMicEnabled[playerid])
    {
        SvAttachSpeakerToStream(lstream[playerid], playerid);
        SvStartRecord(playerid);
        SendClientMessage(playerid, COLOR_WHITE, "{00FF00}[VOICE] {FFFFFF}Mic ranh tay {00FF00}BAT{FFFFFF}. Mic dang mo lien tuc.");
    }
    else
    {
        SvStopRecord(playerid);
        SvDetachSpeakerFromStream(lstream[playerid], playerid);
        SendClientMessage(playerid, COLOR_WHITE, "{FF4444}[VOICE] {FFFFFF}Mic ranh tay {FF4444}TAT{FFFFFF}. Giu {FFFF00}Z/B{FFFFFF} de noi.");
    }
    return 1;
}