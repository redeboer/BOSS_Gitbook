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
// Module: EvtP2GC0.cc
//
// Description: Routine to decay psi(2S) to gamma chi_c0 by assuming E1 dominant
//      
// Modification history:
//
//    Ping R.-G.       December, 2006       Module created
//
//------------------------------------------------------------------------

#ifndef EVTP2GC0_HH
#define EVTP2GC0_HH

#include "EvtGenBase/EvtDecayAmp.hh"

class EvtParticle;

class EvtP2GC0:public  EvtDecayAmp  {

public:

  EvtP2GC0() {}
  virtual ~EvtP2GC0();

  void getName(std::string& name);
  EvtDecayBase* clone();

  void decay(EvtParticle *p); 
  void init();
  void initProbMax();

};

#endif

