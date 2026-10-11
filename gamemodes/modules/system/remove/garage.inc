new g_Blocker[22];
 
stock BlockGarages()
{
    g_Blocker[0] = CreateDynamicObject(19325, 1968.74219, 2162.49219, 12.09380,   0.00000, 0.00000, 0.00000);
    g_Blocker[1] = CreateDynamicObject(19325, 2006.00000, 2303.72656, 11.31250,   0.00000, 0.00000, 90.00000);
    g_Blocker[2] = CreateDynamicObject(19325, 720.01563, -462.52341, 16.85940,   0.00000, 0.00000, 90.00000);
    g_Blocker[3] = CreateDynamicObject(19325, -100.00000, 1111.41406, 21.14060,   0.00000, 0.00000, 90.00000);
    g_Blocker[4] = CreateDynamicObject(19325, -100.00000, 1111.41406, 22.14060,   0.00000, 0.00000, 90.00000);
    g_Blocker[5] = CreateDynamicObject(19325, -1420.52686, 2591.15747, 57.24220,   0.00000, 0.00000, 90.00000);
    g_Blocker[6] = CreateDynamicObject(19325, -1420.52686, 2591.15747, 58.24220,   0.00000, 0.00000, 90.00000);
    g_Blocker[7] = CreateDynamicObject(19325, 1843.36719, -1856.32031, 13.87500,   0.00000, 0.00000, 0.00000);
    g_Blocker[8] = CreateDynamicObject(19325, -2716.35156, 217.47659, 5.38280,   0.00000, 0.00000, 0.00000);
    g_Blocker[9] = CreateDynamicObject(19325, -1935.85938, 239.53130, 35.35160,   0.00000, 0.00000, 90.00000);
    g_Blocker[10] = CreateDynamicObject(19325, -1904.53125, 277.89841, 42.45310,   0.00000, 0.00000, 90.00000);
    g_Blocker[11] = CreateDynamicObject(19325, -1904.53125, 277.89841, 43.45310,   0.00000, 0.00000, 90.00000);
    g_Blocker[12] = CreateDynamicObject(19325, -1786.81250, 1209.42188, 25.83590,   0.00000, 0.00000, 90.00000);
    g_Blocker[13] = CreateDynamicObject(19325, 1798.68750, -2146.73438, 14.00000,   0.00000, 0.00000, 0.00000);
    g_Blocker[14] = CreateDynamicObject(19325, 2644.85938, -2039.23438, 14.03910,   0.00000, 0.00000, 90.00000);
    g_Blocker[15] = CreateDynamicObject(19325, 2071.47656, -1831.42188, 14.56250,   0.00000, 0.00000, 0.00000);
    g_Blocker[16] = CreateDynamicObject(19325, 488.28131, -1734.69531, 12.39060,   0.00000, 0.00000, 81.00000);
    g_Blocker[17] = CreateDynamicObject(19325, -2425.72656, 1027.99219, 51.78130,   0.00000, 0.00000, 90.00000);
    g_Blocker[18] = CreateDynamicObject(19325, -2425.72656, 1027.99219, 52.78130,   0.00000, 0.00000, 90.00000);
    g_Blocker[19] = CreateDynamicObject(19325, 2393.76563, 1483.68750, 12.21090,   0.00000, 0.00000, 90.00000);
    g_Blocker[20] = CreateDynamicObject(19325, 2393.76563, 1483.68750, 13.21090,   0.00000, 0.00000, 90.00000);
    g_Blocker[21] = CreateDynamicObject(19325, 2386.65625, 1043.60156, 11.59380,   0.00000, 0.00000, 90.00000);
    for(new i; i<sizeof(g_Blocker); i++) SetObjectMaterial(g_Blocker[i], 0, 0, "none", "none", 0x00FFFFFF);
}