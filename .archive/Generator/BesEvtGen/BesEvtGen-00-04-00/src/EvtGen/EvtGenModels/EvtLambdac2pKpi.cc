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
#include "EvtLambdac2pKpi.hh"
#include <string>

std::vector<std::vector<EvtVector4R> > EvtLambdac2pKpi::Evt;
EvtLambdac2pKpi::~EvtLambdac2pKpi() {}

void EvtLambdac2pKpi::getName(std::string& model_name){

  model_name="Lambdac2pKpi";     

}

EvtDecayBase* EvtLambdac2pKpi::clone(){

  return new EvtLambdac2pKpi;

}


void EvtLambdac2pKpi::init(){

  // check that there are 0 arguments
  checkNArg(0);
  bool idp  = getDaugs()[0]==EvtPDL::getId(std::string("p+"))  || getDaugs()[0]==EvtPDL::getId(std::string("anti-p-"));
  bool idk  = getDaugs()[1]==EvtPDL::getId(std::string("K+"))  || getDaugs()[1]==EvtPDL::getId(std::string("K-")) || getDaugs()[1]==EvtPDL::getId(std::string("K_S0"));;
  bool idpi = getDaugs()[2]==EvtPDL::getId(std::string("pi+")) || getDaugs()[2]==EvtPDL::getId(std::string("pi-"))|| getDaugs()[2]==EvtPDL::getId(std::string("pi0"));
  if(!(idp && idk && idpi ) ){std::cout<<"EvtLambdac2pKpi: the daughter sequence should be p K pi"<<std::endl;abort();}
  std::vector<EvtVector4R> vp4;
  m_inputFileName=getenv("BESEVTGENROOT");
  m_inputFileName +="/src/EvtGen/EvtGenModels/EvtLambdac2pKpi.dat";
  //  m_inputFileName="EvtLambdac2pKpi.dat";
  m_inputFile.open(m_inputFileName.c_str());
  if (!m_inputFile){
    cout << "EvtLambdac2pKpi: PROBLEMS OPENING FILE "
         << m_inputFileName << endl;
    exit(0);
  }
  nParticles=3; //number of daughters
  Ntot = 100000; //number of events stored in the file m_puthFileName  
  Evt.clear();
  for(int h=0;h<Ntot;h++){
    vp4.clear();
    for (int i = 0; i < nParticles; i++){
      EvtVector4R p4;
      m_inputFile >> pxParticle;  p4.set(1,pxParticle);
      m_inputFile >> pyParticle;  p4.set(2,pyParticle);
      m_inputFile >> pzParticle;  p4.set(3,pzParticle);
      m_inputFile >> eParticle;   p4.set(0,eParticle);
      vp4.push_back(p4);
    }
    Evt.push_back(vp4);
  }

}

void EvtLambdac2pKpi::initProbMax(){

  noProbMax();

}

void EvtLambdac2pKpi::decay( EvtParticle *p ){

  double weight = p->initializePhaseSpace(getNDaug(),getDaugs());
  int rdm= (int)Evt.size()*EvtRandom::Flat(0.0,1.0);
  //debugging
  //std::cout<<"rdm= "<<rdm<<" "<<Evt.size()<<std::endl;

  if(Evt.size()==0) {std::cout<<"EvtLambdac2pKpi: out of stored file record"<<std::endl;abort();}
  for(int i=0;i<nParticles;i++){
    EvtParticle* daug=p->getDaug(i);
    daug->init(daug->getId(),Evt[rdm][i]);
  }
  // std::cout<<p->getDaug(0)->getP4()<<p->getDaug(1)->getP4()<<p->getDaug(2)->getP4()<<std::endl;

  return ;
}



