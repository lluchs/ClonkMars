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

protected func Activate(iPlr) {
  MessageWindow(GetDesc(),iPlr);
}

public func IsChoosable() { return true; } // Regelwähler
