class RTSettings
{	
	static ref RTEventConfig Load()
	{
		ref RTEventConfig m_Config = new RTEventConfig();
		
		// Check if RadioTower folder exists in Profiles
		if (!FileExist(RTConstants.RT_DIRPATH))
		{
			MakeDirectory(RTConstants.RT_DIRPATH);
			Print("[RadioTower] Settings folder created");
		}
		
		// Check if RadioTower/Logs folder exists in Profiles
		if (!FileExist(RTConstants.RT_LOGPATH))
		{
			MakeDirectory(RTConstants.RT_LOGPATH);
			Print("[RadioTower] Logs folder created");
		}
		
		// Check if RadioTower/RadioTowerSettings.json file exists in Profiles
		if (FileExist(RTConstants.RT_CONFIGPATH))
		{
			// Load it
			JsonFileLoader<RTEventConfig>.JsonLoadFile(RTConstants.RT_CONFIGPATH, m_Config);
			Print("[RadioTower] Settings file loaded");
		}
		else
		{
			// Create default settings
			m_Config.Defaults();
			// Save it (with updated & reformatted values)
			JsonFileLoader<RTEventConfig>.JsonSaveFile(RTConstants.RT_CONFIGPATH, m_Config);
			// Log the folders & settings creation
			Print("[RadioTower] Settings created & defaults loaded.");
		}
		
		return m_Config;
	};
};

class RTEventConfig
{
	float eventSpawnInterval;
	float eventLifetime;
	float eventCapturetime;
	float eventHacktime;
	string eventDefaultLootcrate;
	ref array<ref RTEventLocation> eventLocations;
	
	void Defaults()
	{	
		eventSpawnInterval = 2700;
		eventLifetime = 1800;
		eventCapturetime = 600;
		eventHacktime = 2;
		eventDefaultLootcrate = "RTLootcrate_Yellow";
		eventLocations = new array<ref RTEventLocation>();
		
		ref RTEventLoot eventLoot = new RTEventLoot();
		eventLoot.title = "M4A1";
		eventLoot.attachments.Insert("M4_Suppressor");
		
		ref RTEventLocation serverLocation = new RTEventLocation();

		serverLocation.title = "Green mountain";
		serverLocation.eventLootcrate = "RTLootcrate_Red";		
		serverLocation.coordinates_xyz = {3706.91, 402.01, 5982.06};
		serverLocation.orientation_ypr = {70, 0, 0};
		serverLocation.lootcrate_coordinates_xyz = {3703.805664, 401.999969, 5977.849121};
		serverLocation.lootcrate_orientation_ypr = {-123.298050, 0, 0};
		serverLocation.loot.Insert(eventLoot);
		
		eventLocations.Insert(serverLocation);
	};
};

class RTEventLocation
{
    string title;
	string eventLootcrate;
    vector coordinates_xyz;
    vector orientation_ypr;
	vector lootcrate_coordinates_xyz;
    vector lootcrate_orientation_ypr;
	vector vehicle_coordinates_xyz;
    vector vehicle_orientation_ypr;
	float vehicleProbability;
	ref array<ref RTEventLoot> loot;
}

class RTEventLoot
{
    string title;
	int quantity;
	TStringArray attachments;
}