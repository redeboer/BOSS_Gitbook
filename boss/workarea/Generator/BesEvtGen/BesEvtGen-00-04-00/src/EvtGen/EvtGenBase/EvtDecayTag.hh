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
// Module:  EvtDecayTag.hh
//
// Description:  Class to deal with the decay tag, e.g. the decay mode and multiplicity
//
// Modification history:
//
//    Ping R.-G.       December, 2011-Jan       Module created
//
//------------------------------------------------------------------------
//

#ifndef EVTDECAYTAG_HH
#define EVTDECAYTAG_HH


#include <iostream>
#include <math.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
// #include <sys/stat.h>
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtId.hh"
#include "EvtGenBase/EvtPDL.hh"
using std::endl;
using std::fstream;

class EvtDecayTag
{

public:

// constructor
EvtDecayTag(EvtParticle *par){
  root_par = par;
 _nmode.clear();
 _multiplicity.clear();
 for(int i=0;i<10;i++){_multiplicity.push_back(0);}
 makeTag(root_par);
}

//destructor
virtual ~EvtDecayTag(){}

void makeTag(EvtParticle* par);
int TagIndex(EvtId pid);
int getModeTag();
int getMultTag();
int getHdrdBase(int index);

private:

  std::vector <int> _nmode,_multiplicity;
  EvtParticle *root_par;
};

#endif

