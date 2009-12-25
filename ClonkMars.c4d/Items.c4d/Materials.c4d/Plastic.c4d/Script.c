/*--- Holz ---*/

#strict

protected func Hit()
{
  Sound("WoodHit*");
  return(1);
}

func Incineration()
{
  SetClrModulation (RGB(48, 32, 32));
}

public func MarsResearch() { return true; }
