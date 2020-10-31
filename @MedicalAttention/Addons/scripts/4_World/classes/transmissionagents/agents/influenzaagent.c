modded class InfluenzaAgent
{
	float m_AutoInfectTime = 15;

	override void Init()
	{
		m_Type 					= eAgents.INFLUENZA;
		m_Invasibility 			= 0.277;//increase the agent count by this number per second if potent enough to grow
		m_TransferabilityIn		= 1;//to the player
		m_TransferabilityOut	= 1;//from the player
		m_MaxCount 				= 1000;
		m_Digestibility			= 0.1;
		m_AntibioticsResistance = 0;//[0..1], 0 means antibiotics have full effect, 1 means no effect
		m_TransferabilityAirOut = 1;
		m_Potency = EStatLevels.MEDIUM;//grow when player's immune system is at this level or lower
		m_DieOffSpeed = 1;//how fast the agent dies off when not potent enough to grow(per sec)
	}

	override float GetAutoinfectTime() // how long in minutes CanAutoinfectPlayer() condition must be true to trigger Autoinfect
	{
		return m_AutoInfectTime;
	}

	override void SetAutoinfectTime(float value)
	{
		m_AutoInfectTime = value;
	}
}