modded class ActionCollectBloodTarget
{
	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);

		PlayerBase ntarget = PlayerBase.Cast(action_data.m_Target.GetObject());
		if (ntarget.m_InsideBiohazardZone)
		{
			ntarget.InsertAgent(MedicalAttentionAgents.UNKNOWN_TOXINE, 150);
		}
	}
}