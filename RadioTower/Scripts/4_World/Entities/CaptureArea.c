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
	//protected float m_TimeAccuStay;
	protected float m_CapturePct;
	protected float m_CapturePctLocal;
	protected float m_CaptureSlice;
	protected float m_UpdateInterval;
	protected float m_TotalCapturePct;
	
	protected const float LIFETIME_TICKRATE = 1; // seconds
	protected ref Timer m_LifetimeTimer;
	protected float m_Event_Lifetime;
	
	protected int m_InsiderCount;
	protected int m_InsiderCountLocal;
	
	protected ref Timer m_CaptureTimer;
	
	protected bool m_StartCapture;
	protected bool m_StartCaptureLocal;
	
	void CaptureArea()
	{
		RegisterNetSyncVariableFloat("m_CapturePct");
		RegisterNetSyncVariableInt("m_InsiderCount");
		RegisterNetSyncVariableBool("m_StartCapture");
		
		//Print("[RadioTower] Capture area ctor");
		m_StartCapture = false;
		m_StartCaptureLocal = false;
		m_CollisionCylinderRadius = RTConstants.RT_EVENT_TRIGGER_RADIUS_DEFAULT;
		m_CollisionCylinderHeight = RTConstants.RT_EVENT_TRIGGER_HEIGHT_DEFAULT;
		
		m_Event_Capturetime = RTConstants.RT_EVENT_CAPTURETIME_DEFAULT;
		//m_TimeAccuStay = 0;
		m_CapturePct = 0;
		m_UpdateInterval = RTConstants.RT_EVENT_UPDATE_PROGRESS_INTERVAL_DEFAULT;
		m_TotalCapturePct = RTConstants.RT_EVENT_TOTAL_CAPTURE_PCT_DEFAULT;
		
		m_LifetimeTimer = new Timer;
		m_CaptureTimer = new Timer;
		m_Event_Lifetime = RTConstants.RT_EVENT_LIFETIME_DEFAULT;
		
		if (g_RTBase)
		{
			if (g_RTBase.m_Settings)
			{
				//m_Event_Capturetime = g_RTBase.m_Settings.eventCapturetime;
				//m_Event_Lifetime = g_RTBase.m_Settings.eventLifetime;
				m_Event_Capturetime = g_RTBase.m_Settings.kothEvent.captureTime;
				// Make sure that not dividing with 0 on line 108
				if (m_Event_Capturetime <= 0)
					m_Event_Capturetime = RTConstants.RT_EVENT_CAPTURETIME_DEFAULT;
				m_Event_Lifetime = g_RTBase.m_Settings.kothEvent.lifeTime;
			}
			
			RTEvent rtEvent = g_RTBase.GetRTEvent();
			if (rtEvent)
			{
				m_CollisionCylinderRadius = rtEvent.GetEventLocation().captureAreaRadius;
				m_CollisionCylinderHeight = rtEvent.GetEventLocation().captureAreaHeight;
			}
		}
		m_CaptureSlice = (m_TotalCapturePct / m_Event_Capturetime) * m_UpdateInterval;
		
		m_LifetimeTimer.Run(LIFETIME_TICKRATE, this, "Tick", NULL, true);
	}
	
	void ~CaptureArea()
	{
		if (m_LifetimeTimer)
		{
			m_LifetimeTimer.Stop();
			delete m_LifetimeTimer;			
		}
		
		if (m_CaptureTimer)
		{
			m_CaptureTimer.Stop();
			delete m_CaptureTimer;
		}
	}
	
	override void EOnInit(IEntity other, int extra)
	{
		array<Object> objects = new array<Object>;
		GetGame().GetObjectsAtPosition(GetPosition(), 1, objects, null);
		foreach(Object serverObj: objects)
		{
			if (serverObj.IsKindOf("RTServer_Base"))
			{
				RTServer server;
				if (RTServer.CastTo(server, serverObj))
				{
					RTEvent rtEvent = g_RTBase.GetRTEventWithServer(server);
					if (rtEvent)
					{
						m_CollisionCylinderRadius = rtEvent.GetEventLocation().captureAreaRadius;
						m_CollisionCylinderHeight = rtEvent.GetEventLocation().captureAreaHeight;
					}
				}
			}
		}
		//Print("CaptureArea radius: " + m_CollisionCylinderRadius + ", height: " + m_CollisionCylinderHeight);
		SetCollisionCylinder(m_CollisionCylinderRadius, m_CollisionCylinderHeight);
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		
		if (m_CapturePct != m_CapturePctLocal)
		{
			m_CapturePctLocal = m_CapturePct;
		}
		
		if (m_InsiderCount != m_InsiderCountLocal)
		{
			m_InsiderCountLocal = m_InsiderCount;
		}
		
		if (m_StartCapture != m_StartCaptureLocal)
		{
			m_StartCaptureLocal = m_StartCapture;
		}
		
		if (GetGame().IsClient())
		{
			g_Game.SetCapturePct(m_CapturePctLocal);
			g_Game.SetInsiderCount(m_InsiderCountLocal);
			m_StartCaptureLocal = m_StartCapture;
		}
	}
	
	bool NeedsSync()
	{
		return m_CapturePct != m_CapturePctLocal || m_InsiderCount != m_InsiderCountLocal || m_StartCapture != m_StartCaptureLocal;
	}
	
	void SetCapture(bool value)
	{
		m_StartCapture = value;
		SetSynchDirty();
	}
	
	void Tick()
	{
		string date = CF_Date.Now().Format("DD-MM-YYYY hh:mm:ss");
		//if (m_Event_Lifetime.ToString().ToInt() % 60 == 0)
			//Print(date + " CaptureArea lifetime " + m_Event_Lifetime);
		m_Event_Lifetime -= LIFETIME_TICKRATE;
		
		if (m_Event_Lifetime < 0)
		{
			OnEventFinish();
		}
	}
	
	float GetEventLifetime()
	{
		return m_Event_Lifetime;
	}
	
	/*void OnEventCapture()
	{
		m_Event_Lifetime = 0;
		g_RTBase.SpawnEventLootCrate();
		string title = g_RTBase.GetRTEvent().GetEventTitle();
		string msg = title + " has been captured!";
		RTLogger.GetInstance().LogMessage(msg);
		RTMsgHandler.RTSendChatMessage(msg);
		RTMsgHandler.RTSendClientAlert(RTConstants.RT_ICON, msg, 3);
	}*/
	
	void OnEventFinish()
	{
		if (GetGame().IsServer())
		{
			RTEvent rtEvent;
			if (RTEvent.CastTo(rtEvent, g_RTBase.GetRTEventWithTrigger(this)))
			{
				RTServer server = rtEvent.GetEventServer();
				if (server)
				{
					if (rtEvent.GetState() != RTEventState.CAPTURED)
					{
						server.Disable();
						RTLogger.GetInstance().LogMessage("[Event finish] " + rtEvent.GetEventTitle());
						if (g_RTBase.IsNotificationAllowed(RTNotificationType.END))
						{
							string ingame_msg = rtEvent.GetEventTitle() + " event finished";
							RTMsgHandler.RTSendChatMessage(ingame_msg);
							RTMsgHandler.RTSendClientAlert(RTConstants.RT_ICON, ingame_msg, 3);
						}
						server.SetCaptureStateSynchronized(CaptureState.DEFAULT);
					}
				}
				rtEvent.SetState(RTEventState.DELETED);
				#ifdef LBmaster_Groups
				//LBServerMarker marker = rtEvent.GetLBMapMarker();
				rtEvent.RemoveLBMapMarker();
				#endif
				
				GetRPCManager().SendRPC("RadioTower", "ClientDeleteLatestEventLocation", null, true, null);
			}
		}
		
		/*if (GetGame().IsClient())
		{
			Print("client eventfinish");
			array<ref TriggerInsider> insiders = GetInsiders();
			for (int i = 0; i < insiders.Count(); i++)
			{
				PlayerBase player;
				if (PlayerBase.CastTo(player, insiders[i].GetObject()))
					player.SetIsInsideCaptureArea(false);
			}
		}*/
		//Print("[RadioTower] Capture area deleted");
		DeleteSafe();
	}
	
	override bool ShouldRemoveInsider(TriggerInsider insider)
	{
		EntityAI entity;
		if (EntityAI.CastTo(entity, insider.GetObject()))
		{
			if (entity.IsPlayer() && entity.IsAlive())
				return false;
			//if (entity.IsAlive() && (entity.IsZombie() || entity.IsPlayer()))
				//return false;
		}
		return true;
	}
	
	/*protected bool CanAddObjectAsInsider(Object object)
	{
		EntityAI entity;
		if (EntityAI.CastTo(entity, object))
		{
			if (entity.IsPlayer() && entity.IsAlive())
				return true;
		}
		return false;
	}*/
	
	/*override void OnEnterClientEvent(TriggerInsider insider)
	{
		super.OnEnterClientEvent(insider);
		
		Print("CLIENT OnEnter: m_StartCaptureLocal = " + m_StartCaptureLocal);
		if (!m_StartCaptureLocal) return;
		
		PlayerBase player;
		if (Class.CastTo(player, insider.GetObject()))
		{
			//g_Game.SetIsClientInCaptureZone(true);
			player.SetIsInsideCaptureArea(true);
		}
	}*/
	
	override void OnLeaveClientEvent(TriggerInsider insider)
	{
		super.OnLeaveClientEvent(insider);
		
		//Print("CLIENT OnLeave: m_StartCaptureLocal = " + m_StartCaptureLocal);
		if (!m_StartCaptureLocal) return;
		
		PlayerBase player;
		if (Class.CastTo(player, insider.GetObject()))
		{
			//g_Game.SetIsClientInCaptureZone(false);
			player.SetIsInsideCaptureArea(false);
		}
	}
	
	/*override void OnEnterServerEvent(TriggerInsider insider)
	{
		super.OnEnterServerEvent(insider);

		Print("SERVER OnEnter: m_StartCapture = " + m_StartCapture);
		//if (!m_StartCapture) return;
		
		PlayerBase player;
		if (Class.CastTo(player, insider.GetObject()))
		{
			PlayerIdentity identity = player.GetIdentity();
			//if (identity)
				//Print("SERVER: " + identity.GetPlainName() + " entered capture area");
			m_InsiderCount = GetInsiders().Count();
			SetSynchDirty();
			//GetRPCManager().SendRPC("RadioTower", "ClientEnteredCaptureZone", new Param2<int, float>(m_InsiderCount, m_CapturePct), true, identity);
		}
	}
	
	override void OnLeaveServerEvent(TriggerInsider insider)
	{
		super.OnLeaveServerEvent(insider);
		
		Print("SERVER OnLeave: m_StartCapture = " + m_StartCapture);
		if (!m_StartCapture) return;
		
		PlayerBase player;
		if (Class.CastTo(player, insider.GetObject()))
		{
			if (m_CaptureTimer.IsRunning())
				m_CaptureTimer.Stop();
			PlayerIdentity identity = player.GetIdentity();
			//if (identity)
				//Print("SERVER: " + identity.GetPlainName() + " left capture area");
			m_InsiderCount = GetInsiders().Count();
			SetSynchDirty();
			//GetRPCManager().SendRPC("RadioTower", "UpdateInsiderCount", new Param1<int>(m_InsiderCount), true, identity);
		}
	}*/
	
	override void OnEnterServerEvent(TriggerInsider insider)
	{
		super.OnEnterServerEvent(insider);
		
		PlayerBase player = PlayerBase.Cast(insider.GetObject());
		if (player && player.GetIdentity())
			RTLogger.GetInstance().LogMessage("Player " + player.GetIdentity().GetPlainName() + " entered capture area");
		//Print("SERVER OnEnter");
		m_InsiderCount = GetInsiders().Count();
		SetSynchDirty();
	}
	
	override void OnLeaveServerEvent(TriggerInsider insider)
	{
		super.OnLeaveServerEvent(insider);
		
		PlayerBase player = PlayerBase.Cast(insider.GetObject());
		if (player && player.GetIdentity())
			RTLogger.GetInstance().LogMessage("Player " + player.GetIdentity().GetPlainName() + " left capture area");
		//Print("SERVER OnLeave");
		if (!m_StartCapture) return;
		
		// Edit 25.3.2024
		/*
		if (m_CaptureTimer.IsRunning())
				m_CaptureTimer.Stop();
		*/
		
		m_InsiderCount = GetInsiders().Count();
		SetSynchDirty();
	}

	void AddProgress()
	{
		if (m_CapturePct < m_TotalCapturePct)
		{
			m_InsiderCount = GetInsiders().Count();
			//Print("Insider count " + m_InsiderCount);
			//int minPlayersRequired = 1;
			bool hasEnoughPlayers = false;
			bool depleteProgress = false;
			if (g_RTBase && g_RTBase.m_Settings)
			{
				//minPlayersRequired = g_RTBase.m_Settings.kothEvent.minPlayerCountToStartCapture;
				//hasEnoughPlayers = g_RTBase.GetPlayerCount() >= minPlayersRequired;
				hasEnoughPlayers = m_InsiderCount >= g_RTBase.m_Settings.kothEvent.minPlayerCountToStartCapture;
				depleteProgress = g_RTBase.m_Settings.kothEvent.depleteProgressWhenNoPlayersCapturing;
			}
			
			if (hasEnoughPlayers)
			{
				//Print("Area captured " + m_CapturePct + "%");
				m_CapturePct += m_CaptureSlice;
				m_CapturePct = Math.Clamp(m_CapturePct, 0, m_TotalCapturePct);	
				//Print("Add progress, current: " + m_CapturePct);
			}
			else if (depleteProgress && m_CapturePct > 0)
			{
				m_CapturePct -= m_CaptureSlice;
				m_CapturePct = Math.Clamp(m_CapturePct, 0, m_TotalCapturePct);
				//Print("Deplete progress, current: " + m_CapturePct);
			}
			
			if (NeedsSync())
				SetSynchDirty();
		}
		else
		{
			g_RTBase.OnEventCapture(this);
			m_Event_Lifetime = 0;
		}
	}
	
	/*
	void AddProgress(TriggerInsider insider)
	{
		EntityAI entity = EntityAI.Cast(insider.GetObject());
		//EntityAI entity = EntityAI.Cast(insider.GetObject());
		
		//Print("INSIDER COUNT: " + m_InsiderCount);
		//Print(entity.GetType() + " is player " + entity.IsPlayer());
		if (m_CapturePct < m_TotalCapturePct)
		{
			m_InsiderCount = GetInsiders().Count();
			
			int minPlayersRequired = 1;
			bool hasEnoughPlayers = false;
			bool depleteProgress = false;
			if (g_RTBase && g_RTBase.m_Settings)
				hasEnoughPlayers = g_RTBase.GetPlayerCount() >= g_RTBase.m_Settings.kothEvent.minPlayerCountToStartCapture;
			{
				minPlayersRequired = g_RTBase.m_Settings.kothEvent.minPlayerCountToStartCapture;
				hasEnoughPlayers = g_RTBase.GetPlayerCount() >= minPlayersRequired;
				depleteProgress = g_RTBase.m_Settings.kothEvent.depleteProgressWhenNoPlayersCapturing;
			}
			
			if (entity.IsPlayer() && g_RTBase && hasEnoughPlayers)
			{
				//Print("Area captured " + m_CapturePct + "%");
				m_CapturePct += m_CaptureSlice;
				m_CapturePct = Math.Clamp(m_CapturePct, 0, m_TotalCapturePct);	
				Print("Add progress, current: " + m_CapturePct);
			}

			m_InsiderCount = GetInsiders().Count();
			SetSynchDirty();
			/*PlayerBase player;
			if (Class.CastTo(player, insider.GetObject()))
			{
				PlayerIdentity identity = player.GetIdentity();
				Print("IDENTITY: " + identity);
				m_InsiderCount = GetInsiders().Count();
				SetSynchDirty();
			}*/
/*		
}
		else
		{
			//OnEventCapture();
			g_RTBase.OnEventCapture(this);
			m_Event_Lifetime = 0;
			//g_RTBase.OnEventCapture(this);
		}
	}
	*/
	
	override void OnStayClientEvent(TriggerInsider insider, float deltaTime)
	{
		super.OnStayClientEvent(insider, deltaTime);
		
		if (!m_StartCaptureLocal) return;
		
		PlayerBase player;
		if (Class.CastTo(player, insider.GetObject()))
			if (!player.GetIsInsideCaptureArea())
				player.SetIsInsideCaptureArea(true);
		
		//Print("CapturePct is " + m_CapturePctLocal);
	}
	
	override void OnStayServerEvent(TriggerInsider insider, float deltaTime)
	{
		super.OnStayServerEvent(insider, deltaTime);
		
		if (!m_StartCapture) return;

		if (!m_CaptureTimer.IsRunning())
			m_CaptureTimer.Run(m_UpdateInterval, this, "AddProgress", null, true);
			//m_CaptureTimer.Run(m_UpdateInterval, this, "AddProgress", new Param1<TriggerInsider>(insider), true);
		
		//Print("CapturePct is " + m_CapturePct);
	}
	
	/*override void OnStayServerEvent(TriggerInsider insider, float deltaTime)
	{
		super.OnStayServerEvent(insider, deltaTime);
		
		m_TimeAccuStay += deltaTime;
		if (m_TimeAccuStay > m_UpdateInterval)
		{
			m_TimeAccuStay = 0;
			if (m_InsiderCount > 0 && m_CapturePct < m_TotalCapturePct)
			{
				m_CapturePct += m_CaptureSlice;
				m_CapturePct = Math.Clamp(m_CapturePct, 0, m_TotalCapturePct);
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
	}*/
}