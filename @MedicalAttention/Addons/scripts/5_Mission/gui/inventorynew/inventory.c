modded class Inventory
{
	override void UpdateSpecialtyMeter()
	{
		super.UpdateSpecialtyMeter();

		bool showSpecialization = false;

		Mission mission = GetGame().GetMission();
		if (mission)
		{
			IngameHud hud = IngameHud.Cast(mission.GetHud());
			if (hud)
			{
				showSpecialization = !hud.GetHudState();
			}
		}		
		
		m_SpecializationPanel.Show(showSpecialization);
		m_SpecializationIcon.Show(showSpecialization);
	}
}