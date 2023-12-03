/*enum RTServerLedState
{
	OFF = 0,
	LIT,
}*/

class RTServer extends Container_Base
{
	protected bool m_IsOpened;
	protected bool m_IsOpenedLocal;
	protected bool m_IsHacked;
	protected bool m_IsHackedLocal;
	
	//const string COLOR_LED_OFF 		= "#(argb,8,8,3)color(0,0,0,1.0,co)";
	//const string COLOR_LED_LIT 		= "#(argb,8,8,3)color(1,0,0,1.0,co)";
	//const string SELECTION_NAME_LED_ON = "led_on";
	//const string SELECTION_NAME_LED_OFF = "led_off";
	
	//protected RTServerLedState m_LastLEDState;
	
	void RTServer()
	{		
		RegisterNetSyncVariableBool("m_IsSoundSynchRemote");
		RegisterNetSyncVariableBool("m_IsOpened");
		RegisterNetSyncVariableBool("m_IsHacked");
		//RegisterNetSyncVariableInt("m_LastLEDState", 0, EnumTools.GetEnumSize(RTServerLedState));
		
		//UpdateLED(m_LastLEDState);
	}
	
	void ~RTServer() 
	{ 
	}
	
	/*void UpdateLED(RTServerLedState pState)
	{
		int selectionIdxLedOn = GetHiddenSelectionIndex(SELECTION_NAME_LED_ON);
		int selectionIdxLedOff = GetHiddenSelectionIndex(SELECTION_NAME_LED_OFF);

		switch (pState)
		{
		case RTServerLedState.LIT:
			SetObjectTexture(selectionIdxLedOn, COLOR_LED_LIT);
			SetObjectTexture(selectionIdxLedOff, COLOR_LED_OFF);
		break;
		default:
			SetObjectTexture(selectionIdxLedOn, COLOR_LED_OFF);
			SetObjectTexture(selectionIdxLedOff, COLOR_LED_LIT);
		}
		
		m_LastLEDState = pState;
		SetSynchDirty();
	}*/
	
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
		//UpdateLED(m_LastLEDState);
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