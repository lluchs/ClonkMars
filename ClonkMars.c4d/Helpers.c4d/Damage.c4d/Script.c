/*-- Explosionsteuerung --*/

#strict 2

/* Dieses Objekt stellt die Grundfunktionalität für Gebäudeexplosionen
   zur Verfügung. */
   
static StructComp;

protected func Construction() {
	if(GetType(StructComp) != C4V_Array)
		StructComp = CreateHash();
	if(!HashContains(StructComp, GetID())) {
		var ID = GetID();
		var aIDs, aNum, iNum = 0;
		//IDs finden, aus denen das Gebäude zusammengesetzt ist
		aIDs = CreateArray();
		for (var i = 0, component; component = GetComponent(0, i, 0, ID); ++i) {
			aIDs[i] = component; 
		}
		aNum = CreateArray(GetLength(aIDs));
		//Componenten vor der Änderung finden
		for (var i = 0; i < GetLength(aIDs); ++i) {
			aNum[i] = GetComponent(aIDs[i], 0, 0, ID);
			iNum += aNum[i];
		}
		
		HashPut(StructComp, ID, [aIDs, aNum, iNum]);
	}
	return _inherited(...);
}

public func Damage (int iChange)
{
	if (GetDamage() > MaxDamage())
		AddEffect("MaxDamageExplosion", this, 1, 20, this, DACT);
	var ox, oy, wdt, hgt;
	GetRect(GetID(), ox, oy, wdt, hgt);
	var glascount = iChange + Random(4);
	for (var i = 0; i < glascount; ++i) {
		CastParticles("Glas", 1, RandomX(30,50), ox+Random(wdt), oy+Random(hgt), 20, 20);
	}
	var frazzlecount = GetDamage() * 5 / MaxDamage();
	for (var i = 0; i < frazzlecount; ++i) {
		CastParticles("Fragment1", 1, RandomX(30,50), ox+Random(wdt), oy+Random(hgt), 20, 20);		
	}
	
	/* Components verlieren */
	
	// erstmal alles durchzählen
	var aIDs, aNum, iNum = 0;
	//IDs finden, aus denen das Gebäude zusammengesetzt ist
	aIDs = CreateArray();
	for (var i = 0, component; component = GetComponent(0, i); ++i) {
		aIDs[i] = component; 
	}
	aNum = CreateArray(GetLength(aIDs));
	//Componenten vor der Änderung finden
	for (var i = 0; i < GetLength(aIDs); ++i) {
		aNum[i] = GetComponent(aIDs[i]);
		iNum += aNum[i];
	}
	
	var aNormal = HashGet(StructComp, GetID());
	
	// wie viele Components müssen entfernt werden?
	var iCompDiff = aNormal[2] - iNum;
	var iDamDiff = ChangeRange(GetDamage(), 0, MaxDamage(), 0, aNormal[2]);
	//Log("CompDiff: %d; DamDiff: %d; MaxDamage: %d; Normal: %d", iCompDiff, iDamDiff, MaxDamage(), aNormal[2]);
	iDamDiff -= iCompDiff;
	
	// Components zufällig entfernen
	for(var i = 0; iNum && i < iDamDiff; i++) {
		var c = Random(GetLength(aIDs));
		if(aNum[c]) {
			SetComponent(aIDs[c], --aNum[c]);
			iNum--;
			//Log("Removed: %i", aIDs[c]);
		}
		else
			i--; // nochmal versuchen
	}
}

public func FxMaxDamageExplosionTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	if (!pTarget) return -1; //Wir sind nicht Global!
	var ox, oy, wdt, hgt;
	GetRect(pTarget->GetID(), ox, oy, wdt, hgt);
	for (var i = 0; i < 10; ++i) {
		pTarget->CastParticles("Fragment1", 1, RandomX(50,150), ox+Random(wdt), oy+Random(hgt), 20, 30);		
	}
	if (iEffectTime > 1 * 35) {
		for (var i = 0; i < 10; ++i) {
			pTarget->CastParticles("Fragment2", 1, RandomX(50,150), ox+Random(wdt), oy+Random(hgt), 20, 30);
			pTarget->CastParticles("Fragment3", 1, RandomX(50,150), ox+Random(wdt), oy+Random(hgt), 20, 30);	
		}		
	}
	if (iEffectTime > 3 * 35) {
		DestroyBlast(pTarget);
		return -1;
	}
}

private func DestroyBlast(object pTarget) {
	if(!pTarget) if(!(pTarget=this)) return;
	var ox, oy, wdt, hgt;
	GetRect(pTarget->GetID(), ox, oy, wdt, hgt);
	var metlcount = GetComponent(METL, 0, 0, pTarget -> GetID());
	var power = Sqrt(wdt ** 2 + hgt ** 2);
	pTarget->CastObjects(ORE1,metlcount/2,power);
	for(var pObj in FindObjects(Find_Container(pTarget), Find_Not(Find_Func("IsLight"))))
		pObj -> Exit(0, Random(wdt) - wdt / 2, Random(hgt) - hgt / 2);
	pTarget -> Explode(power / 2);
	return true;
}

public func MaxDamage () { return 1; } //Überlade mich!
