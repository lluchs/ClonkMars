/*-- Sat --*/

#strict 2

func ResetCategory()
{
  SetCategory(2576);
  return 1;
}

func OnOwnerChanged(int new_owner, int old_owner)
{
  // Flagge hat den Besitzer verloren? Dann runterfallen!
  if (new_owner == NO_OWNER) SetAction("Idle");
}

protected func Hit() {
  Sound("MetalHit*");
  return(1);
}

protected func Entrance(object pContainer) {
	if(pContainer -> ~IsBase()) {
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
