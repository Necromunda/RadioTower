class RadioTowerBase
{
	ref RTServerConfig m_Config;
	
	void ~RadioTowerBase()
	{
	}
	
	void RadioTowerBase()
	{
		m_Config = RadioTowerSettings.Load();
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(PlaceServer, 20000); 
		GetRPCManager().AddRPC("RadioTower", "FncInvokeDataResponse", this, SingleplayerExecutionType.Client);
	}
    
    void PlaceServer()
    {
		foreach (RTServerLocation o: m_Config.locations)
		{
			vector position = o.coordinates_xyz;
			vector orientation = o.orientation_ypr;
			
			RT_Server s = RT_Server.Cast(GetGame().CreateObjectEx("RT_Server", position, ECE_KEEPHEIGHT));
			s.SetPosition(position);
			s.SetOrientation(orientation);
			
			s.SetFlags(EntityFlags.STATIC, false);
			s.Update();
		}
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).RemoveByName(this, "PlaceServer");
    }
	
	void TestRPCFunction( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target )
    {
        Param1< string > data;
        if ( !ctx.Read( data ) ) return;
        
        if( type == CallType.Server )
        {
            Print( "HLynge Server function called!" );
			Print("Data: " + data);
        }
        else
        {
            Print( "HLynge Client function called!" );
            Print("Data: " + data);
        }
    }
	
	void FncInvokeDataResponse(CallType type, ParamsReadContext ctx, PlayerIdentity sender, Object target)
	{
    	//if this function is trigger anywhere but on client, we return without continuing.
		if(type != CallType.Client)
			return;

		Param1<string> data;
   		//if the data is not retrieved we return to avoid issue
		if (!ctx.Read(data)) 
			return;
	    
	    string clientResponse = data.param1;
	
	    Print(clientResponse);
	}
}

static ref RadioTowerBase g_RadioTowerBase;
static ref RadioTowerBase GetRadioTowerBase()
{
	if (!g_RadioTowerBase)
	{
		g_RadioTowerBase = new ref RadioTowerBase();
	}
	return g_RadioTowerBase;
}