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
	
	
	void Defaults()
	{	
		eventSpawnInterval = 120;
		eventLifetime = 1800;
		eventCapturetime = 30;
		eventHacktime = 4;
		eventDefaultLootcrate = "RTLootcrate_Yellow";
		enableLogging = false;
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
	ref array<ref RTProp> eventProps;

	void Defaults()
	{
		eventProps = new array<ref RTProp>();
		ref RTProp eventProp = new RTProp();
		eventProps.Insert(eventProp);
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

class RTProp
{
	string location;
	string title;
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
		eventLoot.title = "M4A1";
		eventLoot.attachments = new TStringArray;
		eventLoot.attachments.Insert("M4_Suppressor");
		
		ref RTLocation eventLocation = new RTLocation();
		eventLocation.title = "Green mountain";
		eventLocation.lootcrate = "RTLootcrate_Red";		
		eventLocation.locationCoordinatesXYZ = {3706.91, 402.01, 5982.06};
		eventLocation.locationOrientationYPR = {-109, 0, 0};
		eventLocation.lootcrateCoordinatesXYZ = {3703.805664, 401.999969, 5977.849121};
		eventLocation.lootcrateOrientationYPR = {2, 0, 0};
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
	float captureRadius;
    string title;
	string lootcrate;
    vector locationCoordinatesXYZ;
    vector locationOrientationYPR;
	vector lootcrateCoordinatesXYZ;
    vector lootcrateOrientationYPR;
	vector vehicleCoordinatesXYZ;
    vector vehicleOrientationYPR;
	float vehicleProbability;
	string vehicleTitle;
	ref array<ref RTLoot> loot;
}

class RTLoot
{
    string title;
	int quantity;
	ref TStringArray attachments;
}