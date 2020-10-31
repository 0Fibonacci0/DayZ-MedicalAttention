modded class ActionEatMeat
{
	override void OnEndServer( ActionData action_data )
	{
		super.OnEndServer(action_data);

		Edible_Base food_item = Edible_Base.Cast( action_data.m_MainItem );
		if ( food_item )
		{
			if ( food_item.IsMeat() && food_item.IsFoodRaw() )
			{
				action_data.m_Player.GetSymptomManager().QueueUpPrimarySymptom( SymptomIDs.SYMPTOM_VOMIT );
			}
		}
	}
}