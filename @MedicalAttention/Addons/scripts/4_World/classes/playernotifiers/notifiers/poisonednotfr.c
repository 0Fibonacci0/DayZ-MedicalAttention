class PoisonedNotfr: NotifierBase
{
	void PoisonedNotfr(NotifiersManager manager)
	{
		m_Active = false; 
	}

	override int GetNotifierType()
	{
		return MedicalAttentionNotifiers.NTF_POISONED;
	}

	override void DisplayBadge()
	{
		
		DisplayElementBadge dis_elm = DisplayElementBadge.Cast(GetVirtualHud().GetElement(eDisplayElements.DELM_BADGE_POISONED));
		
		if( dis_elm )
		{
			dis_elm.SetLevel(eBadgeLevel.FIRST);
		}
	}

	override void HideBadge()
	{
		
		DisplayElementBadge dis_elm = DisplayElementBadge.Cast(GetVirtualHud().GetElement(eDisplayElements.DELM_BADGE_POISONED));
		
		if( dis_elm )
		{
			dis_elm.SetLevel(eBadgeLevel.NONE);
		}
	}
}