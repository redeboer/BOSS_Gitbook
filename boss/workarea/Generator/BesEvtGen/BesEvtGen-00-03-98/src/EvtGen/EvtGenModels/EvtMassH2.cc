//--------------------------------------------------------------------------
//
// Environment:
//      This software is part of models developed at BES collaboration
//      based on the EvtGen framework.  If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information: See EvtGen/BesCopyright
//      Copyright (A) 2006      Ping Rong-Gang @IHEP
//
// Module: EvtMassH1.cc
//
// Description: Routine to decay a particle  using a Dalitz 
//    plot.
//      
// Modification history:
//
//    Ping R.-G.       December, 2006       Module created
//
//------------------------------------------------------------------------
//
#include "EvtGenBase/EvtPatches.hh"
#include <stdlib.h>
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtGenKine.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtVector4C.hh"
#include "EvtGenBase/EvtVector4R.hh"
#include "EvtGenBase/EvtTensor4C.hh"
#include "EvtGenBase/EvtDiracParticle.hh"
#include "EvtGenBase/EvtScalarParticle.hh"
#include "EvtGenBase/EvtVectorParticle.hh"
#include "EvtGenBase/EvtTensorParticle.hh"
#include "EvtGenBase/EvtPhotonParticle.hh" 
#include "EvtGenBase/EvtNeutrinoParticle.hh"
#include "EvtGenBase/EvtStringParticle.hh"
#include "EvtGenBase/EvtRaritaSchwingerParticle.hh"
#include "EvtGenBase/EvtHighSpinParticle.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtHelSys.hh"
#include "EvtGenModels/EvtMassH2.hh"
#include "EvtGenBase/EvtRandom.hh"
#include <string>

#include "TH1.h"
#include "TAxis.h"
#include "TH2.h"
#include "TFile.h"  
#include "TApplication.h"
#include "TROOT.h"
//#include "CLHEP/config/CLHEP.h"
//#include "CLHEP/config/TemplateFunctions.h"


using std::endl;

EvtMassH2::~EvtMassH2() {}

void EvtMassH2::getName(std::string& model_name){

  model_name="MassH2";     

}

EvtDecayBase* EvtMassH2::clone(){

  return new EvtMassH2;

}


void EvtMassH2::init(){

  // check that there are 4 arguments: Invariant mass part. Index: i,j, histor. file name, Hid 
  checkNArg(0);
  EvtSpinType::spintype parenttype = EvtPDL::getSpinType(getParentId());
}
void EvtMassH2::initProbMax(){

  noProbMax();

}

void EvtMassH2::decay( EvtParticle *p ){

 const char* fl=setFileName();
 const char* hp=setHpoint();
 int* dp;
 dp=setDaugPair();
 int d1=dp[0];  //m(d1,d2) pair at X axis
 int d2=dp[1];
 int d3=dp[2];  //m(d3,d4) pair at Y axis
 int d4=dp[3];

 TFile f(fl);
 TH2F *hid = (TH2F*)f.Get(hp);
 TAxis* xaxis = hid->GetXaxis();
 TAxis* yaxis = hid->GetYaxis();

 int    BINSx  =xaxis->GetLast();
 int    BINSy  =yaxis->GetLast();
 int    BINS   =BINSx*BINSy;
 double yvalue,ymax=0.0;
 int i,j,binxy;

 for(i=1;i<BINSx+1;i++){
     for(j=1;j<BINSy+1;j++){
      binxy=hid->GetBin(i,j);
      yvalue=hid->GetBinContent(binxy);
//      cout<<"binxy,yvalue = "<<binxy<<"; "<<yvalue<<endl;
      if(yvalue>ymax) ymax=yvalue;
     }
 }

loop:
  p->initializePhaseSpace(getNDaug(),getDaugs());

  EvtParticle *id1,*id2,*id3,*id4,*s1;
  EvtVector4R pd1,pd2,pd3,pd4,ps;
  double xmass2,ymass2;
 
  id1 =p->getDaug(d1); 
  id2 =p->getDaug(d2); 
  id3 =p->getDaug(d3);
  id4 =p->getDaug(d4);

  pd1 =id1->getP4Lab();
  pd2 =id2->getP4Lab();
  pd3 =id3->getP4Lab();
  pd4 =id4->getP4Lab();

  xmass2=(pd1+pd2).mass2();
  ymass2=(pd3+pd4).mass2();

  int xbin = hid->GetXaxis()->FindBin(xmass2);
  int ybin = hid->GetYaxis()->FindBin(ymass2);
  int xybin= hid->GetBin(xbin,ybin);
  double zvalue=hid->GetBinContent(xybin);
  double xratio=zvalue/ymax;
//  cout<<"***************zvalue,ymax,xratio= "<<zvalue<<"; "<<ymax<<"; "<<xratio<<endl;
  double rd1=EvtRandom::Flat(0.0, 1.0); 
  if(rd1>xratio) goto loop;
  return ;
}


