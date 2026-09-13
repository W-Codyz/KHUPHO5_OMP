#define     MAX_INVENTORY_ITEMS         21
#define     MAX_ITEM_PER_PAGE           15
#define     INVENTORY_TYPE_TEXTDRAW     0
#define     INVENTORY_TYPE_DIALOG       1

#define     MAX_DROPPED_ITEMS           1000
#define     PICKUP_RANGE                1.5

enum E_DROPPED_ITEM {
    bool:droppedValid,
    droppedModel,
    droppedItemID,
    droppedAmount,
    droppedBy[MAX_PLAYER_NAME],
    Float:droppedX,
    Float:droppedY, 
    Float:droppedZ,
    droppedObject,
    Text3D:droppedLabel
}

enum E_ITEM_INFO
{
    pInfo:itemId,
    bool:itemUsed,
    itemModel,
    itemName[32],
    itemDescription[128],
    Float:itemRotX,
    Float:itemRotY,
    Float:itemRotZ,
    Float:itemZoom
};

new ItemsInfo[MAX_INVENTORY_ITEMS][E_ITEM_INFO] =
{
    {pMats, false, 16444, "Vat lieu", "Su dung de che tao vat pham/vu khi", 82.0000, 0.0000, 0.0000, 1.0000},
    {pFishRod, false, 18632, "Can cau ca ", "Su dung de co the cau ca", 45.0000, -10.0000, 8.0000, 0.8900},
    {pBait, false, 19567, "Moi cau", "Ket hop voi can cau de cau ca", 104.0000, 186.0000, 0.0000, 0.8900},
    {pCrack, true, 1580, "Crack", "Chat kich thich co the su dung", 104.0000, 186.0000, 0.0000, 1.0000},
    {pPot, true, 1576, "Pot", "Chat kich thich co the su dung", 104.0000, 186.0000, 0.0000, 1.0000},
    {pHeroin, true, 1578, "Heroin", "Chat kich thich co the su dung", 104.0000, 186.0000, 0.0000, 1.0000},
    {pOpiumSeeds, false, 11498, "Hat giong thuoc phien", "Hat giong dung de trong thuoc phien", 104.0000, 186.0000, 0.0000, 1.0000},
    {pRawOpium, false, 1575, "Thuoc phien tho", "Su dung de tinh luyen Heroin", 104.0000, 186.0000, 0.0000, 1.0000},
    {pSyringes, false, 333, "Ong tiem", "Su dung de tiem Heroin", 56.0000, 31.0000, 41.0000, 0.6900},
    {pPaper, false, 19805, "Giay", "Su dung de co the gui thu", 4.0000, 6.0000, 7.0000, 1.2000},
    {pRope, false, 11747, "Day thung", "Su dung dung de troi nguoi khac", 93.0000, 6.0000, 7.0000, 0.5900},
    {pCigar, true, 19896, "Xi ga", "Co the dung de hut", 93.0000, 174.0000, 7.0000, 0.5900},
    {pSprunk, true, 13562, "Sprunk", "La do uong co the uong", 160.0000, 174.0000, 64.0000, 0.8900},
    {pSpraycan, false, 2752, "Spraycan", "Su dung de son mau xe", 160.0000, 174.0000, 64.0000, 0.8900},
    {pScrewdriver, false, 18644, "Tua vit", "Su dung de sua chua/che tao", 160.0000, 174.0000, 64.0000, 0.8900},
    {pWristwatch, false, 19039, "Dong ho", "Co the su dung de xem thoi gian", 48.0000, 203.0000, 64.0000, 0.4900},
    {pSurveillance, true, 19623, "Giam sat", "Su dung de theo doi/giam sat", 103.0000, 63.0000, 64.0000, 0.8900},
    {pTire, false, 1080, "Lop xe", "Su dung de thay lop cho chiec xe", 45.0000, 63.0000, 64.0000, 1.1000},
    {pLock, false, 19804, "Khoa", "Su dung de khoa xe", 24.0000, 8.0000, 2.0000, 1.1000},
    {pFirstaid, true, 11738, "Bo so cuu", "Su dung de so cuu cho ban than", 24.0000, 8.0000, 2.0000, 0.8900},
    {pBoombox, false, 2103, "May nghe nhac", "Dat xuong va nghe nhac", 1.0000, 1.0000, 177.0000, 0.8900}
};

/* new DroppedItems[MAX_DROPPED_ITEMS][E_DROPPED_ITEM]; //drop
new Iterator:DropItems<MAX_DROPPED_ITEMS>;

new PlayerItemList[MAX_PLAYERS][MAX_INVENTORY_ITEMS] = {{-1, ...}, ...};
new InventoryOpen[MAX_PLAYERS char];
new InventoryMaxPage[MAX_PLAYERS char];
new InventoryCurrentPage[MAX_PLAYERS char];
new InventorySelected[MAX_PLAYERS char];

new PlayerText:InvTD@Slot[MAX_PLAYERS][15] = {{INVALID_PLAYER_TEXT_DRAW, ...}, ...};
new PlayerText: InvTD@ItemName[MAX_PLAYERS][15] = {{INVALID_PLAYER_TEXT_DRAW, ...}, ...};
new PlayerText: InvTD@OwnerName[MAX_PLAYERS] = {INVALID_PLAYER_TEXT_DRAW, ...};
new PlayerText: InvTD@ItemInfo[MAX_PLAYERS] = {INVALID_PLAYER_TEXT_DRAW, ...};
new PlayerText: InvTD@Page[MAX_PLAYERS] = {INVALID_PLAYER_TEXT_DRAW, ...};
new Text:InvTD@Global[5]; */