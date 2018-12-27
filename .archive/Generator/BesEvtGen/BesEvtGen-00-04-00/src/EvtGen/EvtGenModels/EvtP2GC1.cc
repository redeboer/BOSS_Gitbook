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
// Module: EvtP2GC1.cc
//
// Description: Routine to decay psi(2S) to gamma chi_c1 by assuming E1 dominant
//              
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
#include "EvtP2GC1.hh"
#include "EvtGenBase/EvtdFunctionSingle.hh"
#include <string>
using std::endl;

EvtP2GC1::~EvtP2GC1() {}

void EvtP2GC1::getName(std::string& model_name){

  model_name="P2GC1";     

}


EvtDecayBase* EvtP2GC1::clone(){

  return new EvtP2GC1;

}

void EvtP2GC1::init(){
static EvtId GAM=EvtPDL::getId("gamma");
// check that there are 0 arguments
 checkNArg(0); 
 checkNDaug(2);
 checkSpinParent(EvtSpinType::VECTOR);
 checkSpinDaughter(0,EvtSpinType::PHOTON);
 checkSpinDaughter(1,EvtSpinType::VECTOR);
 if (!(getDaug(0)==GAM)) {
 report(ERROR,"EvtGen") << "EvtP2GC1 generator radiative decays expected "<<endl;
 ::abort();
}
 }


void EvtP2GC1::initProbMax() {

  //Hard coded... should not be hard to calculate...
 setProbMax(1.67);

}      

void EvtP2GC1::decay( EvtParticle *p){

  p->initializePhaseSpace(getNDaug(),getDaugs());

  EvtParticle *v,*s1;
  EvtVector4R pv,ps,ppr;

  v =p->getDaug(0);  //gamma
  s1=p->getDaug(1); //chi_c1
  pv=v->getP4();
  ps=s1->getP4();
  ppr=p->getP4();

//  Put phase space results into the daughters.
  EvtHelSys angles(ppr,pv); //using helicity sys. angles
  double theta =angles.getHelAng(1);
  double phi   =angles.getHelAng(2);
  double gamma =0;
  double alpha=-1./3.;
  double A02=sqrt((1+alpha)/2);
  double A00=sqrt((1-alpha)/4);
  double A12=-A02,A11=-A00;
 int j1,j2,j3;
 for(j1=0;j1<=2;){
 for(j2=0;j2<=1;){
 for(j3=0;j3<=2;){
 vertex(j1,j2,j3,0.0);j3++;}
j2++;}
j1++;}

 vertex(0,1,2,Djmn(1, 1,-1,phi,theta,gamma)*A12); //helicity for psiprime = 1
 vertex(0,1,1,Djmn(1, 1, 0,phi,theta,gamma)*A11);
 vertex(0,0,2,Djmn(1, 1, 1,phi,theta,gamma)*A02); 
 vertex(0,0,0,Djmn(1, 1, 0,phi,theta,gamma)*A00); 

 vertex(1,1,2,Djmn(1,-1,-1,phi,theta,gamma)*A12);  //helicity for psiprime = -1
 vertex(1,1,1,Djmn(1,-1, 0,phi,theta,gamma)*A11);
 vertex(1,0,2,Djmn(1,-1, 1,phi,theta,gamma)*A02);
 vertex(1,0,0,Djmn(1,-1, 0,phi,theta,gamma)*A00);

 vertex(2,1,2,Djmn(1, 0,-1,phi,theta,gamma)*A12);  //helicity for psiprime = -1
 vertex(2,1,1,Djmn(1, 0, 0,phi,theta,gamma)*A11);
 vertex(2,0,2,Djmn(1, 0, 1,phi,theta,gamma)*A02);
 vertex(2,0,0,Djmn(1, 0, 0,phi,theta,gamma)*A00);


 return ;

}




