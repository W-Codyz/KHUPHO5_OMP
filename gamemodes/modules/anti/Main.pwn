#include <YSI/YSI_Coding/y_hooks>



new checkHackPlayer[MAX_PLAYERS];
new speedHackPlayer[MAX_PLAYERS];
new countArenaCheck[MAX_PLAYERS];
new bannedRcon = 1;
// new IsMobile[MAX_PLAYERS];

CMD:rconbanned(playerid, params[]) {
	if(PlayerInfo[playerid][pAdmin] < 9999) return 1;
	if(bannedRcon == 0) {
		bannedRcon = 1;
		SendClientMessageEx(playerid, -1, "{ff0000}[!]{FFFFFF} Rcon Banned Hacking {00ff00}Closed{FFFFFF}");
	}
	else {
		bannedRcon = 0;
		SendClientMessageEx(playerid, -1, "{ff0000}[!]{FFFFFF} Rcon Banned Hacking {00ff00}Start{FFFFFF}");
	}
	return 1;
}

/*
public OnPlayerBreakAir(playerid, breaktype) {
	new string[1280];
    format(string, sizeof(string), "[ANTI-CHEAT] %s nghi van dang su dung OnPlayerBreakAir #%d.", GetPlayerNameEx(playerid), breaktype);
    ABroadCast(COLOR_YELLOW, string, 2);
	return 1;
}

public OnPlayerSpamCars(playerid, number) {
	new string[1280];
    format(string, sizeof(string), "[ANTI-CHEAT] %s nghi van dang su dung OnPlayerSpamCars #%d.", GetPlayerNameEx(playerid), number);
    ABroadCast(COLOR_YELLOW, string, 2);
	return 1;
}

public OnPlayerCarTroll(playerid, vehicleid, trolledid, trolltype) {
	new string[1280];
    format(string, sizeof(string), "[ANTI-CHEAT] %s nghi van dang su dung OnPlayerCarTroll [%d - %d - %d].", GetPlayerNameEx(playerid), vehicleid, trolledid, trolltype);
    ABroadCast(COLOR_YELLOW, string, 2);
	return 1;
}
*/

stock IsAdminPlayer(playerid) {
	return PlayerInfo[playerid][pAdmin];
}

forward startAntiCheat(playerid);
public startAntiCheat(playerid) {
	SendClientMessage(playerid, -1, "{ff0000}ANTI CHEAT{FFFFFF} : {00ff00}Active{FFFFFF}.");
	checkHackPlayer[playerid] = SetTimerEx("AntiHackForPlayer", 1000, true, "d", playerid);
	speedHackPlayer[playerid] = SetTimerEx("checkSpeedTPhat", 500, true, "d", playerid);
	new version[1280];
    GetPlayerVersion(playerid, version, sizeof(version));
    /* if(strcmp(version, "0.3.7", true, strlen(version)) == 0) 
    	IsMobile[playerid] = 1;
    else
    	IsMobile[playerid] = 0; */
	return 1;
}

stock GetSpeedPlayer(playerid)
{
	new	Float:pos[3], Float:PlayerSpeedDistance;
 	GetPlayerVelocity(playerid, pos[0], pos[1], pos[2]);
	PlayerSpeedDistance = floatmul(floatsqroot(floatadd(floatadd(floatpower(pos[0], 2), floatpower(pos[1], 2)),  floatpower(pos[2], 2))), 170.0);
	new
 		spe = floatround(PlayerSpeedDistance * 1);
	return spe;
}

stock GetSpeedKM(playerid)
{
	new	Float:pos[3], Float:PlayerSpeedDistance;
 	GetVehicleVelocity(GetPlayerVehicleID(playerid), pos[0], pos[1], pos[2]);
	PlayerSpeedDistance = floatmul(floatsqroot(floatadd(floatadd(floatpower(pos[0], 2), floatpower(pos[1], 2)),  floatpower(pos[2], 2))), 170.0);
	new
 		spe = floatround(PlayerSpeedDistance * 1);
	return spe;
}

forward AntiTrollCarNe(playerid);
public AntiTrollCarNe(playerid) {
    if(TrollCarTime[playerid] < gettime()) {
        TrollCarTime[playerid] = gettime() + 5;
    }
    else {
        TrollCarAmount[playerid] += 1;
        if(TrollCarAmount[playerid] == 2) 
        {
            new string[1280];
            format(string, sizeof(string), "[ANTI-CHEAT] %s nghi van dang su dung Hack Troll Car.", GetPlayerNameEx(playerid));
            ABroadCast(COLOR_YELLOW, string, 2);
            discord_messHack(string);

            new Float:posx, Float:posy, Float:posz;
            GetPlayerPos(playerid, posx, posy, posz);
            SetPlayerPos(playerid, posx, posy, posz+5);
            PlayerPlaySound(playerid, 1130, posx, posy, posz+5);

            if(AdminOnl() == 0) Kick(playerid);
        }
    }
    return 1;
}

AdminOnl() {
	new adm = 0;
	foreach(new playerid: Player) {
		if(PlayerInfo[playerid][pAdmin] >= 2) {
			adm += 1;
		}
	}
	return adm;
}

forward rconBannedPlayer(playerid);
public rconBannedPlayer(playerid) {
	if(bannedRcon == 0) {
		new rcon_banip[24], rcon_banned[24];
		
		new playerip[32];
		GetPlayerIp(playerid,playerip,sizeof(playerip));
	    GetPlayerIp(playerid, playerip, sizeof(playerip));
		format(rcon_banip, sizeof(rcon_banip),"banip %s", playerip);
		SendRconCommand(rcon_banip);

		format(rcon_banned, sizeof(rcon_banned),"ban %d", playerid);
		SendRconCommand(rcon_banned);
	}
	else {
		Kick(playerid);
	}
	return 1;
}

hook OnPlayerDisconnect(playerid, reason) {
	KillTimer(checkHackPlayer[playerid]);
	KillTimer(speedHackPlayer[playerid]);
	return 1;
}

	/*
		LIST ANTI HACKING BY NDP
		----------------------------------------------------
		1. HELATH / ARMOR
		2. C-BUG
		3. SPAM TEXT , SPAM SERVER , KOTEX SERVER
		4. AMMO
		5. TROLL CAR
		6. OnPlayerGiveDamage
		7. Fake connect
		----------------------------------------------------
	*/

forward AntiHackForPlayer(playerid);
public AntiHackForPlayer(playerid) 
{

	// FAKE ONLINE
	fakeOnline_weapon(playerid);
	fakeOnline_move(playerid);
	fakeOnline_name(playerid);

	// SPEED
	checkSpeed_2(playerid);
	// checkPrison(playerid);
	checkSpeedVehicle(playerid);

	// TROLL CAR
	// checkTrollCar(playerid);

	// CRASHER WEAPON
	AntiCrasherWeapon(playerid);

	// BUG ARENA
	if(gPlayerLogged{playerid} == 1) {
		countArenaCheck[playerid] += 1;
		if(countArenaCheck[playerid] == 10) { // 10 giây, kiểm tra một lần.
			countArenaCheck[playerid] = 0;
			checkArena(playerid);
		}
	}
	/*
	if(ActiveAccount == 0) {
		if(PlayerInfo[playerid][pAdmin] == 0 && GetSpeedPlayer(playerid) > 0) {
			if(PlayerInfo[playerid][ActiveAcount] == 0) {
		        showActiveAccount(playerid);
		        return 1;
		    }
		}
	}*/

	/*new Float:packetLoss;
	GetPlayerPacketLoss(playerid, packetLoss);
	if(packetLoss >= 1.0) {
		new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);

        new string[500];
        format(string, sizeof(string), "[PACKET LOSS] %s co the bi [packet loss] %.2f%%.", GetPlayerNameEx(playerid), packetLoss);
        ABroadCast(COLOR_YELLOW, string, 2);

        new String[1260];
        format( String, sizeof( String ), "[PACKET LOSS]%s co the bi [packet loss] %.2f%% (%f %f %f - VW : %d , INT : %d)", GetPlayerNameEx(playerid), packetLoss, x, y, z, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid));
		Log("logs/packet_loss.log", String);
	}*/
	return 1;
}

stock GetPlayerPacketLoss(playerid, &Float:packetLoss)
{
    if (!IsPlayerConnected(playerid))
    {
        packetLoss = 0.0;
        return 0;
    }

    new networkStats[400], lossStr[16], start, end;
    GetPlayerNetworkStats(playerid, networkStats, sizeof(networkStats));

    // Tìm vị trí của "packetloss"
    start = strfind(networkStats, "packetloss", true);
    if (start == -1) return 0; // Không tìm thấy, tránh lỗi

    // Tìm vị trí của "%"
    end = strfind(networkStats, "%", true, start);
    if (end == -1) return 0;

    // Cắt chuỗi từ vị trí "packetloss" đến "%"
    strmid(lossStr, networkStats, start + 12, end, sizeof(lossStr));

    // Chuyển đổi sang float
    packetLoss = floatstr(lossStr);
    return 1;
}


forward checkArena(playerid);
public checkArena(playerid) {
	if(GetPVarInt(playerid, "IsInArena") >= 0) {
		new arenaid = GetPVarInt(playerid, "IsInArena");

		if(PaintBallArena[arenaid][pbGameType] == 1 || PaintBallArena[arenaid][pbGameType] == 4) {
			if(!IsPlayerInRangeOfPoint(playerid, 500, PaintBallArena[arenaid][pbDeathmatch1][0],PaintBallArena[arenaid][pbDeathmatch1][1],PaintBallArena[arenaid][pbDeathmatch1][2])) {
				LeavePaintballArena(playerid, GetPVarInt(playerid, "IsInArena"));
			}
		}
		else if(PaintBallArena[arenaid][pbGameType] == 2 || PaintBallArena[arenaid][pbGameType] == 3 || PaintBallArena[arenaid][pbGameType] == 5) {
			if(!IsPlayerInRangeOfPoint(playerid, 500, PaintBallArena[arenaid][pbTeamRed1][0],PaintBallArena[arenaid][pbTeamRed1][1],PaintBallArena[arenaid][pbTeamRed1][2])) {
				LeavePaintballArena(playerid, GetPVarInt(playerid, "IsInArena"));
			}
		}
	}
	return 1;
}

																					#include "./modules/anti/Module/fakeOnline.pwn"
																					#include "./modules/anti/Module/Speed.pwn"
																					//#include "./modules/anti/Module/trollCar.pwn"
																					#include "./modules/anti/Module/Rapid.pwn"
																					#include "./modules/anti/Module/Teleport.pwn"
																					#include "./modules/anti/Module/Anti-Teleport.pwn"
																					#include "./modules/anti/Module/Weapon.pwn"
																					#include "./modules/anti/Module/Spam.pwn"
																					#include "./modules/anti/Module/KillAll.pwn"
																					//#include "./modules/anti/Module/antigod.pwn"

																					#include "./modules/anti/Module/trollCar.pwn"
