#include "TH1.h"
#include "TAxis.h"
#include "TH2.h"
#include "TFile.h"
#include "TApplication.h"
#include "TROOT.h"

#include "EvtGenModels/EvtBody3.hh"

//////////////////*****  Body3 Model
const char*  EvtBody3::setFileName(){
  const char* filename;
  filename="./diy.root";  //specify the root histor. name
  return filename;
}

const char*  EvtBody3::setHpoint(){
  const char* hpoint;
  hpoint="hdalitz";    //specify the histor. id
  return hpoint;
}

const char* EvtBody3::setDaugAng(int i){

const char*  DaugAng0="costhetag";  //Daugher numbering defined at DEC file
const char*  DaugAng1="costhetav";
const char*  DaugAng2="xxx";  //not use, then set PN=2 in setDaugAngNo();
  if(i==0)  return DaugAng0;
  if(i==1)  return DaugAng1;
  if(i==2)  return DaugAng2;
}

int EvtBody3::setDaugAngNo(){
 int PN=2;  //e.g. DaugAng2="xxx", set PN=2;  DaugAng0="xxx", set PN=0;
 return PN;
}
 
int* EvtBody3::setDaugPair(){
  static int DP[4];
  DP[0]=0;  // 0,1,2,... indexes for daughter particles
  DP[1]=2;
  DP[2]=1;
  DP[3]=2;
  return DP;
}


