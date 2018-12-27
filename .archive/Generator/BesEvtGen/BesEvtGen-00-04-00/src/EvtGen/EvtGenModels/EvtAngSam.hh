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
// Module: EvtAngSam.cc
//
// Description: Routine to decay a particle to two bodies by sampling angular distribution in Lab 
//              system.
//              
// Modification history:
//
//    Ping R.-G.       December, 2006       Module created
//
//------------------------------------------------------------------------
#ifndef EVTAngSam_HH
#define EVTAngSam_HH

#include "EvtGenBase/EvtDecayIncoherent.hh"

class EvtParticle;

class EvtAngSam:public  EvtDecayIncoherent  {

public:
  
  EvtAngSam() {}
  virtual ~EvtAngSam();

  void getName(std::string& name);

  EvtDecayBase* clone();

  void initProbMax();

  void init();

  void decay(EvtParticle *p); 

};

#endif

