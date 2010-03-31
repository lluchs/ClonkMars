/*-- Waffenlabor --*/

#strict 2

#include B_95
#include OVDR
#include DACT //Damagecontrol

static const Sat_ResearchValue = 50;
static const Sat_AntiSatValue = 50;
static const Rok_NukeValue = 50;

private func SoundOpenDoor() {
	Sound("Door_Metal");
}

private func SoundCloseDoor() {
	Sound("Door_Metal");
}

local pLamp;

protected func Initialize() {
	pLamp = AddLightCone(1000, RGBa(255, 255, 255, 10));
	pLamp -> ChangeSizeXY(800, 1000);
	pLamp -> ChangeOffset(-55,-15);
	pLamp -> ChangeR(230);
	pLamp -> LocalN("bAmbience") = true;
}

/*Steuerung*/
/*etwas hardcode, besser wäre ein Kaufmenü bei dem man 
die drei Typen auswählen kann :: ist sogar buggy, es ist 
alles unendlich kaufbar :(*/

protected func ContainedUp(pCaller) {
  if(GetWealth(GetOwner(pCaller)) >= 250)
    {
    DoWealth(GetOwner(pCaller),-250);
    Sound("Special_Beep");
    }
  Produce();
  }
  
protected func Produce(pBase){
  CreateContents(BSTR,pBase,1);
  }

/*ebenfalls hardcodet. Es sollte auswählbar sein, was
genau von Content gestartet werden soll*/

protected func ContainedDigDouble(pBase){
  if(FindContents(BSTR,pBase))
    {
    SetAction("OpenGate",this());
    Sound("Warning_standclear");
    }
  }

/*.*/

protected func Incineration() {
	pLamp -> RemoveObject();
	return _inherited(...);
}

public func Deconstruction() {
	pLamp -> RemoveObject();
	return _inherited(...);	
}

public func PipelineConnect() {
	return 1;
}

public func MaxDamage() { return 50; } //Maximaler Schaden

public func GetTemp() { return 250; }
