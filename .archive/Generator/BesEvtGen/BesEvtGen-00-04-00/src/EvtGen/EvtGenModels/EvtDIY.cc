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
// Module:  EvtDIY.cc
//
// Description: Model provided by user, see the mannual     
//
// Modification history:
//
//    Ping R.-G.       December, 2006       Module created
//
//------------------------------------------------------------------------

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
#include "EvtGenModels/EvtDIY.hh"
#include "EvtGenBase/EvtRandom.hh"
#include <string>
using namespace std;  //::endl;

EvtDIY::~EvtDIY() {}

void EvtDIY::getName(std::string& model_name){

  model_name="DIY";     

}

EvtDecayBase* EvtDIY::clone(){

  return new EvtDIY;

}


void EvtDIY::init(){

// check that there are 1 arguments:angular distribution parameter 
//  checkNArg(0);
//  checkNDaug(2);
  EvtSpinType::spintype parenttype = EvtPDL::getSpinType(getParentId());
//  if ( parenttype == EvtSpinType::SCALAR ){
//  report(ERROR,"EvtGen")<<"Scalar decays with flat distribution"<<endl;
//  ::abort();
// }
}
void EvtDIY::initProbMax(){
  noProbMax();

}


static int nrun=1;
static double max_amps=0.0;

void EvtDIY::decay( EvtParticle *p ){

double amps,SamAmps,rd1;
// calculated the max amplitude square in 20000 events, is it enough larger?
  if(nrun==1){
     int ir,nd;
     for(ir=0;ir<=200000;ir++){
  loop0:
       p->initializePhaseSpace(getNDaug(),getDaugs());
       int nd=p->getNDaug(),i;
       _nd=nd;
       for(i=0;i<=nd-1;i++){
          _p4Lab[i]=p->getDaug(i)->getP4Lab();
          _p4CM[i]=p->getDaug(i)->getP4();
       }
       amps=AmplitudeSquare();
       if(amps<0) goto loop0;
       if(amps>max_amps) max_amps=amps*1.01;
       nrun++;
     }
  }
if(max_amps==0.0) {report(ERROR,"EvtGen")<<"The decay amplitude square should be positive number"<<endl;abort();}
//cout<<"max_amp="<<max_amps<<endl;


loop:
  p->initializePhaseSpace(getNDaug(),getDaugs());
  int i;
  for(i=0;i<=p->getNDaug()-1;i++){
          _p4Lab[i]=p->getDaug(i)->getP4Lab(); 
          _p4CM[i]=p->getDaug(i)->getP4();
      }
//  Put phase space results into the daughters.
   amps=AmplitudeSquare();
   if(amps < 0) goto loop;
   SamAmps=amps/max_amps;
   rd1=EvtRandom::Flat(0.0, 1.0);
  if(rd1>=SamAmps) goto loop;
  return ;
}


