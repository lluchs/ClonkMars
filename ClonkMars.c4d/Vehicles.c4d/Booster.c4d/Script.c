/*-- Booster --*/

#strict 2

public func Launch() {
	AddEffect("EffectsDust", this, 1, 1, this);
	AddEffect("Launching", this, 1, 1, this);
}

protected func FxLaunchingTimer() {
	SetYDir(Max(GetYDir() - 10, -100));
	if(GetY() <= 0) {
		RemoveObject();
	}
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

public func MarsResearch() {
	return true;
}

