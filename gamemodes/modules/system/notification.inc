enum eNotify
{
	NotifyIcon,
	NotifyMessage[320],
	NotifySize
}

#define NOTIFICATION_INFO     (1)
#define NOTIFICATION_ERROR    (2)
#define NOTIFICATION_SUCCESS  (3)
#define NOTIFICATION_SYNTAX   (4)
#define NOTIFICATION_WARNING  (5)

new InfoNotify[MAX_PLAYERS][7][eNotify];
new MaxPlayerNotify[MAX_PLAYERS];
new NotifyTimer[MAX_PLAYERS][7];
new PlayerText:TextDrawNotifikasi[MAX_PLAYERS][7*10];
new PlayerText:TextDrawNotifikasiLoding[MAX_PLAYERS][7*10];
new IndexNotify[MAX_PLAYERS];
new LoadingNotif[MAX_PLAYERS];

forward HideNotify(playerid);
public HideNotify(playerid)
{
    if(!IndexNotify[playerid]) return 1;

    new oldIdx = IndexNotify[playerid] - 1; // cái cũ nhất nằm ở index cao nhất

    for(new i = -1; ++i < 10;)
    {
        if(TextDrawNotifikasi[playerid][(oldIdx*10) + i] != PlayerText:INVALID_PLAYER_TEXT_DRAW)
        {
            PlayerTextDrawDestroy(playerid, TextDrawNotifikasi[playerid][(oldIdx*10) + i]);
            TextDrawNotifikasi[playerid][(oldIdx*10) + i] = PlayerText:INVALID_PLAYER_TEXT_DRAW;
        }
        if(TextDrawNotifikasiLoding[playerid][(oldIdx*10) + i] != PlayerText:INVALID_PLAYER_TEXT_DRAW)
        {
            PlayerTextDrawDestroy(playerid, TextDrawNotifikasiLoding[playerid][(oldIdx*10) + i]);
            TextDrawNotifikasiLoding[playerid][(oldIdx*10) + i] = PlayerText:INVALID_PLAYER_TEXT_DRAW;
        }
    }

    NotifyTimer[playerid][oldIdx] = INVALID_TIMER;
    IndexNotify[playerid]--;
    if(MaxPlayerNotify[playerid] > 0) MaxPlayerNotify[playerid]--;
    return 1;
}

forward NotifyLoadingUpdet(playerid);
public NotifyLoadingUpdet(playerid)
{
    if(!IndexNotify[playerid]) return 1;
    for(new x = -1; ++x < IndexNotify[playerid];)
    {
        new Float:Value = LoadingNotif[playerid] * 109 / 110;
        new slot = (x * 10) + 2; // chỉ slot này chứa thanh loading
        if(TextDrawNotifikasiLoding[playerid][slot] != PlayerText:INVALID_PLAYER_TEXT_DRAW)
        {
            PlayerTextDrawTextSize(playerid, TextDrawNotifikasiLoding[playerid][slot], Value, -3.0);
            PlayerTextDrawShow(playerid, TextDrawNotifikasiLoding[playerid][slot]);
        }
    }
    return 1;
}

forward NotifyLoading(playerid);
public NotifyLoading(playerid)
{
    if(!IndexNotify[playerid]) return 1;
    LoadingNotif[playerid] -= 10;
    NotifyLoadingUpdet(playerid);
    if(LoadingNotif[playerid] <= 5) LoadingNotif[playerid] = 109;
    return 1;
}

stock ShowTDN(playerid, notifid, text[])
{
    switch(notifid)
    {
        case NOTIFICATION_ERROR:
        {
            PlayerPlaySound(playerid, 1085, 0, 0, 0);
            ShowNotify(playerid, text, 2);
        }
        case NOTIFICATION_INFO:
        {
            PlayerPlaySound(playerid, 1139, 0, 0, 0);
            ShowNotify(playerid, text, 1);
        }
        case NOTIFICATION_SUCCESS:
        {
            PlayerPlaySound(playerid, 5203, 0, 0, 0);
            ShowNotify(playerid, text, 3);
            LoadingNotif[playerid] = 109; // giữ hành vi cũ của SuccesMsg
        }
        case NOTIFICATION_SYNTAX:
        {
            PlayerPlaySound(playerid, 5201, 0, 0, 0);
            ShowNotify(playerid, text, 4);
        }
        case NOTIFICATION_WARNING:
        {
            PlayerPlaySound(playerid, 4203, 0, 0, 0);
            ShowNotify(playerid, text, 5);
        }
    }
    return 1;
}

stock InfoMsg(playerid, string[])
{
    ShowTDN(playerid, NOTIFICATION_INFO, string);
    return 1;
}

stock ErrorMsg(playerid, string[])
{
    ShowTDN(playerid, NOTIFICATION_ERROR, string);
    return 1;
}

stock SuccesMsg(playerid, string[])
{
    ShowTDN(playerid, NOTIFICATION_SUCCESS, string);
    return 1;
}

stock SyntaxMsg(playerid, string[])
{
    ShowTDN(playerid, NOTIFICATION_SYNTAX, string);
    return 1;
}

stock WarningMsg(playerid, string[])
{
    ShowTDN(playerid, NOTIFICATION_WARNING, string);
    return 1;
}

stock ShowNotify(const playerid, const string:message[], const icon)
{
    // Nếu đã có 2 cái -> xoá cái cũ nhất (FIFO)
    while(IndexNotify[playerid] >= 2)
    {
        new oldIdx = IndexNotify[playerid] - 1;

        // Huỷ timer của cái bị xoá (nếu còn pending)
        if(NotifyTimer[playerid][oldIdx] != INVALID_TIMER)
        {
            KillTimer(NotifyTimer[playerid][oldIdx]);
            NotifyTimer[playerid][oldIdx] = INVALID_TIMER;
        }

        for(new i = -1; ++i < 10;)
        {
            if(TextDrawNotifikasi[playerid][(oldIdx*10) + i] != PlayerText:INVALID_PLAYER_TEXT_DRAW)
            {
                PlayerTextDrawDestroy(playerid, TextDrawNotifikasi[playerid][(oldIdx*10) + i]);
                TextDrawNotifikasi[playerid][(oldIdx*10) + i] = PlayerText:INVALID_PLAYER_TEXT_DRAW;
            }
            if(TextDrawNotifikasiLoding[playerid][(oldIdx*10) + i] != PlayerText:INVALID_PLAYER_TEXT_DRAW)
            {
                PlayerTextDrawDestroy(playerid, TextDrawNotifikasiLoding[playerid][(oldIdx*10) + i]);
                TextDrawNotifikasiLoding[playerid][(oldIdx*10) + i] = PlayerText:INVALID_PLAYER_TEXT_DRAW;
            }
        }

        IndexNotify[playerid]--;
        if(MaxPlayerNotify[playerid] > 0) MaxPlayerNotify[playerid]--;
    }

    // Destroy các textdraw còn lại để vẽ lại vị trí mới
    for(new x = -1; ++x < IndexNotify[playerid];)
    {
        for(new i = -1; ++i < 10;)
        {
            if(TextDrawNotifikasi[playerid][(x*10) + i] != PlayerText:INVALID_PLAYER_TEXT_DRAW)
            {
                PlayerTextDrawDestroy(playerid, TextDrawNotifikasi[playerid][(x*10) + i]);
                TextDrawNotifikasi[playerid][(x*10) + i] = PlayerText:INVALID_PLAYER_TEXT_DRAW;
            }
            if(TextDrawNotifikasiLoding[playerid][(x*10) + i] != PlayerText:INVALID_PLAYER_TEXT_DRAW)
            {
                PlayerTextDrawDestroy(playerid, TextDrawNotifikasiLoding[playerid][(x*10) + i]);
                TextDrawNotifikasiLoding[playerid][(x*10) + i] = PlayerText:INVALID_PLAYER_TEXT_DRAW;
            }
        }
    }

    // Dịch dữ liệu cũ lên 1 index (nhường chỗ 0 cho cái mới)
    for(new x = IndexNotify[playerid]; x > 0; x--)
    {
        InfoNotify[playerid][x]  = InfoNotify[playerid][x-1];
        NotifyTimer[playerid][x] = NotifyTimer[playerid][x-1];
    }

    // Thêm cái mới vào index 0
    format(InfoNotify[playerid][0][NotifyMessage], 320, "%s", message);
    InfoNotify[playerid][0][NotifyIcon] = icon;
    InfoNotify[playerid][0][NotifySize] = 3;
    IndexNotify[playerid]++;
    MaxPlayerNotify[playerid]++;

    // Vẽ lại toàn bộ (index 0 ở dưới, index cao hơn ở trên)
    new Float:new_x = 0.0;
    for(new x = -1; ++x < IndexNotify[playerid];)
    {
        CreateNotify(playerid, x, x * 10, new_x);
        new_x += (InfoNotify[playerid][x][NotifySize]*7.25) + 22.0;
    }

    NotifyTimer[playerid][0] = SetTimerEx("HideNotify", 10000, false, "d", playerid);
    return 1;
}

stock CreateNotify(const playerid, index, i, const Float:new_x)
{
	new lines = InfoNotify[playerid][index][NotifySize];
	new Float:x = (lines * 10) + new_x;
	new Float:posisibaru = x - 1.0;
	if(InfoNotify[playerid][index][NotifyIcon] == 1)
	{
		TextDrawNotifikasi[playerid][i] = CreatePlayerTextDraw(playerid, 1000.000000, 108.500000+posisibaru, "");
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawLetterSize(playerid, TextDrawNotifikasi[playerid][i], 0.425000, 1.400000);
		PlayerTextDrawTextSize(playerid, TextDrawNotifikasi[playerid][i], 400.000000, 17.000000);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], -1);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 255);
		PlayerTextDrawBoxColor(playerid, TextDrawNotifikasi[playerid][i], 50);
		PlayerTextDrawUseBox(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawSetSelectable(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 520.000, 116.000+posisibaru, "LD_SPAC:white");
		PlayerTextDrawTextSize(playerid, TextDrawNotifikasi[playerid][i], 109.000, 35.000);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], 893010175);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 255);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 4);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 520.000, 116.000+posisibaru, "LD_SPAC:white");
		PlayerTextDrawTextSize(playerid, TextDrawNotifikasi[playerid][i], -2.000, 36.500);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], 1687547391);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 255);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 4);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 518.000, 116.000+posisibaru, "LD_BEAT:chit");
		PlayerTextDrawTextSize(playerid, TextDrawNotifikasi[playerid][i], 16.000, 16.000);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], -1);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 255);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 4);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 525.000, 119.000+posisibaru, "i");
		PlayerTextDrawLetterSize(playerid, TextDrawNotifikasi[playerid][i], 0.220, 1.099);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], 1687547391);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 150);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 533.000, 119.000+posisibaru, "THONG TIN");
		PlayerTextDrawLetterSize(playerid, TextDrawNotifikasi[playerid][i], 0.158, 1.098);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], -1);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 150);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 521.000, 129.000+posisibaru, InfoNotify[playerid][index][NotifyMessage]);
		PlayerTextDrawLetterSize(playerid, TextDrawNotifikasi[playerid][i], 0.150, 0.699);
		PlayerTextDrawTextSize(playerid, TextDrawNotifikasi[playerid][i], 633.500, 494.000);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], -1);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 255);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

	}
	if(InfoNotify[playerid][index][NotifyIcon] == 2)
	{
		TextDrawNotifikasi[playerid][i] = CreatePlayerTextDraw(playerid, 1000.000000, 108.500000+posisibaru, "");
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawLetterSize(playerid, TextDrawNotifikasi[playerid][i], 0.425000, 1.400000);
		PlayerTextDrawTextSize(playerid, TextDrawNotifikasi[playerid][i], 400.000000, 17.000000);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], -1);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 255);
		PlayerTextDrawBoxColor(playerid, TextDrawNotifikasi[playerid][i], 50);
		PlayerTextDrawUseBox(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawSetSelectable(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 520.000, 116.000+posisibaru, "LD_SPAC:white");
		PlayerTextDrawTextSize(playerid, TextDrawNotifikasi[playerid][i], 109.000, 35.000);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], 893010175);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 255);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 4);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 520.000, 116.000+posisibaru, "LD_SPAC:white");
		PlayerTextDrawTextSize(playerid, TextDrawNotifikasi[playerid][i], -2.000, 36.500);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], -1962934017);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 255);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 4);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 518.000, 116.000+posisibaru, "LD_BEAT:chit");
		PlayerTextDrawTextSize(playerid, TextDrawNotifikasi[playerid][i], 16.000, 16.000);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], -1);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 255);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 4);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 523.400, 119.000+posisibaru, "X");
		PlayerTextDrawLetterSize(playerid, TextDrawNotifikasi[playerid][i], 0.220, 1.099);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], -1962934017);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 150);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 533.000, 119.000+posisibaru, "LOI");
		PlayerTextDrawLetterSize(playerid, TextDrawNotifikasi[playerid][i], 0.158, 1.098);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], -1);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 150);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 521.000, 129.000+posisibaru, InfoNotify[playerid][index][NotifyMessage]);
		PlayerTextDrawLetterSize(playerid, TextDrawNotifikasi[playerid][i], 0.150, 0.699);
		PlayerTextDrawTextSize(playerid, TextDrawNotifikasi[playerid][i], 633.500, 494.000);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], -1);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 255);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

	}
	if(InfoNotify[playerid][index][NotifyIcon] == 3)
	{
		TextDrawNotifikasi[playerid][i] = CreatePlayerTextDraw(playerid, 1000.000000, 108.500000+posisibaru, "");
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawLetterSize(playerid, TextDrawNotifikasi[playerid][i], 0.425000, 1.400000);
		PlayerTextDrawTextSize(playerid, TextDrawNotifikasi[playerid][i], 400.000000, 17.000000);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], -1);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 255);
		PlayerTextDrawBoxColor(playerid, TextDrawNotifikasi[playerid][i], 50);
		PlayerTextDrawUseBox(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawSetSelectable(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 520.000, 116.000+posisibaru, "LD_SPAC:white");
		PlayerTextDrawTextSize(playerid, TextDrawNotifikasi[playerid][i], 109.000, 35.000);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], 893010175);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 255);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 4);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasiLoding[playerid][++i] = CreatePlayerTextDraw(playerid, 520.000, 116.000+posisibaru, "LD_SPAC:white");
		PlayerTextDrawTextSize(playerid, TextDrawNotifikasiLoding[playerid][i], -2.000, 36.500);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasiLoding[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasiLoding[playerid][i], 1689621759);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasiLoding[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasiLoding[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasiLoding[playerid][i], 255);
		PlayerTextDrawFont(playerid, TextDrawNotifikasiLoding[playerid][i], 4);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasiLoding[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasiLoding[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 518.000, 116.000+posisibaru, "LD_BEAT:chit");
		PlayerTextDrawTextSize(playerid, TextDrawNotifikasi[playerid][i], 16.000, 16.000);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], -1);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 255);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 4);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 525.000, 120.000+posisibaru, "/");
		PlayerTextDrawLetterSize(playerid, TextDrawNotifikasi[playerid][i], 0.200, 0.799);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], 16711935);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 150);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 526.000, 123.000+posisibaru, "/");
		PlayerTextDrawLetterSize(playerid, TextDrawNotifikasi[playerid][i], -0.136, 0.398);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], 16711935);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 150);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 533.000, 119.000+posisibaru, "THANH CONG");
		PlayerTextDrawLetterSize(playerid, TextDrawNotifikasi[playerid][i], 0.158, 1.098);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], -1);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 150);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 521.000, 129.000+posisibaru, InfoNotify[playerid][index][NotifyMessage]);
		PlayerTextDrawLetterSize(playerid, TextDrawNotifikasi[playerid][i], 0.150, 0.699);
		PlayerTextDrawTextSize(playerid, TextDrawNotifikasi[playerid][i], 633.500, 494.000);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], -1);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 255);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

	}
	if(InfoNotify[playerid][index][NotifyIcon] == 4)
	{
		TextDrawNotifikasi[playerid][i] = CreatePlayerTextDraw(playerid, 1000.000000, 108.500000+posisibaru, "");
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawLetterSize(playerid, TextDrawNotifikasi[playerid][i], 0.425000, 1.400000);
		PlayerTextDrawTextSize(playerid, TextDrawNotifikasi[playerid][i], 400.000000, 17.000000);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], -1);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 255);
		PlayerTextDrawBoxColor(playerid, TextDrawNotifikasi[playerid][i], 50);
		PlayerTextDrawUseBox(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawSetSelectable(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 520.000, 116.000+posisibaru, "LD_SPAC:white");
		PlayerTextDrawTextSize(playerid, TextDrawNotifikasi[playerid][i], 109.000, 35.000);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], 893010175);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 255);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 4);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 520.000, 116.000+posisibaru, "LD_SPAC:white");
		PlayerTextDrawTextSize(playerid, TextDrawNotifikasi[playerid][i], -2.000, 36.500);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], 1687547391);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 255);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 4);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 518.000, 116.000+posisibaru, "LD_BEAT:chit");
		PlayerTextDrawTextSize(playerid, TextDrawNotifikasi[playerid][i], 16.000, 16.000);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], -1);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 255);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 4);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 525.000, 119.000+posisibaru, "i");
		PlayerTextDrawLetterSize(playerid, TextDrawNotifikasi[playerid][i], 0.220, 1.099);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], 1687547391);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 150);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 533.000, 119.000+posisibaru, "HE THONG");
		PlayerTextDrawLetterSize(playerid, TextDrawNotifikasi[playerid][i], 0.158, 1.098);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], -1);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 150);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 521.000, 129.000+posisibaru, InfoNotify[playerid][index][NotifyMessage]);
		PlayerTextDrawLetterSize(playerid, TextDrawNotifikasi[playerid][i], 0.150, 0.699);
		PlayerTextDrawTextSize(playerid, TextDrawNotifikasi[playerid][i], 633.500, 494.000);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], -1);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 255);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

	}
	if(InfoNotify[playerid][index][NotifyIcon] == 5)
	{
		TextDrawNotifikasi[playerid][i] = CreatePlayerTextDraw(playerid, 1000.000000, 108.500000+posisibaru, "");
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawLetterSize(playerid, TextDrawNotifikasi[playerid][i], 0.425000, 1.400000);
		PlayerTextDrawTextSize(playerid, TextDrawNotifikasi[playerid][i], 400.000000, 17.000000);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], -1);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 255);
		PlayerTextDrawBoxColor(playerid, TextDrawNotifikasi[playerid][i], 50);
		PlayerTextDrawUseBox(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawSetSelectable(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 520.000, 116.000+posisibaru, "LD_SPAC:white");
		PlayerTextDrawTextSize(playerid, TextDrawNotifikasi[playerid][i], 109.000, 35.000);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], 893010175);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 255);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 4);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 520.000, 116.000+posisibaru, "LD_SPAC:white");
		PlayerTextDrawTextSize(playerid, TextDrawNotifikasi[playerid][i], -2.000, 36.500);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], -626712321);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 255);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 4);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 518.000, 116.000+posisibaru, "LD_BEAT:chit");
		PlayerTextDrawTextSize(playerid, TextDrawNotifikasi[playerid][i], 16.000, 16.000);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], -1);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 255);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 4);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 525.000, 119.000+posisibaru, "i");
		PlayerTextDrawLetterSize(playerid, TextDrawNotifikasi[playerid][i], 0.220, 1.099);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], -626712321);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 150);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 533.000, 119.000+posisibaru, "CHU Y");
		PlayerTextDrawLetterSize(playerid, TextDrawNotifikasi[playerid][i], 0.158, 1.098);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], -1);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 150);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

		TextDrawNotifikasi[playerid][++i] = CreatePlayerTextDraw(playerid, 521.000, 129.000+posisibaru, InfoNotify[playerid][index][NotifyMessage]);
		PlayerTextDrawLetterSize(playerid, TextDrawNotifikasi[playerid][i], 0.150, 0.699);
		PlayerTextDrawTextSize(playerid, TextDrawNotifikasi[playerid][i], 633.500, 494.000);
		PlayerTextDrawAlignment(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawColor(playerid, TextDrawNotifikasi[playerid][i], -1);
		PlayerTextDrawSetShadow(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawSetOutline(playerid, TextDrawNotifikasi[playerid][i], 0);
		PlayerTextDrawBackgroundColor(playerid, TextDrawNotifikasi[playerid][i], 255);
		PlayerTextDrawFont(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawSetProportional(playerid, TextDrawNotifikasi[playerid][i], 1);
		PlayerTextDrawShow(playerid, TextDrawNotifikasi[playerid][i]);

	}
	return true;
}

CMD:testnofi(playerid, params[])
{
    new type;
    if(sscanf(params, "i", type)) {
        SendClientMessageEx(playerid, COLOR_GREY, "USAGE: /testnofi [1-5]");
        SendClientMessageEx(playerid, COLOR_GREY, "1: Information | 2: Error | 3: Success | 4: Syntax | 5: Warning");
        return 1;
    }

    switch(type) {
        case 1: ShowTDN(playerid, NOTIFICATION_INFO,    "Day la thong bao Information");
        case 2: ShowTDN(playerid, NOTIFICATION_ERROR,   "Day la thong bao Error");
        case 3: ShowTDN(playerid, NOTIFICATION_SUCCESS, "Day la thong bao Success");
        case 4: ShowTDN(playerid, NOTIFICATION_SYNTAX,  "Day la thong bao Syntax");
        case 5: ShowTDN(playerid, NOTIFICATION_WARNING, "Day la thong bao Warning");
        default: SendClientMessageEx(playerid, COLOR_GREY, "USAGE: /testnofi [1-5]");
    }
    return 1;
}