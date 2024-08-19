class RTConstants
{
	/* ACTION CONDITION DEFAULTS */
	static const float RT_DISTANCE_DEFAULT = 2.0;
	/* EVENT DEFAULTS */
	static const float RT_EVENT_SPAWN_INTERVAL_DEFAULT = 3600;
	static const float RT_EVENT_CAPTURETIME_DEFAULT = 600;
	static const float RT_EVENT_LIFETIME_DEFAULT = 1800;
	static const float RT_EVENT_TOTAL_CAPTURE_PCT_DEFAULT = 100;
	static const float RT_EVENT_UPDATE_PROGRESS_INTERVAL_DEFAULT = 1;
    static const float RT_EVENT_TRIGGER_HEIGHT_DEFAULT = 25;
	static const float RT_EVENT_TRIGGER_RADIUS_DEFAULT = 25;
	static const float RT_TIMETOHACK_DEFAULT = 1;
	static const bool RT_ALLOW_SAME_EVENT_SPAWN_IN_A_ROW = true;
	static const bool RT_ENABLE_LOGGING = false;
	static const string RT_MAP_MARKER_TEXT = "KOTH: %1";
	/* PATHS/DIRS */
	static const string RT_ROOTPATH = "$profile:RadioTower/";
	static const string RT_LOGPATH = "$profile:RadioTower/Logs/";
	static const string RT_BACKUPPATH = "$profile:RadioTower/Backups/";
	/* PATHS/BACKUPS */
	static const string RT_BACKUPPATH_SETTINGS = "$profile:RadioTower/Backups/RTSettings/";
	static const string RT_BACKUPPATH_LOCATIONS = "$profile:RadioTower/Backups/RTLocations/";
	static const string RT_BACKUPPATH_PROPS = "$profile:RadioTower/Backups/RTProps/";
	static const string RT_BACKUPPATH_LOOTSETS = "$profile:RadioTower/Backups/RTLootSets/";
	/* PATHS/CONFIGS */
    static const string RT_SETTINGS_CONFIGPATH = "$profile:RadioTower/RTSettings.json";
	static const string RT_LOCATIONS_CONFIGPATH = "$profile:RadioTower/RTLocations.json";
	static const string RT_PROPS_CONFIGPATH = "$profile:RadioTower/RTProps.json";
	static const string RT_LOOTSETS_CONFIGPATH = "$profile:RadioTower/RTLootSets.json";
	/* LOGGING */
	static const string RT_LOG_PREFIX = "RT_";
	/* ICONS */
	static const string RT_ICON = "RadioTower/RadioTower/Data/Icons/rt_logo.paa";
	static const autoptr TStringArray RT_CITY_ZOMBIES = {"ZmbF_CitizenANormal_Beige","ZmbF_CitizenANormal_Blue","ZmbF_CitizenANormal_Brown","ZmbF_CitizenBSkinny","ZmbF_Clerk_Normal_Blue","ZmbF_Clerk_Normal_Green","ZmbF_Clerk_Normal_Red","ZmbF_Clerk_Normal_White","ZmbF_DoctorSkinny","ZmbF_JoggerSkinny_Blue",
			"ZmbF_JoggerSkinny_Brown","ZmbF_JoggerSkinny_Green","ZmbF_JoggerSkinny_Red","ZmbF_JournalistNormal_Blue","ZmbF_JournalistNormal_Green","ZmbF_JournalistNormal_Red","ZmbF_JournalistNormal_White","ZmbF_NurseFat","ZmbF_ParamedicNormal_Blue","ZmbF_ParamedicNormal_Green",
			"ZmbF_ParamedicNormal_Red","ZmbF_PatientOld","ZmbF_PoliceWomanNormal","ZmbF_ShortSkirt_beige","ZmbF_ShortSkirt_black","ZmbF_ShortSkirt_brown","ZmbF_ShortSkirt_checks","ZmbF_ShortSkirt_green","ZmbF_ShortSkirt_grey","ZmbF_ShortSkirt_red",
			"ZmbF_ShortSkirt_stripes","ZmbF_ShortSkirt_white","ZmbF_ShortSkirt_yellow","ZmbF_SkaterYoung_Brown","ZmbF_SkaterYoung_Striped","ZmbF_SkaterYoung_Violet","ZmbM_CitizenASkinny_Blue","ZmbM_CitizenASkinny_Brown","ZmbM_CitizenASkinny_Grey","ZmbM_CitizenASkinny_Red",
			"ZmbM_CitizenBFat_Blue","ZmbM_CitizenBFat_Green","ZmbM_CitizenBFat_Red","ZmbM_ClerkFat_Brown","ZmbM_ClerkFat_Grey","ZmbM_ClerkFat_Khaki","ZmbM_ClerkFat_White","ZmbM_CommercialPilotOld_Blue","ZmbM_CommercialPilotOld_Brown","ZmbM_CommercialPilotOld_Grey",
			"ZmbM_CommercialPilotOld_Olive","ZmbM_DoctorFat","ZmbM_Jacket_beige","ZmbM_Jacket_black","ZmbM_Jacket_blue","ZmbM_Jacket_bluechecks","ZmbM_Jacket_brown","ZmbM_Jacket_greenchecks","ZmbM_Jacket_grey",
			"ZmbM_Jacket_khaki","ZmbM_Jacket_magenta","ZmbM_Jacket_stripes","ZmbM_JoggerSkinny_Blue","ZmbM_JoggerSkinny_Green","ZmbM_JoggerSkinny_Red","ZmbM_JournalistSkinny","ZmbM_ParamedicNormal_Black","ZmbM_ParamedicNormal_Blue","ZmbM_ParamedicNormal_Green",
			"ZmbM_ParamedicNormal_Red","ZmbM_PatientSkinny","ZmbM_PatrolNormal_Autumn","ZmbM_PatrolNormal_Flat","ZmbM_PatrolNormal_PautRev","ZmbM_PatrolNormal_Summer","ZmbM_PolicemanFat","ZmbM_PolicemanSpecForce","ZmbM_PrisonerSkinny","ZmbM_SkaterYoung_Blue",
			"ZmbM_SkaterYoung_Brown","ZmbM_SkaterYoung_Green","ZmbM_SkaterYoung_Grey"};
	static const autoptr TStringArray RT_COUNTRY_ZOMBIES = {"ZmbF_HikerSkinny_Blue","ZmbF_HikerSkinny_Green","ZmbF_HikerSkinny_Grey","ZmbF_HikerSkinny_Red","ZmbF_MilkMaidOld_Beige","ZmbF_MilkMaidOld_Black","ZmbF_MilkMaidOld_Green","ZmbF_MilkMaidOld_Grey","ZmbF_SurvivorNormal_Blue","ZmbF_SurvivorNormal_Orange",
			"ZmbF_SurvivorNormal_Red","ZmbF_SurvivorNormal_White","ZmbF_VillagerOld_Blue","ZmbF_VillagerOld_Green","ZmbF_VillagerOld_Red","ZmbF_VillagerOld_White","ZmbM_FarmerFat_Beige","ZmbM_FarmerFat_Blue","ZmbM_FarmerFat_Brown","ZmbM_FarmerFat_Green",
			"ZmbM_HermitSkinny_Beige","ZmbM_HermitSkinny_Black","ZmbM_HermitSkinny_Green","ZmbM_HermitSkinny_Red","ZmbM_HikerSkinny_Blue","ZmbM_HikerSkinny_Green","ZmbM_HikerSkinny_Yellow","ZmbM_HunterOld_Autumn","ZmbM_HunterOld_Spring","ZmbM_HunterOld_Summer","ZmbM_HunterOld_Winter",
			"ZmbM_MotobikerFat_Beige","ZmbM_MotobikerFat_Black","ZmbM_MotobikerFat_Blue","ZmbM_VillagerOld_Blue","ZmbM_VillagerOld_Green","ZmbM_VillagerOld_White","ZmbM_priestPopSkinny"};
	static const autoptr TStringArray RT_INDUSTRIAL_ZOMBIES = {"ZmbF_BlueCollarFat_Green","ZmbF_BlueCollarFat_Red","ZmbF_BlueCollarFat_White","ZmbF_MechanicNormal_Beige","ZmbF_MechanicNormal_Green","ZmbF_MechanicNormal_Grey","ZmbF_MechanicNormal_Orange","ZmbM_ConstrWorkerNormal_Beige","ZmbM_ConstrWorkerNormal_Black","ZmbM_ConstrWorkerNormal_Green",
			"ZmbM_ConstrWorkerNormal_Grey","ZmbM_FirefighterNormal","ZmbM_FishermanOld_Blue","ZmbM_FishermanOld_Green","ZmbM_FishermanOld_Grey","ZmbM_FishermanOld_Red","ZmbM_HandymanNormal_Beige","ZmbM_HandymanNormal_Blue","ZmbM_HandymanNormal_Green","ZmbM_HandymanNormal_Grey",
			"ZmbM_HandymanNormal_White","ZmbM_HeavyIndustryWorker","ZmbM_MechanicSkinny_Blue","ZmbM_MechanicSkinny_Green","ZmbM_MechanicSkinny_Grey","ZmbM_MechanicSkinny_Red","ZmbM_OffshoreWorker_Green","ZmbM_OffshoreWorker_Orange","ZmbM_OffshoreWorker_Red","ZmbM_OffshoreWorker_Yellow"};
	static const autoptr TStringArray RT_MILITARY_ZOMBIES = {"ZmbM_PatrolNormal_Autumn","ZmbM_PatrolNormal_Flat","ZmbM_PatrolNormal_PautRev","ZmbM_PatrolNormal_Summer","ZmbM_SoldierNormal"};
	
	static string GetRandomZombieClassname()
	{	
		array<TStringArray> spawnableZombies = {RT_CITY_ZOMBIES, RT_COUNTRY_ZOMBIES, RT_INDUSTRIAL_ZOMBIES, RT_MILITARY_ZOMBIES};
		int type = spawnableZombies.GetRandomIndex();
		TStringArray zombies = spawnableZombies[type];
		int index = zombies.GetRandomIndex();
		return zombies[index];
	}
};