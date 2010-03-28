/*-- Tachyon --*/

#strict 2

func Initialize() {
  SetSkyAdjust(RGBa(117,103,116,127), RGB(244,181,184));
  SetGamma(RGB(50,0,0), RGB(140,100,100), RGB(255,220,220));
  DigBuildingsFree();
  return(1);
}

protected func InitializePlayer(int iPlr) {
	var pCapsule = CreateObject(CPSL, RandomX(100, LandscapeWidth() - 100), 70, iPlr);
	for(var pObj in FindObjects(Find_Owner(iPlr), Find_Or(Find_Category(C4D_Object), Find_OCF(OCF_CrewMember)))) {
		pObj -> Enter(pCapsule);
	}
}
