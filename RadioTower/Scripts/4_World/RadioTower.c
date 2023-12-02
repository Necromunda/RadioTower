class RTEvent
{
	protected string m_LocationTitle;
	
	void ~RTEvent()
	{
		Print(RTConstants.RT_LOG_PREFIX + " RTEvent dtor");
	}
	
	void RTEvent()
	{
		Print(RTConstants.RT_LOG_PREFIX + " RTEvent ctor");
	}
	
	void SetEventTitle(string title)
	{
		m_LocationTitle = title;
	}
	
	string GetEventTitle()
	{
		return m_LocationTitle;
	}
}

class RTBase
{
	ref RTEventConfig m_Config;
	protected ref RTEvent m_RTEvent;
	protected vector m_ServerPos;
	protected bool m_IsClientInCaptureZone;
	
	void ~RTBase()
	{
		Print(RTConstants.RT_LOG_PREFIX + " RTBase dtor");
	}
	
	void RTBase()
	{
		Print(RTConstants.RT_LOG_PREFIX + " RTBase ctor");
		m_Config = RTSettings.Load();
		m_RTEvent = null;
		m_IsClientInCaptureZone = false;
				
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CreateEvent, 20000);	
	}
	
	RTEvent GetRTEvent()
	{
		return m_RTEvent;
	}
    
	bool IsClientInCaptureZone()
	{
		return m_IsClientInCaptureZone;
	}
	
	void SetIsClientInCaptureZone(bool value)
	{
		m_IsClientInCaptureZone = value;
	}
	
    void CreateEvent()
    {
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
				m_RTEvent = new RTEvent();
				m_RTEvent.SetEventTitle(eventLocation.title);
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
		GetGame().CreateObject("CaptureArea", m_ServerPos);
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