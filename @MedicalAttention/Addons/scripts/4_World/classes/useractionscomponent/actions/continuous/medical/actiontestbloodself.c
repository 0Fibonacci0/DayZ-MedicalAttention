class MABloodTestReport
{
	static void DisplayReport(ActionBase action, PlayerBase player, PlayerBase target)
	{
		EStatLevels immunityLevel = target.GetImmunityLevel();
		string immunityLevelStr;
		switch (immunityLevel)
		{
			case EStatLevels.GREAT: immunityLevelStr = "GREAT"; break;
			case EStatLevels.HIGH: immunityLevelStr = "HIGH"; break;
			case EStatLevels.MEDIUM: immunityLevelStr = "MEDIUM"; break;
			case EStatLevels.LOW: immunityLevelStr = "LOW"; break;
			case EStatLevels.CRITICAL: immunityLevelStr = "CRITICAL"; break;
		}
		action.SendMessageToClient(player, "Medical Report:");
		action.SendMessageToClient(player, "Immunity Level: " + immunityLevelStr);
		action.SendMessageToClient(player, "Alcohol: " + target.GetSingleAgentCount(MedicalAttentionAgents.DRUNKENNESS).ToString());
		action.SendMessageToClient(player, "Cholera: " + target.GetSingleAgentCount(eAgents.CHOLERA).ToString());
		action.SendMessageToClient(player, "Salmonella: " + target.GetSingleAgentCount(eAgents.SALMONELLA).ToString());
		action.SendMessageToClient(player, "Influenza: " + target.GetSingleAgentCount(eAgents.INFLUENZA).ToString());
		action.SendMessageToClient(player, "Food Poison: " + target.GetSingleAgentCount(eAgents.FOOD_POISON).ToString());
		action.SendMessageToClient(player, "Wound Infection: " + target.GetSingleAgentCount(eAgents.WOUND_AGENT).ToString());

		if (MAGlobals.Get().g_TestBloodToxin)
		{
			action.SendMessageToClient(player, "Unknown Toxin: " + target.GetSingleAgentCount(MedicalAttentionAgents.UNKNOWN_TOXINE).ToString());
		}
	}
}

modded class ActionTestBloodSelf
{
	override void OnFinishProgressServer(ActionData action_data)
	{	
		super.OnFinishProgressServer(action_data);

		if (MAGlobals.Get().g_TestBloodSelf)
		{
			MABloodTestReport.DisplayReport(this, action_data.m_Player, action_data.m_Player);
		}
	}
}