class UnknownToxineMdfr: ModifierBase
{
	static const int AGENT_THRESHOLD_ACTIVATE = 10;
	static const int AGENT_THRESHOLD_DEACTIVATE = 5;

	override void Init()
	{
		m_TrackActivatedTime	= false;
		m_ID 					= MedicalAttentionModifiers.MDF_UNKNOWN_TOXINE;
		m_TickIntervalInactive 	= 6;//DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= 6;//DEFAULT_TICK_TIME_INACTIVE;
	}

	override protected bool ActivateCondition(PlayerBase player)
	{
		if (player.GetSingleAgentCount(MedicalAttentionAgents.UNKNOWN_TOXINE) > AGENT_THRESHOLD_ACTIVATE) 
		{
			return true;
		}
		else 
		{
			return false;
		}
	}

	override protected void OnActivate(PlayerBase player)
	{
		player.IncreaseDiseaseCount();
	}

	override protected void OnDeactivate(PlayerBase player)
	{
		player.DecreaseDiseaseCount();
	}

	override protected bool DeactivateCondition(PlayerBase player)
	{
		if(player.GetSingleAgentCount(MedicalAttentionAgents.UNKNOWN_TOXINE) < AGENT_THRESHOLD_DEACTIVATE) 
		{
			return true;
		}
		else 
		{
			return false;
		}
	}

	override protected void OnTick(PlayerBase player, float deltaT)
	{
		float chance = Math.Lerp((float)1/25, (float)1/10, player.GetSingleAgentCountNormalized(MedicalAttentionAgents.UNKNOWN_TOXINE));

		if (Math.RandomFloat01() < chance)
		{
			SymptomBase symptom = player.GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_VOMIT);

			if( symptom )
			{
				symptom.SetDuration(2);
				
				int count = player.GetSingleAgentCount(MedicalAttentionAgents.UNKNOWN_TOXINE);
				int countMax = PluginTransmissionAgents.GetAgentMaxCount(MedicalAttentionAgents.UNKNOWN_TOXINE);

				player.AddHealth("","Blood", (float)-300 * count / countMax);
			}
		}
	}
}