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
// Module: EvtChi0BB1.cc
//
// Description: Routine to decay Chi0 to B8 B8bar.
//              
// Modification history:
//
// Pang C.Y. and Ping R.-G.       April, 2007       Module created
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
#include "EvtChi0BB1.hh"
#include <string>
using std::endl;

EvtChi0BB1::~EvtChi0BB1() {}

void EvtChi0BB1::getName(std::string& model_name){

  model_name="Chi0BB1";     

}


EvtDecayBase* EvtChi0BB1::clone(){

  return new EvtChi0BB1;

}

void EvtChi0BB1::init(){
// check that there are 1 arguments
// checkNArg(1); 
 checkNDaug(2);
 checkSpinParent(EvtSpinType::SCALAR);
 checkSpinDaughter(0,EvtSpinType::DIRAC);
 checkSpinDaughter(1,EvtSpinType::DIRAC);

 }

/*
void EvtChi0BB1::initProbMax() {

  //Hard coded... should not be hard to calculate...
 setProbMax(10.0);

}      
*/

void EvtChi0BB1::decay( EvtParticle *p){

  p->initializePhaseSpace(getNDaug(),getDaugs());

  EvtParticle *v,*s1;
  EvtVector4R pv,ps,ppr;
  
  v =p->getDaug(0);  
  s1=p->getDaug(1); 
  pv=v->getP4();
  ps=s1->getP4();
  ppr=p->getP4();

//  Put phase space results into the daughters.
  EvtHelSys angles(ppr,pv); //using helicity sys. angles
  double theta =angles.getHelAng(1); 
  double phi  =angles.getHelAng(2);
  double gamma=0;

 double F00=1.0;
 double F11=F00;

 vertex(0,0,F00);
 vertex(1,1,F11);

  return ;

}




