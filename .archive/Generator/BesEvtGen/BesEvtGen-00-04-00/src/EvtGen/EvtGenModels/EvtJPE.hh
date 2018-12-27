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
// Module: EvtJPE.cc
//
//Description: Routine to decay Vector->photon +pseudoscalar, e.g. J/psi to gamma eta or eta_c
//      
// Modification history:
//
//    Ping R.-G.       Apr., 2007       Module created
//
//------------------------------------------------------------------------

#ifndef EVTJPE_HH
#define EVTJPE_HH

#include "EvtGenBase/EvtDecayAmp.hh"

class EvtParticle;

class EvtJPE:public  EvtDecayAmp  {

public:

  EvtJPE() {}
  virtual ~EvtJPE();

  void getName(std::string& name);
  EvtDecayBase* clone();

  void decay(EvtParticle *p); 
  void init();
//  void initProbMax();

};

#endif

