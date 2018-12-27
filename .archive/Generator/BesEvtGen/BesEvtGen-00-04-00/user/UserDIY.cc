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
// Module: EvtDIY.cc
//
// Description: Routine for users to create model to use the model DIY 
//
// Modification history:
//
//    Ping R.-G.       December, 2006       Module created
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
#include "EvtGenModels/EvtDIY.hh"
#include <string>
using namespace std;  


//    **** angular sample test

class AngularSam{

 public:
 AngularSam(double alpha, EvtVector4R p1){
 _a=alpha;
 pd1=p1;
 }
// virtual ~AngularSam();
 double amps();

 private:
 EvtVector4R pd1;
 double _a;
};

double AngularSam::amps(){
double costheta=pd1.get(3)/pd1.d3mag();
return 1+_a*pow(costheta,2.);
}
//--------------------

// ******** rhopi decays without interference
class rhopi{
 public:
 rhopi(EvtVector4R pd1, EvtVector4R pd2,EvtVector4R pd3){
 _pd[0]=pd1;
 _pd[1]=pd2;
 _pd[2]=pd3;
 }
 double F00(double s);
 double F10(double s);
 double amps1(double s,int i, int j);
 double amps( );

 private:
 EvtVector4R _pd[3];
};

  double rhopi::F00(double s){
  double mpi=0.1395;
  return sqrt(s-4*mpi*mpi)/sqrt(s);
  }
  
   double rhopi::F10(double s){
    double mpi=0.1395,mpsi=3.096916;
    double tep=sqrt((mpsi*mpsi-pow(mpi+sqrt(s),2.))*(mpsi*mpsi-pow(mpi-sqrt(s),2.)));
   return tep;
  }

 double rhopi::amps1(double s, int i, int j){
   double mrho=0.771,wrho=0.1492,dpro;
   EvtComplex img(0.0,1.0);
   dpro=pow(abs(s-mrho*mrho+img*sqrt(s)*wrho),2.);
   EvtVector4R prho;
   prho=_pd[i]+_pd[j];
   EvtHelSys angles(prho,_pd[i]),labAngles;
   double theta,phi,ct1,st1,phi1,st,ct,temp;
   theta=angles.getHelAng(1);
   phi  =angles.getHelAng(2);
   ct1 =labAngles.Angles(prho,1);
   phi1=labAngles.Angles(prho,2);
   st=sin(theta);ct=cos(theta);
   temp=pow(F00(s),2.)*pow(F10(s),2.)*pow(st,2.)/dpro; // *(1+pow(ct1,2.)+pow(st1,2.)*cos(2*(phi1+phi))); 
    return temp;
  }

 double rhopi::amps(){
 double temp,s12,s13,s23;
 s12=(_pd[0]+_pd[1]).mass2();
 s13=(_pd[0]+_pd[2]).mass2();
 s23=(_pd[1]+_pd[2]).mass2();
 temp=amps1(s12,0,1)+amps1(s13,0,2)+amps1(s23,1,2);
 return temp;
 } 
//  ************** end class rhopi


// ******** rhopi decays with interference
class rhopifull{
 public:
 rhopifull(EvtVector4R pd1, EvtVector4R pd2,EvtVector4R pd3){
 _pd[0]=pd1;
 _pd[1]=pd2;
 _pd[2]=pd3;
 }
 double Fij(int i, int j, double r);
 double R00(double r);
 EvtComplex  amps1(int m, int i, int j);
 double amps( );

 private:
 EvtVector4R _pd[3];
};

  double rhopifull::R00(double r){
  double mpi=0.1395;
  return r;
  }
  
   double rhopifull::Fij(int i, int j, double r){
   double mpi=0.1395,mpsi=3.096916;
   double temp=mpsi*r;
   if(i==0&& j==0) return 0;
   if(i==1&& j==0) return temp;
   if(i==-1&& j==0) return -temp;
 
  }

 EvtComplex  rhopifull::amps1(int m,  int i, int j){
   double mrho=0.771,wrho=0.1492,s;
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

 double rhopifull::amps(){
 double temp=0.0;
 int m;
 for(m=-1;m<=1;m+=2) temp=temp+pow(abs(amps1(m,0,1)+amps1(m,0,2)+amps1(m,1,2)),2.);
 return temp;
 } 
//  ************** end class rhopifull


//////////////////*****  DIY Model
  double EvtDIY::AmplitudeSquare(){
  EvtVector4R dp1=GetDaugMomLab(0);
  EvtVector4R dp2=GetDaugMomLab(1),dp3=GetDaugMomLab(2);

// AngularSam ppbar(0.7,dp1);
// return ppbar.amps();

//   rhopi Jpsi2rhopi(dp1,dp2,dp3);
//   return Jpsi2rhopi.amps();

   rhopifull Jpsi2rhopi(dp1,dp2,dp3);
   return Jpsi2rhopi.amps();
 }

