/*-- Spielzielauswahl --*/

#strict 2

local choosable;

protected func Activate(int iPlr) {
	if(!iPlr && GetCursor()) {
		OpenMenu();
	}
	else {
		MessageWindow(GetDesc(), iPlr);
	}
}

private func OpenMenu() {
	var menu = CreateMenuTemplate(GetID(), "$RuleInfo$");
	
	var rules = GameCall("ChoosableRules");
	if(!rules) {
		rules = [];
		for(var i = 0, ID; ID = GetDefinition(i, C4D_Rule); i++) {
			if(ID -> ~IsChoosable())
				PushBack(ID, rules);
		}
	}
	
	var antirules = GameCall("NotChoosableRules");
	choosable = [];
	
	for(var ID in rules) {
		if(GetIndexOf(ID, antirules) != -1)
			continue;
		PushBack(ID, choosable);
		
		var condition = ID -> ~ChoosingCondition();
		AddBoolChoice(menu, 0, ID, condition, GetName(0, ID), ID, ObjectCount2(Find_ID(ID)));
	}
	
	CreateMenuByTemplate(GetCursor(), this, "Choose", menu);
}

public func Choose(array rules, ExtraData, bool canceled) {
	// bei Schlie�en durch Graben nichts tun
	if(canceled)
		return;
	
	// alle w�hlbaren Regeln entfernen
	for(var ID in choosable) {
		RemoveAll(ID);
	}
	
	var iter = HashIter(rules), node;
	while(node = HashIterNext(iter)) {
		if(node[1])
			CreateObject(node[0], 0, 0, NO_OWNER);
	}
	
	RemoveObject();
}
