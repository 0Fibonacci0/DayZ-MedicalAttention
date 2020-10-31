enum MedicalAttentionNotifiers
{
	NTF_POISONED = 1000,
	NTF_FRACTURE
}

modded class NotifiersManager
{
	void NotifiersManager(PlayerBase player)
	{
		m_Notifiers.Insert(new PoisonedNotfr(this));
		m_Notifiers.Insert(new FractureNotfr(this));
	}
}