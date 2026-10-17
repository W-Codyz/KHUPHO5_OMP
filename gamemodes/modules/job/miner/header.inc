// ============================================================
//  Miner Job Header - KP5 Roleplay Cre by: Codyz
// ============================================================

#define MINER_MAX_STONES        150
#define MINER_STONE_CARRY_SLOT  9
#define MINER_TOOL_SLOT         1
#define MINER_MAX_VEH_STONES    5
#define MINER_MINE_TIME         10000   // 10 giay khai thac
#define MINER_RESPAWN_TIME      1200000 // 20 phut respawn da

// Vung khai thac (GangZone)
#define MINER_ZONE_MINX     480.944580
#define MINER_ZONE_MINY     738.666625
#define MINER_ZONE_MAXX     771.944580
#define MINER_ZONE_MAXY     1002.666625

// Vi tri tram rua da
#define MINER_WASH_X        591.372253
#define MINER_WASH_Y        877.100646
#define MINER_WASH_Z        -42.497318

// Vi tri NPC ban khoang san
#define MINER_SELL_X        1218.229003
#define MINER_SELL_Y        -1655.291259
#define MINER_SELL_Z        11.951199
#define MINER_SELL_ANGLE    271.522644

// Model
#define MINER_STONE_MODEL   3930
#define MINER_ROCK_MODEL    3930

// Gia khoang san ($/don vi) - index: 0=Sat 1=Dong 2=Bac 3=Vang 4=KimCuong
new gMinerPrice[5] = {350, 2500, 7500, 25000, 75000};

// Dialog IDs
#define DIALOG_MINER_NPC        5383
#define DIALOG_MINER_SELL       5384
#define DIALOG_MINER_CRAFT      5385
#define DIALOG_MINER_CRAFT_MELEE 5386
#define DIALOG_MINER_CRAFT_GUN  5387
#define DIALOG_MINER_GPS        5388

// Trang thai player
enum E_MINER_PLAYER
{
    MP_VehicleID,
    bool:MP_Working,
    bool:MP_HasStone,
    MP_AttackStone,
    MP_MineTimer
};
new PlayerMinerData[MAX_PLAYERS][E_MINER_PLAYER];

// Da tren xe
new gMinerVehStoneObj[MAX_VEHICLES][MINER_MAX_VEH_STONES];
new gMinerVehStoneCount[MAX_VEHICLES];
new Text3D:gMinerVehLabel[MAX_PLAYERS];

// Da trong the gioi
new bool:gMinerStoneActive[MINER_MAX_STONES];
new bool:gMinerStoneOccupied[MINER_MAX_STONES];
new gMinerStonePlayer[MINER_MAX_STONES];
new gMinerStoneObj[MINER_MAX_STONES];
new Float:gMinerStoneX[MINER_MAX_STONES];
new Float:gMinerStoneY[MINER_MAX_STONES];
new Float:gMinerStoneZ[MINER_MAX_STONES];

