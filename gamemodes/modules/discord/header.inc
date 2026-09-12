// Guild
new DCC_Guild:mainGuild;

// Channel
new DCC_Channel:dc_Verify;
new DCC_Channel:ccChannel;
new DCC_Channel:antiChannel;
new DCC_Channel:renameChannel;
new DCC_Channel:banChannel;
new DCC_Channel:ruathoChannel;
new DCC_Channel:creditChannel;
new DCC_Channel:logChannel;
new DCC_Channel:weaponChannel;
new DCC_Channel:logDCChannel;
new DCC_Channel:moneyChannel;
new DCC_Channel:givegunChannel;
new DCC_Channel:giveChannel;

// Role
new DCC_Role:dcRole_Verified;
new DCC_Role:dcRole_Police;
new DCC_Role:dcRole_Medic;
new DCC_Role:dcRole_Admin;

// Admin Log
new DCC_Channel:dclog_Money;
new DCC_Channel:dclog_Setvip;

// Player Log
new DCC_Channel:dclog_Join;
new DCC_Channel:dclog_Pay;

// Job Log
new DCC_Channel:dclog_Pizza;
new DCC_Channel:dclog_Sweeper;
new DCC_Channel:dclog_Porter;
new DCC_Channel:dclog_Trucker;
new DCC_Channel:dclog_Construction;
new DCC_Channel:dclog_CanSa;
new DCC_Channel:dclog_Fishing;
new DCC_Channel:dclog_Lumber;
new DCC_Channel:dclog_Painter;
new DCC_Channel:dclog_Taxi;


stock discord_messHack(const str[])
{
    new DCC_Embed:embed = DCC_CreateEmbed();
    DCC_SetEmbedTitle(embed, "[F4T] ANTI-CHEAT");
    DCC_SetEmbedDescription(embed, str);
    DCC_SetEmbedFooter(embed, "Copyright by TPhat");
    DCC_SetEmbedColor(embed, 0x00FF00);
    DCC_SendChannelEmbedMessage(antiChannel, embed);
	return 1;
}