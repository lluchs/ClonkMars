/*-- Phobos --*/

#strict 2


func Initialize() {
  //rötlich wegen GI
  SetGamma(RGB(30,0,0), RGB(120,100,100), RGB(235,220,220));
  DigBuildingsFree();
  // Spielziel setzen
  FindObject(CROB)->AddType(MRAS, 2);
  CreateObject(MARS,-0,0,-1);
  return(1);
}

protected func InitializePlayer(int iPlr) {
	var pCapsule = CreateObject(CPSL, RandomX(100, LandscapeWidth() - 100), 70, iPlr);
	for(var pObj in FindObjects(Find_Owner(iPlr), Find_Or(Find_Category(C4D_Object), Find_OCF(OCF_CrewMember)))) {
		pObj -> Enter(pCapsule);
	}
}

//Regelwähler
func NotChoosableRules() { return [ENRG,CNMT]; }
