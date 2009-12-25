/*-- Tank --*/

#strict 2
#include B_60
#include DACT //Damagecontrol

local LiquidStorage, MaxLiquid;

func Initialize() {
  SetMaxLiquidStorage(3500); 
  SetLiquidStorage(0);
  UpdateDisplay();
  return(1);
}


private func UpdateDisplay() {
	// Der Bohrturm füllt evtl. nicht ganz
	var iNum = GetLiquidStorage() * 5 / GetMaxLiquidStorage();
	if(GetMaxLiquidStorage() - GetLiquidStorage() < 5)
		iNum = 5; // trotzdem als voll anzeigen
	SetGraphics(0, this, _BAR, 1, GFXOV_MODE_Action, Format("%d/5", iNum));
	SetObjDrawTransform(1000, 0, -8000, 0, 1000, 3500, this, 1);
	return 1;
}

public func Deconstruction() {
	//Overlay
	SetGraphics(0, this, 0, 1);
}

// Get- Funktionen
public func GetMaxLiquidStorage() {
  return MaxLiquid;
}

public func GetLiquidStorage() {
  return LiquidStorage;
}
// Set- Funktionen
public func SetMaxLiquidStorage(int NewMaxLiquidStorage) {
  MaxLiquid = NewMaxLiquidStorage;
  UpdateDisplay();
  return true;
}

public func SetLiquidStorage(int NewLiquidStorage) {
  if (NewLiquidStorage < GetMaxLiquidStorage()) {
    LiquidStorage = NewLiquidStorage;
  } else {
    LiquidStorage = GetMaxLiquidStorage();
  }
  UpdateDisplay();
  return true;
}
// Misc
public func DoLiquidStorage(int LiquidNewStorage) {
  SetLiquidStorage(GetLiquidStorage()+LiquidNewStorage);
  return true;
}

public func PipelineConnect() {
	return 1;
}

//Steuerung
  
protected func ControlUp(){
  Message("%d",this,GetLiquidStorage());
  }

protected func ControlDig(object pClonk) {
	var iChange = pClonk -> ~DoFuel(GetLiquidStorage() * 100);
	iChange = ChangeRange(iChange, 0, pClonk -> ~MaxFuel(), 0, 100);
	Message("+%d", pClonk, iChange);
	DoLiquidStorage(-iChange);
	return 1;
}

/* Erforschbar */

public func MarsResearch() {
	return true;
}

public func MaxDamage() { return 15; } //Maximaler Schaden

