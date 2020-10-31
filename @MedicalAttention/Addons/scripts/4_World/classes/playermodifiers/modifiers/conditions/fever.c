modded class FeverMdfr
{
	override bool ActivateCondition(PlayerBase player)
	{
		if (!MAGlobals.Get().g_FeverActive)
		{
			return false;
		}

		return player.GetModifiersManager().IsModifierActive(eModifiers.MDF_INFLUENZA) || player.GetModifiersManager().IsModifierActive(eModifiers.MDF_WOUND_INFECTION) || player.GetModifiersManager().IsModifierActive(MedicalAttentionModifiers.MDF_DRUNKENNESS);
	}
}