/*-- Upgrade: Rock Crusher --*/

#strict 2

public func IsUpgrade() { return true; }

public func CanApply(object pLab, object pClonk) {
	return !(pClonk -> ~HeavySuit());
}

public func Apply(object pLab, object pClonk) {
	pClonk -> ~UpgradeSuit();
}
