class ActionHackRTServerConsoleCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		float hackTime = RTConstants.RT_TIMETOHACK_DEFAULT;

		m_ActionData.m_ActionComponent = new CAContinuousTime(hackTime);
	}
}

class ActionHackRTServerConsole: ActionContinuousBase
{
	void ActionHackRTServerConsole()
	{
		m_CallbackClass = ActionHackRTServerConsoleCB;
		// m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_CRAFTING;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
		// m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
	}

	override string GetText()
	{
		return "Hack";
	}
	
	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if( !target ) return false;

		// if(!IsInReach(player, target, RTConstants.RT_DISTANCE_DEFAULT)) return false;

		// string selection = target.GetObject().GetActionComponentName(target.GetComponentIndex());
		// Print("[RadioTower] selection: " + selection);

		// if(selection && selection != "component02")
			// return false;	
		
		RT_Server server = RT_Server.Cast(target.GetObject());
		
		if(server)			
			return server.IsOpen();
			
		return false;
	}

	override void OnFinishProgressServer( ActionData action_data ) 
	{
		Print("[RadioTower] Debug: Hack action OnFinishProgressServer()");
		// RT_Server server = RT_Server.Cast(action_data.m_Target.GetObject());
		// if(server)		
		// {	
			// server.Close();
			// return;
		// }		
	}	
};