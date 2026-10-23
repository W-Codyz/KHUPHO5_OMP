// ============================================================
//  Container Delivery Job Header - KP5 Roleplay
// ============================================================

// --- Dialog IDs (5521-5527, tiep theo DIALOG_ELECTRIC_COLORS 5520) ---
#define DIALOG_CONTAINER_MENU       (5521)
#define DIALOG_CONTAINER_GNAME      (5522)
#define DIALOG_CONTAINER_INVITE     (5523)
#define DIALOG_CONTAINER_KICK       (5524)
#define DIALOG_CONTAINER_JOIN       (5525)
#define DIALOG_CONTAINER_URGTYPE    (5526)
#define DIALOG_CONTAINER_CARGO      (5527)
#define DIALOG_CONTAINER_DELIVERYLIST (5528)

// --- Loai hang ---
#define CARGO_VATLIEU   0   // trailer 450
#define CARGO_XANG      1   // trailer 584
#define CARGO_QUANAO    2   // trailer 591
#define CARGO_LAU       3   // hang lau, random trailer

// --- Model trailer theo loai hang (lau = random trong 3 loai) ---
// (dinh nghia trong container.inc)

// --- Model xe tai theo level ---
#define TRUCK_LV1   403
#define TRUCK_LV5   514
#define TRUCK_LV10  515

// --- Slot & Object boc hang (same as trucker) ---
#define CONTAINER_CARGO_SLOT    6
#define CONTAINER_CARGO_OBJ     1271

// --- Gioi han kien hang ---
#define CONTAINER_SOLO_MAX      10
#define CONTAINER_TEAM_MAX      20

// --- Kho hang ---
#define CONTAINER_WAREHOUSE_COUNT   3
#define CONTAINER_STOCK_DEFAULT     100
#define CONTAINER_RESPAWN_MS        (30*60*1000)    // 30 phut

// --- Diem giao hang: 2+2+2 hop phap + 3 bi mat ---
#define CONTAINER_DELIVERY_COUNT    9

// --- Mau sac ---
#define COLOR_CONTAINER         0xD4A017FF

// --- File cau hinh gia ---
#define CONTAINER_CFG_FILE      "ContainerConfig.ini"

// --- Toa do 3 vi tri spawn xe ---
// (dinh nghia trong container.inc)

// --- Vi tri tra xe ve (NPC spawn) ---
#define CJ_RETURN_X     -1632.052001
#define CJ_RETURN_Y      110.311462
#define CJ_RETURN_Z      3.333974

// --- Enum trang thai cong viec cua player ---
enum E_CJ
{
    E_CJ_GroupName[64],
    E_CJ_GroupID,
    E_CJ_PartnerID,
    E_CJ_LeaderID,
    bool:E_CJ_Working,
    E_CJ_TruckID,
    E_CJ_TrailerID,
    E_CJ_CargoType,
    bool:E_CJ_IsUrgent,
    E_CJ_CargoCount,
    E_CJ_OriginalMax,
    E_CJ_WarehouseIdx,
    E_CJ_DeliveryIdx,
    E_CJ_TimeTotal,
    E_CJ_TimeLeft,
    bool:E_CJ_HasCargo,
    bool:E_CJ_Returning,
    E_CJ_CountdownTimer,
    Float:E_CJ_DeliveryDist
};
new PlayerCJ[MAX_PLAYERS][E_CJ];

// --- Enum kho hang ---
enum E_CJ_WAREHOUSE
{
    Float:CJW_X, Float:CJW_Y, Float:CJW_Z,
    CJW_Stock,
    CJW_MaxStock,
    bool:CJW_Respawn
};
new CJWarehouse[CONTAINER_WAREHOUSE_COUNT][E_CJ_WAREHOUSE];

// --- Enum diem giao hang ---
enum E_CJ_DELIVERY
{
    Float:CJD_X, Float:CJD_Y, Float:CJD_Z,
    CJD_CargoType,
    bool:CJD_IsIllegal,
    CJD_Name[32]
};
new CJDelivery[CONTAINER_DELIVERY_COUNT][E_CJ_DELIVERY];

// --- Forward cho CJ_ShowDeliveryListDialog (stock, but called from trucker.inc which is compiled earlier) ---
forward CJ_ShowDeliveryListDialog(playerid);

// --- 3D Text tren xe va trailer ---
new Text3D:CJVehLabel[MAX_PLAYERS];
new Text3D:CJTrailerLabel[MAX_PLAYERS];

// --- Pickup & label toan cuc cua kho hang ---
new STREAMER_TAG_PICKUP:CJWarehousePickup[CONTAINER_WAREHOUSE_COUNT];
new Text3D:CJWarehouseLabel[CONTAINER_WAREHOUSE_COUNT];

// --- Gia theo loai hang (load tu file) ---
new Float:gCJPrice[4];

// --- Admin lock ---
new gContainerLocked = 0;

// --- Macros ---
#define IsContainerWorking(%0)  (PlayerCJ[%0][E_CJ_Working])

