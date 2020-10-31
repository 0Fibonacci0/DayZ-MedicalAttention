modded class ActionDrinkPondContinuous
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (player.m_SuitPreventsConsume > 0)
		{
			return false;
		}

		return super.ActionCondition(player, target, item);
	}
}