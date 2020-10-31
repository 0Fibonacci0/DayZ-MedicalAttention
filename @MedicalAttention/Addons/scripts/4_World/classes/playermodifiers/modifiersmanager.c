enum MedicalAttentionModifiers
{
	MDF_TIREDNESS = 1000,
	MDF_DRUNKENNESS,
	MDF_BIOHAZZARD,
	MDF_UNKNOWN_TOXINE
}

modded class ModifiersManager
{
	override void Init()
	{
		super.Init();

		AddModifier(new TirednessMdfr);
		AddModifier(new DrunkennessMdfr);
		AddModifier(new BiohazzardMdfr);
		AddModifier(new UnknownToxineMdfr);
		AddModifier(new BrokenLegsMdfr);
		AddModifier(new BoneRegenMdfr);
		AddModifier(new BrokenArmsMdfr);
	}
}