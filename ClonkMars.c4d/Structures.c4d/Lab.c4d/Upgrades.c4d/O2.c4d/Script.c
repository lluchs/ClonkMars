/*-- Upgrade: Rock Crusher --*/

#strict 2

public func IsUpgrade() { return true; }

public func CanApply(object pLab, object pClonk) {
	return FindObject2(Find_Container(pLab), Find_ID(O2BL), Find_Not(Find_Func("IsUpgraded")));
}

public func Apply() {
	var pObj = CanApply(...);
	if(pObj)
		pObj -> Upgrade();
}
