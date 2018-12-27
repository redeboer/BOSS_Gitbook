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
// Module: EvtChi0BB1.cc
//
// Description: Routine to decay Xi1 to B8 B8bar.
//      
// Modification history:
//
//    Pang C.Y. and Ping R.-G.       April, 2007       Module created
//
//------------------------------------------------------------------------

#ifndef EVTChi1BB1_HH
#define EVTChi1BB1_HH

#include "EvtGenBase/EvtDecayAmp.hh"

class EvtParticle;

class EvtChi1BB1:public  EvtDecayAmp  {

public:

  EvtChi1BB1() {}
  virtual ~EvtChi1BB1();

  void getName(std::string& name);
  EvtDecayBase* clone();

  void decay(EvtParticle *p); 
  void init();
//  void initProbMax();
};

#endif

