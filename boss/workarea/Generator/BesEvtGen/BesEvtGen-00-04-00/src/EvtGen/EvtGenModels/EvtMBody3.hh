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
// Module: EvtBody3.cc  //Modified Body3 model
//
// usage:
// Users need to provide the MC and data file in NTuple style. The decays are described as
//                        A->  x1    +   x2     +  x3
//angular distribution:    costheta1, costheta2 ,costheta3
// mass distribution:          m12, m13, and m23, here, m12 ==> mass of x1 and x2, etc.
// these variables are defined as double type, the tree name is mc and data, repectively
// Modification history:
//
//    Ping R.-G.       Aug., 2010       Module created
//
//------------------------------------------------------------------------

#ifndef EVTMBODY3_HH
#define EVTMBODY3_HH
#include "TH1.h"
#include "TAxis.h"
#include "TH2.h"
#include "TH3.h"
#include "TFile.h"  
#include "TApplication.h"
#include "TROOT.h"
#include "TChain.h"
#include "EvtGenBase/EvtDecayIncoherent.hh"
#include "EvtGenBase/EvtNT3.hh"

class EvtParticle;
class EvtMBody3:public  EvtDecayIncoherent  {

public:
  
  EvtMBody3() {}
  virtual ~EvtMBody3();

  void getName(std::string& name);

  EvtDecayBase* clone();

  void initProbMax();

  void init();

  void decay(EvtParticle *p); 

  const char* setFileName(int i );  //Defined by user to specify the root file name

private:

};

#endif

