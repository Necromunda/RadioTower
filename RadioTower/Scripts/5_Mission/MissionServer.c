modded class MissionServer
{
	override void OnInit()
    {
        super.OnInit();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(PlaceServers, 20000);
		
		#ifdef GAMELABS
		GetGameLabs().AddMonitoredAction("ActionOpenRTServerConsole");
        GetGameLabs().AddMonitoredAction("ActionCloseRTServerConsole");
        GetGameLabs().AddMonitoredAction("ActionHackRTServerConsole");
		#endif 
    }
    
    void PlaceServers()
    {
		if(GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			// string path = "$profile:\\RadioTower\\RTServerLocations.json";
			ref RadioTowerConfig m_RadioTowerConfig = new RadioTowerConfig();
			
			GetDayZGame().ServerSet_RT_Server_Config(m_RadioTowerConfig);
			
			foreach (RTServerLocation o: m_RadioTowerConfig.GetRTServerLocationArray().locations)
			{
				vector position = o.coordinates_xyz;
				vector orientation = o.orientation_rpy;
				
				RT_Server a = RT_Server.Cast(GetGame().CreateObjectEx("RT_Server", position, ECE_KEEPHEIGHT));
				a.SetPosition(position);
				a.SetOrientation(orientation);
				
				a.SetFlags(EntityFlags.STATIC, false);
				a.Update();
			}
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).RemoveByName(this, "PlaceServers");
		}
    }
};
