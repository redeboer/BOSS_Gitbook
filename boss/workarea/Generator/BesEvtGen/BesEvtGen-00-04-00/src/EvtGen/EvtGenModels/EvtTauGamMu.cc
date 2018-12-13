//--------------------------------------------------------------------------
//
// Environment:
//      This software is part of models developed at BES collaboration
//      based on the EvtGen framework.  If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information: See EvtGen/BesCopyright
//      Copyright (A) 2014      Ping Rong-Gang
//
// Module: EvtHypWK.hh
//
// Description:  To generate decay tau->gamma mu with uses' helicity amplitudes, it can be used
//  for the decay Fermion -> photon Fermion .
//
// Modification history:
//
//    Pang C.-Y., Ping R.-G.    Mar, 2007       Module created
//    Ping R.G. Apr. 2007  change to helicity amplitude method
//------------------------------------------------------------------------
//
#include "EvtGenBase/EvtPatches.hh"
#include <stdlib.h>
#include <iostream>
#include <string>
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtGenKine.hh"
#include "EvtGenModels/EvtTauGamMu.hh"
#include "EvtGenBase/EvtDiracSpinor.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtVector4C.hh"
#include "EvtGenBase/EvtId.hh"
#include "EvtGenBase/EvtHelSys.hh"
#include "EvtGenBase/EvtdFunction.hh"

using std::cout;
using std::endl;

EvtTauGamMu::~EvtTauGamMu() {}

void EvtTauGamMu::getName(std::string& model_name){

  model_name="TauGamMu";     

}



EvtDecayBase* EvtTauGamMu::clone(){

  return new EvtTauGamMu;

}

void EvtTauGamMu::init(){

  // check that there are 0 arguments
  checkNArg(4);
  checkNDaug(2);


   checkSpinParent(EvtSpinType::DIRAC);
  
   checkSpinDaughter(0,EvtSpinType::PHOTON);
   checkSpinDaughter(1,EvtSpinType::DIRAC);


  for(int i=0;i<=1;i++){
    for(int j=0;j<=2;j++){
      for(int k=0;k<=1;k++){
	vertex(i,j,k,0);
      }
    }
  }

  //---
}


void EvtTauGamMu::decay(EvtParticle *p){


  p->initializePhaseSpace(getNDaug(),getDaugs());

  EvtParticle *l1, *l2;
  l1 = p->getDaug(0);
  l2 = p->getDaug(1);
  EvtVector4R DRP4=l1->getP4(); //photon
  EvtVector4R MEP4=l2->getP4(); //Mu
  EvtVector4R PRP4=p->getP4();  //Parent Momentum
  EvtHelSys angles(PRP4,DRP4);   //using helicity sys.angles
  double tht=angles.getHelAng(1);
  double ph =angles.getHelAng(2);
  double gmm=0;

  double H00=getArg(0); //H(1,1/2) for photon , mu
  double A00=getArg(1); //phase angle for H00
  double H11=getArg(2); //H(-1,-1/2) 
  double A11=getArg(3); //phase angle H11

  EvtComplex A000 = Djmn(0.5, 0.5, 0.5,ph,tht,gmm)*H00*exp(A00); //for 1/2 -> 1 1/2
  EvtComplex A100 = Djmn(0.5,-0.5, 0.5,ph,tht,gmm)*H00*exp(A00); //for -1/2 -> 1 1/2 
  EvtComplex A021 = Djmn(0.5, 0.5,-0.5,ph,tht,gmm)*H11*exp(A11);  // 1/2->-1,-1/2
  EvtComplex A121 = Djmn(0.5,-0.5,-0.5,ph,tht,gmm)*H11*exp(A11);  // -1/2->-1,-1/2

 
  vertex(0,0,0,A000);
  vertex(0,1,1,A021);
  vertex(1,0,0,A100);
  vertex(1,1,1,A121);

  return;

}



