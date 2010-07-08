/*-- Specklicht --*/

#strict 2
#include DACT //Damagecontrol

protected func Initialize() {
  SetAction("Idletime");
}

  
protected func ControlThrow() {
  if(GetAction()=="Idletime")
    {
    SetAction("On");
    }
      else {
        SetAction("Idletime");
        }
  }    
  
protected func ControlDigDouble(pClonk){
  RemoveObject(this());
  CreateContents(BEAC,pClonk,1);
  Sound("Connect");
  }  

public func MaxDamage() { return 4; } //Maximaler Schaden

protected func Hit()
{
  Sound("RockHit*");
	return(1);
}
