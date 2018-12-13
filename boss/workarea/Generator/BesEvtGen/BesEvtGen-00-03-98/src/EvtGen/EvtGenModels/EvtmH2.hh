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
// Module: EvtMassH1.hh
// Description: Routine to decay a particle  using a scatter 
//    plot forn n-body decays (n>3).
//      
//              
// Modification history:
//
//    Ping R.-G.       Oct 2011       Module created
//
//------------------------------------------------------------------------
#ifndef EVTmH2_HH
#define EVTmH2_HH

#include "EvtGenBase/EvtDecayIncoherent.hh"


class EvtParticle;

class EvtmH2:public  EvtDecayIncoherent  {

public:
  
  EvtmH2() {}
  virtual ~EvtmH2();

  void getName(std::string& name);

  EvtDecayBase* clone();

  void initProbMax();

  void init();

  void decay(EvtParticle *p); 

  const char* setFileName();  //Defined by user for specify the root file name
  const char* setHpoint();  //Defined by user for specify the histor. id

private:
  int nbx,nby;
};

#endif

