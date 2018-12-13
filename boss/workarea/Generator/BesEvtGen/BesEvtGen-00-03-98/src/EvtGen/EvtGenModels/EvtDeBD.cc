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
// Module: EvtDeBD.cc
//
// Description: To generate events for decuplet baryon deacys.
//
// Modification history:
//
//    Ping R.-G.    Apr., 2007       Module created
//
//------------------------------------------------------------------------
//
#include "EvtGenBase/EvtPatches.hh"
#include <stdlib.h>
#include <iostream>
#include <string>
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtGenKine.hh"
#include "EvtGenModels/EvtDeBD.hh"
#include "EvtGenBase/EvtDiracSpinor.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtVector4C.hh"
#include "EvtGenBase/EvtId.hh"
#include "EvtGenBase/EvtHelSys.hh"
#include "EvtGenBase/EvtRaritaSchwinger.hh"
#include "EvtGenBase/EvtRaritaSchwingerParticle.hh"

using std::cout;
using std::endl;

EvtDeBD::~EvtDeBD() {}

void EvtDeBD::getName(std::string& model_name){

  model_name="DeBD";     

}


EvtDecayBase* EvtDeBD::clone(){

  return new EvtDeBD;

}

void EvtDeBD::init(){

  // check that there are 0 arguments
  checkNArg(0);
  checkNDaug(2);

   checkSpinParent(EvtSpinType::RARITASCHWINGER);
  
   checkSpinDaughter(0,EvtSpinType::DIRAC);
   checkSpinDaughter(1,EvtSpinType::SCALAR);

}

/*
void EvtDeBD::initProbMax(){
  setProbMax(10.0);
}
*/

void EvtDeBD::decay(EvtParticle *p){

  static EvtId PIM=EvtPDL::getId("pi-");
  static EvtId PI0=EvtPDL::getId("pi0");
  static EvtId KM=EvtPDL::getId("K-");
 
  static EvtId L0 =EvtPDL::getId("Lambda0");
  static EvtId LB =EvtPDL::getId("anti-Lambda0");
  static EvtId XM =EvtPDL::getId("Xi-");
  static EvtId XMB=EvtPDL::getId("anti-Xi+"); 
  static EvtId X0 =EvtPDL::getId("Xi0");
  static EvtId X0B=EvtPDL::getId("anti-Xi0");

  static EvtId OM =EvtPDL::getId("Omega-");
  static EvtId OMB=EvtPDL::getId("anti-Omega+");

  p->initializePhaseSpace(getNDaug(),getDaugs());

  EvtParticle *l1, *l2;
  l1 = p->getDaug(0);
  l2 = p->getDaug(1);

  EvtId p1,d1,d2;
  p1=p ->getId();
  d1=l1->getId();
  d2=l2->getId();

 
  EvtVector4R pDaug=l1->getP4();
  double p3=pDaug.d3mag();

  EvtVector4R pv,ps,ppr;
  pv=l1->getP4Lab(); //baryon
  ps=l2->getP4Lab(); //meson
  ppr=p->getP4();

  double theta,alpha;
  EvtHelSys angles(ppr,pv);   //using helicity sys.angles
  theta=angles.getHelAng(1);
  double phi  =angles.getHelAng(2);
  double gamma=0;

  if((p1==OM)&&(d1==L0)){
    alpha=0.0175;
   }
  else if((p1==OMB)&&(d1==LB)){ 
   alpha=-0.0175;
   }
  else if((p1==OM)&&(d1==X0)){
   alpha=0.09;
   }
  else if((p1==OMB)||(d1==X0B)){
    alpha=-0.09;
   }
  else if((p1==OM)||(d1==XM)){
    alpha=0.05;
   }
  else if((p1==OMB)||(d1==XMB)){
    alpha=-0.05;
   }

  double FP,FM;
  if(p1==OM||p1==OMB){
    FM=sqrt((1+alpha)/2.);
    FP=sqrt((1-alpha)/2.);
   }
    else {
    FP=1;FM=1;
     }

  vertex(0,0,Djmn(1.5, 0.5, 0.5,phi,theta,gamma)*FP);
  vertex(0,1,Djmn(1.5, 0.5,-0.5,phi,theta,gamma)*FM);
  vertex(1,0,Djmn(1.5, 1.5, 0.5,phi,theta,gamma)*FP);
  vertex(1,1,Djmn(1.5, 1.5,-0.5,phi,theta,gamma)*FM);
  vertex(2,0,Djmn(1.5,-1.5, 0.5,phi,theta,gamma)*FP);
  vertex(2,1,Djmn(1.5,-1.5,-0.5,phi,theta,gamma)*FM);
  vertex(3,0,Djmn(1.5,-0.5, 0.5,phi,theta,gamma)*FP);
  vertex(3,1,Djmn(1.5,-0.5,-0.5,phi,theta,gamma)*FM);

  return;

}













