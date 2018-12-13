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
// Module: EvtVVSPwave.cc
//
// Description: Routine to decay vector-> vector scalar in Partial-wave
//              Routine to decay a vector into a vector and scalar.  Started
//              by ryd on Aug 20, 1996.
//
// Modification history:
//
//    Ping R.G. November 24, 2008       P-wave implement
//    RYD       November 24, 1996       Module created
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
#include "EvtGenModels/EvtVVSPwave.hh"
#include <string>
using std::endl;

EvtVVSPwave::~EvtVVSPwave() {}

void EvtVVSPwave::getName(std::string& model_name){

  model_name="VVS_PWAVE";     

}


EvtDecayBase* EvtVVSPwave::clone(){

  return new EvtVVSPwave;

}

void EvtVVSPwave::init(){

  // check that there are 6 arguments
  checkNArg(6);
  checkNDaug(2);

  checkSpinParent(EvtSpinType::VECTOR);
  checkSpinDaughter(0,EvtSpinType::VECTOR);
  checkSpinDaughter(1,EvtSpinType::SCALAR);
}

void EvtVVSPwave::initProbMax() {

  //probmax is 1.0 for all possible decays I think!

   setProbMax(1.0);

}      

void EvtVVSPwave::decay( EvtParticle *p){

  p->initializePhaseSpace(getNDaug(),getDaugs());

  EvtComplex as(getArg(0)*cos(getArg(1)),getArg(0)*sin(getArg(1)));
  EvtComplex ap(getArg(2)*cos(getArg(3)),getArg(2)*sin(getArg(3)));
  EvtComplex ad(getArg(4)*cos(getArg(5)),getArg(4)*sin(getArg(5)));

  EvtParticle *v,*s;
  v=p->getDaug(0);
  s=p->getDaug(1);

  if (ap!=EvtComplex(0.0,0.0)) {  // implement P-wave  pingrg,2008-11-24
  EvtVector4C epi[3],epf[3];

  epi[0] = p->eps(0);  //for parent polarization 
  epi[1] = p->eps(1);
  epi[2] = p->eps(2);

  epf[0] = v->eps(0);  //for sun vector polarization
  epf[1] = v->eps(1);
  epf[2] = v->eps(2);

  EvtVector4R momv = p->getDaug(0)->getP4();
  EvtVector3C p1(momv.get(1),momv.get(2),momv.get(3));

  EvtVector3C ei0(epi[0].get(1),epi[0].get(2),epi[0].get(3));
  EvtVector3C ei1(epi[1].get(1),epi[1].get(2),epi[1].get(3));
  EvtVector3C ei2(epi[2].get(1),epi[2].get(2),epi[2].get(3));

  EvtVector3C ef0(epf[0].get(1),epf[0].get(2),epf[0].get(3));
  EvtVector3C ef1(epf[1].get(1),epf[1].get(2),epf[1].get(3));
  EvtVector3C ef2(epf[2].get(1),epf[2].get(2),epf[2].get(3));

  double norm = 1/momv.d3mag();
  EvtVector3C q0 = p1.cross(ef0);
  EvtVector3C q1 = p1.cross(ef1);
  EvtVector3C q2 = p1.cross(ef2);

  vertex(0,0,norm*ei0.conj()*q0);
  vertex(0,1,norm*ei0.conj()*q1);
  vertex(0,2,norm*ei0.conj()*q2);

  vertex(1,0,norm*ei1.conj()*q0);
  vertex(1,1,norm*ei1.conj()*q1);
  vertex(1,2,norm*ei1.conj()*q2);

  vertex(2,0,norm*ei2.conj()*q0);
  vertex(2,1,norm*ei2.conj()*q1);
  vertex(2,2,norm*ei2.conj()*q2);

  } else if (as!=EvtComplex(0.0,0.0) || ad!=EvtComplex(0.0,0.0) ){// implemet s- and d-wave
    
    //  EvtParticle *v,*s;
    //  v=p->getDaug(0);
    //  s=p->getDaug(1);

  EvtTensor4C d,g;
  
  g.setdiag(1.0,-1.0,-1.0,-1.0);

  d=ad*((1.0/(v->getP4().d3mag()*v->getP4().d3mag()))*directProd(v->getP4(),v->getP4())+(1/3.0)*g)+
    as*g;

  EvtVector4C ep0,ep1,ep2;  
  
  ep0=d.cont1(p->eps(0));
  ep1=d.cont1(p->eps(1));
  ep2=d.cont1(p->eps(2));

  vertex(0,0,ep0.cont(v->eps(0).conj()));
  vertex(0,1,ep0.cont(v->eps(1).conj()));
  vertex(0,2,ep0.cont(v->eps(2).conj()));
  
  vertex(1,0,ep1.cont(v->eps(0).conj()));
  vertex(1,1,ep1.cont(v->eps(1).conj()));
  vertex(1,2,ep1.cont(v->eps(2).conj()));
  
  vertex(2,0,ep2.cont(v->eps(0).conj()));
  vertex(2,1,ep2.cont(v->eps(1).conj()));
  vertex(2,2,ep2.cont(v->eps(2).conj()));
  } else 
    {
      report(ERROR,"EvtGen") << "In EvtVectorToVectorScalar.cc"<<endl;
      report(ERROR,"EvtGen") << "Not all zero value for parameters are need!!"<<endl;
      ::abort();
    }
  return ;

}
