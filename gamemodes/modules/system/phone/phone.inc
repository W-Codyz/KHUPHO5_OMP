#include <open.mp>
#include <YSI\YSI_Coding\y_hooks>

#define PHONE_TEXTDRAW_COUNT    (82)

new Text:  PhoneTD[PHONE_TEXTDRAW_COUNT];
new bool:  PhoneOpen[MAX_PLAYERS];
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
#define                 DIALOG_LIST_MECHANIC    (9455)
#define                 DIALOG_SMS_PHONE        (9453)
#define                 DIALOG_SMS_TEXT         (9454)

// ============================================================
//  Phone_CreateTD — tạo 82 TextDraw global (gọi một lần khi server start)
// ============================================================
static Phone_CreateTD()
{
    PhoneTD[0] = TextDrawCreate(532.500, 344.329, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[0], 46.000, 52.000);
    TextDrawAlignment(PhoneTD[0], 1);
    TextDrawColor(PhoneTD[0], -167773441);
    TextDrawSetShadow(PhoneTD[0], 0);
    TextDrawSetOutline(PhoneTD[0], 0);
    TextDrawBackgroundColor(PhoneTD[0], 255);
    TextDrawFont(PhoneTD[0], 4);
    TextDrawSetProportional(PhoneTD[0], 1);

    PhoneTD[1] = TextDrawCreate(483.000, 140.000, "LD_BUM:blkdot");
    TextDrawTextSize(PhoneTD[1], 71.000, 247.000);
    TextDrawAlignment(PhoneTD[1], 1);
    TextDrawColor(PhoneTD[1], -167773441);
    TextDrawSetShadow(PhoneTD[1], 0);
    TextDrawSetOutline(PhoneTD[1], 0);
    TextDrawBackgroundColor(PhoneTD[1], 255);
    TextDrawFont(PhoneTD[1], 4);
    TextDrawSetProportional(PhoneTD[1], 1);

    PhoneTD[2] = TextDrawCreate(468.000, 158.000, "LD_BUM:blkdot");
    TextDrawTextSize(PhoneTD[2], 103.000, 213.000);
    TextDrawAlignment(PhoneTD[2], 1);
    TextDrawColor(PhoneTD[2], -167773441);
    TextDrawSetShadow(PhoneTD[2], 0);
    TextDrawSetOutline(PhoneTD[2], 0);
    TextDrawBackgroundColor(PhoneTD[2], 255);
    TextDrawFont(PhoneTD[2], 4);
    TextDrawSetProportional(PhoneTD[2], 1);

    PhoneTD[3] = TextDrawCreate(460.100, 131.500, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[3], 46.000, 52.000);
    TextDrawAlignment(PhoneTD[3], 1);
    TextDrawColor(PhoneTD[3], -167773441);
    TextDrawSetShadow(PhoneTD[3], 0);
    TextDrawSetOutline(PhoneTD[3], 0);
    TextDrawBackgroundColor(PhoneTD[3], 255);
    TextDrawFont(PhoneTD[3], 4);
    TextDrawSetProportional(PhoneTD[3], 1);

    PhoneTD[4] = TextDrawCreate(532.500, 131.500, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[4], 46.000, 52.000);
    TextDrawAlignment(PhoneTD[4], 1);
    TextDrawColor(PhoneTD[4], -167773441);
    TextDrawSetShadow(PhoneTD[4], 0);
    TextDrawSetOutline(PhoneTD[4], 0);
    TextDrawBackgroundColor(PhoneTD[4], 255);
    TextDrawFont(PhoneTD[4], 4);
    TextDrawSetProportional(PhoneTD[4], 1);

    PhoneTD[5] = TextDrawCreate(460.100, 344.329, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[5], 46.000, 52.000);
    TextDrawAlignment(PhoneTD[5], 1);
    TextDrawColor(PhoneTD[5], -167773441);
    TextDrawSetShadow(PhoneTD[5], 0);
    TextDrawSetOutline(PhoneTD[5], 0);
    TextDrawBackgroundColor(PhoneTD[5], 255);
    TextDrawFont(PhoneTD[5], 4);
    TextDrawSetProportional(PhoneTD[5], 1);

    PhoneTD[6] = TextDrawCreate(484.000, 143.000, "LD_BUM:blkdot");
    TextDrawTextSize(PhoneTD[6], 71.000, 241.000);
    TextDrawAlignment(PhoneTD[6], 1);
    TextDrawColor(PhoneTD[6], 421097727);
    TextDrawSetShadow(PhoneTD[6], 0);
    TextDrawSetOutline(PhoneTD[6], 0);
    TextDrawBackgroundColor(PhoneTD[6], 255);
    TextDrawFont(PhoneTD[6], 4);
    TextDrawSetProportional(PhoneTD[6], 1);

    PhoneTD[7] = TextDrawCreate(470.000, 158.000, "LD_BUM:blkdot");
    TextDrawTextSize(PhoneTD[7], 99.000, 213.000);
    TextDrawAlignment(PhoneTD[7], 1);
    TextDrawColor(PhoneTD[7], 421097727);
    TextDrawSetShadow(PhoneTD[7], 0);
    TextDrawSetOutline(PhoneTD[7], 0);
    TextDrawBackgroundColor(PhoneTD[7], 255);
    TextDrawFont(PhoneTD[7], 4);
    TextDrawSetProportional(PhoneTD[7], 1);

    PhoneTD[8] = TextDrawCreate(462.500, 134.328, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[8], 43.000, 51.000);
    TextDrawAlignment(PhoneTD[8], 1);
    TextDrawColor(PhoneTD[8], 421097727);
    TextDrawSetShadow(PhoneTD[8], 0);
    TextDrawSetOutline(PhoneTD[8], 0);
    TextDrawBackgroundColor(PhoneTD[8], 255);
    TextDrawFont(PhoneTD[8], 4);
    TextDrawSetProportional(PhoneTD[8], 1);

    PhoneTD[9] = TextDrawCreate(462.500, 341.329, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[9], 43.000, 51.000);
    TextDrawAlignment(PhoneTD[9], 1);
    TextDrawColor(PhoneTD[9], 421097727);
    TextDrawSetShadow(PhoneTD[9], 0);
    TextDrawSetOutline(PhoneTD[9], 0);
    TextDrawBackgroundColor(PhoneTD[9], 255);
    TextDrawFont(PhoneTD[9], 4);
    TextDrawSetProportional(PhoneTD[9], 1);

    PhoneTD[10] = TextDrawCreate(533.500, 341.329, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[10], 43.000, 51.000);
    TextDrawAlignment(PhoneTD[10], 1);
    TextDrawColor(PhoneTD[10], 421097727);
    TextDrawSetShadow(PhoneTD[10], 0);
    TextDrawSetOutline(PhoneTD[10], 0);
    TextDrawBackgroundColor(PhoneTD[10], 255);
    TextDrawFont(PhoneTD[10], 4);
    TextDrawSetProportional(PhoneTD[10], 1);

    PhoneTD[11] = TextDrawCreate(533.500, 134.328, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[11], 43.000, 51.000);
    TextDrawAlignment(PhoneTD[11], 1);
    TextDrawColor(PhoneTD[11], 421097727);
    TextDrawSetShadow(PhoneTD[11], 0);
    TextDrawSetOutline(PhoneTD[11], 0);
    TextDrawBackgroundColor(PhoneTD[11], 255);
    TextDrawFont(PhoneTD[11], 4);
    TextDrawSetProportional(PhoneTD[11], 1);

    PhoneTD[12] = TextDrawCreate(501.000, 143.000, "LD_BUM:blkdot");
    TextDrawTextSize(PhoneTD[12], 37.000, 7.000);
    TextDrawAlignment(PhoneTD[12], 1);
    TextDrawColor(PhoneTD[12], -167773441);
    TextDrawSetShadow(PhoneTD[12], 0);
    TextDrawSetOutline(PhoneTD[12], 0);
    TextDrawBackgroundColor(PhoneTD[12], 255);
    TextDrawFont(PhoneTD[12], 4);
    TextDrawSetProportional(PhoneTD[12], 1);

    PhoneTD[13] = TextDrawCreate(493.000, 138.500, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[13], 19.000, 14.000);
    TextDrawAlignment(PhoneTD[13], 1);
    TextDrawColor(PhoneTD[13], -167773441);
    TextDrawSetShadow(PhoneTD[13], 0);
    TextDrawSetOutline(PhoneTD[13], 0);
    TextDrawBackgroundColor(PhoneTD[13], 255);
    TextDrawFont(PhoneTD[13], 4);
    TextDrawSetProportional(PhoneTD[13], 1);

    PhoneTD[14] = TextDrawCreate(527.000, 138.500, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[14], 19.000, 14.000);
    TextDrawAlignment(PhoneTD[14], 1);
    TextDrawColor(PhoneTD[14], -167773441);
    TextDrawSetShadow(PhoneTD[14], 0);
    TextDrawSetOutline(PhoneTD[14], 0);
    TextDrawBackgroundColor(PhoneTD[14], 255);
    TextDrawFont(PhoneTD[14], 4);
    TextDrawSetProportional(PhoneTD[14], 1);

    PhoneTD[15] = TextDrawCreate(546.000, 148.000, "LD_BUM:blkdot");
    TextDrawTextSize(PhoneTD[15], 1.600, 4.000);
    TextDrawAlignment(PhoneTD[15], 1);
    TextDrawColor(PhoneTD[15], -1);
    TextDrawSetShadow(PhoneTD[15], 0);
    TextDrawSetOutline(PhoneTD[15], 0);
    TextDrawBackgroundColor(PhoneTD[15], 255);
    TextDrawFont(PhoneTD[15], 4);
    TextDrawSetProportional(PhoneTD[15], 1);

    PhoneTD[16] = TextDrawCreate(549.000, 146.399, "LD_BUM:blkdot");
    TextDrawTextSize(PhoneTD[16], 1.600, 5.500);
    TextDrawAlignment(PhoneTD[16], 1);
    TextDrawColor(PhoneTD[16], -1);
    TextDrawSetShadow(PhoneTD[16], 0);
    TextDrawSetOutline(PhoneTD[16], 0);
    TextDrawBackgroundColor(PhoneTD[16], 255);
    TextDrawFont(PhoneTD[16], 4);
    TextDrawSetProportional(PhoneTD[16], 1);

    PhoneTD[17] = TextDrawCreate(552.000, 144.699, "LD_BUM:blkdot");
    TextDrawTextSize(PhoneTD[17], 1.600, 7.500);
    TextDrawAlignment(PhoneTD[17], 1);
    TextDrawColor(PhoneTD[17], -1);
    TextDrawSetShadow(PhoneTD[17], 0);
    TextDrawSetOutline(PhoneTD[17], 0);
    TextDrawBackgroundColor(PhoneTD[17], 255);
    TextDrawFont(PhoneTD[17], 4);
    TextDrawSetProportional(PhoneTD[17], 1);

    PhoneTD[18] = TextDrawCreate(472.000, 236.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[18], 35.000, 35.000);
    TextDrawAlignment(PhoneTD[18], 1);
    TextDrawColor(PhoneTD[18], -251662081);
    TextDrawSetShadow(PhoneTD[18], 0);
    TextDrawSetOutline(PhoneTD[18], 0);
    TextDrawBackgroundColor(PhoneTD[18], 255);
    TextDrawFont(PhoneTD[18], 4);
    TextDrawSetProportional(PhoneTD[18], 1);
    TextDrawSetSelectable(PhoneTD[18], 1);

    PhoneTD[19] = TextDrawCreate(555.000, 145.000, "5G");
    TextDrawLetterSize(PhoneTD[19], 0.136, 0.897);
    TextDrawAlignment(PhoneTD[19], 1);
    TextDrawColor(PhoneTD[19], -1);
    TextDrawSetShadow(PhoneTD[19], 0);
    TextDrawSetOutline(PhoneTD[19], 0);
    TextDrawBackgroundColor(PhoneTD[19], 255);
    TextDrawFont(PhoneTD[19], 1);
    TextDrawSetProportional(PhoneTD[19], 1);

    PhoneTD[20] = TextDrawCreate(490.000, 375.000, "LD_SPAC:white");
    TextDrawTextSize(PhoneTD[20], 60.000, 5.000);
    TextDrawAlignment(PhoneTD[20], 1);
    TextDrawColor(PhoneTD[20], -167773441);
    TextDrawSetShadow(PhoneTD[20], 0);
    TextDrawSetOutline(PhoneTD[20], 0);
    TextDrawBackgroundColor(PhoneTD[20], 255);
    TextDrawFont(PhoneTD[20], 4);
    TextDrawSetProportional(PhoneTD[20], 1);
    TextDrawSetSelectable(PhoneTD[20], 1);

    PhoneTD[21] = TextDrawCreate(502.000, 145.000, "LD_SPAC:white");
    TextDrawTextSize(PhoneTD[21], 26.000, 1.000);
    TextDrawAlignment(PhoneTD[21], 1);
    TextDrawColor(PhoneTD[21], 255);
    TextDrawSetShadow(PhoneTD[21], 0);
    TextDrawSetOutline(PhoneTD[21], 0);
    TextDrawBackgroundColor(PhoneTD[21], 255);
    TextDrawFont(PhoneTD[21], 4);
    TextDrawSetProportional(PhoneTD[21], 1);

    PhoneTD[22] = TextDrawCreate(531.000, 144.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[22], 3.000, 3.000);
    TextDrawAlignment(PhoneTD[22], 1);
    TextDrawColor(PhoneTD[22], 255);
    TextDrawSetShadow(PhoneTD[22], 0);
    TextDrawSetOutline(PhoneTD[22], 0);
    TextDrawBackgroundColor(PhoneTD[22], 255);
    TextDrawFont(PhoneTD[22], 4);
    TextDrawSetProportional(PhoneTD[22], 1);

    PhoneTD[23] = TextDrawCreate(571.000, 185.000, "LD_SPAC:white");
    TextDrawTextSize(PhoneTD[23], 1.000, 26.000);
    TextDrawAlignment(PhoneTD[23], 1);
    TextDrawColor(PhoneTD[23], 255);
    TextDrawSetShadow(PhoneTD[23], 0);
    TextDrawSetOutline(PhoneTD[23], 0);
    TextDrawBackgroundColor(PhoneTD[23], 255);
    TextDrawFont(PhoneTD[23], 4);
    TextDrawSetProportional(PhoneTD[23], 1);

    PhoneTD[24] = TextDrawCreate(571.000, 185.000, "LD_SPAC:white");
    TextDrawTextSize(PhoneTD[24], 14.000, 27.000);
    TextDrawAlignment(PhoneTD[24], 1);
    TextDrawColor(PhoneTD[24], -33949184);
    TextDrawSetShadow(PhoneTD[24], 0);
    TextDrawSetOutline(PhoneTD[24], 0);
    TextDrawBackgroundColor(PhoneTD[24], 255);
    TextDrawFont(PhoneTD[24], 4);
    TextDrawSetProportional(PhoneTD[24], 1);
    TextDrawSetSelectable(PhoneTD[24], 1);

    PhoneTD[25] = TextDrawCreate(467.000, 200.000, "LD_SPAC:white");
    TextDrawTextSize(PhoneTD[25], 1.000, 34.000);
    TextDrawAlignment(PhoneTD[25], 1);
    TextDrawColor(PhoneTD[25], 255);
    TextDrawSetShadow(PhoneTD[25], 0);
    TextDrawSetOutline(PhoneTD[25], 0);
    TextDrawBackgroundColor(PhoneTD[25], 255);
    TextDrawFont(PhoneTD[25], 4);
    TextDrawSetProportional(PhoneTD[25], 1);

    PhoneTD[26] = TextDrawCreate(467.000, 239.000, "LD_SPAC:white");
    TextDrawTextSize(PhoneTD[26], 1.000, 34.000);
    TextDrawAlignment(PhoneTD[26], 1);
    TextDrawColor(PhoneTD[26], 255);
    TextDrawSetShadow(PhoneTD[26], 0);
    TextDrawSetOutline(PhoneTD[26], 0);
    TextDrawBackgroundColor(PhoneTD[26], 255);
    TextDrawFont(PhoneTD[26], 4);
    TextDrawSetProportional(PhoneTD[26], 1);

    PhoneTD[27] = TextDrawCreate(472.000, 163.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[27], 35.000, 35.000);
    TextDrawAlignment(PhoneTD[27], 1);
    TextDrawColor(PhoneTD[27], -2147483393);
    TextDrawSetShadow(PhoneTD[27], 0);
    TextDrawSetOutline(PhoneTD[27], 0);
    TextDrawBackgroundColor(PhoneTD[27], 255);
    TextDrawFont(PhoneTD[27], 4);
    TextDrawSetProportional(PhoneTD[27], 1);
    TextDrawSetSelectable(PhoneTD[27], 1);

    PhoneTD[28] = TextDrawCreate(472.000, 199.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[28], 35.000, 35.000);
    TextDrawAlignment(PhoneTD[28], 1);
    TextDrawColor(PhoneTD[28], 548580095);
    TextDrawSetShadow(PhoneTD[28], 0);
    TextDrawSetOutline(PhoneTD[28], 0);
    TextDrawBackgroundColor(PhoneTD[28], 255);
    TextDrawFont(PhoneTD[28], 4);
    TextDrawSetProportional(PhoneTD[28], 1);
    TextDrawSetSelectable(PhoneTD[28], 1);

    PhoneTD[29] = TextDrawCreate(482.000, 244.000, "HUD:radar_cash");
    TextDrawTextSize(PhoneTD[29], 17.000, 18.000);
    TextDrawAlignment(PhoneTD[29], 1);
    TextDrawColor(PhoneTD[29], -1);
    TextDrawSetShadow(PhoneTD[29], 0);
    TextDrawSetOutline(PhoneTD[29], 0);
    TextDrawBackgroundColor(PhoneTD[29], 255);
    TextDrawFont(PhoneTD[29], 4);
    TextDrawSetProportional(PhoneTD[29], 1);

    PhoneTD[30] = TextDrawCreate(501.000, 163.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[30], 35.000, 35.000);
    TextDrawAlignment(PhoneTD[30], 1);
    TextDrawColor(PhoneTD[30], 1768516095);
    TextDrawSetShadow(PhoneTD[30], 0);
    TextDrawSetOutline(PhoneTD[30], 0);
    TextDrawBackgroundColor(PhoneTD[30], 255);
    TextDrawFont(PhoneTD[30], 4);
    TextDrawSetProportional(PhoneTD[30], 1);
    TextDrawSetSelectable(PhoneTD[30], 1);

    PhoneTD[31] = TextDrawCreate(531.000, 163.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[31], 35.000, 35.000);
    TextDrawAlignment(PhoneTD[31], 1);
    TextDrawColor(PhoneTD[31], 12582911);
    TextDrawSetShadow(PhoneTD[31], 0);
    TextDrawSetOutline(PhoneTD[31], 0);
    TextDrawBackgroundColor(PhoneTD[31], 255);
    TextDrawFont(PhoneTD[31], 4);
    TextDrawSetProportional(PhoneTD[31], 1);
    TextDrawSetSelectable(PhoneTD[31], 1);

    PhoneTD[32] = TextDrawCreate(501.000, 199.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[32], 35.000, 35.000);
    TextDrawAlignment(PhoneTD[32], 1);
    TextDrawColor(PhoneTD[32], 255);
    TextDrawSetShadow(PhoneTD[32], 0);
    TextDrawSetOutline(PhoneTD[32], 0);
    TextDrawBackgroundColor(PhoneTD[32], 255);
    TextDrawFont(PhoneTD[32], 4);
    TextDrawSetProportional(PhoneTD[32], 1);
    TextDrawSetSelectable(PhoneTD[32], 1);

    PhoneTD[33] = TextDrawCreate(531.000, 199.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[33], 35.000, 35.000);
    TextDrawAlignment(PhoneTD[33], 1);
    TextDrawColor(PhoneTD[33], 13554175);
    TextDrawSetShadow(PhoneTD[33], 0);
    TextDrawSetOutline(PhoneTD[33], 0);
    TextDrawBackgroundColor(PhoneTD[33], 255);
    TextDrawFont(PhoneTD[33], 4);
    TextDrawSetProportional(PhoneTD[33], 1);
    TextDrawSetSelectable(PhoneTD[33], 1);

    PhoneTD[34] = TextDrawCreate(541.000, 172.000, "LD_CHAT:badchat");
    TextDrawTextSize(PhoneTD[34], 15.000, 16.000);
    TextDrawAlignment(PhoneTD[34], 1);
    TextDrawColor(PhoneTD[34], -1);
    TextDrawSetShadow(PhoneTD[34], 0);
    TextDrawSetOutline(PhoneTD[34], 0);
    TextDrawBackgroundColor(PhoneTD[34], 255);
    TextDrawFont(PhoneTD[34], 4);
    TextDrawSetProportional(PhoneTD[34], 1);

    PhoneTD[35] = TextDrawCreate(482.000, 172.000, "HUD:radar_waypoint");
    TextDrawTextSize(PhoneTD[35], 15.000, 16.000);
    TextDrawAlignment(PhoneTD[35], 1);
    TextDrawColor(PhoneTD[35], -1);
    TextDrawSetShadow(PhoneTD[35], 0);
    TextDrawSetOutline(PhoneTD[35], 0);
    TextDrawBackgroundColor(PhoneTD[35], 255);
    TextDrawFont(PhoneTD[35], 4);
    TextDrawSetProportional(PhoneTD[35], 1);

    PhoneTD[36] = TextDrawCreate(483.000, 201.000, "/");
    TextDrawLetterSize(PhoneTD[36], 0.537, 2.898);
    TextDrawAlignment(PhoneTD[36], 1);
    TextDrawColor(PhoneTD[36], -1);
    TextDrawSetShadow(PhoneTD[36], 0);
    TextDrawSetOutline(PhoneTD[36], 0);
    TextDrawBackgroundColor(PhoneTD[36], 150);
    TextDrawFont(PhoneTD[36], 2);
    TextDrawSetProportional(PhoneTD[36], 1);

    PhoneTD[37] = TextDrawCreate(495.000, 201.000, "/");
    TextDrawLetterSize(PhoneTD[37], -0.527, 2.890);
    TextDrawAlignment(PhoneTD[37], 1);
    TextDrawColor(PhoneTD[37], -1);
    TextDrawSetShadow(PhoneTD[37], 0);
    TextDrawSetOutline(PhoneTD[37], 0);
    TextDrawBackgroundColor(PhoneTD[37], 150);
    TextDrawFont(PhoneTD[37], 2);
    TextDrawSetProportional(PhoneTD[37], 1);

    PhoneTD[38] = TextDrawCreate(484.000, 217.000, "LD_SPAC:white");
    TextDrawTextSize(PhoneTD[38], 10.000, 1.000);
    TextDrawAlignment(PhoneTD[38], 1);
    TextDrawColor(PhoneTD[38], -1);
    TextDrawSetShadow(PhoneTD[38], 0);
    TextDrawSetOutline(PhoneTD[38], 0);
    TextDrawBackgroundColor(PhoneTD[38], 255);
    TextDrawFont(PhoneTD[38], 4);
    TextDrawSetProportional(PhoneTD[38], 1);

    PhoneTD[39] = TextDrawCreate(503.000, 201.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[39], 31.000, 31.000);
    TextDrawAlignment(PhoneTD[39], 1);
    TextDrawColor(PhoneTD[39], 16711935);
    TextDrawSetShadow(PhoneTD[39], 0);
    TextDrawSetOutline(PhoneTD[39], 0);
    TextDrawBackgroundColor(PhoneTD[39], 255);
    TextDrawFont(PhoneTD[39], 4);
    TextDrawSetProportional(PhoneTD[39], 1);

    PhoneTD[40] = TextDrawCreate(505.000, 205.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[40], 27.000, 22.000);
    TextDrawAlignment(PhoneTD[40], 1);
    TextDrawColor(PhoneTD[40], 255);
    TextDrawSetShadow(PhoneTD[40], 0);
    TextDrawSetOutline(PhoneTD[40], 0);
    TextDrawBackgroundColor(PhoneTD[40], 255);
    TextDrawFont(PhoneTD[40], 4);
    TextDrawSetProportional(PhoneTD[40], 1);

    PhoneTD[41] = TextDrawCreate(505.000, 208.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[41], 27.000, 22.000);
    TextDrawAlignment(PhoneTD[41], 1);
    TextDrawColor(PhoneTD[41], 2096890111);
    TextDrawSetShadow(PhoneTD[41], 0);
    TextDrawSetOutline(PhoneTD[41], 0);
    TextDrawBackgroundColor(PhoneTD[41], 255);
    TextDrawFont(PhoneTD[41], 4);
    TextDrawSetProportional(PhoneTD[41], 1);

    PhoneTD[42] = TextDrawCreate(508.000, 212.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[42], 21.000, 12.000);
    TextDrawAlignment(PhoneTD[42], 1);
    TextDrawColor(PhoneTD[42], 255);
    TextDrawSetShadow(PhoneTD[42], 0);
    TextDrawSetOutline(PhoneTD[42], 0);
    TextDrawBackgroundColor(PhoneTD[42], 255);
    TextDrawFont(PhoneTD[42], 4);
    TextDrawSetProportional(PhoneTD[42], 1);

    PhoneTD[43] = TextDrawCreate(508.000, 215.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[43], 21.000, 12.000);
    TextDrawAlignment(PhoneTD[43], 1);
    TextDrawColor(PhoneTD[43], 2096890111);
    TextDrawSetShadow(PhoneTD[43], 0);
    TextDrawSetOutline(PhoneTD[43], 0);
    TextDrawBackgroundColor(PhoneTD[43], 255);
    TextDrawFont(PhoneTD[43], 4);
    TextDrawSetProportional(PhoneTD[43], 1);

    PhoneTD[44] = TextDrawCreate(512.000, 217.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[44], 13.000, 9.000);
    TextDrawAlignment(PhoneTD[44], 1);
    TextDrawColor(PhoneTD[44], 255);
    TextDrawSetShadow(PhoneTD[44], 0);
    TextDrawSetOutline(PhoneTD[44], 0);
    TextDrawBackgroundColor(PhoneTD[44], 255);
    TextDrawFont(PhoneTD[44], 4);
    TextDrawSetProportional(PhoneTD[44], 1);

    PhoneTD[45] = TextDrawCreate(534.000, 202.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[45], 29.000, 29.000);
    TextDrawAlignment(PhoneTD[45], 1);
    TextDrawColor(PhoneTD[45], -1);
    TextDrawSetShadow(PhoneTD[45], 0);
    TextDrawSetOutline(PhoneTD[45], 0);
    TextDrawBackgroundColor(PhoneTD[45], 255);
    TextDrawFont(PhoneTD[45], 4);
    TextDrawSetProportional(PhoneTD[45], 1);

    PhoneTD[46] = TextDrawCreate(542.000, 212.000, "LD_SPAC:white");
    TextDrawTextSize(PhoneTD[46], 13.000, 10.000);
    TextDrawAlignment(PhoneTD[46], 1);
    TextDrawColor(PhoneTD[46], 512819199);
    TextDrawSetShadow(PhoneTD[46], 0);
    TextDrawSetOutline(PhoneTD[46], 0);
    TextDrawBackgroundColor(PhoneTD[46], 255);
    TextDrawFont(PhoneTD[46], 4);
    TextDrawSetProportional(PhoneTD[46], 1);

    PhoneTD[47] = TextDrawCreate(550.000, 212.000, "LD_SPAC:white");
    TextDrawTextSize(PhoneTD[47], 5.000, 4.000);
    TextDrawAlignment(PhoneTD[47], 1);
    TextDrawColor(PhoneTD[47], -1);
    TextDrawSetShadow(PhoneTD[47], 0);
    TextDrawSetOutline(PhoneTD[47], 0);
    TextDrawBackgroundColor(PhoneTD[47], 255);
    TextDrawFont(PhoneTD[47], 4);
    TextDrawSetProportional(PhoneTD[47], 1);

    PhoneTD[48] = TextDrawCreate(542.000, 218.000, "LD_SPAC:white");
    TextDrawTextSize(PhoneTD[48], 5.000, 4.000);
    TextDrawAlignment(PhoneTD[48], 1);
    TextDrawColor(PhoneTD[48], -1);
    TextDrawSetShadow(PhoneTD[48], 0);
    TextDrawSetOutline(PhoneTD[48], 0);
    TextDrawBackgroundColor(PhoneTD[48], 255);
    TextDrawFont(PhoneTD[48], 4);
    TextDrawSetProportional(PhoneTD[48], 1);

    PhoneTD[49] = TextDrawCreate(501.000, 236.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[49], 35.000, 35.000);
    TextDrawAlignment(PhoneTD[49], 1);
    TextDrawColor(PhoneTD[49], -1);
    TextDrawSetShadow(PhoneTD[49], 0);
    TextDrawSetOutline(PhoneTD[49], 0);
    TextDrawBackgroundColor(PhoneTD[49], 255);
    TextDrawFont(PhoneTD[49], 4);
    TextDrawSetProportional(PhoneTD[49], 1);
    TextDrawSetSelectable(PhoneTD[49], 1);

    PhoneTD[50] = TextDrawCreate(506.000, 240.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[50], 25.000, 27.000);
    TextDrawAlignment(PhoneTD[50], 1);
    TextDrawColor(PhoneTD[50], 579543807);
    TextDrawSetShadow(PhoneTD[50], 0);
    TextDrawSetOutline(PhoneTD[50], 0);
    TextDrawBackgroundColor(PhoneTD[50], 255);
    TextDrawFont(PhoneTD[50], 4);
    TextDrawSetProportional(PhoneTD[50], 1);

    PhoneTD[51] = TextDrawCreate(510.000, 244.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[51], 17.000, 19.000);
    TextDrawAlignment(PhoneTD[51], 1);
    TextDrawColor(PhoneTD[51], -1);
    TextDrawSetShadow(PhoneTD[51], 0);
    TextDrawSetOutline(PhoneTD[51], 0);
    TextDrawBackgroundColor(PhoneTD[51], 255);
    TextDrawFont(PhoneTD[51], 4);
    TextDrawSetProportional(PhoneTD[51], 1);

    PhoneTD[52] = TextDrawCreate(513.000, 248.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[52], 11.000, 11.000);
    TextDrawAlignment(PhoneTD[52], 1);
    TextDrawColor(PhoneTD[52], 579543807);
    TextDrawSetShadow(PhoneTD[52], 0);
    TextDrawSetOutline(PhoneTD[52], 0);
    TextDrawBackgroundColor(PhoneTD[52], 255);
    TextDrawFont(PhoneTD[52], 4);
    TextDrawSetProportional(PhoneTD[52], 1);

    PhoneTD[53] = TextDrawCreate(517.000, 259.000, "LD_SPAC:white");
    TextDrawTextSize(PhoneTD[53], 3.000, 5.000);
    TextDrawAlignment(PhoneTD[53], 1);
    TextDrawColor(PhoneTD[53], -1);
    TextDrawSetShadow(PhoneTD[53], 0);
    TextDrawSetOutline(PhoneTD[53], 0);
    TextDrawBackgroundColor(PhoneTD[53], 255);
    TextDrawFont(PhoneTD[53], 4);
    TextDrawSetProportional(PhoneTD[53], 1);

    PhoneTD[54] = TextDrawCreate(531.000, 236.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[54], 35.000, 35.000);
    TextDrawAlignment(PhoneTD[54], 1);
    TextDrawColor(PhoneTD[54], -1378294017);
    TextDrawSetShadow(PhoneTD[54], 0);
    TextDrawSetOutline(PhoneTD[54], 0);
    TextDrawBackgroundColor(PhoneTD[54], 255);
    TextDrawFont(PhoneTD[54], 4);
    TextDrawSetProportional(PhoneTD[54], 1);
    TextDrawSetSelectable(PhoneTD[54], 1);

    PhoneTD[55] = TextDrawCreate(541.000, 248.000, "LD_SPAC:white");
    TextDrawTextSize(PhoneTD[55], 15.000, 11.000);
    TextDrawAlignment(PhoneTD[55], 1);
    TextDrawColor(PhoneTD[55], 1887473919);
    TextDrawSetShadow(PhoneTD[55], 0);
    TextDrawSetOutline(PhoneTD[55], 0);
    TextDrawBackgroundColor(PhoneTD[55], 255);
    TextDrawFont(PhoneTD[55], 4);
    TextDrawSetProportional(PhoneTD[55], 1);

    PhoneTD[56] = TextDrawCreate(547.000, 250.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[56], 8.000, 9.000);
    TextDrawAlignment(PhoneTD[56], 1);
    TextDrawColor(PhoneTD[56], -1);
    TextDrawSetShadow(PhoneTD[56], 0);
    TextDrawSetOutline(PhoneTD[56], 0);
    TextDrawBackgroundColor(PhoneTD[56], 255);
    TextDrawFont(PhoneTD[56], 4);
    TextDrawSetProportional(PhoneTD[56], 1);

    PhoneTD[57] = TextDrawCreate(548.000, 251.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[57], 6.000, 7.000);
    TextDrawAlignment(PhoneTD[57], 1);
    TextDrawColor(PhoneTD[57], 1768516095);
    TextDrawSetShadow(PhoneTD[57], 0);
    TextDrawSetOutline(PhoneTD[57], 0);
    TextDrawBackgroundColor(PhoneTD[57], 255);
    TextDrawFont(PhoneTD[57], 4);
    TextDrawSetProportional(PhoneTD[57], 1);

    PhoneTD[58] = TextDrawCreate(541.000, 247.000, "LD_SPAC:white");
    TextDrawTextSize(PhoneTD[58], 4.000, 3.000);
    TextDrawAlignment(PhoneTD[58], 1);
    TextDrawColor(PhoneTD[58], -1);
    TextDrawSetShadow(PhoneTD[58], 0);
    TextDrawSetOutline(PhoneTD[58], 0);
    TextDrawBackgroundColor(PhoneTD[58], 255);
    TextDrawFont(PhoneTD[58], 4);
    TextDrawSetProportional(PhoneTD[58], 1);

    PhoneTD[59] = TextDrawCreate(502.000, 331.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[59], 35.000, 35.000);
    TextDrawAlignment(PhoneTD[59], 1);
    TextDrawColor(PhoneTD[59], 1433087999);
    TextDrawSetShadow(PhoneTD[59], 0);
    TextDrawSetOutline(PhoneTD[59], 0);
    TextDrawBackgroundColor(PhoneTD[59], 255);
    TextDrawFont(PhoneTD[59], 4);
    TextDrawSetProportional(PhoneTD[59], 1);
    TextDrawSetSelectable(PhoneTD[59], 1);

    PhoneTD[60] = TextDrawCreate(512.000, 339.000, "LD_CHAT:goodcha");
    TextDrawTextSize(PhoneTD[60], 15.000, 16.000);
    TextDrawAlignment(PhoneTD[60], 1);
    TextDrawColor(PhoneTD[60], -1);
    TextDrawSetShadow(PhoneTD[60], 0);
    TextDrawSetOutline(PhoneTD[60], 0);
    TextDrawBackgroundColor(PhoneTD[60], 255);
    TextDrawFont(PhoneTD[60], 4);
    TextDrawSetProportional(PhoneTD[60], 1);

    PhoneTD[61] = TextDrawCreate(532.000, 331.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[61], 35.000, 35.000);
    TextDrawAlignment(PhoneTD[61], 1);
    TextDrawColor(PhoneTD[61], 548580095);
    TextDrawSetShadow(PhoneTD[61], 0);
    TextDrawSetOutline(PhoneTD[61], 0);
    TextDrawBackgroundColor(PhoneTD[61], 255);
    TextDrawFont(PhoneTD[61], 4);
    TextDrawSetProportional(PhoneTD[61], 1);
    TextDrawSetSelectable(PhoneTD[61], 1);

    PhoneTD[62] = TextDrawCreate(472.000, 331.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[62], 35.000, 35.000);
    TextDrawAlignment(PhoneTD[62], 1);
    TextDrawColor(PhoneTD[62], -1131442177);
    TextDrawSetShadow(PhoneTD[62], 0);
    TextDrawSetOutline(PhoneTD[62], 0);
    TextDrawBackgroundColor(PhoneTD[62], 255);
    TextDrawFont(PhoneTD[62], 4);
    TextDrawSetProportional(PhoneTD[62], 1);
    TextDrawSetSelectable(PhoneTD[62], 1);

    PhoneTD[63] = TextDrawCreate(541.000, 345.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[63], 17.000, 13.000);
    TextDrawAlignment(PhoneTD[63], 1);
    TextDrawColor(PhoneTD[63], -1);
    TextDrawSetShadow(PhoneTD[63], 0);
    TextDrawSetOutline(PhoneTD[63], 0);
    TextDrawBackgroundColor(PhoneTD[63], 255);
    TextDrawFont(PhoneTD[63], 4);
    TextDrawSetProportional(PhoneTD[63], 1);

    PhoneTD[64] = TextDrawCreate(543.000, 338.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[64], 12.000, 11.000);
    TextDrawAlignment(PhoneTD[64], 1);
    TextDrawColor(PhoneTD[64], -1);
    TextDrawSetShadow(PhoneTD[64], 0);
    TextDrawSetOutline(PhoneTD[64], 0);
    TextDrawBackgroundColor(PhoneTD[64], 255);
    TextDrawFont(PhoneTD[64], 4);
    TextDrawSetProportional(PhoneTD[64], 1);

    PhoneTD[65] = TextDrawCreate(493.000, 340.000, "/");
    TextDrawLetterSize(PhoneTD[65], -0.726, 1.699);
    TextDrawAlignment(PhoneTD[65], 1);
    TextDrawColor(PhoneTD[65], 640034559);
    TextDrawSetShadow(PhoneTD[65], 0);
    TextDrawSetOutline(PhoneTD[65], 0);
    TextDrawBackgroundColor(PhoneTD[65], 150);
    TextDrawFont(PhoneTD[65], 1);
    TextDrawSetProportional(PhoneTD[65], 1);

    PhoneTD[66] = TextDrawCreate(481.000, 338.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[66], 12.000, 11.000);
    TextDrawAlignment(PhoneTD[66], 1);
    TextDrawColor(PhoneTD[66], 640034559);
    TextDrawSetShadow(PhoneTD[66], 0);
    TextDrawSetOutline(PhoneTD[66], 0);
    TextDrawBackgroundColor(PhoneTD[66], 255);
    TextDrawFont(PhoneTD[66], 4);
    TextDrawSetProportional(PhoneTD[66], 1);

    PhoneTD[67] = TextDrawCreate(486.000, 348.000, "LD_BEAT:chit");
    TextDrawTextSize(PhoneTD[67], 12.000, 11.000);
    TextDrawAlignment(PhoneTD[67], 1);
    TextDrawColor(PhoneTD[67], 640034559);
    TextDrawSetShadow(PhoneTD[67], 0);
    TextDrawSetOutline(PhoneTD[67], 0);
    TextDrawBackgroundColor(PhoneTD[67], 255);
    TextDrawFont(PhoneTD[67], 4);
    TextDrawSetProportional(PhoneTD[67], 1);

    PhoneTD[68] = TextDrawCreate(485.000, 193.000, "GPS");
    TextDrawLetterSize(PhoneTD[68], 0.158, 0.799);
    TextDrawAlignment(PhoneTD[68], 1);
    TextDrawColor(PhoneTD[68], -1);
    TextDrawSetShadow(PhoneTD[68], 0);
    TextDrawSetOutline(PhoneTD[68], 0);
    TextDrawBackgroundColor(PhoneTD[68], 150);
    TextDrawFont(PhoneTD[68], 1);
    TextDrawSetProportional(PhoneTD[68], 1);

    PhoneTD[69] = TextDrawCreate(509.000, 193.000, "Cai dat");
    TextDrawLetterSize(PhoneTD[69], 0.158, 0.799);
    TextDrawAlignment(PhoneTD[69], 1);
    TextDrawColor(PhoneTD[69], -1);
    TextDrawSetShadow(PhoneTD[69], 0);
    TextDrawSetOutline(PhoneTD[69], 0);
    TextDrawBackgroundColor(PhoneTD[69], 150);
    TextDrawFont(PhoneTD[69], 1);
    TextDrawSetProportional(PhoneTD[69], 1);

    PhoneTD[70] = TextDrawCreate(540.000, 193.000, "Dich vu");
    TextDrawLetterSize(PhoneTD[70], 0.158, 0.799);
    TextDrawAlignment(PhoneTD[70], 1);
    TextDrawColor(PhoneTD[70], -1);
    TextDrawSetShadow(PhoneTD[70], 0);
    TextDrawSetOutline(PhoneTD[70], 0);
    TextDrawBackgroundColor(PhoneTD[70], 150);
    TextDrawFont(PhoneTD[70], 1);
    TextDrawSetProportional(PhoneTD[70], 1);

    PhoneTD[71] = TextDrawCreate(479.000, 229.000, "Aps Store");
    TextDrawLetterSize(PhoneTD[71], 0.158, 0.799);
    TextDrawAlignment(PhoneTD[71], 1);
    TextDrawColor(PhoneTD[71], -1);
    TextDrawSetShadow(PhoneTD[71], 0);
    TextDrawSetOutline(PhoneTD[71], 0);
    TextDrawBackgroundColor(PhoneTD[71], 150);
    TextDrawFont(PhoneTD[71], 1);
    TextDrawSetProportional(PhoneTD[71], 1);

    PhoneTD[72] = TextDrawCreate(512.000, 229.000, "Spotify");
    TextDrawLetterSize(PhoneTD[72], 0.158, 0.799);
    TextDrawAlignment(PhoneTD[72], 1);
    TextDrawColor(PhoneTD[72], -1);
    TextDrawSetShadow(PhoneTD[72], 0);
    TextDrawSetOutline(PhoneTD[72], 0);
    TextDrawBackgroundColor(PhoneTD[72], 150);
    TextDrawFont(PhoneTD[72], 1);
    TextDrawSetProportional(PhoneTD[72], 1);

    PhoneTD[73] = TextDrawCreate(538.000, 229.000, "Thoi gian");
    TextDrawLetterSize(PhoneTD[73], 0.158, 0.799);
    TextDrawAlignment(PhoneTD[73], 1);
    TextDrawColor(PhoneTD[73], -1);
    TextDrawSetShadow(PhoneTD[73], 0);
    TextDrawSetOutline(PhoneTD[73], 0);
    TextDrawBackgroundColor(PhoneTD[73], 150);
    TextDrawFont(PhoneTD[73], 1);
    TextDrawSetProportional(PhoneTD[73], 1);

    PhoneTD[74] = TextDrawCreate(476.000, 266.000, "Ngan hang");
    TextDrawLetterSize(PhoneTD[74], 0.158, 0.799);
    TextDrawAlignment(PhoneTD[74], 1);
    TextDrawColor(PhoneTD[74], -1);
    TextDrawSetShadow(PhoneTD[74], 0);
    TextDrawSetOutline(PhoneTD[74], 0);
    TextDrawBackgroundColor(PhoneTD[74], 150);
    TextDrawFont(PhoneTD[74], 1);
    TextDrawSetProportional(PhoneTD[74], 1);

    PhoneTD[75] = TextDrawCreate(512.000, 266.000, "Grab");
    TextDrawLetterSize(PhoneTD[75], 0.158, 0.799);
    TextDrawAlignment(PhoneTD[75], 1);
    TextDrawColor(PhoneTD[75], -1);
    TextDrawSetShadow(PhoneTD[75], 0);
    TextDrawSetOutline(PhoneTD[75], 0);
    TextDrawBackgroundColor(PhoneTD[75], 150);
    TextDrawFont(PhoneTD[75], 1);
    TextDrawSetProportional(PhoneTD[75], 1);

    PhoneTD[76] = TextDrawCreate(539.000, 266.000, "Camera");
    TextDrawLetterSize(PhoneTD[76], 0.158, 0.799);
    TextDrawAlignment(PhoneTD[76], 1);
    TextDrawColor(PhoneTD[76], -1);
    TextDrawSetShadow(PhoneTD[76], 0);
    TextDrawSetOutline(PhoneTD[76], 0);
    TextDrawBackgroundColor(PhoneTD[76], 150);
    TextDrawFont(PhoneTD[76], 1);
    TextDrawSetProportional(PhoneTD[76], 1);

    PhoneTD[77] = TextDrawCreate(477.000, 361.000, "Dien thoai");
    TextDrawLetterSize(PhoneTD[77], 0.158, 0.799);
    TextDrawAlignment(PhoneTD[77], 1);
    TextDrawColor(PhoneTD[77], -1);
    TextDrawSetShadow(PhoneTD[77], 0);
    TextDrawSetOutline(PhoneTD[77], 0);
    TextDrawBackgroundColor(PhoneTD[77], 150);
    TextDrawFont(PhoneTD[77], 1);
    TextDrawSetProportional(PhoneTD[77], 1);

    PhoneTD[78] = TextDrawCreate(509.000, 361.000, "Tin nhan");
    TextDrawLetterSize(PhoneTD[78], 0.158, 0.799);
    TextDrawAlignment(PhoneTD[78], 1);
    TextDrawColor(PhoneTD[78], -1);
    TextDrawSetShadow(PhoneTD[78], 0);
    TextDrawSetOutline(PhoneTD[78], 0);
    TextDrawBackgroundColor(PhoneTD[78], 150);
    TextDrawFont(PhoneTD[78], 1);
    TextDrawSetProportional(PhoneTD[78], 1);

    PhoneTD[79] = TextDrawCreate(540.000, 361.000, "Lien he");
    TextDrawLetterSize(PhoneTD[79], 0.158, 0.799);
    TextDrawAlignment(PhoneTD[79], 1);
    TextDrawColor(PhoneTD[79], -1);
    TextDrawSetShadow(PhoneTD[79], 0);
    TextDrawSetOutline(PhoneTD[79], 0);
    TextDrawBackgroundColor(PhoneTD[79], 150);
    TextDrawFont(PhoneTD[79], 1);
    TextDrawSetProportional(PhoneTD[79], 1);

    PhoneTD[80] = TextDrawCreate(511.000, 172.000, "HUD:radar_modGarage");
    TextDrawTextSize(PhoneTD[80], 15.000, 16.000);
    TextDrawAlignment(PhoneTD[80], 1);
    TextDrawColor(PhoneTD[80], -1);
    TextDrawSetShadow(PhoneTD[80], 0);
    TextDrawSetOutline(PhoneTD[80], 0);
    TextDrawBackgroundColor(PhoneTD[80], 255);
    TextDrawFont(PhoneTD[80], 4);
    TextDrawSetProportional(PhoneTD[80], 1);

    PhoneTD[81] = TextDrawCreate(476.000, 147.000, "15:00");
    TextDrawLetterSize(PhoneTD[81], 0.170, 0.699);
    TextDrawAlignment(PhoneTD[81], 1);
    TextDrawColor(PhoneTD[81], -1);
    TextDrawSetShadow(PhoneTD[81], 1);
    TextDrawSetOutline(PhoneTD[81], 0);
    TextDrawBackgroundColor(PhoneTD[81], 150);
    TextDrawFont(PhoneTD[81], 1);
    TextDrawSetProportional(PhoneTD[81], 1);

    return 1;
}

// ============================================================
//  Phone_Open / Phone_Close
// ============================================================
stock Phone_Open(playerid)
{
    for(new i = 0; i < PHONE_TEXTDRAW_COUNT; i++)
        TextDrawShowForPlayer(playerid, PhoneTD[i]);

    PhoneOpen[playerid] = true;
    SetPVarInt(playerid, "DIENTHOAI_", 1);
    return 1;
}

stock Phone_Close(playerid)
{
    for(new i = 0; i < PHONE_TEXTDRAW_COUNT; i++)
        TextDrawHideForPlayer(playerid, PhoneTD[i]);

    PhoneOpen[playerid] = false;
    SetPVarInt(playerid, "DIENTHOAI_", 0);
    return 1;
}

hook OnGameModeInit()
{
    Phone_CreateTD();
    return 1;
}

// ============================================================
//  Cleanup khi disconnect — tránh leak / ghost
// ============================================================
hook OnPlayerDisconnect(playerid, reason)
{
    DeletePVar(playerid, "PHONE_LOADING");
    if(PhoneOpen[playerid]) Phone_Close(playerid);
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

    if(PlayerPorter[playerid] == 1 && gPlayerCheckpointStatus[playerid] == CHECKPOINT_PORTERUNLOAD)
        return SendClientMessageEx(playerid, COLOR_GREY, "Ban khong the su dung dien thoai vao luc nay.");

    // ==== ĐÓNG PHONE (kể cả đang loading) ====
    if(GetPVarInt(playerid, "DIENTHOAI_") == 1 || GetPVarInt(playerid, "PHONE_LOADING") == 1)
    {
        DeletePVar(playerid, "PHONE_LOADING");

        Phone_Close(playerid);
        PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);
        CancelSelectTextDraw(playerid, 0xff78bcf0);
        pc_cmd_me(playerid, "da tat dien thoai.");
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
        if(!IsPlayerInAnyVehicle(playerid)) ClearAnimations(playerid);
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
    // B1: Animation phone_in + sound
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
    new str[512];
    format(str, sizeof(str), "{FFD700}=== CHUYEN KHOAN ===\n{AAAAAA}So du hien tai: {00FF7F}$%s\n\n{FFFFFF}Nhap {00CFFF}so tai khoan{FFFFFF} nguoi nhan:", number_format(PlayerInfo[playerid][pAccount]));
    return ShowPlayerDialog(playerid, DIALOG_CHUYENTIEN, DIALOG_STYLE_INPUT, "{FFD700}CHUYEN TIEN", str, "Tiep tuc", "Thoat");
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
    format(str10, sizeof(str), "{1ac6ff}   3. Mo tai khoan thanh toan ({FF6B6B}5,000${1ac6ff}).");
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
            new string[1024];
            format(string, sizeof string, "{FFD700}=== THONG TIN NGUOI NHAN ==={FFFFFF}\n{AAAAAA}Ten chu the  : {00CFFF}%s\n{AAAAAA}So tai khoan : {00CFFF}%d\n\n{FFD700}================={FFFFFF}\n{FFFFFF}Nhap so tien muon giao dich:", namez,stk);
            ShowPlayerDialog(playerid, DIALOG_CHUYENTIEN1, DIALOG_STYLE_INPUT, "{FFD700}CHUYEN TIEN", string, "Tiep tuc", "Thoat");
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
    if(GetPVarInt(playerid, "DIENTHOAI_") == 1 && PhoneOpen[playerid])
    {
        new hour, minute;
        gettime(hour, minute);
        new timeString[32];
        format(timeString, sizeof(timeString), "%02d:%02d", hour, minute);
        TextDrawSetString(PhoneTD[81], timeString);
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
    strcat(string, "Ten\tLoai\tSo dien thoai\n");

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
        if(TransportDuty[i] > 0 && (IsATaxiDriver(i) || PlayerInfo[i][pJob] == JOB_TAXIDRIVER || PlayerInfo[i][pJob2] == JOB_TAXIDRIVER || PlayerInfo[i][pTaxiLicense] == 1))
        {
            if(PlayerInfo[i][pPnumber] == 0) continue;
            new varname[32];
            format(varname, sizeof(varname), "SelectedPhone_%d", count);
            SetPVarInt(playerid, varname, PlayerInfo[i][pPnumber]);

            new tempstr[128];
            format(tempstr, sizeof(tempstr), "%s\t%s\t%d\n", GetPlayerNameEx(i), TransportDuty[i] == 1 ? "Taxi" : "Bus", PlayerInfo[i][pPnumber]);
            strcat(string, tempstr);
            count++;
        }
    }
    SetPVarInt(playerid, "TotalPhones", count);

    if(count == 0)
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Khong co tai xe Taxi/Bus nao dang lam nhiem vu hoac ho chua so huu dien thoai.");
        return 0;
    }
    ShowPlayerDialog(playerid, DIALOG_LIST_TAXI, DIALOG_STYLE_TABLIST_HEADERS, "Danh sach Tai xe Taxi/Bus dang lam nhiem vu", string, "Goi", "Huy");
    return 1;
}

stock ShowOnlineMechanics(playerid)
{
    new string[2048], count = 0;
    string[0] = 0;
    strcat(string, "Ten\tCong ty\tSo dien thoai\n");

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
        if(IsAMechanic(i) && PlayerInfo[i][pDuty] == 1)
        {
            if(PlayerInfo[i][pPnumber] == 0) continue;
            new varname[32];
            format(varname, sizeof(varname), "SelectedPhone_%d", count);
            SetPVarInt(playerid, varname, PlayerInfo[i][pPnumber]);

            new tempstr[128];
            new groupName[48];
            new groupid = PlayerInfo[i][pMember];
            if(groupid >= 0 && strlen(arrGroupData[groupid][g_szGroupName]) > 0)
                strcopy(groupName, arrGroupData[groupid][g_szGroupName], 48);
            else
                groupName = "__";
            format(tempstr, sizeof(tempstr), "%s\t%s\t%d\n", GetPlayerNameEx(i), groupName, PlayerInfo[i][pPnumber]);
            strcat(string, tempstr);
            count++;
        }
    }
    SetPVarInt(playerid, "TotalPhones", count);

    if(count == 0)
    {
        SendClientMessageEx(playerid, COLOR_GREY, "Khong co tho sua xe nao dang lam nhiem vu hoac ho chua so huu dien thoai.");
        return 0;
    }
    ShowPlayerDialog(playerid, DIALOG_LIST_MECHANIC, DIALOG_STYLE_TABLIST_HEADERS, "Danh sach Tho sua xe dang lam nhiem vu", string, "Goi", "Huy");
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