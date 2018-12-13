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
// Module: EvtLunda.cc
//         the necessary file: lundar.F
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
#include "EvtGenModels/EvtLunda.hh"
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


int EvtLunda::nlundadecays=0;
  EvtDecayBasePtr* EvtLunda::lundadecays=0; 
int EvtLunda::ntable=0;

int EvtLunda::ncommand=0;
int EvtLunda::lcommand=0;
std::string* EvtLunda::commands=0;
int EvtLunda::nevt=0;


extern "C" {
  extern void lundar_(int *, int *, float *,int *,int *,int *,
		       double *,double *,double *,double *);
}

extern "C" {
  extern int lucomp_(int* kf);
}

extern "C" {
  extern void lugive0_(const char *cnfgstr,int length);
}

//COMMON/CHECKTAG/DECAYTAG 
extern "C" struct {
double decaytag;
} checktag_;

/*
extern struct
{
  char mypdg[200];
}mypdgfile_;
*/

/*
extern struct
{
  int xpdg; // pdg code for e+e- -> X particle, see subroutine LUBECN( , ) in luarlw.F, 
}nores_;
*/

EvtLunda::EvtLunda(){}
EvtLunda::~EvtLunda(){


  int i;


  //the deletion of commands is really uggly!

  if (nlundadecays==0) {
    delete [] commands;
    commands=0;
    return;
  }

  for(i=0;i<nlundadecays;i++){
    if (lundadecays[i]==this){
      lundadecays[i]=lundadecays[nlundadecays-1];
      nlundadecays--;
      if (nlundadecays==0) {
	delete [] commands;
	commands=0;
      }
      return;
    }
  }

  report(ERROR,"EvtGen") << "Error in destroying Lunda model!"<<endl;
 
}


void EvtLunda::getName(std::string& model_name){

  model_name="LUNDA";     

}

EvtDecayBase* EvtLunda::clone(){

  return new EvtLunda;

}


void EvtLunda::initProbMax(){

  noProbMax();

}


void EvtLunda::init(){

//  checkNArg(1);
  std::string strpdg=getenv("BESEVTGENROOT");
  strpdg +="/share/r_pdg.dat"; 
  //strcpy(mypdgfile_.mypdg, strpdg.c_str());
  std::cout<<"mypdg= "<<strpdg<<std::endl;

  if(getNArg()>2){std::cout<<"Parameter can be 1 or 2, You have "<<getNArg()<<std::endl; ::abort();}

  if (getParentId().isAlias()){

    report(ERROR,"EvtGen") << "EvtLunda finds that you are decaying the"<<endl
                           << " aliased particle "
			   << EvtPDL::name(getParentId()).c_str()
			   << " with the Lunda model"<<endl
			   << " this does not work, please modify decay table."
			   << endl;
    report(ERROR,"EvtGen") << "Will terminate execution!"<<endl;
    ::abort();

  }

  store(this);

}


std::string EvtLunda::commandName(){
     
  return std::string("LundaPar");
  
}

void EvtLunda::command(std::string cmd){
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



void EvtLunda::decay( EvtParticle *p){

  static int iniflag=1;

  static EvtId STRNG=EvtPDL::getId("string");

  int istdheppar=EvtPDL::getStdHep(p->getId());

  /*
  int Xpdg=0;
  if(getNArg()==1){
    Xpdg = getArg(0);
    if(Xpdg == 100443) Xpdg = 30443; //chage the curent pdg code to jetset pdg code for psiprime
  }
    nores_.xpdg = Xpdg;
  */

/*
  if (pycomp_(&istdheppar)==0){
    report(ERROR,"EvtGen") << "Lunda can not decay:"
      <<EvtPDL::name(p->getId()).c_str()<<endl;
    return;
  }
*/
  double mp=p->mass();
  float xmp=mp;
//  std::cout<<"float xmp="<<xmp<<std::endl;

  EvtVector4R p4[20];
  
  int i,more;
  int ip=EvtPDL::getStdHep(p->getId());
  int ndaugjs;
  static int kf[4000];
  EvtId evtnumstable[100],evtnumparton[100];
  int stableindex[100],partonindex[100];
  int numstable;
  int numparton;
  static int km[4000];
  EvtId type[MAX_DAUG];

  int isr=1; //open ISR (default)
  if(getNArg()>0){ isr=getArg(0);}

  static double px[4000],py[4000],pz[4000],e[4000];
  if (iniflag==1) lundar_(&isr,&iniflag,&xmp,&ndaugjs,kf,km,px,py,pz,e);

  if ( p->getNDaug() != 0 ) { p->deleteDaughters(true);}

  int count=0;
  bool mtg=0;
 
  do{
    //report(INFO,"EvtGen") << "calling lunda " << ip<< " " << mp <<endl;
    iniflag=iniflag+1;  //to count the event number
    //std::cout<<"Event number by Lunda: "<<iniflag<<std::endl;
modeSelection:
    lundar_(&isr,&iniflag,&xmp,&ndaugjs,kf,km,px,py,pz,e);

    mtg = checktag_.decaytag==1;
    if(mtg)std::cout<<"checktag_.decaytag="<<checktag_.decaytag<<std::endl;
    //if the Ecms is too low, Lunda fails to decay, so change to 3pi decay exclusively
    //if(mtg) {ExclusiveDecay(p); return;} //see SUBROUTINE LUBEGN(KFL,ECM) in luarlw.F

    LundaInit(0); // allow user to set LundaPar in the decay list 
    numstable=0;
    numparton=0;
    //report(INFO,"EvtGen") << "found some daughters " << ndaugjs << endl;
    double esum=0;
    //for debugging
    //for(int i=0;i<ndaugjs;i++){
    //std::cout<<"ndaugjs,kf,km,px,py,pz,e: "<<i<<", "<<km[i]<<", "<<EvtPDL::name(EvtPDL::evtIdFromStdHep(kf[i]))<<", "<<px[i]<<" ,"<<py[i]<<", "<<pz[i]<<", "<<e[i]<<std::endl; 
    //}

    for(i=0;i<ndaugjs;i++){
      if (abs(kf[i])==11 || kf[i]==92 || kf[i]==22) continue; //fill out the unstatble particle
      //std::cout<<i<<", "<<km[i]<<", "<<kf[i]<<", "<<EvtPDL::name(EvtPDL::evtIdFromStdHep(kf[i]))<<" "<<px[i]<<" ,"<<py[i]<<", "<<pz[i]<<", "<<e[i]<<std::endl; //for debugging
      if (EvtPDL::evtIdFromStdHep(kf[i])==EvtId(-1,-1)) {
	report(ERROR,"EvtGen") << "Lunda returned particle:"<<kf[i]<<endl;
	report(ERROR,"EvtGen") << "This can not be translated to evt number"<<endl;
	report(ERROR,"EvtGen") << "and the decay will be rejected!"<<endl;
	report(ERROR,"EvtGen") << "The decay was of particle:"<<ip<<endl;
	//xmp=(1+0.01)*xmp;
	std::cout<<"xmp= "<<xmp<<std::endl;
	goto modeSelection;
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
      
      esum+=e[i];
      // have to protect against negative mass^2 for massless particles
      // i.e. neutrinos and photons.
      // this is uggly but I need to fix it right now....
      
      if (px[i]*px[i]+py[i]*py[i]+pz[i]*pz[i]>=e[i]*e[i]){
	
        e[i]=sqrt(px[i]*px[i]+py[i]*py[i]+pz[i]*pz[i])+0.0000000001;
	
      }
      
      p4[i].set(e[i],px[i],py[i],pz[i]);
      
      
    }

    int channel=EvtDecayTable::inChannelList(p->getId(),numstable,evtnumstable);
  
    // Test the branching fraction of lunda
    // the specified decay mode is put as the 0-th channel with specifing mother particle
    more=(channel!=-1);
    //debugging
    if(abs(esum-xmp)>0.001 ){
      std::cout<<"Unexpected final states from Lunda with total energy "<<esum<<" unequal to "<<xmp<<std::endl;
      //xmp=(1+0.01)*xmp;
      std::cout<<"xmp= "<<xmp<<std::endl;
      goto modeSelection;
    }

    count++;
 }while( more && (count<10000) && mtg );
    //  }while( more && (count<10000) );
  
  if (count>9999) {
    report(INFO,"EvtGen") << "Too many loops in EvtLunda!!!"<<endl;
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
      report(ERROR,"EvtGen") << "Lunda has failed to do a decay ";
      report(ERROR,"EvtGen") << EvtPDL::name(p->getId()).c_str() << " " << p->mass()<<endl;
      assert(0);
    }

    fixPolarizations(p);
    //debugging
    // p->printTree();

    return ;
   
  }
  else{

    //have partons in LUNDA

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

void EvtLunda::fixPolarizations(EvtParticle *p){

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

void EvtLunda::store(EvtDecayBase* jsdecay){

  if (nlundadecays==ntable){

    EvtDecayBasePtr* newlundadecays=new EvtDecayBasePtr[2*ntable+10];
    int i;
    for(i=0;i<ntable;i++){
      newlundadecays[i]=lundadecays[i];
    }
    ntable=2*ntable+10;
    delete [] lundadecays;
    lundadecays=newlundadecays;
  }

  lundadecays[nlundadecays++]=jsdecay;



}


void EvtLunda::LundaInit(int dummy){
  
  static int first=1;
  if (first){
    
    first=0;
    for(int i=0;i<ncommand;i++)
      lugive0_(commands[i].c_str(),strlen(commands[i].c_str()));
  }

}

void EvtLunda::ExclusiveDecay(EvtParticle* p){
  EvtId daugs[8];
  int _ndaugs =4;
  daugs[0]=EvtPDL::getId(std::string("pi+"));
  daugs[1]=EvtPDL::getId(std::string("pi-"));
  daugs[2]=EvtPDL::getId(std::string("pi+"));
  daugs[3]=EvtPDL::getId(std::string("pi-"));
 checkA:
  p->makeDaughters(_ndaugs,daugs);  
  double totMass=0;
  for(int i=0;i< _ndaugs;i++){            
    EvtParticle* di=p->getDaug(i);
    double xmi=EvtPDL::getMass(di->getId()); 
    di->setMass(xmi);
    totMass+=xmi;
  }              
  if(totMass>p->mass()) goto checkA;
  p->initializePhaseSpace( _ndaugs , daugs);  
  

}
