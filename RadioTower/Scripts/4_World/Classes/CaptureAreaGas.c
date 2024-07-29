class CaptureAreaGas : EffectArea
{
	protected ref Timer m_StartupTimer;
	
	/*
	override void OnPlayerEnterServer(PlayerBase player, EffectTrigger trigger)
	{
		super.OnPlayerEnterServer(player, trigger);
		player.IncreaseContaminatedAreaCount();
	}
	
	override void OnPlayerExitServer(PlayerBase player, EffectTrigger trigger)
	{
		super.OnPlayerExitServer(player, trigger);
		player.DecreaseContaminatedAreaCount();
	}
	*/
	
	/*
	override void EEOnCECreate()
	{
		if ( GetGame().IsServer() )
			m_PPERequesterIdx = GetRequesterIndex(m_PPERequesterType);
		
		SetSynchDirty();
		float delay = 0.1;
		m_StartupTimer = new Timer(CALL_CATEGORY_GAMEPLAY);
		m_StartupTimer.Run(delay, this, "InitZone");
	}
	*/
	
	override void EEInit()
	{
		if ( !m_ToxicClouds )
			m_ToxicClouds = new array<Particle>;
		
		/*
		RTLocation _location = null;
		
		if (GetGame().IsServer())
			_location = g_RTBase.GetRTEvent().GetEventLocation();
		else
			_location = g_Game.GetLatestEventLocation();
		*/
		
		//m_Name = "CaptureAreaGas";
		//m_TriggerType = "CaptureAreaGasTrigger";
		//m_OuterRingOffset = 0;
		//m_Radius = 200;
		//m_PositiveHeight = 50;
		//m_NegativeHeight = 50;
		//m_InnerSpacing = 70;
		//m_OuterSpacing = 40;
		//m_Type = eZoneType.DYNAMIC;
		
		/*
		if (_location)
		{
			m_Radius = _location.captureAreaRadius;
			m_PositiveHeight = _location.captureAreaHeight;
			m_NegativeHeight = _location.captureAreaYAxisOffset;
			m_InnerSpacing = _location.captureAreaRadius / 2;
			m_OuterSpacing = _location.captureAreaRadius / 2;
			m_Type = eZoneType.DYNAMIC;
		}
		*/
		
		SetSynchDirty();
		/*		
		InitZone();
		
		if ( GetGame().IsClient() )
			InitZoneClient();
		*/
		
		//if ( GetGame().IsClient() && GetGame().IsMultiplayer() )
		InitZone();
		
		super.EEInit();
	}
	/*
	override void CreateTrigger( vector pos, int radius )
	{
		super.CreateTrigger( pos, radius );

		CaptureAreaGasTrigger _trigger = CaptureAreaGasTrigger.Cast( m_Trigger );
		if (_trigger)
			_trigger.SetLocalEffects( m_AroundParticleID, m_TinyParticleID, m_PPERequesterIdx );
	}
	*/
	/*
	override void InitZone()
	{
		SetSynchDirty();
		
		super.InitZone();
	}
	*/
	
	override void InitZone()
    {
        super.InitZone();
    }
	
	override void InitZoneServer()
	{
		if ( m_TriggerType != "" )
			CreateTrigger(m_Position, m_Radius);
	}
	
	override void InitZoneClient()
	{
		/*
		if ( !m_ToxicClouds )
			m_ToxicClouds = new array<Particle>;
		*/
		PlaceParticles(m_Position, m_Radius, m_InnerRings, m_InnerSpacing, m_OuterRingToggle, m_OuterSpacing, m_OuterRingOffset, m_ParticleID);	

		/*
		for (int i = 0; i < m_ToxicClouds.Count(); i++)
		{
			m_ToxicClouds[i].PlayInWorld(ParticleList.CONTAMINATED_AREA_GAS_BIGASS, GetWorldPosition());
		}
		*/

	}
	/*
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		
		if ( !m_ToxicClouds )
			m_ToxicClouds = new array<Particle>;
			
		InitZoneClient();
	}
	*/
}