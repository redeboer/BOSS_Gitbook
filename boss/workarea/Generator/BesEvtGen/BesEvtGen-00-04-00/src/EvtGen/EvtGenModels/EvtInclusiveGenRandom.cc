//*********************************************
//
// Environment:
//      This software is part of models developed at BES collaboration
//      based on the EvtGen framework.  If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information: See EvtGen/BesCopyright
//      Copyright (A) 2007      Ping Rong-Gang @IHEP
//
// Module:  EvtRlu.cc
//
// Description: to unify the random engine
//     Rlu in lund_crm1_evtgen.F as EvtGen 
//     pyr in pythia model
//     tauola random engine is redifined with rlu(0), see tauola2.4.F
//   If the unified engine is set by user, then the Flat is replaced with uers' engine
//   See EvtGen.cc
// Modification history:
//
//    Ping R.-G.       Nov., 2007       Module created
//
//------------------------------------------------------------------------

#include "EvtGenBase/EvtRandom.hh"
#include "EvtGenBase/EvtRandomEngine.hh"
#include<iostream>

extern "C" {
  extern double begran_(int *);
}

extern "C" {
  extern double phoran_(int *);  //Photos
}

extern "C" {
  extern double pyr_(int *);     //pythia
}


extern "C"{
  extern double rlu_(int *);      //jetset74
}


double begran_(int *){
  return EvtRandom::Flat();
}
 
double phoran_(int *){
  return EvtRandom::Flat();
}
 

double  rlu_(int *){
  double rdm= EvtRandom::Flat();
  // std::cout<<"rlu_ = "<<rdm<<std::endl;
  return rdm;
}  


double  pyr_(int *){
 return EvtRandom::Flat();
} 
