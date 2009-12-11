/* Baumsteuerung */

#strict 2

#appendto TREE

protected func Construction() {
	if(!FindObject2(Find_Distance(TRFM_RADIUS), Find_ID(TRFM))) { // Terraformingeinheit in der Nähe?
		RemoveObject();
		return;
	}
	AddEffect("TerraformerCheck", this, 10, 50, this);
}

protected func FxTerraformerCheckTimer() {
	if(!FindObject2(Find_Distance(TRFM_RADIUS), Find_ID(TRFM), Find_Func("IsTerraforming"))) { // aktive Terraformingeinheit in der Nähe?
		DoCon(-1);
		if(GetCon() <= 1)
			RemoveObject();
		TreeHurt(5 + Random(10));
	}
}

private func TreeHurt(int iNum, int iLevel) {
	if(!iLevel)
		iLevel = 15 + Random(10);
	var ox, oy, wdt, hgt;
	GetRect(GetID(), ox, oy, wdt, hgt);
	ox = ox * GetCon() / 100;
	oy = oy * GetCon() / 100;
	wdt = wdt * GetCon() / 100;
	hgt = hgt * GetCon() / 100;
	iNum = iNum * GetCon() * 2 / 100;
	for(var cnt=0 ;cnt < iNum; cnt++)
		CreateParticle("GrassBlade",RandomX(ox, wdt), RandomX(oy, hgt),RandomX(-iLevel/3 ,iLevel/3),RandomX(-2*iLevel/3,-iLevel/3),30+Random(30),RGB(255,255,255),0,0);
}

protected func Damage() {
	TreeHurt(15 + Random(10));
	if (GetDamage() > TreeStrength() * Max(GetCon(), 30)/100) {
		TreeHurt(10);
		RemoveObject();
	}
}
