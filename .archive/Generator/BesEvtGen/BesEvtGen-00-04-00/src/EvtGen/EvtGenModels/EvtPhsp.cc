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
// Module: EvtPhsp.cc
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
#include "EvtGenModels/EvtPhsp.hh"
#include "EvtGenModels/EvtGlobalSet.hh"
#include <string>

EvtPhsp::~EvtPhsp() {}

void EvtPhsp::getName(std::string& model_name){

  model_name="PHSP";     

}

EvtDecayBase* EvtPhsp::clone(){

  return new EvtPhsp;

}


void EvtPhsp::init(){

  // check that there are 0 arguments
  checkNArg(0);

}

void EvtPhsp::initProbMax(){

  noProbMax();

}

void EvtPhsp::decay( EvtParticle *p ){

  //unneeded - lange - may13-02
  //if ( p->getNDaug() != 0 ) {
    //Will end up here because maxrate multiplies by 1.2
  //  report(DEBUG,"EvtGen") << "In EvtPhsp: has "
  //			   <<" daugthers should not be here!"<<endl;
  //  return;
  //}  
 EvtFilter:
  double weight = p->initializePhaseSpace(getNDaug(),getDaugs());
  //  std::cout<<"weight= "<<weight<<std::endl;
  if(!EvtGlobalSet::iVV.size()) return;

  for(int i=0;i<EvtGlobalSet::iVV.size();i++){
    EvtVector4R psum(0,0,0,0);
    for(int j=0;j<EvtGlobalSet::iVV[i].size();j++){
      int idx =EvtGlobalSet::iVV[i][j]; 
      psum += p->getDaug(idx)->getP4(); 
      //debugging
      //std::cout<<idx<<std::endl;
    }
    double xmass=psum.mass();
    //std::cout<<xmass<<" "<<EvtGlobalSet::dVV[i][0]<<" "<<EvtGlobalSet::dVV[i][1]<<std::endl;
    if(xmass<EvtGlobalSet::dVV[i][0] || xmass>EvtGlobalSet::dVV[i][1]) goto EvtFilter;
  }
  
  return ;
}


