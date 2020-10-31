class MASettings: Managed
{
	bool g_TirednessEnabled = true;
	int g_TireTime = 0;
	int g_TirednessYawnPoint = 18; // percent of rest
	bool g_TirednessReport = true;
	int g_EnergizingDrinkGain = 10; // rest gain percent from MAX_TIREDNESS
	int g_ZombieBleedChance = 20;
	int g_ZombieSalmonellaChance = 50;
	int g_ZombieArmFractureChance = 33;
	int g_WoundInfectionChance = 50;
	bool g_FeverActive = true;
	bool g_TestBloodSelf = true;
	bool g_TestBloodTarget = true;
	bool g_TestBloodToxin = true;
	bool g_BrainDisease = true;
	bool g_SalineBloodRegen = false;
	bool g_SleepingDOF = false;

	float g_BiohazardDamage = 0;
	int g_ToxinAccumulationTimeMin = 5;
	int g_ToxinAccumulationTimeMax = 70;

	ref array<string> g_SleepingBagClasses = NULL;
	ref array<string> g_EnergizingDrinkClasses = NULL;
	ref map<string, ref MAAlcoholicDrink> g_AlcoholicDrinkClasses = NULL;
	ref map<string, ref MAProtectionSuit> g_ProtectionSuitClasses = NULL;
	ref array<ref MAZone> g_MAZones = NULL;
}

class MAGlobals
{
	private static ref MASettings m_Globals = NULL;
	
	static MASettings Get()
	{
		if (m_Globals == NULL)
		{
			m_Globals = new MASettings;
		}
		return m_Globals;
	}
}
