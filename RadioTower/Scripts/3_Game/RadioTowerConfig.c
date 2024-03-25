const int RT_VERSION_NEEDS_CONVERSION = 115022024;
const int RT_VERSION = 122032024;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~RTSettings.json~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Left because old settings need to be converted to the new format
class RTSettingsOld
{
	int version;
	int eventSpawnInterval;
	int eventLifetime;
	int eventCapturetime;
	int eventHacktime;
	string eventDefaultLootcrate;
	bool enableLogging;
	bool allowSameEventSpawnInARow;
	bool enableConcurrentEvents;
	bool enableNotifications;
	bool enableEventCreateNotification;
	bool enableEventCaptureNotification;
	bool enableEventEndNotification;
	bool prioritizeOldEvent;
	bool spawnZombies;
	bool showCaptureStatusSmoke;
	bool enableLBMapMarker;
	bool enableVPPMapMarker;
	int minimumPlayerCount;
	string mapMarkerText;
	bool useLootSets;
	bool showPlayerCount;
	int minPlayerCountToStartCapture;
	bool depleteProgressWhenNoPlayersCapturing;
	
	void Defaults()
	{	
		version = RT_VERSION;
		eventSpawnInterval = 2700;
		eventLifetime = 1800;
		eventCapturetime = 900;
		eventHacktime = 5;
		eventDefaultLootcrate = "RTLootcrate_White";
		enableLogging = false;
		allowSameEventSpawnInARow = false;
		enableConcurrentEvents = false;
		enableNotifications = true;
		enableEventCreateNotification = true;
		enableEventCaptureNotification = true;
		enableEventEndNotification = true;
		prioritizeOldEvent = true;
		spawnZombies = true;
		showCaptureStatusSmoke = true;
		enableLBMapMarker = false;
		enableVPPMapMarker = false;
		minimumPlayerCount = 1;
		mapMarkerText = RTConstants.RT_MAP_MARKER_TEXT;
		useLootSets = 0;
		showPlayerCount = true;
		minPlayerCountToStartCapture = 1;
		depleteProgressWhenNoPlayersCapturing = false;
	}
	
	static ref RTSettingsOld Load()
	{
		ref RTSettingsOld settings = new RTSettingsOld();
		
		if (FileExist(RTConstants.RT_SETTINGS_CONFIGPATH))
		{
			JsonFileLoader<RTSettingsOld>.JsonLoadFile(RTConstants.RT_SETTINGS_CONFIGPATH, settings);
		}
		else
		{
			settings = null;
		}
		
		return settings;
	}
	
	void ConvertSettings(RTSettings settings) 
	{
		settings.Defaults();
		settings.kothEvent.defaultLootcrate = eventDefaultLootcrate;
		settings.kothEvent.minPlayerCountForSpawn = minimumPlayerCount;
		settings.kothEvent.spawnInterval = eventSpawnInterval;
		settings.kothEvent.lifeTime = eventLifetime;
		settings.kothEvent.captureTime = eventCapturetime;
		settings.kothEvent.hackTime = eventHacktime;
		settings.kothEvent.prioritizeOldEvent = prioritizeOldEvent;
		settings.kothEvent.spawnZombies = spawnZombies;
		settings.kothEvent.useLootSets = useLootSets;
		settings.kothEvent.enableSameEventSpawnInARow = allowSameEventSpawnInARow;
		settings.kothEvent.enableConcurrentEvents = enableConcurrentEvents;
		settings.kothEvent.enableCaptureStatusSmoke = showCaptureStatusSmoke;
		settings.kothEvent.minPlayerCountToStartCapture = minPlayerCountToStartCapture;
		settings.kothEvent.depleteProgressWhenNoPlayersCapturing = depleteProgressWhenNoPlayersCapturing;
		settings.logging.enableLogging = enableLogging;
		settings.notifications.enableNotifications = enableNotifications;
		settings.notifications.enableEventCreateNotification = enableEventCreateNotification;
		settings.notifications.enableEventCaptureNotification = enableEventCaptureNotification;
		settings.notifications.enableEventEndNotification = enableEventEndNotification;
		settings.mapMarkers.mapMarkerText = mapMarkerText;
		settings.mapMarkers.enableLBMapMarker = enableLBMapMarker;
		settings.mapMarkers.enableVPPMapMarker = enableVPPMapMarker;
		settings.mapMarkers.enableVPPMapMarker = enableVPPMapMarker;
		settings.ui.showPlayerCount = true;
		settings.Validate();
		JsonFileLoader<RTSettings>.JsonSaveFile(RTConstants.RT_SETTINGS_CONFIGPATH, settings);
	}
}

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
		if (kothEvent.spawnInterval <= 0)
			kothEvent.spawnInterval = RTConstants.RT_EVENT_SPAWN_INTERVAL_DEFAULT;
		if (kothEvent.lifeTime <= 0)
			kothEvent.lifeTime = RTConstants.RT_EVENT_LIFETIME_DEFAULT;
		if (kothEvent.captureTime <= 0)
			kothEvent.captureTime = RTConstants.RT_EVENT_CAPTURETIME_DEFAULT;
		if (kothEvent.hackTime < 0)
			kothEvent.hackTime = RTConstants.RT_TIMETOHACK_DEFAULT;
		if (kothEvent.minPlayerCountToStartCapture < 0)
			kothEvent.minPlayerCountToStartCapture = 1;
		kothEvent.prioritizeOldEvent 					= Math.Clamp(kothEvent.prioritizeOldEvent, 0, 1);
		kothEvent.spawnZombies 							= Math.Clamp(kothEvent.spawnZombies, 0, 1);
		kothEvent.useLootSets 							= Math.Clamp(kothEvent.useLootSets, 0, 1);
		kothEvent.enableSameEventSpawnInARow 			= Math.Clamp(kothEvent.enableSameEventSpawnInARow, 0, 1);
		kothEvent.enableConcurrentEvents 				= Math.Clamp(kothEvent.enableConcurrentEvents, 0, 1);
		kothEvent.enableCaptureStatusSmoke 				= Math.Clamp(kothEvent.enableCaptureStatusSmoke, 0, 1);
		kothEvent.depleteProgressWhenNoPlayersCapturing = Math.Clamp(kothEvent.depleteProgressWhenNoPlayersCapturing, 0, 1);
		logging.enableLogging 							= Math.Clamp(logging.enableLogging, 0, 1);
		notifications.enableNotifications 				= Math.Clamp(notifications.enableNotifications, 0, 1);
		notifications.enableEventCreateNotification 	= Math.Clamp(notifications.enableEventCreateNotification, 0, 1);
		notifications.enableEventCaptureNotification 	= Math.Clamp(notifications.enableEventCaptureNotification, 0, 1);
		notifications.enableEventEndNotification 		= Math.Clamp(notifications.enableEventEndNotification, 0, 1);
		//if (!mapMarkers.mapMarkerText.Contains("%"))
			//mapMarkers.mapMarkerText = RTConstants.RT_MAP_MARKER_TEXT;
		mapMarkers.enableLBMapMarker 					= Math.Clamp(mapMarkers.enableLBMapMarker, 0, 1);
		mapMarkers.enableVPPMapMarker 					= Math.Clamp(mapMarkers.enableVPPMapMarker, 0, 1);
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
			Print("[RadioTower] RTSettings.json created & defaults loaded.");
		}
		settings.Validate();
		JsonFileLoader<RTSettings>.JsonSaveFile(RTConstants.RT_SETTINGS_CONFIGPATH, settings);
		
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
		}
	}
}

class RTSettingsEvent
{
	string defaultLootcrate;
	int minPlayerCountForSpawn;
	int minPlayerCountToStartCapture;
	int spawnInterval;
	int lifeTime;
	int captureTime;
	int hackTime;
	bool prioritizeOldEvent;
	bool spawnZombies;
	bool useLootSets;
	bool enableSameEventSpawnInARow;
	bool enableConcurrentEvents;
	bool enableCaptureStatusSmoke;
	bool depleteProgressWhenNoPlayersCapturing;
	
	void Defaults()
	{	
		defaultLootcrate = "RTLootcrate_White";
		minPlayerCountForSpawn = 1;
		minPlayerCountToStartCapture = 1;
		spawnInterval = 2700;
		lifeTime = 1800;
		captureTime = 900;
		hackTime = 5;
		prioritizeOldEvent = true;
		spawnZombies = true;
		useLootSets = false;
		enableSameEventSpawnInARow = false;
		enableConcurrentEvents = false;
		enableCaptureStatusSmoke = true;
		depleteProgressWhenNoPlayersCapturing = false;
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
	
	void Defaults()
	{
		enableNotifications = true;
		enableEventCreateNotification = true;
		enableEventCaptureNotification = true;
		enableEventEndNotification = false;
	}
}

class RTSettingsMapMarkers
{
	string mapMarkerText;
	bool enableLBMapMarker;
	bool enableVPPMapMarker;
	
	void Defaults()
	{
		mapMarkerText = RTConstants.RT_MAP_MARKER_TEXT;
		enableLBMapMarker = false;
		enableVPPMapMarker = false;
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

/*
class RTSettings
{
	int version;
	int eventSpawnInterval;
	int eventLifetime;
	int eventCapturetime;
	int eventHacktime;
	string eventDefaultLootcrate;
	bool enableLogging;
	bool allowSameEventSpawnInARow;
	bool enableConcurrentEvents;
	bool enableNotifications;
	bool enableEventCreateNotification;
	bool enableEventCaptureNotification;
	bool enableEventEndNotification;
	bool prioritizeOldEvent;
	bool spawnZombies;
	bool showCaptureStatusSmoke;
	bool enableLBMapMarker;
	int minimumPlayerCount;
	string mapMarkerText;
	bool useLootSets;
	
	void Defaults()
	{	
		version = RT_VERSION;
		eventSpawnInterval = 2700;
		eventLifetime = 1800;
		eventCapturetime = 900;
		eventHacktime = 5;
		eventDefaultLootcrate = "RTLootcrate_White";
		enableLogging = false;
		allowSameEventSpawnInARow = false;
		enableConcurrentEvents = false;
		enableNotifications = true;
		enableEventCreateNotification = true;
		enableEventCaptureNotification = true;
		enableEventEndNotification = false;
		prioritizeOldEvent = true;
		spawnZombies = true;
		showCaptureStatusSmoke = true;
		enableLBMapMarker = false;
		minimumPlayerCount = 0;
		mapMarkerText = RTConstants.RT_MAP_MARKER_TEXT;
		useLootSets = 0;
	}
	
	static ref RTSettings Load()
	{
		ref RTSettings m_Settings = new RTSettings();
		
		// Check if RadioTower/RadioTowerSettings.json file exists in Profiles
		if (FileExist(RTConstants.RT_SETTINGS_CONFIGPATH))
		{
			// Load it
			JsonFileLoader<RTSettings>.JsonLoadFile(RTConstants.RT_SETTINGS_CONFIGPATH, m_Settings);
			// Compare versions and update settings
			RTSettings.CheckVersion(m_Settings);
			Print("[RadioTower] RTSettings.json loaded");
		}
		else
		{
			// Create default settings
			m_Settings.Defaults();
			// Log the folders & settings creation
			Print("[RadioTower] RTSettings.json created & defaults loaded.");
		}
		JsonFileLoader<RTSettings>.JsonSaveFile(RTConstants.RT_SETTINGS_CONFIGPATH, m_Settings);
		
		return m_Settings;
	}
	
	static protected void CheckVersion(RTSettings settings)
	{
		if (settings.version != RT_VERSION)
		{
			string backupFileName = "v_" + settings.version.ToString() + "_RTSettings.json";
			string backupFilePath = RTConstants.RT_BACKUPPATH_SETTINGS + backupFileName;
			
			if (!FileExist(RTConstants.RT_BACKUPPATH_SETTINGS))
			{
				MakeDirectory(RTConstants.RT_BACKUPPATH_SETTINGS);
				Print("[RadioTower] RTSettings backup folder created");
			}
			
			JsonFileLoader<RTSettings>.JsonSaveFile(backupFilePath, settings);
			
			Print("[RadioTower] RTSetings.json update version " + settings.version + " -> " + RT_VERSION);
			settings.version = RT_VERSION;
		}
	}
}
*/

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
		//props.Defaults();
		
		if (FileExist(RTConstants.RT_PROPS_CONFIGPATH))
		{
			JsonFileLoader<RTProps>.JsonLoadFile(RTConstants.RT_PROPS_CONFIGPATH, props);
			props.CheckVersion();
			Print("[RadioTower] RTProps.json loaded");
		}
		else
		{
			props.Defaults();
			Print("[RadioTower] RTProps.json created & defaults loaded.");
		}
		JsonFileLoader<RTProps>.JsonSaveFile(RTConstants.RT_PROPS_CONFIGPATH, props);
		
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
		}
	}
}

class RTLocationProps
{
	string locationTitle;
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
		
		eventLocations.Insert(eventLocation);
	}
	
	static ref RTLocations Load()
	{
		ref RTLocations locations = new RTLocations();
		
		if (FileExist(RTConstants.RT_LOCATIONS_CONFIGPATH))
		{
			JsonFileLoader<RTLocations>.JsonLoadFile(RTConstants.RT_LOCATIONS_CONFIGPATH, locations);
			locations.CheckVersion();
			Print("[RadioTower] RTLocations.json loaded");
		}
		else
		{
			locations.Defaults();
			Print("[RadioTower] RTLocations.json created & defaults loaded.");
		}
		JsonFileLoader<RTLocations>.JsonSaveFile(RTConstants.RT_LOCATIONS_CONFIGPATH, locations);
		
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
		}
	}
}

class RTLocation
{
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
	ref TStringArray vehicleAttachments;
	ref RTLoot loot;
	ref TStringArray lootSets;
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
				totalLimit += lootCategories[i].limit;
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
			Print("[RadioTower] RTLootSets.json created & defaults loaded.");
		}
		JsonFileLoader<RTLootSets>.JsonSaveFile(RTConstants.RT_LOOTSETS_CONFIGPATH, _lootSets);
		
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