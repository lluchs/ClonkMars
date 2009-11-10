/*-- Include for power generators --*/

#strict 2

// Local variable to keep track of the power level inside the generator.
local iPower;

/*-- Public calls --*/
// Functions that specify object properties, should be overloaded by the object.

// Maximum power that can be stored in this object.
public func GetCapacity() 
{
	return 0;
}

// Returns whether the object is a power generator.
public func IsPowerGenerator()
{
	return true;
}

// Returns the generator's priority, consumers preferably drain from generators with the highest priority.
public func GetGeneratorPriority()
{
	return 1;
}

// Returns whether the objects is a power genarator connected to pConsumer.
public func IsPowerGeneratorFor(object pConsumer, object pNext, object pOldLine)
{
	if(!pNext)
		pNext = pConsumer;
	for(var pLine in FindObjects(Find_Func("IsPowerLine")))
	{
		if(pLine == pOldLine) // Recursive -> Not backwards.
			continue;
		if(!pLine -> IsConnectedWith(pNext)) // Line connected with pConsumer.
			continue;
		var pEnd = pLine -> GetConnectedObject(pNext); // What's on the other end.
		if(!pEnd) // Nothing on the other end.
			continue;		
		if(pEnd == pConsumer) // End of a loop.
			continue;
		if(pEnd == this) // Found this object, i.e. the generator.
			return true;
		if(IsPowerGeneratorFor(pConsumer, pEnd, pLine))
			return true;		
	}
	return false;
}

/*-- Public calls --*/
// Functions that manipulate the power level.

// Returns the current power level of this object.
public func GetPower()
{
	return iPower;
}

// Sets the current power level of this object.
public func SetPower(int iSetPower)
{
	iPower = BoundBy(iSetPower, 0, GetCapacity());
  return true;
}

// Adds to the current power level of this object.
public func DoPower(int iDoPower)
{
	iPower = BoundBy(iPower + iDoPower, 0, GetCapacity());
  return true;
}

/*-- Debug --*/
/*
protected func Initialize() 
{
	AddEffect("ShowPower",this,100,10,this);
	return _inherited(...);
}

protected func FxShowPowerTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	Message("P:%d", pTarget, pTarget->GetPower());
	return true;
}
*/
