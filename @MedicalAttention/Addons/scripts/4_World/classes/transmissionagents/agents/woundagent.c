modded class WoundAgent
{
	override void Init()
	{
		m_Type 					= eAgents.WOUND_AGENT;
		m_Invasibility 			= 0.28;//increase the agent count by this number per second if potent enough to grow
		m_TransferabilityIn		= 1;//to the player
		m_TransferabilityOut	= 0;//from the player
		m_AntibioticsResistance = 0.5;//[0..1], 0 means antibiotics have full effect, 1 means no effect
		m_MaxCount 				= 1000;
		m_Potency = EStatLevels.GREAT;//grow when player's immune system is at this level or lower
		m_DieOffSpeed = 1;//how fast the agent dies off when not potent enough to grow(per sec)
	}

	override float GetTransferabilityIn()
	{
		m_TransferabilityIn = (float)MAGlobals.Get().g_WoundInfectionChance / 100;

		return m_TransferabilityIn;
	}
}