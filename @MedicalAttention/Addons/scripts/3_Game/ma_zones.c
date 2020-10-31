enum ZoneType
{
	HARMLESS = 0,
	NON_COMBAT,
	NO_WELLS,
	NO_FUEL,
	BIO_HAZZARD,

	COUNT
}

class MAZone: Managed
{
	vector center;
	float radius;
	ZoneType type;
	string messageIn;
	string messageOut;
}

class MAZoneInstance: MAZone
{
	bool active;
}