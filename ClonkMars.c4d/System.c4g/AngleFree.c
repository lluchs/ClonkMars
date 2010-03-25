#strict 2

/* Checkt ob der Weg zum Himmel in Richtung iAngle frei ist */

global func AngleFree(int iAngle, int ox, int oy) {
	var iX, iY;
	if(!ox && !oy) {
		ox = GetX();
		oy = GetY();
	}
	
	var ax = Sin(iAngle, 50), ay = Cos(iAngle, 50);
	
	while(true) {
		iX = ox + ax;
		iY = oy + ay;
		
		//CreateParticle("PSpark", AbsX(iX), AbsY(iY), 0, 0, 70, RGB(255));
		
		// PathFree mag globale, GBackLiquid lokale Koordinaten
		if(PathFree(iX, iY, ox, oy) && !GBackLiquid(AbsX(iX), AbsY(iY))) {
			if(iX < 0 || iX > LandscapeWidth() || iY < 0)
				return 1;
		}
		else
			return;
		
		ox = iX;
		oy = iY;
	}
}
