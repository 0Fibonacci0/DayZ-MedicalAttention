modded class BrokenLegsMdfr
{
	const int LIFETIME = 900;

	override void Init()
	{
		m_TrackActivatedTime = true;
		m_IsPersistent = true;
		m_ID 					= eModifiers.MDF_BROKEN_LEGS;
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
		player.m_BrokenLegs = true;
		player.SetSynchDirty();
	}

	override protected void OnDeactivate(PlayerBase player)
	{
		if (!player.m_BrokenArms && player.GetNotifiersManager()) player.GetNotifiersManager().DeactivateByType(MedicalAttentionNotifiers.NTF_FRACTURE);
		player.m_BrokenLegs = false;
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
		if (player.GetModifiersManager().IsModifierActive(eModifiers.MDF_MORPHINE) || player.GetModifiersManager().IsModifierActive(eModifiers.MDF_BONE_REGEN))
		{
			return;
		}

		if (Math.RandomFloat01() < 0.7)
		{
			HumanCommandMove hcm = player.GetCommand_Move();
			if (hcm)
			{
				hcm.ForceStance(DayZPlayerConstants.STANCEIDX_PRONE);
			}
		}
	}
}
