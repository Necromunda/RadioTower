modded class DayZGame
{  
    protected ref RTSettings m_Settings;
	protected bool m_IsClientInCaptureZone;
    
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
	
	bool IsClientInCaptureZone()
	{
		return m_IsClientInCaptureZone;
	}
	
	void SetIsClientInCaptureZone(bool value)
	{
		m_IsClientInCaptureZone = value;
	}
};
