#include <YSI/YSI_Coding/y_hooks>

#define FACE_INTERACT_TIMER_MS    1000
#define FACE_INTERACT_MAX_DIST    3.0
#define FACE_INTERACT_LABEL_COLOR 0xa8eb7bFF

new FaceTargetPlayer[MAX_PLAYERS];
new PlayerText3D:FaceTargetLabel[MAX_PLAYERS];
new g_FaceInteractTimer = -1;

forward FaceInteract_Tick();

static stock ClearFaceTarget(playerid) {
    if(FaceTargetLabel[playerid] != PlayerText3D:INVALID_3DTEXT_ID) {
        DeletePlayer3DTextLabel(playerid, FaceTargetLabel[playerid]);
        FaceTargetLabel[playerid] = PlayerText3D:INVALID_3DTEXT_ID;
    }
    FaceTargetPlayer[playerid] = INVALID_PLAYER_ID;
}

static stock UpdateFaceTarget(playerid) {
    // Dung PlayerPos[] da duoc cache san trong playerTabbedLoop (1s/lan)
    new Float:px = PlayerPos[playerid][0];
    new Float:py = PlayerPos[playerid][1];
    new Float:pz = PlayerPos[playerid][2];

    new Float:facingAngle;
    GetPlayerFacingAngle(playerid, facingAngle);

    // Tinh truoc forward vector 1 lan cho tat ca ung vien
    new Float:fX = -floatsin(facingAngle, degrees);
    new Float:fY =  floatcos(facingAngle, degrees);

    new bestTarget = INVALID_PLAYER_ID;
    new Float:bestDistSq = (FACE_INTERACT_MAX_DIST + 1.0) * (FACE_INTERACT_MAX_DIST + 1.0);
    new Float:maxDistSq  = FACE_INTERACT_MAX_DIST * FACE_INTERACT_MAX_DIST;

    foreach(new i : Player) {
        if(i == playerid || IsPlayerNPC(i)) continue;

        new Float:dx = PlayerPos[i][0] - px;
        new Float:dy = PlayerPos[i][1] - py;
        new Float:dz = PlayerPos[i][2] - pz;

        // Check squared distance truoc: khong can sqrt
        new Float:distSq = dx * dx + dy * dy + dz * dz;
        if(distSq > maxDistSq || distSq < 0.01) continue;

        // Chi normalize XY khi da loc qua distance
        new Float:dlen = floatsqroot(dx * dx + dy * dy);
        if(dlen < 0.001) continue;

        new Float:dot = fX * (dx / dlen) + fY * (dy / dlen);
        if(dot >= 0.7071 && distSq < bestDistSq) {
            bestDistSq = distSq;
            bestTarget = i;
        }
    }

    if(bestTarget == INVALID_PLAYER_ID) {
        ClearFaceTarget(playerid);
        return;
    }

    // Xoa label cu truoc khi tao moi
    if(FaceTargetLabel[playerid] != PlayerText3D:INVALID_3DTEXT_ID) {
        DeletePlayer3DTextLabel(playerid, FaceTargetLabel[playerid]);
        FaceTargetLabel[playerid] = PlayerText3D:INVALID_3DTEXT_ID;
    }

    FaceTargetPlayer[playerid] = bestTarget;

    // Attach vao nguoi duoc nhin vao, z=0 la diem ban dau attachment (~ vai/co)
    // Dung gia tri am de ha xuong than nguoi
    FaceTargetLabel[playerid] = CreatePlayer3DTextLabel(playerid,
        "[Y] {ffffff}Tuong tac",
        FACE_INTERACT_LABEL_COLOR,
        0.0, 0.0, -0.5,
        6.0,
        bestTarget, INVALID_VEHICLE_ID);
}

public FaceInteract_Tick() {
    foreach(new i : Player) {
        if(IsPlayerNPC(i)) continue;
        if(IsPlayerInAnyVehicle(i)
            || PlayerCuffed[i] != 0
            || GetPVarInt(i, "Injured") != 0
            || PlayerInfo[i][pHospital] != 0
            || GettingSpectated[i] != INVALID_PLAYER_ID
            || GetPlayerState(i) == PLAYER_STATE_SPECTATING)
        {
            ClearFaceTarget(i);
            continue;
        }
        UpdateFaceTarget(i);
    }
}

hook OnGameModeInit() {
    for(new i = 0; i < MAX_PLAYERS; i++) {
        FaceTargetPlayer[i] = INVALID_PLAYER_ID;
        FaceTargetLabel[i] = PlayerText3D:INVALID_3DTEXT_ID;
    }
    g_FaceInteractTimer = SetTimer("FaceInteract_Tick", FACE_INTERACT_TIMER_MS, true);
    return 1;
}

hook OnPlayerConnect(playerid) {
    FaceTargetPlayer[playerid] = INVALID_PLAYER_ID;
    FaceTargetLabel[playerid] = PlayerText3D:INVALID_3DTEXT_ID;
    return 1;
}

hook OnPlayerDisconnect(playerid, reason) {
    ClearFaceTarget(playerid);
    for(new i = 0; i < MAX_PLAYERS; i++) {
        if(FaceTargetPlayer[i] == playerid) {
            ClearFaceTarget(i);
        }
    }
    return 1;
}
