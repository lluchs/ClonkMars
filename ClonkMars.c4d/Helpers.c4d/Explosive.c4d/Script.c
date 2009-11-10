/*-- Explosionen --*/

#strict

local timer, strength, speedY;


/* Initialisierung */

protected func Initialize()
{
  timer = 1;
  return(1);
}


/* Object-Call */

public func Settings(par_timer, par_strength, par_speed)
{
  timer = par_timer;
  strength = par_strength;
  speedY = par_speed;
  return(1);
}


/* Timer-Call */

protected func Countdown()
{
  // herunterzählen
  if (timer)
  {
    SetPosition(GetX(), GetY()-speedY);
    return(--timer);
  }
  // Explosion
  CreateParticle(Format("Blast%d",Random(3)), 0,0,0,0, strength*10, RGBa(255,255,255,Random(128)));
  var i = strength/8 + 1;
  while (--i)
    Smoke(Random(11)-5, Random(11)-5, 10);
  // Fragmente
  var amount = (strength**2)/60;
  CastParticles("Fragment1", amount, strength*7, Random(7)-3, Random(7)-3, 20,30);
  CastParticles("Fragment2", amount, strength*5, Random(7)-3, Random(7)-3, 20,30);
  CastParticles("Fragment3", amount, strength*3, Random(7)-3, Random(7)-3, 20,30);
  // funkensprühendes Fragment
  var frg = CreateObject(SFRG);
  SetXDir(Random(strength*6) - strength*3, frg);
  SetYDir(-Random(strength*2) - strength, frg);
  // Sound nur ab und zu mal
  if (Random(5))
    return(RemoveObject());
  // Sound abspielen
       if (strength < 35) Sound("Blast1");
  else if (strength < 50) Sound("Blast2");
                     else Sound("Blast3");
  return(RemoveObject());
}

