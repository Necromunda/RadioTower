/** \name OnEnter
    Called when an object enters the trigger
*/
// protected void OnEnterBeginEvent(TriggerInsider insider) {}
// protected void OnEnterServerEvent(TriggerInsider insider) {}
// protected void OnEnterClientEvent(TriggerInsider insider) {}
// protected void OnEnterEndEvent(TriggerInsider insider) {}

/** \name OnStayStart
    Called at the beginning of an update loop
*/
// protected void OnStayStartBeginEvent(int nrOfInsiders) {}
// protected void OnStayStartServerEvent(int nrOfInsiders) {}
// protected void OnStayStartClientEvent(int nrOfInsiders) {}
// protected void OnStayStartEndEvent(int nrOfInsiders) {}

/** \name OnStay
    Called in the update loop
*/
// protected void OnStayBeginEvent(TriggerInsider insider, float deltaTime) {}
// protected void OnStayServerEvent(TriggerInsider insider, float deltaTime) {}
// protected void OnStayClientEvent(TriggerInsider insider, float deltaTime) {}
// protected void OnStayEndEvent(TriggerInsider insider, float deltaTime) {}

/** \name OnStayFinish
    Called at the end of an update loop
*/
// protected void OnStayFinishBeginEvent() {}
// protected void OnStayFinishServerEvent() {}
// protected void OnStayFinishClientEvent() {}
// protected void OnStayFinishEndEvent() {}

/** \name OnLeave
    Called when an object leaves the trigger
*/
// protected void OnLeaveBeginEvent(TriggerInsider insider) {}
// protected void OnLeaveServerEvent(TriggerInsider insider) {}
// protected void OnLeaveClientEvent(TriggerInsider insider) {}
// protected void OnLeaveEndEvent(TriggerInsider insider) {}

class CaptureArea: Trigger
{
	protected float m_CollisionCylinderRadius;
	protected float m_CollisionCylinderHeight;
	
	protected float m_Event_Capturetime;
	protected float m_TimeAccuStay;
	protected float m_CapturePct;
	protected float m_CaptureSlice;
	protected float m_UpdateInterval;
	protected float m_TotalCapturePct;
	
	protected const float LIFETIME_TICKRATE = 1; // seconds
	protected ref Timer m_Timer1;
	protected float m_Event_Lifetime;
	
	protected int m_InsiderCount;
	
	void CaptureArea()
	{
		Print("[RadioTower] Capture area ctor");
		m_CollisionCylinderRadius = RTConstants.RT_EVENT_TRIGGER_RADIUS_DEFAULT;
		m_CollisionCylinderHeight = RTConstants.RT_EVENT_TRIGGER_HEIGHT_DEFAULT;
		
		m_Event_Capturetime = RTConstants.RT_EVENT_CAPTURETIME_DEFAULT;
		m_TimeAccuStay = 0;
		m_CapturePct = 0;
		m_UpdateInterval = RTConstants.RT_EVENT_UPDATE_PROGRESS_INTERVAL_DEFAULT;
		m_TotalCapturePct = RTConstants.RT_EVENT_TOTAL_CAPTURE_PCT_DEFAULT;
		
		m_Timer1 = new Timer;
		m_Event_Lifetime = RTConstants.RT_EVENT_LIFETIME_DEFAULT;
		
		if (g_RTBase)
		{
			m_Event_Capturetime = g_RTBase.m_Config.eventCapturetime;
			m_Event_Lifetime = g_RTBase.m_Config.eventLifetime;
		}
		m_CaptureSlice = (m_TotalCapturePct / m_Event_Capturetime) * m_UpdateInterval;
		
		m_Timer1.Run(LIFETIME_TICKRATE, this, "Tick", NULL, true);
	}
	
	override void EOnInit(IEntity other, int extra)
	{
		SetCollisionCylinder(m_CollisionCylinderRadius, m_CollisionCylinderHeight);
	}
	
	void Tick()
	{
		string date = CF_Date.Now().Format("DD-MM-YYYY hh:mm:ss");
		if (m_Event_Lifetime.ToString().ToInt() % 60 == 0)
			Print(date + " CaptureArea lifetime " + m_Event_Lifetime);
		m_Event_Lifetime -= LIFETIME_TICKRATE;
		if (m_Event_Lifetime < 0)
		{
			OnEventFinish();
		}
	}
	
	void OnEventCapture()
	{
		m_Event_Lifetime = 0;
		g_RTBase.SpawnEventLootCrate();
		string title = g_RTBase.GetRTEvent().GetEventTitle();
		string msg = title + " has been captured!";
		RTMsgHandler.RTSendClientAlert(RTConstants.RT_ICON, msg, 3);
	}
	
	void OnEventFinish()
	{
		DeleteSafe();
		Print("[RadioTower] Capture area deleted");
		g_RTBase.SetIsClientInCaptureZone(false);
	}
	
	override void OnEnterClientEvent(TriggerInsider insider)
	{
		super.OnEnterClientEvent(insider);
		
		PlayerBase player;
		if( Class.CastTo( player, insider.GetObject() ) )
		{
			Print("CLIENT: " + player.GetIdentity().GetPlainName() + " entered capture area");
			g_RTBase.SetIsClientInCaptureZone(true);
		}
	}
	
	override void OnLeaveClientEvent(TriggerInsider insider)
	{
		super.OnLeaveClientEvent(insider);
		
		PlayerBase player;
		if(Class.CastTo( player, insider.GetObject() ) )
		{
			Print("CLIENT: " + player.GetIdentity().GetPlainName() + " left capture area");
			g_RTBase.SetIsClientInCaptureZone(false);
		}
	}
	
	override void OnStayClientEvent(TriggerInsider insider, float deltaTime) 
	{
		super.OnStayClientEvent(insider, deltaTime);
	}
	
	override void OnEnterServerEvent(TriggerInsider insider)
	{
		super.OnEnterServerEvent(insider);

		PlayerBase player;
		if( Class.CastTo( player, insider.GetObject()))
		{
			PlayerIdentity identity = player.GetIdentity();
			Print("SERVER: " + identity.GetPlainName() + " entered capture area");
			m_InsiderCount = GetInsiders().Count();
			GetRPCManager().SendRPC("RadioTower", "ClientEnteredCaptureZone", new Param2<int, float>(m_InsiderCount, m_CapturePct), true, identity);
		}
	}
	
	override void OnLeaveServerEvent(TriggerInsider insider)
	{
		super.OnLeaveServerEvent(insider);
		
		PlayerBase player;
		if(Class.CastTo( player, insider.GetObject()))
		{
			PlayerIdentity identity = player.GetIdentity();
			//Print("SERVER: " + identity.GetPlainName() + " left capture area");
			m_InsiderCount = GetInsiders().Count();
			GetRPCManager().SendRPC("RadioTower", "UpdateInsiderCount", new Param1<int>(m_InsiderCount), true, identity);
		}
	}
	
	override void OnStayServerEvent(TriggerInsider insider, float deltaTime)
	{
		super.OnStayServerEvent(insider, deltaTime);
		
		m_TimeAccuStay += deltaTime;
		if (m_TimeAccuStay > m_UpdateInterval)
		{
			m_TimeAccuStay = 0;
			if (m_InsiderCount > 0 && m_CapturePct <= m_TotalCapturePct)
			{
				m_CapturePct += m_CaptureSlice;
				Print("Area captured " + m_CapturePct + "%");
				PlayerBase player;
				if(Class.CastTo( player, insider.GetObject() ) )
				{
					PlayerIdentity identity = player.GetIdentity();
					GetRPCManager().SendRPC("RadioTower", "UpdateCaptureProgress", new Param1<float>(m_CapturePct), true, identity);
				}
			}
			else
			{
				OnEventCapture();
			}
		} 
	}
}