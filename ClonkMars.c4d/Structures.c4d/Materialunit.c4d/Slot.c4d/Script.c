/*-- Vorsicht vor dem Chippie --*/

#strict

public func Content(){
  //Raus aus dem Hilfsobjekt
  Exit(Contents(),0,0);
  //Rein in das Hauptgeb�ude
  Enter(FindObject(UNIT,-10,-50,100,100));
  }
