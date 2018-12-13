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
// Module:  EvtJTO3P.cc
//
// Description: This Model describles the processes: J/psi or psip->3Pi.
//
// Modification history:
//
// Ping R.-G.    Dec. 2007       Module created
//------------------------------------------------------------------------

#ifndef EVTJTO3P_HH
#define EVTJTO3P_HH
#include "EvtGenBase/EvtDecayAmp.hh"
#include "EvtGenBase/EvtVector4R.hh"
#include "EvtGenBase/EvtHelSys.hh"
class EvtParticle;

class EvtJTO3P:public  EvtDecayAmp {

public:
  
  EvtJTO3P() {}
  virtual ~EvtJTO3P();

  void getName(std::string& name);

  EvtDecayBase* clone();

  //  void initProbMax();

  void init();

  void decay(EvtParticle *p); 
  void init(int,double*);

  //  int GetNdaug(){return _nd;}   

};


#endif

