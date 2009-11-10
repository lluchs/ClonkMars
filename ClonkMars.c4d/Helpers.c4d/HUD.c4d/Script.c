/*-- HUD --*/

#strict 2

func Initialize() {
  SetPosition(150,90);
  SetVisibility(VIS_Owner);
	SetStillOverlayAction("lowoxywarning1", HUD_O2Warning);
  return(1);
}

static const HUD_O2 = 1;
static const HUD_Temp = 2;
static const HUD_O2Warning = 3;

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
		else if(iType == HUD_Temp)
			HUD -> UpdateTemp(iValue);
	}
}

public func UpdateO2(int iO2) {
	if(iO2 % 10 || iO2 < 0 || iO2 > 100) {
		DebugLog("ERROR: HUD: ungültige Sauerstofffüllung übergeben");
		return;
	}
	SetGraphics(0, this, GetID(), HUD_O2, GFXOV_MODE_Action, Format("oxygen%d", iO2));
	SetObjDrawTransform(1000, 0, -60000, 0, 1000, -23000, this, HUD_O2);
	
	if(!warning && iO2 <= 30) {
		warning = 1;
		SwitchWarning();
		Sound("Warning_lowoxygen", true, 0, 0, GetOwner() + 1, +1);
	}
	else if(warning && iO2 > 30) {
		warning = 0;
		SetStillOverlayAction("lowoxywarning1", HUD_O2Warning);
		Sound("Warning_lowoxygen", true, 0, 0, GetOwner() + 1, -1);
	}
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

private func SetWarning() {
	SetStillOverlayAction(Format("lowoxywarning%d", warning), HUD_O2Warning);
	ScheduleCall(this, "SwitchWarning", 20);
}

private func SwitchWarning() {
	if(warning) {
		warning = warning - 1 || 2;
		SetWarning();
	}
}
