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
// Module:  EvtT2GV.cc
//
// Description: Routine to decay chi_c2-> gamma J/psi     
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
#include "EvtGenModels/EvtT2GV.hh"
#include "EvtGenBase/EvtRandom.hh"

#include <string>

EvtT2GV::~EvtT2GV() {}

void EvtT2GV::getName(std::string& model_name){

  model_name="T2GV";     

}

EvtDecayBase* EvtT2GV::clone(){

  return new EvtT2GV;

}


void EvtT2GV::init(){

  // check that there are 0 arguments
  checkNArg(0);

}

void EvtT2GV::initProbMax(){

  //  noProbMax();
setProbMax(30.00001);
}

void EvtT2GV::decay( EvtParticle *p ){

loop:
  p->initializePhaseSpace(getNDaug(),getDaugs());
  EvtParticle *v,*s1;
  EvtVector4R pv,ps,ppr;
  
  v =p->getDaug(0);  //gamma
  s1=p->getDaug(1); //J/psi
  pv=v->getP4();
  ps=s1->getP4();
  ppr=p->getP4();
 
  double alpha=21./73.;
//  Put phase space results into the daughters.
//  EvtHelSys angles(ppr,pv); //using helicity sys. angles
  EvtHelSys angles(ps,pv); 
  double theta =angles.getHelAng(1);
  double phi   =angles.getHelAng(2);
  double gamma=0;
  double F0=1.;
  double F1=sqrt(3.)*F0;  //pure M1 transition assumed,see PRD13,p1203 
  double F2=sqrt(6.)*F0;  //helicity amplitude is labeled with lambda_chi and photon
   
  
 int j1,j2,j3;
 for(j1=0;j1<=4;){
 for(j2=0;j2<=1;){
 for(j3=0;j3<=2;){
 vertex(j1,j2,j3,0.0);j3++;}
j2++;}
j1++;}
  

 vertex(0,0,0, Djmn(1, 1, 1,phi,theta,gamma)*F0);   
 vertex(0,0,1, Djmn(1,-1, 1,phi,theta,gamma)*F0);   
 vertex(0,0,2, Djmn(1, 0, 1,phi,theta,gamma)*F0);   
 vertex(0,1,0, Djmn(1, 1,-1,phi,theta,gamma)*F0); 
 vertex(0,1,1, Djmn(1,-1,-1,phi,theta,gamma)*F0);   
 vertex(0,1,2, Djmn(1, 0,-1,phi,theta,gamma)*F0); 

 vertex(1,0,0, Djmn(1, 1,-1,phi,theta,gamma)*F2);   
 vertex(1,0,1, Djmn(1,-1,-1,phi,theta,gamma)*F2);
 vertex(1,0,2, Djmn(1, 0,-1,phi,theta,gamma)*F2);   

 vertex(2,1,0, Djmn(1, 1, 1,phi,theta,gamma)*F2); 
 vertex(2,1,1, Djmn(1,-1, 1,phi,theta,gamma)*F2); 
 vertex(2,1,2, Djmn(1, 0, 1,phi,theta,gamma)*F2);

 vertex(3,0,0, Djmn(1, 1, 0,phi,theta,gamma)*F1); 
 vertex(3,0,1, Djmn(1,-1, 0,phi,theta,gamma)*F1);
 vertex(3,0,2, Djmn(1, 0, 0,phi,theta,gamma)*F1); 

 vertex(4,1,0, Djmn(1, 1, 0,phi,theta,gamma)*F1); 
 vertex(4,1,1, Djmn(1,-1, 0,phi,theta,gamma)*F1); 
 vertex(4,1,2, Djmn(1, 0, 0,phi,theta,gamma)*F1);


   return ;
}


