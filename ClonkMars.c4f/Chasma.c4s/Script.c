/*-- Chasma --*/

#strict

func Initialize() {
  SetSkyAdjust(RGBa(255,255,255,160), RGB(70,30,15));
  SetGamma(RGB(50,0,0), RGB(140,100,100), RGB(255,220,220));
  //Hintergrund
  CreateObject(CANN,-0,0,-1);
  CreateObject(DUSK,0,0,-1);
  ScriptGo(1);
  
  //Forschungsstation
  //Koordinaten  
  //1135,1106
  //1025,1299
  //1246,1318
  //996,1305
  
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
  
//Regelwähler
func NotChoosableRules() { return [LNGT,ENRG,CNMT]; }
  
func Script8() {
  for(var i;i<1;i++) PlaceInMaterial(_BUB,Material("DuroLava"));
  goto(1);
  }
