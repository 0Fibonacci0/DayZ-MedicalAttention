modded class ActionFillBottleBase
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		Object targetObject = target.GetObject();
		if (targetObject)
		{
			if ((targetObject.IsWell() && player.m_InsideNoWellsZone) || (targetObject.IsFuelStation() && player.m_InsideNoFuelZone))
			{
				return false;
			}
		}

		return super.ActionCondition(player, target, item);
	}
}