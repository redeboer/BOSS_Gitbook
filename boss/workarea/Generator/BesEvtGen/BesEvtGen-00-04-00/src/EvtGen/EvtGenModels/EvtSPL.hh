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
// Module: EvtSPL.cc
//
// Description: Routine to decay Sigma0->photon+Lambda by helicity amplitude.
//      
// Modification history:
//
//    Ping R.-G.    Apr., 2006       Module created
//
//------------------------------------------------------------------------

#ifndef EVTSPL_HH
#define EVTSPL_HH

#include "EvtGenBase/EvtDecayAmp.hh"


class EvtParticle;

class EvtSPL:public  EvtDecayAmp  {

public:

  EvtSPL() {}
  virtual ~EvtSPL();

  void getName(std::string& name);
  EvtDecayBase* clone();

  void decay(EvtParticle *p); 
  void init();
  void init(int,double*);
  void initProbMax();

  double alpha;
  double u;
 

};

#endif

