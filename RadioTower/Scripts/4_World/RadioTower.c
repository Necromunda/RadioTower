enum RTEventState
{
	DELETED,
	ACTIVE,
	CAPTURING,
	CAPTURED
}

enum RTNotificationState
{
	DISABLED = 0,
	ENABLED = 1
	//CHAT = 2,
	//POPUP = 3
}

enum RTNotificationType
{
	CREATE,
	CAPTURE,
	END
}

enum RTEventType
{
	NORMAL = 0,
	GAS = 1
}

class RTEvent
{
	protected RTServer m_Server;
	protected CaptureArea m_CaptureArea;
	protected RTLocation m_EventLocation;
	protected RTLocationProps m_EventProps;
	protected RTEventState m_State;
	protected ref array<Object> m_PropObjects;
	protected string m_LogMessage;
	protected RTEventType m_EventType;
	
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
		m_EventProps = null;
		m_PropObjects = new array<Object>();
		m_LogMessage = "";
	}
	
	// Delete all event objects except lootbox and car
	void CleanUp()
	{
		m_State = RTEventState.DELETED;
		
		if (m_CaptureArea)
			GetGame().ObjectDelete(m_CaptureArea);
		
		if (m_Server)
			GetGame().ObjectDelete(m_Server);
		
		for (int i = 0; i < m_PropObjects.Count(); i++)
		{
			if (m_PropObjects[i])
				GetGame().ObjectDelete(m_PropObjects[i]);
		}
		
		m_CaptureArea = null;
		m_Server = null;
	}
	
	// Delete all event objects
	void CleanUpAll()
	{
		if (m_EventLocation)
		{
			array<Object> objects = new array<Object>;
			vector lootcratePosition = m_EventLocation.lootcrateCoordinatesXYZ;
			vector vehiclePosition = m_EventLocation.vehicleCoordinatesXYZ;
			string vehicleClassname = m_EventLocation.vehicleClassName;
			
			CleanUp();
			
			GetGame().GetObjectsAtPosition(lootcratePosition, 1, objects, null);
			foreach(Object serverObj: objects)
			{
				if (serverObj.IsKindOf("RTLootcrate_Base"))
					GetGame().ObjectDelete(serverObj);
			}
			objects.Clear();
			GetGame().GetObjectsAtPosition(vehiclePosition, 1, objects, null);
			foreach(Object vehicleObj: objects)
			{
				if (vehicleObj.IsKindOf(vehicleClassname))
					GetGame().ObjectDelete(vehicleObj);
			}
		}
	}
	
	void SpawnEventLootCrate()
	{
		string msg = "Spawning lootcrate";
		Print("[RadioTower] " + msg);
		m_LogMessage = GetEventTitle() + ": " + m_LogMessage;
		RTLogger.GetInstance().LogMessage(m_LogMessage);
		
		if (m_EventLocation)
		{
			vector pos = m_EventLocation.lootcrateCoordinatesXYZ;
			vector orientation = m_EventLocation.lootcrateOrientationYPR;
			string lootcrate = m_EventLocation.lootcrateClassName;
			if (!lootcrate || lootcrate == "")
				lootcrate = g_RTBase.GetDefaultLootcrateClassName();;
			
			RTLootcrate_Base crate;
			if (RTLootcrate_Base.CastTo(crate, GetGame().CreateObject(lootcrate, pos, ECE_LOCAL | ECE_KEEPHEIGHT)))
			{
				crate.SetOrientation(orientation);
				array<ref RTLoot> loot = m_EventLocation.loot;
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
			
			string vehicleName = m_EventLocation.vehicleClassName;
			vector vehiclePosition = m_EventLocation.vehicleCoordinatesXYZ;
			vector vehicleOrientation = m_EventLocation.vehicleOrientationYPR;
			TStringArray vehicleAttachments = m_EventLocation.vehicleAttachments;
			float vehicleProbability = Math.Clamp(m_EventLocation.vehicleProbability, 0, 1);
			
			if (Math.RandomFloat(0, 1) <= vehicleProbability)
			{
				Object obj = g_RTBase.SpawnObject(vehicleName, vehiclePosition, vehicleOrientation);
				EntityAI entity = EntityAI.Cast(obj);
				for (int k = 0; k < vehicleAttachments.Count(); k++)
				{
					entity.GetInventory().CreateAttachment(vehicleAttachments[k]);
				}
			}
		}
	}
	
	void SpawnProps(RTLocationProps props)
	{
		for (int i = 0; i < props.locationProps.Count(); i++)
		{
			RTProp prop;
			if (RTProp.CastTo(prop, props.locationProps[i]))
			{
				ref Object obj = g_RTBase.SpawnObject(prop.propClassName, prop.propCoordinatesXYZ, prop.propOrientationYPR);
				m_PropObjects.Insert(obj);
			}
		}
	}
	
	bool IsActive()
	{
		return m_State == RTEventState.ACTIVE || m_State == RTEventState.CAPTURING;
	}
	
	bool IsCaptureInProgress()
	{
		return m_State == RTEventState.CAPTURING;
	}
	
	bool IsDeleted()
	{
		return m_State == RTEventState.DELETED;
	}
	
	bool IsCaptured()
	{
		return m_State == RTEventState.CAPTURED;
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
	
	void SetEventProps(RTLocationProps locationProps)		
	{ 
		m_EventProps = locationProps; 
	}
	
	void SetEventType(RTEventType type)
	{
		m_EventType = type;
	}
	
	RTEventType GetEventType()
	{
		return m_EventType;
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

	ref Timer m_EventSpawnTimer;
	protected ref RTEvent m_RTEvent;
	
	ref array<ref RTEvent> m_Events;
	
	protected string m_DefaultLootcrate;
	protected string m_LogMessage;
	protected bool m_AllowSameEventSpawnInARow;
	
	protected RTNotificationState m_NotificationState;
	
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
		m_NotificationState = RTNotificationState.ENABLED;
		
		m_AllowSameEventSpawnInARow = RTConstants.RT_ALLOW_SAME_EVENT_SPAWN_IN_A_ROW;
		bool enableLogging = RTConstants.RT_ENABLE_LOGGING;
		float spawnInterval = RTConstants.RT_EVENT_SPAWN_INTERVAL_DEFAULT;
		
		if (m_Settings)
		{
			spawnInterval = m_Settings.eventSpawnInterval;
			m_AllowSameEventSpawnInARow = m_Settings.allowSameEventSpawnInARow;
			enableLogging = m_Settings.enableLogging;
			m_DefaultLootcrate = m_Settings.eventDefaultLootcrate;
			m_NotificationState = m_Settings.enableNotifications;
		}
		
		RTLogger.CreateInstance();
		RTLogger.GetInstance().SetCreateLogs(enableLogging);
		
		m_EventSpawnTimer.Run(spawnInterval, this, "CreateEvent", NULL, true);	
	}
	
	void CleanupPastEvents()
	{
		for (int i = 0; i < m_Events.Count(); i++)
		{
			RTEvent rtEvent;
			if (RTEvent.CastTo(rtEvent, m_Events[i]))
				rtEvent.CleanUp();
		}
	}
	
	bool IsNotificationAllowed(RTNotificationType type)
	{
		bool isAllowed = m_NotificationState;
		if (!isAllowed)
			return false;
		
		bool allowCreateNotification = m_Settings.enableEventCreateNotification;
		bool allowCaptureNotification = m_Settings.enableEventCaptureNotification;
		bool allowEndNotification = m_Settings.enableEventEndNotification;
		
		switch (type)
		{
			case RTNotificationType.CREATE:
				isAllowed = allowCreateNotification;
				break;
			case RTNotificationType.CAPTURE:
				isAllowed = allowCaptureNotification;
				break;
			case RTNotificationType.END:
				isAllowed = allowEndNotification;
				break;
		}
		
		return isAllowed;
	}
	
	Object SpawnObject( string type, vector position, vector orientation )
	{
	    auto obj = GetGame().CreateObjectEx( type, position, ECE_CREATEPHYSICS );
	    obj.SetFlags( EntityFlags.STATIC, false );
	    obj.SetPosition( position );
	    obj.SetOrientation( orientation );
	    obj.SetOrientation( obj.GetOrientation() );
	    obj.Update();
	    obj.SetAffectPathgraph( true, false );
	    if( obj.CanAffectPathgraph() ) GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().UpdatePathgraphRegionByObject, 100, false, obj );
		return obj;
	}
	
	string GetDefaultLootcrateClassName()
	{
		return m_DefaultLootcrate;
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
					//Print("Found event with server: " + rtEvent.GetEventTitle());
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
					//Print("Found event with trigger: " + rtEvent.GetEventTitle());
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
		
		// Check if we can't spawn same event in a row
		if (!m_AllowSameEventSpawnInARow)
		{
			RTEvent lastEvent = GetLastRTEvent();
			// Check if new event is the same as last event
			if (lastEvent && lastEvent.GetEventTitle() == locationTitle)
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
		
		for (int i = 0; i < m_Events.Count(); i++)
		{			
			// Check if past event is still active
			RTEvent pastEvent = RTEvent.Cast(m_Events[i]);
			string pastEventTitle = pastEvent.GetEventTitle();
			
			if (!pastEvent.IsDeleted())
			{
				// If allowing multiple events, ignore prioritizeOldEvent
				if (m_Settings && m_Settings.prioritizeOldEvent && !m_Settings.enableConcurrentEvents)
				{
					m_LogMessage = "Trying to create event but " + pastEventTitle + " is still active!";
					return false;
				}
				
				if (locationTitle == pastEventTitle)
				{
					if (pastEvent.IsCaptureInProgress())
					{
						m_LogMessage = "Trying to create event but " + pastEventTitle + " is still being captured!";
						return false;
					}
				}
			} 
			/*else if (pastEvent.IsActive())
			{
				if (m_Settings && m_Settings.prioritizeOldEvent)
				{
					m_LogMessage = "Trying to create event but " + pastEventTitle + " is still active!";
					return false;
				}
			}*/
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
		
		if (m_Settings && !m_Settings.enableConcurrentEvents)
			CleanupPastEvents();
		
		RTEventType type = eventLocation.spawnGas;
		m_RTEvent = new RTEvent();
		m_RTEvent.SetEventLocation(eventLocation);
		m_RTEvent.SetEventType(type);
		
		vector position = eventLocation.locationCoordinatesXYZ;
		vector orientation = eventLocation.locationOrientationYPR;
		/*
		vector lootcratePosition = eventLocation.lootcrateCoordinatesXYZ;
		vector vehiclePosition = eventLocation.vehicleCoordinatesXYZ;
		string vehicleClassname = eventLocation.vehicleClassName;
		
		/*
		array<Object> objects = new array<Object>;
		GetGame().GetObjectsAtPosition(position, 1, objects, null);
		foreach(Object obj: objects)
		{
			if (obj.IsKindOf("RTServer_Base"))
			{
				RTServer server;
				if (RTServer.CastTo(server, obj))
				{
					RTEvent rtEvent = GetRTEventWithServer(server);
					if (rtEvent)
						rtEvent.CleanUp();
				}
				GetGame().ObjectDelete(serverObj);
			}
		}
		objects.Clear();
		GetGame().GetObjectsAtPosition(lootcratePosition, 1, objects, null);
		foreach(Object obj: objects)
		{
			if (obj.IsKindOf("RTLootcrate_Base"))
				GetGame().ObjectDelete(obj);
		}
		objects.Clear();
		GetGame().GetObjectsAtPosition(vehiclePosition, 1, objects, null);
		foreach(Object obj: objects)
		{
			if (obj.IsKindOf(vehicleClassname))
				GetGame().ObjectDelete(obj);
		}
		*/
		
		m_RTEvent.CleanUpAll();
		
		//bool showCaptureStatusSmoke = m_Settings.showCaptureStatusSmoke;
		RTServer item;
		if (RTServer.CastTo(item, GetGame().CreateObjectEx("RTServer", position, ECE_KEEPHEIGHT)))
		{
			item.SetPosition(position);
			item.SetOrientation(orientation);
			//item.SetOrientation(item.GetOrientation());
			item.SetFlags(EntityFlags.STATIC, false);
			item.Update();

			m_RTEvent.SetEventServer(item);
		}
		
		CaptureArea trigger;
		if (CaptureArea.CastTo(trigger, GetGame().CreateObject("CaptureArea", position)))
		{
			float yOffset = eventLocation.captureAreaYAxisOffset;
			vector offset = vector.Zero;
			offset[1] = yOffset;

			trigger.SetPosition(position + offset);
			m_RTEvent.SetEventTrigger(trigger);
		}
		
		for (int i = 0; i < m_Props.eventProps.Count(); i++)
		{
			RTLocationProps locationProps;
			if (RTLocationProps.CastTo(locationProps, m_Props.eventProps[i]))
			{
				if (m_RTEvent.GetEventTitle() == locationProps.locationTitle)
				{
					//m_RTEvent.SetEventProps(locationProps);
					m_RTEvent.SpawnProps(locationProps);
				}
			}
		}
		
		bool spawnZombies = m_Settings.spawnZombies;
		if (spawnZombies)
		{
			int zombieCount = eventLocation.zombieCount;
			
		}
		
		m_RTEvent.SetState(RTEventState.ACTIVE);
		m_Events.Insert(m_RTEvent);
		
		m_LogMessage = "Server has been located in " + eventLocation.locationTitle + "!";
		RTLogger.GetInstance().LogMessage("[Event created] " + eventLocation.locationTitle);
		if (IsNotificationAllowed(RTNotificationType.CREATE))
		{
			RTMsgHandler.RTSendChatMessage(m_LogMessage);
			RTMsgHandler.RTSendClientAlert(RTConstants.RT_ICON, m_LogMessage, 3);
		}
		Print("[RadioTower] Event created! " + m_LogMessage);
    }
	
	void StartEvent(RTServer server)
	{
		RTEvent rtEvent = GetRTEventWithServer(server);
		
		if (rtEvent)
		{
			rtEvent.SetState(RTEventState.CAPTURING);
			m_LogMessage = "Event started in " + rtEvent.GetEventTitle();
			Print("[RadioTower] " + m_LogMessage);
			CaptureArea trigger = rtEvent.GetEventTrigger();
			rtEvent.GetEventTrigger().SetCapture(true);
		}
	}
	
	void OnEventCapture(CaptureArea trigger)
	{
		RTEvent rtEvent = GetRTEventWithTrigger(trigger);
		RTServer rtServer = rtEvent.GetEventServer();
		if (rtServer)
		{
			Print("Server captured, state (should be 1): " + rtServer.GetCaptureState());
			rtServer.SetCaptureStateSynchronized(CaptureState.CAPTURED);
			Print("Server captured, state (should be 2): " + rtServer.GetCaptureState());
		}
		
		if (rtEvent)
		{
			rtEvent.SetState(RTEventState.CAPTURED);
		
			string title = rtEvent.GetEventTitle();
			string msg = title + " has been captured!";
			RTLogger.GetInstance().LogMessage(msg);
			if (IsNotificationAllowed(RTNotificationType.CAPTURE))
			{
				RTMsgHandler.RTSendChatMessage(msg);
				RTMsgHandler.RTSendClientAlert(RTConstants.RT_ICON, msg, 3);
			}
			rtEvent.SpawnEventLootCrate();
			//SpawnEventLootCrate(rtEvent);
		}		
	}
	
	/*
	void SpawnEventLootCrate(RTEvent rtEvent)
	{
		string msg = "Spawning lootcrate";
		Print("[RadioTower] " + msg);
		m_LogMessage = rtEvent.GetEventTitle() + ": " + m_LogMessage;
		RTLogger.GetInstance().LogMessage(m_LogMessage);
		
		RTLocation eventLocation = rtEvent.GetEventLocation();
		
		vector pos = eventLocation.lootcrateCoordinatesXYZ;
		vector orientation = eventLocation.lootcrateOrientationYPR;
		string lootcrate = rtEvent.GetLootcrateClassName();
		if (lootcrate == "")
			lootcrate = m_DefaultLootcrate;
		
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
		
		string vehicleName = eventLocation.vehicleClassName;
		vector vehiclePosition = eventLocation.vehicleCoordinatesXYZ;
		vector vehicleOrientation = eventLocation.vehicleOrientationYPR;
		TStringArray vehicleAttachments = eventLocation.vehicleAttachments;
		float vehicleProbability = Math.Clamp(eventLocation.vehicleProbability, 0, 1);
		
		if (Math.RandomFloat(0, 1) <= vehicleProbability)
		{
			Object obj = SpawnObject(vehicleName, vehiclePosition, vehicleOrientation);
			EntityAI entity = EntityAI.Cast(obj);
			for (int k = 0; k < vehicleAttachments.Count(); k++)
			{
				entity.GetInventory().CreateAttachment(vehicleAttachments[k]);
			}
		}
	}
	*/
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