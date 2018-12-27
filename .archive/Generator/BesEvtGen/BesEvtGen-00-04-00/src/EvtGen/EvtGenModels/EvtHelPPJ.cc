//--------------------------------------------------------------------------
//
// Environment:
//      This software is part of models developed at BES collaboration
//      based on the EvtGen framework.  If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information: See EvtGen/BesCopyright
//      Copyright (A) 2006      Ping Rong-Gang, Pang Cai-Ying@IHEP
//
// Module: EvtHelPPJ.hh
//
// Description: To generate psi(2S)->J/psi sigma0->J/psi pi pi.
// using the chiral effective formula: Z.Phys.C73,541 by Mannel 
// Modification history:
//
//    Ping R.G. Apr. 2007  created
//------------------------------------------------------------------------
//
#include "EvtGenBase/EvtPatches.hh"
#include <stdlib.h>
#include <iostream>
#include <string>
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtGenKine.hh"
#include "EvtGenModels/EvtHelPPJ.hh"
#include "EvtGenBase/EvtDiracSpinor.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtVector4C.hh"
#include "EvtGenBase/EvtId.hh"
#include "EvtGenBase/EvtHelSys.hh"
#include "EvtGenBase/EvtdFunction.hh"

using std::cout;
using std::endl;

EvtHelPPJ::~EvtHelPPJ() {}

void EvtHelPPJ::getName(std::string& model_name){

  model_name="HelPPJ";     

}



EvtDecayBase* EvtHelPPJ::clone(){

  return new EvtHelPPJ;

}

void EvtHelPPJ::init(){

  // check that there are 0 arguments
  checkNArg(0);
  checkNDaug(3);

  checkSpinParent(EvtSpinType::VECTOR);
  
  checkSpinDaughter(0,EvtSpinType::VECTOR);
  checkSpinDaughter(1,EvtSpinType::SCALAR);
  checkSpinDaughter(2,EvtSpinType::SCALAR);

}


void EvtHelPPJ::decay(EvtParticle *p){

  p->initializePhaseSpace(getNDaug(),getDaugs());

  EvtParticle *l1, *l2,*l3;
  l1 = p->getDaug(0);
  l2 = p->getDaug(1);
  l3 = p->getDaug(2);
  EvtVector4R VP4 =l1->getP4(); //Vector Daughter
  EvtVector4R S1P4=l2->getP4(); //Scalar Daughter
  EvtVector4R S2P4=l3->getP4(); //Scalar Daughter
  EvtVector4R PRP4=p->getP4();  //Parent Momentum
  PRP4.set(1,0);
  PRP4.set(2,0);
  PRP4.set(3,0);

  
  double mpipi2=(S1P4+S2P4).mass2();
  double e1pi  =S1P4.get(0);  
  double e2pi  =S2P4.get(0);
  double g0    =0.3;
  double g1    =-0.11;
  double mpi   =0.1396;
  double mpsi  =3.097;
  double epsi  =VP4.get(0);
  double amp1  =(mpipi2-2*mpi*mpi)*g0/2+g1*e1pi*e2pi;
  double amp0  =amp1*epsi/mpsi;


  EvtHelSys angles(PRP4,VP4);   //using helicity sys.angles
  double tht=angles.getHelAng(1);
  double phi=angles.getHelAng(2);
  double gamma=0;

  vertex(0,0,Djmn(1, 1, 1,phi,tht,gamma)*amp1);
  vertex(0,1,Djmn(1, 1,-1,phi,tht,gamma)*amp1);
  vertex(0,2,Djmn(1, 1, 0,phi,tht,gamma)*amp0);
  vertex(1,0,Djmn(1,-1, 1,phi,tht,gamma)*amp1);
  vertex(1,1,Djmn(1,-1,-1,phi,tht,gamma)*amp1);
  vertex(1,2,Djmn(1,-1, 0,phi,tht,gamma)*amp0);
  vertex(2,0,Djmn(1, 0, 1,phi,tht,gamma)*amp1);
  vertex(2,1,Djmn(1, 0,-1,phi,tht,gamma)*amp1);
  vertex(2,2,Djmn(1, 0, 0,phi,tht,gamma)*amp0);

  return;

}

