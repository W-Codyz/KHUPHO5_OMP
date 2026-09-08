
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
    {76, "Charlotte Evans", 2, 5, 1.0, 2.0, 3.0, 4.0},
    // License
    {17, "William Brown", 3, 3, 1.0, 2.0, 3.0, 4.0},
    // Taxi License
    {187, "James Carter", 3, 3, 1.0, 2.0, 3.0, 4.0},
    // Newbie Mission
    {98, "Michael Evans", 0, 0, 1.0, 2.0, 3.0, 4.0},
    // Fisherman
    {95, "Ethan Blake", 0, 0, 1.0, 2.0, 3.0, 4.0},
    // Thue xe
    {12, "Hao Nam", 0, 0, -1980.6089,145.7599,27.6875,90.8890}
};