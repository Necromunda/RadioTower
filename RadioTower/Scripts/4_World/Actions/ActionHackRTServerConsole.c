class ActionHackRTServerConsoleCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		float hackTime = RTConstants.RT_TIMETOHACK_DEFAULT;

		if (g_RTBase && GetGame().IsServer())
		{
			//hackTime = g_RTBase.m_Settings.eventHacktime;
			hackTime = g_RTBase.m_Settings.kothEvent.hackTime;
		}
		else
		{
			RTSettings settings = g_Game.GetRTSettings();
			if (settings && GetGame().IsClient())
				//hackTime = settings.eventHacktime;
				hackTime = settings.kothEvent.hackTime;
		}

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
		if (!target) 
			return false;

		if (!IsInReach(player, target, RTConstants.RT_DISTANCE_DEFAULT)) 
			return false;

		string selection = target.GetObject().GetActionComponentName(target.GetComponentIndex());

		if (selection && selection != "component02")
			return false;	
		
		RTServer server = RTServer.Cast(target.GetObject());
		
		if(server)			
			return server.IsOpen();
			
		return false;
	}

	override void OnFinishProgressServer( ActionData action_data ) 
	{
		//Print("[RadioTower] Debug: Hack action OnFinishProgressServer()");
		string msg = action_data.m_Player.GetIdentity().GetPlainName() + " started an event";
		g_RTBase.Log(RTLogType.INFO, "Server hacked. " + msg);
		RTServer server = RTServer.Cast(action_data.m_Target.GetObject());
		if (server)
		{	
			server.Disable();
			server.SetCaptureStateSynchronized(CaptureState.CAPTURING);
			g_RTBase.StartEvent(server);	
		}
	}	
};