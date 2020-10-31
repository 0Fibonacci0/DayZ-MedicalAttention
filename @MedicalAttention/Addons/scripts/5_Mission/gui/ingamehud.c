modded class IngameHud
{
	private ProgressBarWidget m_Tiredness = NULL;
	private Widget m_TirednessIconPanel = NULL;
	private Widget m_TirednessBarPanel = NULL;

	override void Init(Widget hud_panel_widget)
	{
		super.Init(hud_panel_widget);

		Class.CastTo(m_Tiredness, m_HudPanelWidget.FindAnyWidget("TirednessBar"));
		m_TirednessIconPanel = m_HudPanelWidget.FindAnyWidget("TirednessIconPanel");
		m_TirednessBarPanel = m_HudPanelWidget.FindAnyWidget("TirednessBarPanel");
	}

	override void SetTiredness(int value , int range)
	{
		if (!m_Tiredness)
		{
			return;
		}

		m_Tiredness.SetCurrent((value / range ) * 100);
	}

	override void ShowVehicleInfo()
	{
		super.ShowVehicleInfo();

		if (!m_TirednessIconPanel || !m_TirednessBarPanel)
		{
			return;
		}

		m_TirednessIconPanel.Show(false);
		m_TirednessBarPanel.Show(false);
	}

	override void HideVehicleInfo()
	{
		super.HideVehicleInfo();

		if (!m_TirednessIconPanel || !m_TirednessBarPanel)
		{
			return;
		}

		m_TirednessIconPanel.Show(true);
		m_TirednessBarPanel.Show(true);
	}
}