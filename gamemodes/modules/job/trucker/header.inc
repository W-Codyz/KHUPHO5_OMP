
// ================================================================
//  TRUCKER JOB - HEADER
// ================================================================

// Dialog IDs (9830-9836 range, safe from conflicts)
#define DIALOG_TRUCKERGO        (9830)  // Mua hang hoa
#define DIALOG_XEMHANG          (9831)  // Xem hang tren xe
#define DIALOG_DIEMGIAOHANG     (9832)  // Danh sach diem giao hang
#define DIALOG_BANHANGTRUCK     (9833)  // Xac nhan ban hang
#define DIALOG_TRUCKER_KIEMTRA  (9834)  // Canh sat kiem tra xe
#define DIALOG_BANGHANGCAM      (9835)  // (du phong)
#define DIALOG_HIENTHI_HUONGDAN (9836)  // Huong dan trucker
#define DIALOG_TRUCKERNPC       (9837)  // Menu NPC: lay xe / diem giao / huong dan

// ---- Locations ----
// Ben cang SF - diem lay hang (on foot, bam Y)
#define TRUCKER_BUY_POS         -1554.040405, 101.120498, 3.554687
// Ben cang SF - diem lanh luong (on foot, bam Y)
#define TRUCKER_PAYMENT_POS     -1551.603393, 120.569969, 3.554687

// Mau sac thong bao cua job trucker
#define COLOR_TRUCKER           0xb7eced

// ---- Cargo category constants ----
#define HANG_THUCAN     0
#define HANG_VATLIEU    1
#define HANG_QUANAO     2
#define HANG_CAM        3

// ---- Selling point data (12 diem) ----
enum thumuaenum
{
    idhang,
    giahang,
    giabanhientai,
    tenhang[24],
    loaihang        // 1=ThucAn, 2=VatLieu, 3=QuanAo
};

new ThuMuaTruck[12][thumuaenum] = {
    {1,  50, 0, "Thuc An",  1},
    {2,  50, 0, "Thuc An",  1},
    {3,  50, 0, "Thuc An",  1},
    {4,  50, 0, "Thuc An",  1},
    {5,  80, 0, "Vat Lieu", 2},
    {6,  80, 0, "Vat Lieu", 2},
    {7,  80, 0, "Vat Lieu", 2},
    {8,  80, 0, "Vat Lieu", 2},
    {9,  50, 0, "Quan Ao",  3},
    {10, 50, 0, "Quan Ao",  3},
    {11, 50, 0, "Quan Ao",  3},
    {12, 50, 0, "Quan Ao",  3}
};

// Toa do 12 diem ban hang (tuong ung voi ThuMuaTruck)
new Float:KhuVucThuMua[12][3] = {
    {-1706.942626, 1352.670043, 7.179687},
    {-2516.719970, 1211.980834, 37.421875},
    {-2787.734619, 796.907653, 49.534320},
    {-2457.207275, 780.532348, 35.171875},
    {-1977.662231, 945.479553, 45.445312},
    {-2277.069335, 541.004821, 35.171875},
    {-2089.259521, 84.565551, 35.313430},
    {-1910.224487, 823.038452, 35.171875},
    {-1885.797241, 870.894714, 35.171875},
    {-1677.007690, 436.915771, 7.179687},
    {-2446.061279, -55.994037, 34.265625},
    { -2634.525634, 209.178329, 4.392440}
};

// ---- Illegal buying areas (3 diem) ----
enum BuyAreaInfo
{
    Float:buyX, Float:buyY, Float:buyZ,
    buyText3D,      // STREAMER_TAG_3D_TEXT_LABEL handle (untagged)
    buyStock        // So luong hien co (max 5)
};

new IllegalAreaInfo[3][BuyAreaInfo] = {
    {-2527.096191, -624.251403, 132.762710, -1, 3},
    {-1425.506835, -1528.897094, 102.126312, -1, 4},
    {-1080.323486, -1195.421752, 129.705718, -1, 2}
};

new Float:TruckerSpawnPos[][4] = {
    {-1608.8298, 124.6252, 3.4277,  44.3017},
    {-1610.9657, 122.1086, 3.4277,  44.8588},
    {-1613.1616, 119.8515, 3.4277,  43.7442},
    {-1615.6199, 117.7439, 3.4277,  45.6584}
};

// Diem ban hang cam duy nhat
new Float:SellIllegalPosition[3] = {-1805.887207, -1613.531250, 23.015625};

// ---- Pricing ----
// index: 0=ThucAn, 1=VatLieu, 2=QuanAo, 3=HangCam
new giatruck[4];

// ---- Per-player state ----
// Hang tren tay (dang cam): [ThucAn, VatLieu, QuanAo, HangCam]
new hangtrentay[MAX_PLAYERS][4];
// Hang tren xe: [ThucAn, VatLieu, QuanAo, HangCam]
new hangtrenxe[MAX_PLAYERS][4];
// Tong so kien hang dang tren xe
new LuongHang[MAX_PLAYERS];
// Tong tien luong tich luy (chua lanh)
new lanhluong[MAX_PLAYERS];
// So lan giao hang thanh cong (de tinh XP/skill)
new TruckerDeliveries[MAX_PLAYERS];
// XP tich luy (chua nhan, nhan 1 lan khi lanh luong)
new tongXPTruck[MAX_PLAYERS];
// Player dang trong animation ban hang
new bool:PlayerSellingCargo[MAX_PLAYERS];
// Xe tai duoc gan cho player
new TruckerJobCar[MAX_PLAYERS];
// 3D text label tren xe (nhan dang chu xe, standard SA-MP)
new Text3D:TruckerJobText[MAX_PLAYERS];

enum CargoArrayData { Float:PosX, Float:PosY, Float:PosZ };
new Float:Dropoffs_Legal[16][CargoArrayData];
new Float:Dropoffs_Illegal[15][CargoArrayData];
new Float:LoadTruckPosition[3];
new TruckUsed[MAX_PLAYERS];
new TruckDeliveringTo[MAX_VEHICLES];
new TruckContents[MAX_VEHICLES char];
new TruckRoute[MAX_VEHICLES char];
