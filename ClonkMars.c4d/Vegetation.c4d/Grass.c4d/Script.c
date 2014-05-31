/*-- Grass --*/

#strict 2

#include TRE5

protected func Construction() { 
	// Zufällige Form  
	SetAction("Grass");
	SetPhase(Random(2));
	if (!Random(20)) SetPhase(2);
	// Zufällige Richtung
	if (Random(2)) SetDir(DIR_Right);

	inherited(...);

	// Höhe anpassen
	while (!GBackSolid(0, 5))
		SetPosition(GetX(), GetY() + 1);
	// Gras bleibt hinter Bäumen
	MoveBehindTrees();
}

private func MoveBehindTrees() {
	for (var obj in FindObjects(Find_AtPoint(), Find_OCF(OCF_Chop), Find_NoContainer(), Find_Category(C4D_StaticBack), Find_Func("IsTree")))
		SetObjectOrder(obj, this, 1);
}
