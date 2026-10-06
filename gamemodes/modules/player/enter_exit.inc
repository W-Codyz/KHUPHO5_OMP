#include <YSI\YSI_Coding\y_hooks>

#pragma warning push
#pragma warning disable 217

stock PlayerEnterInterior(playerid)
{
    if(GetPVarInt(playerid, "IsInArena") >= 0 || GetPVarType(playerid, "StreamPrep") || PlayerCuffed[playerid] >= 1) 
        return 1;
    
	new cCar = GetClosestCar(playerid);
    for(new i = 0; i < sizeof(DDoorsInfo); i++) {
        if (IsPlayerInRangeOfPoint(playerid,3.0,DDoorsInfo[i][ddExteriorX], DDoorsInfo[i][ddExteriorY], DDoorsInfo[i][ddExteriorZ]) && PlayerInfo[playerid][pVW] == DDoorsInfo[i][ddExteriorVW]) {
            if(DDoorsInfo[i][ddVIP] > 0 && PlayerInfo[playerid][pDonateRank] < DDoorsInfo[i][ddVIP]) {
                SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong the vao, cap do VIP cua ban khong du.");
                return 1;
            }

            if(DDoorsInfo[i][ddFamed] > 0 && PlayerInfo[playerid][pFamed] < DDoorsInfo[i][ddFamed]) {
                SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong the vao, cap do Famed ban khong du.");
                return 1;
            }

			if(DDoorsInfo[i][ddDPC] > 0 && PlayerInfo[playerid][pRewardHours] < 150) {
                SendClientMessageEx(playerid, COLOR_GRAD2, "You can not enter, you are not a Dedicated Player.");
                return 1;
            }

            if(DDoorsInfo[i][ddFamily] > 0) {
                if(PlayerInfo[playerid][pFMember] != DDoorsInfo[i][ddFamily]) return SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong the vao, canh cua nay chi danh cho Family.");
				else if(PlayerInfo[playerid][pRank] < DDoorsInfo[i][ddRank]) return SendClientMessageEx(playerid, COLOR_GRAD2, "Chuc vu cua ban khong du cao de co the vao.");
            }

			if(DDoorsInfo[i][ddAllegiance] > 0) {
                if(arrGroupData[PlayerInfo[playerid][pMember]][g_iAllegiance] != DDoorsInfo[i][ddAllegiance]) return SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong the vao, canh cua bi gioi han theo quoc gia.");
				else if(PlayerInfo[playerid][pRank] < DDoorsInfo[i][ddRank]) return SendClientMessageEx(playerid, COLOR_GRAD2, "Chuc vu cua ban khong du cao de co the vao.");
            }

			if(DDoorsInfo[i][ddGroupType] > 0) {
                if(arrGroupData[PlayerInfo[playerid][pMember]][g_iGroupType] != DDoorsInfo[i][ddGroupType] && arrGroupData[PlayerInfo[playerid][pMember]][g_iAllegiance] != DDoorsInfo[i][ddAllegiance]) {
					return SendClientMessageEx(playerid, COLOR_GRAD2, "You can not enter, this door is faction restricted.");
				}
				else if(PlayerInfo[playerid][pRank] < DDoorsInfo[i][ddRank]) return SendClientMessageEx(playerid, COLOR_GRAD2, "Chuc vu cua ban khong du cao de co the vao.");
            }

            if(DDoorsInfo[i][ddFaction] != INVALID_GROUP_ID) {
                if(PlayerInfo[playerid][pMember] != DDoorsInfo[i][ddFaction]) return SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong the vao, canh cua nay bi gioi han to chuc.");
				else if(PlayerInfo[playerid][pRank] < DDoorsInfo[i][ddRank]) return SendClientMessageEx(playerid, COLOR_GRAD2, "Chuc vu cua ban khong du cao de co the vao.");
            }

            if(DDoorsInfo[i][ddAdmin] > 0 && PlayerInfo[playerid][pAdmin] < DDoorsInfo[i][ddAdmin]) {
                SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong the vao, chuc vu quan tri vien cua ban khong du cao.");
                return 1;
            }

            if(DDoorsInfo[i][ddWanted] > 0 && PlayerInfo[playerid][pWantedLevel] != 0) {
                SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong the vao, canh cua nay gioi han nhung nguoi bi truy na.");
                return 1;
            }

			if(DDoorsInfo[i][ddLocked] == 1) {
			    return SendClientMessageEx(playerid, COLOR_GRAD2, "Canh cua nay dang bi khoa.");
			}

            if(DDoorsInfo[i][ddInteriorX] == 0.0) {
			    return SendClientMessageEx(playerid, COLOR_GRAD2, "Canh cua nay chua co noi that ben trong.");
			}

            SetPlayerInterior(playerid,DDoorsInfo[i][ddInteriorInt]);
            PlayerInfo[playerid][pInt] = DDoorsInfo[i][ddInteriorInt];
            PlayerInfo[playerid][pVW] = DDoorsInfo[i][ddInteriorVW];
            SetPlayerVirtualWorld(playerid, DDoorsInfo[i][ddInteriorVW]);
            if(DDoorsInfo[i][ddVehicleAble] > 0 && GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
                SetVehiclePos(GetPlayerVehicleID(playerid), DDoorsInfo[i][ddInteriorX],DDoorsInfo[i][ddInteriorY],DDoorsInfo[i][ddInteriorZ]);
                SetVehicleZAngle(GetPlayerVehicleID(playerid), DDoorsInfo[i][ddInteriorA]);
                SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), DDoorsInfo[i][ddInteriorVW]);
                LinkVehicleToInterior(GetPlayerVehicleID(playerid), DDoorsInfo[i][ddInteriorInt]);
	            if(GetPVarInt(playerid, "tpForkliftTimer") > 0)
				{
				    SetPVarInt(playerid, "tpJustEntered", 1);
				    new Float: pX, Float: pY, Float: pZ;
					GetPlayerPos(playerid, pX, pY, pZ);
					SetPVarFloat(playerid, "tpForkliftX", pX);
			 		SetPVarFloat(playerid, "tpForkliftY", pY);
			  		SetPVarFloat(playerid, "tpForkliftZ", pZ);
				}
                if(DynVeh[GetPlayerVehicleID(playerid)] != -1)
				{
				    new vw[1];
					vw[0] = GetVehicleVirtualWorld(GetPlayerVehicleID(playerid));
				    if(DynVehicleInfo[DynVeh[GetPlayerVehicleID(playerid)]][gv_iAttachedObjectModel][0] != INVALID_OBJECT_ID)
				    {
				    	Streamer_SetArrayData(STREAMER_TYPE_OBJECT, DynVehicleInfo[DynVeh[GetPlayerVehicleID(playerid)]][gv_iAttachedObjectID][0], E_STREAMER_WORLD_ID, vw[0]);

					}
					if(DynVehicleInfo[DynVeh[GetPlayerVehicleID(playerid)]][gv_iAttachedObjectModel][1] != INVALID_OBJECT_ID)
				    {
						Streamer_SetArrayData(STREAMER_TYPE_OBJECT, DynVehicleInfo[DynVeh[GetPlayerVehicleID(playerid)]][gv_iAttachedObjectID][1], E_STREAMER_WORLD_ID, vw[0]);

					}
				}
                foreach(new passenger: Player)
                {
                    if(passenger != playerid)
                    {
	                    if(IsPlayerInVehicle(passenger, GetPlayerVehicleID(playerid)))
	                    {
		                    SetPlayerInterior(passenger,DDoorsInfo[i][ddInteriorInt]);
				            PlayerInfo[passenger][pInt] = DDoorsInfo[i][ddInteriorInt];
				            PlayerInfo[passenger][pVW] = DDoorsInfo[i][ddInteriorVW];
				            SetPlayerVirtualWorld(passenger, DDoorsInfo[i][ddInteriorVW]);
	                    }
					}
                }
            }
            else {
                SetPlayerPos(playerid,DDoorsInfo[i][ddInteriorX],DDoorsInfo[i][ddInteriorY],DDoorsInfo[i][ddInteriorZ]);
                SetPlayerFacingAngle(playerid,DDoorsInfo[i][ddInteriorA]);
                SetCameraBehindPlayer(playerid);
            }
			if(DDoorsInfo[i][ddCustomInterior]) Player_StreamPrep(playerid, DDoorsInfo[i][ddInteriorX],DDoorsInfo[i][ddInteriorY],DDoorsInfo[i][ddInteriorZ], FREEZE_TIME);
            return 1;
        }
    }
    for(new i = 0; i < sizeof(FamilyInfo); i++) {
        if (IsPlayerInRangeOfPoint(playerid,3.0,FamilyInfo[i][FamilyEntrance][0], FamilyInfo[i][FamilyEntrance][1], FamilyInfo[i][FamilyEntrance][2])) {
            SetPlayerInterior(playerid,FamilyInfo[i][FamilyInterior]);
            PlayerInfo[playerid][pInt] = FamilyInfo[i][FamilyInterior];
            PlayerInfo[playerid][pVW] = FamilyInfo[i][FamilyVirtualWorld];
            SetPlayerVirtualWorld(playerid, FamilyInfo[i][FamilyVirtualWorld]);
            SetPlayerPos(playerid,FamilyInfo[i][FamilyExit][0],FamilyInfo[i][FamilyExit][1],FamilyInfo[i][FamilyExit][2]);
            SetPlayerFacingAngle(playerid,FamilyInfo[i][FamilyExit][3]);
			if(FamilyInfo[i][FamilyCustomMap]) Player_StreamPrep(playerid, FamilyInfo[i][FamilyExit][0],FamilyInfo[i][FamilyExit][1],FamilyInfo[i][FamilyExit][2], FREEZE_TIME);
            return 1;
        }
    }
    // [HOUSE REWORK] commented out - replaced by House_TryEnter in house.inc
    /*
    for(new i = 0; i < sizeof(HouseInfo); i++) {
        if (IsPlayerInRangeOfPoint(playerid,3,HouseInfo[i][hExteriorX], HouseInfo[i][hExteriorY], HouseInfo[i][hExteriorZ]) && GetPlayerInterior(playerid) == HouseInfo[i][hExtIW] && GetPlayerVirtualWorld(playerid) == HouseInfo[i][hExtVW]) {
            if(PlayerInfo[playerid][pPhousekey] == i || PlayerInfo[playerid][pPhousekey2] == i || HouseInfo[i][hLock] == 0 || PlayerInfo[playerid][pRenting] == i) {
                if(HouseInfo[i][hInteriorX] == 0.0) {
                    return SendClientMessageEx(playerid, COLOR_GRAD2, "Nha nay chua co noi that ben trong.");
                }
                SetPlayerInterior(playerid,HouseInfo[i][hIntIW]);
                PlayerInfo[playerid][pInt] = HouseInfo[i][hIntIW];
                PlayerInfo[playerid][pVW] = HouseInfo[i][hIntVW];
                SetPlayerVirtualWorld(playerid,HouseInfo[i][hIntVW]);
                SetPlayerPos(playerid,HouseInfo[i][hInteriorX],HouseInfo[i][hInteriorY],HouseInfo[i][hInteriorZ]);
                SetPlayerFacingAngle(playerid,HouseInfo[i][hInteriorA]);
                SetCameraBehindPlayer(playerid);
                GameTextForPlayer(playerid, "~w~Chao mung ve nha", 5000, 1);
				if(HouseInfo[i][hCustomInterior] == 1) Player_StreamPrep(playerid, HouseInfo[i][hInteriorX],HouseInfo[i][hInteriorY],HouseInfo[i][hInteriorZ], FREEZE_TIME);
            }
            else GameTextForPlayer(playerid, "~r~Khoa", 5000, 1);
            return 1;
        }
    }
    */

    for(new i = 0; i < sizeof(Businesses); i++) {
        if (IsPlayerInRangeOfPoint(playerid,3,Businesses[i][bExtPos][0], Businesses[i][bExtPos][1], Businesses[i][bExtPos][2])) {
	        if (Businesses[i][bExtPos][1] == 0.0) return 1;
			if (Businesses[i][bStatus]) {
                if(Businesses[i][bIntPos][0] == 0.0) {
                    return SendClientMessageEx(playerid, COLOR_GRAD2, "Cua hang nay chua co noi that ben trong.");
                }
				if (Businesses[i][bType] == BUSINESS_TYPE_GYM)
				{
					if (Businesses[i][bGymEntryFee] > 0 && PlayerInfo[playerid][pCash] < Businesses[i][bGymEntryFee])
					{
						GameTextForPlayer(playerid, "~r~Ban can them tien de vao phong tap the duc nay", 5000, 1);
						return 1;
					}
				}
                else if (Businesses[i][bType] == BUSINESS_TYPE_STORE)
				{
                    SendHintMessage(playerid, "Su dung lenh \"{01A407}/mua{FFFFFF}\" de mua hang tai day.");
                }
                else if (Businesses[i][bType] == BUSINESS_TYPE_RESTAURANT)
				{
                    SendHintMessage(playerid, "Su dung lenh \"{01A407}/muathucan{FFFFFF}\" hoac \"{01A407}an [Y]{FFFFFF}\" de mua thuc an tai day.");
                }

				SetPVarInt(playerid, "BusinessesID", i);

				if(Businesses[i][bVW] == 0) SetPlayerVirtualWorld(playerid, BUSINESS_BASE_VW + i), PlayerInfo[playerid][pVW] = BUSINESS_BASE_VW + i;
				else SetPlayerVirtualWorld(playerid, Businesses[i][bVW]), PlayerInfo[playerid][pVW] = Businesses[i][bVW];


				SetPlayerInterior(playerid,Businesses[i][bInt]);
	            SetPlayerPos(playerid,Businesses[i][bIntPos][0],Businesses[i][bIntPos][1],Businesses[i][bIntPos][2]);
		        SetPlayerFacingAngle(playerid, Businesses[i][bIntPos][3]);
	         	SetCameraBehindPlayer(playerid);
		        PlayerInfo[playerid][pInt] = Businesses[i][bInt];
		        if(Businesses[i][bCustomInterior]) Player_StreamPrep(playerid, Businesses[i][bIntPos][0], Businesses[i][bIntPos][1], Businesses[i][bIntPos][2], FREEZE_TIME);

				if (Businesses[i][bType] == BUSINESS_TYPE_GYM)
				{
					new string[50];
					format(string, sizeof(string), "Ban da tham gia gym va duoc tinh $%i.", Businesses[i][bGymEntryFee]);
					SendClientMessageEx(playerid, COLOR_WHITE, string);
					GivePlayerCash(playerid, -Businesses[i][bGymEntryFee]);
					Businesses[i][bSafeBalance] += Businesses[i][bGymEntryFee];

					if (Businesses[i][bGymType] == 1)
					{
						SendClientMessageEx(playerid, COLOR_WHITE, "Su dung /beginswimming o noi bat dau ho noi.");
						SendClientMessageEx(playerid, COLOR_WHITE, "Su dung /joinboxing de tham gia cac hoat dong.");
					}
					else if (Businesses[i][bGymType] == 2)
					{
						SendClientMessageEx(playerid, COLOR_WHITE, "Su dung /beginparkour de bat dau packour xe.");
					}
				}
			}
			else GameTextForPlayer(playerid, "~r~Dong", 5000, 1);
			return 1;
        }
    }

    new Float:X, Float:Y, Float:Z;
    GetDynamicObjectPos(Carrier[0], X, Y, Z);
    if(IsPlayerInRangeOfPoint(playerid, 2.0, (X-0.377671),(Y-10.917018),11.6986)) {
		//Battle Carrier
        SetPlayerInterior(playerid, 1);
        SetPlayerVirtualWorld(playerid, 7);
        PlayerInfo[playerid][pVW] = 7;
        Streamer_UpdateEx(playerid, 1170.0106201172,-1355.0770263672,2423.0461425781);
        //Old Hospital: 2087.4626,2806.0630,-16.1744
        SetPlayerPos(playerid,1170.0106201172,-1355.0770263672,2423.0461425781);
        PlayerInfo[playerid][pInt] = 1;
        Player_StreamPrep(playerid, 1170.0106201172,-1355.0770263672,2423.0461425781, FREEZE_TIME);
        return 1;
    }
    else if(IsPlayerInRangeOfPoint(playerid, 2.0, (X-6.422671),(Y-10.898918),11.6986)) {
                                                  //Battle Carrier Armoury
        SetPlayerInterior(playerid, 6);
        SetPlayerVirtualWorld(playerid, 1337);
        SetPlayerPos(playerid, 316.4553,-170.2923,999.5938);
        PlayerInfo[playerid][pVW] = 1337;
        return 1;
    }
    else if(IsPlayerInRangeOfPoint(playerid, 2.0, 306.4042,-159.0768,999.5938)) {
                                                  //Battle Carrier Armoury
        SetPlayerPos(playerid, 305.6966,-159.1586,999.5938);
        return 1;
    }
    else if(IsPlayerInRangeOfPoint(playerid, 2.0, (X-5.560629),(Y-3.860818),11.6986)) {
                                                  //Engine Room Entrance
        SetPlayerInterior(playerid, 17);
        SetPlayerVirtualWorld(playerid, 1337);
        SetPlayerPos(playerid, -959.6347,1956.4598,9.0000);
        PlayerInfo[playerid][pVW] = 1337;
        return 1;
    }
    else if(IsPlayerInRangeOfPoint(playerid, 2.0, (X-15.382171),(Y-2.272918),11.6986)) {
                                                  //Briefing Room Entrance
        SetPlayerInterior(playerid, 3);
        SetPlayerVirtualWorld(playerid, 1337);
        SetPlayerPos(playerid, 1494.3763,1303.5875,1093.2891);
        PlayerInfo[playerid][pVW] = 1337;
        return 1;
    }
//NG/LEO Only Entrance

                                                  // Hitman HQ
    else if(IsPlayerInRangeOfPoint(playerid, 2.0, -418.95, -1759.26, 6.22 )) {
        	if (arrGroupData[PlayerInfo[playerid][pMember]][g_iGroupType] == 2 || arrGroupData[PlayerInfo[playerid][pLeader]][g_iGroupType] == 2) {
            SetPlayerVirtualWorld(playerid, 666420);
            PlayerInfo[playerid][pVW] = 666420;
            SetPlayerInterior(playerid, 6);
            PlayerInfo[playerid][pInt] = 6;
            SetPlayerPos(playerid, -68.8239,1351.4331,1080.6257);
            SetPlayerFacingAngle(playerid, 160);
            SetCameraBehindPlayer(playerid);
            Player_StreamPrep(playerid, -68.8239,1351.4331,1080.6257, FREEZE_TIME);
        }
        return 1;
    }
                                                  // Hitman HQ
    else if(IsPlayerInRangeOfPoint(playerid, 2.0, 2323.3135, 7.6760, 26.5640)) {
        if (arrGroupData[PlayerInfo[playerid][pMember]][g_iGroupType] == 2 || arrGroupData[PlayerInfo[playerid][pLeader]][g_iGroupType] == 2) {
            SetPlayerVirtualWorld(playerid, 666421);
            PlayerInfo[playerid][pVW] = 666421;
            SetPlayerInterior(playerid, 6);
            PlayerInfo[playerid][pInt] = 6;
            SetPlayerPos(playerid, -68.8239,1351.4331,1080.6257);
            SetPlayerFacingAngle(playerid, 160);
            SetCameraBehindPlayer(playerid);
            Player_StreamPrep(playerid, -68.8239,1351.4331,1080.6257, FREEZE_TIME);
        }
        return 1;
    }
                                                  //Garage
    else if (IsPlayerInRangeOfPoint(playerid,8.0,1547.1947,29.8561,24.1406)) {
        GameTextForPlayer(playerid, "~w~GARAGE", 5000, 1);
        if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
            SetVehiclePos(GetPlayerVehicleID(playerid), -1790.378295,1436.949829,7.187500);
        }
        else {
            SetPlayerPos(playerid,-1790.378295,1436.949829,7.187500);
        }
        return 1;
    }

                                                      //VIP Garage
    else if (IsPlayerInRangeOfPoint(playerid,12.0,1814.6857,-1559.2028,13.4834)) {
        if(PlayerInfo[playerid][pDonateRank] > 0) {
            if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
                SetVehiclePos(GetPlayerVehicleID(playerid),  2425.8677,-1644.1337,1015.2882);
                SetVehicleZAngle(GetPlayerVehicleID(playerid),  180);
            }
            else {
                Streamer_UpdateEx(playerid, 2425.8677,-1644.1337,1015.2882);
                SetPlayerPos(playerid,2425.8677,-1644.1337,1015.2882);
                SetPlayerFacingAngle(playerid, 180);
            }
        }
        else {
            SendClientMessage(playerid, COLOR_WHITE, "* Ban khong phai la VIP!");
        }
        return 1;
	}
                                                  // LSPD Panel
    else if(IsPlayerInRangeOfPoint(playerid, 3.0, 1575.9766845703, -1636.4899902344, 13.555115699768)) {
        if(IsACop(playerid)) {
            SetPlayerInterior(playerid, 0);
            PlayerInfo[playerid][pInt] = 0;
            SetPlayerVirtualWorld(playerid, 0);
            SetPlayerPos(playerid,1579.0098876953, -1636.2879638672, 13.554491043091);
            SetPlayerFacingAngle(playerid, 190.0520);
            SetCameraBehindPlayer(playerid);
            PlayerInfo[playerid][pVW] = 0;
        }
        else {
            SendClientMessageEx(playerid, COLOR_GREY, "You do not have the keys for that door (LSPD restricted access)!");
        }
        return 1;
    }
     // DoC Exterior Entrance
    else if(IsPlayerInRangeOfPoint(playerid, 3.0, -2033.7502, -154.8784, 35.3203)) {
        if(BackEntrance) {
            SetPlayerInterior(playerid, 0);
            PlayerInfo[playerid][pInt] = 0;
            SetPlayerVirtualWorld(playerid, 0);
            SetPlayerPos(playerid,-2045.0183, -211.6728, 991.5364);
            SetCameraBehindPlayer(playerid);
            PlayerInfo[playerid][pVW] = 0;
            SetCameraBehindPlayer(playerid);
            Player_StreamPrep(playerid, -2045.0183, -211.6728, 991.5364, FREEZE_TIME);
        }
        else {
            SendClientMessageEx(playerid, COLOR_GREY, "You do not have the keys for that door (locked)!");
        }
        return 1;
    }
    else if(IsPlayerInRangeOfPoint(playerid, 3.0, -2088.4797, -199.6259, 978.8315)) {
        if(IsACop(playerid)) {
            SetPlayerPos(playerid,-2091.0200,-199.8031,978.8315);
        }
        else {
            SendClientMessageEx(playerid, COLOR_GREY, "You do not have the keys for that door (SFPD restricted)!");
        }
        return 1;
    }
                                                  //SASD - Elevator
    else if (IsPlayerInRangeOfPoint(playerid,3.0,2530.3774,-1689.9998,562.7922)) {
        if(IsACop(playerid)) {
            SetPlayerFacingAngle(playerid, 359);
            GameTextForPlayer(playerid, "~w~GARAGE", 5000, 1);
            SetPlayerPos(playerid,612.6414,-629.8659,-4.0447);
            SetPlayerVirtualWorld(playerid, 0);
            PlayerInfo[playerid][pVW] = 0;
			Player_StreamPrep(playerid, 612.6414,-629.8659,-4.0447, FREEZE_TIME);
        }
        else {
            return SendClientMessageEx(playerid, COLOR_GRAD2, "Access Denied.");
        }
        return 1;
    }                                                //Gym Garage
    else if (IsPlayerInRangeOfPoint(playerid,8.0,2273.6787,-1728.9022,13.5039)) {
        if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
            SetVehiclePos(GetPlayerVehicleID(playerid),  2240.7332,-1695.9648,-0.0826 + 1100);
            SetVehicleZAngle(GetPlayerVehicleID(playerid),  180);
        }
        else {
            Streamer_UpdateEx(playerid, 2240.7332,-1695.9648,-0.0826 + 1100);
            SetPlayerPos(playerid,2240.7332,-1695.9648,-0.0826 + 1100);
            SetPlayerFacingAngle(playerid, 180);
        }
        return 1;
    }
                                                  //Courthouse Garage
    else if (IsPlayerInRangeOfPoint(playerid,8.0,1423.0443,-1664.5244,13.5810)) {
        if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
            SetVehiclePos(GetPlayerVehicleID(playerid),  1381.7274,-1676.4248,-13.2229 + 1100);
            SetVehicleZAngle(GetPlayerVehicleID(playerid),  90);
        }
        else {
            Streamer_UpdateEx(playerid,1381.7274,-1676.4248,-13.2229 + 1100);
            SetPlayerPos(playerid,1381.7274,-1676.4248,-13.2229 + 1100);
            SetPlayerFacingAngle(playerid, 90);
        }
        return 1;
    }
                                                  //PB Garage
    else if (IsPlayerInRangeOfPoint(playerid,8.0,1218.69,-1425.01,13.15)) {
        if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
            SetVehiclePos(GetPlayerVehicleID(playerid),  1218.8041,-1449.8579,-46.2308 + 1100);
            SetVehicleZAngle(GetPlayerVehicleID(playerid), 360 + 1100);
        }
        else {
            Streamer_UpdateEx(playerid, 1218.8041,-1449.8579,-46.2308 + 1100);
            SetPlayerPos(playerid,1218.8041,-1449.8579,-46.2308 + 1100);
            SetPlayerFacingAngle(playerid, 90);
        }
        return 1;
    }
                                                  // LAOfficeBuilding Roof Entrance
    else if (IsPlayerInRangeOfPoint(playerid,3.0,1779.0928,-1302.7775,131.7344)) {
        SetPlayerPos(playerid, 1771.0253,-1302.8596,125.7209);
        SetPlayerFacingAngle(playerid, 280);
        return 1;
    }
                                                  // LAOfficeBuilding Roof Entrance
    else if (IsPlayerInRangeOfPoint(playerid,3.0,1771.0253,-1302.8596,125.7209)) {
        SetPlayerPos(playerid,1779.0928,-1302.7775,131.7344);
        SetPlayerFacingAngle(playerid, 280);
        return 1;
    }
                                                  // Gold/Plat VIP
    else if (IsPlayerInRangeOfPoint(playerid,3.0,902.2482,1419.8180,-80.9308)) {
        if(PlayerInfo[playerid][pDonateRank] < 3) {
            SendClientMessageEx(playerid, COLOR_WHITE, "* You are not Gold or Platinum VIP!");
        }
        else {
            SetPlayerPos(playerid,911.6200,1426.2729,-81.1762);
            SetPlayerFacingAngle(playerid, 270);
        }
        return 1;
    }

	else if (cCar != INVALID_VEHICLE_ID && (GetVehicleModel(cCar) == 519 || GetVehicleModel(cCar) == 553) && IsPlayerInRangeOfVehicle(playerid, cCar, 5.0) && GetPlayerVehicleID(playerid) != cCar)
	{
	    if(VehicleStatus{cCar} == 1) return SendClientMessageEx(playerid, COLOR_WHITE, "Ban khong duc su dung!");
	    new string[47 + MAX_PLAYER_NAME];
   		format(string, sizeof(string), "* %s da duoc chan nhan goi.", GetPlayerNameEx(playerid));
		ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);

		switch(GetVehicleModel(cCar)) {
			case 519: { // Shamal
				SetPlayerPos(playerid, 2.509036, 23.118730, 1199.593750);
				SetPlayerFacingAngle(playerid, 82.14);
				PlayerInfo[playerid][pInt] = 1;
				SetPlayerInterior(playerid, 1);
			}
			case 553: { // Nevada
				SetPlayerPos(playerid, 315.9396, 973.2628, 1961.5985);
				SetPlayerFacingAngle(playerid, 2.7);
				PlayerInfo[playerid][pInt] = 9;
				SetPlayerInterior(playerid, 9);
			}
		}


        SetCameraBehindPlayer(playerid);
		PlayerInfo[playerid][pVW] = cCar;
		SetPlayerVirtualWorld(playerid, cCar);
		InsidePlane[playerid] = cCar;
		SendClientMessageEx(playerid, COLOR_WHITE, "Type /exit near the door to exit the vehicle, or /window to look outside.");
        return 1;
    }
    return 0;
}

stock PlayerExitInterior(playerid)
{
    if(GetPVarInt(playerid, "IsInArena") >= 0 || GetPVarType(playerid, "StreamPrep") || PlayerCuffed[playerid] >= 1 || PlayerInfo[playerid][pBeingSentenced] != 0) 
        return 1;
    
    for(new i = 0; i <  sizeof(DDoorsInfo); i++) {
        if (IsPlayerInRangeOfPoint(playerid,3.0,DDoorsInfo[i][ddInteriorX], DDoorsInfo[i][ddInteriorY], DDoorsInfo[i][ddInteriorZ]) && PlayerInfo[playerid][pVW] == DDoorsInfo[i][ddInteriorVW]) {
            SetPlayerInterior(playerid,DDoorsInfo[i][ddExteriorInt]);
            PlayerInfo[playerid][pInt] = DDoorsInfo[i][ddExteriorInt];
            SetPlayerVirtualWorld(playerid, DDoorsInfo[i][ddExteriorVW]);
            PlayerInfo[playerid][pVW] = DDoorsInfo[i][ddExteriorVW];
            SetPlayerToTeamColor(playerid);
            if(DDoorsInfo[i][ddVehicleAble] > 0 && GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
                SetVehiclePos(GetPlayerVehicleID(playerid), DDoorsInfo[i][ddExteriorX],DDoorsInfo[i][ddExteriorY],DDoorsInfo[i][ddExteriorZ]);
                SetVehicleZAngle(GetPlayerVehicleID(playerid), DDoorsInfo[i][ddExteriorA]);
                SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), DDoorsInfo[i][ddExteriorVW]);
                LinkVehicleToInterior(GetPlayerVehicleID(playerid), DDoorsInfo[i][ddExteriorInt]);
                if(GetPVarInt(playerid, "tpForkliftTimer") > 0)
				{
				    SetPVarInt(playerid, "tpJustEntered", 1);
				    new Float: pX, Float: pY, Float: pZ;
					GetPlayerPos(playerid, pX, pY, pZ);
					SetPVarFloat(playerid, "tpForkliftX", pX);
			 		SetPVarFloat(playerid, "tpForkliftY", pY);
			  		SetPVarFloat(playerid, "tpForkliftZ", pZ);
				}
                if(DynVeh[GetPlayerVehicleID(playerid)] != -1)
				{
				    new vw[1];
					vw[0] = GetVehicleVirtualWorld(GetPlayerVehicleID(playerid));
				    if(DynVehicleInfo[DynVeh[GetPlayerVehicleID(playerid)]][gv_iAttachedObjectModel][0] != INVALID_OBJECT_ID)
				    {
				    	Streamer_SetArrayData(STREAMER_TYPE_OBJECT, DynVehicleInfo[DynVeh[GetPlayerVehicleID(playerid)]][gv_iAttachedObjectID][0], E_STREAMER_WORLD_ID, vw[0]);

					}
					if(DynVehicleInfo[DynVeh[GetPlayerVehicleID(playerid)]][gv_iAttachedObjectModel][1] != INVALID_OBJECT_ID)
				    {
						Streamer_SetArrayData(STREAMER_TYPE_OBJECT, DynVehicleInfo[DynVeh[GetPlayerVehicleID(playerid)]][gv_iAttachedObjectID][1], E_STREAMER_WORLD_ID, vw[0]);

					}
				}
                foreach(new passenger: Player)
                {
                    if(passenger != playerid)
                    {
	                    if(IsPlayerInVehicle(passenger, GetPlayerVehicleID(playerid)))
	                    {
		                    SetPlayerInterior(passenger,DDoorsInfo[i][ddExteriorInt]);
				            PlayerInfo[passenger][pInt] = DDoorsInfo[i][ddExteriorInt];
				            PlayerInfo[passenger][pVW] = DDoorsInfo[i][ddExteriorVW];
				            SetPlayerVirtualWorld(passenger, DDoorsInfo[i][ddExteriorVW]);
	                    }
					}
                }
            }
            else {
                SetPlayerPos(playerid,DDoorsInfo[i][ddExteriorX],DDoorsInfo[i][ddExteriorY],DDoorsInfo[i][ddExteriorZ]);
                SetPlayerFacingAngle(playerid, DDoorsInfo[i][ddExteriorA]);
                SetCameraBehindPlayer(playerid);
            }
			if(DDoorsInfo[i][ddCustomExterior]) Player_StreamPrep(playerid, DDoorsInfo[i][ddExteriorX],DDoorsInfo[i][ddExteriorY],DDoorsInfo[i][ddExteriorZ], FREEZE_TIME);
            return 1;
        }
    }
    for(new i = 0; i < sizeof(FamilyInfo); i++) {
        if (IsPlayerInRangeOfPoint(playerid,3.0,FamilyInfo[i][FamilyExit][0], FamilyInfo[i][FamilyExit][1], FamilyInfo[i][FamilyExit][2]) && PlayerInfo[playerid][pVW] == FamilyInfo[i][FamilyVirtualWorld]) {
            PlayerInfo[playerid][pInt] = 0;
            SetPlayerVirtualWorld(playerid, 0);
            SetPlayerInterior(playerid,0);
            Streamer_UpdateEx(playerid, FamilyInfo[i][FamilyEntrance][0],FamilyInfo[i][FamilyEntrance][1],FamilyInfo[i][FamilyEntrance][2]);
            SetPlayerPos(playerid,FamilyInfo[i][FamilyEntrance][0],FamilyInfo[i][FamilyEntrance][1],FamilyInfo[i][FamilyEntrance][2]);
            SetPlayerFacingAngle(playerid,FamilyInfo[i][FamilyEntrance][3]);
            return 1;
        }
    }
    // [HOUSE REWORK] commented out - replaced by House_TryExit in house.inc
    /*
    for(new i = 0; i <  sizeof(HouseInfo); i++) {
        if (IsPlayerInRangeOfPoint(playerid,3.0,HouseInfo[i][hInteriorX], HouseInfo[i][hInteriorY], HouseInfo[i][hInteriorZ]) && PlayerInfo[playerid][pVW] == HouseInfo[i][hIntVW]) {
			SetPlayerInterior(playerid,0);
            PlayerInfo[playerid][pInt] = 0;
            SetPlayerPos(playerid,HouseInfo[i][hExteriorX],HouseInfo[i][hExteriorY],HouseInfo[i][hExteriorZ]);
            SetPlayerFacingAngle(playerid, HouseInfo[i][hExteriorA]);
            SetCameraBehindPlayer(playerid);
            SetPlayerVirtualWorld(playerid, HouseInfo[i][hExtVW]);
            PlayerInfo[playerid][pVW] = HouseInfo[i][hExtVW];
			PlayerInfo[playerid][pInt] = HouseInfo[i][hExtIW];
            SetPlayerInterior(playerid, HouseInfo[i][hExtIW]);
			if(HouseInfo[i][hCustomExterior]) Player_StreamPrep(playerid, HouseInfo[i][hExteriorX],HouseInfo[i][hExteriorY],HouseInfo[i][hExteriorZ], FREEZE_TIME);
            return 1;
        }
    }
    */
    for(new i = 0; i < sizeof(Businesses); i++) {
		if (IsPlayerInRangeOfPoint(playerid,3,Businesses[i][bIntPos][0], Businesses[i][bIntPos][1], Businesses[i][bIntPos][2])) {
		    if(Businesses[i][bVW] == 0 && PlayerInfo[playerid][pVW] == BUSINESS_BASE_VW + i || Businesses[i][bVW] != 0 && PlayerInfo[playerid][pVW] == Businesses[i][bVW]) {
				SetPlayerInterior(playerid, 0);
				SetPlayerVirtualWorld(playerid, 0);
        	    SetPlayerPos(playerid,Businesses[i][bExtPos][0],Businesses[i][bExtPos][1],Businesses[i][bExtPos][2]);
        	    SetPlayerFacingAngle(playerid, Businesses[i][bExtPos][3]);
        	    SetCameraBehindPlayer(playerid);
				PlayerInfo[playerid][pInt] = 0;
        	    PlayerInfo[playerid][pVW] = 0;
 				DeletePVar(playerid, "BusinessesID");
           	 	if(Businesses[i][bCustomExterior]) Player_StreamPrep(playerid, Businesses[i][bExtPos][0], Businesses[i][bExtPos][1], Businesses[i][bExtPos][2], FREEZE_TIME);
            	return 1;
			}
        }
    }

    if (IsPlayerInRangeOfPoint(playerid, 3.0, 1753.6423,-1586.9365,13.2424)) {
        if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
            SetVehiclePos(GetPlayerVehicleID(playerid), 1753.4561,-1595.1804,13.5381);
            SetVehicleZAngle(GetPlayerVehicleID(playerid), 347.7918);
        }
        else {
            SetPlayerPos(playerid,1753.4561,-1595.1804,13.5381);
        }
        return 1;
    }
    else if(InsidePlane[playerid] != INVALID_VEHICLE_ID)
	{
	    if(GetPVarType(playerid, "air_Mode")) {
	        SendClientMessageEx(playerid, COLOR_GREY, "You must stop looking out the window! (/window)");
	        return 1;
	    }
	    new string[64];
        format(string, sizeof(string), "* %s exits the airplane.", GetPlayerNameEx(playerid));
        ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);

        if(!IsAPlane(InsidePlane[playerid])) {
            PlayerInfo[playerid][pAGuns][GetWeaponSlot(46)] = 46;
            GivePlayerValidWeapon(playerid, 46, 60000);
            SetPlayerPos(playerid, 0.000000, 0.000000, 420.000000); // lol nick
        }
        else {
            new Float:X, Float:Y, Float:Z;
            GetVehiclePos(InsidePlane[playerid], X, Y, Z);
            SetPlayerPos(playerid, X-2.7912, Y+3.2304, Z);
			Player_StreamPrep(playerid, X-2.7912,Y+3.2304,Z, FREEZE_TIME);
            if(Z > 50.0) {
                PlayerInfo[playerid][pAGuns][GetWeaponSlot(46)] = 46;
                GivePlayerValidWeapon(playerid, 46, 60000);
            }
        }

        PlayerInfo[playerid][pVW] = 0;
        SetPlayerVirtualWorld(playerid, 0);
        PlayerInfo[playerid][pInt] = 0;
        SetPlayerInterior(playerid, 0);
        InsidePlane[playerid] = INVALID_VEHICLE_ID;
        return 1;
    }
    else if(IsPlayerInRangeOfPoint(playerid, 3.0, -2045.0183,-211.6728,991.5364)) {
        if(BackEntrance) {
         	SetPlayerInterior(playerid, 0);
	        SetPlayerVirtualWorld(playerid, 0);
	        SetPlayerPos(playerid, -2033.7502,-154.8784,35.3203);
		}
		else SendClientMessageEx(playerid, COLOR_GREY, "You don't have the keys for that door (locked) !");
        return 1;
    }
    else if(IsPlayerInRangeOfPoint(playerid, 3.0, -2091.0200,-199.8031,978.8315)) {
        if(IsACop(playerid)) {
	        SetPlayerPos(playerid, -2088.4797,-199.6259,978.8315);
        }
		else SendClientMessageEx(playerid, COLOR_GREY, "You don't have the keys for that door (LEO restricted) !");
        return 1;
    }
    else if(IsPlayerInRangeOfPoint(playerid, 3.0, 1169.67, -1356.32, 2423.04) && GetPlayerVirtualWorld(playerid) == 7) {
                                                  //Battle Carrier
        SetPlayerInterior(playerid, 0);
        SetPlayerVirtualWorld(playerid, 0);
        SetPlayerFacingAngle(playerid, 180);
        new Float:X, Float:Y, Float:Z;
        GetDynamicObjectPos(Carrier[0], X, Y, Z);
        SetPlayerPos(playerid, (X-0.377671),(Y-10.917018),11.6986);
        return 1;
    }
    else if(IsPlayerInRangeOfPoint(playerid, 2.0, 316.4553,-170.2923,999.5938) && GetPlayerVirtualWorld(playerid) == 1337) {
                                                  //Battle Carrier Armoury
        SetPlayerInterior(playerid, 0);
        SetPlayerVirtualWorld(playerid, 0);
        PlayerInfo[playerid][pVW] = 0;
        new Float:X, Float:Y, Float:Z;
        GetDynamicObjectPos(Carrier[0], X, Y, Z);
        SetPlayerPos(playerid, (X-6.422671),(Y-10.898918),11.6986);
        return 1;
    }
    else if(IsPlayerInRangeOfPoint(playerid, 2.0, 305.6966,-159.1586,999.5938)&& GetPlayerVirtualWorld(playerid) == 1337) {
                                                  //Battle Carrier Armoury
        SetPlayerPos(playerid, 306.4042,-159.0768,999.5938);
        return 1;
    }
    else if(IsPlayerInRangeOfPoint(playerid, 2.0, -959.6347,1956.4598,9.0000) && GetPlayerVirtualWorld(playerid) == 1337) {
                                                  //Engine Room Exit
        SetPlayerInterior(playerid, 0);
        SetPlayerVirtualWorld(playerid, 0);
        new Float:X, Float:Y, Float:Z;
        GetDynamicObjectPos(Carrier[0], X, Y, Z);
        SetPlayerPos(playerid, (X-5.560629),(Y-3.853518),11.6986);
        PlayerInfo[playerid][pVW] = 0;
        return 1;
    }
    else if(IsPlayerInRangeOfPoint(playerid, 2.0, 1494.3763,1303.5875,1093.2891) && GetPlayerVirtualWorld(playerid) == 1337) {
                                                  //Briefing Room Exit
        SetPlayerInterior(playerid, 0);
        SetPlayerVirtualWorld(playerid, 0);
        new Float:X, Float:Y, Float:Z;
        GetDynamicObjectPos(Carrier[0], X, Y, Z);
        SetPlayerPos(playerid, (X-15.382171),(Y-2.272918),11.6986);
        PlayerInfo[playerid][pVW] = 0;
        return 1;
    }
    else if(IsPlayerInRangeOfPoint(playerid, 2.0, 971.70568847656, 0.73652172088623, 1013.0336914063)) {
        if(PlayerInfo[playerid][pFMember] == 1) {
            SetPlayerPos(playerid, 971.75524902344, 1.949291229248, 1012.8675537109);
            SetCameraBehindPlayer(playerid);
            Player_StreamPrep(playerid, 971.75524902344, 1.949291229248, 1012.8675537109, FREEZE_TIME);
        }
        else {
            SendClientMessageEx(playerid, COLOR_GREY, "You do not have the keys for that door (Hades' Children MC Members only)!");
        }
        return 1;
    }
    else if(IsPlayerInRangeOfPoint(playerid, 2.0, -265.52786254883, 1409.2824707031, -2.2975430488586)) {
        if(PlayerInfo[playerid][pFMember] == 1) {
            SetPlayerPos(playerid, -230.57353210449, 1409.3792724609, -5.2974662780762);
            SetPlayerVirtualWorld(playerid, 0);
            PlayerInfo[playerid][pVW] = 0;
            SetCameraBehindPlayer(playerid);
            SetPlayerInterior(playerid, 0);
            PlayerInfo[playerid][pInt] = 0;
			Player_StreamPrep(playerid, -230.57353210449, 1409.3792724609, -5.2974662780762, FREEZE_TIME);
        }
        else {
            SendClientMessageEx(playerid, COLOR_GREY, "You do not have the keys for that door (Hades' Children MC Members only)!");
        }
        return 1;
    }
    else if(IsPlayerInRangeOfPoint(playerid, 2.0, -270.8628,1417.2441,-2.2902)) {
        if(PlayerInfo[playerid][pFMember] == 1) {
            SetPlayerPos(playerid, 255.84442138672, -158.21658325195, 1.677818775177);
            SetPlayerVirtualWorld(playerid, 0);
            PlayerInfo[playerid][pVW] = 0;
            SetCameraBehindPlayer(playerid);
            SetPlayerInterior(playerid, 0);
            PlayerInfo[playerid][pInt] = 0;
			Player_StreamPrep(playerid, 255.84442138672, -158.21658325195, 1.677818775177, FREEZE_TIME);
        }
        else {
            SendClientMessageEx(playerid, COLOR_GREY, "You do not have the keys for that door (Hades' Children MC Members only)!");
        }
        return 1;
    }
                                                  //Garage
    else if (IsPlayerInRangeOfPoint(playerid,8.0,-1790.378295,1436.949829,7.187500)) {
        if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
            SetVehiclePos(GetPlayerVehicleID(playerid), 1551.8052,31.0254,24.1446);
        }
        else {
            SetPlayerPos(playerid,1551.8052,31.0254,24.1446);
        }
        return 1;
    }
                                                        //VIP Garage
    else if (IsPlayerInRangeOfPoint(playerid,8.0,2425.9028,-1640.0483,1015.3889)) {
        if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
            SetVehiclePos(GetPlayerVehicleID(playerid),  1819.3533,-1560.3534,13.5469);
            SetVehicleZAngle(GetPlayerVehicleID(playerid),  253);
        }
        else {
            SetPlayerPos(playerid,1819.3533,-1560.3534,13.5469);
            SetPlayerFacingAngle(playerid,253);
        }
        return 1;
	}
                                                  // Hitman HQ
    else if(IsPlayerInRangeOfPoint(playerid, 2.0, -68.845336914063, 1351.3386230469, 1079.2109375 ) && (GetPlayerVirtualWorld(playerid) == 666420)) {
        	if (arrGroupData[PlayerInfo[playerid][pMember]][g_iGroupType] == 2 || arrGroupData[PlayerInfo[playerid][pLeader]][g_iGroupType] == 2) {
            SetPlayerVirtualWorld(playerid, 0);
            PlayerInfo[playerid][pVW] = 0;
            SetPlayerInterior(playerid, 0);
            PlayerInfo[playerid][pInt] = 0;
            SetPlayerPos(playerid, -418.95, -1759.26, 6.22);
            SetCameraBehindPlayer(playerid);
        }
        return 1;
    }
                                                  // Hitman HQ
    else if(IsPlayerInRangeOfPoint(playerid, 2.0, -68.845336914063, 1351.3386230469, 1079.2109375 ) && (GetPlayerVirtualWorld(playerid) == 666421)) {
        	if (arrGroupData[PlayerInfo[playerid][pMember]][g_iGroupType] == 2 || arrGroupData[PlayerInfo[playerid][pLeader]][g_iGroupType] == 2) {
            SetPlayerVirtualWorld(playerid, 0);
            PlayerInfo[playerid][pVW] = 0;
            SetPlayerInterior(playerid, 0);
            PlayerInfo[playerid][pInt] = 0;
            SetPlayerPos(playerid, 2323.3135, 7.6760, 26.5640);
            SetPlayerFacingAngle(playerid, 265.11);
            SetCameraBehindPlayer(playerid);
        }
        return 1;
    }
    else if(IsPlayerInRangeOfPoint(playerid, 3.0, 301.228820, 191.181884, 1007.171875)) {
                                                  //SFPD Exit to garage
        if(IsACop(playerid)) {
            SetPlayerInterior(playerid, 0);
            PlayerInfo[playerid][pInt] = 0;
            SetPlayerPos(playerid,-1591.450195, 716.007141, -5.242187);
            SetPlayerFacingAngle(playerid, 271.00);
            SetCameraBehindPlayer(playerid);
        }
        else {
            return SendClientMessageEx(playerid, COLOR_GRAD2, "Access Denied.");
        }
        return 1;
    }
                                                  //SASD - Elevator
    else if (IsPlayerInRangeOfPoint(playerid,3.0,612.6414,-629.8659,-4.0447)) {
        if(IsACop(playerid)) {
            SetPlayerFacingAngle(playerid, 268);
            GameTextForPlayer(playerid, "~w~SASD HQ", 5000, 1);
            SetPlayerPos(playerid,2530.3774,-1689.9998,562.7922);
            SetPlayerVirtualWorld(playerid, 1699);
            PlayerInfo[playerid][pVW] = 1699;
			Player_StreamPrep(playerid, 2530.3774,-1689.9998,562.7922, FREEZE_TIME);
        }
        else {
            return SendClientMessageEx(playerid, COLOR_GRAD2, "Access Denied.");
        }
        return 1;
    }
                                                  // FBI garage exit
    else if(IsPlayerInRangeOfPoint(playerid, 3,310.8672,-1484.7493,-33.5292 )) {
        if(IsACop(playerid)) {
            SetPlayerInterior(playerid,0);
            SetPlayerPos(playerid, 314.30,-1512.07,24.92);
            SetPlayerFacingAngle(playerid, 55.76);
            SetCameraBehindPlayer(playerid);
            PlayerInfo[playerid][pInt] = 0;
            SetPlayerVirtualWorld(playerid, 0);
            PlayerInfo[playerid][pVW] = 0;
        }
        else {
            SendClientMessageEx(playerid, COLOR_GREY, "You do not have the keys for that door (FBI restricted access)!");
        }
        return 1;
    }
    else if (IsPlayerInRangeOfPoint(playerid, 3, 291.8773,-1481.0674,-33.5308)) {
        if(IsACop(playerid)) {
            SetPlayerInterior(playerid,0);
            SetPlayerPos(playerid,298.26, -1544.53, 76.53);
            SetCameraBehindPlayer(playerid);
            PlayerInfo[playerid][pInt] = 0;
            SetPlayerVirtualWorld(playerid, 0);
            PlayerInfo[playerid][pVW] = 0;
        }
        else {
            SendClientMessageEx(playerid, COLOR_GREY, "You do not have the keys for that door (FBI restricted access)!");
        }
        return 1;
    }
    /*else if (IsPlayerInRangeOfPoint(playerid,6.0,-1404.5299,-259.0602,1043.6563)) {
        SetPlayerInterior(playerid,0);
        SetPlayerPos(playerid,2695.6235,-1704.6960,11.8438);
    }
    else if (IsPlayerInRangeOfPoint(playerid,8.0,-1443.0554,-581.1879,1055.0472)) {
        SetPlayerInterior(playerid,0);
        SetPlayerPos(playerid,-2111.5686,-443.9720,38.7344);
    }
    else if (IsPlayerInRangeOfPoint(playerid,8.0,-1464.7732,1557.5533,1052.5313)) {
        SetPlayerInterior(playerid,0);
        SetPlayerPos(playerid,-2080.3079,-406.0309,38.7344);
    }*/
                                                  // NG exit
    else if (IsPlayerInRangeOfPoint(playerid,4.0,-273.884765625,1875.1552734375,29.817853927612)) {
        if (PlayerInfo[playerid][pJailTime] == 0) {
            SetPlayerInterior(playerid,0);
            SetPlayerPos(playerid,200.2569,1869.5732,13.1470);
        }
        else {
            SetPlayerHealth(playerid, 0);
            SendClientMessageEx(playerid, COLOR_WHITE, "You can not escape admin prison!");
        }
        return 1;
    }
                                                  //Courthouse Garage
    else if (IsPlayerInRangeOfPoint(playerid,20.0,1381.7274,-1676.4248,-13.2229+1100.00)) {
        if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
            SetVehiclePos(GetPlayerVehicleID(playerid),  1423.0443,-1664.5244,13.5810);
            SetVehicleZAngle(GetPlayerVehicleID(playerid),  270);
        }
        else {
            SetPlayerPos(playerid,1423.0443,-1664.5244,13.5810);
            SetPlayerFacingAngle(playerid,270);
        }
        return 1;
    }
                                                  //Gym Garage
    else if (IsPlayerInRangeOfPoint(playerid,20.0,2240.7332,-1695.9648,(-0.0826+1100.00))) {
        if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
            SetVehiclePos(GetPlayerVehicleID(playerid),  2273.6787,-1728.9022,13.5039);
            SetVehicleZAngle(GetPlayerVehicleID(playerid),  180);
        }
        else {
            SetPlayerPos(playerid,2273.6787,-1728.9022,13.5039);
            SetPlayerFacingAngle(playerid,180);
        }
        return 1;
    }
                                                  //PB Garage
    else if (IsPlayerInRangeOfPoint(playerid,20.0,1218.60,-1451.69,(-46.85+1100.00))) {
        if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
            SetVehiclePos(GetPlayerVehicleID(playerid),  1218.6017,-1421.5887,13.9084);
            SetVehicleZAngle(GetPlayerVehicleID(playerid),  360);
        }
        else {
            SetPlayerPos(playerid,1218.6017,-1421.5887,13.9084);
        }
        return 1;
    }
                                           // Gold/Plat VIP
    else if (IsPlayerInRangeOfPoint(playerid,3.0, 911.6200,1426.2729,-81.1762)) {
        SetPlayerPos(playerid,902.2482,1419.8180,-80.9308);
        SetPlayerFacingAngle(playerid, 90);
        return 1;
    }
    return 0;
}

stock IsNearAnyDoor(playerid)
{
    new i;
    for(i = 0; i < sizeof(DDoorsInfo); i++) {
        if(IsPlayerInRangeOfPoint(playerid, 3.0, DDoorsInfo[i][ddExteriorX], DDoorsInfo[i][ddExteriorY], DDoorsInfo[i][ddExteriorZ]) && PlayerInfo[playerid][pVW] == DDoorsInfo[i][ddExteriorVW])
            return 1;
        if(IsPlayerInRangeOfPoint(playerid, 3.0, DDoorsInfo[i][ddInteriorX], DDoorsInfo[i][ddInteriorY], DDoorsInfo[i][ddInteriorZ]) && PlayerInfo[playerid][pVW] == DDoorsInfo[i][ddInteriorVW])
            return 1;
    }
    for(i = 0; i < sizeof(FamilyInfo); i++) {
        if(IsPlayerInRangeOfPoint(playerid, 3.0, FamilyInfo[i][FamilyEntrance][0], FamilyInfo[i][FamilyEntrance][1], FamilyInfo[i][FamilyEntrance][2]))
            return 1;
        if(IsPlayerInRangeOfPoint(playerid, 3.0, FamilyInfo[i][FamilyExit][0], FamilyInfo[i][FamilyExit][1], FamilyInfo[i][FamilyExit][2]) && PlayerInfo[playerid][pVW] == FamilyInfo[i][FamilyVirtualWorld])
            return 1;
    }
    for(i = 0; i < sizeof(HouseInfo); i++) {
        if(IsPlayerInRangeOfPoint(playerid, 3.0, HouseInfo[i][hExteriorX], HouseInfo[i][hExteriorY], HouseInfo[i][hExteriorZ]) && GetPlayerInterior(playerid) == HouseInfo[i][hExtIW] && GetPlayerVirtualWorld(playerid) == HouseInfo[i][hExtVW])
            return 1;
        if(IsPlayerInRangeOfPoint(playerid, 3.0, HouseInfo[i][hInteriorX], HouseInfo[i][hInteriorY], HouseInfo[i][hInteriorZ]) && PlayerInfo[playerid][pVW] == HouseInfo[i][hIntVW])
            return 1;
    }
    for(i = 0; i < sizeof(Businesses); i++) {
        if(IsPlayerInRangeOfPoint(playerid, 3.0, Businesses[i][bExtPos][0], Businesses[i][bExtPos][1], Businesses[i][bExtPos][2]))
            return 1;
        if(IsPlayerInRangeOfPoint(playerid, 3.0, Businesses[i][bIntPos][0], Businesses[i][bIntPos][1], Businesses[i][bIntPos][2])) {
            if(Businesses[i][bVW] == 0 && PlayerInfo[playerid][pVW] == BUSINESS_BASE_VW + i || Businesses[i][bVW] != 0 && PlayerInfo[playerid][pVW] == Businesses[i][bVW])
                return 1;
        }
    }
    return 0;
}

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if(PRESSED(KEY_YES)) {
        if(!PlayerEnterInterior(playerid)) {
            PlayerExitInterior(playerid);
        }
    }
    return 1;
}

alias:enter("vao")
CMD:enter(playerid, params[])
{
    if(GetPVarInt(playerid, "IsInArena") >= 0) {
        SendClientMessageEx(playerid, COLOR_WHITE, "Ban khong the lam dieu nay tai dau' truong!");
        return 1;
    }
	if(GetPVarType(playerid, "StreamPrep")) {
		return SendClientMessageEx(playerid, COLOR_WHITE, "Ban khong the lam dieu do vao luc nay , hay doi den khi khi ket thuc.");
	}
    if( PlayerCuffed[playerid] >= 1 ) {
        SendClientMessageEx(playerid, COLOR_WHITE, "Ban khong the lam dieu nay ngay bay gio.");
        return 1;
    }

	PlayerEnterInterior(playerid);
    return 1;
}

alias:exit("ra")
CMD:exit(playerid, params[])
{
	if (GetPVarInt(playerid, "_BikeParkourStage") > 0)
	{
		SendClientMessageEx(playerid, COLOR_GRAD2, "You must finish your parkour activity first, or /leaveparkour.");
		return 1;
	}

	if (GetPVarInt(playerid, "_SwimmingActivity") > 0)
	{
		SendClientMessageEx(playerid, COLOR_WHITE, "You are no longer participating in the swimming activity.");
		DeletePVar(playerid, "_SwimmingActivity");
		DisablePlayerCP(playerid);
	}

	if (GetPVarInt(playerid, "_BoxingQueue") == 1)
	{
		SendClientMessageEx(playerid, COLOR_WHITE, "You are no longer in the boxing queue.");
		DeletePVar(playerid, "_BoxingQueue");
		DeletePVar(playerid, "_BoxingQueueTick");
	}

    if(GetPVarInt(playerid, "IsInArena") >= 0) {
        SendClientMessageEx(playerid, COLOR_WHITE, "Ban khong the lam dieu nay tai dau' truong!");
        return 1;
    }
	if(GetPVarType(playerid, "StreamPrep")) {
		return SendClientMessageEx(playerid, COLOR_WHITE, "Ban khong the lam dieu do vao luc nay Wait for streaming to finish.");
	}
    if( PlayerCuffed[playerid] >= 1 || PlayerInfo[playerid][pBeingSentenced] != 0 ) {
        SendClientMessageEx(playerid, COLOR_WHITE, "Ban khong the lam dieu do vao luc nay");
        return 1;
    }

    PlayerExitInterior(playerid);
    return 1;
}

#pragma warning pop