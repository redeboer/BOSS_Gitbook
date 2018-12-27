#include "TH1.h"
#include "TAxis.h"
#include "TH2.h"
#include "TFile.h"
#include "TApplication.h"
#include "TROOT.h"

#include "EvtGenModels/EvtAngH2.hh"
//////////////////***** AngH2 Model ------------------
const char*  EvtAngH2::setFileName(){
  const char* filename;
  filename="AngH2.root";  //specify the root histor. name
  return filename;
}
  
const char*  EvtAngH2::setHpoint(){
  const char* hpoint;
//specify scatter plot id, x-axis is the angular distribution(in Lab system) for daugher(0) 
//, y-axix is the  the angular distribution(in Lab system) for daugher(1). They are corrected by detection efficiency 
  hpoint="AngScatter";   
  return hpoint;
} 


