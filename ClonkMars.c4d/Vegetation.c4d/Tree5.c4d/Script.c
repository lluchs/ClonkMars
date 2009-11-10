/*-- Baum --*/

#strict

#include TREE

protected func Seed(){
  //checkt ob es in der Luft hängt
  if(GetMaterial(0,10)!=Material("Earth"))
  DoCon(-10,this());
  return(1);
  }
