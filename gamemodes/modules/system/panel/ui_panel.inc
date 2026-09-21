#include <YSI\YSI_Coding\y_hooks>

// All 80 panel TDs as global Text: — preserves original creation-order Z-layering.
// Selectable buttons at indices [5],[10],[12],[13],[14],[16],[18],[20].
new Text: PanelTD[80];

static CreatePanelAllTD()
{
    // [0] background overlay
    PanelTD[0] = TextDrawCreate(0.000, -1.000, "LD_SPAC:black");
    TextDrawTextSize(PanelTD[0], 641.000, 451.000);
    TextDrawAlignment(PanelTD[0], 1);
    TextDrawColor(PanelTD[0], -158);
    TextDrawSetShadow(PanelTD[0], 0);
    TextDrawSetOutline(PanelTD[0], 0);
    TextDrawBackgroundColor(PanelTD[0], 255);
    TextDrawFont(PanelTD[0], 4);
    TextDrawSetProportional(PanelTD[0], 1);

    // [1] "X" close label
    PanelTD[1] = TextDrawCreate(181.000, 277.000, "X");
    TextDrawLetterSize(PanelTD[1], 0.300, 1.500);
    TextDrawTextSize(PanelTD[1], -2.000, 0.000);
    TextDrawAlignment(PanelTD[1], 2);
    TextDrawColor(PanelTD[1], -602653441);
    TextDrawSetShadow(PanelTD[1], 0);
    TextDrawSetOutline(PanelTD[1], 0);
    TextDrawBackgroundColor(PanelTD[1], 150);
    TextDrawFont(PanelTD[1], 1);
    TextDrawSetProportional(PanelTD[1], 1);

    // [2] outer frame
    PanelTD[2] = TextDrawCreate(107.000, 181.000, "LD_SPAC:white");
    TextDrawTextSize(PanelTD[2], 33.000, 38.000);
    TextDrawAlignment(PanelTD[2], 1);
    TextDrawColor(PanelTD[2], -1697828097);
    TextDrawSetShadow(PanelTD[2], 0);
    TextDrawSetOutline(PanelTD[2], 0);
    TextDrawBackgroundColor(PanelTD[2], 255);
    TextDrawFont(PanelTD[2], 4);
    TextDrawSetProportional(PanelTD[2], 1);

    // [3]
    PanelTD[3] = TextDrawCreate(148.000, 181.000, "LD_SPAC:white");
    TextDrawTextSize(PanelTD[3], 33.000, 38.000);
    TextDrawAlignment(PanelTD[3], 1);
    TextDrawColor(PanelTD[3], -1697828097);
    TextDrawSetShadow(PanelTD[3], 0);
    TextDrawSetOutline(PanelTD[3], 0);
    TextDrawBackgroundColor(PanelTD[3], 255);
    TextDrawFont(PanelTD[3], 4);
    TextDrawSetProportional(PanelTD[3], 1);

    // [4]
    PanelTD[4] = TextDrawCreate(188.000, 181.000, "LD_SPAC:white");
    TextDrawTextSize(PanelTD[4], 33.000, 38.000);
    TextDrawAlignment(PanelTD[4], 1);
    TextDrawColor(PanelTD[4], -1697828097);
    TextDrawSetShadow(PanelTD[4], 0);
    TextDrawSetOutline(PanelTD[4], 0);
    TextDrawBackgroundColor(PanelTD[4], 255);
    TextDrawFont(PanelTD[4], 4);
    TextDrawSetProportional(PanelTD[4], 1);

    // [5] Dien thoai button — SELECTABLE
    PanelTD[5] = TextDrawCreate(108.000, 182.000, "LD_SPAC:white");
    TextDrawTextSize(PanelTD[5], 31.000, 27.000);
    TextDrawAlignment(PanelTD[5], 1);
    TextDrawColor(PanelTD[5], 2054847231);
    TextDrawSetShadow(PanelTD[5], 0);
    TextDrawSetOutline(PanelTD[5], 0);
    TextDrawBackgroundColor(PanelTD[5], 255);
    TextDrawFont(PanelTD[5], 4);
    TextDrawSetProportional(PanelTD[5], 1);
    TextDrawSetSelectable(PanelTD[5], 1);

    // [6]
    PanelTD[6] = TextDrawCreate(228.000, 181.000, "LD_SPAC:white");
    TextDrawTextSize(PanelTD[6], 33.000, 38.000);
    TextDrawAlignment(PanelTD[6], 1);
    TextDrawColor(PanelTD[6], -1697828097);
    TextDrawSetShadow(PanelTD[6], 0);
    TextDrawSetOutline(PanelTD[6], 0);
    TextDrawBackgroundColor(PanelTD[6], 255);
    TextDrawFont(PanelTD[6], 4);
    TextDrawSetProportional(PanelTD[6], 1);

    // [7]
    PanelTD[7] = TextDrawCreate(166.000, 229.000, "LD_SPAC:white");
    TextDrawTextSize(PanelTD[7], 33.000, 38.000);
    TextDrawAlignment(PanelTD[7], 1);
    TextDrawColor(PanelTD[7], -1697828097);
    TextDrawSetShadow(PanelTD[7], 0);
    TextDrawSetOutline(PanelTD[7], 0);
    TextDrawBackgroundColor(PanelTD[7], 255);
    TextDrawFont(PanelTD[7], 4);
    TextDrawSetProportional(PanelTD[7], 1);

    // [8]
    PanelTD[8] = TextDrawCreate(158.000, 277.000, "LD_SPAC:white");
    TextDrawTextSize(PanelTD[8], 46.000, 14.000);
    TextDrawAlignment(PanelTD[8], 1);
    TextDrawColor(PanelTD[8], -1697828097);
    TextDrawSetShadow(PanelTD[8], 0);
    TextDrawSetOutline(PanelTD[8], 0);
    TextDrawBackgroundColor(PanelTD[8], 255);
    TextDrawFont(PanelTD[8], 4);
    TextDrawSetProportional(PanelTD[8], 1);

    // [9]
    PanelTD[9] = TextDrawCreate(124.000, 229.000, "LD_SPAC:white");
    TextDrawTextSize(PanelTD[9], 33.000, 38.000);
    TextDrawAlignment(PanelTD[9], 1);
    TextDrawColor(PanelTD[9], -1697828097);
    TextDrawSetShadow(PanelTD[9], 0);
    TextDrawSetOutline(PanelTD[9], 0);
    TextDrawBackgroundColor(PanelTD[9], 255);
    TextDrawFont(PanelTD[9], 4);
    TextDrawSetProportional(PanelTD[9], 1);

    // [10] close button — SELECTABLE
    PanelTD[10] = TextDrawCreate(159.000, 278.000, "LD_SPAC:white");
    TextDrawTextSize(PanelTD[10], 44.000, 12.000);
    TextDrawAlignment(PanelTD[10], 1);
    TextDrawColor(PanelTD[10], 2054847231);
    TextDrawSetShadow(PanelTD[10], 0);
    TextDrawSetOutline(PanelTD[10], 0);
    TextDrawBackgroundColor(PanelTD[10], 255);
    TextDrawFont(PanelTD[10], 4);
    TextDrawSetProportional(PanelTD[10], 1);
    TextDrawSetSelectable(PanelTD[10], 1);

    // [11]
    PanelTD[11] = TextDrawCreate(207.000, 229.000, "LD_SPAC:white");
    TextDrawTextSize(PanelTD[11], 33.000, 38.000);
    TextDrawAlignment(PanelTD[11], 1);
    TextDrawColor(PanelTD[11], -1697828097);
    TextDrawSetShadow(PanelTD[11], 0);
    TextDrawSetOutline(PanelTD[11], 0);
    TextDrawBackgroundColor(PanelTD[11], 255);
    TextDrawFont(PanelTD[11], 4);
    TextDrawSetProportional(PanelTD[11], 1);

    // [12] timvieclam button — SELECTABLE
    PanelTD[12] = TextDrawCreate(208.000, 230.000, "LD_SPAC:white");
    TextDrawTextSize(PanelTD[12], 31.000, 27.000);
    TextDrawAlignment(PanelTD[12], 1);
    TextDrawColor(PanelTD[12], 2054847231);
    TextDrawSetShadow(PanelTD[12], 0);
    TextDrawSetOutline(PanelTD[12], 0);
    TextDrawBackgroundColor(PanelTD[12], 255);
    TextDrawFont(PanelTD[12], 4);
    TextDrawSetProportional(PanelTD[12], 1);
    TextDrawSetSelectable(PanelTD[12], 1);

    // [13] tuido (bottom row) button — SELECTABLE
    PanelTD[13] = TextDrawCreate(167.000, 230.000, "LD_SPAC:white");
    TextDrawTextSize(PanelTD[13], 31.000, 27.000);
    TextDrawAlignment(PanelTD[13], 1);
    TextDrawColor(PanelTD[13], 2054847231);
    TextDrawSetShadow(PanelTD[13], 0);
    TextDrawSetOutline(PanelTD[13], 0);
    TextDrawBackgroundColor(PanelTD[13], 255);
    TextDrawFont(PanelTD[13], 4);
    TextDrawSetProportional(PanelTD[13], 1);
    TextDrawSetSelectable(PanelTD[13], 1);

    // [14] timduong button — SELECTABLE
    PanelTD[14] = TextDrawCreate(125.000, 230.000, "LD_SPAC:white");
    TextDrawTextSize(PanelTD[14], 31.000, 27.000);
    TextDrawAlignment(PanelTD[14], 1);
    TextDrawColor(PanelTD[14], 2054847231);
    TextDrawSetShadow(PanelTD[14], 0);
    TextDrawSetOutline(PanelTD[14], 0);
    TextDrawBackgroundColor(PanelTD[14], 255);
    TextDrawFont(PanelTD[14], 4);
    TextDrawSetProportional(PanelTD[14], 1);
    TextDrawSetSelectable(PanelTD[14], 1);

    // [15]
    PanelTD[15] = TextDrawCreate(117.000, 183.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[15], 2.000, 23.000);
    TextDrawAlignment(PanelTD[15], 1);
    TextDrawColor(PanelTD[15], -1);
    TextDrawSetShadow(PanelTD[15], 0);
    TextDrawSetOutline(PanelTD[15], 0);
    TextDrawBackgroundColor(PanelTD[15], 255);
    TextDrawFont(PanelTD[15], 4);
    TextDrawSetProportional(PanelTD[15], 1);

    // [16] Hanh dong button — SELECTABLE
    PanelTD[16] = TextDrawCreate(229.000, 182.000, "LD_SPAC:white");
    TextDrawTextSize(PanelTD[16], 31.000, 27.000);
    TextDrawAlignment(PanelTD[16], 1);
    TextDrawColor(PanelTD[16], 2054847231);
    TextDrawSetShadow(PanelTD[16], 0);
    TextDrawSetOutline(PanelTD[16], 0);
    TextDrawBackgroundColor(PanelTD[16], 255);
    TextDrawFont(PanelTD[16], 4);
    TextDrawSetProportional(PanelTD[16], 1);
    TextDrawSetSelectable(PanelTD[16], 1);

    // [17]
    PanelTD[17] = TextDrawCreate(117.000, 183.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[17], 14.000, 2.000);
    TextDrawAlignment(PanelTD[17], 1);
    TextDrawColor(PanelTD[17], -1);
    TextDrawSetShadow(PanelTD[17], 0);
    TextDrawSetOutline(PanelTD[17], 0);
    TextDrawBackgroundColor(PanelTD[17], 255);
    TextDrawFont(PanelTD[17], 4);
    TextDrawSetProportional(PanelTD[17], 1);

    // [18] chinhxe button — SELECTABLE
    PanelTD[18] = TextDrawCreate(189.000, 182.000, "LD_SPAC:white");
    TextDrawTextSize(PanelTD[18], 31.000, 27.000);
    TextDrawAlignment(PanelTD[18], 1);
    TextDrawColor(PanelTD[18], 2054847231);
    TextDrawSetShadow(PanelTD[18], 0);
    TextDrawSetOutline(PanelTD[18], 0);
    TextDrawBackgroundColor(PanelTD[18], 255);
    TextDrawFont(PanelTD[18], 4);
    TextDrawSetProportional(PanelTD[18], 1);
    TextDrawSetSelectable(PanelTD[18], 1);

    // [19]
    PanelTD[19] = TextDrawCreate(117.000, 205.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[19], 14.000, 2.000);
    TextDrawAlignment(PanelTD[19], 1);
    TextDrawColor(PanelTD[19], -1);
    TextDrawSetShadow(PanelTD[19], 0);
    TextDrawSetOutline(PanelTD[19], 0);
    TextDrawBackgroundColor(PanelTD[19], 255);
    TextDrawFont(PanelTD[19], 4);
    TextDrawSetProportional(PanelTD[19], 1);

    // [20] tuido (top row) button — SELECTABLE
    PanelTD[20] = TextDrawCreate(149.000, 182.000, "LD_SPAC:white");
    TextDrawTextSize(PanelTD[20], 31.000, 27.000);
    TextDrawAlignment(PanelTD[20], 1);
    TextDrawColor(PanelTD[20], 2054847231);
    TextDrawSetShadow(PanelTD[20], 0);
    TextDrawSetOutline(PanelTD[20], 0);
    TextDrawBackgroundColor(PanelTD[20], 255);
    TextDrawFont(PanelTD[20], 4);
    TextDrawSetProportional(PanelTD[20], 1);
    TextDrawSetSelectable(PanelTD[20], 1);

    // [21]
    PanelTD[21] = TextDrawCreate(210.000, 202.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[21], 3.000, 6.000);
    TextDrawAlignment(PanelTD[21], 1);
    TextDrawColor(PanelTD[21], -1);
    TextDrawSetShadow(PanelTD[21], 0);
    TextDrawSetOutline(PanelTD[21], 0);
    TextDrawBackgroundColor(PanelTD[21], 255);
    TextDrawFont(PanelTD[21], 4);
    TextDrawSetProportional(PanelTD[21], 1);

    // [22]
    PanelTD[22] = TextDrawCreate(129.000, 183.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[22], 2.000, 23.000);
    TextDrawAlignment(PanelTD[22], 1);
    TextDrawColor(PanelTD[22], -1);
    TextDrawSetShadow(PanelTD[22], 0);
    TextDrawSetOutline(PanelTD[22], 0);
    TextDrawBackgroundColor(PanelTD[22], 255);
    TextDrawFont(PanelTD[22], 4);
    TextDrawSetProportional(PanelTD[22], 1);

    // [23]
    PanelTD[23] = TextDrawCreate(120.000, 187.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[23], 8.000, 1.000);
    TextDrawAlignment(PanelTD[23], 1);
    TextDrawColor(PanelTD[23], -1);
    TextDrawSetShadow(PanelTD[23], 0);
    TextDrawSetOutline(PanelTD[23], 0);
    TextDrawBackgroundColor(PanelTD[23], 255);
    TextDrawFont(PanelTD[23], 4);
    TextDrawSetProportional(PanelTD[23], 1);

    // [24]
    PanelTD[24] = TextDrawCreate(157.000, 193.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[24], 14.000, 11.000);
    TextDrawAlignment(PanelTD[24], 1);
    TextDrawColor(PanelTD[24], 1320741631);
    TextDrawSetShadow(PanelTD[24], 0);
    TextDrawSetOutline(PanelTD[24], 0);
    TextDrawBackgroundColor(PanelTD[24], 255);
    TextDrawFont(PanelTD[24], 4);
    TextDrawSetProportional(PanelTD[24], 1);

    // [25]
    PanelTD[25] = TextDrawCreate(158.000, 190.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[25], 12.000, 5.000);
    TextDrawAlignment(PanelTD[25], 1);
    TextDrawColor(PanelTD[25], 1320741631);
    TextDrawSetShadow(PanelTD[25], 0);
    TextDrawSetOutline(PanelTD[25], 0);
    TextDrawBackgroundColor(PanelTD[25], 255);
    TextDrawFont(PanelTD[25], 4);
    TextDrawSetProportional(PanelTD[25], 1);

    // [26]
    PanelTD[26] = TextDrawCreate(156.000, 193.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[26], 16.000, 9.000);
    TextDrawAlignment(PanelTD[26], 1);
    TextDrawColor(PanelTD[26], 1320741631);
    TextDrawSetShadow(PanelTD[26], 0);
    TextDrawSetOutline(PanelTD[26], 0);
    TextDrawBackgroundColor(PanelTD[26], 255);
    TextDrawFont(PanelTD[26], 4);
    TextDrawSetProportional(PanelTD[26], 1);

    // [27]
    PanelTD[27] = TextDrawCreate(155.000, 181.000, "LD_BEAT:chit");
    TextDrawTextSize(PanelTD[27], 18.000, 21.000);
    TextDrawAlignment(PanelTD[27], 1);
    TextDrawColor(PanelTD[27], 1320741631);
    TextDrawSetShadow(PanelTD[27], 0);
    TextDrawSetOutline(PanelTD[27], 0);
    TextDrawBackgroundColor(PanelTD[27], 255);
    TextDrawFont(PanelTD[27], 4);
    TextDrawSetProportional(PanelTD[27], 1);

    // [28]
    PanelTD[28] = TextDrawCreate(160.000, 193.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[28], 8.000, 9.000);
    TextDrawAlignment(PanelTD[28], 1);
    TextDrawColor(PanelTD[28], 2128609279);
    TextDrawSetShadow(PanelTD[28], 0);
    TextDrawSetOutline(PanelTD[28], 0);
    TextDrawBackgroundColor(PanelTD[28], 255);
    TextDrawFont(PanelTD[28], 4);
    TextDrawSetProportional(PanelTD[28], 1);

    // [29]
    PanelTD[29] = TextDrawCreate(160.000, 190.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[29], 8.000, 2.000);
    TextDrawAlignment(PanelTD[29], 1);
    TextDrawColor(PanelTD[29], 2128609279);
    TextDrawSetShadow(PanelTD[29], 0);
    TextDrawSetOutline(PanelTD[29], 0);
    TextDrawBackgroundColor(PanelTD[29], 255);
    TextDrawFont(PanelTD[29], 4);
    TextDrawSetProportional(PanelTD[29], 1);

    // [30]
    PanelTD[30] = TextDrawCreate(158.000, 186.000, "LD_BEAT:chit");
    TextDrawTextSize(PanelTD[30], 12.000, 7.000);
    TextDrawAlignment(PanelTD[30], 1);
    TextDrawColor(PanelTD[30], 2128609279);
    TextDrawSetShadow(PanelTD[30], 0);
    TextDrawSetOutline(PanelTD[30], 0);
    TextDrawBackgroundColor(PanelTD[30], 255);
    TextDrawFont(PanelTD[30], 4);
    TextDrawSetProportional(PanelTD[30], 1);

    // [31]
    PanelTD[31] = TextDrawCreate(161.000, 195.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[31], 6.000, 1.000);
    TextDrawAlignment(PanelTD[31], 1);
    TextDrawColor(PanelTD[31], 1320741631);
    TextDrawSetShadow(PanelTD[31], 0);
    TextDrawSetOutline(PanelTD[31], 0);
    TextDrawBackgroundColor(PanelTD[31], 255);
    TextDrawFont(PanelTD[31], 4);
    TextDrawSetProportional(PanelTD[31], 1);

    // [32]
    PanelTD[32] = TextDrawCreate(162.000, 195.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[32], -1.000, 3.000);
    TextDrawAlignment(PanelTD[32], 1);
    TextDrawColor(PanelTD[32], -1);
    TextDrawSetShadow(PanelTD[32], 0);
    TextDrawSetOutline(PanelTD[32], 0);
    TextDrawBackgroundColor(PanelTD[32], 255);
    TextDrawFont(PanelTD[32], 4);
    TextDrawSetProportional(PanelTD[32], 1);

    // [33]
    PanelTD[33] = TextDrawCreate(160.000, 203.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[33], -1.000, 3.000);
    TextDrawAlignment(PanelTD[33], 1);
    TextDrawColor(PanelTD[33], -1);
    TextDrawSetShadow(PanelTD[33], 0);
    TextDrawSetOutline(PanelTD[33], 0);
    TextDrawBackgroundColor(PanelTD[33], 255);
    TextDrawFont(PanelTD[33], 4);
    TextDrawSetProportional(PanelTD[33], 1);

    // [34]
    PanelTD[34] = TextDrawCreate(170.000, 203.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[34], -1.000, 3.000);
    TextDrawAlignment(PanelTD[34], 1);
    TextDrawColor(PanelTD[34], -1);
    TextDrawSetShadow(PanelTD[34], 0);
    TextDrawSetOutline(PanelTD[34], 0);
    TextDrawBackgroundColor(PanelTD[34], 255);
    TextDrawFont(PanelTD[34], 4);
    TextDrawSetProportional(PanelTD[34], 1);

    // [35]
    PanelTD[35] = TextDrawCreate(171.000, 196.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[35], -1.000, 3.000);
    TextDrawAlignment(PanelTD[35], 1);
    TextDrawColor(PanelTD[35], -1);
    TextDrawSetShadow(PanelTD[35], 0);
    TextDrawSetOutline(PanelTD[35], 0);
    TextDrawBackgroundColor(PanelTD[35], 255);
    TextDrawFont(PanelTD[35], 4);
    TextDrawSetProportional(PanelTD[35], 1);

    // [36]
    PanelTD[36] = TextDrawCreate(158.000, 196.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[36], -1.000, 3.000);
    TextDrawAlignment(PanelTD[36], 1);
    TextDrawColor(PanelTD[36], -1);
    TextDrawSetShadow(PanelTD[36], 0);
    TextDrawSetOutline(PanelTD[36], 0);
    TextDrawBackgroundColor(PanelTD[36], 255);
    TextDrawFont(PanelTD[36], 4);
    TextDrawSetProportional(PanelTD[36], 1);

    // [37]
    PanelTD[37] = TextDrawCreate(195.000, 195.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[37], 20.000, 9.000);
    TextDrawAlignment(PanelTD[37], 1);
    TextDrawColor(PanelTD[37], -1);
    TextDrawSetShadow(PanelTD[37], 0);
    TextDrawSetOutline(PanelTD[37], 0);
    TextDrawBackgroundColor(PanelTD[37], 255);
    TextDrawFont(PanelTD[37], 4);
    TextDrawSetProportional(PanelTD[37], 1);

    // [38]
    PanelTD[38] = TextDrawCreate(197.000, 186.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[38], 15.000, 2.000);
    TextDrawAlignment(PanelTD[38], 1);
    TextDrawColor(PanelTD[38], -1);
    TextDrawSetShadow(PanelTD[38], 0);
    TextDrawSetOutline(PanelTD[38], 0);
    TextDrawBackgroundColor(PanelTD[38], 255);
    TextDrawFont(PanelTD[38], 4);
    TextDrawSetProportional(PanelTD[38], 1);

    // [39]
    PanelTD[39] = TextDrawCreate(197.000, 202.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[39], 3.000, 6.000);
    TextDrawAlignment(PanelTD[39], 1);
    TextDrawColor(PanelTD[39], -1);
    TextDrawSetShadow(PanelTD[39], 0);
    TextDrawSetOutline(PanelTD[39], 0);
    TextDrawBackgroundColor(PanelTD[39], 255);
    TextDrawFont(PanelTD[39], 4);
    TextDrawSetProportional(PanelTD[39], 1);

    // [40] "/"
    PanelTD[40] = TextDrawCreate(194.000, 183.000, "/");
    TextDrawLetterSize(PanelTD[40], 0.300, 1.500);
    TextDrawTextSize(PanelTD[40], -2.000, 0.000);
    TextDrawAlignment(PanelTD[40], 1);
    TextDrawColor(PanelTD[40], -1);
    TextDrawSetShadow(PanelTD[40], 0);
    TextDrawSetOutline(PanelTD[40], 0);
    TextDrawBackgroundColor(PanelTD[40], 150);
    TextDrawFont(PanelTD[40], 1);
    TextDrawSetProportional(PanelTD[40], 1);

    // [41] "|"
    PanelTD[41] = TextDrawCreate(211.000, 183.000, "|");
    TextDrawLetterSize(PanelTD[41], 0.300, 1.500);
    TextDrawTextSize(PanelTD[41], -2.000, 0.000);
    TextDrawAlignment(PanelTD[41], 1);
    TextDrawColor(PanelTD[41], -1);
    TextDrawSetShadow(PanelTD[41], 0);
    TextDrawSetOutline(PanelTD[41], 0);
    TextDrawBackgroundColor(PanelTD[41], 150);
    TextDrawFont(PanelTD[41], 1);
    TextDrawSetProportional(PanelTD[41], 1);

    // [42]
    PanelTD[42] = TextDrawCreate(197.000, 197.000, "LD_BEAT:chit");
    TextDrawTextSize(PanelTD[42], 4.000, 5.000);
    TextDrawAlignment(PanelTD[42], 1);
    TextDrawColor(PanelTD[42], 163221759);
    TextDrawSetShadow(PanelTD[42], 0);
    TextDrawSetOutline(PanelTD[42], 0);
    TextDrawBackgroundColor(PanelTD[42], 255);
    TextDrawFont(PanelTD[42], 4);
    TextDrawSetProportional(PanelTD[42], 1);

    // [43]
    PanelTD[43] = TextDrawCreate(208.000, 197.000, "LD_BEAT:chit");
    TextDrawTextSize(PanelTD[43], 4.000, 5.000);
    TextDrawAlignment(PanelTD[43], 1);
    TextDrawColor(PanelTD[43], 163221759);
    TextDrawSetShadow(PanelTD[43], 0);
    TextDrawSetOutline(PanelTD[43], 0);
    TextDrawBackgroundColor(PanelTD[43], 255);
    TextDrawFont(PanelTD[43], 4);
    TextDrawSetProportional(PanelTD[43], 1);

    // [44]
    PanelTD[44] = TextDrawCreate(214.000, 194.000, "LD_BEAT:chit");
    TextDrawTextSize(PanelTD[44], 4.000, 5.000);
    TextDrawAlignment(PanelTD[44], 1);
    TextDrawColor(PanelTD[44], -1);
    TextDrawSetShadow(PanelTD[44], 0);
    TextDrawSetOutline(PanelTD[44], 0);
    TextDrawBackgroundColor(PanelTD[44], 255);
    TextDrawFont(PanelTD[44], 4);
    TextDrawSetProportional(PanelTD[44], 1);

    // [45]
    PanelTD[45] = TextDrawCreate(192.000, 194.000, "LD_BEAT:chit");
    TextDrawTextSize(PanelTD[45], 4.000, 5.000);
    TextDrawAlignment(PanelTD[45], 1);
    TextDrawColor(PanelTD[45], -1);
    TextDrawSetShadow(PanelTD[45], 0);
    TextDrawSetOutline(PanelTD[45], 0);
    TextDrawBackgroundColor(PanelTD[45], 255);
    TextDrawFont(PanelTD[45], 4);
    TextDrawSetProportional(PanelTD[45], 1);

    // [46]
    PanelTD[46] = TextDrawCreate(233.000, 181.000, "LD_BEAT:chit");
    TextDrawTextSize(PanelTD[46], 23.000, 27.000);
    TextDrawAlignment(PanelTD[46], 1);
    TextDrawColor(PanelTD[46], -1);
    TextDrawSetShadow(PanelTD[46], 0);
    TextDrawSetOutline(PanelTD[46], 0);
    TextDrawBackgroundColor(PanelTD[46], 255);
    TextDrawFont(PanelTD[46], 4);
    TextDrawSetProportional(PanelTD[46], 1);

    // [47]
    PanelTD[47] = TextDrawCreate(129.000, 246.000, "LD_BEAT:chit");
    TextDrawTextSize(PanelTD[47], 23.000, 10.000);
    TextDrawAlignment(PanelTD[47], 1);
    TextDrawColor(PanelTD[47], 1687547391);
    TextDrawSetShadow(PanelTD[47], 0);
    TextDrawSetOutline(PanelTD[47], 0);
    TextDrawBackgroundColor(PanelTD[47], 255);
    TextDrawFont(PanelTD[47], 4);
    TextDrawSetProportional(PanelTD[47], 1);

    // [48] "3"
    PanelTD[48] = TextDrawCreate(243.000, 193.000, "3");
    TextDrawLetterSize(PanelTD[48], 0.189, 0.898);
    TextDrawAlignment(PanelTD[48], 1);
    TextDrawColor(PanelTD[48], 1018393087);
    TextDrawSetShadow(PanelTD[48], 0);
    TextDrawSetOutline(PanelTD[48], 0);
    TextDrawBackgroundColor(PanelTD[48], 150);
    TextDrawFont(PanelTD[48], 1);
    TextDrawSetProportional(PanelTD[48], 1);

    // [49]
    PanelTD[49] = TextDrawCreate(134.000, 229.000, "LD_BEAT:chit");
    TextDrawTextSize(PanelTD[49], 13.000, 14.000);
    TextDrawAlignment(PanelTD[49], 1);
    TextDrawColor(PanelTD[49], -16776961);
    TextDrawSetShadow(PanelTD[49], 0);
    TextDrawSetOutline(PanelTD[49], 0);
    TextDrawBackgroundColor(PanelTD[49], 255);
    TextDrawFont(PanelTD[49], 4);
    TextDrawSetProportional(PanelTD[49], 1);

    // [50]
    PanelTD[50] = TextDrawCreate(136.000, 235.000, "LD_BEAT:chit");
    TextDrawTextSize(PanelTD[50], 9.000, 13.000);
    TextDrawAlignment(PanelTD[50], 1);
    TextDrawColor(PanelTD[50], -16776961);
    TextDrawSetShadow(PanelTD[50], 0);
    TextDrawSetOutline(PanelTD[50], 0);
    TextDrawBackgroundColor(PanelTD[50], 255);
    TextDrawFont(PanelTD[50], 4);
    TextDrawSetProportional(PanelTD[50], 1);

    // [51] "V"
    PanelTD[51] = TextDrawCreate(136.000, 232.000, "V");
    TextDrawLetterSize(PanelTD[51], 0.379, 2.598);
    TextDrawTextSize(PanelTD[51], -2.000, 0.000);
    TextDrawAlignment(PanelTD[51], 1);
    TextDrawColor(PanelTD[51], -16776961);
    TextDrawSetShadow(PanelTD[51], 0);
    TextDrawSetOutline(PanelTD[51], 0);
    TextDrawBackgroundColor(PanelTD[51], 150);
    TextDrawFont(PanelTD[51], 1);
    TextDrawSetProportional(PanelTD[51], 1);

    // [52]
    PanelTD[52] = TextDrawCreate(175.000, 235.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[52], 17.000, 19.000);
    TextDrawAlignment(PanelTD[52], 1);
    TextDrawColor(PanelTD[52], -1);
    TextDrawSetShadow(PanelTD[52], 0);
    TextDrawSetOutline(PanelTD[52], 0);
    TextDrawBackgroundColor(PanelTD[52], 255);
    TextDrawFont(PanelTD[52], 4);
    TextDrawSetProportional(PanelTD[52], 1);

    // [53]
    PanelTD[53] = TextDrawCreate(185.000, 233.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[53], 7.000, 2.000);
    TextDrawAlignment(PanelTD[53], 1);
    TextDrawColor(PanelTD[53], -1);
    TextDrawSetShadow(PanelTD[53], 0);
    TextDrawSetOutline(PanelTD[53], 0);
    TextDrawBackgroundColor(PanelTD[53], 255);
    TextDrawFont(PanelTD[53], 4);
    TextDrawSetProportional(PanelTD[53], 1);

    // [54] "$"
    PanelTD[54] = TextDrawCreate(184.000, 236.000, "$");
    TextDrawLetterSize(PanelTD[54], 0.300, 1.500);
    TextDrawTextSize(PanelTD[54], -2.000, 0.000);
    TextDrawAlignment(PanelTD[54], 2);
    TextDrawColor(PanelTD[54], 163221759);
    TextDrawSetShadow(PanelTD[54], 0);
    TextDrawSetOutline(PanelTD[54], 0);
    TextDrawBackgroundColor(PanelTD[54], 150);
    TextDrawFont(PanelTD[54], 1);
    TextDrawSetProportional(PanelTD[54], 1);

    // [55]
    PanelTD[55] = TextDrawCreate(177.000, 251.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[55], 13.000, 2.000);
    TextDrawAlignment(PanelTD[55], 1);
    TextDrawColor(PanelTD[55], 163221759);
    TextDrawSetShadow(PanelTD[55], 0);
    TextDrawSetOutline(PanelTD[55], 0);
    TextDrawBackgroundColor(PanelTD[55], 255);
    TextDrawFont(PanelTD[55], 4);
    TextDrawSetProportional(PanelTD[55], 1);

    // [56]
    PanelTD[56] = TextDrawCreate(186.000, 234.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[56], 5.000, 2.000);
    TextDrawAlignment(PanelTD[56], 1);
    TextDrawColor(PanelTD[56], 163221759);
    TextDrawSetShadow(PanelTD[56], 0);
    TextDrawSetOutline(PanelTD[56], 0);
    TextDrawBackgroundColor(PanelTD[56], 255);
    TextDrawFont(PanelTD[56], 4);
    TextDrawSetProportional(PanelTD[56], 1);

    // [57]
    PanelTD[57] = TextDrawCreate(213.000, 234.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[57], 11.000, 18.000);
    TextDrawAlignment(PanelTD[57], 1);
    TextDrawColor(PanelTD[57], 489901055);
    TextDrawSetShadow(PanelTD[57], 0);
    TextDrawSetOutline(PanelTD[57], 0);
    TextDrawBackgroundColor(PanelTD[57], 255);
    TextDrawFont(PanelTD[57], 4);
    TextDrawSetProportional(PanelTD[57], 1);

    // [58]
    PanelTD[58] = TextDrawCreate(223.000, 236.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[58], 11.000, 18.000);
    TextDrawAlignment(PanelTD[58], 1);
    TextDrawColor(PanelTD[58], 489901055);
    TextDrawSetShadow(PanelTD[58], 0);
    TextDrawSetOutline(PanelTD[58], 0);
    TextDrawBackgroundColor(PanelTD[58], 255);
    TextDrawFont(PanelTD[58], 4);
    TextDrawSetProportional(PanelTD[58], 1);

    // [59]
    PanelTD[59] = TextDrawCreate(214.000, 236.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[59], 9.000, 14.000);
    TextDrawAlignment(PanelTD[59], 1);
    TextDrawColor(PanelTD[59], -2459137);
    TextDrawSetShadow(PanelTD[59], 0);
    TextDrawSetOutline(PanelTD[59], 0);
    TextDrawBackgroundColor(PanelTD[59], 255);
    TextDrawFont(PanelTD[59], 4);
    TextDrawSetProportional(PanelTD[59], 1);

    // [60]
    PanelTD[60] = TextDrawCreate(224.000, 237.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[60], 9.000, 15.000);
    TextDrawAlignment(PanelTD[60], 1);
    TextDrawColor(PanelTD[60], 16777215);
    TextDrawSetShadow(PanelTD[60], 0);
    TextDrawSetOutline(PanelTD[60], 0);
    TextDrawBackgroundColor(PanelTD[60], 255);
    TextDrawFont(PanelTD[60], 4);
    TextDrawSetProportional(PanelTD[60], 1);

    // [61]
    PanelTD[61] = TextDrawCreate(216.000, 239.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[61], 6.000, 2.000);
    TextDrawAlignment(PanelTD[61], 1);
    TextDrawColor(PanelTD[61], 163221759);
    TextDrawSetShadow(PanelTD[61], 0);
    TextDrawSetOutline(PanelTD[61], 0);
    TextDrawBackgroundColor(PanelTD[61], 255);
    TextDrawFont(PanelTD[61], 4);
    TextDrawSetProportional(PanelTD[61], 1);

    // [62]
    PanelTD[62] = TextDrawCreate(216.000, 242.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[62], 6.000, 2.000);
    TextDrawAlignment(PanelTD[62], 1);
    TextDrawColor(PanelTD[62], 163221759);
    TextDrawSetShadow(PanelTD[62], 0);
    TextDrawSetOutline(PanelTD[62], 0);
    TextDrawBackgroundColor(PanelTD[62], 255);
    TextDrawFont(PanelTD[62], 4);
    TextDrawSetProportional(PanelTD[62], 1);

    // [63]
    PanelTD[63] = TextDrawCreate(216.000, 246.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[63], 6.000, 2.000);
    TextDrawAlignment(PanelTD[63], 1);
    TextDrawColor(PanelTD[63], 163221759);
    TextDrawSetShadow(PanelTD[63], 0);
    TextDrawSetOutline(PanelTD[63], 0);
    TextDrawBackgroundColor(PanelTD[63], 255);
    TextDrawFont(PanelTD[63], 4);
    TextDrawSetProportional(PanelTD[63], 1);

    // [64]
    PanelTD[64] = TextDrawCreate(226.000, 240.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[64], 6.000, 1.000);
    TextDrawAlignment(PanelTD[64], 1);
    TextDrawColor(PanelTD[64], 163221759);
    TextDrawSetShadow(PanelTD[64], 0);
    TextDrawSetOutline(PanelTD[64], 0);
    TextDrawBackgroundColor(PanelTD[64], 255);
    TextDrawFont(PanelTD[64], 4);
    TextDrawSetProportional(PanelTD[64], 1);

    // [65]
    PanelTD[65] = TextDrawCreate(226.000, 242.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[65], 6.000, 1.000);
    TextDrawAlignment(PanelTD[65], 1);
    TextDrawColor(PanelTD[65], 163221759);
    TextDrawSetShadow(PanelTD[65], 0);
    TextDrawSetOutline(PanelTD[65], 0);
    TextDrawBackgroundColor(PanelTD[65], 255);
    TextDrawFont(PanelTD[65], 4);
    TextDrawSetProportional(PanelTD[65], 1);

    // [66]
    PanelTD[66] = TextDrawCreate(226.000, 244.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[66], 6.000, 1.000);
    TextDrawAlignment(PanelTD[66], 1);
    TextDrawColor(PanelTD[66], 163221759);
    TextDrawSetShadow(PanelTD[66], 0);
    TextDrawSetOutline(PanelTD[66], 0);
    TextDrawBackgroundColor(PanelTD[66], 255);
    TextDrawFont(PanelTD[66], 4);
    TextDrawSetProportional(PanelTD[66], 1);

    // [67]
    PanelTD[67] = TextDrawCreate(226.000, 248.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[67], 6.000, 1.000);
    TextDrawAlignment(PanelTD[67], 1);
    TextDrawColor(PanelTD[67], 163221759);
    TextDrawSetShadow(PanelTD[67], 0);
    TextDrawSetOutline(PanelTD[67], 0);
    TextDrawBackgroundColor(PanelTD[67], 255);
    TextDrawFont(PanelTD[67], 4);
    TextDrawSetProportional(PanelTD[67], 1);

    // [68] "Vi tri"
    PanelTD[68] = TextDrawCreate(141.000, 257.000, "Vi tri");
    TextDrawLetterSize(PanelTD[68], 0.200, 0.998);
    TextDrawTextSize(PanelTD[68], -2.000, 27.000);
    TextDrawAlignment(PanelTD[68], 2);
    TextDrawColor(PanelTD[68], -1);
    TextDrawSetShadow(PanelTD[68], 0);
    TextDrawSetOutline(PanelTD[68], 0);
    TextDrawBackgroundColor(PanelTD[68], 150);
    TextDrawFont(PanelTD[68], 1);
    TextDrawSetProportional(PanelTD[68], 1);

    // [69] "Hoa don"
    PanelTD[69] = TextDrawCreate(183.000, 257.000, "Hoa don");
    TextDrawLetterSize(PanelTD[69], 0.180, 0.898);
    TextDrawTextSize(PanelTD[69], -2.000, 33.000);
    TextDrawAlignment(PanelTD[69], 2);
    TextDrawColor(PanelTD[69], -1);
    TextDrawSetShadow(PanelTD[69], 0);
    TextDrawSetOutline(PanelTD[69], 0);
    TextDrawBackgroundColor(PanelTD[69], 150);
    TextDrawFont(PanelTD[69], 1);
    TextDrawSetProportional(PanelTD[69], 1);

    // [70] "Viec lam"
    PanelTD[70] = TextDrawCreate(224.000, 257.000, "Viec lam");
    TextDrawLetterSize(PanelTD[70], 0.189, 0.898);
    TextDrawTextSize(PanelTD[70], -2.000, 37.000);
    TextDrawAlignment(PanelTD[70], 2);
    TextDrawColor(PanelTD[70], -1);
    TextDrawSetShadow(PanelTD[70], 0);
    TextDrawSetOutline(PanelTD[70], 0);
    TextDrawBackgroundColor(PanelTD[70], 150);
    TextDrawFont(PanelTD[70], 1);
    TextDrawSetProportional(PanelTD[70], 1);

    // [71] "Hanh dong"
    PanelTD[71] = TextDrawCreate(245.000, 209.000, "Hanh dong");
    TextDrawLetterSize(PanelTD[71], 0.180, 0.898);
    TextDrawTextSize(PanelTD[71], 5.000, 39.000);
    TextDrawAlignment(PanelTD[71], 2);
    TextDrawColor(PanelTD[71], -1);
    TextDrawSetShadow(PanelTD[71], 0);
    TextDrawSetOutline(PanelTD[71], 0);
    TextDrawBackgroundColor(PanelTD[71], 150);
    TextDrawFont(PanelTD[71], 1);
    TextDrawSetProportional(PanelTD[71], 1);

    // [72] "Phuong tien"
    PanelTD[72] = TextDrawCreate(204.600, 209.000, "Phuong tien");
    TextDrawLetterSize(PanelTD[72], 0.158, 0.898);
    TextDrawTextSize(PanelTD[72], 0.000, 48.000);
    TextDrawAlignment(PanelTD[72], 2);
    TextDrawColor(PanelTD[72], -1);
    TextDrawSetShadow(PanelTD[72], 0);
    TextDrawSetOutline(PanelTD[72], 0);
    TextDrawBackgroundColor(PanelTD[72], 150);
    TextDrawFont(PanelTD[72], 1);
    TextDrawSetProportional(PanelTD[72], 1);

    // [73] "Tui do"
    PanelTD[73] = TextDrawCreate(164.000, 209.000, "Tui do");
    TextDrawLetterSize(PanelTD[73], 0.200, 0.998);
    TextDrawTextSize(PanelTD[73], 78.000, 30.000);
    TextDrawAlignment(PanelTD[73], 2);
    TextDrawColor(PanelTD[73], -1);
    TextDrawSetShadow(PanelTD[73], 0);
    TextDrawSetOutline(PanelTD[73], 0);
    TextDrawBackgroundColor(PanelTD[73], 150);
    TextDrawFont(PanelTD[73], 1);
    TextDrawSetProportional(PanelTD[73], 1);

    // [74] "Dien thoai"
    PanelTD[74] = TextDrawCreate(124.000, 209.000, "Dien thoai");
    TextDrawLetterSize(PanelTD[74], 0.170, 0.898);
    TextDrawTextSize(PanelTD[74], 60.000, 39.000);
    TextDrawAlignment(PanelTD[74], 2);
    TextDrawColor(PanelTD[74], -1);
    TextDrawSetShadow(PanelTD[74], 0);
    TextDrawSetOutline(PanelTD[74], 0);
    TextDrawBackgroundColor(PanelTD[74], 150);
    TextDrawFont(PanelTD[74], 1);
    TextDrawSetProportional(PanelTD[74], 1);

    // [75]
    PanelTD[75] = TextDrawCreate(130.000, 186.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[75], 2.000, 4.000);
    TextDrawAlignment(PanelTD[75], 1);
    TextDrawColor(PanelTD[75], -1);
    TextDrawSetShadow(PanelTD[75], 0);
    TextDrawSetOutline(PanelTD[75], 0);
    TextDrawBackgroundColor(PanelTD[75], 255);
    TextDrawFont(PanelTD[75], 4);
    TextDrawSetProportional(PanelTD[75], 1);

    // [76]
    PanelTD[76] = TextDrawCreate(130.000, 192.000, "LD_BUM:blkdot");
    TextDrawTextSize(PanelTD[76], 2.000, 4.000);
    TextDrawAlignment(PanelTD[76], 1);
    TextDrawColor(PanelTD[76], -1);
    TextDrawSetShadow(PanelTD[76], 0);
    TextDrawSetOutline(PanelTD[76], 0);
    TextDrawBackgroundColor(PanelTD[76], 255);
    TextDrawFont(PanelTD[76], 4);
    TextDrawSetProportional(PanelTD[76], 1);

    // [77]
    PanelTD[77] = TextDrawCreate(239.000, 190.000, "LD_BEAT:chit");
    TextDrawTextSize(PanelTD[77], 4.000, 4.000);
    TextDrawAlignment(PanelTD[77], 1);
    TextDrawColor(PanelTD[77], 1018393087);
    TextDrawSetShadow(PanelTD[77], 0);
    TextDrawSetOutline(PanelTD[77], 0);
    TextDrawBackgroundColor(PanelTD[77], 255);
    TextDrawFont(PanelTD[77], 4);
    TextDrawSetProportional(PanelTD[77], 1);

    // [78]
    PanelTD[78] = TextDrawCreate(246.000, 190.000, "LD_BEAT:chit");
    TextDrawTextSize(PanelTD[78], 4.000, 4.000);
    TextDrawAlignment(PanelTD[78], 1);
    TextDrawColor(PanelTD[78], 1018393087);
    TextDrawSetShadow(PanelTD[78], 0);
    TextDrawSetOutline(PanelTD[78], 0);
    TextDrawBackgroundColor(PanelTD[78], 255);
    TextDrawFont(PanelTD[78], 4);
    TextDrawSetProportional(PanelTD[78], 1);

    // [79]
    PanelTD[79] = TextDrawCreate(137.000, 233.000, "LD_BEAT:chit");
    TextDrawTextSize(PanelTD[79], 7.000, 7.000);
    TextDrawAlignment(PanelTD[79], 1);
    TextDrawColor(PanelTD[79], -2139062017);
    TextDrawSetShadow(PanelTD[79], 0);
    TextDrawSetOutline(PanelTD[79], 0);
    TextDrawBackgroundColor(PanelTD[79], 255);
    TextDrawFont(PanelTD[79], 4);
    TextDrawSetProportional(PanelTD[79], 1);
}

hook OnGameModeInit()
{
    CreatePanelAllTD();
    return 1;
}
