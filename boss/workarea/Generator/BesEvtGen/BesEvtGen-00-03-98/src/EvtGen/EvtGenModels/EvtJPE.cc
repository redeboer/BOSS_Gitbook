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
// Module: EvtJPE.cc
//
// Description: Routine to decay Vector->photon +pseudoscalar, e.g. J/psi to gamma eta or eta_c
//
// Modification history:
//
//    Ping R.-G.       Apr., 2007       Module created
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
#include "EvtJPE.hh"
#include <string>
using std::endl;

EvtJPE::~EvtJPE() {}

void EvtJPE::getName(std::string& model_name){

  model_name="JPE";     

}


EvtDecayBase* EvtJPE::clone(){

  return new EvtJPE;

}

void EvtJPE::init(){
static EvtId GAM=EvtPDL::getId("gamma");
// check that there are 0 arguments
 checkNArg(0); 
 checkNDaug(2);
 checkSpinParent(EvtSpinType::VECTOR);
 checkSpinDaughter(0,EvtSpinType::PHOTON);
 checkSpinDaughter(1,EvtSpinType::SCALAR);
 if (!(getDaug(0)==GAM)) {
 report(ERROR,"EvtGen") << "EvtJPE generator radiative decays expected "<<endl;
 ::abort();
}
 }


/*
void EvtJPE::initProbMax() {

//Hard coded... should not be hard to calculate...
setProbMax(1.0);

}      
*/

void EvtJPE::decay( EvtParticle *p){

  p->initializePhaseSpace(getNDaug(),getDaugs());

  EvtParticle *v,*s1;
  EvtVector4R pv,ps,ppr;
  
  v =p->getDaug(0);  //gamma
  s1=p->getDaug(1); //eta_c
  pv=v->getP4();
  ps=s1->getP4();
  ppr=p->getP4();
  EvtVector4R pvcm=v->getP4();
  
//  Put phase space results into the daughters.
  EvtHelSys angles(ppr,pv); //using helicity sys. angles
  double theta =angles.getHelAng(1); 
  double phi  =angles.getHelAng(2);
  double gamma=0;

  double mass_M = EvtPDL::getMass(getParentId());
  double Mpsip=fabs(mass_M-3.686);
  double Mpsipp=fabs(mass_M-3.771);
  double FP=pvcm.d3mag(); //for J/psi and psi(3770) decays, M1 transition  assumed with form factor =1. P-wave gives the p_gamma factor
  if(Mpsip<0.1) {FP=FP*FP;} else{FP=1;} //for psi(2S) decays, M1 transition assumed with form factor=k_gamma^2


  double FM=-FP;
 vertex(0,0,Djmn(1, 1, 1,phi,theta,gamma)*FP);  // Jpsi helicity =1 corresponding index=0
 vertex(0,1,Djmn(1, 1,-1,phi,theta,gamma)*FM);
 vertex(1,0,Djmn(1,-1, 1,phi,theta,gamma)*FP); // Jpsi helicity =-1 corresponding index=1 
 vertex(1,1,Djmn(1,-1,-1,phi,theta,gamma)*FM);
 vertex(2,0,Djmn(1, 0, 1,phi,theta,gamma)*FP); // Jpsi helicity =0 corresponding index=2
 vertex(2,1,Djmn(1, 0,-1,phi,theta,gamma)*FM);


  return ;

}




