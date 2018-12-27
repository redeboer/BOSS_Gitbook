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
// Module:  EvtRhoPi.cc
//
// Description: Jpsi or psi(2S) decays into 3 pions via rho(1--)pi
//
// Modification history:
//
//    Ping R.-G.       Apr., 2007       Module created
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
#include "EvtGenModels/EvtRhoPi.hh"
#include "EvtGenBase/EvtRandom.hh"
#include "EvtGenModels/EvtVVS.hh"
#include <string>
using namespace std;  //::endl;

EvtRhoPi::~EvtRhoPi() {}

void EvtRhoPi::getName(std::string& model_name){

  model_name="RhoPi";     

}

EvtDecayBase* EvtRhoPi::clone(){

  return new EvtRhoPi;

}


void EvtRhoPi::init(){

//  checkNArg(6);
  EvtSpinType::spintype parenttype = EvtPDL::getSpinType(getParentId());
//  if ( parenttype == EvtSpinType::SCALAR ){
//  report(ERROR,"EvtGen")<<"Scalar decays with flat distribution"<<endl;
//  ::abort();
// }
}
void EvtRhoPi::initProbMax(){
  noProbMax();

}


static int nrun=1;
static double max_amps=0.0;

void EvtRhoPi::decay( EvtParticle *p ){

///// default use rho(770) as the intermediate state, this set parameter will generate the symmetric momentum distribution for pi+ pi-.
double ResonanceMass =0.7710;
double ResonanceWidth=0.1492;
double r1            =0.9;
double phase1        =0;
double r2            =1.1;
double phase2        =1.5;
  if(getNArg()>0){ 
  ResonanceMass =getArg(0);
  ResonanceWidth=getArg(1);
  r1            =getArg(2);
  phase1        =getArg(3);
  r2            =getArg(4);
  phase2        =getArg(5);
  }
double amps,SamAmps,rd1;
// calculated the max amplitude square in 20000 events, is it enough larger?
  if(nrun==1){
     int ir,nd;
     for(ir=0;ir<=20000;ir++){
       p->initializePhaseSpace(getNDaug(),getDaugs());
       int nd=p->getNDaug(),i;
       _nd=nd;
       for(i=0;i<=nd-1;i++){
          _p4Lab[i]=p->getDaug(i)->getP4Lab();
          _p4CM[i]=p->getDaug(i)->getP4();
       }
       amps=AmplitudeSquare(ResonanceMass,ResonanceWidth,r1,r2,phase1,phase2);
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
   amps=AmplitudeSquare(ResonanceMass,ResonanceWidth,r1,r2,phase1,phase2);
   SamAmps=amps/max_amps;
   rd1=EvtRandom::Flat(0.0, 1.0);
  if(rd1>=SamAmps) goto loop;
  return ;
}


double EvtRhoPi::AmplitudeSquare(double ResonanceMass, double ResonanceWidth,double r1,double 
r2,double phase1,double phase2){
  EvtVector4R dp1=GetDaugMomLab(0);
  EvtVector4R dp2=GetDaugMomLab(1);
  EvtVector4R dp3=GetDaugMomLab(2);
  VVS Jpsi2rhopi(dp1,dp2,dp3,ResonanceMass,ResonanceWidth,r1,r2,phase1,phase2);
  return Jpsi2rhopi.amps();
 
 }

