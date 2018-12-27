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
// Description:  Class to pick out the final state
//
// Modification history:
//
//    Ping R.-G.       December, 2011-Mar.       Module created
//
//------------------------------------------------------------------------
//

#ifndef EVTFSPICK_HH
#define EVTFSPICK_HH


#include <iostream>
#include <math.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtId.hh"
#include "EvtGenBase/EvtPDL.hh"
using std::endl;
using std::fstream;

class EvtFSPick
{

public:

// constructor
EvtFSPick(EvtParticle *par){
  root_par = par;
 _EvtFS.clear();
 _vnames.clear();
 _vId.clear();
 _fsid.clear();
 FSPick(root_par);
}

//destructor
  virtual ~EvtFSPick(){}

  void FSPick(EvtParticle* par);
  std::vector<EvtParticle*> getFS(){return _EvtFS;}
  void setFinalStatePar(std::vector <std::string> vnames);
  std::vector <EvtVector4R> getP4();
  std::vector <EvtVector4R> getP4Lab();
  int getIndex(EvtId eid);
 
private:

  std::vector <EvtParticle*> _EvtFS;
  EvtParticle *root_par;
  std::vector <std::string> _vnames;
  std::vector <EvtId> _vId,_fsid;
};

#endif

