/*-- Haus in Grün --*/

#strict 2

#include B_95
#include OVDR
#include PWRC // ist ein Energieverbraucher
#include DACT //Damagecontrol

public func GetTemp() { return 1000;}

func Initialize() {
  return(1);
}

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

/*protected func Overburn(){
  CreateParticle("Fire2", 20, -40, RandomX(-5,5), -10-Random(10), 75 + Random(75), RGBa(255, 230, 230, 50), this());
  CreateParticle("Fire2", 23, -40, RandomX(-5,5), -10-Random(10), 75 + Random(75), RGBa(255, 230, 230, 50), this());
  if(!Random(3))
    CreateParticle("Fire", 23, -40, RandomX(-5,5), -10-Random(10), 55, RGBa(255, 230, 230, 50), this());
    }*/

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
