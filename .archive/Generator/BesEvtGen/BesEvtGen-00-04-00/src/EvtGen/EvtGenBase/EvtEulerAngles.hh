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
// Module:  EvtDIY.cc
//
// Description:  Class to calculate the Euler angles to rotate a system
//
// Modification history:
//
//    Ping R.-G.       December, 2007       Module created
//
//------------------------------------------------------------------------
//
#ifndef EVTEULERANGLES_HH
#define EVTEULERANGLES_HH
  
#include <iostream>
// #include <math.h>
#include "EvtGenBase/EvtVector4R.hh"
#include "EvtGenBase/EvtVector3R.hh"
  
//using std::endl;   
//using std::fstream; 

class EvtEulerAngles
{ 

public: 

//destructor
virtual ~EvtEulerAngles();

// constructor
EvtEulerAngles();
EvtEulerAngles( const EvtVector3R & Yaxis, const EvtVector3R & Zaxis);
EvtEulerAngles( const EvtVector4R & Pyaxis, const EvtVector4R & Pzaxis);
  void EulerAngles();
  double getAlpha();
  double getBeta();
  double getGamma();
private:
  EvtVector3R _Yaxis,_Zaxis;
  double _alpha,_beta,_gamma,_ry,_rz;
};

#endif
