/*
--------------------------------------------------
Anti-Cheat ZnX (Da duoc vo hieu hoa)
--------------------------------------------------
*/

#if defined _ANTI_HACK_CHUAN_INCLUDED
	#endinput
#endif
#define _ANTI_HACK_CHUAN_INCLUDED

#include <open.mp>

// --- Cac ham Safe... gio se hoat dong nhu ham goc ---

stock SafeSetPlayerPos(playerid, Float:x, Float:y, Float:z) {
	return SetPlayerPos(playerid, x, y, z);
}

stock SafeSetPlayerPosFindZ(playerid, Float:x, Float:y, Float:z) {
    return SetPlayerPosFindZ(playerid, x, y, z);
}

stock SafePutPlayerInVehicle(playerid, vehicleid, seatid) {
    return PutPlayerInVehicle(playerid, vehicleid, seatid);
}

stock SafeSetVehiclePos(vehicleid, Float:x, Float:y, Float:z) {
    return SetVehiclePos(vehicleid, x, y, z);
}

stock SafeSetPlayerHealth(playerid, Float:health) {
    return SetPlayerHealth(playerid, health);
}

stock SafeSetPlayerArmour(playerid, Float:armour) {
    return SetPlayerArmour(playerid, armour);
}

stock SafeRemoveVehicle(vehicleid) {
    return DestroyVehicle(vehicleid);
}

stock SafeCreateVehicle(modelid, Float:x, Float:y, Float:z, Float:angle, color1, color2, respawn_delay, addsiren = 0) {
    return CreateVehicle(modelid, x, y, z, angle, color1, color2, respawn_delay, addsiren);
}
