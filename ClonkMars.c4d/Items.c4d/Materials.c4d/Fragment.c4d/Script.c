/*-- Fragment --*/

#strict

func Initialize() {
  SetAction("%d",Random(3)+1);
  return(1);
}

protected func ControlDigDouble(){
  Split2Components(this());
  Sound("Connect");
  }

protected func Hit() {
  Sound("MetalHit*");
  return(1);
}
