#define SERVER_VERSION "v2.0" 
#define MIXED_SPELLINGS

#define MAIL_URL 		"http://localhost/htb/m_mail.php"
#define MAIL_ADDRESS 	""
#define M_MAIL_NAME		"HTB No-Reply"

#pragma warning disable 238
#pragma warning disable 208, 213, 201, 217, 219, 209, 235, 202, 204, 214, 239, 234, 211, 200

#define	NO_TAGS
#include <open.mp>

#include <a_mysql>
#include <streamer>
#include <yom_buttons>
#include <Pawn.CMD>
#include <sscanf2>
#include <progress2>
#include <crashdetect>
#include <colandreas>
#include <discord-connector>

#define YSI_NO_HEAP_MALLOC
#define YSI_NO_MODE_CACHE
#define YSI_NO_OPTIMISATION_MESSAGE

#define CGEN_MEMORY		(40000)

#include <YSI\YSI_Data\y_foreach>
#include <YSI\YSI_Coding\y_timers>
#include <YSI\YSI_Coding\y_va>
#include <YSI\YSI_Core\y_utils>
#include <YSI\YSI_Game\y_vehicledata>

#include <strlib>
#include <string>
#include <GPS>
#include <wave-gps>
#include <memory>
#include <vector>
#include <PreviewModelDialog>
#include <dialog-pages>
#include <m_mail>
#include <pawn-easing-function>
#include <EVF2>
#include <td-actions>

#define OVERRIDE_NATIVE_GAMETEXT 
#include <gametext_plus>

#include "./modules/header"
#include "./modules/enums"

#include "./modules/discord/header"
#include "./modules/variables"
#include "./modules/dialog/header"
#include "./modules/actor/header"

#include "./modules/misc/msg"

//==============JOB HEADER============//
#include "./modules/job/header"
#include "./modules/job/miner/header"
#include "./modules/job/trucker/header"
#include "./modules/job/porter/header"
#include "./modules/job/pizzaboy/header"
#include "./modules/job/construction/header"
#include "./modules/job/lumber/header"

//==============ANTI CHECK============//
#include "./modules/anti/Teleport"
#include "./modules/anti/Main.pwn"
#include "./modules/anti/anti-f4t"
#include "./modules/anti/anti-vpn"

//==============EVENT============//
#include "./modules/Event/SanBoss"

#include "./modules/administrator/cmd"

//==============FAMILY SYSTEM============//
#include "./modules/system/family/assets"
#include "./modules/system/family/turfwars"
#include "./modules/system/family/management"
#include "./modules/system/family/admin"

//==============SYSTEM============//
#include "./modules/system/ruatho"
#include "./modules/system/safezone"
#include "./modules/system/faction/leader_cmd"
#include "./modules/system/logo"
//#include "./modules/system/logo_anim"
#include "./modules/system/support_notif"
#include "./modules/system/notification"
#include "./modules/system/bank"
#include "./modules/system/phone/phone"
#include "./modules/system/phone/callback_phone"
#include "./modules/system/panel/ui_panel"
#include "./modules/system/panel/panel"
#include "./modules/system/food"
#include "./modules/system/sendlocation"

//==============JOB MAIN============//
#include "./modules/job/painter/main"
#include "./modules/job/cansa/main"
#include "./modules/job/fishing/main"
#include "./modules/job/lumber/main"

//==============PLAYER============//
#include "./modules/player/rentcar/header"
#include "./modules/player/inventory/header"
#include "./modules/system/faction/police"
#include "./modules/player/mission/header"
//	#include "./modules/player/loading_circle"
#include "./modules/player/suckhoe"

//==============VEHICLE============//
#include "./modules/vehicle/trunk.inc"

//==============HOSPITAL============//
#include "./modules/system/hospital_spawn"
//#include "./modules/player/phone"

#include "./modules/timers"
#include "./modules/mysql"
#include "./modules/functions"
#include "./modules/commands"
#include "./modules/callbacks"
#include "./modules/textdraws"	
#include "./modules/streamer"

#include "./modules/discord/main"
#include "./modules/discord/command"

#include "./modules/actor/main"
#include "./modules/misc/cityhall"
#include "./modules/misc/license"

#include "./modules/job/main"
#include "./modules/job/taxi/taxi"
#include "./modules/job/trucker/trucker"
#include "./modules/job/porter/porter"
#include "./modules/job/pizzaboy/pizzaboy"
#include "./modules/job/construction/construction"
#include "./modules/job/electric/electric"
#include "./modules/job/miner/miner"
#include "./modules/job/miner/craft"
//#include "./modules/job/side_job/fisher/fisher"
#include "./modules/job/side_job/sweeper/sweeper"

#include "./modules/player/rentcar/rentcar"
//#include "./modules/player/inventory/inventory"
//#include "./modules/player/inventory/drop"
#include "./modules/player/inventory/inv_td"
#include "./modules/player/card/card"
#include "./modules/player/inventory/inv"
#include "./modules/player/mission/newbie"

#include "./modules/player/loading"
#include "./modules/player/nametag"
#include "./modules/player/settings"
#include "./modules/player/payday"
#include "./modules/player/stats"
#include "./modules/player/gps"
#include "./modules/player/help"
#include "./modules/player/player_load"
#include "./modules/player/enter_exit"
//#include "./modules/player/mail"
#include "./modules/player/speedo"
//#include "./modules/player/send_location"

#include "./modules/dialog/main"

main() {}

public OnGameModeInit()
{
	print("Dang cau hinh plugin, xin vui long cho doi...");
	//SetCrashDetectLongCallTime(1000000);
	CA_Init();
	print("Hoan tat cau hinh plugin, chuan bi tai gamemode...");
	g_mysql_Init();
	Discord_Init();
	// Family
	FamilyAssets_InitMySQL();
	FamilyAssets_Load();
	TurfWars_InitMySQL();
	TurfWars_Load();
	TurfSupplyPoints_InitMySQL();
	TurfSupplyPoints_Load();
	return 1;
}

public OnGameModeExit()
{
    g_mysql_Exit();
	return 1;
}