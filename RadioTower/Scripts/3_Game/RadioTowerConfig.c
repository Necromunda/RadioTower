class RadioTowerConfig
{
	protected static string m_Dir = "RadioTower";
	protected static string m_DirPath = "$profile:" + m_Dir;
	protected static string m_ConfigPath = m_DirPath + "\\RTServerLocations.json";
	
	protected ref RTServerLocations m_RTServerLocationArray;
	
	void RadioTowerConfig()
	{
		Print("[RadioTower] Init");
		if (GetGame().IsServer())
		{
			if (FileExist(m_ConfigPath))
			{
				Print("[RadioTower] Loading config");
			    Load();
			}
			else
			{
				Print("[RadioTower] Creating config");
				Create();
			}
		}
	};
	
	void Load()
	{
		JsonFileLoader<RTServerLocations>.JsonLoadFile(m_ConfigPath, m_RTServerLocationArray);
	}
	
	void Create()
	{	
		m_RTServerLocationArray = new RTServerLocations();
		m_RTServerLocationArray.locations = new ref array<RTServerLocation>();
		ref RTServerLocation serverLocation = new RTServerLocation();
		m_RTServerLocationArray.locations.Insert(serverLocation);
		
        MakeDirectory(m_DirPath);
		JsonFileLoader<RTServerLocations>.JsonSaveFile(m_ConfigPath, m_RTServerLocationArray);
	};
	
	RTServerLocations GetRTServerLocationArray()
	{
		return m_RTServerLocationArray;
	}
	
	float GetRTServerHackTime(string title)
	{
		if (m_RTServerLocationArray)
		{
			foreach (RTServerLocation o: m_RTServerLocationArray.locations)
			{
				if (o.title == title)
				{
					return o.timeToHack;
				}
			}
		}
		return RTConstants.RT_TimeToHack_DEFAULT;
	}
};

class RTServerLocations
{
    ref array<RTServerLocation> locations;
};

class RTServerLocation
{
    string title;
	float timeToHack;
    vector coordinates_xyz;
    vector orientation_rpy;
}