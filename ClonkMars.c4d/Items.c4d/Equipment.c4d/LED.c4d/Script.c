/*--- LED ---*/

#strict 2

local light, attached, vertex;

static const LED_AttachBuilding = 1, LED_AttachTunnel = 2;
  
// Licht an/aus
protected func ControlDigDouble() {
	if(light) {
		SetPlrViewRange(0);
		light -> RemoveObject();
	}
	else {
		SetPlrViewRange(25);
		light = AddLight(250, RGB(200, 200, 255), this); // leicht bl�uliches Licht
	}
	return 1;
}

protected func ControlUpDouble() {
	return Activate();
}

protected func Activate() {
	// bereits befestigt?
	if(attached) {
		if(attached == LED_AttachBuilding) {
			var pBuilding = GetActionTarget();
			SetAction("Idle");
			pBuilding -> RemoveVertex(vertex); // Befestigungsvertex entfernen
		}
		else if(attached == LED_AttachTunnel) {
			SetAction("Idle");
		}
		SetCategory(C4D_Object);
		attached = 0;
		if(light)
			ControlDigDouble();
		return 1;
	}
	// nur an Tunnelw�nden und Geb�uden; nicht im Eingangsbereich eines Geb�udes
	var pBuilding = FindObject2(Find_AtPoint(), Find_OCF(OCF_Exclusive)), fTunnel = GetMaterial() == Material("Tunnel");
	if(!pBuilding && !fTunnel || pBuilding && pBuilding -> GetOCF() & OCF_Entrance && InEntrance(pBuilding)) {
		Sound("Error");
		Message("Anbringen hier nicht m�glich", this);
		return 1;
	}
	
	Sound("Connect");
	
	Exit(this, 0, GetDefHeight(GetID()) / 2);
	SetCategory(C4D_Vehicle); // damit man es anfassen kann
	// an Geb�ude anbringen
	if(pBuilding) {
		pBuilding -> AddVertex(GetX() - pBuilding -> GetX(), GetY() - pBuilding -> GetY());
		vertex = pBuilding -> GetVertexNum() - 1;
		SetAction("Attach", pBuilding);
		SetActionData(256*2 + vertex);
		attached = LED_AttachBuilding;
		AddEffect("AttachBuilding", this, 100, 50, this);
	}
	// an Tunnelwand anbringen
	else {
		SetAction("Float");
		attached = LED_AttachTunnel;
	}
	ControlDigDouble();
	return 1;
}

protected func RejectEntrance() {
	return attached;
}

protected func AttachTargetLost() {
	// der Befestigungsvertex ist schon weg
	RemoveEffect("AttachBuilding", this);
	attached = LED_AttachTunnel;
	Activate();
}

protected func FxAttachBuildingTimer() {
	if(GetAction() != "Attach")
		AttachTargetLost();
}

private func InEntrance(gbaeude) 
{
  if(Inside(GetX(),
    GetX(gbaeude)+GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),0),
    GetX(gbaeude)+GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),2)+GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),0) ))
      if(Inside(GetY(),
        GetY(gbaeude)+GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),1),
        GetY(gbaeude)+GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),3)+GetDefCoreVal("Entrance", "DefCore",GetID(gbaeude),1) ))
          return(1);
}

public func MarsResearch() { return true; }
