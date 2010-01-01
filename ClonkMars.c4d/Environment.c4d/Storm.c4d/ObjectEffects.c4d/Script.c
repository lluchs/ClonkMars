#strict 2

static const ReflectIterAngle = 30;
static const ReflectIterRade = 3;


protected func Initialize() {
	AddObjectForWind();
	return _inherited();
}

public func WindEffect() { return GetCon(); } //Callback. Default is 100. 2do: What about GetDefHeight and GetMass?