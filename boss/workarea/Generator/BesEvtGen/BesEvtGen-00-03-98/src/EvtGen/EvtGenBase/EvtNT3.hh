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
//angular distribution:    costheta1, costheta2 ,costheta3
// mass distribution:          m12, m13, and m23, here, m12 ==> mass of x1 and x2, etc.
// these variables are defined as double type, the tree name is mc and data, repectively
// Modification history:
//
//    PING RG     September 11, 2010         Module created
//
//------------------------------------------------------------------------
#ifndef EvtNT3_HH
#define EvtNT3_HH

#include "EvtGenBase/EvtHis2F.hh"
#include "TTree.h"

class EvtNT3:public EvtHis2F{

public:

  void init();
  int calculateBins(int entries);
  bool AR1(double costheta, double mass); //Accept-Reject judging for costheta1 vs. m23 dsitribution
  bool AR2(double costheta, double mass); //Accept-Reject judging for costheta2 vs. m13 dsitribution
  bool AR3(double costheta, double mass); //Accept-Reject judging for costheta3 vs. m12 dsitribution

  void setDTfile(const char* name){datafile=name;}
  void setMCfile(const char* name){mcfile=name;}

private:

  TChain *chainMC, *chainDT;
  TFile *dataf,*mcf;
  const char *datafile, *mcfile;
  TH2F *MC1, *MC2, *MC3; // e.g. MC1: costheta1 vs. m23
  TH2F *DT1, *DT2, *DT3;
  TH2F *WT1, *WT2, *WT3;
  double max1,max2,max3,max;
  double costheta1,costheta2,costheta3,m12,m13,m23;
  double m12_low, m12_up, m13_low, m13_up, m23_low, m23_up;
  int entriesMC,entriesDT,Ncos,Nmass;

};
#endif
