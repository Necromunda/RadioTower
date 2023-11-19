// class RT_Server extends ItemBase
class RT_Server extends Container_Base
// class RT_Server extends Building
{
	protected bool m_IsOpened;
	protected bool m_IsOpenedLocal;
	
	void RT_Server()
	{		
		RegisterNetSyncVariableBool("m_IsSoundSynchRemote");
		RegisterNetSyncVariableBool("m_IsOpened");
	}
	
	void ~RT_Server() { }
	
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
		// GetInventory().UnlockInventory(HIDE_INV_FROM_SCRIPT);
		super.Open();
	}

	override void Close()
	{
		super.Close();
		m_IsOpened = false;
		SoundSynchRemote();
		UpdateVisualState();
		// GetInventory().LockInventory(HIDE_INV_FROM_SCRIPT);
	}	

	override bool IsOpen()
	{
		return m_IsOpened;
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
};