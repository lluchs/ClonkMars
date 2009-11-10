/*-- Fossae --*/

#strict 2

func Initialize() {
  //typical Mars Sky
  //SetSkyAdjust(RGBa(190,143,65,255),RGB(195,143,59));
  SetGamma(RGB(50,0,0), RGB(140,100,100), RGB(255,220,220));
  DigBuildingsFree();
  return(1);
}
