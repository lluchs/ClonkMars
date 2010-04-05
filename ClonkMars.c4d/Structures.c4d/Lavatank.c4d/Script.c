/*-- Tank --*/

#strict 2
#include B_60
#include DACT //Damagecontrol
#include L_SS

func Initialize() {
	SetClrModulation(RGB(255)); // als Grafikersatz ;)
	return 1;
}

public func TankFor(int mat) { return WildcardMatch(MaterialName(mat), "*Lava*"); }

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

/* Erforschbar */

public func MarsResearch() {
	return true;
}

public func MaxDamage() { return 15; } //Maximaler Schaden

private func DestroyBlast(object pTarget) {
	if(!pTarget) if(!(pTarget=this)) return;
	var iAmount = pTarget -> GetFill();
	pTarget -> CastPXS("Lava", RandomX(iAmount / 2, iAmount), 50);
	return inherited(...);
}

