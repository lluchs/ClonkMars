/*-- O2-Flasche --*/

#strict 2

#include L_SS // Speichersystem

private func MaxFill() { return 40; }

private func FillPicture() { return 0; }

protected func Hit() {
  Sound("MetalHit*");
  return(1);
}

protected func Entrance(object pObj) {
	if(pObj -> HasO2()) {
		AddEffect("Refill", this, 100, 5, this);
	}
}

protected func FxRefillTimer() {
	if(!Contained() -> HasO2() || !DoFill(1))
		return -1;
}

protected func Activate(object pClonk) {
	DoFill(-pClonk -> ~DoO2(GetAmount()));
	return 1;
}

public func MarsResearch() { return true; }
