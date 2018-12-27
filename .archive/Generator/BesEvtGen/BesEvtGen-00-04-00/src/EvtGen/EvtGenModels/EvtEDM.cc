//--------------------------------------------------------------------------
//
// Environment:
//
// Module: EvtEDM.cc
//
// Description: tau pair production with EDM, see PRD55, 1643
//              L = tau_bar sigma^{mu,nu} gamma_5 tau
//
// Modification history:
//
//    Ping RG       Dec 23, 2013       Module created
//
//------------------------------------------------------------------------
//
#include "EvtGenBase/EvtPatches.hh"
#include <stdlib.h>
#include <iostream>
#include <string>
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtGenKine.hh"
#include "EvtGenModels/EvtEDM.hh"
#include "EvtGenBase/EvtDiracSpinor.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtVector4C.hh"
#include "EvtGenBase/EvtTensor4C.hh"

EvtEDM::~EvtEDM() {}

void EvtEDM::getName(std::string& model_name){

  model_name="EDM";     

}


EvtDecayBase* EvtEDM::clone(){

  return new EvtEDM;

}

void EvtEDM::init(){

  // check that there are 0 arguments
  checkNArg(4);  // mag(d_tau) arg(d_tau) mag(g_e)  arg(g_e)
  checkNDaug(2);

  checkSpinParent(EvtSpinType::VECTOR);
  
  checkSpinDaughter(0,EvtSpinType::DIRAC);
  checkSpinDaughter(1,EvtSpinType::DIRAC);

}

void EvtEDM::initProbMax(){

  setProbMax(1.0);

}

void EvtEDM::decay(EvtParticle *p){

  p->initializePhaseSpace(getNDaug(),getDaugs());

  EvtParticle *l1, *l2;
  l1 = p->getDaug(0);
  l2 = p->getDaug(1);

  double magtau = getArg(0);
  double phstau = getArg(1);
  double mage   = getArg(2);
  double phse   = getArg(3);
  EvtComplex ctau(magtau*cos(phstau),magtau*sin(phstau));
  EvtComplex ce(mage*cos(phse),mage*sin(phse));
    

  EvtTensor4C l11, l12, l21, l22;
  l11=EvtLeptonTg5Current(l1->spParent(0),l2->spParent(0));
  l12=EvtLeptonTg5Current(l1->spParent(0),l2->spParent(1));
  l21=EvtLeptonTg5Current(l1->spParent(1),l2->spParent(0));
  l22=EvtLeptonTg5Current(l1->spParent(1),l2->spParent(1));

  EvtVector4C eps0=p->eps(0);
  EvtVector4C eps1=p->eps(1);
  EvtVector4C eps2=p->eps(2);
  EvtVector4R parp4 = p->getP4();

  EvtTensor4C p4eps0=directProd(eps0,parp4); 
  EvtTensor4C p4eps1=directProd(eps1,parp4); 
  EvtTensor4C p4eps2=directProd(eps2,parp4); 

  double M2=p->mass();
  M2*=M2;
  double m2=l1->mass();
  m2*=m2;

  double norm=1.0/sqrt(2*M2+4*m2-4*m2*m2/M2);
  EvtComplex norm_edm = norm * ctau;
  EvtComplex norm_e   = norm * ce;

  // for ee->gamma* ->ll pair without EDM contributions
  EvtVector4C L11, L12, L21, L22;
  L11=EvtLeptonVCurrent(l1->spParent(0),l2->spParent(0));
  L12=EvtLeptonVCurrent(l1->spParent(0),l2->spParent(1));
  L21=EvtLeptonVCurrent(l1->spParent(1),l2->spParent(0));
  L22=EvtLeptonVCurrent(l1->spParent(1),l2->spParent(1));

  EvtComplex A000,A001,A010,A011,A100,A101,A110,A111,A200,A201,A210,A211;
  A000 = norm_e*(eps0*L11);
  A001 = norm_e*(eps0*L12);
  A010 = norm_e*(eps0*L21);
  A011 = norm_e*(eps0*L22);
  
  A100 = norm_e*(eps1*L11);
  A101 = norm_e*(eps1*L12);
  A110 = norm_e*(eps1*L21);
  A111 = norm_e*(eps1*L22);
  
  A200 = norm_e*(eps2*L11);
  A201 = norm_e*(eps2*L12);
  A210 = norm_e*(eps2*L21);
  A211 = norm_e*(eps2*L22);

  vertex(0,0,0,norm_edm*cont(p4eps0,l11) + A000);
  vertex(0,0,1,norm_edm*cont(p4eps0,l12) + A001);
  vertex(0,1,0,norm_edm*cont(p4eps0,l21) + A010);
  vertex(0,1,1,norm_edm*cont(p4eps0,l22) + A011);
  
  vertex(1,0,0,norm_edm*cont(p4eps1,l11) + A100);
  vertex(1,0,1,norm_edm*cont(p4eps1,l12) + A101);
  vertex(1,1,0,norm_edm*cont(p4eps1,l21) + A110);
  vertex(1,1,1,norm_edm*cont(p4eps1,l22) + A111);
  
  vertex(2,0,0,norm_edm*cont(p4eps2,l11) + A200);   
  vertex(2,0,1,norm_edm*cont(p4eps2,l12) + A201);
  vertex(2,1,0,norm_edm*cont(p4eps2,l21) + A210);
  vertex(2,1,1,norm_edm*cont(p4eps2,l22) + A211);
  
  return;

}













