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

new Float:RentCarSpawn[4] = { -1976.1603,103.1138,27.3929,90.0232 };
new RentCarList[5][3] = {
    //modelid   price       time (seconds)
    {509,       670,        7200},
    {510,       750,        7200},
    {481,       800,        7200},
    {462,       1300,        7200},
    {496,       1900,        7200}
};