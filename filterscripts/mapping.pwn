#include <open.mp>
#include <streamer>

#pragma warning disable 213, 240
//	Project create by Codyz
//	
//	Copyright by Cody

//____________________________________________________________________________//

#undef MAX_PLAYERS
#define MAX_PLAYERS (500)


public OnFilterScriptExit()
{
    for(new i; i < MAX_PLAYERS; i++)
	{
	    if(GetPlayerVirtualWorld(i) != 0 || GetPlayerInterior(i) != 0) TogglePlayerControllable(i, false);
	}
}

public OnFilterScriptInit()
{

	new tmpobjid;

	//
	new map_world = -1;
	new map_int = -1;

	//HQ for new_bie
	tmpobjid = CreateDynamicObject(3749, -1783.340576, -1110.611816, 59.857261, 0.000000, 0.000000, -248.500000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1787.271240, -1094.942626, 57.174144, 0.000000, -0.999999, -69.499992, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1796.454711, -1090.432250, 57.017562, 0.000000, 0.000000, 17.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1809.764282, -1094.590454, 57.029270, 0.000000, 0.000000, 18.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1814.142944, -1095.974365, 57.040046, 0.000000, 0.000000, 197.500000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1819.585449, -1104.953613, 63.338794, 0.000000, 0.000000, 100.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1817.178100, -1118.697143, 57.068855, 0.000000, 0.000000, 100.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1814.747802, -1132.474487, 57.069885, 0.000000, 0.000000, 100.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1811.739135, -1146.135742, 57.086898, 0.000000, 0.000000, 105.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1805.887207, -1158.645141, 57.033615, 0.000000, 0.000000, 125.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1798.375732, -1170.423706, 57.039699, 0.000000, 0.000000, 120.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1775.232299, -1125.322998, 57.758579, 0.000000, 0.000000, 113.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1769.494995, -1138.145141, 57.741226, 0.000000, 0.000000, 115.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1763.587402, -1150.821899, 57.731746, 0.000000, 0.000000, 115.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1757.713623, -1163.144897, 57.608291, 0.000000, 0.000000, 834.500000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1791.418212, -1182.522583, 57.095951, 0.000000, 0.000000, 120.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1781.949340, -1184.929687, 57.112403, 0.000000, 0.000000, 31.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(827, -1787.934570, -1187.224975, 54.063907, 0.000000, 0.000000, 0.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1769.919189, -1177.823608, 57.085338, 0.000000, 0.000000, 30.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1761.059814, -1172.893432, 57.043312, 0.000000, 0.000000, 28.500000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1787.301269, -1094.822509, 63.533885, 0.000000, -0.999999, -69.499992, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1796.454711, -1090.432250, 63.317558, 0.000000, 0.000000, 17.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1809.764282, -1094.590454, 63.329269, 0.000000, 0.000000, 18.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1814.142944, -1095.974365, 63.340045, 0.000000, 0.000000, 197.500000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1819.585449, -1104.953613, 57.038795, 0.000000, 0.000000, 100.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1817.178100, -1118.697143, 63.368854, 0.000000, 0.000000, 100.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1814.747802, -1132.474487, 63.369884, 0.000000, 0.000000, 100.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1811.739135, -1146.135742, 63.386898, 0.000000, 0.000000, 105.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1805.887207, -1158.645141, 63.333614, 0.000000, 0.000000, 125.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1798.375732, -1170.423706, 63.339698, 0.000000, 0.000000, 120.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1791.418212, -1182.522583, 63.395950, 0.000000, 0.000000, 120.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1781.949340, -1184.929687, 63.412403, 0.000000, 0.000000, 31.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1769.919189, -1177.823608, 63.385341, 0.000000, 0.000000, 30.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1761.059814, -1172.893432, 63.343311, 0.000000, 0.000000, 28.500000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1757.713623, -1163.144897, 63.908290, 0.000000, 0.000000, 834.500000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1763.587402, -1150.821899, 64.031745, 0.000000, 0.000000, 115.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1769.494995, -1138.145141, 64.041229, 0.000000, 0.000000, 115.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19313, -1775.232299, -1125.322998, 64.058578, 0.000000, 0.000000, 113.000000, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(3605, -1787.665039, -1154.410644, 61.934661, 1.984780, -1.585329, 121.256904, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19451, -1783.364379, -1174.916015, 54.108661, 0.000000, 0.000000, -58.899909, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19451, -1785.464355, -1176.306030, 54.108661, 0.000000, 0.000000, -58.899909, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19451, -1791.964355, -1174.705932, 54.213657, -1.500159, 0.000000, -148.899902, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19451, -1795.147460, -1169.405761, 54.438648, -1.500159, 0.000000, -148.899902, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19451, -1798.285522, -1165.276733, 54.678638, 1.022118, -1.545458, 30.617824, -1, -1, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(19451, -1791.709106, -1166.988403, 54.513645, -1.912004, -1.524264, 119.562751, -1, -1, -1, 300.00, 300.00); 

	
	for(new i, Float: fPlayerPos[3]; i < MAX_PLAYERS; i++)
	{
	    if(GetPlayerVirtualWorld(i) != 0 || GetPlayerInterior(i) != 0)
		{
			GetPlayerPos(i, fPlayerPos[0], fPlayerPos[1], fPlayerPos[2]);
			Streamer_UpdateEx(i, fPlayerPos[0], fPlayerPos[1], fPlayerPos[2]);
			CallRemoteFunction("Player_StreamPrep", "ifffi", i, fPlayerPos[0], fPlayerPos[1], fPlayerPos[2], 2500);
		}
	}
 	// Head for static objects - streamed limits are completely independent (cause of old crashing)
	Streamer_VisibleItems(STREAMER_TYPE_OBJECT, 965);
}

public OnPlayerEditDynamicObject(playerid, objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz) // Calling Fix by Winterfield - Do not remove
{
	CallRemoteFunction("OnPlayerEditGate", "iddffffff", playerid, objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz);
}

public OnPlayerConnect(playerid)
{
	//RemoveBuildingForPlayer
	
	
	return 1;
}
