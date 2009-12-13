/*-- Temperatur --*/

#strict 2

protected func Initialize() {
	CreateLandTempEffects();
	
	for(var pClonk in FindObjects(Find_OCF(OCF_Alive), Find_Func("IsClonk")))
		OnClonkRecruitment(pClonk);
}

protected func OnClonkRecruitment(object pClonk) {
	AddEffect("Temperature", pClonk, 10, 35, 0, GetID());
}

/* Temperatureffekt f�r den Clonk */

/* EffectVars
 * 
 * 0: Temperatur (von -MaxTemp bis MaxTemp)
 */

protected func FxTemperatureTimer(object pTarget, int iEffectNumber) {
	var iTemp = EffectVar(0, pTarget, iEffectNumber);
	
	var iOuterTemp = ChangeRange(pTarget -> GetLandTemp(), -MaxTemp, MaxTemp, -80, 80);
	if(pTarget -> Contained())
		iOuterTemp = pTarget -> Contained() -> ~GetTemp();
	
	UpdateHUD(pTarget -> GetOwner(), HUD_Temp, Round(iOuterTemp, 20));
	
	var k = 5; // fixer Wachstumsfaktor, k�nnte je nach Situation ver�ndert werden
	// Beschr�nktes Wachstum
	// Bestand + k * (Schranke - Bestand)
	iTemp += k * (iOuterTemp - iTemp) / 10;
	
	// speichern
	EffectVar(0, pTarget, iEffectNumber) = iTemp;
	
	// Energieverlust bei zu niedrigen/hohen Werten
	
	var iLimit = MaxTemp * 3 / 4;
	if(Abs(iTemp) > iLimit) {
		// max. 10 Energieverlust
		pTarget -> DoEnergy(-ChangeRange(Abs(iTemp) - iLimit, 0, MaxTemp - iLimit, 1, 10));
	}
}


/* Temperatureffekt f�r die Landschaft */

static LandTempEffects;
static const LandTempDist = 100;
static const MaxTemp = 1000;

global func CreateLandTempEffects() {
	LandTempEffects = CreateArray(LandscapeWidth() / LandTempDist);
	for(var x = 0; x * LandTempDist < LandscapeWidth(); x++) {
		LandTempEffects[x] = CreateArray(LandscapeHeight() / LandTempDist);
		for(var y = 0; y * LandTempDist < LandscapeHeight(); y++) {
			LandTempEffects[x][y] = AddEffect("LandTemp", 0, 10, 50, 0, 0, x, y);
		}
	}
}

global func RemoveLandTempEffects() {
	var i = GetEffectCount("LandTemp");
	while(i--)
		RemoveEffect("LandTemp");
}

global func RecreateLandTempEffects() {
	RemoveLandTempEffects();
	CreateLandTempEffects();
}

global func GetLandTempEffect(int x, int y) {
	return LandTempEffects[x][y];
}

global func FxLandTempStart(object pTarget, int iEffectNumber, bool fTemp, int x, int y) {
	if(fTemp)
		return;
	EffectVar(0, 0, iEffectNumber) = x;
	EffectVar(1, 0, iEffectNumber) = y;
	CreateParticle("PSpark", x*LandTempDist, y*LandTempDist, 0, 0, 50, RGB(0, 255));
}

global func FxLandTempTimer(object pTarget, int iEffectNumber) {
	// *= LandTempDist f�r tats�chliche Koordinaten!
	var x = EffectVar(0, 0, iEffectNumber), y = EffectVar(1, 0, iEffectNumber);
	var iX = x * LandTempDist, iY = y * LandTempDist;
	var iTemp = EffectVar(2, 0, iEffectNumber);
	
	// Temperatur der anderen Seiten anpassen
	var iSides, iOther;
	// oben
	if(y) {
		iSides++;
		// etwas weniger, damit es unten k�lter wird
		iOther += EffectVar(2, 0, GetLandTempEffect(x, y-1)); //* 2 / 3;
	}
	// unten
	if(y != LandscapeHeight() / LandTempDist) {
		iSides++;
		iOther += EffectVar(2, 0, GetLandTempEffect(x, y+1));
	}
	// links
	if(x) {
		iSides++;
		iOther += EffectVar(2, 0, GetLandTempEffect(x-1, y));
	}
	// rechts
	if(x != LandscapeWidth() / LandTempDist) {
		iSides++;
		iOther += EffectVar(2, 0, GetLandTempEffect(x+1, y));
	}
	
	// Durchschnitt der Werte berechnen
	iOther /= iSides;
	
	// Erw�rmung der Erde
	if(GBackSolid(iX, iY)) {
		if(!y || (!GBackSolid(iX, iY - LandTempDist) && GBackSky(iX, iY - LandTempDist))) {
			iOther = Min(iOther + GetLightIntensity(), MaxTemp);
		}
	}
	
	// Abk�hlung ganz unten
	if(y == LandscapeHeight() / LandTempDist) {
		iOther = Max(iOther - 75, -MaxTemp);
	}
	
	var k = 1; // Wachstumstgeschwindigkeit * 10
	if(GBackSky(iX, iY))
		k = 10;
	else if(GBackSolid(iX, iY))
		k = 5;
	else if(GBackLiquid(iX, iY))
		k = 2;
	
	// Beschr�nktes Wachstum
	// Bestand + k * (Schranke - Bestand)
	iTemp += k * (iOther - iTemp) / 10;
	
	// neue Temperatur abspeichern
	EffectVar(2, 0, iEffectNumber) = iTemp;
	
	// Toller Partikel!
	var b0 = RGB(0, 0, 255), b1 = RGB(255, 0, 0);
	var i1,i2,b; i2 = (iTemp+MaxTemp)*256/(MaxTemp*2); i1 = 256-i2;

    b =   ((b0&16711935)*i1 + (b1&16711935)*i2)>>8 & 16711935
        | ((b0>>8&16711935)*i1 + (b1>>8&16711935)*i2) & -16711936;
    if (!b && (b0 | b1)) ++b;
	
	CreateParticle("PSpark", iX, iY, 0, 0, 100, b);
}
/*
global func CalcBoundedGrowth(int iValue, int iLimit, int k) {
	return iValue + k * (iLimit - iValue);
}
*/

global func GetLandTemp() {
	var iX = GetX(), iY = GetY(), x, y;
	var iNum, iTemp;
	
	x = Round(iX, LandTempDist, RoundDown) / LandTempDist;
	y = Round(iY, LandTempDist, RoundDown) / LandTempDist;
	
	var iEffectNum;
	if(iEffectNum = GetLandTempEffect(x, y)) {
		iNum++;
		iTemp += EffectVar(2, 0, iEffectNum);
	}
	if(iEffectNum = GetLandTempEffect(x+1, y)) {
		iNum++;
		iTemp += EffectVar(2, 0, iEffectNum);
	}
	if(iEffectNum = GetLandTempEffect(x, y+1)) {
		iNum++;
		iTemp += EffectVar(2, 0, iEffectNum);
	}
	if(iEffectNum = GetLandTempEffect(x+1, y+1)) {
		iNum++;
		iTemp += EffectVar(2, 0, iEffectNum);
	}
	
	iTemp /= iNum;
	
	return iTemp;
}
