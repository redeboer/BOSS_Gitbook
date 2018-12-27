//--------------------------------------------------------------------------
//
// Environment:
//      This software is part of the EvtGen package developed jointly
//      for the BaBar and CLEO collaborations.  If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information: See EvtGen/COPYRIGHT
//      Copyright (C) 1998      Caltech, UCSB
//
// Module: EvtGen/EvtmPhsp.hh
//
// Description:
//Class to handle generic phase space decays not done
//in other decay models by spcifing the mass region of the 
// daughter particles.
//
// Modification history:
//
//    DJL/RYD     August 11, 1998         Module created
//    Ping rg     December, 2007, Modified Phsp model
//------------------------------------------------------------------------

#ifndef EVTMPHSP_HH
#define EVTMPHSP_HH

#include "EvtGenBase/EvtDecayIncoherent.hh"

class EvtParticle;

class EvtmPhsp:public  EvtDecayIncoherent  {

public:
  
  EvtmPhsp() {}
  virtual ~EvtmPhsp();

  void getName(std::string& name);

  EvtDecayBase* clone();

  void initProbMax();

  void init();

  void decay(EvtParticle *p); 

private:
  double ma1,ma2,mb1,mb2,xmass1,xmass2;
  int nd1,nd2;
};

#endif

