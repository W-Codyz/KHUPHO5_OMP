#include <open.mp>
#include <Pawn.CMD>
#include <n_animations>

public OnFilterScriptInit()
{
    return 1;
}

public OnFilterScriptExit()
{
    return 1;
}

CMD:anims(playerid, params[])
{
    ShowAnimsDialog(playerid);
    return 1;
}

CMD:stopanim(playerid, params[])
{
    ClearAnimations(playerid, 1);
    return 1;
}