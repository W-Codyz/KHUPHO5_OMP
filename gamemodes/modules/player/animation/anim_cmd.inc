alias:stopanim("sa")
CMD:stopanim(playerid, params[])
{
	if (OnAnim{playerid}) StopPlayerAnims(playerid);
	return 1;
}

alias:animlist("hanhdong")
CMD:animlist(playerid, params[])
{
	SendClientMessage(playerid,COLOR_GREEN,"____________Danh sach hanh dong____________");
	SendClientMessage(playerid,COLOR_WHITE,"/handsup /piss /sit /dance /aim /smoking /drunk /crack /deal /cellin /cellout");
	SendClientMessage(playerid,COLOR_WHITE,"/getarrested /laugh /crossarms /vomit /eating /sipdrink /chat /taichi /injured");
	SendClientMessage(playerid,COLOR_WHITE,"/slapass /fucku /no /yes /crawl /push /superkick /facepalm /balls /lowbodypush");
	SendClientMessage(playerid,COLOR_WHITE,"/spraying /medic /koface /kostomach /lifejump /rollfall /hoodfrisked /flip /box");
	SendClientMessage(playerid,COLOR_WHITE,"/chant /finger /gpunch /gkick /lowthrow /riot /bp /cry /taxir /taxil /gro /seat");
	SendClientMessage(playerid,COLOR_WHITE,"/tatto /bar /stoptaxi /joy /strip /basket /lean /lay /gsign /wave /police /win");
	SendClientMessage(playerid,COLOR_WHITE,"/waiml /waimr /lookback /caim /creep /sroll /gesture /lookout /agreement /bend");
	SendClientMessage(playerid,COLOR_WHITE,"/cmouth /nfall /hsit /rod /swingback /csit /bracing /pool /search /daim /gog");
	SendClientMessage(playerid,COLOR_WHITE,"/rhandl /rhandr /hstand /nlean /shelter /wsit /waim /hangon /blowj /rseat");
	SendClientMessage(playerid,COLOR_WHITE,"/polwalk /wallow /nflip /csdead /follow /grnd /copa /cmon /lose /gunthreat");
	SendClientMessage(playerid,COLOR_WHITE,"/givecash /robr /serend /heist /beckon /shotgun /colt /climb /pushup /protection");
	SendClientMessage(playerid,COLOR_WHITE,"/tlean /swallow /stand /pullhand /nlook /extend /carchat /repcar /onehand");
	SendClientMessage(playerid,COLOR_WHITE,"/backwalk /mirro /hips /pullo /rtake /obalance /wobalance /taction /catch");
	SendClientMessage(playerid,COLOR_WHITE,"/despair /rooting /spyglass /looksky /show /sh /crossfingers /delight /hdelight");
	SendClientMessage(playerid,COLOR_WHITE,"/showtable /tfacepalm /tpull /fail /wear /fsit /crushjump /scatch /lookfloor");
	SendClientMessage(playerid,COLOR_WHITE,"/tramplefloor /swing /nbp /takehead /readtable /squat /sprawl /throwmoney /faim");
	SendClientMessage(playerid,COLOR_WHITE,"/liftup /putdown /stretch /idle /areload /batidle /tapcig /fall /inbedleft");
	SendClientMessage(playerid,COLOR_WHITE,"/inbedright /cover /dealerstance /stance /yell /rap /tired /gsign /hands /tgro");
	SendClientMessage(playerid,COLOR_WHITE,"/kicking /bomb /dunk /frontfall /sleepl /sleepr /bat /yell /salute /sneak /rob");
	SendClientMessage(playerid,COLOR_WHITE,"/hide /drinkwater /checktime /sleep /blob /wavedown /cpr /showoff /throw /hurt");
	SendClientMessage(playerid,COLOR_WHITE,"/washhands /crabs /jerkoff /lowbodypunch /fightidle /signal /nobreath /fallover");
	SendClientMessage(playerid,COLOR_WHITE,"/pedmove /getjiggy /stripclub /smoke /dj /reload /crouchreload /tag /cheer");
	SendClientMessage(playerid,COLOR_WHITE,"/siteat /spank /sexy /holdup /misc /snatch /blowjob /kiss /idles /sunbathe");
	SendClientMessage(playerid,COLOR_WHITE,"/lowrider /hitchhike /sitonchair /cashier /nope /fuku /phonetalk /write");
	SendClientMessage(playerid,COLOR_WHITE,"/headbutt /airkick /doorkick /leftslap /elbow /coprun /opendoor /robman /camera");
	SendClientMessage(playerid,COLOR_WHITE,"/carry /cslot /croulette /ccards /pose /eat /swata /argue /presenta /basketball");
	SendClientMessage(playerid,COLOR_WHITE,"/dive /getup /robbed /angry /bodypush /lowbodypush /rap /stance /police");
	SendClientMessage(playerid,COLOR_GREEN,"_____________________________________________");

	return 1;
}

CMD:strip(playerid, params[])
{
	new type;

	if (sscanf(params,"d", type)) return SendHelpMessage(playerid, "/strip [1-14]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"STRIP", "strip_A", 4.1, 1, 1, 1, 1, 1 );
		case 2: OnPlayAnim(playerid,"STRIP", "strip_B", 4.1, 1, 1, 1, 1, 1 );
		case 3: OnPlayAnim(playerid,"STRIP", "strip_C", 4.1, 1, 1, 1, 1, 1 );
		case 4: OnPlayAnim(playerid,"STRIP", "strip_D", 4.1, 1, 1, 1, 1, 1 );
		case 5: OnPlayAnim(playerid,"STRIP", "strip_E", 4.1, 1, 1, 1, 1, 1 );
		case 6: OnPlayAnim(playerid,"STRIP", "strip_F", 4.1, 1, 1, 1, 1, 1 );
		case 7: OnPlayAnim(playerid,"STRIP", "strip_G", 4.1, 1, 1, 1, 1, 1 );
		case 8: OnPlayAnim(playerid,"STRIP","STR_A2B",4.1,1,1,1,1,1);
		case 9: OnPlayAnim(playerid,"STRIP","STR_B2C",4.1,1,1,1,1,1);
		case 10: OnPlayAnim(playerid,"STRIP","STR_C1",4.1,1,1,1,1,1);
		case 11: OnPlayAnim(playerid,"STRIP","STR_C2",4.1,1,1,1,1,1);
		case 12: OnPlayAnim(playerid,"STRIP","STR_Loop_A",4.1,1,1,1,1,1);
		case 13: OnPlayAnim(playerid,"STRIP","STR_Loop_B",4.1,1,1,1,1,1);
		case 14: OnPlayAnim(playerid,"STRIP","STR_Loop_C",4.1,1,1,1,1,1);
		default: SendHelpMessage(playerid, "/strip [1-14]");
	}
	return 1;
}

CMD:liftup(playerid, params[])
{
	return OnPlayAnim(playerid, "CARRY", "liftup", 3.0, 0, 0, 0, 0, 0);
}

CMD:putdown(playerid, params[])
{
	return OnPlayAnim(playerid, "CARRY", "putdwn", 3.0, 0, 0, 0, 0, 0);
}

CMD:stretch(playerid, params[])
{
	return OnPlayAnim(playerid,"STRIP","PUN_HOLLER",4.1,1,0,0,1,0);
}

CMD:idle(playerid, params[])
{
	return OnPlayAnim(playerid,"STRIP","PUN_LOOP",4.1,1,0,0,1,0);
}

CMD:batidle(playerid, params[])
{
	return OnPlayAnim(playerid,"BASEBALL","Bat_IDLE",4.1,1,0,0,1,0);
}

CMD:aim(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/aim [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid, "SHOP", "ROB_Loop_Threat", 4.0, 1, 0, 0, 0, 0);
		case 2: OnPlayAnim(playerid,"PED","gang_gunstand",3.0,0,1,1,1,0);
		default: SendHelpMessage(playerid, "/aim [1-2]");
	}
	return 1;
}

CMD:lay(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/lay [1-5]");

	switch(type)
	{
		case 1:	OnPlayAnim(playerid,"BEACH", "bather", 4.0, 1, 0, 0, 0, 0);
		case 2:	OnPlayAnim(playerid,"SUNBATHE","Lay_Bac_in",3.0,0,1,1,1,0);
		case 3:	OnPlayAnim(playerid,"BEACH","ParkSit_W_loop",3.0,0,1,1,1,0);
		case 4:	OnPlayAnim(playerid,"BEACH","SitnWait_loop_W",4.0, 1, 0, 0, 0, 0);
		case 5:	OnPlayAnim(playerid,"SUNBATHE","SBATHE_F_LieB2Sit",3.0,0,1,1,1,0);
		default: SendHelpMessage(playerid, "/lay [1-5]");
	}
	return 1;
}

CMD:crack(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/crack [1-5]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"CRACK","crckdeth2",4.0,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"CRACK","crckidle1",4.0,1,0,0,0,0);
		case 3: OnPlayAnim(playerid,"CRACK","crckidle2",4.0,1,0,0,0,0);
		case 4: OnPlayAnim(playerid,"CRACK","crckidle3",4.0,1,0,0,0,0);
		case 5: OnPlayAnim(playerid,"CRACK","crckidle4",4.0,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/crack [1-5]");
	}
	return 1;
}

CMD:areload(playerid, params[])
{
	return OnPlayAnim(playerid,"TEC","TEC_reload",4.0,0,0,0,0,0);
}

CMD:bat(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/bat [1-3]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"BASEBALL","Bat_IDLE",4.1,1,1,1,1,1);
		case 2: OnPlayAnim(playerid,"CRACK","Bbalbat_Idle_01",4.0,1,0,0,0,0);
		case 3: OnPlayAnim(playerid,"CRACK","Bbalbat_Idle_02",4.0,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/bat [1-3]");
	}
	return 1;
}

CMD:tapcig(playerid, params[])
{
	return OnPlayAnim(playerid,"SMOKING","M_smk_tap",3.0,0,0,0,0,0);
}

CMD:fall(playerid, params[])
{
	return OnPlayAnim(playerid,"PARACHUTE","FALL_skyDive_DIE",3.5,0,0,0,1,0);
}

CMD:laugh(playerid, params[])
{
	return OnPlayAnim(playerid, "RAPPING", "Laugh_01", 4.0, 0, 0, 0, 0, 0);
}

CMD:vomit(playerid, params[])
{
	return OnPlayAnim(playerid, "FOOD", "EAT_Vomit_P", 3.0, 0, 0, 0, 0, 0);
}

CMD:inbedleft(playerid, params[])
{
	return OnPlayAnim(playerid,"INT_HOUSE","BED_Loop_L",4.0,1,0,0,0,0);
}

CMD:inbedright(playerid, params[])
{
	return OnPlayAnim(playerid,"INT_HOUSE","BED_Loop_R",4.0,1,0,0,0,0);
}

CMD:getarrested(playerid, params[])
{
	return OnPlayAnim(playerid,"PED","IDLE_taxi",4.0,0,0,0,0,0);
}

CMD:cry(playerid, params[])
{
	return OnPlayAnim(playerid,"GRAVEYARD","mrnf_loop",4.0,1,0,0,0,0);
}

CMD:sleepl(playerid)
{
	return OnPlayAnim(playerid,"INT_HOUSE","BED_In_L",4.0,0,0,1,1,0);
}

CMD:sleepr(playerid)
{
	return OnPlayAnim(playerid,"INT_HOUSE","BED_In_R",4.0,0,0,1,1,0);
}

CMD:cover(playerid, params[])
{
	return OnPlayAnim(playerid, "ped", "cower", 3.0, 1, 0, 0, 0, 0);
}

CMD:dealerstance(playerid, params[])
{
	return OnPlayAnim(playerid,"DEALER","DEALER_IDLE",4.0,1,0,0,0,0);
}

CMD:stance(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/stance [1-11]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"PED","WOMAN_IDLESTANCE",4.0,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"PED","CAR_HOOKERTALK",4.0,1,0,0,0,0);
		case 3: OnPlayAnim(playerid,"FAT","FatIdle",4.0,1,0,0,0,0);
		case 4: OnPlayAnim(playerid,"WUZI","Wuzi_Stand_Loop",4.0,1,0,0,0,0);
		case 5: OnPlayAnim(playerid,"GRAVEYARD","mrnm_loop",4.0,1,0,0,0,0);
		case 6: OnPlayAnim(playerid,"GRAVEYARD","prst_loopa",4.0,1,0,0,0,0);
		case 7: OnPlayAnim(playerid,"PED","idlestance_fat",4.0,1,0,0,0,0);
		case 8: OnPlayAnim(playerid,"PED","idlestance_old",4.0,1,0,0,0,0);
		case 9: OnPlayAnim(playerid,"PED","turn_l",4.0,1,0,0,0,0);
		case 10: OnPlayAnim(playerid,"BAR","Barcustom_loop",4.0,1,0,0,0,0);
		case 11: OnPlayAnim(playerid,"BAR","Barserve_loop",4.0,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/stance [1-11]");
	}
	return 1;
}

CMD:yell(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/yell [1-3]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"ON_LOOKERS","shout_loop",3.0,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"ON_LOOKERS","shout_01",3.0,1,0,0,0,0);
		case 3: OnPlayAnim(playerid,"ON_LOOKERS","shout_02",3.0,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/yell [1-3]");
	}
	return 1;
}

CMD:gsign(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type))	return SendHelpMessage(playerid, "/gsign [1-10]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"GHANDS","gsign1",4.1,0,1,1,1,1);
		case 2: OnPlayAnim(playerid,"GHANDS","gsign1LH",4.1,0,1,1,1,1);
		case 3: OnPlayAnim(playerid,"GHANDS","gsign2",4.1,0,1,1,1,1);
		case 4: OnPlayAnim(playerid,"GHANDS","gsign2LH",4.1,0,1,1,1,1);
		case 5: OnPlayAnim(playerid,"GHANDS","gsign3",4.1,0,1,1,1,1);
		case 6: OnPlayAnim(playerid,"GHANDS","gsign3LH",4.1,0,1,1,1,1);
		case 7: OnPlayAnim(playerid,"GHANDS","gsign4",4.1,0,1,1,1,1);
		case 8: OnPlayAnim(playerid,"GHANDS","gsign4LH",4.1,0,1,1,1,1);
		case 9: OnPlayAnim(playerid,"GHANDS","gsign5",4.1,0,1,1,1,1);
		case 10: OnPlayAnim(playerid,"GHANDS","gsign5LH",4.1,0,1,1,1,1);
		default: SendHelpMessage(playerid, "/gsign [1-10]");
	}
	return 1;
}

CMD:rap(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/rap [1-9]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"RAPPING","RAP_A_Loop",4.0,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"RAPPING","RAP_C_Loop",4.0,1,0,0,0,0);
		case 3: OnPlayAnim(playerid,"GANGS","prtial_gngtlkD",4.0,1,0,0,1,1);
		case 4: OnPlayAnim(playerid,"GANGS","prtial_gngtlkH",4.0,1,0,0,1,1);
		case 5: OnPlayAnim(playerid,"GANGS","prtial_gngtlkF",4.0,1,0,0,1,1);
		case 6: OnPlayAnim(playerid,"GANGS","prtial_gngtlkG",4.0,1,0,0,1,1);
		case 7: OnPlayAnim(playerid,"PED","ATM",4.0,0,0,0,0,0);
		case 8: OnPlayAnim(playerid,"RAPPING","RAP_B_IN",3.0,0,1,1,1,0);
		case 9: OnPlayAnim(playerid,"GYMNASIUM","gym_tread_celebrate",4.1,0,1,1,1,0);
		default: SendHelpMessage(playerid, "/rap [1-9]");
	}
	return 1;
}

CMD:tired(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/tired [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"PED","IDLE_tired",3.0,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"FAT","IDLE_tired",4.0,0,0,1,1,0);
		default: SendHelpMessage(playerid, "/tired [1-2]");
	}
	return 1;
}

CMD:sit(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/sit [1-7]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"PED","SEAT_down",4.0,0,0,1,1,0);
		case 2: OnPlayAnim(playerid,"MISC","seat_lr",2.0,1,0,0,0,0);
		case 3: OnPlayAnim(playerid,"MISC","seat_talk_01",2.0,1,0,0,0,0);
		case 4: OnPlayAnim(playerid,"MISC","seat_talk_02",2.0,1,0,0,0,0);
		case 5: OnPlayAnim(playerid,"INT_OFFICE","OFF_Sit_Crash",4.0,1,0,1,1,0);
		case 6: OnPlayAnim(playerid,"INT_OFFICE","OFF_Sit_Drink",4.0,1,0,1,1,0);
		case 7: OnPlayAnim(playerid,"INT_OFFICE","OFF_Sit_Watch",4.0,0,0,1,1,0);
		default: SendHelpMessage(playerid, "/sit [1-7]");
	}
	return 1;
}

CMD:dance(playerid, params[])
{
	new type;

	if (GetPVarInt(playerid, "Injured") != 0 || IsPlayerInAnyVehicle(playerid)) return 1;
	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/dance [1-12]");

	switch(type)
	{
		case 1:	SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE1);
		case 2: SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE2);
		case 3: SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE3);
		case 4: SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE4);
		case 5: OnPlayAnim(playerid,"DANCING","bd_clap",4.1,1,1,1,1,1);
		case 6: OnPlayAnim(playerid,"DANCING","bd_clap1",4.1,1,1,1,1,1);
		case 7: OnPlayAnim(playerid,"DANCING","DAN_Down_A",4.1,1,1,1,1,1);
		case 8: OnPlayAnim(playerid,"DANCING","DAN_Left_A",4.1,1,1,1,1,1);
		case 9: OnPlayAnim(playerid,"DANCING","DAN_Loop_A",4.1,1,1,1,1,1);
		case 10: OnPlayAnim(playerid,"DANCING","DAN_Right_A",4.1,1,1,1,1,1);
		case 11: OnPlayAnim(playerid,"DANCING","DAN_Up_A",4.1,1,1,1,1,1);
		case 12: OnPlayAnim(playerid,"DANCING","dnce_M_a",4.1,1,1,1,1,1);
		default: SendHelpMessage(playerid, "/dance [1-12]");
	}
	return 1;
}

CMD:injured(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/injured [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid, "SWEET", "Sweet_injuredloop", 4.0,0,0,1,1,0);
		case 2: OnPlayAnim(playerid, "WUZI", "CS_Dead_Guy", 4.0, 1, 1, 1, 1, 0);
		default: SendHelpMessage(playerid, "/injured [1-2]");
	}
	return 1;
}

CMD:handsup(playerid, params[])
{
	if (PlayerCuffed[playerid] != 0 || GetPVarInt(playerid, "Injured") != 0 || IsPlayerInAnyVehicle(playerid)) return 1;
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_HANDSUP);
	return 1;
}

CMD:deal(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/deal [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid, "DEALER", "DEALER_DEAL", 4.0, 0, 0, 0, 0, 0);
		case 2: OnPlayAnim(playerid, "DEALER", "DEALER_IDLE", 3.0,0,1,1,1,0);
		default: SendHelpMessage(playerid, "/deal [1-2]");
	}
	return 1;
}

CMD:piss(playerid, params[])
{
	if (GetPVarInt(playerid, "Injured") != 0 || IsPlayerInAnyVehicle(playerid)) return 1;
	SetPlayerSpecialAction(playerid, 68);
	return 1;
}

CMD:riot(playerid, params[])
{
	return OnPlayAnim(playerid,"RIOT","RIOT_ANGRY",4.0,0,0,0,0,0);
}

CMD:slapass(playerid, params[])
{
	return OnPlayAnim(playerid, "SWEET", "sweet_ass_slap", 4.0, 0, 0, 0, 0, 0);
}

CMD:wave(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/wave [1-4]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid, "KISSING","gfwave2", 4.1,0,1,1,0,0);
		case 2: OnPlayAnim(playerid, "ON_LOOKERS","wave_loop", 4.1,1,1,1,0,0);
		case 3: OnPlayAnim(playerid,"CAMERA","camstnd_cmon",4.0,0,0,0,0,0);
		case 4: OnPlayAnim(playerid,"PED","endchat_01",4.1,0,1,1,0,0);
		default: SendHelpMessage(playerid, "/wave [1-4]");
	}
	return 1;
}

CMD:crossarms(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/crossarms [1-4]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"COP_AMBIENT","Coplook_loop",4.0,0,1,1,1,-1);
		case 2: OnPlayAnim(playerid,"DEALER","DEALER_IDLE",4.0,1,0,0,0,0);
		case 3: OnPlayAnim(playerid,"GRAVEYARD","mrnM_loop",4.0,1,0,0,0,0);
		case 4: OnPlayAnim(playerid,"GRAVEYARD","prst_loopa",4.0,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/crossarms [1-4]");
	}
	return 1;
}

CMD:lean(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/lean [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"MISC","Plyrlean_loop",4.0,1,1,1,1,1);
		case 2: OnPlayAnim(playerid,"GANGS","leanIDLE",4.0,0,1,1,1,0);
		default: SendHelpMessage(playerid, "/lean [1-2]");
	}
	return 1;
}

CMD:basket(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/basket [1-7]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"BSKTBALL","BBALL_idleloop",4.0,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"BSKTBALL","BBALL_Jump_Shot",4.0,0,0,0,0,0);
		case 3: OnPlayAnim(playerid,"BSKTBALL","BBALL_pickup",4.0,0,0,0,0,0);
		case 4: OnPlayAnim(playerid,"BSKTBALL","BBALL_run",4.1,1,1,1,1,1);
		case 5: OnPlayAnim(playerid,"BSKTBALL","BBALL_def_loop",4.0,1,0,0,0,0);
		case 6: OnPlayAnim(playerid,"BSKTBALL","BBALL_Dnk",4.0,0,0,0,0,0);
		case 7: OnPlayAnim(playerid,"BSKTBALL","BBALL_walk",4.1,1,1,1,1,1);
		default: SendHelpMessage(playerid, "/basket [1-6]");
	}
	return 1;
}

CMD:lookout(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/lookout [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"SWAT","swt_wllpk_L",4.0,0,0,1,1,0);
		case 2: OnPlayAnim(playerid,"SWAT","swt_wllpk_R",4.0,0,0,1,1,0);
		default: SendHelpMessage(playerid, "/lookout [1-2]");
	}
	return 1;
}

CMD:dunk(playerid, params[])
{
	return OnPlayAnim(playerid,"BSKTBALL","BBALL_Dnk",4.0,0,0,0,0,0);
}

CMD:win(playerid, params[])
{
	return OnPlayAnim(playerid,"OTB","wtchrace_win",4.1,0,1,1,1,0);
}

CMD:walk(playerid, params[])
{
	return 1;
}

CMD:walking(playerid, params[])
{
	return 1;
}

CMD:smoking(playerid,params[])
{
	new type; 

	if (sscanf(params,"d", type))	return SendHelpMessage(playerid, "/smoking [1-4]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid, "SMOKING", "M_smklean_loop", 4.0, 1, 0, 0, 0, 0);
		case 2: OnPlayAnim(playerid, "SMOKING", "F_smklean_loop", 4.0, 1, 0, 0, 0, 0);
		case 3: OnPlayAnim(playerid, "SMOKING","M_smkstnd_loop", 4.0, 1, 0, 0, 0, 0);
		case 4: OnPlayAnim(playerid, "SMOKING","M_smk_out", 4.0, 0, 0, 0, 0, 0);
		default: SendHelpMessage(playerid, "/smoking [1-4]");
	}
	return 1;
}

CMD:eating(playerid)
{
	return OnPlayAnim(playerid, "FOOD", "EAT_Burger", 4.1,1,1,1,1,1);
}

CMD:sipdrink(playerid)
{
	return OnPlayAnim(playerid, "GANGS", "drnkbr_prtl", 4.1,1,1,1,1,1);
}

CMD:chat(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/chat [1-7]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"PED","IDLE_CHAT",4.0,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"GANGS","prtial_gngtlkA",4.0,1,0,0,0,0);
		case 3: OnPlayAnim(playerid,"GANGS","prtial_gngtlkB",4.0,1,0,0,0,0);
		case 4: OnPlayAnim(playerid,"GANGS","prtial_gngtlkE",4.0,1,0,0,0,0);
		case 5: OnPlayAnim(playerid,"GANGS","prtial_gngtlkF",4.0,1,0,0,0,0);
		case 6: OnPlayAnim(playerid,"GANGS","prtial_gngtlkG",4.0,1,0,0,0,0);
		case 7: OnPlayAnim(playerid,"GANGS","prtial_gngtlkH",4.0,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/chat [1-7]");
	}
	return 1;
}

CMD:fucku(playerid)
{
	return OnPlayAnim(playerid,"PED","fucku",4.0,0,0,0,0,0);
}

CMD:taichi(playerid)
{
	return OnPlayAnim(playerid,"PARK","Tai_Chi_Loop",4.0,1,0,0,0,0);
}

CMD:no(playerid)
{
	return OnPlayAnim(playerid,"GANGS","Invite_No",4.0,0,0,0,0,0);
}

CMD:yes(playerid)
{
	return OnPlayAnim(playerid,"GANGS","Invite_Yes",4.0,0,0,0,0,0);
}

CMD:crawl(playerid)
{
	return OnPlayAnim(playerid,"PED","CAR_crawloutRHS",4.0,0,0,0,0,0);
}

CMD:push(playerid)
{
	return OnPlayAnim(playerid,"GANGS","shake_cara",4.0,0,0,0,0,0);
}

CMD:superkick(playerid)
{
	return OnPlayAnim(playerid,"POLICE","Door_Kick",4.0,0,0,0,0,0);
}

CMD:facepalm(playerid)
{
	return OnPlayAnim(playerid,"MISC","plyr_shkhead",4.0,0,0,0,0,0);
}

CMD:balls(playerid)
{
	return OnPlayAnim(playerid,"MISC","scratchballs_01",4.0,0,0,0,0,0);
}

CMD:lowbodypush(playerid)
{
	return OnPlayAnim(playerid,"GANGS","shake_carSH",4.0,0,0,0,0,0);
}

CMD:spraying(playerid)
{
	return OnPlayAnim(playerid,"SPRAYCAN","spraycan_full",4.0,0,0,0,0,0);
}

CMD:medic(playerid)
{
	return OnPlayAnim(playerid,"MEDIC","CPR",4.0,0,0,0,0,0);
}

CMD:koface(playerid)
{
	return OnPlayAnim(playerid,"PED","KO_shot_face",4.0,0,1,1,1,0);
}

CMD:kostomach(playerid)
{
	return OnPlayAnim(playerid,"PED","KO_shot_stom",4.0,0,1,1,1,0);
}

CMD:lifejump(playerid)
{
	return OnPlayAnim(playerid,"PED","EV_dive",4.0,0,1,1,1,0);
}

CMD:rollfall(playerid)
{
	return OnPlayAnim(playerid,"PED","BIKE_fallR",4.0,0,1,1,1,0);
}

alias:hoodfrisked("frisked")
CMD:hoodfrisked(playerid)
{
	return OnPlayAnim(playerid,"POLICE","crm_drgbst_01",1.0,0,1,1,1,0);
}

CMD:flip(playerid)
{
	return OnPlayAnim(playerid,"PED","KD_left",4.1,0,1,1,1,0);
}

CMD:frontfall(playerid)
{
	return OnPlayAnim(playerid,"PED","FLOOR_hit_f",4.1,0,1,1,1,0);
}

CMD:box(playerid)
{
	return OnPlayAnim(playerid,"GYMNASIUM","GYMshadowbox",4.0,1,1,1,1,0);
}

CMD:chant(playerid)
{
	return OnPlayAnim(playerid,"RIOT","RIOT_CHANT",4.0,1,1,1,1,0);
}

CMD:finger(playerid)
{
	return OnPlayAnim(playerid,"RIOT","RIOT_FUKU",2.0,0,0,0,0,0);
}

CMD:gpunch(playerid)
{
	return OnPlayAnim(playerid,"FIGHT_B","FightB_G",4.0,0,0,0,0,0);
}

CMD:gkick(playerid)
{
	return OnPlayAnim(playerid,"FIGHT_D","FightD_G",4.0,0,0,0,0,0);
}

CMD:lowthrow(playerid)
{
	return OnPlayAnim(playerid,"GRENADE","WEAPON_throwu",3.0,0,0,0,0,0);
}

CMD:bp(playerid)
{
	return OnPlayAnim(playerid,"benchpress","gym_bp_celebrate",4.0,0,0,0,0,0);
}

CMD:taxir(playerid)
{
	return OnPlayAnim(playerid,"MISC","Hiker_Pose",4.0,0,1,1,1,0);
}

CMD:taxil(playerid)
{
	return OnPlayAnim(playerid,"MISC","Hiker_Pose_L",4.0,0,1,1,1,0);
}

CMD:gro(playerid)
{
	return OnPlayAnim(playerid,"BEACH", "ParkSit_M_loop", 4.0, 1, 0, 0, 0, 0);
}

CMD:seat(playerid)
{
	return OnPlayAnim(playerid,"INT_HOUSE","LOU_In",4.0,0,0,1,1,0);
}

CMD:tatto(playerid)
{
	return OnPlayAnim(playerid,"TATTOOS", "TAT_ArmR_Pose_P", 4.1, 0, 1, 1, 1, 0 );
}
CMD:bar(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/bar [1-9]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"BAR","Barcustom_get",4.0,0,1,0,0,0);
		case 2: OnPlayAnim(playerid,"BAR","Barserve_bottle",4.0,0,0,0,0,0);
		case 3: OnPlayAnim(playerid,"BAR","Barserve_give",4.0,0,0,0,0,0);
		case 4: OnPlayAnim(playerid,"BAR","dnk_stndM_loop",4.0,0,0,0,0,0);
		case 5: OnPlayAnim(playerid,"BAR","BARman_idle",4.0,1,0,0,0,0);
		case 6: OnPlayAnim(playerid,"BAR","Barserve_loop",4.0,1,0,0,0,0);
		case 7: OnPlayAnim(playerid,"BAR","Barserve_order",4.0,0,0,0,0,0);
		case 8: OnPlayAnim(playerid,"BAR","Barcustom_order",4.0,0,0,0,0,0);
		case 9: OnPlayAnim(playerid,"BAR","dnk_stndF_loop",4.0,0,0,0,0,0);
		default: SendHelpMessage(playerid, "/bar [1-9]");
	}
	return 1;
}
CMD:stoptaxi(playerid)
{
	return OnPlayAnim(playerid,"PED","IDLE_taxi",4.0,0,0,0,0,0);
}
CMD:police(playerid,params[])
{
	new type;

	if (sscanf(params,"d", type))	return SendHelpMessage(playerid, "/police [1-4]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"POLICE","CopTraf_Away",3.0,1,0,0,1,0);
		case 2: OnPlayAnim(playerid,"POLICE","CopTraf_Come",3.0,1,0,0,1,0);
		case 3: OnPlayAnim(playerid,"POLICE","CopTraf_Left",3.0,1,0,0,1,0);
		case 4: OnPlayAnim(playerid,"POLICE","CopTraf_Stop",3.0,1,0,0,1,0);
		default: SendHelpMessage(playerid, "/police [1-4]");
	}
	return 1;
}

CMD:waiml(playerid)
{
	return OnPlayAnim(playerid,"PED","Driveby_L",4.1,0,1,1,1,1);
}

CMD:waimr(playerid)
{
	return OnPlayAnim(playerid,"PED","Driveby_R",4.1,0,1,1,1,1);
}

CMD:lookback(playerid)
{
	return OnPlayAnim(playerid,"PED","flee_lkaround_01",4.0,0,0,0,0,0);
}

CMD:caim(playerid)
{
	return OnPlayAnim(playerid,"PED","IDLE_ROCKET",4.0,0,1,1,1,0);
}

CMD:creep(playerid)
{
	return OnPlayAnim(playerid,"PED","Player_Sneak",4.1,1,1,1,1,1);
}

CMD:sroll(playerid)
{
	return OnPlayAnim(playerid,"SWAT","JMP_Wall1m_180",4.1,0,1,1,1,0);
}

CMD:gesture(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/gesture [1-16]");

	switch(type)
	{
		case 1:  OnPlayAnim(playerid,"GHANDS","gsign1",4.0,0,0,0,0,0);
		case 2:  OnPlayAnim(playerid,"GHANDS","gsign1LH",4.0,0,0,0,0,0);
		case 3:  OnPlayAnim(playerid,"GHANDS","gsign2",4.0,0,0,0,0,0);
		case 4:  OnPlayAnim(playerid,"GHANDS","gsign2LH",4.0,0,0,0,0,0);
		case 5:  OnPlayAnim(playerid,"GHANDS","gsign3",4.0,0,0,0,0,0);
		case 6:  OnPlayAnim(playerid,"GHANDS","gsign3LH",4.0,0,0,0,0,0);
		case 7:  OnPlayAnim(playerid,"GHANDS","gsign4",4.0,0,0,0,0,0);
		case 8:  OnPlayAnim(playerid,"GHANDS","gsign4LH",4.0,0,0,0,0,0);
		case 9:  OnPlayAnim(playerid,"GHANDS","gsign5",4.0,0,0,0,0,0);
		case 10: OnPlayAnim(playerid,"GHANDS","gsign5LH",4.0,0,0,0,0,0);
		case 11: OnPlayAnim(playerid,"GANGS","Invite_No",4.0,0,0,0,0,0);
		case 12: OnPlayAnim(playerid,"GANGS","Invite_Yes",4.0,0,0,0,0,0);
		case 13: OnPlayAnim(playerid,"GANGS","prtial_gngtlkD",4.0,0,0,0,0,0);
		case 14: OnPlayAnim(playerid,"GANGS","smkcig_prtl",4.0,0,0,0,0,0);
		case 15: OnPlayAnim(playerid,"PED","endchat_02",4.0,0,0,0,0,0);
		case 16: OnPlayAnim(playerid,"SWAT","swt_lkt",4.0,0,0,0,0,0);
		default: SendHelpMessage(playerid, "/gesture [1-16]");
	}
	return 1;
}

CMD:agreement(playerid)
{
	return OnPlayAnim(playerid,"COP_AMBIENT","Coplook_think",4.0,1,0,0,0,0);
}

CMD:bend(playerid)
{
	return OnPlayAnim(playerid,"BAR","Barserve_order",4.0,0,0,0,0,0);
}

CMD:rod(playerid)
{
	return OnPlayAnim(playerid,"benchpress","gym_bp_up_A",4.0,0,0,1,1,0);
}

CMD:tgro(playerid)
{
	return OnPlayAnim(playerid,"CAMERA","camcrch_cmon",4.0,0,0,1,1,0);
}

CMD:cmouth(playerid)
{
	return OnPlayAnim(playerid,"ON_LOOKERS","panic_in",4.0,0,0,1,1,0);
}

CMD:gog(playerid)
{
	return OnPlayAnim(playerid,"goggles","goggles_put_on",4.1,0,1,1,1,1);
}

CMD:nfall(playerid)
{
	return OnPlayAnim(playerid,"HEIST9","CAS_G2_GasKO",4.1,0,1,1,1,0);
}

CMD:hsit(playerid)
{
	return OnPlayAnim(playerid,"INT_OFFICE","OFF_Sit_Bored_Loop",4.0,1,0,1,1,0);
}

CMD:csit(playerid)
{
	return OnPlayAnim(playerid,"INT_OFFICE","OFF_Sit_Type_Loop",4.0,1,0,1,1,0);
}

CMD:bracing(playerid)
{
	return OnPlayAnim(playerid,"PLAYIDLES","stretch",4.0,0,0,0,0,0);
}

CMD:pool(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/pool [1-5]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"POOL","POOL_ChalkCue",4.0,0,1,1,1,1);
		case 2: OnPlayAnim(playerid,"POOL","POOL_Long_Shot",4.0,0,1,1,1,1);
		case 3: OnPlayAnim(playerid,"POOL","POOL_Med_Shot",4.0,0,1,1,1,1);
		case 4: OnPlayAnim(playerid,"POOL","POOL_Short_Shot",4.0,0,1,1,1,1);
		case 5: OnPlayAnim(playerid,"POOL","POOL_XLong_Shot",4.0,0,1,1,1,1);
		default: SendHelpMessage(playerid, "/pool [1-5]");
	}
	return 1;
}

CMD:search(playerid)
{
	return OnPlayAnim(playerid,"ROB_BANK","CAT_Safe_Rob",4.0,1,0,0,0,0);
}

CMD:daim(playerid)
{
	return OnPlayAnim(playerid,"RYDER","Van_Stand_Crate",3.0,0,1,1,1,0);
}

CMD:rhandl(playerid)
{
	return OnPlayAnim(playerid,"SCRATCHING","scdlulp",4.0,0,0,1,1,0);
}

CMD:rhandr(playerid)
{
	return OnPlayAnim(playerid,"SCRATCHING","scdrulp",4.0,0,0,1,1,0);
}

CMD:hstand(playerid)
{
	return OnPlayAnim(playerid,"SCRATCHING","scdrdlp",4.0,0,0,1,1,0);
}

CMD:nlean(playerid)
{
	return OnPlayAnim(playerid,"SHAMAL","SHAMAL_align",4.1,0,1,1,1,0);
}

CMD:shelter(playerid)
{
	return OnPlayAnim(playerid,"SHOP","SHP_Duck_Fire",4.0,0,0,1,1,0);
}

CMD:wsit(playerid)
{
	return OnPlayAnim(playerid,"SHOP","SHP_Gun_Duck",4.0,0,0,1,1,0);
}

CMD:waim(playerid)
{
	return OnPlayAnim(playerid,"TEC","TEC_reload",4.1,0,1,1,1,1);
}

CMD:hangon(playerid)
{
	return OnPlayAnim(playerid,"TRAIN","tran_ouch",4.0,0,0,1,1,0);
}

CMD:blowj(playerid)
{
	return OnPlayAnim(playerid,"BLOWJOBZ", "BJ_COUCH_START_W", 4.1, 0, 1, 1, 1, 0 );
}

CMD:rseat(playerid)
{
	return OnPlayAnim(playerid,"BLOWJOBZ", "BJ_COUCH_START_P", 4.1, 0, 1, 1, 1, 0 );
}

CMD:polwalk(playerid)
{
	return OnPlayAnim(playerid,"POLICE","Cop_move_FWD",3.0,1,1,1,1,1);
}

CMD:wallow(playerid,params[])
{
	new type;

	if (sscanf(params,"d", type))	return SendHelpMessage(playerid, "/wallow [1-2]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"MD_CHASE","MD_BIKE_Lnd_Roll",4.1,0,1,1,1,0);
		case 2: OnPlayAnim(playerid,"MD_CHASE","MD_BIKE_Lnd_Roll_F",4.1,0,1,1,1,0);
		default: SendHelpMessage(playerid, "/wallow [1-2]");
	}
	return 1;
}

CMD:nflip(playerid)
{
	return OnPlayAnim(playerid,"PED","BIKE_fall_off",4.1,0,1,1,1,0);
}

CMD:csdead(playerid)
{
	return OnPlayAnim(playerid,"WUZI","CS_Dead_Guy",4.1,1,1,1,1,0);
}

CMD:follow(playerid)
{
	return OnPlayAnim(playerid,"WUZI","Wuzi_follow",4.1,0,1,1,1,1);
}

CMD:grnd(playerid)
{
	return OnPlayAnim(playerid,"WUZI","Wuzi_grnd_chk",4.0,0,0,0,0,0);
}

CMD:copa(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/copa [1-9]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"POLICE","CopTraf_Away",4.1,0,0,0,0,0);
		case 2: OnPlayAnim(playerid,"POLICE","CopTraf_Come",4.1,0,0,0,0,0);
		case 3: OnPlayAnim(playerid,"POLICE","CopTraf_Left",4.1,0,0,0,0,0);
		case 4: OnPlayAnim(playerid,"POLICE","CopTraf_Stop",4.1,0,0,0,0,0);
		case 5: OnPlayAnim(playerid,"POLICE","Cop_move_FWD",4.1,1,1,1,1,1);
		case 6: OnPlayAnim(playerid,"POLICE","crm_drgbst_01",4.1,0,0,0,1,5000);
		case 7: OnPlayAnim(playerid,"POLICE","Door_Kick",4.1,0,1,1,1,1);
		case 8: OnPlayAnim(playerid,"POLICE","plc_drgbst_01",4.1,0,0,0,0,5000);
		case 9: OnPlayAnim(playerid,"POLICE","plc_drgbst_02",4.1,0,0,0,0,0);
		default: SendHelpMessage(playerid, "/copa [1-9]");
	}
	return 1;
}

CMD:cmon(playerid)
{
	return OnPlayAnim(playerid,"OTB","wtchrace_cmon",4.1,0,1,1,1,0);
}

CMD:lose(playerid)
{
	return OnPlayAnim(playerid,"OTB","wtchrace_lose",4.1,0,1,1,1,0);
}

CMD:gunthreat(playerid)
{
	return OnPlayAnim(playerid,"SHOP","SHP_Gun_Threat",4.1,0,1,1,1,0);
}

CMD:givecash(playerid)
{
	return OnPlayAnim(playerid,"SHOP","SHP_Rob_GiveCash",4.1,0,1,1,1,0);
}

CMD:robr(playerid)
{
	return OnPlayAnim(playerid,"SHOP","SHP_Rob_React",4.1,0,1,1,1,0);
}

CMD:serend(playerid)
{
	return OnPlayAnim(playerid,"SHOP","SHP_Serve_End",4.1,0,1,1,1,0);
}

CMD:heist(playerid)
{
	return OnPlayAnim(playerid,"HEIST9","Use_SwipeCard",4.0,0,0,0,0,0);
}

CMD:beckon(playerid,params[])
{
	new type;
	
	if (sscanf(params,"d", type))	return SendHelpMessage(playerid, "/beckon [1-3]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"RYDER","RYD_Beckon_01",4.1,0,1,1,1,1);
		case 2: OnPlayAnim(playerid,"RYDER","RYD_Beckon_02",4.1,0,1,1,1,1);
		case 3: OnPlayAnim(playerid,"RYDER","RYD_Beckon_03",4.1,0,1,1,1,1);
		default: SendHelpMessage(playerid, "/beckon [1-3]");
	}
	return 1;
}

CMD:shotgun(playerid,params[])
{
	new type;

	if (sscanf(params,"d", type))	return SendHelpMessage(playerid, "/shotgun [1-3]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"SHOTGUN","shotgun_crouchfire",4.1,0,1,1,1,1);
		case 2: OnPlayAnim(playerid,"SHOTGUN","shotgun_fire",4.1,0,1,1,1,1);
		case 3: OnPlayAnim(playerid,"SHOTGUN","shotgun_fire_poor",4.1,0,1,1,1,1);
		default: SendHelpMessage(playerid, "/shotgun [1-3]");
	}
	return 1;
}

CMD:colt(playerid,params[])
{
	new type;

	if (sscanf(params,"i", type))	return SendHelpMessage(playerid, "/colt [1-3]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"COLT45","colt45_fire_2hands",4.1,0,1,1,1,1);
		case 2: OnPlayAnim(playerid,"COLT45","colt45_reload",4.1,0,1,1,1,1);
		case 3: OnPlayAnim(playerid,"COLT45","sawnoff_reload",4.1,0,1,1,1,1);
		default: SendHelpMessage(playerid, "/colt [1-3]");
	}
	return 1;
}

CMD:climb(playerid)
{
	return OnPlayAnim(playerid,"PED","CLIMB_jump",4.0,0,0,1,1,0);
}

CMD:tlean(playerid)
{
	return OnPlayAnim(playerid,"PED","DRIVE_BOAT",4.0,0,0,1,1,0);
}

CMD:pushup(playerid)
{
	return OnPlayAnim(playerid,"PED","getup_front",4.0,1,0,1,1,0);
}

CMD:protection(playerid)
{
	return OnPlayAnim(playerid,"PED","FightShB",4.0,0,1,1,1,1);
}

CMD:swallow(playerid)
{
	return OnPlayAnim(playerid,"PED","gum_eat",4.0,0,0,0,0,0);
}

CMD:stand(playerid)
{
	return OnPlayAnim(playerid,"PED","IDLE_HBHB",3.0,0,1,1,1,0);
}

CMD:pullhand(playerid)
{
	return OnPlayAnim(playerid,"PED","Jetpack_Idle",3.0,0,1,1,1,0);
}

CMD:nlook(playerid)
{
	return OnPlayAnim(playerid,"PED","roadcross_gang",4.0,0,0,0,0,0);
}

CMD:extend(playerid)
{
	return OnPlayAnim(playerid,"PED","Shove_Partial",4.1,0,1,1,1,1);	
}

CMD:carchat(playerid, params[])
{
	new type;

	if (!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_WHITE, "Ban can phai ngoi trong xe.");
	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/carchat [1-19]");

	switch(type)
	{
		case 1:  ApplyAnimation(playerid,"CAR_CHAT","carfone_in",4.1,0,1,1,1,1,1);
		case 2:  ApplyAnimation(playerid,"CAR_CHAT","carfone_loopA",4.1,0,1,1,1,1,1);
		case 3:  ApplyAnimation(playerid,"CAR_CHAT","carfone_loopA_to_B",4.1,0,1,1,1,1,1);
		case 4:  ApplyAnimation(playerid,"CAR_CHAT","carfone_loopB",4.1,0,1,1,1,1,1);
		case 5:  ApplyAnimation(playerid,"CAR_CHAT","carfone_loopB_to_A",4.1,0,1,1,1,1,1);
		case 6:  ApplyAnimation(playerid,"CAR_CHAT","carfone_out",4.1,0,1,1,1,1,1);
		case 7:  ApplyAnimation(playerid,"CAR_CHAT","CAR_Sc1_BL",4.1,0,1,1,1,1,1);
		case 8:  ApplyAnimation(playerid,"CAR_CHAT","CAR_Sc1_BR",4.1,0,1,1,1,1,1);
		case 9:  ApplyAnimation(playerid,"CAR_CHAT","CAR_Sc1_FL",4.1,0,1,1,1,1,1);
		case 10: ApplyAnimation(playerid,"CAR_CHAT","CAR_Sc1_FR",4.1,0,1,1,1,1,1);
		case 11: ApplyAnimation(playerid,"CAR_CHAT","CAR_Sc2_FL",4.1,0,1,1,1,1,1);
		case 12: ApplyAnimation(playerid,"CAR_CHAT","CAR_Sc3_BR",4.1,0,1,1,1,1,1);
		case 13: ApplyAnimation(playerid,"CAR_CHAT","CAR_Sc3_FL",4.1,0,1,1,1,1,1);
		case 14: ApplyAnimation(playerid,"CAR_CHAT","CAR_Sc3_FR",4.1,0,1,1,1,1,1);
		case 15: ApplyAnimation(playerid,"CAR_CHAT","CAR_Sc4_BL",4.1,0,1,1,1,1,1);
		case 16: ApplyAnimation(playerid,"CAR_CHAT","CAR_Sc4_BR",4.1,0,1,1,1,1,1);
		case 17: ApplyAnimation(playerid,"CAR_CHAT","CAR_Sc4_FL",4.1,0,1,1,1,1,1);
		case 18: ApplyAnimation(playerid,"CAR_CHAT","CAR_Sc4_FR",4.1,0,1,1,1,1,1);
		case 19: ApplyAnimation(playerid,"CAR","Sit_relaxed",4.1,0,1,1,1,1,1);
		default: SendHelpMessage(playerid, "/carchat [1-19]");
	}
	return 1;
}

CMD:repcar(playerid,params[])
{
	new type;

	if (sscanf(params,"d", type))	return SendHelpMessage(playerid, "/repcar [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"CAR","Fixn_Car_Loop",3.0,0,1,1,1,0);
		case 2: OnPlayAnim(playerid,"CAR","Fixn_Car_Out",3.0,0,1,1,1,0);
		default: SendHelpMessage(playerid, "/repcar [1-2]");
	}
	return 1;
}

CMD:onehand(playerid)
{
	return OnPlayAnim(playerid,"PED","CLIMB_jump2fall",4.1,0,1,1,1,0);
}

CMD:backwalk(playerid)
{
	return OnPlayAnim(playerid,"PED","gas_cwr",4.1,1,1,1,1,1);
}

CMD:mirro(playerid)
{
	return OnPlayAnim(playerid,"CLOTHES","CLO_Pose_Hat",4.1,0,1,1,1,0);
}

CMD:hips(playerid)
{
	return OnPlayAnim(playerid,"COP_AMBIENT","Coplook_shake",4.1,0,1,1,1,0);
}

CMD:pullo(playerid)
{
	return OnPlayAnim(playerid,"AIRPORT","thrw_barl_thrw",4.1,0,1,1,1,0);
}

CMD:rtake(playerid)
{
	return OnPlayAnim(playerid,"BAR","Barserve_give",4.0,0,0,0,0,0);
}

CMD:obalance(playerid)
{
	return OnPlayAnim(playerid,"BD_FIRE","BD_Panic_02",4.1,0,1,1,1,0);
}

CMD:wobalance(playerid)
{
	return OnPlayAnim(playerid,"BD_FIRE","BD_Panic_03",4.1,0,1,1,1,0);
}

CMD:taction(playerid)
{
	return OnPlayAnim(playerid,"BD_FIRE","wash_up",4.0,0,0,0,0,0);
}

CMD:catch(playerid)
{
	return OnPlayAnim(playerid,"MD_CHASE","MD_HANG_Loop",4.1,0,1,1,1,0);
}

CMD:despair(playerid)
{
	return OnPlayAnim(playerid,"BSKTBALL","BBALL_react_miss",4.0,0,0,0,0,0);
}

CMD:rooting(playerid)
{
	return OnPlayAnim(playerid,"BSKTBALL","BBALL_react_score",4.1,1,1,1,1,0);
}

CMD:spyglass(playerid)
{
	return OnPlayAnim(playerid,"CAMERA","picstnd_take",4.1,0,1,1,1,0);
}

CMD:looksky(playerid)
{
	return OnPlayAnim(playerid,"ON_LOOKERS","lkup_loop",4.1,0,1,1,1,0);
}

CMD:show(playerid,params[])
{
	new type;

	if (sscanf(params,"d", type))	return SendHelpMessage(playerid, "/show [1-3]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"ON_LOOKERS","panic_point",4.1,0,1,1,1,0);
		case 2: OnPlayAnim(playerid,"ON_LOOKERS","panic_shout",4.1,0,1,1,1,0);
		case 3: OnPlayAnim(playerid,"ON_LOOKERS","point_loop",4.1,0,1,1,1,0);
		default: SendHelpMessage(playerid, "/show [1-3]");
	}
	return 1;
}

CMD:sh(playerid)
{
	return OnPlayAnim(playerid,"ON_LOOKERS","shout_in",4.0,0,0,0,0,0);
}

CMD:crossfingers(playerid)
{
	return OnPlayAnim(playerid,"CASINO","cards_loop",4.1,0,1,1,1,0);
}

CMD:delight(playerid)
{
	return OnPlayAnim(playerid,"CASINO","manwinb",4.0,0,0,0,0,0);
}

CMD:hdelight(playerid)
{
	return OnPlayAnim(playerid,"CASINO","manwind",4.0,0,0,0,0,0);
}

CMD:showtable(playerid)
{
	return OnPlayAnim(playerid,"CASINO","Roulette_bet",4.1,0,1,1,1,0);
}

CMD:tfacepalm(playerid)
{
	return OnPlayAnim(playerid,"CASINO","Roulette_lose",4.1,0,1,1,1,0);
}

CMD:tpull(playerid)
{
	return OnPlayAnim(playerid,"CASINO","Slot_in",4.1,0,1,1,1,0);
}

CMD:fail(playerid)
{
	return OnPlayAnim(playerid,"CASINO","Slot_lose_out",4.0,0,0,0,0,0);
}

CMD:joy(playerid)
{
	return OnPlayAnim(playerid,"CASINO","Slot_win_out",4.0,0,0,0,0,0);
}

CMD:wear(playerid)
{
	return OnPlayAnim(playerid,"CARRY","crry_prtial",4.1,0,1,1,1,1);
}

CMD:fsit(playerid)
{
	return OnPlayAnim(playerid,"JST_BUISNESS","girl_02",4.1,0,1,1,1,0);
}

CMD:crushjump(playerid)
{
	return OnPlayAnim(playerid,"DODGE","Crush_Jump",4.1,0,1,1,1,0);
}

CMD:scatch(playerid)
{
	return OnPlayAnim(playerid,"FINALE","FIN_LegsUp_Loop",4.1,0,1,1,1,0);
}

CMD:lookfloor(playerid)
{
	return OnPlayAnim(playerid,"FINALE","FIN_Cop1_Loop",4.1,0,1,1,1,0);
}

CMD:tramplefloor(playerid)
{
	return OnPlayAnim(playerid,"FINALE","FIN_Cop1_Stomp",4.1,1,1,1,1,0);
}

CMD:swing(playerid)
{
	return OnPlayAnim(playerid,"Freeweights","gym_free_B",4.1,0,1,1,1,0);
}

CMD:nbp(playerid)
{
	return OnPlayAnim(playerid,"Freeweights","gym_free_celebrate",4.0,0,0,0,0,0);
}

CMD:takehead(playerid)
{
	return OnPlayAnim(playerid,"HAIRCUTS","BRB_Cut",4.1,0,1,1,1,0);
}

CMD:readtable(playerid)
{
	return OnPlayAnim(playerid,"SCRATCHING","sclng_r",4.1,0,1,1,1,0);
}

CMD:squat(playerid)
{
	return OnPlayAnim(playerid,"TEC","TEC_crouchfire",4.1,0,1,1,1,0);
}

CMD:sprawl(playerid)
{
	return OnPlayAnim(playerid,"TATTOOS","TAT_Idle_Loop_T",4.1,0,1,1,1,0);
}

CMD:throwmoney(playerid)
{
	return OnPlayAnim(playerid,"STRIP","PUN_CASH",4.0,0,0,0,0,0);
}

CMD:faim(playerid)
{
	return OnPlayAnim(playerid,"SWORD","sword_IDLE",4.1,0,1,1,1,0);
}

CMD:swingback(playerid)
{
	return OnPlayAnim(playerid,"PED","getup",4.1, 0, 1, 1, 1, 0);
}

CMD:hands(playerid,params[])
{
	new type;

	if (sscanf(params,"d", type))	return SendHelpMessage(playerid, "/hands [1-7]");
	
	switch(type)
	{
		case 1: OnPlayAnim(playerid,"LOWRIDER","prtial_gngtlkB",4.1,0,1,1,1,1);
		case 2: OnPlayAnim(playerid,"LOWRIDER","prtial_gngtlkC",4.1,0,1,1,1,1);
		case 3: OnPlayAnim(playerid,"LOWRIDER","prtial_gngtlkD",4.1,0,1,1,1,1);
		case 4: OnPlayAnim(playerid,"LOWRIDER","prtial_gngtlkE",4.1,0,1,1,1,1);
		case 5: OnPlayAnim(playerid,"LOWRIDER","prtial_gngtlkF",4.1,0,1,1,1,1);
		case 6: OnPlayAnim(playerid,"LOWRIDER","prtial_gngtlkG",4.1,0,1,1,1,1);
		case 7: OnPlayAnim(playerid,"LOWRIDER","prtial_gngtlkH",4.1,0,1,1,1,1);
		default: SendHelpMessage(playerid, "/hands [1-7]");
	}
	return 1;
}

CMD:cellin(playerid)
{
	// TO DO
	return SetPlayerSpecialAction(playerid,SPECIAL_ACTION_USECELLPHONE);
}

CMD:cellout(playerid)
{
	// TO DO
	return SetPlayerSpecialAction(playerid,SPECIAL_ACTION_STOPUSECELLPHONE);
}

CMD:drunk(playerid)
{
	return OnPlayAnim(playerid,"PED","WALK_DRUNK",4.1,1,1,1,1,1);
}

CMD:kicking(playerid)
{
	return OnPlayAnim(playerid,"GANGS","shake_carK",4.0,0,0,0,0,0);
}

CMD:bomb(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/bomb [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"BOMBER","BOM_Plant",4.0,0,0,0,0,0);
		case 2: OnPlayAnim(playerid,"BOMBER","BOM_Plant_Loop",4.0,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/bomb [1-2]");
	}
	return 1;
}

CMD:opendoor(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/opendoor [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"PED","Walk_DoorPartial",4.0,0,0,0,0,0);
		case 2: OnPlayAnim(playerid,"AIRPORT","thrw_barl_thrw",4.0,0,0,0,0,0);
		default: SendHelpMessage(playerid, "/opendoor [1-2]");
	}
	return 1;
}

CMD:robman(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/robman [1-4]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"SHOP","ROB_Loop_Threat",4.0,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"SHOP","SHP_Gun_Aim",4.0,1,0,0,0,0);
		case 3: OnPlayAnim(playerid,"SHOP","SHP_Gun_Threat",4.0,1,0,0,0,0);
		case 4: OnPlayAnim(playerid,"PED","Gun_stand",4.0,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/robman [1-4]");
	}
	return 1;
}

CMD:camera(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/camera [1-9]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"CAMERA","camcrch_idleloop",4.0,1,0,0,1,0);
		case 2: OnPlayAnim(playerid,"CAMERA","camcrch_cmon",4.0,1,0,0,0,0);
		case 3: OnPlayAnim(playerid,"CAMERA","camcrch_to_camstnd",4.0,0,0,0,1,1);
		case 4: OnPlayAnim(playerid,"CAMERA","camstnd_cmon",4.0,1,0,0,0,0);
		case 5: OnPlayAnim(playerid,"CAMERA","camstnd_idleloop",4.0,1,0,0,0,0);
		case 6: OnPlayAnim(playerid,"CAMERA","camstnd_lkabt",4.0,1,0,0,0,0);
		case 7: OnPlayAnim(playerid,"CAMERA","camstnd_to_camcrch",4.0,0,0,0,1,1);
		case 8: OnPlayAnim(playerid,"CAMERA","piccrch_take",4.0,1,0,0,0,0);
		case 9: OnPlayAnim(playerid,"CAMERA","picstnd_take",4.0,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/camera [1-9]");
	}
	return 1;
}

CMD:carry(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/carry [1-6]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"CARRY","liftup",3.8,0,0,0,0,0);
		case 2: OnPlayAnim(playerid,"CARRY","liftup05",3.8,0,0,0,0,0);
		case 3: OnPlayAnim(playerid,"CARRY","liftup105",3.8,0,0,0,0,0);
		case 4: OnPlayAnim(playerid,"CARRY","putdwn",3.8,0,0,0,0,0);
		case 5: OnPlayAnim(playerid,"CARRY","putdwn05",3.8,0,0,0,0,0);
		case 6: OnPlayAnim(playerid,"CARRY","putdwn105",3.8,0,0,0,0,0);
		default: SendHelpMessage(playerid, "/carry [1-6]");
	}
	return 1;
}

CMD:cslot(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/cslot [1-7]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"CASINO","Slot_bet_01",4.0,0,0,0,0,0);
		case 2: OnPlayAnim(playerid,"CASINO","Slot_bet_02",4.0,0,0,0,0,0);
		case 3: OnPlayAnim(playerid,"CASINO","Slot_in",4.0,0,0,0,0,0);
		case 4: OnPlayAnim(playerid,"CASINO","Slot_lose_out",4.0,0,0,0,0,0);
		case 5: OnPlayAnim(playerid,"CASINO","Slot_Plyr",4.0,0,0,0,0,0);
		case 6: OnPlayAnim(playerid,"CASINO","Slot_wait",4.0,1,0,0,0,0);
		case 7: OnPlayAnim(playerid,"CASINO","Slot_win_out",4.0,0,0,0,0,0);
		default: SendHelpMessage(playerid, "/cslot [1-7]");
	}
	return 1;
}

CMD:croulette(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/croulette [1-5]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"CASINO","Roulette_bet",4.0,0,0,0,0,0);
		case 2: OnPlayAnim(playerid,"CASINO","Roulette_loop",4.0,0,0,0,0,0);
		case 3: OnPlayAnim(playerid,"CASINO","Roulette_lose",4.0,0,0,0,0,0);
		case 4: OnPlayAnim(playerid,"CASINO","Roulette_out",4.0,0,0,0,0,0);
		case 5: OnPlayAnim(playerid,"CASINO","Roulette_win",4.0,0,0,0,0,0);
		default: SendHelpMessage(playerid, "/croulette [1-5]");
	}
	return 1;
}

CMD:ccards(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/ccards [1-6]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"CASINO","cards_in",4.0,0,0,0,0,0);
		case 2: OnPlayAnim(playerid,"CASINO","cards_loop",4.0,0,0,0,0,0);
		case 3: OnPlayAnim(playerid,"CASINO","cards_lose",4.0,0,0,0,0,0);
		case 4: OnPlayAnim(playerid,"CASINO","cards_pick_01",4.0,0,0,0,0,0);
		case 5: OnPlayAnim(playerid,"CASINO","cards_pick_02",4.0,0,0,0,0,0);
		case 6: OnPlayAnim(playerid,"CASINO","dealone",4.0,0,0,0,0,0);
		default: SendHelpMessage(playerid, "/ccards [1-6]");
	}
	return 1;
}

CMD:pose(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/pose [1-7]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"CLOTHES","CLO_Buy",4.0,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"CLOTHES","CLO_Pose_Hat",4.0,1,0,0,0,0);
		case 3: OnPlayAnim(playerid,"CLOTHES","CLO_Pose_Legs",4.0,1,0,0,0,0);
		case 4: OnPlayAnim(playerid,"CLOTHES","CLO_Pose_Loop",4.0,1,0,0,0,0);
		case 5: OnPlayAnim(playerid,"CLOTHES","CLO_Pose_Shoes",4.0,1,0,0,0,0);
		case 6: OnPlayAnim(playerid,"CLOTHES","CLO_Pose_Torso",4.0,1,0,0,0,0);
		case 7: OnPlayAnim(playerid,"CLOTHES","CLO_Pose_Watch",4.0,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/pose [1-7]");
	}
	return 1;
}

CMD:eat(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/eat [1-3]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"FOOD","EAT_Burger",3.0,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"FOOD","EAT_Chicken",3.0,1,0,0,0,0);
		case 3: OnPlayAnim(playerid,"FOOD","EAT_Pizza",3.0,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/eat [1-3]");
	}
	return 1;
}

CMD:swata(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/swata [1-8]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"HEIST9","swt_wllpk_L",4.0,0,1,1,1,1);
		case 2: OnPlayAnim(playerid,"HEIST9","swt_wllpk_R",4.0,0,1,1,1,1);
		case 3: OnPlayAnim(playerid,"HEIST9","swt_wllshoot_in_L",4.0,0,1,1,1,1);
		case 4: OnPlayAnim(playerid,"HEIST9","swt_wllshoot_in_R",4.0,0,1,1,1,1);
		case 5: OnPlayAnim(playerid,"SWAT","swt_lkt",4.0,0,1,1,1,1);
		case 6: OnPlayAnim(playerid,"SWAT","swt_sty",4.0,0,1,1,1,1);
		case 7: OnPlayAnim(playerid,"ped","Crouch_Roll_L",4.0,0,1,1,1,1);
		case 8: OnPlayAnim(playerid,"ped","Crouch_Roll_R",4.0,0,1,1,1,1);
		default: SendHelpMessage(playerid, "/swata [1-8]");
	}
	return 1;
}

CMD:argue(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/argue [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"KISSING","GF_StreetArgue_02",4.0,1,1,1,1,1);
		case 2: OnPlayAnim(playerid,"KISSING","GF_StreetArgue_01",4.0,1,1,1,1,1);
		default: SendHelpMessage(playerid, "/argue [1-2]");
	}
	return 1;
}

CMD:presenta(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/presenta [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"KISSING","gift_get",4.0,0,1,1,1,1);
		case 2: OnPlayAnim(playerid,"KISSING","gift_give",4.0,0,1,1,1,1);
		default: SendHelpMessage(playerid, "/presenta [1-2]");
	}
	return 1;
}

CMD:basketball(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/basketball [1-7]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"BSKTBALL","BBALL_def_loop",4.0,1,1,1,1,1);
		case 2: OnPlayAnim(playerid,"BSKTBALL","BBALL_idleloop",4.0,1,1,1,1,1);
		case 3: OnPlayAnim(playerid,"BSKTBALL","BBALL_Jump_Shot",4.0,0,1,1,1,1);
		case 4: OnPlayAnim(playerid,"BSKTBALL","BBALL_pickup",4.0,0,1,1,1,1);
		case 5: OnPlayAnim(playerid,"BSKTBALL","BBALL_walk",4.0,1,1,1,1,1);
		case 6: OnPlayAnim(playerid,"BSKTBALL","BBALL_def_jump_shot",4.0,0,1,1,1,1);
		case 7: OnPlayAnim(playerid,"BSKTBALL","BBALL_run",4.0,1,1,1,1,1);
		default: SendHelpMessage(playerid, "/basketball [1-7]");
	}
	return 1;
}

CMD:dive(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/dive [1-3]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"DODGE","Crush_Jump",4.0,0,1,1,1,0);
		case 2: OnPlayAnim(playerid,"PED","EV_dive",4.0,0,1,1,1,0);
		case 3: OnPlayAnim(playerid,"PED","EV_step",4.0,0,1,1,1,0);
		default: SendHelpMessage(playerid, "/dive [1-3]");
	}
	return 1;
}

CMD:getup(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/getup [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"PED","getup",4.0,0,0,0,0,0);
		case 2: OnPlayAnim(playerid,"PED","getup_front",4.0,0,0,0,0,0);
		default: SendHelpMessage(playerid, "/getup [1-2]");
	}
	return 1;
}

CMD:robbed(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/robbed [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"SHOP","SHP_Rob_GiveCash",4.0,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"SHOP","SHP_Rob_React",4.0,0,0,0,0,0);
		default: SendHelpMessage(playerid, "/robbed [1-2]");
	}
	return 1;
}

CMD:angry(playerid, params[])
{
	return OnPlayAnim(playerid,"RIOT","RIOT_ANGRY",4.0,0,0,0,0,0);
}

CMD:headbutt(playerid, params[])
{
	return OnPlayAnim(playerid,"WAYFARER","WF_Fwd",4.0,0,0,0,0,0);
}

CMD:airkick(playerid, params[])
{
	return OnPlayAnim(playerid,"FIGHT_C","FightC_M",4.0,0,1,1,0,0);
}

CMD:doorkick(playerid, params[])
{
	return OnPlayAnim(playerid,"POLICE","Door_Kick",4.0,0,0,0,0,0);
}

CMD:leftslap(playerid, params[])
{
	return OnPlayAnim(playerid,"PED","BIKE_elbowL",4.0,0,0,0,0,0);
}

CMD:elbow(playerid, params[])
{
	return OnPlayAnim(playerid,"FIGHT_D","FightD_3",4.0,0,1,1,0,0);
}

CMD:coprun(playerid, params[])
{
	return OnPlayAnim(playerid,"SWORD","sword_block",50.0,0,1,1,1,1);
}

CMD:rob(playerid, params[])
{
	return OnPlayAnim(playerid,"ped","ARRESTgun",4.0,0,1,1,1,1);
}

CMD:hide(playerid, params[])
{
	return OnPlayAnim(playerid,"ped","cower",3.0,1,0,0,0,0);
}

CMD:drinkwater(playerid, params[])
{
	return OnPlayAnim(playerid,"BAR","dnk_stndF_loop",4.0,1,0,0,0,0);
}

CMD:checktime(playerid, params[])
{
	return OnPlayAnim(playerid,"COP_AMBIENT","Coplook_watch",4.0,0,0,0,0,0);
}

CMD:sleep(playerid, params[])
{
	return OnPlayAnim(playerid,"CRACK","crckdeth4",4.0,0,1,1,1,0);
}

CMD:blob(playerid, params[])
{
	return OnPlayAnim(playerid,"CRACK","crckidle1",4.0,0,1,1,1,0);
}

CMD:wavedown(playerid, params[])
{
	return OnPlayAnim(playerid,"BD_FIRE","BD_Panic_01",4.0,0,0,0,0,0);
}

CMD:cpr(playerid, params[])
{
	return OnPlayAnim(playerid,"MEDIC","CPR",4.0,0,0,0,0,0);
}

CMD:showoff(playerid, params[])
{
	return OnPlayAnim(playerid,"Freeweights","gym_free_celebrate",4.0,1,0,0,0,0);
}

CMD:throw(playerid, params[])
{
	return OnPlayAnim(playerid,"GRENADE","WEAPON_throw",4.0,0,0,0,0,0);
}

CMD:hurt(playerid, params[])
{
	return OnPlayAnim(playerid,"SWAT","gnstwall_injurd",4.0,1,0,0,0,0);
}

CMD:washhands(playerid, params[])
{
	return OnPlayAnim(playerid,"BD_FIRE","wash_up",4.0,1,0,0,0,0);
}

CMD:crabs(playerid, params[])
{
	return OnPlayAnim(playerid,"MISC","Scratchballs_01",4.0,1,0,0,0,0);
}

CMD:salute(playerid, params[])
{
	return OnPlayAnim(playerid,"ON_LOOKERS","Pointup_loop",4.0,1,0,0,0,0);
}

CMD:jerkoff(playerid, params[])
{
	return OnPlayAnim(playerid,"PAULNMAC","wank_out",4.0,1,0,0,0,0);
}

CMD:lowbodypunch(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/lowbodypunch [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"FIGHT_B","FightB_G",4.0,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"KNIFE","Knife_4",4.0,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/lowbodypunch [1-2]");
	}
	return 1;
}

CMD:fightidle(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/fightidle [1-4]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"FIGHT_B","FightB_IDLE",4.0,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"FIGHT_C","FightC_IDLE",4.0,1,0,0,0,0);
		case 3: OnPlayAnim(playerid,"FIGHT_D","FightD_IDLE",4.0,1,0,0,0,0);
		case 4: OnPlayAnim(playerid,"PED","FIGHTIDLE",4.0,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/fightidle [1-4]");
	}
	return 1;
}

CMD:signal(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/signal [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"POLICE","CopTraf_Come",4.0,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"POLICE","CopTraf_Stop",4.0,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/signal [1-2]");
	}
	return 1;
}

CMD:nobreath(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/nobreath [1-3]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"SWEET","Sweet_injuredloop",4.0,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"PED","IDLE_tired",4.0,1,0,0,0,0);
		case 3: OnPlayAnim(playerid,"FAT","IDLE_tired",4.0,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/nobreath [1-3]");
	}
	return 1;
}

CMD:fallover(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/fallover [1-12]");

	switch(type)
	{
		case 1:  OnPlayAnim(playerid,"KNIFE","KILL_Knife_Ped_Die",4.0,0,1,1,1,0);
		case 2:  OnPlayAnim(playerid,"PED","KO_shot_face",4.0,0,1,1,1,0);
		case 3:  OnPlayAnim(playerid,"PED","KO_shot_stom",4.0,0,1,1,1,0);
		case 4:  OnPlayAnim(playerid,"PED","BIKE_fallR",4.1,0,1,1,1,0);
		case 5:  OnPlayAnim(playerid,"PED","BIKE_fall_off",4.1,0,1,1,1,0);
		case 6:  OnPlayAnim(playerid,"BASEBALL","Bat_Hit_3",4.1,0,1,1,1,0);
		case 7:  OnPlayAnim(playerid,"DILDO","Dildo_Hit_3",4.1,0,1,1,1,0);
		case 8:  OnPlayAnim(playerid,"HEIST9","CAS_G2_GasKO",4.1,0,1,1,1,0);
		case 9:  OnPlayAnim(playerid,"FIGHT_B","HitB_3",4.1,0,1,1,1,0);
		case 10: OnPlayAnim(playerid,"FIGHT_C","HitC_3",4.1,0,1,1,1,0);
		case 11: OnPlayAnim(playerid,"FIGHT_D","HitD_3",4.1,0,1,1,1,0);
		case 12: OnPlayAnim(playerid,"FIGHT_E","Hit_fightkick_B",4.1,0,1,1,1,0);
		default: SendHelpMessage(playerid, "/fallover [1-12]");
	}
	return 1;
}

CMD:pedmove(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/pedmove [1-27]");

	switch(type)
	{
		case 1:  OnPlayAnim(playerid,"PED","JOG_femaleA",4.0,1,1,1,1,1);
		case 2:  OnPlayAnim(playerid,"PED","JOG_maleA",4.0,1,1,1,1,1);
		case 3:  OnPlayAnim(playerid,"PED","WOMAN_walkfatold",4.0,1,1,1,1,1);
		case 4:  OnPlayAnim(playerid,"PED","run_fat",4.0,1,1,1,1,1);
		case 5:  OnPlayAnim(playerid,"PED","run_fatold",4.0,1,1,1,1,1);
		case 6:  OnPlayAnim(playerid,"PED","run_old",4.0,1,1,1,1,1);
		case 7:  OnPlayAnim(playerid,"PED","Run_Wuzi",4.0,1,1,1,1,1);
		case 8:  OnPlayAnim(playerid,"PED","swat_run",4.0,1,1,1,1,1);
		case 9:  OnPlayAnim(playerid,"PED","WALK_fat",4.0,1,1,1,1,1);
		case 10: OnPlayAnim(playerid,"PED","WALK_fatold",4.0,1,1,1,1,1);
		case 11: OnPlayAnim(playerid,"PED","WALK_gang1",4.0,1,1,1,1,1);
		case 12: OnPlayAnim(playerid,"PED","WALK_gang2",4.0,1,1,1,1,1);
		case 13: OnPlayAnim(playerid,"PED","WALK_old",4.0,1,1,1,1,1);
		case 14: OnPlayAnim(playerid,"PED","WALK_shuffle",4.0,1,1,1,1,1);
		case 15: OnPlayAnim(playerid,"PED","woman_run",4.0,1,1,1,1,1);
		case 16: OnPlayAnim(playerid,"PED","WOMAN_runbusy",4.0,1,1,1,1,1);
		case 17: OnPlayAnim(playerid,"PED","WOMAN_runfatold",4.0,1,1,1,1,1);
		case 18: OnPlayAnim(playerid,"PED","woman_runpanic",4.0,1,1,1,1,1);
		case 19: OnPlayAnim(playerid,"PED","WOMAN_runsexy",4.0,1,1,1,1,1);
		case 20: OnPlayAnim(playerid,"PED","WOMAN_walkbusy",4.0,1,1,1,1,1);
		case 21: OnPlayAnim(playerid,"PED","WOMAN_walknorm",4.0,1,1,1,1,1);
		case 22: OnPlayAnim(playerid,"PED","WOMAN_walkold",4.0,1,1,1,1,1);
		case 23: OnPlayAnim(playerid,"PED","WOMAN_walkpro",4.0,1,1,1,1,1);
		case 24: OnPlayAnim(playerid,"PED","WOMAN_walksexy",4.0,1,1,1,1,1);
		case 25: OnPlayAnim(playerid,"PED","WOMAN_walkshop",4.0,1,1,1,1,1);
		case 26: OnPlayAnim(playerid,"PED","WOMAN_walkfatold",4.0,1,1,1,1,1);
		case 27: OnPlayAnim(playerid,"FAT","FatWalk",4.0,1,1,1,1,1);
		default: SendHelpMessage(playerid, "/pedmove [1-27]");
	}
	return 1;
}

CMD:getjiggy(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/getjiggy [1-10]");

	switch(type)
	{
		case 1:  OnPlayAnim(playerid,"DANCING","DAN_Down_A",4.0,1,0,0,0,0);
		case 2:  OnPlayAnim(playerid,"DANCING","DAN_Left_A",4.0,1,0,0,0,0);
		case 3:  OnPlayAnim(playerid,"DANCING","DAN_Loop_A",4.0,1,0,0,0,0);
		case 4:  OnPlayAnim(playerid,"DANCING","DAN_Right_A",4.0,1,0,0,0,0);
		case 5:  OnPlayAnim(playerid,"DANCING","DAN_Up_A",4.0,1,0,0,0,0);
		case 6:  OnPlayAnim(playerid,"DANCING","dnce_M_a",4.0,1,0,0,0,0);
		case 7:  OnPlayAnim(playerid,"DANCING","dnce_M_b",4.0,1,0,0,0,0);
		case 8:  OnPlayAnim(playerid,"DANCING","dnce_M_c",4.0,1,0,0,0,0);
		case 9:  OnPlayAnim(playerid,"DANCING","dnce_M_c",4.0,1,0,0,0,0);
		case 10: OnPlayAnim(playerid,"DANCING","dnce_M_d",4.0,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/getjiggy [1-10]");
	}
	return 1;
}

CMD:stripclub(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/stripclub [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"STRIP","PLY_CASH",4.0,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"STRIP","PUN_CASH",4.0,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/stripclub [1-2]");
	}
	return 1;
}

CMD:smoke(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/smoke [1-6]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"SMOKING","M_smk_in",4.0,0,0,0,0,0);
		case 2: OnPlayAnim(playerid,"SMOKING","M_smklean_loop",4.0,1,0,0,0,0);
		case 3: OnPlayAnim(playerid,"SMOKING","F_smklean_loop",4.0,1,0,0,0,0);
		case 4: OnPlayAnim(playerid,"SMOKING","M_smk_tap",4.0,1,0,0,0,0);
		case 5: OnPlayAnim(playerid,"SMOKING","M_smk_drag",4.0,1,0,0,0,0);
		case 6: OnPlayAnim(playerid,"SMOKING","M_smk_loop",4.0,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/smoke [1-6]");
	}
	return 1;
}

CMD:dj(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/dj [1-4]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"SCRATCHING","scdldlp",4.0,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"SCRATCHING","scdlulp",4.0,1,0,0,0,0);
		case 3: OnPlayAnim(playerid,"SCRATCHING","scdrdlp",4.0,1,0,0,0,0);
		case 4: OnPlayAnim(playerid,"SCRATCHING","scdrulp",4.0,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/dj [1-4]");
	}
	return 1;
}

CMD:reload(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/reload [1-7]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"BUDDY","buddy_reload",4.0,0,0,0,0,0);
		case 2: OnPlayAnim(playerid,"PYTHON","python_reload",4.0,0,0,0,0,0);
		case 3: OnPlayAnim(playerid,"COLT45","colt45_reload",4.0,0,0,0,0,0);
		case 4: OnPlayAnim(playerid,"COLT45","sawnoff_reload",4.0,0,0,0,0,0);
		case 5: OnPlayAnim(playerid,"RIFLE","RIFLE_load",4.0,0,0,0,0,0);
		case 6: OnPlayAnim(playerid,"SILENCED","Silence_reload",4.0,0,0,0,0,0);
		case 7: OnPlayAnim(playerid,"TEC","TEC_reload",3.5,0,0,0,0,0);
		default: SendHelpMessage(playerid, "/reload [1-7]");
	}
	return 1;
}

CMD:crouchreload(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/crouchreload [1-5]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"BUDDY","buddy_crouchreload",4.0,0,0,0,0,0);
		case 2: OnPlayAnim(playerid,"COLT45","colt45_crouchreload",4.0,0,0,0,0,0);
		case 3: OnPlayAnim(playerid,"RIFLE","RIFLE_crouchload",4.0,0,0,0,0,0);
		case 4: OnPlayAnim(playerid,"SILENCED","CrouchReload",4.0,0,0,0,0,0);
		case 5: OnPlayAnim(playerid,"UZI","UZI_crouchreload",4.0,0,0,0,0,0);
		default: SendHelpMessage(playerid, "/crouchreload [1-5]");
	}
	return 1;
}

CMD:tag(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/tag [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"GRAFFITI","graffiti_Chkout",4.0,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"GRAFFITI","spraycan_fire",4.0,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/tag [1-2]");
	}
	return 1;
}

CMD:cheer(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/cheer [1-8]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"ON_LOOKERS","shout_01",4.0,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"ON_LOOKERS","shout_02",4.0,1,0,0,0,0);
		case 3: OnPlayAnim(playerid,"ON_LOOKERS","shout_in",4.0,1,0,0,0,0);
		case 4: OnPlayAnim(playerid,"RIOT","RIOT_ANGRY_B",4.0,1,0,0,0,0);
		case 5: OnPlayAnim(playerid,"RIOT","RIOT_CHANT",4.0,1,0,0,0,0);
		case 6: OnPlayAnim(playerid,"RIOT","RIOT_shout",4.0,1,0,0,0,0);
		case 7: OnPlayAnim(playerid,"STRIP","PUN_HOLLER",4.0,1,0,0,0,0);
		case 8: OnPlayAnim(playerid,"OTB","wtchrace_win",4.0,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/cheer [1-8]");
	}
	return 1;
}

CMD:siteat(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/siteat [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"FOOD","FF_Sit_Eat3",4.0,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"FOOD","FF_Sit_Eat2",4.0,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/siteat [1-2]");
	}
	return 1;
}

CMD:spank(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/spank [1-10]");

	switch(type)
	{
		case 1:  OnPlayAnim(playerid,"SNM","SPANKINGW",4.1,1,0,0,0,0);
		case 2:  OnPlayAnim(playerid,"SNM","SPANKINGP",4.1,1,0,0,0,0);
		case 3:  OnPlayAnim(playerid,"SNM","SPANKEDW",4.1,1,0,0,0,0);
		case 4:  OnPlayAnim(playerid,"SNM","SPANKEDP",4.1,1,0,0,0,0);
		case 5:  OnPlayAnim(playerid,"SNM","Spanked_IdleP",4.1,1,0,0,0,0);
		case 6:  OnPlayAnim(playerid,"SNM","Spanked_IdleW",4.1,1,0,0,0,0);
		case 7:  OnPlayAnim(playerid,"SNM","Spanking_endP",4.1,0,0,0,0,0);
		case 8:  OnPlayAnim(playerid,"SNM","Spanking_endW",4.1,0,0,0,0,0);
		case 9:  OnPlayAnim(playerid,"SNM","Spanking_SittingIdleW",4.1,1,0,0,0,0);
		case 10: OnPlayAnim(playerid,"SNM","Spanking_SittingW",4.1,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/spank [1-10]");
	}
	return 1;
}

CMD:sexy(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/sexy [1-6]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"STRIP","strip_E",4.1,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"STRIP","strip_G",4.1,1,0,0,0,0);
		case 3: OnPlayAnim(playerid,"STRIP","STR_A2B",4.1,0,0,0,0,0);
		case 4: OnPlayAnim(playerid,"STRIP","STR_Loop_A",4.1,1,0,0,0,0);
		case 5: OnPlayAnim(playerid,"STRIP","STR_Loop_B",4.1,1,0,0,0,0);
		case 6: OnPlayAnim(playerid,"STRIP","STR_Loop_C",4.1,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/sexy [1-6]");
	}
	return 1;
}

CMD:holdup(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/holdup [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"POOL","POOL_ChalkCue",4.1,0,1,1,1,1);
		case 2: OnPlayAnim(playerid,"POOL","POOL_Idle_Stance",4.1,0,1,1,1,1);
		default: SendHelpMessage(playerid, "/holdup [1-2]");
	}
	return 1;
}

CMD:misc(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/misc [1-4]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"CAR","Fixn_Car_Loop",4.1,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"CAR","flag_drop",4.1,0,0,0,0,0);
		case 3: OnPlayAnim(playerid,"PED","bomber",4.1,0,0,0,0,0);
		case 4: OnPlayAnim(playerid,"MISC","Plunger_01",4.1,0,1,1,1,1);
		default: SendHelpMessage(playerid, "/misc [1-4]");
	}
	return 1;
}

CMD:snatch(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/snatch [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"PED","BIKE_elbowL",4.1,0,0,0,0,0);
		case 2: OnPlayAnim(playerid,"PED","BIKE_elbowR",4.1,0,0,0,0,0);
		default: SendHelpMessage(playerid, "/snatch [1-2]");
	}
	return 1;
}

CMD:blowjob(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/blowjob [1-4]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"BLOWJOBZ","BJ_COUCH_LOOP_P",4.1,1,0,0,0,0);
		case 2: OnPlayAnim(playerid,"BLOWJOBZ","BJ_COUCH_LOOP_W",4.1,1,0,0,0,0);
		case 3: OnPlayAnim(playerid,"BLOWJOBZ","BJ_STAND_LOOP_P",4.1,1,0,0,0,0);
		case 4: OnPlayAnim(playerid,"BLOWJOBZ","BJ_STAND_LOOP_W",4.1,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/blowjob [1-4]");
	}
	return 1;
}

CMD:kiss(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/kiss [1-6]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"KISSING","Playa_Kiss_01",4.1,0,0,0,0,0);
		case 2: OnPlayAnim(playerid,"KISSING","Playa_Kiss_02",4.1,0,0,0,0,0);
		case 3: OnPlayAnim(playerid,"KISSING","Playa_Kiss_03",4.1,0,0,0,0,0);
		case 4: OnPlayAnim(playerid,"KISSING","Grlfrd_Kiss_01",4.1,0,0,0,0,0);
		case 5: OnPlayAnim(playerid,"KISSING","Grlfrd_Kiss_02",4.1,0,0,0,0,0);
		case 6: OnPlayAnim(playerid,"KISSING","Grlfrd_Kiss_03",4.1,0,0,0,0,0);
		default: SendHelpMessage(playerid, "/kiss [1-6]");
	}
	return 1;
}

CMD:idles(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/idles [1-33]");

	switch(type)
	{
		case 1:  OnPlayAnim(playerid,"PLAYIDLES","shift",4.1,1,1,1,1,1);
		case 2:  OnPlayAnim(playerid,"PLAYIDLES","shldr",4.1,1,1,1,1,1);
		case 3:  OnPlayAnim(playerid,"PLAYIDLES","stretch",4.1,1,1,1,1,1);
		case 4:  OnPlayAnim(playerid,"PLAYIDLES","strleg",4.1,1,1,1,1,1);
		case 5:  OnPlayAnim(playerid,"PLAYIDLES","time",4.1,1,1,1,1,1);
		case 6:  OnPlayAnim(playerid,"COP_AMBIENT","Copbrowse_loop",4.1,1,0,0,0,0);
		case 7:  OnPlayAnim(playerid,"COP_AMBIENT","Coplook_loop",4.1,1,0,0,0,0);
		case 8:  OnPlayAnim(playerid,"COP_AMBIENT","Coplook_shake",4.1,1,0,0,0,0);
		case 9:  OnPlayAnim(playerid,"COP_AMBIENT","Coplook_think",4.1,1,0,0,0,0);
		case 10: OnPlayAnim(playerid,"COP_AMBIENT","Coplook_watch",4.1,1,0,0,0,0);
		case 11: OnPlayAnim(playerid,"PED","roadcross",4.1,1,0,0,0,0);
		case 12: OnPlayAnim(playerid,"PED","roadcross_female",4.1,1,0,0,0,0);
		case 13: OnPlayAnim(playerid,"PED","roadcross_gang",4.1,1,0,0,0,0);
		case 14: OnPlayAnim(playerid,"PED","roadcross_old",4.1,1,0,0,0,0);
		case 15: OnPlayAnim(playerid,"PED","woman_idlestance",4.1,1,0,0,0,0);
		case 16: OnPlayAnim(playerid,"DEALER","DEALER_IDLE_01",4.1,1,0,0,0,0);
		case 17: OnPlayAnim(playerid,"DEALER","DEALER_IDLE_02",4.1,1,0,0,0,0);
		case 18: OnPlayAnim(playerid,"DEALER","DEALER_IDLE_03",4.1,1,0,0,0,0);
		case 19: OnPlayAnim(playerid,"DEALER","DEALER_IDLE",4.1,1,0,0,0,0);
		case 20: OnPlayAnim(playerid,"ON_LOOKERS","lkup_point",4.1,1,0,0,0,0);
		case 21: OnPlayAnim(playerid,"ON_LOOKERS","panic_cower",4.1,1,0,0,0,0);
		case 22: OnPlayAnim(playerid,"ON_LOOKERS","panic_hide",4.1,1,0,0,0,0);
		case 23: OnPlayAnim(playerid,"ON_LOOKERS","panic_loop",4.1,1,0,0,0,0);
		case 24: OnPlayAnim(playerid,"ON_LOOKERS","panic_point",4.1,1,0,0,0,0);
		case 25: OnPlayAnim(playerid,"ON_LOOKERS","panic_shout",4.1,1,0,0,0,0);
		case 26: OnPlayAnim(playerid,"ON_LOOKERS","point_loop",4.1,1,0,0,0,0);
		case 27: OnPlayAnim(playerid,"ON_LOOKERS","shout_loop",4.1,1,0,0,0,0);
		case 28: OnPlayAnim(playerid,"OTB","wtchrace_loop",4.1,1,0,0,0,0);
		case 29: OnPlayAnim(playerid,"PAULNMAC","PnM_Loop_A",4.1,1,0,0,0,0);
		case 30: OnPlayAnim(playerid,"PAULNMAC","PnM_Loop_B",4.1,1,0,0,0,0);
		case 31: OnPlayAnim(playerid,"MUSCULAR","MuscleIdle",4.1,1,0,0,0,0);
		case 32: OnPlayAnim(playerid,"FAT","FatIdle",4.1,1,0,0,0,0);
		case 33: OnPlayAnim(playerid,"PED","idlestance_old",4.1,1,0,0,0,0);
		default: SendHelpMessage(playerid, "/idles [1-33]");
	}
	return 1;
}

CMD:sunbathe(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/sunbathe [1-18]");

	switch(type)
	{
		case 1:  OnPlayAnim(playerid,"SUNBATHE","batherdown",4.1,0,1,1,1,1);
		case 2:  OnPlayAnim(playerid,"SUNBATHE","batherup",4.1,0,1,1,1,1);
		case 3:  OnPlayAnim(playerid,"SUNBATHE","Lay_Bac_in",4.1,0,1,1,1,1);
		case 4:  OnPlayAnim(playerid,"SUNBATHE","Lay_Bac_out",4.1,0,1,1,1,1);
		case 5:  OnPlayAnim(playerid,"SUNBATHE","ParkSit_M_IdleA",4.1,0,1,1,1,1);
		case 6:  OnPlayAnim(playerid,"SUNBATHE","ParkSit_M_IdleB",4.1,0,1,1,1,1);
		case 7:  OnPlayAnim(playerid,"SUNBATHE","ParkSit_M_IdleC",4.1,0,1,1,1,1);
		case 8:  OnPlayAnim(playerid,"SUNBATHE","ParkSit_M_in",4.1,0,1,1,1,1);
		case 9:  OnPlayAnim(playerid,"SUNBATHE","ParkSit_M_out",4.1,0,1,1,1,1);
		case 10: OnPlayAnim(playerid,"SUNBATHE","ParkSit_W_idleA",4.1,0,1,1,1,1);
		case 11: OnPlayAnim(playerid,"SUNBATHE","ParkSit_W_idleB",4.1,0,1,1,1,1);
		case 12: OnPlayAnim(playerid,"SUNBATHE","ParkSit_W_idleC",4.1,0,1,1,1,1);
		case 13: OnPlayAnim(playerid,"SUNBATHE","ParkSit_W_in",4.1,0,1,1,1,1);
		case 14: OnPlayAnim(playerid,"SUNBATHE","ParkSit_W_out",4.1,0,1,1,1,1);
		case 15: OnPlayAnim(playerid,"SUNBATHE","SBATHE_F_LieB2Sit",4.1,0,1,1,1,1);
		case 16: OnPlayAnim(playerid,"SUNBATHE","SBATHE_F_Out",4.1,0,1,1,1,1);
		case 17: OnPlayAnim(playerid,"SUNBATHE","SitnWait_in_W",4.1,0,1,1,1,1);
		case 18: OnPlayAnim(playerid,"SUNBATHE","SitnWait_out_W",4.1,0,1,1,1,1);
		default: SendHelpMessage(playerid, "/sunbathe [1-18]");
	}
	return 1;
}

CMD:lowrider(playerid, params[])
{
	new type;

	if (!IsPlayerInAnyVehicle(playerid)) return SendClientMessage(playerid, COLOR_WHITE, "Ban can phai ngoi trong xe.");
	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/lowrider [1-31]");

	switch(type)
	{
		case 1:  ApplyAnimation(playerid,"LOWRIDER","lrgirl_bdbnce",4.1,0,1,1,1,1,1);
		case 2:  ApplyAnimation(playerid,"LOWRIDER","lrgirl_hair",4.1,0,1,1,1,1,1);
		case 3:  ApplyAnimation(playerid,"LOWRIDER","lrgirl_hurry",4.1,0,1,1,1,1,1);
		case 4:  ApplyAnimation(playerid,"LOWRIDER","lrgirl_idleloop",4.1,0,1,1,1,1,1);
		case 5:  ApplyAnimation(playerid,"LOWRIDER","lrgirl_idle_to_l0",4.1,0,1,1,1,1,1);
		case 6:  ApplyAnimation(playerid,"LOWRIDER","lrgirl_l0_bnce",4.1,0,1,1,1,1,1);
		case 7:  ApplyAnimation(playerid,"LOWRIDER","lrgirl_l0_loop",4.1,0,1,1,1,1,1);
		case 8:  ApplyAnimation(playerid,"LOWRIDER","lrgirl_l0_to_l1",4.1,0,1,1,1,1,1);
		case 9:  ApplyAnimation(playerid,"LOWRIDER","lrgirl_l12_to_l0",4.1,0,1,1,1,1,1);
		case 10: ApplyAnimation(playerid,"LOWRIDER","lrgirl_l1_bnce",4.1,0,1,1,1,1,1);
		case 11: ApplyAnimation(playerid,"LOWRIDER","lrgirl_l1_loop",4.1,0,1,1,1,1,1);
		case 12: ApplyAnimation(playerid,"LOWRIDER","lrgirl_l1_to_l2",4.1,0,1,1,1,1,1);
		case 13: ApplyAnimation(playerid,"LOWRIDER","lrgirl_l2_bnce",4.1,0,1,1,1,1,1);
		case 14: ApplyAnimation(playerid,"LOWRIDER","lrgirl_l2_loop",4.1,0,1,1,1,1,1);
		case 15: ApplyAnimation(playerid,"LOWRIDER","lrgirl_l2_to_l3",4.1,0,1,1,1,1,1);
		case 16: ApplyAnimation(playerid,"LOWRIDER","lrgirl_l345_to_l1",4.1,0,1,1,1,1,1);
		case 17: ApplyAnimation(playerid,"LOWRIDER","lrgirl_l3_bnce",4.1,0,1,1,1,1,1);
		case 18: ApplyAnimation(playerid,"LOWRIDER","lrgirl_l3_loop",4.1,0,1,1,1,1,1);
		case 19: ApplyAnimation(playerid,"LOWRIDER","lrgirl_l3_to_l4",4.1,0,1,1,1,1,1);
		case 20: ApplyAnimation(playerid,"LOWRIDER","lrgirl_l4_bnce",4.1,0,1,1,1,1,1);
		case 21: ApplyAnimation(playerid,"LOWRIDER","lrgirl_l4_loop",4.1,0,1,1,1,1,1);
		case 22: ApplyAnimation(playerid,"LOWRIDER","lrgirl_l4_to_l5",4.1,0,1,1,1,1,1);
		case 23: ApplyAnimation(playerid,"LOWRIDER","lrgirl_l5_bnce",4.1,0,1,1,1,1,1);
		case 24: ApplyAnimation(playerid,"LOWRIDER","lrgirl_l5_loop",4.1,0,1,1,1,1,1);
		case 25: ApplyAnimation(playerid,"LOWRIDER","prtial_gngtlkB",4.1,0,1,1,1,1,1);
		case 26: ApplyAnimation(playerid,"LOWRIDER","prtial_gngtlkC",4.1,0,1,1,1,1,1);
		case 27: ApplyAnimation(playerid,"LOWRIDER","prtial_gngtlkD",4.1,0,1,1,1,1,1);
		case 28: ApplyAnimation(playerid,"LOWRIDER","prtial_gngtlkE",4.1,0,1,1,1,1,1);
		case 29: ApplyAnimation(playerid,"LOWRIDER","prtial_gngtlkF",4.1,0,1,1,1,1,1);
		case 30: ApplyAnimation(playerid,"LOWRIDER","prtial_gngtlkG",4.1,0,1,1,1,1,1);
		case 31: ApplyAnimation(playerid,"LOWRIDER","prtial_gngtlkH",4.1,0,1,1,1,1,1);
		default: SendHelpMessage(playerid, "/lowrider [1-31]");
	}
	return 1;
}

CMD:hitchhike(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/hitchhike [1-2]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"MISC","Hiker_Pose",4.1,0,1,1,1,1);
		case 2: OnPlayAnim(playerid,"MISC","Hiker_Pose_L",4.1,0,1,1,1,1);
		default: SendHelpMessage(playerid, "/hitchhike [1-2]");
	}
	return 1;
}

CMD:sitonchair(playerid, params[])
{
	new type;

	if (sscanf(params,"d",type)) return SendHelpMessage(playerid, "/sitonchair [1-9]");

	switch(type)
	{
		case 1: OnPlayAnim(playerid,"Attractors","Stepsit_in",4.0,0,0,0,1,0);
		case 2: OnPlayAnim(playerid,"CRIB","PED_Console_Loop",4.0,1,0,0,0,0);
		case 3: OnPlayAnim(playerid,"INT_HOUSE","LOU_In",4.0,0,0,0,1,1);
		case 4: OnPlayAnim(playerid,"MISC","SEAT_LR",4.0,1,0,0,0,0);
		case 5: OnPlayAnim(playerid,"MISC","Seat_talk_01",4.0,1,0,0,0,0);
		case 6: OnPlayAnim(playerid,"MISC","Seat_talk_02",4.0,1,0,0,0,0);
		case 7: OnPlayAnim(playerid,"ped","SEAT_down",4.0,0,0,0,1,1);
		case 8: OnPlayAnim(playerid,"JST_BUISNESS","girl_02",4.0,1,0,0,1,1);
		case 9: OnPlayAnim(playerid,"MISC","SEAT_watch",4.0,1,0,0,1,1);
		default: SendHelpMessage(playerid, "/sitonchair [1-9]");
	}
	return 1;
}

CMD:cashier(playerid, params[])
{
	return OnPlayAnim(playerid,"INT_SHOP","shop_cashier",4.0,1,0,0,0,0);
}

CMD:nope(playerid, params[])
{
	return OnPlayAnim(playerid,"PED","endchat_02",4.0,0,0,0,0,0);
}

CMD:fuku(playerid, params[])
{
	return OnPlayAnim(playerid,"RIOT","RIOT_FUKU",3.8,0,0,0,0,0);
}

CMD:phonetalk(playerid, params[])
{
	return OnPlayAnim(playerid,"PED","phone_talk",3.8,1,0,0,0,0);
}

CMD:write(playerid, params[])
{
	return OnPlayAnim(playerid,"OTB","betslp_loop",4.0,0,0,0,0,0);
}

CMD:sneak(playerid, params[])
{
	return OnPlayAnim(playerid,"PED","Player_Sneak",4.1,1,1,1,1,1);
}