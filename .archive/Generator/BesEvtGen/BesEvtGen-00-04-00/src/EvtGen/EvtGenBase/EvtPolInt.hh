//--------------------------------------------------------------------------
//
// Environment:
//      This software is part of models developed at BES collaboration
//      based on the EvtGen framework.  If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information: See EvtGen/BesCopyright
//      Copyright (A) 2006      Ping Rong-Gang, Pang Cai-Ying@IHEP
//
// Module: EvtEvtPloInt.hh
//
// Description: Routine to deal with polynomial interpolation 
//
// Modification history:
//
//    Ping R.-G.    December, 2010   Module created
//
//------------------------------------------------------------------------
#ifndef EVTPOLINT_HH
#define EVTPOLINT_HH


#include <cmath>
#include <string>
#include <iostream>
#include <vector>
using std::endl;
using std::vector;

class EvtPolInt{
public:
  EvtPolInt(vector<double> xi, vector<double> yi, double x){
    xx = x;
    size = xi.size();
    vx.clear();
    vy.clear();
    for(int i=0;i<size;i++){
      vx.push_back(xi[i]);
      vy.push_back(yi[i]);
    }
  } 
  virtual ~EvtPolInt(){}

  void polynomial(); //polynomial interpolation see: Numerical recipes in C++ page 82
  void ratint();
  vector <double> spline();
  void splint();
  double getvalue();
  double geterror();

private:
  vector <double> vx, vy;
  double xx,value,error;
  int size;

};



#endif
