/* Sucht eine DPIP-Verbindung */

#strict 2

global func PipelineConnectedWith(object pTarget, object pNext, object pOldLine)
{
	if(!pNext)
		pNext = pTarget;
	for(var pLine in FindObjects(Find_ID(DPIP)))
	{
		if(pLine == pOldLine) // Recursive -> Not backwards.
			continue;
		var pEnd;
		if(pLine -> GetActionTarget(0) == pNext)
			pEnd = pLine -> GetActionTarget(1);
		else if(pLine -> GetActionTarget(1) == pNext)
			pEnd = pLine -> GetActionTarget(0);
		else
			continue;
		if(!pEnd) // Nothing on the other end.
			continue;		
		if(pEnd == pTarget) // End of a loop.
			continue;
		if(pEnd == this) // Found this object, i.e. the generator.
			return true;
		if(PipelineConnectedWith(pTarget, pEnd, pLine))
			return true;		
	}
	return false;
}
