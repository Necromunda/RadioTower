class RTEvent
{
	protected RTServer m_Server;
	protected Trigger m_CaptureArea;
	protected RTEventLocation m_EventLocation;
	
	void ~RTEvent()
	{
		Print(RTConstants.RT_LOG_PREFIX + " RTEvent dtor");
	}
	
	void RTEvent()
	{
		Print(RTConstants.RT_LOG_PREFIX + " RTEvent ctor");
		
		m_Server = null;
		m_CaptureArea = null;
		m_EventLocation = null;
	}
	
	void SetEventServer(RTServer server)  		
	{ 
		m_Server = server; 
	}
	
	void SetEventTrigger(Trigger trigger)		
	{ 
		m_CaptureArea = trigger; 
	}
	
	void SetEventLocation(RTEventLocation location)		
	{ 
		m_EventLocation = location; 
	}
	
	string GetEventTitle()						
	{ 
		return m_EventLocation.title; 
	}
	
	string GetLootcrateTitle() 					
	{ 
		return m_EventLocation.eventLootcrate; 
	}
	
	RTServer GetEventServer() 					
	{ 
		return m_Server; 
	}
	
	Trigger GetEventTrigger() 					
	{ 
		return m_CaptureArea; 
	}
	
	RTEventLocation GetEventLocation()
	{ 
		return m_EventLocation; 
	}
}

class RTBase
{
	ref RTEventConfig m_Config;
	protected ref RTEvent m_RTEvent;
	protected bool m_IsClientInCaptureZone;
	protected ref Timer m_EventSpawnTimer;
	
	void ~RTBase()
	{
		Print(RTConstants.RT_LOG_PREFIX + " RTBase dtor");
	}
	
	void RTBase()
	{
		Print(RTConstants.RT_LOG_PREFIX + " RTBase ctor");
		m_Config = RTSettings.Load();
		m_RTEvent = null;
		m_IsClientInCaptureZone = false;
		//m_EventSpawnTimer = new Timer;
		
		float spawnInterval = RTConstants.RT_EVENT_SPAWN_INTERVAL_DEFAULT;
		if (m_Config)
		{
			spawnInterval = m_Config.eventSpawnInterval;
		}
		//m_EventSpawnTimer.Run(spawnInterval, this, "CreateEvent", NULL, true);
				
		//GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CreateEvent, 30000);	
	}
	
	RTEvent GetRTEvent()
	{
		return m_RTEvent;
	}
    
	bool IsClientInCaptureZone()
	{
		return m_IsClientInCaptureZone;
	}
	
	void SetIsClientInCaptureZone(bool value)
	{
		m_IsClientInCaptureZone = value;
	}
	
    void CreateEvent()
    {
		if (!GetGame().IsServer() || !GetGame().IsMultiplayer())
			return;
		
		int eventLocationCount = m_Config.eventLocations.Count();
		if (eventLocationCount == 0)
			return;
		
		int eventLocationIndex = Math.RandomInt(0, eventLocationCount);
		RTEventLocation eventLocation;
		if (Class.CastTo(eventLocation, m_Config.eventLocations[eventLocationIndex]))
		{
			m_RTEvent = new RTEvent();
			m_RTEvent.SetEventLocation(eventLocation);
			
			vector position = eventLocation.coordinates_xyz;
			vector orientation = eventLocation.orientation_ypr;
			vector lootcratePosition = eventLocation.lootcrate_coordinates_xyz;
			
			array<Object> objects = new array<Object>;
			GetGame().GetObjectsAtPosition(position, 1, objects, null);
			foreach(Object serverObj: objects)
			{
				if (serverObj.IsKindOf("RTServer_Base"))
					GetGame().ObjectDelete(serverObj);
			}
			objects.Clear();
			GetGame().GetObjectsAtPosition(lootcratePosition, 1, objects, null);
			foreach(Object crateObj: objects)
			{
				if (crateObj.IsKindOf("RTLootcrate_Base"))
					GetGame().ObjectDelete(crateObj);
			}
			
			RTServer item;
			if (RTServer.CastTo(item, GetGame().CreateObjectEx("RTServer", position, ECE_KEEPHEIGHT))
			{
				item.SetPosition(position);
				item.SetOrientation(orientation);
				item.SetFlags(EntityFlags.STATIC, false);
				item.Update();

				m_RTEvent.SetEventServer(item);
			}
			string msg = "Server has been located in " + eventLocation.title + "!";
			//RTMsgHandler.RTSendChatMessage("Server has been located in " + eventLocation.title + "!");
			RTMsgHandler.RTSendClientAlert(RTConstants.RT_ICON, msg, 3);
			Print(RTConstants.RT_LOG_PREFIX + " Server placed");
		}
		
		//GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).RemoveByName(this, "CreateEvent");
    }
	
	void StartEvent()
	{
		Print("StartEvent " + m_RTEvent.GetEventServer().GetPosition());
		
		vector pos = m_RTEvent.GetEventServer().GetPosition();
		Trigger trigger;
		if (Trigger.CastTo(trigger, GetGame().CreateObject("CaptureArea", pos)))
		{
			m_RTEvent.SetEventTrigger(trigger);
		}
	}
	
	void SpawnEventLootCrate()
	{
		if (!GetGame().IsServer() || !GetGame().IsMultiplayer())
			return;
		
		Print("Spawning loot crate");
		
		string title = m_RTEvent.GetLootcrateTitle();
		vector pos = m_RTEvent.GetEventLocation().lootcrate_coordinates_xyz;
		vector orientation = m_RTEvent.GetEventLocation().lootcrate_orientation_ypr;
		
		RTLootcrate_Base crate;
		if (RTLootcrate_Base.CastTo(crate, GetGame().CreateObject(title, pos, ECE_LOCAL | ECE_KEEPHEIGHT)))
		{
			crate.SetOrientation(orientation);
			array<ref RTEventLoot> loot = m_RTEvent.GetEventLocation().loot;
			for (int i = 0; i < loot.Count(); i++)
			{
				ItemBase item;
				if (ItemBase.CastTo(item, crate.GetInventory().CreateEntityInCargo(loot[i].title)))
				{
					TStringArray attachments = loot[i].attachments;
					for (int j = 0; j < attachments.Count(); j++)
					{
						item.GetInventory().CreateAttachment(attachments[j]);
					}
				}
			}
		}
	}
}

static ref RTBase g_RTBase;
static ref RTBase GetRTBase()
{
	if (!g_RTBase)
	{
		g_RTBase = new ref RTBase();
	}
	return g_RTBase;
}