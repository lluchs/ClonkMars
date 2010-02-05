/*--Kapsel--*/

#strict 2

#include DACT
#include SRMO

local port, attachvertex;
local mode, blowout, aimblowout; //mode: 1 for automatic; blowout: 0 for no emission, 1 for low emission, 2 for high, 3 for left and 4 for right.
local sat;

static const iCapsMaxSpeed  = 2500; //iPrecision = 500
static const iCapsLandSpeed = 120;
static const iCapsAcceleration = 40;

public func SetBlowout(int bo) {
	if((bo&1<<31) && ((bo^1<<31) != aimblowout)) {return;} //True for a schedule call for an earlier change
	if(bo&1<<31) { //Called by Schedule
		blowout = bo^1<<31;
		bo = blowout;
		aimblowout = -1; 
	} else {
		if(blowout && !(blowout == 1 && bo == 2 || blowout == 2 && bo == 1)) { //If there is an action on a different dir
			ScheduleCall(this, "SetBlowout", 10, 1, bo^1<<31);
			aimblowout = bo;
			bo = 0;
		} else blowout = bo;
	}
	if(!bo) {
		//if(GetIndexOf(GetAction(),["Idle","PortLand","FreeFall"])!=-1) {}
		if(GetAction() == "Blowout") {SetAction("FreeFall");}
		else if(GetAction() == "RightBoostTurnUp") {var phas = GetPhase(); SetAction("RightBoostTurnDown"); SetPhase(4-phas);}
		else if(GetAction() == "RightBoostMax") {SetAction("RightBoostTurnDown");}
		else if(GetAction() == "LeftBoostTurnUp") {var phas = GetPhase(); SetAction("LeftBoostTurnDown"); SetPhase(4-phas);}
		else if(GetAction() == "LeftBoostMax") {SetAction("LeftBoostTurnDown");}
	} else if((bo == 1 || bo == 2) && !(GetAction() == "Blowout")) {
		SetAction("Blowout");
	} else if(bo == 3 && !WildcardMatch(GetAction(), "LeftBoost*")) {
		SetAction("LeftBoostTurnUp");
	} else if(bo == 4 && !WildcardMatch(GetAction(), "RightBoost*")) {
		SetAction("RightBoostTurnUp");
	}
	if(bo && !GetEffect("Blowout", this)) {AddEffect("Blowout", this,1,1,this);}
}

protected func RejectEntrance(object pObj) 
{
  if(pObj->GetID() == BASE) 
    return true;
}

protected func Initialize() {
	aimblowout = -1;
	attachvertex = -1;
	SetAction("FreeFall");
	SetComDir(COMD_Down);
	SetYDir();
	return _inherited();
}

public func SetDstPort(object pPort) {
	if(GetGravity() < 1) { Log("Can't land without gravity."); Explode(3000); return; }
	if(pPort) pPort->Occupy(this);
	port = pPort;
	var dst = Abs(GetY()-GetY(pPort))-30;
	if(!pPort) dst = Abs(GetY()-GetHorizon(GetX())) - 150;
	var iter = 300000/GetGravity()/dst;
	var t;
	while(true){
		t+= iter;
		var i=t,d=0,s=0,tges=t;
		while(i--) {
			s += GetGravity();
			d += s;
		}
		while(s > iCapsLandSpeed) {
			s -= iCapsAcceleration;
			d += s;
			++tges;
		}
		if(d/500 > dst) {
			if(iter == 1) break;
			else {
				t -= 2*iter;
				iter /= 2;
			}
		}
	}
	if(AdvancedWindCalculations()) {
		var dir,pos=100;
		for(var i;i<tges;++i) { //FIXME: Use forecast. And fix that whole thingy
			dir += (dir-GetWind(0,0,true))**2*WindEffect()/1000;
			pos += dir;
		}
		if(GetWind(0,0,true)<0) pos *= -1;
		SetPosition(GetX()-pos/100, GetY());
	}
	ScheduleCall(this, "StartLanding", t);
	if(port)
		ScheduleCall(port, "PortActive", 50);
	return 1;
}

public func SetSat(pSat) {
	sat = pSat;
}

private func DestroyBlast() {
	if(sat) sat -> CapsuleDestroyed();
	return _inherited(...);
}

global func GetHorizon(int iX) { var iY; while(!GBackSemiSolid(iX,AbsY(iY++))); return iY; }

// hat einen Sauerstoffspeicher zum Landen
public func IsO2Producer() {
	return 1;
}

protected func Hit(int iXDir, int iYDir) {
	var hit = Distance(iXDir, iYDir)/3 - 70;
	if(hit < 1) return;
	if(hit < 90) DoDamage(hit/3);
	else DestroyBlast();
	return;
}

protected func ContainedDigDouble() {
	if(GetAction() == "FreeFall")
		StartLanding();
	return 1;
}

private func StartLanding() {
	if(blowout || aimblowout != -1) return;
	if(port) port->PortActive(); //Port wird beim laden zweimal aktiviert, falls die Landung früh anfängt...
	SetBlowout(1);
	return 1;
}

protected func FxBlowoutTimer(object pObj, int iEffectNumber, int iEffectTime) {
	if(Inside(blowout, 1, 2)) {
		if(!(iEffectTime%3)) EffectDust(); //Staub
		//Gegenrotation
		if(GetR() < -1 && GetRDir() < 1) SetRDir(GetRDir(0, 50)+1, 0, 50);
		else if(GetR() > 1 && GetRDir() > -1) SetRDir(GetRDir(0, 50)-1, 0, 50);
		//Beschleunigung
		var accspeed = Min(iCapsMaxSpeed - Cos(GetR()-Angle(0,0,GetXDir(this, 500),GetYDir(this, 500)),Distance(GetYDir(this, 500), GetXDir(this, 500))), iCapsAcceleration+GetGravity());
		if(blowout != 1 || GetYDir(pObj,500) > iCapsLandSpeed) {
			SetXDir(GetXDir(this, 500)+Sin(GetR(),accspeed), this, 500);
			SetYDir(GetYDir(this, 500)-Cos(GetR(),accspeed), this, 500);
		}
		if(mode && GetY() <= -20) {
			for(var pObj in FindObjects(Find_Container(this)))
				if(pObj) pObj -> Sell(GetOwner());
			if(port) port->PortWait();
			RemoveObject();
		}
		if(mode && (iEffectTime > (LandscapeHeight() + 300))) DoDamage(1); //Falls die Kapsel nicht richtig startet
	} else if(blowout == 4) {
		SetXDir(Max(GetXDir(0,70)-1,-100),0, 70);		
		SetRDir(GetRDir(0, 100)-1, 0, 100);
	} else if(blowout == 3) {
		SetXDir(Min(GetXDir(0,70)+1,+100),0, 70);
		SetRDir(GetRDir(0, 100)+1, 0, 100);
	} else {
		if(aimblowout == -1) {return -1;}
	}
}

protected func ContactBottom() { 
	if(blowout && blowout != 2) {
		SetBlowout(0);
		if(port && port==FindObject2(Find_ID(PORT),Find_AtPoint())) {
			attachvertex = GetVertexNum();
			AddVertex();
			SetVertex(attachvertex, 2, CNAT_NoCollision, this, 1);
			AddVertex();
			SetVertex(attachvertex, 0, GetVertex(0, 0, port)+GetX(port)-GetX(), this, 2);
			SetVertex(attachvertex, 1, GetVertex(0, 1, port)+GetY(port)-GetY(), this, 2);
			SetAction("PortLand", port);
			SetActionData(256*attachvertex, this);
			//FIXME: Do that less hacky...
		} 
		if(ObjectCount2(Find_Container(this), Find_OCF(OCF_CrewMember)))
			ScheduleCall(this, "Eject", 30);
		if(port) ScheduleCall(port, "PortWait", 50);
	}
	return 1;
}

protected func Eject() {
	if(blowout || aimblowout != -1) return;
	for(var pObj in FindObjects(Find_Container(this), Find_OCF(OCF_CrewMember)))
		pObj -> Exit();
}

protected func ControlUpDouble() {
	if(port) port->PortActive();
	ScheduleCall(this, "Launch", 60);
}

private func Launch() {
	RemoveVertex(attachvertex);
	attachvertex = -1;
	SetBlowout(2);
	mode = 1;
}

// geklaut von Hazard
protected func EffectDust() {

	// Dust effect
	var mat,i;
	
	// maximum distance in which the shuttle appears
	var maxdistance = 150;
	
	// search for ground (yomisei: please use your sensor-function for that as soon as you finished it)
	for(i=10; i<maxdistance; i+=5) {
		if(GBackSolid(0,i)) {
			mat = GetMaterial(0,i);
			break;
		}
	}
	
	// ground in distance
	if(i<maxdistance) {
	
		// check if digable
		if(CheckDust(mat)) {
		
			// determine material color
			var rand = Random(3);
			var r = GetMaterialColor(mat,rand,0);
			var g = GetMaterialColor(mat,rand,1);
			var b = GetMaterialColor(mat,rand,2);
			
			// all values dependend on distance
			var size = RandomX(20,300-i/2);
			var alpha = Min(255,120+i);
			var pos = RandomX(0,30);
			// the nearer the dust to the center, the faster it is blown aside 
			CreateParticle("Dust",-pos,i,(-50+pos)+GetXDir()/2,RandomX(-5,5),size,RGBa(r,g,b,alpha));
			CreateParticle("Dust",+pos,i,(+50-pos)+GetXDir()/2,RandomX(-5,5),size,RGBa(r,g,b,alpha));
		}
		if(i < 50) {
			var iX, iY, iM;
			for(var j = 0; j < 20; j++) {
				iX = RandomX(-20, 20);
				if(!Random(2))
					iX *= 2;
				iY = 0;
				while(!GBackSolid(iX, ++iY));
				iM = (70 - i);
				iY += RandomX(-2, 2) + iM / 3;
				if(GBackSolid(iX, iY))
					SetLandscapePixel(iX, iY, RGB(iM, iM, iM));
			}
		}
	}
}

public func Flying() { return !GetContact(this, -1);}

public func MaxDamage() { return 20; }
public func WindEffect() { return 200;}

//
public func ContainedUp(pControl) {
	if(GetPlrCoreJumpAndRunControl(GetOwner(pControl))) {
		SetBlowout(1);
	} else {
		if(aimblowout != -1) { 
			if(aimblowout == 2) SetBlowout(1);
			else SetBlowout(0);
		} else if(blowout == 2 || blowout == 1) SetBlowout(blowout-1);
	}
}

public func ContainedDownDouble(pControl) {return ContainedDown(pControl);}
public func ContainedDown(pControl) {
	if(GetPlrCoreJumpAndRunControl(GetOwner(pControl))) {
		SetBlowout(2);
	} else {
		if(aimblowout != -1) {
			if(aimblowout == 1) SetBlowout(2);
			else SetBlowout(1);
		} else {
			if(blowout < 2) SetBlowout(blowout+1);
			else if(blowout != 2) SetBlowout(1);
		}
	}
	SetCommand(pControl, "None");
}

public func ContainedLeft() {
	SetBlowout(3);
}

public func ContainedRight() {
	SetBlowout(4);
}

public func ContainedUpReleased() { SetBlowout(0); }
public func ContainedDownReleased() { SetBlowout(0); }
public func ContainedLeftReleased() { SetBlowout(0); }
public func ContainedRightReleased() { SetBlowout(0); }
public func ContainedDig() { SetBlowout(0); }

protected func RejectEntrance() {return 1;}