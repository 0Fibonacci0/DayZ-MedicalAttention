modded class PlayerBase
{
	private ref Param1<string> m_TirednessReportParam;
	private int m_TirednessRCountdown = 0;

	bool m_TirednessEnabled = true;
	bool m_SleepingBagAttached = false;
	bool m_WasSleeping = false;
	bool m_IsSleeping = false;
	int m_SuitProtectionLevel = 0;
	int m_SuitPreventsConsume = 0;
	bool m_InsideNonCombatZone = false;
	bool m_InsideBiohazardZone = false;
	bool m_InsideNoWellsZone = false;
	bool m_InsideNoFuelZone = false;

	private int m_Tiredness = 0;
	private int m_EnergizingDrinkAllowedRest = 0;
	private int m_EnergizingDrinkAllowedRestMax = 0;

	static const int MAX_TIREDNESS = 1000;
	static const float REST_GAIN_MIN = 1;
	static const float REST_GAIN_PER_SEC = 0.023;

	private float m_PlayerRestTick = 1;
	private float m_AccumulatedRest = 1;//REST_GAIN_MIN

	private float m_FastRingTick = 0;
	private float m_SlowRingTick = 0;
	static const int ZONE_TYPES_COUNT = ZoneType.COUNT;
	private int m_ActiveZones[ZONE_TYPES_COUNT];

	private int m_HeatSource = 0;
	static const int HEAT_SOURCE_COUNTDOWN = 5;

	private PluginPlayerStatus m_Mod_ModulePlayerStatus;

	private int m_LastYawn = 0;
	private int m_CurrentYawn = 0;
	private float m_YawnTime = 0;

	static const float YAWN_TIME_1 = 5;
	static const float YAWN_TIME_2 = 6;
	static const float YAWN_TIME_3 = 6.3;

	private ref array<ref MAZoneInstance> m_FastRingZones = NULL;
	private ref array<ref MAZoneInstance> m_SlowRingZones = NULL;
	private int m_SlowRingCursor = 0;

	bool m_BrokenLegs = false;
	bool m_BrokenArms = false;

	void PlayerBase()
	{
		m_TirednessReportParam = new Param1<string>("");
		m_FastRingZones = new array<ref MAZoneInstance>;
		m_SlowRingZones = new array<ref MAZoneInstance>;

		m_EnergizingDrinkAllowedRestMax = MAX_TIREDNESS * ((float)MAGlobals.Get().g_EnergizingDrinkGain / 100);
		m_EnergizingDrinkAllowedRest = m_EnergizingDrinkAllowedRestMax;
	}

	bool IsPlayerSleeping()
	{
		bool isSleeping = true;
		if (!m_EmoteManager || !m_EmoteManager.m_bEmoteIsPlaying || !m_EmoteManager.m_Callback || m_EmoteManager.m_Callback.m_callbackID != DayZPlayerConstants.CMD_GESTUREFB_LYINGDOWN)
		{
			isSleeping = false;
		}

		if (MAGlobals.Get().g_SleepingDOF)
		{
			m_IsSleeping = isSleeping;
		}

		return isSleeping;
	}

	bool IsPlayerNearbySleepingBag()
	{
		if (MAGlobals.Get().g_SleepingBagClasses.Count() == 0)
		{
			return false;
		}

		array<Object> nearest_objects = new array<Object>;
		array<CargoBase> proxy_cargos = new array<CargoBase>;

		GetGame().GetObjectsAtPosition(GetPosition(), 0.5, nearest_objects, proxy_cargos);

		for (int i = 0; i < nearest_objects.Count(); i++)
		{
			Object nearest_object = nearest_objects.Get(i);

			if (nearest_object != this && nearest_object.IsInherited(ItemBase) && nearest_object.GetParent() == null ) // not in player possession
			{
				string configClass = nearest_object.GetType();

				if (MAGlobals.Get().g_SleepingBagClasses.Find(configClass) >= 0)
				{
					return true;
				}
			}
		}
		
		return false;
	}

	void Yawn()
	{
		m_CurrentYawn++;
		if (m_CurrentYawn > MAX_TIREDNESS)
		{
			m_CurrentYawn = 0;
		}
	}

	private void PlayerYawnCheck()
	{
		if (m_CurrentYawn != m_LastYawn)
		{
			m_LastYawn = m_CurrentYawn;
			if (m_YawnTime == 0)
			{
				m_YawnTime = 0.01;

				EffectSound yawnSound;
				PlaySoundSet(yawnSound, "yawn_SoundSet", 0, 0 );
			}
		}
	}

	private void SendMessageToClient(Object reciever, string message)
	{
		PlayerBase man;
		if (GetGame().IsServer() && Class.CastTo(man, reciever) && m_TirednessReportParam && reciever.IsAlive() && message != "")
		{
			m_TirednessReportParam.param1 = message;
			GetGame().RPCSingleParam(man, ERPCs.RPC_USER_ACTION_MESSAGE, m_TirednessReportParam, true, man.GetIdentity());
		}
	}

	private void PlayerRestCheck(float deltaTime)
	{
		static const float PLAYER_REST_TICK = 3;

		m_PlayerRestTick += deltaTime;

		if (m_PlayerRestTick < PLAYER_REST_TICK)
		{
			return;
		}

		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			m_Tiredness = GetSingleAgentCount(MedicalAttentionAgents.TIREDNESS);

			if (!IsPlayerSleeping())
			{
				m_PlayerRestTick = 0;
				m_AccumulatedRest = REST_GAIN_MIN;
				m_TirednessRCountdown = 0;
				return;
			}

			SetSynchDirty();

			if (MAGlobals.Get().g_TirednessReport)
			{
				m_TirednessRCountdown--;
				if (m_TirednessRCountdown <= 0)
				{
					m_TirednessRCountdown = 5;
					int percent = 100 - (float)m_Tiredness / MAX_TIREDNESS * 100;
					SendMessageToClient(this, "Your rest level is " + percent.ToString() + "%");
				}
			}

			bool heatSource = m_HeatSource > 0 || m_SleepingBagAttached || IsPlayerNearbySleepingBag();

			if (m_HeatSource > 0)
			{
				m_HeatSource--;
			}

			if (m_AccumulatedRest > REST_GAIN_MIN + REST_GAIN_PER_SEC * 120) // if player is sleeping for 2 mins
			{
				m_EnergizingDrinkAllowedRest = m_EnergizingDrinkAllowedRestMax;

				if (!heatSource) // and no heat source then influenza
				{
					InsertAgent(eAgents.INFLUENZA, 1);
				}
			}

			if (m_Tiredness < (MAX_TIREDNESS / 2) && !heatSource) // if no heat source then cap resting at 50%
			{
				m_PlayerRestTick = 0;
				return;
			}

			// old way from Environment.CollectAndSetEnvironmentData()
			/*
			m_DayOrNight = (-1 * Math.AbsFloat( ( 1 - ( g_Game.GetDayTime() / GameConstants.ENVIRO_HIGH_NOON ) ) ) ) + 1;
			if (m_Tiredness < (MAX_TIREDNESS / 4) && m_DayOrNight >= 0.5)
			*/

			if (m_Tiredness < (MAX_TIREDNESS / 4) && !GetGame().GetWorld().IsNight()) // if heat source, but not night then cap resting at 75%
			{
				m_PlayerRestTick = 0;
				return;
			}

			m_AccumulatedRest += (REST_GAIN_PER_SEC * m_PlayerRestTick);
			int rest = m_AccumulatedRest * m_PlayerRestTick;

			if (rest > m_Tiredness)
			{
				rest = m_Tiredness;
			}

			InsertAgent(MedicalAttentionAgents.TIREDNESS, rest * -1);

			m_Tiredness -= rest;
		}
		else //if (!GetGame().IsMultiplayer() || GetGame().IsClient())
		{
			if (m_Mod_ModulePlayerStatus)
			{
				m_Mod_ModulePlayerStatus.SetTiredness(MAX_TIREDNESS - m_Tiredness, MAX_TIREDNESS);
			}

			if (m_IsSleeping != m_WasSleeping)
			{
				m_WasSleeping = m_IsSleeping;

				if (m_IsSleeping)
				{
					//PPEffects.OverrideDOF(true, 4, 700, 20, 10, 1);
					PPEffects.EnableBurlapSackBlindness();
				}
				else
				{
					//PPEffects.ResetDOFOverride();
					PPEffects.DisableBurlapSackBlindness();
				}
			}

			PlayerYawnCheck();
		}

		m_PlayerRestTick = 0;
	}

	private int ActivateDeactivateZone(MAZoneInstance zone, vector playerPosition)
	{
		static const float SAFE_BORDER = 100; // meters

		float distance = vector.Distance(playerPosition, zone.center);

		bool activate = false;
		int returnVal = 1; // stay in fast ring

		if (distance < zone.radius)
		{
			activate = true;

			if (distance + SAFE_BORDER < zone.radius)
			{
				returnVal = 2; // stay in slow ring
			}
		}
		else
		{
			activate = false;
			
			if (distance - SAFE_BORDER >= zone.radius)
			{
				returnVal = 2; // stay in slow ring
			}
		}

		if (zone.active != activate)
		{
			zone.active = activate;
		}
		else
		{
			return returnVal;
		}

		if (activate)
		{
			if (m_ActiveZones[zone.type] == 0)
			{
				if (zone.type == ZoneType.BIO_HAZZARD)
				{
					m_InsideBiohazardZone = true;
					GetModifiersManager().ActivateModifier(MedicalAttentionModifiers.MDF_BIOHAZZARD);
				}
				else if (zone.type == ZoneType.NON_COMBAT)
				{
					m_InsideNonCombatZone = true;
				}
				else if (zone.type == ZoneType.NO_WELLS)
				{
					m_InsideNoWellsZone = true;
				}
				else if (zone.type == ZoneType.NO_FUEL)
				{
					m_InsideNoFuelZone = true;
				}

				if (zone.messageIn.Length() > 0)
				{
					SendMessageToClient(this, zone.messageIn);
				}
			}

			m_ActiveZones[zone.type] = m_ActiveZones[zone.type] + 1;
		}
		else
		{
			m_ActiveZones[zone.type] = m_ActiveZones[zone.type] - 1;

			if (m_ActiveZones[zone.type] == 0)
			{
				if (zone.type == ZoneType.BIO_HAZZARD)
				{
					m_InsideBiohazardZone = false;
					GetModifiersManager().DeactivateModifier(MedicalAttentionModifiers.MDF_BIOHAZZARD);
				}
				else if (zone.type == ZoneType.NON_COMBAT)
				{
					m_InsideNonCombatZone = false;
				}
				else if (zone.type == ZoneType.NO_WELLS)
				{
					m_InsideNoWellsZone = false;
				}
				else if (zone.type == ZoneType.NO_FUEL)
				{
					m_InsideNoFuelZone = false;
				}

				if (zone.messageOut.Length() > 0)
				{
					SendMessageToClient(this, zone.messageOut);
				}
			}
		}

		return returnVal;
	}

	private void PlayerZoneCheck(float deltaTime)
	{
		static const float FAST_RING_TICK = 1;
		static const float SLOW_RING_TICK = 7;

		m_FastRingTick += deltaTime;
		m_SlowRingTick += deltaTime;

		if (m_FastRingTick < FAST_RING_TICK)
		{
			return;
		}

		m_FastRingTick = 0;

		if (!GetGame().IsServer() || !GetGame().IsMultiplayer())
		{
			return;
		}

		vector playerPos = GetPosition();
		playerPos[1] = 0;
		MAZoneInstance zone = NULL;

		int i = 0;
		int ringCount = m_FastRingZones.Count();
		while (i < ringCount)
		{
			zone = m_FastRingZones.Get(i);
			if (ActivateDeactivateZone(zone, playerPos) == 2)
			{
				m_FastRingZones.Remove(i);
				ringCount = m_FastRingZones.Count();

				m_SlowRingZones.Insert(zone);

				continue;
			}

			i++;
		}

		if (m_SlowRingTick < SLOW_RING_TICK)
		{
			return;
		}

		m_SlowRingTick = 0;

		i = 0;
		ringCount = m_SlowRingZones.Count();
		while (i < ringCount)
		{
			zone = m_SlowRingZones.Get(i);
			if (ActivateDeactivateZone(zone, playerPos) == 1)
			{
				m_SlowRingZones.Remove(i);
				ringCount = m_SlowRingZones.Count();

				m_FastRingZones.Insert(zone);

				continue;
			}

			i++;
		}
	}

	override void Init()
	{
		super.Init();

		RegisterNetSyncVariableInt("m_Tiredness", 0, MAX_TIREDNESS);
		RegisterNetSyncVariableBool("m_TirednessEnabled");
		RegisterNetSyncVariableInt("m_CurrentYawn", 0, MAX_TIREDNESS);
		RegisterNetSyncVariableInt("m_SuitPreventsConsume", 0, MAX_TIREDNESS);
		RegisterNetSyncVariableBool("m_InsideNoWellsZone");
		RegisterNetSyncVariableBool("m_InsideNoFuelZone");
		RegisterNetSyncVariableBool("m_IsSleeping");
		RegisterNetSyncVariableBool("m_BrokenLegs");
		RegisterNetSyncVariableBool("m_BrokenArms");

		m_Mod_ModulePlayerStatus = PluginPlayerStatus.Cast(GetPlugin(PluginPlayerStatus));
	}

	override void OnConnect()
	{
		super.OnConnect();

		Print("[MedicalAttention] OnConnect()");
		m_TirednessEnabled = MAGlobals.Get().g_TirednessEnabled;

		m_FastRingZones.Clear(); // just in case OnConnect() would ever be called on the same PlayerBase object twice
		m_SlowRingZones.Clear();
		int i = 0;
		for (i = 0; i < MAGlobals.Get().g_MAZones.Count(); i++)
		{
			MAZone zone = MAGlobals.Get().g_MAZones.Get(i);
			MAZoneInstance zoneInstance = new MAZoneInstance;

			zoneInstance.center = zone.center;
			zoneInstance.radius = zone.radius;
			zoneInstance.type = zone.type;
			zoneInstance.messageIn = zone.messageIn;
			zoneInstance.messageOut = zone.messageOut;
			zoneInstance.active = false;

			m_SlowRingZones.Insert(zoneInstance);
		}
		for (i = 0; i < ZONE_TYPES_COUNT; i++)
		{
			m_ActiveZones[i] = 0;
		}
	}

	override void AddToEnvironmentTemperature(float pTemperature)
	{
		super.AddToEnvironmentTemperature(pTemperature);
		
		m_HeatSource = HEAT_SOURCE_COUNTDOWN;
	}

	override void OnScheduledTick(float deltaTime)
	{
		super.OnScheduledTick(deltaTime);

		PlayerZoneCheck(deltaTime);

		if (!m_TirednessEnabled)
		{
			return;
		}

		PlayerRestCheck(deltaTime);
	}

	void HandleEnergizingDrink(ItemBase source, float amount)
	{
		if (!MAGlobals.Get().g_TirednessEnabled)
		{
			return;
		}

		string configClass = source.GetType();
		if (MAGlobals.Get().g_EnergizingDrinkClasses.Find(configClass) < 0)
		{
			return;
		}

		Print("[MedicalAttention] HandleEnergizingDrink " + configClass);

		m_Tiredness = GetSingleAgentCount(MedicalAttentionAgents.TIREDNESS);

		int rest = 0;
		if (source.GetQuantityMax() > 0)
		{
			rest = 1 + amount * ((float)m_EnergizingDrinkAllowedRestMax / source.GetQuantityMax());
		}

		if (rest == 0 || m_EnergizingDrinkAllowedRest <= 0)
		{
			return;
		}

		if (rest > m_EnergizingDrinkAllowedRest)
		{
			rest = m_EnergizingDrinkAllowedRest;
		}

		m_EnergizingDrinkAllowedRest -= rest;

		if (rest > m_Tiredness)
		{
			rest = m_Tiredness;
		}

		InsertAgent(MedicalAttentionAgents.TIREDNESS, rest * -1);

		m_Tiredness -= rest;
	}

	void HandleAlcoholicDrink(ItemBase source, float amount)
	{
		string configClass = source.GetType();
		MAAlcoholicDrink drink = NULL;
		if (!MAGlobals.Get().g_AlcoholicDrinkClasses.Find(configClass, drink))
		{
			return;
		}

		Print("[MedicalAttention] HandleAlcoholicDrink " + configClass);

		int drunkenness = GetSingleAgentCount(MedicalAttentionAgents.DRUNKENNESS);
		int drunkennessMax = PluginTransmissionAgents.GetAgentMaxCount(MedicalAttentionAgents.DRUNKENNESS);

		amount *= drink.multiplier;

		if (drunkenness + amount > drunkennessMax)
		{
			amount = drunkennessMax - drunkenness;
		}

		InsertAgent(MedicalAttentionAgents.DRUNKENNESS, amount);
	}

	void HandleBiohazardConsume(float amount)
	{
		if (amount <= 0 || !m_InsideBiohazardZone)
		{
			return;
		}

		int toxin = GetSingleAgentCount(MedicalAttentionAgents.UNKNOWN_TOXINE);
		int toxinMax = PluginTransmissionAgents.GetAgentMaxCount(MedicalAttentionAgents.UNKNOWN_TOXINE);

		amount *= 2;

		if (toxin + amount > toxinMax)
		{
			amount = toxinMax - toxin;
		}

		InsertAgent(MedicalAttentionAgents.UNKNOWN_TOXINE, amount);
	}

	override bool Consume(ItemBase source, float amount, EConsumeType consume_type)
	{
		if (source)
		{
			HandleEnergizingDrink(source, amount);
			HandleAlcoholicDrink(source, amount);
		}

		HandleBiohazardConsume(amount);

		return super.Consume(source, amount, consume_type);
	}

	override void CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)	
	{
		super.CommandHandler(pDt,pCurrentCommandID,pCurrentCommandFinished);

		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			return;
		}

		if (m_IsUnconscious)
		{
			m_YawnTime = 0;
			return;
		}

		if (m_YawnTime == 0)
		{
			return;
		}

		m_YawnTime += pDt;
		if (m_YawnTime < YAWN_TIME_1)
		{
			PPEffects.SetVignette(Math.Lerp(0, 2, m_YawnTime / YAWN_TIME_1), 0, 0, 0);
		}
		else if (m_YawnTime < YAWN_TIME_2)
		{
			// wait in darkness
		}
		else if (m_YawnTime < YAWN_TIME_3)
		{
			PPEffects.SetVignette(Math.Lerp(2, 0, (m_YawnTime - YAWN_TIME_2) / (YAWN_TIME_3 - YAWN_TIME_2)), 0, 0, 0);
		}
		else
		{
			PPEffects.ResetVignette();
			m_YawnTime = 0;
		}
	}

	void BreakTheLegs()
	{
		if (GetModifiersManager().IsModifierActive(eModifiers.MDF_BROKEN_LEGS)) //effectively resets the timer
		{
			GetModifiersManager().DeactivateModifier(eModifiers.MDF_BROKEN_LEGS);
		}
		GetModifiersManager().ActivateModifier(eModifiers.MDF_BROKEN_LEGS);

		if (GetModifiersManager().IsModifierActive(eModifiers.MDF_BONE_REGEN))
		{
			GetModifiersManager().DeactivateModifier(eModifiers.MDF_BONE_REGEN);
		}
	}

	void BreakTheArms()
	{
		if (GetModifiersManager().IsModifierActive(eModifiers.MDF_BROKEN_ARMS))
		{
			GetModifiersManager().DeactivateModifier(eModifiers.MDF_BROKEN_ARMS);
		}
		GetModifiersManager().ActivateModifier(eModifiers.MDF_BROKEN_ARMS);
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		if (damageType == DT_CUSTOM && ammo == "FallDamage" && GetHealth("", "") < 70)
		{
			// fall
			BreakTheLegs();
		}
		else if (!damageResult && Math.RandomFloat01() < (float)MAGlobals.Get().g_ZombieArmFractureChance / 100)
		{
			BreakTheArms();
		}
		else if (damageResult)
		{
			float dmg = damageResult.GetHighestDamage("Health");

			if ((dmgZone == "LeftLeg" || dmgZone == "RightLeg" || dmgZone == "LeftFoot" || dmgZone == "RightFoot"))
			{
				if (source && source.IsMeleeWeapon() && dmg >= 30)
				{
					// melee
					BreakTheLegs();
				}
				else if (dmg >= 50)
				{
					// anything else
					BreakTheLegs();
				}
			}
			else if ((dmgZone == "LeftHand" || dmgZone == "RightHand" || dmgZone == "LeftArm" || dmgZone == "RightArm"))
			{
				if (source && source.IsMeleeWeapon() && dmg >= 30)
				{
					// melee
					BreakTheArms();
				}
				else if (dmg >= 50)
				{
					// anything else
					BreakTheArms();
				}
			}
		}

		if (source)
		{
			Man man = source.GetHierarchyRootPlayer();
			if (!man)
			{
				return;
			}

			PlayerBase attacker = PlayerBase.Cast(man);
			if (attacker && (attacker.m_InsideNonCombatZone || m_InsideNonCombatZone))
			{
				if (damageType == DT_CLOSE_COMBAT) // 0
				{
					attacker.SetHealth("", "Shock", 20);
				}
				else if (damageType == DT_FIRE_ARM) // 1
				{
					attacker.SetHealth("", "", 0);
				}
			}
		}
	}

	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);

		if (!GetGame().IsServer() || !GetGame().IsMultiplayer() || !item)
		{
			return;
		}

		string configClass = item.GetType();

		if (m_TirednessEnabled)
		{
			if (MAGlobals.Get().g_SleepingBagClasses.Find(configClass) >= 0)
			{
				m_SleepingBagAttached = true;
				Print("[MedicalAttention] EEItemAttached " + configClass);
			}
		}

		MAProtectionSuit suit = NULL;
		if (MAGlobals.Get().g_ProtectionSuitClasses.Find(configClass, suit))
		{
			m_SuitProtectionLevel += suit.protection;
			if (suit.preventsConsume)
			{
				m_SuitPreventsConsume++;
			}
			Print("[MedicalAttention] EEItemAttached " + configClass);
		}
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		if (!GetGame().IsServer() || !GetGame().IsMultiplayer() || !item)
		{
			return;
		}

		string configClass = item.GetType();

		if (m_TirednessEnabled)
		{
			if (MAGlobals.Get().g_SleepingBagClasses.Find(configClass) >= 0)
			{
				m_SleepingBagAttached = false;
				Print("[MedicalAttention] EEItemDetached " + configClass);
			}
		}

		MAProtectionSuit suit = NULL;
		if (MAGlobals.Get().g_ProtectionSuitClasses.Find(configClass, suit))
		{
			m_SuitProtectionLevel -= suit.protection;
			if (suit.preventsConsume)
			{
				m_SuitPreventsConsume--;
				SetSynchDirty();
			}
			Print("[MedicalAttention] EEItemDetached " + configClass);
		}
	}

	override bool CanSprint()
	{
		if (m_BrokenLegs)
		{
			return false;
		}

		return super.CanSprint();
	}

	override bool CanClimb(int climbType, SHumanCommandClimbResult climbRes)
	{
		if (m_BrokenLegs || m_BrokenArms)
		{
			return false;
		}

		return super.CanClimb(climbType, climbRes);
	}

	override bool CanJump()
	{
		if (m_BrokenLegs)
		{
			return false;
		}

		return super.CanJump();
	}

	override int GetShakeLevel()
	{
		if (m_BrokenArms)
		{
			return 7;
		}
		else
		{
			return super.GetShakeLevel();
		}
	}

	override void ApplySplint()
	{
		if (GetModifiersManager().IsModifierActive(eModifiers.MDF_BONE_REGEN)) //effectively resets the timer
		{
			GetModifiersManager().DeactivateModifier(eModifiers.MDF_BONE_REGEN);
		}
		GetModifiersManager().ActivateModifier(eModifiers.MDF_BONE_REGEN);
	}
}
