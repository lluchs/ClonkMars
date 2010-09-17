/*-- Haus in Grün --*/

#strict 2

#include B_95
#include OVDR
#include PWRC // ist ein Energieverbraucher
#include O2GN // produziert Sauerstoff
#include DACT //Damagecontrol

public func GetTemp() { return 250;}

public func MaxDamage() { return 40; } //Maximaler Schaden

private func ClonkCapacity() {return 2;}

public func MaxFill_O2() { return 200; }

private func SoundOpenDoor() {
	Sound("Door_Metal");
}

private func SoundCloseDoor() {
	Sound("Door_Metal");
}

protected func Initialize() {
	AddEffect("ProduceO2", this, 1, SCNK_O2 / 200, this);
}

protected func FxProduceO2Timer() {
	if(GetCon() == 100 && !IsFull("O2") && CheckPower(10))
		DoO2(10);
}
