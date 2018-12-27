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
// Module: EvtJ2BB1.cc
//
// Description: Routine to decay J/psi-> B_8 bar B_8 using helicity amplitude.
//              
// Modification history:
//
//    Pang C.-Y., Ping R.-G.    December, 2006       Module created
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
#include "EvtJ2BB1.hh"
#include <string>
using std::endl;

EvtJ2BB1::~EvtJ2BB1() {}

void EvtJ2BB1::getName(std::string& model_name){

  model_name="J2BB1";     

}


EvtDecayBase* EvtJ2BB1::clone(){

  return new EvtJ2BB1;

}

/*
void EvtJ2BB1::initProbMax() {
   double ProbMax=12.;
   setProbMax(ProbMax); 
}
*/

void EvtJ2BB1::init(){
 checkNDaug(2);
 checkSpinParent(EvtSpinType::VECTOR);
 checkSpinDaughter(0,EvtSpinType::DIRAC);
 checkSpinDaughter(1,EvtSpinType::DIRAC);
}

void EvtJ2BB1::decay( EvtParticle *p){
  
  p->initializePhaseSpace(getNDaug(),getDaugs());
  
  EvtParticle *v,*s1;
  EvtVector4R pv,ps,ppr;

  v =p->getDaug(0);  //Baryon
  s1=p->getDaug(1); //Baryon
  pv=v->getP4();
  ps=s1->getP4();
  ppr=p->getP4();
//  Put phase space results into the daughters.
 
  EvtHelSys angles(ppr,pv);   //using helicity sys.angles
  double theta=angles.getHelAng(1);
  double phi  =angles.getHelAng(2);
  double gamma=0;
  double costheta=cos(theta);  //using helicity angles in parent system
//  double costheta=ppr.get(3)/ppr.d3mag();  //using Lab system 

  if(getNArg()>0){ alpha=getArg(0);}
   else{double mass_b1 = EvtPDL::getMass(getDaug(0));
        double mass_b2 = EvtPDL::getMass(getDaug(1));
        double mass_M = EvtPDL::getMass(getParentId());
        double c1=pow(pv.d3mag(),2.0);
        double R=(mass_b1+pv.get(0))*(mass_b2+ps.get(0));
        u=pow((R-c1)/(R+c1),2.0); 
        alpha=(pow(4+u,2.0)-u*pow(2+3*u,2.0))/(pow(4+u,2.0)+u*pow(2+3*u,2.0));}

  double F01=sqrt((1+alpha)/2);
  double F00=sqrt((1-alpha)/4);
  double F10=F01;
  double F11=F00;

 vertex(0,0,0,Djmn(1, 1, 0,phi,theta,gamma)*F00);  // J/psi helicity =1 corresponding index=0
 vertex(0,0,1,Djmn(1, 1, 1,phi,theta,gamma)*F01);
 vertex(0,1,0,Djmn(1, 1,-1,phi,theta,gamma)*F10); 
 vertex(0,1,1,Djmn(1, 1, 0,phi,theta,gamma)*F11);
 vertex(1,0,0,Djmn(1,-1, 0,phi,theta,gamma)*F00);  // J/psi helicity =-1 corresponding index=1
 vertex(1,0,1,Djmn(1,-1, 1,phi,theta,gamma)*F01); 
 vertex(1,1,0,Djmn(1,-1,-1,phi,theta,gamma)*F10);
 vertex(1,1,1,Djmn(1,-1, 0,phi,theta,gamma)*F11); 
 vertex(2,0,0,Djmn(1, 0, 0,phi,theta,gamma)*F00);  // J/psi helicity =0 corresponding index=2
 vertex(2,0,1,Djmn(1, 0, 1,phi,theta,gamma)*F01);
 vertex(2,1,0,Djmn(1, 0,-1,phi,theta,gamma)*F10);
 vertex(2,1,1,Djmn(1, 0, 0,phi,theta,gamma)*F11);


  return ;

}




