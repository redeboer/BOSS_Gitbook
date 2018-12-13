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
// Module: EvtChi0BB2.cc
//
// Description: Routine to decay Xi1 to B10 B10bar.
//      
// Modification history:
//
//    Pang C.Y. and Ping R.-G.       April, 2007       Module created
//
//------------------------------------------------------------------------

#ifndef EVTChi2BB2_HH
#define EVTChi2BB2_HH

#include "EvtGenBase/EvtDecayAmp.hh"

class EvtParticle;

class EvtChi2BB2:public  EvtDecayAmp  {

public:

  EvtChi2BB2() {}
  virtual ~EvtChi2BB2();

  void getName(std::string& name);
  EvtDecayBase* clone();

  void decay(EvtParticle *p); 
  void init();
//  void initProbMax();

};

#endif

