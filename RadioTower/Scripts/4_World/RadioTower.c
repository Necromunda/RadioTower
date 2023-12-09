class RTEvent
{
	protected RTServer m_Server;
	protected Trigger m_CaptureArea;
	protected RTLocation m_EventLocation;
	
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
	
	void Cleanup()
	{
		if (m_Server)
			m_Server.Delete();
		
		if (m_CaptureArea)
			m_CaptureArea.Delete();
		
		m_Server = null;
		m_CaptureArea = null;
	}
	
	void SetEventServer(RTServer server)  		
	{ 
		m_Server = server; 
	}
	
	void SetEventTrigger(Trigger trigger)		
	{ 
		m_CaptureArea = trigger; 
	}
	
	void SetEventLocation(RTLocation location)		
	{ 
		m_EventLocation = location; 
	}
	
	string GetEventTitle()						
	{ 
		return m_EventLocation.title; 
	}
	
	string GetLootcrateTitle() 					
	{ 
		return m_EventLocation.lootcrate; 
	}
	
	RTServer GetEventServer() 					
	{ 
		return m_Server; 
	}
	
	Trigger GetEventTrigger() 					
	{ 
		return m_CaptureArea; 
	}
	
	RTLocation GetEventLocation()
	{ 
		return m_EventLocation; 
	}
}

class RTBase
{
	ref RTSettings m_Settings;
	ref RTLocations m_Locations;
	ref RTProps m_Props;
	protected ref RTEvent m_RTEvent;
	ref Timer m_EventSpawnTimer;
	array<ref RTEvent> m_Events;
	
	void ~RTBase()
	{
		Print(RTConstants.RT_LOG_PREFIX + " RTBase dtor");
	}
	
	void RTBase()
	{
		Print(RTConstants.RT_LOG_PREFIX + " RTBase ctor");
		m_Settings = RTSettings.Load();
		m_Props = RTProps.Load();
		m_Locations = RTLocations.Load();
		m_RTEvent = null;
		m_EventSpawnTimer = new Timer;
		m_Events = new array<ref RTEvent>();
		
		float spawnInterval = RTConstants.RT_EVENT_SPAWN_INTERVAL_DEFAULT;
		if (m_Settings)
		{
			spawnInterval = m_Settings.eventSpawnInterval;
		}
		m_EventSpawnTimer.Run(spawnInterval, this, "CreateEvent", NULL, true);	
	}
	
	RTEvent GetRTEvent()
	{
		return m_RTEvent;
	}
	
    void CreateEvent()
    {
		Print("[RadioTower] Creating event");
		int eventLocationCount = m_Locations.eventLocations.Count();
		if (eventLocationCount == 0)
			return;
		
		int eventLocationIndex = Math.RandomInt(0, eventLocationCount);
		RTLocation eventLocation;
		if (Class.CastTo(eventLocation, m_Locations.eventLocations[eventLocationIndex]))
		{
			m_RTEvent = new RTEvent();
			m_RTEvent.SetEventLocation(eventLocation);
			
			vector position = eventLocation.locationCoordinatesXYZ;
			vector orientation = eventLocation.locationOrientationYPR;
			vector lootcratePosition = eventLocation.lootcrateCoordinatesXYZ;
			
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
			if (RTServer.CastTo(item, GetGame().CreateObjectEx("RTServer", position, ECE_KEEPHEIGHT)))
			{
				item.SetPosition(position);
				item.SetOrientation(orientation);
				item.SetFlags(EntityFlags.STATIC, false);
				item.Update();

				m_RTEvent.SetEventServer(item);
			}
			string msg = "Server has been located in " + eventLocation.title + "!";
			RTMsgHandler.RTSendChatMessage(msg);
			RTMsgHandler.RTSendClientAlert(RTConstants.RT_ICON, msg, 3);
			Print(RTConstants.RT_LOG_PREFIX + " Server placed");
			m_Events.Insert(m_RTEvent);
		}
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
		Print("[RadioTower] Spawning lootcrate");
		
		string title = m_RTEvent.GetLootcrateTitle();
		vector pos = m_RTEvent.GetEventLocation().lootcrateCoordinatesXYZ;
		vector orientation = m_RTEvent.GetEventLocation().lootcrateOrientationYPR;
		
		RTLootcrate_Base crate;
		if (RTLootcrate_Base.CastTo(crate, GetGame().CreateObject(title, pos, ECE_LOCAL | ECE_KEEPHEIGHT)))
		{
			crate.SetOrientation(orientation);
			array<ref RTLoot> loot = m_RTEvent.GetEventLocation().loot;
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