/*-- Chasma --*/

#strict

func Initialize() {
  SetSkyAdjust(RGBa(255,255,255,160), RGB(70,30,15));
  SetGamma(RGB(50,0,0), RGB(140,100,100), RGB(255,220,220));
  return(1);
}

protected func InitializePlayer(iPlr){
  //ersten Clonk alles in die Hand geben
  CreateContents(CNKT,GetCrew(iPlr,0),2);
  CreateContents(METL,GetCrew(iPlr,0),2);
  CreateContents(SATD,GetCrew(iPlr,0),1);
  CreateContents(PSTC,GetCrew(iPlr,0),2);
  return(1);
  }

  //Regelw�hler
  func NotChoosableRules() { return [LNGT,ENRG,CNMT]; }
