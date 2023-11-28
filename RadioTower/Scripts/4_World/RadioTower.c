class RadioTowerBase
{
	ref RTServerConfig m_Config;
	
	void ~RadioTowerBase()
	{
	}
	
	void RadioTowerBase()
	{
		m_Config = RadioTowerSettings.Load();
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(PlaceServer, 20000);		
	}
    
    void PlaceServer()
    {
		foreach (RTServerLocation o: m_Config.locations)
		{
			vector position = o.coordinates_xyz;
			vector orientation = o.orientation_ypr;
			
			RT_Server s = RT_Server.Cast(GetGame().CreateObjectEx("RT_Server", position, ECE_KEEPHEIGHT));
			s.SetPosition(position);
			s.SetOrientation(orientation);
			
			s.SetFlags(EntityFlags.STATIC, false);
			s.Update();
		}
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).RemoveByName(this, "PlaceServer");
    }
}

static ref RadioTowerBase g_RadioTowerBase;
static ref RadioTowerBase GetRadioTowerBase()
{
	if (!g_RadioTowerBase)
	{
		g_RadioTowerBase = new ref RadioTowerBase();
	}
	return g_RadioTowerBase;
}