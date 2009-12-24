/*-- Materiallabor --*/

#strict 2

#include B_95
#include OVDR
#include PWRC // ist ein Energieverbraucher
#include DACT //Damagecontrol

static const MTLB_metal_earth_cost = 3;
static const MTLB_plastic_cost = 350;
public func GetTemp() { return 1000;}

func Initialize() {
  return(1);
}

private func CanOpen() {
	return GetAction() == "Idle";
}

private func SoundOpenDoor() {
	Sound("Door_Metal");
}

private func SoundCloseDoor() {
	Sound("Door_Metal");
}

private func EnergyNeedOverlay() {
	return 3; // Overlay 1 und 2: T�rsteuerung
}

/*protected func Overburn(){
  CreateParticle("Fire2", 20, -40, RandomX(-5,5), -10-Random(10), 75 + Random(75), RGBa(255, 230, 230, 50), this());
  CreateParticle("Fire2", 23, -40, RandomX(-5,5), -10-Random(10), 75 + Random(75), RGBa(255, 230, 230, 50), this());
  if(!Random(3))
    CreateParticle("Fire", 23, -40, RandomX(-5,5), -10-Random(10), 55, RGBa(255, 230, 230, 50), this());
    }*/

protected func ContainedUp(object pClonk) {
	return ProduceMenu(pClonk);
}

protected func ContainedDig(object pClonk) {
	return ProduceMenu(pClonk, true);
}

protected func ProduceMenu(object pClonk, fShowAll) {
	if(GetAction() != "Idle") {
		CreateMenu(GetID(), pClonk, this);
		if(fContinue)
			AddMenuItem("Abbruch", "fContinue = false;", RSR2, pClonk, 0, 0, "Abbruch der Dauerproduktion");
		AddMenuItem("Sofortabbruch", "Cancel", RSR2, pClonk, 0, 0, "Abbruch der aktuellen Produktion. Achtung: die Baumaterialien gehen dabei verloren!");
		return 1;
	}
	
	CreateMenu(GetID(), pClonk, this, C4MN_Extra_Components, "$TxtNothingProducible$");
	if(fShowAll || ObjectCount2(Find_Container(this), Find_ID(ORE1)) || ObjectCount2(Find_Container(this), Find_ID(ERTH)) >= MTLB_metal_earth_cost) {
		AddMenuItem("Metall", "ProduceMetal", METL, pClonk);
	}
	if(fShowAll || ObjectCount2(Find_ID(OILT), Find_Func("PipelineConnectedWith", this))) {
		AddMenuItem("Plastik", "ProducePlastic", PSTC, pClonk);
	}
	
	// Lore ebenfalls hardcoded..
	if(fShowAll || CheckComponents(LORY)) {
		AddMenuItem(GetName(0, LORY), "ProduceLorry", LORY, pClonk);
	}
	
	// Universalherstellung von Objekten
	var i, idDef, iPlr = pClonk -> GetOwner();
	while(idDef = GetPlrKnowledge(iPlr, 0, i++, C4D_Object)) {
		if(fShowAll || CheckComponents(idDef))
			AddMenuItem(GetName(0, idDef), "Produce", idDef, pClonk);
	}
	return 1;
}

/* Produce */

local idProduce, fContinue;

protected func ProduceMetal(idItem, iParameter, fRight) {
	if (GetAction() != "Idle" || DoorActive || !FindObject2(Find_Container(this), Find_OCF(OCF_CrewMember)) || !CheckPower(100, true))
		return fContinue = false;
	var pOre = FindObject2(Find_Container(this), Find_ID(ORE1));
	var aEarth = FindObjects(Find_Container(this), Find_ID(ERTH));
	if(pOre || GetLength(aEarth) >= MTLB_metal_earth_cost) {
		if (pOre)
			pOre -> RemoveObject();
		else {
			for (var i = 0; i < MTLB_metal_earth_cost; ++i)
				aEarth[i] -> RemoveObject();
		}
		CheckPower(100);
		SetAction("Produce");
		idProduce = METL;
		if(fRight)
			fContinue = true;
	}
	else {
		Sound("Error");
		fContinue = false;
	}
}

protected func ProducePlastic(idItem, iParameter, fRight) {
	if(GetAction() != "Idle" || DoorActive || !FindObject2(Find_Container(this), Find_OCF(OCF_CrewMember)) || !CheckPower(100, true))
		return fContinue = false;
	for(var pTank in FindObjects(Find_ID(OILT), Find_Func("PipelineConnectedWith", this))) {
		if(pTank -> GetLiquidStorage() >= MTLB_plastic_cost) {
			CheckPower(100);
			pTank -> DoLiquidStorage(-MTLB_plastic_cost);
			SetAction("Produce");
			idProduce = PSTC;
			if(fRight)
				fContinue = true;
			return 1;
		}
	}
	
	Sound("Error");
	Message("$TxtNotEnoughOil$", this);
	fContinue = false;
}

protected func ProduceLorry(idItem, iParameter, fRight) {
	if (GetAction() != "Idle" || DoorActive || !FindObject2(Find_Container(this), Find_OCF(OCF_CrewMember)) || !CheckPower(100, true))
		return fContinue = false;
	
	if(CheckComponents(idItem, true)) {
		SetAction("Produce");
		idProduce = idItem;
		if(fRight)
			fContinue = true;
	}
	else {
		Sound("Error");
		fContinue = false;
	}
}

private func CheckComponents(id idItem, bool fRemove) {
	var i, ID, iAmount, aObjs = [];
	while(ID = GetComponent(0, i++, 0, idItem)) {
		iAmount = GetComponent(ID, 0, 0, idItem);
		for(var pObj in FindObjects(Find_Container(this), Find_ID(ID))) {
			if(fRemove)
				aObjs[GetLength(aObjs)] = pObj;
			if(!--iAmount)
				break;
		}
		if(iAmount)
			return;
	}
	if(fRemove) {
		for(var pObj in aObjs)
			pObj -> RemoveObject();
	}
	
	return 1;
}

// Universal-Herstellen
protected func Produce(idItem, iParameter, fRight) {
	if (GetAction() != "Idle" || DoorActive || !FindObject2(Find_Container(this), Find_OCF(OCF_CrewMember)) || !CheckPower(100, true))
		return fContinue = false;
	
	if(CheckComponents(idItem, true)) {
		SetAction("Produce2");
		idProduce = idItem;
		if(fRight)
			fContinue = true;
	}
	else {
		Sound("Error");
		fContinue = false;
	}
}

protected func Finish() {
	CreateContents(idProduce);
	
	// HUD-Update f�r alle Spieler, die einen Clonk hier drin haben
	var aPlr = CreateArray(GetPlayerCount()), iPlr;
	for(var pClonk in FindObjects(Find_Container(this), Find_OCF(OCF_CrewMember))) {
		iPlr = pClonk -> GetOwner();
		if(!aPlr[iPlr]) {
			UpdateHUD(iPlr, HUD_ItemLog, idProduce);
			aPlr[iPlr] = true;
		}
	}
	
	Sound("Pshshsh");
	if(fContinue) {
		if(idProduce == METL)
			ProduceMetal(0, 0, true);
		else if(idProduce == PSTC)
			ProducePlastic(0, 0, true);
		else if(idProduce == LORY)
			ProduceLorry(idProduce, 0, true);
		else
			Produce(idProduce, 0, true);
	}
}

protected func Cancel() {
	SetAction("Idle");
	Sound("Pshshsh");
	fContinue = false;
}
  
/*Smoke*/

protected func Smoking() {
	if(!Random(2))
		Smoke(17, Random(3) - 23, Random(9) + 20);
	if(!Random(4))
		Smoke(17, Random(3) - 19, Random(5) + 20);
	if(GetActTime() >= 550)
		SetAction("Wait");
}

public func PipelineConnect() {
	return 1;
}

public func MaxDamage() { return 50; } //Maximaler Schaden

/* Kontext */

private func IsComplete()
{
  return (GetCon() >= 100); 
}

/* Erforschbar */

public func MarsResearch() {
	return true;
}

private func ClonkCapacity() {
	return 2;
}
