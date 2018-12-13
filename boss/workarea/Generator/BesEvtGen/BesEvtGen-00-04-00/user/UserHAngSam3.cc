#include "TH1.h"
#include "TAxis.h"
#include "TH2.h"
#include "TFile.h"
#include "TApplication.h"
#include "TROOT.h"

#include "EvtGenModels/EvtHAngSam3.hh"

////////////////*****  HAngSam3 class defined by user
const char*  EvtHAngSam3::setFileName(){
  const char* filename;
  filename="HAngSam3.root";
//specify the root histor. name
  return filename;  
} 
  
const char*  EvtHAngSam3::setHpoint(){
  const char* hpoint;
  hpoint="costheta";    //specify the histor. id
  return hpoint;
}

 


