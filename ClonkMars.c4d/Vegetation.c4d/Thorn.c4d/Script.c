/*-- Stalaktit --*/

#strict 2

local phase;

protected func Construction() {
	SetAction("Hanging");
	SetPhase(phase=Random(4));

	// Drehung nach Erdoberfläche
	var x_off = 18 * GetCon() / 100;
	var y_off = 15 * GetCon() / 100;
	var slope = GetSolidOffset(-x_off, y_off) - GetSolidOffset(x_off, y_off);
	SetR(slope);

	ScheduleCall(this, "Grow", RandomX(100, 500));
}

private func Grow() {
	DoCon(1);
	if(GetCon() < 100)
		ScheduleCall(this, "Grow", 1);
}

protected func Damage() {
	if (GetDamage() > 10) {
		Destroy();
	}
}

private func Destroy() {
	var obj;
	Sound("RockBreak*");
	Sound("Blast2", 0, 0, 50);
	// links oben
	ObjectSetAction(obj=CreateObject(_STP, -4, -4, -1), "Exist");
	SetPhase(phase*2, obj);
	SetRDir(Random(20)-10, obj);
	SetSpeed(Random(30)-15, -Random(35)+10, obj);
	// rechts oben
	ObjectSetAction(obj=CreateObject(_STP, 3, -4, -1), "Exist");
	SetPhase(phase*2+1, obj);
	SetRDir(Random(20)-10, obj);
	SetSpeed(Random(30)-15, -Random(35)+10, obj);
	// unten
	ObjectSetAction(obj=CreateObject(_STP, 0, 12, -1), "LameExist");
	SetPhase(phase, obj);
	SetRDir(Random(20)-10, obj);
	SetSpeed(Random(10)-15, -Random(35)+10, obj);

	Regenerate();
	RemoveObject();
}

private func Regenerate() {
	// Find a new place to live.
	var wipf = PlaceAnimal(WIPF);
	var thorn = CreateObject(GetID(), AbsX(wipf->GetX()), AbsY(wipf->GetY()), NO_OWNER);
	thorn->SetCon(1);
	wipf->RemoveObject();
}
