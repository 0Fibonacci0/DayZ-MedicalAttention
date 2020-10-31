modded class AgentBase
{
	float m_AutoinfectElapsedTime = 0;

	float GetAutoinfectTime() // how long in minutes CanAutoinfectPlayer() condition must be true to trigger Autoinfect
	{
		return -1;
	}

	void SetAutoinfectTime(float value)
	{
	}

	override bool AutoinfectCheck(float deltaT, PlayerBase player)
	{
		if ( GetAutoinfectTime() == -1 )
			return false;

		m_AutoinfectElapsedTime += deltaT;

		if (!CanAutoinfectPlayer(player))
			m_AutoinfectElapsedTime = 0;

		if (m_AutoinfectElapsedTime >= GetAutoinfectTime() * 60)
		{
			SetAutoinfectTime(-1); // don't Autoinfect again
			return true;
		}

		return false;
	}
}