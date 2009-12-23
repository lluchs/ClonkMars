/*-- Sat --*/

#strict 2

func ResetCategory()
{
  SetCategory(GetDefCategory());
  return 1;
}

protected func Hit() {
  Sound("MetalHit*");
  return(1);
}

protected func Entrance(object pContainer) {
	if(pContainer -> ~IsBase() && !FindObject2(Find_ID(GetID()), Find_Action("FlyBase"), Find_ActionTarget(pContainer))) {
		SetAction("FlyBase", pContainer);
		Sound("Sat_Beep");
		SetCategory(C4D_Structure);
		return 1;
	}
}

protected func RejectEntrance() {
	return GetAction() == "FlyBase";
}

local pCapsule;

public func GetCapsule() {
	return pCapsule;
}

public func SetCapsule(object pObj) {
	pCapsule = pObj;
}
