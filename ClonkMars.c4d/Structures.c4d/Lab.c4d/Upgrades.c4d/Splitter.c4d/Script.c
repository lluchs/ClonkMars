/*-- Upgrade: Improved Splitters --*/

#strict 2

// Only upgradable after researching the station.
public func IsUpgrade(int iPlr) {
	return GetPlrKnowledge(iPlr, TRNS);
}

public func UpgradeCompleted(int iPlr) {
	// Handled in the splitter.
}
