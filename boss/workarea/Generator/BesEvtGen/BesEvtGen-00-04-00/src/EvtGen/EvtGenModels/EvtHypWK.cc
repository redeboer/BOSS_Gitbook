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
// Module: EvtHypWK.hh
//
// Description: To generate octet hyperon weak decays.
//
// Modification history:
//
//    Pang C.-Y., Ping R.-G.    Mar, 2007       Module created
//    Ping R.G. Apr. 2007  change to helicity amplitude method
//------------------------------------------------------------------------
//
#include "EvtGenBase/EvtPatches.hh"
#include <stdlib.h>
#include <iostream>
#include <string>
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtGenKine.hh"
#include "EvtGenModels/EvtHypWK.hh"
#include "EvtGenBase/EvtDiracSpinor.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtVector4C.hh"
#include "EvtGenBase/EvtId.hh"
#include "EvtGenBase/EvtHelSys.hh"
#include "EvtGenBase/EvtdFunction.hh"

using std::cout;
using std::endl;

EvtHypWK::~EvtHypWK() {}

void EvtHypWK::getName(std::string& model_name){

  model_name="HypWK";     

}



EvtDecayBase* EvtHypWK::clone(){

  return new EvtHypWK;

}

void EvtHypWK::init(){

  // check that there are 0 arguments
  checkNArg(0);
  checkNDaug(2);

 //  checkSpinParent(EvtSpinType::VECTOR);
   checkSpinParent(EvtSpinType::DIRAC);
  
   checkSpinDaughter(0,EvtSpinType::DIRAC);
   checkSpinDaughter(1,EvtSpinType::SCALAR);
//  checkSpinDaughter(1,EvtSpinType::DIRAC);

}


void EvtHypWK::decay(EvtParticle *p){

  static EvtId PIP=EvtPDL::getId("pi+");
  static EvtId PIM=EvtPDL::getId("pi-");
  static EvtId PI0=EvtPDL::getId("pi0");
  static EvtId PP=EvtPDL::getId("p+");
  static EvtId PM=EvtPDL::getId("anti-p-");
  static EvtId NP=EvtPDL::getId("n0");
  static EvtId NM=EvtPDL::getId("anti-n0");
 
  static EvtId LP=EvtPDL::getId("Lambda0");
  static EvtId LM=EvtPDL::getId("anti-Lambda0");
  static EvtId SPP=EvtPDL::getId("Sigma+");
  static EvtId SPM=EvtPDL::getId("anti-Sigma-");
  static EvtId SMP=EvtPDL::getId("Sigma-");
  static EvtId SMM=EvtPDL::getId("anti-Sigma+");

  static EvtId XP=EvtPDL::getId("Xi-");
  static EvtId XM=EvtPDL::getId("anti-Xi+");
  static EvtId X0P=EvtPDL::getId("Xi0");
  static EvtId X0M=EvtPDL::getId("anti-Xi0");


  static int count=0;

//  if(count==0) findMaxPro(p); //using the default getProbMax() in EvtGenBase/EvtDecayProb.cc 
  count+=1;

  p->initializePhaseSpace(getNDaug(),getDaugs());

  EvtParticle *l1, *l2;
  l1 = p->getDaug(0);
  l2 = p->getDaug(1);
  EvtVector4R DRP4=l1->getP4(); //Dirac Daughter
  EvtVector4R MEP4=l2->getP4(); //Meson Daughter
  EvtVector4R PRP4=p->getP4();  //Parent Momentum
  EvtHelSys angles(PRP4,DRP4);   //using helicity sys.angles
  double tht=angles.getHelAng(1);
  double ph =angles.getHelAng(2);
  double gmm=0;

  EvtVector4R pDaug=l1->getP4();
  double p3=pDaug.d3mag();
    

  double M=p->mass();
  double M2=M*M;
  double m=l1->mass();
  double m2=m*m;
  double theta,phi,alpha,beta,ga;   // the following is the 

  EvtId p1,d1,d2;
  p1=p->getId();
  d1=l1->getId();
  d2=l2->getId();

  if(p1==XP){ alpha=-0.46;phi=-2.1; }
    else if(p1==XM){alpha=0.46;phi=-2.1;}
    else if(p1==X0P){alpha=-0.41;phi=21.0;}
    else if(p1==X0M){alpha=0.41;phi=21.0; }
    else if(p1==LP){alpha=0.64;phi=-6.5; }
    else if(p1==LM){ alpha=-0.64;phi=-6.5; }
    else if(p1==SMP){alpha=-0.07;phi=10.0;}
    else if(p1==SMM){alpha=0.07;phi=10.0; }
    else if(p1==SPP){                                
      if (d1==PP){  alpha=-0.98;phi=36.0; }
      else if(d1==NP){ alpha=0.07;phi=167.0; }
      else { cout<<"The decay is not discussed."<<endl; }
      } 
    else if(p1==SPM){
      if (d1==PM){  alpha=0.98;phi=36.0; }
      else if(d1==NM){ alpha=-0.07;phi=167.0; }
      else { cout<<"The decay is not discussed."<<endl; }
      }         
  else { cout<<"The decay is not discussed.";
       }

  double FP=sqrt((1+alpha)/2.);
  double FM=sqrt((1-alpha)/2.);

  vertex(0,0,Djmn(0.5, 0.5, 0.5,ph,tht,gmm)*FP);
  vertex(0,1,Djmn(0.5, 0.5,-0.5,ph,tht,gmm)*FM);
  vertex(1,0,Djmn(0.5,-0.5, 0.5,ph,tht,gmm)*FP);
  vertex(1,1,Djmn(0.5,-0.5,-0.5,ph,tht,gmm)*FM);

  return;

}



