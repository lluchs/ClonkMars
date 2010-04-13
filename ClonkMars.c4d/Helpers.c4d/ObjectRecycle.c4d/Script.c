/*-- Recycling für Objekte (C4D_Object) --*/

#strict 2

protected func ContextRecycle(object pClonk) {
	[$TxtRecycle$|Image=CRYC]
	pClonk -> SetAction("Recycle", this);
	return 1;
}

protected func Recycle(){
	Exit();
	Split2Components();
}
