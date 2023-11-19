modded class DayZGame
{  
    protected ref RadioTowerConfig m_RT_Server_Config;
    
    RadioTowerConfig Get_RT_Server_Config()
    {
        return m_RT_Server_Config;
    }

    void ServerSet_RT_Server_Config(RadioTowerConfig config)
    {
        if(GetGame().IsClient())
        {
            Error("[RadioTower] ERROR: ServerSet_RT_Server_Config can only be called on server");
            return;
        }
        m_RT_Server_Config = config;
    }
};
