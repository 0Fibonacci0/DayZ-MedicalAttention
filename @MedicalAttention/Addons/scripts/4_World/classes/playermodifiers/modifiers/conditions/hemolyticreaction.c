modded class HemolyticReactionMdfr
{
	override void OnTick(PlayerBase player, float deltaT)
	{
		if (player.GetModifiersManager().IsModifierActive(eModifiers.MDF_SALINE))
			return;

		super.OnTick(player, deltaT);
	}
}