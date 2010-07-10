/*-- Tank --*/

#strict 2
#include B_60
#include DACT //Damagecontrol
#include L_SS
#include L_CA //Baugrafik

local type; // 0: �l; 1: Lava

func Initialize() {
	UpdateTypePicture();
	UpdatePicture();
	return 1;
}

public func TankFor(int mat) {
	if(type == 0)
		return mat == Material("Oil");
	else
		return WildcardMatch(MaterialName(mat), "*Lava*");
}

public func MaxFill() { return 3500; }

private func FillPicture() { return; }

private func UpdatePicture() {
	if(GetCon() != 100)
		return;
	// Der Bohrturm f�llt evtl. nicht ganz
	var iNum = GetFill() * 5 / MaxFill();
	if(MaxFill() - GetFill() < 5)
		iNum = 5; // trotzdem als voll anzeigen
	SetGraphics(0, this, _BAR, 1, GFXOV_MODE_Action, Format("%d/5", iNum));
	SetObjDrawTransform(1000, 0, -8000, 0, 1000, 3500, this, 1);
	UpdateTypePicture();
	return 1;
}

private func UpdateTypePicture() {
	var act = "Oil";
	if(type)
		act = "Lava";
	if(IsFull())
		act = Format("%sFull", act);
	SetGraphics(0, this, PLAT, 2, GFXOV_MODE_Action, act);
	SetObjDrawTransform(1000, 0, 33000, 0, 1000, -2000, this, 2);
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
  if(!Hostile(GetOwner(),GetOwner(Par()))){
  Message("%d",this,GetFill());}
    else{
      Message("$TxtEnemy$",this());
      Sound("Error");}
  }

protected func ControlDig(object pClonk) {
  if(!Hostile(GetOwner(),GetOwner(Par()))){
	if(type)
		Message("$TxtOnlyOil$", pClonk);
	var iChange = pClonk -> ~DoFuel(GetFill() * 100);
	iChange = ChangeRange(iChange, 0, pClonk -> ~MaxFuel(), 0, 100);
	Message("+%d", pClonk, iChange);
	pClonk -> ~UpdateFuelHUD();
	DoFill(-iChange);}
	    else{
      Log("$TxtEnemy$");
      Sound("Error");}
	return 1;
}

protected func ControlThrow() {
  if(!Hostile(GetOwner(),GetOwner(Par()))){
	if(GetFill())
		return Message("$TxtOnlyEmptyTank$", this);
	type = !type;
	UpdateTypePicture();}
	    else{
      Message("$TxtEnemy$",this());
      Sound("Error");}
	return 1;
}

protected func ContextSwitch2Oil() {
  if(!Hostile(GetOwner(),GetOwner(Par()))){
	[Zu �l wechseln|Image=OILT|Condition=ContainsLava]
	ControlThrow();}
		    else{
      Message("$TxtEnemy$",this());
      Sound("Error");}
}

protected func ContextSwitch2Lava() {
  if(!Hostile(GetOwner(),GetOwner(Par()))){
	[Zu Lava wechseln|Image=OILT|Condition=ContainsOil]
	ControlThrow();}
			    else{
      Message("$TxtEnemy$",this());
      Sound("Error");}
}

public func ContainsOil() {
	return type == 0;
}

public func ContainsLava() {
	return type == 1;
}

protected func ContextFlush() {
	[Tank leeren|Image=OILT]
	var mat = "Oil";
	if(type)
		mat = "Lava";
	CastPXS(mat, Abs(DoFill(-MaxFill())), 20);
	return 1;
}

/* Erforschbar */

public func MarsResearch() {
	return true;
}

public func MaxDamage() { return 20; } //Maximaler Schaden

private func DestroyBlast(object pTarget) {
	if(!pTarget) if(!(pTarget=this)) return;
	var iAmount = pTarget -> GetFill();
	var mat = "Oil";
	if(type)
		mat = "Lava";
	pTarget -> CastPXS(mat, RandomX(iAmount / 2, iAmount), 50);
	return inherited(...);
}

public func Deconstruction() {
	var mat = "Oil";
	if(type)
		mat = "Lava";
	CastPXS(mat, Abs(DoFill(-MaxFill())), 20);
	return _inherited(...);
}

/* Anzeige-fix nach Spielstand laden */

func UpdateTransferZone()
{
	UpdatePicture();
}
