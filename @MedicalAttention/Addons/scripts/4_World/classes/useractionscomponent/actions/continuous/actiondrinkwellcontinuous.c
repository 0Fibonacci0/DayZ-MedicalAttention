modded class ActionDrinkWellContinuous
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (player.m_SuitPreventsConsume > 0 || player.m_InsideNoWellsZone)
		{
			return false;
		}

		return super.ActionCondition(player, target, item);
	}
}