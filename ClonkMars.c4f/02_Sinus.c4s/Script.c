/*-- Terra Sinus --*/

#strict 2

func Initialize() {
  // Spielziele setzen
  FindObject(CROB)->AddType(BASE, 1);
  FindObject(CROB)->AddType(PORT, 1);
  FindObject(CROB)->AddType(RADO, 3);
  FindObject(CROB)->SetMissionAccess("3_Abyss");

  //Ölquelle
  CreateObject(OILS,260,490,-1);
  
  //SetSkyAdjust(RGBa(255,255,255,127), RGB(127,127,127));
  SetSkyAdjust(RGBa(190,143,65,0),RGB(195,143,59));
  SetGamma(RGB(50,0,0), RGB(140,100,100), RGB(255,220,220));
   for(var i;i<40;++i) 
  CreateParticle("Dust2",Random(LandscapeWidth()),Random(LandscapeHeight()*2/3),RandomX(3,9),0,RandomX(1000,1500));
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
	return 299;
}
