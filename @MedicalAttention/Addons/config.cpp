class CfgMods
{
	class MedicalAttention
	{
		dir = "MedicalAttention";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "MedicalAttention";
		credits = "Lucian";
		author = "Lucian";
		authorID = "0";
		version = "32";
		extra = 0;
		type = "mod";

		dependencies[] = {"Game", "World", "Mission"};

		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = {"Lucian/MedicalAttention/scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"Lucian/MedicalAttention/scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"Lucian/MedicalAttention/scripts/5_Mission"};
			};
		};
	};
};

class CfgPatches
{
	class MedicalAttentionScipts
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]={};
	};
};
