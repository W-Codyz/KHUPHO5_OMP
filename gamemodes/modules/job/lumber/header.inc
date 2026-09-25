// ============================================================
//  Lumber Job Header - GTA-HTB Roleplay
// ============================================================

#define LUMBER_MAX_TREES        50
#define LUMBER_LOG_CARRY_SLOT   9
#define LUMBER_TOOL_SLOT        1
#define LUMBER_MAX_VEH_LOGS     5
#define LUMBER_CHOP_TIME        15000   // 15 giay chat cay
#define LUMBER_RESPAWN_TIME     900000  // 15 phut respawn cay

// Zone 1
#define LUMBER_ZONE1_MINX   -909.0555
#define LUMBER_ZONE1_MINY   -226.7779
#define LUMBER_ZONE1_MAXX   -608.0555
#define LUMBER_ZONE1_MAXY   -154.7779

// Zone 2
#define LUMBER_ZONE2_MINX   -708.9499
#define LUMBER_ZONE2_MINY   -151.5500
#define LUMBER_ZONE2_MAXX   -609.9499
#define LUMBER_ZONE2_MAXY   -70.5500

// Vi tri NPC cong viec
#define LUMBER_NPC_X    -758.966003
#define LUMBER_NPC_Y    -130.437088
#define LUMBER_NPC_Z    65.813842
#define LUMBER_NPC_A    9.329421

// Vi tri NPC ban / doi go
#define LUMBER_SELL_X   -745.837951
#define LUMBER_SELL_Y   -129.305023
#define LUMBER_SELL_Z   65.905570
#define LUMBER_SELL_A   292.812683

// Vi tri spawn xe
#define LUMBER_VEH_X    -751.7999
#define LUMBER_VEH_Y    -143.3935
#define LUMBER_VEH_Z    66.2233
#define LUMBER_VEH_A    281.5850

// Models
#define LUMBER_LOG_MODEL    1463   // go player cam
#define LUMBER_TREE_MODEL   659     // cay spawn trong the gioi
#define LUMBER_VEH_LOG_MODEL 1463   // go tren xe

// Gia ban go
#define LUMBER_LOG_PRICE    1500

// Dialog IDs
#define DIALOG_LUMBER_NPC   9620
#define DIALOG_LUMBER_SELL  9621
#define DIALOG_LUMBER_GPS   9622

// Trang thai player
enum E_LUMBER_PLAYER
{
    LP_VehicleID,
    bool:LP_Working,
    bool:LP_HasLog,
    LP_AttackTree,
    LP_ChopTimer
};
new PlayerLumberData[MAX_PLAYERS][E_LUMBER_PLAYER];

// Log tren xe
new gLumberVehLogObj[MAX_VEHICLES][LUMBER_MAX_VEH_LOGS];
new gLumberVehLogCount[MAX_VEHICLES];
new Text3D:gLumberVehLabel[MAX_PLAYERS];

// Cay trong the gioi
new bool:gLumberTreeActive[LUMBER_MAX_TREES];
new bool:gLumberTreeOccupied[LUMBER_MAX_TREES];
new gLumberTreePlayer[LUMBER_MAX_TREES];
new gLumberTreeObj[LUMBER_MAX_TREES];
new Float:gLumberTreeX[LUMBER_MAX_TREES];
new Float:gLumberTreeY[LUMBER_MAX_TREES];
new Float:gLumberTreeZ[LUMBER_MAX_TREES];
