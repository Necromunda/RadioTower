class RadioTowerSettings
{	
	static ref RTServerConfig Load()
	{
		ref RTServerConfig m_Config = new RTServerConfig();
		
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
			JsonFileLoader<RTServerConfig>.JsonLoadFile(RTConstants.RT_CONFIGPATH, m_Config);
			Print("[RadioTower] Settings file loaded");
		}
		else
		{
			// Create default settings
			m_Config.Defaults();
			// Save it (with updated & reformatted values)
			JsonFileLoader<RTServerConfig>.JsonSaveFile(RTConstants.RT_CONFIGPATH, m_Config);
			// Log the folders & settings creation
			Print("[RadioTower] Settings created & defaults loaded.");
		}
		
		return m_Config;
	};
};

class RTServerConfig
{
    ref array<ref RTServerLocation> locations;
	
	void Defaults()
	{	
		locations = new array<ref RTServerLocation>();
		ref RTServerLocation serverLocation = new RTServerLocation();

		serverLocation.title = "Green mountain";		
		serverLocation.timeToHack = RTConstants.RT_TIMETOHACK_DEFAULT;
		serverLocation.coordinates_xyz = {3706.91, 402.01, 5982.06};
		serverLocation.orientation_ypr = {70, 0, 0};
		
		locations.Insert(serverLocation);
	};
};

class RTServerLocation
{
    string title;
	float timeToHack;
    vector coordinates_xyz;
    vector orientation_ypr;
}