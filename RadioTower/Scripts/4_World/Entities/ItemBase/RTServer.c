enum CaptureState
{
	DEFAULT,
	CAPTURING,
	CAPTURED
};

class RTServer extends Container_Base
{
	protected bool m_IsOpened;
	protected bool m_IsOpenedLocal;
	protected bool m_IsHacked;
	protected bool m_IsHackedLocal;
	
	protected int m_CaptureState;
	protected int m_CaptureStateLocal;
	
	protected vector m_ParticlePlayPos;
	
	protected Particle m_ParDefaultFire;
	protected Particle m_ParCapturingFire;
	protected Particle m_ParCapturedFire;
	
	static protected int PARTICLE_DEFAULT_FIRE = ParticleList.RT_SMOKE_RED;
	static protected int PARTICLE_CAPTURING_FIRE = ParticleList.RT_SMOKE_YELLOW;
	static protected int PARTICLE_CAPTURED_FIRE = ParticleList.RT_SMOKE_GREEN;
	
	//protected CaptureArea m_Trigger;
	
	void RTServer()
	{	
		//Print("[RadioTower] RTServer ctor");
		RegisterNetSyncVariableBool("m_IsSoundSynchRemote");
		RegisterNetSyncVariableBool("m_IsOpened");
		RegisterNetSyncVariableBool("m_IsHacked");
		RegisterNetSyncVariableInt("m_CaptureState");
		
		m_CaptureState = CaptureState.DEFAULT;
		m_ParticlePlayPos = GetProgressPclPosition();
	}
	
	void ~RTServer() 
	{ 
		//Print("[RadioTower] RTServer dtor");
	}
	
	override void EEInit()
	{
		super.EEInit();	
		Close();	
		//SetCaptureStateSynchronized(CaptureState.DEFAULT);
        /*if(IsOpen())
			Open();
		else
			Close();*/
		
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
		DestroyAllParticles();
		//if (m_Light)
			//m_Light.FadeOut(0);
		//Print("in EEDelete");
		//CleanUpTrigger();
	}

	/*protected void CleanUpTrigger()
	{
		if (m_Trigger)
		{
			GetGame().ObjectDelete(m_Trigger);
		}
	}
	
	CaptureArea GetTrigger()
	{
		return m_Trigger;
	}*/
	
	bool NeedsSync()
	{
		return m_IsOpened != m_IsOpenedLocal || m_IsHacked != m_IsHackedLocal || m_CaptureState != m_CaptureStateLocal;
	}
	
	int GetCaptureState()
	{
		return m_CaptureState;
	}
	
	void SetCaptureStateSynchronized(int state_number)
	{
		if ( !GetGame().IsDedicatedServer() )
			return;
		
		m_CaptureState = state_number;
		SetSynchDirty();
		//Print("[RadioTower] CaptureState: " + m_CaptureState);
		//UpdateActiveParticles();
	}
	
	void UpdateActiveParticles()
	{
		if ( GetGame().IsDedicatedServer() )
			return;
		
		if (!g_Game.IsCaptureStatusSmokeEnabled())
			return;
		
		//DestroyAllParticles();
		switch (m_CaptureStateLocal)
		{				
			case CaptureState.DEFAULT:			
				if (!m_ParDefaultFire)
				{
					DestroyAllParticles();
					m_ParDefaultFire = ParticleManager.GetInstance().PlayOnObject( PARTICLE_DEFAULT_FIRE, this, m_ParticlePlayPos);
					//m_ParDefaultFire = ParticleManager.GetInstance().PlayInWorld(PARTICLE_DEFAULT_FIRE, pclPos);
					//m_ParDefaultFire.SetWiggle( 10, 0.3 );
				}
				break;
				
			case CaptureState.CAPTURING:
				if (!m_ParCapturingFire)
				{
					//DestroyAllParticles();
					m_ParCapturingFire = ParticleManager.GetInstance().PlayOnObject( PARTICLE_CAPTURING_FIRE, this, m_ParticlePlayPos);
					//m_ParCapturingFire.SetWiggle( 7, 0.3 );
				}
				
				DestroyParticleEx(m_ParDefaultFire);
				break;	
				
			case CaptureState.CAPTURED:
				if (!m_ParCapturedFire)
				{
					//DestroyAllParticles();
					m_ParCapturedFire = ParticleManager.GetInstance().PlayOnObject( PARTICLE_CAPTURED_FIRE, this, m_ParticlePlayPos);
					//m_ParCapturedFire.SetWiggle( 4, 0.3 );
				}
				
				DestroyParticleEx(m_ParCapturingFire);
				break;
		}
	}
	
	// Destroys the given particle
	void DestroyParticle( Particle p )
	{
		if (p)
		{
			p.SetWiggle(0,0);
			p.Stop();
		}
	}
	
	void DestroyParticleEx( out Particle p )
	{
		DestroyParticle(p);
		p = null;
	}
	
	void DestroyAllParticles()
	{
		DestroyParticleEx(m_ParDefaultFire);
		DestroyParticleEx(m_ParCapturingFire);
		DestroyParticleEx(m_ParCapturedFire);
	}
	
	vector GetProgressPclPosition()
	{
		if ( MemoryPointExists( "progress_pcl" ) )
		{
			//Print("Memory point exists");
			vector position;
			position = GetMemoryPointPos( "progress_pcl" );
			//Print(position);
			return position;
			//return ModelToWorld( position );
		}
		
		return Vector(0, -10, 0);
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
		m_IsOpened = false;
		SoundSynchRemote();
		UpdateVisualState();
		super.Close();
	}	

	override bool IsOpen()
	{
		return m_IsOpened;
	}
	
	void Disable()
	{
		Hack();
		Close();
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
			m_IsHackedLocal = m_IsHacked;
		
		if (m_CaptureState != m_CaptureStateLocal)
			m_CaptureStateLocal = m_CaptureState;
		
		UpdateVisualState();
		UpdateActiveParticles();
	}
	
	/*override void OnStoreSave( ParamsWriteContext ctx )
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
		
		//SetCaptureState(CaptureState.DEFAULT);
		
		return true;
	}*/

	void UpdateVisualState()
    {
        if ( IsOpen() )
        {
            SetAnimationPhase("Spin", 0);
        }
        else
        {
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