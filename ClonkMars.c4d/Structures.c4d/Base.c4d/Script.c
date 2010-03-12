/*--Basis--*/

#strict 2

#include B_95
#include OVDR
#include PWRC
#include DACT //Damagecontrol
#include L_CA

public func GetTemp() { return 800;}

private func EnergyNeedOverlay() {
	return 3; // Overlay 1 und 2: Türsteuerung
}

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
	return !IsResearching();
}

protected func Collection2(object pObj) {
	if(pObj -> OnFire())
		pObj -> Extinguish();
}

/* Steuerung */

protected func ContextResearch(object pClonk) {
	[$TxtResearch$|Image=RSR1|Condition=IsNotResearching]
	ResearchMenu(pClonk);
	return 1;
}

private func IsNotResearching() {
	return !IsResearching();
}

protected func ContextCancelResearch(object pClonk) {
	[$TxtCancelresearch$|Image=RSR2|Condition=IsResearching]
	PauseAllResearch();
	return 1;
}

protected func ContextRemoveSat() {
	[Sat abmontieren|Image=SATD|Condition=HasSat]
	HasSat() -> SetAction("Idle");
}

protected func ContextStartReproduction(object pClonk) {
	[Reproduktion|Image=SCNK|Condition=IsNotReproducing]
	pClonk -> StartReproduction();
}

protected func ContextStopReproduction(object pClonk) {
	[Reproduktion stoppen|Image=SCNK|Condition=IsReproducing]
	pClonk -> StopReproduction();
}

private func IsReproducing(object pClonk) {
	return pClonk -> ~IsReproducing();
}

private func IsNotReproducing() {
	return !IsReproducing(...);
}

protected func ContextBuy(object pClonk) {
	[Objekte kaufen|Image=SATD|Condition=CapsuleCheck2]
	pClonk->ContainedCall("ContainedUp", this());
}

protected func CapsuleCheck2() {
	return CapsuleCheck();
}

protected func ContainedUp(object pClonk) {
	if(!CapsuleCheck(pClonk,true))
		return;
	var menu = CreateMenuTemplate(CPSL, "$TxtCallCapsule$");
	AddEnumChoice(menu, 0, "Sell");
		AddEnumChoiceItem(menu, 0, "Sell", "SellOnly", "$TxtSell$", 0, "SellOnly");
		AddEnumChoiceItem(menu, 0, "Sell", "Buy", "$TxtBuy$", 0, "Buy", true);
	AddSubmenu(menu, 0, "Buy", MenuCond_Chosen(0, "Sell", "Buy"), "$TxtOrder$");
		var i = 0, ID;
		while(ID = GetHomebaseMaterial(pClonk -> GetOwner(), 0, i++, C4D_Object)) {
			AddRangeChoice(menu, ["Buy"], ID, 0, GetName(0, ID), ID, 0, GetHomebaseMaterial(pClonk -> GetOwner(), ID), 1, 0);
		}
	
	CreateMenuByTemplate(pClonk, this, "OrderCapsule", menu, pClonk);
	
	return 1;
}

public func OrderCapsule(hash, object pClonk, bool fCanceled) {
	if(fCanceled || !CapsuleCheck(pClonk))
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

private func CapsuleCheck(object pClonk, bool announce) {
	var pSat = HasSat();
	if(!pSat)
		return false;
	if(pSat -> GetCapsule() == -1) {
		if(announce) {
			Sound("Error");
			Message("$TxtCapsuleDestroyed$", pClonk);
		}
		return false;
	}
	if(pSat -> GetCapsule()) {
		if(announce) {
			Sound("Error");
			Message("$TxtAlreadyCapsule$", pClonk);
		}
		return false;
	}
	if(IsStormy()) {
		if(announce) {
			Sound("Error");
			Message("$TxtTooStormy$"); 
		}
		return false;	
	}
	return true;
}

private func CreateCapsule() {
	var pPort, pCaps;
	if(pPort = FindObject2(Find_ID(PORT), Find_OCF(OCF_Fullcon), Find_Allied(GetOwner()), Find_Distance(700), Find_Func("IsFree"), Sort_Distance())) {
		pCaps = CreateObject(CPSL, AbsX(GetX(pPort)), AbsY(-100), GetOwner());
	} else {
		var iX;
		while((iX = RandomX(-200, 200) + GetX()) < 0 || iX > LandscapeWidth());
		pCaps = CreateObject(CPSL, AbsX(iX), AbsY(-30), GetOwner());
	}
	pCaps->SetDstPort(pPort);
	
	HasSat() -> SetCapsule(pCaps);
	pCaps -> SetSat(HasSat());
	return pCaps;
}

private func HasSat() {
	return FindObject2(Find_ID(SATD), Find_Action("FlyBase"), Find_ActionTarget(this));
}

/* Wärme */

public func GetWarmth() {
  return(100);
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

global func FindBase(int iPlr) {
	return FindObject2(Find_Owner(iPlr), Find_Func("IsBase"), Sort_Distance()); // FIXME-performance: Use ID for speed optimisation
}

global func GetBase(object pObject) {
	if(!pObject && !(pObject=this)) return NO_OWNER; 
	var sat = pObject->~HasSat();
	if(sat) return GetOwner(sat);
	return NO_OWNER;
}

/* Forschung */

public func IsResearching() {
	for(var iNumber, i = 0; iNumber = GetEffect("Research", this, i); i++) {
		if(GetEffect(0, this, iNumber, 3))
			return true;
	}
}

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
 * 4: Sound wird abgespielt?
 */

protected func FxResearchStart(object pTarget, int iEffectNumber, int iTemp, id idDef, object pClonk) {
	if(iTemp)
		return;
	EffectVar(1, pTarget, iEffectNumber) = idDef;
	var iTime = idDef -> ~GetResearchTime(this, pClonk);
	if(!iTime)
		iTime = RandomX(50, 200);
	EffectVar(2, pTarget, iEffectNumber) = iTime;
	Sound("Lab_Research", 0, 0, 0, 0, +1);
	EffectVar(4, pTarget, iEffectNumber) = true;
}

protected func FxResearchEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber, id idDef, object pClonk) {
	if(szNewEffectName == "Research" && idDef == EffectVar(1, pTarget, iEffectNumber)) {
		ContinueResearch(iEffectNumber);
		return -1;
	}
}

protected func FxResearchTimer(object pTarget, int iEffectNumber, int iEffectTime) {
	if(!CheckPower(10)) {
		Sound("Lab_Research", 0, 0, 0, 0, -1);
		EffectVar(4, pTarget, iEffectNumber) = false;
		return;
	}
	if(!EffectVar(4, pTarget, iEffectNumber)) {
		Sound("Lab_Research", 0, 0, 0, 0, +1);
		EffectVar(4, pTarget, iEffectNumber) = true;
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
	SetPlrKnowledge(GetOwner(), EffectVar(1, pTarget, iEffectNumber));
	// Nachricht ausgeben
	Message("$Txtsdeveloped$", this, GetName(0, EffectVar(1, pTarget, iEffectNumber)));
}

/* Erforschbar */

public func MarsResearch() {
	return true;
}

public func MaxDamage() { return 50; } //Maximaler Schaden

private func ClonkCapacity() {
	return 3;
}
