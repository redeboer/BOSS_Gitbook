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
#include "EvtGenBase/EvtPatches.hh"
#include <iostream>
#include <math.h>
#include <fstream>   
#include <stdio.h>
#include <stdlib.h>
#include "EvtGenBase/EvtEulerAngles.hh"
#include "EvtGenBase/EvtReport.hh"
using namespace std;  //::endl;

EvtEulerAngles::~EvtEulerAngles(){}
EvtEulerAngles::EvtEulerAngles(const EvtVector3R & Yaxis, const EvtVector3R & Zaxis){
  _Yaxis=Yaxis;
  _Zaxis=Zaxis;
   EulerAngles();
}

EvtEulerAngles::EvtEulerAngles( const EvtVector4R & Pyaxis, const EvtVector4R & Pzaxis){
  for (int i=1;i<4;i++){
    _Yaxis.set(i-1,Pyaxis.get(i));
    _Zaxis.set(i-1,Pzaxis.get(i));
  }
  EulerAngles();
}

EvtEulerAngles::EvtEulerAngles(){
}

double EvtEulerAngles::getAlpha(){
  return _alpha;
}

double EvtEulerAngles::getBeta(){
  return _beta;
}

double EvtEulerAngles::getGamma(){
  return _gamma;
}

void EvtEulerAngles::EulerAngles(){
  // to calculate Euler angles with y-convention, i.e. R=R(Z, alpha).R(Y,beta).R(Z,gamma)
  double pi=3.1415926;
    _ry=_Yaxis.d3mag();
    _rz=_Zaxis.d3mag();

    if(_ry==0 ||_rz==0) {
      report(ERROR,"") << "Euler angle calculation specified by zero modules of the axis!"<<endl;
      report(ERROR,"EvtGen") << "Will terminate execution!"<<endl;
      ::abort();
    }
   double tolerance=1e-10;
   bool Y1is0=fabs(_Yaxis.get(0))<tolerance;
   bool Y2is0=fabs(_Yaxis.get(1))<tolerance;
   bool Y3is0=fabs(_Yaxis.get(2))<tolerance;
   bool Z1is0=fabs(_Zaxis.get(0))<tolerance;
   bool Z2is0=fabs(_Zaxis.get(1))<tolerance;
   bool Z3is0=fabs(_Zaxis.get(2))<tolerance;

   if(Y1is0 && Y3is0 && Z1is0 && Z2is0 ){
     _alpha=0; _beta=0; _gamma=0;  
   return;
   }

   if( Z1is0 && Z2is0 && !Y2is0){
      _alpha=0; _beta=0;
      _gamma=acos(_Yaxis.get(0)/_ry);
	if(_Yaxis.get(1)<0) _gamma=2*pi - _gamma;
      return;
   }

  // For general case to calculate Euler angles  
  // to calculate beta  

   if(Z1is0 && Z2is0) {
     _beta=0;
   } else{ _beta =acos(_Zaxis.get(2)/_rz);}

   //to calculate alpha

   if(_beta==0){
     _alpha=0.0;
   }else {
     double cosalpha=_Zaxis.get(0)/_rz/sin(_beta);
     if(fabs(cosalpha)>1.0) cosalpha=cosalpha/fabs(cosalpha);
     _alpha=acos(cosalpha);
     if(_Zaxis.get(1)<0.0) _alpha=2*pi - _alpha;
   }

   //to calculate gamma, alpha=0 and beta=0 case has been calculated, so only alpha !=0 and beta !=0 case left

   double singamma=_Yaxis.get(2)/_ry/sin(_beta);
   double cosgamma=(-_Yaxis.get(0)/_ry-cos(_alpha)*cos(_beta)*singamma)/sin(_alpha);
  if(fabs(singamma)>1.0) singamma=singamma/fabs(singamma);
  _gamma=asin(singamma);
  if(singamma>0 && cosgamma<0 ) _gamma=pi - _gamma;  // _gamma>0
  if(singamma<0 && cosgamma<0 ) _gamma=pi - _gamma;  //_gamma<0
  if(singamma<0 && cosgamma>0 ) _gamma=2*pi + _gamma; //_gamma<0
 
  
}
