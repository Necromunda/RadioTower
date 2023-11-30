class ActionOpenRTServerConsole: ActionInteractBase
{
	void ActionOpenRTServerConsole()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		// m_HUDCursorIcon = CursorIcons.OpenDoors;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "Open laptop";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !target ) 
			return false;

		if (!IsInReach(player, target, RTConstants.RT_DISTANCE_DEFAULT)) 
			return false;

		string selection = target.GetObject().GetActionComponentName(target.GetComponentIndex());

		if (selection && selection != "component02")
			return false;	
		
		RTServer server = RTServer.Cast(target.GetObject());
		
		if (server)
		{
			if (server.IsHacked())
			{
				return false;
			}
			else 
			{
				return !server.IsOpen();
			}
		}			
			
		return false;
	}

	override void OnStartServer( ActionData action_data )
	{
		RTServer server = RTServer.Cast(action_data.m_Target.GetObject());
		if(server)		
			server.Open();
	}
};