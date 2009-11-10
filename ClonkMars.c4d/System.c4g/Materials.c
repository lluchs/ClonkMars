/* rötliche Materialien */

#strict 2

#appendto ROCK
#appendto ERTH

protected func Initialize() {
	SetClrModulation(RGB(250, 100, 0));
	return _inherited(...);
}
