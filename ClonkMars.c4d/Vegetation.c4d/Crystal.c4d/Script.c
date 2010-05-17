#strict 2
#include TREE

local iPhase;

private func ReproductionAreaSize() { return 100; }
private func ReproductionRate()     { return 23; }
private func MaxTreeCount()         { return 6; }

protected func Damage(iDamage) {
	ControlDigDouble();
}

protected func Incineration() { Extinguish(); }

public func Construction() {
	if(GetAction() == "Idle") {
		SetAction("Hang");		// zufällige Animationsphase
		iPhase = Random(4);
		// Zufällige Richtung
		if (Random(1))
			SetDir(DIR_Right);
	}
	// Höhe anpassen
	while (!GBackSolid(0, 12))
		SetPosition(GetX(), GetY() + 1);
	// Drehung nach Erdoberfläche
	var x_off = GetDefWidth(GetID()) / 2;
	var y_off = GetDefHeight(GetID()) / 2;
	var slope = GetSolidOffset(-x_off, y_off) - GetSolidOffset(x_off, y_off);
	SetR(slope*2); // passt besser
	SetPhase(iPhase);
	return 1;
}

private func GetSolidOffset(int x, int y)
{
  var i;
  for (i = -20; GBackSolid(x, y - i) && (i < 20); i++);
  return i;
}

public func Reproduction()
{
  // Ist noch Platz für einen Baum?
  var iSize = ReproductionAreaSize();
  var iOffset = iSize / -2;
  if (ObjectCount(GetID(), iOffset, iOffset, iSize, iSize)<MaxTreeCount()) {
    // OK, hin damit
    var pMush = PlaceVegetation(GetID(), iOffset, iOffset, iSize, iSize, 10);
    return 1;
  }
  // Kein Platz ;'(
  return;
}

/*
public func ContextChop(pClonk)		// per Kontextmenü pflücken
{
  [$TxtPick$|Image=MUSH|Condition=IsStanding]
  Pick();
  return 1;
}*/

protected func RejectEntrance() {
  return GetAction() == "Hang";
}

protected func ControlDigDouble() {
	if(GetAction() == "Hang")
		Set(1);
	return;
}

protected func Seed() {
  if(IsStanding()) return;
  // re-seed
  if(!Contained() && !GetYDir() && !GetXDir())
  	Set(0);
  return _inherited(...);
}

public func IsStanding() { return GetCategory() & C4D_StaticBack; }	// steht noch

public func IsTree() { return 0; } // kein Baum

/*func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(80); }*/

protected func Hit() { Sound("Crystal*"); }

public func Set(int iPar) {
	if(!iPar) {
		SetAction("Hang");
		SetPhase(iPhase);
		return 1;
	}
	if(iPar == 1) {
		SetAction("Object");
		SetPhase(iPhase);
		return 1;
	}
	return;
}
