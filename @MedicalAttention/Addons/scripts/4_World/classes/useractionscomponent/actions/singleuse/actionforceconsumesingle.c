modded class ActionForceConsumeSingle
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		PlayerBase ntarget = PlayerBase.Cast(target.GetObject());
		if (ntarget.m_SuitPreventsConsume > 0)
		{
			return false;
		}

		return super.ActionCondition(player, target, item);
	}
}