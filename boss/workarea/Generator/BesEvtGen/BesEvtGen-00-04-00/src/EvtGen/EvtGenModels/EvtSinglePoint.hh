//--------------------------------------------------------------------------
//
//
// Module: EvtGen/EvtSinglePoint.hh
//
// Description:
//This is a special decay model to generate single particles with fixed momentum.
//
// Modification history:
//
//    DJL/RYD     August 11, 1998         Module created
//
//------------------------------------------------------------------------


#ifndef EVTSINGLEPOINT_HH
#define EVTSINGLEPOINT_HH

#include "EvtGenBase/EvtDecayIncoherent.hh"
class EvtParticle;

class EvtSinglePoint:public  EvtDecayIncoherent  {

public:

  EvtSinglePoint() {}
  virtual ~EvtSinglePoint();

  void getName(std::string& name);
  
  EvtDecayBase* clone();

  void decay(EvtParticle *p); 

  void init();

private:

  double px,py,pz;


};

#endif

