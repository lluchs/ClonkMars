/* Viewport */

#strict 2

local wdt, hgt;
local freeze;

static const VWPT_Freeze = 1;

protected func Initialize() {
	UpdateViewportSize();
}

public func UpdateViewportSize() {
	var iPlr = GetOwner();
	wdt = GetPlrExtraData(iPlr, "ScreenWidth");
	hgt = GetPlrExtraData(iPlr, "ScreenHeight");
	if(!wdt) {
		wdt = 1024;
		hgt = 768;
	}
}

global func SetViewportFreeze(int iPlr, int iFreeze) {
	var pObj = FindObject2(Find_ID(VWPT), Find_Owner(iPlr));
	if(pObj)
		return pObj -> SetFreeze(iFreeze);
}

public func SetFreeze(int iFreeze) {
	if(!freeze) {
		SetGraphics("Frosty", this, GetID(), VWPT_Freeze, GFXOV_MODE_Base);
		SetObjDrawTransform(1000 * wdt / 1024, 0, 0, 0, 1000 * hgt / 768, 0, this, VWPT_Freeze);
		freeze = true;
	}
	
	if(iFreeze == 255) {
		if(freeze) {
			SetGraphics(0, this, 0, VWPT_Freeze);
			freeze = false;
		}
	}
	else
		SetClrModulation(RGBa(255, 255, 255, iFreeze), this, VWPT_Freeze);
	return 1;
}
