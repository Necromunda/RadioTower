class RTEvent
{
	protected RTServer m_Server;
	protected CaptureArea m_CaptureArea;
	protected RTLocation m_EventLocation;
	
	void ~RTEvent()
	{
		Print("[RadioTower] RTEvent dtor");
	}
	
	void RTEvent()
	{
		Print("[RadioTower] RTEvent ctor");
		
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
	
	void SetEventTrigger(CaptureArea trigger)		
	{ 
		m_CaptureArea = trigger; 
	}
	
	void SetEventLocation(RTLocation location)		
	{ 
		m_EventLocation = location; 
	}
	
	string GetEventTitle()						
	{ 
		return m_EventLocation.locationTitle; 
	}
	
	string GetLootcrateClassName() 					
	{ 
		return m_EventLocation.lootcrateClassName; 
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
	protected bool m_AllowSameEventSpawnInARow;
	protected string m_DefaultLootcrate;
	
	void ~RTBase()
	{
		RTLogger.ClearInstance();
		Print("[RadioTower] RTBase dtor");
	}
	
	void RTBase()
	{
		Print("[RadioTower] RTBase ctor");
		
		m_Settings = RTSettings.Load();
		m_Props = RTProps.Load();
		m_Locations = RTLocations.Load();
		
		m_RTEvent = null;
		m_EventSpawnTimer = new Timer;
		m_Events = new array<ref RTEvent>();
		m_LogMessage = "";
		
		m_AllowSameEventSpawnInARow = RTConstants.RT_ALLOW_SAME_EVENT_SPAWN_IN_A_ROW;
		bool enableLogging = RTConstants.RT_ENABLE_LOGGING;
		float spawnInterval = RTConstants.RT_EVENT_SPAWN_INTERVAL_DEFAULT;
		
		if (m_Settings)
		{
			spawnInterval = m_Settings.eventSpawnInterval;
			m_AllowSameEventSpawnInARow = m_Settings.allowSameEventSpawnInARow;
			enableLogging = m_Settings.enableLogging;
			m_DefaultLootcrate = m_Settings.eventDefaultLootcrate;
		}
		
		RTLogger.CreateInstance();
		RTLogger.GetInstance().SetCreateLogs(enableLogging);
		//RTLogger.GetInstance().LogMessage("[RadioTower] Class RTBase initialize complete");
		
		m_EventSpawnTimer.Run(spawnInterval, this, "CreateEvent", NULL, true);	
	}
	
	RTEvent GetRTEvent()
	{
		return m_RTEvent;
	}
	
	
	RTEvent GetLastRTEvent()
	{
		if (m_Events.Count() == 0) return null;

		RTEvent lastEvent;
		if (Class.CastTo(lastEvent, m_Events.Get(m_Events.Count() - 1)))
			return lastEvent;
		
		return null;
	}
	
	int GetIndexOfLocation(string locationTitle)
	{
		if (m_Locations)
		{
			for (int i = 0; i < m_Locations.eventLocations.Count(); i++)
			{
				RTLocation location;
				if (Class.CastTo(location, m_Locations.eventLocations[i]))
				{
					if (locationTitle == location.locationTitle)
						return i;
				}
			}
		}
		
		return -1;
	}
	
	bool AreAllEventsActive()
	{
		if (m_Events)
		{
			for (int i = 0; i < m_Events.Count(); i++)
			{
				RTEvent pastEvent;
				if (RTEvent.CastTo(pastEvent, m_Events[i]))
				{
					if (!pastEvent.IsActive())
						return false;
				}
			}
		}
		return true;
	}
	
    void CreateEvent()
    {
		m_LogMessage = "Creating event";
		Print("[RadioTower] " + m_LogMessage);
		RTLogger.GetInstance().LogMessage(m_LogMessage);
		
		// Check if we have 0 events
		int eventLocationCount = m_Locations.eventLocations.Count();
		if (eventLocationCount == 0)
		{
			m_LogMessage = "Possible misconfiguration! RTLocations.json has 0 locations";
			Print("[RadioTower] " + m_LogMessage);
			RTLogger.GetInstance().LogMessage(m_LogMessage);
			m_EventSpawnTimer.Stop();
			return;
		}
		
		// Check if we have 1 event and we can't spawn same event in a row
		if (m_RTEvent && eventLocationCount == 1 && !m_AllowSameEventSpawnInARow)
		{
			m_LogMessage = "Possible misconfiguration! Event could not be created because allowSameEventSpawnInARow = true and RTLocations.json only has one location";
			Print("[RadioTower] " + m_LogMessage);
			RTLogger.GetInstance().LogMessage(m_LogMessage);
			m_EventSpawnTimer.Stop();
			return;
		}
		
		int eventLocationIndex = Math.RandomInt(0, eventLocationCount);
		RTLocation eventLocation = RTLocation.Cast(m_Locations.eventLocations[eventLocationIndex]);	
		
		for (int i = 0; i < m_Events.Count(); i++)
		{
			// If all events are active, return to avoid infinite loop
			if (AreAllEventsActive())
			{
				m_LogMessage = "Trying to create event but all events are currently being captured!";
				Print("[RadioTower] " + m_LogMessage);
				RTLogger.GetInstance().LogMessage(m_LogMessage);
				return;
			}
			
			// Check if new event is the same as last event
			if (!m_AllowSameEventSpawnInARow)
			{
				RTEvent lastEvent = GetLastRTEvent();
				
				if (eventLocation.locationTitle == lastEvent.GetEventTitle())
				{
					m_LogMessage = "Can't create event in " + m_RTEvent.GetEventTitle() + " because it was the last event";
					Print("[RadioTower] " + m_LogMessage);
					RTLogger.GetInstance().LogMessage(m_LogMessage);
					
					eventLocationIndex = Math.RandomInt(0, eventLocationCount);
					eventLocation = RTLocation.Cast(m_Locations.eventLocations[eventLocationIndex]);
					i = 0;
				}
			}
			
			// Check if past event is still active
			RTEvent pastEvent = RTEvent.Cast(m_Events[i]);
			if (pastEvent.GetEventTitle() == eventLocation.locationTitle && pastEvent.IsActive())
			{
				m_LogMessage = "Trying to create event but " + m_RTEvent.GetEventTitle() + " is currently being captured!";
				Print("[RadioTower] " + m_LogMessage);
				RTLogger.GetInstance().LogMessage(m_LogMessage);
				
				if (eventLocationCount == 1)
				{
					m_LogMessage = "Abort event creation because only 1 location is configured and it's being captured!";
					Print("[RadioTower] " + m_LogMessage);
					RTLogger.GetInstance().LogMessage(m_LogMessage);
					return;
				}
				
				eventLocationIndex = Math.RandomInt(0, eventLocationCount);
				eventLocation = RTLocation.Cast(m_Locations.eventLocations[eventLocationIndex]);
				i = 0;
			}
		}
		
		Print("Event location index is " + eventLocationIndex.ToString());
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
		m_Events.Insert(m_RTEvent);
		
		m_LogMessage = "Server has been located in " + eventLocation.locationTitle + "!";
		RTLogger.GetInstance().LogMessage(m_LogMessage);
		RTMsgHandler.RTSendChatMessage(m_LogMessage);
		RTMsgHandler.RTSendClientAlert(RTConstants.RT_ICON, m_LogMessage, 3);
		Print("[RadioTower] Event created! " + m_LogMessage);
    }
	
	//BUG: When hacking a server that isnt associated with the latest m_RTEvent, it starts the event in the latest m_RTEvent
	// TODO: When server is hacked, find the right event from m_Events
	void StartEvent(RTServer server)
	{
		m_LogMessage = "Event started in " + m_RTEvent.GetEventTitle();
		Print("[RadioTower] " + m_LogMessage);
		RTLogger.GetInstance().LogMessage(m_LogMessage);
		
		vector pos = m_RTEvent.GetEventServer().GetPosition();
		Trigger trigger;
		if (Trigger.CastTo(trigger, GetGame().CreateObject("CaptureArea", pos)))
		{
			m_RTEvent.SetEventTrigger(trigger);
		}
	}
	
	/*void StartEvent()
	{
		m_LogMessage = "Event started in " + m_RTEvent.GetEventTitle();
		Print("[RadioTower] " + m_LogMessage);
		RTLogger.GetInstance().LogMessage(m_LogMessage);
		
		vector pos = m_RTEvent.GetEventServer().GetPosition();
		Trigger trigger;
		if (Trigger.CastTo(trigger, GetGame().CreateObject("CaptureArea", pos)))
		{
			m_RTEvent.SetEventTrigger(trigger);
		}
	}*/
	
	void SpawnEventLootCrate()
	{
		string msg = "Spawning lootcrate";
		Print("[RadioTower] " + msg);
		m_LogMessage = m_RTEvent.GetEventTitle() + ": " + m_LogMessage;
		RTLogger.GetInstance().LogMessage(m_LogMessage);
		
		string lootcrate = m_RTEvent.GetLootcrateClassName();
		if (lootcrate == "")
			lootcrate = m_DefaultLootcrate;
		vector pos = m_RTEvent.GetEventLocation().lootcrateCoordinatesXYZ;
		vector orientation = m_RTEvent.GetEventLocation().lootcrateOrientationYPR;
		
		RTLootcrate_Base crate;
		if (RTLootcrate_Base.CastTo(crate, GetGame().CreateObject(lootcrate, pos, ECE_LOCAL | ECE_KEEPHEIGHT)))
		{
			crate.SetOrientation(orientation);
			array<ref RTLoot> loot = m_RTEvent.GetEventLocation().loot;
			for (int i = 0; i < loot.Count(); i++)
			{
				ItemBase item;
				if (ItemBase.CastTo(item, crate.GetInventory().CreateEntityInCargo(loot[i].lootClassName)))
				{
					m_LogMessage = "Item: " + loot[i].lootClassName;
					RTLogger.GetInstance().LogMessage(m_LogMessage);
					string attachmentsMsg = "";
					
					TStringArray attachments = loot[i].attachments;
					for (int j = 0; j < attachments.Count(); j++)
					{
						item.GetInventory().CreateAttachment(attachments[j]);
						attachmentsMsg += attachments[j] + ", ";
					}
					m_LogMessage += " attachments: " + attachmentsMsg;
					RTLogger.GetInstance().LogMessage(m_LogMessage);
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