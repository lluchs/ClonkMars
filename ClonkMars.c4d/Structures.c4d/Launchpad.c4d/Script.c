/*-- Startrampe --*/

#strict
#include B_60
#include DACT //Damagecontrol

local rocket;
local buildingProcess;

public func MaxDamage() { return 25; } //Maximaler Schaden

//Rakete bauen
public func ControlDigDouble() {
	if(!rocket){
	  if(!buildingProcess){
			if(CheckComponents()){
				RemoveComponents();
				buildingProcess = true;
				rocket = CreateObject(BSTR,-10,45,GetOwner(this));
				rocket -> SetAction("Construct0");
				Schedule("buildingProcess = false",600);
			}
			else{
				Message("$NotEnoughResources$",this);
				Sound("Error");
			}
		}
		else{
			Message("$IsBuilding$",this);
		}
	}
	else{
		Message("$RocketIsStillThere$",this);
	}
}

//gibt zurück ob alle Materialien vorhanden sind
private func CheckComponents(){
	var i,ID;
	while (ID = GetComponent(0, i++, 0, BSTR)){
		var amountNeed = GetComponent(ID,0,0,BSTR);
		var amountFound = GetLength(FindObjects(Find_ID(ID),Find_Container(this)));
		if(amountNeed > amountFound){
			return false;
		}
	}
	return true;
}

//Rohstoffe entfernen
private func RemoveComponents(){
	var i,ID;
	while (ID = GetComponent(0, i++, 0, BSTR)){
		var amountNeed = GetComponent(ID,0,0,BSTR);
		for(var n = 0; n < amountNeed; n++){
			RemoveObject(FindObject2(Find_ID(ID),Find_Container(this)));
		}
	}
}

//Rakete starten
public func ControlUpDouble() {
	if(rocket){
		if(!buildingProcess){
			rocket->Launch();
		}
		else{
			Message("$RocketBuildingInProcess$",this);
		}
	}
	else{
		Message("$NoRocketAvailable$",this);
	}
}
