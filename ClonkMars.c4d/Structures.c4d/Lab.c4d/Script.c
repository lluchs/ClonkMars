/*-- Lab --*/

#strict 2

#include B_80
#include OVDR
#include PWRC // ist ein Energieverbraucher
#include DACT //Damagecontrol

private func EnergyNeedOverlay() {
	return 3; // Overlay 1 und 2: Türsteuerung
}

/* Türsteuerung */

private func CanOpen() {
	return 1;
	//return GetAction() == "Idle";
}

private func SoundOpenDoor() {
	Sound("Door_Metal");
}

private func SoundCloseDoor() {
	Sound("Door_Metal");
}

func Initialize() {
  SetAction("SiGreen");
  AddBuildingLight();
  return(1);
}

protected func SiRed() {
	Sound("Lab_SiBeep_R");
}

/* Forschung */

private func ResearchMenu(object pClonk) {
  // Hauptmenü erzeugen
  CreateMenu(RSR1, pClonk, this);
  // alle gestarteten Forschungen, die noch nicht abgeschlossen wurden, durchgehen
  for(var iEffectNumber, i = 0; iEffectNumber = GetEffect("Research", this, i); i++) {
  	EffectVar(0, this, iEffectNumber) = pClonk;
  	AddMenuItem("$TxtContinuedevelopement$", "SelectResearch", EffectVar(1, this, iEffectNumber), pClonk, 0, iEffectNumber);
  }
  
  AddMenuItem("$TxtResearchs$", "ResearchObjects", RSR3, pClonk, 0, pClonk);
	AddMenuItem("$TxtResearchs$", "ResearchVehicles", RSR4, pClonk, 0, pClonk);
	AddMenuItem("$TxtResearchs$", "ResearchStructures", RSR5, pClonk, 0, pClonk);
}

protected func ResearchObjects(id idImage, object pClonk) { return SelectResearchCategory(idImage, pClonk, C4D_Object); }
protected func ResearchVehicles(id idImage, object pClonk) { return SelectResearchCategory(idImage, pClonk, C4D_Vehicle); }
protected func ResearchStructures(id idImage, object pClonk) { return SelectResearchCategory(idImage, pClonk, C4D_Structure); }

private func SelectResearchCategory(id idImage, object pClonk, int iCategory) {
	// Auswahlmenü erzeugen
	CreateMenu(idImage, pClonk, this, 0, "$TxtNothingelsetoresearc$");
	// Alle geladenen Baupläne überprüfen
	var i, idDef, iPlr = pClonk -> GetOwner();
	while(idDef = GetDefinition(i++, iCategory)) {
		// Passende Kategorie und dem Spieler noch nicht bekannt
		if (GetCategory(0, idDef) & C4D_Knowledge && !GetPlrKnowledge(iPlr, idDef) && idDef -> ~MarsResearch()) {
			// Keine Forschungsgrundlage nötig oder Forschungsgrundlage vorhanden
			var aBase = idDef -> ~GetResearchBase(), fHasBase = true;
			if(aBase) {
				if(GetType(aBase) != C4V_Array)
					aBase = [aBase];
				for(var idBase in aBase)
					if(!GetPlrKnowledge(iPlr, idBase))
						fHasBase = false;
				// Forschungslinks brauchen wir nicht
			}
			// Menüeintrag hinzufügen
			if(fHasBase) 
				AddMenuItem("$TxtResearchs$", "SelectResearch", idDef, pClonk, 0, pClonk);
		}
	}
}

protected func SelectResearch(id idDef, par) {
	var iEffectNumber, pClonk;
	if(GetType(par) == C4V_Int) {
		iEffectNumber = par;
		pClonk = EffectVar(0, this, iEffectNumber);
	}
	else
		pClonk = par;
	if(pClonk -> Contained() != this)
		return;
	StartResearch(idDef, pClonk, iEffectNumber);
}

protected func StartResearch(id idDef, object pClonk, int iEffectNumber) {
	// Spieler hat diesen Bauplan schon
	if(GetPlrKnowledge(pClonk -> GetOwner(), idDef)) {
		Message("$TxtAlreadyDeveloped$", this, GetName(0, idDef));
		Sound("Error");
		if(iEffectNumber)
			RemoveEffect(0, this, iEffectNumber, true);
		return; 
  }
  // Tür muss zu sein
  if(DoorActive) {
  	// gleich nochmal versuchen
  	ScheduleCall(this, "SelectResearch", 10, 0, idDef, pClonk, iEffectNumber);
  	return;
  }
  // Besitzer anpassen
  SetOwner(pClonk -> GetOwner());
	// Pause
	PauseAllResearch();
  // gibt es bereits einen entsprechenden Effekt?
  if(iEffectNumber)
  	ContinueResearch(iEffectNumber);
  // Effekt erstellen
  else
  	AddEffect("Research", this, 100, 15, this, GetID(), idDef, pClonk);
}

private func PauseAllResearch() {
  // alle gestarteten Forschungen pausieren
  for(var iNumber, i = 0; iNumber = GetEffect("Research", this, i); i++) {
  	PauseResearch(iNumber);
  }
  SetAction("SiGreen");
  Sound("Lab_Research", 0, 0, 0, 0, -1);
}

// es ist nicht sichergestellt, dass danach keine Forschung mehr läuft
// daher keine Änderung der Statuslichter
private func PauseResearch(int iNumber) {
	return ChangeEffect(0, this, iNumber, GetEffect(0, this, iNumber, 1), 0);
}

private func ContinueResearch(int iNumber) {
	Sound("Lab_Research", 0, 0, 0, 0, +1);
	return ChangeEffect(0, this, iNumber, GetEffect(0, this, iNumber, 1), 15);
}

protected func Ejection() {
	PauseAllResearch();
	return _inherited(...);
}

/* EffectVars:
 * 0: temporär gespeicherter Clonk
 * 1: ID, die erforscht wird
 * 2: benötigte Gesamtzeit
 * 3: bisherige Forschungsschritte
 */

protected func FxResearchStart(object pTarget, int iEffectNumber, int iTemp, id idDef, object pClonk) {
	if(iTemp)
		return;
	EffectVar(1, pTarget, iEffectNumber) = idDef;
	var iTime = idDef -> ~GetResearchTime(this, pClonk);
	if(!iTime)
		iTime = 100;
	EffectVar(2, pTarget, iEffectNumber) = iTime;
	Sound("Lab_Research", 0, 0, 0, 0, +1);
}

protected func FxResearchTimer(object pTarget, int iEffectNumber, int iEffectTime) {
	if(!CheckPower(10)) {
		if(GetAction() != "SiRed")
			SetAction("SiRed");
		Sound("Lab_Research", 0, 0, 0, 0, -1);
		return;
	}
	if(GetAction() != "SiYellow") {
		Sound("Lab_Research", 0, 0, 0, 0, +1);
		SetAction("SiYellow");
	}
	var iProgress = ++EffectVar(3, pTarget, iEffectNumber);
	if(iProgress == EffectVar(2, pTarget, iEffectNumber))
		return -1;
}

protected func FxResearchStop(object pTarget, int iEffectNumber, int iReason, bool fTemp) {
	if(iReason)
		return;
	Sound("Lab_Research", 0, 0, 0, 0, -1);
	Sound("ResearchDone");
	SetAction("SiGreen");
	SetPlrKnowledge(GetOwner(), EffectVar(1, pTarget, iEffectNumber));
	// Nachricht ausgeben
	Message("$Txtsdeveloped$", this, GetName(0, EffectVar(1, pTarget, iEffectNumber)));
}

/* Steuerung */

public func ContainedUp(object pClonk) {
	[$TxtResearch$|Image=RSR1]
	ResearchMenu(pClonk);
	return 1;
}

/* Wärme */

public func GetWarmth() {
  return(150);
}

public func MaxDamage() { return 60; } //Maximaler Schaden

public func PipelineConnect() {
	return 1;
}
