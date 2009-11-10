/*-- Splitter --*/

#strict 2
#include B_16
#include DACT //Damagecontrol

local pStation;

public func SetStation(object pObj) {
	return pStation = pObj;
}

public func GetStation() {
	return pStation;
}

// Radius anzeigen
protected func ControlUp(object pClonk) {
	if(Hostile(GetOwner(), pClonk -> GetOwner())) {
		Sound("Error");
		return;
	}
	for(var i; i < 360; i++)
		CreateParticle("PSpark", Cos(i, TS_MaxDistance), Sin(i, TS_MaxDistance), 0, 0, 70, RGBa(255, 255, 0, 128));
	for(var pObj in FindTSNodes()) {
		var d = ObjectDistance(pObj);
		var a = Angle(AbsX(pObj->GetX()), AbsY(pObj->GetY()));
		CreateObject(LASR)->Set(a + 180, 5, d, 70);

	}
}

public func FindTSNodes() {
	return FindObjects(Find_Exclude(this), Find_ID(SPTR), Find_OCF(OCF_Fullcon), Find_Allied(GetOwner()), Find_Not(Find_Func("IsReserved")), Find_PathFree(this));
}

// Streckenreservierung

local fReserved;

public func IsReserved() { return fReserved; }

public func ToggleReservation() {
	return fReserved = !fReserved;
}

/* Wärme */

public func GetWarmth() {
  return(100);
}

public func MaxDamage() { return 5; } //Maximaler Schaden
