//--------------------------------------------------------------------------
//
// Environment:
//      This software is part of models developed at BES collaboration
//      based on the EvtGen framework.  If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information: See EvtGen/BesCopyright
//      Copyright (A) 2014-2      Ping Rong-Gang
//
// Module: EvtHypWK.hh
//
// Description:  To generate decay tau->gamma mu with uses' helicity amplitudes, it can be used
//  for the decay Fermion -> photon Fermion .
//
// Modification history:
//
//    Pang C.-Y., Ping R.-G.    Mar, 2007       Module created
//
//------------------------------------------------------------------------

#ifndef EVTTAUGAMMU_HH
#define EVTTAUGAMMU_HH

#include "EvtGenBase/EvtDecayAmp.hh"

class EvtParticle;

class EvtTauGamMu:public  EvtDecayAmp  {

public:

  EvtTauGamMu() {}
  virtual ~EvtTauGamMu();
//  void initProbMax();
  void getName(std::string& name);
  EvtDecayBase* clone();

  void init();
  void decay(EvtParticle *p); 
  void findMaxPro(EvtParticle *p);

};

#endif
