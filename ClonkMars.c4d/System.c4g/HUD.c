#strict 2
#appendto SCNK

public func Recruitment(iPlr)
{
  if(!FindObject2(Find_Owner(iPlr), Find_ID(MHUD)))
    CreateObject(MHUD,-120,60, iPlr);
  return _inherited();
}
