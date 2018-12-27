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
// Module: EvtHAngSam3.hh
//
// Description: Routine to decay a particle  using the invariant mass distribution of histogram 
//    dimension one.  
//             
//              
// Modification history:
//
//    Ping R.-G.       December, 2007       Module created
//
//------------------------------------------------------------------------
#ifndef EVTHAngSam3_HH
#define EVTHAngSam3_HH

#include "EvtGenBase/EvtDecayIncoherent.hh"


class EvtParticle;

class EvtHAngSam3:public  EvtDecayIncoherent  {
   
public:
  
  EvtHAngSam3() {}
  virtual ~EvtHAngSam3();

  void getName(std::string& name);

  EvtDecayBase* clone();

  void initProbMax();

  void init();

  void decay(EvtParticle *p); 

  const char* setFileName();  //Defined by user for specify the root file name
  const char* setHpoint();  //Defined by user for specify the histor. id


};

#endif

