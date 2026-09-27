#include <td-actions>
#include <YSI/YSI_Coding/y_hooks>


// Dinh nghia dialog IDs
#define DIALOG_INVENTORY (11000)
#define DIALOG_ITEM_OPTIONS (11001)
#define DIALOG_DROP_AMOUNT (11002)
#define DIALOG_ITEM_LIST (11003)
#define DIALOG_ITEM_AMOUNT (11004)
#define DIALOG_DELETE_AMOUNT (11005) 
#define DIALOG_WEAPON_OPTIONS (11006)

// Struct cho dropped items
enum DroppedItemInfo {
    Float:dropX,
    Float:dropY,
    Float:dropZ,
    dropVariable,
    dropItemName[MAX_ITEM_NAME],
    dropQuantity,
    Text3D:labelID,
    dropObject,
    bool:isWeapon,
    weaponType,
    bool:isValid
}

#define MAX_DROPPED_ITEMS 100
new DroppedItems[MAX_DROPPED_ITEMS][DroppedItemInfo];
new DroppedItemCount = 0;

// Bien luu item dang chon
new SelectedItem[MAX_PLAYERS];

// Hàm kiểm tra loại vũ khí
GetWeaponType(weaponid) {
    if(weaponid >= 1 && weaponid <= 15) return 1; // Cận chiến
    if(weaponid >= 22 && weaponid <= 46) return 2; // Súng
    return 0; // Không phải vũ khí
}

// Hàm lấy tên vũ khí từ ID
GetWeaponNameFromID(weaponid, name[], size) {
    // Sử dụng hàm có sẵn của SAMP
    GetWeaponName(weaponid, name, size);
    return 1;
}

// Ham show inventory dialog (hoac textdraw neu pInvType == INVENTORY_TYPE_TEXTDRAW)
ShowPlayerInventory(playerid)
{
    if(PlayerInfo[playerid][pInvType] == INVENTORY_TYPE_TEXTDRAW) {
        ShowInvTD(playerid);
        return 1;
    }

    new string[2048], title[64];
    format(title, sizeof(title), "Inventory cua %s", GetPlayerNameEx(playerid));
    new count;

    // Show regular items first
    for(new i = 0; i < sizeof(ItemInventory); i++) 
    {
        new variable = ItemInventory[i][pVariable];
        new quantity = PlayerInfo[playerid][variable];
        
        if(quantity > 0) 
        {
            count++;
            format(string, sizeof(string), "%s%s\t%d\n", 
                string, 
                ItemInventory[i][NameItem],
                quantity
            );
        }
    }
    
    // Add weapons to inventory
    new myweapons[13][2];
    for(new i = 0; i < 13; i++) 
    {
        GetPlayerWeaponData(playerid, i, myweapons[i][0], myweapons[i][1]);
        if(myweapons[i][0] > 0 && PlayerInfo[playerid][pGuns][i] == myweapons[i][0])
        {
            count++;
            new weaponname[32];
            GetWeaponName(myweapons[i][0], weaponname, sizeof(weaponname));
            format(string, sizeof(string), "%s%s\t%d\n",
                string,
                weaponname,
                myweapons[i][1]
            );
        }
    }
    
    if(count > 0) return ShowPlayerDialog(playerid, DIALOG_INVENTORY, DIALOG_STYLE_TABLIST, title, string, "Chon", "Dong");
    else ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, title, "{ffffff}Khong co gi trong tui do!", "Thoat", "");
    return 1;
}

ShowWeaponOptions(playerid, weaponid, ammo) {
    new string[256];
    new weaponname[32];
    GetWeaponName(weaponid, weaponname, sizeof(weaponname));
    
    format(string, sizeof(string), "Vu khi: %s\nSo dan: %d\n\nChon hanh dong:",
        weaponname,
        ammo
    );
    
    ShowPlayerDialog(playerid, DIALOG_WEAPON_OPTIONS, DIALOG_STYLE_LIST, "Tuy chon vu khi",
        "Su dung\nVut", "Chon", "Quay lai");
        
    // Store weapon info for later use
    SetPVarInt(playerid, "SelectedWeaponID", weaponid);
    SetPVarInt(playerid, "SelectedWeaponAmmo", ammo);
    return 1;
}

DropPlayerWeapon(playerid, weaponid, ammo) {
    if(IsPlayerInAnyVehicle(playerid)) return SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban khong the lam dieu nay tren xe.");
    
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    
    new weaponname[32];
    GetWeaponName(weaponid, weaponname, sizeof(weaponname));
    
    new dropIdx = AddDroppedItem(
        weaponid,  // Using weaponid as variable
        weaponname,
        ammo,
        x, y, z,
        true,  // isWeapon = true
        GetWeaponType(weaponid)  // Set proper weapon type
    );
    
    if(dropIdx != -1) {
        // Remove weapon from player
        RemovePlayerWeapon(playerid, weaponid);
        PlayerInfo[playerid][pGuns][GetWeaponSlot(weaponid)] = 0;
        
        // Create notification
        new string[128];
        format(string, sizeof(string), "* %s da vut %s xuong dat.", 
            GetPlayerNameEx(playerid),
            weaponname
        );
        
        ApplyAnimation(playerid, "CARRY", "putdwn", 4.2, 0, 0, 0, 0, 999, 1, 0);
        ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
    }
    return 1;
}

// Ham show options dialog cho item duoc chon
ShowItemOptions(playerid, itemid) {
    new variable = ItemInventory[itemid][pVariable];
    new quantity = PlayerInfo[playerid][variable];

    new string[256];

    if(variable == pIDCard) {
        format(string, sizeof(string), "Item: %s\n\nChon hanh dong:", ItemInventory[itemid][NameItem]);
        ShowPlayerDialog(playerid, DIALOG_ITEM_OPTIONS, DIALOG_STYLE_LIST, "Tuy chon item",
            "Xem giay to\nCho xem giay to", "Chon", "Quay lai");
        SelectedItem[playerid] = itemid;
        return 1;
    }

    format(string, sizeof(string), "Item: %s\nSo luong: %d\n\nChon hanh dong:",
        ItemInventory[itemid][NameItem],
        quantity
    );

    new options[64] = "Su dung\nVut";
    if(PlayerInfo[playerid][pAdmin] > 1) strcat(options, "\nXoa", sizeof(options));
    ShowPlayerDialog(playerid, DIALOG_ITEM_OPTIONS, DIALOG_STYLE_LIST, "Tuy chon item",
        options, "Chon", "Quay lai");

    SelectedItem[playerid] = itemid;
    return 1;
}

// Thêm vật phẩm vào mặt đất với tham số rõ ràng
AddDroppedItem(variable, const itemname[], quantity, Float:x, Float:y, Float:z, bool:isWeaponItem = false, weapType = 0) {
    if(DroppedItemCount >= MAX_DROPPED_ITEMS) return -1;
    
    new idx = DroppedItemCount;
    
    DroppedItems[idx][dropX] = x;
    DroppedItems[idx][dropY] = y;
    DroppedItems[idx][dropZ] = z;
    DroppedItems[idx][dropVariable] = variable;
    format(DroppedItems[idx][dropItemName], MAX_ITEM_NAME, itemname);
    DroppedItems[idx][dropQuantity] = quantity;
    DroppedItems[idx][isValid] = true;
    DroppedItems[idx][isWeapon] = isWeaponItem;
    DroppedItems[idx][weaponType] = weapType;
    
    new string[128];
    
    if(isWeaponItem) {
        if(weapType == 1) { // Vũ khí cận chiến
            format(string, sizeof(string), "%s\nNhan {ffff44}[Y]{ffffff} de nhat", itemname);
        }
        else if(weapType == 2) { // Súng
            format(string, sizeof(string), "%s\n(So luong dan: %d)\nNhan {ffff44}[Y]{ffffff} de nhat", 
                itemname, quantity);
        }
        else { // Trường hợp không xác định
            format(string, sizeof(string), "%s\nNhan {ffff44}[Y]{ffffff} de nhat", itemname);
        }
    }
    else {
        format(string, sizeof(string), "%s\n(So luong: %d)\nNhan {ffff44}[Y]{ffffff} de nhat", 
            itemname, quantity);
    }
    
    DroppedItems[idx][labelID] = Create3DTextLabel(string, 0xFFFFFFFF, x, y, z-0.5, 10.0, 0, 1);
    DroppedItems[idx][dropObject] = CreateDynamicObject(19918, x, y, z-1.0, 0.0, 0.0, 0.0, -1, -1, -1, 100.0);
    
    DroppedItemCount++;
    return idx;
}

// Ham xoa dropped item
RemoveDroppedItem(itemid) {
    if(!DroppedItems[itemid][isValid]) return 0;
    
    Delete3DTextLabel(DroppedItems[itemid][labelID]);
    DestroyDynamicObject(DroppedItems[itemid][dropObject]);
    DroppedItems[itemid][isValid] = false;
    
    // Sap xep lai mang
    for(new i = itemid; i < DroppedItemCount - 1; i++) {
        DroppedItems[i][dropX] = DroppedItems[i+1][dropX];
        DroppedItems[i][dropY] = DroppedItems[i+1][dropY];
        DroppedItems[i][dropZ] = DroppedItems[i+1][dropZ];
        DroppedItems[i][dropVariable] = DroppedItems[i+1][dropVariable];
        format(DroppedItems[i][dropItemName], MAX_ITEM_NAME, DroppedItems[i+1][dropItemName]);
        DroppedItems[i][dropQuantity] = DroppedItems[i+1][dropQuantity];
        DroppedItems[i][labelID] = DroppedItems[i+1][labelID];
        DroppedItems[i][dropObject] = DroppedItems[i+1][dropObject];
        DroppedItems[i][isWeapon] = DroppedItems[i+1][isWeapon];
        DroppedItems[i][weaponType] = DroppedItems[i+1][weaponType];
        DroppedItems[i][isValid] = DroppedItems[i+1][isValid];
    }
    
    DroppedItemCount--;
    return 1;
}

// Ham kiem tra va nhat item gan nhat
PickupNearestItem(playerid) {
    if(GetPVarInt(playerid, "Injured") != 0 || PlayerCuffed[playerid] != 0 || PlayerInfo[playerid][pHospital] != 0) {
        SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong the lam dieu nay vao luc nay.");
        return 1;
    }
    
    new Float:px, Float:py, Float:pz;
    GetPlayerPos(playerid, px, py, pz);
    
    new Float:minDist = 3.0;
    new closestItem = -1;
    
    for(new i = 0; i < DroppedItemCount; i++) {
        if(!DroppedItems[i][isValid]) continue;
        
        new Float:dist = GetDistanceBetweenPoints(px, py, pz,
            DroppedItems[i][dropX],
            DroppedItems[i][dropY],
            DroppedItems[i][dropZ]);
            
        if(dist < minDist) {
            minDist = dist;
            closestItem = i;
        }
    }
    
    if(closestItem != -1) {
        // Xử lý nhặt vật phẩm
        new string[128];
        
        if(DroppedItems[closestItem][isWeapon]) {
            // Xử lý khi nhặt vũ khí
            new weaponid = DroppedItems[closestItem][dropVariable];
            new ammo = DroppedItems[closestItem][dropQuantity];
            
            GivePlayerWeapon(playerid, weaponid, ammo);
            PlayerInfo[playerid][pGuns][GetWeaponSlot(weaponid)] = weaponid;
            
            if(DroppedItems[closestItem][weaponType] == 1) { // Vũ khí cận chiến
                format(string, sizeof(string), "Ban da nhat {FFFF00}%s{FFFFFF}.", DroppedItems[closestItem][dropItemName]);
            }
            else { // Súng và các loại vũ khí khác
                format(string, sizeof(string), "Ban da nhat {FFFF00}%s{FFFFFF} (Dan: %d).", 
                    DroppedItems[closestItem][dropItemName],
                    ammo);
            }
        } 
        else {
            // Xử lý khi nhặt vật phẩm thông thường
            PlayerInfo[playerid][DroppedItems[closestItem][dropVariable]] += DroppedItems[closestItem][dropQuantity];
            
            format(string, sizeof(string), "Ban da nhat {FFFF00}%s{FFFFFF} (So luong: %d).",
                DroppedItems[closestItem][dropItemName],
                DroppedItems[closestItem][dropQuantity]);
        }
        
        SendClientMessage(playerid, -1, string);
        ApplyAnimation(playerid, "CARRY", "liftup", 4.2, 0, 0, 0, 0, 999, 1, 0);
        RemoveDroppedItem(closestItem);
        return 1;
    }
    return 0;
}

// Hook phim H de mo inventory va Y de nhat item
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    if(newkeys & KEY_CTRL_BACK) {
        if(IsPlayerInAnyVehicle(playerid)) return 1;
        // Toggle textdraw: nhan H lan 2 de dong
        if(PlayerInfo[playerid][pInvType] == INVENTORY_TYPE_TEXTDRAW && InvTDOpen[playerid]) {
            HideInvTD(playerid);
            return 1;
        }
        ShowPlayerInventory(playerid);
    }
    else if(newkeys & KEY_YES) {
        if(IsPlayerInAnyVehicle(playerid)) return 1;
        
        // Check if player is targeting another player
        if(GetPlayerTargetPlayer(playerid) != INVALID_PLAYER_ID && ProxDetectorS(5.0, playerid, GetPlayerTargetPlayer(playerid)) && !IsPlayerNPC(GetPlayerTargetPlayer(playerid)))
        {
            if(GetPVarInt(playerid, "TackleMode") == 1) {
                return 1;
            }
            new string[512];
            new name[1024];
            format(name, sizeof(name), "{ffffff} > Giao dich voi {FFFF00}%s", GetPlayerNameEx(GetPlayerTargetPlayer(playerid)));
            SetPVarString(playerid, "pInteractName", name);
            SetPVarInt(playerid, "pInteractID", GetPlayerTargetPlayer(playerid));
            
            format(string, sizeof(string), "Dua tien\nCho vat pham\nCho xem giay to\n");
            ShowPlayerDialog(playerid, INTERACTMAIN, DIALOG_STYLE_LIST, name, string, "Lua chon", "Huy bo");
            return 1;
        }
        
        PickupNearestItem(playerid);
    }
    return 1;
}

// Xu ly dialog responses
hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) {
    switch(dialogid) {
        case DIALOG_INVENTORY: {
            if(!response) return 1;
            
            new currentItem = 0;
            new weaponid = -1;
            new ammo = 0;
            
            for(new i = 0; i < sizeof(ItemInventory); i++) {
                if(PlayerInfo[playerid][ItemInventory[i][pVariable]] > 0) {
                    if(currentItem == listitem) {
                        ShowItemOptions(playerid, i);
                        return 1;
                    }
                    currentItem++;
                }
            }
            
            new myweapons[13][2];
            for(new i = 0; i < 13; i++) {
                GetPlayerWeaponData(playerid, i, myweapons[i][0], myweapons[i][1]);
                if(myweapons[i][0] > 0 && PlayerInfo[playerid][pGuns][i] == myweapons[i][0]) {
                    if(currentItem == listitem) {
                        weaponid = myweapons[i][0];
                        ammo = myweapons[i][1];
                        break;
                    }
                    currentItem++;
                }
            }
            ShowWeaponOptions(playerid, weaponid, ammo);
            /* if(isWeapon) {
                ShowWeaponOptions(playerid, weaponid, ammo);
            } */
        }
        
        case DIALOG_ITEM_OPTIONS: {
            if(!response) {
                ShowPlayerInventory(playerid);
                return 1;
            }
            
            new variable = ItemInventory[SelectedItem[playerid]][pVariable];
            
            switch(listitem) {
                case 0: { // Su dung
                    new itemid = SelectedItem[playerid];

                    switch(variable) 
                    {
                        case pBanhMi: {
                            ShowActionForPlayer(playerid, DIALOG_USEBANHMI, "Ban co muon su dung Banh Mi khong ?", .action_time = 10000);
                        }
                        
                        case pNuocLoc: {
                            ShowActionForPlayer(playerid, DIALOG_USENUOCLOC, "Ban co muon su dung Nuoc Loc khong ?", .action_time = 10000);
                        }
                        
                        /* case pMedkit: {
                            cmd_usemedkit(playerid, "");
                        } */
                        
                        case pTepCanSa: {
                            HienTextdraw(playerid, "Tep can sa co the dem di ban");
                        }
                        case pCanSa: {
                            HienTextdraw(playerid, "La can sa co the dem di ban");
                        }

                        case pPaper: {
                            //cmd_hoadon(playerid, "");
                            HienTextdraw(playerid, "Chuc nang Hoa don dang duoc phat trien...");
                        }

                        case pPizza: {
                            ShowActionForPlayer(playerid, DIALOG_USEPIZZA, "Ban co muon su dung Pizza khong ?", .action_time = 10000);
                        }

                        case pBunRieu: {
                            ShowActionForPlayer(playerid, DIALOG_USEBUNRIEU, "Ban co muon su dung Bun Rieu khong ?", .action_time = 10000);
                        }

                        case pPhoBo: {
                            ShowActionForPlayer(playerid, DIALOG_USEPHOBO, "Ban co muon su dung Pho Bo khong ?", .action_time = 10000);
                        }

                        case pHamburger: {
                            ShowActionForPlayer(playerid, DIALOG_USEHAMBURGER, "Ban co muon su dung Hamburger khong ?", .action_time = 10000);
                        }

                        case pCaPhe: {
                            ShowActionForPlayer(playerid, DIALOG_USECAPHE, "Ban co muon su dung Ca Phe khong ?", .action_time = 10000);
                        }

                        case pSuaBo: {
                            ShowActionForPlayer(playerid, DIALOG_USESUABO, "Ban co muon su dung Sua Bo khong ?", .action_time = 10000);
                        }

                        case pThit: {
                            ShowActionForPlayer(playerid, DIALOG_USETHITHUNKHOI, "Ban co muon su dung Thit Hun Khoi khong ?", .action_time = 10000);
                        }

                        case pTraiCay: {
                            ShowActionForPlayer(playerid, DIALOG_USETRAICAY, "Ban co muon su dung Trai Cay khong ?", .action_time = 10000);
                        }

                        case pNuocEp: {
                            ShowActionForPlayer(playerid, DIALOG_USENUOCEP, "Ban co muon su dung Nuoc Ep khong ?", .action_time = 10000);
                        }
                        
                        case pBoombox: {
                            pc_cmd_placeboombox(playerid, "");
                        }
                        
                        case pRadio: {
                            SendClientMessageEx(playerid, COLOR_GREY, "SU DUNG: /pr [chat]");
			                SendClientMessageEx(playerid, COLOR_GRAD2, "HINT: Su dung /tanso de thiet lap lai tan so radio cua ban.");
                        }
                        case pDice: {
                            pc_cmd_dice(playerid, "");
                        }
                        
                        /* case pTheDoiTen: {
                            cmd_doiten(playerid, "");
                        }

                        case pMask: {
                            cmd_mask(playerid, "");
                        } */
                        
                        case pRope: {
                            pc_cmd_tie(playerid, "");
                        }

                        case pCigar: {
                            pc_cmd_usecigar(playerid, "");
                        }
                        case pPot: {
                            pc_cmd_usepot(playerid, "");
                        }

                        case pMoiCau: {
                            if(PlayerInfo[playerid][pMoiCau] > 0) {
                                SendClientMessage(playerid, COLOR_GREY, "Goi y: /timduong va tim den vi tri cau ca");
                            }
                        }

                        case pIDCard: {
                            pc_cmd_licenses(playerid, "");
                        }

                        default: {
                            SendClientMessage(playerid, COLOR_RED, "Khong the su dung item nay!");
                        }
                    }
                }
                case 1: { // Vứt / Cho xem giay to (pIDCard)
                    if(variable == pIDCard) {
                        pc_cmd_choxemgiayphep(playerid, "");
                        return 1;
                    }
                    if(GetPVarInt(playerid, "Injured") != 0 || PlayerCuffed[playerid] != 0 || PlayerInfo[playerid][pHospital] != 0) {
                        SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong the lam dieu nay vao luc nay.");
                        return 1;
                    }

                    if(IsPlayerInAnyVehicle(playerid)) return SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban khong the lam dieu nay tren xe.");
                    
                    new string[128];
                    format(string, sizeof(string), "Ban dang co: %d %s\nNhap so luong muon vut:", 
                        PlayerInfo[playerid][variable],
                        ItemInventory[SelectedItem[playerid]][NameItem]
                    );
                    ShowPlayerDialog(playerid, DIALOG_DROP_AMOUNT, DIALOG_STYLE_INPUT, "Vut item", string, "Vut", "Huy");
                }
                case 2: { // Xóa (admin only)
                    if(PlayerInfo[playerid][pAdmin] <= 1) return 1;
                    if(GetPVarInt(playerid, "Injured") != 0 || PlayerCuffed[playerid] != 0 || PlayerInfo[playerid][pHospital] != 0) {
                        SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong the lam dieu nay vao luc nay.");
                        return 1;
                    }
                    new string[128];
                    format(string, sizeof(string), "Ban dang co: %d %s\nNhap so luong muon xoa:",
                        PlayerInfo[playerid][variable],
                        ItemInventory[SelectedItem[playerid]][NameItem]
                    );
                    ShowPlayerDialog(playerid, DIALOG_DELETE_AMOUNT, DIALOG_STYLE_INPUT, "Xoa item", string, "Xoa", "Huy");
                }
            }
        }

        case DIALOG_WEAPON_OPTIONS: {
            if(!response) {
                ShowPlayerInventory(playerid);
                return 1;
            }
            
            new weaponid = GetPVarInt(playerid, "SelectedWeaponID");
            new ammo = GetPVarInt(playerid, "SelectedWeaponAmmo");
            
            switch(listitem) {
                case 0: { // Sử dụng
                    if(GetPVarInt(playerid, "Injured") != 0 || PlayerCuffed[playerid] != 0 || PlayerInfo[playerid][pHospital] != 0) {
                        SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong the lam dieu nay vao luc nay.");
                        return 1;
                    }
                    ShowPlayerInventory(playerid);
                }
                case 1: { // Vứt
                    if(GetPVarInt(playerid, "Injured") != 0 || PlayerCuffed[playerid] != 0 || PlayerInfo[playerid][pHospital] != 0) {
                        SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong the lam dieu nay vao luc nay.");
                        return 1;
                    }

                    if(IsPlayerInAnyVehicle(playerid)) return SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban khong the lam dieu nay tren xe.");

                    new iGroupID = PlayerInfo[playerid][pMember];
                    if(0 <= iGroupID < MAX_GROUPS) return SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong the vut vu khi khi dang la thanh vien to chuc.");

                    DropPlayerWeapon(playerid, weaponid, ammo);
                    ShowPlayerInventory(playerid);
                }
            }
            
            DeletePVar(playerid, "SelectedWeaponID");
            DeletePVar(playerid, "SelectedWeaponAmmo");
        }
        
        case DIALOG_DROP_AMOUNT: {
            if(!response) {
                ShowItemOptions(playerid, SelectedItem[playerid]);
                return 1;
            }
            
            new amount = strval(inputtext);
            new variable = ItemInventory[SelectedItem[playerid]][pVariable];
            
            if(amount <= 0) {
                SendClientMessage(playerid, COLOR_RED, "So luong khong hop le!");
                ShowItemOptions(playerid, SelectedItem[playerid]);
                return 1;
            }
            
            if(amount > PlayerInfo[playerid][variable]) {
                SendClientMessage(playerid, COLOR_RED, "Ban khong co du so luong de vut!");
                ShowItemOptions(playerid, SelectedItem[playerid]);
                return 1;
            }
            
            // Lấy vị trí người chơi
            new Float:x, Float:y, Float:z;
            GetPlayerPos(playerid, x, y, z);
            
            // Tạo item dưới đất - chỉ định rõ ràng là vật phẩm thường
            new dropIdx = AddDroppedItem(
                variable,
                ItemInventory[SelectedItem[playerid]][NameItem],
                amount,
                x, y, z,
                false, // Không phải vũ khí
                0      // Loại vật phẩm thường
            );
            
            if(dropIdx != -1) {
                // Giảm số lượng item
                PlayerInfo[playerid][variable] -= amount;
                
                // Tạo text thông báo
                new string[128];
                format(string, sizeof(string), "* %s da vut %d %s xuong dat.", 
                    GetPlayerNameEx(playerid),
                    amount,
                    ItemInventory[SelectedItem[playerid]][NameItem]
                );
                
                ApplyAnimation(playerid, "CARRY", "putdwn", 4.2, 0, 0, 0, 0, 999, 1, 0); // hanh dong

                // Hiển thị text cho người chơi xung quanh
                ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
            }
            
            // Hiển thị lại inventory nếu còn item
            if(PlayerInfo[playerid][variable] > 0) {
                ShowItemOptions(playerid, SelectedItem[playerid]);
            } else {
                ShowPlayerInventory(playerid);
            }
        }
        case DIALOG_DELETE_AMOUNT: {
            if(!response) {
                ShowItemOptions(playerid, SelectedItem[playerid]);
                return 1;
            }
            
            new amount = strval(inputtext);
            new variable = ItemInventory[SelectedItem[playerid]][pVariable];
            
            if(amount <= 0) {
                SendClientMessage(playerid, COLOR_RED, "So luong khong hop le!");
                ShowItemOptions(playerid, SelectedItem[playerid]);
                return 1;
            }
            
            if(amount > PlayerInfo[playerid][variable]) {
                SendClientMessage(playerid, COLOR_RED, "Ban khong co du so luong de xoa!");
                ShowItemOptions(playerid, SelectedItem[playerid]);
                return 1;
            }
            
            // Giảm số lượng item
            PlayerInfo[playerid][variable] -= amount;
            
            // Thông báo
            new string[128];
            format(string, sizeof(string), "Ban da xoa {FFFF00}%d {FFFFFF}%s!", 
                amount, 
                ItemInventory[SelectedItem[playerid]][NameItem]
            );
            SendClientMessage(playerid, -1, string);
            
            // Hiển thị lại inventory nếu còn item
            if(PlayerInfo[playerid][variable] > 0) {
                ShowItemOptions(playerid, SelectedItem[playerid]);
            } else {
                ShowPlayerInventory(playerid);
            }
        }
        case DIALOG_ITEM_LIST: {
            if(!response) return 1;
            
            new string[128];
            format(string, sizeof(string), "Item: %s\nNhap so luong ban muon:", ItemInventory[listitem][NameItem]);
            
            // Lưu item được chọn để xử lý sau
            SelectedItem[playerid] = listitem;
            
            ShowPlayerDialog(playerid, DIALOG_ITEM_AMOUNT, DIALOG_STYLE_INPUT, "Nhap so luong", string, "Nhan", "Huy");
        }
        
        case DIALOG_ITEM_AMOUNT: {
            if(!response) {
                // Quay lại dialog danh sách item
                new string[2048];
                string = "Ten Item\tID\n";
                for(new i = 0; i < sizeof(ItemInventory); i++) {
                    format(string, sizeof(string), "%s%s\t%d\n", 
                        string, 
                        ItemInventory[i][NameItem],
                        ItemInventory[i][pVariable]
                    );
                }
                ShowPlayerDialog(playerid, DIALOG_ITEM_LIST, DIALOG_STYLE_TABLIST_HEADERS, "Danh sach Item", string, "Chon", "Huy");
                return 1;
            }
            
            new amount = strval(inputtext);
            if(amount <= 0) {
                SendClientMessage(playerid, COLOR_RED, "So luong khong hop le!");
                return 1;
            }
            
            // Thêm item vào inventory
            new variable = ItemInventory[SelectedItem[playerid]][pVariable];
            PlayerInfo[playerid][variable] += amount;
            
            // Thông báo
            new string[128];
            format(string, sizeof(string), "Ban da nhan {FFFF00}%d {FFFFFF}%s!", 
                amount, 
                ItemInventory[SelectedItem[playerid]][NameItem]
            );
            SendClientMessage(playerid, -1, string);
        }

        case INTERACTMAIN:
        {
            if(response)
            {
                new name[128];
                GetPVarString(playerid, "pInteractName", name, sizeof(name));
                
                switch(listitem) 
                {
                    case 0: // Pay
                    {
                        ShowPlayerDialog(playerid, INTERACTPAY, DIALOG_STYLE_INPUT, name, "Nhap so tien can dua", "Pay", "Huy bo");
                    }
                    case 1: // Give
                    {
                        new string[2048] = "{ffffff}Item\tSo luong hien co\n";
                        new count = 0;

                        // Check inventory for items
                        for(new i = 0; i < sizeof(ItemInventory); i++)
                        {
                            new variable = ItemInventory[i][pVariable];
                            if(PlayerInfo[playerid][variable] > 0) // Only show items player has
                            {
                                format(string, sizeof(string), "%s%s\t%d\n",
                                    string,
                                    ItemInventory[i][NameItem],
                                    PlayerInfo[playerid][variable]
                                );
                                count++;
                            }
                        }

                        if(count == 0) {
                            SendClientMessageEx(playerid, COLOR_GREY, "Ban khong co gi de cho ca.");
                            DeletePVar(playerid, "pInteractName");
                            DeletePVar(playerid, "pInteractID");
                            return 1;
                        }

                        ShowPlayerDialog(playerid, INTERACTGIVE2, DIALOG_STYLE_TABLIST_HEADERS, name, string, "Lua chon", "Huy bo");
                    }
                    case 2: // Show ID Card
                    {
                        new giveplayerid = GetPVarInt(playerid, "pInteractID");

                        if(!IsPlayerConnected(giveplayerid))
                        {
                            SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi da mat ket noi.");
                            DeletePVar(playerid, "pInteractName");
                            DeletePVar(playerid, "pInteractID");
                            return 1;
                        }

                        if(PlayerInfo[playerid][pIDCard] == 0)
                        {
                            SendClientMessageEx(playerid, COLOR_GREY, "Ban chua so huu Can cuoc cong dan!");
                            DeletePVar(playerid, "pInteractName");
                            DeletePVar(playerid, "pInteractID");
                            return 1;
                        }

                        new proxstring[128];
                        format(proxstring, sizeof(proxstring), "{FF8000}* {C2A2DA}%s dua giay to tuy than ra cho %s xem.", GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid));
                        ProxDetectorWrap(playerid, proxstring, 92, 30.0, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

                        new dlgbody[128];
                        format(dlgbody, sizeof(dlgbody), "{FFFFFF}%s muon cho ban xem giay to tuy than cua ho.\n{FFFF00}Ban co muon xem khong?", GetPlayerNameEx(playerid));
                        SetPVarInt(giveplayerid, "pCardViewFrom", playerid);
                        ShowPlayerDialog(giveplayerid, DIALOG_CARD_VIEW_REQUEST, DIALOG_STYLE_MSGBOX, "{FFFFFF}Kiem tra giay to", dlgbody, "Xem ngay", "Tu choi");

                        DeletePVar(playerid, "pInteractName");
                        DeletePVar(playerid, "pInteractID");
                    }
                }
            }
            else
            {
                DeletePVar(playerid, "pInteractName");
                DeletePVar(playerid, "pInteractID");
            }
        }

        case INTERACTPAY:
        {
            if(response)
            {
                new giveplayerid = GetPVarInt(playerid, "pInteractID");
                new amount = strval(inputtext);
                
                if(giveplayerid == INVALID_PLAYER_ID || !IsPlayerConnected(giveplayerid))
                    return SendClientMessageEx(playerid, COLOR_GREY, "Nguoi nhan khong hop le hoac da ngat ket noi.");

                if(amount < 1)
                    return SendClientMessageEx(playerid, COLOR_GREY, "So tien khong hop le. Vui long nhap so tien lon hon 0.");
                    
                if(GetPlayerCash(playerid) < amount)
                    return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong co du tien de dua.");
                    
                // Chuyển tiền
                GivePlayerCash(playerid, -amount);
                GivePlayerCash(giveplayerid, amount);
                
                // Thông báo
                new string[128];
                format(string, sizeof(string), "Ban da dua $%s cho %s.", number_format(amount), GetPlayerNameEx(giveplayerid));
                SendClientMessageEx(playerid, COLOR_GREEN, string);
                
                format(string, sizeof(string), "%s da dua ban $%s.", GetPlayerNameEx(playerid), number_format(amount));
                SendClientMessageEx(giveplayerid, COLOR_GREEN, string);
                
                // ProxDetectorWrap
                new proxstring[128];
                format(proxstring, sizeof(proxstring), "{FF8000}* {C2A2DA}%s dua mot so tien cho %s.", GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid));
                ProxDetectorWrap(playerid, proxstring, 92, 30.0, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

                // Animation
                ApplyAnimation(playerid, "DEALER", "shop_pay", 4.0, 0, 0, 0, 0, 0, 1);

                // Log
                new ip[32], ipex[32];
                GetPlayerIp(playerid, ip, sizeof(ip));
                GetPlayerIp(giveplayerid, ipex, sizeof(ipex));
                format(string, sizeof(string), "%s(IP:%s) da dua $%s cho %s(IP:%s)",
                    GetPlayerNameEx(playerid), ip, number_format(amount), GetPlayerNameEx(giveplayerid), ipex);
                Log("logs/pay.log", string);

                new DCC_Channel:channel = DCC_FindChannelById("1548216250415456376");
                new nofi[520], title[128];
                format(title, sizeof(title), "**%s (ID: %d) đã cho %s (ID: %d)**", GetPlayerNameEx(playerid), playerid, GetPlayerNameEx(giveplayerid), giveplayerid);
                format(nofi,sizeof(nofi),":money_with_wings: **SỐ TIỀN NHẬN ĐƯỢC: %s$**", number_format(amount));
                DiscordLog(channel, title, nofi);
            }
            
            DeletePVar(playerid, "pInteractName");
            DeletePVar(playerid, "pInteractID");
        }

        case INTERACTGIVE2: {
            if(response) {
                new currentItem = 0;
                new selectedItemId = -1;

                // Tìm item được chọn
                for(new i = 0; i < sizeof(ItemInventory); i++) {
                    if(PlayerInfo[playerid][ItemInventory[i][pVariable]] > 0) {
                        if(currentItem == listitem) {
                            selectedItemId = i;
                            break;
                        }
                        currentItem++;
                    }
                }

                if(selectedItemId != -1) {
                    SetPVarInt(playerid, "GiveItemID", selectedItemId);
                    
                    new string[128];
                    format(string, sizeof(string), "{ffffff}Ban dang co: {FFFF00}%d {ffffff}%s\nNhap so luong muon cho:", 
                        PlayerInfo[playerid][ItemInventory[selectedItemId][pVariable]],
                        ItemInventory[selectedItemId][NameItem]
                    );
                    
                    ShowPlayerDialog(playerid, INTERACTGIVE3, DIALOG_STYLE_INPUT, 
                        "{ffffff}Nhap so luong", string, "Cho", "Huy bo");
                }
            }
            else {
                DeletePVar(playerid, "pInteractName");
                DeletePVar(playerid, "pInteractID");
            }
        }

        case INTERACTGIVE3: {
            if(response) {
                new giveplayerid = GetPVarInt(playerid, "pInteractID");
                new itemid = GetPVarInt(playerid, "GiveItemID");
                new amount = strval(inputtext);

                if(!IsPlayerConnected(giveplayerid))
                    return SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi da mat ket noi.");

                if(amount <= 0)
                    return SendClientMessageEx(playerid, COLOR_GREY, "So luong khong hop le.");
                    
                if(PlayerInfo[playerid][ItemInventory[itemid][pVariable]] < amount)
                    return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong co du so luong de cho.");

                // Chuyển item
                PlayerInfo[playerid][ItemInventory[itemid][pVariable]] -= amount;
                PlayerInfo[giveplayerid][ItemInventory[itemid][pVariable]] += amount;

                // Thông báo
                new string[256];
                format(string, sizeof(string), "Ban da cho %s %d %s.", 
                    GetPlayerNameEx(giveplayerid), amount, ItemInventory[itemid][NameItem]);
                SendClientMessageEx(playerid, COLOR_GREEN, string);

                format(string, sizeof(string), "%s da cho ban %d %s.", 
                    GetPlayerNameEx(playerid), amount, ItemInventory[itemid][NameItem]);
                SendClientMessageEx(giveplayerid, COLOR_GREEN, string);

                format(string, sizeof(string), "{FF8000}* {C2A2DA}%s dua %s cho %s.",
                    GetPlayerNameEx(playerid), ItemInventory[itemid][NameItem], GetPlayerNameEx(giveplayerid));
                ProxDetectorWrap(playerid, string, 92, 30.0, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);

                // Animation
                ApplyAnimation(playerid, "DEALER", "shop_pay", 4.0, 0, 0, 0, 0, 0, 1);

                // Log
                new ip[32], ipex[32];
                GetPlayerIp(playerid, ip, sizeof(ip));
                GetPlayerIp(giveplayerid, ipex, sizeof(ipex));
                format(string, sizeof(string), "%s(IP:%s) da cho %s(IP:%s) %d %s",
                    GetPlayerNameEx(playerid), ip, GetPlayerNameEx(giveplayerid), ipex, 
                    amount, ItemInventory[itemid][NameItem]);
                Log("logs/give.log", string);

                new DCC_Channel:channel = DCC_FindChannelById("1331919236691398687");
                new nofi[520], title[128];
                format(title, sizeof(title), "**%s (ID: %d) đã cho %s (ID: %d)**", GetPlayerNameEx(playerid), playerid, GetPlayerNameEx(giveplayerid), giveplayerid);
                format(nofi,sizeof(nofi),":white_check_mark: **VẬT PHẨM NHẬN ĐƯỢC: %d %s**", amount, ItemInventory[itemid][NameItem]);
                DiscordLog(channel, title, nofi);
            }
            
            DeletePVar(playerid, "pInteractID");
            DeletePVar(playerid, "GiveItemID");
            DeletePVar(playerid, "pInteractName");
        }
    }
    return 1;
}

// Command test de them item
CMD:additem(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 99999) return 1;
    new string[2048];
    string = "Ten Item\tID\n";
    
    // Hiển thị danh sách item dạng tablist
    for(new i = 0; i < sizeof(ItemInventory); i++) {
        format(string, sizeof(string), "%s%s\t%s\n", 
            string, 
            ItemInventory[i][NameItem],
            ItemInventory[i][pVariable]
        );
    }
    
    ShowPlayerDialog(playerid, DIALOG_ITEM_LIST, DIALOG_STYLE_TABLIST_HEADERS, "Danh sach Item", string, "Chon", "Huy");
    return 1;
}

CMD:inv(playerid, params[])
{
    ShowPlayerInventory(playerid);
    return 1;
}

CMD:tuido(playerid, params[])
{
    ShowPlayerInventory(playerid);
    return 1;
}

/* CMD:give(playerid, params[])
{
    new targetid;
    if(sscanf(params, "u", targetid)) return SendClientMessage(playerid, -1, "Usage: /give [id/ten]");
    
    if(!IsPlayerConnected(targetid)) return SCM(playerid, "Nguoi choi khong hop le.");
    if(targetid == playerid)  return SCM(playerid, "Khong the giao dich voi chinh minh.");
    
    if(!IsPlayerNearPlayer(playerid, targetid, 3.0)) 
        return SCM(playerid, "Nguoi choi nay khong o gan ban.");
    
    if(GetPVarInt(playerid, "TackleMode") == 1) {
        return 1;
    }
    new string[64];
    new name[128];
    format(name, sizeof(name), "{ffffff} > Giao dich voi {FFFF00}%s", GetPlayerNameEx(GetPlayerTargetPlayer(playerid)));
    SetPVarString(playerid, "pInteractName", name);
    SetPVarInt(playerid, "pInteractID", GetPlayerTargetPlayer(playerid));
            
    format(string, sizeof(string), "Dua tien\nCho vat pham\n");
    ShowPlayerDialog(playerid, INTERACTMAIN, DIALOG_STYLE_LIST, name, string, "Lua chon", "Huy bo");

    return 1;
} */

Action:DIALOG_USEBANHMI(playerid, response)
{
    if (response == ACTION_RESPONSE_YES)
    {
        if(PlayerInfo[playerid][pThucAn] >= 100) return SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban da qua no de co the an them.");
        if(PlayerInfo[playerid][pBanhMi] >= 1)
        {
            if(!IsPlayerInAnyVehicle(playerid))
            {
                TogglePlayerControllable(playerid, 0);
            }
            SetPlayerAttachedObject(playerid, AT_SLOT_SAE, 19579, 6, 0.022, 0.044, 0.000, -86.399, 48.699, -86.500);
            SetLoading(playerid, 3000, "Dang an Banh Mi...", "OnPlayerUseBanhMi");

            if(!IsPlayerInAnyVehicle(playerid)){
				ApplyAnimation(playerid, "FOOD", "EAT_Chicken", 4.0,0,0,0,0,0,0);
			}
			else if(!IsABike(GetPlayerVehicleID(playerid)))
			{
				ApplyAnimation(playerid,"CAR_CHAT", "carfone_out",4.0,0,0,0,0,0,1);
			}
        }
            else SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban khong co o Banh Mi nao.");
    }
}

Action:DIALOG_USENUOCLOC(playerid, response)
{
    if (response == ACTION_RESPONSE_YES)
    {
        if(PlayerInfo[playerid][pNuoc] >= 100) return SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban da qua no de co the uong them.");
        if(PlayerInfo[playerid][pNuocLoc] >= 1)
        {
            if(!IsPlayerInAnyVehicle(playerid))
            {
                TogglePlayerControllable(playerid, 0);
            }
            SetPlayerAttachedObject(playerid, AT_SLOT_SAE, 19570, 5, 0.066, 0.058, 0.021, -176.799, 9.500, 0.000);
            SetLoading(playerid, 3000, "Dang uong Nuoc Loc...", "OnPlayerUseNuocLoc");

            if(!IsPlayerInAnyVehicle(playerid)){
				ApplyAnimation(playerid, "VENDING", "VEND_Drink2_P", 4.0,0,0,0,0,0,0);
			}
			else if(!IsABike(GetPlayerVehicleID(playerid)))
			{
				ApplyAnimation(playerid,"CAR_CHAT", "carfone_out",4.0,0,0,0,0,0,1);
			}
        }
        else SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban khong co chai Nuoc Loc nao.");
    }
}

Action:DIALOG_USEPIZZA(playerid, response)
{
    if (response == ACTION_RESPONSE_YES)
    {
        if(PlayerInfo[playerid][pThucAn] >= 100) return SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban da qua no de co the an them.");
        if(PlayerInfo[playerid][pPizza] >= 1)
        {
            if(!IsPlayerInAnyVehicle(playerid))
            {
                TogglePlayerControllable(playerid, 0);
            }
            SetPlayerAttachedObject(playerid, AT_SLOT_SAE, 2702, 6, 0.060, 0.105, 0.000, 0.000, 0.000, 0.000);
            SetLoading(playerid, 3000, "Dang an Pizza...", "OnPlayerUsePizza");
            if(!IsPlayerInAnyVehicle(playerid)){
				ApplyAnimation(playerid, "FOOD", "EAT_Pizza", 4.0,0,0,0,0,0,0);
			}
			else if(!IsABike(GetPlayerVehicleID(playerid)))
			{
				ApplyAnimation(playerid,"CAR_CHAT", "carfone_out",4.0,0,0,0,0,0,1);
			}
        }
        else SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban khong co banh Pizza nao.");
    }
}

Action:DIALOG_USEBUNRIEU(playerid, response)
{
    if (response == ACTION_RESPONSE_YES)
    {
        if(PlayerInfo[playerid][pThucAn] >= 100) return SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban da qua no de co the an them.");
        if(PlayerInfo[playerid][pBunRieu] >= 1)
        {
            if(!IsPlayerInAnyVehicle(playerid))
            {
                TogglePlayerControllable(playerid, 0);
            }
            SetPlayerAttachedObject(playerid, AT_SLOT_SAE, 19993, 6, 0.067, 0.041, 0, 0, 0, 32.4);
            SetLoading(playerid, 5000, "Dang an Bun Rieu...", "OnPlayerUseBunRieu");
            if(!IsPlayerInAnyVehicle(playerid)){
				ApplyAnimation(playerid, "FOOD", "EAT_Pizza", 4.0,0,0,0,0,0,0);
			}
			else if(!IsABike(GetPlayerVehicleID(playerid)))
			{
				ApplyAnimation(playerid,"CAR_CHAT", "carfone_out",4.0,0,0,0,0,0,1);
			}
        }
        else SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban khong co Bun Rieu nao.");
    }
}

Action:DIALOG_USEPHOBO(playerid, response)
{
    if (response == ACTION_RESPONSE_YES)
    {
        if(PlayerInfo[playerid][pThucAn] >= 100) return SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban da qua no de co the an them.");
        if(PlayerInfo[playerid][pPhoBo] >= 1)
        {
            if(!IsPlayerInAnyVehicle(playerid))
            {
                TogglePlayerControllable(playerid, 0);
            }
            SetPlayerAttachedObject(playerid, AT_SLOT_SAE, 19993, 6, 0.067, 0.041, 0, 0, 0, 32.4);
            SetLoading(playerid, 5000, "Dang an Pho Bo...", "OnPlayerUsePhoBo");
            if(!IsPlayerInAnyVehicle(playerid)){
				ApplyAnimation(playerid, "FOOD", "EAT_Pizza", 4.0,0,0,0,0,0,0);
			}
			else if(!IsABike(GetPlayerVehicleID(playerid)))
			{
				ApplyAnimation(playerid,"CAR_CHAT", "carfone_out",4.0,0,0,0,0,0,1);
			}
        }
        else SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban khong co Pho Bo nao.");
    }
}

Action:DIALOG_USEHAMBURGER(playerid, response)
{
    if (response == ACTION_RESPONSE_YES)
    {
        if(PlayerInfo[playerid][pThucAn] >= 100) return SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban da qua no de co the an them.");
        if(PlayerInfo[playerid][pHamburger] >= 1)
        {
            if(!IsPlayerInAnyVehicle(playerid))
            {
                TogglePlayerControllable(playerid, 0);
            }
            SetPlayerAttachedObject(playerid, AT_SLOT_SAE, 2703, 6, 0.067, 0.041, 0, 0, 0, 32.4);
            SetLoading(playerid, 4000, "Dang an Hamburger...", "OnPlayerUseHamburger");
            if(!IsPlayerInAnyVehicle(playerid)){
				ApplyAnimation(playerid, "FOOD", "EAT_Pizza", 4.0,0,0,0,0,0,0);
			}
			else if(!IsABike(GetPlayerVehicleID(playerid)))
			{
				ApplyAnimation(playerid,"CAR_CHAT", "carfone_out",4.0,0,0,0,0,0,1);
			}
        }
        else SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban khong co Hamburger nao.");
    }
}

Action:DIALOG_USECAPHE(playerid, response)
{
    if (response == ACTION_RESPONSE_YES)
    {
        if(PlayerInfo[playerid][pNuoc] >= 100) return SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban da qua no de co the uong them.");
        if(PlayerInfo[playerid][pCaPhe] >= 1)
        {
            if(!IsPlayerInAnyVehicle(playerid))
            {
                TogglePlayerControllable(playerid, 0);
            }
            SetPlayerAttachedObject(playerid, AT_SLOT_SAE, 19835, 5, 0.089, 0.038, -0.027, 160.999, -13.900, 0.000);
            SetLoading(playerid, 3000, "Dang uong Ca Phe...", "OnPlayerUseCaPhe");
            if(!IsPlayerInAnyVehicle(playerid)){
				ApplyAnimation(playerid, "VENDING", "VEND_Drink2_P", 4.0,0,0,0,0,0,0);
			}
			else if(!IsABike(GetPlayerVehicleID(playerid)))
			{
				ApplyAnimation(playerid,"CAR_CHAT", "carfone_out",4.0,0,0,0,0,0,1);
			}
        }
        else SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban khong co Ca Phe nao.");
    }
}

Action:DIALOG_USESUABO(playerid, response)
{
    if (response == ACTION_RESPONSE_YES)
    {
        if(PlayerInfo[playerid][pNuoc] >= 100) return SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban da qua no de co the uong them.");
        if(PlayerInfo[playerid][pSuaBo] >= 1)
        {
            if(!IsPlayerInAnyVehicle(playerid))
            {
                TogglePlayerControllable(playerid, 0);
            }
            SetPlayerAttachedObject(playerid, AT_SLOT_SAE, 19569, 5, 0.054, 0.069, 0.123, -178.199, 0.000, 10.700);
            SetLoading(playerid, 4000, "Dang uong Sua Bo...", "OnPlayerUseSuaBo");
            if(!IsPlayerInAnyVehicle(playerid)){
				ApplyAnimation(playerid, "VENDING", "VEND_Drink2_P", 4.0,0,0,0,0,0,0);
			}
			else if(!IsABike(GetPlayerVehicleID(playerid)))
			{
				ApplyAnimation(playerid,"CAR_CHAT", "carfone_out",4.0,0,0,0,0,0,1);
			}
        }
        else SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban khong co Sua Bo nao.");
    }
}

Action:DIALOG_USETHITHUNKHOI(playerid, response)
{
    if (response == ACTION_RESPONSE_YES)
    {
        if(PlayerInfo[playerid][pThucAn] >= 100) return SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban da qua no de co the an them.");
        if(PlayerInfo[playerid][pThit] >= 1)
        {
            if(!IsPlayerInAnyVehicle(playerid))
            {
                TogglePlayerControllable(playerid, 0);
            }
            SetPlayerAttachedObject(playerid, AT_SLOT_SAE, 19847, 6, 0.079, 0.015, 0.000, -87.699, 4.000, 9.899, 0.461, 0.518, 0.528);
            SetLoading(playerid, 4000, "Dang an Thit Hun Khoi...", "OnPlayerUseThitHunKhoi");
            if(!IsPlayerInAnyVehicle(playerid)){
				ApplyAnimation(playerid, "FOOD", "EAT_Chicken", 4.0,0,0,0,0,0,0);
			}
			else if(!IsABike(GetPlayerVehicleID(playerid)))
			{
				ApplyAnimation(playerid,"CAR_CHAT", "carfone_out",4.0,0,0,0,0,0,1);
			}
        }
        else SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban khong co Thit Hun Khoi nao.");
    }
}

Action:DIALOG_USETRAICAY(playerid, response)
{
    if (response == ACTION_RESPONSE_YES)
    {
        if(PlayerInfo[playerid][pThucAn] >= 100) return SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban da qua no de co the an them.");
        if(PlayerInfo[playerid][pTraiCay] >= 1)
        {
            if(!IsPlayerInAnyVehicle(playerid))
            {
                TogglePlayerControllable(playerid, 0);
            }
            SetPlayerAttachedObject(playerid, AT_SLOT_SAE, 19574 + random(2), 6, 0.059, 0.056, 0.018);
            SetLoading(playerid, 3000, "Dang an Trai Cay...", "OnPlayerUseTraiCay");
            if(!IsPlayerInAnyVehicle(playerid)){
				ApplyAnimation(playerid, "FOOD", "EAT_Pizza", 4.0,0,0,0,0,0,0);
			}
			else if(!IsABike(GetPlayerVehicleID(playerid)))
			{
				ApplyAnimation(playerid,"CAR_CHAT", "carfone_out",4.0,0,0,0,0,0,1);
			}
        }
        else SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban khong co Trai Cay nao.");
    }
}

Action:DIALOG_USENUOCEP(playerid, response)
{
    if (response == ACTION_RESPONSE_YES)
    {
        if(PlayerInfo[playerid][pNuoc] >= 100) return SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban da qua no de co the uong them.");
        if(PlayerInfo[playerid][pNuocEp] >= 1)
        {
            if(!IsPlayerInAnyVehicle(playerid))
            {
                TogglePlayerControllable(playerid, 0);
            }
            SetPlayerAttachedObject(playerid, AT_SLOT_SAE, 19563 + random(2), 5, 0.076, 0.037, 0.160, -175.899, 0.000, 9.599);
            SetLoading(playerid, 4000, "Dang uong Nuoc Ep...", "OnPlayerUseNuocEp");
            if(!IsPlayerInAnyVehicle(playerid)){
				ApplyAnimation(playerid, "VENDING", "VEND_Drink2_P", 4.0,0,0,0,0,0,0);
			}
			else if(!IsABike(GetPlayerVehicleID(playerid)))
			{
				ApplyAnimation(playerid,"CAR_CHAT", "carfone_out",4.0,0,0,0,0,0,1);
			}
        }
        else SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban khong co Nuoc Ep nao.");
    }
}

forward OnPlayerUseBanhMi(playerid);
forward OnPlayerUseNuocLoc(playerid);
forward OnPlayerUsePizza(playerid);
forward OnPlayerUseBunRieu(playerid);
forward OnPlayerUsePhoBo(playerid);
forward OnPlayerUseHamburger(playerid);
forward OnPlayerUseCaPhe(playerid);
forward OnPlayerUseSuaBo(playerid);
forward OnPlayerUseThitHunKhoi(playerid);
forward OnPlayerUseTraiCay(playerid);
forward OnPlayerUseNuocEp(playerid);

public OnPlayerUseBanhMi(playerid)
{
    if(!IsPlayerInAnyVehicle(playerid))
	{
		ClearAnimations(playerid);
	}

	TogglePlayerControllable(playerid, 1);
	RemovePlayerAttachedObject(playerid, AT_SLOT_SAE);
	new rand = 15 + random(10);
	PlayerInfo[playerid][pThucAn] += rand;
	PlayerInfo[playerid][pBanhMi] -= 1;
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Ban da an 1 o Banh Mi.[/suckhoe]");
}

public OnPlayerUseNuocLoc(playerid)
{
    if(!IsPlayerInAnyVehicle(playerid))
	{
		ClearAnimations(playerid);
	}

    TogglePlayerControllable(playerid, 1);
    RemovePlayerAttachedObject(playerid, AT_SLOT_SAE);
	new rand = 15 + random(10);
	PlayerInfo[playerid][pNuoc] += rand;
	PlayerInfo[playerid][pNuocLoc] -= 1;
	ShowTDN(playerid, NOTIFICATION_SUCCESS, "Ban da uong 1 chai Nuoc Loc.[/suckhoe]");
}

public OnPlayerUsePizza(playerid)
{
    if(!IsPlayerInAnyVehicle(playerid))
    {
        ClearAnimations(playerid);
    }

    TogglePlayerControllable(playerid, 1);
    RemovePlayerAttachedObject(playerid, AT_SLOT_SAE);
    new rand = 60 + random(10);
    PlayerInfo[playerid][pThucAn] += rand;
    PlayerInfo[playerid][pPizza] -= 1;
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Ban da an 1 banh Pizza.[/suckhoe]");
    return 1;
}

public OnPlayerUseBunRieu(playerid)
{
    if(!IsPlayerInAnyVehicle(playerid))
    {
        ClearAnimations(playerid);
    }

    TogglePlayerControllable(playerid, 1);
    RemovePlayerAttachedObject(playerid, AT_SLOT_SAE);
    new rand = 35 + random(10);
    new rand1 = 10 + random(10);
    PlayerInfo[playerid][pThucAn] += rand;
    PlayerInfo[playerid][pNuoc] += rand1;
    PlayerInfo[playerid][pBunRieu] -= 1;
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Ban da an 1 Bun Rieu.[/suckhoe]");
    return 1;
}

public OnPlayerUsePhoBo(playerid)
{
    if(!IsPlayerInAnyVehicle(playerid))
    {
        ClearAnimations(playerid);
    }

    TogglePlayerControllable(playerid, 1);
    RemovePlayerAttachedObject(playerid, AT_SLOT_SAE);
    new rand = 40 + random(10);
    new rand1 = 10 + random(10);
    PlayerInfo[playerid][pThucAn] += rand;
    PlayerInfo[playerid][pNuoc] += rand1;
    PlayerInfo[playerid][pPhoBo] -= 1;
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Ban da an 1 Pho Bo.[/suckhoe]"); // sửa lại cho đúng món
    return 1;
}

public OnPlayerUseHamburger(playerid)
{
    if(!IsPlayerInAnyVehicle(playerid))
    {
        ClearAnimations(playerid);
    }

    TogglePlayerControllable(playerid, 1);
    RemovePlayerAttachedObject(playerid, AT_SLOT_SAE);
    new rand = 35 + random(10);
    new rand1 = 10 + random(10);
    PlayerInfo[playerid][pThucAn] += rand;
    PlayerInfo[playerid][pNuoc] += rand1;
    PlayerInfo[playerid][pHamburger] -= 1;
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Ban da an 1 Hamburger.[/suckhoe]");
    return 1;
}

public OnPlayerUseCaPhe(playerid)
{
    if(!IsPlayerInAnyVehicle(playerid))
    {
        ClearAnimations(playerid);
    }

    TogglePlayerControllable(playerid, 1);
    RemovePlayerAttachedObject(playerid, AT_SLOT_SAE);
    new rand = 10 + random(10);
    new rand1 = 25 + random(10);
    PlayerInfo[playerid][pThucAn] += rand;
    PlayerInfo[playerid][pNuoc] += rand1;
    PlayerInfo[playerid][pCaPhe] -= 1;
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Ban da uong Ca phe.[/suckhoe]");
    return 1;
}

public OnPlayerUseSuaBo(playerid)
{
    if(!IsPlayerInAnyVehicle(playerid))
    {
        ClearAnimations(playerid);
    }

    TogglePlayerControllable(playerid, 1);
    RemovePlayerAttachedObject(playerid, AT_SLOT_SAE);
    new rand = 10 + random(10);
    new rand1 = 20 + random(10);
    PlayerInfo[playerid][pThucAn] += rand;
    PlayerInfo[playerid][pNuoc] += rand1;
    PlayerInfo[playerid][pSuaBo] -= 1;
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Ban da uong 1 Sua Bo.[/suckhoe]");
    return 1;
}

public OnPlayerUseThitHunKhoi(playerid)
{
    if(!IsPlayerInAnyVehicle(playerid))
    {
        ClearAnimations(playerid);
    }

    TogglePlayerControllable(playerid, 1);
    RemovePlayerAttachedObject(playerid, AT_SLOT_SAE);
    new rand = 40 + random(10);
    //new rand1 = 10 + random(10);
    PlayerInfo[playerid][pThucAn] += rand;
    //PlayerInfo[playerid][pNuoc] += rand1;
    PlayerInfo[playerid][pThit] -= 1;
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Ban da an 1 Thit Hun Khoi.[/suckhoe]");
    return 1;
}

public OnPlayerUseTraiCay(playerid)
{
    if(!IsPlayerInAnyVehicle(playerid))
    {
        ClearAnimations(playerid);
    }

    TogglePlayerControllable(playerid, 1);
    RemovePlayerAttachedObject(playerid, AT_SLOT_SAE);
    new rand = 15 + random(10);
    new rand1 = 10 + random(10);
    PlayerInfo[playerid][pThucAn] += rand;
    PlayerInfo[playerid][pNuoc] += rand1;
    PlayerInfo[playerid][pTraiCay] -= 1;
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Ban da an 1 Trai cay.[/suckhoe]");
    return 1;
}

public OnPlayerUseNuocEp(playerid)
{
    if(!IsPlayerInAnyVehicle(playerid))
    {
        ClearAnimations(playerid);
    }

    TogglePlayerControllable(playerid, 1);
    RemovePlayerAttachedObject(playerid, AT_SLOT_SAE);
    new rand = 10 + random(10);
    new rand1 = 30 + random(10);
    PlayerInfo[playerid][pThucAn] += rand;
    PlayerInfo[playerid][pNuoc] += rand1;
    PlayerInfo[playerid][pNuocEp] -= 1;
    ShowTDN(playerid, NOTIFICATION_SUCCESS, "Ban da uong 1 Nuoc Ep.[/suckhoe]");
    return 1;
}
