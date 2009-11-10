/*-- ErrorMessages --*/

#strict 2

// gibt dem Spieler Messages aus 

global func Notify(string sText, object obj){
  if(!obj){
    obj=this;
    }
  if(!obj)
  	Message(sText,obj);
  PlayerMessage(GetController(obj),sText,obj);
}


/* Dokumentation 

Notify steht für den ProduktLog. 

  ItemLog(KBKT,0,2); 
  
-> Bedeutet in diesem Falle, dass zwei Wassereimer für den Ersteller  geloggt wird.
Anmerkung: [ID], [Player], [Count]   !!! allerdings ist [Player] immer +1
=> 0 ist nicht Spieler 1 (so wie in C4Script generell, sondern 1 ist wirklich 1 (korrekterweise)).
(0 einfach stehen lassen i.d.R.) 

*/


