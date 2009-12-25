/*-- Rig --*/

#strict 2
#include DRCK
#include B_50
#include PWRC
#include DACT //Damagecontrol

local pLamp;
func Initialize() {
  SetAction("Wait");
  pLamp = AddLightCone(1000, RGBa(255, 255, 255, 10));
	pLamp -> ChangeSizeXY(800, 1000);
	pLamp -> ChangeOffset(-3,-42);
	pLamp -> ChangeR(105);
	pLamp -> LocalN("bAmbience") = true;
  return _inherited(...);
}

protected func Incineration() {
	pLamp -> RemoveObject();
	pDrillHead -> RemoveObject();
	return _inherited(...);
}

public func Deconstruction() {
	pLamp -> RemoveObject();
	pDrillHead -> RemoveObject();
	return _inherited(...);	
}

private func EnergyCheck(int iEnergy, object pObj) {
	if(!pObj)
		pObj = this;
	return pObj -> CheckPower(iEnergy / 200);
}

protected func Pumping() {
	if(!ReadyToPump() || GetEffect("EnergyNeed", this))
		SetAction("Wait", pDrillHead);
}

private func PipeHeadCheck()
{
  // Bei Bedarf Bohrkopf und Leitung erzeugen
  if (!pDrillHead) 
  {              
    // Bohrkopf
    pDrillHead = CreateObject(PIPH, 13, 36, GetOwner());
    SetAction("Pump", pDrillHead);
    pDrillHead -> SetAction("Stop", this);
    CreateObject(DPLI, 13, 0, -1)->DPLI::Init(this, pDrillHead);
  }
  else {
  	if(GetAction() != "Pump")
  		SetAction("Pump", pDrillHead);
  }
  return(pDrillHead);
}

public func Transfer(int iAmount, bool fNoChange) {
	for(var pTank in FindObjects(Find_ID(OILT), Find_Func("PipelineConnectedWith", this))) {
		if(pTank -> GetLiquidStorage() + iAmount <= pTank -> GetMaxLiquidStorage()) {
			if(!fNoChange)
				pTank -> DoLiquidStorage(iAmount);
			return 1;
		}
	}
	return;
}

public func MaxDamage() { return 18; } //Maximaler Schaden

public func PipelineConnect() {
	return 1;
}

public func MarsResearch() {
	return true;
}

