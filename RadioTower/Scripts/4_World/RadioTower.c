class RTBase
{
	protected RTLogger m_Logger;
	protected RTNotificationState m_NotificationState;
	ref RTSettings m_Settings;
	ref RTLocations m_Locations;
	ref RTProps m_Props;
	ref RTLootSets m_LootSets;

	ref Timer m_EventSpawnTimer;
	protected ref RTEvent m_RTEvent;
	
	ref array<ref RTEvent> m_Events;
	
	protected string m_DefaultLootcrate;
	protected bool m_AllowSameEventSpawnInARow;
	protected int m_TimeSinceLastEvent;
	
	void ~RTBase()
	{
		RTLogger.ClearInstance();
	}
	
	void RTBase()
	{
		if (!FileExist(RTConstants.RT_ROOTPATH))
		{
			MakeDirectory(RTConstants.RT_ROOTPATH);
			Print("[RadioTower] Root folder created");
		}
		
		if (!FileExist(RTConstants.RT_LOGPATH))
		{
			MakeDirectory(RTConstants.RT_LOGPATH);
			Print("[RadioTower] Logs folder created");
		}
		
		if (!FileExist(RTConstants.RT_BACKUPPATH))
		{
			MakeDirectory(RTConstants.RT_BACKUPPATH);
			Print("[RadioTower] Backup folder created");
		}
		
		m_Settings 	= RTSettings.Load();
		m_Props 	= RTProps.Load();
		m_Locations = RTLocations.Load();
		m_LootSets 	= RTLootSets.Load();
		
		m_RTEvent = null;
		m_EventSpawnTimer = new Timer;
		m_Events = new array<ref RTEvent>();
		m_NotificationState = m_Settings.notifications.enableNotifications;
		
		m_AllowSameEventSpawnInARow = m_Settings.kothEvent.enableSameEventSpawnInARow;
		m_DefaultLootcrate = m_Settings.kothEvent.defaultLootcrate;
		
		m_Logger = RTLogger.CreateInstance(m_Settings.logging.enableLogging);
		
		foreach (RTLocation loc : m_Locations.eventLocations)
		{
			for (int i = 0; i < loc.loot.lootCategories.Count(); i++)
			{
				for (int j = 1 + i; j < loc.loot.lootCategories.Count(); j++)
				{
					if (loc.loot.lootCategories[i].lootCategoryTitle == loc.loot.lootCategories[j].lootCategoryTitle)
					{
						Log(RTLogType.WARNING, "Duplicate lootCategoryTitle: " + loc.loot.lootCategories[i].lootCategoryTitle + "! Location id: " + loc.id);
						break;
					}
				}
			}
		}
		
		m_EventSpawnTimer.Run(1, this, "Tick", null, true);	
	}
	
	void Tick()
	{
		int min = Math.Max(m_Settings.kothEvent.minTimeBeforeSpawn, 0);
		int max = Math.Max(m_Settings.kothEvent.maxTimeBeforeSpawn, 1);
		
		m_TimeSinceLastEvent += 1;
		
		if (m_TimeSinceLastEvent < min)
		{
			//PrintFormat("m_TimeSinceLastEvent = %1, min = %2, max = %3", m_TimeSinceLastEvent, min, max);
			return;
		}
		
		float rand = Math.RandomFloat01();
		float timeElapsed = m_TimeSinceLastEvent - Math.Max(min, 0);
		float minMaxRange = Math.Max(max - min, 1);
		float spawnProbability = timeElapsed / minMaxRange;
		//PrintFormat("timeElapsed = %1, min = %2, max = %3, rand = %4, spawnProbability = %5", timeElapsed, min, max, rand, spawnProbability);
		if (rand <= spawnProbability)
		{
			m_TimeSinceLastEvent = 0;
			CreateEvent();
		}
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
		
		switch (type)
		{
			case RTNotificationType.CREATE:
				isAllowed = m_Settings.notifications.enableEventCreateNotification;
				break;
			case RTNotificationType.CAPTURE:
				isAllowed = m_Settings.notifications.enableEventCaptureNotification;
				break;
			case RTNotificationType.END:
				isAllowed = m_Settings.notifications.enableEventEndNotification;
				break;
			case RTNotificationType.PLAYER_ENTER:
				isAllowed = m_Settings.notifications.enablePlayerEnterCaptureAreaNotification;
				break;
			case RTNotificationType.PLAYER_LEAVE:
				isAllowed = m_Settings.notifications.enablePlayerLeaveCaptureAreaNotification;
				break;
		}
		
		return isAllowed;
	}
	
	void SendNotification(RTNotificationType type, string message)
	{
		if (IsNotificationAllowed(type))
		{
			RTMsgHandler.RTSendChatMessage(message);
			RTMsgHandler.RTSendClientAlert(RTConstants.RT_ICON, message, 3);
		}
	}
	
	void Log(RTLogType type, string message)
	{
		m_Logger.LogMessage(type, message);
	}
	
	RTLootSet GetRTLootSet(string name)
	{
		foreach (RTLootSet lootSet : m_LootSets.lootSets)
		{
			if (lootSet.name == name)
				return lootSet;
		}
		
		return null;
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
	    if (obj.CanAffectPathgraph()) 
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, obj);
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
		string locationId = location.id;
		Log(RTLogType.DEBUG, "Trying location: " + location.locationTitle);
		
		// Check if we have 0 events
		int eventLocationCount = GetEventLocationCount();
		if (eventLocationCount == 0)
		{
			Log(RTLogType.WARNING, "Possible misconfiguration! RTLocations.json has 0 locations");
			m_EventSpawnTimer.Stop();
			return false;
		}
		
		// Check if we can't spawn same event in a row
		if (!m_AllowSameEventSpawnInARow)
		{
			RTEvent lastEvent = GetLastRTEvent();
			// Check if new event is the same as last event
			if (lastEvent && lastEvent.GetEventLocationId() == locationId)
			{
				if (eventLocationCount == 1)
				{
					Log(RTLogType.WARNING, "Possible misconfiguration! Event could not be created because allowSameEventSpawnInARow = false and RTLocations.json only has one location");
					m_EventSpawnTimer.Stop();
				}
				else
				{
					Log(RTLogType.DEBUG, "Can't create event in " + location.locationTitle + " because it was the last event");
				}
				return false;
			}
		}
		
		for (int i = 0; i < m_Events.Count(); i++)
		{			
			// Check if past event is still active
			RTEvent pastEvent = RTEvent.Cast(m_Events[i]);
			string pastEventTitle = pastEvent.GetEventTitle();
			string pastEventId = pastEvent.GetEventLocationId();
			
			if (!pastEvent.IsDeleted())
			{
				// If allowing multiple events, ignore prioritizeOldEvent
				if (m_Settings && m_Settings.kothEvent.prioritizeOldEvent && !m_Settings.kothEvent.enableConcurrentEvents)
				{
					Log(RTLogType.DEBUG, "Trying to create event but " + pastEventTitle + " is still active!");
					return false;
				}
				
				if (locationId == pastEventId)
				{
					if (pastEvent.IsCaptureInProgress())
					{
						Log(RTLogType.DEBUG, "Trying to create event but " + pastEventTitle + " is still being captured!");
						return false;
					}
				}
			} 
		}
		
		Log(RTLogType.DEBUG, "Valid event location");
		return true;
	}
	
	int GetPlayerCount()
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);
		int playerCount = players.Count();
		return playerCount;
	}
	
    void CreateEvent()
    {
		int playerCount = GetPlayerCount();
		if (playerCount < m_Settings.kothEvent.minPlayerCountForSpawn) 
		{
			Log(RTLogType.DEBUG, "Playercount too low for event spawn. Current: " + playerCount + ", required: " + m_Settings.kothEvent.minPlayerCountForSpawn);
			return;
		}
		
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
					Log(RTLogType.DEBUG, "No available event locations");
					return;
				}
				eventLocationIndex = Math.RandomInt(0, eventLocationCount);
			}
			
			eventLocation = RTLocation.Cast(m_Locations.eventLocations[eventLocationIndex]);
			isValid = IsEventLocationValid(eventLocation);
		}
		
		if (m_Settings && !m_Settings.kothEvent.enableConcurrentEvents)
			CleanupPastEvents();
		
		m_RTEvent = new RTEvent(eventLocation);
		m_Events.Insert(m_RTEvent);
		
		/*
		vector position = eventLocation.locationCoordinatesXYZ;
		vector orientation = eventLocation.locationOrientationYPR;
		float yOffset = eventLocation.captureAreaYAxisOffset;
		vector offset = vector.Zero;
		offset[1] = yOffset;
		
		m_RTEvent.CleanUpAll();
		
		RTServer item;
		if (RTServer.CastTo(item, GetGame().CreateObjectEx("RTServer", position, ECE_KEEPHEIGHT)))
		{
			item.SetAllowDamage(false);
			item.SetPosition(position);
			item.SetOrientation(orientation);
			item.SetFlags(EntityFlags.STATIC, false);
			item.Update();

			m_RTEvent.SetEventServer(item);
		}
		
		CaptureArea trigger;
		if (CaptureArea.CastTo(trigger, GetGame().CreateObject("CaptureArea", position)))
		{
			trigger.SetPosition(position + offset);
			m_RTEvent.SetEventTrigger(trigger);
		}
	
		
		CaptureAreaGas gasArea;
		if (CaptureArea.CastTo(trigger, GetGame().CreateObject("CaptureAreaGas", position)))
		{
			m_RTEvent.SetEventGasArea(gasArea);
		}
		
		string locationId = m_RTEvent.GetEventLocationId();
		if (locationId != "")
		{
			for (int i = 0; i < m_Props.eventProps.Count(); i++)
			{
				RTLocationProps locationProps;
				if (RTLocationProps.CastTo(locationProps, m_Props.eventProps[i]))
				{
					if (locationId == locationProps.locationId)
					{
						m_RTEvent.SpawnProps(locationProps);
					}
				}
			}
		}

		if (m_Settings.kothEvent.spawnZombies)
		{
			int zombieCount = eventLocation.zombieCount;
			float radius = Math.AbsFloat(eventLocation.captureAreaRadius);
			m_RTEvent.SpawnZombies(zombieCount, position, radius);
		}
		
		string mapMarkerText = m_Settings.mapMarkers.mapMarkerText;
		if (mapMarkerText.Contains("%1"))
		{
			mapMarkerText = string.Format(mapMarkerText, eventLocation.locationTitle);
		}
		#ifdef LBmaster_Groups
		if (m_Settings.mapMarkers.enableLBMapMarker)
		{
			//ref LBServerMarker marker = m_RTEvent.CreateLBMapMarker(mapMarkerText, position, "LBmaster_Groups/gui/icons/skull.paa", ARGB(255, 200, 0, 0), false, false, true, true);
			//m_RTEvent.SetLBMapMarker(marker);
			m_RTEvent.CreateLBMapMarker(mapMarkerText, position, "LBmaster_Groups/gui/icons/skull.paa", ARGB(255, 200, 0, 0), false, true, true, true);
		}
		#endif
		
		#ifdef EXPANSIONMODNAVIGATION
		if (m_Settings.mapMarkers.enableExpansionMapMarker)
		{
			m_RTEvent.CreateMissionMarker(mapMarkerText, position, 0);
		}
		#endif
		
		m_RTEvent.SetState(RTEventState.ACTIVE);
		m_Events.Insert(m_RTEvent);
		*/
		
		Log(RTLogType.INFO, "Event created in " + eventLocation.locationTitle);
		SendNotification(RTNotificationType.CREATE, eventLocation.createdNotificationTitle);
		
		GetRPCManager().SendRPC("RadioTower", "ClientSetLatestEventLocation", new Param1<RTLocation>(eventLocation), true, null);
	}
	
	void StartEvent(RTServer server)
	{
		RTEvent rtEvent = GetRTEventWithServer(server);
		
		if (rtEvent)
		{
			rtEvent.SetState(RTEventState.CAPTURING);
			Log(RTLogType.INFO, "Event capture started in " + rtEvent.GetEventTitle());
			rtEvent.GetEventTrigger().SetCapture(true);
		}
	}
	
	bool IsSafeSpawnPos(vector pos)
    {
        float x = pos[0];
        float z = pos[2];

        if (GetGame().SurfaceIsSea(x, z))
            return false;
		
        if (GetGame().SurfaceIsPond(x, z))
            return false;

        string surface_type;
        GetGame().SurfaceGetType(x, z, surface_type);

        string cfgSurfacePath = "CfgSurfaces " + surface_type;
        int is_interior = GetGame().ConfigGetInt(cfgSurfacePath + " interior");

         //Invalid if GetInt(CfgSurfaces >> surface_type >> interior) == 1
        if (is_interior == 1)
            return false;

        float friction = GetGame().ConfigGetFloat(cfgSurfacePath + " friction");

        //Invalid if GetFloat(... friction) < 0.94     
        if (friction < 0.94)
            return false;

        vector start = pos;
        vector end = pos + Vector( 0, 1, 0 );
        float radius = 2.0; 
        PhxInteractionLayers collisionLayerMask = PhxInteractionLayers.VEHICLE|PhxInteractionLayers.BUILDING|PhxInteractionLayers.DOOR|PhxInteractionLayers.ITEM_LARGE|PhxInteractionLayers.FENCE;
        Object m_HitObject;
        vector m_HitPosition;
        vector m_HitNormal;
        float m_HitFraction;

        //check if safe from object collisions using same raycast as players tp
        bool m_Hit = DayZPhysics.SphereCastBullet( start, end, radius, collisionLayerMask, NULL, m_HitObject, m_HitPosition, m_HitNormal, m_HitFraction );
        if (m_Hit)
            return false;

        return true;
    }
	
	void OnEventCapture(CaptureArea trigger)
	{
		RTEvent rtEvent = GetRTEventWithTrigger(trigger);
		if (rtEvent)
		{
			RTServer rtServer = rtEvent.GetEventServer();
			if (rtServer)
			{
				rtServer.SetCaptureStateSynchronized(CaptureState.CAPTURED);
			}
			rtEvent.SetState(RTEventState.CAPTURED);
		
			RTLocation loc = rtEvent.GetEventLocation();
			Log(RTLogType.INFO, "Event captured in " + loc.locationTitle);
			SendNotification(RTNotificationType.CAPTURE, loc.capturedNotificationTitle);
			rtEvent.SpawnEventLootCrate();
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