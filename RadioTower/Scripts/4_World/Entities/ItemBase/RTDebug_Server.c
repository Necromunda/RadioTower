class RTDebug_Server extends Container_Base
{
    override void SetActions()
	{
		super.SetActions();
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionPlaceObject);
		AddAction(ActionDebugLog);
	}
	
	override bool IsDeployable()
	{
		return true;
	}
	override bool CanPutInCargo(EntityAI parent)
	{
		return true;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return true;
	}
}