class TirednessAgent: AgentBase
{
	override void Init()
	{
		m_Type 					= MedicalAttentionAgents.TIREDNESS;
		m_Invasibility 			= 0.08;//increase the agent count by this number per second if potent enough to grow
		m_TransferabilityIn		= 0;//to the player
		m_TransferabilityOut	= 0;//from the player
		m_AntibioticsResistance = 1;//[0..1], 0 means antibiotics have full effect, 1 means no effect
		m_MaxCount 				= 1000;
		m_Potency = EStatLevels.GREAT;//grow when player's immune system is at this level or lower
		m_DieOffSpeed = 1;//how fast the agent dies off when not potent enough to grow(per sec)
	}

	override float GetInvasibility()
	{
		if (MAGlobals.Get().g_TireTime > 0)
		{
			m_Invasibility = (float)m_MaxCount / (MAGlobals.Get().g_TireTime * 60);
		}

		return m_Invasibility;
	}
}