/*-- Teleskop --*/

#strict 2
#include B_40
#include DACT //Damagecontrol
#include PWRC // Energieverbraucher
#include L_CA // Baugrafik

public func MaxDamage() { return 27; } //Maximaler Schaden
protected func Initialize(){
	SetAction("Panel");
	AddEffect("DestroyMeteorites", this, 10, 5, this);
}
public func MarsResearch() {
	return true;
}

protected func ControlLeft() {
	var iPhase = GetPhase() - 1;
	if(iPhase < 1) {
		Sound("Click");
		return;
	}
	SetPhase(iPhase);
	return 1;
}
protected func ControlRight() {
	var iPhase = GetPhase() + 1;
	if(iPhase > 25) {
		Sound("Click");
		return;
	}
	SetPhase(iPhase);
	return 1;
}

protected func ControlDig() {
	DoBlast(true);
}

private func GetAngle() {
	return [322,321,327,328,331,334,337,339,341,345,352,0,13,24,33,42,50,53,58,60,61,62,63,64,66][GetPhase()-1];
}

private func AngleCriteria() {
	var iAngle = GetAngle();
	return Find_And(Find_Distance(250), Find_Angle(iAngle + 20, iAngle - 20), Find_PathFree());
}

protected func FxDestroyMeteoritesTimer() {
	DoBlast(false);
}

// power: wenn true, wird immer ein Blast ausgeführt, unabhängig davon, ob was gefunden wird
private func DoBlast(bool power) {
	// Wenn generell ein Strahl abgeschossen werden soll
	if(power && !CheckPower(50)) {
		return;
	}
	
	// Nur bei Bedarf
	if(!power) {
		if(!ObjectCount2(Find_ID(METO), AngleCriteria()) || !CheckPower(50)) {
			return;
		}
	}
	
	var iAngle = GetAngle();
	
	for(var pObj in FindObjects(Find_Or(Find_ID(METO), Find_OCF(OCF_Alive), Find_OCF(OCF_Collectible)), Find_NoContainer(), AngleCriteria())) {
		Fling(pObj, Sin(iAngle, 10), -Cos(iAngle, 10));
		pObj -> DoEnergy(-RandomX(5, 15));
		pObj -> Hit();
	}
	
	var pObj = CreateObject(RADE, 0, 0, GetOwner());
	pObj -> SetR(iAngle);
	pObj -> SetAction("Effect", this);
	
	Sound("RADO_Shoot");
}
