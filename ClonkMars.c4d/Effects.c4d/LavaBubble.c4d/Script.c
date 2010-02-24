/*-- Lavablase --*/

#strict


protected func Initialize()
{
  SetComDir(COMD_Up());
  SetAction("Grow");
  SetDir(Random(4));
}

private func Floating()
{
  // an der Oberfl�che zerplatzen
  if (!GBackLiquid(0,-3) || (GetActTime() > 1500))
  {
    CastPXS("DuroLava", 75, 25);
    return(RemoveObject());
  }
  // sich den Weg bahnen
  if (GBackSolid(0, -10)) SetYDir(+15);
  if (GBackSolid(-8,-8)) SetXDir(+5);
  if (GBackSolid(+8,-8)) SetXDir(-10);
  // Str�mung
  if (!Random(3))
    if (GetY() > 1650)
      SetXDir(5-Random(30));     // leichte Str�mung nach links
    else
      SetXDir(-10-Random(15));   // starke Str�mung nach links
}

