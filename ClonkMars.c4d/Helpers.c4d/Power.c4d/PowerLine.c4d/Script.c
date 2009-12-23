/*-- Power line --*/

#strict 2

protected func Initialize()
{
  SetAction("Connect");  
  SetVertexXY(0, GetX(), GetY());
  SetVertexXY(1, GetX(), GetY());
  SetPosition(0, 0);
}

public func IsPowerLine()
{
	return GetAction() == "Connect";
}

public func IsConnectedWith(object pObject)
{
	return GetActionTarget(0) == pObject || GetActionTarget(1) == pObject;
}

public func LineKitID() { return WIRO; }

public func GetConnectedObject(object pObject)
{
	if(GetActionTarget(0) == pObject)
		return GetActionTarget(1);
	else if(GetActionTarget(1) == pObject) 
		return GetActionTarget(0);		
	else
		return false;
}


protected func LineBreak(bool fNoMsg)
{
  Sound("LineBreak");
  if(!fNoMsg) BreakMessage();
}
  
private func BreakMessage()
{
  
  var line = GetActionTarget(0);
  if(GetID(line) != LineKitID()) line = GetActionTarget(1);

  Message("$TxtLinebroke$", line);
}
