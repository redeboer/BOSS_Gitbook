#include "TH1.h"
#include "TAxis.h"
#include "TH2.h"
#include "TFile.h"
#include "TApplication.h"
#include "TROOT.h"

#include "EvtGenModels/EvtMassH1.hh"

////////////////*****  MassH1 class defined by user
const char*  EvtMassH1::setFileName(){
  const char* filename;
  filename="massh1.root";
//specify the root histor. name
  return filename;  
} 
  
const char*  EvtMassH1::setHpoint(){
  const char* hpoint;
  hpoint="xmass12";    //specify the histor. id
  return hpoint;
}

int* EvtMassH1::setDaugPair(){
  static int DP[2];
  DP[0]=0;  // 0,1,2,... indexes for daughter particles
  DP[1]=1;
  return DP;
}

