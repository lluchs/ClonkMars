/*--- Kit ---*/

#strict 2
#include L_RC // Recycling
#include L_SS

private func MaxFill() { return 3; }
private func FillPicture() { return; }

local Used;

protected func Initialize() {
	DoFill(3);
}

protected func Activate(object pClonk) {
	var act = pClonk -> GetAction();
	var dir, r = 0, x = 0, y = 10;
	if(act == "Hangle") {
		dir = DIR_Top;
		r = 180;
		y = 14;
	}
	else if(act == "Scale" || act == "ScaleDown") {
		dir = pClonk -> GetDir();
		//y = 10;
		if(dir == DIR_Left) {
			r = 90;
			x = 5;
		}
		else {
			r = 270;
			x = -5;
		}
	}
	else
		dir = DIR_Bottom;
	

	
	var pSite = CreateConstruction(SPTR, x, y, pClonk -> GetOwner(), 100, false, false);	
	if(pSite) {
		// Drehung nach Erdoberfläche
		pSite -> SetR(r);
		var x_off = [pSite->GetVertex(1), pSite->GetVertex(3)];
		var y_off = [pSite->GetVertex(1, true), pSite->GetVertex(3, true)];
		var slope = pSite -> GetSolidOffset(x_off[0], y_off[0], dir) - pSite -> GetSolidOffset(x_off[1], y_off[1], dir);
		pSite -> SetR(r + slope*2);
		
		// debug
		/*
		Message("slope: %d|%v,%v", pSite, slope, x_off, y_off);
		pSite->CreateParticle("PSpark", x_off[0], y_off[0], 0, 0, 50, RGB(255), pSite);
		pSite->CreateParticle("PSpark", x_off[1], y_off[1], 0, 0, 50, RGB(0, 255), pSite);
		*/
		
		DoFill(-1);
		if(!GetFill())
			RemoveObject();
	}
	return 1;
}

static const DIR_Top = 2;
static const DIR_Bottom = 3;

global func GetSolidOffset(int x, int y, int dir) {
	var i;
	if(dir == DIR_Left)
		for (i = -15; GBackSolid(x + i, y) && (i < 15); i++);
	if(dir == DIR_Right)
		for (i = -15; GBackSolid(x - i, y) && (i < 15); i++);
	if(dir == DIR_Top)
		for (i = -15; GBackSolid(x, y + i) && (i < 15); i++);
	if(dir == DIR_Bottom)
		for (i = -15; GBackSolid(x, y - i) && (i < 15); i++);
	return i;
}

public func MarsResearch() { return true; }
