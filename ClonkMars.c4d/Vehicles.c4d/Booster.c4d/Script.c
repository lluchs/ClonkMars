/*-- Booster --*/

#strict 2


public func StartCountdown() {
	AddEffect("Countdown", this, 123, 36,this);
}

local countdown;

//Countdown
protected func FxCountdownTimer(){
	//RemoveEffect("Countdown",this);
	if(countdown == 0 || countdown == 30 || countdown == 45){
		Log("$rocketStarts$ %s $rocketStarts2$ %d $seconds$",GetPlayerName(GetOwner(this)),60-countdown);
	}
	if(countdown >= 50){
		Log("%d",60-countdown);
		//Soundausgabe den letzten Sekunden -------------NF!!--------------einfach wieder reinnehmen
		Sound(Format("t_%d",60-countdown));
	}
	if(countdown == 60){
		RemoveEffect("Countdown",this);
	}
	countdown++;
}

//Ende des Countdowns
protected func FxCountdownStop(){
	SetYDir(GetYDir(this)-1);
	AddEffect("Launch", this, 123, 1,this);
}

protected func FxLaunchStart(){
	LaunchEarthquake(0, 40,500);//Erdbeben
}

//Tolle Effekte
protected func FxLaunchTimer(){
	if(GetEffect("Launch",this,0,6) < 100){
		SetYDir(-3);
		Sound("Rocket_enginestart",0,this,0,0,0);
	}
	else{
		SetYDir(GetYDir(this)-2);
		Sound("Rocket_engineloop",0,this,100,0,0);
		for(var i = 0; i < 15; i++){
			CreateParticle("Thrust",0,35,RandomX(-2,2),100,180,RGBa(255,50,0,50));
			CreateParticle("Thrust",11,35,RandomX(-1,1),40,100,RGBa(255,100,40,50));
			CreateParticle("Thrust",-11,35,RandomX(-1,1),60,100,RGBa(255,100,40,50));
		}
		Smoke(RandomX(-20,20),RandomX(34,40),20);
		Smoke(RandomX(-20,20),RandomX(34,40),RandomX(10,30));
	}
	if(GetY(this) < -50){
		RemoveObject(this);
	}
}

public func MarsResearch() {
	return true;
}

