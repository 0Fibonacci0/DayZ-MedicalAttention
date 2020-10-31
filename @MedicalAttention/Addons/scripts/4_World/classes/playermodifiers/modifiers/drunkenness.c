class DrunkennessMdfr: ModifierBase
{
	static const int AGENT_THRESHOLD_ACTIVATE = 100;
	static const int AGENT_THRESHOLD_DEACTIVATE = 20;
	override void Init()
	{
		m_TrackActivatedTime	= false;
		m_ID 					= MedicalAttentionModifiers.MDF_DRUNKENNESS;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_INACTIVE;
	}

	override protected bool ActivateCondition(PlayerBase player)
	{
		if (player.GetSingleAgentCount(MedicalAttentionAgents.DRUNKENNESS) > AGENT_THRESHOLD_ACTIVATE) 
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
		//player.IncreaseDiseaseCount();
		if (player.GetNotifiersManager()) player.GetNotifiersManager().ActivateByType(eNotifiers.NTF_PILLS);
		// copy from PainKillersMdfr
		m_Player.m_InjuryHandler.m_ForceInjuryAnimMask = m_Player.m_InjuryHandler.m_ForceInjuryAnimMask | eInjuryOverrides.PAIN_KILLERS_LVL0;
	}

	override protected void OnDeactivate(PlayerBase player)
	{
		//player.DecreaseDiseaseCount();
		if (player.GetNotifiersManager()) player.GetNotifiersManager().DeactivateByType(eNotifiers.NTF_PILLS);
		m_Player.m_InjuryHandler.m_ForceInjuryAnimMask = m_Player.m_InjuryHandler.m_ForceInjuryAnimMask & ~eInjuryOverrides.PAIN_KILLERS_LVL0;
		m_Player.m_InjuryHandler.m_ForceInjuryAnimMask = m_Player.m_InjuryHandler.m_ForceInjuryAnimMask & ~eInjuryOverrides.PAIN_KILLERS_LVL1;
	}

	override protected bool DeactivateCondition(PlayerBase player)
	{
		if(player.GetSingleAgentCount(MedicalAttentionAgents.DRUNKENNESS) < AGENT_THRESHOLD_DEACTIVATE) 
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
		float drunkenness = player.GetSingleAgentCount(MedicalAttentionAgents.DRUNKENNESS);

		if (player.IsPlayerSleeping() || player.IsUnconscious())
		{
			return;
		}

		if (drunkenness >= 250)
		{
			if (Math.RandomFloat01() < 1 / 15)
			{
				player.GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_LAUGHTER);
			}
		}

		if (drunkenness < 850 || player.IsInVehicle())
		{
			return;
		}

		if (Math.RandomFloat01() < 1 / 25)
		{
			player.SetHealth("", "Shock", 20);
		}
	}
}