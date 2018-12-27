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
// Module: EvtJ2BB3.cc
//
// Description: Routine to decay J/psi-> B_8 bar B_10 with using the
//              GVDM approach.(see J.-G. Korner Z.Physik C-Particles 
//              and Fields 33,529-535(1987))
//      
      
// Modification history:
//
//    Pang C.-Y., Ping R.-G.    Mar, 2007       Module created
//
//------------------------------------------------------------------------

#ifndef EVTJ2BB3_HH
#define EVTJ2BB3_HH

#include "EvtGenBase/EvtDecayAmp.hh"


class EvtParticle;

class EvtJ2BB3:public  EvtDecayAmp  {

public:

  EvtJ2BB3() {}
  virtual ~EvtJ2BB3();

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

