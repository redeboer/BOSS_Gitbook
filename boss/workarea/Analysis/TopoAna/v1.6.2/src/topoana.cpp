#include "../include/topoana.h"

int main(int argc,char *argv[])
{
  topoana ta;
  ta.makeMapsOnPdata();
  if(argc>1) ta.readCard(argv[1]);
  else ta.readCard("topoana.card");      
  ta.checkInput();
  ta.getRslt();
  ta.writeRsltIntoTxtFl();
  ta.writeRsltIntoTexFl();
  ta.getPdfFlFromTexFl();
  ta.writeInfOnRslt();
  return 0;
}
