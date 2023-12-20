//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\\
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~RTSettings.json~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\\
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\\
class RTSettings
{
	float eventSpawnInterval;
	float eventLifetime;
	float eventCapturetime;
	float eventHacktime;
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
	
	void Defaults()
	{	
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
	}
	
	static ref RTSettings Load()
	{
		ref RTSettings m_Settings = new RTSettings();
		
		// Check if RadioTower folder exists in Profiles
		if (!FileExist(RTConstants.RT_ROOTPATH))
		{
			MakeDirectory(RTConstants.RT_ROOTPATH);
			Print("[RadioTower] Root folder created");
		}
		
		// Check if RadioTower/Logs folder exists in Profiles
		if (!FileExist(RTConstants.RT_LOGPATH))
		{
			MakeDirectory(RTConstants.RT_LOGPATH);
			Print("[RadioTower] Logs folder created");
		}
		
		// Check if RadioTower/RadioTowerSettings.json file exists in Profiles
		if (FileExist(RTConstants.RT_SETTINGS_CONFIGPATH))
		{
			// Load it
			JsonFileLoader<RTSettings>.JsonLoadFile(RTConstants.RT_SETTINGS_CONFIGPATH, m_Settings);
			Print("[RadioTower] RTSettings.json loaded");
		}
		else
		{
			// Create default settings
			m_Settings.Defaults();
			// Save it (with updated & reformatted values)
			JsonFileLoader<RTSettings>.JsonSaveFile(RTConstants.RT_SETTINGS_CONFIGPATH, m_Settings);
			// Log the folders & settings creation
			Print("[RadioTower] RTSettings.json created & defaults loaded.");
		}
		
		return m_Settings;
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\\
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~RTProps.json~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\\
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\\
class RTProps
{
	ref array<ref RTLocationProps> eventProps;

	void Defaults()
	{
		eventProps = new array<ref RTLocationProps>();
		ref RTLocationProps location = new RTLocationProps();
		location.locationProps = new array<ref RTProp>();
		ref RTProp eventProp = new RTProp();
		location.locationProps.Insert(eventProp);
		eventProps.Insert(location);
	}
	
	static ref RTProps Load()
	{
		ref RTProps m_Props = new RTProps();
		
		// Check if RadioTower folder exists in Profiles
		if (!FileExist(RTConstants.RT_ROOTPATH))
		{
			MakeDirectory(RTConstants.RT_ROOTPATH);
			Print("[RadioTower] Root folder created");
		}
		
		// Check if RadioTower/Logs folder exists in Profiles
		if (!FileExist(RTConstants.RT_LOGPATH))
		{
			MakeDirectory(RTConstants.RT_LOGPATH);
			Print("[RadioTower] Logs folder created");
		}
		
		// Check if RadioTower/RadioTowerSettings.json file exists in Profiles
		if (FileExist(RTConstants.RT_PROPS_CONFIGPATH))
		{
			// Load it
			JsonFileLoader<RTProps>.JsonLoadFile(RTConstants.RT_PROPS_CONFIGPATH, m_Props);
			Print("[RadioTower] RTProps.json loaded");
		}
		else
		{
			// Create default settings
			m_Props.Defaults();
			// Save it (with updated & reformatted values)
			JsonFileLoader<RTProps>.JsonSaveFile(RTConstants.RT_PROPS_CONFIGPATH, m_Props);
			// Log the folders & settings creation
			Print("[RadioTower] RTProps.json created & defaults loaded.");
		}
		
		return m_Props;
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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\\
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~RTLocations.json~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\\
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\\
class RTLocations
{
	ref array<ref RTLocation> eventLocations;

	void Defaults()
	{
		eventLocations = new array<ref RTLocation>();
		
		ref RTLoot eventLoot = new RTLoot();
		eventLoot.lootClassName = "M4A1";
		eventLoot.attachments = new TStringArray;
		eventLoot.attachments.Insert("M4_Suppressor");
		
		ref RTLocation eventLocation = new RTLocation();
		eventLocation.captureAreaRadius = 25;
		eventLocation.captureAreaHeight = 25;
		eventLocation.captureAreaYAxisOffset = 0;
		eventLocation.spawnGas = false;
		eventLocation.zombieCount = 10;
		eventLocation.locationTitle = "Green mountain";
		eventLocation.lootcrateClassName = "RTLootcrate_Red";		
		eventLocation.locationCoordinatesXYZ = {3706.91, 402.01, 5982.06};
		eventLocation.locationOrientationYPR = {-109, 0, 0};
		eventLocation.lootcrateCoordinatesXYZ = {3703.805664, 401.999969, 5977.849121};
		eventLocation.lootcrateOrientationYPR = {2, 0, 0};
		eventLocation.vehicleCoordinatesXYZ = {3696.678467, 402.303986, 5992.726074};
		eventLocation.vehicleOrientationYPR = {0, 0, 0};
		eventLocation.vehicleProbability = 1;
		eventLocation.vehicleClassName = "OffroadHatchback";
		eventLocation.vehicleAttachments = {"HatchbackWheel", "HatchbackWheel", "HatchbackWheel", "HatchbackWheel", "HatchbackDoors_Driver"};
		eventLocation.loot = new array<ref RTLoot>();
		eventLocation.loot.Insert(eventLoot);
		
		eventLocations.Insert(eventLocation);
	}
	
	static ref RTLocations Load()
	{
		ref RTLocations m_Locations = new RTLocations();
		
		// Check if RadioTower folder exists in Profiles
		if (!FileExist(RTConstants.RT_ROOTPATH))
		{
			MakeDirectory(RTConstants.RT_ROOTPATH);
			Print("[RadioTower] Root folder created");
		}
		
		// Check if RadioTower/Logs folder exists in Profiles
		if (!FileExist(RTConstants.RT_LOGPATH))
		{
			MakeDirectory(RTConstants.RT_LOGPATH);
			Print("[RadioTower] Logs folder created");
		}
		
		// Check if RadioTower/RadioTowerSettings.json file exists in Profiles
		if (FileExist(RTConstants.RT_LOCATIONS_CONFIGPATH))
		{
			// Load it
			JsonFileLoader<RTLocations>.JsonLoadFile(RTConstants.RT_LOCATIONS_CONFIGPATH, m_Locations);
			Print("[RadioTower] RTLocations.json loaded");
		}
		else
		{
			// Create default settings
			m_Locations.Defaults();
			// Save it (with updated & reformatted values)
			JsonFileLoader<RTLocations>.JsonSaveFile(RTConstants.RT_LOCATIONS_CONFIGPATH, m_Locations);
			// Log the folders & settings creation
			Print("[RadioTower] RTLocations.json created & defaults loaded.");
		}
		
		return m_Locations;
	}
}

class RTLocation
{
	float captureAreaRadius;
	float captureAreaHeight;
	float captureAreaYAxisOffset;
	bool spawnGas;
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
	ref array<ref RTLoot> loot;
}

class RTLoot
{
    string lootClassName;
	int quantity;
	ref TStringArray attachments;
}