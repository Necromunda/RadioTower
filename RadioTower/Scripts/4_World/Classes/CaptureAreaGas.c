class CaptureAreaGas : EffectArea
{
	protected ref Timer m_StartupTimer;
	//protected vector 	m_OffsetPos;
	//const float 		AIRBORNE_FX_OFFSET = 50;
	
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
	
	override void EEOnCECreate()
	{
		Print("EEOnCECreate");
		if ( GetGame().IsServer() )
			m_PPERequesterIdx = GetRequesterIndex(m_PPERequesterType);
		
		SetSynchDirty();
		float delay = 0.1;
		m_StartupTimer = new Timer(CALL_CATEGORY_GAMEPLAY);
		m_StartupTimer.Run(delay, this, "InitZone");
	}
	
	override void EEInit()
	{
		//Print("EEInit");
		if ( !m_ToxicClouds )
			m_ToxicClouds = new array<Particle>;
		
		RTLocation _location = null;
		
		if (GetGame().IsServer())
			_location = g_RTBase.GetRTEvent().GetEventLocation();
		else
			_location = g_Game.GetLatestEventLocation();
		
		m_Name = "CaptureAreaGas setup";
		m_TriggerType = "CaptureAreaGasTrigger";
		m_OuterRingOffset = 0;
		
		if (_location)
		{
			//m_Name = "CaptureAreaGas setup";
			m_Radius = _location.captureAreaRadius;
			m_PositiveHeight = _location.captureAreaHeight;
			m_NegativeHeight = _location.captureAreaYAxisOffset;
			//m_InnerRings = 1;
			m_InnerSpacing = _location.captureAreaRadius / 2;
			m_OuterSpacing = _location.captureAreaRadius / 2;
			//m_OuterRingOffset = 0;
			m_Type = eZoneType.DYNAMIC;
			//m_TriggerType = "CaptureAreaGasTrigger";
		}

		/*
		m_Name = "CaptureAreaGas";
		m_Radius = 120;
		m_PositiveHeight = 7;
		m_NegativeHeight = 10;
		m_InnerRings = 1;
		m_InnerSpacing = 40;
		m_OuterSpacing = 30;
		m_OuterRingOffset = 0;
		m_Type = eZoneType.DYNAMIC;
		m_TriggerType = "CaptureAreaGasTrigger";
		*/
		
		SetSynchDirty();
		
		//m_OffsetPos = GetPosition();
		//m_OffsetPos[1] = m_OffsetPos[1] + AIRBORNE_FX_OFFSET;
		
		InitZone();
		
		if ( GetGame().IsClient() )
			InitZoneClient();
		
		super.EEInit();
	}
	
	override void CreateTrigger( vector pos, int radius )
	{
		super.CreateTrigger( pos, radius );
		
		/*
		ContaminatedTrigger_Dynamic dynaTrigger = ContaminatedTrigger_Dynamic.Cast( m_Trigger );
		if (dynaTrigger)
			dynaTrigger.SetLocalEffects( m_AroundParticleID, m_TinyParticleID, m_PPERequesterIdx );
		*/
		CaptureAreaGasTrigger _trigger = CaptureAreaGasTrigger.Cast( m_Trigger );
		if (_trigger)
			_trigger.SetLocalEffects( m_AroundParticleID, m_TinyParticleID, m_PPERequesterIdx );
	}
	
	override void InitZone()
	{
		SetSynchDirty();
		
		super.InitZone();
	}
	
	override void InitZoneServer()
	{
		//super.InitZoneServer();

		if ( m_TriggerType != "" )
			CreateTrigger(m_Position, m_Radius);
	}
	
	override void InitZoneClient()
	{
		//super.InitZoneClient();
		Print("InitZoneClient()");
		if ( !m_ToxicClouds )
			m_ToxicClouds = new array<Particle>;

		//PlaceParticles(GetWorldPosition(), m_Radius, m_InnerRings, m_InnerSpacing, m_OuterRingToggle, m_OuterSpacing, m_OuterRingOffset, m_ParticleID);	
		PlaceParticles(m_Position, m_Radius, m_InnerRings, m_InnerSpacing, m_OuterRingToggle, m_OuterSpacing, m_OuterRingOffset, m_ParticleID);	
		
		/*
		for (int i = 0; i < m_ToxicClouds.Count(); i++)
		{
			Print(i.ToString() + ": " + GetWorldPosition());
			Print(i.ToString() + ": " + m_ToxicClouds[i]);
			m_ToxicClouds[i].PlayInWorld(ParticleList.CONTAMINATED_AREA_GAS_BIGASS, GetWorldPosition());
		}
		*/
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		
		if ( !m_ToxicClouds )
			m_ToxicClouds = new array<Particle>;
			
		InitZoneClient();
	}
}