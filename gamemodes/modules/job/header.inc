enum _:MAX_JOB_TYPE
{
	JOB_NONE,
	JOB_TAXIDRIVER, // 1
	JOB_TRUCKER, // 2
    JOB_PORTER, // 3
    JOB_PIZZABOY, // 4
    JOB_CONSTRUCTION, // 5
    JOB_CRAFTER, // 6
    JOB_ELECTRIC, // 7
    JOB_MINER,   // 8
    JOB_LUMBER   // 9
};

enum E_JOB_INFO
{
    jobName[32],
    jobLevel,
    jobSkin,
    jobVW,
    jobInt,
    Float:jobPosX,
    Float:jobPosY,
    Float:jobPosZ,
    Float:jobPosA
};


enum E_SIDEJOB_INFO
{
    sideJobName[32],
    Float:sideJobPosX,
    Float:sideJobPosY,
    Float:sideJobPosZ
};

new JobHelp[MAX_JOB_TYPE][1028] =
{
    {""},
    {"{F5F52F}Taxi Driver{FFFFFF} la mot cong viec thu vi va day thu thach tai \
      {FF6666}GTA-HTB Roleplay{FFFFFF}.\nKhi tro thanh mot tai xe taxi, ban se \
      dam nhan nhiem vu don va dua hanh khach\nden noi ho can mot cach an toan va nhanh \
      chong. Day khong chi la mot cong \nviec kiem song ma con la co hoi de ban kham pha moi \
      ngoc ngach cua thanh pho,\ngap go nhung nhan vat thu vi, va khang dinh ky nang lai xe \
      cua minh.\n\n\n{3F7EAB}Cau lenh co san: /fare /ataxi"},
    {"{F5F52F}Van chuyen hang hoa {FFFFFF}la cong viec mua va giao hang cho cac cua hang.\n\n\
        {FFFF00}FLOW:\n{FFFFFF}1. Len xe Mule/Yankee tai Ben Cang SF\n\
        2. Di bo den Diem Lay Hang, bam [Y] mua hang\n\
        3. Dem hang den gan xe va bam [Y] gan xe de chat len xe (max 5 kien)\n\
        4. Lai xe den diem ban hang (/diemgiaohang de xem)\n\
        5. Dung xe, dung /xemhang lay hang, di bo ban hang\n\
        6. Quay ve Ben Cang SF bam [Y] Diem Lanh Luong\n\n\
        {FF0606}Hang cam:{FFFFFF} Mua tai diem bi mat, can 2+ cops on-duty\n\n\
        {3F7EAB}Lenh: /diemgiaohang /xemhang /kiemtrahang /huongdan"},
    {"{F5F52F}Boc vac{FFFFFF} la mot cong viec kiem them thu nhap mot cach don gian va\n\
        nhanh chong khong yeu cau ve trinh do. Cong viec cua ban chi can\nkhuan vac cac thung \
        hang cua Ben cang vao kho hang mot cach nhanh\nchong, cong viec doi hoi su nhanh nhen, \
        can than va kien nhan.\n\n\n{3F7EAB}Cau lenh co san: /lamviec /dunglamviec"},
    {"{F5F52F}Pizza Boy{FFFFFF} la mot cong viec don gian va day thu vi tai {FF6666}GTA-HTB Roleplay{FFFFFF}\n\
        ban se dam nhan vai tro giao nhung chiec banh pizza nong hoi den tay khach\nhang. Chi can mot chiec xe may va kha nang \
        lai xe linh hoat, ban co the de\ndang kiem tien bang cach hoan thanh cac don hang dung thoi gian.\n\n\n\
        {3F7EAB}Cau lenh co san: /lamviec /dunglamviec /laybanh"},
    {"{F5F52F}Tho xay dung{FFFFFF} la mot cong viec don gian va day thu vi tai {FF6666}GTA-HTB Roleplay{FFFFFF}\n\
        ban se dam nhan thanh mot tho xay dung chan chinh voi doi tay manh me, dieu\nnghe xay dung nhung cong trinh that chac chan \
        mang lai nhung thanh tuu lon tai\nthanh pho.\n\n\n{3F7EAB}Cau lenh co san: /lamviec /dunglamviec"},
    {"{F5F52F}Tho thu cong{FFFFFF} la mot cong viec tai {FF6666}GTA-HTB Roleplay{FFFFFF} cho phep ban che tao\n\
        bat ki vat pham hay trang bi nao de phuc vu cho hoat dong cua ban than, ban\nmuon co vu khi? muon co trang bi tot? vay thi dung ngung ngai gi ma khong\n\
        nhan cong viec nay co the lam nhung gi ban muon.\n\n\n{3F7EAB}Cau lenh co san: /lamviec /dunglamviec"},
    {"{F5F52F}Tho sua dien{FFFFFF} la mot cong viec doc dao tai {FF6666}GTA-HTB Roleplay{FFFFFF}\n\
        ban se dam nhan vai tro bao tri va sua chua he thong dien trong\nthanh pho. Cong viec yeu cau \
        su tap trung va kha nang quan sat\nde xu ly cac su co dien .\n\n\n\
        {3F7EAB}Cau lenh co san: /takekit /egroup /xemday /dunglamviec"},
    {"{F5F52F}Tho Khai Thac{FFFFFF} la mot cong viec doc dao tai {FF6666}GTA-HTB Roleplay{FFFFFF}\n\
        ban se khai thac da quy trong khu mo, rua sach de thu hoach khoang san va ban lai.\n\
        Cong viec yeu cau su kien nhan va kha nang quan ly van chuyen hang hoa.\n\n\n\
        {3F7EAB}Cau lenh co san: /layda /dunglamviec"},
    {"{0f8d47}Chat Go{FFFFFF} la mot cong viec tai {FF6666}GTA-HTB Roleplay{FFFFFF} cho phep ban\n\
        khai thac go tu khu rung, dat go len xe va ban cho thuong lai.\n\
        Mo tat ca cac loai cay trong 2 khu vuc rung, moi khuc go ban hoac giu lai de lam viec khac.\n\
        Co the doi go tho de luu vao tui va ban sau.\n\n\n\
        {3F7EAB}Cau lenh co san: /laygo /chatgo /dunglamviec"}
};

new JobInfo[MAX_JOB_TYPE][E_JOB_INFO] =
{
    {"That nghiep", 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0},
    {"Taxi Driver", 1, 61, 0, 0, -1980.6090, 132.0104, 27.6875, 87.2845},
    {"Van chuyen hang", 1, 1, 0, 0, -1557.5031, 126.4185, 3.5547, 38.4066},
    {"Boc vac", 1, 73, 0, 0, -1710.878540, 22.986656, 3.749292, 229.971527},
    {"Pizza Boy", 1, 155, 0, 0, -1715.8992, 1366.7646, 7.1875, 45.4883},
    {"Tho xay dung", 1, 27, 0, 0, -2071.5452, 227.5330, 36.1192, 88.0978},
    {"Tho thu cong", 1, 6, 0, 0, -2120.1162, -14.2599, 35.3203, 88.2260},
    {"Tho sua dien", 1, 16, 0, 0, -2520.9807, -624.4470, 132.7828, 358.126220},
    {"Tho Khai Thac", 1, 27, 0, 0, 584.901367, 867.991882, -42.497318, 267.125885},
    {"Lumberjack", 1, 291, 0, 0, -758.966003, -130.437088, 65.813842, 9.329421}
};

new SideJobInfo[][E_SIDEJOB_INFO] = {
    {"Quet don duong pho", -1661.0928, 442.7479, 7.1797},
    {"Cau ca", -1628.3528, 1394.4696, 7.1813}
};

new PlayerWorking[MAX_PLAYERS];