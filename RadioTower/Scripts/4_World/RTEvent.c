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
	}
	
	void RTEvent()
	{
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
		int tries = 0;
        while (spawnedZombies < count)
        {
			// Prevent infinite loop
			if (tries > 10000)
			{
				Print("[RadioTower] Not enough space for spawning zombies, reduce zombie count!");
				RTLogger.GetInstance().LogMessage("[RadioTower] Not enough space for spawning zombies, reduce zombie count!");
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
					Print("[RadioTower] Error spawning zombie, check that " + zombieClassname + " is a valid classname!");
					RTLogger.GetInstance().LogMessage("[RadioTower] Error spawning zombie, check that " + zombieClassname + " is a valid classname!");
				}
				spawnedZombies++;
			}	
			tries++;
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
					vehicle.Fill(CarFluid.COOLANT, 1000);
				}
			}
		}
	}
	
	//void SpawnLoot(RTLootcrate_Base target)
	void SpawnLoot(EntityAI target)
	{
		TStringIntMap lootedCountCategoryMap = new TStringIntMap();
		
		array<ref RTLootCategory> categories = m_EventLocation.loot.lootCategories;
	    float totalCategoriesProbability = m_EventLocation.loot.GetTotalCategoriesProbability();
		int lootCount = m_EventLocation.loot.lootCount;
		int totalLimit = m_EventLocation.loot.GetTotalCategoriesLimit();
		
		if (lootCount > totalLimit)
		{
			lootCount = totalLimit;
		}
		
		// Init lootedCount to 0 for all categories
		for (int m = 0; m < categories.Count(); m++)
		{
			RTLootCategory cat = categories[m];
			lootedCountCategoryMap.Insert(cat.lootCategoryTitle, 0);
		}
		
	    for (int i = 0; i < lootCount; i++)
	    {
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
					//int lootedCount = category.lootedCount;
					int lootedCount = lootedCountCategoryMap.Get(category.lootCategoryTitle);
					
					if(lootLimit > 0 && lootedCount >= lootLimit)
					{
						i--;
						break;
					}
					
					//category.lootedCount++;
					lootedCountCategoryMap.Set(category.lootCategoryTitle, lootedCount++);
	
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
							
							for (int l = 0; l < quantity; l++)
							{
								RTLogger.GetInstance().LogMessage("[Item] " + itemClassName);
								entity = target.GetInventory().CreateEntityInCargo(itemClassName);
								ItemBase ingameItem = ItemBase.Cast(entity);
								//Print("Spawning item: " + ingameItem.ClassName());
								if (ingameItem.HasQuantity())
								{
									if (item.hasRandomQuantity)
									{
										quantity = Math.RandomInt(1, quantity);
									}
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