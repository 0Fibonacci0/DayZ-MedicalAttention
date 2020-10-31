modded class SalmonellaMdfr
{
	int m_AllowedVomitLevel = 0;
	int m_AllowedVomitLevelDelta = 0;

	private void CalculateAllowedVomitLevel(int salmonella, int salmonellaMax)
	{
		m_AllowedVomitLevelDelta = (salmonellaMax - salmonella) / 3;
		m_AllowedVomitLevel = salmonella + m_AllowedVomitLevelDelta;
	}

	static const int AGENT_THRESHOLD_ACTIVATE = 33;
	static const int AGENT_THRESHOLD_DEACTIVATE = 21;

	override void Init()
	{
		m_TrackActivatedTime			= false;
		m_ID 					= eModifiers.MDF_SALMONELLA;
		m_TickIntervalInactive 	= 6;//DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= 6;//DEFAULT_TICK_TIME_ACTIVE;
	}

	override protected void OnTick(PlayerBase player, float deltaT)
	{
		int salmonella = player.GetSingleAgentCount(eAgents.SALMONELLA);
		int salmonellaMax = PluginTransmissionAgents.GetAgentMaxCount(eAgents.SALMONELLA);

		if (salmonella + m_AllowedVomitLevelDelta < m_AllowedVomitLevel)
		{
			CalculateAllowedVomitLevel(salmonella, salmonellaMax);
		}

		if ((salmonella >= (float)salmonellaMax * 0.9 || salmonella > m_AllowedVomitLevel) && Math.RandomFloat01() < 1 / 25)
		{
			CalculateAllowedVomitLevel(salmonella, salmonellaMax);

			SymptomBase symptom = player.GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_VOMIT);

			if( symptom )
			{
				symptom.SetDuration(5);
				if (!player.GetModifiersManager().IsModifierActive(eModifiers.MDF_SALINE))
				{
					player.GetStatWater().Add(-300);
				}
			}
		}
	}
}