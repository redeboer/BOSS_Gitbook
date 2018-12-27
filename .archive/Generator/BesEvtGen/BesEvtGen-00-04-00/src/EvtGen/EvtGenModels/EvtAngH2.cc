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
// Description: 
//  This model allows user to specify scatter plot id, x-axis is the angular distribution(in Lab system) for daugher(0) 
//, y-axix is the  the angular distribution(in Lab system) for daugher(1). They are corrected by detection efficiency. 
//  The scatter plot is filled with cos(theta_0) vs. cos(theta_1),where subscript 0,1 denote the daughter index    
//  if son_0 and son_1 are identitical particles, to distinguish them by E_0<E_1
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
#include "EvtGenModels/EvtAngH2.hh"
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

EvtAngH2::~EvtAngH2() {}

void EvtAngH2::getName(std::string& model_name){

  model_name="AngH2";     

}

EvtDecayBase* EvtAngH2::clone(){

  return new EvtAngH2;

}


void EvtAngH2::init(){

  // check that there are 4 arguments: Invariant mass part. Index: i,j, histor. file name, Hid 
  checkNArg(0);
  EvtSpinType::spintype parenttype = EvtPDL::getSpinType(getParentId());
}
void EvtAngH2::initProbMax(){

  noProbMax();

}

void EvtAngH2::decay( EvtParticle *p ){

 const char* fl=setFileName();
 const char* hp=setHpoint();
 int* dp;

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
  double xcostheta,ycostheta;
 
  id1 =p->getDaug(0); 
  id2 =p->getDaug(1); 
  id3 =p->getDaug(2);
  

  pd1 =id1->getP4Lab();
  pd2 =id2->getP4Lab();
  pd3 =id3->getP4Lab();

  xcostheta=pd1.get(3)/pd1.d3mag();
  ycostheta=pd2.get(3)/pd2.d3mag();
  if( EvtPDL::name(p->getDaug(0)->getId()) == EvtPDL::name(p->getDaug(1)->getId()) ){
    if(pd1.get(0)>pd2.get(0)){
    xcostheta=pd2.get(3)/pd2.d3mag();
    ycostheta=pd1.get(3)/pd1.d3mag();
    }
  }
  int xbin = hid->GetXaxis()->FindBin(xcostheta);
  int ybin = hid->GetYaxis()->FindBin(ycostheta);
  int xybin= hid->GetBin(xbin,ybin);
  double zvalue=hid->GetBinContent(xybin);
  double xratio=zvalue/ymax;
//  cout<<"***************zvalue,ymax,xratio= "<<zvalue<<"; "<<ymax<<"; "<<xratio<<endl;
  double rd1=EvtRandom::Flat(0.0, 1.0); 
  if(rd1>xratio) goto loop;
  return ;
}


