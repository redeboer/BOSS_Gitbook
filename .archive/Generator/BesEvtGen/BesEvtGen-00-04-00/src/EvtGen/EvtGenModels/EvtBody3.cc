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
// Module: EvtBody3.cc
//
// Description: Routine to decay a particle into three bodies  using the Dalitz plots and two particle 
//             angular distributions.
//      
// Modification history:
//
//    Ping R.-G.       December, 2006       Module created
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
#include "EvtGenModels/EvtBody3.hh"
#include "EvtGenBase/EvtRandom.hh"
#include <string>

#include "TH1.h"
#include "TAxis.h"
#include "TH2.h"
#include "TFile.h"  
#include "TApplication.h"
#include "TROOT.h"
//#include "CLHEP/config/CLHEP.h"
//#include "CLHEP/config/TemplateFunctions.h"

//using namespace CLHEP;
using std::endl;

EvtBody3::~EvtBody3() {}

void EvtBody3::getName(std::string& model_name){

  model_name="Body3";     

}

EvtDecayBase* EvtBody3::clone(){

  return new EvtBody3;

}


void EvtBody3::init(){

  // check that there are 4 arguments: Invariant mass part. Index: i,j, histor. file name, Hid 
  checkNArg(0);
  EvtSpinType::spintype parenttype = EvtPDL::getSpinType(getParentId());
}
void EvtBody3::initProbMax(){

  noProbMax();

}

void EvtBody3::decay( EvtParticle *p ){

 const char* fl=setFileName();
 const char* hp=setHpoint();
 int* dp;
 int nd1,nd2,ii,sn;


 sn=setDaugAngNo();

 if(sn==2){
  nd1=0;
  nd2=1;
 }

 if(sn==0){
  nd1=1;
  nd2=2;
 }

 if(sn==1){
  nd1=0;
  nd2=2;
 }
 const char*  DA1=setDaugAng(nd1);
 const char*  DA2=setDaugAng(nd2);


 dp=setDaugPair();
 int d1=dp[0];  //m(d1,d2) pair at X axis
 int d2=dp[1];
 int d3=dp[2];  //m(d3,d4) pair at Y axis
 int d4=dp[3];


 TFile f(fl);
 TH1F *did1= (TH1F*)f.Get(DA1);
 TH1F *did2= (TH1F*)f.Get(DA2);
 TH2F *hid = (TH2F*)f.Get(hp);
 
 TAxis* d1x=did1->GetXaxis();
 TAxis* d2x=did2->GetXaxis();
 TAxis* xaxis = hid->GetXaxis();
 TAxis* yaxis = hid->GetYaxis();
 
 int    BINSd1 =did1->GetXaxis()->GetLast();
 int    BINSd2 =did2->GetXaxis()->GetLast();
 int    BINSx  =xaxis->GetLast();
 int    BINSy  =yaxis->GetLast();
 int    BINS   =BINSx*BINSy;

 double av1,av2,avm1,avm2;
 avm1=0.;
 avm2=0.;
 double yvalue,ymax=0.0;
 int i,j,binxy;

// look for maxmum for the first hist.1
 for(i=1;i<BINSd1+1;i++){
   av1=did1->GetBinContent(i);
   if(av1>avm1) avm1=av1;
 }

// look for maxmum for the second hist.1
 for(i=1;i<BINSd2+1;i++){
   av2=did2->GetBinContent(i);
   if(av2>avm2) avm2=av2;
 }

// look for maxmum for the Dalitz plot

 for(i=1;i<BINSx+1;i++){
     for(j=1;j<BINSy+1;j++){
      binxy=hid->GetBin(i,j);
      yvalue=hid->GetBinContent(binxy);
//      cout<<"binxy,yvalue = "<<binxy<<"; "<<yvalue<<endl;
      if(yvalue>ymax) ymax=yvalue;
     }
 }

loop:
  p->initializePhaseSpace(getNDaug(),getDaugs());

  EvtParticle *id1,*id2,*id3,*id4,*s1;
  EvtVector4R pd1,pd2,pd3,pd4,ps;
  EvtVector4R dp1,dp2;
  double xmass2,ymass2;
 
  id1 =p->getDaug(d1); 
  id2 =p->getDaug(d2); 
  id3 =p->getDaug(d3);
  id4 =p->getDaug(d4);

  pd1 =id1->getP4Lab();
  pd2 =id2->getP4Lab();
  pd3 =id3->getP4Lab();
  pd4 =id4->getP4Lab();
 
  dp1 =p->getDaug(nd1)->getP4Lab();
  dp2 =p->getDaug(nd2)->getP4Lab();

  xmass2=(pd1+pd2).mass2();
  ymass2=(pd3+pd4).mass2();

  int xbin = hid->GetXaxis()->FindBin(xmass2);
  int ybin = hid->GetYaxis()->FindBin(ymass2);
  int xybin= hid->GetBin(xbin,ybin);
  double zvalue=hid->GetBinContent(xybin);
  double xratio=zvalue/ymax;
  if(xratio ==0 ) goto loop;
  double rd1=EvtRandom::Flat(0.0, 1.0); 
  if(rd1>xratio) goto loop;  //sigle out event by 2-D mass distribution histo.
  
  double da1=dp1.get(3)/dp1.d3mag();
  double da2=dp2.get(3)/dp2.d3mag();

  int dbin1=did1->FindBin(da1);
  int dbin2=did2->FindBin(da2);
  
  double dr1=(did1->GetBinContent(dbin1))/avm1;
  double dr2=(did2->GetBinContent(dbin2))/avm2;
  if(dr1==0 || dr2==0) goto loop;
  rd1=EvtRandom::Flat(0.0, 1.0);
  if(rd1>dr1) goto loop;  //sigle out event by the first angular distribution histo.
  
  rd1=EvtRandom::Flat(0.0, 1.0);
  if(rd1>dr2) goto loop;  //sigle out event by the second angular distribution histo.

  return ;
}


