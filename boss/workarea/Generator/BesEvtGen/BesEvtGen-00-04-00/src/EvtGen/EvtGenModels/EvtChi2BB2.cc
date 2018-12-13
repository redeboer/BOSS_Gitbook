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
// Module: EvtChi2BB2.cc
//
// Description: Routine to decay Chi2 to B10 B10bar.
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
#include "EvtChi2BB2.hh"
#include <string>
using std::endl;
using std::cout;
EvtChi2BB2::~EvtChi2BB2() {}

void EvtChi2BB2::getName(std::string& model_name){

  model_name="Chi2BB2";     

}


EvtDecayBase* EvtChi2BB2::clone(){

  return new EvtChi2BB2;

}

void EvtChi2BB2::init(){
// check that there are 0 arguments
 checkNArg(5); 
 checkNDaug(2);
 checkSpinParent(EvtSpinType::TENSOR);
 checkSpinDaughter(0,EvtSpinType::RARITASCHWINGER);
 checkSpinDaughter(1,EvtSpinType::RARITASCHWINGER);

 }

/*
void EvtChi2BB2::initProbMax() {

  //Hard coded... should not be hard to calculate...
 setProbMax(100.0);

}      
*/
void EvtChi2BB2::decay( EvtParticle *p){

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
  double F00,F01,F02,F03,F10,F11,F13,F20,F22,F23,F30,F31,F32,F33;
  F00=getArg(0);
  F01=getArg(1); 
  F02=getArg(2);
  F03=getArg(3);
  F11=getArg(4);

  F33=F00;  F32=F01;  F31=F02; F30=F03; F22=F11;
  F10=F01;  F20=F02;  F23=F10; F13=F20; 
//cout<<"********F00,F01,F02,F03,F10,F11,F13,F20,F22,F23,F30,F31,F32,F33="<<F00<<F01<<F02<<F03<<F10<<F11<<F13<<F20<<F22<<F23<<F30<<F31<<F32<<F33<<endl;



 vertex(0,0,0,Djmn(2, 0, 0,phi,theta,gamma)*F00);//Chi2  helicity =0 corresponding index 0;
 vertex(0,0,1,Djmn(2, 0,-1,phi,theta,gamma)*F01);
 vertex(0,0,2,Djmn(2, 0, 2,phi,theta,gamma)*F02);
 vertex(0,0,3,Djmn(2, 0, 1,phi,theta,gamma)*F03);
 vertex(0,1,0,Djmn(2, 0, 1,phi,theta,gamma)*F10);
 vertex(0,1,1,Djmn(2, 0, 0,phi,theta,gamma)*F11);
 vertex(0,1,2,0.0);
 vertex(0,1,3,Djmn(2, 0, 2,phi,theta,gamma)*F13);
 vertex(0,2,0,Djmn(2, 0,-2,phi,theta,gamma)*F20);
 vertex(0,2,1,0.0);
 vertex(0,2,2,Djmn(2, 0, 0,phi,theta,gamma)*F22);
 vertex(0,2,3,Djmn(2, 0,-1,phi,theta,gamma)*F23);
 vertex(0,3,0,Djmn(2, 0,-1,phi,theta,gamma)*F30);
 vertex(0,3,1,Djmn(2, 0,-2,phi,theta,gamma)*F31);
 vertex(0,3,2,Djmn(2, 0, 1,phi,theta,gamma)*F32);
 vertex(0,3,3,Djmn(2, 0, 0,phi,theta,gamma)*F33);

 vertex(1,0,0,Djmn(2, 2, 0,phi,theta,gamma)*F00);//Chi2  helicity =2 corresponding index 1;
 vertex(1,0,1,Djmn(2, 2,-1,phi,theta,gamma)*F01);
 vertex(1,0,2,Djmn(2, 2, 2,phi,theta,gamma)*F02);
 vertex(1,0,3,Djmn(2, 2, 1,phi,theta,gamma)*F03);
 vertex(1,1,0,Djmn(2, 2, 1,phi,theta,gamma)*F10);
 vertex(1,1,1,Djmn(2, 2, 0,phi,theta,gamma)*F11);
 vertex(1,1,2,0.0);
 vertex(1,1,3,Djmn(2, 2, 2,phi,theta,gamma)*F13);
 vertex(1,2,0,Djmn(2, 2,-2,phi,theta,gamma)*F20);
 vertex(1,2,1,0.0);
 vertex(1,2,2,Djmn(2, 2, 0,phi,theta,gamma)*F22);
 vertex(1,2,3,Djmn(2, 2,-1,phi,theta,gamma)*F23);
 vertex(1,3,0,Djmn(2, 2,-1,phi,theta,gamma)*F30);
 vertex(1,3,1,Djmn(2, 2,-2,phi,theta,gamma)*F31);
 vertex(1,3,2,Djmn(2, 2, 1,phi,theta,gamma)*F32);
 vertex(1,3,3,Djmn(2, 2, 0,phi,theta,gamma)*F33);

 vertex(2,0,0,Djmn(2,-2, 0,phi,theta,gamma)*F00);//Chi2  helicity =-2 corresponding index 2;
 vertex(2,0,1,Djmn(2,-2,-1,phi,theta,gamma)*F01);
 vertex(2,0,2,Djmn(2,-2, 2,phi,theta,gamma)*F02);
 vertex(2,0,3,Djmn(2,-2, 1,phi,theta,gamma)*F03);
 vertex(2,1,0,Djmn(2,-2, 1,phi,theta,gamma)*F10);
 vertex(2,1,1,Djmn(2,-2, 0,phi,theta,gamma)*F11);
 vertex(2,1,2,0.0); 
 vertex(2,1,3,Djmn(2,-2, 2,phi,theta,gamma)*F13);
 vertex(2,2,0,Djmn(2,-2,-2,phi,theta,gamma)*F20);
 vertex(2,2,1,0.0);
 vertex(2,2,2,Djmn(2,-2, 0,phi,theta,gamma)*F22);
 vertex(2,2,3,Djmn(2,-2,-1,phi,theta,gamma)*F23);
 vertex(2,3,0,Djmn(2,-2,-1,phi,theta,gamma)*F30);
 vertex(2,3,1,Djmn(2,-2,-2,phi,theta,gamma)*F31);
 vertex(2,3,2,Djmn(2,-2, 1,phi,theta,gamma)*F32);
 vertex(2,3,3,Djmn(2,-2, 0,phi,theta,gamma)*F33);
 
 vertex(3,0,0,Djmn(2, 1, 0,phi,theta,gamma)*F00);//Chi2  helicity =1 corresponding index 3;
 vertex(3,0,1,Djmn(2, 1,-1,phi,theta,gamma)*F01);
 vertex(3,0,2,Djmn(2, 1, 2,phi,theta,gamma)*F02);
 vertex(3,0,3,Djmn(2, 1, 1,phi,theta,gamma)*F03);
 vertex(3,1,0,Djmn(2, 1, 1,phi,theta,gamma)*F10);
 vertex(3,1,1,Djmn(2, 1, 0,phi,theta,gamma)*F11);
 vertex(3,1,2,0.0); 
 vertex(3,1,3,Djmn(2, 1, 2,phi,theta,gamma)*F13);
 vertex(3,2,0,Djmn(2, 1,-2,phi,theta,gamma)*F20);
 vertex(3,2,1,0.0);
 vertex(3,2,2,Djmn(2, 1, 0,phi,theta,gamma)*F22);
 vertex(3,2,3,Djmn(2, 1,-1,phi,theta,gamma)*F23);
 vertex(3,3,0,Djmn(2, 1,-1,phi,theta,gamma)*F30);
 vertex(3,3,1,Djmn(2, 1,-2,phi,theta,gamma)*F31);
 vertex(3,3,2,Djmn(2, 1, 1,phi,theta,gamma)*F32);
 vertex(3,3,3,Djmn(2, 1, 0,phi,theta,gamma)*F33);

 vertex(4,0,0,Djmn(2,-1, 0,phi,theta,gamma)*F00);//Chi2  helicity =-1 corresponding index 4;
 vertex(4,0,1,Djmn(2,-1,-1,phi,theta,gamma)*F01);
 vertex(4,0,2,Djmn(2,-1, 2,phi,theta,gamma)*F02);
 vertex(4,0,3,Djmn(2,-1, 1,phi,theta,gamma)*F03);
 vertex(4,1,0,Djmn(2,-1, 1,phi,theta,gamma)*F10);
 vertex(4,1,1,Djmn(2,-1, 0,phi,theta,gamma)*F11);
 vertex(4,1,2,0.0);
 vertex(4,1,3,Djmn(2,-1, 2,phi,theta,gamma)*F13);
 vertex(4,2,0,Djmn(2,-1,-2,phi,theta,gamma)*F20);
 vertex(4,2,1,0.0);
 vertex(4,2,2,Djmn(2,-1, 0,phi,theta,gamma)*F22);
 vertex(4,2,3,Djmn(2,-1,-1,phi,theta,gamma)*F23);
 vertex(4,3,0,Djmn(2,-1,-1,phi,theta,gamma)*F30);
 vertex(4,3,1,Djmn(2,-1,-2,phi,theta,gamma)*F31);
 vertex(4,3,2,Djmn(2,-1, 1,phi,theta,gamma)*F32);
 vertex(4,3,3,Djmn(2,-1, 0,phi,theta,gamma)*F33);
  
 return ;

}




