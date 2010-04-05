/*-- Tank --*/

#strict 2
#include B_60
#include DACT //Damagecontrol
#include L_SS

func Initialize() {
  return 1;
}

public func MaxFill() { return 3500; }
private func FillPicture() { return; }

private func UpdatePicture() {
	// Der Bohrturm füllt evtl. nicht ganz
	var iNum = GetFill() * 5 / MaxFill();
	if(MaxFill() - GetFill() < 5)
		iNum = 5; // trotzdem als voll anzeigen
	SetGraphics(0, this, _BAR, 1, GFXOV_MODE_Action, Format("%d/5", iNum));
	SetObjDrawTransform(1000, 0, -8000, 0, 1000, 3500, this, 1);
	return 1;
}

public func Deconstruction() {
	//Overlay
	SetGraphics(0, this, 0, 1);
	
	return _inherited(...);
}

public func PipelineConnect() {
	return 1;
}

//Steuerung
  
protected func ControlUp(){
  Message("%d",this,GetFill());
  }

protected func ControlDig(object pClonk) {
	var iChange = pClonk -> ~DoFuel(GetFill() * 100);
	iChange = ChangeRange(iChange, 0, pClonk -> ~MaxFuel(), 0, 100);
	Message("+%d", pClonk, iChange);
	pClonk -> ~UpdateFuelHUD();
	DoFill(-iChange);
	return 1;
}

/* Erforschbar */

public func MarsResearch() {
	return true;
}

public func MaxDamage() { return 15; } //Maximaler Schaden

private func DestroyBlast(object pTarget) {
	if(!pTarget) if(!(pTarget=this)) return;
	var iAmount = pTarget -> GetFill();
	pTarget -> CastPXS("Oil", RandomX(iAmount / 2, iAmount), 50);
	return inherited(...);
}

