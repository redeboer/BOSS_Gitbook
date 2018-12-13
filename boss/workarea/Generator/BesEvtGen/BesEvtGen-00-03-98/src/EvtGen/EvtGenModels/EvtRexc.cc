//--------------------------------------------------------------------------
// pingrg-2014-10-16
// Model REXC : used to decay vgam to the final state as in ConExc model
// Name: REXC: R-scan exclusive decay model    
// Decay cards:
//  Decay vgam
//  1  REXC;
//  Enddecay
//------------------------------------------------------------------------

#include "EvtGenBase/EvtPatches.hh"
#include <stdlib.h>
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtGenKine.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenModels/EvtRexc.hh"
#include "EvtGenBase/EvtRandom.hh"
#include "EvtGenBase/EvtHelSys.hh"
#include <string>

EvtRexc::~EvtRexc() {}

void EvtRexc::getName(std::string& model_name){

  model_name="REXC"; //R-scan exclusive decay model    

}

EvtDecayBase* EvtRexc::clone(){

  return new EvtRexc;

}


void EvtRexc::init(){

  // check that there are 0 arguments
  checkNArg(0);
  myconexc = new EvtConExc();
}

void EvtRexc::initProbMax(){

  noProbMax();

}

void EvtRexc::decay( EvtParticle *p ){
  double mhds = p->mass();
  int mymode = EvtConExc::conexcmode;
  myconexc->init_mode(mymode);
  //std::cout<<"EvtRexc:: A selected mode is "<<mymode<<" with Mhds= "<<mhds<<std::endl; //debugging
  int _ndaugs = myconexc->getNdaugs();
  EvtId *daugs=myconexc->getDaugId();
  //debugging
  /*
  std::cout<<"Ndaugs= "<<_ndaugs<<std::endl;
  for(int i=0;i<_ndaugs;i++){
    std::cout<<i<<" "<<EvtPDL::getStdHep(daugs[i])<<std::endl;
  }
  */
 Loop:
  double totmass=0;
  p->makeDaughters(_ndaugs,daugs);
  for(int i=0;i< _ndaugs;i++){  
    EvtParticle* di=p->getDaug(i);
    double xmi=EvtPDL::getMass(di->getId());
    di->setMass(xmi);
    totmass += xmi;
  }
  if(totmass > p->mass()) goto Loop;
 
  double weight = p->initializePhaseSpace( _ndaugs , daugs);
  //  std::cout<<"weight= "<<weight<<std::endl;
  if( EvtConExc::multimode==1 && _ndaugs==2){
    _daugs[0]=daugs[0];
    _daugs[1]=daugs[1];
    EvtVector4R pd1 = p->getDaug(0)->getP4(); 
    double _cms=EvtPDL::getMass(p->getId());
    EvtVector4R pcm(_cms,0,0,0);
    bool bang=hadron_angle_sampling(pd1,pcm);
    if(!bang) goto Loop;
  }else if( (2.5< mhds && mhds<3.092 || mhds>3.1012) && !angularSampling(p)) goto Loop;
  return ;
}

bool EvtRexc::angularSampling(EvtParticle* par){
  bool tagp,tagk;
  tagk=0;
  tagp=0;
  int nds = par->getNDaug();
  for(int i=0;i<par->getNDaug();i++){
    EvtId di=par->getDaug(i)->getId();
    EvtVector4R p4i = par->getDaug(i)->getP4Lab();
    int pdgcode =EvtPDL::getStdHep( di );
    double alpha=1;
    double angmax = 1+alpha;
    double costheta = cos(p4i.theta());
    double ang=1+alpha*costheta*costheta;
    double xratio = ang/angmax;
    double xi=EvtRandom::Flat(0.,1);
    //std::cout<<"pdgcode "<<pdgcode<<std::endl;
    //std::cout<<ang<<" "<<angmax<<" "<<xi<<" "<<xratio<<std::endl;
    if(xi>xratio) return false;
  }//loop over duaghters
  return true;
}

double EvtRexc::baryonAng(double mx){
  double mp=0.938;
  double u = 0.938/mx;
  u = u*u;
  double u2 = (1+u)*(1+u);
  double uu = u*(1+6*u);
  double alpha = (u2-uu)/(u2+uu);
  return alpha;
}

bool EvtRexc::hadron_angle_sampling(EvtVector4R ppi, EvtVector4R pcm){
  EvtVector4R pbst=-1*pcm;
  pbst.set(0,pcm.get(0));
  EvtVector4R p4i = boostTo(ppi,pbst);
    EvtSpinType::spintype type0 = EvtPDL::getSpinType(_daugs[0]);
    EvtSpinType::spintype type1 = EvtPDL::getSpinType(_daugs[1]);
    if(type0==EvtSpinType::SCALAR &&type1==EvtSpinType::SCALAR ){
    bool msn_ang = meson_sampling(pcm,p4i);
    return msn_ang;
    }else if(type0==EvtSpinType::VECTOR &&type1==EvtSpinType::SCALAR || type1==EvtSpinType::VECTOR &&type0==EvtSpinType::SCALAR ){
    bool msn_ang = VP_sampling(pcm,p4i);
    return msn_ang;
   }
  return true;
}

bool EvtRexc::meson_sampling(EvtVector4R pcm, EvtVector4R pi){
  EvtHelSys angles(pcm,pi);   //using helicity sys.angles
  double theta=angles.getHelAng(1);
  double phi  =angles.getHelAng(2);
  double costheta=cos(theta);  //using helicity angles in parent system

  double pm= EvtRandom::Flat(0.,1);
  double ang = 1 - costheta*costheta;
  if(pm< ang/1.) {return true;}else{return false;}
}

bool EvtRexc::VP_sampling(EvtVector4R pcm, EvtVector4R pi){
  EvtHelSys angles(pcm,pi);   //using helicity sys.angles
  double theta=angles.getHelAng(1);
  double phi  =angles.getHelAng(2);
  double costheta=cos(theta);  //using helicity angles in parent system

  double pm= EvtRandom::Flat(0.,1);
  double ang = 1 + costheta*costheta;
  if(pm< ang/2.) {return true;}else{return false;}
}
