/*-- Crystal Mountain --*/

#strict

protected func Initialize(){
  SetSkyAdjust(RGBa(255,255,255,220), RGB(50,30,50));
  SetGamma(RGB(0,0,50), RGB(100,100,140), RGB(220,220,255));
  return(1);
  }
  
protected func InitializePlayer(int iPlr) {
	var pCapsule = CreateObject(CPSL, RandomX(100, LandscapeWidth() - 100), 70, iPlr);
	for(var pObj in FindObjects(Find_Owner(iPlr), Find_Or(Find_Category(C4D_Object), Find_OCF(OCF_CrewMember)))) {
		pObj -> Enter(pCapsule);
	}
	pCapsule->SetDstPort(0, true);
}
