class CfgPatches
{
	class RadioTower_USBStick
	{
		units[]=
		{
			"USBStick"
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
	class Inventory_Base;
	class USBStick_Base: Inventory_Base
	{
		scope=0;
		displayName="";
		descriptionShort="";
		model="RadioTower\RadioTower\Data\USBStick\usbstick.p3d";
		weight=15;
		itemSize[]={1,1};
		isMeleeWeapon=1;
		rotationFlags=16;
		inventorySlot[]={};
		hiddenSelections[]=
		{
			"zbytek"
		};
		hiddenSelectionsTextures[]=
		{
			"RadioTower\RadioTower\Data\USBStick\data\usbstick_co.paa"
		};
		stackedRandom=0;
		canBeDigged=0;
		varQuantityDestroyOnMin=1;
		itemBehaviour=1;
		heavyItem=0;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
					healthLevels[]=
					{
						{
							1,
							{
								"RadioTower\RadioTower\Data\USBStick\data\usbstick.rvmat"
							}
						},
						{
							0.69999999,
							{
								"RadioTower\RadioTower\Data\USBStick\data\usbstick.rvmat"
							}
						},
						{
							0.5,
							{
								"RadioTower\RadioTower\Data\USBStick\data\usbstick_damage.rvmat"
							}
						},
						{
							0.30000001,
							{
								"RadioTower\RadioTower\Data\USBStick\data\usbstick_damage.rvmat"
							}
						},
						{
							0,
							{
								"RadioTower\RadioTower\Data\USBStick\data\usbstick_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		repairableWithKits[]={};
		repairCosts[]={};
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUpItem
				{
					soundSet="pickUpPaper_SoundSet";
					id=797;
				};
			};
		};
	};
	class USBStick: USBStick_Base
	{
		scope=0;
		displayName="USB Stick";
		descriptionShort="Plug into a server and see what happens.";
	};
};
