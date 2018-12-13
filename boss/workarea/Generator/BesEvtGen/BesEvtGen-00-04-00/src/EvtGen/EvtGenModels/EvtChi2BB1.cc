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
// Module: EvtChi2BB1.cc
//
// Description: Routine to decay Chi2 to B8 B8bar.
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
#include "EvtChi2BB1.hh"
#include <string>
using std::endl;

EvtChi2BB1::~EvtChi2BB1() {}

void EvtChi2BB1::getName(std::string& model_name){

  model_name="Chi2BB1";     

}


EvtDecayBase* EvtChi2BB1::clone(){

  return new EvtChi2BB1;

}

void EvtChi2BB1::init(){
// check that there are 0 arguments
 checkNArg(2); 
 checkNDaug(2);
 checkSpinParent(EvtSpinType::TENSOR);
 checkSpinDaughter(0,EvtSpinType::DIRAC);
 checkSpinDaughter(1,EvtSpinType::DIRAC);

 }

/*
void EvtChi2BB1::initProbMax() {

  //Hard coded... should not be hard to calculate...
 setProbMax(100.0);

}      
*/

void EvtChi2BB1::decay( EvtParticle *p){

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

  double F01=getArg(0);
  double F00=getArg(1); 
  double F10=F01;
  double F11=F00;

 vertex(0,0,0,Djmn(2, 0, 0,phi,theta,gamma)*F00);//Chi1  helicity =0 corresponding index=0
 vertex(0,0,1,Djmn(2, 0, 1,phi,theta,gamma)*F01);
 vertex(0,1,0,Djmn(2, 0,-1,phi,theta,gamma)*F10);
 vertex(0,1,1,Djmn(2, 0, 0,phi,theta,gamma)*F11);

 vertex(1,0,0,Djmn(2, 2, 0,phi,theta,gamma)*F00);//Chi1  helicity =2 corresponding index=1
 vertex(1,0,1,Djmn(2, 2, 1,phi,theta,gamma)*F01);
 vertex(1,1,0,Djmn(2, 2,-1,phi,theta,gamma)*F10);
 vertex(1,1,1,Djmn(2, 2, 0,phi,theta,gamma)*F11);

 vertex(2,0,0,Djmn(2,-2, 0,phi,theta,gamma)*F00);//Chi1  helicity =-2 corresponding index=2
 vertex(2,0,1,Djmn(2,-2, 1,phi,theta,gamma)*F01);
 vertex(2,1,0,Djmn(2,-2,-1,phi,theta,gamma)*F10);
 vertex(2,1,1,Djmn(2,-2, 0,phi,theta,gamma)*F11);

 vertex(3,0,0,Djmn(2, 1, 0,phi,theta,gamma)*F00);//Chi1  helicity =1 corresponding index=3
 vertex(3,0,1,Djmn(2, 1, 1,phi,theta,gamma)*F01);
 vertex(3,1,0,Djmn(2, 1,-1,phi,theta,gamma)*F10);
 vertex(3,1,1,Djmn(2, 1, 0,phi,theta,gamma)*F11);

 vertex(4,0,0,Djmn(2,-1, 0,phi,theta,gamma)*F00);//Chi1  helicity =-1 corresponding index=4
 vertex(4,0,1,Djmn(2,-1, 1,phi,theta,gamma)*F01);
 vertex(4,1,0,Djmn(2,-1,-1,phi,theta,gamma)*F10);
 vertex(4,1,1,Djmn(2,-1, 0,phi,theta,gamma)*F11);

  return ;

}




