modded class SalineMdfr
{
	override void OnTick(PlayerBase player, float deltaT)
	{
		if (MAGlobals.Get().g_SalineBloodRegen)
		{
			super.OnTick(player, deltaT);
		}

		player.GetStatWater().Add(PlayerConstants.SALINE_BLOOD_REGEN_PER_SEC * deltaT);
	}
}