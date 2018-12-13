//--------------------------------------------------------------------------  
//   
// Environment:  
//      This software is part of models developed at BES collaboration  
//      based on the EvtGen framework.  If you use all or part  
//      of it, please give an appropriate acknowledgement.  
//        
// Copyright Information: See EvtGen/BesCopyright  
//      Copyright (A) 2006      Ping Rong-Gang
//        
// Module: EvtTauola.cc
//         the necessary file: tauola2.4.F
//                            
// Description: interface to the tauola package
//               
// Modification history:
//   
//   Ping R.-G.    2008-07-13       Module created    
//
//------------------------------------------------------------------------
// 
#include "EvtGenBase/EvtPatches.hh"
#include "EvtGenBase/EvtPatches.hh"
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtStringParticle.hh"
#include "EvtGenBase/EvtDecayTable.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenModels/EvtTauola.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtDecayBase.hh"
#include <string>
#include "EvtGenBase/EvtId.hh"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
using std::endl;
using std::fstream;
using std::ios;
using std::ofstream;
using std::resetiosflags;
using std::setiosflags;
using std::setw;


int EvtTauola::ntauoladecays=0;
  EvtDecayBasePtr* EvtTauola::tauoladecays=0; 
int EvtTauola::ntable=0;

int EvtTauola::ncommand=0;
int EvtTauola::lcommand=0;
std::string* EvtTauola::commands=0;


extern "C" {
  extern void dectes_(int *, int *,int *,int *,int *,
		       double *,double *,double *,double *);
}


EvtTauola::EvtTauola(){}

EvtTauola::~EvtTauola(){

  
  int i;


  //the deletion of commands is really uggly!

  if (ntauoladecays==0) {
    delete [] commands;
    commands=0;
    return;
  }

  for(i=0;i<ntauoladecays;i++){
    if (tauoladecays[i]==this){
      tauoladecays[i]=tauoladecays[ntauoladecays-1];
      ntauoladecays--;
      if (ntauoladecays==0) {
	delete [] commands;
	commands=0;
      }
      return;
      } 
      }  

  report(ERROR,"EvtGen") << "Error in destroying Tauola model!"<<endl;
 
}


void EvtTauola::getName(std::string& model_name){

  model_name="TAUOLA";     

}

EvtDecayBase* EvtTauola::clone(){

  return new EvtTauola;

}


void EvtTauola::initProbMax(){

  noProbMax();

}


void EvtTauola::init(){

//  checkNArg(1);


  if (getParentId().isAlias()){

    report(ERROR,"EvtGen") << "EvtTauola finds that you are decaying the"<<endl
                           << " aliased particle "
			   << EvtPDL::name(getParentId()).c_str()
			   << " with the Tauola model"<<endl
			   << " this does not work, please modify decay table."
			   << endl;
    report(ERROR,"EvtGen") << "Will terminate execution!"<<endl;
    ::abort();

      }

  store(this);
  // std::cout<<"Tauola initialization"<<std::endl;

}


std::string EvtTauola::commandName(){
     
  return std::string("TauolaPar");
  
}

void EvtTauola::command(std::string cmd){
  
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



void EvtTauola::decay( EvtParticle *p){

  static int iniflag=0;

  static EvtId STRNG=EvtPDL::getId("string");

  int istdheppar=EvtPDL::getStdHep(p->getId());

/*
  if (pycomp_(&istdheppar)==0){
    report(ERROR,"EvtGen") << "Tauola can not decay:"
      <<EvtPDL::name(p->getId()).c_str()<<endl;
    return;
  }
*/

  EvtVector4R p4[20];


  int i,more;
  int idtau=EvtPDL::getStdHep(p->getId());
  int ndaugjs;
  static int kf[20];
  EvtId evtnumstable[20],evtnumparton[20];
  int stableindex[20],partonindex[20];
  int numstable;
  int numparton;
  static int km[20];
  EvtId type[MAX_DAUG];

  static double px[20],py[20],pz[20],e[20];

  //  std::cout<<"cc: inifag,idtau,taup,polt"<<iniflag<<"; "<<idtau<<"; "<<taup[0]<<"; "<<polt[3]<<endl;
  if (iniflag==0) dectes_(&iniflag,&idtau,&ndaugjs,kf,km,px,py,pz,e);
  //std::cout<<"Tauola initialized"<<endl;
  if ( p->getNDaug() != 0 ) { p->deleteDaughters(true);}

  int count=0;

  do{
    //report(INFO,"EvtGen") << "calling tauola " << idtau<< " " << mp <<endl;
    iniflag=iniflag+1;  //to count the event number
    dectes_(&iniflag,&idtau,&ndaugjs,kf,km,px,py,pz,e);

    numstable=0;
    numparton=0;
    //    report(INFO,"EvtGen") << "found some daughters " << ndaugjs << endl;
    for(i=0;i<ndaugjs;i++){
      //std::cout<<"ndaugjs,kf,km,px,py,pz,e: "<<i<<", "<<km[i]<<", "<<kf[i]<<", "<<px[i]<<" ,"<<py[i]<<", "<<pz[i]<<", "<<e[i]<<std::endl; //for debugging

      if (EvtPDL::evtIdFromStdHep(kf[i])==EvtId(-1,-1)) {
	report(ERROR,"EvtGen") << "Tauola returned particle:"<<kf[i]<<endl;
	report(ERROR,"EvtGen") << "This can not be translated to evt number"<<endl;
	report(ERROR,"EvtGen") << "and the decay will be rejected!"<<endl;
	report(ERROR,"EvtGen") << "The decay was of particle:"<<idtau<<endl;

      }

      //sort out the partons
      if (abs(kf[i])<=6||kf[i]==21){
	partonindex[numparton]=i;
	evtnumparton[numparton]=EvtPDL::evtIdFromStdHep(kf[i]);
	numparton++;
      }
      else{
	stableindex[numstable]=i;
	evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(kf[i]); 
	numstable++;
      }


      // have to protect against negative mass^2 for massless particles
      // i.e. neutrinos and photons.
      // this is uggly but I need to fix it right now....

      if (px[i]*px[i]+py[i]*py[i]+pz[i]*pz[i]>=e[i]*e[i]){

        e[i]=sqrt(px[i]*px[i]+py[i]*py[i]+pz[i]*pz[i])+0.0000000001;

      }

      p4[i].set(e[i],px[i],py[i],pz[i]);


    }

    int channel=EvtDecayTable::inChannelList(p->getId(),numstable,evtnumstable);


   more=(channel!=-1);

      
  count++;

  }while( more && (count<10000) );

  if (count>9999) {
    report(INFO,"EvtGen") << "Too many loops in EvtTauola!!!"<<endl;
    report(INFO,"EvtGen") << "Parent:"<<EvtPDL::name(getParentId()).c_str()<<endl;
    for(i=0;i<numstable;i++){
      report(INFO,"EvtGen") << "Daug("<<i<<")"<<EvtPDL::name(evtnumstable[i]).c_str()<<endl;
    }

  }



  if (numparton==0){

    p->makeDaughters(numstable,evtnumstable);
    int ndaugFound=0;
    for(i=0;i<numstable;i++){
      p->getDaug(i)->init(evtnumstable[i],p4[stableindex[i]]);
      ndaugFound++;
    }
    if ( ndaugFound == 0 ) {
      report(ERROR,"EvtGen") << "Tauola has failed to do a decay ";
      report(ERROR,"EvtGen") << EvtPDL::name(p->getId()).c_str() << " " << p->mass()<<endl;
      assert(0);
    }

    fixPolarizations(p);

    return ;
   
  }
  else{

    //have partons in TAUOLA

    EvtVector4R p4string(0.0,0.0,0.0,0.0);

    for(i=0;i<numparton;i++){
      p4string+=p4[partonindex[i]];
    }

    int nprimary=1;
    type[0]=STRNG;
    for(i=0;i<numstable;i++){
      if (km[stableindex[i]]==0){
	type[nprimary++]=evtnumstable[i];
      }
    }

    p->makeDaughters(nprimary,type);

    p->getDaug(0)->init(STRNG,p4string);

    EvtVector4R p4partons[10];

    for(i=0;i<numparton;i++){
      p4partons[i]=p4[partonindex[i]];
    }

    ((EvtStringParticle*)p->getDaug(0))->initPartons(numparton,p4partons,evtnumparton);



    nprimary=1;

    for(i=0;i<numstable;i++){

      if (km[stableindex[i]]==0){
	p->getDaug(nprimary++)->init(evtnumstable[i],p4[stableindex[i]]);
      }
    }


    int nsecond=0;
    for(i=0;i<numstable;i++){
      if (km[stableindex[i]]!=0){
	type[nsecond++]=evtnumstable[i];
      }
    }


    p->getDaug(0)->makeDaughters(nsecond,type);

    EvtVector4R p4stringboost(p4string.get(0),-p4string.get(1),
			      -p4string.get(2),-p4string.get(3));

    nsecond=0;
    for(i=0;i<numstable;i++){
      if (km[stableindex[i]]!=0){
	p4[stableindex[i]]=boostTo(p4[stableindex[i]],p4stringboost);
	p->getDaug(0)->getDaug(nsecond)->init(evtnumstable[i],p4[stableindex[i]]);
	p->getDaug(0)->getDaug(nsecond)->setDiagonalSpinDensity();
	p->getDaug(0)->getDaug(nsecond)->decay();
	nsecond++;
      }
    }

    if ( nsecond == 0 ) {
      report(ERROR,"EvtGen") << "Jetset has failed to do a decay ";
      report(ERROR,"EvtGen") << EvtPDL::name(p->getId()).c_str() << " " << p->mass() <<endl;
      assert(0);
    }

    fixPolarizations(p);

    return ;

  }

}

void EvtTauola::fixPolarizations(EvtParticle *p){

  //special case for now to handle the tau polarization

  int ndaug=p->getNDaug();
  
  int i;

  //  static EvtId tau=EvtPDL::getId("tau-");
 static EvtId tau=getParentId();
 
  for(i=0;i<ndaug;i++){
    if(p->getDaug(i)->getId()==tau){
  
      EvtSpinDensity rho;
      
      rho.SetDim(2);
      rho.Set(0,0,1.0);
      rho.Set(0,1,0.0);

      rho.Set(1,0,0.0);
      rho.Set(1,1,1.0);


      EvtVector4R p4Psi=p->getDaug(i)->getP4();

      double alpha=atan2(p4Psi.get(2),p4Psi.get(1));
      double beta=acos(p4Psi.get(3)/p4Psi.d3mag());


      p->getDaug(i)->setSpinDensityForwardHelicityBasis(rho,alpha,beta,0.0);
      setDaughterSpinDensity(i);

    }
  }

}

void EvtTauola::store(EvtDecayBase* jsdecay){

  if (ntauoladecays==ntable){

    EvtDecayBasePtr* newtauoladecays=new EvtDecayBasePtr[2*ntable+10];
    int i;
    for(i=0;i<ntable;i++){
      newtauoladecays[i]=tauoladecays[i];
    }
    ntable=2*ntable+10;
    delete [] tauoladecays;
    tauoladecays=newtauoladecays;
  }

  tauoladecays[ntauoladecays++]=jsdecay;



}


