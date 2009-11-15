/*-- Terraformer --*/

#strict 2
#include B_40
#include PWRC
#include DACT //Damagecontrol

static const TERA_RADIUS = 200; // Radius in Pixeln

protected func Initialize() {
	CreateDigger();
	fOn = true;
}

private func CreateDigger() {
	// Erde weggraben
	for(var iX = -TERA_RADIUS; iX < TERA_RADIUS; iX += 2) {
		var iY = 0;
		// an die Oberfläche versetzen
		while(GBackSolid(iX, iY))
			iY--;
		// direkt über dem Boden
		while(!GBackSolid(iX, iY + 2))
			iY++;
		
		// noch im Einflussbereich?
		if(Distance(iX, iY) > TERA_RADIUS)
			continue;
		// Effekt erstellen
		AddEffect("DigEarth", this, 10, 10, this, 0, iX, iY);
	}
}

public func RemoveDigger() {
	var i = GetEffectCount("DigEarth", this);
	while(i--)
		RemoveEffect("DigEarth", this);
}

public func RecreateDigger() {
	RemoveDigger();
	CreateDigger();
}

protected func Incineration() {
	RemoveDigger();
	return _inherited(...);
}

public func Deconstruction() {
	RemoveDigger();
	return _inherited(...);	
}

local iEnergy; // kleines Polster falls die Energie kurzzeitig nicht ausreicht
local fTerraforming, fOn;

protected func Terraforming() { // TimerCall
	if(!(GetOCF() & OCF_Fullcon))
		return;
	if(!fOn || !CheckPower(30)) {
		iEnergy++;
		fTerraforming = false;
		if(NoEnergy() && !Random(5)) {
			var pTree = FindObject2(Find_Distance(TERA_RADIUS), Find_Func("IsTree"), Find_Not(Find_Func("IsDeadTree")));
			if(pTree)
				pTree -> ChangeDef(GetDefBurnTo(pTree -> GetID()));
		}
		return;
	}
		
	//play Animation
  SetAction("WindTurn");
  
  // Regen
  Rain();
  
  if(iEnergy)
  	iEnergy--;
  	
  fTerraforming = true;
  
  var pTree;
  if(!Random(10) && (pTree = FindObject2(Find_Distance(TERA_RADIUS), Find_Func("IsDeadTree")))) { // Bäume wiederbeleben
  	pTree -> ChangeDef(pTree -> BurnedFrom());
  }
  
  if(!Random(10) && ObjectCount2(Find_Distance(TERA_RADIUS), Find_Func("IsTree")) < Random(10)) { // neue Bäume
  	var pTree = PlaceVegetation(RandomTreeID(), TERA_RADIUS / -2, TERA_RADIUS / -2, TERA_RADIUS, TERA_RADIUS, 10);
  	if(ObjectDistance(pTree) > TERA_RADIUS)
  		pTree -> RemoveObject();
  }
}

public func NoEnergy() {
	return iEnergy > 10;
}

public func IsTerraforming() {
	return fTerraforming;
}

private func RandomTreeID() {
	var aTrees = [TRE5, TRE6];
	return aTrees[Random(GetLength(aTrees))];
}

private func Rain() {
	for(var i = RandomX(10, 20); i; i--) {
		var iX = 0, iY = 0;
		while(!iX || Distance(iX, iY) > TERA_RADIUS) {
			iX = Random(TERA_RADIUS) - TERA_RADIUS / 2;
			iY = RandomX(-TERA_RADIUS, -TERA_RADIUS / 2);
		}
		if(GBackSolid(iX, iY))
			continue;
		CreateParticle("Raindrop", iX, iY, RandomX(GetWind(), GetWind()*3), RandomX(100, 200), 5*64 + Random(32));
	}
}

protected func FxDigEarthStart(object pTarget, int iEffectNumber, bool fTemp, int iX, int iY) {
	if(!fTemp) {
		EffectVar(0, pTarget, iEffectNumber) = iX;
		EffectVar(1, pTarget, iEffectNumber) = iY;
	}
}

protected func FxDigEarthTimer(object pTarget, int iEffectNumber) {
	var iX = EffectVar(0, pTarget, iEffectNumber), iY = EffectVar(1, pTarget, iEffectNumber);
	// Koordinaten anpassen
	if(GBackSolid(iX, iY)) {
		while(GBackSolid(iX, iY - 3))
			iY -= 1;
		if(pTarget -> IsTerraforming() && GBackSolid(iX, iY))
			DigFree(pTarget -> GetX() + iX, pTarget -> GetY() + iY, 2);
	}
	else if(!GBackSolid(iX, iY + 2)) {
		iY += 2;
	}
	
	// sind wir aus dem Einflussbereich gerutscht?
	if(pTarget -> Distance(iX, iY) > TERA_RADIUS)
		return -1;
	
	EffectVar(1, pTarget, iEffectNumber) = iY;
}

public func ShowDigger() {
	var i = GetEffectCount("DigEarth", this);
	while(i--) {
		var iNum = GetEffect("DigEarth", this, i);
		CreateParticle("PSpark", EffectVar(0, this, iNum), EffectVar(1, this, iNum), 0, 0, 30, GetPlrColorDw(GetOwner()));
	}
}

/* Steuerung */

protected func ControlUp(){
// Bereich anzeigen
	for(var i; i < 360; i++)
		CreateParticle("PSpark", Cos(i, TERA_RADIUS), Sin(i, TERA_RADIUS), 0, 0, 70, RGBa(255, 255, 255, 128));
		}

protected func ControlDig() {
	ShowDigger();
}

protected func ControlThrow() {
	fOn = !fOn;
	fTerraforming = fOn;
}

protected func ContextRecreateDigger() {
	[Digger neu erstellen]
	RecreateDigger();
}

/* Erforschbar */

public func MarsResearch() {
	return true;
}

public func MaxDamage() { return 20; } //Maximaler Schaden
