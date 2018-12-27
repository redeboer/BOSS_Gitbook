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
// Module: EvtGen/EvtUclaII.hh
//
// Description:
//
// Modification history:
//
//    DJL/RYD     August 11, 1998         Module created
//
//------------------------------------------------------------------------

#ifndef EVTUCLAII_HH
#define EVTUCLAII_HH

#include "EvtGenBase/EvtDecayIncoherent.hh"

class EvtParticle;

class EvtUclaII:public  EvtDecayIncoherent  {

public:
  
  EvtUclaII() {}
  virtual ~EvtUclaII();
  
  void getName(std::string& name);
  EvtDecayBase* clone();

  void init();
  void initProbMax();

  void decay(EvtParticle *p); 

};

#endif


