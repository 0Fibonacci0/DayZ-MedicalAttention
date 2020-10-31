modded class MissionGameplay
{
	static const string CLIENT_CONFIG_FILE = "$saves:MedicalAttention.txt";
	static const string DISPLAY_TIREDNESS = "notirednesshud";

	bool ShouldDisplayTirednessHud()
	{
		FileHandle file = OpenFile(CLIENT_CONFIG_FILE, FileMode.READ);
		if (file != 0)
		{
			Print("[MedicalAttention] ShouldDisplayTirednessHud: file was opened");
			string line = "";
			FGets(file, line);
			CloseFile(file);
			line.ToLower();
			if (line.Contains(DISPLAY_TIREDNESS))
			{
				Print("[MedicalAttention] ShouldDisplayTirednessHud: no");
				return false;
			}
		}
		Print("[MedicalAttention] ShouldDisplayTirednessHud: yes");
		return true;
	}

	void SaveDisplayTirednessHud(bool value)
	{
		Print("[MedicalAttention] SaveDisplayTirednessHud(" + value.ToString() + ")");
		FileHandle file = OpenFile(CLIENT_CONFIG_FILE, FileMode.WRITE);
		if (file != 0)
		{
			Print("[MedicalAttention] SaveDisplayTirednessHud: file was opened");
			if (!value)
			{
				FPrintln(file, DISPLAY_TIREDNESS);
			}
			CloseFile(file);
		}		
	}

	override void OnInit()
	{
		if (!ShouldDisplayTirednessHud())
		{
			super.OnInit();
			return;
		}
		
		if ( m_Initialized )
		{
			return;
		}
			
		PPEffects.Init();
		MapMarkerTypes.Init();
		
		m_UIManager = GetGame().GetUIManager();
			
		m_Initialized				= true;

		// init hud ui
		if ( !m_HudRootWidget )
		{
			m_HudRootWidget			= GetGame().GetWorkspace().CreateWidgets("Lucian/MedicalAttention/layout/mod_day_z_hud.layout");
			
			m_HudRootWidget.Show(false);
			
			m_Chat.Init(m_HudRootWidget.FindAnyWidget("ChatFrameWidget"));
			
			m_ActionMenu.Init( m_HudRootWidget.FindAnyWidget("ActionsPanel"), TextWidget.Cast( m_HudRootWidget.FindAnyWidget("DefaultActionWidget") ) );
			
			m_Hud.Init( m_HudRootWidget.FindAnyWidget("HudPanel") );
			
			// von enabled icon
			m_MicrophoneIcon = ImageWidget.Cast( m_HudRootWidget.FindAnyWidget("mic") );
			m_MicrophoneIcon.Show(false);
			
			// von voice level
			m_VoiceLevels = m_HudRootWidget.FindAnyWidget("VoiceLevelsPanel");
			m_VoiceLevelsWidgets = new map<int, ImageWidget>; // [key] voice level
			m_VoiceLevelTimers = new map<int,ref WidgetFadeTimer>; // [key] voice level
		
			if( m_VoiceLevels )
			{
				m_VoiceLevelsWidgets.Set(VoiceLevelWhisper, ImageWidget.Cast( m_VoiceLevels.FindAnyWidget("Whisper") ));
				m_VoiceLevelsWidgets.Set(VoiceLevelTalk, ImageWidget.Cast( m_VoiceLevels.FindAnyWidget("Talk") ));
				m_VoiceLevelsWidgets.Set(VoiceLevelShout, ImageWidget.Cast( m_VoiceLevels.FindAnyWidget("Shout") ));
				
				m_VoiceLevelTimers.Set(VoiceLevelWhisper, new WidgetFadeTimer);
				m_VoiceLevelTimers.Set(VoiceLevelTalk, new WidgetFadeTimer);
				m_VoiceLevelTimers.Set(VoiceLevelShout, new WidgetFadeTimer);
			}
			
			HideVoiceLevelWidgets();
			
			// chat channel
			m_ChatChannelArea		= m_HudRootWidget.FindAnyWidget("ChatChannelPanel");
			m_ChatChannelText		= TextWidget.Cast( m_HudRootWidget.FindAnyWidget("ChatChannelText") );
		}
		
		// init hud ui
		if ( GetGame().IsDebug() )
		{
			m_HudDebug				= new HudDebug;
			
			if ( !m_HudDebug.IsInitialized() )
			{
				m_HudDebug.Init( GetGame().GetWorkspace().CreateWidgets("gui/layouts/debug/day_z_hud_debug.layout") );
				
				Debug.SetEnabledLogs(PluginConfigDebugProfile.GetInstance().GetLogsEnabled());
			}
		}

		//AIBehaviourHL.RegAIBehaviour("zombie2",AIBehaviourHLZombie2,AIBehaviourHLDataZombie2);
		//RegBehaviour("zombie2",AIBehaviourHLZombie2,AIBehaviourHLDataZombie2);
		
		if( GetGame().IsMultiplayer() )
		{
			OnlineServices.m_MuteUpdateAsyncInvoker.Insert( SendMuteListToServer );
		}
	}

	override void OnMissionFinish()
	{
		super.OnMissionFinish();
		
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player)
		{
			SaveDisplayTirednessHud(player.m_TirednessEnabled);
		}
	}
}