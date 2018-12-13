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
#ifndef EVTEDM_HH
#define EVTEDM_HH

#include "EvtGenBase/EvtDecayAmp.hh"

class EvtParticle;

class EvtEDM:public  EvtDecayAmp  {

public:

  EvtEDM() {}
  virtual ~EvtEDM();

  void getName(std::string& name);
  EvtDecayBase* clone();

  void initProbMax();
  void init();
  void decay(EvtParticle *p); 

};

#endif
