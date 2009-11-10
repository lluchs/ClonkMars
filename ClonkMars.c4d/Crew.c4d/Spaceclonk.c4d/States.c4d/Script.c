/*-- States to include! --*/

#strict 2

local iWarmth,iWarmTime;

private func MaximumWarmth() { return(2000); }

protected func Initialize() {
	// Wäremewert
	iWarmth = MaximumWarmth();
	AddEffect("States", this, 100, 35, this);
	return _inherited(...);
}

/* EffectCall */

protected func FxStatesTimer() {
  
  // Schon tot
  if (!GetAlive()) return(0);
  var iWarmthChange,iBuildingWarmth;

  // Wärmeverlust/Aufwärmung je nach Temperatur
  iWarmthChange=BoundBy(GetTemperature()-10, -100, +20);
  // Extremer Wärmeverlust im Wasser
  if (!Contained())
    if (InLiquid()) 
      iWarmthChange*=8;
  // Aufwärmung in Gebäuden
  if (Contained())
    if (iBuildingWarmth=(Contained()->~GetWarmth()))
      iWarmthChange=iBuildingWarmth;
  // Wärmewert verändern
  iWarmth=BoundBy( iWarmth+iWarmthChange, 0, MaximumWarmth() );

  // Wärmewert 0: Energieverlust
  if (!iWarmth) DoEnergy(-1);
  
  CheckWarnings(iWarmthChange);
      
  // Zeit in Gebäude und warm
  if (Contained() && iWarmthChange>0) iWarmTime++;
  else iWarmTime=0;
  // Zur Zeit kein Platz im Iglu: Aufenthaltwert beschränken
  if (Contained())
    if (ObjectCall(Contained(),"IsOccupied"))
      iWarmTime=Min(iWarmTime/2);     
  // Zur Kontrolle
  //Message("Temperatur: %d|Wärmeverlust: %d|Wärme: %d",this(),GetTemperature(),iWarmthChange,iWarmth);      
}

protected func FxStatesStop() {
	RemoveWarnings();
}

/* Status */

private func IsCold() {
  if (iWarmth<MaximumWarmth()/3) return(1);
}

protected func Entrance()  { CheckWarnings(); return _inherited(...); }
protected func Departure() { CheckWarnings(); return _inherited(...); }

/* Warnungen */

private func CheckWarnings(iWarmthChange) {
  var pActT;

  if(!GetAlive(this)) return(0);

  pActT=this();
  if(Contained()) pActT=Contained();

  // Aufwärmung
  if(iWarmth<9*MaximumWarmth()/10 && iWarmthChange>0)
            SetWarning(WNWM,pActT);
  else          RemWarning(WNWM);

  // Wärmewert: Warnung
  if(IsCold() && iWarmthChange<0)
            SetWarning(WNCL,pActT);
  else          RemWarning(WNCL);
}

private func GetWarning(idType) {
  var pWarn, szWarnAct=DefinitionCall(idType,"WarnAction");
  while(pWarn=FindObject(idType,0,0,0,0,0,szWarnAct,0,0,pWarn))
    if(GetActionTarget(1,pWarn)==this())
      return(pWarn);
}

private func RemWarning(idType) {
  var pWarn=GetWarning(idType);
  if(!pWarn) return(0);
  RemoveObject(pWarn);
}

private func SetWarning(idType,pAct0) {
  var pWarn=GetWarning(idType), szWarnAct=DefinitionCall(idType,"WarnAction");
  if(!pWarn) return(ObjectSetAction(CreateObject(idType),szWarnAct,pAct0,this()));
  if(GetActionTarget(0,pWarn)!=pAct0)
    SetActionTargets(pAct0,this(),pWarn);
}

private func RemoveWarnings() {
  return( RemWarning(WNHN) || RemWarning(WNCL) || RemWarning(WNWM));
}
