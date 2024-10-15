class CfgPatches
{
	class RadioTower
	{
		units[]=
		{
			"RTLootcrate_Grey",
			"RTLootcrate_White",
			"RTLootcrate_Red",
			"RTLootcrate_Yellow",
			"RTDebug_Lootcrate"
		};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};
class CfgVehicles
{
	class Container_Base;
	class RTLootcrate_Base: Container_Base
	{
		scope=0;
		displayName="Supply crate";
		descriptionShort="A crate containing various useful items.";
		model="RadioTower\RadioTower\Data\Lootcrate\lootcrate.p3d";
		itemBehaviour=0;
		itemSize[]={1,1};
		carveNavmesh=1;
		canBeDigged=0;
		rotationFlags=2;
		soundImpactType="plastic";
		physLayer="item_large";
		class Cargo
		{
			itemsCargoSize[]={10,50};
			openable=0;
			allowOwnedCargoManipulation=1;
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class movement
				{
					soundSet="seachest_movement_SoundSet";
					id=1;
				};
				class pickUpItem_Light
				{
					soundSet="pickUpSeaChest_SoundSet";
					id=796;
				};
				class pickUpItem
				{
					soundSet="pickUpSeaChest_SoundSet";
					id=797;
				};
				class drop
				{
					soundset="seachest_drop_SoundSet";
					id=898;
				};
			};
		};
	};
	// class RTLootcrate_Base: Container_Base
	// {
		// scope=0;
		// displayName="Supply crate";
		// descriptionShort="A crate containing various useful items.";
		// model="RadioTower\RadioTower\Data\Lootcrate\lootcrate.p3d";
		// rotationFlags=2;
		// itemSize[]={5,5};
		// itemsCargoSize[]={10,50};
		// canBeDigged=0;
		// allowOwnedCargoManipulation=1;
		// soundImpactType="plastic";
		// physLayer="item_large";
	// };
	class RTLootcrate_Grey: RTLootcrate_Base
	{
		scope=2;
		hiddenSelections[]=
		{
			"camo"
		};
		hiddenSelectionsTextures[]=
		{
			"RadioTower\RadioTower\Data\Lootcrate\data\lootcrate_grey_co.paa"
		};
	};
	class RTLootcrate_White: RTLootcrate_Base
	{
		scope=2;
		hiddenSelections[]=
		{
			"camo"
		};
		hiddenSelectionsTextures[]=
		{
			"RadioTower\RadioTower\Data\Lootcrate\data\lootcrate_white_co.paa"
		};
	};
	class RTLootcrate_Red: RTLootcrate_Base
	{
		scope=2;
		hiddenSelections[]=
		{
			"camo"
		};
		hiddenSelectionsTextures[]=
		{
			"RadioTower\RadioTower\Data\Lootcrate\data\lootcrate_red_co.paa"
		};
	};
	class RTLootcrate_Yellow: RTLootcrate_Base
	{
		scope=2;
		hiddenSelections[]=
		{
			"camo"
		};
		hiddenSelectionsTextures[]=
		{
			"RadioTower\RadioTower\Data\Lootcrate\data\lootcrate_yellow_co.paa"
		};
	};
	class RTDebug_Lootcrate: RTLootcrate_Base
	{
		scope=2;
		displayName="Debug supply crate";
		descriptionShort="DEBUG USE ONLY! Place on the ground and press F to log coordinates and orientation.";
		itemSize[]={1,1};
		hiddenSelections[]=
		{
			"camo"
		};
		hiddenSelectionsTextures[]=
		{
			"RadioTower\RadioTower\Data\Lootcrate\data\lootcrate_yellow_co.paa"
		};
	};
};
