/*-- Abyss --*/

#strict 2

func Initialize() {
  //typical Mars Sky
  SetSkyAdjust(RGBa(190,143,65,255),RGB(195,143,59));
  SetGamma(RGB(50,0,0), RGB(140,100,100), RGB(255,220,220));
  SetSkyParallax(0,10,0,3,0);
  var Rain=CreateObject(FXP1,0,0,-1);
  Rain->Activate(Material("DuroLava"),960,50);
  ScriptGo(1);
  CreateObject(BG_3, LandscapeWidth()/2, 200);
  return(1);
}

/*protected func InitializePlayer(int iPlr) {
	var pCapsule = CreateObject(CPSL, RandomX(100, LandscapeWidth() - 100), 70, iPlr);
	for(var pObj in FindObjects(Find_Owner(iPlr), Find_Or(Find_Category(C4D_Object), Find_OCF(OCF_CrewMember)))) {
		pObj -> Enter(pCapsule);
	}
	pCapsule->SetDstPort(0, true);
}*/

protected func Script5(){
  //place Dust
  for(var i;i<40;++i) 
  CreateParticle("Dust",Random(LandscapeWidth()),Random(LandscapeHeight()*2/3),RandomX(3,9),0,RandomX(1000,1500));
  return(goto(0));
  }
  
//Regelwähler
func NotChoosableRules() { return [LNGT,LDAY,ENRG,CNMT]; }

func LowerCooling() { return 75; }

func RegulatedMaxTemp() {
	return 500;
}
