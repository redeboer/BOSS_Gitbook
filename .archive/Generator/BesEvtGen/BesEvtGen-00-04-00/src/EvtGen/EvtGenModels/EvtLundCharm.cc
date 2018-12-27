//--------------------------------------------------------------------------  
//   
// Environment:  
//      This software is part of models developed at BES collaboration  
//      based on the EvtGen framework.  If you use all or part  
//      of it, please give an appropriate acknowledgement.  
//        
// Copyright Information: See EvtGen/BesCopyright  
//      Copyright (A) 2006      Ping Rong-Gang, Pang Cai-Ying@IHEP  
//        
// Module: EvtLundCharm.cc
//         the necessary file: jetset74.F,lund_crm1_evtgen.F
//                             fist.inc,gen.inc  mix.inc  stdhep.inc
// Description: Modified Lund model at tau-charm energy level, see
//               PHYSICAL REVIEW D, VOLUME 62, 034003
// Modification history:
//   
//   Ping R.-G.    Octo., 2007       Module created    
//
//------------------------------------------------------------------------
// 
#include "EvtGenBase/EvtPatches.hh"
#include "EvtGenBase/EvtPatches.hh"
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtStringParticle.hh"
#include "EvtGenBase/EvtParityC.hh"
#include "EvtGenBase/EvtDecayTable.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenModels/EvtLundCharm.hh"
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
using std::endl;
using std::fstream;
using std::ios;
using std::ofstream;
using std::resetiosflags;
using std::setiosflags;
using std::setw;


int EvtLundCharm::nlundcharmdecays=0;
  EvtDecayBasePtr* EvtLundCharm::lundcharmdecays=0; 
int EvtLundCharm::ntable=0;

int EvtLundCharm::ncommand=0;
int EvtLundCharm::lcommand=0;
std::string* EvtLundCharm::commands=0;
int EvtLundCharm::nevt=0;

extern "C" {
  extern int lucomp_(int* kf);
}


extern "C" {
  extern void lundcrm_(int *, int *,float *,int *,int *,int *,
		       double *,double *,double *,double *, int *);
}

extern "C" {
  extern void lugive_(const char *cnfgstr,int length);
}

EvtLundCharm::EvtLundCharm(){}

EvtLundCharm::~EvtLundCharm(){


  int i;


  //the deletion of commands is really uggly!

  if (nlundcharmdecays==0) {
    delete [] commands;
    commands=0;
    return;
  }

  for(i=0;i<nlundcharmdecays;i++){
    if (lundcharmdecays[i]==this){
      lundcharmdecays[i]=lundcharmdecays[nlundcharmdecays-1];
      nlundcharmdecays--;
      if (nlundcharmdecays==0) {
	delete [] commands;
	commands=0;
      }
      return;
    }
  }

  report(ERROR,"EvtGen") << "Error in destroying LundCharm model!"<<endl;
 
}


void EvtLundCharm::getName(std::string& model_name){

  model_name="LUNDCHARM";     

}

EvtDecayBase* EvtLundCharm::clone(){

  return new EvtLundCharm;

}


void EvtLundCharm::initProbMax(){

  noProbMax();

}


void EvtLundCharm::init(){

//  checkNArg(1);
    parityC::readParityC();

  if (getParentId().isAlias()){

    report(ERROR,"EvtGen") << "EvtLundCharm finds that you are decaying the"<<endl
                           << " aliased particle "
			   << EvtPDL::name(getParentId()).c_str()
			   << " with the LundCharm model"<<endl
			   << " this does not work, please modify decay table."
			   << endl;
    report(ERROR,"EvtGen") << "Will terminate execution!"<<endl;
    ::abort();

  }

  store(this);

}


std::string EvtLundCharm::commandName(){
     
  return std::string("LundCharmPar");
  
}

void EvtLundCharm::command(std::string cmd){

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



void EvtLundCharm::decay( EvtParticle *p){

  static int iniflag=0;

  static EvtId STRNG=EvtPDL::getId("string");

  int istdheppar=EvtPDL::getStdHep(p->getId());

/*
  if (pycomp_(&istdheppar)==0){
    report(ERROR,"EvtGen") << "LundCharm can not decay:"
      <<EvtPDL::name(p->getId()).c_str()<<endl;
    return;
  }
*/

//  std::cout<<"Lundcharm decaying "<<EvtPDL::name(p->getId())<<" mass: "<<p->getP4().mass()<<std::endl;

// no eta_c(2S) in jetset74, so we don't include it in lundcharm
  if(istdheppar != 443 && istdheppar != 100443 && istdheppar != 10441 &&istdheppar != 20443 &&istdheppar != 445 &&istdheppar != 10443 &&istdheppar != 441 &&istdheppar!= 30443){
    std::cout<<"EvtGen: EvtLundCharm cann't not decay the particle pid= "<<istdheppar<<endl;
    ::abort();
  }
  
  double mp=p->mass();
  float xmp=mp;
  double totEn=0;
//  std::cout<<"float xmp="<<xmp<<std::endl;

  EvtVector4R p4[20];
  
  int i,more, pflag;;
  int ip=EvtPDL::getStdHep(p->getId());
  int ndaugjs;
  static int kf[100];
  EvtId evtnumstable[100],evtnumparton[100];
  int stableindex[100],partonindex[100];
  int numstable;
  int numparton;
  static int km[100];
  EvtId type[MAX_DAUG];

  static double px[100],py[100],pz[100],e[100];
  static int myflag;
  if (iniflag==0) lundcrm_(&iniflag,&istdheppar,&xmp,&ndaugjs,kf,km,px,py,pz,e, &myflag);
  LundcrmInit(0); // Allow user to set LundCharmPar in decay list

  if ( p->getNDaug() != 0 ) { p->deleteDaughters(true);}

  string name_parent = EvtPDL::name(p->getId());
  double parityi=parityC::getC(name_parent);
  int count=0;
  double totalM=0;
  do{
    //report(INFO,"EvtGen") << "calling lundcharm " << ip<< " " << mp <<endl;
    iniflag=iniflag+1;  //to count the event number
    lundcrm_(&iniflag,&istdheppar,&xmp,&ndaugjs,kf,km,px,py,pz,e, &myflag);
    //-- change myflag to unsigned int

    p->setGeneratorFlag(myflag);
    // std::cout<<"EvtLundCharm::setGeneratorFalg= "<<myflag<<std::endl; 
    numstable=0;
    numparton=0;
    //report(INFO,"EvtGen") << "found some daughters " << ndaugjs << endl;
    totEn=0;    
    double parityf=1;
    for(i=0;i<ndaugjs;i++){
      //std::cout<<"ndaugjs,kf,km,px,py,pz,e: "<<i<<", "<<km[i]<<", "<<kf[i]<<", "<<px[i]<<" ,"<<py[i]<<", "<<pz[i]<<", "<<e[i]<<std::endl; //for debugging
      totEn +=e[i];
      string name_daugi = EvtPDL::name( EvtPDL::evtIdFromStdHep(kf[i]) );
      parityf = parityf*parityC::getC(name_daugi);
      totalM += EvtPDL::getMeanMass( EvtPDL::evtIdFromStdHep(kf[i]));
      if (EvtPDL::evtIdFromStdHep(kf[i])==EvtId(-1,-1)) {
	report(ERROR,"EvtGen") << "LundCharm returned particle:"<<kf[i]<<endl;
	report(ERROR,"EvtGen") << "This can not be translated to evt number"<<endl;
	report(ERROR,"EvtGen") << "and the decay will be rejected!"<<endl;
	report(ERROR,"EvtGen") << "The decay was of particle:"<<ip<<endl;

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
  
    // Test the branching fraction of lundcharm
    // the specified decay mode is put as the 0-th channel with specifing mother particle
    /*
    if(ip==100443 && channel==0){
      nevt++;
      std::cout<<"nevt= "<<nevt<<std::endl;
      channel=-1;
    }
    */
    // std::cout<<"channel= "<<channel<<std::endl;
   if(parityi!=0 && parityf!=0){
      pflag=(parityi!=parityf);
   }else{pflag=2;}

    bool eck = fabs(xmp-totEn)>0.01;
      // std::cout<<"eck= "<<eck<<", "<<fabs(xmp-totEn)<<std::endl;
    more=(channel!=-1 || pflag ==1 || eck );
  // more=(channel!=-1);

    //---debugging
    //std::cout<<"parentId= "<<istdheppar<<", pflag= "<<pflag<<std::endl;
    //if(pflag==1) abort();


  count++;

  }while( more && (count<10000) );

  /*
  if(fabs(xmp-totEn)>0.01){ 
    std::cout<<"Warning:LUNDCHARM generate incomplet final state, "<<mp<<" "<<totEn<<endl;
    ::abort();
  }
  */

  if (count>9999) {
    report(INFO,"EvtGen") << "Too many loops in EvtLundCharm!!!"<<endl;
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
      report(ERROR,"EvtGen") << "Lundcharm has failed to do a decay ";
      report(ERROR,"EvtGen") << EvtPDL::name(p->getId()).c_str() << " " << p->mass()<<endl;
      assert(0);
    }

    fixPolarizations(p);

    return ;
   
  }
  else{

    //have partons in LUNDCHARM

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

void EvtLundCharm::fixPolarizations(EvtParticle *p){

  //special case for now to handle the J/psi polarization

  int ndaug=p->getNDaug();
  
  int i;

  static EvtId Jpsi=EvtPDL::getId("J/psi");

  for(i=0;i<ndaug;i++){
    if(p->getDaug(i)->getId()==Jpsi){
  
      EvtSpinDensity rho;
      
      rho.SetDim(3);
      rho.Set(0,0,0.5);
      rho.Set(0,1,0.0);
      rho.Set(0,2,0.0);

      rho.Set(1,0,0.0);
      rho.Set(1,1,1.0);
      rho.Set(1,2,0.0);

      rho.Set(2,0,0.0);
      rho.Set(2,1,0.0);
      rho.Set(2,2,0.5);

      EvtVector4R p4Psi=p->getDaug(i)->getP4();

      double alpha=atan2(p4Psi.get(2),p4Psi.get(1));
      double beta=acos(p4Psi.get(3)/p4Psi.d3mag());


      p->getDaug(i)->setSpinDensityForwardHelicityBasis(rho,alpha,beta,0.0);
      setDaughterSpinDensity(i);

    }
  }

}

void EvtLundCharm::store(EvtDecayBase* jsdecay){

  if (nlundcharmdecays==ntable){

    EvtDecayBasePtr* newlundcharmdecays=new EvtDecayBasePtr[2*ntable+10];
    int i;
    for(i=0;i<ntable;i++){
      newlundcharmdecays[i]=lundcharmdecays[i];
    }
    ntable=2*ntable+10;
    delete [] lundcharmdecays;
    lundcharmdecays=newlundcharmdecays;
  }

  lundcharmdecays[nlundcharmdecays++]=jsdecay;



}

void EvtLundCharm::LundcrmInit(int dummy){
  
  static int first=1;
  if (first){
    
    first=0;
    for(int i=0;i<ncommand;i++)
      lugive_(commands[i].c_str(),strlen(commands[i].c_str()));
  }


}
