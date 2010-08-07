Breed:
  if (Not(Random(2000))) Call("Break");
  return(1);
Hit:
  if (Random(2)) return(1);
  return(Call("Break"));
Damage:
  return(Call("Break"));
Break:
  if (Not(ActIdle())) return(0);
  if (GBackSolid()) return(0);
  Sound("EggBreak");
  SetAction("Break");
  AssignVar(0, CreateObject(_CHP) );
  return(1);
Destroy:
  AssignRemoval();
  return(1);
