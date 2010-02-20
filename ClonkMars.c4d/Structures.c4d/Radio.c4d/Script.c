/*-- Teleskop --*/

#strict 2
#include B_40
#include DACT //Damagecontrol
#include PWRC // Energieverbraucher

public func MaxDamage() { return 20; } //Maximaler Schaden
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
		Sound("Error");
		return;
	}
	SetPhase(iPhase);
	return 1;
}
protected func ControlRight() {
	var iPhase = GetPhase() + 1;
	if(iPhase > 25) {
		Sound("Error");
		return;
	}
	SetPhase(iPhase);
	return 1;
}

protected func FxDestroyMeteoritesTimer() {
	var iPhase = GetPhase();
	var iAngle = [322,321,327,328,331,334,337,339,341,345,352,0,13,24,33,42,50,53,58,60,61,62,63,64,66][iPhase-1];

	//AngleFree(iAngle) + AngleFree(iAngle - 20) + AngleFree(iAngle + 20);
	
	for(var pObj in FindObjects(Find_ID(METO), Find_Distance(250), Find_Angle(iAngle + 20, iAngle - 20), Find_PathFree())) {
		if(CheckPower(50))
			pObj -> Hit();
	}
	
	for(var i=0; i < 5; i++) {
		var angle = RandomX(iAngle - 20, iAngle + 20);
		CreateParticle("PSpark", 0, 0, Sin(angle, 50), -Cos(angle, 50), 50, GetPlrColorDw(GetOwner()), this, true);
	}
}
