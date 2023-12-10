modded class MissionServer
{	
	void MissionServer()
	{
		#ifdef GAMELABS
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
    
    	//GetRPCManager().SendRPC("RadioTower", "SendConfigToClient", new Param1<RTBase>(g_RTBase), true, identity); 
		GetRPCManager().SendRPC("RadioTower", "SendConfigToClient", new Param1<RTSettings>(g_RTBase.m_Settings), true, identity); 
  	}
};

modded class MissionGameplay
{
	private ref CaptureAreaUI m_CaptureAreaUI;
	
	void MissionGameplay()
	{
		if (GetGame().IsClient())
        	m_CaptureAreaUI = new CaptureAreaUI();
		
		GetRPCManager().AddRPC("RadioTower", "SendConfigToClient", this, SingleplayerExecutionType.Client);	
		//GetRPCManager().AddRPC("RadioTower", "UpdateInsiderCount", this, SingleplayerExecutionType.Client);
		//GetRPCManager().AddRPC("RadioTower", "UpdateCaptureProgress", this, SingleplayerExecutionType.Client);
		//GetRPCManager().AddRPC("RadioTower", "ClientEnteredCaptureZone", this, SingleplayerExecutionType.Client);
	}
	
	override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);
		
		PlayerBase player;
		if (PlayerBase.CastTo(player, GetGame().GetPlayer()) && player.GetIsInsideCaptureArea())
		{
			if (!m_CaptureAreaUI.IsCaptureUIVisible())
				m_CaptureAreaUI.ToggleCaptureUI();
			
			m_CaptureAreaUI.SetCaptureProgress(g_Game.GetCapturePct());
			m_CaptureAreaUI.SetInsiderCount(g_Game.GetInsiderCount().ToString());
		}	
		else
		{
			if (m_CaptureAreaUI.IsCaptureUIVisible())
				m_CaptureAreaUI.ToggleCaptureUI();
		}
    }
	
	void SendConfigToClient(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		//if this function is trigger anywhere but on client, we return without continuing.
		if(type != CallType.Client)
			return;

		Param1<RTSettings> data;
		//if the data is not retrieved we return to avoid issue
		if (!ctx.Read(data)) 
			return;

		RTSettings clientResponse = data.param1;
		g_Game.ClientSetRTSettings(clientResponse);

		Print(RTConstants.RT_LOG_PREFIX + " Client received settings " + clientResponse);
	}
	
	/*void ClientEnteredCaptureZone(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		//if this function is trigger anywhere but on client, we return without continuing.
		if(type != CallType.Client)
			return;

		Param2<int, float> data;
		//if the data is not retrieved we return to avoid issue
		if (!ctx.Read(data)) 
			return;
		
		if (m_CaptureAreaUI)
		{
			m_CaptureAreaUI.SetInsiderCount(data.param1.ToString());
			m_CaptureAreaUI.SetCaptureProgress(data.param2);
		}
	}
	
	void UpdateCaptureProgress(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target) 
	{
		//if this function is trigger anywhere but on client, we return without continuing.
		if(type != CallType.Client)
			return;

		Param1<float> data;
		//if the data is not retrieved we return to avoid issue
		if (!ctx.Read(data)) 
			return;
		
		if (m_CaptureAreaUI)
		{
			m_CaptureAreaUI.SetCaptureProgress(data.param1);
		}
	}
	
	void UpdateInsiderCount(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
		//if this function is trigger anywhere but on client, we return without continuing.
		if(type != CallType.Client)
			return;

		Param1<int> data;
		//if the data is not retrieved we return to avoid issue
		if (!ctx.Read(data)) 
			return;
		
		if (m_CaptureAreaUI)
		{
			m_CaptureAreaUI.SetInsiderCount(data.param1.ToString());
		}
	}*/
}