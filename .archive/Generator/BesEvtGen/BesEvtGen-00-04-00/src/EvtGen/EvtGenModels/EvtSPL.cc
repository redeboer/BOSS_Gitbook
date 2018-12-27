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
// Module: EvtSPL.cc
//
// Description: Routine to decay Sigma0->photon+Lambda by helicity amplitude.
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
#include "EvtSPL.hh"
#include <string>
using std::endl;

EvtSPL::~EvtSPL() {}

void EvtSPL::getName(std::string& model_name){

  model_name="SPL";     

}


EvtDecayBase* EvtSPL::clone(){

  return new EvtSPL;

}

void EvtSPL::initProbMax() {
   double ProbMax=4.;
   setProbMax(ProbMax); 
}

void EvtSPL::init(){
 checkNDaug(2);
 checkSpinParent(EvtSpinType::DIRAC);
 checkSpinDaughter(0,EvtSpinType::PHOTON);
 checkSpinDaughter(1,EvtSpinType::DIRAC);
}

void EvtSPL::decay( EvtParticle *p){
  
  p->initializePhaseSpace(getNDaug(),getDaugs());
  EvtParticle *v,*s1;
  EvtVector4R pv,ps,ppr;

  v =p->getDaug(0); //photon 
  s1=p->getDaug(1); //Lambda
  pv=v->getP4Lab();
  ps=s1->getP4Lab();
  ppr=p->getP4();

//  Put phase space results into the daughters.
  
  EvtHelSys angles(ppr,pv);   //using helicity sys.angles
  double theta=angles.getHelAng(1);
  double phi  =angles.getHelAng(2);
  double gamma=0;  


 vertex(0,0,0, Djmn(0.5, 0.5, 0.5,phi,theta,gamma));
 vertex(0,1,1,-Djmn(0.5, 0.5,-0.5,phi,theta,gamma));
 vertex(1,0,0, Djmn(0.5,-0.5, 0.5,phi,theta,gamma));
 vertex(1,1,1,-Djmn(0.5,-0.5,-0.5,phi,theta,gamma));           
 vertex(0,0,1,                       0.0);
 vertex(0,1,0,                       0.0);
 vertex(1,0,1,                       0.0);
 vertex(1,1,0,                       0.0);
  return ;

}




