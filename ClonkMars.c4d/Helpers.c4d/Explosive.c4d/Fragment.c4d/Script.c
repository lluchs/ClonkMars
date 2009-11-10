/*-- Fragment --*/

#strict

local timer;

private func Check()
{
  ++timer;
  if (timer > 100)
    return(RemoveObject());
  if (timer > 10)
    CastParticles("TSpark", (130-timer)/40, 10, 0,0, 20,20, RGB(192,192,160),RGB(255,255,255));
  return(1);
}

