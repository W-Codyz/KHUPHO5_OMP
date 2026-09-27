#include <YSI\YSI_Coding\y_hooks>

// ===================================================================
// Constants
// ===================================================================
#define CARD_TAB_IDCARD         0
#define CARD_TAB_CARLICEN       1
#define CARD_GLOBAL_COUNT       21   // 19 from editor + 2 static value texts

#define CARD_POLICE_X           1323.191406
#define CARD_POLICE_Y           745.308959
#define CARD_POLICE_Z           112.236236
#define CARD_POLICE_RANGE       5.0

// ===================================================================
// Global textdraws — created once on init, shown/hidden per use
// ===================================================================
static Text: g_IDCard_TD   [CARD_GLOBAL_COUNT];
static Text: g_CarLic_TD   [CARD_GLOBAL_COUNT];

// ===================================================================
// Per-player textdraws — created on demand, destroyed after use
// ===================================================================
static PlayerText: g_CardNavNext      [MAX_PLAYERS];
static PlayerText: g_CardNavPrev      [MAX_PLAYERS];
static PlayerText: g_CardNavNextLabel [MAX_PLAYERS]; // ">" render tren box
static PlayerText: g_CardNavPrevLabel [MAX_PLAYERS]; // "<" render tren box
static PlayerText: g_TotalLicense_TD  [MAX_PLAYERS];

// CCCD
static PlayerText: g_IDCard_Preview  [MAX_PLAYERS];
static PlayerText: g_IDCard_Number   [MAX_PLAYERS];
static PlayerText: g_IDCard_Name     [MAX_PLAYERS];
static PlayerText: g_IDCard_DOB      [MAX_PLAYERS];
static PlayerText: g_IDCard_Gender   [MAX_PLAYERS];

// Giay phep lai xe
static PlayerText: g_CarLic_Preview  [MAX_PLAYERS];
static PlayerText: g_CarLic_Number   [MAX_PLAYERS];
static PlayerText: g_CarLic_Name     [MAX_PLAYERS];
static PlayerText: g_CarLic_DOB      [MAX_PLAYERS];
static PlayerText: g_CarLic_Class    [MAX_PLAYERS];

// ===================================================================
// State
// ===================================================================
static bool: g_CardTDCreated  [MAX_PLAYERS];
static       g_CardViewOwner  [MAX_PLAYERS]; // ownerid dang duoc xem (-1 = khong)
static       g_CardCurrentTab [MAX_PLAYERS]; // CARD_TAB_*
static       g_CardTotalCount [MAX_PLAYERS]; // so luong giay to cua owner

// ===================================================================
// Internal: tao global (shared) textdraws
// ===================================================================
static CreateCardTDGlobal()
{
    // ---- Can cuoc cong dan ----
    g_IDCard_TD[0] = TextDrawCreate(231.000, 172.000, "LD_SPAC:white");
    TextDrawTextSize(g_IDCard_TD[0], 185.000, 137.000);
    TextDrawAlignment(g_IDCard_TD[0], 1);
    TextDrawColor(g_IDCard_TD[0], -865025);
    TextDrawSetShadow(g_IDCard_TD[0], 0);
    TextDrawSetOutline(g_IDCard_TD[0], 0);
    TextDrawBackgroundColor(g_IDCard_TD[0], 255);
    TextDrawFont(g_IDCard_TD[0], 4);
    TextDrawSetProportional(g_IDCard_TD[0], 1);

    g_IDCard_TD[1] = TextDrawCreate(214.000, 158.000, "LD_BEAT:chit");
    TextDrawTextSize(g_IDCard_TD[1], 32.000, 32.000);
    TextDrawAlignment(g_IDCard_TD[1], 1);
    TextDrawColor(g_IDCard_TD[1], -865025);
    TextDrawSetShadow(g_IDCard_TD[1], 0);
    TextDrawSetOutline(g_IDCard_TD[1], 0);
    TextDrawBackgroundColor(g_IDCard_TD[1], 255);
    TextDrawFont(g_IDCard_TD[1], 4);
    TextDrawSetProportional(g_IDCard_TD[1], 1);

    g_IDCard_TD[2] = TextDrawCreate(399.000, 158.000, "LD_BEAT:chit");
    TextDrawTextSize(g_IDCard_TD[2], 32.000, 32.000);
    TextDrawAlignment(g_IDCard_TD[2], 1);
    TextDrawColor(g_IDCard_TD[2], -865025);
    TextDrawSetShadow(g_IDCard_TD[2], 0);
    TextDrawSetOutline(g_IDCard_TD[2], 0);
    TextDrawBackgroundColor(g_IDCard_TD[2], 255);
    TextDrawFont(g_IDCard_TD[2], 4);
    TextDrawSetProportional(g_IDCard_TD[2], 1);

    g_IDCard_TD[3] = TextDrawCreate(399.000, 291.000, "LD_BEAT:chit");
    TextDrawTextSize(g_IDCard_TD[3], 32.000, 32.000);
    TextDrawAlignment(g_IDCard_TD[3], 1);
    TextDrawColor(g_IDCard_TD[3], -865025);
    TextDrawSetShadow(g_IDCard_TD[3], 0);
    TextDrawSetOutline(g_IDCard_TD[3], 0);
    TextDrawBackgroundColor(g_IDCard_TD[3], 255);
    TextDrawFont(g_IDCard_TD[3], 4);
    TextDrawSetProportional(g_IDCard_TD[3], 1);

    g_IDCard_TD[4] = TextDrawCreate(214.000, 291.000, "LD_BEAT:chit");
    TextDrawTextSize(g_IDCard_TD[4], 32.000, 32.000);
    TextDrawAlignment(g_IDCard_TD[4], 1);
    TextDrawColor(g_IDCard_TD[4], -865025);
    TextDrawSetShadow(g_IDCard_TD[4], 0);
    TextDrawSetOutline(g_IDCard_TD[4], 0);
    TextDrawBackgroundColor(g_IDCard_TD[4], 255);
    TextDrawFont(g_IDCard_TD[4], 4);
    TextDrawSetProportional(g_IDCard_TD[4], 1);

    g_IDCard_TD[5] = TextDrawCreate(219.399, 172.000, "LD_SPAC:white");
    TextDrawTextSize(g_IDCard_TD[5], 12.000, 137.000);
    TextDrawAlignment(g_IDCard_TD[5], 1);
    TextDrawColor(g_IDCard_TD[5], -865025);
    TextDrawSetShadow(g_IDCard_TD[5], 0);
    TextDrawSetOutline(g_IDCard_TD[5], 0);
    TextDrawBackgroundColor(g_IDCard_TD[5], 255);
    TextDrawFont(g_IDCard_TD[5], 4);
    TextDrawSetProportional(g_IDCard_TD[5], 1);

    g_IDCard_TD[6] = TextDrawCreate(413.500, 172.000, "LD_SPAC:white");
    TextDrawTextSize(g_IDCard_TD[6], 12.000, 137.000);
    TextDrawAlignment(g_IDCard_TD[6], 1);
    TextDrawColor(g_IDCard_TD[6], -865025);
    TextDrawSetShadow(g_IDCard_TD[6], 0);
    TextDrawSetOutline(g_IDCard_TD[6], 0);
    TextDrawBackgroundColor(g_IDCard_TD[6], 255);
    TextDrawFont(g_IDCard_TD[6], 4);
    TextDrawSetProportional(g_IDCard_TD[6], 1);

    g_IDCard_TD[7] = TextDrawCreate(417.000, 163.199, "LD_SPAC:white");
    TextDrawTextSize(g_IDCard_TD[7], -188.000, 15.000);
    TextDrawAlignment(g_IDCard_TD[7], 1);
    TextDrawColor(g_IDCard_TD[7], -865025);
    TextDrawSetShadow(g_IDCard_TD[7], 0);
    TextDrawSetOutline(g_IDCard_TD[7], 0);
    TextDrawBackgroundColor(g_IDCard_TD[7], 255);
    TextDrawFont(g_IDCard_TD[7], 4);
    TextDrawSetProportional(g_IDCard_TD[7], 1);

    g_IDCard_TD[8] = TextDrawCreate(418.000, 302.500, "LD_SPAC:white");
    TextDrawTextSize(g_IDCard_TD[8], -189.000, 15.000);
    TextDrawAlignment(g_IDCard_TD[8], 1);
    TextDrawColor(g_IDCard_TD[8], -865025);
    TextDrawSetShadow(g_IDCard_TD[8], 0);
    TextDrawSetOutline(g_IDCard_TD[8], 0);
    TextDrawBackgroundColor(g_IDCard_TD[8], 255);
    TextDrawFont(g_IDCard_TD[8], 4);
    TextDrawSetProportional(g_IDCard_TD[8], 1);

    g_IDCard_TD[9] = TextDrawCreate(320.000, 170.000, "CITY AND COUNTY OF SAN FIERRO");
    TextDrawLetterSize(g_IDCard_TD[9], 0.300, 1.700);
    TextDrawAlignment(g_IDCard_TD[9], 2);
    TextDrawColor(g_IDCard_TD[9], 421097727);
    TextDrawSetShadow(g_IDCard_TD[9], 0);
    TextDrawSetOutline(g_IDCard_TD[9], 0);
    TextDrawBackgroundColor(g_IDCard_TD[9], 150);
    TextDrawFont(g_IDCard_TD[9], 1);
    TextDrawSetProportional(g_IDCard_TD[9], 1);

    g_IDCard_TD[10] = TextDrawCreate(226.000, 298.000, "Gia tri: Vo thoi han");
    TextDrawLetterSize(g_IDCard_TD[10], 0.159, 0.999);
    TextDrawAlignment(g_IDCard_TD[10], 1);
    TextDrawColor(g_IDCard_TD[10], 33023);
    TextDrawSetShadow(g_IDCard_TD[10], 0);
    TextDrawSetOutline(g_IDCard_TD[10], 0);
    TextDrawBackgroundColor(g_IDCard_TD[10], 150);
    TextDrawFont(g_IDCard_TD[10], 1);
    TextDrawSetProportional(g_IDCard_TD[10], 1);

    g_IDCard_TD[11] = TextDrawCreate(323.000, 193.000, "CAN CUOC CONG DAN");
    TextDrawLetterSize(g_IDCard_TD[11], 0.338, 2.198);
    TextDrawAlignment(g_IDCard_TD[11], 2);
    TextDrawColor(g_IDCard_TD[11], -602653441);
    TextDrawSetShadow(g_IDCard_TD[11], 0);
    TextDrawSetOutline(g_IDCard_TD[11], 0);
    TextDrawBackgroundColor(g_IDCard_TD[11], 150);
    TextDrawFont(g_IDCard_TD[11], 1);
    TextDrawSetProportional(g_IDCard_TD[11], 1);

    g_IDCard_TD[12] = TextDrawCreate(320.000, 185.000, "HTB ROLEPLAY - CITIZEN CARD");
    TextDrawLetterSize(g_IDCard_TD[12], 0.200, 0.799);
    TextDrawAlignment(g_IDCard_TD[12], 2);
    TextDrawColor(g_IDCard_TD[12], 421097727);
    TextDrawSetShadow(g_IDCard_TD[12], 0);
    TextDrawSetOutline(g_IDCard_TD[12], 0);
    TextDrawBackgroundColor(g_IDCard_TD[12], 150);
    TextDrawFont(g_IDCard_TD[12], 1);
    TextDrawSetProportional(g_IDCard_TD[12], 1);

    g_IDCard_TD[13] = TextDrawCreate(284.000, 223.000, "Citizen ID / Ma so:");
    TextDrawLetterSize(g_IDCard_TD[13], 0.159, 0.999);
    TextDrawAlignment(g_IDCard_TD[13], 1);
    TextDrawColor(g_IDCard_TD[13], 33023);
    TextDrawSetShadow(g_IDCard_TD[13], 0);
    TextDrawSetOutline(g_IDCard_TD[13], 0);
    TextDrawBackgroundColor(g_IDCard_TD[13], 150);
    TextDrawFont(g_IDCard_TD[13], 1);
    TextDrawSetProportional(g_IDCard_TD[13], 1);

    g_IDCard_TD[14] = TextDrawCreate(284.000, 236.000, "Full Name / Ho Ten:");
    TextDrawLetterSize(g_IDCard_TD[14], 0.159, 0.999);
    TextDrawAlignment(g_IDCard_TD[14], 1);
    TextDrawColor(g_IDCard_TD[14], 33023);
    TextDrawSetShadow(g_IDCard_TD[14], 0);
    TextDrawSetOutline(g_IDCard_TD[14], 0);
    TextDrawBackgroundColor(g_IDCard_TD[14], 150);
    TextDrawFont(g_IDCard_TD[14], 1);
    TextDrawSetProportional(g_IDCard_TD[14], 1);

    g_IDCard_TD[15] = TextDrawCreate(284.000, 249.000, "Date of Birth / Ngay Sinh:");
    TextDrawLetterSize(g_IDCard_TD[15], 0.159, 0.999);
    TextDrawAlignment(g_IDCard_TD[15], 1);
    TextDrawColor(g_IDCard_TD[15], 33023);
    TextDrawSetShadow(g_IDCard_TD[15], 0);
    TextDrawSetOutline(g_IDCard_TD[15], 0);
    TextDrawBackgroundColor(g_IDCard_TD[15], 150);
    TextDrawFont(g_IDCard_TD[15], 1);
    TextDrawSetProportional(g_IDCard_TD[15], 1);

    g_IDCard_TD[16] = TextDrawCreate(284.000, 262.000, "Gender / Gioi Tinh:");
    TextDrawLetterSize(g_IDCard_TD[16], 0.159, 0.999);
    TextDrawAlignment(g_IDCard_TD[16], 1);
    TextDrawColor(g_IDCard_TD[16], 33023);
    TextDrawSetShadow(g_IDCard_TD[16], 0);
    TextDrawSetOutline(g_IDCard_TD[16], 0);
    TextDrawBackgroundColor(g_IDCard_TD[16], 150);
    TextDrawFont(g_IDCard_TD[16], 1);
    TextDrawSetProportional(g_IDCard_TD[16], 1);

    g_IDCard_TD[17] = TextDrawCreate(284.000, 276.000, "Authority / Noi Cap:");
    TextDrawLetterSize(g_IDCard_TD[17], 0.159, 0.999);
    TextDrawAlignment(g_IDCard_TD[17], 1);
    TextDrawColor(g_IDCard_TD[17], 33023);
    TextDrawSetShadow(g_IDCard_TD[17], 0);
    TextDrawSetOutline(g_IDCard_TD[17], 0);
    TextDrawBackgroundColor(g_IDCard_TD[17], 150);
    TextDrawFont(g_IDCard_TD[17], 1);
    TextDrawSetProportional(g_IDCard_TD[17], 1);

    g_IDCard_TD[18] = TextDrawCreate(284.000, 290.000, "Nationality / Quoc Tich:");
    TextDrawLetterSize(g_IDCard_TD[18], 0.159, 0.999);
    TextDrawAlignment(g_IDCard_TD[18], 1);
    TextDrawColor(g_IDCard_TD[18], 33023);
    TextDrawSetShadow(g_IDCard_TD[18], 0);
    TextDrawSetOutline(g_IDCard_TD[18], 0);
    TextDrawBackgroundColor(g_IDCard_TD[18], 150);
    TextDrawFont(g_IDCard_TD[18], 1);
    TextDrawSetProportional(g_IDCard_TD[18], 1);

    // Gia tri tinh - "Authority" va "Nationality"
    g_IDCard_TD[19] = TextDrawCreate(347.000, 276.000, "S.F Police Department");
    TextDrawLetterSize(g_IDCard_TD[19], 0.170, 0.999);
    TextDrawAlignment(g_IDCard_TD[19], 1);
    TextDrawColor(g_IDCard_TD[19], 255);
    TextDrawSetShadow(g_IDCard_TD[19], 0);
    TextDrawSetOutline(g_IDCard_TD[19], 0);
    TextDrawBackgroundColor(g_IDCard_TD[19], 150);
    TextDrawFont(g_IDCard_TD[19], 1);
    TextDrawSetProportional(g_IDCard_TD[19], 1);

    g_IDCard_TD[20] = TextDrawCreate(354.000, 290.000, "San Andreas");
    TextDrawLetterSize(g_IDCard_TD[20], 0.170, 0.999);
    TextDrawAlignment(g_IDCard_TD[20], 1);
    TextDrawColor(g_IDCard_TD[20], 255);
    TextDrawSetShadow(g_IDCard_TD[20], 0);
    TextDrawSetOutline(g_IDCard_TD[20], 0);
    TextDrawBackgroundColor(g_IDCard_TD[20], 150);
    TextDrawFont(g_IDCard_TD[20], 1);
    TextDrawSetProportional(g_IDCard_TD[20], 1);

    // ---- Giay phep lai xe ----
    g_CarLic_TD[0] = TextDrawCreate(231.000, 172.000, "LD_SPAC:white");
    TextDrawTextSize(g_CarLic_TD[0], 185.000, 137.000);
    TextDrawAlignment(g_CarLic_TD[0], 1);
    TextDrawColor(g_CarLic_TD[0], -5065985);
    TextDrawSetShadow(g_CarLic_TD[0], 0);
    TextDrawSetOutline(g_CarLic_TD[0], 0);
    TextDrawBackgroundColor(g_CarLic_TD[0], 255);
    TextDrawFont(g_CarLic_TD[0], 4);
    TextDrawSetProportional(g_CarLic_TD[0], 1);

    g_CarLic_TD[1] = TextDrawCreate(214.000, 158.000, "LD_BEAT:chit");
    TextDrawTextSize(g_CarLic_TD[1], 32.000, 32.000);
    TextDrawAlignment(g_CarLic_TD[1], 1);
    TextDrawColor(g_CarLic_TD[1], -5065985);
    TextDrawSetShadow(g_CarLic_TD[1], 0);
    TextDrawSetOutline(g_CarLic_TD[1], 0);
    TextDrawBackgroundColor(g_CarLic_TD[1], 255);
    TextDrawFont(g_CarLic_TD[1], 4);
    TextDrawSetProportional(g_CarLic_TD[1], 1);

    g_CarLic_TD[2] = TextDrawCreate(399.000, 158.000, "LD_BEAT:chit");
    TextDrawTextSize(g_CarLic_TD[2], 32.000, 32.000);
    TextDrawAlignment(g_CarLic_TD[2], 1);
    TextDrawColor(g_CarLic_TD[2], -5065985);
    TextDrawSetShadow(g_CarLic_TD[2], 0);
    TextDrawSetOutline(g_CarLic_TD[2], 0);
    TextDrawBackgroundColor(g_CarLic_TD[2], 255);
    TextDrawFont(g_CarLic_TD[2], 4);
    TextDrawSetProportional(g_CarLic_TD[2], 1);

    g_CarLic_TD[3] = TextDrawCreate(399.000, 291.000, "LD_BEAT:chit");
    TextDrawTextSize(g_CarLic_TD[3], 32.000, 32.000);
    TextDrawAlignment(g_CarLic_TD[3], 1);
    TextDrawColor(g_CarLic_TD[3], -5065985);
    TextDrawSetShadow(g_CarLic_TD[3], 0);
    TextDrawSetOutline(g_CarLic_TD[3], 0);
    TextDrawBackgroundColor(g_CarLic_TD[3], 255);
    TextDrawFont(g_CarLic_TD[3], 4);
    TextDrawSetProportional(g_CarLic_TD[3], 1);

    g_CarLic_TD[4] = TextDrawCreate(214.000, 291.000, "LD_BEAT:chit");
    TextDrawTextSize(g_CarLic_TD[4], 32.000, 32.000);
    TextDrawAlignment(g_CarLic_TD[4], 1);
    TextDrawColor(g_CarLic_TD[4], -5065985);
    TextDrawSetShadow(g_CarLic_TD[4], 0);
    TextDrawSetOutline(g_CarLic_TD[4], 0);
    TextDrawBackgroundColor(g_CarLic_TD[4], 255);
    TextDrawFont(g_CarLic_TD[4], 4);
    TextDrawSetProportional(g_CarLic_TD[4], 1);

    g_CarLic_TD[5] = TextDrawCreate(219.399, 172.000, "LD_SPAC:white");
    TextDrawTextSize(g_CarLic_TD[5], 12.000, 137.000);
    TextDrawAlignment(g_CarLic_TD[5], 1);
    TextDrawColor(g_CarLic_TD[5], -5065985);
    TextDrawSetShadow(g_CarLic_TD[5], 0);
    TextDrawSetOutline(g_CarLic_TD[5], 0);
    TextDrawBackgroundColor(g_CarLic_TD[5], 255);
    TextDrawFont(g_CarLic_TD[5], 4);
    TextDrawSetProportional(g_CarLic_TD[5], 1);

    g_CarLic_TD[6] = TextDrawCreate(413.500, 172.000, "LD_SPAC:white");
    TextDrawTextSize(g_CarLic_TD[6], 12.000, 137.000);
    TextDrawAlignment(g_CarLic_TD[6], 1);
    TextDrawColor(g_CarLic_TD[6], -5065985);
    TextDrawSetShadow(g_CarLic_TD[6], 0);
    TextDrawSetOutline(g_CarLic_TD[6], 0);
    TextDrawBackgroundColor(g_CarLic_TD[6], 255);
    TextDrawFont(g_CarLic_TD[6], 4);
    TextDrawSetProportional(g_CarLic_TD[6], 1);

    g_CarLic_TD[7] = TextDrawCreate(417.000, 163.199, "LD_SPAC:white");
    TextDrawTextSize(g_CarLic_TD[7], -188.000, 15.000);
    TextDrawAlignment(g_CarLic_TD[7], 1);
    TextDrawColor(g_CarLic_TD[7], -5065985);
    TextDrawSetShadow(g_CarLic_TD[7], 0);
    TextDrawSetOutline(g_CarLic_TD[7], 0);
    TextDrawBackgroundColor(g_CarLic_TD[7], 255);
    TextDrawFont(g_CarLic_TD[7], 4);
    TextDrawSetProportional(g_CarLic_TD[7], 1);

    g_CarLic_TD[8] = TextDrawCreate(418.000, 302.500, "LD_SPAC:white");
    TextDrawTextSize(g_CarLic_TD[8], -189.000, 15.000);
    TextDrawAlignment(g_CarLic_TD[8], 1);
    TextDrawColor(g_CarLic_TD[8], -5065985);
    TextDrawSetShadow(g_CarLic_TD[8], 0);
    TextDrawSetOutline(g_CarLic_TD[8], 0);
    TextDrawBackgroundColor(g_CarLic_TD[8], 255);
    TextDrawFont(g_CarLic_TD[8], 4);
    TextDrawSetProportional(g_CarLic_TD[8], 1);

    g_CarLic_TD[9] = TextDrawCreate(320.000, 170.000, "SAN FIERRO DEPARTMENT OF MOTOR VEHICLES");
    TextDrawLetterSize(g_CarLic_TD[9], 0.240, 1.700);
    TextDrawAlignment(g_CarLic_TD[9], 2);
    TextDrawColor(g_CarLic_TD[9], 421097727);
    TextDrawSetShadow(g_CarLic_TD[9], 0);
    TextDrawSetOutline(g_CarLic_TD[9], 0);
    TextDrawBackgroundColor(g_CarLic_TD[9], 150);
    TextDrawFont(g_CarLic_TD[9], 1);
    TextDrawSetProportional(g_CarLic_TD[9], 1);

    g_CarLic_TD[10] = TextDrawCreate(226.000, 298.000, "Gia tri: Vo thoi han");
    TextDrawLetterSize(g_CarLic_TD[10], 0.159, 0.999);
    TextDrawAlignment(g_CarLic_TD[10], 1);
    TextDrawColor(g_CarLic_TD[10], 33023);
    TextDrawSetShadow(g_CarLic_TD[10], 0);
    TextDrawSetOutline(g_CarLic_TD[10], 0);
    TextDrawBackgroundColor(g_CarLic_TD[10], 150);
    TextDrawFont(g_CarLic_TD[10], 1);
    TextDrawSetProportional(g_CarLic_TD[10], 1);

    g_CarLic_TD[11] = TextDrawCreate(323.000, 193.000, "GIAY PHEP LAI XE");
    TextDrawLetterSize(g_CarLic_TD[11], 0.358, 2.198);
    TextDrawAlignment(g_CarLic_TD[11], 2);
    TextDrawColor(g_CarLic_TD[11], -602653441);
    TextDrawSetShadow(g_CarLic_TD[11], 0);
    TextDrawSetOutline(g_CarLic_TD[11], 0);
    TextDrawBackgroundColor(g_CarLic_TD[11], 150);
    TextDrawFont(g_CarLic_TD[11], 1);
    TextDrawSetProportional(g_CarLic_TD[11], 1);

    g_CarLic_TD[12] = TextDrawCreate(320.000, 186.000, "HTB ROLEPLAY - DRIVING LICENSE");
    TextDrawLetterSize(g_CarLic_TD[12], 0.200, 0.799);
    TextDrawAlignment(g_CarLic_TD[12], 2);
    TextDrawColor(g_CarLic_TD[12], 421097727);
    TextDrawSetShadow(g_CarLic_TD[12], 0);
    TextDrawSetOutline(g_CarLic_TD[12], 0);
    TextDrawBackgroundColor(g_CarLic_TD[12], 150);
    TextDrawFont(g_CarLic_TD[12], 1);
    TextDrawSetProportional(g_CarLic_TD[12], 1);

    g_CarLic_TD[13] = TextDrawCreate(284.000, 223.000, "License No / So bang:");
    TextDrawLetterSize(g_CarLic_TD[13], 0.159, 0.999);
    TextDrawAlignment(g_CarLic_TD[13], 1);
    TextDrawColor(g_CarLic_TD[13], 33023);
    TextDrawSetShadow(g_CarLic_TD[13], 0);
    TextDrawSetOutline(g_CarLic_TD[13], 0);
    TextDrawBackgroundColor(g_CarLic_TD[13], 150);
    TextDrawFont(g_CarLic_TD[13], 1);
    TextDrawSetProportional(g_CarLic_TD[13], 1);

    g_CarLic_TD[14] = TextDrawCreate(284.000, 236.000, "Full Name / Ho Ten:");
    TextDrawLetterSize(g_CarLic_TD[14], 0.159, 0.999);
    TextDrawAlignment(g_CarLic_TD[14], 1);
    TextDrawColor(g_CarLic_TD[14], 33023);
    TextDrawSetShadow(g_CarLic_TD[14], 0);
    TextDrawSetOutline(g_CarLic_TD[14], 0);
    TextDrawBackgroundColor(g_CarLic_TD[14], 150);
    TextDrawFont(g_CarLic_TD[14], 1);
    TextDrawSetProportional(g_CarLic_TD[14], 1);

    g_CarLic_TD[15] = TextDrawCreate(284.000, 249.000, "Date of Birth / Ngay Sinh:");
    TextDrawLetterSize(g_CarLic_TD[15], 0.159, 0.999);
    TextDrawAlignment(g_CarLic_TD[15], 1);
    TextDrawColor(g_CarLic_TD[15], 33023);
    TextDrawSetShadow(g_CarLic_TD[15], 0);
    TextDrawSetOutline(g_CarLic_TD[15], 0);
    TextDrawBackgroundColor(g_CarLic_TD[15], 150);
    TextDrawFont(g_CarLic_TD[15], 1);
    TextDrawSetProportional(g_CarLic_TD[15], 1);

    g_CarLic_TD[16] = TextDrawCreate(284.000, 291.000, "Class / Hang:");
    TextDrawLetterSize(g_CarLic_TD[16], 0.159, 0.999);
    TextDrawAlignment(g_CarLic_TD[16], 1);
    TextDrawColor(g_CarLic_TD[16], 33023);
    TextDrawSetShadow(g_CarLic_TD[16], 0);
    TextDrawSetOutline(g_CarLic_TD[16], 0);
    TextDrawBackgroundColor(g_CarLic_TD[16], 150);
    TextDrawFont(g_CarLic_TD[16], 1);
    TextDrawSetProportional(g_CarLic_TD[16], 1);

    g_CarLic_TD[17] = TextDrawCreate(284.000, 263.000, "Issuer / Noi cap:");
    TextDrawLetterSize(g_CarLic_TD[17], 0.159, 0.999);
    TextDrawAlignment(g_CarLic_TD[17], 1);
    TextDrawColor(g_CarLic_TD[17], 33023);
    TextDrawSetShadow(g_CarLic_TD[17], 0);
    TextDrawSetOutline(g_CarLic_TD[17], 0);
    TextDrawBackgroundColor(g_CarLic_TD[17], 150);
    TextDrawFont(g_CarLic_TD[17], 1);
    TextDrawSetProportional(g_CarLic_TD[17], 1);

    g_CarLic_TD[18] = TextDrawCreate(284.000, 277.000, "Nationality / Quoc Tich:");
    TextDrawLetterSize(g_CarLic_TD[18], 0.159, 0.999);
    TextDrawAlignment(g_CarLic_TD[18], 1);
    TextDrawColor(g_CarLic_TD[18], 33023);
    TextDrawSetShadow(g_CarLic_TD[18], 0);
    TextDrawSetOutline(g_CarLic_TD[18], 0);
    TextDrawBackgroundColor(g_CarLic_TD[18], 150);
    TextDrawFont(g_CarLic_TD[18], 1);
    TextDrawSetProportional(g_CarLic_TD[18], 1);

    // Gia tri tinh - "Issuer" va "Nationality"
    g_CarLic_TD[19] = TextDrawCreate(338.000, 262.000, "S.F Police Department");
    TextDrawLetterSize(g_CarLic_TD[19], 0.170, 0.999);
    TextDrawAlignment(g_CarLic_TD[19], 1);
    TextDrawColor(g_CarLic_TD[19], 255);
    TextDrawSetShadow(g_CarLic_TD[19], 0);
    TextDrawSetOutline(g_CarLic_TD[19], 0);
    TextDrawBackgroundColor(g_CarLic_TD[19], 150);
    TextDrawFont(g_CarLic_TD[19], 1);
    TextDrawSetProportional(g_CarLic_TD[19], 1);

    g_CarLic_TD[20] = TextDrawCreate(354.000, 277.000, "San Andreas");
    TextDrawLetterSize(g_CarLic_TD[20], 0.170, 0.999);
    TextDrawAlignment(g_CarLic_TD[20], 1);
    TextDrawColor(g_CarLic_TD[20], 255);
    TextDrawSetShadow(g_CarLic_TD[20], 0);
    TextDrawSetOutline(g_CarLic_TD[20], 0);
    TextDrawBackgroundColor(g_CarLic_TD[20], 150);
    TextDrawFont(g_CarLic_TD[20], 1);
    TextDrawSetProportional(g_CarLic_TD[20], 1);

}

// ===================================================================
// Internal: tao per-player textdraws
// ===================================================================
static CreateCardTDPlayer(playerid)
{
    // Nut < va > (co the click)
    g_CardNavNext[playerid] = CreatePlayerTextDraw(playerid, 443.000, 225.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, g_CardNavNext[playerid], 27.000, 44.000);
    PlayerTextDrawAlignment(playerid, g_CardNavNext[playerid], 1);
    PlayerTextDrawColor(playerid, g_CardNavNext[playerid], -421070081);
    PlayerTextDrawSetShadow(playerid, g_CardNavNext[playerid], 0);
    PlayerTextDrawSetOutline(playerid, g_CardNavNext[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, g_CardNavNext[playerid], 255);
    PlayerTextDrawFont(playerid, g_CardNavNext[playerid], 4);
    PlayerTextDrawSetProportional(playerid, g_CardNavNext[playerid], 1);
    PlayerTextDrawSetSelectable(playerid, g_CardNavNext[playerid], 1);

    g_CardNavPrev[playerid] = CreatePlayerTextDraw(playerid, 174.000, 225.000, "LD_SPAC:white");
    PlayerTextDrawTextSize(playerid, g_CardNavPrev[playerid], 27.000, 44.000);
    PlayerTextDrawAlignment(playerid, g_CardNavPrev[playerid], 1);
    PlayerTextDrawColor(playerid, g_CardNavPrev[playerid], -421070081);
    PlayerTextDrawSetShadow(playerid, g_CardNavPrev[playerid], 0);
    PlayerTextDrawSetOutline(playerid, g_CardNavPrev[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, g_CardNavPrev[playerid], 255);
    PlayerTextDrawFont(playerid, g_CardNavPrev[playerid], 4);
    PlayerTextDrawSetProportional(playerid, g_CardNavPrev[playerid], 1);
    PlayerTextDrawSetSelectable(playerid, g_CardNavPrev[playerid], 1);

    // Hien thi so luong giay to (vi du: "So luong: 1/2")
    g_TotalLicense_TD[playerid] = CreatePlayerTextDraw(playerid, 305.000, 145.000, "So luong: 1/1");
    PlayerTextDrawLetterSize(playerid, g_TotalLicense_TD[playerid], 0.210, 0.899);
    PlayerTextDrawAlignment(playerid, g_TotalLicense_TD[playerid], 2);
    PlayerTextDrawColor(playerid, g_TotalLicense_TD[playerid], -1);
    PlayerTextDrawSetShadow(playerid, g_TotalLicense_TD[playerid], 1);
    PlayerTextDrawSetOutline(playerid, g_TotalLicense_TD[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, g_TotalLicense_TD[playerid], 150);
    PlayerTextDrawFont(playerid, g_TotalLicense_TD[playerid], 1);
    PlayerTextDrawSetProportional(playerid, g_TotalLicense_TD[playerid], 1);

    // ---- CCCD: anh model, so ID, ten, ngay sinh, gioi tinh ----
    g_IDCard_Preview[playerid] = CreatePlayerTextDraw(playerid, 226.000, 214.000, "_");
    PlayerTextDrawTextSize(playerid, g_IDCard_Preview[playerid], 55.000, 78.000);
    PlayerTextDrawAlignment(playerid, g_IDCard_Preview[playerid], 1);
    PlayerTextDrawColor(playerid, g_IDCard_Preview[playerid], -1);
    PlayerTextDrawSetShadow(playerid, g_IDCard_Preview[playerid], 0);
    PlayerTextDrawSetOutline(playerid, g_IDCard_Preview[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, g_IDCard_Preview[playerid], 85);
    PlayerTextDrawFont(playerid, g_IDCard_Preview[playerid], 5);
    PlayerTextDrawSetProportional(playerid, g_IDCard_Preview[playerid], 0);
    PlayerTextDrawSetPreviewModel(playerid, g_IDCard_Preview[playerid], 0);
    PlayerTextDrawSetPreviewRot(playerid, g_IDCard_Preview[playerid], 0.000, 0.000, 0.000, 1.000);
    PlayerTextDrawSetPreviewVehCol(playerid, g_IDCard_Preview[playerid], 0, 0);

    g_IDCard_Number[playerid] = CreatePlayerTextDraw(playerid, 345.000, 223.000, "00000000");
    PlayerTextDrawLetterSize(playerid, g_IDCard_Number[playerid], 0.159, 0.999);
    PlayerTextDrawAlignment(playerid, g_IDCard_Number[playerid], 1);
    PlayerTextDrawColor(playerid, g_IDCard_Number[playerid], 255);
    PlayerTextDrawSetShadow(playerid, g_IDCard_Number[playerid], 0);
    PlayerTextDrawSetOutline(playerid, g_IDCard_Number[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, g_IDCard_Number[playerid], 150);
    PlayerTextDrawFont(playerid, g_IDCard_Number[playerid], 1);
    PlayerTextDrawSetProportional(playerid, g_IDCard_Number[playerid], 1);

    g_IDCard_Name[playerid] = CreatePlayerTextDraw(playerid, 346.000, 236.000, "Player_Name");
    PlayerTextDrawLetterSize(playerid, g_IDCard_Name[playerid], 0.159, 0.999);
    PlayerTextDrawAlignment(playerid, g_IDCard_Name[playerid], 1);
    PlayerTextDrawColor(playerid, g_IDCard_Name[playerid], 255);
    PlayerTextDrawSetShadow(playerid, g_IDCard_Name[playerid], 0);
    PlayerTextDrawSetOutline(playerid, g_IDCard_Name[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, g_IDCard_Name[playerid], 150);
    PlayerTextDrawFont(playerid, g_IDCard_Name[playerid], 1);
    PlayerTextDrawSetProportional(playerid, g_IDCard_Name[playerid], 1);

    g_IDCard_DOB[playerid] = CreatePlayerTextDraw(playerid, 361.000, 249.000, "DD-MM-YYYY");
    PlayerTextDrawLetterSize(playerid, g_IDCard_DOB[playerid], 0.159, 0.999);
    PlayerTextDrawAlignment(playerid, g_IDCard_DOB[playerid], 1);
    PlayerTextDrawColor(playerid, g_IDCard_DOB[playerid], 255);
    PlayerTextDrawSetShadow(playerid, g_IDCard_DOB[playerid], 0);
    PlayerTextDrawSetOutline(playerid, g_IDCard_DOB[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, g_IDCard_DOB[playerid], 150);
    PlayerTextDrawFont(playerid, g_IDCard_DOB[playerid], 1);
    PlayerTextDrawSetProportional(playerid, g_IDCard_DOB[playerid], 1);

    g_IDCard_Gender[playerid] = CreatePlayerTextDraw(playerid, 347.000, 262.000, "Nam");
    PlayerTextDrawLetterSize(playerid, g_IDCard_Gender[playerid], 0.159, 0.999);
    PlayerTextDrawAlignment(playerid, g_IDCard_Gender[playerid], 1);
    PlayerTextDrawColor(playerid, g_IDCard_Gender[playerid], 255);
    PlayerTextDrawSetShadow(playerid, g_IDCard_Gender[playerid], 0);
    PlayerTextDrawSetOutline(playerid, g_IDCard_Gender[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, g_IDCard_Gender[playerid], 150);
    PlayerTextDrawFont(playerid, g_IDCard_Gender[playerid], 1);
    PlayerTextDrawSetProportional(playerid, g_IDCard_Gender[playerid], 1);

    // ---- Giay phep lai xe: anh model, so bang, ten, ngay sinh, hang xe ----
    g_CarLic_Preview[playerid] = CreatePlayerTextDraw(playerid, 226.000, 214.000, "_");
    PlayerTextDrawTextSize(playerid, g_CarLic_Preview[playerid], 55.000, 78.000);
    PlayerTextDrawAlignment(playerid, g_CarLic_Preview[playerid], 1);
    PlayerTextDrawColor(playerid, g_CarLic_Preview[playerid], -1);
    PlayerTextDrawSetShadow(playerid, g_CarLic_Preview[playerid], 0);
    PlayerTextDrawSetOutline(playerid, g_CarLic_Preview[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, g_CarLic_Preview[playerid], 85);
    PlayerTextDrawFont(playerid, g_CarLic_Preview[playerid], 5);
    PlayerTextDrawSetProportional(playerid, g_CarLic_Preview[playerid], 0);
    PlayerTextDrawSetPreviewModel(playerid, g_CarLic_Preview[playerid], 0);
    PlayerTextDrawSetPreviewRot(playerid, g_CarLic_Preview[playerid], 0.000, 0.000, 0.000, 1.000);
    PlayerTextDrawSetPreviewVehCol(playerid, g_CarLic_Preview[playerid], 0, 0);

    g_CarLic_Number[playerid] = CreatePlayerTextDraw(playerid, 345.000, 223.000, "HTB00000000");
    PlayerTextDrawLetterSize(playerid, g_CarLic_Number[playerid], 0.159, 0.999);
    PlayerTextDrawAlignment(playerid, g_CarLic_Number[playerid], 1);
    PlayerTextDrawColor(playerid, g_CarLic_Number[playerid], 255);
    PlayerTextDrawSetShadow(playerid, g_CarLic_Number[playerid], 0);
    PlayerTextDrawSetOutline(playerid, g_CarLic_Number[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, g_CarLic_Number[playerid], 150);
    PlayerTextDrawFont(playerid, g_CarLic_Number[playerid], 1);
    PlayerTextDrawSetProportional(playerid, g_CarLic_Number[playerid], 1);

    g_CarLic_Name[playerid] = CreatePlayerTextDraw(playerid, 346.000, 236.000, "Player_Name");
    PlayerTextDrawLetterSize(playerid, g_CarLic_Name[playerid], 0.159, 0.999);
    PlayerTextDrawAlignment(playerid, g_CarLic_Name[playerid], 1);
    PlayerTextDrawColor(playerid, g_CarLic_Name[playerid], 255);
    PlayerTextDrawSetShadow(playerid, g_CarLic_Name[playerid], 0);
    PlayerTextDrawSetOutline(playerid, g_CarLic_Name[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, g_CarLic_Name[playerid], 150);
    PlayerTextDrawFont(playerid, g_CarLic_Name[playerid], 1);
    PlayerTextDrawSetProportional(playerid, g_CarLic_Name[playerid], 1);

    g_CarLic_DOB[playerid] = CreatePlayerTextDraw(playerid, 361.000, 249.000, "DD-MM-YYYY");
    PlayerTextDrawLetterSize(playerid, g_CarLic_DOB[playerid], 0.159, 0.999);
    PlayerTextDrawAlignment(playerid, g_CarLic_DOB[playerid], 1);
    PlayerTextDrawColor(playerid, g_CarLic_DOB[playerid], 255);
    PlayerTextDrawSetShadow(playerid, g_CarLic_DOB[playerid], 0);
    PlayerTextDrawSetOutline(playerid, g_CarLic_DOB[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, g_CarLic_DOB[playerid], 150);
    PlayerTextDrawFont(playerid, g_CarLic_DOB[playerid], 1);
    PlayerTextDrawSetProportional(playerid, g_CarLic_DOB[playerid], 1);

    g_CarLic_Class[playerid] = CreatePlayerTextDraw(playerid, 327.000, 291.000, "Full Class / Tat ca cac hang");
    PlayerTextDrawLetterSize(playerid, g_CarLic_Class[playerid], 0.170, 0.999);
    PlayerTextDrawAlignment(playerid, g_CarLic_Class[playerid], 1);
    PlayerTextDrawColor(playerid, g_CarLic_Class[playerid], 255);
    PlayerTextDrawSetShadow(playerid, g_CarLic_Class[playerid], 0);
    PlayerTextDrawSetOutline(playerid, g_CarLic_Class[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, g_CarLic_Class[playerid], 150);
    PlayerTextDrawFont(playerid, g_CarLic_Class[playerid], 1);
    PlayerTextDrawSetProportional(playerid, g_CarLic_Class[playerid], 1);

    // ---- Nhan ">" va "<" tao sau box de render len tren ----
    g_CardNavNextLabel[playerid] = CreatePlayerTextDraw(playerid, 456.000, 218.000, ">");
    PlayerTextDrawLetterSize(playerid, g_CardNavNextLabel[playerid], 0.619, 5.999);
    PlayerTextDrawAlignment(playerid, g_CardNavNextLabel[playerid], 2);
    PlayerTextDrawColor(playerid, g_CardNavNextLabel[playerid], 1768516095);
    PlayerTextDrawSetShadow(playerid, g_CardNavNextLabel[playerid], 0);
    PlayerTextDrawSetOutline(playerid, g_CardNavNextLabel[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, g_CardNavNextLabel[playerid], 150);
    PlayerTextDrawFont(playerid, g_CardNavNextLabel[playerid], 1);
    PlayerTextDrawSetProportional(playerid, g_CardNavNextLabel[playerid], 1);

    g_CardNavPrevLabel[playerid] = CreatePlayerTextDraw(playerid, 187.000, 218.000, "<");
    PlayerTextDrawLetterSize(playerid, g_CardNavPrevLabel[playerid], 0.619, 5.999);
    PlayerTextDrawAlignment(playerid, g_CardNavPrevLabel[playerid], 2);
    PlayerTextDrawColor(playerid, g_CardNavPrevLabel[playerid], 1768516095);
    PlayerTextDrawSetShadow(playerid, g_CardNavPrevLabel[playerid], 0);
    PlayerTextDrawSetOutline(playerid, g_CardNavPrevLabel[playerid], 0);
    PlayerTextDrawBackgroundColor(playerid, g_CardNavPrevLabel[playerid], 150);
    PlayerTextDrawFont(playerid, g_CardNavPrevLabel[playerid], 1);
    PlayerTextDrawSetProportional(playerid, g_CardNavPrevLabel[playerid], 1);
}

// ===================================================================
// Internal: destroy per-player textdraws
// ===================================================================
static DestroyCardTDPlayer(playerid)
{
    #define _DPTD(%0) if(%0[playerid] != INVALID_PLAYER_TEXT_DRAW) { PlayerTextDrawDestroy(playerid, %0[playerid]); %0[playerid] = INVALID_PLAYER_TEXT_DRAW; }
    _DPTD(g_CardNavNext)      _DPTD(g_CardNavPrev)      _DPTD(g_TotalLicense_TD)
    _DPTD(g_CardNavNextLabel) _DPTD(g_CardNavPrevLabel)
    _DPTD(g_IDCard_Preview)   _DPTD(g_IDCard_Number)    _DPTD(g_IDCard_Name)
    _DPTD(g_IDCard_DOB)       _DPTD(g_IDCard_Gender)
    _DPTD(g_CarLic_Preview)   _DPTD(g_CarLic_Number)    _DPTD(g_CarLic_Name)
    _DPTD(g_CarLic_DOB)       _DPTD(g_CarLic_Class)
    #undef _DPTD
    g_CardTDCreated[playerid] = false;
}

// ===================================================================
// Internal: dien thong tin vao textdraw theo tab hien tai
// ===================================================================
static PopulateCardTab(viewerid, tab)
{
    new ownerid = g_CardViewOwner[viewerid];
    new str[64];

    if(tab == CARD_TAB_IDCARD)
    {
        PlayerTextDrawSetPreviewModel(viewerid, g_IDCard_Preview[viewerid], GetPlayerSkin(ownerid));
        PlayerTextDrawShow(viewerid, g_IDCard_Preview[viewerid]);

        format(str, sizeof(str), "%d", PlayerInfo[ownerid][pIDCard]);
        PlayerTextDrawSetString(viewerid, g_IDCard_Number[viewerid], str);
        PlayerTextDrawShow(viewerid, g_IDCard_Number[viewerid]);

        PlayerTextDrawSetString(viewerid, g_IDCard_Name[viewerid], GetPlayerNameEx(ownerid));
        PlayerTextDrawShow(viewerid, g_IDCard_Name[viewerid]);

        PlayerTextDrawSetString(viewerid, g_IDCard_DOB[viewerid], PlayerInfo[ownerid][pBirthDate]);
        PlayerTextDrawShow(viewerid, g_IDCard_DOB[viewerid]);

        new genderStr[8];
        if     (PlayerInfo[ownerid][pSex] == 1) genderStr = "Nam";
        else if(PlayerInfo[ownerid][pSex] == 2) genderStr = "Nu";
        else                                    genderStr = "N/A";
        PlayerTextDrawSetString(viewerid, g_IDCard_Gender[viewerid], genderStr);
        PlayerTextDrawShow(viewerid, g_IDCard_Gender[viewerid]);
    }
    else
    {
        PlayerTextDrawSetPreviewModel(viewerid, g_CarLic_Preview[viewerid], GetPlayerSkin(ownerid));
        PlayerTextDrawShow(viewerid, g_CarLic_Preview[viewerid]);

        format(str, sizeof(str), "HTB%d", PlayerInfo[ownerid][pIDCard]);
        PlayerTextDrawSetString(viewerid, g_CarLic_Number[viewerid], str);
        PlayerTextDrawShow(viewerid, g_CarLic_Number[viewerid]);

        PlayerTextDrawSetString(viewerid, g_CarLic_Name[viewerid], GetPlayerNameEx(ownerid));
        PlayerTextDrawShow(viewerid, g_CarLic_Name[viewerid]);

        PlayerTextDrawSetString(viewerid, g_CarLic_DOB[viewerid], PlayerInfo[ownerid][pBirthDate]);
        PlayerTextDrawShow(viewerid, g_CarLic_DOB[viewerid]);

        PlayerTextDrawSetString(viewerid, g_CarLic_Class[viewerid], "Full Class / Tat ca cac hang");
        PlayerTextDrawShow(viewerid, g_CarLic_Class[viewerid]);
    }
}

// ===================================================================
// Internal: chuyen tab (an tab cu, hien tab moi)
// ===================================================================
static ShowCardTab(viewerid, tab)
{
    new oldTab = g_CardCurrentTab[viewerid];
    if(oldTab == tab) return;

    if(oldTab == CARD_TAB_IDCARD)
    {
        for(new i = 0; i < CARD_GLOBAL_COUNT; i++) TextDrawHideForPlayer(viewerid, g_IDCard_TD[i]);
        PlayerTextDrawHide(viewerid, g_IDCard_Preview[viewerid]);
        PlayerTextDrawHide(viewerid, g_IDCard_Number[viewerid]);
        PlayerTextDrawHide(viewerid, g_IDCard_Name[viewerid]);
        PlayerTextDrawHide(viewerid, g_IDCard_DOB[viewerid]);
        PlayerTextDrawHide(viewerid, g_IDCard_Gender[viewerid]);
    }
    else
    {
        for(new i = 0; i < CARD_GLOBAL_COUNT; i++) TextDrawHideForPlayer(viewerid, g_CarLic_TD[i]);
        PlayerTextDrawHide(viewerid, g_CarLic_Preview[viewerid]);
        PlayerTextDrawHide(viewerid, g_CarLic_Number[viewerid]);
        PlayerTextDrawHide(viewerid, g_CarLic_Name[viewerid]);
        PlayerTextDrawHide(viewerid, g_CarLic_DOB[viewerid]);
        PlayerTextDrawHide(viewerid, g_CarLic_Class[viewerid]);
    }

    g_CardCurrentTab[viewerid] = tab;

    if(tab == CARD_TAB_IDCARD)
        for(new i = 0; i < CARD_GLOBAL_COUNT; i++) TextDrawShowForPlayer(viewerid, g_IDCard_TD[i]);
    else
        for(new i = 0; i < CARD_GLOBAL_COUNT; i++) TextDrawShowForPlayer(viewerid, g_CarLic_TD[i]);

    PopulateCardTab(viewerid, tab);

    new str[32];
    format(str, sizeof(str), "So luong: %d/%d", tab + 1, g_CardTotalCount[viewerid]);
    PlayerTextDrawSetString(viewerid, g_TotalLicense_TD[viewerid], str);
}

// ===================================================================
// Public API
// ===================================================================
stock ShowCardTD(viewerid, ownerid)
{
    if(!IsPlayerConnected(viewerid)) return 0;

    new total = 0;
    if(PlayerInfo[ownerid][pIDCard] != 0) total++;
    if(PlayerInfo[ownerid][pCarLic] != 0) total++;
    if(total == 0) return 0;

    g_CardViewOwner[viewerid]  = ownerid;
    g_CardCurrentTab[viewerid] = CARD_TAB_IDCARD;
    g_CardTotalCount[viewerid] = total;

    if(!g_CardTDCreated[viewerid])
    {
        CreateCardTDPlayer(viewerid);
        g_CardTDCreated[viewerid] = true;
    }

    for(new i = 0; i < CARD_GLOBAL_COUNT; i++) TextDrawShowForPlayer(viewerid, g_IDCard_TD[i]);
    PlayerTextDrawShow(viewerid, g_CardNavNextLabel[viewerid]);
    PlayerTextDrawShow(viewerid, g_CardNavPrevLabel[viewerid]);

    PopulateCardTab(viewerid, CARD_TAB_IDCARD);

    PlayerTextDrawShow(viewerid, g_CardNavNext[viewerid]);
    PlayerTextDrawShow(viewerid, g_CardNavPrev[viewerid]);

    new str[32];
    format(str, sizeof(str), "So luong: 1/%d", total);
    PlayerTextDrawSetString(viewerid, g_TotalLicense_TD[viewerid], str);
    PlayerTextDrawShow(viewerid, g_TotalLicense_TD[viewerid]);

    PlayerPlaySound(viewerid, 17803, 0.0, 0.0, 0.0);
    SelectTextDraw(viewerid, 0xFFFFFF88);
    return 1;
}

stock HideCardTD(playerid)
{
    if(g_CardViewOwner[playerid] == -1) return 0;

    if(g_CardCurrentTab[playerid] == CARD_TAB_IDCARD)
        for(new i = 0; i < CARD_GLOBAL_COUNT; i++) TextDrawHideForPlayer(playerid, g_IDCard_TD[i]);
    else
        for(new i = 0; i < CARD_GLOBAL_COUNT; i++) TextDrawHideForPlayer(playerid, g_CarLic_TD[i]);

    PlayerTextDrawHide(playerid, g_CardNavNextLabel[playerid]);
    PlayerTextDrawHide(playerid, g_CardNavPrevLabel[playerid]);

    if(g_CardTDCreated[playerid]) DestroyCardTDPlayer(playerid);

    g_CardViewOwner[playerid]  = -1;
    g_CardCurrentTab[playerid] = CARD_TAB_IDCARD;
    g_CardTotalCount[playerid] = 0;
    CancelSelectTextDraw(playerid);
    return 1;
}

// ===================================================================
// Lenh /capcccd — canh sat cap CCCD cho nguoi dan tai don canh sat
// ===================================================================
CMD:capcccd(playerid, params[])
{
    if(!IsACop(playerid))
        return SendClientMessageEx(playerid, COLOR_LIGHTRED, "Ban khong phai canh sat de su dung lenh nay."),
               ShowTDN(playerid, NOTIFICATION_ERROR, "Ban khong phai canh sat de su dung lenh nay."), 1;

    if(!PlayerInfo[playerid][pDuty])
        return SendClientMessageEx(playerid, COLOR_LIGHTRED, "Ban phai thi hanh nhiem vu de thuc hien dieu nay. (/badge)"),
               ShowTDN(playerid, NOTIFICATION_ERROR, "Ban phai thi hanh nhiem vu de thuc hien dieu nay."), 1;

    if(!IsPlayerInRangeOfPoint(playerid, CARD_POLICE_RANGE, CARD_POLICE_X, CARD_POLICE_Y, CARD_POLICE_Z))
        return SendClientMessageEx(playerid, COLOR_LIGHTRED, "Ban phai o trong don canh sat de cap CCCD."),
               ShowTDN(playerid, NOTIFICATION_ERROR, "Ban phai o trong don canh sat de cap CCCD."), 1;

    new targetid;
    if(sscanf(params, "u", targetid))
        return SendHelpMessage(playerid, "/capcccd [ID/Ten nguoi choi]"),
               ShowTDN(playerid, NOTIFICATION_SYNTAX, "Su dung: /capcccd [ID/Ten nguoi choi]"), 1;

    if(!IsPlayerConnected(targetid))
        return SendClientMessageEx(playerid, COLOR_LIGHTRED, "Nguoi choi nay khong ket noi."),
               ShowTDN(playerid, NOTIFICATION_ERROR, "Nguoi choi nay khong ket noi."), 1;

    if(targetid == playerid)
        return SendClientMessageEx(playerid, COLOR_LIGHTRED, "Ban khong the cap CCCD cho chinh minh."),
               ShowTDN(playerid, NOTIFICATION_ERROR, "Ban khong the cap CCCD cho chinh minh."), 1;

    if(!IsPlayerInRangeOfPoint(targetid, 5.0, CARD_POLICE_X, CARD_POLICE_Y, CARD_POLICE_Z))
        return SendClientMessageEx(playerid, COLOR_LIGHTRED, "Nguoi dan phai co mat tai don canh sat."),
               ShowTDN(playerid, NOTIFICATION_ERROR, "Nguoi dan phai co mat tai don canh sat."), 1;

    if(PlayerInfo[targetid][pIDCard] != 0)
        return SendClientMessageEx(playerid, COLOR_LIGHTRED, "Nguoi choi nay da co Can cuoc cong dan roi."),
               ShowTDN(playerid, NOTIFICATION_ERROR, "Nguoi choi nay da co Can cuoc cong dan roi."), 1;

    if(!strcmp(PlayerInfo[targetid][pBirthDate], "00-00-0000"))
        return SendClientMessageEx(playerid, COLOR_LIGHTRED, "Nguoi choi nay chua dang ky ngay sinh."),
               ShowTDN(playerid, NOTIFICATION_ERROR, "Nguoi choi nay chua dang ky ngay sinh."), 1;

    CreateIDCard(targetid);
    UpdateNewbieMission(targetid, 0);

    SendClientMessageEx(targetid, COLOR_WHITE,
        "Ban da duoc cap {A3F15F}Can cuoc cong dan{FFFFFF} boi canh sat {6DB0E5}%s{FFFFFF}.",
        GetPlayerNameEx(playerid));
    SendClientMessageEx(playerid, COLOR_WHITE,
        "Da cap {A3F15F}Can cuoc cong dan{FFFFFF} thanh cong cho {6DB0E5}%s{FFFFFF}.",
        GetPlayerNameEx(targetid));

    new proxstring[128];
    format(proxstring, sizeof(proxstring),
        "{FF8000}* {C2A2DA}Canh sat %s da cap Can cuoc cong dan cho %s.",
        GetPlayerNameEx(playerid), GetPlayerNameEx(targetid));
    ProxDetectorWrap(playerid, proxstring, 92, 15.0, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE, COLOR_PURPLE);
    return 1;
}

// ===================================================================
// Hooks
// ===================================================================
hook OnGameModeInit()
{
    CreateCardTDGlobal();
    return 1;
}

hook OnPlayerConnect(playerid)
{
    g_CardNavNext[playerid]      = INVALID_PLAYER_TEXT_DRAW;
    g_CardNavPrev[playerid]      = INVALID_PLAYER_TEXT_DRAW;
    g_CardNavNextLabel[playerid] = INVALID_PLAYER_TEXT_DRAW;
    g_CardNavPrevLabel[playerid] = INVALID_PLAYER_TEXT_DRAW;
    g_TotalLicense_TD[playerid]  = INVALID_PLAYER_TEXT_DRAW;
    g_IDCard_Preview[playerid]  = INVALID_PLAYER_TEXT_DRAW;
    g_IDCard_Number[playerid]   = INVALID_PLAYER_TEXT_DRAW;
    g_IDCard_Name[playerid]     = INVALID_PLAYER_TEXT_DRAW;
    g_IDCard_DOB[playerid]      = INVALID_PLAYER_TEXT_DRAW;
    g_IDCard_Gender[playerid]   = INVALID_PLAYER_TEXT_DRAW;
    g_CarLic_Preview[playerid]  = INVALID_PLAYER_TEXT_DRAW;
    g_CarLic_Number[playerid]   = INVALID_PLAYER_TEXT_DRAW;
    g_CarLic_Name[playerid]     = INVALID_PLAYER_TEXT_DRAW;
    g_CarLic_DOB[playerid]      = INVALID_PLAYER_TEXT_DRAW;
    g_CarLic_Class[playerid]    = INVALID_PLAYER_TEXT_DRAW;
    g_CardTDCreated[playerid]   = false;
    g_CardViewOwner[playerid]   = -1;
    g_CardCurrentTab[playerid]  = CARD_TAB_IDCARD;
    g_CardTotalCount[playerid]  = 0;
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    if(g_CardViewOwner[playerid] != -1)
        HideCardTD(playerid);

    foreach(new i: Player)
    {
        if(g_CardViewOwner[i] == playerid)
            HideCardTD(i);
    }
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_CARD_VIEW_REQUEST)
    {
        new ownerid = GetPVarInt(playerid, "pCardViewFrom");
        DeletePVar(playerid, "pCardViewFrom");

        if(!response)
        {
            if(IsPlayerConnected(ownerid))
                SendClientMessageEx(ownerid, COLOR_GREY, "%s da tu choi xem giay to cua ban.", GetPlayerNameEx(playerid));
            return 1;
        }

        if(!IsPlayerConnected(ownerid))
            return SendClientMessageEx(playerid, COLOR_GREY, "Nguoi choi da mat ket noi."), 1;

        ShowCardTD(playerid, ownerid);

        // Giay to khong co textdraw - hien thi tren chatlog
        new licStr[144];
        new flyStr[32], boatStr[32], taxiStr[32], gunStr[32];

        if(PlayerInfo[ownerid][pFlyLic])      flyStr  = "{0EEF5D}Da so huu";  else flyStr  = "{C2A347}Chua so huu";
        if(PlayerInfo[ownerid][pBoatLic])     boatStr = "{0EEF5D}Da so huu";  else boatStr = "{C2A347}Chua so huu";
        if(PlayerInfo[ownerid][pTaxiLicense]) taxiStr = "{0EEF5D}Da so huu";  else taxiStr = "{C2A347}Chua so huu";
        if(PlayerInfo[ownerid][pGunLic])      gunStr  = "{0EEF5D}Da so huu";  else gunStr  = "{C2A347}Chua so huu";
        //if(PlayerInfo[ownerid][pFishLic])     fishStr = "{0EEF5D}Da so huu";  else fishStr = "{C2A347}Chua so huu";

        format(licStr, sizeof(licStr), "{FFFF00}[ Giay to khac cua %s ]", GetPlayerNameEx(ownerid));
        SendClientMessageEx(playerid, COLOR_WHITE, licStr);

        format(licStr, sizeof(licStr), "{FFFFFF}Giay phep May bay: %s{FFFFFF}  |  {FFFFFF}Giay phep Thuyen: %s", flyStr, boatStr);
        SendClientMessageEx(playerid, COLOR_WHITE, licStr);

        format(licStr, sizeof(licStr), "{FFFFFF}Giay phep Taxi: %s{FFFFFF}  |  {FFFFFF}Giay phep Sung: %s", taxiStr, gunStr);
        SendClientMessageEx(playerid, COLOR_WHITE, licStr);

        return 1;
    }
    return 1;
}

hook OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    if(g_CardViewOwner[playerid] == -1) return 0;
    if(clickedid == INVALID_TEXT_DRAW)
    {
        PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);
        HideCardTD(playerid);
        return 1;
    }
    return 0;
}

hook OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
    if(g_CardViewOwner[playerid] == -1) return 0;

    if(playertextid == g_CardNavNext[playerid])
    {
        if(g_CardCurrentTab[playerid] == CARD_TAB_IDCARD)
        {
            if(g_CardTotalCount[playerid] > 1)
            {
                PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);
                ShowCardTab(playerid, CARD_TAB_CARLICEN);
            }
            else
            {
                ShowTDN(playerid, NOTIFICATION_INFO, "Day la giay to duy nhat.");
                SendHintMessage(playerid, "Nguoi nay chi so huu 1 giay to.");
            }
        }
        else
        {
            ShowTDN(playerid, NOTIFICATION_INFO, "Day la giay to cuoi cung.");
            SendHintMessage(playerid, "Day la giay to cuoi cung.");
        }
        return 1;
    }

    if(playertextid == g_CardNavPrev[playerid])
    {
        if(g_CardCurrentTab[playerid] == CARD_TAB_CARLICEN)
        {
            PlayerPlaySound(playerid, 17803, 0.0, 0.0, 0.0);
            ShowCardTab(playerid, CARD_TAB_IDCARD);
        }
        else
        {
            ShowTDN(playerid, NOTIFICATION_INFO, "Day la giay to dau tien.");
            SendHintMessage(playerid, "Day la giay to dau tien.");
        }
        return 1;
    }
    return 0;
}
