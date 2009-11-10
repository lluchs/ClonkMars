#strict

local amount;

public func Start(id idObject, int iOwner, int count){
  SetOwner(iOwner);
  SetVisibility(VIS_Owner);
  if(count)
  {
  	amount = count;
  	CustomMessage(Format("@%d",count),this, iOwner, 0, 40, RGB(255,255,255));
  }
//  Message("@{{%i}}",this(),idObject);
	SetGraphics(0,this,idObject,1,GFXOV_MODE_IngamePicture);
  SetPosition(30,100);
  SetCategory(C4D_StaticBack | C4D_Parallax);
  while(Var()=FindObjectOwner(GetID(),GetOwner(),0,0,0,0,0,0,0,Var())){
    SetPosition(GetX(Var())+20,GetY(Var()),Var());
  }
  for(var s in FindObjects(Find_ID(KLOG), Find_InRect(AbsX(100),AbsY(),LandscapeWidth()-100,LandscapeHeight())))
		s->FadeOut(s);
  ScheduleCall(this,"FadeOut",150);
  return(true);
}

func GetCount() { return amount; }

global func ItemLog(id def, int owner, int count) { if(!owner) owner=GetOwner(); return(CreateObject(KLOG)->KLOG::Start(def,owner,count)); }

// *aus Hazard klau*

func FadeOut(object pObject, int level)
{
  if(!pObject)
    pObject = this();
    
  if(GetEffect("*FadeOut*", pObject)) return();
  return(AddEffect("FadeOut", pObject, 101, 2,pObject,KLOG,level));
}

func FxFadeOutStart(target, no, temp, level)
{
  if(!level)
    EffectVar(1, target, no) = 255;
  else
    EffectVar(1, target, no) = level;
}

func FxFadeOutTimer(target, no)
{
  EffectVar(0, target, no) += 5;
  var alpha = EffectVar(0, target, no);
  SetClrModulation(RGBa(255,255,255, alpha), target,1);
 	CustomMessage(Format("@%d",target->GetCount()),target, GetOwner(target), 0, 40, RGBa(255,255,255,alpha));
  if(alpha >= EffectVar(1, target, no)) { if(EffectVar(1, target, no) >= 255) RemoveObject(target); return(-1); }
}
