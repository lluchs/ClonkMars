/*--- Erde ---*/

#strict 2

protected func Initialize() {
	SetGraphics("Grey", this);
}

protected func Hit()
{
  CastPXS("Earth", 100, 20);
  RemoveObject();
  return(1);
}

func IsAlchemContainer() { return(true); }
func AlchemProcessTime() { return(120); }