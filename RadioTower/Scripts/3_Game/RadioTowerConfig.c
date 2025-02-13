const int RT_VERSION = 131102024;

class RTSettings
{
	int version;
	ref RTSettingsEvent kothEvent;
	ref RTSettingsLogging logging;
	ref RTSettingsNotifications notifications;
	ref RTSettingsMapMarkers mapMarkers;
	ref RTSettingsUI ui;
	
	void Defaults()
	{	
		version 		= RT_VERSION;
		kothEvent 		= new RTSettingsEvent();
		logging 		= new RTSettingsLogging();
		notifications 	= new RTSettingsNotifications();
		mapMarkers 		= new RTSettingsMapMarkers();
		ui 				= new RTSettingsUI();
		
		kothEvent.Defaults();
		logging.Defaults();
		notifications.Defaults();
		mapMarkers.Defaults();
		ui.Defaults();
	}
	
	void Validate() 
	{
		if (kothEvent.defaultLootcrate == "")
			kothEvent.defaultLootcrate = "RTLootcrate_White";
		if (kothEvent.minPlayerCountForSpawn < 0)
			kothEvent.minPlayerCountForSpawn = 1;
		if (kothEvent.minPlayerCountToStartCapture < 0)
			kothEvent.minPlayerCountForSpawn = 1;
		if (kothEvent.captureTime <= 0)
			kothEvent.captureTime = RTConstants.RT_EVENT_CAPTURETIME_DEFAULT;
		if (kothEvent.hackTime < 0)
			kothEvent.hackTime = RTConstants.RT_TIMETOHACK_DEFAULT;
		if (kothEvent.minPlayerCountToStartCapture < 0)
			kothEvent.minPlayerCountToStartCapture = 1;
		if (kothEvent.minTimeBeforeSpawn <= 0)
			kothEvent.minTimeBeforeSpawn = -1;
		if (kothEvent.maxTimeBeforeSpawn <= 0)
			kothEvent.maxTimeBeforeSpawn = -1;
		kothEvent.prioritizeOldEvent 					= Math.Clamp(kothEvent.prioritizeOldEvent, 0, 1);
		kothEvent.spawnZombies 							= Math.Clamp(kothEvent.spawnZombies, 0, 1);
		kothEvent.enableSameEventSpawnInARow 			= Math.Clamp(kothEvent.enableSameEventSpawnInARow, 0, 1);
		kothEvent.enableConcurrentEvents 				= Math.Clamp(kothEvent.enableConcurrentEvents, 0, 1);
		kothEvent.enableCaptureStatusSmoke 				= Math.Clamp(kothEvent.enableCaptureStatusSmoke, 0, 1);
		kothEvent.depleteProgressWhenNoPlayersCapturing = Math.Clamp(kothEvent.depleteProgressWhenNoPlayersCapturing, 0, 1);
		kothEvent.pauseLifetimeDepleteDuringCapturing   = Math.Clamp(kothEvent.pauseLifetimeDepleteDuringCapturing, 0, 1);
		logging.enableLogging 							= Math.Clamp(logging.enableLogging, 0, 1);
		notifications.enableNotifications 				= Math.Clamp(notifications.enableNotifications, 0, 1);
		notifications.enableEventCreateNotification 	= Math.Clamp(notifications.enableEventCreateNotification, 0, 1);
		notifications.enableEventCaptureNotification 	= Math.Clamp(notifications.enableEventCaptureNotification, 0, 1);
		notifications.enableEventEndNotification 		= Math.Clamp(notifications.enableEventEndNotification, 0, 1);
		mapMarkers.enableLBMapMarker 					= Math.Clamp(mapMarkers.enableLBMapMarker, 0, 1);
		mapMarkers.enableVPPMapMarker 					= Math.Clamp(mapMarkers.enableVPPMapMarker, 0, 1);
		mapMarkers.enableBasicMapMarker 				= Math.Clamp(mapMarkers.enableBasicMapMarker, 0, 1);
		mapMarkers.enableExpansionMapMarker 			= Math.Clamp(mapMarkers.enableExpansionMapMarker, 0, 1);
		ui.showPlayerCount 								= Math.Clamp(ui.showPlayerCount, 0, 1);
	}
	
	static ref RTSettings Load()
	{
		ref RTSettings settings = new RTSettings();
		settings.Defaults();
		
		if (FileExist(RTConstants.RT_SETTINGS_CONFIGPATH))
		{
			JsonFileLoader<RTSettings>.JsonLoadFile(RTConstants.RT_SETTINGS_CONFIGPATH, settings);
			settings.CheckVersion();
			Print("[RadioTower] RTSettings.json loaded");
		}
		else
		{
			JsonFileLoader<RTSettings>.JsonSaveFile(RTConstants.RT_SETTINGS_CONFIGPATH, settings);
			Print("[RadioTower] RTSettings.json created & defaults loaded.");
		}
		settings.Validate();
		//JsonFileLoader<RTSettings>.JsonSaveFile(RTConstants.RT_SETTINGS_CONFIGPATH, settings);
		
		return settings;
	}
	
	void CheckVersion()
	{
		if (version != RT_VERSION)
		{
			string backupFileName = "v_" + version.ToString() + "_RTSettings.json";
			string backupFilePath = RTConstants.RT_BACKUPPATH_SETTINGS + backupFileName;
			
			if (!FileExist(RTConstants.RT_BACKUPPATH_SETTINGS))
			{
				MakeDirectory(RTConstants.RT_BACKUPPATH_SETTINGS);
				Print("[RadioTower] RTSettings backup folder created");
			}
			
			JsonFileLoader<RTSettings>.JsonSaveFile(backupFilePath, this);
			
			Print("[RadioTower] RTSetings.json update version " + version + " -> " + RT_VERSION);
			version = RT_VERSION;
			JsonFileLoader<RTSettings>.JsonSaveFile(RTConstants.RT_SETTINGS_CONFIGPATH, this);
		}
	}
}

class RTSettingsEvent
{
	string defaultLootcrate;
	int minPlayerCountForSpawn;
	int minPlayerCountToStartCapture;
	int lifeTime;
	int captureTime;
	int hackTime;
	int minTimeBeforeSpawn;
	int maxTimeBeforeSpawn;
	bool prioritizeOldEvent;
	bool spawnZombies;
	bool enableSameEventSpawnInARow;
	bool enableConcurrentEvents;
	bool enableCaptureStatusSmoke;
	bool depleteProgressWhenNoPlayersCapturing;
	bool pauseLifetimeDepleteDuringCapturing;
	
	void Defaults()
	{	
		defaultLootcrate = "RTLootcrate_White";
		minPlayerCountForSpawn = 1;
		minPlayerCountToStartCapture = 1;
		lifeTime = 1800;
		captureTime = 900;
		hackTime = 5;
		minTimeBeforeSpawn = 2700;
		maxTimeBeforeSpawn = 3600;
		prioritizeOldEvent = true;
		spawnZombies = true;
		enableSameEventSpawnInARow = false;
		enableConcurrentEvents = false;
		enableCaptureStatusSmoke = true;
		depleteProgressWhenNoPlayersCapturing = false;
		pauseLifetimeDepleteDuringCapturing = true;
	}
}

class RTSettingsLogging
{
	bool enableLogging;
	
	void Defaults()
	{
		enableLogging = false;
	}
}

class RTSettingsNotifications
{
	bool enableNotifications;
	bool enableEventCreateNotification;
	bool enableEventCaptureNotification;
	bool enableEventEndNotification;
	bool enablePlayerEnterCaptureAreaNotification;
	bool enablePlayerLeaveCaptureAreaNotification;
	
	void Defaults()
	{
		enableNotifications = true;
		enableEventCreateNotification = true;
		enableEventCaptureNotification = true;
		enableEventEndNotification = false;
		enablePlayerEnterCaptureAreaNotification = false;
		enablePlayerLeaveCaptureAreaNotification = false;
	}
}

class RTSettingsMapMarkers
{
	string mapMarkerText;
	bool enableLBMapMarker;
	bool enableVPPMapMarker;
	bool enableBasicMapMarker;
	bool enableExpansionMapMarker;
	
	void Defaults()
	{
		mapMarkerText = RTConstants.RT_MAP_MARKER_TEXT;
		enableLBMapMarker = false;
		enableVPPMapMarker = false;
		enableBasicMapMarker = false;
		enableExpansionMapMarker = false;
	}
}

class RTSettingsUI
{
	bool showPlayerCount;
	
	void Defaults()
	{
		showPlayerCount = true;
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~RTProps.json~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
class RTProps
{
	int version;
	ref array<ref RTLocationProps> eventProps;

	void Defaults()
	{
		version = RT_VERSION;
		eventProps = new array<ref RTLocationProps>();
	}
	
	static ref RTProps Load()
	{
		ref RTProps props = new RTProps();
		
		if (FileExist(RTConstants.RT_PROPS_CONFIGPATH))
		{
			JsonFileLoader<RTProps>.JsonLoadFile(RTConstants.RT_PROPS_CONFIGPATH, props);
			props.CheckVersion();
			Print("[RadioTower] RTProps.json loaded");
		}
		else
		{
			props.Defaults();
			JsonFileLoader<RTProps>.JsonSaveFile(RTConstants.RT_PROPS_CONFIGPATH, props);
			Print("[RadioTower] RTProps.json created & defaults loaded.");
		}
		//JsonFileLoader<RTProps>.JsonSaveFile(RTConstants.RT_PROPS_CONFIGPATH, props);
		
		return props;
	}
	
	void CheckVersion()
	{
		if (version != RT_VERSION)
		{
			string backupFileName = "v_" + version.ToString() + "_RTProps.json";
			string backupFilePath = RTConstants.RT_BACKUPPATH_PROPS + backupFileName;
			
			if (!FileExist(RTConstants.RT_BACKUPPATH_PROPS))
			{
				MakeDirectory(RTConstants.RT_BACKUPPATH_PROPS);
				Print("[RadioTower] RTProps backup folder created");
			}
			
			JsonFileLoader<RTProps>.JsonSaveFile(backupFilePath, this);
			
			Print("[RadioTower] RTProps.json update version " + version + " -> " + RT_VERSION);
			version = RT_VERSION;
			JsonFileLoader<RTProps>.JsonSaveFile(RTConstants.RT_PROPS_CONFIGPATH, this);
		}
	}
	
	RTLocationProps GetPropsByLocationId(string id)
	{
		for (int i = 0; i < eventProps.Count(); i++)
		{
			if (id == eventProps[i].locationId)
			{
				return eventProps[i];
			}
		}
		
		return null;
	}
}

class RTLocationProps
{
	string locationId;
	ref array<ref RTProp> locationProps;
}

class RTProp
{
	string propClassName;
	vector propCoordinatesXYZ;
    vector propOrientationYPR;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~RTLocations.json~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
class RTLocations
{
	int version;
	ref array<ref RTLocation> eventLocations;

	void Defaults()
	{
		version = RT_VERSION;
		eventLocations = new array<ref RTLocation>();
	
		ref RTLocation eventLocation = new RTLocation();
		eventLocation.loot = new RTLoot();
		int uuid[4];
		UUIDApi.Generate(uuid);
		eventLocation.id = UUIDApi.FormatString(uuid);
		eventLocation.captureAreaRadius = 25;
		eventLocation.captureAreaHeight = 25;
		eventLocation.captureAreaYAxisOffset = 0;
		eventLocation.zombieCount = 10;
		eventLocation.locationTitle = "Green mountain";
		eventLocation.lootcrateClassName = "RTLootcrate_Red";		
		eventLocation.locationCoordinatesXYZ = {3706.91, 402.01, 5982.06};
		eventLocation.locationOrientationYPR = {-109, 0, 0};
		eventLocation.lootcrateCoordinatesXYZ = {3703.805664, 401.999969, 5977.849121};
		eventLocation.lootcrateOrientationYPR = {2, 0, 0};
		eventLocation.vehicleCoordinatesXYZ = {3696.678467, 402.303986, 5992.726074};
		eventLocation.vehicleOrientationYPR = {0, 0, 0};
		eventLocation.vehicleProbability = 0.5;
		eventLocation.vehicleClassName = "OffroadHatchback";
		eventLocation.vehicleAttachments = {"HatchbackWheel", "HatchbackWheel", "HatchbackWheel", "HatchbackWheel", "HatchbackDoors_Driver"};
		eventLocation.zombies = {};
		eventLocation.createdNotificationTitle = "KOTH event created";
		eventLocation.capturedNotificationTitle = "KOTH event captured";
		eventLocation.endedNotificationTitle = "KOTH event ended";
		
		eventLocations.Insert(eventLocation);
	}
	
	void Validate()
	{
		foreach (RTLocation loc : eventLocations)
		{
			if (loc.id == "")
			{
				int uuid[4];
				UUIDApi.Generate(uuid);
				loc.id = UUIDApi.FormatString(uuid);
				//RTLogger.GetInstance().LogMessage(RTLogType.DEBUG, "Generated id: " + loc.id + " for " + loc.locationTitle);
				//Print("Generated id: " + loc.id + " for " + loc.locationTitle);
			}
		}
	}
	
	static ref RTLocations Load()
	{
		ref RTLocations locations = new RTLocations();
		//locations.Defaults();
		
		if (FileExist(RTConstants.RT_LOCATIONS_CONFIGPATH))
		{
			JsonFileLoader<RTLocations>.JsonLoadFile(RTConstants.RT_LOCATIONS_CONFIGPATH, locations);
			locations.CheckVersion();
			Print("[RadioTower] RTLocations.json loaded");
		}
		else
		{
			JsonFileLoader<RTLocations>.JsonLoadFile(RTConstants.RT_LOCATIONS_CONFIGPATH, locations);
			Print("[RadioTower] RTLocations.json created & defaults loaded.");
		}
		
		locations.Validate();
		//JsonFileLoader<RTLocations>.JsonSaveFile(RTConstants.RT_LOCATIONS_CONFIGPATH, locations);
		
		return locations;
	}
	
	void CheckVersion()
	{
		if (version != RT_VERSION)
		{
			string backupFileName = "v_" + version.ToString() + "_RTLocations.json";
			string backupFilePath = RTConstants.RT_BACKUPPATH_LOCATIONS + backupFileName;
			
			if (!FileExist(RTConstants.RT_BACKUPPATH_LOCATIONS))
			{
				MakeDirectory(RTConstants.RT_BACKUPPATH_LOCATIONS);
				Print("[RadioTower] RTLocations backup folder created");
			}
			
			JsonFileLoader<RTLocations>.JsonSaveFile(backupFilePath, this);
			
			Print("[RadioTower] RTLocations.json update version " + version + " -> " + RT_VERSION);
			version = RT_VERSION;
			JsonFileLoader<RTLocations>.JsonSaveFile(RTConstants.RT_LOCATIONS_CONFIGPATH, this);
		}
	}
}

class RTLocation
{
	string id;
	float captureAreaRadius;
	float captureAreaHeight;
	float captureAreaYAxisOffset;
	int zombieCount;
    string locationTitle;
	string lootcrateClassName;
    vector locationCoordinatesXYZ;
    vector locationOrientationYPR;
	vector lootcrateCoordinatesXYZ;
    vector lootcrateOrientationYPR;
	vector vehicleCoordinatesXYZ;
    vector vehicleOrientationYPR;
	float vehicleProbability;
	string vehicleClassName;
	string createdNotificationTitle;
	string capturedNotificationTitle;
	string endedNotificationTitle;
	ref TStringArray vehicleAttachments;
	ref RTLoot loot;
	ref TStringArray zombies;
	
	string GetRandomZombieClassname()
	{	
		if (zombies.Count() == 0)
			return "";
		
		return zombies[zombies.GetRandomIndex()];
	}
}

class RTLoot
{
	int lootCount;
	ref array<ref RTLootCategory> lootCategories;
	ref RTLootLootSet lootSets;
	
	float GetTotalCategoriesProbability()
	{
		float totalProbability = 0;
		
		if (lootCategories)
		{
			for (int i = 0; i < lootCategories.Count(); i++)
			{
				totalProbability += lootCategories[i].probability;
			}
		}
		
		return totalProbability;
	}
	
	// Useful when making sure that lootCount is higher than all the category limits combined because otherwise we have problems
	int GetTotalCategoriesLimit()
	{
		float totalLimit = 0;
		
		if (lootCategories)
		{
			for (int i = 0; i < lootCategories.Count(); i++)
			{
				int limit = lootCategories[i].limit;
				
				if (limit > -1)
					totalLimit += limit;
			}
		}
		
		return totalLimit;
	}
}

class RTLootCategory
{
	string lootCategoryTitle;
	float probability;
	int limit;
	ref array<ref RTLootItem> items;
	
	float GetTotalItemsProbability()
	{
		float totalProbability = 0;
				
		if (items)
		{
			for (int i = 0; i < items.Count(); i++)
			{
				totalProbability += items[i].probability;
			}
		}
		
		return totalProbability;
	}
}

class RTLootItem
{
	string lootItemClassName;
	float probability;
	int quantity;
	bool hasRandomQuantity;
	ref array<ref RTLootItemAttachmentCategory> attachmentCategories;
	
	float GetTotalAttachmentCategoriesProbability()
	{
		float totalProbability = 0;
		
		if (attachmentCategories)
		{
			for (int i = 0; i < attachmentCategories.Count(); i++)
			{
				totalProbability += attachmentCategories[i].probability;
			}
		}
		
		return totalProbability;
	}
}

class RTLootItemAttachmentCategory
{
	string attachmentCategoryTitle;
	float probability;
	ref array<ref RTLootItemAttachment> attachments;
	
	float GetTotalAttachmentsProbability()
	{
		float totalProbability = 0;
		
		if (attachments)
		{
			for (int i = 0; i < attachments.Count(); i++)
			{
				totalProbability += attachments[i].probability;
			}
		}
		
		return totalProbability;
	}
}

class RTLootItemAttachment
{
	string attachmentClassName;
	float probability;
}

class RTLootLootSet
{
	int setCount;
	ref array<ref RTLootSetMap> sets;
}

class RTLootSetMap
{
	string name;
	bool isGuaranteed;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~RTLootSets.json~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
class RTLootSets
{
	int version;
	ref array<ref RTLootSet> lootSets;

	void Defaults()
	{
		version = RT_VERSION;
		lootSets = new array<ref RTLootSet>();
	}
	
	static ref RTLootSets Load()
	{
		ref RTLootSets _lootSets = new RTLootSets();
		
		if (FileExist(RTConstants.RT_LOOTSETS_CONFIGPATH))
		{
			JsonFileLoader<RTLootSets>.JsonLoadFile(RTConstants.RT_LOOTSETS_CONFIGPATH, _lootSets);
			_lootSets.CheckVersion();
			Print("[RadioTower] RTLootSets.json loaded");
		}
		else
		{
			_lootSets.Defaults();
			JsonFileLoader<RTLootSets>.JsonSaveFile(RTConstants.RT_LOOTSETS_CONFIGPATH, _lootSets);
			Print("[RadioTower] RTLootSets.json created & defaults loaded.");
		}
		//JsonFileLoader<RTLootSets>.JsonSaveFile(RTConstants.RT_LOOTSETS_CONFIGPATH, _lootSets);
		
		return _lootSets;
	}
	
	void CheckVersion()
	{
		if (version != RT_VERSION)
		{
			string backupFileName = "v_" + version.ToString() + "_RTLootSets.json";
			string backupFilePath = RTConstants.RT_BACKUPPATH_LOOTSETS + backupFileName;
			
			if (!FileExist(RTConstants.RT_BACKUPPATH_LOOTSETS))
			{
				MakeDirectory(RTConstants.RT_BACKUPPATH_LOOTSETS);
				Print("[RadioTower] RTLootSets backup folder created");
			}
			
			JsonFileLoader<RTLootSets>.JsonSaveFile(backupFilePath, this);
			
			Print("[RadioTower] RTLootSets.json update version " + version + " -> " + RT_VERSION);
			version = RT_VERSION;
			JsonFileLoader<RTLootSets>.JsonSaveFile(RTConstants.RT_LOOTSETS_CONFIGPATH, this);
		}
	}
}

class RTLootSet
{
	string name;
	ref array<ref RTLootSetItem> items;
}

class RTLootSetItem
{
	string name;
	int quantity;
	ref array<ref RTLootSetItem> attachments;
	ref array<ref RTLootSetItem> cargo;
}