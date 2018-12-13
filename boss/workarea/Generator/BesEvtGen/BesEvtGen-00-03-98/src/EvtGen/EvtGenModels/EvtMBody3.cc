
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
// Module: EvtBody3.cc  //Modified Body3 model
//
// usage:
// Users need to provide the MC and data file in NTuple style. The decays are described as
//                        A->  x1    +   x2     +  x3
//angular distribution:    costheta1, costheta2 ,costheta3
// mass distribution:          m12, m13, and m23, here, m12 ==> mass of x1 and x2, etc.
// these variables are defined as double type, the tree name is mc and data, repectively
// Modification history:
//
//    Ping R.-G.       Aug., 2010       Module created
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
#include "EvtGenBase/EvtDiracParticle.hh"
#include "EvtGenBase/EvtScalarParticle.hh"
#include "EvtGenBase/EvtVectorParticle.hh"
#include "EvtGenBase/EvtTensorParticle.hh"
#include "EvtGenBase/EvtPhotonParticle.hh" 
#include "EvtGenBase/EvtNeutrinoParticle.hh"
#include "EvtGenBase/EvtStringParticle.hh"
#include "EvtGenBase/EvtRaritaSchwingerParticle.hh"
#include "EvtGenBase/EvtHighSpinParticle.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtHelSys.hh"
#include "EvtGenModels/EvtMBody3.hh"
#include "EvtGenBase/EvtRandom.hh"
#include <string>
#include "TFile.h"

using std::endl;

EvtNT3 body3Ntuple;
EvtMBody3::~EvtMBody3() {}

void EvtMBody3::getName(std::string& model_name){

  model_name="MBody3";     

}

EvtDecayBase* EvtMBody3::clone(){

  return new EvtMBody3;

}

void EvtMBody3::init(){

  // check that there are 4 arguments: Invariant mass part. Index: i,j, histor. file name, Hid 
  checkNArg(0);

  const char* datafile;
  const char* mcfile;
  datafile=setFileName(1);
  mcfile  =setFileName(2);
  body3Ntuple.setMCfile(mcfile);
  body3Ntuple.setDTfile(datafile);
  body3Ntuple.init();
  std::cout<<"End to initiate the MBody3 model"<<std::endl;
 }  ///end of ini

void EvtMBody3::initProbMax(){

  noProbMax();

}

void EvtMBody3::decay( EvtParticle *p ){

loop:
  p->initializePhaseSpace(getNDaug(),getDaugs());

  EvtVector4R pd1,pd2,pd3,ps;
  double m12,m13,m23,costheta1,costheta2,costheta3;

  pd1 =p->getDaug(0)->getP4Lab();
  pd2 =p->getDaug(1)->getP4Lab();
  pd3 =p->getDaug(2)->getP4Lab();
  ps =p->getP4();
 
  m12=(pd1 + pd2).mass(); 
  m13=(pd1 + pd3).mass();
  m23=(pd2 + pd3).mass();

  costheta1=pd1.get(3)/pd1.d3mag();
  costheta2=pd2.get(3)/pd2.d3mag();
  costheta3=pd3.get(3)/pd3.d3mag();
  /*
  bool ac=body3Ntuple.AR123(costheta1,costheta2,costheta3,
                                  m23,      m13,      m12);
  */
  bool b1=body3Ntuple.AR1(costheta1,m23);
  bool b2=body3Ntuple.AR2(costheta2,m13);
  bool b3=body3Ntuple.AR3(costheta3,m12);
 
  if(!(b1 && b2 && b3)) goto loop;
  
			  // if(!(ac)) goto loop;
  //  std::cout<<"I find an envent." <<std::endl;
  return ;
}


