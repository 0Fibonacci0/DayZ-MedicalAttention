modded class ActionTestBloodTarget
{
	override void OnFinishProgressServer( ActionData action_data )
	{	
		super.OnFinishProgressServer(action_data);

		PlayerBase ntarget = PlayerBase.Cast( action_data.m_Target.GetObject() );

		if (MAGlobals.Get().g_TestBloodTarget)
		{
			MABloodTestReport.DisplayReport(this, action_data.m_Player, ntarget);
		}
	}
}