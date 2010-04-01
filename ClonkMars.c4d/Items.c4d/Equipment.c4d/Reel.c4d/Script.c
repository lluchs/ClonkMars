/*-- Wire Roll --*/

#strict 2

#include L_LK

public func ConnectionCondition() { return Find_Func("CanPowerConnect"); }
public func LineID() { return PWRL; }
public func MarsResearch() { return true; }

protected func Hit() {
  Sound("MetalHit*");
  return(1);
}
