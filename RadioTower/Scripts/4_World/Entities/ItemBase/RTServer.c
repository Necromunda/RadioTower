class RTServer extends Container_Base
{
	protected bool m_IsOpened;
	protected bool m_IsOpenedLocal;
	protected bool m_IsHacked;
	protected bool m_IsHackedLocal;
	
	protected CaptureArea m_Trigger;
	protected float m_CollisionCylinderRadius;
	protected float m_CollisionCylinderHeight;
	
	void RTServer()
	{	
		RegisterNetSyncVariableBool("m_IsSoundSynchRemote");
		RegisterNetSyncVariableBool("m_IsOpened");
		RegisterNetSyncVariableBool("m_IsHacked");
		
		m_CollisionCylinderRadius = RTConstants.RT_EVENT_TRIGGER_RADIUS_DEFAULT;
		m_CollisionCylinderHeight = RTConstants.RT_EVENT_TRIGGER_HEIGHT_DEFAULT;
		RTEvent rtEvent = g_RTBase.GetRTEvent();
		if (rtEvent)
		{
			m_CollisionCylinderRadius = rtEvent.GetEventLocation().captureAreaRadius;
			m_CollisionCylinderHeight = rtEvent.GetEventLocation().captureAreaHeight;
		}
	}
	
	void ~RTServer() 
	{ 
	}
	
	override void EEInit()
	{
		super.EEInit();		
        if(IsOpen())
			Open();
		else
			Close();
		
		/*if (!m_Trigger)
		{
			//Print("radius = " + m_CollisionCylinderRadius + ", height = " + m_CollisionCylinderHeight);
			CaptureArea.CastTo(m_Trigger, GetGame().CreateObject("CaptureArea", GetPosition()));
			AddChild(m_Trigger, -1);
			Print("Trigger " + m_Trigger + " created in " + GetPosition());
		}*/
	}
	
	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		Print("in EEDelete");
		CleanUpTrigger();
	}

	protected void CleanUpTrigger()
	{
		if (m_Trigger)
		{
			GetGame().ObjectDelete(m_Trigger);
		}
	}
	
	CaptureArea GetTrigger()
	{
		return m_Trigger;
	}

	override void Open()
	{
		m_IsOpened = true;
		SoundSynchRemote();
		UpdateVisualState();
		super.Open();
	}

	override void Close()
	{
		super.Close();
		m_IsOpened = false;
		SoundSynchRemote();
		UpdateVisualState();
	}	

	override bool IsOpen()
	{
		return m_IsOpened;
	}
	
	void Hack()
	{
		m_IsHacked = true;
		SetSynchDirty();
	}
	
	bool IsHacked()
	{
		return m_IsHacked;
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		
		if ( m_IsOpened != m_IsOpenedLocal )
		{		
			if ( IsOpen() && IsSoundSynchRemote() && !IsBeingPlaced() )
			{
				SoundOpenPlay();
			}
			
			if ( !IsOpen() && IsSoundSynchRemote() && !IsBeingPlaced() )
			{
				SoundClosePlay();
			}	
			m_IsOpenedLocal = m_IsOpened;
		}
		
		if (m_IsHacked != m_IsHackedLocal)
		{
			m_IsHackedLocal = m_IsHacked;
		}
		
		UpdateVisualState();
	}
	
	override void OnStoreSave( ParamsWriteContext ctx )
	{   
		super.OnStoreSave( ctx );		
		ctx.Write( m_IsOpened );
	}
	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( !super.OnStoreLoad( ctx, version ) )
			return false;
		
		if (!ctx.Read( m_IsOpened ) )
			return false;
		
		if ( m_IsOpened )
			Open();
		else
			Close();
		
		return true;
	}

	void UpdateVisualState()
    {
        if ( IsOpen() )
        {
            // SetAnimationPhase("server_console_screen",1);
            SetAnimationPhase("Spin", 0);
        }
        else
        {
            // SetAnimationPhase("server_console_screen",0);
            SetAnimationPhase("Spin", 1);
        }
    }
	
	void SoundOpenPlay()
	{
	}
	
	void SoundClosePlay()
	{
	}
	
	override bool IsContainer()
	{
		return false;
	}
	
	override bool CanPutInCargo( EntityAI parent )
	{
		return false;
	}
    override bool CanPutIntoHands( EntityAI parent )
	{
		return false;
	}
	
	override bool CanReceiveItemIntoCargo( EntityAI item )
	{
		return false;
	}
	
	override bool CanReleaseCargo( EntityAI attachment )
	{
		return false;
	}
	
    override void SetActions()
	{
		super.SetActions();
		AddAction(ActionOpenRTServerConsole);
		AddAction(ActionCloseRTServerConsole);
		AddAction(ActionHackRTServerConsole);
	}
}