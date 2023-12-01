/** \name OnEnter
    Called when an object enters the trigger
*/
// protected void OnEnterBeginEvent(TriggerInsider insider) {}
// protected void OnEnterServerEvent(TriggerInsider insider) {}
// protected void OnEnterClientEvent(TriggerInsider insider) {}
// protected void OnEnterEndEvent(TriggerInsider insider) {}

/** \name OnStayStart
    Called at the beginning of an update loop
*/
// protected void OnStayStartBeginEvent(int nrOfInsiders) {}
// protected void OnStayStartServerEvent(int nrOfInsiders) {}
// protected void OnStayStartClientEvent(int nrOfInsiders) {}
// protected void OnStayStartEndEvent(int nrOfInsiders) {}

/** \name OnStay
    Called in the update loop
*/
// protected void OnStayBeginEvent(TriggerInsider insider, float deltaTime) {}
// protected void OnStayServerEvent(TriggerInsider insider, float deltaTime) {}
// protected void OnStayClientEvent(TriggerInsider insider, float deltaTime) {}
// protected void OnStayEndEvent(TriggerInsider insider, float deltaTime) {}

/** \name OnStayFinish
    Called at the end of an update loop
*/
// protected void OnStayFinishBeginEvent() {}
// protected void OnStayFinishServerEvent() {}
// protected void OnStayFinishClientEvent() {}
// protected void OnStayFinishEndEvent() {}

/** \name OnLeave
    Called when an object leaves the trigger
*/
// protected void OnLeaveBeginEvent(TriggerInsider insider) {}
// protected void OnLeaveServerEvent(TriggerInsider insider) {}
// protected void OnLeaveClientEvent(TriggerInsider insider) {}
// protected void OnLeaveEndEvent(TriggerInsider insider) {}

class CaptureArea: Trigger
{
	float m_Capture_Tickrate = RTConstants.RT_CAPTURE_TICK_RATE_DEFAULT;
	float m_TimeAccuStay = 0;
	float m_CapturePct = 0;
	
	const float LIFETIME_TICKRATE = 1; // seconds
	ref Timer 	m_Timer1 = new Timer;
	float 		m_Lifetime = 30; 		 // seconds
	
	void CaptureArea()
	{
		m_Lifetime = g_RTBase.m_Config.eventLifetime;
		m_Capture_Tickrate = g_RTBase.m_Config.eventCapturetime / 100;
		
		float radius = 25.0;
		float height = 25.0;
		SetCollisionCylinder(radius, height);
		m_Timer1.Run(LIFETIME_TICKRATE, this, "Tick", NULL, true);
	}
	
	void Tick()
	{
		string date = CF_Date.Now().Format("DD-MM-YYYY hh:mm:ss");
		if (m_Lifetime.ToString().ToInt() % 60 == 0)
			Print(date + " CaptureArea lifetime " + m_Lifetime);
		m_Lifetime -= LIFETIME_TICKRATE;
		if (m_Lifetime < 0)
		{
			DeleteSafe();
			Print("Capture area deleted");
		}
	}
	
	override void OnEnterClientEvent(TriggerInsider insider)
	{
		super.OnEnterClientEvent(insider);
		
		PlayerBase player;
		if( Class.CastTo( player, insider.GetObject()))
		{
			Print("CLIENT: " + player.GetIdentity().GetPlainName() + " entered capture area");
			g_RTBase.SetIsInCaptureZone(true);
		}
	}
	
	override void OnLeaveClientEvent(TriggerInsider insider)
	{
		super.OnLeaveClientEvent(insider);
		
		PlayerBase player;
		if(Class.CastTo( player, insider.GetObject()))
		{
			Print("CLIENT: " + player.GetIdentity().GetPlainName() + " left capture area");
			g_RTBase.SetIsInCaptureZone(false);
		}
	}
	
	override void OnStayClientEvent(TriggerInsider insider, float deltaTime) 
	{
		super.OnStayClientEvent(insider, deltaTime);
		
		m_TimeAccuStay += deltaTime;
		if (m_TimeAccuStay > m_Capture_Tickrate)
		{
			m_TimeAccuStay = 0;
			int insiderCount = GetInsiders().Count();
			Print("Insider count: " + insiderCount)
			//Print("Insider count: " + insiderCount);
			if (insiderCount > 0)
			{
				m_CapturePct += 1;
				Print("Area captured " + m_CapturePct + "%");
			}
		} 
	}
	
	override void OnEnterServerEvent(TriggerInsider insider)
	{
		super.OnEnterServerEvent(insider);
		
		PlayerBase player;
		if( Class.CastTo( player, insider.GetObject()))
		{
			Print("SERVER: " + player.GetIdentity().GetPlainName() + " entered capture area");
			g_RTBase.SetInsiderCount(GetInsiders().Count());
		}
	}
	
	override void OnLeaveServerEvent(TriggerInsider insider)
	{
		super.OnLeaveServerEvent(insider);
		
		PlayerBase player;
		if(Class.CastTo( player, insider.GetObject()))
		{
			Print("SERVER: " + player.GetIdentity().GetPlainName() + " left capture area");
			//GetRPCManager().SendRPC("RadioTower", "SetInsiderCount", new Param1<int>(GetInsiders().Count()), true, identity);
			//g_RTBase.SetInsiderCount(GetInsiders().Count());
			//g_RTBase.SetIsInCaptureZone(false);
		}
	}
	
	override void OnStayServerEvent(TriggerInsider insider, float deltaTime)
	{
		super.OnStayServerEvent(insider, deltaTime);
		
		m_TimeAccuStay += deltaTime;
		if (m_TimeAccuStay > m_Capture_Tickrate)
		{
			m_TimeAccuStay = 0;
			int insiderCount = GetInsiders().Count();
			Print("Insider count: " + insiderCount)
			//Print("Insider count: " + insiderCount);
			if (insiderCount > 0)
			{
				m_CapturePct += 1;
				Print("Area captured " + m_CapturePct + "%");
			}
		} 
	}
}