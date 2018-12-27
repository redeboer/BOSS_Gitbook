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
// Module: EvtGen/EvtSingleParticle.hh
//
// Description:
//This is a special decay model to generate single particles.
//
// Modification history:
//
//    DJL/RYD     August 11, 1998         Module created
//
//------------------------------------------------------------------------


#ifndef EVTSINGLEPARTICLE2_HH
#define EVTSINGLEPARTICLE2_HH

#include "EvtGenBase/EvtDecayIncoherent.hh"
class EvtParticle;

class EvtSingleParticle2:public  EvtDecayIncoherent  {

public:

  EvtSingleParticle2() {}
  virtual ~EvtSingleParticle2();

  void getName(std::string& name);
  
  EvtDecayBase* clone();

  void decay(EvtParticle *p); 

  void init();

private:

  double pxymax,pxymin,pmag;
  double thetamin,thetamax;
  double cthetamin,cthetamax;
  double phimin,phimax;

};

#endif

