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
// Module: EvtDeBD.hh
//
// Description: To generate events for decuplet baryon deacys.
//
// Modification history:
//
//    Ping R.-G.    Apr., 2007       Module created
//
//------------------------------------------------------------------------

#ifndef EVTDEBD_HH
#define EVTDEBD_HH

#include "EvtGenBase/EvtDecayAmp.hh"

class EvtParticle;

class EvtDeBD:public  EvtDecayAmp  {

public:

  EvtDeBD() {}
  virtual ~EvtDeBD();

  void getName(std::string& name);
  EvtDecayBase* clone();

//  void initProbMax();
  void init();
  void decay(EvtParticle *p); 

};

#endif
