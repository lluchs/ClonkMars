/* Aktion mit Animation als Overlay setzen */

#strict 2

global func OverlayShiftX(int iWidth) {
	return 1000*(GetDefCoreVal("Offset", "DefCore", GetID(), 0) + iWidth/2);
}

global func OverlayShiftY(int iHeight) {
	return 1000*(GetDefCoreVal("Offset", "DefCore", GetID(), 0) + iHeight/2);
}

/* Methode 1: alle Einzelframes in der ActMap.txt */

global func OverlayActionInit(string szAction, id ID) {
	if(!ID && !(ID = GetID()))
		return;
	var iLength = GetActMapVal("Length", szAction, ID);
	if(!iLength)
		return;
	var iFacetX = GetActMapVal("Facet", szAction, ID, 0),
	    iFacetY = GetActMapVal("Facet", szAction, ID, 1),
	    iFacetWidth = GetActMapVal("Facet", szAction, ID, 2),
	    iFacetHeight = GetActMapVal("Facet", szAction, ID, 3),
	    iFacetPX = GetActMapVal("Facet", szAction, ID, 4),
	    iFacetPY = GetActMapVal("Facet", szAction, ID, 5);
	for(var i = 0; i < iLength; i++) {
		Log("[Action]");
		Log("Name=%s%d", szAction, i);
		Log("Length=1");
		Log("Facet=%d,%d,%d,%d,%d,%d", iFacetX + iFacetWidth * i, iFacetY, iFacetWidth, iFacetHeight, iFacetPX, iFacetPY);
	}
	return iLength;
}

// Grafik einer nicht animierten Aktion setzen
global func SetStillOverlayAction(string szAction, int iOverlay) {
	if(!iOverlay)
		iOverlay = GetUnusedOverlayID(1);
	SetGraphics(0, this, GetID(), iOverlay, GFXOV_MODE_Action, szAction);
	if(GetDir() == DIR_Left)
		SetObjDrawTransform(1000, 0, 1000*(GetDefCoreVal("Offset", "DefCore", GetID(), 0) + GetActMapVal("Facet", szAction, GetID(), 2)/2 + GetActMapVal("Facet", szAction, GetID(), 4)), 0, 1000, 1000*(GetDefCoreVal("Offset", "DefCore", GetID(), 1) + GetActMapVal("Facet", szAction, GetID(), 3)/2 + GetActMapVal("Facet", szAction, GetID(), 5)), this, iOverlay);
	else if(GetDir() == DIR_Right) {
		SetObjDrawTransform(-1000, 0, 1000*(-GetDefCoreVal("Offset", "DefCore", GetID(), 0) - GetActMapVal("Facet", szAction, GetID(), 2)/2 - GetActMapVal("Facet", szAction, GetID(), 4)), 0, 1000, 1000*(GetDefCoreVal("Offset", "DefCore", GetID(), 1) + GetActMapVal("Facet", szAction, GetID(), 3)/2 + GetActMapVal("Facet", szAction, GetID(), 5)), this, iOverlay);
	}
}

global func SetOverlayAction(string szAction, int iOverlay, bool fReverse, bool fNoCalls, string szPhaseCall, string szEndCall) {
	if(!iOverlay)
		iOverlay = GetUnusedOverlayID(1);
	AddEffect("IntOverlayAction", this, 100, GetActMapVal("Delay", szAction, GetID()), this, 0, szAction, iOverlay, fReverse, [fNoCalls, szPhaseCall, szEndCall]);
	return iOverlay;
}

global func FxIntOverlayActionStart(object pTarget, int iEffectNumber, int iTemp, string szAction, int iOverlay, bool fReverse, array aCalls) {
	if(iTemp)
		return;
	var iLength = GetActMapVal("Length", szAction, GetID());
	EffectVar(0, pTarget, iEffectNumber) = szAction;
	EffectVar(1, pTarget, iEffectNumber) = iOverlay;
	EffectVar(2, pTarget, iEffectNumber) = iLength;
	// 3: i
	if(fReverse) {
		EffectVar(3, pTarget, iEffectNumber) = iLength - 1;
		EffectVar(4, pTarget, iEffectNumber) = true;
	}
	if(!aCalls[0]) {
		var szStartCall = GetActMapVal("StartCall", szAction, GetID()),
			  szPhaseCall = GetActMapVal("PhaseCall", szAction, GetID()),
			  szEndCall = GetActMapVal("EndCall", szAction, GetID());
		if(szStartCall)
			Call(szStartCall);
		EffectVar(5, pTarget, iEffectNumber) = szPhaseCall;
		EffectVar(6, pTarget, iEffectNumber) = szEndCall;
	}
	else {
		EffectVar(5, pTarget, iEffectNumber) = aCalls[1];
		EffectVar(6, pTarget, iEffectNumber) = aCalls[2];
	}
}

global func FxIntOverlayActionTimer(object pTarget, int iEffectNumber) {
	var szAction = EffectVar(0, pTarget, iEffectNumber),
	    iLength = EffectVar(2, pTarget, iEffectNumber),
	    i = EffectVar(3, pTarget, iEffectNumber),
	    fReverse = EffectVar(4, pTarget, iEffectNumber),
	    iOverlay = EffectVar(1, pTarget, iEffectNumber),
	    szPhaseCall = EffectVar(5, pTarget, iEffectNumber);
	SetGraphics(0, this, GetID(), iOverlay, GFXOV_MODE_Action, Format("%s%d", szAction, i));
	if(GetDir() == DIR_Left)
		SetObjDrawTransform(1000, 0, 1000*(GetDefCoreVal("Offset", "DefCore", GetID(), 0) + GetActMapVal("Facet", Format("%s%d", szAction, i), GetID(), 2)/2 + GetActMapVal("Facet", Format("%s%d", szAction, i), GetID(), 4)), 0, 1000, 1000*(GetDefCoreVal("Offset", "DefCore", GetID(), 1) + GetActMapVal("Facet", Format("%s%d", szAction, i), GetID(), 3)/2 + GetActMapVal("Facet", Format("%s%d", szAction, i), GetID(), 5)), this, iOverlay);
	else if(GetDir() == DIR_Right) {
		SetObjDrawTransform(-1000, 0, 1000*(-GetDefCoreVal("Offset", "DefCore", GetID(), 0) - GetActMapVal("Facet", Format("%s%d", szAction, i), GetID(), 2)/2 - GetActMapVal("Facet", Format("%s%d", szAction, i), GetID(), 4)), 0, 1000, 1000*(GetDefCoreVal("Offset", "DefCore", GetID(), 1) + GetActMapVal("Facet", Format("%s%d", szAction, i), GetID(), 3)/2 + GetActMapVal("Facet", Format("%s%d", szAction, i), GetID(), 5)), this, iOverlay);
	}
	if(szPhaseCall)
		Call(szPhaseCall, szAction, iOverlay, iLength, i, fReverse);
	if(fReverse) {
		i--;
		if(i < 0)
			return -1;
	}
	else {
		i++;
		if(iLength == i)
			return -1;
	}
	EffectVar(3, pTarget, iEffectNumber) = i;
}

global func FxIntOverlayActionStop(object pTarget, int iEffectNumber) {
	var iOverlay = EffectVar(1, pTarget, iEffectNumber),
	    szEndCall = EffectVar(6, pTarget, iEffectNumber);
	if(szEndCall)
		Call(szEndCall, EffectVar(0, pTarget, iEffectNumber), EffectVar(1, pTarget, iEffectNumber));
}
