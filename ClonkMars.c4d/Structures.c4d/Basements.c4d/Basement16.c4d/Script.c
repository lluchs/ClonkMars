/*-- Fundament --*/

#strict 2

/* Alle Fundamente leiten ihre Grundfunktionalität von BAS7 ab */

#include BAS7

private func BasementID() { return(B_16); }
private func BasementWidth() { return(16); }

/* Verschiebt die SolidMask bei Rotation */
public func SetR(int r) {
	var ID = GetID();
	if (r != 0 && ID == BasementID()) {
		var wdt = GetDefWidth(ID), hgt = GetDefHeight(ID);
		var ssgn = -1, csgn = 1;
		if (Abs(r) > 90) {
			ssgn *= -1;
			csgn *= -1;
		}
		SetSolidMask(0, 0, wdt, hgt, ssgn*Sin(r, hgt / 2), csgn*Cos(r, hgt / 2));
	}
	return inherited(r, ...);
}

