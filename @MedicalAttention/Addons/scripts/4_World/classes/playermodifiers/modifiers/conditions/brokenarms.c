modded class BrokenArmsMdfr
{
	const int LIFETIME = 600;
	bool m_BrokenArms = false;

	override void Init()
	{
		m_TrackActivatedTime = true;
		m_IsPersistent = true;
		m_ID 					= eModifiers.MDF_BROKEN_ARMS;
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
		if (player.GetNotifiersManager()) player.GetNotifiersManager().ActivateByType(MedicalAttentionNotifiers.NTF_FRACTURE);
		player.m_BrokenArms = true;
		player.SetSynchDirty();
	}

	override protected void OnDeactivate(PlayerBase player)
	{
		if (!player.m_BrokenLegs && player.GetNotifiersManager()) player.GetNotifiersManager().DeactivateByType(MedicalAttentionNotifiers.NTF_FRACTURE);
		player.m_BrokenArms = false;
		player.SetSynchDirty();
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
		if (player.GetModifiersManager().IsModifierActive(eModifiers.MDF_MORPHINE) || player.GetModifiersManager().IsModifierActive(eModifiers.MDF_PAINKILLERS))
		{
			m_BrokenArms = false;
		}
		else
		{
			m_BrokenArms = true;
		}
		
		if (m_BrokenArms != player.m_BrokenArms)
		{
			player.m_BrokenArms = m_BrokenArms;
			player.SetSynchDirty();
		}
	}
}
