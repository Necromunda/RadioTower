class ActionDebugLog: ActionInteractBase
{
	void ActionDebugLog()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "Log coords and orientation";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !target ) 
			return false;

		if (!IsInReach(player, target, RTConstants.RT_DISTANCE_DEFAULT)) 
			return false;	
		
		ItemBase target_item;
		
		if (ItemBase.CastTo(target_item, target.GetObject()))
			return true;		
			
		return false;
	}

	override void OnStartServer( ActionData action_data )
	{
		ItemBase item;
		
		if (ItemBase.CastTo(item, action_data.m_Target.GetObject()))
		{
			Print("[RadioTower Debug] " + item.GetName() + " position: " + item.GetPosition());
			Print("[RadioTower Debug] " + item.GetName() + " orientation: " + item.GetOrientation());
		}
	}
};