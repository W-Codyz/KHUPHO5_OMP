PreloadAnimations(playerid)
{
	for (new i = 0; i < sizeof(PreloadAnimLibs); i ++) 
    {
	    ApplyAnimation(playerid, PreloadAnimLibs[i], "null", 4.0, 0, 0, 0, 0, 0, 1);
	}
	return 1;
}

OnPlayAnim(playerid,animlib[],animname[], Float:Speed, looping, lockx, locky, lockz, lp)
{
	if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || GetPVarInt(playerid, "Injured") != 0 || PlayerCuffed[playerid] != 0)
	{
		return 0;
	}

	HienTextdraw(playerid, "~r~[!]~w~NHAN ~y~~k~~PED_SPRINT~~w~ HOAC ~y~/SA~w~ DE DUNG HANH DONG");
	SendHintMessage(playerid, "Nhan {F0D961}F{ffffff} hoac {F0D961}/SA{ffffff} de dung hanh dong");
	OnAnim{playerid} = true;
	ApplyAnimation(playerid, animlib, animname, Speed, looping, lockx, locky, lockz, lp, 1);
	
	return 1;
}

StopPlayerAnims(playerid)
{
	if (GetPVarInt(playerid, "Injured") != 0 || PlayerCuffed[playerid] != 0) return 0;
	//GetPlayerState(playerid) != PLAYER_STATE_ONFOOT

	OnAnim{playerid} = false;
	ClearAnimations(playerid, 1);
	ApplyAnimation(playerid, "CARRY", "crry_prtial", 4.0, 0, 0, 0, 0, 0, 1);
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
	return 1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(GetPVarInt(playerid, "Injured") != 0) return;

	if(IsKeyJustDown(KEY_SPRINT,newkeys,oldkeys))
		if(!(GetPVarType(playerid, "PlayerCuffed") || GetPVarType(playerid, "Injured") || GetPVarType(playerid, "IsFrozen") || PlayerInfo[playerid][pHospital]))
	    	StopPlayerAnims(playerid);

	if(IsKeyJustDown(KEY_ACTION, newkeys, oldkeys))
	{
		if(OnAnim{playerid}) StopPlayerAnims(playerid);
	}
}