/*-- Include for power consumers --*/

#strict 2

/*-- Public calls --*/
// Functions that specify object properties, should be overloaded by the object.

// Returns whether the object is a power consumer.
public func IsPowerConsumer() 
{
	return true;
}

private func EnergyNeedOverlay() {
	return GFX_Overlay;
}

// Returns if a power line can be connected to this object.
public func CanPowerConnect() // Other name?
{
	return GetCon() >= 100;
}

/*-- Other --*/

// Checks whether there is enough power to sustain iPowerNeed.
// returns true and substracts iPowerNeed if there is enough power, otherwise false.
// fSubstract determines whether the check substracts iPowerNeed.
// If false it starts showing the power need object.
// If true it stops showing the power need object.
public func CheckPower(int iPowerCheck, bool fNoSubstract, bool fNoWarning) 
{
	if(!FindObject2(Find_ID(ENRG))) 
		return true;
	var aGenerators = FindObjects(Find_PowerGenerator(), Sort_GeneratorPriority()), i;
	for(var pGenerator in aGenerators)
	{
		// kein wildes Hin- und Herleiten zwischen Akkus
		if(this -> ~IsBattery() && pGenerator -> ~IsBattery()) {
			continue;
		}
		
		var iPower =  pGenerator -> GetPower();
		if(iPower >= iPowerCheck)
		{
			if(!fNoSubstract) {
				//Message("-%d|%v", pGenerator, iPowerCheck, this);
				pGenerator -> DoPower(-iPowerCheck);
				i--;
				while(i >= 0) {
					//Message("-%d|%v", aGenerators[i], aGenerators[i] -> GetPower(), this);
					aGenerators[i] -> DoPower(-aGenerators[i] -> GetPower());
					i--;
				}
			}
			RemoveEffect("EnergyNeed",this);
			return true;
		}
		else
			iPowerCheck -= iPower;
		i++;
	}
	if(!fNoWarning)
		AddEffect("EnergyNeed",this,100,12,this);
	return false;
}

// Finds all power generators connected to pConsumer.
private func Find_PowerGenerator(object pConsumer)
{
  if(!pConsumer) pConsumer = this;
  return [C4FO_Func, "IsPowerGeneratorFor", pConsumer];
}

// Sorts power generators according to GetGeneratorPriority(), highest return value -> first in array.
global func Sort_GeneratorPriority()
{
	return [C4SO_Reverse, [C4SO_Func, "GetGeneratorPriority"]];
}

/*-- Effect to show energy need --*/

private func FxEnergyNeedStart(object pTarget, int iEffectNumber, int iTemp)
{
	SetGraphics(0, pTarget, SENG, EnergyNeedOverlay(), GFXOV_MODE_Action, "ShowEnergy");
	SetObjDrawTransform(1000, 0, 0, 0, 1000, -500*GetDefCoreVal("Height", "DefCore", GetID()), pTarget, EnergyNeedOverlay());
	//var y = -12-GetDefCoreVal("Height", "DefCore", GetID())/2;
	//EffectVar(0, pTarget, iEffectNumber) = CreateObject(SENG,0,y,NO_OWNER);
	return 1;
}

protected func FxEnergyNeedStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
	SetGraphics(0, pTarget, SENG, EnergyNeedOverlay(), GFXOV_MODE_Action, "NoEnergy");
	return 1;
}

protected func FxEnergyNeedEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber)
{
	if(szNewEffectName == "EnergyNeed")
		return -1;
}

protected func FxEnergyNeedTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	if(GetCon() < 100)
		return -1;
	iEffectTime %= 24;
	//Log("%d",iEffectTime);
	if(iEffectTime == 0)
		SetGraphics(0, pTarget, SENG, EnergyNeedOverlay(), GFXOV_MODE_Action, "NoEnergy");
	if(iEffectTime == 12)
		SetGraphics(0, pTarget, SENG, EnergyNeedOverlay(), GFXOV_MODE_Base, "ShowEnergy");
		//SetObjDrawTransform(1000, 0, 0, 0, 1000, -500*GetDefCoreVal("Height", "DefCore", GetID()), pTarget, EnergyNeedOverlay());
	return 1;
}
	







