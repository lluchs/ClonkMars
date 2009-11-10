/*-- Platform --*/

#strict 2
#include B_60
#include DACT //Damagecontrol

local pCaps;

public func MaxDamage() { return 35; } //Maximaler Schaden
protected func Initialize(){SetAction("Wait");}

public func PortActive(){
  //ausgef�hrt sobald Kapsel kommt
  if(GetAction() != "Active") SetAction("Active");
  }
  
public func PortWait(){
  if(GetAction() != "Wait") SetAction("Wait");
  }
  
public func Occupy(object pOcc){
  if(pOcc && pCaps) Log("Internal Error: Capsule requesting occupation for occupated Port.");
  pCaps = pOcc;
  }
  
public func IsFree(){
	if(pCaps) return false;
	return true;
}
