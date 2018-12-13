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
// Description: Routine to decay a particle  using the invariant mass distribution of histogram
//    dimension one.
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
#include "EvtGenModels/EvtHAngSam3.hh"
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

EvtHAngSam3::~EvtHAngSam3() {}

void EvtHAngSam3::getName(std::string& model_name){

  model_name="HAngSam3";     

}

EvtDecayBase* EvtHAngSam3::clone(){

  return new EvtHAngSam3;

}


void EvtHAngSam3::init(){

  // check that there are 4 arguments: Invariant mass part. Index: i,j, histor. file name, Hid 
  checkNArg(0);
  EvtSpinType::spintype parenttype = EvtPDL::getSpinType(getParentId());
}
void EvtHAngSam3::initProbMax(){

  noProbMax();

}

void EvtHAngSam3::decay( EvtParticle *p ){

 const char* fl=setFileName();
 const char* hp=setHpoint();
 int* dp;
 
 TFile f(fl);
 TH1F *hid = (TH1F*)f.Get(hp);
 TAxis* xaxis = hid->GetXaxis();

 double BLE   =xaxis->GetBinLowEdge(1);
 int    BINS  =xaxis->GetLast();
 double yvalue[20000];
 static double ymax=0.0;
 int i;
 double Ntotal=0,yc[20000];
 static int icount=0;

 if(icount==0){
 for(i=1;i<BINS+1;i++){
    yvalue[i]=hid->GetBinContent(i);
    if(yvalue[i]>ymax) ymax=yvalue[i];
 }
 icount++;
 ymax=ymax*1.2;
 }

loop:
  p->initializePhaseSpace(getNDaug(),getDaugs());

  EvtParticle *d1,*d2,*d3;
  EvtVector4R pd1,pd2,pd3,pp,nor;
  double costheta,cosphi;
 
  d1 =p->getDaug(0); 
  d2 =p->getDaug(1); 
  pd1=d1->getP4();
  pd2=d2->getP4();
  pp =p->getP4();
  double ppmag=pp.d3mag();
  nor=pd1.cross(pd2);
  
  if(ppmag<0.0001){costheta=nor.get(3)/nor.d3mag();} else
    {costheta=nor.dot(pp)/nor.d3mag()/pp.d3mag();}

  int xbin =hid->FindBin(costheta);
  double xratio=(hid->GetBinContent(xbin))/ymax;

  double rd1=EvtRandom::Flat(0.0, 1.0); 
  if(rd1>xratio) goto loop;
  return ;
}


