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
// Description: Routine to decay a particle  using a scatter 
//    plot forn n-body decays (n>3).
//      
// Modification history:
//
//    Ping R.-G.       Oct. 2011       Module created
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
#include "EvtGenModels/EvtmH2.hh"
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

EvtmH2::~EvtmH2() {}

void EvtmH2::getName(std::string& model_name){

  model_name="mH2";     

}

EvtDecayBase* EvtmH2::clone(){

  return new EvtmH2;

}


void EvtmH2::init(){

  // check that there are 4 arguments: Invariant mass part. Index: i,j, histor. file name, Hid 
  checkNArg(2);
  EvtSpinType::spintype parenttype = EvtPDL::getSpinType(getParentId());
  nbx = getArg(0);
  nby = getArg(1);
}
void EvtmH2::initProbMax(){

  noProbMax();

}

void EvtmH2::decay( EvtParticle *p ){

 const char* fl=setFileName();
 const char* hp=setHpoint();

 TFile f(fl);
 TH2F *hid = (TH2F*)f.Get("mH2");
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

  if(p->getNDaug()!= nbx+nby) {std::cout<<"The number of specified particles is not equal the number of decay daughters "<<endl;::abort();}

  EvtVector4R pt,pt2;
  double xmass,ymass;
 
  pt=p->getDaug(0)->getP4Lab();
  for (int ii=1;ii<nbx;ii++){
    pt=pt+p->getDaug(ii)->getP4Lab();
  }
    xmass=pt.mass();

    pt2=p->getDaug(nbx)->getP4Lab();
    for(int jj=nbx+1;jj<nbx+nby;jj++) pt2=pt2+p->getDaug(jj)->getP4Lab();
    ymass=pt2.mass();


  int xbin = hid->GetXaxis()->FindBin(xmass);
  int ybin = hid->GetYaxis()->FindBin(ymass);
  int xybin= hid->GetBin(xbin,ybin);
  double zvalue=hid->GetBinContent(xybin);
  double xratio=zvalue/ymax;
//  cout<<"***************zvalue,ymax,xratio= "<<zvalue<<"; "<<ymax<<"; "<<xratio<<endl;
  double rd1=EvtRandom::Flat(0.0, 1.0); 
  if(rd1>xratio) goto loop;
  return ;
}


