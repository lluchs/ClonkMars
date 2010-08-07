/*--- Chippie ---*/

Aggression: 
  return(Local(0));

Initialize:
  Sound("Birth");
  SetAction("Walk");
  ChangeDir();
  return(1);

Redefine:
  ChangeDef(Par(0));
  SetAction("Walk");
  return(1);

Damage:
  // Aggression steigern
  SetLocal(0,Sum(Local(0),+1));
  //Message("%d",this(),GetEnergy());
  Sound("Damage");
  return(1);

Activity:

  if (Not(Random(50))) ChangeDir();

  if (Not(Random(25))) JumpDrop();
  
  if (Or(OnFire(),InLiquid())) 
    Frenzy();

  if (InLiquid())
    SetLocal(0,0);
  
  if (Not(Aggression()))
    if(SetVar(0,FindObject(0,-100,-100,200,200,OCF_CrewMember()))) 
      Escape(Var(0));

  if (Aggression())
    if (SetVar(0,FindObject(0,-75,-50,150,100,OCF_CrewMember()))) 
      JumpAt(Var(0));

  if (SEqual(GetAction(),"Dead"))
    if (GreaterThan(GetActTime(),2000))
      RemoveObject();

//*  if (Random(25)) RandomSound();
  return(1);

Frenzy:
  if (SEqual(GetAction(),"Hug")) SetAction("Walk");
  SetComDir(Sum(1,Mul(Random(4),2)));
  if (Not(Random(5))) JumpDrop();
  return(1);

JumpDrop:
  if (SEqual(GetAction(),"Hangle")) return(SetAction("Jump"));
  Jump();
  return(1);

JumpAt:
  if (Not(GetAlive())) return(0);
  SetAction("JumpAt");
  SetXDir( Sub(GetX(Par(0)),GetX()) );
  SetYDir( Sub(GetY(Par(0)),GetY(),20) );
  return(1);

JumpAtTravel:
  if (SetVar(0,FindObject(0,+1,+1,0,0,OCF_CrewMember())))
    SetAction("Hug",Var(0));
  return(1);

SuckHug:
  Sound("ChippieSuck");
  DoEnergy(-3,GetActionTarget());
  if (Not(GetAlive(GetActionTarget()))) SetAction("Walk");
  return(1);

ChangeDir:
  if(SEqual(GetAction(),"Hangle")) return(ChgDirHangle());
  SetVar(0,COMD_Left());
  if(Random(2)) SetVar(0,COMD_Right());
  SetComDir(Var(0));
  return(1);

Escape:
  if(SEqual(GetAction(),"Hangle")) return(EscapeHangle());
  SetVar(0,Sub(GetX(this()),GetX(Par(0))));
  if(GreaterThan(Var(0),0)) return(SetComDir(COMD_Right()));
  return(SetComDir(COMD_Left()));

ChgDirHangle:
  SetVar(0,COMD_Up());
  if(Random(2)) SetVar(0,COMD_Down());
  SetComDir(Var(0));
  return(1);

EscapeHangle:
  SetVar(0,Sub(GetY(this()),GetY(Par(0))));
  if(GreaterThan(Var(0),0)) return(SetComDir(COMD_Down()));
  return(SetComDir(COMD_Up()));

RandomSound:
  if (SEqual(GetAction(),"Dead")) return(1);
  if (Random(5)) return(Sound("Random2"));
  if (Random(10)) return(Sound("Random3"));
  return(1);

Remove:
 RemoveObject(this());
 return(1);

DeathSound:
   Sound("Death");
   return(1);