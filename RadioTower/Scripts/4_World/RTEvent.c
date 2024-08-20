class RTEvent
{
	protected RTServer m_Server;
	protected CaptureArea m_CaptureArea;
	protected CaptureAreaGas m_CaptureAreaGas;
	protected RTLocation m_EventLocation;
	
	protected RTEventState m_State;
	protected ref array<Object> m_PropObjects;
	protected RTEventType m_EventType;
	protected ref array<EntityAI> m_Zombies;
	protected EntityAI m_Lootcrate;
	protected bool m_IsGasEvent;
	
	#ifdef LBmaster_Groups
	protected ref LBServerMarker m_LBMapMarker;
	#endif
	
	#ifdef EXPANSIONMODNAVIGATION
    private ExpansionMarkerModule m_MarkerModule;
    private ExpansionMarkerData m_ExpansionMapMarker;
	#endif
	
	void RTEvent(RTLocation _location, bool _isGasEvent = false)
	{
		m_State = RTEventState.DELETED;
		m_EventLocation = _location;
		m_IsGasEvent = _isGasEvent;
		m_PropObjects = new array<Object>();
		m_Zombies = new array<EntityAI>();
		
		vector position = _location.locationCoordinatesXYZ;
		vector orientation = _location.locationOrientationYPR;
		float yOffset = _location.captureAreaYAxisOffset;
		vector offset = vector.Zero;
		offset[1] = yOffset;
		
		CleanUpAll();
		
		RTServer item;
		if (RTServer.CastTo(item, GetGame().CreateObjectEx("RTServer", position, ECE_KEEPHEIGHT)))
		{
			item.SetAllowDamage(false);
			item.SetPosition(position);
			item.SetOrientation(orientation);
			item.SetFlags(EntityFlags.STATIC, false);
			item.Update();

			m_Server = item;
		}
		
		CaptureArea trigger;
		if (CaptureArea.CastTo(trigger, GetGame().CreateObject("CaptureArea", position)))
		{
			trigger.SetPosition(position + offset);
			trigger.SetCollisionCylinder(_location.captureAreaRadius, _location.captureAreaHeight);
						
			m_CaptureArea = trigger;
		}
	
		/*
		CaptureAreaGas gasArea;
		if (CaptureArea.CastTo(trigger, GetGame().CreateObject("CaptureAreaGas", position)))
		{
			m_RTEvent.SetEventGasArea(gasArea);
		}
		*/
		
		SpawnProps(g_RTBase.m_Props.GetPropsByLocationId(_location.id));

		if (g_RTBase.m_Settings.kothEvent.spawnZombies)
		{
			int zombieCount = _location.zombieCount;
			float radius = Math.AbsFloat(_location.captureAreaRadius);
			SpawnZombies(zombieCount, position, radius);
		}
		
		string mapMarkerText = g_RTBase.m_Settings.mapMarkers.mapMarkerText;
		if (mapMarkerText.Contains("%1"))
		{
			mapMarkerText = string.Format(mapMarkerText, _location.locationTitle);
		}
		#ifdef LBmaster_Groups
		if (g_RTBase.m_Settings.mapMarkers.enableLBMapMarker)
		{
			CreateLBMapMarker(mapMarkerText, position, "LBmaster_Groups/gui/icons/skull.paa", ARGB(255, 200, 0, 0), false, true, true, true);
		}
		#endif
		
		#ifdef EXPANSIONMODNAVIGATION
		if (g_RTBase.m_Settings.mapMarkers.enableExpansionMapMarker)
		{
			CreateMissionMarker(mapMarkerText, position, 0);
		}
		#endif
		
		SetState(RTEventState.ACTIVE);
	}
	
	// Delete all event objects except lootbox and car
	void CleanUp()
	{
		m_State = RTEventState.DELETED;
		
		if (m_CaptureArea)
			GetGame().ObjectDelete(m_CaptureArea);
		
		if (m_CaptureAreaGas)
			GetGame().ObjectDelete(m_CaptureAreaGas);
		
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
		m_CaptureAreaGas = null;
		m_Server = null;
		#ifdef LBmaster_Groups
		RemoveLBMapMarker();
		m_LBMapMarker = null;
		#endif
		#ifdef EXPANSIONMODNAVIGATION
		if (m_ExpansionMapMarker)
			RemoveMissionMarker(m_ExpansionMapMarker.GetUID());
		m_ExpansionMapMarker = null;
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
				if (crateObj.IsKindOf("Container_Base"))
					GetGame().ObjectDelete(crateObj);
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
	
	void SpawnZombies(int count, vector centerPos, float radius)
	{
		int spawnedZombies = 0;
		int tries = 0;
        while (spawnedZombies < count)
        {
			// Prevent infinite loop
			if (tries > 10000)
			{
				g_RTBase.Log(RTLogType.WARNING, "Not enough space, " + spawnedZombies + "/" + count + " zombies spawned");
				break;			
			}
			
			float angle = Math.RandomFloat(0, 2 * Math.PI);
			float distance = Math.RandomFloat(0, radius);
			float x = centerPos[0] + distance * Math.Cos(angle);
			float z = centerPos[2] + distance * Math.Sin(angle);
			/*
            float x = Math.RandomFloat(centerPos[0] - radius, centerPos[0] + radius);
            float z = Math.RandomFloat(centerPos[2] - radius, centerPos[2] + radius);
			*/
            float y = GetGame().SurfaceY(x, z);
            vector position = Vector(x, y, z);

            if (g_RTBase.IsSafeSpawnPos(position))
            {
				string zombieClassname;
				if (m_EventLocation.zombies)
				{
					zombieClassname = m_EventLocation.GetRandomZombieClassname();
					//Print("[RadioTower] Location has overriden zombies, spawning " + zombieClassname);
				}
				if (zombieClassname == "")
				{
					zombieClassname = RTConstants.GetRandomZombieClassname();
					//Print("[RadioTower] Location uses default zombies, spawning " + zombieClassname);
				}
				ref EntityAI zombie;
				if (EntityAI.CastTo(zombie, GetGame().CreateObject(zombieClassname, position, false, true)))
				{
					m_Zombies.Insert(zombie);
				}
				else
				{
					g_RTBase.Log(RTLogType.ERROR, "Error spawning zombie, check that " + zombieClassname + " is a valid classname!");
				}
				spawnedZombies++;
				tries = 0;
			}	
			tries++;
		}
	}
	
	EntityAI CreateInInventory(notnull EntityAI container, string type)
	{
		EntityAI item = GetGame().CreateObject(type, container.GetPosition());
		if (container.GetInventory().CanAddEntityInto(item))
		{
			container.GetInventory().TakeEntityToCargo(InventoryMode.SERVER, item);
			return item;
		}
		else
		{
			if (item)
				GetGame().ObjectDelete(item);
		}
		
		return null;
	}
	
	void SpawnEventLootCrate()
	{
		g_RTBase.Log(RTLogType.INFO, "Spawning lootcrate in " + GetEventTitle());
		
		if (m_EventLocation)
		{
			vector pos = m_EventLocation.lootcrateCoordinatesXYZ;
			vector orientation = m_EventLocation.lootcrateOrientationYPR;
			string lootcrate = m_EventLocation.lootcrateClassName;
			if (!lootcrate || lootcrate == "")
				lootcrate = g_RTBase.GetDefaultLootcrateClassName();

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
			
			if (vehicleName != "")
			{
				if (Math.RandomFloat(0, 1) <= vehicleProbability)
				{
					Object obj = g_RTBase.SpawnObject(vehicleName, vehiclePosition, vehicleOrientation);
					EntityAI carAI = EntityAI.Cast(obj);
					
					for (int k = 0; k < vehicleAttachments.Count(); k++)
					{
						carAI.GetInventory().CreateAttachment(vehicleAttachments[k]);
					}
					
					CarScript vehicle = CarScript.Cast(carAI);
					vehicle.Fill(CarFluid.FUEL, 200);
					if (vehicleAttachments.Find("CarRadiator") != -1)
						vehicle.Fill(CarFluid.COOLANT, 1000);
				}
			}
		}
	}
	
	void SpawnLoot(EntityAI target)
	{
		/*
		if (g_RTBase.m_Settings.kothEvent.useLootSets)
		{
			SpawnLootSets(target);
			return;
		}
		*/
		
		SpawnLootSets(target);
			
		TStringIntMap lootedCountCategoryMap = new TStringIntMap();
		
		array<ref RTLootCategory> categories = m_EventLocation.loot.lootCategories;
	    float totalCategoriesProbability = m_EventLocation.loot.GetTotalCategoriesProbability();
		int lootCount = m_EventLocation.loot.lootCount;
		int totalLimit = m_EventLocation.loot.GetTotalCategoriesLimit();
		
		// Init lootedCount to 0 for all categories
		for (int m = 0; m < categories.Count(); m++)
		{
			RTLootCategory cat = categories[m];
			lootedCountCategoryMap.Insert(cat.lootCategoryTitle, 0);
			
			if (categories[m].limit < 0)
				totalLimit = lootCount;
		}
		
		lootCount = Math.Clamp(lootCount, 0, totalLimit);
		
		int totalTries = 0;
		
	    for (int i = 0; i < lootCount; i++)
	    {
			if (totalTries > 1000)
			{
				g_RTBase.Log(RTLogType.DEBUG, "Loot spawning stopped");
				break;
			}
			else
			{
				totalTries++;
			}
			
	        float randomValue = Math.RandomFloat(0, totalCategoriesProbability);
	        float cumulativeProbability = 0;
	        EntityAI entity = null;
	
	        // Iterate through each category and pick one
	        for (int j = 0; j < categories.Count(); j++)
	        {
				RTLootCategory category = categories[j];
	            cumulativeProbability += category.probability;
	
	            if (randomValue <= cumulativeProbability)
	            {
	                array<ref RTLootItem> items = category.items;
					int lootLimit = category.limit;
					int lootedCount = lootedCountCategoryMap.Get(category.lootCategoryTitle);
					
					// Pick new loot category if limit is 0 or enough items has already been spawned and limit is not unlimited == -1
					if (lootLimit == 0 || (lootedCount >= lootLimit && lootLimit >= 0))
					{
						i--;
						break;
					}
					
					//lootedCount = lootedCount + 1;
					//lootedCountCategoryMap.Set(category.lootCategoryTitle, lootedCount);
					//Print("Spawn item, Key: " + category.lootCategoryTitle + ", value: " + lootedCount);
	
	                // Calculate the total probability for items within the category
	                float totalItemProbability = category.GetTotalItemsProbability();
	                float randomItemValue = Math.RandomFloat(0, totalItemProbability);
	                float cumulativeItemProbability = 0;
	
	                // Iterate through the items within the category and pick one
					for (int k = 0; k < items.Count(); k++)
	                {
						RTLootItem item = items[k];
	                    cumulativeItemProbability += item.probability;
	
	                    if (randomItemValue <= cumulativeItemProbability)
	                    {
							int quantity = item.quantity;
							string itemClassName = item.lootItemClassName;
							bool useMaxQuantity = item.quantity == -1;
							int itemSpawnCount = 1;
							if (!useMaxQuantity)
								itemSpawnCount = item.quantity;
							
							for (int l = 0; l < itemSpawnCount; l++)
							{
								g_RTBase.Log(RTLogType.INFO, "Spawning item " + itemClassName);
								
								entity = CreateInInventory(target, itemClassName);
								if (!entity)
								{
									g_RTBase.Log(RTLogType.DEBUG, "No space for item " + itemClassName);
									j--;
									break;
								}
								
								lootedCount = lootedCount + 1;
								lootedCountCategoryMap.Set(category.lootCategoryTitle, lootedCount);
																
								ItemBase ingameItem = ItemBase.Cast(entity);
								if (!useMaxQuantity && ingameItem.HasQuantity())
								{
									if (item.hasRandomQuantity)
									{
										quantity = Math.RandomInt(1, quantity);
									}
									
									if (ingameItem.HasEnergyManager())
										ingameItem.GetCompEM().SetEnergy(quantity);
									else
										ingameItem.SetQuantity(Math.Clamp(quantity, ingameItem.GetQuantityMin(), ingameItem.GetQuantityMax()));
									
									break;
								}
								
								SpawnAttachments(entity, item.attachmentCategories);
							}
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

		for (int i = 0; i < attachmentCategories.Count(); i++)
		{
			RTLootItemAttachmentCategory category = attachmentCategories[i];
			
			if (category.probability <= 0)
				continue;
			
			float randomCategoryValue = Math.RandomFloat(0, 1);
			
			if (category.probability >= 1 || randomCategoryValue < category.probability)
			{
				float totalItemAttachmentsProbability = category.GetTotalAttachmentsProbability();
				float randomAttachmentValue = Math.RandomFloat(0, totalItemAttachmentsProbability);
				float cumulativeItemAttachmentsProbability = 0;

				for (int j = 0; j < category.attachments.Count(); j++)
				{
					RTLootItemAttachment attachment = category.attachments[j];
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
							g_RTBase.Log(RTLogType.INFO, "Spawning attachment " + attachment.attachmentClassName);
						}
						break;
					}
				}
			}
		}
	}
	
	void CreateLootSetItemsInContainer(EntityAI target, RTLootSet lootSet)
	{
		foreach (RTLootSetItem lootSetItem : lootSet.items)
		{
			//EntityAI entity = target.GetInventory().CreateEntityInCargo(lootSetItem.name);
			g_RTBase.Log(RTLogType.INFO, "Spawning lootset item " + lootSetItem.name);
			//EntityAI entity = target.GetInventory().CreateInInventory(lootSetItem.name);
			EntityAI entity = CreateInInventory(target, lootSetItem.name);
			if (!entity)
			{
				g_RTBase.Log(RTLogType.DEBUG, "No space for lootset item " + lootSetItem.name);
				continue;
			}

			ItemBase ingameItem = ItemBase.Cast(entity);
			if (lootSetItem.quantity != -1)
			{
				if (ingameItem.IsMagazine() || ingameItem.IsAmmoPile())
				{
					Magazine_Base ammo = Magazine_Base.Cast(entity);
					ammo.ServerSetAmmoCount(lootSetItem.quantity);
				}
				else
				{
					if (ingameItem.HasEnergyManager())
					{
						ingameItem.GetCompEM().SetEnergy(lootSetItem.quantity);
					}
					else
					{
						ingameItem.SetQuantity(lootSetItem.quantity);
					}
				}
			}

			SpawnLootSetItemAttachments(entity, lootSetItem);
			SpawnLootSetItemCargo(entity, lootSetItem);
		}
	}
	
	void SpawnLootSets(EntityAI target)
	{
		// First spawn guaranteed sets, regardless of setCount
		foreach (RTLootSetMap lootSetMap : m_EventLocation.loot.lootSets.sets)
		{
			RTLootSet lootSet = g_RTBase.GetRTLootSet(lootSetMap.name);
			if (lootSet && lootSetMap.isGuaranteed)
			{
				CreateLootSetItemsInContainer(target, lootSet);
			}
		}
		
		// After guaranteed sets, spawn setCount amount of lootSets
		if (m_EventLocation.loot.lootSets.sets.Count() > 0)
		{
			for (int i = 0; i < m_EventLocation.loot.lootSets.setCount; i++)
			{
				RTLootSetMap lsMap = m_EventLocation.loot.lootSets.sets.GetRandomElement();
				RTLootSet ls = g_RTBase.GetRTLootSet(lsMap.name);
				if (ls)
				{
					CreateLootSetItemsInContainer(target, ls);
				}
			}
		}
	}
	
	/*
	void SpawnLootSets(EntityAI target)
	{
		foreach (string lootSetName : m_EventLocation.lootSets)
		{
			RTLootSet lootSet = g_RTBase.GetRTLootSet(lootSetName);
			if (lootSet)
			{
				foreach (RTLootSetItem lootSetItem : lootSet.items)
				{
					//EntityAI entity = target.GetInventory().CreateEntityInCargo(lootSetItem.name);
					g_RTBase.Log(RTLogType.INFO, "Spawning item " + lootSetItem.name);
					//EntityAI entity = target.GetInventory().CreateInInventory(lootSetItem.name);
					EntityAI entity = CreateInInventory(target, lootSetItem.name);
					if (!entity)
					{
						g_RTBase.Log(RTLogType.DEBUG, "No space for item " + lootSetItem.name);
						continue;
					}

					ItemBase ingameItem = ItemBase.Cast(entity);
					if (lootSetItem.quantity != -1)
					{
						if (ingameItem.IsMagazine() || ingameItem.IsAmmoPile())
						{
							Magazine_Base ammo = Magazine_Base.Cast(entity);
							ammo.ServerSetAmmoCount(lootSetItem.quantity);
						}
						else
						{
							if (ingameItem.HasEnergyManager())
							{
								ingameItem.GetCompEM().SetEnergy(lootSetItem.quantity);
							}
							else
							{
								ingameItem.SetQuantity(lootSetItem.quantity);
							}
						}
					}

					SpawnLootSetItemAttachments(entity, lootSetItem);
					SpawnLootSetItemCargo(entity, lootSetItem);
				}
			}
		}
	}
	*/
	
	void SpawnLootSetItemAttachments(EntityAI target, RTLootSetItem lootSetItem)
	{		
		foreach (RTLootSetItem lootSetItemAttachment : lootSetItem.attachments)
		{
			g_RTBase.Log(RTLogType.INFO, "Spawning attachment " + lootSetItemAttachment.name);
			EntityAI attachment = target.GetInventory().CreateAttachment(lootSetItemAttachment.name);
			ItemBase ingameItem = ItemBase.Cast(attachment);
					
			if (lootSetItemAttachment.quantity != -1)
			{
				if (ingameItem.IsMagazine() || ingameItem.IsAmmoPile())
				{
					Magazine_Base ammo = Magazine_Base.Cast(attachment);
					ammo.ServerSetAmmoCount(lootSetItemAttachment.quantity);
				}
				else
				{
					if (ingameItem.HasEnergyManager())
					{
						//ingameItem.SetQuantity(lootSetItemAttachment.quantity);
						ingameItem.GetCompEM().SetEnergy(lootSetItemAttachment.quantity);
					}
					else
					{
						ingameItem.SetQuantity(lootSetItemAttachment.quantity);
					}
				}
			}
			SpawnLootSetItemAttachments(attachment, lootSetItemAttachment);
			SpawnLootSetItemCargo(attachment, lootSetItemAttachment);
		}
	}
	
	void SpawnLootSetItemCargo(EntityAI target, RTLootSetItem lootSetItem)
	{		
		foreach (RTLootSetItem lootSetItemCargoItem : lootSetItem.cargo)
		{
			g_RTBase.Log(RTLogType.INFO, "Spawning item in cargo " + lootSetItemCargoItem.name);
			EntityAI cargoItem = target.GetInventory().CreateEntityInCargo(lootSetItemCargoItem.name);
			ItemBase ingameItem = ItemBase.Cast(cargoItem);
					
			if (lootSetItemCargoItem.quantity != -1)
			{
				if (ingameItem.IsMagazine() || ingameItem.IsAmmoPile())
				{
					Magazine_Base ammo = Magazine_Base.Cast(cargoItem);
					ammo.ServerSetAmmoCount(lootSetItemCargoItem.quantity);
				}
				else
				{
					if (ingameItem.HasEnergyManager())
					{
						ingameItem.GetCompEM().SetEnergy(lootSetItemCargoItem.quantity);
					}
					else
					{
						ingameItem.SetQuantity(lootSetItemCargoItem.quantity);
					}
				}
			}
			SpawnLootSetItemCargo(cargoItem, lootSetItemCargoItem);
		}
	}
	
	void SpawnProps(RTLocationProps props)
	{
		if (!props)
			return;
		
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
	
	void SetEventGasArea(CaptureAreaGas gasArea)		
	{ 
		m_CaptureAreaGas = gasArea; 
	}
	
	void SetEventLocation(RTLocation location)		
	{ 
		m_EventLocation = location; 
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
	
	CaptureAreaGas GetEventGasArea() 					
	{ 
		return m_CaptureAreaGas;
	}
	
	RTLocation GetEventLocation()
	{ 
		return m_EventLocation; 
	}
	
	string GetEventLocationId()
	{
		return m_EventLocation.id;
	}
	
	#ifdef LBmaster_Groups
	/*
	LBServerMarker CreateLBMapMarker(string name, vector position, string icon, int argb, bool toSurface, bool display3D, bool displayMap, bool displayGPS)
	{
		if (g_RTBase.m_Settings.mapMarkers.enableLBMapMarker)
		{
			//LBServerMarker marker = LBStaticMarkerManager.Get().AddTempServerMarker(name, position, icon, argb, toSurface, display3D, displayMap, displayGPS);
			LBServerMarker marker = LBStaticMarkerManager.Get.AddTempServerMarker(name, position, icon, argb, toSurface, display3D, displayMap, displayGPS);
	
			return marker;
		}
		return null;
	}
	*/
	
	void CreateLBMapMarker(string name, vector position, string icon, int argb, bool toSurface, bool display3D, bool displayMap, bool displayGPS)
	{
		m_LBMapMarker = LBStaticMarkerManager.Get.AddTempServerMarker(name, position, icon, argb, toSurface, display3D, displayMap, displayGPS);
		m_LBMapMarker.SetRadius(Math.AbsFloat(m_EventLocation.captureAreaRadius), 255,200,0,0, false);
	}
	
	bool RemoveLBMapMarker()
	{	
		if (m_LBMapMarker)
			return LBStaticMarkerManager.Get.RemoveServerMarker(m_LBMapMarker);
		
		return false;
	}
	
	/*
	void SetLBMapMarker(LBServerMarker markerObject)
	{
		m_LBMapMarker = markerObject;
		markerObject.SetRadius(Math.AbsFloat(m_EventLocation.captureAreaRadius), 255,200,0,0, false);
	}
	*/
	
	LBServerMarker GetLBMapMarker()
	{
		return m_LBMapMarker;
	}
	#endif
	
    #ifdef EXPANSIONMODNAVIGATION
    void CreateMissionMarker(string markerName, vector location, int timer)
    {
    	if (!m_MarkerModule)
		{
      		initMarkerModule();
			
      		if (!m_MarkerModule) 
	 		{
		        Print("MarkerModule error");
		        return;
      		}
    	}
        m_ExpansionMapMarker = m_MarkerModule.CreateServerMarker(markerName, "Territory", location, ARGB(255, 235, 59, 90), true);
   		//GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( this.RemoveMissionMarker, timer, false, m_ExpansionMapMarker.GetUID());
    }
	
  	void initMarkerModule()
	{
    	CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule);
  	}

    void RemoveMissionMarker(string uid)
    {
        if (!m_ExpansionMapMarker)
            return;
		
        m_MarkerModule.RemoveServerMarker(uid);
    }
	
	ExpansionMarkerData GetMissionMarker()
	{
		return m_ExpansionMapMarker;
	}
    #endif
}