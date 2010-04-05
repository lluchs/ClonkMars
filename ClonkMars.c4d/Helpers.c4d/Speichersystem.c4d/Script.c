/*-- Speichersystem --*/

#strict 2

protected func Construction() {
	if(GetID() == SAVS) {
		DebugLog("ERROR: Dieses Objekt darf nicht erstellt werden");
		RemoveObject();
	}
	hFillLevel = CreateHash();
	UpdatePicture();
	return _inherited(...);
}

// F�llstand
local hFillLevel; // h wie Hash :x

private func TypeCheck() {
	if(GetType(hFillLevel) != C4V_Array)
		hFillLevel = CreateHash();
}
/* F�llung */

// Aktuelle F�llung
public func & GetFill(Key) { TypeCheck(); return HashGet(hFillLevel, Key); }

// Maximale F�llung
public func GetMaxFill(Key) {
	var szFunc = "MaxFill", szFunc2, iFill;
	if(Key) {
		if(GetType(Key) == C4V_String)
			szFunc2 = Format("%s_%s", szFunc, Key);
		else
			szFunc2 = Format("%s_%v", szFunc, Key);
	}
	if(!(szFunc2 && (iFill = Call(szFunc2))))
		iFill = Call(szFunc);
	
	return iFill;
}

// F�llung erh�hen/verringern
public func DoFill(int iChange, Key, bool fNoSound)
{
  var iNewFill = BoundBy(GetFill(Key) + iChange, 0, GetMaxFill(Key));
  if (iNewFill == GetFill(Key)) return;
  iChange = iNewFill - GetFill(Key);
  OnFillChange(Key, iChange);
  if(!fNoSound)
  	FillSound(Key, iChange);
  HashPut(hFillLevel, Key, iNewFill);
  UpdatePicture();
  // Tats�chliche �nderung des F�llstandes zur�ckgeben
  return iChange;
}

// Voll?
public func IsFull(Key) 
{ 
  return GetFill(Key) == GetMaxFill(Key);
}

/* Grafik anpassen */
public func UpdatePicture()
{
	var Key = FillPicture();
	if(Key == -1)
		return;
	if(GetFill(Key)>99)
  {
    SetGraphics(0,0,GetNumberID(GetFill(Key) / 100),1,GFXOV_MODE_Picture);    
    SetObjDrawTransform(400,0,-14000,0,400,+10000, this, 1);
  }
  else SetGraphics(0,0,0,1,0);
  
  if(GetFill(Key)>9)
  {
    SetGraphics(0,0,GetNumberID(GetFill(Key) / 10 - GetFill(Key) / 100 * 10),2,GFXOV_MODE_Picture);    
    SetObjDrawTransform(400,0,-7000,0,400,+10000, this, 2);
  }
  else SetGraphics(0,0,0,2,0);  

  SetGraphics(0,0,GetNumberID(GetFill(Key) % 10),3,GFXOV_MODE_Picture);   
  SetObjDrawTransform(400,0,0,0,400,+10000, this, 3);
  return 1;
}

/* ID des passenden Zahlobjektes ausgeben */
private func GetNumberID(i)
{
  return(C4Id(Format("SNB%d", i)));
}

/* Bei Bedarf �berladen */

// F�llstand als Men�grafik? Wenn ja, Key des gew�nschten F�llstandes zur�ckgeben
private func FillPicture() { return -1; }

// Sound beim Ver�ndern des F�llstandes? (Wenn ja: komplettes Sound() muss in der Funktion stattfinden)
private func FillSound(Key, int iChange) { return; }

// Wird bei F�llstandsver�nderung aufgerufen
private func OnFillChange(Key, int iChange) { return; }
