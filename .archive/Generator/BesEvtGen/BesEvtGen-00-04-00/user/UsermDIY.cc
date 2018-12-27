//--------------------------------------------------------------------------
//
// Environment:
//      This software is part of models developed at BES collaboration
//      based on the EvtGen framework.  If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information: See EvtGen/BesCopyright
//      Copyright (A) 2006      Ping Rong-Gang
//
// Module: Modified DIY model, see EvtDeay.cc 
//
// Description: Routine to sample the decays using the amplitude specified by users
// This model allow use to specify the intermediate states
//
// Modification history:
//
// Ping R.-G.    4-25, 2010       Module created
//
//------------------------------------------------------------------------
#ifndef EVTDECAY_H
#define EVTDECAY_H

#include "EvtDecay.h"
#include "EvtGenBase/EvtVector4R.hh"
#include "EvtGenBase/EvtVector3R.hh"
#include "EvtGenBase/EvtHelSys.hh"
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtParticleFactory.hh"
#include "EvtGen.hh"
#include "EvtGenBase/EvtRandomEngine.hh"
#include "EvtGenBase/EvtDecayTable.hh"
#include "EvtGenBase/EvtEulerAngles.hh"
#include <stdlib.h>
#include <string.h>
using std::cout;
using std::endl;
using namespace std;

extern "C" {
  extern double myamps_(double *,double *,  double *);
}


double EvtDecay::CalAmpsMDIY(EvtParticle* part ){
  double amps=0;
//#include "LcLc.C"
//#include "LcLc2.C"
//#include "Lenu.C"
#include "etaphi.C"
//#include "omegapi0.C"
//std::cout<<"amps= "<<amps<<std::endl;
///////////======== don't touch follows =======================
    if(amps <=0){
    report(INFO,"EvtGen") << "Amplitude square of modified DIY should be positive, but found to be equal "<<amps<<endl;
    abort();
    } else {
    return amps;
    }
}


#endif
