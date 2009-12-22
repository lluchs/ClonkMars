/*--Basis--*/

#strict 2

#include B_95
#include OVDR
#include PWRC
#include DACT //Damagecontrol
#include L_CA

public func GetTemp() { return 800;}

protected func Initialize() {
	AddBuildingLight();
	return _inherited(...);
}

protected func CheckSky(){
	if(!AngleFree(180))
		SetAction("Wait");
	else
		SetAction("Green");
}

private func SoundOpenDoor() {
	Sound("Door_Metal");
}

private func SoundCloseDoor() {
	Sound("Door_Metal");
}

private func CanOpen() {
	return 1;
}

/* Steuerung */

protected func ContextRemoveSat() {
	[Sat abmontieren|Image=SATD|Condition=HasSat]
	HasSat() -> SetAction("Idle");
}

protected func ContainedUp(object pClonk) {
	if(CapsuleCheck(pClonk))
		return;
	var menu = CreateMenuTemplate(CPSL, "Kapsel rufen...");
	AddEnumChoice(menu, 0, "Sell");
		AddEnumChoiceItem(menu, 0, "Sell", "SellOnly", "Nur verkaufen", 0, "SellOnly");
		AddEnumChoiceItem(menu, 0, "Sell", "Buy", "Kaufen", 0, "Buy", true);
	AddSubmenu(menu, 0, "Buy", MenuCond_Chosen(0, "Sell", "Buy"), "Objekte bestellen");
		var i = 0, ID;
		while(ID = GetHomebaseMaterial(pClonk -> GetOwner(), 0, i++, C4D_Object)) {
			AddRangeChoice(menu, ["Buy"], ID, 0, GetName(0, ID), ID, 0, GetHomebaseMaterial(pClonk -> GetOwner(), ID), 1, 0);
		}
	
	CreateMenuByTemplate(pClonk, this, "OrderCapsule", menu, pClonk);
	
	return 1;
}

public func OrderCapsule(hash, object pClonk, bool fCanceled) {
	if(fCanceled || CapsuleCheck(pClonk))
		return;
	if(HashGet(hash, "Sell") == "SellOnly") {
		return !!CreateCapsule();
	}
	else {
		var pCapsule = CreateCapsule();
		if(!pCapsule) return;
		var objs = HashGet(hash, "Buy");
		var iter = HashIter(objs); // Iterator erzeugen
		var node;
		while(node = HashIterNext(iter)) {
			if(node[1] > GetHomebaseMaterial(pClonk -> GetOwner(), node[0]))
				node[1] = GetHomebaseMaterial(pClonk -> GetOwner(), node[0]);
			for(var i = 0; i < node[1]; i++) {
				if(!Buy(node[0], pClonk -> GetOwner(), pClonk -> GetOwner(), pCapsule))
					return true;
			}
		}
	}
	return true;
}

private func CapsuleCheck(object pClonk) {
	var pSat = HasSat();
	if(!pSat)
		return 1;
	if(pSat -> GetCapsule()) {
		Sound("Error");
		Message("$TxtAlreadyCapsule$", pClonk);
		return 1;
	}
}

private func CreateCapsule() {
	var pPort, pCaps;
	if(pPort = FindObject2(Find_ID(PORT), Find_OCF(OCF_Fullcon), Find_Allied(GetOwner()), Find_Distance(700), Find_Func("IsFree"), Sort_Distance())) {
		pCaps = CreateObject(CPSL, AbsX(GetX(pPort)), AbsY(-100), GetOwner());
	} else {
		pCaps = CreateObject(CPSL, RandomX(-200, 200), AbsY(-30), GetOwner());
	}
	pCaps->SetDstPort(pPort);
	
	HasSat() -> SetCapsule(pCaps);
	return pCaps;
}

private func HasSat() {
	return FindObject2(Find_ID(SATD), Find_Action("FlyBase"), Find_ActionTarget(this));
}

/* Wärme */

public func GetWarmth() {
  return(100);
}

/* Erforschbar */

public func MarsResearch() {
	return true;
}

private func ClonkCapacity() {
	return 3;
}

public func Reproduction() {
	return GetAction() == "Green";
}

/* Produziert Sauerstoff */

public func IsO2Producer() {
	return GetAction() == "Green";
}

public func PipelineConnect() {
	return 1;
}

public func IsBase() {
	return true;
}

public func MaxDamage() { return 50; } //Maximaler Schaden

global func FindBase(int iPlr) {
	return FindObject2(Find_Owner(iPlr), Find_Func("IsBase"), Sort_Distance()); // FIXME-performance: Use ID for speed optimisation
}

global func GetBase(object pObject) {
	if(!pObject && !(pObject=this)) return NO_OWNER; 
	var sat = pObject->~HasSat();
	if(sat) return GetOwner(sat);
	return NO_OWNER;
}
