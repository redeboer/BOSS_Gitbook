//--------------------------------------------------------------------------
//
// Environment:
//
// Module: EvtGen/EvtNT3.hh
//
// Description: Class to handle NTuple for three body decays
// usage:
// Users need to provide the MC and data file in NTuple style. The decays are described as
//                        A->  x1    +   x2     +  x3
// angular distribution:  costheta1, costheta2 ,costheta3
// mass distribution:     m12, m13, and m23, here, m12 ==> mass of x1 and x2, etc.
// these variables are defined as double type, the tree name is mc and data, repectively
// Modification history:
//
//    PING RG     September 11, 2010         Module created
//
//------------------------------------------------------------------------
#include "EvtGenBase/EvtNT3.hh"
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>

void EvtNT3::init(){
  max=0;
  Ncos=20;
  chainMC = new TChain("mc");
  chainDT = new TChain("data");

  chainMC->SetDirectory(0);
  chainDT->SetDirectory(0);

  chainMC->Add(mcfile);
  chainDT->Add(datafile); 
  //--MC
  chainMC->SetBranchAddress("costheta1",&costheta1); 
  chainMC->SetBranchAddress("costheta2",&costheta2); 
  chainMC->SetBranchAddress("costheta3",&costheta3); 
  chainMC->SetBranchAddress("m12",&m12); 
  chainMC->SetBranchAddress("m13",&m13); 
  chainMC->SetBranchAddress("m23",&m23); 
  //--Data
  chainDT->SetBranchAddress("costheta1",&costheta1); 
  chainDT->SetBranchAddress("costheta2",&costheta2); 
  chainDT->SetBranchAddress("costheta3",&costheta3); 
  chainDT->SetBranchAddress("m12",&m12); 
  chainDT->SetBranchAddress("m13",&m13); 
  chainDT->SetBranchAddress("m23",&m23); 

  entriesMC=(Int_t)chainMC->GetEntries(); 
  entriesDT=(Int_t)chainDT->GetEntries(); 

  m12_low=chainDT->GetMinimum("m12");
  m13_low=chainDT->GetMinimum("m13");
  m23_low=chainDT->GetMinimum("m23");

  m12_up =chainDT->GetMaximum("m12");
  m13_up =chainDT->GetMaximum("m13");
  m23_up =chainDT->GetMaximum("m23");

  Int_t  ny=calculateBins(entriesDT); //bins in mass axisis (Y-axisis)

  MC1 = new TH2F("myMC1","",Ncos,-1.0,1.0,ny,m23_low,m23_up); // costheta1 vs. m23
  MC2 = new TH2F("myMC2","",Ncos,-1.0,1.0,ny,m13_low,m13_up); // costheta2 vs. m13
  MC3 = new TH2F("myMC3","",Ncos,-1.0,1.0,ny,m12_low,m12_up); // costheta3 vs. m12

  DT1 = new TH2F("myDT1","",Ncos,-1.0,1.0,ny,m23_low,m23_up); // costheta1 vs. m23
  DT2 = new TH2F("myDT2","",Ncos,-1.0,1.0,ny,m13_low,m13_up); // costheta2 vs. m13
  DT3 = new TH2F("myDT3","",Ncos,-1.0,1.0,ny,m12_low,m12_up); // costheta3 vs. m12

  WT1 = new TH2F("myWT1","",Ncos,-1.0,1.0,ny,m23_low,m23_up); // costheta1 vs. m23
  WT2 = new TH2F("myWT2","",Ncos,-1.0,1.0,ny,m13_low,m13_up); // costheta2 vs. m13
  WT3 = new TH2F("myWT3","",Ncos,-1.0,1.0,ny,m12_low,m12_up); // costheta3 vs. m12

  MC1->SetDirectory(0);
  MC2->SetDirectory(0);
  MC3->SetDirectory(0);

  DT1->SetDirectory(0);
  DT2->SetDirectory(0);
  DT3->SetDirectory(0);

  WT1->SetDirectory(0);
  WT2->SetDirectory(0);
  WT3->SetDirectory(0);

  //filling MC histogram
   for(Int_t j=0;j<entriesMC;j++) {
     chainMC->GetEntry(j);  
     MC1->Fill(costheta1,m23);
     MC2->Fill(costheta2,m13);
     MC3->Fill(costheta3,m12);
   }

  //filling data histogram
   for(Int_t j=0;j<entriesDT;j++) {
     chainDT->GetEntry(j);  
     DT1->Fill(costheta1,m23);
     DT2->Fill(costheta2,m13);
     DT3->Fill(costheta3,m12);
   }
   //--------debugging ---------
   //  showFrame(MC1);
   //  showFrame(MC2);
   //  showFrame(MC3);
 
  EvtHis2F::init(MC1,DT1,WT1);
  WT1  = EvtHis2F::getHwt();
  max1 = EvtHis2F::getZmax();

  EvtHis2F::init(MC2,DT2,WT2);
  WT2  = EvtHis2F::getHwt();
  max2 = EvtHis2F::getZmax();

  EvtHis2F::init(MC3,DT3,WT3);
  WT3  = EvtHis2F::getHwt();
  max3 = EvtHis2F::getZmax();
 
 //------------
 
  /* 
   show(WT1);
   std::cout<<"================================================="<<std::endl;
   show(WT2);
   std::cout<<"================================================="<<std::endl;
   show(WT3);
  */

}


int EvtNT3::calculateBins(int entries){
  int bins,ncell;
  ncell=30;  //at lease to require each cell to have 30 events
  bins=entries/ncell/Ncos;
  if(bins>100){bins=100;}
  return bins;
}


bool EvtNT3::AR1(double costheta, double mass){
  bool accept=false;
  accept= EvtHis2F::AR(costheta,mass,max1,WT1);
  //--- debugging
  //  std::cout<<"Max_mass= "<<getZmax()<<std::endl;

  return accept;
}

bool EvtNT3::AR2(double costheta, double mass){
  bool accept=false;
  accept= EvtHis2F::AR(costheta,mass,max2,WT2);
 
  return accept;
}


bool EvtNT3::AR3(double costheta, double mass){
  bool accept=false;
  accept= EvtHis2F::AR(costheta,mass,max3,WT3);
 
  return accept;
}

