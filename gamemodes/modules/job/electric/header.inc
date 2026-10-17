// ============================================================
//  Electric Job Header - KP5 Roleplay
// ============================================================

#define IsElectricanWorking(%0)         PlayerElectricJob[%0][E_EJ_Working]
#define SetElectricanWorking(%0,%1)     PlayerElectricJob[%0][E_EJ_Working] = %1
#define ELECTRIC_KIT_SLOT               8
#define ELECTRIC_MAX_POINTS             14

enum E_ELECTRICJOB {
    E_EJ_GroupName[64],
    E_EJ_GroupID,
    E_EJ_PartnerID,
    E_EJ_VehicleID,
    bool:E_EJ_Locked,
    E_EJ_FixCount,
    E_EJ_LeaderID,
    bool:E_EJ_Working
};
new PlayerElectricJob[MAX_PLAYERS][E_ELECTRICJOB];

new STREAMER_TAG_CP:gElectricCP[MAX_PLAYERS][ELECTRIC_MAX_POINTS];
new STREAMER_TAG_MAP_ICON:gElectricIcon[MAX_PLAYERS][ELECTRIC_MAX_POINTS];
new STREAMER_TAG_RACE_CP:gElectricFinish[MAX_PLAYERS];
new bool:gElectricFixed[MAX_PLAYERS][ELECTRIC_MAX_POINTS];
new gToggleElectric = 0;
new Text3D:gElectricVehLabel[MAX_PLAYERS];
