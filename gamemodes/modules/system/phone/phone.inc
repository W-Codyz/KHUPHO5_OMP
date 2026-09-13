#include <open.mp>
#include <YSI\YSI_Coding\y_hooks>

#define PHONE_TEXTDRAW_COUNT    (82)

new PlayerText: Text_Player[MAX_PLAYERS][PHONE_TEXTDRAW_COUNT];
new bool:       PhoneBuilt[MAX_PLAYERS];
new bool:       PhoneOpen[MAX_PLAYERS];
new takingselfie[MAX_PLAYERS];
new Float:Degree[MAX_PLAYERS];
const Float: RadiusPhone = 1.4;
const Float: SpeedPhone  = 1.25;
const Float: HeightPhone = 1.0;
new Float:lX[MAX_PLAYERS];
new Float:lY[MAX_PLAYERS];
new Float:lZ[MAX_PLAYERS];

#define         DIALOG_DIENTHOAI        (5093)
#define         DIALOG_CALL             (5094)
#define         DIALOG_DANHBA           (5095)

#define                 DANHBA_1                (9100)
#define                 DANHBA_2                (9101)
#define                 DANHBA_3                (9102)
#define                 DANHBA_4                (9103)
#define                 DANHBA_5                (9104)
#define                 DANHBA_6                (9105)
#define                 DANHBA_7                (9106)
#define                 DANHBA_8                (9107)
#define                 DANHBA_9                (9108)

#define                 GOIDIEN_1               (9200)
#define                 NHANTIN_1               (9201)
#define                 EDITDB_1                (9202)
#define                 EDITDB_1_1              (9203)
#define                 GOIDIEN_2               (9204)
#define                 NHANTIN_2               (9205)
#define                 EDITDB_2                (9206)
#define                 EDITDB_2_2              (9207)
#define                 GOIDIEN_3               (9208)
#define                 NHANTIN_3               (9209)
#define                 EDITDB_3                (9210)
#define                 EDITDB_3_3              (9211)
#define                 GOIDIEN_4               (9212)
#define                 NHANTIN_4               (9213)
#define                 EDITDB_4                (9214)
#define                 EDITDB_4_4              (9215)
#define                 GOIDIEN_5               (9216)
#define                 NHANTIN_5               (9217)
#define                 EDITDB_5                (9218)
#define                 EDITDB_5_5              (9219)
#define                 GOIDIEN_6               (9220)
#define                 NHANTIN_6               (9221)
#define                 EDITDB_6                (9222)
#define                 EDITDB_6_6              (9223)
#define                 GOIDIEN_7               (9224)
#define                 NHANTIN_7               (9225)
#define                 EDITDB_7                (9226)
#define                 EDITDB_7_7              (9227)
#define                 GOIDIEN_8               (9228)
#define                 NHANTIN_8               (9229)
#define                 EDITDB_8                (9230)
#define                 EDITDB_8_8              (9231)
#define                 GOIDIEN_9               (9232)
#define                 NHANTIN_9               (9233)
#define                 EDITDB_9                (9234)
#define                 EDITDB_9_9              (9235)

#define                 DIENTHOAI_MP3           (9300)
#define                 DIALOG_SACOMBANK        (9400)
#define                 DIALOG_THEBANK          (9401)
#define                 DIALOG_CHUYENTIEN       (9402)
#define                 DIALOG_CHUYENTIEN1      (9403)
#define                 DIALOG_CHUYENTIEN2      (9404)
#define                 DIALOG_LIST_COPS        (9450)
#define                 DIALOG_LIST_MEDICS      (9451)
#define                 DIALOG_LIST_TAXI        (9452)
#define                 DIALOG_SMS_PHONE        (9453)
#define                 DIALOG_SMS_TEXT         (9454)

// ============================================================
//  Phone_Build — tạo 82 TextDraw (chỉ gọi khi mở phone)
// ============================================================
stock Phone_Build(playerid)
{
    if(PhoneBuilt[playerid]) return 1;

    Text_Player[playerid][0] = CreatePlayerTextDraw(playerid, 532.500, 344.329, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][0], 46.000, 52.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][0], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][0], -167773441);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][0], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][0], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][0], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][0], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][0], 1);

    Text_Player[playerid][1] = CreatePlayerTextDraw(playerid, 483.000, 140.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][1], 71.000, 247.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][1], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][1], -167773441);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][1], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][1], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][1], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][1], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][1], 1);

    Text_Player[playerid][2] = CreatePlayerTextDraw(playerid, 468.000, 158.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][2], 103.000, 213.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][2], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][2], -167773441);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][2], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][2], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][2], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][2], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][2], 1);

    Text_Player[playerid][3] = CreatePlayerTextDraw(playerid, 460.100, 131.500, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][3], 46.000, 52.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][3], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][3], -167773441);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][3], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][3], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][3], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][3], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][3], 1);

    Text_Player[playerid][4] = CreatePlayerTextDraw(playerid, 532.500, 131.500, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][4], 46.000, 52.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][4], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][4], -167773441);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][4], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][4], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][4], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][4], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][4], 1);

    Text_Player[playerid][5] = CreatePlayerTextDraw(playerid, 460.100, 344.329, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][5], 46.000, 52.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][5], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][5], -167773441);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][5], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][5], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][5], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][5], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][5], 1);

    Text_Player[playerid][6] = CreatePlayerTextDraw(playerid, 484.000, 143.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][6], 71.000, 241.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][6], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][6], 421097727);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][6], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][6], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][6], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][6], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][6], 1);

    Text_Player[playerid][7] = CreatePlayerTextDraw(playerid, 470.000, 158.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][7], 99.000, 213.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][7], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][7], 421097727);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][7], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][7], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][7], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][7], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][7], 1);

    Text_Player[playerid][8] = CreatePlayerTextDraw(playerid, 462.500, 134.328, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][8], 43.000, 51.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][8], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][8], 421097727);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][8], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][8], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][8], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][8], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][8], 1);

    Text_Player[playerid][9] = CreatePlayerTextDraw(playerid, 462.500, 341.329, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][9], 43.000, 51.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][9], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][9], 421097727);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][9], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][9], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][9], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][9], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][9], 1);

    Text_Player[playerid][10] = CreatePlayerTextDraw(playerid, 533.500, 341.329, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][10], 43.000, 51.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][10], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][10], 421097727);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][10], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][10], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][10], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][10], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][10], 1);

    Text_Player[playerid][11] = CreatePlayerTextDraw(playerid, 533.500, 134.328, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][11], 43.000, 51.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][11], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][11], 421097727);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][11], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][11], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][11], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][11], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][11], 1);

    Text_Player[playerid][12] = CreatePlayerTextDraw(playerid, 501.000, 143.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][12], 37.000, 7.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][12], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][12], -167773441);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][12], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][12], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][12], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][12], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][12], 1);

    Text_Player[playerid][13] = CreatePlayerTextDraw(playerid, 493.000, 138.500, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][13], 19.000, 14.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][13], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][13], -167773441);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][13], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][13], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][13], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][13], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][13], 1);

    Text_Player[playerid][14] = CreatePlayerTextDraw(playerid, 527.000, 138.500, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][14], 19.000, 14.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][14], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][14], -167773441);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][14], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][14], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][14], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][14], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][14], 1);

    Text_Player[playerid][15] = CreatePlayerTextDraw(playerid, 546.000, 148.000, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][15], 1.600, 4.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][15], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][15], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][15], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][15], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][15], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][15], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][15], 1);

    Text_Player[playerid][16] = CreatePlayerTextDraw(playerid, 549.000, 146.399, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][16], 1.600, 5.500);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][16], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][16], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][16], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][16], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][16], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][16], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][16], 1);

    Text_Player[playerid][17] = CreatePlayerTextDraw(playerid, 552.000, 144.699, "LD_BUM:blkdot");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][17], 1.600, 7.500);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][17], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][17], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][17], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][17], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][17], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][17], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][17], 1);

    Text_Player[playerid][18] = CreatePlayerTextDraw(playerid, 472.000, 236.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][18], 35.000, 35.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][18], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][18], -251662081);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][18], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][18], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][18], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][18], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][18], 1);
    PlayerTextDrawSetSelectable(playerid, Text_Player[playerid][18], 1);

    Text_Player[playerid][19] = CreatePlayerTextDraw(playerid, 555.000, 145.000, "5G");
    PlayerTextDrawLetterSize(playerid, Text_Player[playerid][19], 0.136, 0.897);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][19], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][19], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][19], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][19], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][19], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][19], 1);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][19], 1);

    Text_Player[playerid][20] = CreatePlayerTextDraw(playerid, 490.000, 375.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][20], 60.000, 5.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][20], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][20], -167773441);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][20], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][20], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][20], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][20], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][20], 1);
    PlayerTextDrawSetSelectable(playerid, Text_Player[playerid][20], 1);

    Text_Player[playerid][21] = CreatePlayerTextDraw(playerid, 502.000, 145.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][21], 26.000, 1.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][21], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][21], 255);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][21], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][21], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][21], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][21], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][21], 1);

    Text_Player[playerid][22] = CreatePlayerTextDraw(playerid, 531.000, 144.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][22], 3.000, 3.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][22], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][22], 255);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][22], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][22], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][22], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][22], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][22], 1);

    Text_Player[playerid][23] = CreatePlayerTextDraw(playerid, 571.000, 185.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][23], 1.000, 26.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][23], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][23], 255);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][23], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][23], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][23], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][23], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][23], 1);

    Text_Player[playerid][24] = CreatePlayerTextDraw(playerid, 571.000, 185.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][24], 14.000, 27.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][24], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][24], -33949184);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][24], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][24], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][24], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][24], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][24], 1);
    PlayerTextDrawSetSelectable(playerid, Text_Player[playerid][24], 1);

    Text_Player[playerid][25] = CreatePlayerTextDraw(playerid, 467.000, 200.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][25], 1.000, 34.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][25], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][25], 255);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][25], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][25], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][25], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][25], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][25], 1);

    Text_Player[playerid][26] = CreatePlayerTextDraw(playerid, 467.000, 239.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][26], 1.000, 34.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][26], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][26], 255);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][26], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][26], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][26], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][26], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][26], 1);

    Text_Player[playerid][27] = CreatePlayerTextDraw(playerid, 472.000, 163.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][27], 35.000, 35.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][27], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][27], -2147483393);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][27], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][27], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][27], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][27], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][27], 1);
    PlayerTextDrawSetSelectable(playerid, Text_Player[playerid][27], 1);

    Text_Player[playerid][28] = CreatePlayerTextDraw(playerid, 472.000, 199.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][28], 35.000, 35.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][28], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][28], 548580095);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][28], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][28], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][28], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][28], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][28], 1);
    PlayerTextDrawSetSelectable(playerid, Text_Player[playerid][28], 1);

    Text_Player[playerid][29] = CreatePlayerTextDraw(playerid, 482.000, 244.000, "HUD:radar_cash");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][29], 17.000, 18.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][29], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][29], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][29], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][29], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][29], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][29], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][29], 1);

    Text_Player[playerid][30] = CreatePlayerTextDraw(playerid, 501.000, 163.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][30], 35.000, 35.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][30], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][30], 1768516095);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][30], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][30], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][30], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][30], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][30], 1);
    PlayerTextDrawSetSelectable(playerid, Text_Player[playerid][30], 1);

    Text_Player[playerid][31] = CreatePlayerTextDraw(playerid, 531.000, 163.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][31], 35.000, 35.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][31], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][31], 12582911);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][31], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][31], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][31], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][31], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][31], 1);
    PlayerTextDrawSetSelectable(playerid, Text_Player[playerid][31], 1);

    Text_Player[playerid][32] = CreatePlayerTextDraw(playerid, 501.000, 199.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][32], 35.000, 35.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][32], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][32], 255);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][32], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][32], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][32], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][32], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][32], 1);
    PlayerTextDrawSetSelectable(playerid, Text_Player[playerid][32], 1);

    Text_Player[playerid][33] = CreatePlayerTextDraw(playerid, 531.000, 199.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][33], 35.000, 35.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][33], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][33], 13554175);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][33], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][33], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][33], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][33], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][33], 1);
    PlayerTextDrawSetSelectable(playerid, Text_Player[playerid][33], 1);

    Text_Player[playerid][34] = CreatePlayerTextDraw(playerid, 541.000, 172.000, "LD_CHAT:badchat");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][34], 15.000, 16.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][34], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][34], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][34], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][34], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][34], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][34], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][34], 1);

    Text_Player[playerid][35] = CreatePlayerTextDraw(playerid, 482.000, 172.000, "HUD:radar_waypoint");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][35], 15.000, 16.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][35], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][35], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][35], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][35], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][35], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][35], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][35], 1);

    Text_Player[playerid][36] = CreatePlayerTextDraw(playerid, 483.000, 201.000, "/");
    PlayerTextDrawLetterSize(playerid, Text_Player[playerid][36], 0.537, 2.898);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][36], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][36], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][36], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][36], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][36], 150);
    PlayerTextDrawFont(playerid, Text_Player[playerid][36], 2);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][36], 1);

    Text_Player[playerid][37] = CreatePlayerTextDraw(playerid, 495.000, 201.000, "/");
    PlayerTextDrawLetterSize(playerid, Text_Player[playerid][37], -0.527, 2.890);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][37], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][37], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][37], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][37], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][37], 150);
    PlayerTextDrawFont(playerid, Text_Player[playerid][37], 2);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][37], 1);

    Text_Player[playerid][38] = CreatePlayerTextDraw(playerid, 484.000, 217.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][38], 10.000, 1.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][38], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][38], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][38], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][38], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][38], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][38], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][38], 1);

    Text_Player[playerid][39] = CreatePlayerTextDraw(playerid, 503.000, 201.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][39], 31.000, 31.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][39], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][39], 16711935);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][39], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][39], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][39], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][39], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][39], 1);

    Text_Player[playerid][40] = CreatePlayerTextDraw(playerid, 505.000, 205.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][40], 27.000, 22.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][40], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][40], 255);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][40], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][40], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][40], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][40], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][40], 1);

    Text_Player[playerid][41] = CreatePlayerTextDraw(playerid, 505.000, 208.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][41], 27.000, 22.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][41], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][41], 2096890111);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][41], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][41], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][41], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][41], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][41], 1);

    Text_Player[playerid][42] = CreatePlayerTextDraw(playerid, 508.000, 212.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][42], 21.000, 12.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][42], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][42], 255);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][42], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][42], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][42], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][42], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][42], 1);

    Text_Player[playerid][43] = CreatePlayerTextDraw(playerid, 508.000, 215.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][43], 21.000, 12.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][43], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][43], 2096890111);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][43], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][43], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][43], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][43], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][43], 1);

    Text_Player[playerid][44] = CreatePlayerTextDraw(playerid, 512.000, 217.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][44], 13.000, 9.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][44], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][44], 255);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][44], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][44], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][44], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][44], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][44], 1);

    Text_Player[playerid][45] = CreatePlayerTextDraw(playerid, 534.000, 202.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][45], 29.000, 29.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][45], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][45], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][45], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][45], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][45], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][45], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][45], 1);

    Text_Player[playerid][46] = CreatePlayerTextDraw(playerid, 542.000, 212.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][46], 13.000, 10.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][46], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][46], 512819199);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][46], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][46], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][46], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][46], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][46], 1);

    Text_Player[playerid][47] = CreatePlayerTextDraw(playerid, 550.000, 212.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][47], 5.000, 4.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][47], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][47], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][47], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][47], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][47], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][47], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][47], 1);

    Text_Player[playerid][48] = CreatePlayerTextDraw(playerid, 542.000, 218.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][48], 5.000, 4.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][48], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][48], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][48], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][48], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][48], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][48], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][48], 1);

    Text_Player[playerid][49] = CreatePlayerTextDraw(playerid, 501.000, 236.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][49], 35.000, 35.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][49], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][49], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][49], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][49], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][49], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][49], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][49], 1);
    PlayerTextDrawSetSelectable(playerid, Text_Player[playerid][49], 1);

    Text_Player[playerid][50] = CreatePlayerTextDraw(playerid, 506.000, 240.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][50], 25.000, 27.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][50], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][50], 579543807);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][50], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][50], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][50], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][50], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][50], 1);

    Text_Player[playerid][51] = CreatePlayerTextDraw(playerid, 510.000, 244.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][51], 17.000, 19.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][51], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][51], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][51], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][51], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][51], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][51], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][51], 1);

    Text_Player[playerid][52] = CreatePlayerTextDraw(playerid, 513.000, 248.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][52], 11.000, 11.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][52], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][52], 579543807);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][52], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][52], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][52], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][52], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][52], 1);

    Text_Player[playerid][53] = CreatePlayerTextDraw(playerid, 517.000, 259.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][53], 3.000, 5.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][53], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][53], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][53], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][53], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][53], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][53], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][53], 1);

    Text_Player[playerid][54] = CreatePlayerTextDraw(playerid, 531.000, 236.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][54], 35.000, 35.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][54], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][54], -1378294017);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][54], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][54], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][54], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][54], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][54], 1);
    PlayerTextDrawSetSelectable(playerid, Text_Player[playerid][54], 1);

    Text_Player[playerid][55] = CreatePlayerTextDraw(playerid, 541.000, 248.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][55], 15.000, 11.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][55], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][55], 1887473919);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][55], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][55], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][55], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][55], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][55], 1);

    Text_Player[playerid][56] = CreatePlayerTextDraw(playerid, 547.000, 250.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][56], 8.000, 9.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][56], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][56], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][56], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][56], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][56], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][56], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][56], 1);

    Text_Player[playerid][57] = CreatePlayerTextDraw(playerid, 548.000, 251.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][57], 6.000, 7.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][57], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][57], 1768516095);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][57], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][57], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][57], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][57], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][57], 1);

    Text_Player[playerid][58] = CreatePlayerTextDraw(playerid, 541.000, 247.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][58], 4.000, 3.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][58], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][58], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][58], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][58], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][58], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][58], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][58], 1);

    Text_Player[playerid][59] = CreatePlayerTextDraw(playerid, 502.000, 331.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][59], 35.000, 35.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][59], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][59], 1433087999);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][59], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][59], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][59], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][59], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][59], 1);
    PlayerTextDrawSetSelectable(playerid, Text_Player[playerid][59], 1);

    Text_Player[playerid][60] = CreatePlayerTextDraw(playerid, 512.000, 339.000, "LD_CHAT:goodcha");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][60], 15.000, 16.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][60], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][60], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][60], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][60], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][60], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][60], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][60], 1);

    Text_Player[playerid][61] = CreatePlayerTextDraw(playerid, 532.000, 331.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][61], 35.000, 35.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][61], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][61], 548580095);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][61], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][61], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][61], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][61], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][61], 1);
    PlayerTextDrawSetSelectable(playerid, Text_Player[playerid][61], 1);

    Text_Player[playerid][62] = CreatePlayerTextDraw(playerid, 472.000, 331.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][62], 35.000, 35.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][62], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][62], -1131442177);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][62], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][62], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][62], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][62], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][62], 1);
    PlayerTextDrawSetSelectable(playerid, Text_Player[playerid][62], 1);

    Text_Player[playerid][63] = CreatePlayerTextDraw(playerid, 541.000, 345.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][63], 17.000, 13.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][63], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][63], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][63], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][63], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][63], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][63], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][63], 1);

    Text_Player[playerid][64] = CreatePlayerTextDraw(playerid, 543.000, 338.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][64], 12.000, 11.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][64], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][64], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][64], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][64], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][64], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][64], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][64], 1);

    Text_Player[playerid][65] = CreatePlayerTextDraw(playerid, 493.000, 340.000, "/");
    PlayerTextDrawLetterSize(playerid, Text_Player[playerid][65], -0.726, 1.699);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][65], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][65], 640034559);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][65], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][65], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][65], 150);
    PlayerTextDrawFont(playerid, Text_Player[playerid][65], 1);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][65], 1);

    Text_Player[playerid][66] = CreatePlayerTextDraw(playerid, 481.000, 338.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][66], 12.000, 11.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][66], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][66], 640034559);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][66], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][66], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][66], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][66], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][66], 1);

    Text_Player[playerid][67] = CreatePlayerTextDraw(playerid, 486.000, 348.000, "LD_BEAT:chit");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][67], 12.000, 11.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][67], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][67], 640034559);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][67], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][67], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][67], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][67], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][67], 1);

    Text_Player[playerid][68] = CreatePlayerTextDraw(playerid, 485.000, 193.000, "GPS");
    PlayerTextDrawLetterSize(playerid, Text_Player[playerid][68], 0.158, 0.799);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][68], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][68], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][68], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][68], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][68], 150);
    PlayerTextDrawFont(playerid, Text_Player[playerid][68], 1);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][68], 1);

    Text_Player[playerid][69] = CreatePlayerTextDraw(playerid, 509.000, 193.000, "Cai dat");
    PlayerTextDrawLetterSize(playerid, Text_Player[playerid][69], 0.158, 0.799);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][69], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][69], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][69], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][69], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][69], 150);
    PlayerTextDrawFont(playerid, Text_Player[playerid][69], 1);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][69], 1);

    Text_Player[playerid][70] = CreatePlayerTextDraw(playerid, 540.000, 193.000, "Dich vu");
    PlayerTextDrawLetterSize(playerid, Text_Player[playerid][70], 0.158, 0.799);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][70], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][70], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][70], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][70], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][70], 150);
    PlayerTextDrawFont(playerid, Text_Player[playerid][70], 1);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][70], 1);

    Text_Player[playerid][71] = CreatePlayerTextDraw(playerid, 479.000, 229.000, "Aps Store");
    PlayerTextDrawLetterSize(playerid, Text_Player[playerid][71], 0.158, 0.799);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][71], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][71], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][71], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][71], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][71], 150);
    PlayerTextDrawFont(playerid, Text_Player[playerid][71], 1);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][71], 1);

    Text_Player[playerid][72] = CreatePlayerTextDraw(playerid, 512.000, 229.000, "Spotify");
    PlayerTextDrawLetterSize(playerid, Text_Player[playerid][72], 0.158, 0.799);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][72], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][72], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][72], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][72], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][72], 150);
    PlayerTextDrawFont(playerid, Text_Player[playerid][72], 1);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][72], 1);

    Text_Player[playerid][73] = CreatePlayerTextDraw(playerid, 538.000, 229.000, "Thoi gian");
    PlayerTextDrawLetterSize(playerid, Text_Player[playerid][73], 0.158, 0.799);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][73], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][73], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][73], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][73], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][73], 150);
    PlayerTextDrawFont(playerid, Text_Player[playerid][73], 1);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][73], 1);

    Text_Player[playerid][74] = CreatePlayerTextDraw(playerid, 476.000, 266.000, "Ngan hang");
    PlayerTextDrawLetterSize(playerid, Text_Player[playerid][74], 0.158, 0.799);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][74], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][74], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][74], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][74], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][74], 150);
    PlayerTextDrawFont(playerid, Text_Player[playerid][74], 1);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][74], 1);

    Text_Player[playerid][75] = CreatePlayerTextDraw(playerid, 512.000, 266.000, "Grab");
    PlayerTextDrawLetterSize(playerid, Text_Player[playerid][75], 0.158, 0.799);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][75], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][75], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][75], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][75], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][75], 150);
    PlayerTextDrawFont(playerid, Text_Player[playerid][75], 1);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][75], 1);

    Text_Player[playerid][76] = CreatePlayerTextDraw(playerid, 539.000, 266.000, "Camera");
    PlayerTextDrawLetterSize(playerid, Text_Player[playerid][76], 0.158, 0.799);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][76], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][76], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][76], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][76], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][76], 150);
    PlayerTextDrawFont(playerid, Text_Player[playerid][76], 1);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][76], 1);

    Text_Player[playerid][77] = CreatePlayerTextDraw(playerid, 477.000, 361.000, "Dien thoai");
    PlayerTextDrawLetterSize(playerid, Text_Player[playerid][77], 0.158, 0.799);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][77], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][77], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][77], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][77], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][77], 150);
    PlayerTextDrawFont(playerid, Text_Player[playerid][77], 1);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][77], 1);

    Text_Player[playerid][78] = CreatePlayerTextDraw(playerid, 509.000, 361.000, "Tin nhan");
    PlayerTextDrawLetterSize(playerid, Text_Player[playerid][78], 0.158, 0.799);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][78], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][78], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][78], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][78], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][78], 150);
    PlayerTextDrawFont(playerid, Text_Player[playerid][78], 1);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][78], 1);

    Text_Player[playerid][79] = CreatePlayerTextDraw(playerid, 540.000, 361.000, "Lien he");
    PlayerTextDrawLetterSize(playerid, Text_Player[playerid][79], 0.158, 0.799);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][79], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][79], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][79], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][79], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][79], 150);
    PlayerTextDrawFont(playerid, Text_Player[playerid][79], 1);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][79], 1);

    Text_Player[playerid][80] = CreatePlayerTextDraw(playerid, 511.000, 172.000, "HUD:radar_modGarage");
    PlayerTextDrawTextSize(playerid, Text_Player[playerid][80], 15.000, 16.000);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][80], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][80], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][80], 0);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][80], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][80], 255);
    PlayerTextDrawFont(playerid, Text_Player[playerid][80], 4);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][80], 1);

    Text_Player[playerid][81] = CreatePlayerTextDraw(playerid, 476.000, 147.000, "15:00");
    PlayerTextDrawLetterSize(playerid, Text_Player[playerid][81], 0.170, 0.699);
    PlayerTextDrawAlignment(playerid, Text_Player[playerid][81], 1);
    PlayerTextDrawColour(playerid, Text_Player[playerid][81], -1);
    PlayerTextDrawSetShadow(playerid, Text_Player[playerid][81], 1);
    PlayerTextDrawSetOutline(playerid, Text_Player[playerid][81], 0);
    PlayerTextDrawBackgroundColor(playerid, Text_Player[playerid][81], 150);
    PlayerTextDrawFont(playerid, Text_Player[playerid][81], 1);
    PlayerTextDrawSetProportional(playerid, Text_Player[playerid][81], 1);

    PhoneBuilt[playerid] = true;
    return 1;
}

// ============================================================
//  Phone_Destroy — huỷ toàn bộ TextDraw (chỉ khi đóng phone)
// ============================================================
stock Phone_Destroy(playerid)
{
    if(!PhoneBuilt[playerid]) return 1;

    for(new i = 0; i < PHONE_TEXTDRAW_COUNT; i++)
    {
        if(Text_Player[playerid][i] != PlayerText:-1)
            PlayerTextDrawDestroy(playerid, Text_Player[playerid][i]);
        Text_Player[playerid][i] = PlayerText:-1;
    }
    PhoneBuilt[playerid] = false;
    return 1;
}

// ============================================================
//  Phone_Open / Phone_Close — gộp Build/Show, Hide/Destroy
// ============================================================
stock Phone_Open(playerid)
{
    if(!PhoneBuilt[playerid]) Phone_Build(playerid);

    for(new i = 0; i < PHONE_TEXTDRAW_COUNT; i++)
    {
        if(Text_Player[playerid][i] != PlayerText:-1)
            PlayerTextDrawShow(playerid, Text_Player[playerid][i]);
    }

    PhoneOpen[playerid] = true;
    SetPVarInt(playerid, "DIENTHOAI_", 1);
    return 1;
}

stock Phone_Close(playerid)
{
    // Hide trước khi destroy (tránh 1 frame nhấp nháy nếu destroy đột ngột)
    for(new i = 0; i < PHONE_TEXTDRAW_COUNT; i++)
    {
        if(Text_Player[playerid][i] != PlayerText:-1)
            PlayerTextDrawHide(playerid, Text_Player[playerid][i]);
    }

    Phone_Destroy(playerid);
    PhoneOpen[playerid] = false;
    SetPVarInt(playerid, "DIENTHOAI_", 0);
    return 1;
}

// ============================================================
//  Cleanup khi disconnect — tránh leak / ghost
// ============================================================
hook OnPlayerDisconnect(playerid, reason)
{
    DeletePVar(playerid, "PHONE_LOADING");
    Phone_Destroy(playerid);
    PhoneOpen[playerid] = false;
    return 1;
}

// ============================================================
//  Commands
// ============================================================
CMD:phone(playerid, params[]) {
    return pc_cmd_dienthoai(playerid, params);
}

CMD:dienthoai(playerid, params[])
{
    if(PlayerInfo[playerid][pPnumber] == 0)
        return SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban khong so huu hien thoai.");

    // ==== ĐÓNG PHONE (kể cả đang loading) ====
    if(GetPVarInt(playerid, "DIENTHOAI_") == 1 || GetPVarInt(playerid, "PHONE_LOADING") == 1)
    {
        DeletePVar(playerid, "PHONE_LOADING");

        Phone_Close(playerid);
        PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);
        CancelSelectTextDraw(playerid, 0xff78bcf0);
        pc_cmd_me(playerid, "da tat dien thoai.");
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
        ClearAnimations(playerid);
        RemovePlayerAttachedObject(playerid, 9);
        SetPlayerChatBubble(playerid, "", COLOR_PURPLE, 1.0, 1000);

        if(takingselfie[playerid] == 1)
        {
            TogglePlayerControllable(playerid, 1);
            SetCameraBehindPlayer(playerid);
            takingselfie[playerid] = 0;
            ApplyAnimation(playerid, "PED", "ATM", 4.1, 0, 1, 1, 0, 1, 1);
            return 1;
        }
        return 1;
    }

    // ==== MỞ PHONE ====
    // B1: Build 82 TD vào RAM (KHÔNG show)
    if(!PhoneBuilt[playerid]) Phone_Build(playerid);

    // B2: Animation phone_in + sound
    ApplyAnimation(playerid, "PED", "phone_in", 4.1, false, false, false, false, 0);
    PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);

    // B3: Gắn object + bubble chat
    if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER || GetPlayerState(playerid) == PLAYER_STATE_PASSENGER)
    {
        SetPlayerChatBubble(playerid, "> Dang su dung dien thoai <", COLOR_PURPLE, 15.0, 5000);
    }
    else
    {
        SetPlayerAttachedObject(playerid, 9, 18866, 5, 0.135999, 0.017999, -0.053999,
                                -91.299995, -88.799972, -2.199998, 1.000000, 1.000000, 1.000000);
        SetPlayerChatBubble(playerid, "> Dang su dung dien thoai <", COLOR_PURPLE, 15.0, 5000);
    }

    // B4: Loading -> callback show TD
    SetPVarInt(playerid, "PHONE_LOADING", 1);
    SetLoading(playerid, 1500, "Dang tai dien thoai...", "OnPhoneLoadingFinish");
    return 1;
}

CMD:selfie(playerid, params[])
{
    if(PlayerInfo[playerid][pPnumber] == 0)
        return SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban khong so huu hien thoai.");

    if(takingselfie[playerid] == 0)
    {
        GetPlayerPos(playerid, lX[playerid], lY[playerid], lZ[playerid]);
        static Float: n1X, Float: n1Y;
        if(Degree[playerid] >= 360) Degree[playerid] = 0;
        Degree[playerid] += SpeedPhone;
        n1X = lX[playerid] + RadiusPhone * floatcos(Degree[playerid], degrees);
        n1Y = lY[playerid] + RadiusPhone * floatsin(Degree[playerid], degrees);
        SetPlayerCameraPos(playerid, n1X, n1Y, lZ[playerid] + HeightPhone);
        SetPlayerCameraLookAt(playerid, lX[playerid], lY[playerid], lZ[playerid]+1);
        SetPlayerFacingAngle(playerid, Degree[playerid] - 90.0);
        takingselfie[playerid] = 1;
        ApplyAnimation(playerid, "PED", "gang_gunstand", 4.1, 1, 1, 1, 1, 1, 1);
        return 1;
    }
    if(takingselfie[playerid] == 1)
    {
        TogglePlayerControllable(playerid,1);
        SetCameraBehindPlayer(playerid);
        takingselfie[playerid] = 0;
        ApplyAnimation(playerid, "PED", "ATM", 4.1, 0, 1, 1, 0, 1, 1);
        return 1;
    }
    return 1;
}

stock ShowChuyenTienHihi(playerid) {
    return ShowPlayerDialog(playerid, DIALOG_CHUYENTIEN, DIALOG_STYLE_INPUT, "Chuyen tien", "{ffffff}Vui long nhap so tai khoan de tiep tuc giao dich", "Tuy chon", "Thoat");
}

CMD:xemtiendt(playerid, params[])
{
    new str[2460], str1[1280], str2[1280], str3[1280], str4[1280], str5[1280], name[1280];
    format(str1, sizeof(str), "{f2f2f2}Nha mang {00ffff}Viettel");
    format(str2, sizeof(str), "{f2f2f2}THONG TIN CUA BAN :");
    format(str3, sizeof(str), "{f2f2f2}   -> Ten nguoi dung : {ff9900}%s", GetPlayerNameEx(playerid));
    format(str4, sizeof(str), "{f2f2f2}   -> Tai khoan chinh : {00ff00}%s$", number_format(PlayerInfo[playerid][pCash]));
    format(str5, sizeof(str), "{f2f2f2}   -> So dien thoai : {00ff00}%i", PlayerInfo[playerid][pPnumber]);
    format(str, sizeof(str), "%s\n\n%s\n%s\n%s\n%s", str1,str2,str3,str4,str5);
    format(name, sizeof(name), "{ffffff}THONG TIN DIEN THOAI");
    ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, name, str, "Dong y", "Huy Bo");
    return 1;
}

CMD:sacombank(playerid, params[])
{
    new thongtinthe[1280];
    if(PlayerInfo[playerid][pSoTaiKhoan] == 0)
        format(thongtinthe, sizeof(thongtinthe), "Chua kich hoat");
    else
        format(thongtinthe, sizeof(thongtinthe), "%d", PlayerInfo[playerid][pSoTaiKhoan]);

    new str[2460], str1[1280], str2[1280], str3[1280], str4[1280], str5[1280], str6[1280], str7[1280], str8[1280], str9[1280], str10[1280], name[1280];
    format(str1, sizeof(str), "{f2f2f2}Chao mung ban da den voi he thong {0066ff}Vietcombank");
    format(str2, sizeof(str), "{f2f2f2}THONG TIN CUA BAN :");
    format(str3, sizeof(str), "{f2f2f2}   -> Ten nguoi dung : {ff9900}%s", GetPlayerNameEx(playerid));
    format(str4, sizeof(str), "{f2f2f2}   -> So tai khoan : {00ff00}%s", thongtinthe);
    format(str5, sizeof(str), "{f2f2f2}   -> So du tai khoan : {00ff00}%d", PlayerInfo[playerid][pAccount]);
    format(str6, sizeof(str), "------------------------------------------------");
    format(str7, sizeof(str), "{f2f2f2}Hien tai ngan hang cho phep ban duoc su dung cac chuc nang sau:");
    format(str8, sizeof(str), "{1ac6ff}   1. Quan ly the va tai khoan.");
    format(str9, sizeof(str), "{1ac6ff}   2. Chuyen tien.");
    format(str10, sizeof(str), "{1ac6ff}   3. Mo tai khoan thanh toan (500$).");
    format(str, sizeof(str), "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s", str1,str2,str3,str4,str5,str6,str7,str8,str9,str10);
    format(name, sizeof(name), "{0066ff}Vietcombank Pay");
    ShowPlayerDialog(playerid, DIALOG_SACOMBANK, DIALOG_STYLE_MSGBOX, name, str, "Dong y", "Huy Bo");
    return 1;
}

forward UpdateTK(playerid);
public UpdateTK(playerid) {
    new namez[24],bank;
    new rows = cache_num_rows();
    if(rows)
    {
        for(new i=0; i<rows; i++)
        {
            cache_get_field_content(i, "UserName", namez, MainPipeline, 24);
            bank = cache_get_field_content_int(i, "Bank", MainPipeline);
            bank += GetPVarInt(playerid, "TienGD");
            new string[129];
            format(string, sizeof(string), "UPDATE accounts SET `Bank`=%d WHERE `SoTaiKhoan` = %d", bank, GetPVarInt(playerid, "STK"));
            mysql_function_query(MainPipeline, string, false, "OnQueryFinish", "ii", SENDDATA_THREAD, playerid);
            return 1;
        }
    }
    else
    {
        ShowPlayerDialog(playerid, DIALOG_CHUYENTIEN, DIALOG_STYLE_INPUT, "Chuyen tien", "So tai khoan khong ton tai !\nVui long nhap so tai khoan de tiep tuc giao dich", "Tuy chon", "Thoat");
        return 1;
    }
    return 1;
}

forward STK_check(playerid);
public STK_check(playerid) {
    new namez[24],stk;
    new rows = cache_num_rows();
    if(rows)
    {
        for(new i=0; i<rows; i++)
        {
            cache_get_field_content(i, "UserName",  namez, MainPipeline, 24);
            stk = cache_get_field_content_int(i, "SoTaiKhoan", MainPipeline);
            SetPVarString(playerid, "_NguoiNhan", namez);
            SetPVarInt(playerid, "Offline", 1);
            SetPVarInt(playerid, "STK", stk);
            new string[129];
            format(string, sizeof string, "Ten chu the\t\t\t\t%s\nSo tai khoan\t\t\t\t%d\nBan hay nhap so tien muon giao dich o ben duoi", namez,stk);
            ShowPlayerDialog(playerid, DIALOG_CHUYENTIEN1, DIALOG_STYLE_INPUT, "Chuyen tien", string, "Tuy chon", "Thoat");
            return 1;
        }
    }
    else {
        ShowPlayerDialog(playerid, DIALOG_CHUYENTIEN, DIALOG_STYLE_INPUT, "Chuyen tien", "So tai khoan khong ton tai !\nVui long nhap so tai khoan de tiep tuc giao dich", "Tuy chon", "Thoat");
        return 1;
    }
    return 1;
}

CMD:danhbacuatoi(playerid, params[])
{
    if(PlayerInfo[playerid][pPnumber] == 0)
        return SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban khong so huu dien thoai.");
    if(PlayerInfo[playerid][pPhoneBook] == 0)
        return SendClientMessageEx(playerid, COLOR_WHITE, "{ff4400}>{ffffff} Ban chua mua sim cho dien thoai.");

    new str[2460];
    str[0] = EOS;
    format(str, sizeof(str), "1. TEN: %s || SDT: %d\n", PlayerInfo[playerid][pDanhBa1], PlayerInfo[playerid][pSDT1]);
    format(str, sizeof(str), "%s2. TEN: %s || SDT: %d\n", str, PlayerInfo[playerid][pDanhBa2], PlayerInfo[playerid][pSDT2]);
    format(str, sizeof(str), "%s3. TEN: %s || SDT: %d\n", str, PlayerInfo[playerid][pDanhBa3], PlayerInfo[playerid][pSDT3]);
    format(str, sizeof(str), "%s4. TEN: %s || SDT: %d\n", str, PlayerInfo[playerid][pDanhBa4], PlayerInfo[playerid][pSDT4]);
    format(str, sizeof(str), "%s5. TEN: %s || SDT: %d\n", str, PlayerInfo[playerid][pDanhBa5], PlayerInfo[playerid][pSDT5]);
    format(str, sizeof(str), "%s6. TEN: %s || SDT: %d\n", str, PlayerInfo[playerid][pDanhBa6], PlayerInfo[playerid][pSDT6]);
    format(str, sizeof(str), "%s7. TEN: %s || SDT: %d\n", str, PlayerInfo[playerid][pDanhBa7], PlayerInfo[playerid][pSDT7]);
    format(str, sizeof(str), "%s8. TEN: %s || SDT: %d\n", str, PlayerInfo[playerid][pDanhBa8], PlayerInfo[playerid][pSDT8]);
    format(str, sizeof(str), "%s9. TEN: %s || SDT: %d", str, PlayerInfo[playerid][pDanhBa9], PlayerInfo[playerid][pSDT9]);

    ShowPlayerDialog(playerid, DIALOG_DANHBA, DIALOG_STYLE_LIST, "DANH BA CUA TOI", str, "Dong y", "Huy bo");
    return 1;
}

stock UpdatePhoneTime(playerid)
{
    if(GetPVarInt(playerid, "DIENTHOAI_") == 1 && PhoneBuilt[playerid])
    {
        new hour, minute;
        gettime(hour, minute);
        new timeString[32];
        format(timeString, sizeof(timeString), "%02d:%02d", hour, minute);
        if(IsValidPlayerTextDraw(playerid, Text_Player[playerid][81]))
            PlayerTextDrawSetString(playerid, Text_Player[playerid][81], timeString);
    }
    return 1;
}

stock ShowOnlineCops(playerid)
{
    new string[2048], count = 0;
    string[0] = 0;
    strcat(string, "Ten\tSo dien thoai\n");

    new oldTotal = GetPVarInt(playerid, "TotalPhones");
    for(new i = 0; i < oldTotal; i++)
    {
        new oldVarname[32];
        format(oldVarname, sizeof(oldVarname), "SelectedPhone_%d", i);
        DeletePVar(playerid, oldVarname);
    }
    DeletePVar(playerid, "TotalPhones");

    foreach(new i: Player)
    {
        if(IsACop(i) && PlayerInfo[i][pDuty] == 1)
        {
            if(PlayerInfo[i][pPnumber] == 0) continue;
            new varname[32];
            format(varname, sizeof(varname), "SelectedPhone_%d", count);
            SetPVarInt(playerid, varname, PlayerInfo[i][pPnumber]);

            new tempstr[128];
            format(tempstr, sizeof(tempstr), "%s\t%d\n", GetPlayerNameEx(i), PlayerInfo[i][pPnumber]);
            strcat(string, tempstr);
            count++;
        }
    }
    SetPVarInt(playerid, "TotalPhones", count);

    if(count == 0)
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Khong co canh sat nao dang lam nhiem vu hoac ho chua so huu dien thoai.");
        return 0;
    }
    ShowPlayerDialog(playerid, DIALOG_LIST_COPS, DIALOG_STYLE_TABLIST_HEADERS, "Danh sach Canh sat dang lam nhiem vu", string, "Goi", "Huy");
    return 1;
}

stock ShowOnlineMedics(playerid)
{
    new string[2048], count = 0;
    string[0] = 0;
    strcat(string, "Ten\tSo dien thoai\n");

    new oldTotal = GetPVarInt(playerid, "TotalPhones");
    for(new i = 0; i < oldTotal; i++)
    {
        new oldVarname[32];
        format(oldVarname, sizeof(oldVarname), "SelectedPhone_%d", i);
        DeletePVar(playerid, oldVarname);
    }
    DeletePVar(playerid, "TotalPhones");

    foreach(new i: Player)
    {
        if(IsAMedic(i) && PlayerInfo[i][pDuty] == 1)
        {
            if(PlayerInfo[i][pPnumber] == 0) continue;
            new varname[32];
            format(varname, sizeof(varname), "SelectedPhone_%d", count);
            SetPVarInt(playerid, varname, PlayerInfo[i][pPnumber]);

            new tempstr[128];
            format(tempstr, sizeof(tempstr), "%s\t%d\n", GetPlayerNameEx(i), PlayerInfo[i][pPnumber]);
            strcat(string, tempstr);
            count++;
        }
    }
    SetPVarInt(playerid, "TotalPhones", count);

    if(count == 0)
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Khong co bac si nao dang lam nhiem vu hoac ho chua so huu dien thoai.");
        return 0;
    }
    ShowPlayerDialog(playerid, DIALOG_LIST_MEDICS, DIALOG_STYLE_TABLIST_HEADERS, "Danh sach Bac si dang lam nhiem vu", string, "Goi", "Huy");
    return 1;
}

stock ShowOnlineTaxiDrivers(playerid)
{
    new string[2048], count = 0;
    string[0] = 0;
    strcat(string, "Ten\tSo dien thoai\n");

    new oldTotal = GetPVarInt(playerid, "TotalPhones");
    for(new i = 0; i < oldTotal; i++)
    {
        new oldVarname[32];
        format(oldVarname, sizeof(oldVarname), "SelectedPhone_%d", i);
        DeletePVar(playerid, oldVarname);
    }
    DeletePVar(playerid, "TotalPhones");

    foreach(new i: Player)
    {
        if(IsATaxiDriver(i) && TransportDuty[i] > 0)
        {
            if(PlayerInfo[i][pPnumber] == 0) continue;
            new varname[32];
            format(varname, sizeof(varname), "SelectedPhone_%d", count);
            SetPVarInt(playerid, varname, PlayerInfo[i][pPnumber]);

            new tempstr[128];
            format(tempstr, sizeof(tempstr), "%s\t%d\n", GetPlayerNameEx(i), PlayerInfo[i][pPnumber]);
            strcat(string, tempstr);
            count++;
        }
    }
    SetPVarInt(playerid, "TotalPhones", count);

    if(count == 0)
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Khong co tai xe taxi nao dang lam nhiem vu hoac ho chua so huu dien thoai.");
        return 0;
    }
    ShowPlayerDialog(playerid, DIALOG_LIST_TAXI, DIALOG_STYLE_TABLIST_HEADERS, "Danh sach Tai xe taxi dang lam nhiem vu", string, "Goi", "Huy");
    return 1;
}

CMD:goisdt(playerid, params[])
{
    ShowPlayerDialog(playerid, DIALOG_DIENTHOAI, DIALOG_STYLE_LIST, "DIEN THOAI", "Goi bang so dien thoai\nGoi canh sat\nGoi cap cuu\nGoi tho sua xe\nGoi taxi", "Lua chon", "Huy bo");
    return 1;
}

stock ProcessSMS(playerid, phonenumb, text[])
{
    new string[128];

    if(Spectating[playerid] == 0)
    {
        format(string, sizeof(string), "* %s lay dien thoai di dong tu trong tui quan.", GetPlayerNameEx(playerid));
        ProxDetector(30.0, playerid, string, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
    }

    if(phonenumb == 555)
    {
        if ((strcmp("yes", text, true, strlen(text)) == 0) && (strlen(text) == strlen("yes")))
        {
            SendClientMessageEx(playerid, COLOR_WHITE, "Tin nhan van ban.");
            SendClientMessageEx(playerid, COLOR_YELLOW, "SMS: Toi dang theo doi ban, Sender: MOLE (555)");
            RingTone[playerid] = 20;
            return 1;
        }
        else
        {
            SendClientMessageEx(playerid, COLOR_YELLOW, "SMS: Toi dang theo doi ban, Sender: MOLE (555)");
            RingTone[playerid] = 20;
            return 1;
        }
    }

    foreach(new i: Player)
    {
        if(PlayerInfo[i][pPnumber] == phonenumb && phonenumb != 0)
        {
            new giveplayerid = i;
            if(Mobile[giveplayerid] != INVALID_PLAYER_ID)
            {
                SendClientMessageEx(playerid, COLOR_GREY, "Dien thoai cua nguoi do dang ban.");
                return 1;
            }
            Mobile[playerid] = giveplayerid;
            if(IsPlayerConnected(giveplayerid))
            {
                if(giveplayerid != INVALID_PLAYER_ID)
                {
                    if(PhoneOnline[giveplayerid] > 0)
                    {
                        SendClientMessageEx(playerid, COLOR_GREY, "Dien thoai cua nguoi do dang tat.");
                        Mobile[playerid] = INVALID_PLAYER_ID;
                        return 1;
                    }
                    foreach(new u: Player)
                    {
                        if(GetPVarInt(u, "BigEar") == 6 && (GetPVarInt(u, "BigEarPlayer") == playerid || GetPVarInt(u, "BigEarPlayer") == giveplayerid))
                        {
                            format(string, sizeof(string), "(BE) %s SMS cho %s: %s", GetPlayerNameEx(playerid), GetPlayerNameEx(giveplayerid), text);
                            SendClientMessageEx(u,COLOR_YELLOW, string);
                        }
                    }
                    if(PlayerInfo[playerid][pPhonePrivacy] == 1)
                    {
                        format(string, sizeof(string), "SMS: %s, Nguoi gui: Khong biet.", text);
                    }
                    else
                    {
                        format(string, sizeof(string), "SMS: %s, Nguoi gui: %s (%d)", text, GetPlayerNameEx(playerid), PlayerInfo[playerid][pPnumber]);
                    }

                    if(i != playerid)
                    {
                        if(PlayerInfo[i][pSmslog] > 0)
                        {
                            new query[384], ftext[128];
                            mysql_escape_string(text, ftext);
                            if(PlayerInfo[playerid][pPhonePrivacy] == 1)
                                format(query, sizeof(query), "INSERT INTO `sms` (`id`, `sender`, `senderid`, `sendernumber`, `receiver`, `receiverid`, `receivernumber`, `message`, `date`) VALUES (NULL, '%s', %d, 0, '%s', %d, %d, '%s', NOW())",
                                    GetPlayerNameExt(playerid), GetPlayerSQLId(playerid), GetPlayerNameExt(i), GetPlayerSQLId(i), phonenumb, ftext);
                            else
                                format(query, sizeof(query), "INSERT INTO `sms` (`id`, `sender`, `senderid`, `sendernumber`, `receiver`, `receiverid`, `receivernumber`, `message`, `date`) VALUES (NULL, '%s', %d, %d, '%s', %d, %d, '%s', NOW())",
                                    GetPlayerNameExt(playerid), GetPlayerSQLId(playerid), PlayerInfo[playerid][pPnumber], GetPlayerNameExt(i), GetPlayerSQLId(i), phonenumb, ftext);
                            mysql_function_query(MainPipeline, query, false, "OnQueryFinish", "i", SENDDATA_THREAD);
                        }
                    }

                    RingTone[giveplayerid] = 20;
                    SendClientMessageEx(giveplayerid, COLOR_YELLOW, string);
                    SendClientMessageEx(playerid, COLOR_YELLOW, string);
                    SendClientMessageEx(playerid, COLOR_WHITE, "Gui tin nhan van ban");
                    format(string, sizeof(string), "~r~$-%d", 25);
                    GameTextForPlayer(playerid, string, 5000, 1);
                    GivePlayerCash(playerid,-25);
                    Mobile[playerid] = INVALID_PLAYER_ID;

                    if(strcmp(PlayerInfo[giveplayerid][pAutoTextReply], "Nothing", true) != 0)
                    {
                        format(string, sizeof(string), "SMS: %s, Nguoi gui: %s [automated response] (%d)",
                            PlayerInfo[giveplayerid][pAutoTextReply], GetPlayerNameEx(giveplayerid), PlayerInfo[giveplayerid][pPnumber]);
                        SendClientMessageEx(playerid, COLOR_YELLOW, string);
                    }
                    return 1;
                }
            }
        }
    }
    SendClientMessageEx(playerid, COLOR_GRAD2, "Gui tin nhan that bai...");
    return 1;
}