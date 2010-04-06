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
	var mat = GetMaterial();
	// Passendes Ziel mit genügend Platz vorhanden?
	if(!(pTarget -> Transfer(3, mat, true)))
		return StopTransfer(pTarget);
	
	if(!HasEnergy || GetActTime() % 10) {
		if(pTarget -> CheckPower(5))
			HasEnergy = true;
		else {
			HasEnergy = false;
			return StopTransfer(pTarget);
		}
	}
	
	// in einem Durchgang wird nur ein Materialtyp extrahiert
	for(var i = 3; i; i--) {
		if(ExtractLiquid() == mat)
			iExtracted++;
		else
			break;
	}
	if(pTarget -> GetAction() != "Pump")
		pTarget -> SetAction("Pump", this);
	pTarget -> Transfer(iExtracted, mat);
}

private func StopTransfer(object pTarget) {
	if(pTarget -> GetAction() != "Wait")
		pTarget -> SetAction("Wait", this);
}
