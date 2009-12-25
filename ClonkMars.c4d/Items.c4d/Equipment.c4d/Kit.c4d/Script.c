/*--- Kit ---*/

#strict 2

static const SPCT_MaxUse = 3;

local Used;

protected func Activate(object pClonk) {
	var pSite = CreateConstruction(SPTR, 0, 10, pClonk -> GetOwner(), 1, true, true);
	if(pSite) {
		Used++;
		if(Used == SPCT_MaxUse)
			RemoveObject();
	}
	return 1;
}

public func MarsResearch() { return true; }
