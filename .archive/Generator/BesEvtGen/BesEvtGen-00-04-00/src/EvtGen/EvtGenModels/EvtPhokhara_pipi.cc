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
// Module: EvtPhokhara.cc
//         the necessary file: phokharar.F
//                             fist.inc,gen.inc  mix.inc  stdhep.inc
// Description: Modified Lund model at tau-charm energy level, see
//               PHYSICAL REVIEW D, VOLUME 62, 034003
// Modification history:
//   
//   Ping R.-G.    Jan.25, 2010       Module created    
//   The random engine RLU0 is unified with RLU for BesEvtGen
//------------------------------------------------------------------------
// 
#include "EvtGenBase/EvtPatches.hh"
#include "EvtGenBase/EvtPatches.hh"
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtStringParticle.hh"
#include "EvtGenBase/EvtDecayTable.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenModels/EvtPhokhara_pipi.hh"
#include "EvtGenModels/EvtPhokharaDef.hh"
#include "EvtGenBase/EvtReport.hh"
#include <string>
#include "EvtGenBase/EvtId.hh"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "GeneratorObject/McGenEvent.h"
#include "BesKernel/IBesRndmGenSvc.h"
#include "CLHEP/Random/RandomEngine.h"
#include "cfortran/cfortran.h"

using namespace std;
using namespace CLHEP;

using std::endl;
using std::fstream;
using std::ios;
using std::ofstream;
using std::resetiosflags;
using std::setiosflags;
using std::setw;

int EvtPhokhara_pipi::nevtgen=0;
int EvtPhokhara_pipi::nphokharadecays=0;
EvtDecayBasePtr* EvtPhokhara_pipi::phokharadecays=0; 
int EvtPhokhara_pipi::ntable=0;

int EvtPhokhara_pipi::ncommand=0;
int EvtPhokhara_pipi::lcommand=0;
std::string* EvtPhokhara_pipi::commands=0;
int EvtPhokhara_pipi::nevt=0;

EvtPhokhara_pipi::EvtPhokhara_pipi(){}
EvtPhokhara_pipi::~EvtPhokhara_pipi(){
  int i;
  //the deletion of commands is really uggly!

  if (nphokharadecays==0) {
    delete [] commands;
    commands=0;
    return;
  }

  for(i=0;i<nphokharadecays;i++){
    if (phokharadecays[i]==this){
      phokharadecays[i]=phokharadecays[nphokharadecays-1];
      nphokharadecays--;
      if (nphokharadecays==0) {
	delete [] commands;
	commands=0;
      }
      return;
    }
  }

  report(ERROR,"EvtGen") << "Error in destroying Phokhara model!"<<endl;
 
}


void EvtPhokhara_pipi::getName(std::string& model_name){

  model_name="PHOKHARA_pipi";     

}

EvtDecayBase* EvtPhokhara_pipi::clone(){

  return new EvtPhokhara_pipi;

}


void EvtPhokhara_pipi::initProbMax(){

  noProbMax();

}


void EvtPhokhara_pipi::init_mode(EvtParticle* p){
 m_pion=1; 
 // mu+mu-(0),pi+pi-(1),2pi0pi+pi-(2),
 // 2pi+2pi-(3),ppbar(4),nnbar(5),
 // K+K-(6),K0K0bar(7),pi+pi-pi0(8), 
 // Lamb Lambbar->pi-pi+ppbar(9) 
 // pi+pi-eta 
#include "Phokhara_init_mode.txt"
}



void EvtPhokhara_pipi::init(){
  checkNArg(0);

  std::string locvp=getenv("BESEVTGENROOT");
  system("cat $BESEVTGENROOT/share/phokhara_9.1.param>phokhara_9.1.param");
  system("cat $BESEVTGENROOT/share/phokhara_9.1.fferr>phokhara_9.1.fferr");
  system("cat $BESEVTGENROOT/share/phokhara_9.1.ffwarn>phokhara_9.1.ffwarn");

  
  if (getParentId().isAlias()){
    
    report(ERROR,"EvtGen") << "EvtPhokhara finds that you are decaying the"<<endl
			   << " aliased particle "
			   << EvtPDL::name(getParentId()).c_str()
			   << " with the Phokhara model"<<endl
			   << " this does not work, please modify decay table."
			   << endl;
    report(ERROR,"EvtGen") << "Will terminate execution!"<<endl;
    ::abort();

  }

  store(this);

}


std::string EvtPhokhara_pipi::commandName(){
     
  return std::string("PhokharaPar");
  
}

void EvtPhokhara_pipi::command(std::string cmd){

  if (ncommand==lcommand){

    lcommand=10+2*lcommand;

    std::string* newcommands=new std::string[lcommand];
    
    int i;

    for(i=0;i<ncommand;i++){
      newcommands[i]=commands[i];
    }
    
    delete [] commands;

    commands=newcommands;

  }

  commands[ncommand]=cmd;

  ncommand++;

}



void EvtPhokhara_pipi::decay( EvtParticle *p){
  EvtId myvpho=EvtPDL::getId("vpho");
  if(p->getId()!=myvpho) {std::cout<<"Parent particle is required to be vpho for Phokhara model"<<std::endl;abort();}
  if(nevtgen==0) {init_mode(p);}
  else{init_evt(p);}

  std::cout<<"PHOKHARA :  pi+pi-  mode "<<std::endl;
  int istdheppar=EvtPDL::getStdHep(p->getId());
  int ntrials = 0;
  int tr_old[3];
  tr_old[0] = (int)maxima_.tr[0];
  tr_old[1] = (int)maxima_.tr[1];
  tr_old[2] = (int)maxima_.tr[2];

  while( ntrials < 1000000)
    {
      ievent++;
      RANLXDF(Ar_r,1);
      Ar[1] = Ar_r[0];
      
     if (Ar[1] <= (maxima_.Mmax[0]/(maxima_.Mmax[0]+maxima_.Mmax[1]+maxima_.Mmax[2]))) {
        maxima_.count[0] = maxima_.count[0]+1.0;
        GEN_0PH(2,qqmin,ctes_.Sp,cos3min,cos3max);
       }else 
    if (Ar[1] <= ( (maxima_.Mmax[0]+maxima_.Mmax[1])/(maxima_.Mmax[0]+maxima_.Mmax[1]+maxima_.Mmax[2]))) {
        maxima_.count[1] = maxima_.count[1]+1.0;
        GEN_1PH(2,qqmin,qqmax,cos1min,cos1max,cos3min,cos3max);
       }
       else {
         maxima_.count[2] = maxima_.count[2]+1.0;
         GEN_2PH(2,qqmin,cos1min,cos1max,cos2min,cos2max,cos3min,cos3max);
       }
      
      if( ((int)maxima_.tr[0]+(int)maxima_.tr[1]+(int)maxima_.tr[2]) > (tr_old[0]+tr_old[1]+tr_old[2]) ) // event accepted after cuts
        {
          goto storedEvents;
        }
      ntrials ++;
    }
  
  std::cout <<"FATAL: Could not satisfy cuts after " << ntrials << "trials. Terminate." <<std::endl;
  //----
 storedEvents:
  int more=0;
  int numstable=0;
  int numparton=0;
  EvtId evtnumstable[100];//
  EvtVector4R p4[20];

  // except ISR photos, products depending on channel
  if (flags_.pion == 0) { // mu+ mu-
    // mu+
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(-13);
    p4[numstable].set(ctes_.momenta[0][5],ctes_.momenta[1][5], ctes_.momenta[2][5], ctes_.momenta[3][5]);     
    numstable++;
    // mu -
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(13);
    p4[numstable].set(ctes_.momenta[0][6],ctes_.momenta[1][6], ctes_.momenta[2][6], ctes_.momenta[3][6]);     
    numstable++;
  }
  if (flags_.pion == 1) { // pi+ pi-
    // pi+
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(211);
    p4[numstable].set(ctes_.momenta[0][5],ctes_.momenta[1][5], ctes_.momenta[2][5], ctes_.momenta[3][5]);     
    numstable++;
    // pi -
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(-211);
    p4[numstable].set(ctes_.momenta[0][6],ctes_.momenta[1][6], ctes_.momenta[2][6], ctes_.momenta[3][6]);     
    numstable++;
  }
  if (flags_.pion == 2) { // pi+ pi-2pi0
    // pi0
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(111);
    p4[numstable].set(ctes_.momenta[0][5],ctes_.momenta[1][5], ctes_.momenta[2][5], ctes_.momenta[3][5]);     
    numstable++;
    // pi0
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(111);
    p4[numstable].set(ctes_.momenta[0][6],ctes_.momenta[1][6], ctes_.momenta[2][6], ctes_.momenta[3][6]);     
    numstable++;
    // pi-
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(-211);
    p4[numstable].set(ctes_.momenta[0][7],ctes_.momenta[1][7], ctes_.momenta[2][7], ctes_.momenta[3][7]);     
    numstable++;
    // pi +
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(211);
    p4[numstable].set(ctes_.momenta[0][8],ctes_.momenta[1][8], ctes_.momenta[2][8], ctes_.momenta[3][8]);     
    numstable++;
  }   
  if (flags_.pion == 3) { // 2(pi+ pi-)
    // pi+
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(211);
    p4[numstable].set(ctes_.momenta[0][5],ctes_.momenta[1][5], ctes_.momenta[2][5], ctes_.momenta[3][5]);     
    numstable++;
    // pi-
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(-211);
    p4[numstable].set(ctes_.momenta[0][6],ctes_.momenta[1][6], ctes_.momenta[2][6], ctes_.momenta[3][6]);     
    numstable++;
    // pi+
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(-211);
    p4[numstable].set(ctes_.momenta[0][7],ctes_.momenta[1][7], ctes_.momenta[2][7], ctes_.momenta[3][7]);     
    numstable++;
    // pi -
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(211);
    p4[numstable].set(ctes_.momenta[0][8],ctes_.momenta[1][8], ctes_.momenta[2][8], ctes_.momenta[3][8]);     
    numstable++;
  }   
  if (flags_.pion == 4) { // ppbar
    // pbar
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(-2212);
    p4[numstable].set(ctes_.momenta[0][5],ctes_.momenta[1][5], ctes_.momenta[2][5], ctes_.momenta[3][5]);     
    numstable++;
    // p
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(2212);
    p4[numstable].set(ctes_.momenta[0][6],ctes_.momenta[1][6], ctes_.momenta[2][6], ctes_.momenta[3][6]);     
    numstable++;
  }
  if (flags_.pion == 5) { // nnbar
    // pbar
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(-2112);
    p4[numstable].set(ctes_.momenta[0][5],ctes_.momenta[1][5], ctes_.momenta[2][5], ctes_.momenta[3][5]);     
    numstable++;
    // p
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(2112);
    p4[numstable].set(ctes_.momenta[0][6],ctes_.momenta[1][6], ctes_.momenta[2][6], ctes_.momenta[3][6]);     
    numstable++;
  }
  if (flags_.pion == 6) { // K+ K-
    // K+
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(321);
    p4[numstable].set(ctes_.momenta[0][5],ctes_.momenta[1][5], ctes_.momenta[2][5], ctes_.momenta[3][5]);     
    numstable++;
    // K -
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(-321);
    p4[numstable].set(ctes_.momenta[0][6],ctes_.momenta[1][6], ctes_.momenta[2][6], ctes_.momenta[3][6]);     
    numstable++;
  }
  if (flags_.pion == 7) { // K0K0bar
    // Kbar
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(311);
    p4[numstable].set(ctes_.momenta[0][5],ctes_.momenta[1][5], ctes_.momenta[2][5], ctes_.momenta[3][5]);     
    numstable++;
    // K0
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(-311);
    p4[numstable].set(ctes_.momenta[0][6],ctes_.momenta[1][6], ctes_.momenta[2][6], ctes_.momenta[3][6]);     
    numstable++;
  }
  if (flags_.pion == 8) { // pi+ pi-pi0
    // pi+
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(211);
    p4[numstable].set(ctes_.momenta[0][5],ctes_.momenta[1][5], ctes_.momenta[2][5], ctes_.momenta[3][5]);     
    numstable++;
    // pi-
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(-211);
    p4[numstable].set(ctes_.momenta[0][6],ctes_.momenta[1][6], ctes_.momenta[2][6], ctes_.momenta[3][6]);     
    numstable++;
    // pi0
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(111);
    p4[numstable].set(ctes_.momenta[0][7],ctes_.momenta[1][7], ctes_.momenta[2][7], ctes_.momenta[3][7]);     
    numstable++;
  } 
  if (flags_.pion == 9) { //Lambda Lambdabar-> pi+ pi- ppbar
    // pi+
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(211);
    p4[numstable].set(ctes_.momenta[0][7],ctes_.momenta[1][7], ctes_.momenta[2][7], ctes_.momenta[3][7]);     
    numstable++;
    // pbar
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(-2212);
    p4[numstable].set(ctes_.momenta[0][8],ctes_.momenta[1][8], ctes_.momenta[2][8], ctes_.momenta[3][8]);     
    numstable++;
    // pi-
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(-211);
    p4[numstable].set(ctes_.momenta[0][9],ctes_.momenta[1][9], ctes_.momenta[2][9], ctes_.momenta[3][9]);     
    numstable++; 
    // p
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(2212);
    p4[numstable].set(ctes_.momenta[0][10],ctes_.momenta[1][10], ctes_.momenta[2][10], ctes_.momenta[3][10]);     
    numstable++;
  }   
  
 // ISR gamma      
  evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(22);
  p4[numstable].set(ctes_.momenta[0][2],ctes_.momenta[1][2], ctes_.momenta[2][2], ctes_.momenta[3][2]);     
  numstable++;
  if( ctes_.momenta[0][3] != 0 ) // second photon exists
    {
      evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(22);
      p4[numstable].set(ctes_.momenta[0][3],ctes_.momenta[1][3], ctes_.momenta[2][3], ctes_.momenta[3][3]);     
      numstable++;
    }
  
  int channel=EvtDecayTable::inChannelList(p->getId(),numstable,evtnumstable);
  more=(channel!=-1);
  if(more) {std::cout<<"Existence of mode "<<channel<<" in exclusive decay list has the same final state as this one"<<std::endl;abort(); }
  
  p->makeDaughters(numstable,evtnumstable);
  //double weight = p->initializePhaseSpace(getNDaug(),getDaugs());

  int ndaugFound=0;
  EvtVector4R SUMP4(0,0,0,0);
  for(int i=0;i<numstable;i++){
    p->getDaug(i)->init(evtnumstable[i],p4[i]);
    ndaugFound++;
  }
  if ( ndaugFound == 0 ) {
    report(ERROR,"EvtGen") << "Phokhara has failed to do a decay ";
    report(ERROR,"EvtGen") << EvtPDL::name(p->getId()).c_str() << " " << p->mass()<<endl;
    assert(0);
  }

  nevtgen++;  
  return ;
  
}



void EvtPhokhara_pipi::store(EvtDecayBase* jsdecay){

  if (nphokharadecays==ntable){

    EvtDecayBasePtr* newphokharadecays=new EvtDecayBasePtr[2*ntable+10];
    int i;
    for(i=0;i<ntable;i++){
      newphokharadecays[i]=phokharadecays[i];
    }
    ntable=2*ntable+10;
    delete [] phokharadecays;
    phokharadecays=newphokharadecays;
  }

  phokharadecays[nphokharadecays++]=jsdecay;



}


void EvtPhokhara_pipi::PhokharaInit(int dummy){
  static int first=1;
  if (first){
    
    first=0;
    //for(int i=0;i<ncommand;i++)
    // lugive0_(commands[i].c_str(),strlen(commands[i].c_str()));
  }
  
}



void EvtPhokhara_pipi::init_evt(EvtParticle* p){
 m_pion=1; 
 // mu+mu-(0),pi+pi-(1),2pi0pi+pi-(2),
 // 2pi+2pi-(3),ppbar(4),nnbar(5),
 // K+K-(6),K0K0bar(7),pi+pi-pi0(8), 
 // Lamb Lambbar->pi-pi+ppbar(9)
 // pi+pi-eta 
#include "Phokhara_init_evt.txt"
}


