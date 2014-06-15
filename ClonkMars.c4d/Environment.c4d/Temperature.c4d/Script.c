/*-- Temperatur --*/

#strict 2

protected func Initialize() {
	InitializeLandTempPoints();
	
	for(var pClonk in FindObjects(Find_OCF(OCF_Alive), Find_Func("IsClonk")))
		OnClonkRecruitment(pClonk);
}

protected func OnClonkRecruitment(object pClonk) {
	AddEffect("Temperature", pClonk, 10, 25, 0, GetID());
	AddEffect("LandTempHUDUpdate", pClonk, 1, 10, 0, GetID());
}

/* Temperatureffekt für den Clonk */

/* EffectVars
 * 
 * 0: Temperatur (von -MaxTemp bis MaxTemp)
 */

protected func FxTemperatureTimer(object pTarget, int iEffectNumber) {
	var iTemp = EffectVar(0, pTarget, iEffectNumber);
	
	var k = 7; // Wachstumsfaktor
	
	var iOuterTemp = pTarget -> GetLandTemp();
	
	if(pTarget -> Contained()) {
		var new = pTarget -> Contained() -> ~GetTemp();
		if(new) {
			iOuterTemp = new;
			if(pTarget -> Contained() -> ~FastTempChange())
				k = 10;
		}
	}
	// ist in heißem Material?
	else if(GetMaterialVal("Incindiary", "Material", pTarget -> GetMaterial()))
		iOuterTemp = MaxTemp;
	
	// schnelle Änderung in Flüssigkeiten
	if(pTarget -> GBackLiquid())
		k = 50;
	
	// Schwerer Anzug => kaum Temperaturänderung
	if(pTarget -> ~HeavySuit())
		k = 2;
	
	var diff = iOuterTemp - iTemp;
	// Beschränktes Wachstum
	// Bestand + k * (Schranke - Bestand)
	var iChange =  k * (diff) / 100;
	// Änderung minimal 1
	if(!iChange && diff != 0) {
		if(diff < 0)
			iChange = -1;
		else
			iChange = 1;
	}
	// außer bei schwerem Anzug
	if(Abs(iChange) == 1 && pTarget -> ~HeavySuit()) {
		iChange = 0;
	}
	
	iTemp += iChange;
	
	// speichern
	EffectVar(0, pTarget, iEffectNumber) = iTemp;
	
	// Energieverlust bei zu niedrigen/hohen Werten
	
	var iLimit = MaxTemp * 3 / 4;
	if(iTemp < -iLimit) {
		// max. 10 Energieverlust
		pTarget -> DoEnergy(-ChangeRange(Abs(iTemp) - iLimit, 0, MaxTemp - iLimit, 1, 10));
	}
	
	if(iTemp > iLimit) {
		// max. 15 Energieverlust
		pTarget -> DoEnergy(-ChangeRange(Abs(iTemp) - iLimit, 0, MaxTemp - iLimit, 1, 15));
		// wir brennen!
		//pTarget -> AddFireEffect(GetEffect("FireEffect", pTarget, 0, 6) + 25, 0, true);
		Schedule("CreateParticle(\"Fire2\", -6 + PRandom(12), -5 + PRandom(15), GetWind() / 2, -PRandomX(10, 20), PRandomX(20, 50), 0, this)", 1, 25, pTarget);
	}
	
	pTarget -> UpdateHUDValue(HUD_ClonkTemp, iTemp);
}

protected func FxLandTempHUDUpdateTimer(object pTarget, int iEffectNumber) {
	var iLandTemp = pTarget -> GetLandTemp();
	pTarget -> UpdateHUDValue(HUD_LandTemp, iLandTemp);
}


/* Temperatureffekt für die Landschaft */

static LandTempPoints, LandTempDebug;
static const LandTempDist = 100;
static const MaxTemp = 1000;

// Toggles the pretty debug particles.
global func ToggleLandTempDebug() {
	return LandTempDebug = !LandTempDebug;
}

// Initializes the land temperature points, overriding previous ones.
global func InitializeLandTempPoints() {
	LandTempPoints = CreateArray(LandscapeWidth() / LandTempDist);
	// x-1 bzw. y-1, damit auch am Spielfeldrand ein Effekt vorhanden ist
	// dieser ist natürlich außerhalb der Landschaft
	for(var x = 0; (x-1) * LandTempDist < LandscapeWidth(); x++) {
		LandTempPoints[x] = CreateArray(LandscapeHeight() / LandTempDist);
		for(var y = 0; (y-1) * LandTempDist < LandscapeHeight(); y++) {
			LandTempPoints[x][y] = InitLandTempPoint(x, y);
		}
	}
}

global func GetLandTempPoint(int x, int y) {
	return LandTempPoints[x][y];
}

global func GetRandomLandTempPoint() {
	var rx = Random(GetLength(LandTempPoints));
	var ry = Random(GetLength(LandTempPoints[rx]));
	return LandTempPoints[rx][ry];
}

// Initializes a land temp point with the given coordinates.
global func InitLandTempPoint(int x, int y) {
	// Starttemperatur bei heißem Material
	var temp = 0;
	if(GetMaterialVal("Incindiary", "Material", GetMaterial(x*LandTempDist, y*LandTempDist))) {
		temp = MaxTemp / 2;
	}
	
	if(LandTempDebug)
		CreateParticle("PSpark", x*LandTempDist, y*LandTempDist, 0, 0, 50, RGB(0, 255));

	return [x, y, temp];
}

// Returns the number of points to process per tick.
private func PointsPerTick() {
	return GetLength(LandTempPoints) * 2;
}

// Timer call in the Temperature object.
// Selectes PointsPerTick() random land temp points to tick.
protected func Timer() {
	var i = PointsPerTick(), ltp;
	while (i--) {
		ltp = GetRandomLandTempPoint();
		LandTempPoints[ltp[0]][ltp[1]][2] = LandTempTick(ltp);
	}
}

// Performs a timer tick for the given land temp point, returning the new temperature.
global func LandTempTick(array ltp) {
	// *= LandTempDist für tatsächliche Koordinaten!
	var x = ltp[0], y = ltp[1];
	var iX = x * LandTempDist, iY = y * LandTempDist;
	var iTemp = ltp[2];
	
	// Temperatur der anderen Seiten anpassen
	var iSides, iOther, k;
	// oben
	if(y) {
		k =  10 - GetLandTempChangeSpeed(iX, iY - LandTempDist);
		iSides += k;
		iOther += k * GetLandTempPoint(x, y-1)[2];
	}
	// unten
	if((y+1) * LandTempDist < LandscapeHeight()) {
		k =  10 - GetLandTempChangeSpeed(iX, iY + LandTempDist);
		iSides += k;
		iOther += k * GetLandTempPoint(x, y+1)[2];
	}
	// links
	if(x) {
		k =  10 - GetLandTempChangeSpeed(iX - LandTempDist, iY);
		iSides += k;
		iOther += k * GetLandTempPoint(x-1, y)[2];
	}
	// rechts
	if((x+1) * LandTempDist < LandscapeWidth()) {
		k =  10 - GetLandTempChangeSpeed(iX + LandTempDist, iY);
		iSides += k;
		iOther += k * GetLandTempPoint(x+1, y)[2];
	}
	
	// Durchschnitt der Werte berechnen
	iOther /= iSides;
	
	// Erwärmung der Erde
	if(GBackSolid(iX, iY)) {
		if(!y || (!GBackSolid(iX, iY - LandTempDist) && GBackSky(iX, iY - LandTempDist))) {
			var earthWarming = GameCall("EarthWarming");
			if(!earthWarming)
				earthWarming = GetLightIntensity() * 7 / 4;
			iOther = Min(iOther + earthWarming, GetRegulatedMaxTemp());
		}
	}
	
	// Erwärmung bei heißem Material
	if(GetMaterialVal("Incindiary", "Material", GetMaterial(iX, iY))) {
		iOther = Min(iOther + 200, MaxTemp);
	}
	
	// Abkühlung ganz unten
	if(y == LandscapeHeight() / LandTempDist) {
		var lowerCooling = GameCall("LowerCooling");
		if(!lowerCooling)
			lowerCooling = 150;
		iOther = Max(iOther - lowerCooling, GetRegulatedMinTemp());
	}
	
	// Abkühlung ganz oben, abhängig von Tageszeit
	if(!y) {
		var upperCooling = GameCall("UpperCooling");
		if(!upperCooling)
			upperCooling = 110 - GetLightIntensity() / 3;
		iOther = Max(iOther - upperCooling, GetRegulatedMinTemp());
	}
	
	var k = GetLandTempChangeSpeed(iX, iY); // Wachstumstgeschwindigkeit * 10
	
	// Beschränktes Wachstum
	// Bestand + k * (Schranke - Bestand)
	iTemp += k * (iOther - iTemp) / 10;
	
	// Toller Partikel!
	if(LandTempDebug) {
		var b0 = RGB(0, 0, 255), b1 = RGB(255, 0, 0);
		var i1,i2,b; i2 = (iTemp+MaxTemp)*256/(MaxTemp*2); i1 = 256-i2;
	
	    b =   ((b0&16711935)*i1 + (b1&16711935)*i2)>>8 & 16711935
	        | ((b0>>8&16711935)*i1 + (b1>>8&16711935)*i2) & -16711936;
	    if (!b && (b0 | b1)) ++b;
		
		CreateParticle("PSpark", iX, iY, 0, 0, 100, b);
	}

	return iTemp;
}

// *cough*
global func GetLandTempChangeSpeed(int iX, int iY) {
	var k = 1; // Wachstumstgeschwindigkeit * 10
	if(GBackSky(iX, iY))
		k = 9;
	else if(GBackSolid(iX, iY))
		k = 5;
	else if(GBackLiquid(iX, iY))
		k = 2;
	
	return k;
}

global func GetRegulatedMaxTemp() {
	return GameCall("RegulatedMaxTemp") || MaxTemp;
}

global func GetRegulatedMinTemp() {
	return GameCall("RegulatedMinTemp") || -MaxTemp;
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
	
	var ltp;
	if(ltp = GetLandTempPoint(x, y)) {
		iNum++;
		iTemp += ltp[2];
	}
	if(ltp = GetLandTempPoint(x+1, y)) {
		iNum++;
		iTemp += ltp[2];
	}
	if(ltp = GetLandTempPoint(x, y+1)) {
		iNum++;
		iTemp += ltp[2];
	}
	if(ltp = GetLandTempPoint(x+1, y+1)) {
		iNum++;
		iTemp += ltp[2];
	}
	
	iTemp /= iNum;
	
	return iTemp;
}

// Changes the land temperatur in the caller's position.
global func DoLandTemp(int iChange, int k) {
	var iX = GetX(), iY = GetY(), x, y;
	
	x = Round(iX, LandTempDist, RoundDown) / LandTempDist;
	y = Round(iY, LandTempDist, RoundDown) / LandTempDist;
	
	var ltp;
	if(ltp = GetLandTempPoint(x, y)) {
		_DoLandTemp(ltp, iChange, k);
	}
	if(ltp = GetLandTempPoint(x+1, y)) {
		_DoLandTemp(ltp, iChange, k);
	}
	if(ltp = GetLandTempPoint(x, y+1)) {
		_DoLandTemp(ltp, iChange, k);
	}
	if(ltp = GetLandTempPoint(x+1, y+1)) {
		_DoLandTemp(ltp, iChange, k);
	}
	
	return 1;
}

global func _DoLandTemp(array ltp, int iChange, int k) {
	if(!k)
		k = GetLandTempChangeSpeed(LandTempDist * ltp[0], LandTempDist * ltp[1]);
	var temp = ltp[2];
	temp += k * iChange / 10;
	temp = BoundBy(temp, -MaxTemp, MaxTemp);
	LandTempPoints[ltp[0]][ltp[1]][2] = temp;
}
