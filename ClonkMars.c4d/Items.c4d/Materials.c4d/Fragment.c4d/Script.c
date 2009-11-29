/*-- Fragment --*/

#strict

func Initialize() {
  SetAction(Format("g%d",Random(4)+1));
  //SetGraphics (0, 0, 0, 0, GFXOV_MODE_Action, Format("g%d",Random(4)+1));
  return(1);
}

protected func Activate(){
  Split2Components(this());
  Sound("Connect");
  }

protected func Hit() {
  Sound("MetalHit*");
  return(1);
}
