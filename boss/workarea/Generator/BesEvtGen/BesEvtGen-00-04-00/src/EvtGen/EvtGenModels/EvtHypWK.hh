//--------------------------------------------------------------------------
//
// Environment:
//      This software is part of models developed at BES collaboration
//      based on the EvtGen framework.  If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information: See EvtGen/BesCopyright
//      Copyright (A) 2006      Ping Rong-Gang, Pang Cai-Ying@IHEP
//
// Module: EvtHypWK.hh
//
// Description: To generate octet hyperon weak decays.
//
// Modification history:
//
//    Pang C.-Y., Ping R.-G.    Mar, 2007       Module created
//
//------------------------------------------------------------------------

#ifndef EVTHypWK_HH
#define EVTHypWK_HH

#include "EvtGenBase/EvtDecayAmp.hh"

class EvtParticle;

class EvtHypWK:public  EvtDecayAmp  {

public:

  EvtHypWK() {}
  virtual ~EvtHypWK();
//  void initProbMax();
  void getName(std::string& name);
  EvtDecayBase* clone();

  void init();
  void decay(EvtParticle *p); 
  void findMaxPro(EvtParticle *p);

};

#endif
