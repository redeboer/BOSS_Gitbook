//--------------------------------------------------------------------------
//
// Environment:
//      This software is part of the EvtGen package developed jointly
//      for the BaBar and CLEO collaborations.  If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information: See EvtGen/COPYRIGHT
//      Copyright (C) 1998      Caltech, UCSB
//
// Module: EvtCalHelAmp.cc
//
// Description: Routine to decay a particle according th phase space
//
// Modification history:
//
//    RYD       January 8, 1997       Module created
//
//------------------------------------------------------------------------
//
#include "EvtGenBase/EvtPatches.hh"
#include <stdlib.h>
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtGenKine.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenModels/EvtCalHelAmp.hh"
#include "EvtGenModels/EvtGlobalSet.hh"
#include <string>

double EvtCalHelAmp::_H2err=0;
double EvtCalHelAmp::_H1err=0;
double EvtCalHelAmp::_H0err=0;
double EvtCalHelAmp::_H2=0;
double EvtCalHelAmp::_H1=0;
double EvtCalHelAmp::_H0=0;
int EvtCalHelAmp::nevt=0;
EvtCalHelAmp::~EvtCalHelAmp() {}

void EvtCalHelAmp::getName(std::string& model_name){

  model_name="CALHELAMP";     
 
}

EvtDecayBase* EvtCalHelAmp::clone(){

  return new EvtCalHelAmp;

}


void EvtCalHelAmp::init(){
  _H2=0;_H1=0;_H0=0;_H2err=0;_H1err=0;_H0err=0;nevt=0;
  // check that there are 1 arguments (the number of parameters)
  checkNArg(4);
  nd=getNArg();
  VC.resize(nd); 
  for(int i=0;i<nd;i++) VC[i].resize(nd);
  ifstream coverrf("myerr.dat");
  if(coverrf==0) {std::cout<<"File of covariant error (myerr.dat)" <<" is not found"<<endl;abort();}
  for(int i=0;i<nd;i++){
    for(int j=0;j<nd;j++){
      double xr;
      coverrf>>xr;
      VC[i][j]=xr;
    }
  }

}

void EvtCalHelAmp::initProbMax(){

  noProbMax();

}

void EvtCalHelAmp::decay( EvtParticle *p ){
  if(getNArg()!=4) {cout<<"The model have 6 parameters: |g00| phi00 |g22| phi22 |g42| phi42"<<endl; abort();}

  double weight = p->initializePhaseSpace(getNDaug(),getDaugs());
  //  std::cout<<"weight= "<<weight<<std::endl;
  //std::cout<<p->getP4().mass()<<" "<<p->getDaug(0)->getP4().mass()<<" "<<p->getDaug(1)->getP4().mass()<<std::endl;
  std::string p0str=EvtPDL::name(p->getId());
  std::string p1str=EvtPDL::name(p->getDaug(0)->getId());
  std::string p2str=EvtPDL::name(p->getDaug(1)->getId());
  if(p1str=="K_2*+" || p2str=="K_2*+" ||p1str=="K_2*0" || p2str=="K_2*0" ) {flag=2;} else
  if(p1str=="K_3*+" || p2str=="K_3*+" ||p1str=="K_3*0" || p2str=="K_3*0" ) {flag=3;} else
  if(p1str=="Zc+"   || p2str=="pi-"   ||p1str=="Zc-"   || p2str=="pi^+"  ) {flag=4;} else
  if(p0str=="Zc+"   && (p1str=="J/psi" || p2str=="J/psi")  ) {flag=5;} 
  //Helicity amplitude for 2+ -> VP
  EvtVector4R p1= p->getDaug(0)->getP4();
  double r= 2*p1.d3mag();  //to be consistent with PWA HelAmp.cc
  double R=3.0;
  double pi=3.1415926;
  double mm0=EvtPDL::getMeanMass(p->getId());
  double mm1=EvtPDL::getMeanMass(p->getDaug(0)->getId());
  double mm2=EvtPDL::getMeanMass(p->getDaug(1)->getId());
  double pmag=phsp(mm0,mm1,mm2);//for normalization Blatt factor, momentum without factor 2, consistent with the PWA HelAmp.cc
  double b0=getBlattRatio(0,R,r,pmag);
  double b1=getBlattRatio(1,R,r,pmag);
  double b2=getBlattRatio(2,R,r,pmag);
  double b3=getBlattRatio(3,R,r,pmag);
  double b4=getBlattRatio(4,R,r,pmag);
  //std::cout<<"b0,b1,b2,b3,b4 "<<b0<<" "<<b1<<" "<<b2<<" "<<b3<<" "<<b4<<std::endl;	
  double g02   = getArg(0);
  double phi02 = getArg(1)*2*pi;
  double g22 =   getArg(2);
  double phi22 = getArg(3)*2*pi;
  //double g42 =   getArg(4);
  //double phi42 = getArg(5)*2*pi;
  EvtComplex G02(g02*cos(phi02),g02*sin(phi02));
  EvtComplex G22(g22*cos(phi22),g22*sin(phi22));
  //EvtComplex G42(g42*cos(phi42),g42*sin(phi42));
  std::vector<EvtComplex> VG; VG.clear();
  VG.push_back(G02); VG.push_back(G22);// VG.push_back(G42);
  std::vector<double> VH2,VH1,VH0;
  VH2.resize(nd/2);VH1.resize(nd/2);VH0.resize(nd/2);
  if(flag==2){
    VH2[0]=sqrt(2./5.)*b1*r;  VH2[1]= 1/sqrt(10.)*r*r*r*b3; 
    VH1[0]=-1/sqrt(10.)*b1*r; VH1[1]= r*r*r*b3*sqrt(2./5.);
  }else if(flag==3){
    VH2[0]=sqrt(5./14.)*b2*r*r;  VH2[1]= 1/sqrt(7.)*r*r*r*r*b4; 
    VH1[0]=sqrt(1./7.)*b2*r*r;   VH1[1]= -sqrt(5./14.)*r*r*r*r*b4;
  }else if(flag==4 || flag==5){
    VH2[0]=sqrt(1./3.)*b0;  VH2[1]= 1/sqrt(6.)*r*r*b2; 
    VH1[0]=sqrt(1./3.)*b0;  VH1[1]=-2/sqrt(6.)*r*r*b2;
  }else{std::cout<<"Not allowed mode!"<<std::endl; abort();}
  EvtComplex H2 = G02*VH2[0] + G22*VH2[1];
  EvtComplex H1 = G02*VH1[0] + G22*VH1[1];
  
  _H2 += abs2(H2); _H1 += abs2(H1); 
  std::vector<double> DH1,DH2;
  DH2=firstder(VH2);
  DH1=firstder(VH1);

  // std::cout<<DH2[0]<<" "<<DH2[1]<<" "<<DH2[2]<<" "<<DH2[3]<<std::endl;
  //std::cout<<DH1[0]<<" "<<DH1[1]<<" "<<DH1[2]<<" "<<DH1[3]<<std::endl;

  for(int i=0;i<nd;i++){
    for(int j=0;j<nd;j++){
      _H2err += DH2[i]*DH2[j]*VC[i][j]; 
      _H1err += DH1[i]*DH1[j]*VC[i][j];
    }
  }
  
  nevt++;

  return ;
}


std::vector<double> EvtCalHelAmp::firstder(std::vector<double> vh){
  // vh: vector of helicity part
  double pi=3.1415926;
  std::vector<double> fd;
  double g02 =   getArg(0);
  double phi02 = getArg(1)*2*pi;
  double g22 =   getArg(2);
  double phi22 = getArg(3)*2*pi;
  std::vector<double> vpar;
  vpar.push_back(g02);vpar.push_back(phi02);vpar.push_back(g22);vpar.push_back(phi22);
  EvtComplex G02(vpar[0]*cos(vpar[1]),vpar[0]*sin(vpar[1]));
  EvtComplex G22(vpar[2]*cos(vpar[3]),vpar[2]*sin(vpar[3]));
  EvtComplex H20 = G02*vh[0] + G22*vh[1];
  double hamps0 = abs2(H20);
  for(int i=0;i<vpar.size();i++){
    vpar.clear();
    vpar.push_back(g02);vpar.push_back(phi02);vpar.push_back(g22);vpar.push_back(phi22);
    double dev=0.01;
    vpar[i] += dev;
    EvtComplex G02(vpar[0]*cos(vpar[1]),vpar[0]*sin(vpar[1]));
    EvtComplex G22(vpar[2]*cos(vpar[3]),vpar[2]*sin(vpar[3]));
    EvtComplex H20 = G02*vh[0] + G22*vh[1];
    double hamps2=abs2(H20);
    double xder=(hamps2-hamps0)/dev; 
    fd.push_back(xder);
  }
  return fd;
}

double EvtCalHelAmp::blattfactor(int L, double R, double pmag){//pmag is the magnitude of one daughter particle, instead of difference of momentum between two daugs.
  double rp = R*pmag;
  double rp2 = rp*rp;
  double rp4 = rp2*rp2;
  double rp6 = rp2 * rp4;
  double rp8 = rp4 * rp4;
  switch (L){
  case 0:
    return 1.0;
    break;
  case 1:
    return 1.0/sqrt(1+R*R*pmag*pmag);
    break;
  case 2:
    return 1.0/sqrt(1+rp2/3.+rp4/9.);
    break;
  case 3:
    return 1.0/sqrt(225 + 45*rp2 + 6*rp4 + rp6 );
    break;
  case 4:
    return 1.0/sqrt(11025 + 1575*rp2 + 135*rp4 + 10*rp6 + rp8 );
    break;
  default:
    cout<<"lineshape::blattfactor: No expression for L>4 is availabe."<<endl;
    ::abort();
  }

}


double EvtCalHelAmp::getBlattRatio(int L, double R, double pmag,double pmag0){
  double blattRatio = blattfactor(L,R,pmag)/blattfactor(L,R,pmag0);
  return blattRatio;
}

double EvtCalHelAmp::phsp(double m0,double m1, double m2){  //phase space for m0->m1 + m2
     if(m1+m2>m0) {cout<<"HelAmp::phsp: m1("<<m1<<") + m2("<<m2<<") > m0("<<m0<<") "<<endl;::abort();}
     //if(m1+m2>m0) {return 0;}
     double m0s = m0*m0;
     double m12 =(m1+m2);
     double m12s = m12*m12;
     double md12 = m1-m2;
     double md12s=md12*md12;
 
     double pmag = sqrt( fabs( (m0s-m12s)*(m0s-md12s) ) )/2/m0;
  
     //-- for debugging
     // cout<<m0<<" ->"<<m1<<" + "<<m2<<", pmag= "<<pmag<<endl;
       return pmag;
 } 
