modded class CholeraMdfr
{
	static const int AGENT_THRESHOLD_ACTIVATE = 33;
	static const int AGENT_THRESHOLD_DEACTIVATE = 21;

	override protected void OnTick(PlayerBase player, float deltaT)
	{
		float stomach_volume = player.m_PlayerStomach.GetStomachVolume();
		if( stomach_volume < 200 )
		{
			return;
		}

		float chance = Math.Lerp((float)1/80, (float)1/10, player.GetSingleAgentCountNormalized(eAgents.CHOLERA));

		if( Math.RandomFloat01() < chance )
		{
			SymptomBase symptom = player.GetSymptomManager().QueueUpPrimarySymptom(SymptomIDs.SYMPTOM_VOMIT);

			if( symptom )
			{ 
				symptom.SetDuration(Math.RandomIntInclusive(4,8));
			}
		}
	}
}