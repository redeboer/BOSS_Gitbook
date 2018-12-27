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
#include "EvtGenModels/EvtVVS.hh"
#include <string>
using namespace std;  


  double VVS::R00(double r){
  return r;
  }
  
   double VVS::Fij(int i, int j, double r){
   double mpsi=(_pd[0]+_pd[1]+_pd[2]).mass();
   double temp=mpsi*r;
   if(i==0&& j==0) return 0;
   if(i==1&& j==0) return temp;
   if(i==-1&& j==0) return -temp;
 
  }

 EvtComplex  VVS::amps1(int m,  int i, int j){
   double mrho=_res[0];
   double wrho=_res[1];
   double s;
   EvtComplex img(0.0,1.0),dpro;
   EvtVector4R prho;
   prho=_pd[i]+_pd[j];
   s=prho.mass2();
   dpro=s-mrho*mrho+img*sqrt(s)*wrho;
   EvtHelSys angles(prho,_pd[i]),labAngles;
   double theta,phi,ct1,st1,phi1,st,ct;
   double rpp=angles.getHelAng(0);
   theta=angles.getHelAng(1);
   phi  =angles.getHelAng(2);
   ct1 =labAngles.Angles(prho,1);
   phi1=labAngles.Angles(prho,2);
   int lamb;
   EvtComplex temp(0.0,0.0);
   for(lamb=-1;lamb<=1;lamb++) temp=temp+Fij(lamb,0,prho.d3mag())*Djmn(1,m,lamb,phi1,ct1,0.0)/dpro*R00(rpp)*Djmn(1,lamb,0,phi,theta,0.0); 
    return temp;
  }

 double VVS::amps(){
 double r1=_par[0];
 double phase1=_par[1];
 double r2=_par[2];
 double phase2=_par[3];
 double temp=0.0;
 int m;
 for(m=-1;m<=1;m+=2) temp=temp+pow(abs(r1*exp(EvtComplex(0,phase1))*amps1(m,0,1)+r2*exp(EvtComplex(0,phase2))*amps1(m,0,2)+amps1(m,1,2)),2.);
 return temp;
 } 
