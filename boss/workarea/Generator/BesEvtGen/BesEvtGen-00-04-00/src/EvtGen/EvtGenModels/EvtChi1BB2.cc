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
// Module: EvtChi1BB2.cc
//
// Description: Routine to decay Chi1 to B10 B10bar.
//              
// Modification history:
//
// Pang C.Y. and Ping R.-G.       April, 2007       Module created
//
//------------------------------------------------------------------------
// 
#include "EvtGenBase/EvtPatches.hh"
#include <stdlib.h>
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtGenKine.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtVector4C.hh"
#include "EvtGenBase/EvtVector4R.hh"
#include "EvtGenBase/EvtTensor4C.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtdFunction.hh"
#include "EvtGenBase/EvtHelSys.hh"
#include "EvtChi1BB2.hh"
#include <string>
using std::endl;
using std::cout;
EvtChi1BB2::~EvtChi1BB2() {}

void EvtChi1BB2::getName(std::string& model_name){

  model_name="Chi1BB2";     

}


EvtDecayBase* EvtChi1BB2::clone(){

  return new EvtChi1BB2;

}

void EvtChi1BB2::init(){
// check that there are 1 arguments
 checkNArg(4); 
 checkNDaug(2);
 checkSpinParent(EvtSpinType::VECTOR);
 checkSpinDaughter(0,EvtSpinType::RARITASCHWINGER);
 checkSpinDaughter(1,EvtSpinType::RARITASCHWINGER);

 }

/*
void EvtChi1BB2::initProbMax() {

  //Hard coded... should not be hard to calculate...
 setProbMax(50.0);

}      
*/

void EvtChi1BB2::decay( EvtParticle *p){

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

/* double alpha=getArg(0);
 double F01=sqrt((1+alpha)/2);
 double F00=sqrt((1-alpha)/4);
*/
 double F00=getArg(0);
 double F01=getArg(1);
 double F03=getArg(2);
 double F11=getArg(3);
 double F33=-F00; double F32=-F01; double F30=-F03;
 double F10=F01; double F23=-F10; double F22=-F11;
//cout<<"********F00,F01,F03,F10,F11,F22,F23,F30,F32,F33="<<F00<<F01<<F03<<F10<<F11<<F22<<F23<<F30<<F32<<F33<<endl;
/*cout<<"********Djmn(1, 1, 0,phi,theta,gamma)="<<Djmn(1, 1, 0,phi,theta,gamma);
cout<<"********Djmn(1, 1,-1,phi,theta,gamma)="<<Djmn(1, 1,-1,phi,theta,gamma);
cout<<"********Djmn(1, 1, 1,phi,theta,gamma)="<<Djmn(1, 1, 1,phi,theta,gamma);
*/
 int j1,j2,j3;
 for(j1=0;j1<=2;){
 for(j2=0;j2<=3;){
 for(j3=0;j3<=3;){
 vertex(j1,j2,j3,0.0);j3++;}
j2++;}
j1++;}

 vertex(0,0,0,Djmn(1, 1, 0,phi,theta,gamma)*F00);//Chi1  helicity =1 corresponding index=0
 vertex(0,0,1,Djmn(1, 1,-1,phi,theta,gamma)*F01);
 vertex(0,0,3,Djmn(1, 1, 1,phi,theta,gamma)*F03);
 vertex(0,1,0,Djmn(1, 1, 1,phi,theta,gamma)*F10);
 vertex(0,1,1,Djmn(1, 1, 0,phi,theta,gamma)*F11);
 vertex(0,2,2,Djmn(1, 1, 0,phi,theta,gamma)*F22);
 vertex(0,2,3,Djmn(1, 1,-1,phi,theta,gamma)*F23);
 vertex(0,3,0,Djmn(1, 1,-1,phi,theta,gamma)*F30);
 vertex(0,3,2,Djmn(1, 1, 1,phi,theta,gamma)*F32);
 vertex(0,3,3,Djmn(1, 1, 0,phi,theta,gamma)*F33);

 vertex(1,0,0,Djmn(1,-1, 0,phi,theta,gamma)*F00);//Chi1  helicity =-1 corresponding index=1
 vertex(1,0,1,Djmn(1,-1,-1,phi,theta,gamma)*F01);
 vertex(1,0,3,Djmn(1,-1, 1,phi,theta,gamma)*F03);
 vertex(1,1,0,Djmn(1,-1, 1,phi,theta,gamma)*F10);
 vertex(1,1,1,Djmn(1,-1, 0,phi,theta,gamma)*F11);
 vertex(1,2,2,Djmn(1,-1, 0,phi,theta,gamma)*F22);
 vertex(1,2,3,Djmn(1,-1,-1,phi,theta,gamma)*F23);
 vertex(1,3,0,Djmn(1,-1,-1,phi,theta,gamma)*F30);
 vertex(1,3,2,Djmn(1,-1, 1,phi,theta,gamma)*F32);
 vertex(1,3,3,Djmn(1,-1, 0,phi,theta,gamma)*F33);

 vertex(2,0,0,Djmn(1, 0, 0,phi,theta,gamma)*F00);//Chi1  helicity =0 corresponding index=2
 vertex(2,0,1,Djmn(1, 0,-1,phi,theta,gamma)*F01);
 vertex(2,0,3,Djmn(1, 0, 1,phi,theta,gamma)*F03);
 vertex(2,1,0,Djmn(1, 0, 1,phi,theta,gamma)*F10);
 vertex(2,1,1,Djmn(1, 0, 0,phi,theta,gamma)*F11);
 vertex(2,2,2,Djmn(1, 0, 0,phi,theta,gamma)*F22);
 vertex(2,2,3,Djmn(1, 0,-1,phi,theta,gamma)*F23);
 vertex(2,3,0,Djmn(1, 0,-1,phi,theta,gamma)*F30);
 vertex(2,3,2,Djmn(1, 0, 1,phi,theta,gamma)*F32);
 vertex(2,3,3,Djmn(1, 0, 0,phi,theta,gamma)*F33);

 return ;

}

