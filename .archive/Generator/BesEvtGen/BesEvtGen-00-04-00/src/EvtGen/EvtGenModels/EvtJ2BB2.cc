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
// Module: EvtJ2BB2.cc //
// Description: Routine to decay jpsi or psi(2S) to B10 B10 bar. 
// The augular distribution parameter alpha is writed as -- in the model
// with  use the Carimalo approach that it is expended by us for jpsi or
// psi(2S) decays B10 B10 bar.
// 
//Modification history:
//
//  Pang C.Y. and  Ping R.-G.       Apr, 2007       Module created
//
//------------------------------------------------------------------------

#include "EvtGenBase/EvtPatches.hh"
#include <stdlib.h>
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtGenKine.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtVector4C.hh"
#include "EvtGenBase/EvtVector4R.hh"
#include "EvtGenBase/EvtTensor4C.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtHelSys.hh"
#include "EvtJ2BB2.hh"
#include "EvtGenBase/EvtdFunctionSingle.hh"
#include "EvtGenBase/EvtRaritaSchwinger.hh"
#include "EvtGenBase/EvtRaritaSchwingerParticle.hh" 

#include <string>

using std::endl;
using std::cout;
EvtJ2BB2::~EvtJ2BB2() {}

void EvtJ2BB2::getName(std::string& model_name){

  model_name="J2BB2";     

}


EvtDecayBase* EvtJ2BB2::clone(){

  return new EvtJ2BB2;

}

void EvtJ2BB2::init(){
// checkNArg(1); 
 checkNDaug(2);
 checkSpinParent(EvtSpinType::VECTOR);
 checkSpinDaughter(0,EvtSpinType::RARITASCHWINGER);
 checkSpinDaughter(1,EvtSpinType::RARITASCHWINGER);

 }

/*
void EvtJ2BB2::initProbMax() {

  //Hard coded... should not be hard to calculate...
 setProbMax(100.0);
}      
*/

void EvtJ2BB2::decay( EvtParticle *p){

  static EvtId DZP=EvtPDL::getId("Delta0");
  static EvtId DZM=EvtPDL::getId("anti-Delta0");
  static EvtId DPP=EvtPDL::getId("Delta+");
  static EvtId DPM=EvtPDL::getId("anti-Delta-");
  static EvtId DMP=EvtPDL::getId("Delta-");
  static EvtId DMM=EvtPDL::getId("anti-Delta+");
  static EvtId DPPP=EvtPDL::getId("Delta++");
  static EvtId DPPM=EvtPDL::getId("anti-Delta--");

  static EvtId SZP=EvtPDL::getId("Sigma*0");
  static EvtId SZM=EvtPDL::getId("anti-Sigma*0");
  static EvtId SPP=EvtPDL::getId("Sigma*+");
  static EvtId SPM=EvtPDL::getId("anti-Sigma*-");
  static EvtId SMP=EvtPDL::getId("Sigma*-");
  static EvtId SMM=EvtPDL::getId("anti-Sigma*+");
  
  static EvtId XP=EvtPDL::getId("Xi*-");
  static EvtId XM=EvtPDL::getId("anti-Xi*+");
  static EvtId XZP=EvtPDL::getId("Xi*0");
  static EvtId XZM=EvtPDL::getId("anti-Xi*0");

  static EvtId OP=EvtPDL::getId("Omega-");
  static EvtId OM=EvtPDL::getId("anti-Omega+");

  p->initializePhaseSpace(getNDaug(),getDaugs());

  EvtParticle *v,*s1;
  EvtVector4R pv,ps,ppr;

  v =p->getDaug(0);    
  s1=p->getDaug(1); 
  pv=v->getP4();
  ps=s1->getP4();
  ppr=p->getP4();
//  Put phase space results into the daughters.
  EvtHelSys angles(ppr,pv); //using helicity sys. angles
  double theta =angles.getHelAng(1);  
  double phi  =angles.getHelAng(2);
  double gamma=0;
/*  double costheta=cos(theta);
  double sintheta=sqrt(1-costheta*costheta);
  double d11=(1+costheta)/2.;
  double d1m1=(1-costheta)/2.;
  double d10=-sintheta/sqrt(2.);
  double dm10=-d10;
  double dm11=d1m1,dm1m1=d11;
  double d0m1=d10,d01=-d10,d00=costheta;
*/
//  double mass_b1 = EvtPDL::getMass(getDaug(0));
//  double mass_b2 = EvtPDL::getMass(getDaug(1));
  double mass_b1,mass_b2;
  EvtId d1;
  d1=v->getId();

// the mass of daughters are defined in our model. Because the distribution of Delta BreitWigner mass is very large.
     
  if((d1==XP)||(d1==XM)||(d1==XZP)||(d1==XZM)) {mass_b1=1.532;}
   else if((d1==SPP)||(d1==SPM)||(d1==SMP)||(d1==SMM)||(d1==SZP)||(d1==SZM)){mass_b1=1.383;} 
    else if((d1==DZP)||(d1==DZM)||(d1==DPP)||(d1==DPM)||(d1==DMP)||(d1==DMM)||(d1==DPPP)||(d1==DPPM)){mass_b1=1.232;} 
     else if((d1==OP)||d1==OM){mass_b1=1.67245;}
  else{cout<<"The decay is not the process: J/psi->B10 B10bar(decuplet baryon) ."<<endl;}

  mass_b2=mass_b1;
  double mass_M = EvtPDL::getMass(getParentId());

//  double c1=pow(pv.d3mag(),2.0);
//  double R=(mass_b1+pv.get(0))*(mass_b2+ps.get(0));

  double R=pow(mass_b1+mass_M/2.0,2.0);
  double c1=mass_M*mass_M/4.0-mass_b1*mass_b1;
  double  u=pow((R-c1)/(R+c1),2.0);

  if(getNArg()>0){ alpha=getArg(0);}
   else{
        alpha=(-9*u*u*u+17*u*u-16*u+8)/(9*u*u*u-7*u*u+8);
       }

  double F00,F01,F03,F10,F11,F22,F23,F30,F32,F33;
  double scale1=sqrt(3.0)*u/(u-2.0);                                        //scale1=F01:F03.
  double scale2=3.0*u/(3.0*u-4);                                              //scale2=F11:F00.

//  F00=1.0; F01=scale1*F03;F11=scale2*F00; 
//  F03=sqrt(2.0*(1.0+scale2*scale2)*(1.0+alpha)/((1.0-alpha)*(1.0+2.0*scale1*scale1)));

  F00=sqrt((1-alpha)/(4.0*(1.0+scale2*scale2)));
  F03=sqrt((1+alpha)/(2.0*(1.0+2.0*scale1*scale1)));  
  F01=scale1*F03;F11=scale2*F00;
  F22=F11;F32=F01;F33=F00;F30=F03;F10=F01;F23=F32;   //07.2

// J/psi helicity =1,-1,0 corresponding index=0,1,2 
//decuplt baryon helicity =1/2,3/2,-3/2,-1/2 corresponding index=0,1,2,3
 vertex(0,0,0,Djmn(1, 1, 0,phi,theta,gamma)*F00);
 vertex(0,0,1,Djmn(1, 1,-1,phi,theta,gamma)*F01);
 vertex(0,0,2,0.0); 
 vertex(0,0,3,Djmn(1, 1, 1,phi,theta,gamma)*F03);
 vertex(0,1,0,Djmn(1, 1, 1,phi,theta,gamma)*F10);
 vertex(0,1,1,Djmn(1, 1, 0,phi,theta,gamma)*F11);
 vertex(0,1,2,0.0); 
 vertex(0,1,3,0.0);
 vertex(0,2,0,0.0); 
 vertex(0,2,1,0.0);
 vertex(0,2,2,Djmn(1, 1, 0,phi,theta,gamma)*F22); 
 vertex(0,2,3,Djmn(1, 1,-1,phi,theta,gamma)*F23);
 vertex(0,3,0,Djmn(1, 1,-1,phi,theta,gamma)*F30);
 vertex(0,3,1,0.0); 
 vertex(0,3,2,Djmn(1, 1, 1,phi,theta,gamma)*F32);
 vertex(0,3,3,Djmn(1, 1, 0,phi,theta,gamma)*F33);

 vertex(1,0,0,Djmn(1,-1, 0,phi,theta,gamma)*F00);
 vertex(1,0,1,Djmn(1,-1,-1,phi,theta,gamma)*F01);
 vertex(1,0,2,0.0);
 vertex(1,0,3,Djmn(1,-1, 1,phi,theta,gamma)*F03);
 vertex(1,1,0,Djmn(1,-1, 1,phi,theta,gamma)*F10);
 vertex(1,1,1,Djmn(1,-1, 0,phi,theta,gamma)*F11);
 vertex(1,1,2,0.0);
 vertex(1,1,3,0.0);
 vertex(1,2,0,0.0);
 vertex(1,2,1,0.0);
 vertex(1,2,2,Djmn(1,-1, 0,phi,theta,gamma)*F22);
 vertex(1,2,3,Djmn(1,-1,-1,phi,theta,gamma)*F23);
 vertex(1,3,0,Djmn(1,-1,-1,phi,theta,gamma)*F30);
 vertex(1,3,1,0.0);
 vertex(1,3,2,Djmn(1,-1, 1,phi,theta,gamma)*F32);
 vertex(1,3,3,Djmn(1,-1, 0,phi,theta,gamma)*F33);

 vertex(2,0,0,Djmn(1, 0, 0,phi,theta,gamma)*F00);
 vertex(2,0,1,Djmn(1, 0,-1,phi,theta,gamma)*F01);
 vertex(2,0,2,0.0);
 vertex(2,0,3,Djmn(1, 0, 1,phi,theta,gamma)*F03);
 vertex(2,1,0,Djmn(1, 0, 1,phi,theta,gamma)*F10);
 vertex(2,1,1,Djmn(1, 0, 0,phi,theta,gamma)*F11);
 vertex(2,1,2,0.0);
 vertex(2,1,3,0.0);
 vertex(2,2,0,0.0);
 vertex(2,2,1,0.0);
 vertex(2,2,2,Djmn(1, 0, 0,phi,theta,gamma)*F22);
 vertex(2,2,3,Djmn(1, 0,-1,phi,theta,gamma)*F23);
 vertex(2,3,0,Djmn(1, 0,-1,phi,theta,gamma)*F30);
 vertex(2,3,1,0.0);
 vertex(2,3,2,Djmn(1, 0, 1,phi,theta,gamma)*F32);
 vertex(2,3,3,Djmn(1, 0, 0,phi,theta,gamma)*F33);

  return ;
  
}




