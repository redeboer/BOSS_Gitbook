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
// Module: EvtSingleParticle.cc
//
// Description: Special model to generate single particles.
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
#include "EvtGenModels/EvtSingleParticle2.hh"
#include "EvtGenBase/EvtReport.hh"
#include <string>
#include "EvtGenBase/EvtConst.hh"
using std::endl;

EvtSingleParticle2::~EvtSingleParticle2() {}

void EvtSingleParticle2::getName(std::string& model_name){

  model_name="SINGLE2";     

}

EvtDecayBase* EvtSingleParticle2::clone(){

  return new EvtSingleParticle2();

}

void EvtSingleParticle2::init(){


  //turn off checks for charge conservation
  disableCheckQ();

  if ((getNArg()==6)||(getNArg()==4)||(getNArg()==2)) {
    
    if (getNArg()==6){
      //copy the arguments into eaiser to remember names!

      pxymin=getArg(0);
      pxymax=getArg(1);
      
      thetamin=getArg(2);
      thetamax=getArg(3);
      
      phimin=getArg(4);
      phimax=getArg(5);

    }

    if (getNArg()==4){
      //copy the arguments into eaiser to remember names!
      pxymin=getArg(0);
      pxymax=getArg(1);
      
      thetamin=getArg(2);
      thetamax=getArg(3);

      phimin=0.0;
      phimax=EvtConst::twoPi;

    }

    if (getNArg()==2){
      //copy the arguments into eaiser to remember names!
      pxymin=getArg(0);
      pxymax=getArg(1);
      
      thetamin=0.376383;
      thetamax=2.76521;   //|cos(theta)|<0.93
      
      phimin=0.0;
      phimax=EvtConst::twoPi;

    }


  }else{
    
    report(ERROR,"EvtGen") << "EvtSingleParticle generator expected "
                           << " 5, 3, or 1 arguments but found:"<<getNArg()<<endl;
    report(ERROR,"EvtGen") << "Will terminate execution!"<<endl;
    ::abort();
  }


  report(INFO,"EvtGen") << "The single particle generator has been configured:"
    <<endl;
  report(INFO,"EvtGen") << thetamax << " > theta > " << thetamin <<endl;
  report(INFO,"EvtGen") << phimax << " > phi > " << phimin <<endl;

}

void EvtSingleParticle2::decay( EvtParticle *p ){

  EvtParticle *d;
  EvtVector4R p4;

  double mass=EvtPDL::getMass(getDaug(0));

  p->makeDaughters(getNDaug(),getDaugs());
  d=p->getDaug(0);

  //generate flat distribution in p 
  //we are now in the parents restframe! This means the 
  //restframe of the e+e- collison.
  
  double theta;
  if(thetamax==thetamin) {theta=thetamax;} else
  theta=EvtRandom::Flat(thetamin,thetamax);
  double phi;
  if(phimin==phimax){phi=phimin;} else
  phi = EvtRandom::Flat(phimin,phimax);
  double pxy;
  if(pxymin==pxymax){pxy=pxymin;} else
    pxy =  EvtRandom::Flat(pxymin,pxymax);

  pmag=pxy/sin(theta);  
  cthetamax=cos(thetamax);
  cthetamin=cos(thetamin);
  double cthetalab;

  //  do{
    //generate flat distribution in costheta
 
    p4.set(sqrt(mass*mass+pmag*pmag),pmag*cos(phi)*sin(theta),
	   pmag*sin(phi)*sin(theta),pmag*cos(theta));
    d->init( getDaug(0),p4);
    //get 4 vector in the lab frame!
    EvtVector4R p4lab=d->getP4Lab();
    cthetalab=p4lab.get(3)/p4lab.d3mag();
    //  }while (cthetalab>cthetamax||cthetalab<cthetamin);

  return ;
}



