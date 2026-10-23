
stock ShowHelpCommand(playerid, listitem)
{
	if(!IsPlayerConnected(playerid)) return 0;

	switch(listitem) 
	{
		case 0:
		{
			SendClientMessageEx(playerid, 0x1CFF18FF, "* [CHUNG]{FFFFFF} /pay /writecheck /cashchecks /donggop /time [paycheck/jail] /mua /(check)id /music /choxemgiayphep");
			SendClientMessageEx(playerid, 0x1CFF18FF, "* [CHUNG]{FFFFFF} /apply /skill /stopani /kill /muaquanao /droplicense /calculate /doxang /car /seatbelt /checkbelt");
			SendClientMessageEx(playerid, 0x1CFF18FF, "* [CHUNG]{FFFFFF} /huybo /chapnhan /eject /usepot /usecrack /contract /dichvu /families /thamgiasukien /checkplant");
			SendClientMessageEx(playerid, 0x1CFF18FF, "* [CHUNG]{FFFFFF} /speedo /viewmotd /nextgift /pointtime /trangphuc /mywarrants");
		}
		case 1:
		{
			SendClientMessageEx(playerid, 0x7068FFFF, "* [TAI KHOAN]{FFFFFF} /quydinh /hoidap /(net)stats /tuido /quickstats /sungcuatoi ");
			SendClientMessageEx(playerid, 0x7068FFFF, "* [TAI KHOAN]{FFFFFF} /nangcap /doimatkhau /xoamuctieu /resetupgrades(100k)");
		}
		case 2:
		{
			SendClientMessageEx(playerid, 0x7068FFFF, "* [CHAT]{FFFFFF} /w(hisper) /o(oc) /s(hout) /l(ow) /b /ad(vertisement)s /f(amily) /me");
			SendClientMessageEx(playerid, 0x7068FFFF, "* [CHAT]{FFFFFF} /togooc /tognews /togfam /togwhisper /do /huycuocgoi");
		}
		case 3:
		{
			SendClientMessageEx(playerid, 0x1CFF18FF, "* [BANK]{FFFFFF} /taikhoan /ruttien /guitien /chuyentien /ataikhoan /aruttien /aguitien /achuyentien");
		}
		case 4:
		{
			SendClientMessageEx(playerid, 0xFFF717FF,"* [SHOP] /shophelp /credits");
		}
		case 5:
		{
			new iGroupID = PlayerInfo[playerid][pMember];
			if(iGroupID == INVALID_GROUP_ID && PlayerInfo[playerid][pFMember] == INVALID_FAMILY_ID)
				return SendClientMessage(playerid, COLOR_GREY, "Ban khong o trong Faction/Family nao.");

			if (PlayerInfo[playerid][pFMember] != INVALID_FAMILY_ID)
			{
				if(PlayerInfo[playerid][pRank] < 5)
				{
					SendClientMessageEx(playerid, 0xFF66F7FF, "* [FAMILY]{FFFFFF} (/f)amily /clothes /safehelp /movegate /taginfo");
				}
				else
				{
					SendClientMessageEx(playerid, 0xFF66F7FF, "* [FAMILY]{FFFFFF} (/f)amily /clothes /safehelp /movegate /fdeposit /fwithdraw /fbalance /claim /chiemdong");
					SendClientMessageEx(playerid, 0xFF66F7FF, "* [FAMILY]{FFFFFF} /dvtrackcar /dvpark /adjustdvrank /spraytag /fmotd /adjust, /adjustdiv");
				}

				return 1;
			}

			switch(arrGroupData[iGroupID][g_iGroupType])
			{
				case 1:
				{
					SendClientMessageEx(playerid, COLOR_WHITE, "* [%s] (/r)adio /dept (/m)egaphone (/su)spect /tudo /mdc /dualenxe /dandi /arrest /warrantarrest /wanted /cuff /tazer", arrGroupData[PlayerInfo[playerid][pMember]][g_szGroupName]);
					SendClientMessageEx(playerid, COLOR_WHITE, "* [%s] /ticket (/gov)ernment /clothes /ram /invite /giverank /deploy /destroy /pddoor /pdcell", arrGroupData[PlayerInfo[playerid][pMember]][g_szGroupName]);
					SendClientMessageEx(playerid, COLOR_WHITE, "* [%s] /spikes /tichthubanglai /vcheck /vmdc /vticket /tow /untow /impound /dmvrelease /gdonate /togradio /togdept", arrGroupData[PlayerInfo[playerid][pMember]][g_szGroupName]);
					SendClientMessageEx(playerid, COLOR_WHITE, "* [%s] /flares /cones /wants /docarrest /siren /destroyplant /radargun /searchcar /dvsiren /vradar %s", arrGroupData[PlayerInfo[playerid][pMember]][g_szGroupName], PlayerInfo[playerid][pRank] >= arrGroupData[PlayerInfo[playerid][pMember]][g_iBugAccess] ? ("/bug /listbugs /clearbugs /hfind") : (""));
					if(arrGroupData[PlayerInfo[playerid][pMember]][g_iCrateIsland] != INVALID_RANK) {
						SendClientMessageEx(playerid, COLOR_WHITE, "* [%s] /cratelimit /viewcrateorders", arrGroupData[PlayerInfo[playerid][pMember]][g_szGroupName]);
					}
					SendClientMessageEx(playerid, COLOR_WHITE, "* [%s] /loadkit /usekit /backup (code2) /backupall /calls /a(ccept)c(all) /i(gnore)c(all)", arrGroupData[PlayerInfo[playerid][pMember]][g_szGroupName]);
					SendClientMessageEx(playerid, COLOR_WHITE, "* [%s] /capcccd /lucsoatcopxe /truyduoi /lucsoat /tichthu /kiemtrahang", arrGroupData[PlayerInfo[playerid][pMember]][g_szGroupName]);
				}	
				case 2:
				{
					SendClientMessageEx(playerid, COLOR_WHITE, "* [%s] (/f)amily /r /hopdong /givemehit /order /ranks /profile /h(show)badge /hfind /togbr", arrGroupData[PlayerInfo[playerid][pMember]][g_szGroupName]);
					SendClientMessageEx(playerid, COLOR_WHITE, "* [%s] /plantbomb /plantcarbomb /pickupbomb /myc4 /clothes /invite /giverank /showmehq /showmehq2 /showmehq3", arrGroupData[PlayerInfo[playerid][pMember]][g_szGroupName]);
				}
				case 3:
				{
					SendClientMessageEx(playerid, COLOR_WHITE, "* [%s] (/r)adio /dept (/m)egaphone /heal /clothes /invite /giverank /tudo /gdonate", arrGroupData[PlayerInfo[playerid][pMember]][g_szGroupName]);
					SendClientMessageEx(playerid, COLOR_WHITE, "* [%s] /getpt /movept /loadpt /deliverpt /destroyplant /calls /a(ccept)c(all) /i(gnore)c(all)", arrGroupData[PlayerInfo[playerid][pMember]][g_szGroupName]);
				}
				case 4:
				{
					SendClientMessageEx(playerid, COLOR_WHITE, "* [NEWS AGENCY] /live /news [text] /broadcast /cameraangle /clothes /invite /giverank /liveban");
				}
				case 5:
				{
					if(PlayerInfo[playerid][pRank] < 3)
					{
						SendClientMessageEx(playerid, COLOR_WHITE, "* [GOVERNMENT] (/r)adio /dept /tudo /mdc /deploy /destroy /spikes /tazer /lucsoat /cuff");
					}
					else
					{
						SendClientMessageEx(playerid, COLOR_WHITE, "* [GOVERNMENT] (/r)adio /dept /tudo /settax /checktax /taxwithdraw /invite /giverank (/gov)ernment (/su)spect");
						SendClientMessageEx(playerid, COLOR_WHITE, "* [GOVERNMENT] /mdc /dualenxe /arrest /wanted /cuff /tazer /lucsoat /take /ticket /clothes /ram /invite /giverank /setbudget");
						SendClientMessageEx(playerid, COLOR_WHITE, "* [GOVERNMENT] /spikes /destroyplant /radargun /warrantarrest /pardon /commute /wants /deploy /destroy");
					}
				}
				case 6:
				{
					SendClientMessageEx(playerid, COLOR_WHITE, "* [JUDICIAL SYSTEM] (/r)adio /dept /warrant /warrantwd /judgefine /judgejail /judgeprison /probation /wants /subpoena");
					SendClientMessageEx(playerid, COLOR_WHITE, "* [JUDICIAL SYSTEM] /invite /uninvite /giverank /trial /adjourn /sentence /reward /checkjudgements /reversejudgement");
					SendClientMessageEx(playerid, COLOR_WHITE, "* [JUDICIAL SYSTEM] /present /freezebank /freezeassets /probation /gdonate");
				}
				case 7:
				{
					SendClientMessageEx(playerid, COLOR_WHITE, "* [TRANSPORT] /fare /ataxi /tonline /r /invite /giverank /eba /gdonate");
				}
				case 8:
				{
					SendClientMessageEx(playerid, COLOR_WHITE, "* [MECHANIC] /badge /g (OOC Chat) /tudo /tow /untow /gdonate /suaxe /chetaodungcu /dungcu");
				}
				case 9:
				{
					SendClientMessageEx(playerid, COLOR_WHITE, "* [URL] /countdown");
				}
			}

			if (0 <= PlayerInfo[playerid][pLeader] < MAX_GROUPS)
			{
				SendClientMessageEx(playerid, COLOR_WHITE, "* [GROUP LEADER] /invite /uninvite /ouninvite /setdiv /giverank /online");
				if(arrGroupData[iGroupID][g_iGroupType] == 1 || arrGroupData[iGroupID][g_iGroupType] == 3 || arrGroupData[iGroupID][g_iGroupType] == 6 || arrGroupData[iGroupID][g_iGroupType] == 7)
				{
					SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* [GROUP LEADER] /viewbudget /grepocars /gvbuyback /gdonate /ordercrates /dvtrackcar /gwithdraw, /dvstorage");
				}
				else if(arrGroupData[iGroupID][g_iGroupType] == 5)
				{
					SendClientMessageEx(playerid, COLOR_LIGHTBLUE, "* [GROUP LEADER] /checkapps /deport");
				}
			}
		}
		case 6:
		{
			if(PlayerInfo[playerid][pDonateRank] == 0)
				return SendClientMessage(playerid, COLOR_GREY, "Ban khong phai la VIP Member.");

			if(PlayerInfo[playerid][pDonateRank] >= 1)
			{
				SendClientMessageEx(playerid, COLOR_PURPLE, "* [VIP] /viplocker /khoevip /tokenhelp /buddyinvite /phoneprivacy /setautoreply");
			}

			if(PlayerInfo[playerid][pDonateRank] == 5)
			{
				SendClientMessageEx(playerid, COLOR_PURPLE, "* [VIP Moderator] /vipparty /vto /vtoreset /vmute /vsuspend");
			}
		}
		case 7:
		{
			PC_EmulateCommand(playerid, "/fishhelp");
		}
	}
	return 1;
}

stock ShowJobHelp(playerid, jobid)
{
	if(jobid <= 0 || jobid >= MAX_JOB_TYPE) return 0;

	new dialogTitle[64];
	format(dialogTitle, _, "Job Help: %s", GetJobName(jobid));
	ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, dialogTitle, JobHelp[jobid], "Dong", "");
	return 1;
}

CMD:cmds(playerid, params[])
{
	ShowPlayerDialog(playerid, DIALOG_HELPCMD, DIALOG_STYLE_LIST, "Danh sach cac cau lenh", "\
		Chung\nTai khoan\nTro chuyen\nNgan hang\nKP5 Shop\nTo chuc/Family\n{C2A2DA}VIP{FFFFFF}\n\
		Cau ca", "Chon", "Dong");
	return 1;
}

alias:help("trogiup")
CMD:help(playerid, params[])
{
	SendClientMessage(playerid, 0x20FF42FF, "================== Tro Giup ==================");
	SendClientMessage(playerid, 0xFFFFFFFF, "Su dung {FFDB65}\"/cmds\"{FFFFFF} xem danh sach cac cau lenh trong may chu.");
	SendClientMessage(playerid, 0xFFFFFFFF, "Su dung {FFDB65}\"/newb\"{FFFFFF} de dat cau hoi tro Nguoi tro giup.");
	SendClientMessage(playerid, 0xFFFFFFFF, "Su dung {FFDB65}\"/yeucautrogiup\"{FFFFFF} de yeu cau su tro giup cua Advisor.");
	SendClientMessage(playerid, 0xFFFFFFFF, "Su dung {FFDB65}\"/baocao\"{FFFFFF} de bao cao nguoi choi hoac van de khac.");
	SendClientMessage(playerid, 0x20FF42FF, "============================================");
	/*
	SendClientMessageEx(playerid, COLOR_WHITE,"*** OTHER *** /trogiupdienthoai /carhelp /househelp /toyhelp /renthelp /jobhelp /animhelp /fishhelp /insurehelp");
	SendClientMessageEx(playerid, COLOR_WHITE,"*** OTHER *** /mailhelp /businesshelp /voucherhelp");
    */
	return 1;
}

CMD:jobhelp(playerid, params[])
{
	if(isnull(params)) {
		SendHelpMessage(playerid, "/jobhelp [id]");
		SendClientMessage(playerid, COLOR_GRAD2, "[1] Cong viec chinh: %s", GetJobName(PlayerInfo[playerid][pJob]));
		SendClientMessage(playerid, COLOR_GRAD2, "[2] Cong viec phu: %s", GetJobName(PlayerInfo[playerid][pJob2]));
		return 1;
	}

	new id = strval(params);
	if(id < 1 || id > 2)
		return SendClientMessage(playerid, COLOR_GREY, "ID khong hop le.");

	if(id == 1)
	{
		if(PlayerInfo[playerid][pJob] == JOB_NONE)
			return SendClientMessage(playerid, COLOR_GREY, "Ban khong co cong viec chinh.");

		ShowJobHelp(playerid, PlayerInfo[playerid][pJob]);
	}
	else if(id == 2)
	{
		if(PlayerInfo[playerid][pJob2] == JOB_NONE)
			return SendClientMessage(playerid, COLOR_GREY, "Ban khong co cong viec phu.");

		ShowJobHelp(playerid, PlayerInfo[playerid][pJob2]);
	}

	return 1;
}