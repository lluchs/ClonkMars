/*--- Laser Flint ---*/

#strict 2

static const LFLN_radius = 100;

protected func Hit() {
	var crystals = FindObjects(Find_ID(CRY2), Find_Distance(LFLN_radius));
	for (var crystal in crystals) {
		var laser = LaserConnection(this, crystal);
		laser->SetClrModulation(RGB(Random(255), Random(255), Random(255)));
		crystal->CreateObject(CRYS, 0, 0, crystal->GetOwner());
		crystal->RemoveObject();
	}

	if (GetLength(crystals)) {
		Sound("DeEnergize");
		RemoveObject();
	} else {
		Sound("Object_clunk");
	}
}

public func MarsResearch() { return true; }
