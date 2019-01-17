#include "../include/topoana.h"
#include <cstdlib>

void topoana::getPdfFlFromTexFl()
{
  string NmOfOptTexFl=m_mainNmOfOptFls+".tex";
  string pdflatexcmd="pdflatex "+NmOfOptTexFl;
  system(pdflatexcmd.c_str());
  system(pdflatexcmd.c_str());
  system(pdflatexcmd.c_str());

  string NmOfOptAuxFl=m_mainNmOfOptFls+".aux";
  string NmOfOptLogFl=m_mainNmOfOptFls+".log";
  string NmOfOptLotFl=m_mainNmOfOptFls+".lot";
  string NmOfOptOutFl=m_mainNmOfOptFls+".out";
  string rmcmd="rm "+NmOfOptAuxFl+" "+NmOfOptLogFl+" "+NmOfOptLotFl+" "+NmOfOptOutFl;
  system(rmcmd.c_str());
}
