//--------------------------------------------------------------------------
//
// Environment:
//      This software is part of models developed at BES collaboration
//      based on the EvtGen framework.  If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information: See CVS repository
//      Copyright (A) 2011      Ping Rong-Gang
//
// Module: EvtOpenCharm.cc
//
// Description: Routine to decay charmonium-> DD, DDpi according the
// cross section measurement by CLEO  PRD 80, 072001.
//
// Modification history:
//
//    Ping R.-G.    December, 2011       Module created
//
//------------------------------------------------------------------------
//

#include "EvtGenBase/EvtPatches.hh"
#include "EvtGenBase/EvtPatches.hh"
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtStringParticle.hh"
#include "EvtGenBase/EvtDecayTable.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenModels/EvtOpenCharm.hh"
#include "EvtGenModels/EvtPsi3Sdecay.hh"
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


int EvtOpenCharm::nOpencharmdecays=0;
EvtDecayBasePtr* EvtOpenCharm::Opencharmdecays=0; 
int EvtOpenCharm::ntable=0;

int EvtOpenCharm::ncommand=0;
int EvtOpenCharm::lcommand=0;
std::string* EvtOpenCharm::commands=0;
int EvtOpenCharm::nevt=0;

int EvtOpenCharm::myiter = 1;
std::vector<EvtId> EvtOpenCharm::mypar;
std::vector<int> EvtOpenCharm::vmode;
std::vector<double> EvtOpenCharm::Vcms;


EvtOpenCharm::EvtOpenCharm(){}

EvtOpenCharm::~EvtOpenCharm(){


  int i;


  //the deletion of commands is really uggly!

  if (nOpencharmdecays==0) {
    delete [] commands;
    commands=0;
    return;
  }

  for(i=0;i<nOpencharmdecays;i++){
    if (Opencharmdecays[i]==this){
      Opencharmdecays[i]=Opencharmdecays[nOpencharmdecays-1];
      nOpencharmdecays--;
      if (nOpencharmdecays==0) {
	delete [] commands;
	commands=0;
      }
      return;
    }
  }

  report(ERROR,"EvtGen") << "Error in destroying OpenCharm model!"<<endl;
 
}


void EvtOpenCharm::getName(std::string& model_name){

  model_name="OPENCHARM";     

}

EvtDecayBase* EvtOpenCharm::clone(){

  return new EvtOpenCharm;

}


void EvtOpenCharm::initProbMax(){

  noProbMax();

}


void EvtOpenCharm::init(){

//  checkNArg(1);


  if (getParentId().isAlias()){

    report(ERROR,"EvtGen") << "EvtOpenCharm finds that you are decaying the"<<endl
                           << " aliased particle "
			   << EvtPDL::name(getParentId()).c_str()
			   << " with the OpenCharm model"<<endl
			   << " this does not work, please modify decay table."
			   << endl;
    report(ERROR,"EvtGen") << "Will terminate execution!"<<endl;
    ::abort();

  }

  store(this);

}


std::string EvtOpenCharm::commandName(){
     
  return std::string("OpenCharmPar");
  
}

void EvtOpenCharm::command(std::string cmd){

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



void EvtOpenCharm::decay( EvtParticle *p){

  int istdheppar=EvtPDL::getStdHep(p->getId());

  if(istdheppar != 9000443 && istdheppar != 9010443 && istdheppar != 9030443 &&istdheppar != 9020443 ){
    std::cout<<"EvtGen: EvtOpenCharm cann't not decay the particle pid= "<<istdheppar<<endl;
    ::abort();
  }
  
  double mp=p->mass();
  float xmp=mp;
  double totEn=0;

  //debugging
  //  std::cout<<"parent "<<EvtPDL::name(p->getId())<<"float xmp="<<xmp<<" "<<p->getP4Lab()<<std::endl;

  EvtVector4R p4[20];
  
  int i,more;
  int ip=EvtPDL::getStdHep(p->getId());
  int ndaugjs;

  static int myflag;
  EvtPsi3Sdecay theIni; 
  EvtId pid = p->getId();

  static int themode;
  if(getNArg()==1){ themode = getArg(0); theIni.setMode(themode);}

  int count=0;
  do{

    theIni.PHSPDecay(p);
    std::vector<EvtVector4R> v_p4=theIni.getDaugP4();
    std::vector<EvtId> Vid=theIni.getDaugId();
    ndaugjs = Vid.size();

    EvtId  myId[3];

    for(int i=0;i<ndaugjs;i++){myId[i]=Vid[i];}
  

    if(p->getNDaug()!=0) p->resetNDaug();
    p->makeDaughters(ndaugjs,myId);

    for(int i=0;i<ndaugjs;i++){
      // std::cout<<"Vid "<<EvtPDL::name(Vid[i])<<" p4: "<<v_p4[i]<<std::endl;
      p->getDaug(i)->init(Vid[i],v_p4[i]);
    }
    

    theMode = theIni.getMode();
    p->setGeneratorFlag(theMode);


    totEn=0;    
    for(i=0;i<ndaugjs;i++){
       totEn +=p->getDaug(i)->getP4().get(0);
      if ( p->getDaug(i)->getId() ==EvtId(-1,-1)) {
	report(ERROR,"EvtGen") << "OpenCharm returned particle:"<<EvtPDL::name( p->getDaug(i)->getId() )<<endl;
	report(ERROR,"EvtGen") << "This can not be translated to evt number"<<endl;
	report(ERROR,"EvtGen") << "and the decay will be rejected!"<<endl;
	report(ERROR,"EvtGen") << "The decay was of particle:"<<ip<<endl;

      }

    }
   int channel=EvtDecayTable::inChannelList(p->getId(),ndaugjs,myId);
     
    // std::cout<<"channel= "<<channel<<std::endl;
   more=(channel!=-1);
   //if(more){std::cout<<"count= "<<count<<" inchannel= "<<channel<<std::endl;}

  count++;

  }while( more && (count<10000) );

  if(fabs(xmp-totEn)>0.01){ 
    std::cout<<"Warning:OPENCHARM generate incomplet final state, "<<mp<<" "<<totEn<<endl;
    ::abort();
  }

  if (count>9999) {
    report(INFO,"EvtGen") << "Too many loops in EvtOpenCharm!!!"<<endl;
    report(INFO,"EvtGen") << "Parent:"<<EvtPDL::name(getParentId()).c_str()<<endl;
   }

    fixPolarizations(p);

    return ;
  

}

void EvtOpenCharm::fixPolarizations(EvtParticle *p){

  //special case for now to handle the J/psi polarization

  int ndaug=p->getNDaug();
  
  int i;

  static EvtId Jpsi  =EvtPDL::getId("J/psi");
  static EvtId psip  =EvtPDL::getId("psi(2S)");
  static EvtId psipp =EvtPDL::getId("psi(3770)");
  static EvtId psi_a =EvtPDL::getId("psi(4040)");
  static EvtId psi_b =EvtPDL::getId("psi(4160)");
  static EvtId psi_c =EvtPDL::getId("psi(4260)");

  for(i=0;i<ndaug;i++){
    EvtId idp = p->getDaug(i)->getId();
    bool bflag=(idp==Jpsi || idp==psip || idp==psipp || idp==psi_a || idp==psi_b || idp ==psi_c); 
    if(bflag){
  
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

void EvtOpenCharm::store(EvtDecayBase* jsdecay){

  if (nOpencharmdecays==ntable){

    EvtDecayBasePtr* newOpencharmdecays=new EvtDecayBasePtr[2*ntable+10];
    int i;
    for(i=0;i<ntable;i++){
      newOpencharmdecays[i]=Opencharmdecays[i];
    }
    ntable=2*ntable+10;
    delete [] Opencharmdecays;
    Opencharmdecays=newOpencharmdecays;
  }

  Opencharmdecays[nOpencharmdecays++]=jsdecay;



}

void EvtOpenCharm::OpencrmInit(int dummy){
}


bool EvtOpenCharm::isbelong(EvtId myid){
  for (int i=0;i<mypar.size();i++){
    if(myid == mypar[i]) {_index = i; return true;}
  }
  return false;
}
    
int EvtOpenCharm::which_mode(EvtId myid){
  for (int i=0;i<mypar.size();i++){
    if(myid == mypar[i]){
      _index = i;
      return i;
    }
  }
  std::cout<<"EvtOpenCharm::which_mode() fails to find element"<<std::endl; abort();
}
  

