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
// Module: EvtPBB2.hh
//
// Description: Routine to decay pseudoscalar to  B_10 bar B_10 using 
//              helicity amplitude.
//      
// Modification history:
//
//     Ping R.-G.    Apr., 2007       Module created
//
//------------------------------------------------------------------------

#ifndef EVTPBB2_HH
#define EVTPBB2_HH

#include "EvtGenBase/EvtDecayAmp.hh"


class EvtParticle;

class EvtPBB2:public  EvtDecayAmp  {

public:

  EvtPBB2() {}
  virtual ~EvtPBB2();

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

