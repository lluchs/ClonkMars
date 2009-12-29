/*-- Haus in Grün --*/

#strict 2

#include B_95
#include OVDR
#include PWRC // ist ein Energieverbraucher
#include DACT //Damagecontrol

public func GetTemp() { return 600;}

func Initialize() {
  return(1);
}

/* Los Funktionos*/

protected func ContainedUp(object pClonk) {
	return ProduceMenu(pClonk);
}

protected func ProduceMenu(object pClonk, fShowAll) {
	if(GetAction() != "Idle") {
		CreateMenu(GetID(), pClonk, this);
		if(fContinue)
			AddMenuItem("$TxtCancel$", "fContinue = false;", RSR2, pClonk, 0, 0, "$TxtCancelFull$");
		AddMenuItem("$TxtCancelNow$", "Cancel", RSR2, pClonk, 0, 0, "$TxtCancelNow$");
		return 1;
	}

/* End */

private func CanOpen() {
	return GetAction() == "Idle";
}

private func SoundOpenDoor() {
	Sound("Door_Metal");
}

private func SoundCloseDoor() {
	Sound("Door_Metal");
}

private func EnergyNeedOverlay() {
	return 3; // Overlay 1 und 2: Türsteuerung
}

public func PipelineConnect() {
	return 1;
}

public func MaxDamage() { return 60; } //Maximaler Schaden

/* Kontext */

private func IsComplete()
{
  return (GetCon() >= 100); 
}

/* Erforschbar */

public func MarsResearch() {
	return true;
}

private func ClonkCapacity() {
	return 2;
}
