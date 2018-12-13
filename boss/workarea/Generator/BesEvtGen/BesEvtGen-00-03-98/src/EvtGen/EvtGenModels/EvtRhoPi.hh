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
// Module:  EvtRhoPi.cc
//
// Description: Jpsi or psi(2S) decays into 3 pions via rho(1--)pi
//
// Modification history:
//
//    Ping R.-G.       Apr., 2007       Module created
//
//------------------------------------------------------------------------

#ifndef EVTRHOPI_HH
#define EVTRHOPI_HH

#include "EvtGenBase/EvtDecayIncoherent.hh"
#include "EvtGenBase/EvtVector4R.hh"

class EvtParticle;

class EvtRhoPi:public  EvtDecayIncoherent  {

public:
  
  EvtRhoPi() {}
  virtual ~EvtRhoPi();

  void getName(std::string& name);

  EvtDecayBase* clone();

  void initProbMax();

  void init();

  void decay(EvtParticle *p); 

  double AmplitudeSquare(double ResonanceMass, double ResonanceWidth,double r1,double   
r2,double phase1,double phase2);

  EvtVector4R GetDaugMomLab(int i){return _p4Lab[i];}
  EvtVector4R GetDaugMomCM(int i){return _p4CM[i];}
  EvtVector4R GetDaugMomHel(int i){return _p4Hel[i];}


  int GetNdaug(){return _nd;}   

private:

 EvtVector4R _p4Lab[10],_p4CM[10],_p4Hel[10];
 int _nd;

};


#endif

