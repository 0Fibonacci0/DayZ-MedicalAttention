modded class BoneRegenMdfr
{
	const int LIFETIME = 900;

	override void Init()
	{
		m_TrackActivatedTime = true;
		m_IsPersistent = true;
		m_ID 					= eModifiers.MDF_BONE_REGEN;
		m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}

	override protected bool ActivateCondition(PlayerBase player)
	{
		return false;
	}

	override void OnReconnect(PlayerBase player)
	{
		OnActivate(player);
	}

	override protected void OnActivate(PlayerBase player)
	{
		if (player.GetNotifiersManager()) player.GetNotifiersManager().ActivateByType(eNotifiers.NTF_PILLS);
	}

	override protected void OnDeactivate(PlayerBase player)
	{
		if (player.GetNotifiersManager()) player.GetNotifiersManager().DeactivateByType(eNotifiers.NTF_PILLS);
	}

	override protected bool DeactivateCondition(PlayerBase player)
	{
		float attached_time = GetAttachedTime();
		
		if( attached_time >= LIFETIME )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	override protected void OnTick(PlayerBase player, float deltaT)
	{
	}
}
