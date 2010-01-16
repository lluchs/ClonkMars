/*-- Long Night --*/

#strict 2

public func OverloadsTime() {
	return true;
}

public func TimeOverload(&evening, &morning) {
	evening = TimeResolution() / 6;
	morning = TimeResolution() * 5 / 6;
	return 1;
}

protected func Activate(iPlr) {
  MessageWindow(GetDesc(),iPlr);
}
