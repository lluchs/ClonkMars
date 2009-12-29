#strict 2

static iWindEffectedCount; //Used for optimisation of that array.
static aWindEffectedObjects;
static iGlobalWind; //Wind speed
static aWindForecast; //Two ints: Frame, speed to set at that frame
//static iGlobalHorizon;
static iLastGlobalHorizonCheck;
static iWindProfilingObjects;
static aBorderHeights;
static pWindControl;

//2do: Intelligent SRMP-Placing
//Think about: 
//	Don't ignore the global horizon, go down in wind speed a bit when coming closer.
//	Check GetScenLeft/RightOpen() in Initialize

global func GetWind(int iX, int iY, bool fGlobal) {
	iX += GetX(); iY += GetY(); //Make stuff global
	if(fGlobal) return iGlobalWind;
	var iAng, iDst = (((iGlobalWind < 0) && LandscapeWidth()) - iX);
	//Note on && hack: if(iGlobalWind < 0) {second &&-condition is not to be checked and thus not returned} else {second condition is true, so it is returned}
	for(iAng=1;iAng<45; iAng+=15) { //Itering in 15°, subtest once with -8; If you change, take care that max is at 45°
		if(PathFree(iX, iY, (iGlobalWind < 0) && LandscapeWidth(), Sin(iAng,iDst)*iDst/Cos(iAng,iDst))) {
			if(iAng > 1 && PathFree(iX, iY, (iGlobalWind < 0) && LandscapeWidth(), Sin(iAng-8,iDst)*iDst/Cos(iAng-8,iDst)))
				iAng -= 8;
			break;
		}
	}
	return iGlobalWind * (45-iAng) / 45 * (Random(51) + 975) / 1000; //* (((GetHorizon()>iY) && (GetHorizon()-iY)) || 200) / 200
}

global func SetWind(int iWind, int iHold) {
	iHold += FrameCounter();
	if(iWind > 250) iWind = 250;
	else if(iWind < -250) iWind = -250;
	var i;
	while(i<GetLength() && aWindForecast[i][0] < iHold) ++i;
	PopElements(aWindForecast, 1, i-1);
	aWindForecast[0] = [FrameCounter(), iWind];
}

global func IsStormy() {
	return Abs(iGlobalWind) > 100;
}

protected func Initialize() {
	if(pWindControl) return RemoveObject();
	pWindControl = this;
	aWindForecast = [[0, Random(101)-50]]; //Moderate at the beginning
	aWindEffectedObjects = [];
	aBorderHeights = [];
	if(!GetEffect("IntWindExec")) AddEffect("IntWindExec", 0, 1, 3, 0, STRM);
	var i, posy;
	for(var posx in [0,LandscapeWidth()]){
		posy=0;
		while(!GBackSolid(posx,posy)) posy++;
		aBorderHeights[i] = posy;
		i++;
	}
}

global func FxIntWindExecTimer() {
		// Update Forecast
		if(aWindForecast[GetLength(aWindForecast)-1][0] - FrameCounter() < 10000)
			var frm = aWindForecast[GetLength(aWindForecast)-1][0]+Random(101)+Random(101); //Hack to avoid funny engine bug
			var dir = BoundBy(aWindForecast[GetLength(aWindForecast)-1][1]+Random(121)-60, -250, 250); //If you put it directly into the array, it fails.
			aWindForecast[GetLength(aWindForecast)] = [frm,dir];
		// Calc speed
		if(aWindForecast[0][0] < FrameCounter()) {
			if(aWindForecast[0][1] != iGlobalWind) {
				if(iGlobalWind < aWindForecast[0][1]) iGlobalWind += 2;
				else iGlobalWind -= 2;
				if(Abs(aWindForecast[0][1] - iGlobalWind) < 3) iGlobalWind = aWindForecast[0][1]; //Correctness jump, I don't want it to hang
			} else {
				PopElements(aWindForecast, 0);
			}
		}
		// Effect Objects
		var i = iWindEffectedCount;
		while(i--) {
			if(!aWindEffectedObjects[i]) {
				aWindEffectedObjects[i] == aWindEffectedObjects[--iWindEffectedCount]; 
				if(GetLength(aWindEffectedObjects) - iWindEffectedCount > 20) SetLength(aWindEffectedObjects,iWindEffectedCount+15);
				continue;
			}
			Log("%v, %v, %v, %v, %v, %v", 
				(GetXDir(aWindEffectedObjects[i],10)-iGlobalWind>0)-(GetXDir(aWindEffectedObjects[i],10)-iGlobalWind<0), 
				iGlobalWind, 
				GetXDir(aWindEffectedObjects[i],100), 
				(GetXDir(aWindEffectedObjects[i],100)-iGlobalWind*10)**2, 
				aWindEffectedObjects[i]->WindEffect(), 
				GetXDir(aWindEffectedObjects[i],100));
			SetXDir((GetXDir(aWindEffectedObjects[i],10)-iGlobalWind>0)-(GetXDir(aWindEffectedObjects[i],10)-iGlobalWind<0) * //Vorzeichen
			        (GetXDir(aWindEffectedObjects[i],100)-iGlobalWind*10)**2*
			        aWindEffectedObjects[i]->WindEffect()/1000+
			        GetXDir(aWindEffectedObjects[i],100),aWindEffectedObjects[i],200);
		}
		// Effect Landscape
		while(iWindProfilingObjects < (Max(Abs(iGlobalWind)-30, 0)**2 / 250)) {
			CreateObject(SRMP, Random(LandscapeWidth()), 1, NO_OWNER)->SetYDir(100);
		}
}

global func AddObjectForWind(object pObj) {
	if(!pWindControl) return;
	if(!pObj) if(!(pObj=this)) return;
	if(GetLength(aWindEffectedObjects) >= iWindEffectedCount) SetLength(aWindEffectedObjects,iWindEffectedCount+15);
	aWindEffectedObjects[iWindEffectedCount++] = pObj;
}

/*global func GetHorizon() {
	if(FrameCounter() - iLastGlobalHorizonCheck < 30) return iGlobalHorizon; //Assuming it won't be that obvious if the effect is delayed for a second.
	while(PathFree(0, iGlobalHorizon, GetLandscapeWidth(), iGlobalHorizon)) iGlobalHorizon += 5;
	while(!PathFree(0, iGlobalHorizon, GetLandscapeWidth(), iGlobalHorizon)) iGlobalHorizon -= 5;
	iLastGlobalHorizonCheck = FrameCounter();
	return iGlobalHorizon;
}*/