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
// Module: EvtPycont.cc
//
// Description: Routine to generate e+e- --> q\barq  via Jetset
//
// Modification history:
//
//    PCK     August 4, 1997        Module created
//    RS      October 28, 2002      copied from EvtJscont.cc
//
//------------------------------------------------------------------------
// 
#include "EvtGenBase/EvtPatches.hh"
#include <stdlib.h>
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtDecayTable.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenModels/EvtPycont.hh"
#include "EvtGenModels/EvtPythia.hh"
#include "EvtGenBase/EvtId.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenModels/EvtGlobalSet.hh"
#include <string>
#include <iostream>
using std::endl;

//common/CBBEAM/MAXIMUM
extern "C" struct {
double maximum;
} cbbeam_;

// COMMON/ISRFLAG/myisr
extern "C" struct {
int myisr;
} isrflag_;

extern "C" struct
{
  int mint[400];
  double vint[400];
}pyint1_;  

extern "C" struct
{
  int mstp[200];
  double parp[200]; //parp(2) is the lowers energy allowed by pythia
  int msti[200];
  double pari[200];
}pypars_;

extern "C" {
  extern void pystat_(int &);
}

extern "C" struct
{
  int dc[18];
} decaych_;


extern "C" {
  extern void initpythia_(int *);
}


EvtPycont::~EvtPycont() {
  //  int i=1;
  //  pystat_(i);
} 
 
void EvtPycont::getName(std::string& model_name)
{
  model_name="PYCONT";  
}

EvtDecayBase* EvtPycont::clone()
{
  return new EvtPycont;
}

void EvtPycont::init()
{
  // check that there are 1 argument
  if ( !(getNArg() == 12 || getNArg() == 0 || getNArg() == 1) ) {
    report(ERROR,"EvtGen") << "EvtPYCONT expects "
			   << " 12 arguments (d u s c b t e nu_e mu nu_mu tau nu_tau) but found: "
			   << getNArg() <<endl;

  }
  checkNArg(0,1,12);
  isrflag_.myisr=1; //default value, turn on ISR
   for( int i=0; i<18; i++) decaych_.dc[i]=0;
   if ( getNArg() == 12 ) {
     decaych_.dc[0]=(int)getArg(0);
     decaych_.dc[1]=(int)getArg(1);
     decaych_.dc[2]=(int)getArg(2);
     decaych_.dc[3]=(int)getArg(3);
     decaych_.dc[4]=(int)getArg(4);
     decaych_.dc[5]=(int)getArg(5);
     decaych_.dc[10]=(int)getArg(6);
     decaych_.dc[11]=(int)getArg(7);
     decaych_.dc[12]=(int)getArg(8);
     decaych_.dc[13]=(int)getArg(9);
     decaych_.dc[14]=(int)getArg(10);
     decaych_.dc[15]=(int)getArg(11);
   } else if(getNArg()==1){
     isrflag_.myisr=(int)getArg(0);
     decaych_.dc[0]=1;
     decaych_.dc[1]=1;
     decaych_.dc[2]=1;
     decaych_.dc[3]=1;
   }

}

void EvtPycont::initProbMax()
{
  noProbMax();
}

void EvtPycont::decay( EvtParticle *p)
{

  double energy=p->mass();
  cbbeam_.maximum = energy;  //ini. parp(171), see EvtPythia.cc

  //std::cout<<"energy= "<<energy<<std::endl;

  if(energy==0){std::cout<<"Particle "<<EvtPDL::name(p->getId())<<" has zero mass"<<std::endl;abort();}
  EvtPythia::pythiaInit(0);//init. for the firt events
  int mydummy=0;
  initpythia_(&mydummy);  //init. energy event by event

  EvtVector4R p4[100];
  
  
  int i,more,nson;
  int ndaugjs;
  int kf[100];
  EvtId id[100];
  int type[MAX_DAUG]; 
  
  double px[100],py[100],pz[100],e[100];
  
  if ( p->getNDaug() != 0 ) { return;}
  do{
   EvtPythia::pythiacont(&energy,&ndaugjs,kf,px,py,pz,e);
    
    double toteng=0;

    for(i=0;i<ndaugjs;i++)
      {
	   
        id[i]=EvtPDL::evtIdFromStdHep(kf[i]);
  
        type[i]=EvtPDL::getSpinType(id[i]);
  
        //std::cout<<"i, id[i] "<<i<<" "<<EvtPDL::getStdHep(id[i])<<std::endl;
   
        // have to protect against negative mass^2 for massless particles
        // i.e. neutrinos and photons.
        // this is uggly but I need to fix it right now....
        toteng += e[i];
        if (px[i]*px[i]+py[i]*py[i]+pz[i]*pz[i]>=e[i]*e[i])
          e[i]=sqrt(px[i]*px[i]+py[i]*py[i]+pz[i]*pz[i])+0.0000000000001;
    
        p4[i].set(e[i],px[i],py[i],pz[i]);

      }
    //if( fabs(toteng-energy)>0.001 ) std::cout<<"Total energy of pythia decayed particle is larger than parent energy"<<std::endl;
  
    int channel=EvtDecayTable::inChannelList(p->getId(),ndaugjs,id);
        
    more=((channel!=-1)&&(channel!=p->getChannel()) );
    //std::cout<<"more="<<more<<std::endl;
        
  }while(more);
        
  p->makeDaughters(ndaugjs,id);
        
  for(i=0;i<ndaugjs;i++)
    p->getDaug(i)->init( id[i], p4[i] );
        
  if(energy<2.0) { EvtGlobalSet::ConExcPythia=0;}else{EvtGlobalSet::ConExcPythia=1;} //cut off for minimum Mhad
  return ;
}
              
