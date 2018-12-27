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
// Module:  EvtAV2GV.cc
//
// Description: Routine to decay chi_c1-> gamma J/psi  using 
//    helicity amplitude, assuming E1 dominance. see prd13,1203             
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
#include "EvtGenModels/EvtAV2GV.hh"
#include "EvtGenBase/EvtRandom.hh"
#include <string>
#include "EvtGenBase/EvtHelSys.hh"

using std::endl;

EvtAV2GV::~EvtAV2GV() {}

void EvtAV2GV::getName(std::string& model_name){

  model_name="AV2GV";     

}


EvtDecayBase* EvtAV2GV::clone(){

  return new EvtAV2GV;

}

void EvtAV2GV::init(){
static EvtId GAM=EvtPDL::getId("gamma");
// check that there are 0 arguments
 checkNArg(0); 
 checkNDaug(2);
 checkSpinParent(EvtSpinType::VECTOR);
 checkSpinDaughter(0,EvtSpinType::PHOTON);
 checkSpinDaughter(1,EvtSpinType::VECTOR);
 if (!(getDaug(0)==GAM)) {
 report(ERROR,"EvtGen") << "EvtAV2GV generator radiative decays expected "<<endl;
 ::abort();
}
 }


void EvtAV2GV::initProbMax() {

//Hard coded... should not be hard to calculate...
setProbMax(12.00001);
}      

void EvtAV2GV::decay( EvtParticle *p){
  p->initializePhaseSpace(getNDaug(),getDaugs());

  EvtParticle *v,*s1;
  EvtVector4R pv,ps,ppr;

  v =p->getDaug(0);  //gamma
  s1=p->getDaug(1); //J/psi
  pv=v->getP4();
  ps=s1->getP4();
  ppr=p->getP4();
//  Put phase space results into the daughters.
  EvtHelSys angles(ps,pv); //using helicity sys. angles


  double theta =angles.getHelAng(1);
  double phi   =angles.getHelAng(2);
  double gamma=0;

 int j1,j2,j3;
 for(j1=0;j1<=2;){
 for(j2=0;j2<=1;){
 for(j3=0;j3<=2;){
 vertex(j1,j2,j3,0.0);j3++;}
j2++;}
j1++;}

 vertex(0,0,0, Djmn(1, 1, 0,phi,theta,gamma));   //0 for chi_c1 helicity= 1
 vertex(0,0,1, Djmn(1,-1, 0,phi,theta,gamma));   //1 for chi_c1 helicity=-1
 vertex(0,0,2, Djmn(1, 0, 0,phi,theta,gamma));   //2 for chi_c1 helicity= 0

 vertex(1,1,0, Djmn(1, 1, 0,phi,theta,gamma));
 vertex(1,1,1, Djmn(1,-1, 0,phi,theta,gamma));
 vertex(1,1,2, Djmn(1, 0, 0,phi,theta,gamma));  

 vertex(2,0,0, Djmn(1, 1, 1,phi,theta,gamma)); 
 vertex(2,0,1, Djmn(1,-1, 1,phi,theta,gamma)); 
 vertex(2,0,2, Djmn(1, 0, 1,phi,theta,gamma)); 

 vertex(2,1,0, Djmn(1, 1,-1,phi,theta,gamma));
 vertex(2,1,1, Djmn(1,-1,-1,phi,theta,gamma)); 
 vertex(2,1,2, Djmn(1, 0,-1,phi,theta,gamma)); 

  return ;

}




