class RTServer extends Container_Base
{
	protected bool m_IsOpened;
	protected bool m_IsOpenedLocal;
	protected bool m_IsHacked;
	protected bool m_IsHackedLocal;
	
	void RTServer()
	{	
		RegisterNetSyncVariableBool("m_IsSoundSynchRemote");
		RegisterNetSyncVariableBool("m_IsOpened");
		RegisterNetSyncVariableBool("m_IsHacked");
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