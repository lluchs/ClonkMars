/* Sucht eine DPIP-Verbindung */

#strict 2

// The other three Parameters pNext, pOldLine and aOld are only used for recursive purposes.
global func PipelineConnectedWith(object pConsumer, object pNext, object pOldLine, array aOld)
{
	if(!pNext) { // Initial call to this function.
		pNext = pConsumer;
		aOld = [];
	}
	for(var pLine in FindObjects(Find_ID(DPIP))) // Check all lines connected to pNext.
	{
		if(pLine == pOldLine) // Recursive -> Not backwards<->forwards through lines.
			continue;
		//if(!pLine -> IsConnectedWith(pNext)) // Power line connected with pConsumer.
			//continue;
		var pEnd = pLine -> GetConnectedObject(pNext); // What is on the line's other end.
		if(!pEnd) // Nothing on the other end.
			continue;		
		if(pEnd == pConsumer) // End of a recursive loop.
			continue;
		if(GetIndexOf(pEnd, aOld) != -1) // We already know this
			continue;
		if(pEnd == this) // Found this object, i.e. the generator.
			return true;
		PushBack(pEnd, aOld);
		if(PipelineConnectedWith(pConsumer, pEnd, pLine, aOld)) // This building is not found, continue with next pEnd as next building.
			return true;		
	}
	return false;
}
