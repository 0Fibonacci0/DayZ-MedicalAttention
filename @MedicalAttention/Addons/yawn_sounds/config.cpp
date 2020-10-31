class CfgPatches
{
	class MAYawnSounds
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Sounds_Effects"
		};
	};
};

class CfgSoundShaders
{
	class baseCharacter_SoundShader;
	class yawn_SoundShader: baseCharacter_SoundShader
	{
		samples[]=
		{
			{
				"Lucian\MedicalAttention\yawn_sounds\yawn",
				1
			}
		};
		volume=1;
	};
};

class CfgSoundSets
{
	class baseCharacter_SoundSet;
	class yawn_SoundSet: baseCharacter_SoundSet
	{
		soundShaders[]=
		{
			"yawn_SoundShader"
		};
	};
};
