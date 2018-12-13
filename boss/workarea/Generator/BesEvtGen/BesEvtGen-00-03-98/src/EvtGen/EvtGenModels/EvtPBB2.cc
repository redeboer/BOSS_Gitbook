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
// Module: EvtPBB2.cc 
//
// Description: Routine to decay pseudoscalar to  B_10 bar B_10 using
//              helicity amplitude.
// 
// Modification history:
//    
//     Ping R.-G.    Apr., 2007       Module created
//
//------------------------------------------------------------------------


#include "EvtGenBase/EvtPatches.hh"
#include <stdlib.h>
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtGenKine.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtVector4C.hh"
#include "EvtGenBase/EvtVector4R.hh"
#include "EvtGenBase/EvtTensor4C.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtHelSys.hh"
#include "EvtPBB2.hh"
#include "EvtGenBase/EvtdFunctionSingle.hh"
#include "EvtGenBase/EvtRaritaSchwinger.hh"
#include "EvtGenBase/EvtRaritaSchwingerParticle.hh" 

#include <string>

using std::endl;
using std::cout;
EvtPBB2::~EvtPBB2() {}

void EvtPBB2::getName(std::string& model_name){

  model_name="PBB2";     

}


EvtDecayBase* EvtPBB2::clone(){

  return new EvtPBB2;

}

void EvtPBB2::init(){
 checkNDaug(2);
 checkSpinParent(EvtSpinType::SCALAR);
 checkSpinDaughter(0,EvtSpinType::RARITASCHWINGER);
 checkSpinDaughter(1,EvtSpinType::RARITASCHWINGER);

 }


void EvtPBB2::initProbMax() {

  //Hard coded... should not be hard to calculate...
 double rt  =getArg(0);
 double pmax=2+2*rt*rt;
 setProbMax(pmax);
}      

void EvtPBB2::decay( EvtParticle *p){


  p->initializePhaseSpace(getNDaug(),getDaugs());
  
  double rt =getArg(0);
  double phs=getArg(0);

  int ii,jj;
  for(ii=0;ii<4;ii++){ 
     for(jj=0;jj<4;jj++){ 
     vertex(ii,jj,0);
     }
  }

  vertex(0,0,                       1.);
  vertex(1,1, rt*exp(EvtComplex(0,phs)));
  vertex(2,2,-rt*exp(EvtComplex(0,phs)));
  vertex(3,3,                       -1);
  return ;
  
}




