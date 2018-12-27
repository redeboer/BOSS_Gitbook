#include "TH1.h"
#include "TAxis.h"
#include "TH2.h"
#include "TFile.h"
#include "TApplication.h"
#include "TROOT.h"

#include "EvtGenModels/EvtmH2.hh"
//////////////////***** mH2 Model ------------------
const char*  EvtmH2::setFileName(){
  const char* filename;
  filename="mH2.root";  //specify the root histor. name
  return filename;
}
  
const char*  EvtmH2::setHpoint(){
  const char* hpoint;
  hpoint="mH2";    //specify the histor. id
  return hpoint;
} 



