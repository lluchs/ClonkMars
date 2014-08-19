/*--Panel--*/

#strict 2

#include PWRG // Ist ein Energieerzeuger
#include B_40
#include DACT //Damagecontrol
#include L_CA

public func GetCapacity() {
	return 150;
}

public func GetGeneratorPriority() {
	return 10;
}

local iPhases, iEnergy;

protected func Initialize() {
	iPhases = GetActMapVal("Length", "Panel", GetID());
	SetAction("Panel");
	AdjustSolarPanel();
}

protected func CheckDay() { // TimerCall
	AdjustSolarPanel();
	if(IsDay()) {
		ProduceEnergy();
	}
	else	
		iEnergy = 0;
}

// The upgradable maximum energy output.
private func BaseEnergy() {
	if(upgraded)
		return 25;
	else
		return 15;
}

private func ProduceEnergy() {
	var iBase = BaseEnergy();
	//regulates Power-producing by Sunstrength (powered by Luchs)
	iEnergy = (iBase - GetDarkness(iBase)) * PhaseFree() / 3;
	return DoPower(iEnergy);
}

private func AdjustSolarPanel() {
	if(GetAction() != "Panel")
		return;
	var pTime = FindObject2(Find_ID(TIME));
	if(!pTime)
		return SetPhase(iPhases / 2);
	
	var iTime = pTime -> Local(1), iLight = pTime -> Local(2);
	if(!iLight) {
		if(GetPhase())
			SetAction("Back");
		return;
	}
	
	var evening, morning;
	GetTimePoints(evening, morning);
	
	if(iTime < TimeResolution() / 2) {
		SetPhase(iPhases / 2 + iPhases / 2 * iTime / evening);
	}
	else {
		SetPhase((iPhases / 2) * (iTime - morning) / (TimeResolution() - morning));
	}
}

private func PhaseFree() {
	var iPhase = GetPhase();
	var iAngle = 270;
	if(iPhase < 18)
		iAngle -= iPhase * 2;
	else if(iPhase < 21)
		iAngle -= iPhase * 3;
	else if(iPhase < 33)
		iAngle -= iPhase * 11 / 3;
	else if(iPhase < 43)
		iAngle -= iPhase * 4;
	else
		iAngle = 90 + 50 - iPhase;
	
	return AngleFree(iAngle) + AngleFree(iAngle - 15) + AngleFree(iAngle + 15);
}

/* Upgrade */
local upgraded;

public func IsUpgradable() {
	return !upgraded && UpgradeComplete(GetOwner(), U_SP);
}

public func Upgrade() {
	upgraded = true;
	SetGraphics("Upgrade", this, GetID(), 1, GFXOV_MODE_Base);
}

/* Steuerung */

// aktuell erzeugte Energie anzeigen
protected func ControlUp() {
	[$TxtShowEnergy$|Image=CXEC]
	Message("$TxtShowEnergyMsg$", this, iEnergy);
}

protected func ContextShowEnergy() {
	[$TxtShowEnergy$|Image=CXEC]
	ControlUp();
}

/* Erforschbar */

public func MarsResearch() {
	return true;
}

public func MaxDamage() { return 21; } //Maximaler Schaden
