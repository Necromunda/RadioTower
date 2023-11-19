
class CfgPatches
{
	class RadioTower_Scripts
	{
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};

class CfgMods 
{
	class RadioTower
	{
		dir = "RadioTower";
		credits = "";
		author = "";
		creditsJson = "RadioTower/RadioTower/Data/Credits.json";
		versionPath = "RadioTower/RadioTower/Data/Version.hpp";
		inputs = "RadioTower/RadioTower/Data/Inputs.xml";
		type = "mod";
		dependencies[] =
		{
			"Game", "World", "Mission"
		};
		class defs
		{
			class imageSets
			{
				files[]= {};
			};
			class gameScriptModule
			{
				value="";
				files[] = 
				{
					"RadioTower/RadioTower/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[] = 
				{
					"RadioTower/RadioTower/scripts/4_World"
				};
			};

			class missionScriptModule 
			{
				value="";
				files[] = 
				{
					"RadioTower/RadioTower/scripts/5_Mission"
				};
			};
		};
	};
};
