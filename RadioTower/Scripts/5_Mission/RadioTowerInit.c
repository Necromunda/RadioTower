modded class MissionServer
{
	void ~MissionServer()
	{
		delete g_RadioTowerBase;
	}
	
	void MissionServer()
	{
		#ifdef GAMELABS
		// GetGameLabs().AddMonitoredAction("ActionOpenRTServerConsole");
        // GetGameLabs().AddMonitoredAction("ActionCloseRTServerConsole");
        GetGameLabs().AddMonitoredAction("ActionHackRTServerConsole");
		#endif
		
		if (GetGame().IsServer())
		{
			g_RadioTowerBase = null;
			GetRadioTowerBase();
		}
	}
	
	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player,identity);
    
    	GetRPCManager().SendRPC("RadioTower", "FncInvokeDataResponse", new Param1<string>("config"), true, identity); 
  	}
};

modded class MissionGameplay
{
	void MissionGameplay()
	{
		if (GetGame().IsServer())
		{
			g_RadioTowerBase = null;
			GetRadioTowerBase();
		}
	}
};