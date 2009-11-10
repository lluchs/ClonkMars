/*-- Pipelinekit --*/

#strict 2

protected func Hit()
{
  Sound("RockHit*");
}

protected func Activate(clonk)
{
  [$TxtConnectline$]
  // 
  var obj = FindObject2(Find_AtPoint(),Find_Func("PipelineConnect"));
  // 
  if(!obj) return Message("$TxtNoNewLine$", clonk);
  // 
  //var line = FindObject2(Find_Action("Connect"),Find_ActionTarget(this));
  var line = FindObject(0, 0,0,0,0, 0, "Connect", this() );
  
  SetComDir(COMD_Stop, clonk);
  if(line) 
  {
  	if(obj == line->GetActionTarget(0) || obj == line->GetActionTarget(1))
  	{
  		line -> RemoveObject();
  		Sound("Connect");
  		return Message("$TxtLineRemoval$", clonk);
  	} 
  	else
  	{
  		line -> SetAction("Connect",obj,line->GetActionTarget(1));
  		Sound("Connect");
			Message("$TxtConnect$", clonk, obj->GetName());
  		return RemoveObject();
  	}
  }
  else
  {
  	line = CreateObject(DPIP,0,0,NO_OWNER);
  	line -> SetAction("Connect",this,obj);
  	Sound("Connect");
  	return Message("$TxtConnect$", clonk, obj->GetName());
  }
  return true;
}

public func IsLineKit() { return true; }
