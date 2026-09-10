#define MAX_TDN 4
#define TDN_POS_X_INFO 4.00000
#define TDN_POS_X 358.000000
#define TDN_POS_Y 7.000000
#define TDN_FONT 1
#define TDN_LETTER_SIZE_X 0.250000
#define TDN_LETTER_SIZE_Y 1.000000
#define TDN_SIZE 112.000000
#define TDN_COLOR 0xFFFFFFFF
#define TDN_COLOR_BOX 1296911871
#define TDN_PROPORTIONAL 1
#define TDN_DISTANCE 5.0
#define TDN_MODE_DOWN
#define TDN_TIME 10000
#define MAX_TDN_TEXT 800

#if !defined NOTIFICATION_ERROR
    #define NOTIFICATION_ERROR 0
    #define NOTIFICATION_INFO 1
    #define NOTIFICATION_SUCCESS 2
    #define NOTIFICATION_SYNTAX 3
    #define NOTIFICATION_WARNING 4
#endif


#define X_NOTIFICATION - 50.000

#define MAX_TDN  4

#define TINGGI_BOX  1.7
#include <td-string-width>
 
enum InformationTDN
{
    notifyUse,
    notifyLine,
    notifyText[MAX_TDN_TEXT],
    PlayerText:notifyTextDraw,
    PlayerText:textdraw_notification[5],
    Float:notifyMinPosY,
    Float:notifyMaxPosY,
    notifyHide,
    notifymode,
    notifyTime,
    Float:notifyWidth,
    bool:notifyGanjil
}
new TextDrawsNotification[MAX_PLAYERS][MAX_TDN][InformationTDN],
    notifycounter[MAX_PLAYERS];

timer TimerHideTDN[2000](playerid)
{
    for(new cycle; cycle < MAX_TDN; cycle++)
    {
        if(TextDrawsNotification[playerid][cycle][notifyHide] == -1)
        {
            TextDrawsNotification[playerid][cycle][notifyUse] = 0;
            if(TextDrawsNotification[playerid][cycle][notifyTextDraw] != PlayerText:-1)
            {
                PlayerTextDrawDestroys(playerid, TextDrawsNotification[playerid][cycle][notifyTextDraw]);
                TextDrawsNotification[playerid][cycle][notifyLine] = 0;
                TextDrawsNotification[playerid][cycle][notifyText][0] = EOS;
                TextDrawsNotification[playerid][cycle][notifyMinPosY] = 0;
                TextDrawsNotification[playerid][cycle][notifyMaxPosY] = 0;
                TextDrawsNotification[playerid][cycle][notifyTextDraw] = PlayerText:-1;
                TextDrawsNotification[playerid][cycle][notifyTime] = 100;
                forex(txd, 5)
                {
                    PlayerTextDrawDestroys(playerid, TextDrawsNotification[playerid][cycle][textdraw_notification][txd]);
                    TextDrawsNotification[playerid][cycle][textdraw_notification][txd] = PlayerText:-1;
                }
            }
            TextDrawsNotification[playerid][cycle][notifyHide] = -1;
            UpdateTDN(playerid);

            return 1;
        }
    }
    return 0;
}


JavaTR:ShowTDN_Manual(playerid, type, const reason[])
{
    for(new cycle; cycle < MAX_TDN; cycle++)
    {
        if(!TextDrawsNotification[playerid][cycle][notifyUse])
        {
            TextDrawsNotification[playerid][cycle][notifyText][0] = EOS;

            new text[MAX_TDN_TEXT];

            for(new len = strlen(reason), pos; pos < len; pos ++)
            {
                switch(reason[pos])
                {
                    default:  text[pos] = reason[pos];
                }
            }

            strcat(TextDrawsNotification[playerid][cycle][notifyText], text, MAX_TDN_TEXT);
            TextDrawsNotification[playerid][cycle][notifymode] = type;
 
            TextDrawsNotification[playerid][cycle][notifyUse] = 1;
 
            LinesTDN(playerid, cycle);

            #if defined TDN_MODE_DOWN

            MinPosYTDN(playerid, cycle);
            MaxPosYTDN(playerid, cycle);

            #endif

            #if defined TDN_MODE_UP

            MaxPosYTDN(playerid, cycle);
            MinPosYTDN(playerid, cycle);
            
            #endif

            CreateTDN(playerid, type, cycle);

            for(new i; i < MAX_TDN; i++)
            {
                if(used(playerid, notifycounter[playerid]))
                {
                    if(notifycounter[playerid] == MAX_TDN - 1) notifycounter[playerid] = 0;
                    else notifycounter[playerid]++;
                }
                else break;
            }

            new TDN = notifycounter[playerid];

            TextDrawsNotification[playerid][cycle][notifyHide] = TDN;

            if(notifycounter[playerid] == MAX_TDN - 1) notifycounter[playerid] = 0;
            else notifycounter[playerid]++;

            return TDN;
        }
    }
    return -1;
}

stock used(playerid, id)
{
    for(new cycle; cycle < MAX_TDN; cycle++)
    {
        if(TextDrawsNotification[playerid][cycle][notifyHide] == id) return 1;
    }
    return 0;
}

JavaTR:hideTDN(playerid, TDN)
{
    for(new cycle; cycle < MAX_TDN; cycle++)
    {
        if(TextDrawsNotification[playerid][cycle][notifyHide] == TDN)
        {
            TextDrawsNotification[playerid][cycle][notifyUse] = 0;
            if(TextDrawsNotification[playerid][cycle][notifyTextDraw] != PlayerText:-1)
            {
                PlayerTextDrawDestroys(playerid, TextDrawsNotification[playerid][cycle][notifyTextDraw]);
                TextDrawsNotification[playerid][cycle][notifyLine] = 0;
                TextDrawsNotification[playerid][cycle][notifyText][0] = EOS;
                TextDrawsNotification[playerid][cycle][notifyMinPosY] = 0;
                TextDrawsNotification[playerid][cycle][notifyMaxPosY] = 0;
                TextDrawsNotification[playerid][cycle][notifyTextDraw] = PlayerText:-1;
                TextDrawsNotification[playerid][cycle][notifyTime] = 100;
                forex(txd, 5)
                {
                    PlayerTextDrawDestroys(playerid, TextDrawsNotification[playerid][cycle][textdraw_notification][txd]);
                    TextDrawsNotification[playerid][cycle][textdraw_notification][txd] = PlayerText:-1;
                }
            }
            TextDrawsNotification[playerid][cycle][notifyHide] = -1;
            UpdateTDN(playerid);
            return 1;
        }
    }
    return 0;
}

stock UpdateTDN(playerid)
{
    for(new cycle = 0; cycle < MAX_TDN; cycle ++)
    {
        if(!TextDrawsNotification[playerid][cycle][notifyUse])
        {
            if(cycle != MAX_TDN - 1)
            {
                if(TextDrawsNotification[playerid][cycle + 1][notifyUse])
                {
                    TextDrawsNotification[playerid][cycle][notifyUse] = TextDrawsNotification[playerid][cycle + 1][notifyUse];
                    TextDrawsNotification[playerid][cycle][notifyLine] = TextDrawsNotification[playerid][cycle + 1][notifyLine];
                    strcat(TextDrawsNotification[playerid][cycle][notifyText], TextDrawsNotification[playerid][cycle + 1][notifyText], MAX_TDN_TEXT);
                    TextDrawsNotification[playerid][cycle][notifymode] = TextDrawsNotification[playerid][cycle + 1][notifymode];
                    TextDrawsNotification[playerid][cycle][notifyTextDraw] = TextDrawsNotification[playerid][cycle + 1][notifyTextDraw];
                    TextDrawsNotification[playerid][cycle][notifyHide] = TextDrawsNotification[playerid][cycle + 1][notifyHide];
                    TextDrawsNotification[playerid][cycle][notifyTime] = TextDrawsNotification[playerid][cycle + 1][notifyTime];

                    TextDrawsNotification[playerid][cycle + 1][notifyUse] = 0;
                    TextDrawsNotification[playerid][cycle + 1][notifyLine] = 0;
                    TextDrawsNotification[playerid][cycle + 1][notifyText][0] = EOS;
                    TextDrawsNotification[playerid][cycle + 1][notifyTextDraw] = PlayerText:-1;
                    TextDrawsNotification[playerid][cycle + 1][notifyMinPosY] = 0;
                    TextDrawsNotification[playerid][cycle + 1][notifyMaxPosY] = 0;
                    TextDrawsNotification[playerid][cycle + 1][notifyHide] = -1;
                    TextDrawsNotification[playerid][cycle + 1][notifymode] = -1;
                    TextDrawsNotification[playerid][cycle + 1][notifyTime] = 100;

                    forex(txd, 5)
                    {
                        TextDrawsNotification[playerid][cycle][textdraw_notification][txd] = TextDrawsNotification[playerid][cycle + 1][textdraw_notification][txd];
                        TextDrawsNotification[playerid][cycle + 1][textdraw_notification][txd] = PlayerText:-1;
                    }

                    #if defined TDN_MODE_DOWN

                    MinPosYTDN(playerid, cycle);
                    MaxPosYTDN(playerid, cycle);
                    
                    #endif

                    #if defined TDN_MODE_UP
                    
                    MaxPosYTDN(playerid, cycle);
                    MinPosYTDN(playerid, cycle);
                    
                    #endif       
                }
            }
        }
        else if(TextDrawsNotification[playerid][cycle][notifyUse])
        {
            if(cycle != 0)
            {
                if(!TextDrawsNotification[playerid][cycle - 1][notifyUse])
                {
                    TextDrawsNotification[playerid][cycle - 1][notifyUse] = TextDrawsNotification[playerid][cycle][notifyUse];
                    TextDrawsNotification[playerid][cycle - 1][notifyLine] = TextDrawsNotification[playerid][cycle][notifyLine];
                    strcat(TextDrawsNotification[playerid][cycle - 1][notifyText], TextDrawsNotification[playerid][cycle][notifyText], MAX_TDN_TEXT);
                    TextDrawsNotification[playerid][cycle - 1][notifymode] = TextDrawsNotification[playerid][cycle][notifymode];
                    TextDrawsNotification[playerid][cycle - 1][notifyTextDraw] = TextDrawsNotification[playerid][cycle][notifyTextDraw];
                    TextDrawsNotification[playerid][cycle - 1][notifyHide] = TextDrawsNotification[playerid][cycle][notifyHide];
                    TextDrawsNotification[playerid][cycle - 1][notifyTime] = TextDrawsNotification[playerid][cycle][notifyTime];

                    TextDrawsNotification[playerid][cycle][notifyUse] = 0;
                    TextDrawsNotification[playerid][cycle][notifyLine] = 0;
                    TextDrawsNotification[playerid][cycle][notifyText][0] = EOS;
                    TextDrawsNotification[playerid][cycle][notifymode] = -1;
                    TextDrawsNotification[playerid][cycle][notifyTextDraw] = PlayerText:-1;
                    TextDrawsNotification[playerid][cycle][notifyMinPosY] = 0;
                    TextDrawsNotification[playerid][cycle][notifyMaxPosY] = 0;
                    TextDrawsNotification[playerid][cycle][notifyHide] = -1;
                    TextDrawsNotification[playerid][cycle][notifyTime] = 100;

                    forex(txd, 5)
                    {
                        TextDrawsNotification[playerid][cycle - 1][textdraw_notification][txd] = TextDrawsNotification[playerid][cycle][textdraw_notification][txd];
                        TextDrawsNotification[playerid][cycle][textdraw_notification][txd] = PlayerText:-1;
                    }

                    #if defined TDN_MODE_DOWN

                    MinPosYTDN(playerid, cycle - 1);
                    MaxPosYTDN(playerid, cycle - 1);
                    
                    #endif

                    #if defined TDN_MODE_UP
                    
                    MaxPosYTDN(playerid, cycle - 1);
                    MinPosYTDN(playerid, cycle - 1);
                    
                    #endif
                }
            }
        }
        CreateTDN(playerid, TextDrawsNotification[playerid][cycle][notifymode], cycle);
    }
    return 1;
}

stock MinPosYTDN(playerid, TDN)
{
    #if defined TDN_MODE_DOWN

    if(TDN == 0)
    {
        TextDrawsNotification[playerid][TDN][notifyMinPosY] = TDN_POS_Y;
    }
    else
    {
        TextDrawsNotification[playerid][TDN][notifyMinPosY] = TextDrawsNotification[playerid][TDN - 1][notifyMaxPosY] + TDN_DISTANCE;
    }
    return 1;

    #endif

    #if defined TDN_MODE_UP

    TextDrawsNotification[playerid][TDN][notifyMinPosY] = TextDrawsNotification[playerid][TDN][notifyMaxPosY] - ((TDN_LETTER_SIZE_Y * 2) + 2) - ((TDN_LETTER_SIZE_Y * 5.75) * TINGGI_BOX) - ((TINGGI_BOX - 1) * ((TDN_LETTER_SIZE_Y * 2) + 2 + TDN_LETTER_SIZE_Y)) - (TDN_LETTER_SIZE_Y + 3);
    return 1;

    #endif
}

stock MaxPosYTDN(playerid, TDN)
{
    #if defined TDN_MODE_DOWN

    TextDrawsNotification[playerid][TDN][notifyMaxPosY] = TextDrawsNotification[playerid][TDN][notifyMinPosY] + (TDN_LETTER_SIZE_Y * 2) + 2 + (TDN_LETTER_SIZE_Y * 5.75 * TINGGI_BOX) + ((TINGGI_BOX - 1) * ((TDN_LETTER_SIZE_Y * 2) + 2 + TDN_LETTER_SIZE_Y)) + TDN_LETTER_SIZE_Y + 3;
    return 1;

    #endif

    #if defined TDN_MODE_UP

    if(TDN == 0)
    {
        TextDrawsNotification[playerid][TDN][notifyMaxPosY] = TDN_POS_Y;
    }
    else
    {
        TextDrawsNotification[playerid][TDN][notifyMaxPosY] = TextDrawsNotification[playerid][TDN - 1][notifyMinPosY] - TDN_DISTANCE;
    }
    return 1;

    #endif
}

stock LinesTDN(playerid, TDN)
{
    new lines = 1, Float:width, lastspace = -1, supr, len = strlen(TextDrawsNotification[playerid][TDN][notifyText]);
 
    for(new i = 0; i < len; i ++)
    {
        if(TextDrawsNotification[playerid][TDN][notifyText][i] == '~')
        {
            if(supr == 0)
            {
                supr = 1;
                if(TextDrawsNotification[playerid][TDN][notifyText][i+2] != '~') SendClientMessage(playerid, -1, "Error: '~' used incorrectly'");
            }
            else if(supr == 1) supr = 0;
        }
        else
        {
            if(supr == 1)
            {
                if(TextDrawsNotification[playerid][TDN][notifyText][i] == 'n')
                {
                    lines ++;
                    lastspace = -1;
                    width = 0;
                }
            }
            else
            {
                if(TextDrawsNotification[playerid][TDN][notifyText][i] == ' ') lastspace = i;
 
                width += TDN_LETTER_SIZE_X * GetTextDrawCharacterWidth(TextDrawsNotification[playerid][TDN][notifyText][i], TDN_FONT, bool:TDN_PROPORTIONAL);

                if(width > TDN_SIZE - 3)
                {
                    if(lastspace != i && lastspace != -1)
                    {
                        lines ++;
                        i = lastspace;
                        lastspace = -1;
                        width = 0;
                    }
                    else
                    {
                        lines ++;
                        lastspace = -1;
                        width = 0;
                    }
                }
            }
        }
    }
    
    TextDrawsNotification[playerid][TDN][notifyLine] = lines;
 
    return 1;
}

stock PlayerTextDrawDestroys(playerid, PlayerText:textid)
{
    if(textid != PlayerText:-1)
        PlayerTextDrawDestroy(playerid, PlayerText:textid);
    return 1;
}

stock CreateTDN(playerid, type, TDN)
{
    if(TextDrawsNotification[playerid][TDN][notifyUse] == 1)
    {
        TextDrawsNotification[playerid][TDN][notifyWidth] = 0.0;
        TextDrawsNotification[playerid][TDN][notifymode] = type;
        if(TextDrawsNotification[playerid][TDN][notifyTextDraw] != PlayerText:-1)
        {
            PlayerTextDrawDestroy(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw]);
            forex(txd, 5)
            {
                if(TextDrawsNotification[playerid][TDN][textdraw_notification][txd] != PlayerText:-1)
                    PlayerTextDrawDestroy(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][txd]);
            }
        }
        new lines = 1, supr, len = strlen(TextDrawsNotification[playerid][TDN][notifyText]);
 
        for(new i = 0; i < len; i ++)
        {
            if(TextDrawsNotification[playerid][TDN][notifyText][i] == '~')
            {
                if(supr == 0)
                {
                    supr = 1;
                    if(TextDrawsNotification[playerid][TDN][notifyText][i+2] != '~') SendClientMessage(playerid, -1, "Error: '~' used incorrectly'");
                }
                else if(supr == 1) supr = 0;
            }
            else
            {
                if(supr == 1)
                {
                    if(TextDrawsNotification[playerid][TDN][notifyText][i] == 'n')
                    {
                        lines ++;
                        TextDrawsNotification[playerid][TDN][notifyWidth] = 0;
                    }
                }
                else
                {
                    TextDrawsNotification[playerid][TDN][notifyWidth] += TDN_LETTER_SIZE_X * GetTextDrawCharacterWidth(TextDrawsNotification[playerid][TDN][notifyText][i], TDN_FONT, bool:TDN_PROPORTIONAL);
                }
            }
        }

        new row = TDN + 1;
        
        if(TextDrawsNotification[playerid][TDN][notifyWidth] < 200.0000)
        {
            if(TextDrawsNotification[playerid][TDN][notifyWidth] == 200)
            {
                TextDrawsNotification[playerid][TDN][notifyGanjil] = true;
            }
            else
            {
                if((row % 2) == 0) TextDrawsNotification[playerid][TDN][notifyGanjil] = false;
                else TextDrawsNotification[playerid][TDN][notifyGanjil] = true;
            }

            TextDrawsNotification[playerid][TDN][notifyWidth] += 10.0000;
        }
        else
        {
            TextDrawsNotification[playerid][TDN][notifyGanjil] = true;
        }

        new color;

        switch(type)
        {
            case NOTIFICATION_ERROR:
            {
                color = -1052226817;
            }
            case NOTIFICATION_SUCCESS:
            {
                color = 1018393087;
            }
            case NOTIFICATION_WARNING:
            {
                color = -626712321;
            }
            case NOTIFICATION_SYNTAX:
            {
                color = -2139062017;
            }
            case NOTIFICATION_INFO:
            {
                color = 1097458175;
            }
        }
        if(TextDrawsNotification[playerid][TDN][notifyGanjil])
        {
            if(TDN > 1)
            {
                new Float:kurangi;
                if(!TextDrawsNotification[playerid][TDN - 1][notifyGanjil])
                {
                    kurangi += 30.0000;
                }
                if(!TextDrawsNotification[playerid][TDN - 2][notifyGanjil])
                {
                    kurangi += 30.0000;
                }
                TextDrawsNotification[playerid][TDN][notifyMinPosY] = TextDrawsNotification[playerid][TDN][notifyMinPosY] - kurangi;

                TextDrawsNotification[playerid][TDN][textdraw_notification][0] = CreatePlayerTextDraw(playerid, 342.000000, TextDrawsNotification[playerid][TDN][notifyMinPosY] - (TDN_POS_Y - -1.000000), "ld_beat:chit");
                PlayerTextDrawFont(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 4);
                PlayerTextDrawLetterSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 0.600000, 2.000000);
                PlayerTextDrawTextSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 3.000000, 23.000000);
                PlayerTextDrawSetOutline(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 1);
                PlayerTextDrawSetShadow(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 0);
                PlayerTextDrawAlignment(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 1);
                PlayerTextDrawColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], color);
                PlayerTextDrawBackgroundColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 255);
                PlayerTextDrawBoxColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 50);
                PlayerTextDrawUseBox(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 0);
                PlayerTextDrawSetProportional(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 1);
                PlayerTextDrawSetSelectable(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 0);

                TextDrawsNotification[playerid][TDN][textdraw_notification][1] = CreatePlayerTextDraw(playerid, 343.000000, TextDrawsNotification[playerid][TDN][notifyMinPosY] - (TDN_POS_Y - 3.000000), "ld_bum:blkdot");
                PlayerTextDrawFont(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 4);
                PlayerTextDrawLetterSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 0.600000, 2.000000);
                PlayerTextDrawTextSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], (TextDrawsNotification[playerid][TDN][notifyWidth] / 1.8) + 3.0, 15.000000);
                PlayerTextDrawSetOutline(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 1);
                PlayerTextDrawSetShadow(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 0);
                PlayerTextDrawAlignment(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 1);
                PlayerTextDrawColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], color);
                PlayerTextDrawBackgroundColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 572732415);
                PlayerTextDrawBoxColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 50);
                PlayerTextDrawUseBox(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 0);
                PlayerTextDrawSetProportional(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 1);
                PlayerTextDrawSetSelectable(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 0);

                TextDrawsNotification[playerid][TDN][textdraw_notification][2] = CreatePlayerTextDraw(playerid, 344.000000, TextDrawsNotification[playerid][TDN][notifyMinPosY] - (TDN_POS_Y - 3.000000), "ld_bum:blkdot");
                PlayerTextDrawFont(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 4);
                PlayerTextDrawLetterSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 0.600000, 2.000000);
                PlayerTextDrawTextSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], TextDrawsNotification[playerid][TDN][notifyWidth] / 1.8, 15.000000);
                PlayerTextDrawSetOutline(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 1);
                PlayerTextDrawSetShadow(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 0);
                PlayerTextDrawAlignment(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 1);
                PlayerTextDrawColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 255);
                PlayerTextDrawBackgroundColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 572732415);
                PlayerTextDrawBoxColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 50);
                PlayerTextDrawUseBox(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 0);
                PlayerTextDrawSetProportional(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 1);
                PlayerTextDrawSetSelectable(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 0);

                TextDrawsNotification[playerid][TDN][textdraw_notification][3] = CreatePlayerTextDraw(playerid, 345.000000, TextDrawsNotification[playerid][TDN][notifyMinPosY] - (TDN_POS_Y - 4.000000), "ld_beat:chit");
                PlayerTextDrawFont(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 4);
                PlayerTextDrawLetterSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 0.600000, 2.000000);
                PlayerTextDrawTextSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 11.000000, 13.500000);
                PlayerTextDrawSetOutline(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 1);
                PlayerTextDrawSetShadow(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 0);
                PlayerTextDrawAlignment(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 1);
                PlayerTextDrawColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], color);
                PlayerTextDrawBackgroundColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 255);
                PlayerTextDrawBoxColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 50);
                PlayerTextDrawUseBox(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 0);
                PlayerTextDrawSetProportional(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 1);
                PlayerTextDrawSetSelectable(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 0);

                TextDrawsNotification[playerid][TDN][textdraw_notification][4] = CreatePlayerTextDraw(playerid, 349.000000, TextDrawsNotification[playerid][TDN][notifyMinPosY] - (TDN_POS_Y - 6.000000), "i");
                PlayerTextDrawFont(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 0);
                PlayerTextDrawLetterSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 0.254166, 0.900000);
                PlayerTextDrawTextSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 400.000000, 17.000000);
                PlayerTextDrawSetOutline(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 0);
                PlayerTextDrawSetShadow(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 0);
                PlayerTextDrawAlignment(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 1);
                PlayerTextDrawColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 255);
                PlayerTextDrawBackgroundColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 255);
                PlayerTextDrawBoxColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 50);
                PlayerTextDrawUseBox(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 0);
                PlayerTextDrawSetProportional(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 1);
                PlayerTextDrawSetSelectable(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 0);

                TextDrawsNotification[playerid][TDN][notifyTextDraw] = CreatePlayerTextDraw(playerid, 358.000000, TextDrawsNotification[playerid][TDN][notifyMinPosY], TextDrawsNotification[playerid][TDN][notifyText]);
                PlayerTextDrawFont(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 1);
                PlayerTextDrawLetterSize(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 0.120833, 0.700000);
                PlayerTextDrawTextSize(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 675.000000, 17.000000);
                PlayerTextDrawSetOutline(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 0);
                PlayerTextDrawSetShadow(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 0);
                PlayerTextDrawAlignment(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 1);
                PlayerTextDrawColor(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], -1);
                PlayerTextDrawBackgroundColor(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 255);
                PlayerTextDrawBoxColor(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 50);
                PlayerTextDrawUseBox(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 0);
                PlayerTextDrawSetProportional(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 1);
                PlayerTextDrawSetSelectable(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 0);
            }
            else
            {
                TextDrawsNotification[playerid][TDN][textdraw_notification][0] = CreatePlayerTextDraw(playerid, 342.000000, TextDrawsNotification[playerid][TDN][notifyMinPosY] - (TDN_POS_Y - -1.000000), "ld_beat:chit");
                PlayerTextDrawFont(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 4);
                PlayerTextDrawLetterSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 0.600000, 2.000000);
                PlayerTextDrawTextSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 3.000000, 23.000000);
                PlayerTextDrawSetOutline(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 1);
                PlayerTextDrawSetShadow(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 0);
                PlayerTextDrawAlignment(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 1);
                PlayerTextDrawColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], color);
                PlayerTextDrawBackgroundColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 255);
                PlayerTextDrawBoxColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 50);
                PlayerTextDrawUseBox(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 0);
                PlayerTextDrawSetProportional(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 1);
                PlayerTextDrawSetSelectable(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 0);

                TextDrawsNotification[playerid][TDN][textdraw_notification][1] = CreatePlayerTextDraw(playerid, 343.000000, TextDrawsNotification[playerid][TDN][notifyMinPosY] - (TDN_POS_Y - 3.000000), "ld_bum:blkdot");
                PlayerTextDrawFont(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 4);
                PlayerTextDrawLetterSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 0.600000, 2.000000);
                PlayerTextDrawTextSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], (TextDrawsNotification[playerid][TDN][notifyWidth] / 1.8) + 3.0, 15.000000);
                PlayerTextDrawSetOutline(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 1);
                PlayerTextDrawSetShadow(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 0);
                PlayerTextDrawAlignment(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 1);
                PlayerTextDrawColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], color);
                PlayerTextDrawBackgroundColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 572732415);
                PlayerTextDrawBoxColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 50);
                PlayerTextDrawUseBox(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 0);
                PlayerTextDrawSetProportional(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 1);
                PlayerTextDrawSetSelectable(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 0);

                TextDrawsNotification[playerid][TDN][textdraw_notification][2] = CreatePlayerTextDraw(playerid, 344.000000, TextDrawsNotification[playerid][TDN][notifyMinPosY] - (TDN_POS_Y - 3.000000), "ld_bum:blkdot");
                PlayerTextDrawFont(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 4);
                PlayerTextDrawLetterSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 0.600000, 2.000000);
                PlayerTextDrawTextSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], TextDrawsNotification[playerid][TDN][notifyWidth] / 1.8, 15.000000);
                PlayerTextDrawSetOutline(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 1);
                PlayerTextDrawSetShadow(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 0);
                PlayerTextDrawAlignment(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 1);
                PlayerTextDrawColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 255);
                PlayerTextDrawBackgroundColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 572732415);
                PlayerTextDrawBoxColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 50);
                PlayerTextDrawUseBox(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 0);
                PlayerTextDrawSetProportional(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 1);
                PlayerTextDrawSetSelectable(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 0);

                TextDrawsNotification[playerid][TDN][textdraw_notification][3] = CreatePlayerTextDraw(playerid, 345.000000, TextDrawsNotification[playerid][TDN][notifyMinPosY] - (TDN_POS_Y - 4.000000), "ld_beat:chit");
                PlayerTextDrawFont(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 4);
                PlayerTextDrawLetterSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 0.600000, 2.000000);
                PlayerTextDrawTextSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 11.000000, 13.500000);
                PlayerTextDrawSetOutline(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 1);
                PlayerTextDrawSetShadow(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 0);
                PlayerTextDrawAlignment(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 1);
                PlayerTextDrawColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], color);
                PlayerTextDrawBackgroundColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 255);
                PlayerTextDrawBoxColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 50);
                PlayerTextDrawUseBox(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 0);
                PlayerTextDrawSetProportional(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 1);
                PlayerTextDrawSetSelectable(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 0);

                TextDrawsNotification[playerid][TDN][textdraw_notification][4] = CreatePlayerTextDraw(playerid, 349.000000, TextDrawsNotification[playerid][TDN][notifyMinPosY] - (TDN_POS_Y - 6.000000), "i");
                PlayerTextDrawFont(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 0);
                PlayerTextDrawLetterSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 0.254166, 0.900000);
                PlayerTextDrawTextSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 400.000000, 17.000000);
                PlayerTextDrawSetOutline(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 0);
                PlayerTextDrawSetShadow(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 0);
                PlayerTextDrawAlignment(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 1);
                PlayerTextDrawColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 255);
                PlayerTextDrawBackgroundColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 255);
                PlayerTextDrawBoxColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 50);
                PlayerTextDrawUseBox(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 0);
                PlayerTextDrawSetProportional(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 1);
                PlayerTextDrawSetSelectable(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 0);

                TextDrawsNotification[playerid][TDN][notifyTextDraw] = CreatePlayerTextDraw(playerid, 358.000000, TextDrawsNotification[playerid][TDN][notifyMinPosY], TextDrawsNotification[playerid][TDN][notifyText]);
                PlayerTextDrawFont(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 1);
                PlayerTextDrawLetterSize(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 0.120833, 0.700000);
                PlayerTextDrawTextSize(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 675.000000, 17.000000);
                PlayerTextDrawSetOutline(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 0);
                PlayerTextDrawSetShadow(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 0);
                PlayerTextDrawAlignment(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 1);
                PlayerTextDrawColor(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], -1);
                PlayerTextDrawBackgroundColor(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 255);
                PlayerTextDrawBoxColor(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 50);
                PlayerTextDrawUseBox(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 0);
                PlayerTextDrawSetProportional(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 1);
                PlayerTextDrawSetSelectable(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 0);
            }
        }
        else
        {
            //
            TextDrawsNotification[playerid][TDN][textdraw_notification][0] = CreatePlayerTextDraw(playerid, 342.000000 + ((TextDrawsNotification[playerid][TDN - 1][notifyWidth] / 1.8) + 5.0000), TextDrawsNotification[playerid][TDN - 1][notifyMinPosY] - (TDN_POS_Y - -1.000000), "ld_beat:chit");
            PlayerTextDrawFont(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 4);
            PlayerTextDrawLetterSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 0.600000, 2.000000);
            PlayerTextDrawTextSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 3.000000, 23.000000);
            PlayerTextDrawSetOutline(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 1);
            PlayerTextDrawSetShadow(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 0);
            PlayerTextDrawAlignment(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 1);
            PlayerTextDrawColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], color);
            PlayerTextDrawBackgroundColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 255);
            PlayerTextDrawBoxColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 50);
            PlayerTextDrawUseBox(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 1);
            PlayerTextDrawSetProportional(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 1);
            PlayerTextDrawSetSelectable(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][0], 0);

            TextDrawsNotification[playerid][TDN][textdraw_notification][1] = CreatePlayerTextDraw(playerid, 343.000000 + ((TextDrawsNotification[playerid][TDN - 1][notifyWidth] / 1.8) + 5.0000), TextDrawsNotification[playerid][TDN - 1][notifyMinPosY] - (TDN_POS_Y - 3.000000), "ld_bum:blkdot");
            PlayerTextDrawFont(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 4);
            PlayerTextDrawLetterSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 0.600000, 2.000000);
            PlayerTextDrawTextSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], (TextDrawsNotification[playerid][TDN][notifyWidth] / 1.8) + 3.0, 15.000000);
            PlayerTextDrawSetOutline(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 1);
            PlayerTextDrawSetShadow(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 0);
            PlayerTextDrawAlignment(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 1);
            PlayerTextDrawColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], color);
            PlayerTextDrawBackgroundColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 572732415);
            PlayerTextDrawBoxColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 50);
            PlayerTextDrawUseBox(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 1);
            PlayerTextDrawSetProportional(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 1);
            PlayerTextDrawSetSelectable(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][1], 0);

            TextDrawsNotification[playerid][TDN][textdraw_notification][2] = CreatePlayerTextDraw(playerid, 344.000000 + ((TextDrawsNotification[playerid][TDN - 1][notifyWidth] / 1.8) + 5.0000), TextDrawsNotification[playerid][TDN - 1][notifyMinPosY] - (TDN_POS_Y - 3.000000), "ld_bum:blkdot");
            PlayerTextDrawFont(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 4);
            PlayerTextDrawLetterSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 0.600000, 2.000000);
            PlayerTextDrawTextSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], TextDrawsNotification[playerid][TDN][notifyWidth] / 1.8, 15.000000);
            PlayerTextDrawSetOutline(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 1);
            PlayerTextDrawSetShadow(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 0);
            PlayerTextDrawAlignment(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 1);
            PlayerTextDrawColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 255);
            PlayerTextDrawBackgroundColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 572732415);
            PlayerTextDrawBoxColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 50);
            PlayerTextDrawUseBox(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 1);
            PlayerTextDrawSetProportional(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 1);
            PlayerTextDrawSetSelectable(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][2], 0);

            TextDrawsNotification[playerid][TDN][textdraw_notification][3] = CreatePlayerTextDraw(playerid, 345.000000 + ((TextDrawsNotification[playerid][TDN - 1][notifyWidth] / 1.8) + 5.0000), TextDrawsNotification[playerid][TDN - 1][notifyMinPosY] - (TDN_POS_Y - 4.000000), "ld_beat:chit");
            PlayerTextDrawFont(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 4);
            PlayerTextDrawLetterSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 0.600000, 2.000000);
            PlayerTextDrawTextSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 11.000000, 13.500000);
            PlayerTextDrawSetOutline(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 1);
            PlayerTextDrawSetShadow(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 0);
            PlayerTextDrawAlignment(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 1);
            PlayerTextDrawColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], color);
            PlayerTextDrawBackgroundColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 255);
            PlayerTextDrawBoxColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 50);
            PlayerTextDrawUseBox(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 1);
            PlayerTextDrawSetProportional(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 1);
            PlayerTextDrawSetSelectable(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][3], 0);

            TextDrawsNotification[playerid][TDN][textdraw_notification][4] = CreatePlayerTextDraw(playerid, 349.000000 + ((TextDrawsNotification[playerid][TDN - 1][notifyWidth] / 1.8) + 5.0000), TextDrawsNotification[playerid][TDN - 1][notifyMinPosY] - (TDN_POS_Y - 6.000000), "i");
            PlayerTextDrawFont(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 0);
            PlayerTextDrawLetterSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 0.254166, 0.900000);
            PlayerTextDrawTextSize(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 400.000000, 17.000000);
            PlayerTextDrawSetOutline(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 0);
            PlayerTextDrawSetShadow(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 0);
            PlayerTextDrawAlignment(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 1);
            PlayerTextDrawColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 255);
            PlayerTextDrawBackgroundColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 255);
            PlayerTextDrawBoxColor(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 50);
            PlayerTextDrawUseBox(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 0);
            PlayerTextDrawSetProportional(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 1);
            PlayerTextDrawSetSelectable(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][4], 0);

            TextDrawsNotification[playerid][TDN][notifyTextDraw] = CreatePlayerTextDraw(playerid, 358.000000 + ((TextDrawsNotification[playerid][TDN - 1][notifyWidth] / 1.8) + 5.0000), TextDrawsNotification[playerid][TDN - 1][notifyMinPosY], TextDrawsNotification[playerid][TDN][notifyText]);
            PlayerTextDrawFont(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 1);
            PlayerTextDrawLetterSize(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 0.120833, 0.700000);
            PlayerTextDrawTextSize(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 675.000000, 17.000000);
            PlayerTextDrawSetOutline(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 0);
            PlayerTextDrawSetShadow(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 0);
            PlayerTextDrawAlignment(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 1);
            PlayerTextDrawColor(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], -1);
            PlayerTextDrawBackgroundColor(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 255);
            PlayerTextDrawBoxColor(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 50);
            PlayerTextDrawUseBox(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 0);
            PlayerTextDrawSetProportional(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 1);
            PlayerTextDrawSetSelectable(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw], 0);
        }

        forex(txd, 5)
        {
            PlayerTextDrawShow(playerid, TextDrawsNotification[playerid][TDN][textdraw_notification][txd]);
        }
        PlayerTextDrawShow(playerid, TextDrawsNotification[playerid][TDN][notifyTextDraw]);
    }
    return 1;
}


#include <YSI\YSI_Coding\y_hooks>
    enum e_notify
    {
        notifyIcon,
        notifyMessage[320],
        notifySize
    }
    new NotifInfo[MAX_PLAYERS][4][e_notify];
    new MaxPlayerNotify[MAX_PLAYERS];
    new PlayerText: notifyTextdraws[MAX_PLAYERS][4*8];
    new NotifIndex[MAX_PLAYERS];

    forward NotifyHide(playerid);
    public NotifyHide(playerid)
    {
        if(!NotifIndex[playerid]) return 1;
        -- NotifIndex[playerid];
        MaxPlayerNotify[playerid] --;
        for (new i = -1; ++ i < 8;) PlayerTextDrawDestroy(playerid, notifyTextdraws[playerid][(NotifIndex[playerid] * 8) + i]);
        return 1;
    }


ShowTDN(playerid, notifid, text[])
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

/*stock ShowTDN(playerid, notifid, pesan[])
{
    switch(notifid)
    {
        case NOTIFICATION_ERROR:
		{
            if(MaxPlayerNotify[playerid] >= 4) return false;
			PlayerPlaySound(playerid, 1085, 0, 0, 0);
			ShowNotify(playerid, pesan, 2);
		}
		case NOTIFICATION_INFO:
		{
            if(MaxPlayerNotify[playerid] >= 4) return false;
			PlayerPlaySound(playerid, 1139, 0, 0, 0);
			ShowNotify(playerid, pesan, 1);
		}
		case NOTIFICATION_SUCCESS:
		{
            if(MaxPlayerNotify[playerid] >= 4) return false;
			PlayerPlaySound(playerid, 5203, 0, 0, 0);
			ShowNotify(playerid, pesan, 3);
		}
		case NOTIFICATION_SYNTAX:
		{
            if(MaxPlayerNotify[playerid] >= 4) return false;
			PlayerPlaySound(playerid, 5201, 0, 0, 0);
			ShowNotify(playerid, pesan, 4);
		}
		case NOTIFICATION_WARNING:
		{
            if(MaxPlayerNotify[playerid] >= 4) return false;
			PlayerPlaySound(playerid, 4203, 0, 0, 0);
			ShowNotify(playerid, pesan, 5);
		}
    }
    return 1;
}*/

stock ShowNotify(playerid, message[], icon)
{
    if(MaxPlayerNotify[playerid] >= 4) return 1;
    MaxPlayerNotify[playerid] ++;
    for (new x = -1; ++ x < NotifIndex[playerid] && x < 4;)
    {
        for (new i = -1; ++ i < 8;) PlayerTextDrawDestroy(playerid, notifyTextdraws[playerid][(x * 8) + i]);
        NotifInfo[playerid][NotifIndex[playerid] - x] = NotifInfo[playerid][(NotifIndex[playerid] - x) - 1];
    }
    format(NotifInfo[playerid][0][notifyMessage], 320, "%s", message);
    NotifInfo[playerid][0][notifyIcon] = icon;
    NotifInfo[playerid][0][notifySize] = 3;

    ++ NotifIndex[playerid];
    new Float: new_x = 0.0;
    for (new x = -1; ++ x < NotifIndex[playerid];)
    {
        CreateNotify(playerid, x, x * 8, new_x);
        new_x += (NotifInfo[playerid][x][notifySize] * 7.25) + 20.0;
    }
    SetTimerEx("NotifyHide", 6500, false, "d", playerid);
    return 1;
}

CreateNotify(playerid, index, i, Float:new_x)
{
    new lines = NotifInfo[playerid][index][notifySize];
    new Float:x = (lines * 8) + new_x;
    new Float:newpos = x - 20.0;
    if(NotifInfo[playerid][index][notifyIcon] == 1)
    {
        notifyTextdraws[playerid][i] = CreatePlayerTextDraw(playerid, 500.000, 103.000+newpos, "LD_BUM:blkdot");
        PlayerTextDrawTextSize(playerid, notifyTextdraws[playerid][i], 113.000, 35.000);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], 1097458175);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 255);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 4);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++i] = CreatePlayerTextDraw(playerid, 500.000, 107.000+newpos, "LD_BUM:blkdot");
        PlayerTextDrawTextSize(playerid, notifyTextdraws[playerid][i], 113.000, 35.000);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], 943210495);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 255);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 4);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++i] = CreatePlayerTextDraw(playerid, 502.000, 107.000+newpos, "LD_BEAT:chit");
        PlayerTextDrawTextSize(playerid, notifyTextdraws[playerid][i], 17.000, 17.000);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], -1);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 255);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 4);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++i] = CreatePlayerTextDraw(playerid, 503.000, 108.000+newpos, "LD_BEAT:chit");
        PlayerTextDrawTextSize(playerid, notifyTextdraws[playerid][i], 15.000, 15.000);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], 1097458175);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 255);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 4);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++i] = CreatePlayerTextDraw(playerid, 509.000, 109.000+newpos, "i");
        PlayerTextDrawLetterSize(playerid, notifyTextdraws[playerid][i], 0.310, 1.299);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], -1);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 150);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++i] = CreatePlayerTextDraw(playerid, 518.000, 110.000+newpos, "THONG TIN");
        PlayerTextDrawLetterSize(playerid, notifyTextdraws[playerid][i], 0.200, 1.099);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], 1097458175);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 150);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++i] = CreatePlayerTextDraw(playerid, 506.000, 121.000+newpos, NotifInfo[playerid][index][notifyMessage]);
        PlayerTextDrawLetterSize(playerid, notifyTextdraws[playerid][i], 0.133, 0.949);
        PlayerTextDrawTextSize(playerid, notifyTextdraws[playerid][i], 610.000, 21.000);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], -1);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 255);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);
    }
    if(NotifInfo[playerid][index][notifyIcon] == 2)
    {
        notifyTextdraws[playerid][i] = CreatePlayerTextDraw(playerid, 500.000, 103.000+newpos, "LD_BUM:blkdot");
        PlayerTextDrawTextSize(playerid, notifyTextdraws[playerid][i], 113.000, 35.000);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], -1052226817);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 255);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 4);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++i] = CreatePlayerTextDraw(playerid, 500.000, 107.000+newpos, "LD_BUM:blkdot");
        PlayerTextDrawTextSize(playerid, notifyTextdraws[playerid][i], 113.000, 35.000);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], 943210495);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 255);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 4);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++i] = CreatePlayerTextDraw(playerid, 506.000, 109.000+newpos, "LD_BUM:blkdot");
        PlayerTextDrawTextSize(playerid, notifyTextdraws[playerid][i], 20.000, 5.000);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], -1052226817);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 255);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 4);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++i] = CreatePlayerTextDraw(playerid, 506.000, 115.000+newpos, "LD_BUM:blkdot");
        PlayerTextDrawTextSize(playerid, notifyTextdraws[playerid][i], 20.000, 5.000);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], -1052226817);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 255);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 4);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++i] = CreatePlayerTextDraw(playerid, 525.000, 110.000+newpos, "LD_BUM:blkdot");
        PlayerTextDrawTextSize(playerid, notifyTextdraws[playerid][i], -5.000, 3.000);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], 943210495);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 255);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 4);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++i] = CreatePlayerTextDraw(playerid, 525.000, 116.000+newpos, "LD_BUM:blkdot");
        PlayerTextDrawTextSize(playerid, notifyTextdraws[playerid][i], -5.000, 3.000);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], 943210495);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 255);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 4);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++i] = CreatePlayerTextDraw(playerid, 528.000, 109.000+newpos, "LOI");
        PlayerTextDrawLetterSize(playerid, notifyTextdraws[playerid][i], 0.210, 1.099);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], -1052226817);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 150);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++i] = CreatePlayerTextDraw(playerid, 506.000, 121.000+newpos, NotifInfo[playerid][index][notifyMessage]);
        PlayerTextDrawLetterSize(playerid, notifyTextdraws[playerid][i], 0.133, 0.949);
        PlayerTextDrawTextSize(playerid, notifyTextdraws[playerid][i], 610.000, 21.000);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], -1);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 255);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);
    }
    if(NotifInfo[playerid][index][notifyIcon] == 3)
    {
        notifyTextdraws[playerid][i] = CreatePlayerTextDraw(playerid, 500.000, 103.000+newpos, "LD_BUM:blkdot");
        PlayerTextDrawTextSize(playerid, notifyTextdraws[playerid][i], 113.000, 35.000);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], 1018393087);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 255);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 4);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++ i] = CreatePlayerTextDraw(playerid, 500.000, 107.000+newpos, "LD_BUM:blkdot");
        PlayerTextDrawTextSize(playerid, notifyTextdraws[playerid][i], 113.000, 35.000);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], 943210495);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 255);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 4);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++ i] = CreatePlayerTextDraw(playerid, 502.000, 107.000+newpos, "LD_BEAT:chit");
        PlayerTextDrawTextSize(playerid, notifyTextdraws[playerid][i], 17.000, 17.000);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], -1);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 255);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 4);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++ i] = CreatePlayerTextDraw(playerid, 510.000, 111.000+newpos, "/");
        PlayerTextDrawLetterSize(playerid, notifyTextdraws[playerid][i], 0.230, 0.699);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], 16744447);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 150);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++ i] = CreatePlayerTextDraw(playerid, 512.000, 114.000+newpos, "/");
        PlayerTextDrawLetterSize(playerid, notifyTextdraws[playerid][i], -0.260, 0.499);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], 16744447);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 150);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++ i] = CreatePlayerTextDraw(playerid, 518.000, 110.000+newpos, "THANH CONG");
        PlayerTextDrawLetterSize(playerid, notifyTextdraws[playerid][i], 0.200, 1.099);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], 1018393087);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 150);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++ i] = CreatePlayerTextDraw(playerid, 506.000, 121.000+newpos, NotifInfo[playerid][index][notifyMessage]);
        PlayerTextDrawLetterSize(playerid, notifyTextdraws[playerid][i], 0.133, 0.949);
        PlayerTextDrawTextSize(playerid, notifyTextdraws[playerid][i], 610.000, 21.000);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], -1);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 255);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);
    }
    if(NotifInfo[playerid][index][notifyIcon] == 4)
    {
        notifyTextdraws[playerid][i] = CreatePlayerTextDraw(playerid, 500.000, 103.000+newpos, "LD_BUM:blkdot");
        PlayerTextDrawTextSize(playerid, notifyTextdraws[playerid][i], 113.000, 35.000);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], -2139062017);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 255);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 4);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++ i] = CreatePlayerTextDraw(playerid, 500.000, 107.000+newpos, "LD_BUM:blkdot");
        PlayerTextDrawTextSize(playerid, notifyTextdraws[playerid][i], 113.000, 35.000);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], 943210495);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 255);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 4);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++ i] = CreatePlayerTextDraw(playerid, 507.000, 108.000+newpos, "!");
        PlayerTextDrawLetterSize(playerid, notifyTextdraws[playerid][i], 0.329, 1.399);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], -2139062017);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 150);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++ i] = CreatePlayerTextDraw(playerid, 513.000, 110.000+newpos, "MEO");
        PlayerTextDrawLetterSize(playerid, notifyTextdraws[playerid][i], 0.159, 1.100);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], -2139062017);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 150);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++ i] = CreatePlayerTextDraw(playerid, 506.000, 121.000+newpos, NotifInfo[playerid][index][notifyMessage]);
        PlayerTextDrawLetterSize(playerid, notifyTextdraws[playerid][i], 0.133, 0.949);
        PlayerTextDrawTextSize(playerid, notifyTextdraws[playerid][i], 610.000, 21.000);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], -1);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 255);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);
    }
    if(NotifInfo[playerid][index][notifyIcon] == 5)
    {
        notifyTextdraws[playerid][i] = CreatePlayerTextDraw(playerid, 500.000, 103.000+newpos, "LD_BUM:blkdot");
        PlayerTextDrawTextSize(playerid, notifyTextdraws[playerid][i], 113.000, 35.000);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], -626712321);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 255);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 4);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++ i] = CreatePlayerTextDraw(playerid, 500.000, 107.000+newpos, "LD_BUM:blkdot");
        PlayerTextDrawTextSize(playerid, notifyTextdraws[playerid][i], 113.000, 35.000);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], 943210495);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 255);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 4);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++ i] = CreatePlayerTextDraw(playerid, 507.000, 108.000+newpos, "!");
        PlayerTextDrawLetterSize(playerid, notifyTextdraws[playerid][i], 0.329, 1.399);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], -626712321);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 150);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++ i] = CreatePlayerTextDraw(playerid, 513.000, 110.000+newpos, "CHU Y");
        PlayerTextDrawLetterSize(playerid, notifyTextdraws[playerid][i], 0.159, 1.100);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], -626712321);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 150);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);

        notifyTextdraws[playerid][++ i] = CreatePlayerTextDraw(playerid, 506.000, 121.000+newpos, NotifInfo[playerid][index][notifyMessage]);
        PlayerTextDrawLetterSize(playerid, notifyTextdraws[playerid][i], 0.133, 0.949);
        PlayerTextDrawTextSize(playerid, notifyTextdraws[playerid][i], 610.000, 21.000);
        PlayerTextDrawAlignment(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawColor(playerid, notifyTextdraws[playerid][i], -1);
        PlayerTextDrawSetShadow(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawSetOutline(playerid, notifyTextdraws[playerid][i], 0);
        PlayerTextDrawBackgroundColor(playerid, notifyTextdraws[playerid][i], 255);
        PlayerTextDrawFont(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawSetProportional(playerid, notifyTextdraws[playerid][i], 1);
        PlayerTextDrawShow(playerid, notifyTextdraws[playerid][i]);
    }
}

CMD:testnotif(playerid, params[])
{
    new choice;

	if(PlayerInfo[playerid][pAdmin] < 1)
		return SendClientMessageEx(playerid, COLOR_GRAD2, "Ban khong duoc phep su dung lenh nay.");

    if(strlen(params) > 0)
    {
        if(sscanf(params, "d", choice) || choice < 1 || choice > 5)
            return SendClientMessageEx(playerid, COLOR_GRAD2, "Su dung: /testnotif [1-5]. 1 INFO, 2 ERROR, 3 SUCCESS, 4 SYNTAX, 5 WARNING.");

        switch(choice)
        {
            case 1: ShowTDN(playerid, NOTIFICATION_INFO, "Test notification: INFO");
            case 2: ShowTDN(playerid, NOTIFICATION_ERROR, "Test notification: ERROR");
            case 3: ShowTDN(playerid, NOTIFICATION_SUCCESS, "Test notification: SUCCESS");
            case 4: ShowTDN(playerid, NOTIFICATION_SYNTAX, "Test notification: SYNTAX");
            case 5: ShowTDN(playerid, NOTIFICATION_WARNING, "Test notification: WARNING");
        }
        return 1;
    }

	ShowTDN(playerid, NOTIFICATION_INFO, "Test notification: INFO");
	ShowTDN(playerid, NOTIFICATION_ERROR, "Test notification: ERROR");
	ShowTDN(playerid, NOTIFICATION_SUCCESS, "Test notification: SUCCESS");
	ShowTDN(playerid, NOTIFICATION_SYNTAX, "Test notification: SYNTAX");
	ShowTDN(playerid, NOTIFICATION_WARNING, "Test notification: WARNING");
	return 1;
}