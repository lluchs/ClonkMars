/*-- Upgrade: Rock Crusher --*/

#strict 2

public func IsUpgrade() { return true; }

public func UpgradeCompleted(int iPlr) {
	SetPlrKnowledge(iPlr, ERTH);
}
