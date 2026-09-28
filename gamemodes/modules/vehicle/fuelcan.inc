// ============================================================
//  Fuel Can System (Binh xang)
//  - Mua binh xang rong tai 24/7 ($5000)
//  - /muaxang   : do xang vao binh khi dung tai cay xang (khong tren xe)
//  - /dungmuaxang : dung do xang vao binh
//  - Inventory -> Su dung : do xang tu binh vao xe gan nhat
//  - Inventory -> Vut      : vut binh xuong dat (ham DropFuelCan)
//  - pBinhXang logic:
//      0.0               = khong co binh
//      FUELCAN_EMPTY_MARKER (0.1) = co binh nhung rong, player thay "0.0 L"
//      X + 0.1 (X > 0)   = co binh chua X lit xang
// ============================================================

forward FuelCanFillTimer(playerid, iBusinessID, iPumpID);
forward FuelCanPourTimer(playerid, vehicleid);
public FuelCanFillTimer(playerid, iBusinessID, iPumpID)
{
    if(GetPVarInt(playerid, "FillingFuelCan") != 1) {
        KillTimer(GetPVarInt(playerid, "FuelCanTimer"));
        DeletePVar(playerid, "FuelCanTimer");
        return 1;
    }

    if(!IsPlayerConnected(playerid)) {
        KillTimer(GetPVarInt(playerid, "FuelCanTimer"));
        DeletePVar(playerid, "FuelCanTimer");
        return 1;
    }

    if(PlayerInfo[playerid][pBinhXang] <= 0.0) {
        SendClientMessageEx(playerid, COLOR_RED, "Ban khong co binh xang.");
        KillTimer(GetPVarInt(playerid, "FuelCanTimer"));
        DeletePVar(playerid, "FuelCanTimer");
        DeletePVar(playerid, "FillingFuelCan");
        DeletePVar(playerid, "FuelCanStartX");
        DeletePVar(playerid, "FuelCanStartY");
        DeletePVar(playerid, "FuelCanStartZ");
        return 1;
    }

    // Kiem tra nguoi choi di chuyen > 3m
    new Float:spx = GetPVarFloat(playerid, "FuelCanStartX");
    new Float:spy = GetPVarFloat(playerid, "FuelCanStartY");
    new Float:spz = GetPVarFloat(playerid, "FuelCanStartZ");
    if(GetPlayerDistanceFromPoint(playerid, spx, spy, spz) > 3.0) {
        SendClientMessageEx(playerid, COLOR_RED, "Ban da di chuyen, da huy viec do xang vao binh.");
        KillTimer(GetPVarInt(playerid, "FuelCanTimer"));
        DeletePVar(playerid, "FuelCanTimer");
        DeletePVar(playerid, "FillingFuelCan");
        DeletePVar(playerid, "FuelCanStartX");
        DeletePVar(playerid, "FuelCanStartY");
        DeletePVar(playerid, "FuelCanStartZ");
        return 1;
    }

    new Float:fPumpAmount = 0.15; // 1.5L/tick (1 gallon = 10L)
    new Float:fFuelInCan  = PlayerInfo[playerid][pBinhXang] - FUELCAN_EMPTY_MARKER;
    if(fFuelInCan < 0.0) fFuelInCan = 0.0;

    if(fFuelInCan >= FUELCAN_MAX_LITERS) {
        PlayerInfo[playerid][pBinhXang] = FUELCAN_MAX_LITERS + FUELCAN_EMPTY_MARKER;
        SendClientMessageEx(playerid, COLOR_GREEN, "Binh xang da day (35.0 L).");
        GameTextForPlayer(playerid, "~g~Binh xang day!", 3000, 3);
        KillTimer(GetPVarInt(playerid, "FuelCanTimer"));
        DeletePVar(playerid, "FuelCanTimer");
        DeletePVar(playerid, "FillingFuelCan");
        DeletePVar(playerid, "FuelCanStartX");
        DeletePVar(playerid, "FuelCanStartY");
        DeletePVar(playerid, "FuelCanStartZ");
        return 1;
    }

    if(fPumpAmount > Businesses[iBusinessID][GasPumpGallons][iPumpID]) {
        SendClientMessageEx(playerid, COLOR_RED, "Tram xang het hang.");
        KillTimer(GetPVarInt(playerid, "FuelCanTimer"));
        DeletePVar(playerid, "FuelCanTimer");
        DeletePVar(playerid, "FillingFuelCan");
        DeletePVar(playerid, "FuelCanStartX");
        DeletePVar(playerid, "FuelCanStartY");
        DeletePVar(playerid, "FuelCanStartZ");
        return 1;
    }

    if(GetPlayerCash(playerid) < floatround(Businesses[iBusinessID][GasPumpSalePrice][iPumpID], floatround_ceil)) {
        SendClientMessageEx(playerid, COLOR_RED, "Ban het tien.");
        KillTimer(GetPVarInt(playerid, "FuelCanTimer"));
        DeletePVar(playerid, "FuelCanTimer");
        DeletePVar(playerid, "FillingFuelCan");
        DeletePVar(playerid, "FuelCanStartX");
        DeletePVar(playerid, "FuelCanStartY");
        DeletePVar(playerid, "FuelCanStartZ");
        return 1;
    }

    new Float:fAddAmount = 1.5;
    if(fFuelInCan + fAddAmount > FUELCAN_MAX_LITERS)
        fAddAmount = FUELCAN_MAX_LITERS - fFuelInCan;

    Businesses[iBusinessID][GasPumpGallons][iPumpID] -= fPumpAmount;
    Businesses[iBusinessID][GasPumpSaleGallons][iPumpID] += fPumpAmount;
    Businesses[iBusinessID][GasPumpSalePrice][iPumpID] += fPumpAmount * Businesses[iBusinessID][bGasPrice];

    new iCost = floatround(fPumpAmount * Businesses[iBusinessID][bGasPrice], floatround_ceil);
    GivePlayerCash(playerid, -iCost);

    PlayerInfo[playerid][pBinhXang] = fFuelInCan + fAddAmount + FUELCAN_EMPTY_MARKER;

    new Float:fCurrentFuel = PlayerInfo[playerid][pBinhXang] - FUELCAN_EMPTY_MARKER;
    new szGameText[48];
    format(szGameText, sizeof(szGameText), "~w~Binh xang: ~g~%.1f ~w~/ 35.0 L", fCurrentFuel);
    SendMessageTD(playerid, 800, szGameText);

    if(fCurrentFuel >= FUELCAN_MAX_LITERS) {
        PlayerInfo[playerid][pBinhXang] = FUELCAN_MAX_LITERS + FUELCAN_EMPTY_MARKER;
        SendClientMessageEx(playerid, COLOR_GREEN, "Binh xang da day (35.0 L).");
        KillTimer(GetPVarInt(playerid, "FuelCanTimer"));
        DeletePVar(playerid, "FuelCanTimer");
        DeletePVar(playerid, "FillingFuelCan");
        DeletePVar(playerid, "FuelCanStartX");
        DeletePVar(playerid, "FuelCanStartY");
        DeletePVar(playerid, "FuelCanStartZ");
    }

    return 1;
}

// Do xang vao binh khi dung tai tram xang (khong tren xe)
alias:muaxang("fillcan")
CMD:muaxang(playerid, params[])
{
    if(IsPlayerInAnyVehicle(playerid))
        return SendClientMessageEx(playerid, COLOR_RED, "Ban phai ra khoi xe truoc.");

    if(GetPVarInt(playerid, "FillingFuelCan") == 1) {
        SetPVarInt(playerid, "FillingFuelCan", 0);
        KillTimer(GetPVarInt(playerid, "FuelCanTimer"));
        DeletePVar(playerid, "FuelCanTimer");
        DeletePVar(playerid, "FuelCanStartX");
        DeletePVar(playerid, "FuelCanStartY");
        DeletePVar(playerid, "FuelCanStartZ");
        SendClientMessageEx(playerid, COLOR_GREEN, "Da dung do xang vao binh.");
        return 1;
    }

    if(PlayerInfo[playerid][pBinhXang] <= 0.0)
        return SendClientMessageEx(playerid, COLOR_RED, "Ban khong co binh xang. Mua tai cua hang 24/7 ($5000).");

    new Float:fFuelInCan = PlayerInfo[playerid][pBinhXang] - FUELCAN_EMPTY_MARKER;
    if(fFuelInCan < 0.0) fFuelInCan = 0.0;
    if(fFuelInCan >= FUELCAN_MAX_LITERS)
        return SendClientMessageEx(playerid, COLOR_RED, "Binh xang da day (35.0 L).");

    new iBusinessID, iPumpID;
    GetClosestGasPump(playerid, iBusinessID, iPumpID);

    if(iBusinessID == INVALID_BUSINESS_ID)
        return SendClientMessageEx(playerid, COLOR_RED, "Ban khong o gan tram xang.");

    if(!Businesses[iBusinessID][bStatus])
        return SendClientMessageEx(playerid, COLOR_RED, "Tram xang da dong cua.");

    if(Businesses[iBusinessID][GasPumpGallons][iPumpID] <= 0.0)
        return SendClientMessageEx(playerid, COLOR_RED, "Tram xang nay het xang.");

    if(Businesses[iBusinessID][GasPumpVehicleID][iPumpID] > 0)
        return SendClientMessageEx(playerid, COLOR_RED, "Bom xang dang ban, vui long cho.");

    Businesses[iBusinessID][GasPumpSaleGallons][iPumpID] = 0.0;
    Businesses[iBusinessID][GasPumpSalePrice][iPumpID] = 0.0;

    new Float:spx, Float:spy, Float:spz;
    GetPlayerPos(playerid, spx, spy, spz);
    SetPVarFloat(playerid, "FuelCanStartX", spx);
    SetPVarFloat(playerid, "FuelCanStartY", spy);
    SetPVarFloat(playerid, "FuelCanStartZ", spz);

    SetPVarInt(playerid, "FillingFuelCan", 1);
    new iTimer = SetTimerEx("FuelCanFillTimer", 500, true, "iii", playerid, iBusinessID, iPumpID);
    SetPVarInt(playerid, "FuelCanTimer", iTimer);

    SendClientMessageEx(playerid, COLOR_WHITE, "Dang do xang vao binh, vui long cho...");
    SendClientMessageEx(playerid, COLOR_YELLOW, "Su dung /muaxang hoac /dungmuaxang de dung lai.");

    return 1;
}

CMD:dungmuaxang(playerid, params[])
{
    if(GetPVarInt(playerid, "FillingFuelCan") != 1)
        return SendClientMessageEx(playerid, COLOR_RED, "Ban khong dang do xang vao binh.");

    SetPVarInt(playerid, "FillingFuelCan", 0);
    KillTimer(GetPVarInt(playerid, "FuelCanTimer"));
    DeletePVar(playerid, "FuelCanTimer");
    DeletePVar(playerid, "FuelCanStartX");
    DeletePVar(playerid, "FuelCanStartY");
    DeletePVar(playerid, "FuelCanStartZ");

    new Float:fFuelInCan = PlayerInfo[playerid][pBinhXang] - FUELCAN_EMPTY_MARKER;
    if(fFuelInCan < 0.0) fFuelInCan = 0.0;
    new string[64];
    format(string, sizeof(string), "Da dung do xang. Binh xang: %.1f L.", fFuelInCan);
    SendClientMessageEx(playerid, COLOR_GREEN, string);
    return 1;
}

// Timer: do xang tu binh vao xe (500ms/tick, 0.5L/tick)
public FuelCanPourTimer(playerid, vehicleid)
{
    if(GetPVarInt(playerid, "PouringFuelCan") != 1) {
        KillTimer(GetPVarInt(playerid, "FuelCanPourTimerID"));
        DeletePVar(playerid, "FuelCanPourTimerID");
        return 1;
    }
    if(!IsPlayerConnected(playerid)) {
        KillTimer(GetPVarInt(playerid, "FuelCanPourTimerID"));
        DeletePVar(playerid, "FuelCanPourTimerID");
        return 1;
    }

    // Kiem tra nguoi choi di chuyen > 3m
    new Float:spx = GetPVarFloat(playerid, "PourStartX");
    new Float:spy = GetPVarFloat(playerid, "PourStartY");
    new Float:spz = GetPVarFloat(playerid, "PourStartZ");
    if(GetPlayerDistanceFromPoint(playerid, spx, spy, spz) > 3.0) {
        SendClientMessageEx(playerid, COLOR_RED, "Ban da di chuyen, da huy viec do xang vao xe.");
        KillTimer(GetPVarInt(playerid, "FuelCanPourTimerID"));
        DeletePVar(playerid, "FuelCanPourTimerID");
        DeletePVar(playerid, "PouringFuelCan");
        DeletePVar(playerid, "PourStartX");
        DeletePVar(playerid, "PourStartY");
        DeletePVar(playerid, "PourStartZ");
        return 1;
    }

    new Float:fFuelInCan = PlayerInfo[playerid][pBinhXang] - FUELCAN_EMPTY_MARKER;
    if(fFuelInCan < 0.0) fFuelInCan = 0.0;

    if(fFuelInCan <= 0.0) {
        SendClientMessageEx(playerid, COLOR_GREEN, "Binh xang da het. Hay den tram xang de nap them.");
        KillTimer(GetPVarInt(playerid, "FuelCanPourTimerID"));
        DeletePVar(playerid, "FuelCanPourTimerID");
        DeletePVar(playerid, "PouringFuelCan");
        DeletePVar(playerid, "PourStartX");
        DeletePVar(playerid, "PourStartY");
        DeletePVar(playerid, "PourStartZ");
        return 1;
    }

    if(!GetVehicleModel(vehicleid)) {
        SendClientMessageEx(playerid, COLOR_RED, "Xe khong con o day.");
        KillTimer(GetPVarInt(playerid, "FuelCanPourTimerID"));
        DeletePVar(playerid, "FuelCanPourTimerID");
        DeletePVar(playerid, "PouringFuelCan");
        DeletePVar(playerid, "PourStartX");
        DeletePVar(playerid, "PourStartY");
        DeletePVar(playerid, "PourStartZ");
        return 1;
    }

    new Float:fVehicleMax = GetVehicleFuelCapacity(vehicleid);
    if(VehicleFuel[vehicleid] >= fVehicleMax) {
        new szMsg[64];
        format(szMsg, sizeof(szMsg), "Xe da day xang (%.1f L).", fVehicleMax);
        SendClientMessageEx(playerid, COLOR_GREEN, szMsg);
        KillTimer(GetPVarInt(playerid, "FuelCanPourTimerID"));
        DeletePVar(playerid, "FuelCanPourTimerID");
        DeletePVar(playerid, "PouringFuelCan");
        DeletePVar(playerid, "PourStartX");
        DeletePVar(playerid, "PourStartY");
        DeletePVar(playerid, "PourStartZ");
        return 1;
    }

    new Float:fPour = 0.5;
    new Float:fVehicleNeed = fVehicleMax - VehicleFuel[vehicleid];
    if(fPour > fFuelInCan) fPour = fFuelInCan;
    if(fPour > fVehicleNeed) fPour = fVehicleNeed;

    VehicleFuel[vehicleid] += fPour;
    if(VehicleFuel[vehicleid] > fVehicleMax) VehicleFuel[vehicleid] = fVehicleMax;

    new Float:fNewInCan = fFuelInCan - fPour;
    if(fNewInCan < 0.0) fNewInCan = 0.0;
    PlayerInfo[playerid][pBinhXang] = fNewInCan + FUELCAN_EMPTY_MARKER;

    new szGameText[64];
    format(szGameText, sizeof(szGameText), "~w~Xe: ~g~%.1f ~w~/ %.1f L  ~w~Binh: ~y~%.1f L", VehicleFuel[vehicleid], fVehicleMax, fNewInCan);
    SendMessageTD(playerid, 800, szGameText);

    if(fNewInCan <= 0.0 || VehicleFuel[vehicleid] >= fVehicleMax) {
        new szMsg[80];
        if(VehicleFuel[vehicleid] >= fVehicleMax)
            format(szMsg, sizeof(szMsg), "Xe da day xang (%.1f L).", fVehicleMax);
        else
            format(szMsg, sizeof(szMsg), "Binh xang da het. Hay den tram xang de nap them.");
        SendClientMessageEx(playerid, COLOR_GREEN, szMsg);
        KillTimer(GetPVarInt(playerid, "FuelCanPourTimerID"));
        DeletePVar(playerid, "FuelCanPourTimerID");
        DeletePVar(playerid, "PouringFuelCan");
        DeletePVar(playerid, "PourStartX");
        DeletePVar(playerid, "PourStartY");
        DeletePVar(playerid, "PourStartZ");
    }

    return 1;
}

// Do xang tu binh vao xe gan nhat (goi tu inventory)
stock UseFuelCan(playerid)
{
    if(GetPVarInt(playerid, "PouringFuelCan") == 1) {
        SetPVarInt(playerid, "PouringFuelCan", 0);
        KillTimer(GetPVarInt(playerid, "FuelCanPourTimerID"));
        DeletePVar(playerid, "FuelCanPourTimerID");
        DeletePVar(playerid, "PourStartX");
        DeletePVar(playerid, "PourStartY");
        DeletePVar(playerid, "PourStartZ");
        new Float:fLeft = PlayerInfo[playerid][pBinhXang] - FUELCAN_EMPTY_MARKER;
        if(fLeft < 0.0) fLeft = 0.0;
        new string[64];
        format(string, sizeof(string), "Da dung do xang. Binh xang con lai: %.1f L.", fLeft);
        SendClientMessageEx(playerid, COLOR_GREEN, string);
        return;
    }

    if(PlayerInfo[playerid][pBinhXang] <= 0.0) {
        SendClientMessageEx(playerid, COLOR_RED, "Ban khong co binh xang.");
        return;
    }

    new Float:fFuelInCan = PlayerInfo[playerid][pBinhXang] - FUELCAN_EMPTY_MARKER;
    if(fFuelInCan < 0.0) fFuelInCan = 0.0;

    if(fFuelInCan <= 0.0) {
        SendClientMessageEx(playerid, COLOR_RED, "Binh xang cua ban dang rong. Hay den tram xang dung /muaxang de nap xang.");
        return;
    }

    new Float:px, Float:py, Float:pz;
    GetPlayerPos(playerid, px, py, pz);

    new closestVeh = -1;
    new Float:minDist = 3.0;
    for(new v = 1; v < MAX_VEHICLES; v++) {
        if(!GetVehicleModel(v)) continue;
        if(!IsRefuelableVehicle(v)) continue;
        if(IsVIPcar(v) || IsFamedVeh(v) || IsAdminSpawnedVehicle(v)) continue;
        new Float:d = GetVehicleDistanceFromPoint(v, px, py, pz);
        if(d < minDist) {
            minDist = d;
            closestVeh = v;
        }
    }

    if(closestVeh == -1) {
        SendClientMessageEx(playerid, COLOR_RED, "Khong co xe nao gan day (trong vong 3m).");
        return;
    }

    if(VehicleFuel[closestVeh] >= GetVehicleFuelCapacity(closestVeh)) {
        SendClientMessageEx(playerid, COLOR_RED, "Xe nay da day xang.");
        return;
    }

    SetPVarFloat(playerid, "PourStartX", px);
    SetPVarFloat(playerid, "PourStartY", py);
    SetPVarFloat(playerid, "PourStartZ", pz);
    SetPVarInt(playerid, "PouringFuelCan", 1);
    new iTimer = SetTimerEx("FuelCanPourTimer", 500, true, "ii", playerid, closestVeh);
    SetPVarInt(playerid, "FuelCanPourTimerID", iTimer);

    SendClientMessageEx(playerid, COLOR_WHITE, "Dang do xang vao xe, vui long cho...");
    SendClientMessageEx(playerid, COLOR_YELLOW, "Su dung binh xang lai lan nua de ngung viec do xang.");

    new proxstring[128];
    format(proxstring, sizeof(proxstring), "{FF8000}* {C2A2DA}%s do xang tu binh vao xe.", GetPlayerNameEx(playerid));
    ProxDetector(30.0, playerid, proxstring, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
}

// Vut binh xang xuong dat (goi tu inventory drop)
stock DropFuelCan(playerid)
{
    if(PlayerInfo[playerid][pBinhXang] <= 0.0) {
        SendClientMessageEx(playerid, COLOR_RED, "Ban khong co binh xang.");
        return;
    }

    new Float:fFuelInCan = PlayerInfo[playerid][pBinhXang] - FUELCAN_EMPTY_MARKER;
    if(fFuelInCan < 0.0) fFuelInCan = 0.0;

    new Float:px, Float:py, Float:pz;
    GetPlayerPos(playerid, px, py, pz);

    new szLabel[96];
    if(fFuelInCan <= 0.0)
        format(szLabel, sizeof(szLabel), "Binh xang\n(Rong)\nNhan {ffff44}[Y]{ffffff} de nhat");
    else
        format(szLabel, sizeof(szLabel), "Binh xang\n(%.1f L)\nNhan {ffff44}[Y]{ffffff} de nhat", fFuelInCan);

    // Store raw float bits as int in dropQuantity for later pickup
    new iRawBits = (_:PlayerInfo[playerid][pBinhXang]);

    // Build entry manually to use model 1650 (jerry can)
    if(DroppedItemCount >= MAX_DROPPED_ITEMS) {
        SendClientMessageEx(playerid, COLOR_RED, "Khong the vut them vat pham luc nay.");
        return;
    }

    new idx = DroppedItemCount;
    DroppedItems[idx][dropX] = px;
    DroppedItems[idx][dropY] = py;
    DroppedItems[idx][dropZ] = pz;
    DroppedItems[idx][dropVariable] = (_:pBinhXang);
    format(DroppedItems[idx][dropItemName], MAX_ITEM_NAME, "Binh xang");
    DroppedItems[idx][dropQuantity] = iRawBits;
    DroppedItems[idx][isValid] = true;
    DroppedItems[idx][isWeapon] = false;
    DroppedItems[idx][weaponType] = 0;
    DroppedItems[idx][labelID] = Create3DTextLabel(szLabel, 0xFFFFFFFF, px, py, pz - 0.5, 10.0, 0, 1);
    DroppedItems[idx][dropObject] = CreateDynamicObject(1650, px, py, pz - 1.0, 0.0, 0.0, 0.0, -1, -1, -1, 100.0);
    DroppedItemCount++;

    PlayerInfo[playerid][pBinhXang] = 0.0;

    new string[128];
    format(string, sizeof(string), "{FF8000}* {C2A2DA}%s vut binh xang (%.1f L) xuong dat.", GetPlayerNameEx(playerid), fFuelInCan);
    ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
    ApplyAnimation(playerid, "CARRY", "putdwn", 4.2, 0, 0, 0, 0, 999, 1, 0);
}

// Huy timer khi nguoi choi disconnect
hook OnPlayerDisconnect(playerid, reason)
{
    if(GetPVarInt(playerid, "FillingFuelCan") == 1) {
        KillTimer(GetPVarInt(playerid, "FuelCanTimer"));
        DeletePVar(playerid, "FuelCanTimer");
        DeletePVar(playerid, "FillingFuelCan");
        DeletePVar(playerid, "FuelCanStartX");
        DeletePVar(playerid, "FuelCanStartY");
        DeletePVar(playerid, "FuelCanStartZ");
    }
    if(GetPVarInt(playerid, "PouringFuelCan") == 1) {
        KillTimer(GetPVarInt(playerid, "FuelCanPourTimerID"));
        DeletePVar(playerid, "FuelCanPourTimerID");
        DeletePVar(playerid, "PouringFuelCan");
        DeletePVar(playerid, "PourStartX");
        DeletePVar(playerid, "PourStartY");
        DeletePVar(playerid, "PourStartZ");
    }
    return 1;
}
