/*-- Explosionsteuerung --*/

#strict 2

/* Dieses Objekt stellt die Grundfunktionalität für Gebäudeexplosionen
   zur Verfügung. */

public func Damage (int iChange)
{
	if (GetDamage() > MaxDamage())
		AddEffect("MaxDamageExplosion", this, 1, 20, 0, DACT);
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
}

public func FxMaxDamageExplosionTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	if (!pTarget) return; //Wir sind nicht Global!
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
	var metlcount = pTarget->GetComponent(METL);
	var power = Sqrt(wdt ** 2 + hgt ** 2);
	AddLightFlash(power);
	pTarget->CreateParticle(Format("Blast%d",Random(3)), 0,0,0,0, power*10, RGBa(255,255,255,Random(128)));
	Sound("Blast3", false, pTarget, 100);
	pTarget->CastObjects(ORE1,metlcount/2,power);
	for(var pObj in FindObjects(Find_Container(this), Find_Not(Find_Func("IsLight"))))
		pObj -> Exit(0, Random(wdt) - wdt / 2, Random(hgt) - hgt / 2);
	BlastObjects(pTarget -> GetX(), pTarget -> GetY(), power);
	RemoveObject(pTarget, true);
	return true;
}

public func MaxDamage () { return 1; } //Überlade mich!
