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
    ref array<ref RTEventLocation> eventLocations;
	float eventLifetime;
	float eventCapturetime;
	float eventTimeToHack;
	
	void Defaults()
	{	
		eventLifetime = 1800;
		eventCapturetime = 600;
		eventTimeToHack = 2;
		eventLocations = new array<ref RTEventLocation>();
		ref RTEventLocation serverLocation = new RTEventLocation();

		serverLocation.title = "Green mountain";		
		//serverLocation.timeToHack = RTConstants.RT_TIMETOHACK_DEFAULT;
		serverLocation.coordinates_xyz = {3706.91, 402.01, 5982.06};
		serverLocation.orientation_ypr = {70, 0, 0};
		
		eventLocations.Insert(serverLocation);
	};
};

class RTEventLocation
{
    string title;
    vector coordinates_xyz;
    vector orientation_ypr;
}