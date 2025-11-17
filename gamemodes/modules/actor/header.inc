
enum _:MAX_SERVER_ACTORS
{
    ACTOR_CITYHALL,
    ACTOR_CARLICENSE,
    ACTOR_TAXILICENSE,
    ACTOR_NEWBIEMISSION,
    ACTOR_FISHERMAN,
    ACTOR_RENTCAR
};

enum E_ACTOR_INFO
{
    actorModelID,
    actorName[32],
    actorVW,
    actorInt,
    Float:actorX,
    Float:actorY,
    Float:actorZ,
    Float:actorRot
};

new ActorInfo[MAX_SERVER_ACTORS][E_ACTOR_INFO] =
{
    // City Hall
    {76, "Charlotte Evans", 2, 5, 254.0705, -7.7358, 1501.0045, 1.6946},
    // License
    {17, "William Brown", 3, 3, -2035.7214, -119.7877, 1041.6584, 281.6877},
    // Taxi License
    {187, "James Carter", 3, 3, -2035.8455, -109.7447, 1041.6584, 279.6841},
    // Newbie Mission
    {98, "Michael Evans", 0, 0, -1757.5547, 943.1161, 24.8906, 358.5212},
    // Fisherman
    {95, "Ethan Blake", 0, 0, -1631.5626, 1390.7188, 7.1875, 317.7094},
    // Thue xe
    {12, "Zoe Reed", 0, 0, -1752.4004, 943.0482, 24.8906, 359.1405}
};