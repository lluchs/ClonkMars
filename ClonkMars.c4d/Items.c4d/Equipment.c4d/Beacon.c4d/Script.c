/*-- Specklicht --*/

#strict 2
#include L_RC // Recycling

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

public func MarsResearch() { return true; }
