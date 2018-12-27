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
// Module: EvtGen/EvtPhsp.hh
//
// Description:
//Class to handle generic phase space decays not done
//in other decay models.
//
// Modification history:
//
//    DJL/RYD     August 11, 1998         Module created
//
//------------------------------------------------------------------------

#ifndef EVTCALHELAMP_HH
#define EVTCALHELAMP_HH

#include "EvtGenBase/EvtDecayIncoherent.hh"
#include "EvtGenBase/EvtBlattWeisskopf.hh"

#include <vector>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <math.h>


using namespace std;
class EvtParticle;

class EvtCalHelAmp:public  EvtDecayIncoherent  {

public:
  
  EvtCalHelAmp() {}
  virtual ~EvtCalHelAmp();

  void getName(std::string& name);

  EvtDecayBase* clone();

  void initProbMax();

  void init();
  void decay(EvtParticle *p); 
  std::vector<double> firstder(std::vector<double> vh);
  static double _H2,_H1,_H0,_H2err,_H1err,_H0err;
  static int nevt;
double getBlattRatio(int L, double R, double pmag,double pmag0);
double blattfactor(int L, double R, double pmag);
double phsp(double m0,double m1, double m2);

private:
  vector<vector<double > > VC;
  int nd;
  int flag;
};

#endif

