//--------------------------------------------------------------------------
//
// Environment:
//      This software is part of models developed at BES collaboration
//      based on the EvtGen framework.  If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information: See CVS repository
//      Copyright (A) 2011      Ping Rong-Gang
//
// Module: EvtOpenCharm.cc
//
// Description: Routine to decay charmonium-> DD, DDpi according the
// cross section measurement by CLEO  PRD 80, 072001.
//
// Modification history:
//
//    Ping R.-G.    December, 2011       Module created
//
//------------------------------------------------------------------------
//


#ifndef EVTOPENCHARM_HH
#define EVTOPENCHARM_HH

#include "EvtGenBase/EvtDecayIncoherent.hh"


class EvtParticle;

typedef EvtDecayBase* EvtDecayBasePtr;

#include <iosfwd>
 
class EvtOpenCharm:public  EvtDecayIncoherent  {

public:

  EvtOpenCharm();
  virtual ~EvtOpenCharm();

  void getName(std::string& name);
  EvtDecayBase* clone();
  void decay(EvtParticle *p); 

  std::string commandName();
  void command(std::string cmd);

  void init();

  void initProbMax();
  int getTotalEvt(){return nevt;}
  static void OpencrmInit(int f);

  static int myiter;
  
  bool isbelong(EvtId myid);
  int which_mode(EvtId myid);

private:

  void store(EvtDecayBase* jsdecay);
  void fixPolarizations(EvtParticle* p);

  static int nOpencharmdecays;
  static EvtDecayBasePtr* Opencharmdecays;
  static int ntable;

  static int ncommand;
  static int lcommand;
  static std::string* commands;
  static int nevt;

  int theMode;
  std::vector<EvtId> theId;
  int ndaugjs;
  static std::vector<EvtId> mypar;
  static std::vector<int> vmode;
  static std::vector<double> Vcms;
  int _index;
};

#endif




