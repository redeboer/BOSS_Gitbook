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
// Module: EvtPBB1.cc
//
// Description: Routine to decay Pseudoscalar-> B_8 bar B_8 using helicity amplitude.
//      
// Modification history:
//
//    Ping R.-G.    Apr., 2006       Module created
//
//------------------------------------------------------------------------

#ifndef EVTPBB1_HH
#define EVTPBB1_HH

#include "EvtGenBase/EvtDecayAmp.hh"


class EvtParticle;

class EvtPBB1:public  EvtDecayAmp  {

public:

  EvtPBB1() {}
  virtual ~EvtPBB1();

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

