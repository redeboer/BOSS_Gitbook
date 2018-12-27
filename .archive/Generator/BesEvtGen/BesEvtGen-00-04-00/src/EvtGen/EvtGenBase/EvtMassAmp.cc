//-----------------------------------------------------------------------
// File and Version Information: 
//      $Id: EvtMassAmp.cc,v 1.2 2011/01/06 22:57:56 pingrg Exp $
// 
// Environment:
//      This software is part of the EvtGen package developed jointly
//      for the BaBar and CLEO collaborations. If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information:
//      Copyright (C) 1998 Caltech, UCSB
//
// Module creator:
//      Alexei Dvoretskii, Caltech, 2001-2002.
//-----------------------------------------------------------------------
#include "EvtGenBase/EvtPatches.hh"

#include "EvtGenBase/EvtMassAmp.hh"

EvtMassAmp::EvtMassAmp(const EvtPropBreitWignerRel& prop, const EvtTwoBodyVertex& vd)
  : EvtAmplitude<EvtPoint1D>()
  ,_prop(prop), _vd(vd), _vb(0)
  ,_useBirthFact(false), _useDeathFact(false)
  ,_useBirthFactFF(false), _useDeathFactFF(false)
  ,_fixUpMassForMax(false),_addfactor(0.)
{}

EvtMassAmp::EvtMassAmp(const EvtMassAmp& other)
  : EvtAmplitude<EvtPoint1D>(other)
  ,_prop(other._prop), _vd(other._vd)
  ,_vb(other._vb ? new EvtTwoBodyVertex(*other._vb) : 0)
  ,_useBirthFact(other._useBirthFact)
  ,_useDeathFact(other._useDeathFact)
  ,_useBirthFactFF(other._useBirthFactFF)
  ,_useDeathFactFF(other._useDeathFactFF)
  ,_fixUpMassForMax(other._fixUpMassForMax)
  ,_addfactor(other._addfactor)
{}


EvtMassAmp::~EvtMassAmp() 
{
  if(_vb) delete _vb;
}


EvtComplex EvtMassAmp::amplitude(const EvtPoint1D& p) const 
{
  // Modified vertex

  double m = p.value();
  // keep things from crashing..

  if ( m< (_vd.mA()+_vd.mB()) ) return EvtComplex(0.,0.);

  EvtTwoBodyKine vd(_vd.mA(),_vd.mB(),m);
  
  // Compute mass-dependent width for relativistic propagator

  EvtPropBreitWignerRel bw(_prop.m0(),_prop.g0()*_vd.widthFactor(vd)); 
  EvtComplex amp = bw.evaluate(m);


  // Birth vertex factors

  if(_useBirthFact) {

    assert(_vb);
    if ( (m+_vb->mB()) < _vb->mAB() ) {  
      EvtTwoBodyKine vb(m,_vb->mB(),_vb->mAB());
      double phsp= _vb->phaseSpaceFactor(vb,EvtTwoBodyKine::AB);
      amp *= phsp;

      if ( _fixUpMassForMax ) {
	amp *= sqrt((vb.p() / _vb->pD()));
      }
      if(_useBirthFactFF) {
	
	assert(_vb);
	amp *= _vb->formFactor(vb);
      }
    }
    else{
      if ( _vb->L() != 0 ) amp=0.;
    }
  }


  // Decay vertex factors

  if(_useDeathFact) {
    amp *= _vd.phaseSpaceFactor(vd,EvtTwoBodyKine::AB);
    if ( _fixUpMassForMax ) {
      amp *= sqrt((vd.p() / _vd.pD()));
    }
  }
  if(_useDeathFactFF) amp *= _vd.formFactor(vd);

  // AddFactorPn

  if(fabs(_addfactor) > 0.00000001){ //addFactorPn, pingrg-2011-1-6
    assert(_vb);
    double phsp;
    if ( (m+_vb->mB()) < _vb->mAB() ) {  
      EvtTwoBodyKine vb(m,_vb->mB(),_vb->mAB());
      phsp= vb.p(EvtTwoBodyKine::AB);
    } else{
      if ( _vb->L() != 0 ) phsp=0.;
    }

    amp *= pow(phsp,_addfactor);
    //   std::cout<<"EvtMassAmp::addFactorPn= "<<pow(phsp,_addfactor)<<std::endl;
   }
 
  return amp;
}






