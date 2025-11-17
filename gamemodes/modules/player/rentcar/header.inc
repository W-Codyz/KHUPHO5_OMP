const MAX_RENTALCAR_SLOT = 5;

enum E_RENTCAR_INFO
{
    rcSqlid,
    rcId,
    rcModel,
    rcTime,
    Float:rcPosX,
    Float:rcPosY,
    Float:rcPosZ,
    Float:rcAngle
};

new CarRentalInfo[MAX_PLAYERS][MAX_RENTALCAR_SLOT][E_RENTCAR_INFO];

new Float:RentCarSpawn[4] = { -1753.3867, 949.2808, 24.7237, 89.9368 };
new RentCarList[5][3] = {
    //modelid   price       time (seconds)
    {509,       170,        7200},
    {510,       250,        7200},
    {481,       300,        7200},
    {462,       800,        7200},
    {496,       1400,        7200}
};