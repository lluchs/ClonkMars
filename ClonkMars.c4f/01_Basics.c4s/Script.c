/*-- Basics --*/

#strict 2

func Initialize() {
  ScriptGo(1);
  CreateConstruction(BASE,50,320,0,100,1,0);  //1
  CreateConstruction(BASE,840,310,0,100,1,0); //2
  
  SetSkyAdjust(RGBa(200,94,51,90),RGB(200,110,55));
  SetGamma(RGB(50,0,0), RGB(140,100,100), RGB(255,220,220));
  //Hintergrund
  CreateObject(BG_2,-0,0,-1);
  
  return(1);
}

func Script10()
{
  MsgWindow(GetCrew(), "$Txt01$");
}

func Script12()
{
  if(!GetMenu(GetCrew()))
  {
    MsgWindow(GetCrew(), "$Txt02$");
    var Arrow=SetArrowToObj(FindObject2(Find_ID(BASE), Find_InRect(10, 210, 100, 100)));
  }
  else return goto(11);
}

func Script14()
{
  if(!GetMenu(GetCrew())&&FindObject2(Find_Func("NF_FindHelper_InBase")))
  {
    MsgWindow(GetCrew(), "$Txt03$");
    RemoveArrow();
    
  }
  else return goto(13);
}



func NF_FindHelper_InBase()
{
    return Contained() && Contained()->GetID() == BASE;
}

global func MsgWindow(pClonk, szText)
{
  Sound("Command");
  pClonk->SetComDir(COMD_Stop);
  CreateMenu(GetID(pClonk), pClonk, 0, 0, "", 0, 3);
  AddMenuItem("Portrait:SCNK::ff7777::3", "", NONE, pClonk, 0, 0, "", 5, 0, 0);
  AddMenuItem(szText, "", NONE, pClonk, 0, 0, "", 512);
  SetMenuTextProgress(1, pClonk);
  SetMenuDecoration(MD69, pClonk);
}
