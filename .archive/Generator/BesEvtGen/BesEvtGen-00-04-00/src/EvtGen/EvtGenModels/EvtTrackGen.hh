//--------------------------------------------------------------------------
//
// Environment:
//      This software is part of the EvtGen package developed jointly
//      for the BaBar and CLEO collaborations.  If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information: See EvtGen/COPYRIGHT
//      Copyright (C) 1998      Caltech, UCSB
//
// Module: EvtGen/EvtTracks.hh
//
// Description:
//Class to handle generic phase space decays not done
//in other decay models.
//
// Modification history:
//
//    DJL/RYD     August 11, 1998         Module created
//
//------------------------------------------------------------------------

#ifndef EVTTRACKGEN_HH
#define EVTTRACKGEN_HH

#include "EvtGenBase/EvtDecayIncoherent.hh"
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class EvtParticle;

class EvtTrackGen:public  EvtDecayIncoherent  {

public:
  
  EvtTrackGen() {}
  virtual ~EvtTrackGen();

  void getName(std::string& name);

  EvtDecayBase* clone();

  void initProbMax();

  void init();

  void decay(EvtParticle *p);

private:
  std::string m_inputFileName;
  ifstream m_inputFile; 
  int nParticles;
  int idParticles[100];
  double pxParticle;
  double pyParticle;
  double pzParticle;
  double eParticle;
  std::vector<std::vector<EvtVector4R> >Evt;

};

#endif

