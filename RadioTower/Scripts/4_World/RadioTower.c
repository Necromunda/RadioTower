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
	protected ref array<EntityAI> m_Zombies;
	protected EntityAI m_Lootcrate;
	
#ifdef LBmaster_Groups
	protected ref LBServerMarker m_LBMapMarker;
#endif
	
	void ~RTEvent()
	{
		//Print("[RadioTower] RTEvent dtor");
	}
	
	void RTEvent()
	{
		//Print("[RadioTower] RTEvent ctor");
		
		m_State = RTEventState.DELETED;
		m_Server = null;
		m_CaptureArea = null;
		m_EventLocation = null;
		m_EventProps = null;
		m_PropObjects = new array<Object>();
		m_LogMessage = "";
		m_Zombies = new array<EntityAI>();
		m_Lootcrate = null;
#ifdef LBmaster_Groups
		m_LBMapMarker = null;
#endif
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
		
		for (int j = 0; j < m_Zombies.Count(); j++)
		{
			if (m_Zombies[j])
				GetGame().ObjectDelete(m_Zombies[j]);
		}
		
		m_CaptureArea = null;
		m_Server = null;
#ifdef LBmaster_Groups
		RemoveLBMapMarker();
		m_LBMapMarker = null;
#endif
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
			string lootcrateClassName = GetLootcrateClassName();
			
			CleanUp();
			
			GetGame().GetObjectsAtPosition(lootcratePosition, 1, objects, null);
			foreach(Object crateObj: objects)
			{
				//if (crateObj.IsKindOf("RTLootcrate_Base"))
				if (crateObj.IsKindOf("Container_Base"))
					GetGame().ObjectDelete(crateObj);
			}
			objects.Clear();
			
			//if (m_Lootcrate)
				//GetGame().ObjectDelete(m_Lootcrate);
			
			GetGame().GetObjectsAtPosition(vehiclePosition, 1, objects, null);
			foreach(Object vehicleObj: objects)
			{
				if (vehicleObj.IsKindOf(vehicleClassname))
					GetGame().ObjectDelete(vehicleObj);
			}
		}
	}
	
	void SpawnZombies(int count, vector centerPos, float radius)
	{
		int spawnedZombies = 0;
        while(spawnedZombies < count)
        {
			float angle = Math.RandomFloat(0, 2 * Math.PI);
			float distance = Math.RandomFloat(0, radius);
			float x = centerPos[0] + distance * Math.Cos(angle);
			float z = centerPos[2] + distance * Math.Sin(angle);
			/*
            float x = Math.RandomFloat(centerPos[0] - radius, centerPos[0] + radius);
            float z = Math.RandomFloat(centerPos[2] - radius, centerPos[2] + radius);
			*/
            float y = GetGame().SurfaceY(x, z);
            vector position = Vector( x, y, z );

            if (g_RTBase.IsSafeSpawnPos(position))
            {
				string zombieClassname = RTConstants.GetRandomZombieClassname();
				ref EntityAI zombie;
				if (EntityAI.CastTo(zombie, GetGame().CreateObject(zombieClassname, position, false, true)))
					m_Zombies.Insert(zombie);
				spawnedZombies++;
			}	
		}
	}
	
	void SpawnEventLootCrate()
	{
		string msg = "Spawning lootcrate in " + GetEventTitle();
		Print("[RadioTower] " + msg);
		m_LogMessage = "[Spawn loot] " + GetEventTitle();
		RTLogger.GetInstance().LogMessage(m_LogMessage);
		
		if (m_EventLocation)
		{
			vector pos = m_EventLocation.lootcrateCoordinatesXYZ;
			vector orientation = m_EventLocation.lootcrateOrientationYPR;
			string lootcrate = m_EventLocation.lootcrateClassName;
			if (!lootcrate || lootcrate == "")
				lootcrate = g_RTBase.GetDefaultLootcrateClassName();
			
			/*
			RTLootcrate_Base crate;
			if (RTLootcrate_Base.CastTo(crate, GetGame().CreateObject(lootcrate, pos, ECE_LOCAL | ECE_KEEPHEIGHT)))
			{
				crate.SetOrientation(orientation);

				SpawnLoot(crate);
			}
			*/
			ref EntityAI crate;
			if (EntityAI.CastTo(crate, GetGame().CreateObject(lootcrate, pos, ECE_LOCAL | ECE_KEEPHEIGHT)))
			{
				m_Lootcrate = crate;
				m_Lootcrate.SetOrientation(orientation);

				SpawnLoot(m_Lootcrate);
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
	
	//void SpawnLoot(RTLootcrate_Base target)
	void SpawnLoot(EntityAI target)
	{
		array<ref RTLootCategory> categories = m_EventLocation.loot.lootCategories;
	    float totalCategoriesProbability = m_EventLocation.loot.GetTotalCategoriesProbability();
		int lootCount = m_EventLocation.loot.lootCount;
		int totalLimit = m_EventLocation.loot.GetTotalCategoriesLimit();
		
		if (lootCount > totalLimit)
		{
			lootCount = totalLimit;
		}
	
	    for (int i = 0; i < lootCount; i++)
	    {
	        float randomValue = Math.RandomFloat(0, totalCategoriesProbability);
	        float cumulativeProbability = 0;
	        EntityAI entity = null;
	
	        // Iterate through each category and its items to determine the spawned item
	        for (int j = 0; j < categories.Count(); j++)
	        {
				RTLootCategory category = categories[j];
	            cumulativeProbability += category.probability;
	
	            if (randomValue <= cumulativeProbability)
	            {
	                array<ref RTLootItem> items = category.items;
					int lootLimit = category.limit;
					int lootedCount = category.lootedCount;
					
					if(lootLimit > 0 && lootedCount >= lootLimit)
					{
						i--;
						break;
					}
					
					category.lootedCount++;
	
	                // Calculate the total probability for items within the category
	                float totalItemProbability = category.GetTotalItemsProbability();
	                float randomItemValue = Math.RandomFloat(0, totalItemProbability);
	                float cumulativeItemProbability = 0;
	
	                // Iterate through the items within the category to determine the spawned item
	                for (int k = 0; k < items.Count(); k++)
	                {
						RTLootItem item = items[k];
	                    cumulativeItemProbability += item.probability;
	
	                    if (randomItemValue <= cumulativeItemProbability)
	                    {
	                        entity = target.GetInventory().CreateEntityInCargo(item.lootItemClassName);
							
							RTLogger.GetInstance().LogMessage("[Item] " + item.lootItemClassName);
							
							if(item.quantity > 1)
							{
								ItemBase ingameItem = ItemBase.Cast(entity);
								int quantity = item.quantity;
								/*if (item.HasRandomQuantity)
								{
									quantity = Math.RandomInt(1, quantity);
								}*/
								ingameItem.SetQuantity(quantity);
							}
	                        SpawnAttachments(entity, item.attachmentCategories);
	                        break;
	                    }
	                }
	                break;
	            }
	        }
	    }
	}
	
	void SpawnAttachments(EntityAI target, ref array<ref RTLootItemAttachmentCategory> attachmentCategories)
	{
		if (attachmentCategories.Count() == 0)
			return;
		
		if (target.GetInventory().GetAttachmentSlotsCount() == 0)
			return;
		
		array<string> spawnedAttachments = {};

		for(int l = 0; l < attachmentCategories.Count(); l++)
		{
			RTLootItemAttachmentCategory category = attachmentCategories[l];
			
			if (category.probability <= 0)
				continue;
			
			float randomCategoryValue = Math.RandomFloat(0, 1);
			
			if (category.probability >= 1 || randomCategoryValue < category.probability)
			{
				float totalItemAttachmentsProbability = category.GetTotalAttachmentsProbability();
				float randomAttachmentValue = Math.RandomFloat(0, totalItemAttachmentsProbability);
				float cumulativeItemAttachmentsProbability = 0;

				for(int m = 0; m < category.attachments.Count(); m++)
				{
					RTLootItemAttachment attachment = category.attachments[m];
					if (attachment.probability < 1)
					{
						cumulativeItemAttachmentsProbability += attachment.probability;
					}
					
					if (attachment.probability >= 1 || randomAttachmentValue <= cumulativeItemAttachmentsProbability)
					{
						if (spawnedAttachments.Find(attachment.attachmentClassName) == -1)
						{
							spawnedAttachments.Insert(attachment.attachmentClassName);
							target.GetInventory().CreateAttachment(attachment.attachmentClassName);
							RTLogger.GetInstance().LogMessage("[Att] " + attachment.attachmentClassName);
						}
						break;
					}
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
		string crate = m_EventLocation.lootcrateClassName;
		if (!crate || crate == "")
			return g_RTBase.GetDefaultLootcrateClassName();
		return crate; 
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
	
#ifdef LBmaster_Groups
	LBServerMarker CreateLBMapMarker(string name, vector position, string icon, int argb, bool toSurface, bool display3D, bool displayMap, bool displayGPS)
	{
		LBServerMarker marker = LBStaticMarkerManager.Get().AddTempServerMarker(name, position, icon, argb, toSurface, display3D, displayMap, displayGPS);

		return marker;
	}
	
	bool RemoveLBMapMarker()
	{	
		bool success = LBStaticMarkerManager.Get().RemoveServerMarker(m_LBMapMarker);

		return success;
	}
	
	void SetLBMapMarker(LBServerMarker markerObject)
	{
		m_LBMapMarker = markerObject;
	}
	
	LBServerMarker GetLBMapMarker()
	{
		return m_LBMapMarker;
	}
#endif
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
		//Print("[RadioTower] RTBase dtor");
	}
	
	void RTBase()
	{
		//Print("[RadioTower] RTBase ctor");
		
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
			Print("[RadioTower] " + m_LogMessage);
			RTLogger.GetInstance().LogMessage("[Warning] " + m_LogMessage);
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
					Print("[RadioTower] " + m_LogMessage);
					RTLogger.GetInstance().LogMessage("[Warning] " + m_LogMessage);
					m_EventSpawnTimer.Stop();
				}
				else
				{
					Print("[RadioTower] Can't create event in " + locationTitle + " because it was the last event");
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
					Print("[RadioTower] Trying to create event but " + pastEventTitle + " is still active!");
					return false;
				}
				
				if (locationTitle == pastEventTitle)
				{
					if (pastEvent.IsCaptureInProgress())
					{
						Print("[RadioTower] Trying to create event but " + pastEventTitle + " is still being captured!");
						return false;
					}
				}
			} 
		}
		
		Print("[RadioTower] Valid event location");
		return true;
	}
	
    void CreateEvent()
    {
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
					Print("[RadioTower] No available event locations");
					return;
				}
				eventLocationIndex = Math.RandomInt(0, eventLocationCount);
			}
			
			eventLocation = RTLocation.Cast(m_Locations.eventLocations[eventLocationIndex]);
			isValid = IsEventLocationValid(eventLocation);
		}
		
		if (m_Settings && !m_Settings.enableConcurrentEvents)
			CleanupPastEvents();
		
		//RTEventType type = eventLocation.spawnGas;
		m_RTEvent = new RTEvent();
		m_RTEvent.SetEventLocation(eventLocation);
		//m_RTEvent.SetEventType(type);
		
		vector position = eventLocation.locationCoordinatesXYZ;
		vector orientation = eventLocation.locationOrientationYPR;
		
		m_RTEvent.CleanUpAll();
		
		RTServer item;
		if (RTServer.CastTo(item, GetGame().CreateObjectEx("RTServer", position, ECE_KEEPHEIGHT)))
		{
			item.SetPosition(position);
			item.SetOrientation(orientation);
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
					m_RTEvent.SpawnProps(locationProps);
				}
			}
		}

		if (m_Settings.spawnZombies)
		{
			int zombieCount = eventLocation.zombieCount;
			float radius = Math.AbsFloat(eventLocation.captureAreaRadius);
			//SpawnZombies(zombieCount, position, radius);
			m_RTEvent.SpawnZombies(zombieCount, position, radius);
		}
		
#ifdef LBmaster_Groups
		if (m_Settings.enableLBMapMarker)
		{
			string title = string.Format("KOTH: %1 Event", eventLocation.locationTitle);
			ref LBServerMarker marker = m_RTEvent.CreateLBMapMarker(title, position, "LBmaster_Groups/gui/icons/skull.paa", ARGB(255, 200, 0, 0), false, false, true, true);
			m_RTEvent.SetLBMapMarker(marker);
		}
#endif
		
		m_RTEvent.SetState(RTEventState.ACTIVE);
		m_Events.Insert(m_RTEvent);
		
		RTLogger.GetInstance().LogMessage("[Event created] " + eventLocation.locationTitle);
		if (IsNotificationAllowed(RTNotificationType.CREATE))
		{
			string ingame_msg = "Server has been located in " + eventLocation.locationTitle + "!";
			RTMsgHandler.RTSendChatMessage(ingame_msg);
			RTMsgHandler.RTSendClientAlert(RTConstants.RT_ICON, ingame_msg, 3);
		}
    }
	
	void StartEvent(RTServer server)
	{
		RTEvent rtEvent = GetRTEventWithServer(server);
		
		if (rtEvent)
		{
			rtEvent.SetState(RTEventState.CAPTURING);
			string title = rtEvent.GetEventTitle();
			Print("[RadioTower] Event capture started in " + title);
			RTLogger.GetInstance().LogMessage("[Capture start] " + title);
			CaptureArea trigger = rtEvent.GetEventTrigger();
			rtEvent.GetEventTrigger().SetCapture(true);
		}
	}
	
	/*void SpawnZombies(int count, vector centerPos, float radius)
	{
		int spawnedZombies = 0;
        while(spawnedZombies < count)
        {
			float angle = Math.RandomFloat(0, 2 * Math.PI);
			float distance = Math.RandomFloat(0, radius);
			float x = centerPos[0] + distance * Math.Cos(angle);
			float z = centerPos[2] + distance * Math.Sin(angle);
			
            //float x = Math.RandomFloat(centerPos[0] - radius, centerPos[0] + radius);
            //float z = Math.RandomFloat(centerPos[2] - radius, centerPos[2] + radius);
			
            float y = GetGame().SurfaceY(x, z);
            vector position = Vector( x, y, z );

            if (IsSafeSpawnPos(position))
            {
				spawnedZombies++;
				GetGame().CreateObject("ZmbM_PatientSkinny", position, false, true);
			}	
		}
	}
	*/
	
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
        if(is_interior == 1)
            return false;

        float friction = GetGame().ConfigGetFloat(cfgSurfacePath + " friction");

        //Invalid if GetFloat(... friction) < 0.94     
        if(friction < 0.94)
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
        if(m_Hit)
            return false;

        return true;
    }
	
	void OnEventCapture(CaptureArea trigger)
	{
		RTEvent rtEvent = GetRTEventWithTrigger(trigger);
		RTServer rtServer = rtEvent.GetEventServer();
		if (rtServer)
		{
			rtServer.SetCaptureStateSynchronized(CaptureState.CAPTURED);
		}
		
		if (rtEvent)
		{
			rtEvent.SetState(RTEventState.CAPTURED);
		
			string title = rtEvent.GetEventTitle();
			RTLogger.GetInstance().LogMessage("[Event captured] " + title);
			if (IsNotificationAllowed(RTNotificationType.CAPTURE))
			{
				string ingame_msg = title + " has been captured!";
				RTMsgHandler.RTSendChatMessage(ingame_msg);
				RTMsgHandler.RTSendClientAlert(RTConstants.RT_ICON, ingame_msg, 3);
			}
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