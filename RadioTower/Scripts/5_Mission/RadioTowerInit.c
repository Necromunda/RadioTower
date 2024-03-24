modded class MissionServer
{	
	void MissionServer()
	{
		#ifdef GAMELABS
        GetGameLabs().AddMonitoredAction("ActionHackRTServerConsole");
		#endif
		
		if (GetGame().IsServer())
		{
			g_RTBase = null;
			GetRTBase();
		}	
	}
	
	void ~MissionServer()
	{
		if (g_RTBase)
			delete g_RTBase;
	}
	
	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player,identity);
    
		GetRPCManager().SendRPC("RadioTower", "SendConfigToClient", new Param1<RTSettings>(g_RTBase.m_Settings), true, identity); 
		if (g_RTBase.GetRTEvent())
			GetRPCManager().SendRPC("RadioTower", "ClientSetLatestEventLocation", new Param1<RTLocation>(g_RTBase.GetRTEvent().GetEventLocation()), true, null);
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
		GetRPCManager().AddRPC("RadioTower", "ClientSetLatestEventLocation", this, SingleplayerExecutionType.Client);
		GetRPCManager().AddRPC("RadioTower", "ClientDeleteLatestEventLocation", this, SingleplayerExecutionType.Client);
	}
	
	void ~MissionGameplay()
	{
		if (g_RTBase)
			delete g_RTBase;
		
		if (m_CaptureAreaUI)
			delete m_CaptureAreaUI;
	}
	
	override void OnUpdate(float timeslice)
    {
        super.OnUpdate(timeslice);
		
		PlayerBase player;
		if (PlayerBase.CastTo(player, GetGame().GetPlayer()) && player.GetIsInsideCaptureArea())
		{
			#ifdef LBmaster_Groups
			if (LBMarkerVisibilityManager.Get().compassEnabled)
				m_CaptureAreaUI.SetYOffset(65);
			else
				m_CaptureAreaUI.SetYOffset(0);
			#endif
			
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

		Print("[RadioTower] Client received settings " + clientResponse);
	}
	
	void ClientSetLatestEventLocation( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
	{
		Param1<ref RTLocation> location;
		if ( !ctx.Read( location ) ) return;

		g_Game.SetLatestEventLocation(location.param1);
	}
	
	void ClientDeleteLatestEventLocation( CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target )
	{
		g_Game.DeleteLatestEventLocation();
	}
}