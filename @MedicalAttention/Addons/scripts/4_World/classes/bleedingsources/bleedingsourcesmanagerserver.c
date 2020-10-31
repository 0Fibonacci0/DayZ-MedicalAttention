modded class BleedingSourcesManagerServer
{
	override void ProcessHit(float damage, EntityAI source, int component, string zone, string ammo, vector modelPos)
	{
		float dmg_max = m_Player.GetMaxHealth(zone, "Blood");
		float dmg = damage;
		float bleed_threshold = GetGame().ConfigGetFloat( "CfgAmmo " + ammo + " DamageApplied " + "bleedThreshold" );
		bleed_threshold = Math.Clamp(bleed_threshold,0,1);
		//Print("dmg_max = " + dmg_max);
		//Print("dmg = " + dmg);
		//Print("bleed_threshold = " + bleed_threshold);

		//hackerino for zombino:
		if (source.IsZombie())
		{
			float chance = (float)MAGlobals.Get().g_ZombieBleedChance / 100;
			if (Math.RandomFloat01() < chance)
			{
				AttemptAddBleedingSource(component);
				
				chance = (float)MAGlobals.Get().g_ZombieSalmonellaChance / 100;
				if (Math.RandomFloat01() < chance)
				{
					m_Player.InsertAgent(eAgents.SALMONELLA, 1);
				}

				if (m_Player.m_InsideBiohazardZone)
				{
					m_Player.InsertAgent(MedicalAttentionAgents.UNKNOWN_TOXINE, 150);
				}
			}
		}
		else if ( dmg > (dmg_max * (1 - bleed_threshold)) )
		{
			AttemptAddBleedingSource(component);
			//Print("BLEEDING");

			if (m_Player.m_InsideBiohazardZone)
			{
				m_Player.InsertAgent(MedicalAttentionAgents.UNKNOWN_TOXINE, 150);
			}
		}
	}
}