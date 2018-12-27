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

#include "EvtGenBase/EvtPatches.hh"
#include <iostream>
#include <math.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <strstream>
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtVector4R.hh"
#include "EvtGenBase/EvtVector3R.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtCPUtil.hh"
#include "EvtGenBase/EvtHelSys.hh"
#include "EvtGenBase/EvtKine.hh"
#include "EvtGenBase/EvtdFunction.hh"
using namespace std;  //::endl;

//using std::fstream;
//using std::strstream;

EvtHelSys::~EvtHelSys() {
}

 EvtHelSys::EvtHelSys( const EvtVector4R& p4p, const EvtVector4R& p4d) {
   _p4parent=p4p;   // parent momentum in its parent CM system
   _p4daug  =p4d;   // daughter momentum in its parent CM system
 }

 EvtHelSys::EvtHelSys( ) {
 }

  double  EvtHelSys::getHelAng(int i) {
  EvtVector4R b_p4p,rp4p, rp4d,boostdaug; 
  EvtVector3R GetHelAng;
  while (_p4parent.d3mag()!=0) {

    //  b_p4p=-1 * _p4parent;   //boost from Lab to HEL sys. required to reverse mom.Vec.
    //  b_p4p.set(0,_p4parent.get(0));  
    //  _bp4p=b_p4p;

// first to rotate the mother and daugher momentum to the helicity system
  double theta=Angles(_p4parent,1);
  double phi=Angles(_p4parent,2);


  rp4p=Helrotate(_p4parent,phi,theta);
  rp4d=Helrotate(_p4daug,  phi,theta);

// then boos to the CM system
//  EvtVector4R r_p4p=-1*rp4p;  //boost from Lab to HEL sys. required to reverse mom.Vec.
//  r_p4p.set(0,rp4p.get(0));
//  boostdaug=boostTo(rp4d,r_p4p);

  _rotatep4p=rp4p;
  _rotatep4d=rp4d;
  _bst=rp4d;
  //  _bp4p=r_p4p;
  return Angles(_bst,i);  //_bst:daughter momentum in helicity system, i=0==>|_bst|;i=1,2==>(theta,phi)
  }
 return Angles(_p4daug,i);
}

EvtVector4R EvtHelSys::checkparent(){ return _p4parent;}
EvtVector4R EvtHelSys::checkdaug(){ return _p4daug;}
EvtVector4R EvtHelSys::checkst(int i){
getHelAng(1);
if(i==0) return _bp4p;      //parent momentum used to boost the daughter to the CM sys.  
if(i==1) return _rotatep4p; //the parent momentum in Hel system by rotation
if(i==2) return _rotatep4d; //the daughter momentum in Hel. system by rotation
if(i==3) return _bst;       //_bst:daughter momentum in helicity system
}


EvtVector4R EvtHelSys::Helrotate(EvtVector4R p1, double phi, double theta){
EvtVector4R Rp;
double cp=cos(phi);
double sp=sin(phi);
double ct=cos(theta);
double st=sin(theta);
double t=p1.get(0),x=p1.get(1), y=p1.get(2),   z=p1.get(3);
double xp=x*cp*ct+y*sp*ct-z*st,yp= -x*sp+y*cp,zp=x*cp*st+y*sp*st+z*ct;
      Rp.set(t,xp,yp,zp);
 return Rp;
}

double EvtHelSys::Angles(EvtVector4R bbst,int i){

double rxy=sqrt(pow(bbst.get(1),2.)+pow(bbst.get(2),2.));
  if(bbst.d3mag()<=1e-10) {
      if(i==0) return 0.;
      else if (i==1) return 0.;
      else if (i==2) return 0.;
      else {cout<<"Angles(i): i<=2"<<endl;abort();}
   }
  else if(rxy<=1e-10){
      if(i==0) return bbst.d3mag();
      if(i==1) return 0.;
      if(i==2) return 0.;
      else {cout<<"Angles(i): i<=2"<<endl;abort();}
   }
   else {
   double theta=acos(bbst.get(3)/bbst.d3mag());
   double csphi=bbst.get(1)/bbst.d3mag()/sin(theta);
   if(fabs(csphi)>1.0) csphi=csphi/fabs(csphi);
   double phi=acos(csphi);
   if(bbst.get(2)<0.0) phi=2*3.1415926-phi;
      if(i==0) return bbst.d3mag();
      else if (i==1) return theta;
      else if (i==2) return phi;
      else {cout<<"Angles(i): i<=2"<<endl;abort();}
    }
    
} 


double djmn(int j, int m, int n, double theta){  
int j2=j*2,m2=m*2,n2=n*2;
double temp=EvtdFunction::d(j2,m2,n2,theta);
return temp;
}

double djmn(double j, double m, double n, double theta){  
int j2=(int)(j*2*1.1),m2=(int)(m*2*1.1),n2=(int)(n*2*1.1);
double temp=EvtdFunction::d(j2,m2,n2,theta);
return temp;
} 


EvtComplex Djmn(int j, int m, int n, double phi,double theta, double gamma){
int j2=j*2,m2=m*2,n2=n*2;
EvtComplex gp(cos(-phi*m  ), -sin(phi*m));
EvtComplex gm(cos(-gamma*n), -sin(gamma*n));
double  tp3=EvtdFunction::d(j2,m2,n2,theta);

//EvtComplex temp=wignerD(j2,m2,n2,phi,theta,gamma); //wignerD is corrected by pingrg, 2007,04,28, it gives the same result as this definition

EvtComplex temp=gp * tp3 * gm;

return temp;
}
  

EvtComplex Djmn(double j, double m, double n, double phi,double theta, double gamma){
int j2=(int)(j*2*1.1),m2=(int)(m*2*1.1),n2=(int)(n*2*1.1);
EvtComplex gp(cos(-phi*m  ), -sin(phi*m));
EvtComplex gm(cos(-gamma*n), -sin(gamma*n)); 
double  tp3=EvtdFunction::d(j2,m2,n2,theta);
EvtComplex temp=gp * tp3 * gm;
return temp;
}

