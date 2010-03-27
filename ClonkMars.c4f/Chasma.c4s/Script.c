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
  PlaceObjects(DBSE,1,"Durolava",950,1200,150,150);
  PlaceObjects(DLAB,1,"Durolava",950,1200,150,150);
  PlaceObjects(BONE,9,"Durolava",950,1200,150,150);
  PlaceObjects(SKUL,3,"Durolava",950,1200,150,150);    
  PlaceObjects(DSOL,3,"Durolava",950,1200,150,150);   
  PlaceObjects(METL,2,"Durolava",950,1200,150,150);   
  PlaceObjects(CELL,1,"Durolava",950,1200,150,150);   
  PlaceObjects(SATD,1,"Durolava",950,1200,150,150);
  PlaceObjects(ORE1,8,"Durolava",950,1200,150,150);    
  
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
func NotChoosableRules() { return [LNGT,ENRG,CNMT,RSNB]; }

// 1px-Wand ignorieren
func RightClosed() { return true; }
  
func Script8() {
  for(var i;i<1;i++) PlaceInMaterial(_BUB,Material("DuroLava"));
  goto(1);
  }
