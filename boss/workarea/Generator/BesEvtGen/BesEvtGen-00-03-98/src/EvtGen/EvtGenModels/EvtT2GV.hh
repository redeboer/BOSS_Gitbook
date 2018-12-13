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
// Module:  EvtT2GV.cc
//
// Description: Routine to decay chi_c2-> gamma J/psi  
//      
// Modification history:
//
//    Ping R.-G.       December, 2006       Module created
//
//------------------------------------------------------------------------

#ifndef EVTT2GV_HH
#define EVTT2GV_HH

#include "EvtGenBase/EvtDecayAmp.hh"
//#include "EvtGenBase/EvtDecayIncoherent.hh"

class EvtParticle;

class EvtT2GV:public  EvtDecayAmp  {
//class EvtT2GV:public  EvtDecayIncoherent  {

public:
  
  EvtT2GV() {}
  virtual ~EvtT2GV();

  void getName(std::string& name);

  EvtDecayBase* clone();

  void initProbMax();

  void init();

  void decay(EvtParticle *p); 

};

#endif

