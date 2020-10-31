class TirednessMdfr: ModifierBase
{
	override void Init()
	{
		m_TrackActivatedTime	= false;
		m_ID 					= MedicalAttentionModifiers.MDF_TIREDNESS;
		m_TickIntervalInactive 	= 6;//DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= 6;//DEFAULT_TICK_TIME_ACTIVE;
	}

	override protected bool ActivateCondition(PlayerBase player)
	{
		return true;
	}

	override protected void OnActivate(PlayerBase player)
	{
		//player.IncreaseDiseaseCount();
	}

	override protected void OnDeactivate(PlayerBase player)
	{
		//player.DecreaseDiseaseCount();
	}

	override protected bool DeactivateCondition(PlayerBase player)
	{
		return false;
	}

	override protected void OnTick(PlayerBase player, float deltaT)
	{
		float tiredness = player.GetSingleAgentCount(MedicalAttentionAgents.TIREDNESS);

		if (!MAGlobals.Get().g_TirednessEnabled)
		{
			if (tiredness > 0)
			{
				player.RemoveAgent(MedicalAttentionAgents.TIREDNESS);
			}
			return;
		}

		if (tiredness == 0)
		{
			player.InsertAgent(MedicalAttentionAgents.TIREDNESS, 1);
			return;
		}

		if (player.IsPlayerSleeping() || player.IsUnconscious())
		{
			return;
		}

		if (tiredness >= 1000 * ((float)(100 - MAGlobals.Get().g_TirednessYawnPoint) / 100))
		{
			if (Math.RandomFloat01() < 1 / 15)
			{
				player.Yawn();
			}
		}

		if (tiredness < 850 || player.IsInVehicle())
		{
			return;
		}

		static const float chanceModMin = 20;
		static const float chanceModMax = 600;

		float chanceMod = player.GetSingleAgentCountNormalized(MedicalAttentionAgents.TIREDNESS) * (chanceModMax - chanceModMin) + chanceModMin;
		chanceMod = chanceModMax - chanceMod + chanceModMin;

		float unconsciousChance = 1 / chanceMod;
		if (Math.RandomFloat01() < unconsciousChance)
		{
			player.SetHealth("", "Shock", 0);
		}
	}
}