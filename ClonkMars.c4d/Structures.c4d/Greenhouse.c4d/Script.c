/*-- Haus in Gr�n --*/

#strict 2

#include B_95
#include OVDR
#include PWRC // ist ein Energieverbraucher
#include DACT //Damagecontrol

public func GetTemp() { return 250;}

public func MaxDamage() { return 40; } //Maximaler Schaden

private func ClonkCapacity() {return 2;}
