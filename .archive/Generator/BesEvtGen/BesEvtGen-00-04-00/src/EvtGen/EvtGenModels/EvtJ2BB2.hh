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
// Module: EvtJ2BB2.hh
//
// Description: Routine to decay J/psi-> B_10 bar B_10 using helicity amplitude.
//      
// Modification history:
//
//    Pang C.-Y., Ping R.-G.    Apr, 2007       Module created
//
//------------------------------------------------------------------------

#ifndef EVTJ2BB2_HH
#define EVTJ2BB2_HH

#include "EvtGenBase/EvtDecayAmp.hh"


class EvtParticle;

class EvtJ2BB2:public  EvtDecayAmp  {

public:

  EvtJ2BB2() {}
  virtual ~EvtJ2BB2();

  void getName(std::string& name);
  EvtDecayBase* clone();

  void decay(EvtParticle *p); 
  void init();
  void init(int,double*);
//  void initProbMax();

  double alpha;
  double u;
 

};

#endif

