//--------------------------------------------------------------------------
//
// Modification history:
//
//    pingrg       January 22, 2014       Module created
//
// -------- input file must take the form like:
//           [number of final state particles in this event]
//           [PDG ID of particle 1]  [P_x] [P_y] [P_z] [E]
//           [PDG ID of particle 2]  [P_x] [P_y] [P_z] [E]
//       and so on
//------------------------------------------------------------------------
//
#include "EvtGenBase/EvtPatches.hh"
#include <stdlib.h>
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtGenKine.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtRandom.hh"
#include "EvtTrackGen.hh"
#include "EvtGlobalSet.hh"
#include <string>

EvtTrackGen::~EvtTrackGen() {}

void EvtTrackGen::getName(std::string& model_name){

  model_name="TrackGen";     

}

EvtDecayBase* EvtTrackGen::clone(){

  return new EvtTrackGen;

}


void EvtTrackGen::init(){

  // check that there are 0 arguments
  checkNArg(1);
  int idx=getArg(0);
  if(EvtGlobalSet::SV.size()==0){
    std::cout<<"No track data file is available! "<<std::endl;abort();
  }else{m_inputFileName=EvtGlobalSet::SV[idx];}

  m_inputFile.open(m_inputFileName.c_str());
  if (!m_inputFile){
    cout << "EvtTrackGen: PROBLEMS OPENING FILE "
         << m_inputFileName << endl;
    exit(0);
  }
  //load the event P4
  Evt.clear();
  while(!m_inputFile.eof()){
    m_inputFile >> nParticles;
    std::vector<EvtVector4R> vp4;
    for (int i = 0; i < nParticles; i++){
      EvtVector4R p4;
      m_inputFile >> idParticles[i];
      m_inputFile >> pxParticle;  p4.set(1,pxParticle);
      m_inputFile >> pyParticle;  p4.set(2,pyParticle);
      m_inputFile >> pzParticle;  p4.set(3,pzParticle);
      m_inputFile >> eParticle;   p4.set(0,eParticle);
      vp4.push_back(p4);
    }
    Evt.push_back(vp4);
  }
  Evt.pop_back();
  //check daughters id
  if(nParticles!=getNDaug()){std::cout<<"The number of daughters are not cosistent with that the data file"<<std::endl;abort();}
  for(int i=0;i<nParticles;i++){
    EvtId pid = EvtPDL::evtIdFromStdHep(idParticles[i] );
    if(pid!=getDaug(i) ){std::cout<<"The daughter particle pdg in your data file is not consistent with you decay card."<<std::endl;abort();}
  }
  //debugging
  /*
  for(int i=0;i<Evt.size();i++){
    std::cout<<"Event "<<i<<std::endl;
    for(int j=0;j<nParticles;j++){
      std::cout<<Evt[i][j].get(0)<<" "<<Evt[i][j].get(1)<<" "<<Evt[i][j].get(2)<<" "<<Evt[i][j].get(3)<<std::endl;
    }
  }
  */
//---------
}

void EvtTrackGen::initProbMax(){

  noProbMax();

}

void EvtTrackGen::decay( EvtParticle *p ){

  double weight = p->initializePhaseSpace(getNDaug(),getDaugs());
  int rdm= (int)Evt.size()*EvtRandom::Flat(0.0,1.0);
  if(Evt.size()==0) {std::cout<<"EvtTrackGen: out of stored file record"<<std::endl;abort();}
  EvtVector4R ptot(0,0,0,0);
  for(int i=0;i<nParticles;i++){
    EvtParticle* daug=p->getDaug(i);
    ptot += Evt[rdm][i];
    daug->init(daug->getId(),Evt[rdm][i]);
  }
  //p->init(p->getId(),ptot);

  //debugging
  // std::cout<<p->getDaug(getNDaug()-1)->getP4()<<" =? "<<eParticle<<" "<<pxParticle<<" "<<pyParticle<<" "<<pzParticle<<std::endl;

  return ;
}



