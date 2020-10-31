modded class ActionGiveBloodSelf
{
	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);

		if (action_data.m_Player.m_InsideBiohazardZone)
		{
			action_data.m_Player.InsertAgent(MedicalAttentionAgents.UNKNOWN_TOXINE, 150);
		}
	}
}