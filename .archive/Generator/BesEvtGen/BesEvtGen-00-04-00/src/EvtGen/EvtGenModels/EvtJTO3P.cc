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
// Module:  EvtJTO3P.cc
//
// Description: Routine to a vector decays into 3 pseudoscalar with only contact term
    
//
// Modification history:
//
//  Ping R.-G.    December, 2007       Module created
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
#include "EvtGenModels/EvtJTO3P.hh"
#include "EvtGenBase/EvtRandom.hh"
#include "EvtGenBase/EvtEulerAngles.hh"
#include <string>
using namespace std;  //::endl;

EvtJTO3P::~EvtJTO3P() {}

void EvtJTO3P::getName(std::string& model_name){

  model_name="JTO3P";     

}

EvtDecayBase* EvtJTO3P::clone(){

  return new EvtJTO3P;

}


void EvtJTO3P::init(){

// check that there are 1 arguments:angular distribution parameter 
//  checkNArg(0);
  checkNDaug(3);
  checkSpinParent(EvtSpinType::VECTOR);
  checkSpinDaughter(0,EvtSpinType::SCALAR);
  checkSpinDaughter(1,EvtSpinType::SCALAR);
  checkSpinDaughter(2,EvtSpinType::SCALAR);
}

//void EvtJTO3P::initProbMax(){
// setProbMax(1.0);
//}


void EvtJTO3P::decay( EvtParticle *p ){

 p->initializePhaseSpace(getNDaug(),getDaugs());
       
       EvtVector4R mompi1 = p->getDaug(0)->getP4();
       EvtVector4R mompi2 = p->getDaug(1)->getP4();
       EvtVector4R mompi3 = p->getDaug(2)->getP4();
       EvtVector4R Zaxis  = mompi2.cross(mompi3);
       
       EvtEulerAngles EA(mompi1,Zaxis);
       double alpha=EA.getAlpha();
       double beta =EA.getBeta();
       double gamma=EA.getGamma();

       EvtVector4R h1,h2; //rotate pi1 and pi2 momentum to helicity system
       h1 = rotateEuler(mompi1,-gamma,-beta,-alpha);
       h2 = rotateEuler(mompi2,-gamma,-beta,-alpha);
       //       cout<<h1<<h2<<endl;
       double F0 = h1.get(1)*h2.get(2)-h1.get(2)*h2.get(1);
       //       cout<<"F0 = "<<F0<<endl;       
 
       vertex(0,Djmn(1, 1,0,alpha,beta,gamma)*F0);
       vertex(1,Djmn(1,-1,0,alpha,beta,gamma)*F0);
       vertex(2,Djmn(1, 0,0,alpha,beta,gamma)*F0);

}


