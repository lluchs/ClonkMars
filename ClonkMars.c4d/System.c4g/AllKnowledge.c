/*-- Alle Baupl�ne --*/

#strict 2

global func AllKnowledge(iPlayer)
{
  //Geb�ude
  SetPlrKnowledge(iPlayer, BASE); //Basis
  SetPlrKnowledge(iPlayer, MTLB); //Materiallab
  SetPlrKnowledge(iPlayer, SOLA); //Paneel
  SetPlrKnowledge(iPlayer, ACCU); //Akku
  SetPlrKnowledge(iPlayer, RIG_); //Bohrturm
  SetPlrKnowledge(iPlayer, OILT); //�ltank
  SetPlrKnowledge(iPlayer, PORT); //Kapselport
  SetPlrKnowledge(iPlayer, TRFM); //Terraformer
  SetPlrKnowledge(iPlayer, TRNS); //Transmitter
  SetPlrKnowledge(iPlayer, SPTR); //Splitter
  SetPlrKnowledge(iPlayer, WEPB); //Waffenbasis
  SetPlrKnowledge(iPlayer, LABR); //Labor
  SetPlrKnowledge(iPlayer, RADO); //Radioteleskop
  return(1);
}
