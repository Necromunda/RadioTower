modded class MissionServer
{	
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
	
	void ~MissionServer()
	{
		delete g_RadioTowerBase;
	}
	
	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player,identity);
    
    	GetRPCManager().SendRPC("RadioTower", "FncInvokeDataResponse", new Param1<RadioTowerBase>(g_RadioTowerBase), true, identity); 
    	// GetRPCManager().SendRPC("RadioTower", "FncInvokeDataResponse", new Param1<string>("test"), true, identity); 
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
		GetRPCManager().AddRPC("RadioTower", "FncInvokeDataResponse", this, SingleplayerExecutionType.Client);	
	}
	
	void FncInvokeDataResponse(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		//if this function is trigger anywhere but on client, we return without continuing.
		if(type != CallType.Client)
			return;

		Param1<RadioTowerBase> data;
		// Param1<string> data;
		// Print("[RadioTower] 1 Data: " + data);
		//if the data is not retrieved we return to avoid issue
		if (!ctx.Read(data)) 
			return;

		RadioTowerBase clientResponse = data.param1;
		g_RadioTowerBase = clientResponse;
		// string clientResponse = data.param1;

		Print("[RadioTower] 2 Data: " + clientResponse.m_Config.locations[0].title);
		// Print(clientResponse);
	}
};