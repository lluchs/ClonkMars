/*-- O2-Flasche --*/

#strict 2

#include L_RC // Recycling
#include L_SS // Speichersystem

private func MaxFill() { if(IsUpgraded()) return 80; else return 40; }

private func FillPicture() { return 0; }

protected func Hit() {
  Sound("O2_hit");
  return(1);
}

protected func Entrance(object pObj) {
	if(pObj -> HasO2()) {
		AddEffect("Refill", this, 100, 5, this);
	}
}

protected func FxRefillTimer() {
	if(!Contained() || !Contained() -> HasO2() || !DoFill(1))
		return -1;
}

protected func Activate(object pClonk) {
  Sound("O2_fillup");
	DoFill(-pClonk -> ~DoO2(GetFill()));
	return 1;
}

public func MarsResearch() { return true; }

/* Upgrade */

local upgraded;

public func IsUpgraded() {
	return upgraded;
}

public func Upgrade() {
	upgraded = true;
}
