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
static const HUD_ItemLog = 5;

local warning;

global func UpdateHUD(int iPlr, int iType, int iValue) {
	var HUDs;
	if(iPlr == NO_OWNER)
		HUDs = FindObjects(Find_ID(MHUD));
	else
		HUDs = FindObjects(Find_ID(MHUD), Find_Owner(iPlr));
	if(!HUDs[0])
		return;
	for(var HUD in HUDs) {
		if(iType == HUD_O2)
			HUD -> UpdateO2(iValue);
		else if(iType == HUD_Fuel)
			HUD -> UpdateFuel(iValue);
		else if(iType == HUD_Gencode)
			HUD -> UpdateGencode(iValue);
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

public func UpdateTemp(int iTemp) {
	if(iTemp % 20 || iTemp < -80 || iTemp > 80) {
		DebugLog("ERROR: HUD: ungültige Temperatur übergeben");
		return;
	}
	SetGraphics(0, this, GetID(), HUD_Temp, GFXOV_MODE_Action, Format("%d", iTemp));
	SetObjDrawTransform(1000, 0, -8000, 0, 1000, -18000, this, HUD_Temp);
	return 1;
}
