/*-- Sat --*/

#strict 2

#include L_RC // Recycling

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

public func CapsuleDestroyed() {
	pCapsule = -1;
	ScheduleCall(this, "CapsuleRecovered", 5*60*36);
	return 1;
}

public func CapsuleRecovered() {
	if(pCapsule == -1)
		pCapsule = 0;
}

public func MarsResearch() { return true; }

public func NoDeconstruction() { return true; } // Sat könnte sonst rückgebaut werden, während es hängt

/* Relaunch functionality */

local reproductionProgress;

// Putting a Clonk into a Sat dish turns it into a relaunch object.
protected func Collection2(object clonk) {
	if(reproductionProgress) return;

	// Make invisible and static.
	SetVisibility(VIS_None);
	SetCategory(C4D_StaticBack);
	SetPosition(LandscapeWidth() / 2, LandscapeHeight() / 2);

	Reproduction();
}

private func Reproduction() {
	var clonk = Contents();
	reproductionProgress++;
	clonk->UpdateHUDValue(HUD_Gencode, reproductionProgress);
	var targetEnergy = Max(1, reproductionProgress * clonk->GetPhysical("Energy") / 100000);
	clonk->DoEnergy(targetEnergy - clonk->GetEnergy());
	if(reproductionProgress == 100) {
		var caps = CreateObject(CPSL, AbsX(RandomX(200, LandscapeWidth() - 200)), AbsY(-30), clonk->GetOwner());
		// Activate boosters automatically.
		caps->SetDstPort(0, true);
		clonk->Enter(caps);
		clonk->Birth();
		clonk->Schedule("UpdateHUDValue(HUD_Gencode, 0)", 50);

		RemoveObject();
		return;
	}
	ScheduleCall(this, "Reproduction", 40);
}

public func IsO2Producer() { return true; }
public func GetTemp() { return 250; }
