/*-- Eos --*/

#strict 2

func Initialize() {
  SetSkyAdjust(RGBa(200,94,51,90),RGB(200,110,55));
  SetGamma(RGB(50,0,0), RGB(140,100,100), RGB(255,220,220));
  //Hintergrund
  CreateObject(BG_2,-0,0,-1);
  return(1);
  }

protected func InitializePlayer(int iPlr) {
	var pCapsule = CreateObject(CPSL, RandomX(100, LandscapeWidth() - 100), 70, iPlr);
	for(var pObj in FindObjects(Find_Owner(iPlr), Find_Or(Find_Category(C4D_Object), Find_OCF(OCF_CrewMember)))) {
		pObj -> Enter(pCapsule);
	}
	pCapsule->SetDstPort(0, true);
}  
  
  //Regelwähler
func NotChoosableRules() { return [ENRG,CNMT]; }

func RegulatedMaxTemp() {
	return 425;
}
