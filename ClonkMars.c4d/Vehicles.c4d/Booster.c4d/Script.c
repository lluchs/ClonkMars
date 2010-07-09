/*-- Booster --*/

#strict 2

local countdown;

public func StartCountdown() {
	AddEffect("Countdown", this, 123, 36,this);
	//debug
	countdown = 89;
}

//Countdown
protected func FxCountdownTimer(){
	//bei 2 min, 1 min und 30 sekunden Zeit ausgeben
	if(countdown == 0 || countdown == 60 || countdown == 90){
		Log("$rocketStarts$ %s $rocketStarts2$ %d $seconds$",GetPlayerName(GetOwner(this)),120-countdown);
		//Sound(Format("t_%d",120-cSound("Rocket_engineloop",0,this,100,0,1);ountdown));
	}
	//noch 30s
	if(countdown == 90){
		Sound("Rocket_pressure",0,this,0,0,1);
	}
	//noch 10s
	if(countdown == 110){
		Sound("Rocket_pressure",0,this,0,0,-1);
		Sound("Rocket_sparkle_start",0,this,0,0,1);
	}
	//noch 4s
	if(countdown == 116){
		Sound("Rocket_sparkle_start",0,this,0,0,-1);
		Sound("Rocket_enginestart",0,this,0,0,0);
		AddEffect("Launch", this, 123, 1,this);
	}
	//Ab 10s Countdown
	if(countdown >= 110){
		Log("%d",60-countdown);
		//Sound(Format("t_%d",120-countdown));
	}
	//Nach 2 Minuten Countdown löschen
	if(countdown == 120){
		RemoveEffect("Countdown",this);
	}
	//Zeit erhöhen
	countdown++;
}

//Ende des Countdowns
protected func FxCountdownStop(){
	SetYDir(GetYDir(this)-1);
}

protected func FxLaunchStart(){
	LaunchEarthquake(0, 40,500);//Erdbeben
}

//Tolle Effekte
protected func FxLaunchTimer(){
	//Enginetart - noch nicht bewegen
	if(GetEffect("Launch",this,0,6) < 144){
		for(var i = 0; i < 15; i++){
			CreateParticle("Thrust",0,35,RandomX(-2,2),100,180,RGBa(255,50,0,50));
			CreateParticle("Thrust",11,35,RandomX(-1,1),40,100,RGBa(255,100,40,50));
			CreateParticle("Thrust",-11,35,RandomX(-1,1),60,100,RGBa(255,100,40,50));
		}
		Smoke(RandomX(-20,20),RandomX(34,40),20);
		Smoke(RandomX(-20,20),RandomX(34,40),RandomX(10,30));
	}
	else{
		Sound("Rocket_engineloop",0,this,100,0,0);
		SetYDir(GetYDir(this)-2);
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

