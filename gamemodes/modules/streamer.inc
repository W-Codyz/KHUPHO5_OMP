forward LoadStreamerStaticVehicles();
public LoadStreamerStaticVehicles()
{
    /* TruckerVehicles[0] = AddStaticVehicleEx(414,-1572.34008789,129.09317017,3.69948244,225.00000000,-1,-1,300); //Mule
	TruckerVehicles[1] = AddStaticVehicleEx(414,-1575.85583496,125.90543365,3.69948244,225.00000000,-1,-1,300); //Mule
	TruckerVehicles[2] = AddStaticVehicleEx(414,-1578.88806152,122.54198456,3.69948244,225.00000000,-1,-1,300); //Mule
	TruckerVehicles[3] = AddStaticVehicleEx(414,-1581.52319336,119.51720428,3.69948244,225.00000000,-1,-1,300); //Mule
	TruckerVehicles[4] = AddStaticVehicleEx(414,-1584.36132812,116.68750000,3.69948244,225.00000000,-1,-1,300); //Mule
	TruckerVehicles[5] = AddStaticVehicleEx(456,-1588.0071,112.8444,3.7230,225.0000,-1,-1,300); //Yankee
	TruckerVehicles[6] = AddStaticVehicleEx(456,-1591.9919,108.2651,3.7230,225.0000,-1,-1,300); //Yankee
	TruckerVehicles[7] = AddStaticVehicleEx(456,-1615.8422,77.3187,3.7234,135.0000,-1,-1,300); //Yankee
	TruckerVehicles[8] = AddStaticVehicleEx(456,-1625.7900,67.1034,3.7250,135.0000,-1,-1,300); //Yankee */
	
	for(new i = 0; i < sizeof(TruckerVehicles); i++)
	{
		new plate[12];
		format(plate, _, "TRUCKER %02d", i + 1);
		SetVehicleNumberPlate(TruckerVehicles[i], plate);
	}

	PizzaVehicles[0] = AddStaticVehicleEx(448,-1701.99133301,1378.75817871,6.85771942,45.00000000,3,6,300); //Pizzaboy
	PizzaVehicles[1] =	AddStaticVehicleEx(448,-1702.81005859,1378.07958984,6.85771942,45.00000000,3,6,300); //Pizzaboy
	PizzaVehicles[2] =	AddStaticVehicleEx(448,-1703.49853516,1377.28295898,6.85771942,45.00000000,3,6,300); //Pizzaboy
	PizzaVehicles[3] =	AddStaticVehicleEx(448,-1704.31005859,1376.46240234,6.85771942,45.00000000,3,6,300); //Pizzaboy
	PizzaVehicles[4] =	AddStaticVehicleEx(448,-1705.17895508,1375.72802734,6.85771942,45.00000000,3,6,300); //Pizzaboy
	PizzaVehicles[5] =	AddStaticVehicleEx(448,-1709.15429688,1371.57226562,6.85771942,45.00000000,3,6,300); //Pizzaboy
	PizzaVehicles[6] =	AddStaticVehicleEx(448,-1709.98718262,1370.84716797,6.85771942,45.00000000,3,6,300); //Pizzaboy
	PizzaVehicles[7] =	AddStaticVehicleEx(448,-1710.77893066,1370.11706543,6.85771942,45.00000000,3,6,300); //Pizzaboy
	PizzaVehicles[8] =	AddStaticVehicleEx(448,-1711.57092285,1369.38659668,6.85771942,45.00000000,3,6,300); //Pizzaboy
	PizzaVehicles[9] =	AddStaticVehicleEx(448,-1712.32312012,1368.69396973,6.85771942,45.00000000,3,6,300); //Pizzaboy
	PizzaVehicles[10] =	AddStaticVehicleEx(448,-1713.02709961,1367.94238281,6.85771942,45.00000000,3,6,300); //Pizzaboy
	PizzaVehicles[11] =	AddStaticVehicleEx(448,-1713.83740234,1367.19580078,6.85771942,45.00000000,3,6,300); //Pizzaboy
	
	for(new i = 0; i < sizeof(PizzaVehicles); i++)
	{
		new plate[12];
		format(plate, _, "PIZZA %02d", i + 1);
		SetVehicleNumberPlate(PizzaVehicles[i], plate);
	}

	SweeperVehicles[0] = AddStaticVehicleEx(574,-1656.4377,457.5047,6.9049,134.4184,1,1,300); // 
	SweeperVehicles[1] = AddStaticVehicleEx(574,-1654.6307,455.6168,6.9049,134.0974,1,1,300); // 
	SweeperVehicles[2] = AddStaticVehicleEx(574,-1652.6704,453.5923,6.9049,133.4167,1,1,300); //
	SweeperVehicles[3] = AddStaticVehicleEx(574,-1650.6864,451.5199,6.9056,135.3742,1,1,300); // 
	SweeperVehicles[4] = AddStaticVehicleEx(574,-1648.6034,449.5022,6.9049,136.4296,1,1,300); // 

	for(new i = 0; i < sizeof(SweeperVehicles); i++)
	{
		new plate[12];
		format(plate, _, "SWEEPER %02d", i + 1);
		SetVehicleNumberPlate(SweeperVehicles[i], plate);
	}

	/*TruckerVehicles[9] = AddStaticVehicleEx(456,-1599.55334473,102.00447845,3.79948235,225.00000000,-1,-1,300); //Yankee
	TruckerVehicles[10] = AddStaticVehicleEx(456,-1603.49829102,98.16529083,3.79948235,225.00000000,-1,-1,300); //Yankee
	TruckerVehicles[11] = AddStaticVehicleEx(456,-1606.95983887,94.76251984,3.79948235,225.00000000,-1,-1,300); //Yankee
	TruckerVehicles[12] = AddStaticVehicleEx(456,-1610.01025391,91.14582062,3.79948235,225.00000000,-1,-1,300); //Yankee
	TruckerVehicles[13] = AddStaticVehicleEx(443,-1614.05834961,87.06182861,3.79948235,225.00000000,-1,-1,300); //Packer
	TruckerVehicles[14] = AddStaticVehicleEx(443,-1617.96765137,83.13320160,3.79948235,225.00000000,-1,-1,300); //Packer
	TruckerVehicles[15] = AddStaticVehicleEx(443,-1621.37426758,79.81375122,3.79948235,225.00000000,-1,-1,300); //Packer
	TruckerVehicles[16] = AddStaticVehicleEx(443,-1625.06359863,75.71626282,3.69948244,225.00000000,-1,-1,300); //Tank truck
	TruckerVehicles[17] = AddStaticVehicleEx(443,-1628.41735840,72.35322571,3.69948244,225.00000000,-1,-1,300); //Tank truck
	TruckerVehicles[18] = AddStaticVehicleEx(443,-1631.25317383,69.12777710,3.69948244,225.00000000,-1,-1,300); //Tank truck*/

	/*AddStaticVehicleEx(593,382.5172,2536.9561,16.5338,179.7261,-1,-1,2000);//Flyschool
	AddStaticVehicleEx(513,348.3660,2536.9792,16.7418,178.8195,-1,-1,2000);//Flyschool
	AddStaticVehicleEx(487,365.4068,2537.0327,16.6645,181.9959,-1,-1,2000);//Flyschool
	AddStaticVehicleEx(593,-1429.2843,-505.8203,13.7824,205.7709,-1,-1,2000);//Airport SF
	AddStaticVehicleEx(593,-1462.4779,-524.2803,13.7832,204.8137,-1,-1,2000);//Airport SF
	AddStaticVehicleEx(593,1907.7614,-2624.4097,13.5416,53.1810,-1,-1,2000);//Airport LS
	AddStaticVehicleEx(593,1887.0736,-2627.6543,13.5394,52.6116,-1,-1,2000);//Airport LS
	AddStaticVehicleEx(513,-1354.3270,-466.5467,13.7827,162.0134,-1,-1,2000);//Airport SF
	AddStaticVehicleEx(513,-1390.3510,-482.7985,13.7883,250.3124,-1,-1,2000);//Airport SF
	AddStaticVehicleEx(513,1835.3215,-2624.1938,13.5405,56.0569,-1,-1,2000);//Airport LS
	AddStaticVehicleEx(513,1813.2893,-2625.6296,13.5394,53.2858,-1,-1,2000);//Airport LS
	AddStaticVehicleEx(487,1963.29,-2647.07,13.76,21.93,-1,-1, 2000); // Maverick
	AddStaticVehicleEx(487,1948.81,-2647.30,13.68,21.93,-1,-1, 2000); // Maverick*/


	// VIP garage (respawn_delay = -1: tat auto-respawn)
	VIPVehicles[0] = AddStaticVehicleEx(411,-4365.93212891,839.28680420,986.18029785,0.00000000,-1,-1,-1); //Infernus
	VIPVehicles[1] = AddStaticVehicleEx(429,-4370.52832031,840.57843018,986.13031006,0.00000000,-1,-1,-1); //Banshee
	VIPVehicles[2] = AddStaticVehicleEx(451,-4374.41894531,840.36810303,986.14465332,0.00000000,-1,-1,-1); //Turismo
	VIPVehicles[3] = AddStaticVehicleEx(541,-4378.48046875,840.84783936,986.08032227,0.00000000,-1,-1,-1); //Bullet
	VIPVehicles[4] = AddStaticVehicleEx(559,-4382.43701172,840.60235596,986.13439941,0.00000000,-1,-1,-1); //Jester
	VIPVehicles[5] = AddStaticVehicleEx(560,-4386.68066406,841.29382324,986.18530273,0.00000000,-1,-1,-1); //Sultan
	VIPVehicles[6] = AddStaticVehicleEx(603,-4390.54345703,841.60748291,986.38299561,0.00000000,-1,-1,-1); //Phoenix
	VIPVehicles[7] = AddStaticVehicleEx(480,-4394.61035156,841.88873291,986.23028564,0.00000000,-1,-1,-1); //Comet
	VIPVehicles[8] = AddStaticVehicleEx(506,-4399.16455078,842.31146240,986.17242432,0.00000000,-1,-1,-1); //Super GT
	VIPVehicles[9] = AddStaticVehicleEx(587,-4402.89990234,842.34979248,986.19030762,0.00000000,-1,-1,-1); //Euros
	VIPVehicles[10] = AddStaticVehicleEx(411,-4407.21337891,842.98492432,986.18029785,0.00000000,-1,-1,-1); //Infernus
	VIPVehicles[11] = AddStaticVehicleEx(429,-4410.96923828,843.07391357,986.13031006,0.00000000,-1,-1,-1); //Banshee
	VIPVehicles[12] = AddStaticVehicleEx(451,-4415.64257812,843.46972656,986.14465332,0.00000000,-1,-1,-1); //Turismo
	VIPVehicles[13] = AddStaticVehicleEx(541,-4419.40478516,843.48645020,986.08032227,0.00000000,-1,-1,-1); //Bullet
	VIPVehicles[14] = AddStaticVehicleEx(559,-4423.40332031,843.19854736,986.13439941,0.00000000,-1,-1,-1); //Jester
	VIPVehicles[15] = AddStaticVehicleEx(560,-4427.88232422,843.82849121,986.18530273,0.00000000,-1,-1,-1); //Sultan
	VIPVehicles[16] = AddStaticVehicleEx(603,-4431.95849609,844.09509277,986.38299561,0.00000000,-1,-1,-1); //Phoenix
	VIPVehicles[17] = AddStaticVehicleEx(480,-4436.15429688,844.76721191,986.23028564,0.00000000,-1,-1,-1); //Comet
	VIPVehicles[18] = AddStaticVehicleEx(506,-4441.21337891,850.94598389,986.17242432,270.00000000,-1,-1,-1); //Super GT
	VIPVehicles[19] = AddStaticVehicleEx(587,-4441.05224609,854.70550537,986.19030762,270.00000000,-1,-1,-1); //Euros
	VIPVehicles[20] = AddStaticVehicleEx(522,-4418.77490234,858.65576172,986.04071045,180.00000000,-1,-1,-1); //NRG-500
	VIPVehicles[21] = AddStaticVehicleEx(522,-4414.77441406,858.50390625,986.04071045,180.00000000,-1,-1,-1); //NRG-500
	VIPVehicles[22] = AddStaticVehicleEx(522,-4410.77441406,858.35253906,986.04071045,180.00000000,-1,-1,-1); //NRG-500
	VIPVehicles[23] = AddStaticVehicleEx(522,-4406.30957031,857.78234863,986.04071045,180.00000000,-1,-1,-1); //NRG-500
	VIPVehicles[24] = AddStaticVehicleEx(522,-4394.11425781,856.77416992,986.04071045,180.00000000,-1,-1,-1); //NRG-500
	VIPVehicles[25] = AddStaticVehicleEx(522,-4390.35644531,856.70147705,986.04071045,180.00000000,-1,-1,-1); //NRG-500
	VIPVehicles[26] = AddStaticVehicleEx(522,-4386.21923828,856.22369385,986.04071045,180.00000000,-1,-1,-1); //NRG-500
	VIPVehicles[27] = AddStaticVehicleEx(522,-4382.18164062,855.81323242,986.04071045,180.00000000,-1,-1,-1); //NRG-500
	VIPVehicles[28] = AddStaticVehicleEx(522,-4417.77832031,876.99304199,986.04071045,0.00000000,-1,-1,-1); //NRG-500
	VIPVehicles[29] = AddStaticVehicleEx(522,-4414.06494141,876.46301270,986.04071045,0.00000000,-1,-1,-1); //NRG-500
	VIPVehicles[30] = AddStaticVehicleEx(522,-4409.78320312,876.41296387,986.04071045,0.00000000,-1,-1,-1); //NRG-500
	VIPVehicles[31] = AddStaticVehicleEx(522,-4405.26074219,876.33081055,986.04071045,0.00000000,-1,-1,-1); //NRG-500
	VIPVehicles[32] = AddStaticVehicleEx(522,-4393.28417969,875.69360352,986.04071045,0.00000000,-1,-1,-1); //NRG-500
	VIPVehicles[33] = AddStaticVehicleEx(522,-4389.04248047,875.41735840,986.04071045,0.00000000,-1,-1,-1); //NRG-500
	VIPVehicles[34] = AddStaticVehicleEx(522,-4385.04980469,875.21435547,986.04071045,0.00000000,-1,-1,-1); //NRG-500
	VIPVehicles[35] = AddStaticVehicleEx(522,-4381.05761719,875.00488281,986.04071045,0.00000000,-1,-1,-1); //NRG-500
	VIPVehicles[36] = AddStaticVehicleEx(411,-4407.94287109,891.34375000,986.18029785,180.00000000,-1,-1,-1); //Infernus
	VIPVehicles[37] = AddStaticVehicleEx(411,-4403.66748047,891.17761230,986.18029785,180.00000000,-1,-1,-1); //Infernus
	VIPVehicles[38] = AddStaticVehicleEx(411,-4399.91601562,891.28747559,986.18029785,180.00000000,-1,-1,-1); //Infernus
	VIPVehicles[39] = AddStaticVehicleEx(411,-4395.91601562,891.31347656,986.18029785,180.00000000,-1,-1,-1); //Infernus
	VIPVehicles[40] = AddStaticVehicleEx(451,-4391.23242188,891.03985596,986.14465332,180.00000000,-1,-1,-1); //Turismo
	VIPVehicles[41] = AddStaticVehicleEx(451,-4387.48339844,891.00421143,986.14465332,180.00000000,-1,-1,-1); //Turismo
	VIPVehicles[42] = AddStaticVehicleEx(451,-4383.49218750,890.44665527,986.14465332,180.00000000,-1,-1,-1); //Turismo
	VIPVehicles[43] = AddStaticVehicleEx(429,-4374.92187500,890.17211914,986.13031006,180.00000000,-1,-1,-1); //Banshee
	VIPVehicles[44] = AddStaticVehicleEx(429,-4370.80273438,889.91503906,986.13031006,180.00000000,-1,-1,-1); //Banshee
	VIPVehicles[45] = AddStaticVehicleEx(429,-4379.33349609,890.09112549,986.13031006,180.00000000,-1,-1,-1); //Banshee
	VIPVehicles[46] = AddStaticVehicleEx(560,-4366.63867188,889.44537354,986.18530273,180.00000000,-1,-1,-1); //Sultan
	VIPVehicles[47] = AddStaticVehicleEx(560,-4358.66015625,888.96386719,986.18530273,180.00000000,-1,-1,-1); //Sultan
	VIPVehicles[48] = AddStaticVehicleEx(560,-4354.67675781,888.44500732,986.18530273,180.00000000,-1,-1,-1); //Sultan
	VIPVehicles[49] = AddStaticVehicleEx(560,-4362.83789062,889.30908203,986.18530273,180.00000000,-1,-1,-1); //Sultan
	
	//Sultan
	FamedVehicles[0] = AddStaticVehicleEx(560,2515.6797,2381.6501,3.9175,90.5219,-1,-1,180);
	FamedVehicles[1] = AddStaticVehicleEx(560,2515.9753,2372.6431,3.9167,89.3516,-1,-1,180);
	FamedVehicles[2] = AddStaticVehicleEx(560,2515.2947,2363.1985,3.9228,89.3915,-1,-1,180);
	//Cheetah
	FamedVehicles[3] = AddStaticVehicleEx(415,2515.2170,2377.3411,3.9823,90.2256,-1,-1,180);
	FamedVehicles[4] = AddStaticVehicleEx(415,2515.2529,2368.2183,3.9826,90.2256,-1,-1,180);
	//NRG
	FamedVehicles[5] = AddStaticVehicleEx(522,2498.9292,2363.5708,3.7897,242.9848,-1,-1,180);
	FamedVehicles[6] = AddStaticVehicleEx(522,2499.3206,2366.6091,3.8081,242.9848,-1,-1,180);
	FamedVehicles[7] = AddStaticVehicleEx(522,2499.4883,2370.2681,3.8290,242.9847,-1,-1,180);
	FamedVehicles[8] = AddStaticVehicleEx(522,2499.3398,2382.6699,3.8842,242.9847,-1,-1,180);
	FamedVehicles[9] = AddStaticVehicleEx(522,2499.2588,2385.4790,3.7989,242.9847,-1,-1,180);
	FamedVehicles[10] = AddStaticVehicleEx(522,2499.0728,2390.9985,3.8373,242.9846,-1,-1,180);
	//PCJ
	FamedVehicles[11] = AddStaticVehicleEx(461,2494.5908,2353.1921,3.7971,328.9557,-1,-1,180);
	FamedVehicles[12] = AddStaticVehicleEx(461,2497.6455,2352.8145,3.7880,328.9523,-1,-1,180);
	FamedVehicles[13] = AddStaticVehicleEx(461,2501.5059,2352.7971,3.7973,328.9482,-1,-1,180);
	FamedVehicles[14] = AddStaticVehicleEx(461,2506.0073,2352.9568,3.7935,328.9489,-1,-1,180);
	//Comets
	FamedVehicles[15] = AddStaticVehicleEx(480,2524.4268,2381.5957,3.9851,269.3853,-1,-1,180);
	FamedVehicles[16] = AddStaticVehicleEx(480,2524.3342,2372.9875,3.9898,269.3844,-1,-1,180);
	FamedVehicles[17] = AddStaticVehicleEx(480,2524.2336,2363.5969,3.9947,269.3852,-1,-1,180);
	//Bullet
	FamedVehicles[18] = AddStaticVehicleEx(541,2524.1758,2377.0989,3.8359,269.6020,-1,-1,180);
	FamedVehicles[19] = AddStaticVehicleEx(541,2524.1130,2368.1453,3.8404,269.6032,-1,-1,180);
	//Jester
	FamedVehicles[20] = AddStaticVehicleEx(559,2542.2134,2381.6531,3.8673,89.7158,-1,-1,180);
	FamedVehicles[21] = AddStaticVehicleEx(559,2541.9707,2372.5603,3.8675,89.7158,-1,-1,180);
	//SuperGT
	FamedVehicles[22] = AddStaticVehicleEx(506,2541.7937,2367.7646,3.9154,89.7431,-1,-1,180);
	FamedVehicles[23] = AddStaticVehicleEx(506,2541.8357,2377.2507,3.9155,89.7431,-1,-1,180);
	//Banshee
	FamedVehicles[24] = AddStaticVehicleEx(429,2542.5437,2363.3955,3.8906,90.2345,-1,-1,180);
	//Infernus
	FamedVehicles[25] = AddStaticVehicleEx(411,2510.7610,2397.8477,3.9380,180.5807,-1,-1,180);
	FamedVehicles[26] = AddStaticVehicleEx(411,2519.9934,2397.9417,3.9424,180.5807,-1,-1,180);
	//Uranus
	FamedVehicles[27] = AddStaticVehicleEx(558,2515.1958,2398.0537,3.8412,179.9306,-1,-1,180);
	FamedVehicles[28] = AddStaticVehicleEx(558,2524.5942,2397.4172,3.8415,179.9293,-1,-1,180);
	//Huntley
	FamedVehicles[31] = AddStaticVehicleEx(579,2528.9141,2397.9255,4.1505,179.4067,-1,-1,180);
	FamedVehicles[32] = AddStaticVehicleEx(579,2537.8970,2397.8325,4.1541,179.4066,-1,-1,180);
	//Premier
	FamedVehicles[33] = AddStaticVehicleEx(426,2533.3384,2397.8184,3.9538,179.7989,-1,-1,180);
	FamedVehicles[34] = AddStaticVehicleEx(426,2542.4468,2397.7859,3.9583,179.8030,-1,-1,180);
	//Sanchez
	FamedVehicles[35] = AddStaticVehicleEx(468,2508.5623,2352.0640,3.8801,327.8297,-1,-1,180);
	FamedVehicles[36] = AddStaticVehicleEx(468,2512.6743,2352.0581,3.8747,327.8297,-1,-1,180);
	FamedVehicles[37] = AddStaticVehicleEx(468,2513.9478,2351.9365,3.8822,327.8291,-1,-1,180);
	FamedVehicles[38] = AddStaticVehicleEx(468,2516.2051,2352.1504,3.8789,327.8291,-1,-1,180);
	

	/*AddStaticVehicleEx(593,1402.44238281,-2428.44042969,14.10486984,180.00000000,-1,-1,2000); Dodo - 255
	AddStaticVehicleEx(593,1418.70190430,-2427.98681641,14.10486984,180.00000000,-1,-1,2000); Dodo
	AddStaticVehicleEx(593,1433.76721191,-2427.92626953,14.10486984,180.00000000,-1,-1,2000); Dodo*/
	AddStaticVehicleEx(509,1702.7448,-1884.2257,13.0817,291.3965,-1,-1,2000); // BIKENEWBIESPAWN1
	AddStaticVehicleEx(509,1702.6656,-1882.6167,13.0815,298.5609,-1,-1,2000); // BIKENEWBIESPAWN2
	AddStaticVehicleEx(509,1702.5789,-1881.3337,13.0545,289.8956,-1,-1,2000); // BIKENEWBIESPAWN3
	AddStaticVehicleEx(509,1702.5829,-1879.5234,13.0822,296.3651,-1,-1,2000); // BIKENEWBIESPAWN4
	AddStaticVehicleEx(509,1702.6434,-1877.2375,13.0821,302.0038,-1,-1,2000); // BIKENEWBIESPAWN5
	AddStaticVehicleEx(481,1727.4503,-1885.0702,13.0807,65.2583,-1,-1,2000); // BIKENEWBIESPAWN6
	AddStaticVehicleEx(481,1727.5376,-1882.7361,13.0772,66.9472,-1,-1,2000); // BIKENEWBIESPAWN7
	AddStaticVehicleEx(481,1727.2814,-1880.4712,13.0770,65.2958,-1,-1,2000); // BIKENEWBIESPAWN8
	AddStaticVehicleEx(481,1727.7151,-1877.8586,13.0783,65.9359,-1,-1,2000); // BIKENEWBIESPAWN9
	AddStaticVehicleEx(481,1727.5710,-1875.3071,13.0782,70.6519,-1,-1,2000); // BIKENEWBIESPAWN10

	//Train[0] = AddStaticVehicleEx(538,1462.0745,2630.8787,10.8203,200.0,-1,-1,2000); //LV passenger
	//Train[1] = AddStaticVehicleEx(538,-1942.7950,168.4164,27.0006,200.0,-1,-1,2000); //SF passenger
	//Train[2] = AddStaticVehicleEx(538,1700.7551,-1953.6531,14.8756,200.0,-1,-1,2000); //LS passenger
	print("[Streamer] Dynamic Static Vehicles has been loaded.");
	
	return 1;
}

forward LoadStreamerDynamicPickups();
public LoadStreamerDynamicPickups()
{
	// Points
	new string[128];
    for(new h = 0; h < sizeof(Points); h++)
	{
		if(Points[h][Type] == 3)
		{
			format(string, sizeof(string), " POT/THUOC PHIEN CO SAN: %d/1000.", Points[h][Stock]);
			Points[h][TextLabel] = CreateDynamic3DTextLabel(string, COLOR_YELLOW, Points[h][Pointx], Points[h][Pointy], Points[h][Pointz], 4.0);
			Points[h][CratePoint] = 1;
		}
		else if(Points[h][Type] == 4)
		{
			format(string, sizeof(string), " CRACK CO SAN: %d/500.", Points[h][Stock]);
			Points[h][TextLabel] = CreateDynamic3DTextLabel(string, COLOR_YELLOW, Points[h][Pointx], Points[h][Pointy], Points[h][Pointz], 4.0);
			Points[h][CratePoint] = 2;
		}
		CreateDynamicPickup(1239, 23, Points[h][Pointx], Points[h][Pointy], Points[h][Pointz]);
	}

    // Pickups
	CreateDynamicPickup(1239, 23, -2024.1606, -107.2681, 1041.6584, -1); // /paymytickets point
	CreateDynamicPickup(1239, 23, -4429.944824, 905.032470, 987.078186, -1); // VIP Garage Travel
	CreateDynamicPickup(1239, 23, 2102.71,-103.97,2.28, -1); // Matrun 3
	CreateDynamicPickup(1239, 23, -1816.528686, -179.502624, 9.398437, -1); // Matrun 4 (pickup)
	CreateDynamicPickup(1239, 23, -1872.879760, 1416.312500, 7.180089, -1); // Matrun 4 (dropoff)
	CreateDynamicPickup(1239, 23, -1522.7351, 2587.2612, 55.8359, -1); //Matrun TR (Pickup)
	CreateDynamicPickup(1239, 23, -688.7897,966.1434,12.1627, -1); //Matrun TR (DropOff)
  	//CreateDynamicPickup(1239, 23, 2565.346191, 1403.409790, 7699.584472, -1);// VIP ph changing station.
  	CreateDynamicPickup(1239, 23, 701.7953,-519.8322,16.3348, -1); //Rental Icon
	CreateDynamicPickup(1239, 23, -1528.0924,2688.7837,55.8359, -1); // Drug Dealer Job (TR)
	//CreateDynamicPickup(1239, 23, 757.3734,5.7227,1000.7012, -1); // Train Pos
	CreateDynamicPickup(1239, 23, 758.43,-78.0,1000.65, -1); // Train Pos (MALL GYM)
	CreateDynamicPickup(1239, 23, 293.6505,188.3670,1007.1719, -1); //FBI
	CreateDynamicPickup(1240, 23, 1179.4012451172,-1331.5632324219,2423.0461425781, -1);// /healme//Old Healme: 2103.4998,2824.2568,-16.1672
	CreateDynamicPickup(1239, 23, 1607.1567,1798.9805,-20.7645, -1);// /Old Insurance: 2086.4915,2826.7122,-16.1744
    CreateDynamicPickup(1210, 23, -71.419654846191,1360.4097900391,1080.2185058594, -1); //Hitman Pickup
	CreateDynamicPickup(1240, 23, -1528.814331, 2540.706054, 55.835937, -1);// Deliverpt (TR - El Quebrados)
	CreateDynamicPickup(1240, 23, 1142.4733,-1326.3633,13.6259, -1);// Deliverpt (All Saints)
	CreateDynamicPickup(1240, 23, 2027.0599,-1410.6870,16.9922, -1);// Deliverpt (County General)
	CreateDynamicPickup(1240, 23, 1227.2339,306.4730,19.7028, -1);// Deliverpt (Montgomery)
	CreateDynamicPickup(1240, 23, -339.2989,1055.8138,19.7392, -1);// Deliverpt (Fort Carson)
	CreateDynamicPickup(1240, 23, -2695.5725,639.4147,14.4531, -1); // Deliverpt (SF)
	CreateDynamicPickup(1240, 23, 1165.1564,-1368.8240,26.6502, -1);// Deliverpt (All Saints Rooftop)
	CreateDynamicPickup(1240, 23, 2024.5742,-1382.7844,48.3359, -1);// Deliverpt (County General)
	CreateDynamicPickup(1240, 23, 1233.3384,316.4022,24.7578, -1);// Deliverpt (Montgomery Rooftop)
	CreateDynamicPickup(1240, 23, -334.1560,1051.4434,26.0125, -1);// Deliverpt (Fort Carson Rooftop)
	CreateDynamicPickup(1240, 23, -2656.0339,615.2567,66.0938, -1);// Deliverpt (SF Rooftop)
	CreateDynamicPickup(1240, 23, 1579.58,1768.88,10.82, -1);// Deliverpt (LV Hospital)
	CreateDynamicPickup(1240, 23, -2482.4338,2231.1106,4.8463, -1);// Deliverpt (TR - Bayside)
	CreateDynamicPickup(1240, 23, 225.3467,1981.8497,17.6406, -1); //Deliverpt (DeMorgan)
	CreateDynamicPickup(1239, 23, 366.54, 159.09, 1008.38, -1); // LICENSES @ CITY HALL
	CreateDynamicPickup(371, 23, 1544.2,-1353.4,329.4); //LS towertop
	CreateDynamicPickup(371, 23, 1536.0, -1360.0, 1150.0); //LS towertop
	CreateDynamicPickup(1242, 23, 1527.5,-12.1,1002.0); //binco armor
	//CreateDynamicPickup(1240, 23, 279.3000,1853.5619,8.7649); //area51 health
	CreateDynamicPickup(2485, 23, 2958.2200, -1339.2900, 5.2100);// NGGShop - Car Shop
	CreateDynamicPickup(1239, 23, 2950.4014, -1283.0776, 4.6875);// NGGShop - Plane Shop
	CreateDynamicPickup(1239, 23, 2974.7520, -1462.9265, 2.8184);// NGGShop - Boat Shop
	CreateDynamicPickup(1314, 23, 2939.0134, -1401.2946, 11.0000);// NGGShop - VIP Shop
	CreateDynamicPickup(1272, 23, 2938.2734, -1391.0596, 11.0000);// NGGShop - House Shop
	CreateDynamicPickup(1239, 23, 2939.8442, -1411.2906, 11.0000);// NGGShop - Misc Shop
	CreateDynamicPickup(1239, 23, 2927.5000, -1530.0601, 11.0000);// NGGShop - ATM
	CreateDynamicPickup(1239, 23, 2958.0425, -1393.6724, 5.5500);// NGGShop - Hotdog Stand
	CreateDynamicPickup(1241, 23, 2946.8672, -1484.9561, 11.0000);// NGGShop - Healthcare
	CreateDynamicPickup(1239, 23, 2937.2878, -1357.2294, 11.0000);// NGGShop - Gift Reset
	CreateDynamicPickup(1582, 23, -1713.961425, 1348.545166, 7.1804520);// Get Pizza
	print("[Streamer] Dynamic Pickups has been loaded.");
	
	return 1;
}

forward LoadStreamerDynamic3DTextLabels();
public LoadStreamerDynamic3DTextLabels()
{
	CreateDynamicPickup(1239, 23, 1376.550537, -28.821121, 1000.872924);
	CreateDynamic3DTextLabel("{00ff00}[NGAN HANG]\n{99cc00}Khu vuc chuyen tien / gui tien.{FFFFFF}\nSu dung {ffff00}[Y]{FFFFFF} de bat dau su dung ngan hang.", -1, 1376.550537, -28.821121, 1000.872924, 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1);

	CreateDynamicPickup(1239, 23, 1375.455322, -21.559141, 1000.872924);
	CreateDynamic3DTextLabel("{00ff00}[NGAN HANG]\n{99cc00}Khu vuc dang ky tai khoan bank.\n{FFFFFF}Su dung {ffff00}[/dangkybank]{FFFFFF} de dang ky.", -1, 1375.455322, -21.559141, 1000.872924, 30.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, -1, -1, -1);

	new string[128];
    CreateDynamic3DTextLabel("{FF8000}/loadforklift\nDe dua cac thung len xe forklift", COLOR_YELLOW, -2114.1, -1723.5, 11984.5, 20.0, _, _, 1, 0, -1, _, 20.0);
	CreateDynamic3DTextLabel("{FF8000}LOCKDOWN\nCHI BAM TRONG TRUONG HOP KHAN CAP", COLOR_YELLOW, -1121.70, 4290.41, 5.88, 20.0, _, _, 1, 0, 0, _, 20.0);

	// General text labels
	CreateDynamic3DTextLabel("All Saints Plaza Parking Garage\nType /enter de dua xe cua ban vao bai",COLOR_YELLOW,1218.69,-1425.01,13.15+0.6,25.0);
	CreateDynamic3DTextLabel("Club VIP Parking\nSu dung /enter de dua xe cua ban vao bai",COLOR_YELLOW,1814.6857,-1559.2028,13.4834+0.4,15.0);
	CreateDynamic3DTextLabel("Gym Parking Garage\nSu dung /enter de dua xe cua ban vao bai",COLOR_YELLOW,2273.4026,-1724.3064,13.5469+0.6,25.0);
	CreateDynamic3DTextLabel("Courthouse Parking Garage\nSu dung /enter de dua xe cua ban vao bai",COLOR_YELLOW,1418.5190,-1663.8959,13.5395+0.6,25.0);
    CreateDynamic3DTextLabel("DMV Menu\n\n{FFFFFF}Go \"{85E09F}/dmvmenu{FFFFFF}\" de xem",COLOR_YELLOW,-2024.1606,-107.2681,1041.6584+0.6,4.0);//
    CreateDynamic3DTextLabel("Thay doi so dien thoai VIP, su dung /vipnum",COLOR_YELLOW,2549.548095, 1404.047729, 7699.584472+0.6,4.0);//
    CreateDynamic3DTextLabel("Thay doi so dien thoai VIP, su dung /vipnum",COLOR_YELLOW,1832.6000, 1375.1700, 1464.4600+0.6,4.0);//
    //CreateDynamic3DTextLabel("Su dung /travel\nDe dua chiec xe cua ban ra khoi bai do xe VIP",COLOR_YELLOW,-4429.944824, 905.032470, 987.078186+0.5,13.0);// VIP garage travel
    CreateDynamic3DTextLabel("/viplocker\nDe mo tu do VIP.",COLOR_YELLOW,2555.747314, 1404.106079, 7699.584472+0.6,4.0);/// LS VIP locker room
	CreateDynamic3DTextLabel("/viplocker\nDe mo tu do VIP.",COLOR_YELLOW,1832.0533, 1380.7281, 1464.3822+0.6,4.0);/// LV Main VIP locker room
    CreateDynamic3DTextLabel("/viplocker\nDe mo tu do VIP.",COLOR_YELLOW,772.4844, 1715.7213, 1938.0391+0.6,4.0);/// LV Plat VIP locker room
	CreateDynamic3DTextLabel("/getgift\nDe nhan phan thuong ben trong tui qua.",COLOR_YELLOW,2546.611572, 1403.323608, 7700.768066,8.0);/// VIP Lounge
	CreateDynamic3DTextLabel("/nhanqua\nDe nhan phan thuong ben trong tui qua.",COLOR_YELLOW,1726.1000, 1370.1000, 1449.9000,8.0);/// LV VIP Lounge
	CreateDynamic3DTextLabel("/nhanqua\nDe nhan phan thuong ben trong tui qua.",COLOR_YELLOW,1763.5000, 1432.5000, 2015.7000,8.0);/// LV GVIP Lounge
	CreateDynamic3DTextLabel("/nhanqua\nDe nhan phan thuong ben trong tui qua.",COLOR_YELLOW,772.4000, 1743.2000, 1938.8800,8.0);/// LV PVIP Lounge
    CreateDynamic3DTextLabel("Thue mot chiec xe dap theo yeu cau",COLOR_YELLOW,701.7953,-519.8322,16.3348+0.6,4.0);
	CreateDynamic3DTextLabel("Factions Duty Point", COLOR_DBLUE, -2043.527832, -189.208435, 985.183105+0.6, 4.0); //
	CreateDynamic3DTextLabel("Factions Duty Point", COLOR_DBLUE, 1112.7, -2055.8, 1409.7+0.6, 4.0); //
	CreateDynamic3DTextLabel("Factions Duty Point", COLOR_DBLUE, 321.43, 303.96, 999.14+0.6, 4.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 2112, 5, -1); //
	CreateDynamic3DTextLabel("Factions Duty Point", COLOR_DBLUE, 1533.6, -1760.9, 2264.4+0.6, 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 2345, 18, -1); //
	CreateDynamic3DTextLabel("Tro lai Entrance", COLOR_DBLUE, -2033.7502,-154.8784,35.3203+0.6, 4.0); // Prison
	/* CreateDynamic3DTextLabel("/deliverpt",COLOR_LIGHTRED,225.3467,1981.8497,17.6406+0.6,4.0);//Deliverpt (DeMorgan)
   	CreateDynamic3DTextLabel("/deliverpt",COLOR_LIGHTRED,-1528.814331, 2540.706054, 55.835937+0.6,4.0);///Deliverpt (TR - El Quebrados)
   	CreateDynamic3DTextLabel("/deliverpt",COLOR_LIGHTRED,1142.4733,-1326.3633,13.6259+0.6,4.0);///Deliverpt (All Saints)
   	CreateDynamic3DTextLabel("/deliverpt",COLOR_LIGHTRED,2027.0599,-1410.6870,16.9922+0.6,4.0);///Deliverpt (County General)
   	CreateDynamic3DTextLabel("/deliverpt",COLOR_LIGHTRED,1227.2339,306.4730,19.7028+0.6,4.0);///Deliverpt (Montgomery)
   	CreateDynamic3DTextLabel("/deliverpt",COLOR_LIGHTRED,-339.2989,1055.8138,19.7392+0.6,4.0);///Deliverpt (Fort Carson)
   	CreateDynamic3DTextLabel("/deliverpt",COLOR_LIGHTRED,1165.1564,-1368.8240,26.6502+0.6,4.0);///Deliverpt (All Saints Rooftop)
   	CreateDynamic3DTextLabel("/deliverpt",COLOR_LIGHTRED,2024.5742,-1382.7844,48.3359+0.6,4.0);///Deliverpt (County General)
   	CreateDynamic3DTextLabel("/deliverpt",COLOR_LIGHTRED,1233.3384,316.4022,24.7578+0.6,4.0);///Deliverpt (Montgomery Rooftop)
   	CreateDynamic3DTextLabel("/deliverpt",COLOR_LIGHTRED,-334.1560,1051.4434,26.0125+0.6,4.0);///Deliverpt (Fort Carson Rooftop)
   	CreateDynamic3DTextLabel("/deliverpt",COLOR_LIGHTRED,-2656.0339,615.2567,66.0938+0.6,4.0);///Deliverpt (SF Rooftop)
	CreateDynamic3DTextLabel("/deliverpt",COLOR_LIGHTRED,1579.58,1768.88,10.82+0.6,4.0); // /deliverpt (LV Hospital) */
   	CreateDynamic3DTextLabel(
    "{FF0000}[BENH VIEN]{FFFFFF}\n{FFFF00}KHU VUC CAP CUU{FFFFFF}\n{00FF00}[/capcuu]{FFFFFF}",
    COLOR_DBLUE,
    -2695.5725, 639.4147, 14.4531 + 0.6,
    4.0,
    INVALID_PLAYER_ID,
    INVALID_VEHICLE_ID,
    1,      // testLOS = 1 
    -1,     // streamdistance mặc định
    -1,     // area
    -1      // priority
);
	/* CreateDynamic3DTextLabel("/deliverpt",COLOR_LIGHTRED,-2482.4338,2231.1106,4.8463+0.6,4.0);///Deliverpt (TR - Bayside)
   	CreateDynamic3DTextLabel("/deliver",COLOR_DBLUE,1991.7953,-2321.2532,13.5469+0.6,4.0);// Deliver */
   	CreateDynamic3DTextLabel("/healme \nDe chua benh cua ban",COLOR_DBLUE,1179.4012451172,-1331.5632324219,2423.0461425781+0.6,4.0);// /healme
   	CreateDynamic3DTextLabel("/muabaohiem \n {F57171}Mua bao hiem y te",COLOR_WHITE,1607.1567,1798.9805,-20.7645+0.6,4.0);// Insurance
	CreateDynamic3DTextLabel("Su dung /layvatlieu tu mot chiec thuyen \nde co duoc vat lieu",COLOR_YELLOW,2102.71,-103.97,2.28+0.5,8.0);// Delivery
	CreateDynamic3DTextLabel("Su dung /muagiayphep to \nDe chon mua giay phep.",COLOR_YELLOW,366.54,159.09,1008.38+0.5,8.0);// Licenses.
	//CreateDynamic3DTextLabel("Type /registerplate to\nchange your license plate.",COLOR_YELLOW,366.54,159.09,1008.38+1,8.0);// Licenses.
    CreateDynamic3DTextLabel("/muaquanao\n De thay trang phuc ca nhan.",COLOR_YELLOW,208.8970,-100.3289,1005.2578,8.0);
    CreateDynamic3DTextLabel("/muaquanao\n De thay trang phuc ca nhan.",COLOR_YELLOW,206.5429,-41.2313,1001.8047,8.0);
    CreateDynamic3DTextLabel("/muaquanao\n De thay trang phuc ca nhan.",COLOR_YELLOW,209.3064,-8.3033,1005.2109,8.0);
    CreateDynamic3DTextLabel("/muaquanao\n De thay trang phuc ca nhan.",COLOR_YELLOW,159.6771,-83.2866,1001.8120,8.0);
    CreateDynamic3DTextLabel("/muaquanao\n De thay trang phuc ca nhan.",COLOR_YELLOW,206.4552,-163.0903,1000.5234,8.0);
    CreateDynamic3DTextLabel("/muadochoi\n De mua phu kien trang tri.",COLOR_YELLOW,206.3253,-100.3268,1005.2578,8.0);
    CreateDynamic3DTextLabel("/muadochoi\n De mua phu kien trang tri.",COLOR_YELLOW,201.0413,-40.1616,1001.8047,8.0);
    CreateDynamic3DTextLabel("/muadochoi\n De mua phu kien trang tri.",COLOR_YELLOW,206.3963,-8.2122,1001.2109,8.0);
    CreateDynamic3DTextLabel("/muadochoi\n De mua phu kien trang tri.",COLOR_YELLOW,162.7706,-83.2726,1001.8047,8.0);
    CreateDynamic3DTextLabel("/muadochoi\n De mua phu kien trang tri.",COLOR_YELLOW,2787.095947, 2390.353027, 1240.531127+0.6,4.0); // Gold+ VIP Room
    CreateDynamic3DTextLabel("/muadochoi\n De mua phu kien trang tri.",COLOR_YELLOW,202.6777,-163.0623,1000.5306,8.0);
	CreateDynamic3DTextLabel("/muadochoi\n De mua phu kien trang tri.",COLOR_YELLOW,1774.7533, 1422.6665, 2013.4979,8.0);
	CreateDynamic3DTextLabel("/muadochoi\n De mua phu kien trang tri.",COLOR_YELLOW,775.0900, 1742.4900, 1938.3800,8.0);
	
    CreateDynamic3DTextLabel("Tank Spawn\nZone",COLOR_YELLOW,286.7334, 2024.3762, 17.6406,10.0);
    CreateDynamic3DTextLabel("Air Vehicle Spawn\nZone", COLOR_YELLOW, 326.2754,1887.7212,17.6566, 10.0);

	// Paintball Arena
 	CreateDynamic3DTextLabel("Phong tap ban arena\n\nSu dung /thamgiaarena de vao phong",COLOR_YELLOW, -1492.395507, 920.093566, 7.187500+0.5,6.0);

	// SANews Broadcast
	SANews3DText = CreateDynamic3DTextLabel(string,COLOR_LIGHTBLUE,632.7400,-14.2350,1108.2181,8.0);
	
	// Buildings 3D labels
	//CreateDynamic3DTextLabel("Press F (enter) to go inside.",COLOR_YELLOW,1568.5718,-1691.0177,5.8906+0.6,4.0); //LSPD Garage enter
	//CreateDynamic3DTextLabel("Press F (enter) to go inside.",COLOR_YELLOW,1564.8464,-1666.1444,28.3956+0.6,4.0); //LSPD Roof enter

    CreateDynamic3DTextLabel("Su dung /layvatlieu de lay vat lieu",COLOR_YELLOW,-1816.528686, -179.502624, 9.398437+0.6,5.0);
    CreateDynamic3DTextLabel("Su dung /layvatlieu de lay vat lieu",COLOR_YELLOW,-1522.7351, 2587.2612, 55.8359+0.6, 5.0);
	CreateDynamic3DTextLabel("Diem thay doi ten \nSu dung /doiten de thay doi ten",COLOR_YELLOW,1154.7295,-1440.2323,15.7969+0.6,18.0); // Namechange at mall

    /* Sprunk 3D Text */
    CreateDynamic3DTextLabel("May uong nuoc \nBam phim 'F' de lay nuoc uong",COLOR_YELLOW,2140.4504,-1161.4298,23.9922+0.6,12.0); // May uong nuoc
    CreateDynamic3DTextLabel("May uong nuoc \nBam phim 'F' de lay nuoc uong",COLOR_YELLOW,2153.8826,-1015.5273,62.8895+0.6,12.0); // May uong nuoc
    CreateDynamic3DTextLabel("May uong nuoc \nBam phim 'F' de lay nuoc uong",COLOR_YELLOW,2325.9705,-1646.1234,14.8270+0.6,12.0); // May uong nuoc
    CreateDynamic3DTextLabel("May uong nuoc \nBam phim 'F' de lay nuoc uong",COLOR_YELLOW,2060.2163,-1898.6940,13.5538+0.6,12.0); // May uong nuoc
    CreateDynamic3DTextLabel("May uong nuoc \nBam phim 'F' de lay nuoc uong",COLOR_YELLOW,1929.8986,-1772.5122,13.5469+0.6,12.0); // May uong nuoc
    CreateDynamic3DTextLabel("May uong nuoc \nBam phim 'F' de lay nuoc uong",COLOR_YELLOW,1153.6688,-1460.8262,15.7969+0.6,12.0); // May uong nuoc

	/* Job's 3D Text */
    CreateDynamic3DTextLabel("Tierra Robada Gant Bridge Access.",0xFFFF00AA,-2678.2702636719,2148.0134277344,55.4296875+0.6,20.0);// Border Lable
	CreateDynamic3DTextLabel("Tierra Robada Airport",0xFFFF00AA,-1873.2819824219,2172.4621582031,6.2687501907349+0.6,20.0);// Airport Lable.
	CreateDynamic3DTextLabel("Famed Locker \nType /famedlocker to access the locker", COLOR_YELLOW,901.4825,1429.7404,-82.3235+0.6,4.0); //Famed Locker
	CreateDynamic3DTextLabel("Diem lay banh\n{FFFFFF}Go \"{43F8A7}/laybanh{FFFFFF}\" hoac \"{43F8A7}an [Y]{FFFFFF}\"de lay banh pizza", COLOR_YELLOW, -1713.961425, 1348.545166, 7.180452+0.6,4.0); //Famed Locker
	//NGGShop
	CreateDynamic3DTextLabel("Cua hang xe\n/carshop", COLOR_YELLOW, 2958.2200, -1339.2900, 5.2100+0.6, 10.0);
	CreateDynamic3DTextLabel("Cua hang may bay\n/planeshop", COLOR_YELLOW, 2950.4014, -1283.0776, 4.6875+0.6, 10.0);
	CreateDynamic3DTextLabel("Cua hang thuyen\n/boatshop", COLOR_YELLOW, 2974.7520, -1462.9265, 2.8184+0.6, 10.0);
	CreateDynamic3DTextLabel("VIP Shop\n/vipshop", COLOR_YELLOW, 2939.0134, -1401.2946, 11.0000+0.6, 10.0);
	CreateDynamic3DTextLabel("Toy Shop\n/toyshop", COLOR_YELLOW, 2927.5244, -1549.1826, 11.0000+0.6, 10.0);
	CreateDynamic3DTextLabel("Gift Reset\n/buygiftreset", COLOR_YELLOW, 2937.2878, -1357.2294, 11.0000+0.6, 10.0);
	CreateDynamic3DTextLabel("House Shop\n/houseshop", COLOR_YELLOW, 2938.2734, -1391.0596, 11.0000+0.6, 10.0);
	CreateDynamic3DTextLabel("Misc Shop\n/miscshop", COLOR_YELLOW, 2939.8442, -1411.2906, 11.0000+0.6, 10.0);
	CreateDynamic3DTextLabel("Hot Dogs\n/buyhotdog", COLOR_YELLOW, 2958.0425, -1393.6724, 5.5500+0.6, 10.0);
	CreateDynamic3DTextLabel("HealthCare Shop\n/buyhealthcare", COLOR_YELLOW, 2946.8672, -1484.9561, 11.0000+0.6, 10.0);

	print("[Streamer] Dynamic 3D Text Labels has been loaded.");
	return 1;
}

forward LoadStreamerDynamicButtons();
public LoadStreamerDynamicButtons()
{
    /* LSPD buttons */
 	eastin = CreateButton(253.44921875,110.59960938,1003.79998779,90.00000000); //object(fire_break) (1)
	westout = CreateButton(239.79492188,116.18457031,1003.87286377,90.00000000); //object(fire_break) (2)
	westin = CreateButton(239.32031250,116.27441406,1003.87286377,270.00000000); //object(fire_break) (3)
	eastout = CreateButton(253.00000000,110.59960938,1003.79998779,270.00000000); //
	elevator = CreateButton(275.83984375,120.94921875,1005.12280273,90.00000000); //object(kmb_keypad) (1)
	cctvin = CreateButton(264.55566406,115.97949219,1005.12280273,179.99450684); //object(kmb_keypad) (2)
	cctvout = CreateButton(264.55566406,115.69531250,1005.12280273,0.00000000); //object(kmb_keypad) (3)
	lockerin = CreateButton(267.21679688,112.40917969,1005.12280273,0.00000000); //object(kmb_keypad) (4)
	lockerout = CreateButton(267.21679688,112.66992188,1005.12280273,179.99450684); //object(kmb_keypad) (5)
	chiefout = CreateButton(232.50000000,119.38476562,1010.81384277,0.00000000); //object(sec_keypad) (2)
	chiefin = CreateButton(229.67089844,119.66992188,1010.81384277,179.99450684); //object(sec_keypad) (1)
	roofkey = CreateButton(1565.93652344,-1667.35058594,28.85165977,179.99450684); //object(sec_keypad) (3)
	garagekey = CreateButton(1567.14550781,-1689.62011719,6.69999981,0.00000000); //object(sec_keypad) (4)
 	sasdbtn1 = CreateButton(2514.59179688,-1697.05761719,563.19116211,0.00000000); //SASD West
	sasdbtn2 = CreateButton(2522.82299805,-1660.15917969,563.15893555,0.00000000); //SASD East
	sasdbtn3 = CreateButton(2525.09863281,-1697.05761719,563.16284180,0.00000000); //SASD R4
	sasdbtn4 = CreateButton(2519.84375000,-1697.00659180,563.20904541,0.00000000); //SASD R5
	sasdbtn5 = CreateButton(2514.77880859,-1660.15917969,563.16925049); //SASD R6
	
	/* Noose Int Buttons */
	nooseenter[0] = CreateButton(-765.87152100,2539.55102539,10021.18847847,90); //object(kmb_keypad) (5) yes entrance
    nooseenter[1] = CreateButton(-766.81298828,2539.55664062,10021.18847847,0); //object(kmb_keypad) (9) yes entrance
    radarroom[0] = CreateButton(-747.22851562,2533.17968750,10021.18847656,260);
    radarroom[1] = CreateButton(-745.98876953,2533.04150391,10021.18847656,90);
	intergate[0] = CreateButton(-765.34863281,2549.97070312,10021.18847656,90);
	intergate[1] = CreateButton(-765.13671875,2554.51660156,10020.90917969,260 );
	intergate[2] = CreateButton(-773.29394531,2546.13134766,10020.86816406,90); // Cage
	bottomroom[0] = CreateButton(-764.06152344,2566.59277344,10021.18847656, 90);
	bottomroom[1] = CreateButton(-764.51269531,2566.65332031,10021.18847656, 260);
	ncontrolroom[0] = CreateButton(-754.32763672,2538.71655273,10021.61132812, 178.50000000);
	ncontrolroom[1] = CreateButton(-752.47076416,2538.23266602,10021.00195312, 0);

	/* DoC Prison Buttons */
	PrisonButtons[0] = CreateButton(-2097, -191.100006, 996.299987, 90); // Control Room
	PrisonButtons[1] = CreateButton(-2092.550048, -204, 996.099975, 0); // Control Room Door 2
	PrisonButtons[2] = CreateButton(-2088.530029, -213, 996, 90); //Control Room Door 1
	PrisonButtons[3] = CreateButton(-2037, -191, 992.200012, 270); // Security Room
	PrisonButtons[4] = CreateButton(-2050.5, -205.880004, 985.599975, 0); // Staff Lounge
	PrisonButtons[5] = CreateButton(-2046.599975, -194, 985.700012, 270); // Duty Room
	PrisonButtons[6] = CreateButton(-2092.772949, -193.570007, 979.299987, 0); // Security Room 2
 	PrisonButtons[7] = CreateButton( -2054.699951, -84, 35.7, 270);
 	PrisonButtons[8] = CreateButton(-2058, -146.75, 988.7,270);

 	// Create the required buttons for the interior.
	SFPDButton[0] = CreateButton(-1635.12597656,708.45117188,19996.32812500, 180.0);
	SFPDButton[1] = CreateButton(-1635.18176270,687.56250000,19996.32812500, 180.0);
	SFPDButton[2] = CreateButton(-1636.74023438,687.56250000,19996.32812500, 180.0);
	SFPDButton[3] = CreateButton(-1636.29687500,708.36035156,19996.32812500, 270.0);
	SFPDButton[4] = CreateButton(-1627.03222656,712.46093750,19996.32812500, 0.0);
	SFPDButton[5] = CreateButton(-1627.03295898,712.63281250,19996.32812500, 180.0);
	SFPDButton[6] = CreateButton(-1614.06738281,683.54052734,19990.56250000, 270.0);
	SFPDButton[7] = CreateButton(-1612.91674805,683.60369873,19990.56250000, 0.0);
	SFPDButton[8] = CreateButton(-1632.86157227,712.62530518,19996.32812500, 180.0);
	SFPDButton[9] = CreateButton(-1632.861328125, 712.47210693359, 19996.328125, 0.0);
	
	FBILobbyLeftBTN[0] = CreateButton(297.66613770,-1498.67749023,-44.59006119,0.79565430); //Lobby Button Left
	FBILobbyLeftBTN[1] = CreateButton(297.24850464,-1498.23107910,-44.59006119,180); //Lobby Button Left
	FBILobbyRightBTN[0] = CreateButton(300.05300903,-1521.40747070,-44.59006119,180); //Lobby Button Right
	FBILobbyRightBTN[1] = CreateButton(300.16033936,-1521.84387207,-44.59006119,0); //Lobby Button Right
	FBIPrivateBTN[0] = CreateButton(298.87384033,-1495.87316895,-27.32773209,270); //Private Office Button
	FBIPrivateBTN[1] = CreateButton(300.49453735,-1495.33837891,-27.28091812,180.49487305); //Private Office Button
	
	SANewsPrivate = CreateButton(625.51953125,-3.59961796,1108.50000000,269.99450684); //object(sec_keypad) (2)
	SANewsOffice = CreateButton(614.58581543,17.78320312,1108.55004883,0); //object(sec_keypad) (3)
	SANewsStudio = CreateButton(625.50000000,-13.60000038,1108.50000000,270.00000000); //object(sec_keypad) (1)
	SANewsPrivateOPP = CreateButton(626.13519287,-0.08516422,1108.61315918,0); //object(sec_keypad) (4)
	
	lockdownbutton = CreateButton(-1121.70, 4290.41, 4.88, 180.00);
	print("[Streamer] Dynamic Buttons has been loaded.");
	
	return 1;
}

forward LoadStreamerDynamicObjects();
public LoadStreamerDynamicObjects()
{
    CrateLoad = CreateDynamicObject(964,-2114.1, -1723.5, 11984.5, 0, 0, 337.994, .worldid = 0, .interiorid = 1, .streamdistance = 200); //object(cj_metal_crate) (1)
	IslandGate = CreateDynamicObject(16773,-1083.90002441,4289.70019531,14.10000038,0.00000000,0.00000000,0.00000000, .streamdistance = 400); //object(door_savhangr1) (5)

	new VIPLogo = CreateDynamicObject(19353, 1803.89, -1593.99, 14.05,   0.00, 0.00, 312.26);
	SetDynamicObjectMaterialText(VIPLogo, 0, "{842787}VIP LOUNGE", 90, "Impact", 56, 1, 0xFFFFFFFF, 0, 1);

	new FCTS = CreateDynamicObject(19482, -58.9155, -1118.4808, 7.4781, 0.0000, 0.0000, 160.3858);
	SetDynamicObjectMaterialText(FCTS, 0, "{880000}Flint County Towing Services\n{000000}CALL 1715 OR 2112", 110, "Impact", 40, 1, 0xFF000000, 0, 1);

    new vipsf = CreateObject(19353, -2455.0632, 503.8305, 32.7488, 0.1999, 31.5000, -0.0145);
	SetObjectMaterialText(vipsf, "{FFFFFF}KP5 {FF6666}Roleplay", 0, 130, "Times New Roman", 30, false, -16776961, 0,OBJECT_MATERIAL_TEXT_ALIGN_CENTER);

    /* (ongamemodeinit) - LSPD stuff */
	eastlobby1 = CreateDynamicObject(1536,253.14941406,110.59960938,1002.21502686,0.00000000,0.00000000,270.00000000,-1,10,-1,100.0); // East lobby door (moves north)
	eastlobby2 = CreateDynamicObject(1536,253.18457031,107.59960938,1002.21502686,0.00000000,0.00000000,90.00000000,-1,10,-1,100.0); // East lobby door (moves south)
	westlobby1 = CreateDynamicObject(1536,239.71582031,116.09179688,1002.21502686,0.00000000,0.00000000,90.00000000,-1,10,-1,100.0); // West lobby door (moves north)
	westlobby2 = CreateDynamicObject(1536,239.67968750,119.09960938,1002.21502686,0.00000000,0.00000000,269.98901367,-1,10,-1,100.0); // West lobby door (moves south)
	locker1 = CreateDynamicObject(1536,267.29980469,112.56640625,1003.61718750,0.00000000,0.00000000,179.99450684,-1,10,-1,100.0); // Locker door (moves east)
	locker2 = CreateDynamicObject(1536,264.29980469,112.52929688,1003.61718750,0.00000000,0.00000000,0.00000000,-1,10,-1,100.0); // Locker door (moves west)
	cctv1 = CreateDynamicObject(1536,264.44921875,115.79980469,1003.61718750,0.00000000,0.00000000,0.00000000,-1,10,-1,100.0); // CCTV door (moves west)
	cctv2 = CreateDynamicObject(1536,267.46875000,115.83691406,1003.61718750,0.00000000,0.00000000,179.99450684,-1,10,-1,100.0); // CCTV door (moves east)
	chief1 = CreateDynamicObject(1536,229.59960938,119.50000000,1009.21875000,0.00000000,0.00000000,0.00000000,-1,10,-1,100.0); // innermost chief's door
	chief2 = CreateDynamicObject(1536,232.59960938,119.53515625,1009.21875000,0.00000000,0.00000000,179.99450684,-1,10,-1,100.0); // outermost chief's door (both move west)
	
	// New LSPD Interior Doors & Cells
	AkaDoor[0] = CreateDynamicObject(1495,1487.00000000,-1762.42504883,3284.23608398,0.00000000,0.00000000,270.00000000); //object(gen_doorext01) (1)
	AkaDoor[1] = CreateDynamicObject(1495,1483.79003906,-1762.42504883,3284.23608398,0.00000000,0.00000000,270.00000000); //object(gen_doorext01) (2)
	AkaDoor[2] = CreateDynamicObject(1495,1479.85998535,-1758.31994629,3284.23388672,0.00000000,0.00000000,0.00000000); //object(gen_doorext01) (2)
	AkaDoor[3] = CreateDynamicObject(1495,1467.06701660,-1758.31994629,3284.23388672,0.00000000,0.00000000,0.00000000); //object(gen_doorext01) (2)
	AkaCell[0] = CreateDynamicObject(1567,1491.21484375,-1764.90002441,3284.25048828,0.00000000,0.00000000,0.00000000); //object(gen_wardrobe) (2)
	AkaCell[1] = CreateDynamicObject(1567,1494.41210938,-1764.90002441,3284.25048828,0.00000000,0.00000000,0.00000000); //object(gen_wardrobe) (3)
	AkaCell[2] = CreateDynamicObject(1567,1497.61132812,-1764.90002441,3284.25048828,0.00000000,0.00000000,0.00000000); //object(gen_wardrobe) (4)
	AkaCell[3] = CreateDynamicObject(1567,1500.81445312,-1764.90002441,3284.25048828,0.00000000,0.00000000,0.00000000); //object(gen_wardrobe) (6)
	AkaCell[4] = CreateDynamicObject(1567,1500.81994629,-1761.51000977,3284.25048828,0.00000000,0.00000000,0.00000000); //object(gen_wardrobe) (7)
	AkaCell[5] = CreateDynamicObject(1567,1491.22094727,-1761.50000000,3284.25048828,0.00000000,0.00000000,0.00000000); //object(gen_wardrobe) (8)
	AkaCell[6] = CreateDynamicObject(1567,1494.41894531,-1761.51000977,3284.25048828,0.00000000,0.00000000,0.00000000); //object(gen_wardrobe) (9)
	AkaCell[7] = CreateDynamicObject(1567,1497.61999512,-1761.51000977,3284.25048828,0.00000000,0.00000000,0.00000000); //object(gen_wardrobe) (10)

	/* Noose Int Buttons End */
	sasd1A = CreateDynamicObject(1536,2511.65332031,-1697.00976562,561.79223633,0.00000000,0.00000000,0.00000000); //R6
	sasd1B = CreateDynamicObject(1536,2514.67211914,-1696.97485352,561.79223633,0.00000000,0.00000000,180.00000000); //object(gen_doorext15) (2)
 	sasd2A = CreateDynamicObject(1536,2516.87548828,-1697.01525879,561.79223633,0.00000000,0.00000000,0.00000000); //R5
	sasd2B = CreateDynamicObject(1536,2519.89257812,-1696.97509766,561.79223633,0.00000000,0.00000000,179.99450684); //object(gen_doorext15) (4)
	sasd3A = CreateDynamicObject(1536,2522.15600586,-1697.01550293,561.79223633,0.00000000,0.00000000,0.00000000); //R4
	sasd3B = CreateDynamicObject(1536,2525.15893555,-1696.98010254,561.79223633,0.00000000,0.00000000,179.99450684); //object(gen_doorext15) (6)
	sasd4A = CreateDynamicObject(1536,2511.84130859,-1660.08081055,561.79528809,0.00000000,0.00000000,0.00000000); //West
	sasd4B = CreateDynamicObject(1536,2514.81982422,-1660.04650879,561.80004883,0.00000000,0.00000000,180.00000000); //object(gen_doorext15) (1)
	sasd5A = CreateDynamicObject(1536,2522.86059570,-1660.07177734,561.80206299,0.00000000,0.00000000,179.99450684); //East
	sasd5B = CreateDynamicObject(1536,2519.84228516,-1660.10888672,561.80004883,0.00000000,0.00000000,0.00000000); //object(gen_doorext15) (1)

	lspddoor1 = CreateDynamicObject(1569, 246.35150146484, 72.547714233398, 1002.640625, 0.000000, 0.000000, 0.000000); //
	lspddoor2 = CreateDynamicObject(1569, 245.03300476074, 72.568511962891, 1002.640625, 0.000000, 0.000000, 0.000000); //

	FBIPrivate[0] = CreateDynamicObject(1536,299.29986572,-1492.82666016,-28.73300552,0.00000000,0.00000000,270.00000000,1324123, .interiorid = 1); //Private Office Door Left
	FBIPrivate[1] = CreateDynamicObject(1536,299.33737183,-1495.83911133,-28.73300552,0.00000000,0.00000000,90.00000000,1324123, .interiorid = 1); //Private Office Door Right
	FBILobbyLeft = CreateDynamicObject(1536,295.40136719,-1498.43457031,-46.13965225,0.00000000,0.00000000,0.00000000,1324123, .interiorid = 1); //Lobby Door Left
	FBILobbyRight = CreateDynamicObject(1536,302.39355469,-1521.62988281,-46.13965225,0.00000000,0.00000000,179.99450684,1324123, .interiorid = 1); //Lobby Door Right


	//SANews Custom Interior
	SANewsStudioA = CreateDynamicObject(1536,625.60937500,-10.80000019,1106.96081543,0.00000000,0.00000000,270.00000000, .interiorid = 1); //object(gen_doorext15) (1)
	SANewsStudioB = CreateDynamicObject(1536,625.64941406,-13.77000046,1106.96081543,0.00000000,0.00000000,89.99450684, .interiorid = 1); //object(gen_doorext15) (2)

    SANewsPrivateA = CreateDynamicObject(1536,625.60937500,-0.55000001,1106.96081543,0.00000000,0.00000000,269.98901367, .interiorid = 1); //object(gen_doorext15) (4)
	SANewsPrivateB = CreateDynamicObject(1536,625.65002441,-3.54999995,1106.96081543,0.00000000,0.00000000,89.99450684, .interiorid = 1); //object(gen_doorext15) (4)

	SANewsOfficeA = CreateDynamicObject(1536,614.66998291,17.82812500,1106.98425293,0.00000000,0.00000000,0.00000000, .interiorid = 1); //object(gen_doorext15) (3)
	SANewsOfficeB = CreateDynamicObject(1536,617.69000244,17.86899948,1106.98425293,0.00000000,0.00000000,179.99450684, .interiorid = 1); //object(gen_doorext15) (5)


	/* SAAS CARRIER */
	/*sidelift = CreateDynamicObject(3114, 231.916656, 3615.134277, 17.269205, 0.0000, 0.0000, 0.0000); // Side Lift Up
	backhatch = CreateDynamicObject(3113, 180.344864, 3600.390137, 2.516232, 0.0000, 0.0000, 0.0000); // Back Hatch Closed
	backlift = CreateDynamicObject(3115, 189.694626, 3599.983398, 17.483730, 0.0000, 0.0000, 0.0000); // Back Lift Up

	Carrier[0] = CreateDynamicObject(10771, 288.665771, 3600.003418, 6.032381, 0.0000, 0.0000, 0.0000);
	Carrier[1] = CreateDynamicObject(11145, 225.782196, 3600.015137, 4.754915, 0.0000, 0.0000, 0.0000);
	Carrier[2] = CreateDynamicObject(11149, 282.526093, 3594.805176, 12.487646, 0.0000, 0.0000, 0.0000);
	Carrier[3] = CreateDynamicObject(11146, 279.620544, 3600.541016, 12.893089, 0.0000, 0.0000, 0.0000);
	Carrier[4] = CreateDynamicObject(10770, 291.858917, 3592.397949, 39.171509, 0.0000, 0.0000, 0.0000);
	Carrier[5] = CreateDynamicObject(10772, 290.014313, 3599.787598, 17.833616, 0.0000, 0.0000, 0.0000);
	Carrier[6] = CreateDynamicObject(1671, 354.860748, 3589.442383, 11.234554, 0.0000, 0.0000, 175.3254);
	Carrier[7] = CreateDynamicObject(925, 304.330383, 3589.067383, 11.735489, 0.0000, 0.0000, 0.0000);
	Carrier[8] = CreateDynamicObject(930, 301.851654, 3588.497070, 11.131838, 0.0000, 0.0000, 0.0000);
	Carrier[9] = CreateDynamicObject(930, 301.856079, 3589.598145, 11.181837, 0.0000, 0.0000, 0.0000);
	Carrier[10] = CreateDynamicObject(964, 300.513062, 3589.303711, 10.705961, 0.0000, 0.0000, 177.4217);
	Carrier[11] = CreateDynamicObject(964, 299.024902, 3589.362793, 10.698584, 0.0000, 0.0000, 177.4217);
	Carrier[12] = CreateDynamicObject(1271, 305.058319, 3591.442871, 11.048584, 0.0000, 0.0000, 359.1406);
	Carrier[13] = CreateDynamicObject(1431, 303.009491, 3591.383789, 11.253574, 0.0000, 0.0000, 0.0000);
	Carrier[14] = CreateDynamicObject(2567, 297.100800, 3591.239746, 12.558563, 0.0000, 0.0000, 91.1003);
	Carrier[15] = CreateDynamicObject(3576, 301.050110, 3593.777344, 12.198634, 0.0000, 0.0000, 0.0000);
	Carrier[16] = CreateDynamicObject(3633, 304.567841, 3593.262207, 11.173386, 0.0000, 0.0000, 0.0000);

	CarrierS[0] = CreateDynamicObject(3267, 320.358582, 3592.519043, 21.567169, 0.0000, 0.0000, 0.0000);
	CarrierS[1] = CreateDynamicObject(11237, 291.557526, 3592.407715, 39.065594, 0.0000, 0.0000, 0.0000);
	CarrierS[2] = CreateDynamicObject(3395, 354.861725, 3590.989746, 10.797120, 0.0000, 0.0000, 88.0403);
	CarrierS[3] = CreateDynamicObject(1671, 356.571838, 3588.612793, 11.234554, 0.0000, 0.0000, 134.9316);
	CarrierS[4] = CreateDynamicObject(3393, 358.360016, 3588.834961, 10.797121, 0.0000, 0.0000, 0.0000);
	CarrierS[5] = CreateDynamicObject(3277, 320.391876, 3592.538086, 21.514416, 0.0000, 0.0000, 164.0483); */
	
	entrancedoor = CreateDynamicObject(2951,-766.27539062,2536.58691406,10019.5,0.98876953,0.00000000,85.49011230); //object(a51_labdoor) (1)

	blastdoor[0] = CreateDynamicObject(2927,-764.11816406,2568.81445312,10021.5,0.70861816,0.49438477,86.46789551); //object(a51_blastdoorr) (5)
	blastdoor[1] = CreateDynamicObject(2927,-746.02636719,2535.19433594,10021.5,359.74731445,0.00000000,265.24291992); //object(a51_blastdoorr) (1)
	cage = CreateDynamicObject(2930,-773.52050781,2545.62109375,10022.29492188,359.73632812,359.23095703,354.73205566); //object(chinatgate) (1)
	blastdoor[2] = CreateDynamicObject(2927,-765.26171875,2552.31347656,10021.5,0.00000000,0.00000000,87.44567871); //object(a51_blastdoorr) (5)

	ncontrolroomobjects[0] = CreateDynamicObject(3037,-760.61718750,2544.21679688,10024.92480469,0.00000000,0.00000000,355.98999023); //object(warehouse_door2b) (1)
	ncontrolroomobjects[1] = CreateDynamicObject(3037,-759.52246094,2560.88574219,10024.79785156,0.00000000,0.00000000,355.98999023); //object(warehouse_door2b) (2)
	ncontrolroomobjects[2] = CreateDynamicObject(3037,-755.53906250,2538.61035156,10025.02636719,0.00000000,0.00000000,85.99548340); //object(warehouse_door2b) (3)

	CellGates[1] = CreateDynamicObject(10252,-2075.55273438,-191.64550781,992.19836426,0.00000000,0.00000000,0.00000000, .interiorid = 10); //object(china_town_gateb)(3)
 	CellGates[0] = CreateDynamicObject(10252,-2080.28613281,-193.01757812,992.19836426,0.00000000,0.00000000,179.99450684, .interiorid = 10); //object(china_town_gateb)(3)
    CellGates[2] = CreateDynamicObject(10252,-2068.00195312,-193.01757812,992.19836426,0.00000000,0.00000000,179.99450684, .interiorid = 10); //object(china_town_gateb)(3)
    CellGates[3] = CreateDynamicObject(10252,-2063.56738281,-191.64550781,992.19836426,0.00000000,0.00000000,0.00000000, .interiorid = 10); //object(china_town_gateb)(3)
    CellGates[4] = CreateDynamicObject(10252,-2055.99511719,-193.01757812,992.19836426,0.00000000,0.00000000,179.99450684, .interiorid = 10); //object(china_town_gateb)(3)
    CellGates[5] = CreateDynamicObject(10252,-2052.22460938,-191.64550781,992.19836426,0.00000000,0.00000000,0.00000000, .interiorid = 10); //object(china_town_gateb)(3)
    CellGates[11] = CreateDynamicObject(10252,-2084.99902344,-207.03710938,992.19836426,0.00000000,0.00000000,179.99450684, .interiorid = 10); //object(china_town_gateb)(3)
    CellGates[10] = CreateDynamicObject(10252,-2081.52539062,-205.66894531,992.19836426,0.00000000,0.00000000,0.00000000, .interiorid = 10); //object(china_town_gateb)(3)
    CellGates[8] = CreateDynamicObject(10252,-2069.53710938,-205.66894531,992.19836426,0.00000000,0.00000000,0.00000000, .interiorid = 10); //object(china_town_gateb)(3)
    CellGates[9] = CreateDynamicObject(10252,-2074.00585938,-207.03710938,992.19836426,0.00000000,0.00000000,179.99450684, .interiorid = 10); //object(china_town_gateb)(3)
    CellGates[6] = CreateDynamicObject(10252,-2057.59765625,-205.66894531,992.19836426,0.00000000,0.00000000,0.00000000, .interiorid = 10); //object(china_town_gateb)(3)
    CellGates[7] = CreateDynamicObject(10252,-2061.96289062,-207.03710938,992.19836426,0.00000000,0.00000000,179.99450684, .interiorid = 10); //object(china_town_gateb)(3)
    BlastDoors[1] = CreateDynamicObject(2952,-2088.76562500,-211.33984375,994.66918945,0.00000000,0.00000000,0.00000000); //object(kmb_gimpdoor)(1)
    BlastDoors[6] = CreateDynamicObject(2952,-2088.76562500,-209.21093750,994.66918945,0.00000000,0.00000000,0.00000000); //object(kmb_gimpdoor)(2)
    BlastDoors[0] = CreateDynamicObject(2952,-2095.10156250,-203.91210938,994.66918945,0.00000000,0.00000000,90.00000000); //prisongate1
    BlastDoors[2] = CreateDynamicObject(2952,-2048.29296875,-205.54394531,990.45825195,0.00000000,0.00000000,0.00000000); //object(kmb_gimpdoor)(1)
    BlastDoors[7] = CreateDynamicObject(2952,-2048.29296875,-207.67382812,990.45825195,0.00000000,0.00000000,0.00000000); //object(kmb_gimpdoor)(1)
    BlastDoors[3] = CreateDynamicObject(2952,-2041.79785156,-195.64550781,990.45825195,0.00000000,0.00000000,179.99450684); //object(kmb_gimpdoor)(1)
    BlastDoors[8] = CreateDynamicObject(2952,-2041.79687500,-197.77246094,990.45825195,0.00000000,0.00000000,179.99450684); //object(kmb_gimpdoor)(1)
    BlastDoors[4] = CreateDynamicObject(2952,-2041.78613281,-211.28515625,984.02539062,0.00000000,0.00000000,0.00000000); //object(kmb_gimpdoor)(1)
    BlastDoors[9] = CreateDynamicObject(2952,-2041.78808594,-209.15917969,984.02539062,0.00000000,0.00000000,0.00000000); //object(kmb_gimpdoor)(1)
    BlastDoors[5] = CreateDynamicObject(2952,-2053.92187500,-205.46679688,977.75732422,0.00000000,0.00000000,0.00000000); //object(kmb_gimpdoor)(1)
    BlastDoors[10] = CreateDynamicObject(2952,-2053.92187500,-207.59570312,977.75732422,0.00000000,0.00000000,0.00000000); //object(kmb_gimpdoor)(1)
    BlastDoors[11] = CreateDynamicObject(2952,-2050.50097656,-205.82617188,984.02539062,0.00000000,0.00000000,90.00000000); //object(kmb_gimpdoor)(1)
    BlastDoors[16] = CreateDynamicObject(1569, -2057.9, -144.905, 987.24 ,0, 0.00000000, 270.0);

	SFPDObject[0] =	CreateDynamicObject(976,-1636.02539062,701.49707031,19994.54101562,0.00000000,0.00000000,90.00000000, .interiorid = 3); //object(phils_compnd_gate) (1)
	SFPDObject[1] =	CreateDynamicObject(976,-1635.99414062,696.53320312,19994.55078125,0.00000000,0.00000000,270.00000000, .interiorid = 3); //object(phils_compnd_gate) (2)
	SFPDObject[2] =	CreateDynamicObject(1569,-1625.28808594,712.56250000,19994.85937500,0.00000000,0.00000000,179.99450684, .interiorid = 3); //object(adam_v_door) (2)
	SFPDObject[3] =	CreateDynamicObject(1569,-1613.92871094,681.78125000,19989.05468750,0.00000000,0.00000000,90.00000000, .interiorid = 3); //object(adam_v_door) (4)
	SFPDObject[4] =	CreateDynamicObject(1569,-1634.79492188,712.56250000,19994.85937500,0.00000000,0.00000000,0.00000000, .interiorid = 3); //object(adam_v_door) (7)

	// Gym dynamic objects (bmx parkour)
	BikeParkourObjects[0] = CreateDynamicObject(2669,2848.1015625,-2243.1552734,99.0883789,0.0000000,0.0000000,90.0000000, .worldid = 5, .streamdistance = 300); //object(cj_chris_crate) (1)
	//BikeParkourObjects[1] = CreateDynamicObject(1381,2847.4970703,-2243.1191406,4901.4877930,0.0000000,0.0000000,69.7851562, .worldid = -1, .streamdistance = 300); //object(magnocrane_04) (1)

    MoveDynamicObject(BikeParkourObjects[0], 2848.1015625, -2238.1552734, 99.0883789, 0.5, 0.0, 0.0, 90.0);
	//MoveDynamicObject(BikeParkourObjects[1], 2847.4970703, -2243.1191406, 4901.4877930, 0.5, 0.0, 0.0, 69.7851562);
	
	gFerrisWheel = CreateObject( 18877, gFerrisOrigin[0], gFerrisOrigin[1], gFerrisOrigin[2],
	  							 0.0, 0.0, -270.0, 300.0 );
	CreateObject( 18878, gFerrisOrigin[0], gFerrisOrigin[1], gFerrisOrigin[2],
	  							 0.0, 0.0, -270.0, 300.0 );
 	for(new x;x<10;x++)
    {
        gFerrisCages[x] = CreateObject( 19316, gFerrisOrigin[0], gFerrisOrigin[1], gFerrisOrigin[2],
	  							 0.0, 0.0, -270.0, 300.0 );

        AttachObjectToObject( gFerrisCages[x], gFerrisWheel,
							  gFerrisCageOffsets[x][0],
							  gFerrisCageOffsets[x][1],
	  						  gFerrisCageOffsets[x][2],
							  0.0, 0.0, -270.0, false );
	}

	/* new tmpobjid; // for map
	#include "./modules/map/truck_sf"
	#include "./modules/map/near_sfvip"
	#include "./modules/map/hstreet_1"
	#include "./modules/map/hstreet_2"
	#include "./modules/map/hstreet_3"
	#include "./modules/map/hstreet_4"
	#include "./modules/map/hstreet_5"
	#include "./modules/map/hstreet_6"
	#include "./modules/map/garage_nearspawn"
	#include "./modules/map/pizza_SF"
	#include "./modules/map/cityhall_SF"
	#include "./modules/map/cityhall_Int"
	#include "./modules/map/chruch_SF"
	#include "./modules/map/police_SF"
	#include "./modules/map/driveschool"
	#include "./modules/map/ds-int"
	#include "./modules/map/sf_dock"
	#include "./modules/map/bank_sf"
	#include "./modules/map/hospital_int"
	#include "./modules/map/miner_int"
	#include "./modules/map/craft_sf" */

	Streamer_VisibleItems(STREAMER_TYPE_OBJECT, 800);
	print("[Streamer] Dynamic Objects has been loaded.");
	
	return 1;
}

forward RemoveBuildings(playerid);
public RemoveBuildings(playerid)
{
	/* // Bank San Fierro
	RemoveBuildingForPlayer(playerid, 10949, -2076.270, 359.320, 44.591, 0.250);
	RemoveBuildingForPlayer(playerid, 11024, -2076.270, 359.320, 44.591, 0.250);
	RemoveBuildingForPlayer(playerid, 3867, -2022.088, 364.578, 49.154, 999.000);
	RemoveBuildingForPlayer(playerid, 3868, -2022.088, 364.578, 49.154, 999.000);
	RemoveBuildingForPlayer(playerid, 10946, -2076.657, 354.367, 35.789, 0.250);
	RemoveBuildingForPlayer(playerid, 11403, -2076.657, 354.367, 35.789, 0.250);
	RemoveBuildingForPlayer(playerid, 792, -2032.000, 477.000, 34.270, 25.000);

	// San Fierro Dock
	RemoveBuildingForPlayer(playerid, 1232, -2938.447, 457.531, 6.500, 0.250);
	RemoveBuildingForPlayer(playerid, 1232, -2961.887, 484.015, 6.500, 0.250);
	RemoveBuildingForPlayer(playerid, 9829, -2920.570, 461.795, -61.388, 0.250);
	RemoveBuildingForPlayer(playerid, 9842, -2920.570, 461.795, -61.388, 0.250);
	RemoveBuildingForPlayer(playerid, 1280, -2886.590, 422.351, 4.289, 0.250);
	RemoveBuildingForPlayer(playerid, 1280, -2911.417, 422.351, 4.289, 0.250);
	// DriveSchool Interior
	RemoveBuildingForPlayer(playerid, 1721, -2022.446, -118.579, 1035.171, 20.000);

	// Police Station
	RemoveBuildingForPlayer(playerid, 10392, -2339.860107, -53.905998, 40.296001, 0.250000);
	RemoveBuildingForPlayer(playerid, 10507, -2339.860107, -53.905998, 40.296001, 0.250000);
	///////////////////////////////////////////////////////////////////////////////
    RemoveBuildingForPlayer(playerid, 6463, 389.7734, -2028.4688, 19.8047, 0.25);
	RemoveBuildingForPlayer(playerid, 3751, 389.8750, -2028.5000, 32.2266, 0.25);
	RemoveBuildingForPlayer(playerid, 3751, 389.8750, -2021.6406, 29.9297, 0.25);
	RemoveBuildingForPlayer(playerid, 3751, 389.8750, -2017.4531, 24.0313, 0.25);
	RemoveBuildingForPlayer(playerid, 3751, 389.8750, -2017.4297, 16.8516, 0.25);
	RemoveBuildingForPlayer(playerid, 3751, 389.8750, -2021.6328, 10.9844, 0.25);
	RemoveBuildingForPlayer(playerid, 3751, 389.8750, -2028.5156, 8.7813, 0.25);
	RemoveBuildingForPlayer(playerid, 3751, 389.8750, -2035.3984, 10.9453, 0.25);
	RemoveBuildingForPlayer(playerid, 3751, 389.8750, -2039.6406, 16.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 3751, 389.8750, -2039.6563, 24.1094, 0.25);
	RemoveBuildingForPlayer(playerid, 3751, 389.8750, -2035.3828, 29.9531, 0.25);
	RemoveBuildingForPlayer(playerid, 3752, 389.8750, -2039.6406, 16.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 3752, 389.8750, -2039.6563, 24.1094, 0.25);
	RemoveBuildingForPlayer(playerid, 6298, 389.7734, -2028.4688, 19.8047, 0.25);
	RemoveBuildingForPlayer(playerid, 6461, 389.7734, -2028.5000, 20.1094, 0.25);
	RemoveBuildingForPlayer(playerid, 3752, 389.8750, -2028.5000, 32.2266, 0.25);
	RemoveBuildingForPlayer(playerid, 3752, 389.8750, -2021.6406, 29.9297, 0.25);
	RemoveBuildingForPlayer(playerid, 3752, 389.8750, -2017.4531, 24.0313, 0.25);
	RemoveBuildingForPlayer(playerid, 3752, 389.8750, -2017.4297, 16.8516, 0.25);
	RemoveBuildingForPlayer(playerid, 3752, 389.8750, -2021.6328, 10.9844, 0.25);
	RemoveBuildingForPlayer(playerid, 3752, 389.8750, -2028.5156, 8.7813, 0.25);
	RemoveBuildingForPlayer(playerid, 3752, 389.8750, -2035.3984, 10.9453, 0.25);
	RemoveBuildingForPlayer(playerid, 3752, 389.8750, -2035.3828, 29.9531, 0.25);

	// Mailboxes
	RemoveBuildingForPlayer(playerid, 1258, 1445.0078, -1704.7656, 13.6953, 0.25);
	RemoveBuildingForPlayer(playerid, 1258, 1445.0078, -1692.2344, 13.6953, 0.25);
	RemoveBuildingForPlayer(playerid, 1258, 1510.8906, -1607.3125, 13.6953, 0.25);
	RemoveBuildingForPlayer(playerid, 1291, 231.2813, -1590.0547, 32.7734, 0.25);
	RemoveBuildingForPlayer(playerid, 1291, 546.6953, -1519.8594, 14.1094, 0.25);
	RemoveBuildingForPlayer(playerid, 1291, 300.3750, -1645.3750, 32.7578, 0.25);
	RemoveBuildingForPlayer(playerid, 1258, -2617.3984, 817.4375, 49.5859, 0.25);
	RemoveBuildingForPlayer(playerid, 1258, -2516.0781, 898.7813, 64.5703, 0.25);
	RemoveBuildingForPlayer(playerid, 1258, -2596.3672, 459.0234, 14.2266, 0.25);
	RemoveBuildingForPlayer(playerid, 1258, -2756.4375, 698.5547, 40.9531, 0.25);
	RemoveBuildingForPlayer(playerid, 1258, -2374.5313, 678.2109, 34.8047, 0.25);
	RemoveBuildingForPlayer(playerid, 3407, 322.6875, -131.1328, 0.5703, 0.25);
	RemoveBuildingForPlayer(playerid, 3408, 322.7422, -89.7734, 0.5469, 0.25);
	RemoveBuildingForPlayer(playerid, 3407, 242.8672, -131.0781, 0.5625, 0.25);
	RemoveBuildingForPlayer(playerid, 3408, 242.8750, -82.6641, 0.5547, 0.25);
	RemoveBuildingForPlayer(playerid, 3407, 270.7344, -61.7813, 0.5703, 0.25);
	RemoveBuildingForPlayer(playerid, 3408, 291.5625, -61.7266, 0.5625, 0.25);
	RemoveBuildingForPlayer(playerid, 3407, 731.8047, -578.8203, 15.3047, 0.25);
	RemoveBuildingForPlayer(playerid, 3408, 746.6484, -519.9688, 15.3281, 0.25);
	RemoveBuildingForPlayer(playerid, 3407, 746.6875, -540.2422, 15.3047, 0.25);
	RemoveBuildingForPlayer(playerid, 3407, 759.5391, -520.0703, 15.2969, 0.25);
	RemoveBuildingForPlayer(playerid, 3407, 782.8125, -520.0703, 15.2969, 0.25);
	RemoveBuildingForPlayer(playerid, 3408, 816.7969, -519.9688, 15.3281, 0.25);
	RemoveBuildingForPlayer(playerid, 1478, 1950.8438, 2732.2422, 10.4453, 0.25);
	RemoveBuildingForPlayer(playerid, 1478, 1933.3125, 2732.2422, 10.4453, 0.25);
	RemoveBuildingForPlayer(playerid, 1478, 1954.2500, 2752.1953, 10.4453, 0.25);
	RemoveBuildingForPlayer(playerid, 1478, 1965.7109, 2732.2422, 10.4453, 0.25);
	RemoveBuildingForPlayer(playerid, 1478, 1996.1250, 2752.1953, 10.4453, 0.25);
	RemoveBuildingForPlayer(playerid, 1478, 2023.0078, 2732.2422, 10.4453, 0.25);
	RemoveBuildingForPlayer(playerid, 1478, 2018.0703, 2732.2422, 10.4453, 0.25);
	RemoveBuildingForPlayer(playerid, 1478, 1999.8125, 2732.2422, 10.4453, 0.25);
	RemoveBuildingForPlayer(playerid, 1478, 2005.7734, 2752.1953, 10.4453, 0.25);
	RemoveBuildingForPlayer(playerid, 1478, 2037.7031, 2752.1953, 10.4453, 0.25);
	RemoveBuildingForPlayer(playerid, 1478, 2045.4844, 2752.1953, 10.4453, 0.25);

	// Gas Pumps
	RemoveBuildingForPlayer(playerid, 1676, 1941.6563, -1778.4531, 14.1406, 0.25);
	RemoveBuildingForPlayer(playerid, 1676, 1941.6563, -1774.3125, 14.1406, 0.25);
	RemoveBuildingForPlayer(playerid, 1676, 1941.6563, -1771.3438, 14.1406, 0.25);
	RemoveBuildingForPlayer(playerid, 1676, 1941.6563, -1767.2891, 14.1406, 0.25);
	RemoveBuildingForPlayer(playerid, 1686, -2410.8047, 970.8516, 44.4844, 0.25);
	RemoveBuildingForPlayer(playerid, 1686, -2410.8047, 976.1875, 44.4844, 0.25);
	RemoveBuildingForPlayer(playerid, 1686, -2410.8047, 981.5234, 44.4844, 0.25);
	RemoveBuildingForPlayer(playerid, 1676, -92.1016, -1161.7891, 2.9609, 0.25);
	RemoveBuildingForPlayer(playerid, 1676, -97.0703, -1173.7500, 3.0313, 0.25);
	RemoveBuildingForPlayer(playerid, 1676, -85.2422, -1165.0313, 2.6328, 0.25);
	RemoveBuildingForPlayer(playerid, 1676, -90.1406, -1176.6250, 2.6328, 0.25);
	RemoveBuildingForPlayer(playerid, 1686, -1685.9688, 409.6406, 6.3828, 0.25);
	RemoveBuildingForPlayer(playerid, 1686, -1679.3594, 403.0547, 6.3828, 0.25);
	RemoveBuildingForPlayer(playerid, 1686, -1681.8281, 413.7813, 6.3828, 0.25);
	RemoveBuildingForPlayer(playerid, 1686, -1675.2188, 407.1953, 6.3828, 0.25);
	RemoveBuildingForPlayer(playerid, 1686, -1676.5156, 419.1172, 6.3828, 0.25);
	RemoveBuildingForPlayer(playerid, 1686, -1669.9063, 412.5313, 6.3828, 0.25);
	RemoveBuildingForPlayer(playerid, 1686, -1672.1328, 423.5000, 6.3828, 0.25);
	RemoveBuildingForPlayer(playerid, 1686, -1665.5234, 416.9141, 6.3828, 0.25);

	//SAAS SF Base
	RemoveBuildingForPlayer(playerid, 10814, -1146.5547, -145.3047, 17.0781, 0.25);
	RemoveBuildingForPlayer(playerid, 3814, -1178.1016, -114.8281, 19.7656, 0.25);
	RemoveBuildingForPlayer(playerid, 3814, -1217.1406, -67.1719, 19.7656, 0.25);
	RemoveBuildingForPlayer(playerid, 3814, -1272.0781, -660.3281, 19.7656, 0.25);
	RemoveBuildingForPlayer(playerid, 3814, -1334.4766, -660.3281, 19.7656, 0.25);
	RemoveBuildingForPlayer(playerid, 3814, -1396.8750, -660.3281, 19.7656, 0.25);
	RemoveBuildingForPlayer(playerid, 3814, -1459.2734, -660.3281, 19.7656, 0.25);
	RemoveBuildingForPlayer(playerid, 3815, -1178.1016, -114.8281, 19.7656, 0.25);
	RemoveBuildingForPlayer(playerid, 3815, -1217.1406, -67.1719, 19.7656, 0.25);
	RemoveBuildingForPlayer(playerid, 3815, -1272.0781, -660.3281, 19.7656, 0.25);
	RemoveBuildingForPlayer(playerid, 3815, -1334.4766, -660.3281, 19.7656, 0.25);
	RemoveBuildingForPlayer(playerid, 3815, -1396.8750, -660.3281, 19.7656, 0.25);
	RemoveBuildingForPlayer(playerid, 3815, -1459.2734, -660.3281, 19.7656, 0.25);
	RemoveBuildingForPlayer(playerid, 1278, -1365.1563, -643.6875, 25.3359, 0.25);
	RemoveBuildingForPlayer(playerid, 968, -1526.4375, 481.3828, 6.9063, 0.25);
	RemoveBuildingForPlayer(playerid, 966, -1526.3906, 481.3828, 6.1797, 0.25);

	//NOOSE
	RemoveBuildingForPlayer(playerid, 785, -594.9141, -1954.3984, 31.5625, 0.25);
	RemoveBuildingForPlayer(playerid, 785, -635.9219, -1923.8828, 8.8125, 0.25);
	RemoveBuildingForPlayer(playerid, 785, -535.4766, -1851.0703, 8.8125, 0.25);
	RemoveBuildingForPlayer(playerid, 722, -827.9922, -2001.9375, 7.9453, 0.25);
	RemoveBuildingForPlayer(playerid, 722, -817.7266, -2025.5313, 17.7266, 0.25);
	RemoveBuildingForPlayer(playerid, 721, -794.8984, -2066.2422, 13.3438, 0.25);
	RemoveBuildingForPlayer(playerid, 721, -737.4844, -2112.3125, 20.3672, 0.25);
	RemoveBuildingForPlayer(playerid, 722, -686.2813, -1954.3203, 14.9531, 0.25);
	RemoveBuildingForPlayer(playerid, 721, -790.9297, -1900.4531, 3.3672, 0.25);
	RemoveBuildingForPlayer(playerid, 17027, -779.6563, -1845.1016, 27.1797, 0.25);
	RemoveBuildingForPlayer(playerid, 722, -737.9688, -1874.2578, 6.1719, 0.25);
	RemoveBuildingForPlayer(playerid, 722, -732.8906, -1837.1406, 15.3203, 0.25);
	RemoveBuildingForPlayer(playerid, 722, -713.1172, -1837.2266, 14.5703, 0.25);
	RemoveBuildingForPlayer(playerid, 722, -688.0234, -1867.7734, 7.4297, 0.25);
	RemoveBuildingForPlayer(playerid, 17076, -660.6719, -1958.4141, -0.0938, 0.25);
	RemoveBuildingForPlayer(playerid, 791, -594.9141, -1954.3984, 31.5625, 0.25);
	RemoveBuildingForPlayer(playerid, 791, -635.9219, -1923.8828, 8.8125, 0.25);
	RemoveBuildingForPlayer(playerid, 791, -535.4766, -1851.0703, 8.8125, 0.25);
	RemoveBuildingForPlayer(playerid, 17071, -464.1875, -1884.9531, 5.8984, 0.25);

	//San Fierro VIP Lounge -- Exterior
	// RemoveBuildingForPlayer(playerid, 10662, -2453.6563, 514.5000, 24.4375, 0.25);
	// RemoveBuildingForPlayer(playerid, 966, -2436.8516, 495.4531, 28.9531, 0.25);
	// RemoveBuildingForPlayer(playerid, 967, -2438.7266, 495.0078, 29.1016, 0.25);
	// RemoveBuildingForPlayer(playerid, 968, -2436.8125, 495.4688, 29.6797, 0.25);

	//Flint County Towing -- LS Office (Farva)
	RemoveBuildingForPlayer(playerid, 4976, 1931.0000, -1871.3906, 15.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 1412, 1834.3125, -1879.5547, 13.6797, 0.25);
	RemoveBuildingForPlayer(playerid, 1412, 1839.5859, -1879.5547, 13.6797, 0.25);
	RemoveBuildingForPlayer(playerid, 1412, 1844.8672, -1879.5547, 13.6797, 0.25);
	RemoveBuildingForPlayer(playerid, 1412, 1850.1406, -1879.5547, 13.6797, 0.25);
	RemoveBuildingForPlayer(playerid, 1412, 1855.4141, -1879.5547, 13.6797, 0.25);
	RemoveBuildingForPlayer(playerid, 1413, 1879.9922, -1879.8906, 13.6797, 0.25);
	RemoveBuildingForPlayer(playerid, 1413, 1885.2578, -1879.8984, 13.6797, 0.25);
	RemoveBuildingForPlayer(playerid, 1413, 1864.1797, -1879.6641, 13.6797, 0.25);
	RemoveBuildingForPlayer(playerid, 1413, 1869.4609, -1879.6641, 13.6797, 0.25);
	RemoveBuildingForPlayer(playerid, 1412, 1874.7344, -1879.6641, 13.6797, 0.25);
	RemoveBuildingForPlayer(playerid, 1412, 1890.5234, -1879.6641, 13.6797, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, 1869.0547, -1876.6328, 11.5391, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, 1915.0547, -1890.4531, 11.5391, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, 1951.7344, -1913.2656, 11.5156, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, 1971.1328, -1891.1484, 11.5156, 0.25);
	RemoveBuildingForPlayer(playerid, 1413, 1904.3750, -1879.7344, 13.6797, 0.25);
	RemoveBuildingForPlayer(playerid, 1413, 1909.6563, -1879.7344, 13.6797, 0.25);
	RemoveBuildingForPlayer(playerid, 1412, 1914.9297, -1879.7344, 13.6797, 0.25);
	RemoveBuildingForPlayer(playerid, 1413, 1920.1953, -1879.9531, 13.6797, 0.25);
	RemoveBuildingForPlayer(playerid, 1413, 1925.4609, -1879.9609, 13.6797, 0.25);
	RemoveBuildingForPlayer(playerid, 1412, 1930.7188, -1879.7344, 13.6797, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, 1969.4766, -1875.8906, 12.7188, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, 1946.4766, -1877.1094, 11.5391, 0.25);
	RemoveBuildingForPlayer(playerid, 4848, 1931.0000, -1871.3906, 15.8438, 0.25);
	RemoveBuildingForPlayer(playerid, 4983, 1961.0313, -1871.4063, 23.7734, 0.25);
	RemoveBuildingForPlayer(playerid, 1226, 1931.8750, -1863.4609, 16.3203, 0.25);
	RemoveBuildingForPlayer(playerid, 1226, 1915.7422, -1863.4609, 16.3203, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, 1952.0313, -1862.9297, 11.5156, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, 1971.1641, -1826.6953, 12.6953, 0.25);
	RemoveBuildingForPlayer(playerid, 1308, 1952.0313, -1801.0156, 11.7188, 0.25);
	RemoveBuildingForPlayer(playerid, 1440, 1141.9844, -1346.1094, 13.2656, 0.25);
	RemoveBuildingForPlayer(playerid, 1440, 1148.6797, -1385.1875, 13.2656, 0.25);
	RemoveBuildingForPlayer(playerid, 1411, 347.1953, 1799.2656, 18.7578, 0.25);
	RemoveBuildingForPlayer(playerid, 1411, 342.9375, 1796.2891, 18.7578, 0.25);
	RemoveBuildingForPlayer(playerid, 16094, 191.1406, 1870.0391, 21.4766, 0.25);
	// Car Parking Benh vien LS
	RemoveBuildingForPlayer(playerid, 5955, 1301.1875, -1257.0625, 21.5078, 0.25);
	RemoveBuildingForPlayer(playerid, 5967, 1259.4375, -1246.8125, 17.1094, 0.25);
	RemoveBuildingForPlayer(playerid, 1388, 1238.3750, -1258.2813, 57.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 1391, 1238.3750, -1258.2734, 44.6641, 0.25);
	RemoveBuildingForPlayer(playerid, 1294, 1254.6875, -1276.1641, 17.0781, 0.25);
	RemoveBuildingForPlayer(playerid, 5857, 1259.4375, -1246.8125, 17.1094, 0.25);
	RemoveBuildingForPlayer(playerid, 1219, 1284.1797, -1239.6406, 12.9141, 0.25);
	RemoveBuildingForPlayer(playerid, 5709, 1301.1875, -1257.0625, 21.5078, 0.25);
	RemoveBuildingForPlayer(playerid, 1412, 1327.4297, -1239.9844, 13.9375, 0.25);
	RemoveBuildingForPlayer(playerid, 1412, 1327.4297, -1234.7422, 13.9375, 0.25);
	RemoveBuildingForPlayer(playerid, 1219, 1332.8359, -1241.7188, 13.4141, 0.25);
	RemoveBuildingForPlayer(playerid, 3778, 337.4531, -1875.0000, 3.4063, 0.25);
	RemoveBuildingForPlayer(playerid, 3778, 289.2266, -1875.0000, 3.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 3615, 289.2266, -1875.0000, 3.2031, 0.25);
	RemoveBuildingForPlayer(playerid, 3615, 337.4531, -1875.0000, 3.4063, 0.25);

	// Trucker SF
	RemoveBuildingForPlayer(playerid, 10843, -1711.468, -107.569, 10.616, 0.250);
	RemoveBuildingForPlayer(playerid, 10892, -1711.468, -107.569, 10.616, 0.250);
	RemoveBuildingForPlayer(playerid, 1350, -1753.890, -111.234, 2.546, 0.250);
	RemoveBuildingForPlayer(playerid, 1226, -1728.599, -95.849, 6.302, 0.250);
	RemoveBuildingForPlayer(playerid, 1226, -1725.958, -64.794, 5.992, 0.250);
	RemoveBuildingForPlayer(playerid, 1226, -1679.921, -47.631, 6.656, 0.250);

	// Near SF VIP
	RemoveBuildingForPlayer(playerid, 768, -2403.396, 678.781, 33.958, 0.250);
	RemoveBuildingForPlayer(playerid, 713, -2418.270, 677.382, 33.937, 0.250);
	RemoveBuildingForPlayer(playerid, 715, -2441.729, 686.953, 42.289, 0.250);
	RemoveBuildingForPlayer(playerid, 768, -2461.300, 692.765, 33.875, 0.250);
	RemoveBuildingForPlayer(playerid, 715, -2409.468, 648.992, 41.812, 0.250);
	RemoveBuildingForPlayer(playerid, 768, -2479.489, 690.078, 33.375, 0.250);
	RemoveBuildingForPlayer(playerid, 715, -2467.677, 676.007, 41.465, 0.250);
	RemoveBuildingForPlayer(playerid, 1283, -2390.427, 660.039, 37.208, 0.250);
	RemoveBuildingForPlayer(playerid, 1306, -2225.350, 642.414, 54.429, 0.250);

	// House Street 1
	RemoveBuildingForPlayer(playerid, 3877, -2652.698, 695.117, 67.866, 0.250);
	RemoveBuildingForPlayer(playerid, 3877, -2706.198, 695.117, 67.866, 0.250);
	RemoveBuildingForPlayer(playerid, 3877, -2652.697, 695.117, 67.865, 0.250);
	RemoveBuildingForPlayer(playerid, 3877, -2706.197, 695.117, 67.865, 0.250);
	RemoveBuildingForPlayer(playerid, 9507, -2692.080, 828.984, 61.101, 0.250);
	RemoveBuildingForPlayer(playerid, 9672, -2692.080, 828.984, 61.101, 0.250);
	RemoveBuildingForPlayer(playerid, 9823, -2696.010, 821.445, 50.851, 0.250);
	RemoveBuildingForPlayer(playerid, 9566, -2692.648, 825.085, 61.702, 0.250);
	RemoveBuildingForPlayer(playerid, 673, -2617.000, 822.859, 48.772, 0.250);
	RemoveBuildingForPlayer(playerid, 9513, -2642.770, 823.921, 53.734, 0.250);
	RemoveBuildingForPlayer(playerid, 9674, -2642.770, 823.921, 53.734, 0.250);

	// House Street 2
	RemoveBuildingForPlayer(playerid, 737, -1791.098, 827.757, 24.304, 0.250);
	RemoveBuildingForPlayer(playerid, 737, -1780.369, 827.757, 24.304, 0.250);
	RemoveBuildingForPlayer(playerid, 737, -1769.640, 827.757, 24.304, 0.250);
	RemoveBuildingForPlayer(playerid, 737, -1758.910, 827.757, 24.304, 0.250);
	RemoveBuildingForPlayer(playerid, 737, -1748.180, 827.757, 24.304, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 820.781, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 810.117, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 799.453, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 788.789, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 778.125, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 767.460, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 758.335, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 737, -1791.098, 827.757, 24.304, 0.250);
	RemoveBuildingForPlayer(playerid, 737, -1780.369, 827.757, 24.304, 0.250);
	RemoveBuildingForPlayer(playerid, 737, -1769.640, 827.757, 24.304, 0.250);
	RemoveBuildingForPlayer(playerid, 737, -1758.910, 827.757, 24.304, 0.250);
	RemoveBuildingForPlayer(playerid, 737, -1748.180, 827.757, 24.304, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 820.781, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 810.117, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 799.453, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 788.789, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 778.125, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 767.460, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 758.335, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 10149, -1695.067, 1035.619, 45.701, 0.250);
	//RemoveBuildingForPlayer(playerid, 10194, -1754.199, 1019.299, 60.460, 0.250);
	RemoveBuildingForPlayer(playerid, 10238, -1754.199, 1019.299, 60.460, 0.250);

	// House Street 3
	RemoveBuildingForPlayer(playerid, 673, -2177.590, 972.851, 78.234, 0.250);
	RemoveBuildingForPlayer(playerid, 673, -2194.860, 961.546, 78.234, 0.250);
	RemoveBuildingForPlayer(playerid, 673, -2182.928, 961.546, 78.234, 0.250);
	RemoveBuildingForPlayer(playerid, 9982, -2191.302, 969.804, 83.132, 0.250);
	RemoveBuildingForPlayer(playerid, 10015, -2191.302, 969.804, 83.132, 0.250);
	RemoveBuildingForPlayer(playerid, 10014, -2187.340, 1012.229, 89.500, 0.250);
	RemoveBuildingForPlayer(playerid, 9981, -2187.340, 1012.229, 89.500, 0.250);

	// House Street 4
	RemoveBuildingForPlayer(playerid, 1412, -2303.794, 85.990, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2300.312, 90.000, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 3819, -2292.507, 95.648, 35.312, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2298.601, 94.638, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2298.601, 99.953, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2298.601, 105.273, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 3819, -2292.419, 119.484, 35.312, 0.250);
	RemoveBuildingForPlayer(playerid, 3819, -2292.333, 107.703, 35.312, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2314.646, 139.828, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2319.740, 138.460, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2324.687, 136.570, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1341, -2286.539, 147.632, 35.319, 0.250);
	RemoveBuildingForPlayer(playerid, 673, -2288.877, 142.507, 34.367, 0.250);
	RemoveBuildingForPlayer(playerid, 11035, -2190.437, 5.570, 48.452, 0.250);
	RemoveBuildingForPlayer(playerid, 10990, -2190.437, 5.570, 48.452, 0.250);
	RemoveBuildingForPlayer(playerid, 11033, -2229.968, -22.531, 46.155, 0.250);
	RemoveBuildingForPlayer(playerid, 11001, -2229.968, -22.531, 46.155, 0.250);
	RemoveBuildingForPlayer(playerid, 11032, -2229.272, 46.679, 47.319, 0.250);
	RemoveBuildingForPlayer(playerid, 792, -2245.553, 54.405, 34.444, 0.250);
	RemoveBuildingForPlayer(playerid, 11000, -2229.272, 46.679, 47.319, 0.250);
	RemoveBuildingForPlayer(playerid, 3819, -2289.521, 204.875, 35.312, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2298.638, 204.828, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2300.646, 209.186, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2304.656, 212.671, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2325.239, 214.382, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2319.927, 214.382, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2314.610, 214.382, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2309.300, 214.382, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 774, -2309.600, 392.234, 34.164, 0.250);
	RemoveBuildingForPlayer(playerid, 767, -2321.447, 384.178, 34.164, 0.250);
	RemoveBuildingForPlayer(playerid, 774, -2332.917, 371.976, 34.164, 0.250);
	RemoveBuildingForPlayer(playerid, 737, -2344.697, 375.234, 34.271, 0.250);
	RemoveBuildingForPlayer(playerid, 705, -2339.520, 366.773, 34.437, 0.250);
	RemoveBuildingForPlayer(playerid, 705, -2339.620, 353.570, 34.414, 0.250);
	RemoveBuildingForPlayer(playerid, 705, -2329.570, 335.156, 34.279, 0.250);
	RemoveBuildingForPlayer(playerid, 772, -2347.887, 319.125, 36.296, 0.250);
	RemoveBuildingForPlayer(playerid, 703, -2368.850, 334.703, 34.492, 0.250);
	RemoveBuildingForPlayer(playerid, 772, -2360.550, 336.664, 34.164, 0.250);
	RemoveBuildingForPlayer(playerid, 772, -2356.417, 323.256, 34.164, 0.250);
	RemoveBuildingForPlayer(playerid, 703, -2350.458, 344.898, 34.492, 0.250);
	RemoveBuildingForPlayer(playerid, 1308, -2343.090, 371.367, 34.476, 0.250);
	RemoveBuildingForPlayer(playerid, 10705, -2416.020, 401.125, 45.976, 0.250);
	RemoveBuildingForPlayer(playerid, 10706, -2398.300, 268.178, 44.179, 0.250);
	RemoveBuildingForPlayer(playerid, 792, -2312.468, 431.492, 34.271, 0.250);
	RemoveBuildingForPlayer(playerid, 768, -2403.397, 678.781, 33.958, 0.250);
	RemoveBuildingForPlayer(playerid, 713, -2418.270, 677.382, 33.937, 0.250);
	RemoveBuildingForPlayer(playerid, 715, -2441.729, 686.953, 42.289, 0.250);
	RemoveBuildingForPlayer(playerid, 768, -2461.300, 692.765, 33.875, 0.250);
	RemoveBuildingForPlayer(playerid, 715, -2409.468, 648.992, 41.812, 0.250);
	RemoveBuildingForPlayer(playerid, 768, -2479.489, 690.078, 33.375, 0.250);
	RemoveBuildingForPlayer(playerid, 715, -2467.677, 676.007, 41.466, 0.250);
	RemoveBuildingForPlayer(playerid, 1283, -2390.427, 660.039, 37.208, 0.250);
	RemoveBuildingForPlayer(playerid, 1226, -1952.479, 738.132, 48.234, 0.250);
	RemoveBuildingForPlayer(playerid, 1226, -1919.338, 724.476, 48.234, 0.250);
	RemoveBuildingForPlayer(playerid, 737, -1791.098, 827.757, 24.304, 0.250);
	RemoveBuildingForPlayer(playerid, 737, -1780.369, 827.757, 24.304, 0.250);
	RemoveBuildingForPlayer(playerid, 737, -1769.640, 827.757, 24.304, 0.250);
	RemoveBuildingForPlayer(playerid, 737, -1758.910, 827.757, 24.304, 0.250);
	RemoveBuildingForPlayer(playerid, 737, -1748.180, 827.757, 24.304, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 820.781, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 810.117, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 799.453, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 788.789, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 778.125, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 767.460, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 758.335, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 737, -1791.098, 827.757, 24.304, 0.250);
	RemoveBuildingForPlayer(playerid, 737, -1780.369, 827.757, 24.304, 0.250);
	RemoveBuildingForPlayer(playerid, 737, -1769.640, 827.757, 24.304, 0.250);
	RemoveBuildingForPlayer(playerid, 737, -1758.910, 827.757, 24.304, 0.250);
	RemoveBuildingForPlayer(playerid, 737, -1748.180, 827.757, 24.304, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 820.781, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 810.117, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 799.453, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 788.789, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 778.125, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 767.460, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 717, -1727.437, 758.335, 24.117, 0.250);
	RemoveBuildingForPlayer(playerid, 10148, -1688.109, 1036.270, 46.201, 0.250);
	RemoveBuildingForPlayer(playerid, 10258, -1688.109, 1036.270, 46.201, 0.250);
	RemoveBuildingForPlayer(playerid, 10149, -1695.067, 1035.619, 45.701, 0.250);
	RemoveBuildingForPlayer(playerid, 10194, -1754.199, 1019.299, 60.460, 0.250);
	RemoveBuildingForPlayer(playerid, 10238, -1754.199, 1019.299, 60.460, 0.250);
	
	// House Street 5
	RemoveBuildingForPlayer(playerid, 1468, -2346.044, 179.070, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2342.695, 174.960, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2351.218, 188.296, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2348.888, 183.539, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2354.482, 198.343, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2353.107, 193.250, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2298.638, 194.203, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 3819, -2289.607, 181.039, 35.312, 0.250);
	RemoveBuildingForPlayer(playerid, 3819, -2289.437, 193.093, 35.312, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2298.638, 199.514, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 3819, -2289.521, 204.875, 35.312, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2298.638, 204.828, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2300.646, 209.186, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2304.656, 212.671, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2325.239, 214.382, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2319.927, 214.382, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2314.610, 214.382, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2309.300, 214.382, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 774, -2309.600, 392.234, 34.164, 0.250);
	RemoveBuildingForPlayer(playerid, 767, -2321.447, 384.178, 34.164, 0.250);
	RemoveBuildingForPlayer(playerid, 774, -2332.917, 371.976, 34.164, 0.250);
	RemoveBuildingForPlayer(playerid, 737, -2344.697, 375.234, 34.271, 0.250);
	RemoveBuildingForPlayer(playerid, 705, -2339.520, 366.773, 34.437, 0.250);
	RemoveBuildingForPlayer(playerid, 705, -2339.620, 353.570, 34.414, 0.250);
	RemoveBuildingForPlayer(playerid, 705, -2329.570, 335.156, 34.279, 0.250);
	RemoveBuildingForPlayer(playerid, 772, -2347.887, 319.125, 36.296, 0.250);
	RemoveBuildingForPlayer(playerid, 703, -2368.850, 334.703, 34.492, 0.250);
	RemoveBuildingForPlayer(playerid, 772, -2360.550, 336.664, 34.164, 0.250);
	RemoveBuildingForPlayer(playerid, 772, -2356.417, 323.256, 34.164, 0.250);
	RemoveBuildingForPlayer(playerid, 703, -2350.458, 344.898, 34.492, 0.250);
	RemoveBuildingForPlayer(playerid, 1308, -2343.090, 371.367, 34.476, 0.250);
	RemoveBuildingForPlayer(playerid, 10705, -2416.020, 401.125, 45.976, 0.250);
	RemoveBuildingForPlayer(playerid, 10706, -2398.300, 268.178, 44.179, 0.250);
	RemoveBuildingForPlayer(playerid, 792, -2312.468, 431.492, 34.271, 0.250);
	RemoveBuildingForPlayer(playerid, 1570, -2198.979, 636.710, 49.694, 0.250);
	RemoveBuildingForPlayer(playerid, 1570, -2195.406, 636.710, 49.694, 0.250);
	RemoveBuildingForPlayer(playerid, 1570, -2189.550, 636.710, 49.694, 0.250);
	RemoveBuildingForPlayer(playerid, 1570, -2183.780, 636.710, 49.694, 0.250);
	RemoveBuildingForPlayer(playerid, 1342, -2193.770, 605.187, 35.208, 0.250);
	RemoveBuildingForPlayer(playerid, 1342, -2197.590, 613.406, 35.208, 0.250);
	RemoveBuildingForPlayer(playerid, 1342, -2194.020, 613.406, 35.208, 0.250);
	RemoveBuildingForPlayer(playerid, 1342, -2187.145, 614.546, 35.208, 0.250);
	RemoveBuildingForPlayer(playerid, 1342, -2181.479, 613.703, 35.208, 0.250);
	RemoveBuildingForPlayer(playerid, 1306, -2225.350, 642.414, 54.429, 0.250);

	// House Street 6
	RemoveBuildingForPlayer(playerid, 1468, -2353.218, 100.859, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2303.794, 85.990, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2300.312, 90.000, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 3819, -2292.507, 95.648, 35.312, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2298.601, 94.638, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2298.601, 99.953, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2298.601, 105.273, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2350.240, 110.968, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2344.982, 120.099, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2347.857, 115.685, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2352.044, 106.007, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2329.445, 134.233, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2333.914, 131.389, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2338.021, 128.039, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2341.695, 124.240, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 3819, -2292.419, 119.484, 35.312, 0.250);
	RemoveBuildingForPlayer(playerid, 3819, -2292.333, 107.703, 35.312, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2314.646, 139.828, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2319.740, 138.460, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2324.687, 136.570, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1341, -2286.539, 147.632, 35.319, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2315.513, 159.764, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2325.625, 162.742, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2320.664, 160.936, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2330.343, 165.125, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2346.044, 179.070, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2342.695, 174.960, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2338.896, 171.289, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2334.757, 168.000, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2351.218, 188.296, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2348.888, 183.539, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2354.482, 198.343, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1468, -2353.107, 193.250, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2298.638, 194.203, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 3819, -2289.607, 181.039, 35.312, 0.250);
	RemoveBuildingForPlayer(playerid, 3819, -2289.437, 193.093, 35.312, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2298.638, 199.514, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 3819, -2289.521, 204.875, 35.312, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2298.638, 204.828, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2300.646, 209.186, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2304.656, 212.671, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 673, -2288.877, 142.507, 34.367, 0.250);
	RemoveBuildingForPlayer(playerid, 673, -2354.927, 142.147, 34.367, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2325.239, 214.382, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2319.927, 214.382, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2314.610, 214.382, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2309.300, 214.382, 35.583, 0.250);
	RemoveBuildingForPlayer(playerid, 708, -2362.132, 166.468, 34.631, 0.250);
	RemoveBuildingForPlayer(playerid, 705, -2393.552, 103.093, 34.590, 0.250);
	RemoveBuildingForPlayer(playerid, 705, -2388.239, 148.983, 34.590, 0.250);
	RemoveBuildingForPlayer(playerid, 673, -2403.927, 194.125, 34.367, 0.250);
	RemoveBuildingForPlayer(playerid, 10706, -2398.300, 268.178, 44.179, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2308.159, 83.984, 35.585, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2313.469, 83.984, 35.585, 0.250);
	RemoveBuildingForPlayer(playerid, 1412, -2318.790, 83.984, 35.585, 0.250); */
	return 1;
}

