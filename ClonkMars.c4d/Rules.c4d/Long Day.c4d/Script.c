/*-- Long Night --*/

#strict 2

public func OverloadsTime() {
	return true;
}

public func TimeOverload(&evening, &morning) {
	evening = TimeResolution() / 3;
	morning = TimeResolution() * 2 / 3;
	return 1;
}
