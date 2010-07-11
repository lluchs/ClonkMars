/*-- Terraforming --*/

#strict 2
#include GOAL

local percent;

public func IsFulfilled() {
	var iGreen = 0;
	// alle stehenden Bäume
	for(var pTree in FindObjects(Find_Category(C4D_StaticBack), Find_Func("IsTree"))) {
		iGreen += GetDefWidth(pTree -> GetID()) * pTree -> GetCon() / 100;
	}
	percent = iGreen / LandscapeWidth();
	return percent >= GetGoal();
}

private func GetGoal() {
	return GameCall("TerraformingGoal") || 100;
}

protected func Activate(int iPlr) {
	if(IsFulfilled())
		return MessageWindow("Huge Success! \\o/", iPlr);
	else
		return MessageWindow(Format("Aktueller Stand: %d; zu erreichen: %d", percent, GetGoal()), iPlr);
}
