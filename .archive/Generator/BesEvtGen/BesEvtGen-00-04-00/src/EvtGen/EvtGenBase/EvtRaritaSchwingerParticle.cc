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
// Module: EvtRaritaSchwingerParticle.cc
//
// Description: Class to describe spin 1/2 particles.
//
// Modification history:
//
//   RYD     August 9, 2000         Module created
//
//------------------------------------------------------------------------
// 
#include "EvtGenBase/EvtPatches.hh"
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "EvtGenBase/EvtComplex.hh"
#include "EvtGenBase/EvtRaritaSchwingerParticle.hh"
#include "EvtGenBase/EvtDiracSpinor.hh"
#include "EvtGenBase/EvtVector4R.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtSpinDensity.hh"
using std::endl;

EvtRaritaSchwingerParticle::~EvtRaritaSchwingerParticle(){
}


EvtRaritaSchwingerParticle::EvtRaritaSchwingerParticle(){

  return;
  
}

void EvtRaritaSchwingerParticle::init(EvtId id,const EvtVector4R& p4){

  _validP4=true;
  setp(p4);
  setpart_num(id);

  if (EvtPDL::getStdHep(id)==0){
    report(ERROR,"EvtGen") << "Error in EvtRaritaSchwingerParticle::init, part_n="
                           << id.getId()<<endl;
    ::abort();
  }

  static EvtVector4R e1(0.0,1.0,0.0,0.0);
  static EvtVector4R e2(0.0,0.0,1.0,0.0);
  static EvtVector4R e3(0.0,0.0,0.0,1.0);
  

  if (EvtPDL::getStdHep(id)>0){  

    EvtDiracSpinor u1,u2;

    u1.set(EvtComplex(sqrt(2.0*mass()),0.0),EvtComplex(0.0,0.0),
	   EvtComplex(0.0,0.0),EvtComplex(0.0,0.0));
    u2.set(EvtComplex(0.0,0.0),EvtComplex(sqrt(2.0*mass()),0.0),
	   EvtComplex(0.0,0.0),EvtComplex(0.0,0.0));

    

    _spinorRest[0]=dirProd(e3,u1+u2);
    _spinorRest[1]=dirProd(e1+EvtComplex(0.0,1.0)*e2,u1);
    _spinorRest[2]=dirProd(e1-EvtComplex(0.0,1.0)*e2,u2);
    _spinorRest[3]=dirProd(e1,(u1+u2))+dirProd(EvtComplex(0.0,1.0)*e2,(u1-u2));
    
    _spinor[0]=boostTo(_spinorRest[0],p4);
    _spinor[1]=boostTo(_spinorRest[1],p4);
    _spinor[2]=boostTo(_spinorRest[2],p4);
    _spinor[3]=boostTo(_spinorRest[3],p4);

  }
  else{

    EvtDiracSpinor u1,u2;

    u1.set(EvtComplex(0.0,0.0),EvtComplex(0.0,0.0),
	   EvtComplex(sqrt(2.0*mass()),0.0),EvtComplex(0.0,0.0));
    u2.set(EvtComplex(0.0,0.0),EvtComplex(0.0,0.0),
	   EvtComplex(0.0,0.0),EvtComplex(sqrt(2.0*mass()),0.0));

    

    _spinorRest[0]=dirProd(e3,(u1+u2));
    _spinorRest[1]=dirProd(e1+EvtComplex(0.0,1.0)*e2,u1);
    _spinorRest[2]=dirProd(e1-EvtComplex(0.0,1.0)*e2,u2);
    _spinorRest[3]=dirProd(e1,(u1+u2))+dirProd(EvtComplex(0.0,1.0)*e2,(u1-u2));
    
    _spinor[0]=boostTo(_spinorRest[0],p4);
    _spinor[1]=boostTo(_spinorRest[1],p4);
    _spinor[2]=boostTo(_spinorRest[2],p4);
    _spinor[3]=boostTo(_spinorRest[3],p4);

  }

  setLifetime();
}


EvtRaritaSchwinger EvtRaritaSchwingerParticle::spRSParent(int i) const {

  return _spinor[i];

}

EvtRaritaSchwinger EvtRaritaSchwingerParticle::spRS(int i) const {

  return _spinorRest[i];

}



EvtSpinDensity EvtRaritaSchwingerParticle::rotateToHelicityBasis() const{

  double sqmt2=sqrt(2.0*(this->getP4().mass()));
  
  EvtDiracSpinor spplus;
  EvtDiracSpinor spminus;
      
  if (EvtPDL::getStdHep(getId())>0){  
    spplus.set(1.0,0.0,0.0,0.0);
    spminus.set(0.0,1.0,0.0,0.0);
  } else {
    spplus.set(0.0,0.0,1.0,0.0);
    spminus.set(0.0,0.0,0.0,1.0);
  }

  EvtVector4C eplus(0.0,-1.0/sqrt(2.0),EvtComplex(0.0,-1.0/sqrt(2.0)),0.0);
  EvtVector4C ezero(0.0,0.0,0.0,1.0);
  EvtVector4C eminus(0.0,1.0/sqrt(2.0),EvtComplex(0.0,-1.0/sqrt(2.0)),0.0);

  EvtRaritaSchwinger sppp=dirProd(eplus,spplus);
  EvtRaritaSchwinger spp=dirProd(sqrt(2.0/3.0)*ezero,spplus)+
    dirProd(sqrt(1.0/3.0)*eplus,spminus);
  EvtRaritaSchwinger spm=dirProd(sqrt(2.0/3.0)*ezero,spminus)+
    dirProd(sqrt(1.0/3.0)*eminus,spplus);
  EvtRaritaSchwinger spmm=dirProd(eminus,spminus);

  //

  EvtSpinDensity R;
  R.SetDim(4);


  for ( int i=0; i<4; i++) {
    R.Set(0,i,(sppp*_spinorRest[i])/sqmt2);
    R.Set(1,i,(spp*_spinorRest[i])/sqmt2);
    R.Set(2,i,(spm*_spinorRest[i])/sqmt2);
    R.Set(3,i,(spmm*_spinorRest[i])/sqmt2);
  }

  return R;

}


EvtSpinDensity EvtRaritaSchwingerParticle::rotateToHelicityBasis(double alpha,
								 double beta,
								 double gamma) const{

  EvtDiracSpinor spplus;
  EvtDiracSpinor spminus;
      
  if (EvtPDL::getStdHep(getId())>0){  
    spplus.set(1.0,0.0,0.0,0.0);
    spminus.set(0.0,1.0,0.0,0.0);
  } else {
    spplus.set(0.0,0.0,1.0,0.0);
    spminus.set(0.0,0.0,0.0,1.0);
  }

  EvtVector4C eplus(0.0,-1.0/sqrt(2.0),EvtComplex(0.0,-1.0/sqrt(2.0)),0.0);
  EvtVector4C ezero(0.0,0.0,0.0,1.0);
  EvtVector4C eminus(0.0,1.0/sqrt(2.0),EvtComplex(0.0,-1.0/sqrt(2.0)),0.0);

  EvtRaritaSchwinger sppp=dirProd(eplus,spplus);
  EvtRaritaSchwinger spp=dirProd(sqrt(2.0/3.0)*ezero,spplus)+
    dirProd(sqrt(1.0/3.0)*eplus,spminus);
  EvtRaritaSchwinger spm=dirProd(sqrt(2.0/3.0)*ezero,spminus)+
    dirProd(sqrt(1.0/3.0)*eminus,spplus);
  EvtRaritaSchwinger spmm=dirProd(eminus,spminus);
  //

  sppp.applyRotateEuler(alpha,beta,gamma);
  spp.applyRotateEuler(alpha,beta,gamma);
  spm.applyRotateEuler(alpha,beta,gamma);
  spmm.applyRotateEuler(alpha,beta,gamma);

  EvtSpinDensity R;
  R.SetDim(4);

  double sqmt2=sqrt(2.0*(this->getP4().mass()));

  for ( int i=0; i<4; i++) {
    R.Set(0,i,(sppp*_spinorRest[i])/sqmt2);
    R.Set(1,i,(spp*_spinorRest[i])/sqmt2);
    R.Set(2,i,(spm*_spinorRest[i])/sqmt2);
    R.Set(3,i,(spmm*_spinorRest[i])/sqmt2);
  }

  return R;

}

