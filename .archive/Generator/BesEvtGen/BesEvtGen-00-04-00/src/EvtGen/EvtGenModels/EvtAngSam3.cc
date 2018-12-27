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
// Module: EvtAngSam3.cc
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
#include "EvtGenModels/EvtAngSam3.hh"
#include "EvtGenBase/EvtRandom.hh"
#include <string>
using std::cout;
using std::endl;

EvtAngSam3::~EvtAngSam3() {}

void EvtAngSam3::getName(std::string& model_name){

  model_name="AngSam3";     

}

EvtDecayBase* EvtAngSam3::clone(){

  return new EvtAngSam3;

}


void EvtAngSam3::init(){

  // check angular distribution parameters  
  checkNDaug(3);
  EvtSpinType::spintype parenttype = EvtPDL::getSpinType(getParentId());

  if(getNArg()>6 || getNArg()<2 ) {
    report(ERROR,"EvtGen")<<"  2<= expected parameters <=6, but it is "<< getNArg()<<endl;
  ::abort();
  }
} 
     
void EvtAngSam3::initProbMax(){

  noProbMax();

}

void EvtAngSam3::decay( EvtParticle *p ){

loop:
  p->initializePhaseSpace(getNDaug(),getDaugs());

  EvtParticle *v,*s1,*s2;
  EvtVector4R pv,ps;
  EvtVector4R Lpt,xp;
 
  v =p->getDaug(0); 
  s1=p->getDaug(1); 
  s2=p->getDaug(2);
  pv=v->getP4();
  ps=s1->getP4();
  Lpt=p->getP4();
  xp=pv.cross(ps);

  // calculate the cos (theta) between xp and Lpt
  double costheta;
  if(Lpt.d3mag()<0.0001){
    costheta = xp.get(3)/xp.d3mag();
  } else{
    costheta = (xp.get(1)*Lpt.get(1) + xp.get(2)*Lpt.get(2) + xp.get(3)*Lpt.get(3))/xp.d3mag()/Lpt.d3mag();
  }
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


