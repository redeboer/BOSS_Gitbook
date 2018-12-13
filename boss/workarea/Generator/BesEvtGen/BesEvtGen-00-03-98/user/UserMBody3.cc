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

#include "TH1.h"
#include "TAxis.h"
#include "TH2.h"
#include "TH3.h"
#include "TFile.h"
#include "TApplication.h"
#include "TROOT.h"

#include "EvtGenModels/EvtMBody3.hh"


//////////////////***** modified  Body3 Model
const char*  EvtMBody3::setFileName(int i ){
  const char* datafile;
  const char* mcfile;
// --- user to specify the name of the NTuple for data (datafile ) and MC

  datafile="./body3data.root";  //specify the data root file  name
  mcfile  ="./body3mc.root";    //specify the mc root fine name using for eff. correction

//--------- don't touch follows
  if(i==1){return datafile;} else if(i==2){return mcfile;}
}

