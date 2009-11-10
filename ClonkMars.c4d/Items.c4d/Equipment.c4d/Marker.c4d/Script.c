#strict

local pLaser;

protected func OnActivate() {
    pLaser = CreateObject(LASR,0,0,-1);
    SetClrModulation(RGBa(255,0,0,25),pLaser);
    pLaser->SetPosition(GetX()+Sin(GetR(),3),GetY()-Cos(GetR(),3));
    pLaser->Set(GetR(),2,200,0,this(),this());
    // bei Explosionen nicht wegfliegen
    SetCategory(C4D_Vehicle);
}

protected func OnActive() {             // Mine ist nun aktiv
    SetClrModulation(RGBa(255,255,255,100));
    SetClrModulation(RGBa(255,0,0,200),pLaser);
}
