//--------------------------------------------------------------------------
// pingrg-2014-10-16
// Model REXC : used to decay vgam to the final state as in ConExc model
// Name: REXC: R-scan exclusive decay model    
// Decay cards:
//  Decay vgam
//  1  REXC;
//  Enddecay
// particle vgam is mandatory
//------------------------------------------------------------------------

#ifndef EVTREXC_HH
#define EVTREXC_HH

#include "EvtGenBase/EvtDecayIncoherent.hh"
#include "EvtGenModels/EvtConExc.hh"
class EvtParticle;

class EvtRexc:public  EvtDecayIncoherent  {

public:
  
  EvtRexc() {}
  virtual ~EvtRexc();

  void getName(std::string& name);

  EvtDecayBase* clone();

  void initProbMax();

  void init();

  void decay(EvtParticle *p); 

  double baryonAng(double mx);
  bool angularSampling(EvtParticle* par);
  bool hadron_angle_sampling(EvtVector4R ppi, EvtVector4R pcm);
  bool meson_sampling(EvtVector4R pcm, EvtVector4R pi);
  bool VP_sampling(EvtVector4R pcm, EvtVector4R pi);
private:
  EvtConExc *myconexc;
  double cosp,cosk;
  EvtId _daugs[10],gamId;
};

#endif

