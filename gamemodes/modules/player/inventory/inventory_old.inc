#include <YSI\YSI_Coding\y_hooks>

static AddItemID[MAX_PLAYERS];

stock ResetInventoryVariables(playerid)
{
	for(new i; i < MAX_INVENTORY_ITEMS; i++) {
		PlayerItemList[playerid][i] = -1;
	}

	InventoryMaxPage{playerid} = 0;
	InventoryCurrentPage{playerid} = 0;
	InventorySelected{playerid} = -1;
	return 1;
}

stock GetInventoryItems(playerid)
{
	new count = -1;
	for(new i; i < sizeof(ItemsInfo); i++)
	{
		if(PlayerInfo[playerid][ItemsInfo[i][itemId]] > 0)
		{
			PlayerItemList[playerid][++count] = i;
		}
	}
	return count;
}

stock ShowItemInfo(playerid, itemIndex)
{
	if(itemIndex == -1) return 0;

	PlayerTextDrawSetString(playerid, InvTD@ItemInfo[playerid], "~y~[>]~w~%s~n~~r~[?]~w~%s~n~~g~Nhan mot lan nua de tuong tac", ItemsInfo[itemIndex][itemName], ItemsInfo[itemIndex][itemDescription]);
	PlayerTextDrawShow(playerid, InvTD@ItemInfo[playerid]);
	return 1;
}

stock ShowInventoryPage(playerid, page)
{
	if(InventoryOpen{playerid} == 0) return 0;
	if(page < 1 || page > InventoryMaxPage{playerid}) return 0;

	InventorySelected{playerid} = -1;
	GetInventoryItems(playerid);
	PlayerPlaySound(playerid, 16200);

	for(new i=0;i<MAX_ITEM_PER_PAGE;i++) 
	{
		new count = (page-1)*MAX_ITEM_PER_PAGE + i, itemIndex = -1;
		if(count < MAX_INVENTORY_ITEMS) itemIndex = PlayerItemList[playerid][(page-1)*MAX_ITEM_PER_PAGE + i];
		if(itemIndex != -1 && count < MAX_INVENTORY_ITEMS) {
			PlayerTextDrawSetPreviewModel(playerid, InvTD@Slot[playerid][i], ItemsInfo[itemIndex][itemModel]);
			PlayerTextDrawSetPreviewRot(playerid, InvTD@Slot[playerid][i], ItemsInfo[itemIndex][itemRotX], ItemsInfo[itemIndex][itemRotY], ItemsInfo[itemIndex][itemRotZ], ItemsInfo[itemIndex][itemZoom]);
			PlayerTextDrawSetString(playerid, InvTD@ItemName[playerid][i], "%s~n~(%d)", ItemsInfo[itemIndex][itemName], PlayerInfo[playerid][ItemsInfo[itemIndex][itemId]]);
		}
		else {
			PlayerTextDrawSetPreviewModel(playerid, InvTD@Slot[playerid][i], 19475);
			PlayerTextDrawSetString(playerid, InvTD@ItemName[playerid][i], "_");
		}

		PlayerTextDrawShow(playerid, InvTD@Slot[playerid][i]);
		PlayerTextDrawShow(playerid, InvTD@ItemName[playerid][i]);
	}

	PlayerTextDrawSetString(playerid, InvTD@Page[playerid], "%d/%d", page, InventoryMaxPage{playerid});
	PlayerTextDrawShow(playerid, InvTD@Page[playerid]);

	PlayerTextDrawSetString(playerid, InvTD@ItemInfo[playerid], "_");
	PlayerTextDrawShow(playerid, InvTD@ItemInfo[playerid]);
	return 1;
}

stock ShowInventory(playerid, type)
{
	ResetInventoryVariables(playerid);
	if(type == INVENTORY_TYPE_DIALOG) {

		new szTitle[64];
		format(szTitle, _, "Tui do cua %s", GetPlayerNameEx(playerid));

		new count = -1;
		for(new i; i < sizeof(ItemsInfo); i++)
		{
			if(PlayerInfo[playerid][ItemsInfo[i][itemId]] > 0)
			{
				PlayerItemList[playerid][++count] = i;
				AddDialogListitem(playerid, "{FFFFFF}%s\tSL: %d", ItemsInfo[i][itemName], PlayerInfo[playerid][ItemsInfo[i][itemId]]);
			}
		}

		for(new i=count+1;i<MAX_INVENTORY_ITEMS;i++) AddDialogListitem(playerid, "Trong");

		InventoryOpen{playerid} = 1;
		Dialog_ShowPages(playerid, DIALOG_INVENTORY, DIALOG_STYLE_TABLIST, szTitle, "Chon", "Dong", 10);
	}
	else if(type == INVENTORY_TYPE_TEXTDRAW) {
		if(InventoryOpen{playerid} == 1) DestroyInventoryTextDraws(playerid);
		CreateInventoryTextDraws(playerid);

		for(new i;i < 5;i++) TextDrawShowForPlayer(playerid, InvTD@Global[i]);

		new count = GetInventoryItems(playerid);

		for(new i; i < MAX_ITEM_PER_PAGE; i++)
		{
			if(PlayerItemList[playerid][i] == -1) break;
			new itemIndex = PlayerItemList[playerid][i];
			PlayerTextDrawSetPreviewModel(playerid, InvTD@Slot[playerid][i], ItemsInfo[itemIndex][itemModel]);
			PlayerTextDrawSetPreviewRot(playerid, InvTD@Slot[playerid][i], ItemsInfo[itemIndex][itemRotX], ItemsInfo[itemIndex][itemRotY], ItemsInfo[itemIndex][itemRotZ], ItemsInfo[itemIndex][itemZoom]);
			PlayerTextDrawSetString(playerid, InvTD@ItemName[playerid][i], "%s~n~(%d)", ItemsInfo[itemIndex][itemName], PlayerInfo[playerid][ItemsInfo[itemIndex][itemId]]);
		}
		
		for(new i=0;i<MAX_ITEM_PER_PAGE;i++) 
		{
			PlayerTextDrawShow(playerid, InvTD@Slot[playerid][i]);
			PlayerTextDrawShow(playerid, InvTD@ItemName[playerid][i]);
		}

		InventorySelected{playerid} = -1;
		InventoryCurrentPage{playerid} = 1;
		InventoryMaxPage{playerid} = ((count / MAX_ITEM_PER_PAGE) + 1 < 1) ? (1) : ((count / MAX_ITEM_PER_PAGE) + 1);
		InventoryOpen{playerid} = 1;

		PlayerTextDrawSetString(playerid, InvTD@OwnerName[playerid], "Tui do cua %s", GetPlayerNameEx(playerid));
		PlayerTextDrawShow(playerid, InvTD@OwnerName[playerid]);

		PlayerTextDrawSetString(playerid, InvTD@Page[playerid], "%d/%d", InventoryCurrentPage{playerid}, InventoryMaxPage{playerid});
		PlayerTextDrawShow(playerid, InvTD@Page[playerid]);

		SelectTextDraw(playerid, 0x9E6AF8FF);
	}
	return 1;
}

alias:inventory("tuido", "inv")
CMD:inventory(playerid, params[])
{
	ShowInventory(playerid, PlayerInfo[playerid][pInvType]);
	return 1;
}

CMD:additem(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 99999) 
        return SendUnAuth(playerid);
    
    for(new i = 0; i < sizeof(ItemsInfo); i++) {
        AddDialogListitem(playerid, "{FFFFFF}%d.\t%s", i, ItemsInfo[i][itemName]);
    }
    
    Dialog_ShowPages(playerid, DIALOG_ITEM_LIST, DIALOG_STYLE_TABLIST, "Danh sach vat pham", "Chon", "Dong", 15);
    return 1;
}

DialogPages:DIALOG_ITEM_LIST(playerid, response, listitem, inputtext[])
{
    if(!response) return 1;
    
    new string[128];
    format(string, sizeof(string), "{ffffff}Item: {00ff8e}%s\n{ffffff}Nhap so luong ban muon:", ItemsInfo[listitem][itemName]);
    
    AddItemID[playerid] = listitem;
    
    ShowPlayerDialog(playerid, DIALOG_ITEM_AMOUNT, DIALOG_STYLE_INPUT, "Nhap so luong", string, "Nhan", "Huy");
	return 1;
}

DialogPages:DIALOG_INVENTORY(playerid, response, listitem, inputtext[])
{
	if(!response) {
		ResetInventoryVariables(playerid);
		InventoryOpen{playerid} = 0;
		return 1;
	}

	if(listitem < 0 || listitem >= MAX_INVENTORY_ITEMS) 
		return SendClientMessageEx(playerid, COLOR_GREY, "Co loi xay ra voi tui do, vui long mo lai!");
	
	if(PlayerItemList[playerid][listitem] == -1) {
		return ShowInventory(playerid, PlayerInfo[playerid][pInvType]);
	}

	new title[128];
	format(title, _, "{FFFFFF}Vat pham: %s", ItemsInfo[PlayerItemList[playerid][listitem]][itemName]);
	ShowPlayerDialog(playerid, DIALOG_INVACTION, DIALOG_STYLE_LIST, title, "{FFFFFF}Su dung\n{CD4545}Vut bo", "Chon", "Quay lai");
	SetPVarInt(playerid, "ItemListSetected", listitem);
	return 1;
}

forward OnPlayerUseItem(playerid, itemIndex);
public OnPlayerUseItem(playerid, itemIndex)
{
	if(itemIndex < 0 || itemIndex >= sizeof(ItemsInfo)) return 0;
	
	if(ItemsInfo[itemIndex][itemUsed] == false) {
		SendClientMessageEx(playerid, COLOR_GREY, "Vat pham khong the su dung bang cach nay!");
		if(PlayerInfo[playerid][pInvType] == INVENTORY_TYPE_DIALOG)
			ShowInventory(playerid, INVENTORY_TYPE_DIALOG);

		return 1;
	}
	switch(ItemsInfo[itemIndex][itemId])
	{
		case pSprunk: PC_EmulateCommand(playerid, "/usesprunk");
		case pCigar: PC_EmulateCommand(playerid, "/usecigar");
	}

	if(InventoryOpen{playerid} == 1 && PlayerInfo[playerid][pInvType] == INVENTORY_TYPE_TEXTDRAW) {
		ShowInventoryPage(playerid, InventoryCurrentPage{playerid});
	}
	else if(InventoryOpen{playerid} == 1 && PlayerInfo[playerid][pInvType] == INVENTORY_TYPE_DIALOG) {
		ShowInventory(playerid, INVENTORY_TYPE_DIALOG);
	}
	return 1;
}

hook OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	if(InventoryOpen{playerid} == 1 && PlayerInfo[playerid][pInvType] == INVENTORY_TYPE_TEXTDRAW) {
		if(clickedid == INVALID_TEXT_DRAW || clickedid == InvTD@Global[2]) {
			for(new i;i < 5;i++) TextDrawHideForPlayer(playerid, InvTD@Global[i]);
			DestroyInventoryTextDraws(playerid);
			ResetInventoryVariables(playerid);
			InventoryOpen{playerid} = 0;
			CancelSelectTextDraw(playerid);
			return 1;
		}

		if(InventoryMaxPage{playerid} > 1) {
			if(clickedid == InvTD@Global[3]) {// left
				if(InventoryCurrentPage{playerid} > 1) {
					ShowInventoryPage(playerid, --InventoryCurrentPage{playerid});
				}
			}
			else if(clickedid == InvTD@Global[4]) {// right
				if(InventoryCurrentPage{playerid} < InventoryMaxPage{playerid}) {
					ShowInventoryPage(playerid, ++InventoryCurrentPage{playerid});
				}
			}
		}
	}
	return 1;
}

hook OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
	if(InventoryOpen{playerid} == 1 && PlayerInfo[playerid][pInvType] == INVENTORY_TYPE_TEXTDRAW) {
		for(new i; i < MAX_ITEM_PER_PAGE; i++) {
			if(playertextid == InvTD@Slot[playerid][i]) {
				new itemIndex = PlayerItemList[playerid][(InventoryCurrentPage{playerid}-1)*MAX_ITEM_PER_PAGE + i];
				if(itemIndex == -1) return 1;
				
				if(InventorySelected{playerid} == i) {
					InventorySelected{playerid} = -1;
					PlayerTextDrawSetString(playerid, InvTD@ItemInfo[playerid], "_");
					PlayerTextDrawShow(playerid, InvTD@ItemInfo[playerid]);

					new listitem = (InventoryCurrentPage{playerid}-1)*MAX_ITEM_PER_PAGE + i;
					if(listitem < 0 || listitem >= MAX_INVENTORY_ITEMS || PlayerItemList[playerid][listitem] == -1) 
						return SendClientMessageEx(playerid, COLOR_GREY, "Co loi xay ra voi tui do, vui long mo lai!");
					
					new title[128];
					format(title, _, "{FFFFFF}Vat pham: %s", ItemsInfo[PlayerItemList[playerid][listitem]][itemName]);
					ShowPlayerDialog(playerid, DIALOG_INVACTION, DIALOG_STYLE_LIST, title, "{FFFFFF}Su dung\n{CD4545}Vut bo", "Chon", "Quay lai");
					SetPVarInt(playerid, "ItemListSetected", listitem);
					return 1;
				}

				ShowItemInfo(playerid, itemIndex);
				InventorySelected{playerid} = i;
				PlayerPlaySound(playerid, 6801);
				return 1;
			}
		}
	}
	return 0;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_INVACTION)
	{
		if(!response) {
			if(PlayerInfo[playerid][pInvType] == INVENTORY_TYPE_DIALOG) {
				return ShowInventory(playerid, INVENTORY_TYPE_DIALOG);
			}
		}

		new itemIndex = PlayerItemList[playerid][GetPVarInt(playerid, "ItemListSetected")];

		if(itemIndex < 0 || itemIndex >= sizeof(ItemsInfo)) return 1;
		if(listitem == 0) // use
		{
			CallLocalFunction("OnPlayerUseItem", "ii", playerid, itemIndex);
			DeletePVar(playerid, "ItemListSetected");
		}
		else { // drop
			new string[128];
			format(string, sizeof(string), "{FFFFFF}Vat pham: %s\nSo luong hien co: %d\nNhap so luong muon vut:", 
				ItemsInfo[itemIndex][itemName],
				PlayerInfo[playerid][ItemsInfo[itemIndex][itemId]]
			);
			ShowPlayerDialog(playerid, DIALOG_DROP_AMOUNT, DIALOG_STYLE_INPUT, "Vut vat pham", string, "Vut", "Huy");
			SetPVarInt(playerid, "SelectedItem", itemIndex);
		}
	}

	if(dialogid == DIALOG_ITEM_AMOUNT)
	{
		if(!response) {
			PC_EmulateCommand(playerid, "/additem");
			return 1;
		}
		
		new amount = strval(inputtext);
		if(amount <= 0) {
			SendClientMessageEx(playerid, COLOR_GREY, "So luong phai lon hon 1.");
			return 1;
		}
		
		// add item
		new pInfo:variable = ItemsInfo[AddItemID[playerid]][itemId];
		PlayerInfo[playerid][variable] += amount;
		
		SendClientMessageEx(playerid, -1, "Ban da nhan duoc {FFFF00}%d {FFFFFF}%s!", amount, ItemsInfo[AddItemID[playerid]][itemName]);
	}
	return 1;
}

hook OnPlayerConnect(playerid)
{
	for(new i=0;i<MAX_ITEM_PER_PAGE;i++) 
	{
		InvTD@Slot[playerid][i] = INVALID_PLAYER_TEXT_DRAW;
		InvTD@ItemName[playerid][i] = INVALID_PLAYER_TEXT_DRAW;
	}

	InvTD@OwnerName[playerid] = INVALID_PLAYER_TEXT_DRAW;
	InvTD@ItemInfo[playerid] = INVALID_PLAYER_TEXT_DRAW;
	InvTD@Page[playerid] = INVALID_PLAYER_TEXT_DRAW;
	InventoryOpen{playerid} = 0;
	ResetInventoryVariables(playerid);
	return 1;
}