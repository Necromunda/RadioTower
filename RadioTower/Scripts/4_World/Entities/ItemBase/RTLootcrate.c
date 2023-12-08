class RTLootcrate_Base extends Container_Base 
{ 
	override bool IsContainer()
	{
		return true;
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
		return true;
	}
	
	override bool CanReleaseCargo( EntityAI attachment )
	{
		return true;
	}
	
    override void SetActions()
	{
		super.SetActions();
	}
};

class RTLootcrate_Grey extends RTLootcrate_Base { };
class RTLootcrate_White extends RTLootcrate_Base { };
class RTLootcrate_Red extends RTLootcrate_Base { };
class RTLootcrate_Yellow extends RTLootcrate_Base { };