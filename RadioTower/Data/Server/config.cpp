class CfgPatches
{
	class RadioTower
	{
		units[]=
		{
			"RTServer",
			"RTDebug_Server"
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
	class RTServer_Base: Container_Base
	{
		scope=0;
		displayName="";
		descriptionShort="";
		model="RadioTower\RadioTower\Data\Server\server.p3d";
		rotationFlags=2;
		itemBehaviour=0;
		class AnimationSources
		{
			class Spin
			{
				source="user";
				initPhase=0;
				animPeriod=1;
			};
		};
		soundImpactType="metal";
		physLayer="item_large";
		// slopeTolerance=0.40000001;
		// yawPitchRollLimit[]={45,45,45};
		// weight=45000;
		// itemSize[]={10,10};
		// attachments[]=
		// {
			// "CarBattery",
			// "custom_metal_slot"
		// }; 
		// carveNavmesh=1;
		// heavyItem=1;
		// hiddenSelections[]={};
		// hiddenSelectionsTextures[]={};
		// hiddenSelectionsMaterials[]={};
		// hologramMaterial="power_generator";
		// hologramMaterialPath="dz\gear\camping\data";
		// physLayer="item_large";
		// class DamageSystem
		// {
			// class GlobalHealth
			// {
				// class Health
				// {
					// hitpoints=100;
					// healthLevels[]=
					// {
						// {
							// 1,
							// {
								// "RadioTower\RadioTower\Data\Server\data\servers.rvmat",
								// "RadioTower\RadioTower\Data\Server\data\server_rack.rvmat",
								// "RadioTower\RadioTower\Data\Server\data\server_console.rvmat"
							// }
						// },
						// {
							// 0.69999999,
							// {
								// "RadioTower\RadioTower\Data\Server\data\servers.rvmat",
								// "RadioTower\RadioTower\Data\Server\data\server_rack.rvmat",
								// "RadioTower\RadioTower\Data\Server\data\server_console.rvmat"
							// }
						// },
						// {
							// 0.5,
							// {
								// "RadioTower\RadioTower\Data\Server\data\servers.rvmat",
								// "RadioTower\RadioTower\Data\Server\data\server_rack.rvmat",
								// "RadioTower\RadioTower\Data\Server\data\server_console.rvmat"
							// }
						// },
						// {
							// 0.30000001,
							// {
								// "RadioTower\RadioTower\Data\Server\data\servers.rvmat",
								// "RadioTower\RadioTower\Data\Server\data\server_rack.rvmat",
								// "RadioTower\RadioTower\Data\Server\data\server_console.rvmat"
							// }
						// },
						// {
							// 0,
							// {
								// "RadioTower\RadioTower\Data\Server\data\servers.rvmat",
								// "RadioTower\RadioTower\Data\Server\data\server_rack.rvmat",
								// "RadioTower\RadioTower\Data\Server\data\server_console.rvmat"
							// }
						// }
					// };
				// };
			// };
		// };
		// soundImpactType="metal";
		// class AnimEvents
		// {
			// class SoundWeapon
			// {
				// class walk
				// {
					// soundSet="powergenerator_movement_walk_SoundSet";
					// id=1;
				// };
				// class pickUpItem_Light
				// {
					// soundSet="pickUpPowerGenerator_SoundSet";
					// id=796;
				// };
				// class pickUpItem
				// {
					// soundSet="pickUpPowerGenerator_SoundSet";
					// id=797;
				// };
				// class drop
				// {
					// soundset="powergenerator_drop_SoundSet";
					// id=898;
				// };
			// };
		// };
	};
	class RTServer: RTServer_Base
	{
		scope=2;
		displayName="Server";
		descriptionShort="Hack the server to start the event.";
	};
	class RTDebug_Server: RTServer_Base
	{
		scope=2;
		displayName="Debug Server";
		descriptionShort="DEBUG USE ONLY! Place on the ground and press F to log coordinates and orientation.";
	};
};
