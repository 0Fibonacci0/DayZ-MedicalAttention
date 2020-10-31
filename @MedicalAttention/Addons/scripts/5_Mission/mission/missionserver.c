modded class MissionServer
{
	static const int MAX_CONFIG_LINES = 500;

	void ReadMedicalConfig()
	{
		FileHandle file = OpenFile("$profile:MedicalAttention/config.txt", FileMode.READ);
		if (file != 0)
		{
			for (int i = 0; i < MAX_CONFIG_LINES; i++)
			{
				string line = "";
				if (FGets(file, line) == -1)
				{
					break;
				}

				Print("[MedicalAttention] config.txt >> " + line);

				array<string> lineParts = new array<string>;
				line.Split("=", lineParts);
				if (lineParts.Count() < 2)
				{
					continue;
				}

				string strKey = lineParts.Get(0);
				strKey.ToLower();

				string strValue = lineParts.Get(1).Trim();
				int value = strValue.ToInt();

				if (strKey.Contains("tiredness"))
				{
					MAGlobals.Get().g_TirednessEnabled = (bool)value;
				}
				else if (strKey.Contains("textreports"))
				{
					MAGlobals.Get().g_TirednessReport = (bool)value;
				}
				else if (strKey.Contains("feveractive"))
				{
					MAGlobals.Get().g_FeverActive = (bool)value;
				}
				else if (strKey.Contains("testbloodtoxin"))
				{
					MAGlobals.Get().g_TestBloodToxin = (bool)value;
				}
				else if (strKey.Contains("testbloodself"))
				{
					MAGlobals.Get().g_TestBloodSelf = (bool)value;
				}
				else if (strKey.Contains("testbloodtarget"))
				{
					MAGlobals.Get().g_TestBloodTarget = (bool)value;
				}
				else if (strKey.Contains("braindisease"))
				{
					MAGlobals.Get().g_BrainDisease = (bool)value;
				}
				else if (strKey.Contains("salinebloodregen"))
				{
					MAGlobals.Get().g_SalineBloodRegen = (bool)value;
				}
				else if (strKey.Contains("sleepingdof"))
				{
					MAGlobals.Get().g_SleepingDOF = (bool)value;
				}
				else if (strKey.Contains("tiretime"))
				{
					if (value > 0)
					{
						MAGlobals.Get().g_TireTime = value;
					}
				}
				else if (strKey.Contains("energizingdrinkgain"))
				{
					if (value > 0 && value <= 100)
					{
						MAGlobals.Get().g_EnergizingDrinkGain = value;
					}
				}
				else if (strKey.Contains("eyeclosingpoint"))
				{
					if (value >= 0 && value <= 100)
					{
						MAGlobals.Get().g_TirednessYawnPoint = value;
					}
				}
				else if (strKey.Contains("zombiebleedchance"))
				{
					if (value >= 0 && value <= 100)
					{
						MAGlobals.Get().g_ZombieBleedChance = value;
					}
				}
				else if (strKey.Contains("zombiesalmonellachance"))
				{
					if (value >= 0 && value <= 100)
					{
						MAGlobals.Get().g_ZombieSalmonellaChance = value;
					}
				}
				else if (strKey.Contains("zombiearmfracturechance"))
				{
					if (value >= 0 && value <= 100)
					{
						MAGlobals.Get().g_ZombieArmFractureChance = value;
					}
				}
				else if (strKey.Contains("woundinfectionchance"))
				{
					if (value >= 0 && value <= 100)
					{
						MAGlobals.Get().g_WoundInfectionChance = value;
					}
				}
				else if (strKey.Contains("sleepingbagclass"))
				{
					if (strValue.Length() > 0)
					{
						MAGlobals.Get().g_SleepingBagClasses.Insert(strValue);
					}
				}
				else if (strKey.Contains("energizingdrinkclass"))
				{
					if (strValue.Length() > 0)
					{
						MAGlobals.Get().g_EnergizingDrinkClasses.Insert(strValue);
					}
				}
				else if (strKey.Contains("alcoholicdrinkclass"))
				{
					if (strValue.Length() > 0)
					{
						MAAlcoholicDrink drink = new MAAlcoholicDrink;
						MAGlobals.Get().g_AlcoholicDrinkClasses.Insert(strValue, drink);

						if (lineParts.Count() > 2)
						{
							strValue = lineParts.Get(2).Trim();
							drink.multiplier = strValue.ToFloat();
						}
					}
				}
				else if (strKey.Contains("protectionsuit"))
				{
					if (strValue.Length() > 0)
					{
						MAProtectionSuit suit = new MAProtectionSuit;
						MAGlobals.Get().g_ProtectionSuitClasses.Insert(strValue, suit);

						if (lineParts.Count() > 2)
						{
							strValue = lineParts.Get(2).Trim();
							suit.protection = strValue.ToInt();
						}

						if (lineParts.Count() > 3)
						{
							strValue = lineParts.Get(3).Trim();
							suit.preventsConsume = (bool)strValue.ToInt();
						}
					}
				}
				else if (strKey.Contains("declarezone"))
				{
					if (value >= 0 && value < ZoneType.COUNT && lineParts.Count() >= 5)
					{
						MAZone zone = new MAZone;

						zone.type = value;

						strValue = lineParts.Get(2).Trim();
						zone.center[0] = strValue.ToFloat();

						strValue = lineParts.Get(3).Trim();
						zone.center[2] = strValue.ToFloat();

						strValue = lineParts.Get(4).Trim();
						zone.radius = strValue.ToFloat();

						if (lineParts.Count() > 5)
						{
							strValue = lineParts.Get(5).Trim();
							zone.messageIn = strValue;
						}

						if (lineParts.Count() > 6)
						{
							strValue = lineParts.Get(6).Trim();
							zone.messageOut = strValue;
						}

						MAGlobals.Get().g_MAZones.Insert(zone);
					}
				}
				else if (strKey.Contains("declarerandomzone"))
				{
					if (value >= 0 && value < ZoneType.COUNT && lineParts.Count() >= 7)
					{
						strValue = lineParts.Get(2).Trim();
						int enableChance = strValue.ToInt();
						if (enableChance < 0 || enableChance > 100)
						{
							continue;
						}
						float chance = (float)enableChance / 100;
						if (Math.RandomFloat01() > chance)
						{
							Print("[MedicalAttention] config.txt >> NOT ACTIVE");
							continue;
						}

						MAZone randomZone = new MAZone;

						randomZone.type = value;

						strValue = lineParts.Get(3).Trim();
						randomZone.center[0] = strValue.ToFloat();

						strValue = lineParts.Get(4).Trim();
						randomZone.center[2] = strValue.ToFloat();

						strValue = lineParts.Get(5).Trim();
						float minRadius = strValue.ToFloat();

						strValue = lineParts.Get(6).Trim();
						float maxRadius = strValue.ToFloat();

						randomZone.radius = Math.RandomFloatInclusive(minRadius, maxRadius);

						Print("[MedicalAttention] config.txt >> Active with radius " + randomZone.radius.ToString());

						if (lineParts.Count() > 7)
						{
							strValue = lineParts.Get(7).Trim();
							randomZone.messageIn = strValue;
						}

						if (lineParts.Count() > 8)
						{
							strValue = lineParts.Get(8).Trim();
							randomZone.messageOut = strValue;
						}

						MAGlobals.Get().g_MAZones.Insert(randomZone);
					}
				}
				else if (strKey.Contains("toxinaccumulationtimemin"))
				{
					if (value > 0)
					{
						MAGlobals.Get().g_ToxinAccumulationTimeMin = value;
					}
				}
				else if (strKey.Contains("toxinaccumulationtimemax"))
				{
					if (value > 0)
					{
						MAGlobals.Get().g_ToxinAccumulationTimeMax = value;
					}
				}
				else if (strKey.Contains("oldbiohazarddamage"))
				{
					if (strValue.ToFloat() > 0)
					{
						MAGlobals.Get().g_BiohazardDamage = strValue.ToFloat();
					}
				}
			}

			CloseFile(file);
		}
	}

	override void OnInit()
	{
		super.OnInit();

		MAGlobals.Get().g_SleepingBagClasses = new array<string>;
		MAGlobals.Get().g_EnergizingDrinkClasses = new array<string>;
		MAGlobals.Get().g_AlcoholicDrinkClasses = new map<string, ref MAAlcoholicDrink>;
		MAGlobals.Get().g_ProtectionSuitClasses = new map<string, ref MAProtectionSuit>;
		MAGlobals.Get().g_MAZones = new array<ref MAZone>;

		ReadMedicalConfig();
	}
}