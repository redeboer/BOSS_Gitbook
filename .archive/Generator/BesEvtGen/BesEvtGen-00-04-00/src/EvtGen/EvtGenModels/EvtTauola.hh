//--------------------------------------------------------------------------  
//   
// Environment:  
//      This software is part of models developed at BES collaboration  
//      based on the EvtGen framework.  If you use all or part  
//      of it, please give an appropriate acknowledgement.  
//        
// Copyright Information: See EvtGen/BesCopyright  
//      Copyright (A) 2006      Ping Rong-Gang
//        
// Module: EvtTauola.cc
//         the necessary file: tauola2.4.F
//                            
// Description: interface to the tauola package
//               
// Modification history:
//   
//   Ping R.-G.    2008-07-13       Module created    
//
//------------------------------------------------------------------------

#ifndef EVTTAUOLA_HH
#define EVTTAUOLA_HH

#include "EvtGenBase/EvtDecayIncoherent.hh"

class EvtParticle;

typedef EvtDecayBase* EvtDecayBasePtr;

#include <iosfwd>

class EvtTauola:public  EvtDecayIncoherent  {

public:

  EvtTauola();
  virtual ~EvtTauola();

  void getName(std::string& name);
  EvtDecayBase* clone();
  void decay(EvtParticle *p); 

  std::string commandName();
  void command(std::string cmd);

  void init();

  void initProbMax();

private:

  void store(EvtDecayBase* jsdecay);
  void fixPolarizations(EvtParticle* p);

  static int ntauoladecays;
  static EvtDecayBasePtr* tauoladecays;
  static int ntable;

  static int ncommand;
  static int lcommand;
  static std::string* commands;

};

#endif




