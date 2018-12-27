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
// Module: EvtJpipi.cc
//
// Description: Routine to decay psi(2S)-> J/psi pi pi using chiral Lagrangian and 
//      information by fitting the exp. data
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
#include "EvtGenBase/EvtTensor4C.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenModels/EvtJpipi.hh"
#include <string>
using std::endl;

EvtJpipi::~EvtJpipi() {}

void EvtJpipi::getName(std::string& model_name){

  model_name="JPIPI";     

}


EvtDecayBase* EvtJpipi::clone(){

  return new EvtJpipi;

}

void EvtJpipi::init(){

  static EvtId PIP=EvtPDL::getId("pi+");
  static EvtId PIM=EvtPDL::getId("pi-");
  static EvtId PI0=EvtPDL::getId("pi0");

  // check that there are 0 arguments
  checkNArg(0);
  checkNDaug(3);

  checkSpinParent(EvtSpinType::VECTOR);
  checkSpinDaughter(0,EvtSpinType::VECTOR);



  if ((!(getDaug(1)==PIP&&getDaug(2)==PIM))&&
      (!(getDaug(1)==PI0&&getDaug(2)==PI0))) {
    report(ERROR,"EvtGen") << "EvtJpipi generator expected "
                           << " pi+ and pi- (or pi0 and pi0) "
			   << "as 2nd and 3rd daughter. "<<endl;
    report(ERROR,"EvtGen") << "Will terminate execution!"<<endl;
    ::abort();
  }

}

void EvtJpipi::initProbMax() {

  //Hard coded... should not be hard to calculate...
 setProbMax(0.019);

}      

void EvtJpipi::decay( EvtParticle *p){

  p->initializePhaseSpace(getNDaug(),getDaugs());

  EvtParticle *v,*s1,*s2;
  
  v=p->getDaug(0);
  s1=p->getDaug(1);
  s2=p->getDaug(2);

//  Put phase space results into the daughters.
  
  EvtVector4C ep0,ep1,ep2;  
  
  ep0=p->eps(0);
  ep1=p->eps(1);
  ep2=p->eps(2);

//  double fac=(s1->getP4()+s2->getP4()).mass2()-4*s1->mass()*s2->mass();
  double fac1=(s1->getP4()+s2->getP4()).mass2()-2*s1->mass()*s2->mass();
  double fac=0.3*fac1-2*0.11*(s1->getP4()).get(0)*(s2->getP4()).get(0);

  vertex(0,0,fac*(ep0*v->epsParent(0).conj()));
  vertex(0,1,fac*(ep0*v->epsParent(1).conj()));
  vertex(0,2,fac*(ep0*v->epsParent(2).conj()));
  
  vertex(1,0,fac*(ep1*v->epsParent(0).conj()));
  vertex(1,1,fac*(ep1*v->epsParent(1).conj()));
  vertex(1,2,fac*(ep1*v->epsParent(2).conj()));
  
  vertex(2,0,fac*(ep2*v->epsParent(0).conj()));
  vertex(2,1,fac*(ep2*v->epsParent(1).conj()));
  vertex(2,2,fac*(ep2*v->epsParent(2).conj()));

  return ;

}




