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
// Module: EvtJ2BB3.cc
//
// Description: Routine to decay J/psi-> B_8 bar B_10 with using the
//              GVDM approach.(see J.-G. Korner Z.Physik C-Particles 
//              and Fields 33,529-535(1987))
//      
// Modification history:
//
//    Pang C.-Y., Ping R.-G.    Mar, 2007       Module created
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
#include "EvtJ2BB3.hh"
#include "EvtGenBase/EvtdFunctionSingle.hh"
#include "EvtGenBase/EvtRaritaSchwinger.hh"
#include "EvtGenBase/EvtRaritaSchwingerParticle.hh" 

#include <string>

using std::endl;
using std::cout;
EvtJ2BB3::~EvtJ2BB3() {}

void EvtJ2BB3::getName(std::string& model_name){

  model_name="J2BB3";     

}


EvtDecayBase* EvtJ2BB3::clone(){

  return new EvtJ2BB3;

}

void EvtJ2BB3::init(){
// checkNArg(1); 
 checkNDaug(2);
 checkSpinParent(EvtSpinType::VECTOR);
 checkSpinDaughter(0,EvtSpinType::DIRAC);
 checkSpinDaughter(1,EvtSpinType::RARITASCHWINGER);
 }

/*
void EvtJ2BB3::initProbMax() {

  //Hard coded... should not be hard to calculate...
 //setProbMax(10000.0);  //the ProbMax value must have noticed. 
   setProbMax(100.0);
}      
*/

void EvtJ2BB3::decay( EvtParticle *p){
 loop:
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

  double m_b0 = EvtPDL::getMass(getDaug(0));  //the mass of daughter1 is large than daughter0.
  double m_b1 = EvtPDL::getMass(getDaug(1));


  double m_M = EvtPDL::getMass(getParentId());
  double M2=pow(m_M,2.0);double b2_p=pow((m_b0+m_b1),2.0);
  double b2_m=pow((m_b1-m_b0),2.0);

  if(M2-b2_p<=0)  goto loop; 
  
  double P_c=sqrt((M2-b2_p)*(M2-b2_m)/(4.0*M2));
  double Q=M2-b2_p;
  double F1=-0.54/(m_b0*b2_p);                             
  double F2=-0.54*(m_b1-m_b0)/(m_b0*(m_b0+m_b1));
  double F3=-0.89/m_b0;
  double H1,H0,Hm1;

//OK. We have all  tested these parameters.   

  if(getNArg()>0)
    { alpha=getArg(0);
      H0=1.0;H1=sqrt((1+alpha)/(1-alpha));Hm1=H1;     
    }
   else{
         H1=-1.15*P_c*m_M*(-M2*Q*F1-Q*F2+(M2-m_b0*(m_b0+m_b1))*F3)/(sqrt(Q)*m_b1);
         H0=-0.82*P_c*M2*(-(m_b1-m_b0)*Q*F1/m_b1-Q*F2/(m_b1*(m_b1-m_b0))+2.0*F3)/sqrt(Q);
         Hm1=2*P_c*m_M*(m_b0+m_b1)*F3/sqrt(Q);
         alpha=(H1*H1+Hm1*Hm1-2*H0*H0)/(H1*H1+Hm1*Hm1+2*H0*H0); 
        }
//cout<<"------alpha="<<alpha<<";H0="<<H0<<";H1="<<H1<<";Hm1="<<Hm1<<endl;

// J/psi helicity =1 corresponding index=0
 vertex(0,0,0,Djmn(1, 1, 0,phi,theta,gamma)*H0);
 vertex(0,0,1,Djmn(1, 1,-1,phi,theta,gamma)*Hm1);
 vertex(0,0,2,0.0);
 vertex(0,0,3,Djmn(1, 1, 1,phi,theta,gamma)*H1);
 vertex(0,1,0,Djmn(1, 1,-1,phi,theta,gamma)*H1);
 vertex(0,1,1,0.0);
 vertex(0,1,2,Djmn(1, 1, 1,phi,theta,gamma)*Hm1);
 vertex(0,1,3,Djmn(1, 1, 0,phi,theta,gamma)*H0);
// J/psi helicity =-1 corresponding index=1 
 vertex(1,0,0,Djmn(1,-1, 0,phi,theta,gamma)*H0);
 vertex(1,0,1,Djmn(1,-1,-1,phi,theta,gamma)*Hm1);
 vertex(1,0,2,0.0);
 vertex(1,0,3,Djmn(1,-1, 1,phi,theta,gamma)*H1);
 vertex(1,1,0,Djmn(1,-1,-1,phi,theta,gamma)*H1);
 vertex(1,1,1,0.0);
 vertex(1,1,2,Djmn(1,-1, 1,phi,theta,gamma)*Hm1);
 vertex(1,1,3,Djmn(1,-1, 0,phi,theta,gamma)*H0);

// J/psi helicity =0 corresponding index=2
 vertex(2,0,0,Djmn(1, 0, 0,phi,theta,gamma)*H0);
 vertex(2,0,1,Djmn(1, 0,-1,phi,theta,gamma)*Hm1);
 vertex(2,0,2,0.0);
 vertex(2,0,3,Djmn(1, 0, 1,phi,theta,gamma)*H1);
 vertex(2,1,0,Djmn(1, 0,-1,phi,theta,gamma)*H1);
 vertex(2,1,1,0.0);
 vertex(2,1,2,Djmn(1, 0, 1,phi,theta,gamma)*Hm1);
 vertex(2,1,3,Djmn(1, 0, 0,phi,theta,gamma)*H0);

  return ;
  
}




