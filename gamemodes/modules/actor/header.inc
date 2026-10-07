
enum _:MAX_SERVER_ACTORS
{
    ACTOR_CITYHALL,
    ACTOR_CARLICENSE,
    ACTOR_TAXILICENSE,
    ACTOR_NEWBIEMISSION,
    ACTOR_FISHERMAN,
    ACTOR_RENTCAR,
    ACTOR_IDCARD
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
    {295, "Can Bo Ghi Danh", 0, 1, 1324.036010, 735.908386, 111.320312, 97.589454},
    // Taxi License
    {187, "James Carter", 3, 3, 1.0, 2.0, 3.0, 4.0},
    // Newbie Mission
    {98, "Michael Evans", 0, 0, 1.0, 2.0, 3.0, 4.0},
    // Fisherman
    {95, "Ethan Blake", 0, 0, 1.0, 2.0, 3.0, 4.0},
    // Thue xe
    {12, "Hao Nam", 0, 0, -1980.6089,145.7599,27.6875,90.8890},
    // Cap Can cuoc cong dan
    {280, "Can Bo Hanh Chinh", 0, 1, 1324.032958, 739.907531, 111.320312, 92.450729}
};