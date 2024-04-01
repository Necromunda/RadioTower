class CaptureAreaGasTrigger extends ContaminatedTrigger 
{ 
	void CaptureAreaGasTrigger()
	{
		m_Manager = TriggerEffectManager.GetInstance();
		m_Manager.RegisterTriggerType( this );
	}

	override protected void OnStayClientEvent(TriggerInsider insider, float deltaTime) 
	{
		if ( insider.GetObject().IsMan() )
		{
			PlayerBase playerInsider = PlayerBase.Cast( insider.GetObject() );
			
			// We will only handle the controlled player, as effects are only relevant to this player instance
			if ( playerInsider.IsControlledPlayer() )
				playerInsider.RequestTriggerEffect(this, m_PPERequester, m_AroundPartId, m_TinyPartId, GetAmbientSoundsetName() );
		}
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		
		// Is the controlled player inside when trigger is synchronized
		for ( int i = 0; i < GetInsiders().Count(); i++ )
		{
			PlayerBase playerInsider = PlayerBase.Cast( m_insiders.Get( i ).GetObject() );
			if ( playerInsider )
			{
				if ( playerInsider.IsControlledPlayer() )
				{
					playerInsider.RequestTriggerEffect(this, m_PPERequester, m_AroundPartId, m_TinyPartId, GetAmbientSoundsetName() );
					return;
				}
			}
		}
	}
}