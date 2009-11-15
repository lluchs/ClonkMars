/*-- Sat --*/

#strict 2

func ResetCategory()
{
  SetCategory(2576);
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
		return 1;
	}
}

local pCapsule;

public func GetCapsule() {
	return pCapsule;
}

public func SetCapsule(object pObj) {
	pCapsule = pObj;
}
