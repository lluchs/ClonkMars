/*-- Kaufmen� --*/

#strict 2

global func CreateBuyMenu(object MenuObject)
{
	// hat das Objekt schon ein offenes Men�?
	if(GetMenu(MenuObject))
	// Jap. Zumachen (wegen Autokontexmen�)
		CloseMenu(MenuObject);

	// Men� aufmachen
	CreateMenu(GOLD, MenuObject, 0, C4MN_Extra_Value, "$BuyMenu$", 0, C4MN_Style_Context, true);
	
	// Eintr�ge einf�gen
	for(var i=1; Par(i); i++)
		AddMenuItem("$BuyItem$","BuyMenuItem", Par(i), MenuObject, 0, MenuObject);
}


// Kauft ein Objekt wenn m�glich. Gibt true zur�ck falls erfolgreich, sonst false
// Sollte sich das Objekt in einem anderen befinden, so wird das gekaufte dort hinein gelegt
global func BuyMenuItem(id ItemID, object MenuObject)
{
	var owner = GetOwner(MenuObject);
	var value = GetDefValue(ItemID);
	if(GetWealth(owner) >= value)
	{
		DoWealth(owner,-value);
		
		var obj;
		
		// Ist der Clonk in etwas drin, gehen wir davon aus, dass es da rein soll
		if(Contained(MenuObject))
			obj = CreateContents(ItemID,Contained(MenuObject));
		else
			obj = CreateContents(ItemID,MenuObject);
		
		// Ist es ein Crewmember, so wird es hinzugef�gt
		if(GetOCF(obj) | OCF_CrewMember)
		{
			MakeCrewMember(obj, owner);
		}
		
		Sound("Cash");
	}
	else
	{
		Message("$NotenoughMoney$");
		Sound("CommandFailure1");
		
		return false;
	}
	
	return true;
}
