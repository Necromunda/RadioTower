class RTEvent
{
	float m_EventProgress;
	
	void ~RTEvent()
	{
		Print(RTConstants.RT_LOG_PREFIX + " RTEvent destructor");
	}
	
	void RTEvent()
	{
		Print(RTConstants.RT_LOG_PREFIX + " RTEvent constructor");
		
		m_EventProgress = 0;
	}
}

class RTBase
{
	ref RTEventConfig m_Config;
	ref RTEvent m_RTEvent;
	vector m_ServerPos;
	
	void ~RTBase()
	{
	}
	
	void RTBase()
	{
		Print(RTConstants.RT_LOG_PREFIX + " RTBase constructor");
		m_RTEvent = null;
		m_Config = RTSettings.Load();
				
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CreateEvent, 20000);	
	}
    
	
    void CreateEvent()
    {
		/*
		foreach (RTEventLocation o: m_Config.eventLocations)
		{
			RTEventLocation o: m_Config.eventLocation
			vector position = o.coordinates_xyz;
			vector orientation = o.orientation_ypr;
			
			RTServer s = RTServer.Cast(GetGame().CreateObjectEx("RTServer", position, ECE_KEEPHEIGHT));
			s.SetPosition(position);
			s.SetOrientation(orientation);
			
			s.SetFlags(EntityFlags.STATIC, false);
			s.Update();
			//m_ServerPos = s.GetPosition();
		}
		*/
		int eventLocationCount = m_Config.eventLocations.Count();
		if (eventLocationCount == 0)
			return;
		
		int eventLocationIndex = Math.RandomInt(0, eventLocationCount);
		RTEventLocation eventLocation;
		if (Class.CastTo(eventLocation, m_Config.eventLocations[eventLocationIndex]))
		{
			vector position = eventLocation.coordinates_xyz;
			vector orientation = eventLocation.orientation_ypr;
			
			RTServer item;
			if (RTServer.CastTo(item, GetGame().CreateObjectEx("RTServer", position, ECE_KEEPHEIGHT))
			{
				item.SetPosition(position);
				item.SetOrientation(orientation);
				item.SetFlags(EntityFlags.STATIC, false);
				item.Update();
				
				m_ServerPos = item.GetPosition();
			}
			Print(RTConstants.RT_LOG_PREFIX + " Server placed");
		}
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).RemoveByName(this, "CreateEvent");
    }
	
	
	
	void StartEvent()
	{
		//vector pos = m_ActiveRTServer.GetPosition();
		GetGame().CreateObject("CaptureArea", m_ServerPos);
	}
	

	/*
	void StartEvent()
	{
		m_RTEvent.StartEvent();
	}
	
	void CreateEvent()
	{
		int eventLocationCount = m_Config.eventLocations.Count();
		int eventLocationIndex = Math.RandomInt(0, eventLocationCount);
		ref RTServerLocation location = m_Config.eventLocations[eventLocationIndex];
		
		m_RTEvent = new RTEvent(location);
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