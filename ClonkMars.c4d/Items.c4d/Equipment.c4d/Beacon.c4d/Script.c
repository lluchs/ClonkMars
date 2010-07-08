/*-- Specklicht --*/

#strict 2

func Initialize() {
  SetAction("Idle");
}

protected func Activate(pClonk){
  var pBeacon = CreateObject(BECP,0,Min(11, LandscapeHeight()-GetY()-1),GetOwner(pClonk) );
  RemoveObject(this());
  Sound("Connect");
  }

protected func Hit()
{
  Sound("RockHit*");
	return(1);
}
