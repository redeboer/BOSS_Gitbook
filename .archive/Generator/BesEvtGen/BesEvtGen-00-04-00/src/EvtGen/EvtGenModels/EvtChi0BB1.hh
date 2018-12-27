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
// Description: Routine to decay Xi0 to B8 B8bar.
//      
// Modification history:
//
//    Pang C.Y. and Ping R.-G.       April, 2007       Module created
//
//------------------------------------------------------------------------

#ifndef EVTChi0BB1_HH
#define EVTChi0BB1_HH

#include "EvtGenBase/EvtDecayAmp.hh"

class EvtParticle;

class EvtChi0BB1:public  EvtDecayAmp  {

public:

  EvtChi0BB1() {}
  virtual ~EvtChi0BB1();

  void getName(std::string& name);
  EvtDecayBase* clone();

  void decay(EvtParticle *p); 
  void init();
//  void initProbMax();

};

#endif

