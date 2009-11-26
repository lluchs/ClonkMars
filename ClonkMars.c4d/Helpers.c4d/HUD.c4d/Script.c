/*-- HUD --*/

#strict 2

func Initialize() {
  SetPosition(150,90);
  SetVisibility(VIS_Owner);
  
  SetStillOverlayAction("Temperatur", HUD_Temp);
  SetStillOverlayAction("ItemLog", HUD_ItemLog);
  return(1);
}

static const HUD_O2 = 1;
static const HUD_Fuel = 2;
static const HUD_Gencode = 3;
static const HUD_Temp = 4;
static const HUD_ItemLog = 5; // +2

local warning;

global func UpdateHUD(int iPlr, int iType, value) {
	var HUDs;
	if(iPlr == NO_OWNER)
		HUDs = FindObjects(Find_ID(MHUD));
	else
		HUDs = FindObjects(Find_ID(MHUD), Find_Owner(iPlr));
	if(!HUDs[0])
		return;
	for(var HUD in HUDs) {
		if(iType == HUD_O2)
			HUD -> UpdateO2(value);
		else if(iType == HUD_Fuel)
			HUD -> UpdateFuel(value);
		else if(iType == HUD_Gencode)
			HUD -> UpdateGencode(value);
		else if(iType == HUD_ItemLog)
			HUD -> AddLog(value);
	}
}

public func UpdateO2(int iO2) {
	if(iO2 < 0 || iO2 > 100) {
		DebugLog("ERROR: HUD: ungültige Sauerstofffüllung übergeben");
		return;
	}
	
	SetStillOverlayAction(Format("O2%d", 100-iO2), HUD_O2);
	
	if(!warning && iO2 <= 30) {
		warning = 1;
		Sound("Warning_lowoxygen", true, 0, 0, GetOwner() + 1, +1);
	}
	else if(warning && iO2 > 30) {
		warning = 0;
		Sound("Warning_lowoxygen", true, 0, 0, GetOwner() + 1, -1);
	}
	return 1;
}

public func UpdateFuel(int iFuel) {
	if(iFuel < 0 || iFuel > 100) {
		DebugLog("ERROR: HUD: ungültige Treibstofffüllung übergeben");
		return;
	}
	
	SetStillOverlayAction(Format("Fuel%d", 100-iFuel), HUD_Fuel);
	return 1;
}

public func UpdateGencode(int iValue) {
	if(iValue < 0 || iValue > 100) {
		DebugLog("ERROR: HUD: ungültige Treibstofffüllung übergeben");
		return;
	}
	
	SetStillOverlayAction(Format("Gencode%d", 100-iValue), HUD_Gencode);
	return 1;
}

public func AddLog(id ID) {
	if(!ID) {
		DebugLog("ERROR: HUD: keine ID");
		return;
	}
	
	AddEffect("Log", this, 10, 1, this, 0, ID);
}

protected func FxLogStart(object pTarget, int iEffectNumber, bool fTemp, id ID) {
	if(fTemp)
		return;
	EffectVar(0, pTarget, iEffectNumber) = [255, 0];
	EffectVar(1, pTarget, iEffectNumber) = CreateArray();
	
	DrawLogItem(1, ID, iEffectNumber);
	
	// auch Platz 2 initialisieren, aber unsichtbar machen
	DrawLogItem(2, ID, iEffectNumber);
	SetClrModulation(RGBa(255, 255, 255, 255), this, HUD_ItemLog+2);
}

protected func FxLogTimer(object pTarget, int iEffectNumber) {
	var alpha = EffectVar(0, pTarget, iEffectNumber);
	var waiting = EffectVar(1, pTarget, iEffectNumber);
	alpha[0]--;
	alpha[1]--;
	
	if(!alpha[0]) {
		if(alpha[1] <= 0)
			return -1;
		else {
			DrawLogItem(1, waiting[0], iEffectNumber);
			alpha[0] = alpha[1];
			DeleteArrayItem(0, waiting);
			if(waiting[0]) {
				DrawLogItem(2, waiting[0], iEffectNumber);
				alpha[1] = 255;
			}
			else
				alpha[1] = 0;
		}
	}
	
	SetClrModulation(RGBa(255, 255, 255, 255-alpha[0]), this, HUD_ItemLog+1);
	if(alpha[1] < 0)
		alpha[1] = 0;
	SetClrModulation(RGBa(255, 255, 255, 255-alpha[1]), this, HUD_ItemLog+2);
	EffectVar(0, pTarget, iEffectNumber) = alpha;
	EffectVar(1, pTarget, iEffectNumber) = waiting;
}

protected func FxLogEffect(string szEffectName) {
	if(szEffectName == "Log")
		return -2;
}

protected func FxLogAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer, id ID) {
	var alpha = EffectVar(0, pTarget, iEffectNumber);
	EffectVar(1, pTarget, iEffectNumber)[GetLength(EffectVar(1, pTarget, iEffectNumber))] = ID;
	if(alpha[1] <= 0) {
		DrawLogItem(2, ID, iEffectNumber);
		EffectVar(0, pTarget, iEffectNumber)[1] = 255;
	}
}

protected func FxLogStop(object pTarget, int iEffectNumber, int iReason, bool fTemp) {
	if(fTemp)
		return;
	
	// Grafiken zurücksetzen
	SetGraphics(0, this, 0, HUD_ItemLog+1);
	SetGraphics(0, this, 0, HUD_ItemLog+2);
}

private func DrawLogItem(int iItem, id ID, int iEffectNumber) {
	var iOverlay = HUD_ItemLog + iItem;
	
	SetGraphics(0, this, ID, iOverlay, GFXOV_MODE_IngamePicture);
	iItem--;
	// Größe des Bildes: angenommene 64x64 - leider stimmt das nicht, viel zu groß
	// daher muss hier die größe angepasst werden
	SetObjDrawTransform(1000, 0, OverlayShiftX(64) + 1000*(8 + iItem * 64), 0, 1000, OverlayShiftY(64) + 5000, this, iOverlay);
	
	return 1;
}

public func UpdateTemp(int iTemp) {
	if(iTemp % 20 || iTemp < -80 || iTemp > 80) {
		DebugLog("ERROR: HUD: ungültige Temperatur übergeben");
		return;
	}
	SetGraphics(0, this, GetID(), HUD_Temp, GFXOV_MODE_Action, Format("%d", iTemp));
	SetObjDrawTransform(1000, 0, -8000, 0, 1000, -18000, this, HUD_Temp);
	return 1;
}
