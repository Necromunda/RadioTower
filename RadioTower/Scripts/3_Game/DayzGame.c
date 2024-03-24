modded class DayZGame
{  
    protected ref RTSettings m_Settings;
	protected float m_CapturePct;
	protected int m_InsiderCount;
	protected ref RTLocation m_LatestEventLocation;
    
    RTSettings GetRTSettings()
    {
        return m_Settings;
    }

    void ClientSetRTSettings(RTSettings settings)
    {
        if ( GetGame().IsServer() || !GetGame().IsClient() )
        {
            Error("[RadioTower] ERROR: ClientSetRTSettings can only be called on client");
            return;
        }
        m_Settings = settings;
    }
	
	float GetCapturePct()
	{
		return m_CapturePct;
	}
	
	void SetCapturePct(float value)
	{
		m_CapturePct = value;
	}
	
	int GetInsiderCount()
	{
		return m_InsiderCount;
	}
	
	void SetInsiderCount(int value)
	{
		m_InsiderCount = value;
	}
	
	bool IsCaptureStatusSmokeEnabled()
	{
		bool enabled = false;
		if (m_Settings && m_Settings.kothEvent)
			//enabled = m_Settings.showCaptureStatusSmoke;
			enabled = m_Settings.kothEvent.enableCaptureStatusSmoke;
		return enabled;
	}
	
	void SetLatestEventLocation(ref RTLocation eventLocation)
	{
		m_LatestEventLocation = eventLocation;
	}
	
	ref RTLocation GetLatestEventLocation()
	{
		return m_LatestEventLocation;
	}
	
	void DeleteLatestEventLocation()
	{
		m_LatestEventLocation = null;
	}
}
