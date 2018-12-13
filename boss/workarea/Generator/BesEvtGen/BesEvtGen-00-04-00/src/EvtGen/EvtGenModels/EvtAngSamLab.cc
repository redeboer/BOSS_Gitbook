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
// Module: EvtAngSamLab.cc
//
// Description: Routine to decay a particle to two bodies by sampling angular distribution in Lab
//              system.
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
#include "EvtGenBase/EvtDiracParticle.hh"
#include "EvtGenBase/EvtScalarParticle.hh"
#include "EvtGenBase/EvtVectorParticle.hh"
#include "EvtGenBase/EvtTensorParticle.hh"
#include "EvtGenBase/EvtPhotonParticle.hh" 
#include "EvtGenBase/EvtNeutrinoParticle.hh"
#include "EvtGenBase/EvtStringParticle.hh"
#include "EvtGenBase/EvtRaritaSchwingerParticle.hh"
#include "EvtGenBase/EvtHighSpinParticle.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtHelSys.hh"
#include "EvtGenModels/EvtAngSamLab.hh"
#include "EvtGenBase/EvtRandom.hh"
#include <string>
using std::endl;

EvtAngSamLab::~EvtAngSamLab() {}

void EvtAngSamLab::getName(std::string& model_name){

  model_name="AngSamLab";     

}

EvtDecayBase* EvtAngSamLab::clone(){

  return new EvtAngSamLab;

}


void EvtAngSamLab::init(){

  // check that there are 1 arguments:angular distribution parameter 
  checkNDaug(2);
  EvtSpinType::spintype parenttype = EvtPDL::getSpinType(getParentId());
//  if ( parenttype == EvtSpinType::SCALAR ){
//  report(ERROR,"EvtGen")<<"Scalar decays with flat distribution"<<endl;
//  ::abort();
// }
  if(getNArg()>6 || getNArg()<2 ) {
    report(ERROR,"EvtGen")<<" 2<= expected parameters <=6, but it is "<< getNArg()<<endl;
  ::abort();
  }
}
void EvtAngSamLab::initProbMax(){

  noProbMax();

}

void EvtAngSamLab::decay( EvtParticle *p ){

loop:
  p->initializePhaseSpace(getNDaug(),getDaugs());

  EvtParticle *v,*s1;
  EvtVector4R pv;
 
  v =p->getDaug(0); 
  s1=p->getDaug(1); 
  pv=v->getP4Lab();
//  Put phase space results into the daughters.

  double theta =pv.theta();

// The angular form is: c0 + c2*cos^2(theta) + c4*cos^4(theta) + c6*cos^6(theta) + c8*cos^8(theta) + c10*cos^10(theta)
// The parameters c0-c10 are specified by user in the decay list 
  int narg=getNArg();
  double c0,c2,c4,c6,c8,c10;
  c0=0;c2=0;c4=0;c6=0;c8=0;c10=0; 
  if(narg==2) {
   c0=getArg(0);
   c2=getArg(1);
  } else if (narg==3){
   c0=getArg(0);
   c2=getArg(1);
   c4=getArg(2);
  } else if (narg==4){
   c0=getArg(0);
   c2=getArg(1);
   c4=getArg(2);
   c6=getArg(3);
  } else if (narg==5){
   c0=getArg(0);
   c2=getArg(1);
   c4=getArg(2);
   c6=getArg(3);
   c8=getArg(4);
  } else if (narg==6){
   c0=getArg(0);
   c2=getArg(1);
   c4=getArg(2);
   c6=getArg(3);
   c8=getArg(4);
   c10=getArg(5);
  }

  double costheta= cos(theta);
  double costheta2= costheta*costheta;
  double costheta4= costheta2*costheta2;
  double costheta6= costheta4*costheta2;
  double costheta8= costheta6*costheta2;
  double costheta10= costheta8*costheta2;

  double amp1=(c0+c2*costheta2+c4*costheta4+c6*costheta6+c8*costheta8+c10*costheta10);
  double a0,a2,a4,a6,a8,a10;
  double ampflag;
  if(c0<0) {a0=0;}else{a0=c0;}
  if(c2<0) {a2=0;}else{a2=c2;}
  if(c4<0) {a4=0;}else{a4=c4;}
  if(c6<0) {a6=0;}else{a6=c6;}
  if(c8<0) {a8=0;}else{a8=c8;}
  if(c10<0) {a10=0;}else{a10=c10;}
  ampflag=a0+a2+a4+a6+a8+a10;
  if( ampflag<=0 ) {
    report(ERROR,"EvtGen")<<" The maxium value of amplitude square should be positive, but it is "<< ampflag<<endl;
  ::abort();
  }
  ampflag = amp1/ampflag;
  double rd1=EvtRandom::Flat(0.0, 1.0);
  if(rd1>=ampflag) goto loop;
  return ;
}


