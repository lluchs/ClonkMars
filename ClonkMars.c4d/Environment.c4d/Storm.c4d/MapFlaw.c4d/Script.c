#strict 2

static const ReflectIterAngle = 30;
static const ReflectIterRade = 3;

protected func Initialize() {
	++iWindProfilingObjects;
}

protected func Hit(int iXDir, int iYDir){
	//CreateParticle("Blast", 0,0, 0,0, 100, RGBa(128,128,255,0));
	--iWindProfilingObjects;
	var ang1 = -1, ang2 = -1, angc, sld = GBackSolid(0, -ReflectIterRade), ang;
	for(var i=ReflectIterAngle; i<360; i += ReflectIterAngle){
		if(GBackSolid(Sin(i, ReflectIterRade), -Cos(i, ReflectIterRade)) != sld) {
			sld = !sld;
			if(!angc) { ang1 = i; angc=1; }
			else { ang2 = i;  break; }
		}
	}
	ang = Abs((ang1+ang2)/2-90) % 180;
	var action;
	if(ang < 90) {
		if(Random(90)<ang) action = (GetWind()>0)*2-1;
	} else {
		if(Random(90)+90<ang) action = (GetWind()<0)*2-1;
	}
	if(!action) { RemoveObject(); return;}
	if(action > 0) {
		InsertMaterial(Material("Earth"), 0, 0, 3, -10);
		InsertMaterial(Material("Earth"), 0, 0, 0, -10);
		InsertMaterial(Material("Earth"), 0, 0, -3, -10);
	} else {
		DigFree(GetX(), GetY(), 1);
	}
	RemoveObject();
	return;
	/*ang = 2 * ang - Angle(0, 0, iXDir, iYDir); //Damit berechnet man den Reflexionswinkel. Und ich bin so selbstverliebt, dass ich das nicht löschen will.
	var spd = Distance(iXDir, iYDir);
	SetXDir(+Sin(ang, spd), 0, 100);
	SetYDir(-Cos(ang, spd), 0, 100);*/
}
