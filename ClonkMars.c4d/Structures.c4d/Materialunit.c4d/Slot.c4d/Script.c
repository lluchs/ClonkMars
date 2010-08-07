/*-- Vorsicht vor dem Chippie --*/

#strict 2

local unit;

public func SetUnit(object pObj) {
	unit = pObj;
}

// wird aufgerufen, wenn ein Objekt reingesteckt wird
protected func Collection(object pObj) {
	pObj->Enter(unit);
}
