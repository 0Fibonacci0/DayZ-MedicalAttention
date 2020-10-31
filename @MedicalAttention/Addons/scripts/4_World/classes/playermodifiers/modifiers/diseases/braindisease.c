modded class BrainDiseaseMdfr
{
	override protected void OnActivate(PlayerBase player)
	{
		if (MAGlobals.Get().g_BrainDisease)
		{
			player.IncreaseDiseaseCount();
		}
	}

	override protected void OnDeactivate(PlayerBase player)
	{
		if (MAGlobals.Get().g_BrainDisease)
		{
			player.DecreaseDiseaseCount();
		}
	}

	override protected void OnTick(PlayerBase player, float deltaT)
	{
		if (MAGlobals.Get().g_BrainDisease)
		{
			super.OnTick(player, deltaT);
		}
	}
}