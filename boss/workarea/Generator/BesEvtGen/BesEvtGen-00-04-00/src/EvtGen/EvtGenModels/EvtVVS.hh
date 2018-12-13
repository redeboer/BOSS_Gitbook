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
// Module: VVS.hh
//
// Description: To define the helicity for vector(s=+1,-1)->vector(s=-1,0,1)+Pseudoscalar 
//
// Modification history:
//
//    Ping R.-G.       Apr., 2006       Module created
//
//------------------------------------------------------------------------
//
#include "EvtGenBase/EvtPatches.hh"
#include <math.h>
#include <stdlib.h>
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtKine.hh"

#include "EvtGenBase/EvtVector4C.hh"
#include "EvtGenBase/EvtVector4R.hh"
#include "EvtGenBase/EvtTensor4C.hh"
#include "EvtGenBase/EvtHelSys.hh"
#include <string>
using namespace std;  


class VVS{
 public:
 VVS(EvtVector4R pd1, EvtVector4R pd2,EvtVector4R pd3,double ResonanceMass, double ResonanceWidth,double r1,double r2,double phase1,double phase2){
////////////////////////////////////////////////////////////////////////////////////
//     For the decay 1-- -->R(1--) +Pseudoscalar-->3 Pseudoscalars
//  pd1,pd2,pd3: four momentum vector for three daughters
//   ResonanceMass,  ResonanceWidth : resonance parameter for intermediate state R(1--)
//  r1,r2      : the relative coupling strength for two resonaces R R.P.T another R
//  phase1,phase2: the relatev phase of two phases R.P.T another R
///////////////////////////////////////////////////////////////////////////////////////////
 _pd[0]=pd1;
 _pd[1]=pd2;
 _pd[2]=pd3;
 _res[0]=ResonanceMass;
 _res[1]=ResonanceWidth;
 _par[0]=r1;
 _par[1]=phase1;
 _par[2]=r2;
 _par[3]=phase2;
 }
 double Fij(int i, int j, double r);
 double R00(double r);
 EvtComplex  amps1(int m, int i, int j);
 double amps( );

 private:
 EvtVector4R _pd[5];
 double _res[5];
 double _par[5];
};

  
