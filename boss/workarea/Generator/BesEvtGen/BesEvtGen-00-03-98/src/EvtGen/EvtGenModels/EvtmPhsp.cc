//--------------------------------------------------------------------------
//
// Environment:
//      This software is part of the EvtGen package developed jointly
//      for the BaBar and CLEO collaborations.  If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information: See EvtGen/COPYRIGHT
//      Copyright (C) 1998      Caltech, UCSB
//
// Module: EvtmPhsp.cc
//
// Description: Routine to decay a particle according th phase space
//
// Modification history:
//
//    RYD       January 8, 1997       Module created
//
//------------------------------------------------------------------------
//
#include "EvtGenBase/EvtPatches.hh"
#include <stdlib.h>
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtGenKine.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenModels/EvtmPhsp.hh"
#include <string>
using std::endl;

EvtmPhsp::~EvtmPhsp() {}

void EvtmPhsp::getName(std::string& model_name){

  model_name="mPHSP";     

}

EvtDecayBase* EvtmPhsp::clone(){

  return new EvtmPhsp;

}


void EvtmPhsp::init(){

  if (getNArg()==3) {
    nd1=getArg(0); //assign the first nd1 particle in the son list
    ma1=getArg(1); // the lower mass for the nd1 son cluster
    ma2=getArg(2); // the upper mass for the nd1 son cluster
  }else if(getNArg()==6){
    nd1=getArg(0); //assign the first nd1 particle in the son list
    ma1=getArg(1); // the lower mass for the nd1 son cluster
    ma2=getArg(2); // the upper mass for the nd1 son cluster
    nd2=getArg(3); //assign the first nd1 particle in the son list
    mb1=getArg(4); // the lower mass for the nd2 son cluster
    mb2=getArg(5); // the upper mass for the nd2 son cluster
  }else{
    
    report(ERROR,"EvtGen") << "EvtmPHSP generator expected "
                           << " 3 or 6 arguments but found:"<<getNArg()<<endl;
    report(ERROR,"EvtGen") << "Will terminate execution!"<<endl;
    ::abort();
  }

}

void EvtmPhsp::initProbMax(){

  noProbMax();

}

void EvtmPhsp::decay( EvtParticle *p ){
  EvtVector4R pt,pt2;
 loop:

  p->initializePhaseSpace(getNDaug(),getDaugs());

  pt=p->getDaug(0)->getP4Lab();
  for (int ii=1;ii<nd1;ii++){
    pt=pt+p->getDaug(ii)->getP4Lab();
  }
  xmass1=pt.mass();

  if(getNArg()==6) {
    pt2=p->getDaug(nd1)->getP4Lab();
    for(int jj=nd1+1;jj<nd1+nd2;jj++) pt2=pt2+p->getDaug(jj)->getP4Lab();
   }
  xmass2=pt2.mass();

  // std::cout<<"xmass1= "<<xmass1<<std::endl;

  if(getNArg()==3 && (xmass1<ma1||xmass1>ma2) )goto loop;
  if(getNArg()==6 && (xmass1<ma1||xmass1>ma2 || xmass2<mb1 || xmass2>mb2) ) goto loop;

  return ;
}


