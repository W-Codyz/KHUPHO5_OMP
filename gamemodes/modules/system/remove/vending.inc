#include <YSI\YSI_Coding\y_hooks>

stock RemoveDefaultVendingMachines(playerid)
{
    RemoveBuildingForPlayer(playerid, 955,  0.0, 0.0, 0.0, 20000.0); // CJ_EXT_SPRUNK
    RemoveBuildingForPlayer(playerid, 956,  0.0, 0.0, 0.0, 20000.0); // CJ_EXT_CANDY
    RemoveBuildingForPlayer(playerid, 1209, 0.0, 0.0, 0.0, 20000.0); // vendmach
    RemoveBuildingForPlayer(playerid, 1302, 0.0, 0.0, 0.0, 20000.0); // vendmachfd
    RemoveBuildingForPlayer(playerid, 1775, 0.0, 0.0, 0.0, 20000.0); // CJ_SPRUNK1
    RemoveBuildingForPlayer(playerid, 1776, 0.0, 0.0, 0.0, 20000.0); // CJ_CANDYVENDOR
    RemoveBuildingForPlayer(playerid, 1977, 0.0, 0.0, 0.0, 20000.0); // vendin3
}

hook OnPlayerConnect(playerid)
{
    RemoveDefaultVendingMachines(playerid);
    return 1;
}
