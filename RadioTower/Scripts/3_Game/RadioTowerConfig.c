const int RT_VERSION = 115022024;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~RTSettings.json~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
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
		ref RTProps m_Props = new RTProps();
		
		// Check if RadioTower/RadioTowerSettings.json file exists in Profiles
		if (FileExist(RTConstants.RT_PROPS_CONFIGPATH))
		{
			// Load it
			JsonFileLoader<RTProps>.JsonLoadFile(RTConstants.RT_PROPS_CONFIGPATH, m_Props);
			// Compare versions and update props
			RTProps.CheckVersion(m_Props);
			Print("[RadioTower] RTProps.json loaded");
		}
		else
		{
			// Create default settings
			m_Props.Defaults();
			// Log the folders & settings creation
			Print("[RadioTower] RTProps.json created & defaults loaded.");
		}
		JsonFileLoader<RTProps>.JsonSaveFile(RTConstants.RT_PROPS_CONFIGPATH, m_Props);
		
		return m_Props;
	}
	
	static protected void CheckVersion(RTProps props)
	{
		if (props.version != RT_VERSION)
		{
			string backupFileName = "v_" + props.version.ToString() + "_RTProps.json";
			string backupFilePath = RTConstants.RT_BACKUPPATH_PROPS + backupFileName;
			
			if (!FileExist(RTConstants.RT_BACKUPPATH_PROPS))
			{
				MakeDirectory(RTConstants.RT_BACKUPPATH_PROPS);
				Print("[RadioTower] RTProps backup folder created");
			}
			
			JsonFileLoader<RTProps>.JsonSaveFile(backupFilePath, props);
			
			Print("[RadioTower] RTProps.json update version " + props.version + " -> " + RT_VERSION);
			props.version = RT_VERSION;
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
		ref RTLocations m_Locations = new RTLocations();
		
		// Check if RadioTower/RadioTowerSettings.json file exists in Profiles
		if (FileExist(RTConstants.RT_LOCATIONS_CONFIGPATH))
		{
			// Load it
			JsonFileLoader<RTLocations>.JsonLoadFile(RTConstants.RT_LOCATIONS_CONFIGPATH, m_Locations);
			RTLocations.CheckVersion(m_Locations);
			Print("[RadioTower] RTLocations.json loaded");
		}
		else
		{
			// Create default settings
			m_Locations.Defaults();
			// Log the folders & settings creation
			Print("[RadioTower] RTLocations.json created & defaults loaded.");
		}
		JsonFileLoader<RTLocations>.JsonSaveFile(RTConstants.RT_LOCATIONS_CONFIGPATH, m_Locations);
		
		return m_Locations;
	}
	
	static protected void CheckVersion(RTLocations locations)
	{
		if (locations.version != RT_VERSION)
		{
			string backupFileName = "v_" + locations.version.ToString() + "_RTLocations.json";
			string backupFilePath = RTConstants.RT_BACKUPPATH_LOCATIONS + backupFileName;
			
			if (!FileExist(RTConstants.RT_BACKUPPATH_LOCATIONS))
			{
				MakeDirectory(RTConstants.RT_BACKUPPATH_LOCATIONS);
				Print("[RadioTower] RTLocations backup folder created");
			}
			
			JsonFileLoader<RTLocations>.JsonSaveFile(backupFilePath, locations);
			
			Print("[RadioTower] RTLocations.json update version " + locations.version + " -> " + RT_VERSION);
			locations.version = RT_VERSION;
		}
	}
}

class RTLocation
{
	float captureAreaRadius;
	float captureAreaHeight;
	float captureAreaYAxisOffset;
	//bool spawnGas;
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