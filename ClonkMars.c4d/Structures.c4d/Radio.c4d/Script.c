/*-- Teleskop --*/

#strict 2
#include B_40
#include DACT //Damagecontrol

public func MaxDamage() { return 20; } //Maximaler Schaden
protected func Initialize(){SetAction("Wait");}
public func MarsResearch() {
	return true;
}
