#define     MAX_FISH_BAG      5

enum pFishing
{
	pWeight[MAX_FISH_BAG],
	pFid[MAX_FISH_BAG],
	pLastFish,
	pFishID,
	pLastWeight
};

new Fishes[MAX_PLAYERS][pFishing];
new FishCount[MAX_PLAYERS];
new IsFishing[MAX_PLAYERS];
new FishingTime[MAX_PLAYERS];

new FSpecialAreaID = INVALID_STREAMER_ID;
new FSpecialZoneID = INVALID_GANG_ZONE;

new FishNames[22][20] = {
    {"Jacket"}, {"Amberjack"}, {"Grouper"},
    {"Red Snapper"}, {"Pants"}, {"Trout"},
    {"Blue Marlin"}, {"Can"}, {"Mackeral"},
    {"Sea Bass"}, {"Shoes"}, {"Pike"},
    {"Sail Fish"}, {"Garbage"}, {"Tuna"},
    {"Eel"}, {"Dolphin"}, {"Shark"},
    {"Turtle"}, {"Catfish"}, {"Money Bag"},
    {"Swordfish"}
};

new Float:FishPosition[6][3] = {
    {-1615.0636, 1396.6761, 7.1730},
    {-1617.4104, 1410.9554, 7.1754},
    {-1621.1986, 1420.8882, 7.1735},
    {-1630.3568, 1430.0598, 7.1735},
    {-1639.3212, 1432.8120, 7.1778},
    {-1650.5123, 1434.1290, 7.1736}
};

new Float:SpecialFishZone[4] = {
    -2642.0000, 1593.0000, -1944.0000, 1982.0000
};