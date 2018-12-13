//--------------------------------------------------------------------------
//
//
// Module: EvtSinglePoint.cc
//
// Description: Special model to generate single particles with fixed momentu.
//
// Modification history:
//
//    RYD       Febuary 17,1998       Module created
//
//------------------------------------------------------------------------
//
#include "EvtGenBase/EvtPatches.hh"
#include <stdlib.h>
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtRandom.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenModels/EvtSinglePoint.hh"
#include "EvtGenBase/EvtReport.hh"
#include <string>
#include "EvtGenBase/EvtConst.hh"
using std::endl;

EvtSinglePoint::~EvtSinglePoint() {}

void EvtSinglePoint::getName(std::string& model_name){

  model_name="SinglePoint";     

}

EvtDecayBase* EvtSinglePoint::clone(){

  return new EvtSinglePoint();

}

void EvtSinglePoint::init(){


  //turn off checks for charge conservation
  disableCheckQ();

  if ((getNArg()==3)) {
    px = getArg(0);
    py = getArg(1);
    pz = getArg(2);
    
  } else{
    std::cout<<"expected three parameters: px, py, pz, but find "<<getNArg()<<" parameters"<<std::endl;
    ::abort();
  }
  
}

void EvtSinglePoint::decay( EvtParticle *p ){

  EvtParticle *d;
  EvtVector4R p4;

  double mass=EvtPDL::getMass(getDaug(0));

  p->makeDaughters(getNDaug(),getDaugs());
  d=p->getDaug(0);

  //generate flat distribution in p 
  //we are now in the parents restframe! This means the 
  //restframe of the e+e- collison.
  
  double pmag;
  pmag = sqrt(px*px + py*py + py*pz);

 
    p4.set(sqrt(mass*mass+pmag*pmag),px,py,pz);
    d->init( getDaug(0),p4);
  return ;
}



