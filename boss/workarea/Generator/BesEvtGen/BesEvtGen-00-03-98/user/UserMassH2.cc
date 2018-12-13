#include "TH1.h"
#include "TAxis.h"
#include "TH2.h"
#include "TFile.h"
#include "TApplication.h"
#include "TROOT.h"

#include "EvtGenModels/EvtMassH2.hh"
//////////////////***** MassH2 Model ------------------
const char*  EvtMassH2::setFileName(){
  const char* filename;
  filename="diy.root";  //specify the root histor. name
  return filename;
}
  
const char*  EvtMassH2::setHpoint(){
  const char* hpoint;
  hpoint="hdalitz";    //specify the histor. id
  return hpoint;
} 

int* EvtMassH2::setDaugPair(){
  static int DP[4];
  DP[0]=0;  // 0,1,2,... indexes for daughter particles
  DP[1]=1;
  DP[2]=0;
  DP[3]=2;
  return DP;
}

