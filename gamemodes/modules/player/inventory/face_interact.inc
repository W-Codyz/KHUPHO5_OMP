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
    new Float:px, Float:py, Float:pz;
    GetPlayerPos(playerid, px, py, pz);

    new Float:facingAngle;
    GetPlayerFacingAngle(playerid, facingAngle);

    new bestTarget = INVALID_PLAYER_ID;
    new Float:bestDist = FACE_INTERACT_MAX_DIST + 1.0;

    for(new i = 0; i < MAX_PLAYERS; i++) {
        if(i == playerid || !IsPlayerConnected(i) || IsPlayerNPC(i)) continue;

        new Float:tx, Float:ty, Float:tz;
        GetPlayerPos(i, tx, ty, tz);

        new Float:dist = GetDistanceBetweenPoints(px, py, pz, tx, ty, tz);
        if(dist > FACE_INTERACT_MAX_DIST || dist < 0.1) continue;

        // Normalize huong den target (chi mat phang XY)
        new Float:dx = tx - px;
        new Float:dy = ty - py;
        new Float:dlen = floatsqroot(dx * dx + dy * dy);
        if(dlen < 0.001) continue;
        dx /= dlen;
        dy /= dlen;

        // Screenshot xac nhan: (sin,-cos) bi nguoc 180 do
        // Forward vector dung: (-sin, cos)
        //   angle=0  -> (0, 1)  = North (+Y) ← huong nhan vat dang nhin
        //   angle=90 -> (-1,0)  = West
        //   angle=180-> (0,-1)  = South
        //   angle=270-> (1, 0)  = East
        new Float:fX = -floatsin(facingAngle, degrees);
        new Float:fY =  floatcos(facingAngle, degrees);

        // Dot product >= cos(45°) ~ 0.7071 nghia la target nam trong cone
        new Float:dot = fX * dx + fY * dy;
        if(dot >= 0.7071 && dist < bestDist) {
            bestDist = dist;
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
    for(new i = 0; i < MAX_PLAYERS; i++) {
        if(!IsPlayerConnected(i) || IsPlayerNPC(i)) continue;
        if(IsPlayerInAnyVehicle(i)
            || PlayerCuffed[i] != 0
            || GetPVarInt(i, "Injured") != 0
            || PlayerInfo[i][pHospital] != 0)
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
