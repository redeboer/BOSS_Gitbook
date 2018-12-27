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
// Module: EvtHelPPJ.hh
//
// Description: To generate psi(2S)->J/psi sigma0->pi pi J/psi.
//
// Modification history:
//
//   Ping R.-G.    Mar, 2007       Module created
//
//------------------------------------------------------------------------

#ifndef EVTHelPPJ_HH
#define EVTHelPPJ_HH

#include "EvtGenBase/EvtDecayAmp.hh"

class EvtParticle;

class EvtHelPPJ:public  EvtDecayAmp  {

public:

  EvtHelPPJ() {}
  virtual ~EvtHelPPJ();
//  void initProbMax();
  void getName(std::string& name);
  EvtDecayBase* clone();

  void init();
  void decay(EvtParticle *p); 
  void findMaxPro(EvtParticle *p);
};

#endif
