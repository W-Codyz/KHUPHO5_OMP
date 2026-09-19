//=====================INCLUDES=====================

#define FILTERSCRIPT
#include <open.mp>
#include <Pawn.CMD>
#include <sscanf2>

//=====================DIALOGS======================

#define DIALOG_DAMAGE 1927

//==================CONTROL PANEL===================

#define MAX_DAMAGES 1000

enum {
	BODY_PART_TORSO = 3,
	BODY_PART_GROIN,
	BODY_PART_RIGHT_ARM,
	BODY_PART_LEFT_ARM,
	BODY_PART_RIGHT_LEG,
	BODY_PART_LEFT_LEG,
	BODY_PART_HEAD
}

main() { }

//===================ENUMERATORS====================

enum dmgInfo
{
	Float:dmgDamage,     // lưu float, không l� m tròn
	dmgWeapon,
	dmgBodypart,
	dmgKevlarhit,
	dmgSeconds,
	bool:dmgUsed         // cờ riêng, không phụ thuộc v� o dmgDamage == 0
}
new DamageInfo[MAX_PLAYERS][MAX_DAMAGES][dmgInfo];

public OnFilterScriptInit()
{
	print("	Loading DamageCheck ...");
	return 1;
}

public OnFilterScriptExit()
{
	return 1;
}

public OnPlayerConnect(playerid)
{
	ResetPlayerDamages(playerid);
	return 1;
}

public OnPlayerDeath(playerid, killerid, WEAPON:reason)
{
	ResetPlayerDamages(playerid);
	return 1;
}

CMD:dmgcheck(playerid, params[])
{
	new id, Float:pPos[3];

	if(sscanf(params, "u", id))
		return SendClientMessage(playerid, 0xFFFFFFAA, "USAGE: {FFFFFF}/dmgcheck [playerid]");

	if(!IsPlayerConnected(id))
		return SendClientMessage(playerid, 0xFFFFFFAA, "{00ffff}>{ffffff} Nguoi choi ban muon kiem tra khong online.");

	GetPlayerPos(id, pPos[0], pPos[1], pPos[2]);

	if(IsPlayerInRangeOfPoint(playerid, 7.0, pPos[0], pPos[1], pPos[2]))
		ShowPlayerDamages(id, playerid);
	else
		return SendClientMessage(playerid, 0xFFFFFFAA, "{00ffff}>{ffffff} Ban dung qua xa.");

	return 1;
}

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, WEAPON:weaponid, bodypart)
{
	if(!IsPlayerConnected(playerid)) return 1;
	if(amount <= 0.0) return 1;    // bỏ qua damage = 0 (godmode, injured, tazer đã set amount=0...)

	// Tìm ô trống dựa v� o cờ dmgUsed
	new id = -1;
	for(new i = 0; i < MAX_DAMAGES; i++)
	{
		if(!DamageInfo[playerid][i][dmgUsed])
		{
			id = i;
			break;
		}
	}
	if(id == -1) id = 0;    // tr� n mảng → ghi đè ô đầu

	DamageInfo[playerid][id][dmgDamage]   = amount;
	DamageInfo[playerid][id][dmgWeapon]   = _:weaponid;
	DamageInfo[playerid][id][dmgBodypart] = bodypart;

	new Float:pArm;
	GetPlayerArmour(playerid, pArm);
	DamageInfo[playerid][id][dmgKevlarhit] = (pArm > 0.0) ? 1 : 0;

	DamageInfo[playerid][id][dmgSeconds] = gettime();
	DamageInfo[playerid][id][dmgUsed]    = true;

	return 1;
}

GetWeaponNameEx(WEAPON:weaponid)
{
	new weaponname[60];
	GetWeaponName(weaponid, weaponname, sizeof(weaponname));
	return weaponname;
}

GetBodypartName(bodypart)
{
	new bodyname[11];
	switch(bodypart)
	{
		case BODY_PART_TORSO:     bodyname = "Than";
		case BODY_PART_GROIN:     bodyname = "Hang";
		case BODY_PART_RIGHT_ARM: bodyname = "Tay phai";
		case BODY_PART_LEFT_ARM:  bodyname = "Tay trai";
		case BODY_PART_RIGHT_LEG: bodyname = "Chan phai";
		case BODY_PART_LEFT_LEG:  bodyname = "Chan trai";
		case BODY_PART_HEAD:      bodyname = "Dau";
		default:                  bodyname = "UNKNOWN";
	}
	return bodyname;
}

ResetPlayerDamages(playerid)
{
	for(new id = 0; id < MAX_DAMAGES; id++)
	{
		DamageInfo[playerid][id][dmgDamage]    = 0.0;
		DamageInfo[playerid][id][dmgWeapon]    = 0;
		DamageInfo[playerid][id][dmgBodypart]  = 0;
		DamageInfo[playerid][id][dmgKevlarhit] = 0;
		DamageInfo[playerid][id][dmgSeconds]   = 0;
		DamageInfo[playerid][id][dmgUsed]      = false;
	}
	return 1;
}

ShowPlayerDamages(playerid, toid)
{
	new str[1280], str1[1280], count = 0, name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));

	for(new id = 0; id < MAX_DAMAGES; id++)
	{
		if(DamageInfo[playerid][id][dmgUsed]) count++;
	}

	if(count == 0)
		return ShowPlayerDialog(toid, DIALOG_DAMAGE, DIALOG_STYLE_LIST, name,
			"Khong co sat thuong nao duoc gay ra...", "Dong", "");

	for(new id = 0; id < MAX_DAMAGES; id++)
	{
		if(!DamageInfo[playerid][id][dmgUsed]) continue;

		new giap[10];
		giap = (DamageInfo[playerid][id][dmgKevlarhit] == 1) ? "Co" : "Khong";

		if(DamageInfo[playerid][id][dmgWeapon] == 0)
		{
			format(str1, sizeof str1,
				"> {ff7700}%.2f {ffffff}sat thuong boi {00ff77}Fist {ffffff}o {7700ff}%s {ffffff}(Giap: %s) {00ff77}%d {ffffff}giay truoc\n",
				DamageInfo[playerid][id][dmgDamage],
				GetBodypartName(DamageInfo[playerid][id][dmgBodypart]),
				giap,
				gettime() - DamageInfo[playerid][id][dmgSeconds]
			);
		}
		else
		{
			format(str1, sizeof str1,
				"> {ff7700}%.2f {ffffff}sat thuong boi {00ff77}%s {ffffff}o {7700ff}%s {ffffff}(Giap: %s) {00ff77}%d {ffffff}giay truoc\n",
				DamageInfo[playerid][id][dmgDamage],
				GetWeaponNameEx(WEAPON:DamageInfo[playerid][id][dmgWeapon]),
				GetBodypartName(DamageInfo[playerid][id][dmgBodypart]),
				giap,
				gettime() - DamageInfo[playerid][id][dmgSeconds]
			);
		}
		strcat(str, str1);
	}

	ShowPlayerDialog(toid, DIALOG_DAMAGE, DIALOG_STYLE_LIST, name, str, "Dong", "");
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_DAMAGE)
		return 1;
	return 1;
}