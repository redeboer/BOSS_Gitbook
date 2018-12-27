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
// Module: EvtLundCharm.cc
//         the necessary file: jetset74.F,lund_crm1_evtgen.F
//                             fist.inc,gen.inc  mix.inc  stdhep.inc
// Description: Modified Lund model at tau-charm energy level, see
//               PHYSICAL REVIEW D, VOLUME 62, 034003
// Modification history:
//   
//   Ping R.-G.    Octo., 2007       Module created    
//
//------------------------------------------------------------------------

#ifndef EVTLUNDCHARM_HH
#define EVTLUNDCHARM_HH

#include "EvtGenBase/EvtDecayIncoherent.hh"


class EvtParticle;

typedef EvtDecayBase* EvtDecayBasePtr;

#include <iosfwd>
 
class EvtLundCharm:public  EvtDecayIncoherent  {

public:

  EvtLundCharm();
  virtual ~EvtLundCharm();

  void getName(std::string& name);
  EvtDecayBase* clone();
  void decay(EvtParticle *p); 

  std::string commandName();
  void command(std::string cmd);

  void init();

  void initProbMax();
  int getTotalEvt(){return nevt;}
  static void LundcrmInit(int f);

private:

  void store(EvtDecayBase* jsdecay);
  void fixPolarizations(EvtParticle* p);

  static int nlundcharmdecays;
  static EvtDecayBasePtr* lundcharmdecays;
  static int ntable;

  static int ncommand;
  static int lcommand;
  static std::string* commands;
  static int nevt;

};

#endif




