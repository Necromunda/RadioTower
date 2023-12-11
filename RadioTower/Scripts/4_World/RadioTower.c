enum RTEventState
{
	DELETED,
	ACTIVE,
	CAPTURING
}

class RTEvent
{
	protected RTServer m_Server;
	protected CaptureArea m_CaptureArea;
	protected RTLocation m_EventLocation;
	protected RTEventState m_State;
	
	void ~RTEvent()
	{
		Print("[RadioTower] RTEvent dtor");
	}
	
	void RTEvent()
	{
		Print("[RadioTower] RTEvent ctor");
		
		m_State = RTEventState.DELETED;
		m_Server = null;
		m_CaptureArea = null;
		m_EventLocation = null;
	}
	
	void StartEvent()
	{
		if (m_CaptureArea)
		{
			
		}
	}
	
	bool IsActive()
	{
		if (m_State == RTEventState.ACTIVE || m_State == RTEventState.CAPTURING)
			return true;

		return false;
	}
	
	bool IsCaptureInProgress()
	{
		if (m_State == RTEventState.CAPTURING)
			return true;

		return false;
	}
	
	bool IsDeleted()
	{
		if (m_State == RTEventState.DELETED)
			return true;

		return false;
	}
	
	void SetState(RTEventState state)
	{
		m_State = state;
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
	
	RTEventState GetState()
	{
		return m_State;
	}
	
	RTServer GetEventServer() 					
	{ 
		return m_Server; 
	}
	
	CaptureArea GetEventTrigger() 					
	{ 
		//return m_CaptureArea;
		return m_Server.GetTrigger();
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
	
	RTEvent GetRTEventWithServer(RTServer source)
	{
		RTEvent rtEvent;
		for (int i = 0; i < m_Events.Count(); i++)
		{
			if (RTEvent.CastTo(rtEvent, m_Events[i]))
			{
				RTServer target = rtEvent.GetEventServer();
				if (source == target)
				{
					Print("Found event with server: " + rtEvent.GetEventTitle());
					return rtEvent;
				}
			}
		}
		return null;
	}
	
	RTEvent GetRTEventWithTrigger(CaptureArea source)
	{
		RTEvent rtEvent;
		for (int i = 0; i < m_Events.Count(); i++)
		{
			if (RTEvent.CastTo(rtEvent, m_Events[i]))
			{
				CaptureArea target = rtEvent.GetEventTrigger();
				if (source == target)
				{
					Print("Found event with trigger: " + rtEvent.GetEventTitle());
					return rtEvent;
				}
			}
		}
		return null;
	}
	
	
	RTEvent GetLastRTEvent()
	{
		if (m_Events.Count() == 0) return null;

		RTEvent lastEvent;
		if (Class.CastTo(lastEvent, m_Events.Get(m_Events.Count() - 1)))
			return lastEvent;
		
		return null;
	}
	
	/*int GetIndexOfLocation(string locationTitle)
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
	}*/
	
	/*bool IsPastEventActive(string locationTitle)
	{
		for (int i = 0; i < m_Events.Count(); i++)
		{
			RTEvent rtEvent = m_Events[i];
			if (!rtEvent.IsDeleted() && rtEvent.GetEventTitle() == locationTitle)
				return true;
		}
		return false;
	}*/
	
	/*bool IsPastEventCaptureInProgress(string locationTitle)
	{
		for (int i = 0; i < m_Events.Count(); i++)
		{
			RTEvent rtEvent = m_Events[i];
			if (rtEvent.IsCaptureInProgress() && rtEvent.GetEventTitle() == locationTitle)
				return true;
		}
		return false;
	}*/
	
	int GetEventLocationCount()
	{
		if (m_Locations)
			return m_Locations.eventLocations.Count();

		return 0;
	}
	
	bool IsEventLocationValid(RTLocation location)
	{
		string locationTitle = location.locationTitle;
		
		// Check if we have 0 events
		int eventLocationCount = GetEventLocationCount();
		if (eventLocationCount == 0)
		{
			m_LogMessage = "Possible misconfiguration! RTLocations.json has 0 locations";
			m_EventSpawnTimer.Stop();
			return false;
		}
		
		// Check if we have 1 event and we can't spawn same event in a row
		/*if (!m_AllowSameEventSpawnInARow)
		{
			RTEvent lastEvent = GetLastRTEvent();
			if (lastEvent && !lastEvent.IsDeleted())
			{
				// Check if new event is the same as last event and thats our only event
				if (lastEvent.GetEventTitle() == locationTitle)
				{
					if (eventLocationCount == 1)
					{
						m_LogMessage = "Possible misconfiguration! Event could not be created because allowSameEventSpawnInARow = true and RTLocations.json only has one location";
						m_EventSpawnTimer.Stop();
					}
					else
					{
						m_LogMessage = "Can't create event in " + locationTitle + " because it was the last event";
					}
					return false;
				}
			}
		}*/
		
		for (int i = 0; i < m_Events.Count(); i++)
		{			
			// Check if past event is still active
			RTEvent pastEvent = RTEvent.Cast(m_Events[i]);
			if (!pastEvent.IsDeleted())
			{
				if (locationTitle == pastEvent.GetEventTitle())
				{
					if (pastEvent.IsCaptureInProgress())
					{
						m_LogMessage = "Trying to create event but " + pastEvent.GetEventTitle() + " is still being captured!";
						return false;
					}
				}
			}
		}
		
		m_LogMessage = "Valid event location";
		return true;
	}
	
    void CreateEvent()
    {
		m_LogMessage = "Creating event";
		Print("[RadioTower] " + m_LogMessage);
		RTLogger.GetInstance().LogMessage(m_LogMessage);
		
		array<int> exhaustedEventIndexes = {};
		int eventLocationCount = GetEventLocationCount();
		int eventLocationIndex = Math.RandomInt(0, eventLocationCount);
		RTLocation eventLocation = RTLocation.Cast(m_Locations.eventLocations[eventLocationIndex]);
		
		bool isValid = IsEventLocationValid(eventLocation);
		
		while (exhaustedEventIndexes.Count() <= eventLocationCount)
		{
			if (isValid)
				break;

			exhaustedEventIndexes.Insert(eventLocationIndex);
			eventLocationIndex = Math.RandomInt(0, eventLocationCount);
			while (exhaustedEventIndexes.Find(eventLocationIndex) != -1)
			{
				if (exhaustedEventIndexes.Count() == eventLocationCount)
				{
					m_LogMessage = "No available event locations";
					Print("[RadioTower] " + m_LogMessage);
					RTLogger.GetInstance().LogMessage(m_LogMessage);
					return;
				}
				eventLocationIndex = Math.RandomInt(0, eventLocationCount);
			}
			
			eventLocation = RTLocation.Cast(m_Locations.eventLocations[eventLocationIndex]);
			isValid = IsEventLocationValid(eventLocation);
		}
		
		Print("[RadioTower] " + m_LogMessage);
		RTLogger.GetInstance().LogMessage(m_LogMessage);

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
			{
				RTServer server;
				if (RTServer.CastTo(server, serverObj))
				{
					RTEvent rtEvent = GetRTEventWithServer(server);
					if (rtEvent)
						rtEvent.SetState(RTEventState.DELETED);
				}
				GetGame().ObjectDelete(serverObj);
			}
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
		m_RTEvent.SetState(RTEventState.ACTIVE);
		m_Events.Insert(m_RTEvent);
		
		m_LogMessage = "Server has been located in " + eventLocation.locationTitle + "!";
		RTLogger.GetInstance().LogMessage(m_LogMessage);
		RTMsgHandler.RTSendChatMessage(m_LogMessage);
		RTMsgHandler.RTSendClientAlert(RTConstants.RT_ICON, m_LogMessage, 3);
		Print("[RadioTower] Event created! " + m_LogMessage);
    }
	
	void StartEvent(RTServer server)
	{
		//Print("Source server: " + server);
		RTEvent rtEvent = GetRTEventWithServer(server);
		
		if (rtEvent)
		{
			rtEvent.SetState(RTEventState.CAPTURING);
			m_LogMessage = "Event started in " + rtEvent.GetEventTitle();
			Print("[RadioTower] " + m_LogMessage);
			/*RTLogger.GetInstance().LogMessage(m_LogMessage);
			rtEvent.GetEventTrigger().SetCapture(true);
			Print("CaptureSet to true for " + rtEvent.GetEventTrigger());*/
			vector pos = rtEvent.GetEventServer().GetPosition();
			CaptureArea trigger;
			if (CaptureArea.CastTo(trigger, GetGame().CreateObject("CaptureArea", pos)))
				rtEvent.SetEventTrigger(trigger);
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
	
	void OnEventCapture(CaptureArea trigger)
	{
		RTEvent rtEvent = GetRTEventWithTrigger(trigger);
		rtEvent.SetState(RTEventState.DELETED);
		string title = rtEvent.GetEventTitle();
		string msg = title + " has been captured!";
		RTLogger.GetInstance().LogMessage(msg);
		RTMsgHandler.RTSendChatMessage(msg);
		RTMsgHandler.RTSendClientAlert(RTConstants.RT_ICON, msg, 3);
		SpawnEventLootCrate(rtEvent);
	}
	
	void SpawnEventLootCrate(RTEvent rtEvent)
	{
		string msg = "Spawning lootcrate";
		Print("[RadioTower] " + msg);
		m_LogMessage = rtEvent.GetEventTitle() + ": " + m_LogMessage;
		RTLogger.GetInstance().LogMessage(m_LogMessage);
		
		string lootcrate = rtEvent.GetLootcrateClassName();
		if (lootcrate == "")
			lootcrate = m_DefaultLootcrate;
		vector pos = rtEvent.GetEventLocation().lootcrateCoordinatesXYZ;
		vector orientation = rtEvent.GetEventLocation().lootcrateOrientationYPR;
		
		RTLootcrate_Base crate;
		if (RTLootcrate_Base.CastTo(crate, GetGame().CreateObject(lootcrate, pos, ECE_LOCAL | ECE_KEEPHEIGHT)))
		{
			crate.SetOrientation(orientation);
			array<ref RTLoot> loot = rtEvent.GetEventLocation().loot;
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