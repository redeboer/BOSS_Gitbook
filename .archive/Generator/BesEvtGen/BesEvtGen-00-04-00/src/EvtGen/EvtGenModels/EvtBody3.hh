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
// Module: EvtBody3.cc
//
// Description: Routine to decay a particle into three bodies  using the Dalitz plots and two particle
//             angular distributions.             
//              
// Modification history:
//
//    Ping R.-G.       Mar, 2007       Module created
//
//------------------------------------------------------------------------
#ifndef EVTBODY3_HH
#define EVTBODY3_HH

#include "EvtGenBase/EvtDecayIncoherent.hh"

class EvtParticle;

class EvtBody3:public  EvtDecayIncoherent  {

public:
  
  EvtBody3() {}
  virtual ~EvtBody3();

  void getName(std::string& name);

  EvtDecayBase* clone();

  void initProbMax();

  void init();

  void decay(EvtParticle *p); 

  const char* setFileName();  //Defined by user to specify the root file name
  const char* setHpoint();    //Defined by user to specify the histor. id
  const char* setDaugAng(int i); //Defined by user to specify the daughter angular distr.
  int setDaugAngNo();

  int*  setDaugPair();  //Defined by user for specify the histo. point


};

#endif

