/*--- Kristall ---*/

#strict

protected func Hit()
{
  Sound("Crystal*");
  return(1);
}

/* Verkauf entsprechend Groesse */

protected func CalcValue()
{
  return( (GetCon() - 50) / 10 + (5 - (GetCon() / 20)) );
}

func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(150); }
