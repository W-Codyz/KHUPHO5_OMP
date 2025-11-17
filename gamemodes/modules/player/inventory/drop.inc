#include <YSI\YSI_Coding\y_hooks>

stock CreateDroppedItem(itemid, amount, playerid, Float:x, Float:y, Float:z, worldid, interiorid)
{
    new i = Iter_Free(DropItems);
    if(i > -1 && !DroppedItems[i][droppedValid])
    {
        DroppedItems[i][droppedValid] = true;
        DroppedItems[i][droppedItemID] = itemid;
        DroppedItems[i][droppedAmount] = amount;
        DroppedItems[i][droppedModel] = ItemsInfo[itemid][itemModel];
        GetPlayerName(playerid, DroppedItems[i][droppedBy], MAX_PLAYER_NAME);
        
        DroppedItems[i][droppedX] = x;
        DroppedItems[i][droppedY] = y;
        DroppedItems[i][droppedZ] = z;
        
        // Tao object va 3Dtext
        DroppedItems[i][droppedObject] = CreateDynamicObject(19918, x, y, z-1.0, 0.0, 0.0, 0.0, worldid, interiorid, -1, 100.0, 70.0);
        
        new string[128];
        format(string, sizeof(string), "%s (SL: %d)\nNhan \"{81FB7D}ALT{FFFFFF}\" de nhat", ItemsInfo[itemid][itemName], amount);
        DroppedItems[i][droppedLabel] = CreateDynamic3DTextLabel(string, 0xFFFFFFFF, x, y, z-0.5, PICKUP_RANGE*3.0, _, _, 1, worldid, interiorid);
        Iter_Add(DropItems, i);
    }
    return i;
}

stock DeleteDroppedItem(itemid)
{
    if(!DroppedItems[itemid][droppedValid]) return 0;
    
    DroppedItems[itemid][droppedItemID] = -1;
    DroppedItems[itemid][droppedAmount] = 0;
    DroppedItems[itemid][droppedModel] = 0;
    DroppedItems[itemid][droppedBy][0] = EOS;
    
    DroppedItems[itemid][droppedX] = 0.0;
    DroppedItems[itemid][droppedY] = 0.0;
    DroppedItems[itemid][droppedZ] = 0.0;

    DroppedItems[itemid][droppedValid] = false;
    DestroyDynamicObject(DroppedItems[itemid][droppedObject]);
    DroppedItems[itemid][droppedObject] = INVALID_OBJECT_ID;
    DestroyDynamic3DTextLabel(DroppedItems[itemid][droppedLabel]);
    DroppedItems[itemid][droppedLabel] = INVALID_3DTEXT_ID;
    Iter_Remove(DropItems, itemid);
    return 1;
}

stock DropPlayerItem(playerid, itemid, amount)
{
    if(amount <= 0 || amount > PlayerInfo[playerid][ItemsInfo[itemid][itemId]])
        return SendClientMessage(playerid, COLOR_GREY, "So luong khong hop le!");
        
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    
    ApplyAnimation(playerid, "CARRY", "putdwn", 4.2, false, false, false, false, 1000, SYNC_ALL);
    
    // call func create drop
    CreateDroppedItem(itemid, amount, playerid, x, y, z, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid));
    
    // down value item for player
    PlayerInfo[playerid][ItemsInfo[itemid][itemId]] -= amount;
    
    new string[128];
    format(string, sizeof(string), "* %s da vut %d %s xuong dat.", GetPlayerNameEx(playerid), amount, ItemsInfo[itemid][itemName]);
    ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
    
    return 1;
}

stock PickupDroppedItem(playerid, itemid)
{
    if(!DroppedItems[itemid][droppedValid]) return 0;

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    
    if(GetDistanceBetweenPoints(x, y, z, DroppedItems[itemid][droppedX], DroppedItems[itemid][droppedY], DroppedItems[itemid][droppedZ]) > PICKUP_RANGE)
        return SendClientMessage(playerid, COLOR_GREY, "Ban phai den gan hon de nhat vat pham!");
    
    PlayerInfo[playerid][ItemsInfo[DroppedItems[itemid][droppedItemID]][itemId]] += DroppedItems[itemid][droppedAmount];
    
    new string[128];
    format(string, sizeof(string), "Ban da nhat %d %s.", DroppedItems[itemid][droppedAmount], ItemsInfo[DroppedItems[itemid][droppedItemID]][itemName]);
    SendClientMessage(playerid, COLOR_WHITE, string);
    
    // call func delete object va text3D
    DeleteDroppedItem(itemid);
    
    return 1;
}

forward DeleteAllDroppedItems(step);
public DeleteAllDroppedItems(step)
{
    if(Iter_Count(DropItems) > 0)
    {
        if(step == 0) {
            SendClientMessageToAllEx(0xFCFF4BFF, "SERVER: Tat ca vat pham tren mat dat se bi xoa bo sau 10 phut nua.");
            SetTimerEx("DeleteAllDroppedItems", 60000 * 10, false, "i", 1);
            return 1;
        }

        foreach(new i: DropItems)
        {
            DeleteDroppedItem(i);
        }

        SendClientMessageToAllEx(0xFCFF4BFF, "SERVER: Tat ca vat pham roi tren mat dat da bi xoa bo.");
    }

    SetTimerEx("DeleteAllDroppedItems", 60000 * 50, false, "i", 0);
    return 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(PRESSED(KEY_WALK))
    {
        foreach(new i: DropItems)
        {
            if(DroppedItems[i][droppedValid])
            {
                new Float:x, Float:y, Float:z;
                GetPlayerPos(playerid, x, y, z);
                
                if(GetDistanceBetweenPoints(x, y, z, DroppedItems[i][droppedX], DroppedItems[i][droppedY], DroppedItems[i][droppedZ]) <= PICKUP_RANGE)
                {
                    ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.2, false, false, false, false, 1000, SYNC_ALL);
                    PickupDroppedItem(playerid, i);
                    break;
                }
            }
        }
    }
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_DROP_AMOUNT)
    {
        if(!response) return ShowInventory(playerid, PlayerInfo[playerid][pInvType]);
        
        new itemid = GetPVarInt(playerid, "SelectedItem");
        new amount = strval(inputtext);
        
        DropPlayerItem(playerid, itemid, amount);
        DeletePVar(playerid, "SelectedItem");
        
        ShowInventory(playerid, PlayerInfo[playerid][pInvType]);
    }
    return 1;
}