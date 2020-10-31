modded class Epinephrine
{
	override void OnApply(PlayerBase player)
	{
		super.OnApply(player);

		if (!MAGlobals.Get().g_TirednessEnabled)
		{
			return;
		}

		int tiredness = player.GetSingleAgentCount(MedicalAttentionAgents.TIREDNESS);
		int maxTiredness = PluginTransmissionAgents.GetAgentMaxCount(MedicalAttentionAgents.TIREDNESS);
		int penalty = 150;

		if (tiredness + penalty > maxTiredness)
		{
			penalty = maxTiredness - tiredness;
		}

		player.InsertAgent(MedicalAttentionAgents.TIREDNESS, penalty);
	}
}
