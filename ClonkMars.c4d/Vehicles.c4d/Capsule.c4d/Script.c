/*--Kapsel--*/

#strict 2

#include DACT

local port, attachvertex;

static const iCapsMaxSpeed  = 2500; //iPrecision = 500
static const iCapsLandSpeed = 100;
static const iCapsAcceleration = 50;

protected func RejectEntrance(object pObj) 
{
  if(pObj->GetID() == BASE) 
    return true;
}

protected func Initialize() {
	attachvertex = -1;
	SetAction("FreeFall");
	SetComDir(COMD_Down);
	SetYDir();
	return 1;
}

public func SetDstPort(object pPort) {
	port = pPort;
	var dst = Abs(GetY()-GetY(pPort))-15;
	if(!pPort) dst = Abs(GetY()-GetHorizon(GetX())) - 150;
	var t1 = iCapsLandSpeed / iCapsAcceleration; //Time to reach land speed in acceleration
	var t2 = (2*iCapsLandSpeed + Sqrt(4*iCapsLandSpeed**2 + (GetGravity()+iCapsAcceleration*2)*dst*2000)) / (4*(25+iCapsAcceleration)) *9/7;
	//Eigentlich sollte diese Formel (Ohne *4/3) die Zeit liefern, die die Kapsel frei fliegen darf, bevor sie abbremsen muss
	//um genau 30 Pixel �ber der Platform iCapsLandSpeed zu erreichen. Wer fixen will, nurzu. Ungef�hr funktionert sie allerdings.
	/*Diese hingegen versagt. Wer eine (vermutlich kaputte) Herleitung will, JCaesar ist schuld
	var g = -iCapsAcceleration; //Scal: 500
	var v = iCapsLandSpeed; //Scal: 500
	var f = GetGravity(); //Scal: 500
	var r = 10* g / f; //Scal: 10
	var ta = 10* v / f; //Scal: 10
	var a = (5*f + g*r**2/20); //Scal: 5.000
	var b = -g*r**2*ta/10 -10*r*v; //Scal: 5.000
	var c = g*r**2*ta**2/20 + r*v*ta; //Scal: 5.000
	Log("g%d, v%d, f%d, r%d, ta%d, a%d, b%d, c%d -> %d", g,v,f,r,ta,a,b,c, (-b+Sqrt(b**-4*a*c))/(2*a));
	ScheduleCall(this, "StartLanding", (-b+Sqrt(b**-4*a*c))/(2*a));*/
	ScheduleCall(this, "StartLanding", t1+t2);
	if(port)
		ScheduleCall(port, "PortActive", 50);
	return 1;
}

global func GetHorizon(int iX) { var iY; while(!GBackSemiSolid(iX,AbsY(iY++))); return iY; }

// hat einen Sauerstoffspeicher zum Landen
public func IsO2Producer() {
	return 1;
}

protected func ContactBottom() { if(GetEffect("Landing",this)) {RemoveEffect("Landing", this); Fertig();}}

protected func Hit(int iXDir, int iYDir) {
	var hit = Distance(iXDir, iYDir)/2 - 70;
	if(hit < 1) return;
	if(hit > 90) DoDamage(hit/3);
	else DestroyBlast();
	return;
}

protected func ContainedDigDouble() {
	if(GetAction() == "FreeFall")
		StartLanding();
	return 1;
}

private func StartLanding() {
	if(port) port->PortActive(); //Port wird beim laden zweimal aktiviert, falls die Landung fr�h anf�ngt...
	SetAction("BlowoutStart");
	AddEffect("Landing",this,1,1,this);
	AddEffect("EffectsDust", this, 1, 3, this);
	return 1;
}

protected func FxLandingTimer(pObj) {
	SetYDir(Max(GetYDir(pObj,500)-GetGravity()-iCapsAcceleration,iCapsLandSpeed),pObj,500);
}

private func Fertig() {
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
	} else  SetAction("Idle");
	if(ObjectCount2(Find_Container(this), Find_OCF(OCF_CrewMember)))
		ScheduleCall(this, "Eject", 30);
	RemoveEffect("Landing", this);
	RemoveEffect("EffectsDust", this);
	if(port) ScheduleCall(port, "PortWait", 50);
	return 1;
}

protected func Eject() {
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
	SetAction("BlowoutStart");
	AddEffect("EffectsDust", this, 1, 3, this);
	AddEffect("Launching", this, 1, 1, this);
}

protected func FxLaunchingTimer(object pTarget, int iEffectNumber, int iEffectTime) {
	var accspeed = Min(iCapsMaxSpeed - Cos(GetR()-Angle(0,0,GetXDir(this, 500),GetYDir(this, 500)),Distance(GetYDir(this, 500), GetXDir(this, 500))), iCapsAcceleration/3+GetGravity());
	SetXDir(GetXDir(this, 500)+Sin(GetR(),accspeed), this, 500);
	SetYDir(GetYDir(this, 500)-Cos(GetR(),accspeed), this, 500);
	if(GetY() <= 0) {
		for(var pObj in FindObjects(Find_Container(this)))
			pObj -> Sell(GetOwner());
		if(port) port->PortWait();
		RemoveObject();
	}
	if(GetR()) {
		if(GetR() < 0) SetRDir(+1, 0, 50);
		else SetRDir(-1, 0, 50);
	}
	if(iEffectTime > (LandscapeHeight() + 300)) DoDamage(1); //Falls die Kapsel nicht richtig startet
}

// geklaut von Hazard
protected func FxEffectsDustTimer() {

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

public func MaxDamage() { return 20; }