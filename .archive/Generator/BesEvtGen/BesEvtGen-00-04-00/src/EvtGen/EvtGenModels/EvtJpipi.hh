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
// Module: EvtJpipi.cc
//
// Description: Routine to decay psi(2S)-> J/psi pi pi using chiral Lagrangian and 
//      information by fitting the exp. data
//              
// Modification history:
//
//    Ping R.-G.       December, 2006       Module created
//
//------------------------------------------------------------------------
#ifndef EVTJPIPI_HH
#define EVTJPIPI_HH

#include "EvtGenBase/EvtDecayAmp.hh"

class EvtParticle;

class EvtJpipi:public  EvtDecayAmp  {

public:

  EvtJpipi() {}
  virtual ~EvtJpipi();

  void getName(std::string& name);
  EvtDecayBase* clone();

  void decay(EvtParticle *p); 
  void init();
  void initProbMax();

};

#endif

