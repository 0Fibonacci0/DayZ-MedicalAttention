class BiohazzardMdfr: ModifierBase
{
	override void Init()
	{
		m_TrackActivatedTime	= false;
		m_ID 					= MedicalAttentionModifiers.MDF_BIOHAZZARD;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_INACTIVE;
	}

	override protected bool ActivateCondition(PlayerBase player)
	{
		return false;
	}

	override protected void OnActivate(PlayerBase player)
	{
		if (player.GetNotifiersManager()) player.GetNotifiersManager().ActivateByType(MedicalAttentionNotifiers.NTF_POISONED);
	}

	override protected void OnDeactivate(PlayerBase player)
	{
		if (player.GetNotifiersManager()) player.GetNotifiersManager().DeactivateByType(MedicalAttentionNotifiers.NTF_POISONED);
	}

	override protected bool DeactivateCondition(PlayerBase player)
	{
		return false;
	}

	override protected void OnTick(PlayerBase player, float deltaT)
	{
		int count = player.GetSingleAgentCount(MedicalAttentionAgents.UNKNOWN_TOXINE);
		int countMax = PluginTransmissionAgents.GetAgentMaxCount(MedicalAttentionAgents.UNKNOWN_TOXINE);

		float protection = (float)player.m_SuitProtectionLevel / 100;
		if (protection > 1)
		{
			protection = 1;
		}

		float accumulationRate = 0;

		if (protection == 1)
		{
			accumulationRate = 0.278; //PluginTransmissionAgents.GetDieOffSpeed(MedicalAttentionAgents.UNKNOWN_TOXINE);
		}
		else // protection < 1
		{
			float accumulationTime = Math.Lerp(MAGlobals.Get().g_ToxinAccumulationTimeMin, MAGlobals.Get().g_ToxinAccumulationTimeMax, protection) * 60;
			accumulationRate = (float)countMax / accumulationTime + 0.278;
		}

		float amount = accumulationRate * deltaT;

		if (count + amount > countMax)
		{
			amount = countMax - count;
		}
		player.InsertAgent(MedicalAttentionAgents.UNKNOWN_TOXINE, amount);

		// Obsolete
		float damage = MAGlobals.Get().g_BiohazardDamage * (1 - protection);
		player.AddHealth("GlobalHealth", "Health" , damage * deltaT * -1);
	}
}