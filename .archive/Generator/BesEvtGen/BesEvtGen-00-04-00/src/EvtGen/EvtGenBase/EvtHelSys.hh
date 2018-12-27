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
// Description:  Class to boost a daughter momentum into the mother helicity system
//
// Modification history:
//
//    Ping R.-G.       December, 2006       Module created
//
//------------------------------------------------------------------------
//

#ifndef EVTHELSYS_HH
#define EVTHELSYS_HH

// #include "EvtGenBase/EvtPatches.hh"
#include <iostream>
// #include <math.h>
#include "EvtGenBase/EvtVector4R.hh"
#include "EvtGenBase/EvtVector3R.hh"

#include <fstream>
// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/stat.h>
// #include "EvtGenBase/EvtParticle.hh"
// #include "EvtGenBase/EvtReport.hh"
// #include "EvtGenBase/EvtCPUtil.hh"
// #include "EvtGenBase/EvtParticleFactory.hh"

using std::endl;
using std::fstream;
EvtComplex Djmn(int j, int m, int n, double phi,double theta,double gamma);
class EvtHelSys
{

friend double djmn(int j, int m, int n, double theta);
friend double djmn(double j, double m, double n, double theta);
friend EvtComplex Djmn(int j, int m, int n, double phi,double theta,double gamma);
friend EvtComplex Djmn(double j, double m, double n, double phi,double theta,double gamma);
public:

//destructor
virtual ~EvtHelSys();

// constructor
EvtHelSys();
EvtHelSys( const EvtVector4R & p4p, const EvtVector4R & p4d);

double getHelAng(int i), Angles(EvtVector4R, int);

EvtVector4R checkparent(),checkdaug(),checkst(int i);

EvtVector4R Helrotate(EvtVector4R p1, double phi, double theta);
private:
EvtVector4R _p4parent,_p4daug,_bp4p,_rotatep4p,_rotatep4d,_bst;

};

#endif

