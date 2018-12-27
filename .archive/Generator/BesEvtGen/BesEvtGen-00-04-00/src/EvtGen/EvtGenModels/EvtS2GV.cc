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
// Module:  EvtS2GV.cc
//
// Description: Routine to decay chi_c1-> gamma J/psi  using 
//    helicity amplitude, assuming E1 dominance.             
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
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtHelSys.hh"
#include "EvtGenModels/EvtS2GV.hh"
#include "EvtGenBase/EvtRandom.hh"
#include <string>
using std::endl;

EvtS2GV::~EvtS2GV() {}

void EvtS2GV::getName(std::string& model_name){

  model_name="S2GV";     

}


EvtDecayBase* EvtS2GV::clone(){

  return new EvtS2GV;

}

void EvtS2GV::init(){
static EvtId GAM=EvtPDL::getId("gamma");
// check that there are 0 arguments
 checkNArg(0); 
 checkNDaug(2);
 checkSpinParent(EvtSpinType::SCALAR);
 checkSpinDaughter(0,EvtSpinType::PHOTON);
 checkSpinDaughter(1,EvtSpinType::VECTOR);
 if (!(getDaug(0)==GAM)) {
 report(ERROR,"EvtGen") << "EvtS2GV generator radiative decays expected "<<endl;
 ::abort();
}
 }


void EvtS2GV::initProbMax() {

//Hard coded... should not be hard to calculate...
setProbMax(2.0001);
}      

void EvtS2GV::decay( EvtParticle *p){
  p->initializePhaseSpace(getNDaug(),getDaugs());
 
 double phs=0.;
 EvtComplex F11=exp(EvtComplex(0,phs));

 vertex(0,0, F11);   
 vertex(0,2,   0);  
 vertex(0,1,   0);  
 vertex(1,0,   0);
 vertex(1,2,   0);
 vertex(1,1,-F11);  
  return ;

}




