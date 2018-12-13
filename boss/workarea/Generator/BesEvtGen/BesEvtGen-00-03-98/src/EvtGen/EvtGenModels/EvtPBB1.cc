//--------------------------------------------------------------------------
//
// Environment:
//      This software is part of models developed at BES collaboration
//      based on the EvtGen framework.  If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information: See EvtGen/BesCopyright 
//      Copyright (A) 2006      Ping Rong-Gang, Pang Cai-Ying@IHEP
//
//
// Module: EvtPBB1.cc
//
// Description: Routine to decay Pseudoscalar-> B_8 bar B_8 using helicity amplitude.
//
// Modification history:
//
//    Ping R.-G.    Apr., 2006       Module created
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
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtdFunction.hh"
#include "EvtGenBase/EvtHelSys.hh"
#include "EvtPBB1.hh"
#include <string>
using std::endl;

EvtPBB1::~EvtPBB1() {}

void EvtPBB1::getName(std::string& model_name){

  model_name="PBB1";     

}


EvtDecayBase* EvtPBB1::clone(){

  return new EvtPBB1;

}

void EvtPBB1::initProbMax() {
   double ProbMax=2.;
   setProbMax(ProbMax); 
}

void EvtPBB1::init(){
 checkNDaug(2);
 checkSpinParent(EvtSpinType::SCALAR);
 checkSpinDaughter(0,EvtSpinType::DIRAC);
 checkSpinDaughter(1,EvtSpinType::DIRAC);
}

void EvtPBB1::decay( EvtParticle *p){
  
  p->initializePhaseSpace(getNDaug(),getDaugs());
  
 double phs=getArg(0); 


 vertex(0,0, exp(EvtComplex(0,phs)));
 vertex(0,1,                    0.0);
 vertex(1,0,                    0.0);
 vertex(1,1,-exp(EvtComplex(0,phs)));

  return ;

}




