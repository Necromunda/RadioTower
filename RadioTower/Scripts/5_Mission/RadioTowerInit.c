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
			Print(RTConstants.RT_LOG_PREFIX + " MissionServer init g_RTBase");
			g_RTBase = null;
			GetRTBase();
		}
	}
	
	void ~MissionServer()
	{
		delete g_RTBase;
	}
	
	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player,identity);
    
    	GetRPCManager().SendRPC("RadioTower", "SendConfigToClient", new Param1<RTBase>(g_RTBase), true, identity); 
  	}
};

modded class MissionGameplay
{
	/*
	private ref ExampleUI m_ExampleUI;
    
    override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);

        if ( GetGame().IsClient() )
        {
            // My conditions to enable the UI here !
            // For example on button pressed
            m_ExampleUI = new ExampleUI();
        }
    }
	*/
	
	void MissionGameplay()
	{
		if (GetGame().IsServer())
		{
			Print(RTConstants.RT_LOG_PREFIX + " MissionGameplay init g_RTBase");
			g_RTBase = null;
			GetRTBase();
		}
		GetRPCManager().AddRPC("RadioTower", "SendConfigToClient", this, SingleplayerExecutionType.Client);	
	}
	
	void SendConfigToClient(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		//if this function is trigger anywhere but on client, we return without continuing.
		if(type != CallType.Client)
			return;

		Param1<RTBase> data;
		//if the data is not retrieved we return to avoid issue
		if (!ctx.Read(data)) 
			return;

		RTBase clientResponse = data.param1;
		g_RTBase = clientResponse;

		Print(RTConstants.RT_LOG_PREFIX + " Client received config " + clientResponse);
	}
};