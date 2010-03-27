/* PipeHead */

#strict 2

#appendto PIPH

private func FindDerrick() {
	var pObj = FindObject2(Find_ID(RIG_), Find_ActionTarget(this), Find_Or(Find_Action("Pump"), Find_Action("Wait")));
	return pObj;
}

local HasEnergy;

protected func Transfer() {
	var pTarget = GetActionTarget(), iExtracted;
	if (!pTarget) return; //Bohrturm verschwunden?
	// Wir pumpen nur Öl
	if(!(pTarget -> Transfer(3, true) && GetMaterial() == Material("Oil")))
		return StopTransfer(pTarget);
	
	if(!HasEnergy || GetActTime() % 10) {
		if(pTarget -> EnergyCheck(5))
			HasEnergy = true;
		else {
			HasEnergy = false;
			return StopTransfer(pTarget);
		}
	}
	
	for(var i = 3; i; i--) {
		if(ExtractLiquid() == Material("Oil"))
			iExtracted++;
		else
			break;
	}
	if(pTarget -> GetAction() != "Pump")
		pTarget -> SetAction("Pump", this);
	pTarget -> Transfer(iExtracted);
}

private func StopTransfer(object pTarget) {
	if(pTarget -> GetAction() != "Wait")
		pTarget -> SetAction("Wait", this);
}
