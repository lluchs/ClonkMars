/* Baumsteuerung */

#strict 2

#appendto TREE

public func BurnedFrom() { // zum �berladen
	return;
}

private func PlaceVegetation() {
	var pObj = _inherited(...);
	if(!pObj || !pObj -> FindObject2(Find_Distance(TERA_RADIUS), Find_ID(TERA))) // Terraformingeinheit in der N�he?
		pObj -> RemoveObject();
	return pObj;
}
