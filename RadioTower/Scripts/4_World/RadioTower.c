class RTEvent
{
	protected RTServer m_Server;
	protected CaptureArea m_CaptureArea;
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
	
	bool IsActive()
	{
		if (m_CaptureArea && m_CaptureArea.GetEventLifetime() > 0)
			return true;

		return false;
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
	
	CaptureArea GetEventTrigger() 					
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
	ref array<ref RTEvent> m_Events;
	protected string m_LogMessage;
	
	void ~RTBase()
	{
		RTLogger.ClearInstance();
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
		m_LogMessage = "";
		
		int createLogs = m_Settings.enableLogging;
		RTLogger.CreateInstance();
		RTLogger.GetInstance().SetCreateLogs(createLogs);
		RTLogger.GetInstance().LogMessage("[RadioTower] CTOR");
		
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
		m_LogMessage = "Creating event";
		Print("[RadioTower] " + m_LogMessage);
		RTLogger.GetInstance().LogMessage(m_LogMessage);
		
		int eventLocationCount = m_Locations.eventLocations.Count();
		if (eventLocationCount == 0)
			return;
		
		int eventLocationIndex = Math.RandomInt(0, eventLocationCount);
		RTLocation eventLocation = RTLocation.Cast(m_Locations.eventLocations[eventLocationIndex]);	
			
		int pastEventsCount = m_Events.Count();
		for (int i = 0; i < pastEventsCount; i++)
		{
			RTEvent pastEvent = RTEvent.Cast(m_Events[i]);
			if (pastEvent.GetEventTitle() == eventLocation.title && pastEvent.IsActive())
			{
				Print("[RadioTower] Trying to spawn event but " + m_RTEvent.GetEventTitle() + " is active!");
				if (eventLocationCount == 1)
					return;
				
				eventLocationIndex = Math.RandomInt(0, eventLocationCount);
				eventLocation = RTLocation.Cast(m_Locations.eventLocations[eventLocationIndex]);
				i = 0;
			}
		}

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
		m_LogMessage = "Server has been located in " + eventLocation.title + "!";
		RTLogger.GetInstance().LogMessage(m_LogMessage);
		RTMsgHandler.RTSendChatMessage(m_LogMessage);
		RTMsgHandler.RTSendClientAlert(RTConstants.RT_ICON, m_LogMessage, 3);
		Print(RTConstants.RT_LOG_PREFIX + " " + m_LogMessage);
		m_Events.Insert(m_RTEvent);
    }
	
	void StartEvent()
	{
		m_LogMessage = "Event started in " + m_RTEvent.GetEventTitle();
		Print(m_LogMessage);
		RTLogger.GetInstance().LogMessage(m_LogMessage);
		
		vector pos = m_RTEvent.GetEventServer().GetPosition();
		Trigger trigger;
		if (Trigger.CastTo(trigger, GetGame().CreateObject("CaptureArea", pos)))
		{
			m_RTEvent.SetEventTrigger(trigger);
		}
	}
	
	void SpawnEventLootCrate()
	{
		string msg = "Spawning lootcrate";
		Print("[RadioTower] " + msg);
		m_LogMessage = m_RTEvent.GetEventTitle() + " " + m_LogMessage;
		RTLogger.GetInstance().LogMessage(m_LogMessage);
		
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
					m_LogMessage = "Item: " + loot[i].title;
					RTLogger.GetInstance().LogMessage(m_LogMessage);
					TStringArray attachments = loot[i].attachments;
					for (int j = 0; j < attachments.Count(); j++)
					{
						item.GetInventory().CreateAttachment(attachments[j]);
						m_LogMessage = "Attachment: " + attachments[j];
						RTLogger.GetInstance().LogMessage(m_LogMessage);
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