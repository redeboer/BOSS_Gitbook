//--------------------------------------------------------------------------
// Environment:
//      This software is part of models developed at BES collaboration
//      based on the EvtGen framework.  If you use all or part
//      of it, please give an appropriate acknowledgement.
//
// Copyright Information: See EvtGen/BesCopyright
//      Copyright (A) 2006  Ping Rong-Gang @IHEP, Wang Weiping @USTC  
//
// Module: EvtXsection.hh
//
// Description: To define cross section for the continuum exclusive process
// Experimental cross section taken from PRD73,012005, PRD76,092006, also 
// see a review: Rev. Mod. Phys. 83,1545
// Modification history:
//
//    Ping R.-G.       Nov., 2012       Module created
//    Wang Weiping:    Nov., 2017       added the mode==85: e+e- --> D_s*+D_s*-
//    Wang Weiping:    Nov., 2017       fixed final states of mode==28,33,92
//    Wang Weiping:    Nov., 2017       fixed the angle sampling of VP states
//
  /*******************--- mode definition: also see EvtXsection.cc
 mode==0: ppbar
 mode==1: nnbar
 mode==2: Lambda0 anti-Lambda0
 mode==3: Sigma0 anti-Sigma0
 mode==4: Lambda0 anti-Sigma0
 mode==5: Sigma0 anti-Lambda0
 mode==6: pi+ pi-
 mode==7: pi+ pi- pi0
 mode==8: K+K- pi0
 mode==9: KsK+pi- 
 mode==10: KsK-pi+
 mode==11: K+K-eta
 mode==12: 2(pi+pi-)
 mode==13: pi+pi-2pi0
 mode==14: K+K-pi+pi-
 mode==15: K+K-2pi0
 mode==16: 2(K+K-)
 mode==17: 2(pi+pi-)pi0
 mode==18: 2(pi+pi-)eta
 mode==19: K+K-pi+pi-pi0  		
 mode==20: K+K-pi+pi-eta
 mode==21: 3(pi+pi-)
 mode==22: 2(pi+pi-pi0)
 mode==23: phi eta
 mode==24: phi pi0
 mode==25: K+K*-
 mode==26: K-K*+
 mode==27: K_SK*0-bar
 mode==28: K*0(892)K+pi-
 mode==29: K*0(892)K-pi+
 mode==30: K*+K-pi0
 mode==31: K*-K+pi0
 mode==32: K_2*(1430)0 K+pi-
 mode==33: K_2*(1430)0 K-pi+
 mode==34: K+K-rho
 mode==35: phi pi+pi-
 mode==36: phi f0(980)
 mode==37: eta pi+pi-
 mode==38: omega pi+ pi-
 mode==39: omega f0(980)
 mode==40: eta' pi+ pi-
 mode==41: f_1(1285)pi+pi-
 mode==42: omega K+K-
 mode==43: omega pi+pi-pi0
 mode==44: Sigma+ Sigma- (Sigma0 Sigma0-bar SU(3) extention )
 mode==45: K+K-
 mode==46: K_S K_L
 mode==47: omega eta
 mode==48: phi eta'
 mode==49: phi K+K-
 mode==50: ppbar pi0
 mode==51: ppbar eta
 mode==52: omega pi0

 mode==70: D0 anti-D0
 mode==71: D+D-
 mode==72: D+D*-
 mode==73: D-D*+
 mode==74: D*+D*-
 mode==68: D0 anti-D*0
 mode==69: anti-D0 D*0
 mode==67: D*0 anti-D*0

 mode==59: D0 anti-D0 pi0
 mode==63: D+D-pi0
 mode==75: D0D-pi+
 mode==76: anti-D0 D+pi-

 mode==77: D0D*-pi+
 mode==78: anti-D0 D*+pi-
 mode==65: D-D*0pi+
 mode==66: D+ anti-D*0pi-
 mode==60: anti-D0 D*0pi0
 mode==61: D0 anti-D*0pi0
 mode==62: D+D*-pi0
 mode==64: D-D*+pi0

 mode==90: pi+pi- J/psi
 mode==92: pi0pi0 J/psi
 mode==91: pi+pi- psi(2S)
 mode==79: pi0pi0 psi(2S)
 mode==80: eta J/psi
 mode==81: pi+pi- h_c
 mode==82: pi0pi0 h_c
 mode==83: K+K- J/psi
 mode==84: KsKs J/psi

 mode==93: D_s+ D_s-
 mode==94: D_s^{*+}D_s^-
 mode==95: D_s^{*-}D_s^+
 mode==85: D_s^{*+}D_s^{*-}

 mode==96: Lambda_c+ anti-Lamba_c-
 mode==111: e+e- ->Zc(3900)+ pi-
 mode==112: e+e- ->Zc(3900)- pi+
//--- 
  For cross section, see Ref. Rev.Mod. Phys.83,1545
  *************************************/
//------------------------------------------------------------------------
#include <math.h>
#include "EvtGenBase/EvtPatches.hh"
#include <stdlib.h>
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtGenKine.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenModels/EvtPhsp.hh"
#include "EvtGenModels/EvtConExc.hh"
#include "EvtGenBase/EvtVector4R.hh"
#include "EvtGenBase/EvtParticleFactory.hh"
#include "EvtGenBase/EvtRandom.hh"
#include "EvtGenBase/EvtHelSys.hh"
#include "EvtGenBase/EvtDecayTable.hh"
#include "EvtGenBase/EvtDecayBase.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenModels/EvtGlobalSet.hh"
#include <string> 
#include <iostream>
#include <fstream>
#include <istream>
#include <strstream>
#include <stdio.h>
#include <fstream>
#include <strstream>
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TPostScript.h" 
#include "TStyle.h"
#include "TMultiGraph.h"
using namespace std;

int EvtConExc::nconexcdecays=0;
EvtDecayBasePtr* EvtConExc::conexcdecays=0;
int EvtConExc::ntable=0;

int EvtConExc::ncommand=0;
int EvtConExc::lcommand=0;
std::string* EvtConExc::commands=0;
double EvtConExc::AF[600];
double EvtConExc::AA[600];
double EvtConExc::MH[600];

double EvtConExc::mlow=0;
double EvtConExc::mup=0;

extern "C" {
  extern double dgauss_( double (*fun)(double*), double *,double *, double *);
}

extern "C" {
  extern double divdif_( float*, float *,int *, float *,int*);
}

extern "C" {
  extern void polint_( float*, float *,int *, float *,float*);
}

extern "C" {
  extern void hadr5n12_( float*, float *,float *, float *,float *, float *);
}


double Rad2difXs(double *m);
double Rad2difXs_er(double *m);

EvtXsection *EvtConExc::myxsection;
EvtXsection *EvtConExc::staxsection;
double EvtConExc::_cms;
double EvtConExc::XS_max;

double EvtConExc::_xs0=0;
double EvtConExc::_xs1=0;
double EvtConExc::_er0=0;
double EvtConExc::_er1=0;
int EvtConExc::_nevt=0;

std::ofstream oa;
int EvtConExc::getMode;
int EvtConExc::conexcmode=-1;
int EvtConExc::multimode=0;
std::vector<std::vector <double> > EvtConExc::VXS;

EvtConExc::~EvtConExc() {
//  if(_mode==74110)checkEvtRatio();
  if(myFisr.size())OutStaISR();
  if(mydbg){
    // xs->Write();
    myfile->Write();
  }

  delete myxsection;
  delete staxsection;
  gamH->deleteTree();

  //the deletion of commands is really uggly!

  int i;
  if (nconexcdecays==0) {
    delete [] commands;
    commands=0;
    return;
  }

  for(i=0;i<nconexcdecays;i++){
    if (conexcdecays[i]==this){
      conexcdecays[i]=conexcdecays[nconexcdecays-1];
      nconexcdecays--;
      if (nconexcdecays==0) {
        delete [] commands;
        commands=0;
      }
      return;
    }
  }


}

void EvtConExc::getName(std::string& model_name){

  model_name="ConExc";     

}

EvtDecayBase* EvtConExc::clone(){

  return new EvtConExc;

}


void EvtConExc::init(){
  mydbg=false;
  myFisr.clear();
  ReadVP();
  VXS.resize(120);
  for(int i=0;i<120;i++){
    VXS[i].resize(600);
  }
  _mode = getArg(0);
  for(int i=0;i<97;i++){
    if(53<=i && i<=58) continue;
    if(86<=i && i<=89) continue;
    if(_mode==74110 ||_mode==-100) vmode.push_back(i);
  }
  if(_mode==74110||_mode==-100){
    vmode.push_back(74100);
    vmode.push_back(74101);
    vmode.push_back(74102);
    vmode.push_back(74103);
    vmode.push_back(74104);
    vmode.push_back(74105);
    vmode.push_back(74110);
  }
  
  std::cout<<"==== Parameters set by users====="<<std::endl;
  for(int i=0;i<ncommand;i++){
    std::cout<<commands[i]<<std::endl;
  }
  std::cout<<"==================================="<<std::endl;
  InitPars();
  std::cout<<threshold<<" "<<beamEnergySpread<<std::endl;
  //----------------
  // check that there are 0 arguments
  // checkNArg(1);
  //Vector ISR process
  VISR=0;
  if(getNDaug()==2){
    if(getDaugs()[0]==EvtPDL::getId("gamma")&&getDaugs()[1]==EvtPDL::getId("gamma*") || getDaugs()[0]==EvtPDL::getId("gamma*")&&getDaugs()[1]==EvtPDL::getId("gamma")) VISR=1;
    }else if(getNDaug()>2){std::cout<<"Bad daughter specified"<<std::endl;abort();
  }

  //  cmsspread=0.0015; //CMC energy spread
  testflag=0;
  getResMass(); 
  if(getArg(0) == -1){ 
    radflag=0;
    mydbg=false;
    _mode   = getArg(0);
    pdgcode = getArg(1);
    pid = EvtPDL::evtIdFromStdHep(pdgcode );
    nson = getNArg()-2;
    std::cout<<"The decay daughters are "<<std::endl;
    for(int i=2;i<getNArg();i++){son[i-2]=EvtPDL::evtIdFromStdHep(getArg(i));std::cout<<EvtPDL::name(son[i-2])<<" ";}
    std::cout<<std::endl;
  }else if(getArg(0)==-2){
    radflag=0;mydbg=false;
    _mode   = getArg(0);
    nson = getNArg()-1;
    for(int i=1;i<getNArg();i++){son[i-1]=EvtPDL::evtIdFromStdHep(getArg(i));}
  }else if(getArg(0)==-100){
    _mode = getArg(0);
    multimode=1;
    vmd.clear();
    for(int i=1;i<getNArg();i++){vmd.push_back(getArg(i));}
    std::cout<<" multi-exclusive mode "<<std::endl;
    for(int i=1;i<getNArg();i++){std::cout<<getArg(i)<<" ";}
    std::cout<<std::endl;
    if(vmd[vmd.size()-1]==74110){std::cout<<"74110 is not allowd for multimode simulation"<<std::endl;abort();}
  }else if(getNArg()==1){ _mode = getArg(0); radflag=0;mydbg=false;
  }else if(getNArg()==2){ _mode = getArg(0); radflag=getArg(1);mydbg=false;
  }else if(getNArg()==3){ _mode = getArg(0); radflag=getArg(1);mydbg=getArg(2);
  }else{std::cout<<"ConExc:number of parameter should be 1,2 or 3, but you set "<<getNArg()<<std::endl;::abort(); }
  //--- debugging
  //std::cout<<_mode<<" "<<pdgcode<<" "<<nson<<" "<<EvtPDL::name(son[0])<<" "<<EvtPDL::name(son[1])<<std::endl;
  
  gamId = EvtPDL::getId(std::string("gamma"));
  init_mode(_mode);
  XS_max=-1;
  //-----debugging to make root file
  mydbg = false; //designer need to remove this line
    if(mydbg){
      myfile = new TFile("mydbg.root","RECREATE");
      xs=new TTree ("xs","momentum of rad. gamma and hadrons");
      xs->Branch("imode",&imode,"imode/I");
      xs->Branch("pgam",pgam,"pgam[4]/D");
      xs->Branch("phds",phds,"phds[4]/D");
      xs->Branch("ph1",ph1,"ph1[4]/D");
      xs->Branch("ph2",ph2,"ph2[4]/D");
      xs->Branch("mhds",&mhds,"mhds/D");
      xs->Branch("mass1",&mass1,"mass1/D");
      xs->Branch("mass2",&mass2,"mass2/D");
      xs->Branch("costheta",&costheta,"costheta/D");
      xs->Branch("cosp",&cosp,"cosp/D");
      xs->Branch("cosk",&cosk,"cosk/D");
      xs->Branch("sumxs",&sumxs,"sumxs/D");
      xs->Branch("selectmode",&selectmode,"selectmode/D");
    }
    //--- prepare the output information
    EvtId parentId =EvtPDL::getId(std::string("vpho"));
    EvtPDL::reSetWidth(parentId,0.0);
    double parentMass = EvtPDL::getMass(parentId);
    std::cout<<"parent mass = "<<parentMass<<std::endl;

    
    EvtVector4R p_init(parentMass,0.0,0.0,0.0);
    EvtParticle *p=EvtParticleFactory::particleFactory(parentId,p_init);
    theparent = p;
    myxsection = new EvtXsection (_mode);
    staxsection = new EvtXsection (_mode);
    if(_mode==-100) {
      myxsection->setModes(vmd);
      myxsection->ini_data_multimode();
      staxsection->setModes(vmd);
      staxsection->ini_data_multimode();
    }
    double mth=0;
    double mx = EvtPDL::getMeanMass(parentId); //p->getP4().mass();
    if(_mode ==-1){
      myxsection->setBW(pdgcode);
      staxsection->setBW(pdgcode);
      for(int i=0;i<nson;i++){mth +=EvtPDL::getMeanMass(son[i]);}
      if(mx<mth){std::cout<<"The CMS energy is lower than the threshold of the final states"<<std::endl;abort();}
    }else if(_mode == -2){
      mth=myxsection->getXlw();
    }else{ mth=myxsection->getXlw();}
    _cms = mx;
    _unit = myxsection->getUnit();

    std::cout<<"The specified mode is "<<_mode<<std::endl;
    EvtConExc::getMode = _mode;
    //std::cout<<"getXlw= "<<mth<<std::endl;

    Egamcut = 0.001; //set photon energy cut according to the BESIII detector
    double Esig = 0.0001;  //sigularity energy 
    double x = 2*Egamcut/_cms;
    double s = _cms*_cms;
    double mhscut = sqrt(s*(1-x));

    //for vaccum polarization
    float fe,fst2,fder,ferrder,fdeg,ferrdeg;
    fe=_cms; 
    //using the vacuum pol. value as given by  http://www-com.physik.hu-berlin.de/~fjeger/software.html
    vph=getVP(_cms);
    if(3.0943<_cms && _cms<3.102) vph=1;// For J/psi, the vacuum factor is included in the resonance
    std::cout<<"sum of leptonic, hadronic contributions(u,d,s,c,b) to vacuum polarization: "<<vph<<" @"<<fe<<"GeV"<<std::endl;

    //caculate the xs for ISR to psiprime, J/psi and phi narrow resonance.
    double totxsIRS=0;
    init_Br_ee();  
    double mthrld = EvtPDL::getMeanMass(daugs[0]); //threshold mass of hadrons
    for(int jj=1;jj<_ndaugs;jj++){
      mthrld += EvtPDL::getMeanMass(daugs[jj]);
    }

    ISRXS.clear();ISRM.clear();ISRFLAG.clear();
    for(int ii=0;ii<3;ii++){
      double mR = EvtPDL::getMeanMass(ResId[ii]);
      if(mx<mR || _mode !=74110) continue;
      double narRxs=Ros_xs(mx,BR_ee[ii],ResId[ii]);
      std::cout<<"The corss section for gamma "<<EvtPDL::name(ResId[ii]).c_str()<<" is: "<< narRxs<<"*Br (nb)."<<std::endl;
      ISRXS.push_back(narRxs); 
      ISRM.push_back(mR);
      ISRFLAG.push_back(1.);
      ISRID.push_back(ResId[ii]);
      totxsIRS += narRxs;
    }
    std::cout<<"==========================================================================="<<std::endl;

    //-- calculated with MC integration method
     double mhdL=myxsection->getXlw();
     if(mhdL<SetMthr) mhdL=SetMthr;
std::cout<<"SetMthr= "<<SetMthr<<std::endl;
     EgamH = (s-mhdL*mhdL)/2/sqrt(s);
     int nmc=1000000; 
     _xs0 = gamHXSection(s,Esig,Egamcut,nmc);  
     _er0 = _er1; // stored when calculate _xs0
     std::cout<<"_er0= "<<_er0<<std::endl;
     _xs1 = gamHXSection(s,Egamcut,EgamH,nmc);
     std::cout<<"_xs1= "<<_xs0<<std::endl;
     double xs1_err = _er1;
 
    
    //--- sigularity point x from 0 to 0.0001GeV
    double xb= 2*Esig/_cms;
    double sig_xs = SoftPhoton_xs(s,xb)*(myxsection->getXsection(mx));
    _xs0 += sig_xs;
    std::cout<<"_xs0= "<<_xs0<<std::endl;

    //prepare the observed cross section with interpotation function divdif in CERNLIB
    testflag=1;
    int Nstp=598;
    double stp=(EgamH - Egamcut)/Nstp;
    for(int i=0;i<Nstp;i++){//points within Egam(max) ~ Egamcut=25MeV
      double Eg0=EgamH - i*stp;
      double Eg1=EgamH - (i+1)*stp;
      int nmc=20000;
      int nint=100;
      //double xsi= gamHXSection(s,Eg1,Eg0,nmc);
      double xsi= trapezoid(s,Eg1,Eg0,nint,myxsection);  
      AA[i]=(Eg1+Eg0)/2;
      double mhi=sqrt(_cms*_cms-2*_cms*AA[i]);
      double mh2=sqrt(_cms*_cms-2*_cms*Eg1);
      double binwidth = mh2-mhi;
      //if(_mode==74110) xsi += addNarrowRXS(mhi,binwidth);
      if(i==0) AF[0]=xsi; 
      if(i>=1) AF[i]=AF[i-1]+xsi;
      RadXS[i]=xsi/stp;
    } 
    //add the interval 0~Esig
    AA[598]=Egamcut; AA[599]=0; //AA[i]: E_gamma
    AF[598]=AF[597];
    AF[599]=AF[598]+ _xs0;
    RadXS[599]=_xs0;

    
    //prepare observed cross section for each mode
std::cout<<"mhdL = "<<mhdL<<", SetMthr= "<<SetMthr<<", EgamH = "<<EgamH<<std::endl;
    for(int i=0;i<vmode.size();i++){
      if(_mode==74110||_mode==-100) mk_VXS(Esig,Egamcut,EgamH,i);
    }
    if(_mode==74110||_mode==-100) writeDecayTabel();
    //after mk_VXS, restore the myxsection to _mode
    if(_mode !=-100){
      delete myxsection;
      myxsection = new EvtXsection (_mode);
    }
    //debugging VXS
    /*
    double xtmp=0;
    for(int i=0;i<vmode.size();i++){
      xtmp+=VXS[i][599];
      for(int j=0;j<600;j++){
	std::cout<<VXS[i][j]<<" ";
	}
      std::cout<<std::endl;
    }
    */
    //output observed cross section for the gamma Resonance mode
    //std::cout<<"vmode.size======="<<vmode.size()<<std::endl;
    for(int i=0;i<vmode.size();i++){
      int md=vmode[i];
      if(md<74100 || md>74106) continue;
      std::string partname="";
      if(md==74100) {partname="J/psi";}
      else if(md==74101) {partname="psi(2S)";}
      else if(md==74102) {partname="psi(3770)";}
      else if(md==74103) {partname="phi";}
      else if(md==74104) {partname="omega";}
      else if(md==74105) {partname="rho0";}
      else if(md==74106) {partname="rho(3S)0";}
      else{;} 
      std::cout<<"The observed cross section for gamma "<<partname<<": "<<VXS[i][599]<<" nb"<<std::endl;
    }
    //--

    for(int i=0;i<600;i++){
      MH[i]=sqrt(_cms*_cms-2*_cms*AA[i]);
      //std::cout<<i<<" Egamma "<<AA[i]<<" Mhadons "<<MH[i]<<std::endl;
    }
    //for debugging
    //for(int i=0;i<600;i++){double s=_cms*_cms; double mhds=sqrt(s-2*_cms*AA[i]);std::cout<<"Mhds="<<mhds<<" Egam="<<AA[i]<<" "<<AF[i]<<std::endl;}
    //std::cout<<"VXS[79][599]: "<<VXS[79][599]<<" VXS[79][598]= "<<VXS[79][598]<<std::endl;
    std::cout<<"EgamH = "<<EgamH<<",  obsvXS = "<<_xs0+_xs1<<",    _xs1 = "<<_xs1 << ", _xs0 = "<<_xs0<<std::endl;
    std::cout<<"EgamH = "<<EgamH<<", AF[599] = "<< AF[599]<< ", AF[598] = "<<AF[598]<<", _xs0 = "<<_xs0<<std::endl;
    //double Etst=0.0241838;
    //std::cout<<"Etst="<<Etst<<" "<<gamHXSection(s,Etst,EgamH,nmc)<<" "<< lgr(AA,AF,600,Etst)<<std::endl; abort(); 

    //for information output
    double bornXS   = myxsection->getXsection(mx);  // std::cout<<"BornXS= "<<bornXS<<std::endl;
    double bornXS_er= myxsection->getErr(mx);
    double obsvXS   = _xs0 + _xs1; //gamHXSection(mth ,mx);
    double obsvXS_er= _er0 + xs1_err;
    double corr = obsvXS/bornXS; 
    double corr_er =corr*sqrt(bornXS_er*bornXS_er/bornXS/bornXS + obsvXS_er*obsvXS_er/obsvXS/obsvXS);

 
    if(bornXS==0){std::cout<<"EvtConExc::init : The Born cross section at this point is zero!"<<std::endl;abort();}
    if(obsvXS==0){std::cout<<"EvtConExc::init : The observed cross section at this point is zero!"<<std::endl;abort();}

    //_xs0 += bornXS;//  events with very soft photon (Egam<0.025) are taken as the born process 
    //_er0  = sqrt(_er0*_er0 + bornXS_er*bornXS_er);

    std::cout<<""<<std::endl;
    std::cout<<"========= Generation using cross section (Egamcut="<<Egamcut<<" GeV) =============="<<std::endl;
    std::cout<<" sqrt(s)= "<<mx<< " GeV "<<std::endl;
    if(_mode>=0 || _mode ==-2 || _mode ==-100){
    std::cout<<"The generated born cross section (sigma_b): "<<_xs0<<"+/-"<<_er0<<" in Unit "<<_unit.c_str()<<std::endl;
    std::cout<<"The generated radiative correction cross section (sigma_isr): "<<_xs1<<"+/-"<<xs1_err<<" in Unit "<<_unit.c_str()<<std::endl;
    std::cout<<"The Born process is sampled according to the ratio sigma_b/(sigma_b+sigma_isr)"<<std::endl;
    std::cout<<"The radiative correction factor f_ISR*f_vacuum= sigma_obs/sigma_born(s)  = "<<corr<<"+/-"<< fabs(corr_er)<<","<<std::endl;
    std::cout<<"which is calcualted with these quantities:"<<std::endl;
    std::cout<<"the observed cross section is "<<obsvXS<<"+/-"<< obsvXS_er<<_unit.c_str()<<std::endl;
    std::cout<<"and the Born cross section (s) is  "<<bornXS<<"+/-"<<bornXS_er<<_unit.c_str()<<std::endl;
    std::cout<<"and the vaccum polarziation factor (lep. + hadr.) 1/|1-Pi|^2= "<<vph<<std::endl;
    std::cout<<"Within the range "<<myxsection->getXlw()<<" ~ "<<myxsection->getXup()<<" GeV, "<<myxsection->getMsg().c_str()<<std::endl;
    std::cout<<"==========================================================================="<<std::endl;
    }else if(_mode==-1){
    std::cout<<"The generated born cross section (sigma_b): "<<_xs0<<" *Br_ee"<<" in Unit "<<_unit.c_str()<<std::endl;
    std::cout<<"The generated radiative correction cross section (sigma_isr): "<<_xs1<<"*Br_ee in Unit "<<_unit.c_str()<<std::endl;
    std::cout<<"The Born process is sampled according to the ratio sigma_b/(sigma_b+sigma_isr)"<<std::endl;
    std::cout<<"The radiative correction factor sigma_obs/sigma_born(s) = (1+delta) = "<<corr<<"+/-"<< fabs(corr_er)<<std::endl;
    std::cout<<"==========================================================================="<<std::endl;
    }
    std::cout<<std::endl;
    std::cout<<std::endl;

    findMaxXS(p);
    _mhdL=myxsection->getXlw();
    //--debugging
    //std::cout<<"maxXS= "<<maxXS<<std::endl;
    
    if(_xs0 == 0 && _xs1==0){std::cout<<"EvtConExc::ini() has zero cross section"<<std::endl;abort();}

    std::cout<<std::endl;
    std::cout<<std::endl;
    mlow=myxsection->getXlw();
    mup =myxsection->getXup();
    //--- for debugging
   if(mydbg  && _mode!=74110){
     int nd = myxsection->getYY().size();
     double  xx[10000],yy[10000],xer[10000],yer[10000];
     for(int i=0;i<nd;i++){
       xx[i]=myxsection->getXX()[i];
       yy[i]=myxsection->getYY()[i];
       yer[i]=myxsection->getEr()[i];
       xer[i]=0.;
       //std::cout<<"yy "<<yy[i]<<std::endl;
     }
     myth=new TH1F("myth","Exp.data",200,xx[0],xx[nd]);
     for(int i=0;i<nd;i++){
       myth->Fill(xx[i],yy[i]);
     }
     myth->SetError(yer);
     myth->Write();
    
     //std::cout<<"mass1= "<<mass1<<" mass2= "<<mass2<<std::endl;
   }else if(mydbg && _mode==74110){
     int nd = myxsection->getYY().size();
     double  xx[10000],yy[10000],xer[10000],yer[10000],ysum[10000],yysum[10000];
     for(int i=0;i<nd;i++){
       xx[i]=myxsection->getXX()[i];
       yy[i]=myxsection->getYY()[i];
       yer[i]=myxsection->getEr()[i];
       xer[i]=0.;
       std::cout<<"yy "<<yy[i]<<std::endl;
     }

#include "sty.C"
     double xhigh=5.0;
     double xlow = myxsection->getXlw();
     TGraphErrors *Gdt = new TGraphErrors(nd,xx,yy,xer,yer);

     myth=new TH1F("myth","Exp.data",600,xlow,xhigh);
     Xsum=new TH1F("sumxs","sum of exclusive xs",600,xlow,xhigh);
     double mstp=(xhigh-xlow)/600;
     for(int i=0;i<600;i++){
       double mx=i*mstp+xlow;
       double xsi = myxsection->getXsection(mx);
       if(xsi==0) continue;
       myth->Fill(mx,xsi);
   //std::cout<<mx<<" "<<xsi<<std::endl;
     }

     for(int i=0;i<600;i++){
       double mx=i*mstp+xlow;
       ysum[i]=sumExc(mx);
       if(ysum[i]==0) continue;
       Xsum->Fill(mx,ysum[i]);
   //std::cout<<mx<<" "<<ysum[i]<<std::endl;
     }

     for(int i=0;i<nd;i++){
       yysum[i]=sumExc(xx[i]);
     }

     myth->SetError(yer);
     myth->Write();
     Xsum->Write();

     TGraphErrors *Gsum = new TGraphErrors(nd,xx,yysum,xer,yer);
     //draw graph
     double ex[600]={600*0};
     double ey[600],ta[600];
     double exz[600]={600*0};
     double eyz[600]={600*0};
     for(int i=0;i<600;i++){
       ey[i]=AF[i]*0.001;
     }
     TGraphErrors *gr0 = new TGraphErrors(600,MH,AF,ex,ey);
     TGraphErrors *gr1 = new TGraphErrors(600,MH,RadXS,exz,eyz);
     TPostScript *ps = new TPostScript("xsobs.ps",111); 
     TCanvas *c1 = new TCanvas("c1","TGraphs for data",200,10,600,400);
     gPad-> SetLogy(1);
     //      c1->SetLogy(1); 
     gStyle->SetCanvasColor(0);
     gStyle->SetStatBorderSize(1);
     c1->Divide(1,1);

     c1->Update(); 
     ps->NewPage();
     c1->Draw();
     c1->cd(1);
     c1->SetLogy(1); 
     gr0->SetMarkerStyle(10);
     gr0->Draw("AP");
     gr0->GetXaxis()->SetTitle("M_{hadrons} (GeV)");
     gr0->GetYaxis()->SetTitle("observed cross section (nb)");
     gr0->Write();

     c1->Update(); 
     ps->NewPage();
     c1->Draw();
     c1->cd(1);
     c1->SetLogy(1); 
     gr1->SetMarkerStyle(10);
     gr1->Draw("AP");
     gr1->GetXaxis()->SetTitle("M_{hadrons} (GeV)");
     gr1->GetYaxis()->SetTitle("Rad*BornXS");
     gr1->Write();

     //--------- imposing graphs to a pad
     TMultiGraph *mg = new TMultiGraph();
     mg->SetTitle("Exclusion graphs");
     Gdt->SetMarkerStyle(2);
     Gdt->SetMarkerSize(1);
     Gsum->SetLineColor(2);
     Gsum->SetLineWidth(1);
     mg->Add(Gdt);
     mg->Add(Gsum);

     c1->Update(); 
     ps->NewPage();
     c1->Draw();
     c1->cd(1);
     gPad-> SetLogy(1);
     mg->Draw("APL");
     mg->GetXaxis()->SetTitle("M_{hadrons} (GeV)");
     mg->GetYaxis()->SetTitle("observed cross section (nb)");
     mg->Write();
     //-------

     c1->Update(); 
     ps->NewPage();
     c1->Draw();
     c1->cd(1);
     Gdt->SetMarkerStyle(2);
     Gdt->Draw("AP");
     Gdt->GetXaxis()->SetTitle("M_{hadrons} (GeV)");
     Gdt->GetYaxis()->SetTitle("observed cross section (nb)");
     Gdt->Write();

     c1->Update(); 
     ps->NewPage();
     c1->Draw();
     c1->cd(1);
     Gsum->SetMarkerStyle(2);
     Gsum->SetMarkerSize(1);
     Gsum->Draw("AP");
     Gsum->SetLineWidth(0);
     Gsum->GetXaxis()->SetTitle("M_{hadrons} (GeV)");
     Gsum->GetYaxis()->SetTitle("observed cross section (nb)");
     Gsum->Write();

     c1->Update(); 
     ps->NewPage();
     c1->Draw();
     c1->cd(1);
     // gPad-> SetLogx(1);
     Gdt->SetMarkerStyle(2);
     Gdt->SetMarkerSize(1);
     Gdt->SetMaximum(8000.0);
     Gsum->SetLineColor(2);
     Gsum->SetLineWidth(1.5);
     Gsum->Draw("AL"); //A: draw axis
     Gdt->Draw("P");   // superpose to the Gsum, without A-option
     Gsum->GetXaxis()->SetTitle("M_{hadrons} (GeV)");
     Gsum->GetYaxis()->SetTitle("cross section (nb)");
     Gsum->Write();

     ps->Close(); 
   } //if(mydbg)_block
   //-------------------------

}


//--
void EvtConExc::init_mode(int mode){
  if(mode==0){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("p+"));
    daugs[1]=EvtPDL::getId(std::string("anti-p-"));
  }else if(mode==1){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("n0"));
    daugs[1]=EvtPDL::getId(std::string("anti-n0"));
  }else if(mode==2){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("Lambda0"));
    daugs[1]=EvtPDL::getId(std::string("anti-Lambda0"));
  }else if(mode==3){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("Sigma0"));
    daugs[1]=EvtPDL::getId(std::string("anti-Sigma0"));
  }else if(mode==4){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("Lambda0"));
    daugs[1]=EvtPDL::getId(std::string("anti-Sigma0"));
  }else if(mode==5){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("Sigma0"));
    daugs[1]=EvtPDL::getId(std::string("anti-Lambda0"));
  }else if(mode==6){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("pi+"));
    daugs[1]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==7){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("pi+"));
    daugs[1]=EvtPDL::getId(std::string("pi-"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==8){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("K+"));
    daugs[1]=EvtPDL::getId(std::string("K-"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==9){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("K_S0"));
    daugs[1]=EvtPDL::getId(std::string("K+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==10){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("K_S0"));
    daugs[1]=EvtPDL::getId(std::string("K-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
  }else if(mode==11){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("K+"));
    daugs[1]=EvtPDL::getId(std::string("K+"));
    daugs[2]=EvtPDL::getId(std::string("eta"));
  }else if(mode==12){
    _ndaugs =4;
    daugs[0]=EvtPDL::getId(std::string("pi+"));
    daugs[1]=EvtPDL::getId(std::string("pi-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
    daugs[3]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==13){
    _ndaugs =4;
    daugs[0]=EvtPDL::getId(std::string("pi+"));
    daugs[1]=EvtPDL::getId(std::string("pi-"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
    daugs[3]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==14){
    _ndaugs =4;
    daugs[0]=EvtPDL::getId(std::string("K+"));
    daugs[1]=EvtPDL::getId(std::string("K-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
    daugs[3]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==15){
    _ndaugs =4;
    daugs[0]=EvtPDL::getId(std::string("K+"));
    daugs[1]=EvtPDL::getId(std::string("K-"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
    daugs[3]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==16){
    _ndaugs =4;
    daugs[0]=EvtPDL::getId(std::string("K+"));
    daugs[1]=EvtPDL::getId(std::string("K-"));
    daugs[2]=EvtPDL::getId(std::string("K+"));
    daugs[3]=EvtPDL::getId(std::string("K-"));
  }else if(mode==17){ 
    _ndaugs =5;
    daugs[0]=EvtPDL::getId(std::string("pi+"));
    daugs[1]=EvtPDL::getId(std::string("pi-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
    daugs[3]=EvtPDL::getId(std::string("pi-"));
    daugs[4]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==18){
    _ndaugs =5;
    daugs[0]=EvtPDL::getId(std::string("pi+"));
    daugs[1]=EvtPDL::getId(std::string("pi-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
    daugs[3]=EvtPDL::getId(std::string("pi-"));
    daugs[4]=EvtPDL::getId(std::string("eta"));
  }else if(mode==19){
    _ndaugs =5;
    daugs[0]=EvtPDL::getId(std::string("K+"));
    daugs[1]=EvtPDL::getId(std::string("K-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
    daugs[3]=EvtPDL::getId(std::string("pi-"));
    daugs[4]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==20){
    _ndaugs =5;
    daugs[0]=EvtPDL::getId(std::string("K+"));
    daugs[1]=EvtPDL::getId(std::string("K-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
    daugs[3]=EvtPDL::getId(std::string("pi-"));
    daugs[4]=EvtPDL::getId(std::string("eta"));
  }else if(mode==21){
    _ndaugs =6;
    daugs[0]=EvtPDL::getId(std::string("pi+"));
    daugs[1]=EvtPDL::getId(std::string("pi-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
    daugs[3]=EvtPDL::getId(std::string("pi-"));
    daugs[4]=EvtPDL::getId(std::string("pi+"));
    daugs[5]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==22){
    _ndaugs =6;
    daugs[0]=EvtPDL::getId(std::string("pi+"));
    daugs[1]=EvtPDL::getId(std::string("pi-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
    daugs[3]=EvtPDL::getId(std::string("pi-"));
    daugs[4]=EvtPDL::getId(std::string("pi0"));
    daugs[5]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==23){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("eta"));
    daugs[1]=EvtPDL::getId(std::string("phi"));
  }else if(mode==24){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("phi"));
    daugs[1]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==25){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("K+"));
    daugs[1]=EvtPDL::getId(std::string("K*-"));
  }else if(mode==26){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("K-"));
    daugs[1]=EvtPDL::getId(std::string("K*+"));
  }else if(mode==27){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("K_S0"));
    daugs[1]=EvtPDL::getId(std::string("anti-K*0"));
  }else if(mode==28){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("anti-K*0"));
    daugs[1]=EvtPDL::getId(std::string("K+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==29){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("K*0"));
    daugs[1]=EvtPDL::getId(std::string("K-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
  }else if(mode==30){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("K*+"));
    daugs[1]=EvtPDL::getId(std::string("K-"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==31){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("K*-"));
    daugs[1]=EvtPDL::getId(std::string("K+"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==32){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("anti-K_2*0"));
    daugs[1]=EvtPDL::getId(std::string("K+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==33){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("K_2*0"));
    daugs[1]=EvtPDL::getId(std::string("K-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
  }else if(mode==34){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("K+"));
    daugs[1]=EvtPDL::getId(std::string("K-"));
    daugs[2]=EvtPDL::getId(std::string("rho0"));
  }else if(mode==35){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("phi"));
    daugs[1]=EvtPDL::getId(std::string("pi-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
  }else if(mode==36){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("phi"));
    daugs[1]=EvtPDL::getId(std::string("f_0"));
  }else if(mode==37){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("eta"));
    daugs[1]=EvtPDL::getId(std::string("pi+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==38){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("omega"));
    daugs[1]=EvtPDL::getId(std::string("pi+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==39){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("omega"));
    daugs[1]=EvtPDL::getId(std::string("f_0"));
  }else if(mode==40){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("eta'"));
    daugs[1]=EvtPDL::getId(std::string("pi+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==41){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("f_1"));
    daugs[1]=EvtPDL::getId(std::string("pi+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==42){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("omega"));
    daugs[1]=EvtPDL::getId(std::string("K+"));
    daugs[2]=EvtPDL::getId(std::string("K-"));
  }else if(mode==43){
    _ndaugs =4;
    daugs[0]=EvtPDL::getId(std::string("omega"));
    daugs[1]=EvtPDL::getId(std::string("pi+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
    daugs[3]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==44){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("Sigma-"));
    daugs[1]=EvtPDL::getId(std::string("anti-Sigma+"));
  }else if(mode==45){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("K+"));
    daugs[1]=EvtPDL::getId(std::string("K-"));
  }else if(mode==46){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("K_S0"));
    daugs[1]=EvtPDL::getId(std::string("K_L0"));
  }else if(mode==47){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("omega"));
    daugs[1]=EvtPDL::getId(std::string("eta"));
  }else if(mode==48){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("phi"));
    daugs[1]=EvtPDL::getId(std::string("eta'"));
  }else if(mode==49){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("phi"));
    daugs[1]=EvtPDL::getId(std::string("K+"));
    daugs[2]=EvtPDL::getId(std::string("K-"));
  }else if(mode==50){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("p+"));
    daugs[1]=EvtPDL::getId(std::string("anti-p-"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==51){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("p+"));
    daugs[1]=EvtPDL::getId(std::string("anti-p-"));
    daugs[2]=EvtPDL::getId(std::string("eta"));
  }else if(mode==52){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("omega"));
    daugs[1]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==59){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("anti-D0"));
    daugs[1]=EvtPDL::getId(std::string("D0"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==60){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("anti-D0"));
    daugs[1]=EvtPDL::getId(std::string("D*0"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==61){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("D0"));
    daugs[1]=EvtPDL::getId(std::string("anti-D*0"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==62){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("D+"));
    daugs[1]=EvtPDL::getId(std::string("D*-"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==63){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("D-"));
    daugs[1]=EvtPDL::getId(std::string("D+"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==64){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("D-"));
    daugs[1]=EvtPDL::getId(std::string("D*+"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==65){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("D-"));
    daugs[1]=EvtPDL::getId(std::string("D*0"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
  }else if(mode==66){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("D+"));
    daugs[1]=EvtPDL::getId(std::string("anti-D*0"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==67){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("D*0"));
    daugs[1]=EvtPDL::getId(std::string("anti-D*0"));
  }else if(mode==68){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("D0"));
    daugs[1]=EvtPDL::getId(std::string("anti-D*0"));
  }else if(mode==69){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("anti-D0"));
    daugs[1]=EvtPDL::getId(std::string("D*0"));
  }else if(mode==70){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("D0"));
    daugs[1]=EvtPDL::getId(std::string("anti-D0"));
  }else if(mode==71){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("D+"));
    daugs[1]=EvtPDL::getId(std::string("D-"));
  }else if(mode==72){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("D+"));
    daugs[1]=EvtPDL::getId(std::string("D*-"));
  }else if(mode==73){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("D-"));
    daugs[1]=EvtPDL::getId(std::string("D*+"));
  }else if(mode==74){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("D*+"));
    daugs[1]=EvtPDL::getId(std::string("D*-"));
  }else if(mode==75){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("D0"));
    daugs[1]=EvtPDL::getId(std::string("D-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
  }else if(mode==76){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("anti-D0"));
    daugs[1]=EvtPDL::getId(std::string("D+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==77){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("D0"));
    daugs[1]=EvtPDL::getId(std::string("D*-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
  }else if(mode==78){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("anti-D0"));
    daugs[1]=EvtPDL::getId(std::string("D*+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==79){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("psi(2S)"));
    daugs[1]=EvtPDL::getId(std::string("pi0"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==80){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("eta"));
    daugs[1]=EvtPDL::getId(std::string("J/psi"));
  }else if(mode==81){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("h_c"));
    daugs[1]=EvtPDL::getId(std::string("pi+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==82){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("h_c"));
    daugs[1]=EvtPDL::getId(std::string("pi0"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==83){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("J/psi"));
    daugs[1]=EvtPDL::getId(std::string("K+"));
    daugs[2]=EvtPDL::getId(std::string("K-"));
  }else if(mode==84){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("J/psi"));
    daugs[1]=EvtPDL::getId(std::string("K_S0"));
    daugs[2]=EvtPDL::getId(std::string("K_S0"));
  }else if(mode==85){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("D_s*+"));
    daugs[1]=EvtPDL::getId(std::string("D_s*-"));
  }else if(mode==90){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("J/psi"));
    daugs[1]=EvtPDL::getId(std::string("pi+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==91){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("psi(2S)"));
    daugs[1]=EvtPDL::getId(std::string("pi+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==92){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("J/psi"));
    daugs[1]=EvtPDL::getId(std::string("pi0"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==93){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("D_s+"));
    daugs[1]=EvtPDL::getId(std::string("D_s-"));
  }else if(mode==94){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("D_s*+"));
    daugs[1]=EvtPDL::getId(std::string("D_s-"));
  }else if(mode==95){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("D_s*-"));
    daugs[1]=EvtPDL::getId(std::string("D_s+"));
  }else if(mode==96){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("Lambda_c+"));
    daugs[1]=EvtPDL::getId(std::string("anti-Lambda_c-"));
  }else if(mode==111){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("Zc(4020)+"));
    daugs[1]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==112){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("Zc(4020)-"));
    daugs[1]=EvtPDL::getId(std::string("pi+"));
  }else if(mode==74100){
    _ndaugs =1;
    daugs[0]=EvtPDL::getId(std::string("J/psi"));     
  }else if(mode==74101){
    _ndaugs =1;
    daugs[0]=EvtPDL::getId(std::string("psi(2S)"));
  }else if(mode==74102){
    _ndaugs =1;
    daugs[0]=EvtPDL::getId(std::string("psi(3770)"));  
  }else if(mode==74103){
    _ndaugs =1;
    daugs[0]=EvtPDL::getId(std::string("phi"));
  }else if(mode==74104){
    _ndaugs =1;
    daugs[0]=EvtPDL::getId(std::string("omega"));
  }else if(mode==74105){
    _ndaugs =1;
    daugs[0]=EvtPDL::getId(std::string("rho0"));
  }else if(mode==74106){
    _ndaugs =1;
    daugs[0]=EvtPDL::getId(std::string("rho(3S)0")); 
  }else if(mode==74107){
    _ndaugs =1;
    daugs[0]=EvtPDL::getId(std::string("omega(2S)"));          
  }else if(mode==74110){
    _ndaugs =1;
    daugs[0]=EvtPDL::getId(std::string("gamma*")); 
    EvtId  myvpho = EvtPDL::getId(std::string("gamma*"));
    EvtPDL::reSetMass(myvpho,mhds*0.9); //for calculating maxXS, it will be resent when this mode is selected   
    _modeFlag.clear();
    _modeFlag.push_back(74110); //R-value generator tag
    _modeFlag.push_back(0);     //to contain the mode selected
  }else if(mode==-1){
    _ndaugs = nson;
    for(int i=0;i<nson;i++){ daugs[i]=son[i];  }
    std::cout<<"The paraent particle: "<<EvtPDL::name(pid)<<std::endl;
  }else if(mode==-2){
    _ndaugs = nson;
    for(int i=0;i<nson;i++){  daugs[i]=son[i];  }
  }else if(mode==-100){
    _ndaugs =1;
    daugs[0]=EvtPDL::getId(std::string("gamma*"));
    _modeFlag.clear();
    _modeFlag.push_back(-100); //R-value generator tag
    _modeFlag.push_back(0);     //to contain the mode selected 
    return;
  }else if(mode==10000){//use for check ISR
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("pi+"));
    daugs[1]=EvtPDL::getId(std::string("pi-"));
  }else {
    std::cout<<"Bad mode_index number " <<mode<<", please refer to the manual."<<std::endl;
    ::abort();
  }
  // std::cout<<_ndaugs<<", "<<daugs[0]<<daugs[1]<<std::endl;

  if(VISR){
    _ndaugs=2;
    daugs[0]=getDaugs()[0];//EvtPDL::getId(std::string("gamma"));
    daugs[1]=getDaugs()[1];//EvtPDL::getId(std::string("gamma*"));
  }

  double fmth=0;
  for(int i=0;i<_ndaugs;i++){
    double xmi=EvtPDL::getMinMass(daugs[i]);
    fmth +=xmi;
  }
  myxsection = new EvtXsection (mode);
  Mthr=myxsection->getXlw();   
  if(!(_mode==74110 || _mode==-100)){
   if(Mthr<fmth) {std::cout<<"Low end of cross section: ("<<Mthr<<" < (mass of final state)"<<fmth<<") GeV."<< std::endl; abort();}
  }
}

//--
std::vector<EvtId> EvtConExc::get_mode(int mode){
  int _ndaugs;
  EvtId daugs[100];
  if(mode==0){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("p+"));
    daugs[1]=EvtPDL::getId(std::string("anti-p-"));
  }else if(mode==1){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("n0"));
    daugs[1]=EvtPDL::getId(std::string("anti-n0"));
  }else if(mode==2){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("Lambda0"));
    daugs[1]=EvtPDL::getId(std::string("anti-Lambda0"));
  }else if(mode==3){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("Sigma0"));
    daugs[1]=EvtPDL::getId(std::string("anti-Sigma0"));
  }else if(mode==4){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("Lambda0"));
    daugs[1]=EvtPDL::getId(std::string("anti-Sigma0"));
  }else if(mode==5){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("Sigma0"));
    daugs[1]=EvtPDL::getId(std::string("anti-Lambda0"));
  }else if(mode ==6){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("pi+"));
    daugs[1]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==7){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("pi+"));
    daugs[1]=EvtPDL::getId(std::string("pi-"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==8){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("K+"));
    daugs[1]=EvtPDL::getId(std::string("K-"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==9){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("K_S0"));
    daugs[1]=EvtPDL::getId(std::string("K+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==10){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("K_S0"));
    daugs[1]=EvtPDL::getId(std::string("K-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
  }else if(mode==11){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("K+"));
    daugs[1]=EvtPDL::getId(std::string("K-"));
    daugs[2]=EvtPDL::getId(std::string("eta"));
  }else if(mode==12){
    _ndaugs =4;
    daugs[0]=EvtPDL::getId(std::string("pi+"));
    daugs[1]=EvtPDL::getId(std::string("pi-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
    daugs[3]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==13){
    _ndaugs =4;
    daugs[0]=EvtPDL::getId(std::string("pi+"));
    daugs[1]=EvtPDL::getId(std::string("pi-"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
    daugs[3]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==14){
    _ndaugs =4;
    daugs[0]=EvtPDL::getId(std::string("K+"));
    daugs[1]=EvtPDL::getId(std::string("K-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
    daugs[3]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==15){
    _ndaugs =4;
    daugs[0]=EvtPDL::getId(std::string("K+"));
    daugs[1]=EvtPDL::getId(std::string("K-"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
    daugs[3]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==16){
    _ndaugs =4;
    daugs[0]=EvtPDL::getId(std::string("K+"));
    daugs[1]=EvtPDL::getId(std::string("K-"));
    daugs[2]=EvtPDL::getId(std::string("K+"));
    daugs[3]=EvtPDL::getId(std::string("K-"));
  }else if(mode==17){ 
    _ndaugs =5;
    daugs[0]=EvtPDL::getId(std::string("pi+"));
    daugs[1]=EvtPDL::getId(std::string("pi-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
    daugs[3]=EvtPDL::getId(std::string("pi-"));
    daugs[4]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==18){
    _ndaugs =5;
    daugs[0]=EvtPDL::getId(std::string("pi+"));
    daugs[1]=EvtPDL::getId(std::string("pi-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
    daugs[3]=EvtPDL::getId(std::string("pi-"));
    daugs[4]=EvtPDL::getId(std::string("eta"));
  }else if(mode==19){
    _ndaugs =5;
    daugs[0]=EvtPDL::getId(std::string("K+"));
    daugs[1]=EvtPDL::getId(std::string("K-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
    daugs[3]=EvtPDL::getId(std::string("pi-"));
    daugs[4]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==20){
    _ndaugs =5;
    daugs[0]=EvtPDL::getId(std::string("K+"));
    daugs[1]=EvtPDL::getId(std::string("K-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
    daugs[3]=EvtPDL::getId(std::string("pi-"));
    daugs[4]=EvtPDL::getId(std::string("eta"));
  }else if(mode==21){
    _ndaugs =6;
    daugs[0]=EvtPDL::getId(std::string("pi+"));
    daugs[1]=EvtPDL::getId(std::string("pi-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
    daugs[3]=EvtPDL::getId(std::string("pi-"));
    daugs[4]=EvtPDL::getId(std::string("pi+"));
    daugs[5]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==22){
    _ndaugs =6;
    daugs[0]=EvtPDL::getId(std::string("pi+"));
    daugs[1]=EvtPDL::getId(std::string("pi-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
    daugs[3]=EvtPDL::getId(std::string("pi-"));
    daugs[4]=EvtPDL::getId(std::string("pi0"));
    daugs[5]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==23){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("eta"));
    daugs[1]=EvtPDL::getId(std::string("phi"));
  }else if(mode==24){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("phi"));
    daugs[1]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==25){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("K+"));
    daugs[1]=EvtPDL::getId(std::string("K*-"));
  }else if(mode==26){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("K-"));
    daugs[1]=EvtPDL::getId(std::string("K*+"));
  }else if(mode==27){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("K_S0"));
    daugs[1]=EvtPDL::getId(std::string("anti-K*0"));
  }else if(mode==28){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("anti-K*0"));
    daugs[1]=EvtPDL::getId(std::string("K+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==29){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("K*0"));
    daugs[1]=EvtPDL::getId(std::string("K-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
  }else if(mode==30){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("K*+"));
    daugs[1]=EvtPDL::getId(std::string("K-"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==31){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("K*-"));
    daugs[1]=EvtPDL::getId(std::string("K+"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==32){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("anti-K_2*0"));
    daugs[1]=EvtPDL::getId(std::string("K+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==33){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("K_2*0"));
    daugs[1]=EvtPDL::getId(std::string("K-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
  }else if(mode==34){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("K+"));
    daugs[1]=EvtPDL::getId(std::string("K-"));
    daugs[2]=EvtPDL::getId(std::string("rho0"));
  }else if(mode==35){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("phi"));
    daugs[1]=EvtPDL::getId(std::string("pi-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
  }else if(mode==36){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("phi"));
    daugs[1]=EvtPDL::getId(std::string("f_0"));
  }else if(mode==37){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("eta"));
    daugs[1]=EvtPDL::getId(std::string("pi+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==38){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("omega"));
    daugs[1]=EvtPDL::getId(std::string("pi+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==39){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("omega"));
    daugs[1]=EvtPDL::getId(std::string("f_0"));
  }else if(mode==40){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("eta'"));
    daugs[1]=EvtPDL::getId(std::string("pi+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==41){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("f_1"));
    daugs[1]=EvtPDL::getId(std::string("pi+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==42){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("omega"));
    daugs[1]=EvtPDL::getId(std::string("K+"));
    daugs[2]=EvtPDL::getId(std::string("K-"));
  }else if(mode==43){
    _ndaugs =4;
    daugs[0]=EvtPDL::getId(std::string("omega"));
    daugs[1]=EvtPDL::getId(std::string("pi+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
    daugs[3]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==44){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("Sigma-"));
    daugs[1]=EvtPDL::getId(std::string("anti-Sigma+"));
  }else if(mode==45){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("K+"));
    daugs[1]=EvtPDL::getId(std::string("K-"));
  }else if(mode==46){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("K_S0"));
    daugs[1]=EvtPDL::getId(std::string("K_L0"));
  }else if(mode==47){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("omega"));
    daugs[1]=EvtPDL::getId(std::string("eta"));
  }else if(mode==48){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("phi"));
    daugs[1]=EvtPDL::getId(std::string("eta'"));
  }else if(mode==49){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("phi"));
    daugs[1]=EvtPDL::getId(std::string("K+"));
    daugs[2]=EvtPDL::getId(std::string("K-"));
  }else if(mode==50){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("p+"));
    daugs[1]=EvtPDL::getId(std::string("anti-p-"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==51){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("p+"));
    daugs[1]=EvtPDL::getId(std::string("anti-p-"));
    daugs[2]=EvtPDL::getId(std::string("eta"));
  }else if(mode==52){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("omega"));
    daugs[1]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==59){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("anti-D0"));
    daugs[1]=EvtPDL::getId(std::string("D0"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==60){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("anti-D0"));
    daugs[1]=EvtPDL::getId(std::string("D*0"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==61){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("D0"));
    daugs[1]=EvtPDL::getId(std::string("anti-D*0"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==62){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("D+"));
    daugs[1]=EvtPDL::getId(std::string("D*-"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==63){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("D-"));
    daugs[1]=EvtPDL::getId(std::string("D+"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==64){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("D-"));
    daugs[1]=EvtPDL::getId(std::string("D*+"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==65){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("D-"));
    daugs[1]=EvtPDL::getId(std::string("D*0"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
  }else if(mode==66){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("D+"));
    daugs[1]=EvtPDL::getId(std::string("anti-D*0"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==67){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("D*0"));
    daugs[1]=EvtPDL::getId(std::string("anti-D*0"));
  }else if(mode==68){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("D0"));
    daugs[1]=EvtPDL::getId(std::string("anti-D*0"));
  }else if(mode==69){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("anti-D0"));
    daugs[1]=EvtPDL::getId(std::string("D*0"));
  }else if(mode==70){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("D0"));
    daugs[1]=EvtPDL::getId(std::string("anti-D0"));
  }else if(mode==71){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("D+"));
    daugs[1]=EvtPDL::getId(std::string("D-"));
  }else if(mode==72){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("D+"));
    daugs[1]=EvtPDL::getId(std::string("D*-"));
  }else if(mode==73){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("D-"));
    daugs[1]=EvtPDL::getId(std::string("D*+"));
  }else if(mode==74){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("D*+"));
    daugs[1]=EvtPDL::getId(std::string("D*-"));
  }else if(mode==75){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("D0"));
    daugs[1]=EvtPDL::getId(std::string("D-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
  }else if(mode==76){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("anti-D0"));
    daugs[1]=EvtPDL::getId(std::string("D+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==77){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("D0"));
    daugs[1]=EvtPDL::getId(std::string("D*-"));
    daugs[2]=EvtPDL::getId(std::string("pi+"));
  }else if(mode==78){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("anti-D0"));
    daugs[1]=EvtPDL::getId(std::string("D*+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==79){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("psi(2S)"));
    daugs[1]=EvtPDL::getId(std::string("pi0"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==80){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("eta"));
    daugs[1]=EvtPDL::getId(std::string("J/psi"));
  }else if(mode==81){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("h_c"));
    daugs[1]=EvtPDL::getId(std::string("pi+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==82){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("h_c"));
    daugs[1]=EvtPDL::getId(std::string("pi0"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==83){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("J/psi"));
    daugs[1]=EvtPDL::getId(std::string("K+"));
    daugs[2]=EvtPDL::getId(std::string("K-"));
  }else if(mode==84){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("J/psi"));
    daugs[1]=EvtPDL::getId(std::string("K_S0"));
    daugs[2]=EvtPDL::getId(std::string("K_S0"));
  }else if(mode==85){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("D_s*+"));
    daugs[1]=EvtPDL::getId(std::string("D_s*-"));
  }else if(mode==90){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("J/psi"));
    daugs[1]=EvtPDL::getId(std::string("pi+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==91){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("psi(2S)"));
    daugs[1]=EvtPDL::getId(std::string("pi+"));
    daugs[2]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==92){
    _ndaugs =3;
    daugs[0]=EvtPDL::getId(std::string("J/psi"));
    daugs[1]=EvtPDL::getId(std::string("pi0"));
    daugs[2]=EvtPDL::getId(std::string("pi0"));
  }else if(mode==93){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("D_s+"));
    daugs[1]=EvtPDL::getId(std::string("D_s-"));
  }else if(mode==94){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("D_s*+"));
    daugs[1]=EvtPDL::getId(std::string("D_s-"));
  }else if(mode==95){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("D_s*-"));
    daugs[1]=EvtPDL::getId(std::string("D_s+"));
  }else if(mode==96){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("Lambda_c+"));
    daugs[1]=EvtPDL::getId(std::string("anti-Lambda_c-"));
  }else if(mode==111){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("Zc(4020)+"));
    daugs[1]=EvtPDL::getId(std::string("pi-"));
  }else if(mode==112){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("Zc(4020)-"));
    daugs[1]=EvtPDL::getId(std::string("pi+"));
  }else if(mode==74100){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("J/psi"));   
    daugs[1]=EvtPDL::getId(std::string("gamma"));  
  }else if(mode==74101){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("psi(2S)"));
    daugs[1]=EvtPDL::getId(std::string("gamma"));
  }else if(mode==74102){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("psi(3770)"));
    daugs[1]=EvtPDL::getId(std::string("gamma"));  
  }else if(mode==74103){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("phi"));
    daugs[1]=EvtPDL::getId(std::string("gamma"));
  }else if(mode==74104){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("omega"));
    daugs[1]=EvtPDL::getId(std::string("gamma"));
  }else if(mode==74105){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("rho0"));
    daugs[1]=EvtPDL::getId(std::string("gamma"));
  }else if(mode==74106){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("rho(3S)0"));
    daugs[1]=EvtPDL::getId(std::string("gamma")); 
  }else if(mode==74107){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("omega(2S)"));
    daugs[1]=EvtPDL::getId(std::string("gamma"));          
  }else if(mode==74110){
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("gamma*")); 
    daugs[1]=EvtPDL::getId(std::string("gamma"));
    EvtId  myvpho = EvtPDL::getId(std::string("gamma*"));
    EvtPDL::reSetMass(myvpho,mhds*0.9); //for calculating maxXS, it will be resent when this mode is selected   
    _modeFlag.clear();
    _modeFlag.push_back(74110); //R-value generator tag
    _modeFlag.push_back(0);     //to contain the mode selected
  }else if(mode==-1){
    _ndaugs = nson;
    for(int i=0;i<nson;i++){ daugs[i]=son[i];  }
    std::cout<<"The paraent particle: "<<EvtPDL::name(pid)<<std::endl;
  }else if(mode==-2){
    _ndaugs = nson;
    for(int i=0;i<nson;i++){  daugs[i]=son[i];  }
  }else if(mode==-100){
    _ndaugs =1;
    daugs[0]=EvtPDL::getId(std::string("gamma*"));
    _modeFlag.clear();
    _modeFlag.push_back(-100); //R-value generator tag
    _modeFlag.push_back(0);     //to contain the mode selected

  }else if(mode==10000){//use for check ISR
    _ndaugs =2;
    daugs[0]=EvtPDL::getId(std::string("pi+"));
    daugs[1]=EvtPDL::getId(std::string("pi-"));
  }else {
    std::cout<<"Bad mode_index number " <<mode<<", please refer to the manual."<<std::endl;
    ::abort();
  }
  // std::cout<<_ndaugs<<", "<<daugs[0]<<daugs[1]<<std::endl;

  if(VISR){
    _ndaugs=2;
    daugs[0]=getDaugs()[0];//EvtPDL::getId(std::string("gamma"));
    daugs[1]=getDaugs()[1];//EvtPDL::getId(std::string("gamma*"));
  }

 std::vector<EvtId> theDaugs; 
 for(int i=0;i<_ndaugs;i++){
    theDaugs.push_back(daugs[i]);
  }
 if(theDaugs.size()) {return theDaugs;} else {std::cout<<"EvtConExc::get_mode: zero size"<<std::endl;abort();}
}

 
void EvtConExc::initProbMax(){

  noProbMax();

}

void EvtConExc::decay( EvtParticle *p){
  //std::cout<<"_cms= "<<_cms<<" mode= "<<_mode<<std::endl;
  EvtId  myvpho = EvtPDL::getId(std::string("vpho"));
  if(myvpho != p->getId()){
    std::cout<<"Parent needs to be vpho, but found "<<EvtPDL::name(p->getId())<<std::endl;
    abort();
  }
  //-- to read _cms from database or to consider beam energy spread
  double mvpho=EvtPDL::getMeanMass(myvpho);
  bool beamflag=0;
  if(mvpho!=EvtConExc::_cms){ //if read cms energy from database
    EvtConExc::_cms=mvpho;
    beamflag=1;
  }

  /*
  if(beamEnergySpread!=0){
  cmsspread=sqrt(2.)*beamEnergySpread;
  _cms = energySpread(_cms,cmsspread);
  beamflag=1;
  std::cout<<"test_cms "<<_cms<<std::endl;
  }
  */

  if(beamflag) calAF(_cms);


  //debugging
  //double mvpho=EvtPDL::getMeanMass(myvpho);
  //std::cout<<"myvpho= "<<mvpho<<std::endl;

  //for fill root tree
  EvtVector4R vgam,hd1,hd2,vhds;

  //first for Rscan generator with _mode=74110
  if(_mode==74110){
    //preparation of mode sampling
    std::vector<int> vmod; vmod.clear();
    int mn[83]={0,1,2,3,4,5,6,7,8,9,10,11,13,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,32,33,34,35,36,37,38,40,41,44,45,46, // 12,14, 30,31,39,42 and 43 is removed
		50,51,
		59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,
		90,91,92,93,94,95,96,
		74100,74101,74102,74103,74104,74105,74110};
    int mn2[84]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,32,33,34,35,36,37,38,40,41,44,45,46,//mode 14,30,31,42, 43 are removed
		 50,51,
		 59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,
		 90,91,92,93,94,95,96,
		 74100,74101,74102,74103,74104,74105,74110};
    int mn3[77]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,44,45,46,
		 50,51,
		 68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,
		 90,91,92,93,94,95,96,
		 74100,74101,74102,74110}; //remove 43, 74103,74104,74105, this is included in PHOKHARA
    double mx = p->getP4().mass();
  
    if(_cms>2.5){
      for(int i=0;i<83;i++){vmod.push_back(mn[i]);}
    }else{
      for(int i=0;i<84;i++){vmod.push_back(mn2[i]);}  
    }
    
    //for(int i=0;i<76;i++){vmod.push_back(mn3[i]);}

    int mymode;
    double pm= EvtRandom::Flat(0.,1);

    //std::cout<<"AF[598], AF[598] "<<AF[598]<<" "<<AF[599]<<std::endl;
    //--
    //-- where AF[599]: cross section with full region (0-Egamma^max GeV), AF[598]: cross section within Egam=(0.025-Egamma^max GeV)
    if(pm <_xs0/(_xs0 + _xs1)){//without ISR photon for mode=74110

      mhds=_cms;
      mymode=selectMode(vmod,mhds);
      _selectedMode = mymode;
      std::cout<<"A selected mode is "<<mymode<<" with Mhds= "<<mhds<<std::endl; //debugging
      delete myxsection;
      myxsection =new EvtXsection (mymode);     
      init_mode(mymode);                        
      resetResMass();                           
                                                
      if(_ndaugs>1){//for e+e- -> exclusive decays
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
	if(!checkdecay(p)) goto checkA;
	vhds = p->getP4();
	if(_cms>2.5 && !angularSampling(p)) goto checkA;
	if(_cms<2.5 && !photonSampling(p)) goto checkA;
      }else{// for e+e- -> vector resonace, add a very soft photon
	EvtId mydaugs[2];
	mydaugs[0]=daugs[0];
	EvtPDL::reSetMass(mydaugs[0],mhds-0.001);
	//EvtPDL::reSetWidth(mydaugs[0],0);
	mydaugs[1]=gamId;    //ISR photon
	p->makeDaughters(2,mydaugs); 
      checkB:
	double totMass=0;
	for(int i=0;i< 2;i++){
	  EvtParticle* di=p->getDaug(i);
	  double xmi=EvtPDL::getMass(di->getId());
	  di->setMass(xmi);
	  totMass+=xmi;
	}
	//std::cout<<"checkB "<<totMass<<" p->mass() "<<p->mass()<<" "<<checkdecay(p)<<std::endl;
	if(totMass>p->mass()) goto checkB;
	p->initializePhaseSpace(2,mydaugs);

	if(!checkdecay(p)) goto checkB; 
	vhds = p->getDaug(0)->getP4();;
	vgam = p->getDaug(1)->getP4();
      }
 //-----
    }else{// with ISR photon for mode=74110
Sampling_mhds:
      mhds=Mhad_sampling(MH,AF);
      //std::cout<<"SetMthr= "<<SetMthr<<std::endl;
      if(mhds<SetMthr) goto Sampling_mhds;
      double xeng=1-mhds*mhds/(_cms*_cms); //photon energy ratio
      double theta=ISR_ang_sampling(xeng); //ISR photon polar angle in rad unit
      
      if(mydbg) mass2=mhds;
      
      //generating events
    ModeSelection:
      EvtGlobalSet::ConExcPythia=1;
      mymode=selectMode(vmod,mhds);
      if(mymode==-10) goto Sampling_mhds;
      conexcmode = mymode; //save for model REXC ( see EvtRexc.cc) decay 
      if(mhds<2.3 && mymode==74110) {goto ModeSelection;}  // E<2.3 GeV, fully exclusive production 
      std::cout<<"A selected mode is "<<mymode<<" with Mhds= "<<mhds<<std::endl; //debugging
      _selectedMode = mymode;
      delete myxsection;
      myxsection =new EvtXsection (mymode);
      init_mode(mymode);
      EvtId  myvpho = EvtPDL::getId(std::string("vgam"));
      EvtPDL::reSetMass(myvpho,mhds); //set to continum cms energy
      EvtPDL::reSetWidth(myvpho,0);
      
      //debugging
      //for(int i=0;i<_ndaugs+1;i++){std::cout<<_ndaugs<<" "<<EvtPDL::name(daugs[i])<<std::endl;}
      
      //decay e+e- ->gamma_ISR gamma*
      EvtId mydaugs[2];
      if(_ndaugs>1) { //gamma* -> exclusive decays 
	resetResMass(); 
	mydaugs[0]=myvpho;
      }else{// vector resonance decays
	resetResMass(); 
	mydaugs[0]=daugs[0];
	EvtPDL::reSetMass(mydaugs[0],mhds);
	//EvtPDL::reSetWidth(mydaugs[0],0);
      }
      mydaugs[1]=gamId;    //ISR photon
      int maxflag=0;
      int trycount=0;
    ISRphotonAng_sampling:
      double totMass=0;
      p->makeDaughters(2,mydaugs); 
      for(int i=0;i< 2;i++){
	EvtParticle* di=p->getDaug(i);
	double xmi=EvtPDL::getMass(di->getId());
	di->setMass(xmi);
	totMass+=xmi;
      }
      if(totMass>p->mass()) goto ISRphotonAng_sampling;
      //std::cout<<"Ndaugs= "<<_ndaugs<<" Mhds= "<<mhds<<std::endl;
      double weight1 = p->initializePhaseSpace(2,mydaugs);
      if(!checkdecay(p)) goto ISRphotonAng_sampling;
      //set the ISR photon angular distribution
      SetP4Rvalue(p,mhds,xeng,theta);  //end of decay e+e- -> vpho gamma_ISR

      if(maxflag==0) {findMaxXS(mhds); maxflag=1;}
      vhds = p->getDaug(0)->getP4();
      vgam=p->getDaug(1)->getP4();
      double gx=vgam.get(1);
      double gy=vgam.get(2);
      double sintheta= sqrt(gx*gx+gy*gy)/vgam.d3mag();
      bool gam_ang=gam_sampling(mhds,sintheta);
      trycount++;

    } // with and without ISR sampling block       
    // finish event generation
    // for debugging
    if(mydbg){
      pgam[0]=vgam.get(0);
      pgam[1]=vgam.get(1);
      pgam[2]=vgam.get(2);
      pgam[3]=vgam.get(3);   
       
      phds[0]=vhds.get(0);
      phds[1]=vhds.get(1);    
      phds[2]=vhds.get(2);
      phds[3]=vhds.get(3);
      costheta = vgam.get(3)/vgam.d3mag();
      selectmode = mymode;
      xs->Fill();
      //std::cout<<"mass1= "<<mass1<<" mass2= "<<mass2<<std::endl;
    }
    _modeFlag[1]= _selectedMode;
    p->setIntFlag(_modeFlag);
    return;
  }//end block  if(_mode==74110)

  // ======================================================
  // multi-exclusive mode 
  //=======================================================
  if(_mode==-100){
    int mymode;
    double pm= EvtRandom::Flat(0.,1);

    //std::cout<<"AF[598], AF[598] "<<AF[598]<<" "<<AF[599]<<std::endl;
    //--
    //-- where AF[599]: cross section with full region (0-Egamma^max GeV), AF[598]: cross section within Egam=(0.025-Egamma^max GeV)
    if(pm <_xs0/(_xs0 + _xs1)){//without ISR photon for mode=74110

      mhds=_cms;
      mymode=selectMode(vmd,mhds);
      _selectedMode = mymode;
      std::cout<<"A selected mode is "<<mymode<<" with Mhds= "<<mhds<<std::endl; //debugging
      delete myxsection;                        
      myxsection =new EvtXsection (mymode);     
      init_mode(mymode);                        
      resetResMass();                           
                   
      if(_ndaugs>1){//for e+e- -> exclusive decays
      checkAA:
	p->makeDaughters(_ndaugs,daugs); 
	double totMass=0;
	for(int i=0;i< _ndaugs;i++){            
	  EvtParticle* di=p->getDaug(i);
	  double xmi=EvtPDL::getMass(di->getId()); 
	  di->setMass(xmi);
	  totMass+=xmi;
	} 
	if(totMass>p->mass()) goto checkAA;
	p->initializePhaseSpace( _ndaugs , daugs);   
	if(!checkdecay(p)) goto checkAA;
	vhds = p->getP4();
	bool byn_ang;
	EvtVector4R pd1 = p->getDaug(0)->getP4();  
	EvtVector4R pcm(_cms,0,0,0);
	//p->printTree();
	if(_ndaugs==2){//angular distribution for the hadrons only for two-body decays
	  byn_ang=hadron_angle_sampling(pd1, pcm);
	  if(!byn_ang) goto checkAA;
	}

      }else{// for e+e- -> vector resonace, add a very soft photon
	EvtId mydaugs[2];
	mydaugs[0]=daugs[0];
	EvtPDL::reSetMass(mydaugs[0],mhds-0.001);
	//EvtPDL::reSetWidth(mydaugs[0],0);
	mydaugs[1]=gamId;    //ISR photon
	p->makeDaughters(2,mydaugs); 
      checkBA:
	double totMass=0;
	for(int i=0;i< 2;i++){
	  EvtParticle* di=p->getDaug(i);
	  double xmi=EvtPDL::getMass(di->getId());
	  di->setMass(xmi);
	  totMass+=xmi;
	}
	//std::cout<<"checkB "<<totMass<<" p->mass() "<<p->mass()<<" "<<checkdecay(p)<<std::endl;
	if(totMass>p->mass()) goto checkBA;
	p->initializePhaseSpace(2,mydaugs);

	if(!checkdecay(p)) goto checkBA; 
	vhds = p->getDaug(0)->getP4();;
	vgam = p->getDaug(1)->getP4();
      }
 //-----
    }else{// with ISR photon for mode=-100
Sampling_mhds_A:
      mhds=Mhad_sampling(MH,AF);
      //std::cout<<"SetMthr= "<<SetMthr<<std::endl;
      if(mhds<SetMthr) goto Sampling_mhds_A;
      double xeng=1-mhds*mhds/(_cms*_cms); //photon energy ratio
      double theta=ISR_ang_sampling(xeng); //ISR photon polar angle in rad unit
      
      if(mydbg) mass2=mhds;
      
      //generating events
    ModeSelection_A:
      mymode=selectMode(vmd,mhds);
      if(mymode==-10) goto Sampling_mhds_A;
      conexcmode = mymode; //save for model REXC ( see EvtRexc.cc) decay 
      std::cout<<"A selected mode is "<<mymode<<" with Mhds= "<<mhds<<std::endl; //debugging
      _selectedMode = mymode;
      delete myxsection;
      myxsection =new EvtXsection (mymode);
      init_mode(mymode);
      EvtId  myvpho = EvtPDL::getId(std::string("vgam"));
      EvtPDL::reSetMass(myvpho,mhds); //set to continum cms energy
      EvtPDL::reSetWidth(myvpho,0);
      
      //debugging
      // for(int i=0;i<_ndaugs+1;i++){std::cout<<_ndaugs<<" "<<EvtPDL::name(daugs[i])<<std::endl;}
      
      //decay e+e- ->gamma_ISR gamma*
      EvtId mydaugs[2];
      if(_ndaugs>1) { //gamma* -> exclusive decays 
	resetResMass(); 
	mydaugs[0]=myvpho;
      }else{// vector resonance decays
	resetResMass(); 
	mydaugs[0]=daugs[0];
	EvtPDL::reSetMass(mydaugs[0],mhds);
	//EvtPDL::reSetWidth(mydaugs[0],0);
      }
      mydaugs[1]=gamId;    //ISR photon
      int maxflag=0;
      int trycount=0;
    ISRphotonAng_sampling_A:
      double totMass=0;
      p->makeDaughters(2,mydaugs); 
      for(int i=0;i< 2;i++){
	EvtParticle* di=p->getDaug(i);
	double xmi=EvtPDL::getMass(di->getId());
	di->setMass(xmi);
	totMass+=xmi;
      }
      if(totMass>p->mass()) goto ISRphotonAng_sampling_A;
      //std::cout<<"Ndaugs= "<<_ndaugs<<" Mhds= "<<mhds<<std::endl;
      double weight1 = p->initializePhaseSpace(2,mydaugs);
      if(!checkdecay(p)) goto ISRphotonAng_sampling_A;
      //set the ISR photon angular distribution
      SetP4Rvalue(p,mhds,xeng,theta);  //end of decay e+e- -> vpho gamma_ISR

      //--debugging
      //p->printTree();

      if(maxflag==0) {findMaxXS(mhds); maxflag=1;}
      vhds = p->getDaug(0)->getP4();
      vgam=p->getDaug(1)->getP4();
      double gx=vgam.get(1);
      double gy=vgam.get(2);
      double sintheta= sqrt(gx*gx+gy*gy)/vgam.d3mag();
      bool gam_ang=gam_sampling(mhds,sintheta);
      trycount++;

    } // with and without ISR sampling block 
    _modeFlag[0]=-100;
    _modeFlag[1]= _selectedMode;
    p->setIntFlag(_modeFlag);      
    // finish event generation
    // for debugging
    if(mydbg){
      pgam[0]=vgam.get(0);
      pgam[1]=vgam.get(1);
      pgam[2]=vgam.get(2);
      pgam[3]=vgam.get(3);   
       
      phds[0]=vhds.get(0);
      phds[1]=vhds.get(1);    
      phds[2]=vhds.get(2);
      phds[3]=vhds.get(3);
      costheta = vgam.get(3)/vgam.d3mag();
      selectmode = mymode;
      xs->Fill();
      //std::cout<<"mass1= "<<mass1<<" mass2= "<<mass2<<std::endl;
    }
    return;
  }//end block  if(_mode==-100)

  //=======================================================
  // e+e- -> gamma_ISR gamma*
  //=======================================================
  if(VISR){
     EvtId gid=EvtPDL::getId("gamma*"); 
     double pm= EvtRandom::Flat(0.,1);

     if(pm <_xs0/(_xs0 + _xs1)){//with a very soft ISR photon
       EvtPDL::reSetMass(gid,(_cms-0.0001) );
       mhds = _cms-0.0001;
     }else{
       mhds=Mhad_sampling(MH,AF);
       EvtPDL::reSetMass(gid,mhds);
     }
     //debugging
     std::cout<<"generatedMass "<<EvtPDL::getMeanMass(gid)<<std::endl;
     double xeng=1-mhds*mhds/(_cms*_cms); //photon energy ratio
     double theta=ISR_ang_sampling(xeng); //ISR photon polar angle in rad unit
     p->makeDaughters(2,daugs);
     for(int i=0;i< 2;i++){
       EvtParticle* di=p->getDaug(i);
       //if(EvtPDL::name(di->getId())=="gamma*") di->setVectorSpinDensity(); 
       double xmi=EvtPDL::getMeanMass(di->getId());
       di->setMass(xmi);
     }
     p->initializePhaseSpace(2,daugs);
     SetP4(p,mhds,xeng,theta);
    return;
  }


  //========================================================
  //=== for other mode generation with _mode != 74110
  //========================================================

  if((_xs0 + _xs1)==0) {std::cout<<"EvtConExc::zero total xsection"<<std::endl;::abort();}
  double pm= EvtRandom::Flat(0.,1);
  double xsratio = _xs0/(_xs0 + _xs1);
  int iflag=2; //flag = 0 for ee->hadrons, 1 for ee->gamma_ISR hadrons, 2: mix 0 and 1 case
  if(getArg(0)!= -2){// exclude DIY case
    if(getNArg()==3 && radflag==1){iflag=1;xsratio=0;} // only generating gamma hadrons mode
    else if(getNArg()==3 && radflag==0) {iflag=0;xsratio=1;}
  }
 
  // std::cout<<"xsratio= "<<xsratio<<std::endl;
 
  if(pm<xsratio){// no ISR photon
  masscheck:
    double summassx=0;
    p->makeDaughters(_ndaugs,daugs);
    for(int i=0;i< _ndaugs;i++){
      EvtParticle* di=p->getDaug(i);
      double xmi=EvtPDL::getMass(di->getId());
      di->setMass(xmi);
      summassx += xmi;
      //std::cout<<"PID and mass "<<di->getId()<<" "<<xmi<<std::endl;
    }    
    if(summassx>p->mass()) goto masscheck;
  angular_hadron:
    p->initializePhaseSpace(_ndaugs,daugs);
    bool byn_ang;
    EvtVector4R pd1 = p->getDaug(0)->getP4();  
    EvtVector4R pcm(_cms,0,0,0);
    if(_ndaugs==2){//angular distribution for the hadrons only for two-body decays
      byn_ang=hadron_angle_sampling(pd1, pcm);
      if(!byn_ang) goto angular_hadron;
    }
    
    //for histogram
    cosp = pd1.get(3)/pd1.d3mag();
    mhds = _cms;
    //std::cout<<"cosp, mhds "<<cosp<<" "<<mhds<<std::endl;
    //p->printTree();

  }else{// sampling m_hadrons and decay e+e- ->gamma gamma*
    double mhdr=Mhad_sampling(MH,AF);
    double xeng=1-mhdr*mhdr/(_cms*_cms); //photon energy ratio
    double theta=ISR_ang_sampling(xeng); //ISR photon polar angle in rad unit
    EvtId gid =EvtPDL::getId("vhdr");
    EvtPDL::reSetMass(gid,mhdr);
    int ndaugs =2;
    EvtId mydaugs[2];
    mydaugs[0] =EvtPDL::getId(std::string("gamma"));
    mydaugs[1]=EvtPDL::getId(std::string("vhdr"));

    //for histogram
    mhds = mhdr;
    costheta = cos(theta);
    //-- 

    p->makeDaughters(2,mydaugs);
    for(int i=0;i< 2;i++){
      EvtParticle* di=p->getDaug(i);
      double xmi=EvtPDL::getMass(di->getId());
      //if(EvtPDL::name(di->getId())=="vhdr") di->setVectorSpinDensity(); 
      di->setMass(xmi);
    }
    p->initializePhaseSpace(2,mydaugs);
    SetP4(p,mhdr,xeng,theta);  //end of decay e+e- -> gamma_ISR gamma*
    //p->printTree();

 //----
  }//end of gamma gamma*, gamma*->hadrons generation
  // p->printTree();

  //-----------------------------------------
  // End of decays for all topology
  //----------------------------------------
  //================================= event analysis 
    if(_nevt ==0){
      std::cout<<"The decay chain: "<<std::endl;
      p->printTree();
    }
    _nevt++;
    //--- for debuggint to make root file
  if(mydbg){
    xs->Fill();
  }
  
  //----
  return ;
}

bool EvtConExc::hadron_angle_sampling(EvtVector4R ppi, EvtVector4R pcm){
  EvtVector4R pbst=-1*pcm;
  pbst.set(0,pcm.get(0));
  EvtVector4R p4i = boostTo(ppi,pbst);
  if((_mode>=0 && _mode<=5) || _mode==44 || _mode==96){//ee->two baryon mode, VP, SP, mode=-2 is excluded
    bool byn_ang = baryon_sampling(pcm, p4i);
    return byn_ang;
  }else if(_mode==6 || _mode==45 || _mode==46 || _mode==70 || _mode==71|| _mode==93){// ee->PP (pi+pi-,..) mode
    bool msn_ang = meson_sampling(pcm,p4i);
    return msn_ang;
  }else if(_mode==23 || _mode==24 || _mode==25 || _mode==26 || _mode==27 || _mode==36 || _mode==39 || _mode==47 || _mode==48 || _mode==52 || _mode==68 || _mode==69 || _mode==72 || _mode==73 || _mode==80 || _mode==94 || _mode==95){
    bool msn_ang = VP_sampling(pcm,p4i);
    return msn_ang;
  }else if(_mode==-2){
    EvtSpinType::spintype type0 = EvtPDL::getSpinType(daugs[0]);
    EvtSpinType::spintype type1 = EvtPDL::getSpinType(daugs[1]);
    if(type0==EvtSpinType::SCALAR &&type1==EvtSpinType::SCALAR){
    bool msn_ang = meson_sampling(pcm,p4i);
    return msn_ang;
    }else if(type0==EvtSpinType::VECTOR &&type1==EvtSpinType::SCALAR || type1==EvtSpinType::VECTOR &&type0==EvtSpinType::SCALAR){
    bool msn_ang = VP_sampling(pcm,p4i);
    return msn_ang;
    }
  }
  return true;
}


double EvtConExc::gamHXSection(EvtParticle *p,double El,double Eh,int nmc){//El, Eh : the lower and higher energy of radiative photons
  //std::cout<<"nmc= "<<nmc<<std::endl;
  gamId = EvtPDL::getId(std::string("gamma"));
  EvtId xvp = EvtPDL::getId(std::string("xvpho"));
  EvtVector4R p_init( p->getP4().mass(),0.,0.,0.);
  double totxs = 0;
  maxXS=-1;
  _er1=0;
  Rad2Xs =0;
  for(int ii=0;ii<nmc;ii++){// integarted the gamma hadrons xsection
  gamH=EvtParticleFactory::particleFactory(xvp, p_init);
  int gamdaugs = _ndaugs+1;

  EvtId theDaugs[10];
  for(int i=0;i<_ndaugs;i++){
    theDaugs[i] = daugs[i];
  }
  theDaugs[_ndaugs]=gamId;

  gamH->makeDaughters(gamdaugs,theDaugs); 

  for(int i=0;i<gamdaugs;i++){
    EvtParticle* di=gamH->getDaug(i);
    double xmi=EvtPDL::getMass(di->getId());
    di->setMass(xmi);
   }
  loop:
  double weight = gamH->initializePhaseSpace(  gamdaugs , theDaugs);
  //-- slection:theta_gamma > 1 degree
  EvtVector4R pgam = gamH -> getDaug(_ndaugs)->getP4();
  double pmag = pgam.d3mag();
  double pz = pgam.get(3);
  double sintheta = sqrt( pmag*pmag - pz*pz)/pmag;
  double onedegree = 1./180.*3.1415926;
  //if(sintheta < onedegree) {goto loop;}
  if(pmag <El || pmag >Eh) {goto loop;}
  //--------
  // std::cout<<"pmag= "<<pmag<<std::endl;
  
  double thexs = difgamXs(gamH); //prepare the photon angular distribution
  Rad2Xs += Rad2difXs( gamH );
  if(thexs>maxXS) {maxXS=thexs;}
  double s = p_init.mass2();
  double x = 2*pgam.get(0)/sqrt(s);
  double rad1xs = Rad1difXs(gamH); //first order xs by Eq(4)hep-ph/9910523
  totxs += rad1xs;
  _er1 += differ;
  gamH->deleteDaughters();
  }  //for int_i block
  _er1/=nmc;

  Rad2Xs/=nmc; // the leading second order correction 
  totxs/=nmc;  // first order correction XS

  // return totxs;  // first order correction XS
   return Rad2Xs;  // the leading second order correction 
}


double EvtConExc::gamHXSection(double s, double El,double Eh,int nmc){//El, Eh : the lower and higher energy of radiative photons
  //--for narrow resonance psi(2S),J/psi, phi, which can not calculated with the MC integration
  //double mxL = sqrt( s-2*Eh*sqrt(s) ); //low mass
  //double mxH = sqrt( s-2*El*sqrt(s) ); //high mass
  //double sigv = narrowRXS(mxL,mxH);
  //--

  double totxs = 0; 
  maxXS=-1;
  _er1=0;
  Rad2Xs =0;
  double xL=2*El/sqrt(s);
  double xH=2*Eh/sqrt(s);
  for(int i=0;i<nmc;i++){//It is found that the narrow resonance can not included in this MC integartion
    double rdm = EvtRandom::Flat(0.,1.);// set angular cut 1^o
    double x= xL+ (xH-xL)*rdm;
    Rad2Xs += Rad2difXs(s,x);
    _er1 += differ2; //stored when calculate Rad2Xs
    //  std::cout<<"i= "<<i<<" Rad2Xs= "<<Rad2Xs<<std::endl;
  }
  _er1/=nmc;
  _er1*=(xH-xL);
  //std::cout<<"_er1= "<<_er1<<std::endl;
  Rad2Xs/=nmc; // the leading second order correction 
  double thexs= Rad2Xs*(xH-xL);  //xH-xL is the Jacobi factor 
  //std::cout<<"thexs= "<<thexs<<std::endl;
  //if(sigv != -1) thexs += sigv; //add narrow resonance ISR cross section
  return thexs;
}
 


double EvtConExc::gamHXSection(double El,double Eh){// using Gaussian integration subroutine from Cern lib
  std::cout<<" "<<std::endl;
  extern double Rad2difXs(double *x);
  extern double Rad2difXs2(double *x);
  double eps = 0.01;
  double Rad2Xs;
  if(_mode!=-2) {Rad2Xs= dgauss_(&Rad2difXs,&El,&Eh,&eps);}else{Rad2Xs = dgauss_(&Rad2difXs2,&El,&Eh,&eps);}
  if(Rad2Xs==0){
    for(int iter=0;iter<10;iter++){
      eps += 0.01;
      if(_mode!=-2) {Rad2Xs= dgauss_(&Rad2difXs,&El,&Eh,&eps);}else{Rad2Xs = dgauss_(&Rad2difXs2,&El,&Eh,&eps);}
      if(!Rad2Xs)  return Rad2Xs;
    }
  }
  return Rad2Xs;  // the leading second order correction 
}


double EvtConExc::gamHXSection(double El,double Eh, int mode){// using Gaussian integration subroutine from Cern lib
  std::cout<<" "<<std::endl;
  extern double Rad2difXs(double *x);
  extern double Rad2difXs2(double *x);
  double eps = 0.01;
  double Rad2Xs;
  if(mode!=-2) {Rad2Xs= dgauss_(&Rad2difXs,&El,&Eh,&eps);}else{Rad2Xs = dgauss_(&Rad2difXs2,&El,&Eh,&eps);}
  if(Rad2Xs==0){
    for(int iter=0;iter<10;iter++){
      eps += 0.01;
      if(mode!=-2) {Rad2Xs= dgauss_(&Rad2difXs,&El,&Eh,&eps);}else{Rad2Xs = dgauss_(&Rad2difXs2,&El,&Eh,&eps);}
      if(!Rad2Xs)  return Rad2Xs;
    }
  }
  return Rad2Xs;  // the leading second order correction 
}


double EvtConExc::gamHXSection_er(double El,double Eh){// using Gaussian integration subroutine from Cern lib
  std::cout<<"  "<<std::endl;
  extern double Rad2difXs_er(double *x);
  extern double Rad2difXs_er2(double *x);
  double eps = 0.01;
  double Rad2Xs;
  if(_mode !=-2){Rad2Xs = dgauss_(&Rad2difXs_er,&El,&Eh,&eps);}else{ Rad2Xs = dgauss_(&Rad2difXs_er2,&El,&Eh,&eps);}
  if(Rad2Xs==0){
    for(int iter=0;iter<10;iter++){
      eps += 0.01;
      if(_mode !=-2){Rad2Xs = dgauss_(&Rad2difXs_er,&El,&Eh,&eps);}else{ Rad2Xs = dgauss_(&Rad2difXs_er2,&El,&Eh,&eps);}
      if(!Rad2Xs) return Rad2Xs;;
    }
  }
  return Rad2Xs;  // the leading second order correction 
}


void  EvtConExc::findMaxXS(EvtParticle *p){
  //std::cout<<"nmc= "<<nmc<<std::endl;
  gamId = EvtPDL::getId(std::string("gamma"));
  EvtId xvp = EvtPDL::getId(std::string("xvpho"));
  EvtVector4R p_init( p->getP4().mass(),0.,0.,0.);
  double totxs = 0;
  maxXS=-1;
  _er1=0;
  Rad2Xs =0;
  int nmc =  50000;
  for(int ii=0;ii<nmc;ii++){// integarted the gamma hadrons xsection
    gamH=EvtParticleFactory::particleFactory(xvp, p_init);
    int gamdaugs = _ndaugs+1;
    
    EvtId theDaugs[10];
    for(int i=0;i<_ndaugs;i++){
      theDaugs[i] = daugs[i];
    }
    theDaugs[_ndaugs]=gamId;
    
    gamH->makeDaughters(gamdaugs,theDaugs); 
  loop:    
    double totm=0;
    for(int i=0;i<gamdaugs;i++){
      EvtParticle* di=gamH->getDaug(i);
      double xmi=EvtPDL::getMass(di->getId());
      di->setMass(xmi);
      totm += xmi;
    }
    
    //std::cout<<totm<<" "<<p_init.get(0)<<std::endl;
    if(totm >= p_init.get(0) ) goto loop;

    double weight = gamH->initializePhaseSpace(  gamdaugs , theDaugs);
    double thexs = difgamXs(gamH); //prepare the photon angular distribution
    EvtVector4R p4gam = gamH->getDaug(_ndaugs)->getP4();
    double costheta = p4gam.get(3)/p4gam.d3mag();
    double sintheta = sqrt(1-costheta*costheta);
    bool acut=(sintheta>0.04) && (sintheta<0.96); //set photon auglar cut 2^o
    if(thexs>maxXS && acut ) {maxXS=thexs;}
    //gamH->deleteTree();
  }
  
}

double EvtConExc::difgamXs(EvtParticle *p){// dsigma/dm/dcos\theta* (Eq(1)@PRD73,012005)
  // where p -> hdarons(0 ~ _ndaugs-1) + gamma
  EvtVector4R p0 = p->getDaug(0)->getP4();
  for(int i=1;i<_ndaugs;i++){
  p0 += p->getDaug(i)->getP4();
  }
  EvtVector4R pgam = p->getDaug(_ndaugs)->getP4();
  double mhs = p0.mass();
  double egam = pgam.get(0);
  double sin2theta =  pgam.get(3)/pgam.d3mag();
  sin2theta = 1-sin2theta*sin2theta;

  double cms = p->getP4().mass();
  double alpha = 1./137.;
  double pi = 3.1415926;
  double x = 2*egam/cms;
  double wsx = alpha/pi/x*( (2-2*x+x*x)/sin2theta-x*x/2. );
  //  std::cout<<"pgam"<<pgam<<" mhs,egam,sin2theta,cms,x,wsx"<<  mhs<<", "<<egam<<", "<<sin2theta<<", "<<cms<<", "<<x<<", "<<wsx<<std::endl;

  double xs = myxsection->getXsection(mhs);
  double difxs = 2*mhs/cms/cms * wsx *xs; 
  differ = 2*mhs/cms/cms * wsx *(myxsection->getErr(mhs));
  return difxs;
  
}


bool EvtConExc::gam_sampling(EvtParticle *p){//photon angular distribution sampling
  double pm= EvtRandom::Flat(0.,1);
  double xs = difgamXs( p );
  double xsratio = xs/maxXS;
  if(pm<xsratio){return true;}else{return false;}
}

bool EvtConExc::gam_sampling(double mhds,double sintheta){
  double pm= EvtRandom::Flat(0.,1);
  double xs = difgamXs( mhds,sintheta );
  double xsratio = xs/maxXS;
  if(pm<xsratio){return true;}else{return false;}
}

bool EvtConExc::xs_sampling(double xs){
  double pm= EvtRandom::Flat(0.,1);
  //std::cout<<"Rdm= "<<pm<<std::endl;
  if(pm <xs/XS_max) {return true;} else {return false;}
}

bool EvtConExc::xs_sampling(double xs,double xs0){
  double pm= EvtRandom::Flat(0.,1);
  // std::cout<<"Rdm= "<<pm<<std::endl;
  if(pm <xs/(xs0*1.1)) {return true;} else {return false;}
}

bool EvtConExc::baryon_sampling(EvtVector4R pcm, EvtVector4R pi){
  EvtHelSys angles(pcm,pi);   //using helicity sys.angles
  double theta=angles.getHelAng(1);
  double phi  =angles.getHelAng(2);
  double costheta=cos(theta);  //using helicity angles in parent system

  // double costh = pcm.dot(pi)/pcm.d3mag()/pi.d3mag();
  //std::cout<<"costheta: "<<costheta<<", "<<costh<<std::endl;
  double alpha = baryonAng(_cms);
  if(_mode ==96){alpha=-0.34;}
  double pm= EvtRandom::Flat(0.,1);
  double ang = 1 + alpha*costheta*costheta;
  double myang;
  if(alpha>=0){myang=1+alpha;}else{myang=1;}
  if(pm< ang/myang) {return true;}else{return false;}
}

bool EvtConExc::meson_sampling(EvtVector4R pcm, EvtVector4R pi){
  EvtHelSys angles(pcm,pi);   //using helicity sys.angles
  double theta=angles.getHelAng(1);
  double phi  =angles.getHelAng(2);
  double costheta=cos(theta);  //using helicity angles in parent system

  double pm= EvtRandom::Flat(0.,1);
  double ang = 1 - costheta*costheta;
  if(pm< ang/1.) {return true;}else{return false;}
}

bool EvtConExc::VP_sampling(EvtVector4R pcm, EvtVector4R pi){
  EvtHelSys angles(pcm,pi);   //using helicity sys.angles
  double theta=angles.getHelAng(1);
  double phi  =angles.getHelAng(2);
  double costheta=cos(theta);  //using helicity angles in parent system

  double pm= EvtRandom::Flat(0.,1);
  double ang = 1 + costheta*costheta;
  if(pm< ang/2.) {return true;}else{return false;}
}

double EvtConExc::Rad1(double s, double x){ //first order correction
  //radiator correction upto second order, see Int. J. of Moder.Phys. A
  // Spectroscopy at B-factory using hard emision photon, by M. Benayoun, et al
  double alpha = 1./137.;
  double pi=3.1415926;
  double me = 0.5*0.001; //e mass
  double sme = sqrt(s)/me;
  double L = 2*log(sme);
  double wsx = 2*alpha/pi/x*(L-1)*(1 - x + x*x/2 );
  return wsx;
}

double EvtConExc::Rad2(double s, double x){
  //radiator correction upto second order, see Int. J. of Moder.Phys. A, hep-ph/9910523, Eq(8)
  // Spectroscopy at B-factory using hard emision photon, by M. Benayoun, et al
  double alpha = 1./137.;
  double pi=3.1415926;
  double me = 0.5*0.001; //e mass
  double xi2 = 1.64493407;
  double xi3=1.2020569;
  double sme = sqrt(s)/me;
  double L = 2*log(sme);
  double beta = 2*alpha/pi*(L-1);
  double delta2 = (9./8.-2*xi2)*L*L - (45./16.-5.5*xi2-3*xi3)*L-6./5.*xi2*xi2-4.5*xi3-6*xi2*log(2.)+3./8.*xi2+57./12.;
  double ap = alpha/pi;
  double Delta = 1 + ap *(1.5*L + 1./3.*pi*pi-2) + ap*ap *delta2;
  double wsx = Delta * beta * pow(x,beta-1)-0.5*beta*(2-x);
  double wsx2 = (2-x)*( 3*log(1-x)-4*log(x) ) -4*log(1-x)/x-6+x;
  wsx = wsx + beta*beta/8. *wsx2;
  double mymx = sqrt(s*(1-x));
  //  return wsx*vph; //getVP(mymx);//vph is vaccum polarization factor
  return wsx*getVP(mymx);//vph is vaccum polarization factor
}



double EvtConExc::Rad2difXs(EvtParticle *p){// leading second order xsection
  // where p -> hdarons(0 ~ _ndaugs-1) + gamma
  double summass = p->getDaug(0)->getP4().mass();
  EvtVector4R v4p=p->getDaug(0)->getP4();
  for(int i=1;i<_ndaugs;i++){
    summass += p->getDaug(i)->getP4().mass();
    v4p +=  p->getDaug(i)->getP4();
  }

  double Egam = p->getDaug(_ndaugs)->getP4().d3mag();
  double cms = p->getP4().mass();
  double mrg = cms - summass;
  double pm= EvtRandom::Flat(0.,1);
  double mhs = pm*mrg + summass;

  double s = cms * cms;
  double x = 2*Egam/cms;
  //double mhs = sqrt( s*(1-x) );
  double wsx = Rad2(s,x);

  //  std::cout<<"random : "<<pm<<std::endl;

  double xs = myxsection->getXsection(mhs);
  if(xs>XS_max){XS_max = xs;}
  double difxs = 2*mhs/cms/cms * wsx *xs;
  differ2 = 2*mhs/cms/cms * wsx *(myxsection->getErr(mhs));
  differ *= mrg; //Jacobi factor for variable m_hds
  difxs *= mrg;
  return difxs;
}

double EvtConExc::Rad2difXs(double s, double x){// leading second order xsection
  double wsx = Rad2(s,x);
  double mhs = sqrt(s*(1-x));
  double xs = myxsection->getXsection(mhs);
  //if(testflag==1) std::cout<<"s= "<<s<<" mhs= "<<mhs<<" x= "<<x<<" xs= "<<xs<<std::endl;
  if(xs>XS_max){XS_max = xs;}
  double difxs =  wsx *xs;
  differ2 =  wsx *(myxsection->getErr(mhs));
  return difxs;
}

double EvtConExc::Rad2difXs(double s, double x, EvtXsection* myxsection){// leading second order xsection
  double wsx = Rad2(s,x);
  double mhs = sqrt(s*(1-x));
  double xs = myxsection->getXsection(mhs);
  //if(testflag==1) std::cout<<"s= "<<s<<" mhs= "<<mhs<<" x= "<<x<<" xs= "<<xs<<std::endl;
  if(xs>XS_max){XS_max = xs;}
  double difxs =  wsx *xs;
  differ2 =  wsx *(myxsection->getErr(mhs));
  return difxs;
}


double EvtConExc::Rad1difXs(EvtParticle *p){// // first order xsection
  // where p -> hdarons(0 ~ _ndaugs-1) + gamma
  double summass  = p->getDaug(0)->getP4().mass();
  for(int i=1;i<_ndaugs;i++){
    summass += p->getDaug(i)->getP4().mass();
  }

  double cms = p->getP4().mass();
  double mrg = cms - summass;
  double pm= EvtRandom::Flat(0.,1);
  double mhs = pm*mrg + summass;

  double s = cms * cms;
  double x = 1 - mhs*mhs/s;
  double wsx = Rad1(s,x);

  //  std::cout<<"pgam"<<pgam<<" mhs,egam,sin2theta,cms,x,wsx"<<  mhs<<", "<<egam<<", "<<sin2theta<<", "<<cms<<", "<<x<<", "<<wsx<<std::endl;

  double xs = myxsection->getXsection(mhs);
  if(xs>XS_max){XS_max = xs;}
  double difxs = 2*mhs/cms/cms * wsx *xs;

  differ = 2*mhs/cms/cms * wsx *(myxsection->getErr(mhs));
  differ *= mrg; //Jacobi factor for variable m_hds
  difxs *= mrg;
  return difxs;
}

void EvtConExc::init_Br_ee(){
  //  double psipp_ee=9.6E-06;
  double psip_ee =7.73E-03;
  double jsi_ee  =5.94*0.01;
  double phi_ee  =2.954E-04;
  // double omega_ee=7.28E-05;
  // double rho_ee = 4.72E-05;
  EvtId psppId=EvtPDL::getId(std::string("psi(3770)"));
  EvtId pspId=EvtPDL::getId(std::string("psi(2S)"));
  EvtId jsiId=EvtPDL::getId(std::string("J/psi"));
  EvtId phiId=EvtPDL::getId(std::string("phi"));
  EvtId omegaId=EvtPDL::getId(std::string("omega"));
  EvtId rhoId=EvtPDL::getId(std::string("rho0"));
  BR_ee.clear();
  ResId.clear();

  //BR_ee.push_back(rho_ee);  
  //BR_ee.push_back(omega_ee);
  BR_ee.push_back(phi_ee);
  BR_ee.push_back(jsi_ee);
  BR_ee.push_back(psip_ee);
  //BR_ee.push_back(psipp_ee);

  //ResId.push_back(rhoId);
  //ResId.push_back(omegaId);
  ResId.push_back(phiId);
  ResId.push_back(jsiId);
  ResId.push_back(pspId);
  //ResId.push_back(psppId);
  
}

double EvtConExc::Ros_xs(double mx,double bree, EvtId pid){// in unit nb
  double pi=3.1415926;
  double s= mx*mx;
  double width = EvtPDL::getWidth(pid);
  double mass = EvtPDL::getMeanMass(pid);
  double xv = 1-mass*mass/s;
  double sigma = 12*pi*pi*bree*width/mass/s;
  sigma *= Rad2(s, xv);
  double nbar = 3.89379304*100000;  // ! GeV-2 = 3.89379304*10^5 nbar
   return sigma*nbar;
}


  double Rad2difXs(double *mhs){// leading second order xsection, mhs: the mass of final states
  double cms = EvtConExc::_cms;
  double s = cms * cms;
  double x = 1 - (*mhs)*(*mhs)/s;
  EvtConExc myconexc; 
  double wsx;
  double dhs=(*mhs);
  double xs = EvtConExc::myxsection->getXsection(dhs); 
  wsx=myconexc.Rad2(s,x);
  if(xs>EvtConExc::XS_max){EvtConExc::XS_max = xs;}
  double difxs = 2*dhs/cms/cms * wsx *xs; 
  std::ofstream oa;oa<<x<<std::endl; //without this line, the running will breaking, I don't know why
  return difxs;
}
double Rad2difXs_er(double *mhs){// leading second order xsection, mhs: the mass of final states
  double cms = EvtConExc::_cms;
  double s = cms * cms;
  double x = 1 - (*mhs)*(*mhs)/s;
  EvtConExc myconexc; 
  double wsx;
  double xs = EvtConExc::myxsection->getErr(*mhs);
  wsx=myconexc.Rad2(s,x);
  double differ2 = 2*(*mhs)/cms/cms * wsx *xs; 
  std::ofstream oa;oa<<x<<std::endl;
  return differ2;
}

double Rad2difXs2(double *mhs){// leading second order xsection, mhs: the mass of final states
  double cms = EvtConExc::_cms;
  double s = cms * cms;
  double x = 1 - (*mhs)*(*mhs)/s;
  EvtConExc myconexc; 
  double wsx;
  double dhs=(*mhs);
  double xs = EvtConExc::myxsection->getXsection(dhs);
  wsx=myconexc.Rad2(s,x);
  if(xs>EvtConExc::XS_max){EvtConExc::XS_max = xs;}
  double difxs = 2*dhs/cms/cms * wsx *xs; 
  oa<<x<<std::endl; //without this line, the running will breaking, I don't know why
  return difxs;
}

double Rad2difXs_er2(double *mhs){// leading second order xsection, mhs: the mass of final states
  double cms = EvtConExc::_cms;
  double s = cms * cms;
  double x = 1 - (*mhs)*(*mhs)/s;
  EvtConExc myconexc; 
  double wsx;
  double xs = EvtConExc::myxsection->getErr(*mhs);
  wsx=myconexc.Rad2(s,x);
  double differ2 = 2*(*mhs)/cms/cms * wsx *xs; 
  oa<<x<<std::endl;
  return differ2;
}


double EvtConExc::SoftPhoton_xs(double s, double b){
  double alpha = 1./137.;
  double pi=3.1415926;
  double me = 0.5*0.001; //e mass
  double xi2 = 1.64493407;
  double xi3=1.2020569;
  double sme = sqrt(s)/me;
  double L = 2*log(sme);
  double beta = 2*alpha/pi*(L-1);
  double delta2 = (9./8.-2*xi2)*L*L - (45./16.-5.5*xi2-3*xi3)*L-6./5.*xi2*xi2-4.5*xi3-6*xi2*log(2.)+3./8.*xi2+57./12.;
  double ap = alpha/pi;
  double Delta = 1 + ap *(1.5*L + 1./3.*pi*pi-2) + ap*ap *delta2;

  double beta2 = beta*beta;
  double b2 = b*b;

  double xs=(-32*b*beta + 8*pow(b,2)*beta - 10*b*pow(beta,2) + pow(b,2)*pow(beta,2)  + 32*pow(b,beta)*Delta - 
     6*(3 - 4*b + pow(b,2))*pow(beta,2)*log(1 - b) - 32*b*pow(beta,2)*log(b) + 8*pow(b,2)*pow(beta,2)*log(b) + 
	     16*pow(beta,2)*Li2(b))/32. ;
  double mymx = sqrt(s*(1-b));
  //return xs*vph; //getVP(mymx); //vph :the vaccum polarzation factor
  return xs*getVP(mymx); //vph :the vaccum polarzation factor

}

double EvtConExc::Li2(double x){
//  double li2= -x +x*x/4. - x*x*x/9; // wangwp: may be not correct!
  double li2= +x +x*x/4. + x*x*x/9; // corrected by wangwp
  return li2;
}


double EvtConExc::lgr(double *x,double *y,int n,double t)
{ int n0=-1;
  double z;
  for(int i=0;i<n-1;i++){
    if(x[i]>=t && t> x[i+1]) {n0=i;break;}
  }
  if(n0==-1) {return 0.0;}else{
    double k=(y[n0]-y[n0+1])/(x[n0]-x[n0+1]);
    z= y[n0+1]+k*(t-x[n0+1]);
  }
  //std::cout<<"t="<<t<<" "<<x[n0+1]<<" "<<x[n0]<<std::endl;
  //std::cout<<"t="<<t<<" "<<y[n0+1]<<" "<<y[n0]<<std::endl;
  return z;
}

bool EvtConExc::islgr(double *x,double *y,int n,double t)
{ int n0=-1;
  double z;
  for(int i=0;i<n-1;i++){
    if(x[i]>=t && t> x[i+1]) {n0=i;break;}
  }
  double xstotal=y[599];
  if(n0==-1) {return 0;}else{
    double p1=y[n0]/xstotal;
    double p2=y[n0+1]/xstotal;
    double pm= EvtRandom::Flat(0.,1);
    if(p1<pm && pm<=p2) {return 1;}else{return 0;}
  }
}

double EvtConExc::LLr(double *x,double *y,int n,double t)
{ int n0=-1;
  double z;
  if( t==x[n-1] ) return y[n-1];
  for(int i=0;i<n-1;i++){
    if(x[i]<=t && t< x[i+1]) {n0=i;break;}
  }
  if(n0==-1) {return 0.0;}else{
    double k=(y[n0]-y[n0+1])/(x[n0]-x[n0+1]);
    z= y[n0+1]+k*(t-x[n0+1]);
  }
  //std::cout<<"t="<<t<<" "<<x[n0+1]<<" "<<x[n0]<<std::endl;
  //std::cout<<"t="<<t<<" "<<y[n0+1]<<" "<<y[n0]<<std::endl;
  return z;
}

double EvtConExc::Mhad_sampling(double *x,double *y){//sample ISR hadrons, return Mhadron
  //x=MH: array contains the Mhad
  //y=AF: array contains the Mhad*Xsection
  //n: specify how many bins in the hadron sampling
  int n=598; //AF[599] is the total xs including Ecms point
  double pm= EvtRandom::Flat(0.,1);
  double xrt=1;
  int mybin=1;
  double xst=y[n]; 
  for(int i=0;i<n;i++){
    if((y[i]/xst)<pm && pm<=(y[i+1]/xst)){
      mybin=i+1;
      break;
    }
  }
  pm= EvtRandom::Flat(0.,1);
  if(pm>1){std::cout<<"random larger than 1: "<<pm<<std::endl;}
  double mhd=x[mybin-1]+(x[mybin]-x[mybin-1])*pm;

  if((mhd - _cms)>0){std::cout<<"selected mhd larger than Ecms: "<<mhd<<" > "<<x[mybin] <<std::endl;abort();}
  if(mhd<_mhdL){
    std::cout<<"the sample mhassrons is less than the low end of XS"<<mhd<<" <"<<_mhdL<<std::endl;
    for(int i=0;i<598;i++){std::cout<<i<<" "<<x[i]<<" "<<y[i]<<std::endl;}
    abort();
  }
  return mhd;
}

double EvtConExc::ISR_ang_integrate(double x,double theta){
  //see Eq(11) in arXif:hep-ph/9910523, return h(theta)/h(pi)
  double costheta=cos(theta);
  double eb=_cms/2;
  double cos2=costheta*costheta;
  double sin2=1-cos2;
  double me=0.51*0.001;
  double L=2*log(_cms/me);
  double meE2= me*me/eb/eb;
  double hpi=L-1;
  double hq1= meE2/2*costheta/(sin2+meE2*cos2);
  double hq2= -0.5*log((1+sqrt(1-meE2)*costheta)/(1-sqrt(1-meE2)*costheta));
  double hq3=x*x*costheta/2/(x*x-2*x+2)*(1-meE2/(sin2+meE2*cos2));
  double hq=(L-1)/2.+hq1+hq2+hq3;
  hq /= hpi;   //Eq (11) in  arXif:hep-ph/9910523
  return hq;
}

double EvtConExc::ISR_ang_sampling(double x){
  double xx[180],yy[180];
  double dgr2rad=1./180.*3.1415926;
  xx[0]=0;
  xx[1]=5*dgr2rad;  //first bin is 5 degree
  int nc=2;
  for(int i=6;i<=175;i++){ //last bin is 5 degree
    xx[nc]=i*dgr2rad;
    nc++;
  }
  xx[nc]=180*dgr2rad;
  for(int j=0;j<=nc;j++){
    yy[j]=ISR_ang_integrate(x,xx[j]);
  }
  double pm= EvtRandom::Flat(0.,1);
  int mypt=1;
  for(int j=1;j<=nc;j++){
    if(yy[j-1]<pm && pm<=yy[j]){mypt=j;break;}
  }
  pm= EvtRandom::Flat(0.,1);
  double mytheta= xx[mypt-1]+ (xx[mypt]-xx[mypt-1])*pm;
  return mytheta; //theta in rad unit
}

void EvtConExc::SetP4(EvtParticle *p, double mhdr, double xeng,double theta){ //set the gamma and gamma* momentum according sampled results
  double  pm= EvtRandom::Flat(0.,1);
  double phi=2*3.1415926*pm;
  double gamE = _cms/2*xeng;
  double hdrE = sqrt(mhdr*mhdr+gamE*gamE);
  double px = gamE*sin(theta)*cos(phi);
  double py = gamE*sin(theta)*sin(phi);
  double pz = gamE*cos(theta);
  EvtVector4R p4[2];
  p4[0].set(gamE,px,py,pz);//ISR photon
  p4[1].set(hdrE,-px,-py,-pz); //mhdraons
  
  for(int i=0;i<2;i++){
    EvtId myid = p->getDaug(i)->getId();
    p->getDaug(i)->init(myid,p4[i]);
    if(EvtPDL::name(myid)=="gamma*"){
     if( (_cms-mhdr)<0.002){
      EvtVector4R PG(_cms,0,0,0);
      p->getDaug(i)->init(myid,PG);
     }
    }
  }
}

void EvtConExc::SetP4Rvalue(EvtParticle *p, double mhdr, double xeng,double theta){ //set the gamma and gamma* momentum according sampled results
  double  pm= EvtRandom::Flat(0.,1);
  double phi=2*3.1415926*pm;
  double gamE = _cms/2*xeng;
  double hdrE = sqrt(mhdr*mhdr+gamE*gamE);
  double px = gamE*sin(theta)*cos(phi);
  double py = gamE*sin(theta)*sin(phi);
  double pz = gamE*cos(theta);
  EvtVector4R p4[2];
  p4[0].set(hdrE,px,py,pz);//mhdraons
  p4[1].set(gamE,-px,-py,-pz); //ISR photon
  for(int i=0;i<2;i++){
    EvtId myid = p->getDaug(i)->getId();
    p->getDaug(i)->init(myid,p4[i]);
  }
}


void EvtConExc::findMaxXS(double mhds){// dsigma/dm/dcos\theta* (Eq(1)@PRD73,012005)
  maxXS=-1;
  for(int i=0;i<90000;i++){
    double x =  1-(mhds/_cms)*(mhds/_cms);
    double cos = EvtRandom::Flat(0.0006,0.9994); //set cut on ISR gamma 2^o
    double sin2theta =sqrt(1-cos*cos);
    double alpha = 1./137.;
    double pi = 3.1415926;
    double wsx = alpha/pi/x*( (2-2*x+x*x)/sin2theta-x*x/2. );
    double xs = myxsection->getXsection(mhds);
    double difxs = 2*mhds/_cms/_cms * wsx *xs;
    if(difxs>maxXS)maxXS=difxs;
  }
}

double EvtConExc::difgamXs(double mhds,double sintheta){// dsigma/dm/dcos\theta* (Eq(1)@PRD73,012005)
  double x = 1-(mhds/_cms)*(mhds/_cms);
  double sin2theta = sintheta*sintheta;
  double alpha = 1./137.;
  double pi = 3.1415926;
  double wsx = alpha/pi/x*( (2-2*x+x*x)/sin2theta-x*x/2. );
  double xs = myxsection->getXsection(mhds);
  double difxs = 2*mhds/_cms/_cms * wsx *xs;  
  return difxs;
}

int EvtConExc::selectMode(std::vector<int> vmod, double mhds){
  //first get xsection for each mode in vmod array
  //std::cout<<"vmod.size, mhds "<<vmod.size()<<" "<<mhds<<std::endl;
  std::vector<int>excmod;
  excmod.push_back(0);
  excmod.push_back(1);
  excmod.push_back(2);
  excmod.push_back(6);
  excmod.push_back(7);
  excmod.push_back(12);
  excmod.push_back(13);
  excmod.push_back(45);
  excmod.push_back(46);
  double uscale = 1;  
  std::vector<double> vxs;vxs.clear();       
  for (int i=0;i<vmod.size();i++){
    int imod = vmod[i];
    delete myxsection;                       
    myxsection =new EvtXsection (imod);
    if(myxsection->getUnit() =="pb") {uscale =0.001;}else{uscale=1;}
    //if(uscale!=1) std::cout<<"mode="<<imod<<" uscale= "<<uscale<<std::endl;

    double myxs=uscale*myxsection->getXsection(mhds);   

    if(i==0) {vxs.push_back(myxs);} 
    else if(imod==74110){//for continuum process
      double xcon = myxs - vxs[i-1];
      if(xcon<0) {xcon=vxs[i-1];}else{xcon=myxs;}
      if(mhds<2.0) xcon=vxs[i-1]; //continuum cut: _cms energy less than 1.1, sampling phi,rho0 and omega resonance, see parp(2)=1.08 at Pythia.F
      vxs.push_back(xcon); 
    }else{
      vxs.push_back(myxs+vxs[i-1]);
    }
  }//for_i_block
  //degugging
  // for(int i=0;i<vxs.size();i++){std::cout<<"Mhds="<<mhds<<" Mode="<<vmod[i]<<" xs_i= "<<vxs[i]<<" totalxs "<< vxs[vxs.size()-1]<<std::endl;} 

  
  double totxs = vxs[vxs.size()-1];
  //if(totxs==0){std::cout<<"totalXs=0, vxs.size()= "<<vxs.size()<<std::endl;}
  int icount=0;
  int themode;
 mode_iter:
        double pm= EvtRandom::Flat(0.,1);  //std::cout<<"pm= "<<pm<<" mhds= "<<mhds<<std::endl;
         if(pm <= vxs[0]/totxs) {
           themode= vmod[0];
           std::vector<EvtId> theDaug=get_mode(themode);
           EvtId daugs[100];
           for(int i=0;i<theDaug.size();i++){daugs[i]=theDaug[i];}
           int exmode=EvtDecayTable::inChannelList(theparent->getId(),theDaug.size(),daugs);
           if(_mode!=74110){return themode;}
	   else if(exmode==-1){ return themode;}else{goto mycount;} 
          } 

          for(int j=1;j<vxs.size();j++){
            double x0 = vxs[j-1]/totxs;
            double x1 = vxs[j]/totxs;   //std::cout<<"j,x0,x1 "<<j<<" "<<x0<<" "<<x1<<std::endl;
            if(x0<pm && pm<=x1){
              themode=vmod[j];
              std::vector<EvtId> theDaug=get_mode(themode);
              EvtId daugs[100];
              for(int i=0;i<theDaug.size();i++){daugs[i]=theDaug[i];}
              int exmode=EvtDecayTable::inChannelList(theparent->getId(),theDaug.size(),daugs);
	      if(_mode!=74110){return themode;}
              else if(exmode==-1){ return themode;} else{ break;}
            }
          }
 
 mycount:
          icount++;
          if(icount<20000 ) goto mode_iter;
          //debugging
          //for(int i=0;i<vxs.size();i++){std::cout<<"Random="<<pm<<" Mode,Mhad "<<vmod[i]<<", "<<mhds<<" xs_i "<<vxs[i]<<" xsi/totalxs "<<vxs[i]/totxs<<std::endl;}
          std::cout<<"EvtConExc::selectMode can not find a mode with 20000 iteration for Mhadrons="<<mhds<<std::endl;
          return -10;
          //abort();

}


void EvtConExc::resetResMass(){
  // EvtGen base class resetwidth has bugs, it make the mass of particle changed.
  EvtId  myres = EvtPDL::getId(std::string("J/psi"));
  EvtPDL::reSetMass(myres,mjsi); 
  //EvtPDL::reSetWidth(myres,wjsi); 

  myres = EvtPDL::getId(std::string("psi(2S)"));
  EvtPDL::reSetMass(myres,mpsip); 
  //EvtPDL::reSetWidth(myres,wpsip); 

  myres = EvtPDL::getId(std::string("psi(3770)"));
  EvtPDL::reSetMass(myres,mpsipp); 
  //EvtPDL::reSetWidth(myres,wpsipp); 

  myres = EvtPDL::getId(std::string("phi"));
  EvtPDL::reSetMass(myres,mphi); 
  //EvtPDL::reSetWidth(myres,wphi); 

  myres = EvtPDL::getId(std::string("omega"));
  EvtPDL::reSetMass(myres,momega);
  //EvtPDL::reSetWidth(myres,womega);

  myres = EvtPDL::getId(std::string("rho0"));
  EvtPDL::reSetMass(myres,mrho0);
  //EvtPDL::reSetWidth(myres,wrho0);

  myres = EvtPDL::getId(std::string("rho(3S)0"));
  EvtPDL::reSetMass(myres,mrho3s); 
  //EvtPDL::reSetWidth(myres,wrho3s); 

  myres = EvtPDL::getId(std::string("omega(2S)"));
  EvtPDL::reSetMass(myres,momega2s);
  //EvtPDL::reSetWidth(myres,womega2s);
}

void EvtConExc::getResMass(){
  EvtId  myres = EvtPDL::getId(std::string("J/psi"));
  mjsi = EvtPDL::getMeanMass(myres); 
  wjsi = EvtPDL::getWidth(myres); 

  myres = EvtPDL::getId(std::string("psi(2S)"));
  mpsip= EvtPDL::getMeanMass(myres); 
  wpsip= EvtPDL::getWidth(myres); 

  myres = EvtPDL::getId(std::string("psi(3770)"));
  mpsipp= EvtPDL::getMeanMass(myres); 
  wpsipp= EvtPDL::getWidth(myres); 

  myres = EvtPDL::getId(std::string("phi"));
  mphi = EvtPDL::getMeanMass(myres); 
  wphi = EvtPDL::getWidth(myres); 

  myres = EvtPDL::getId(std::string("omega"));
  momega= EvtPDL::getMeanMass(myres);
  womega= EvtPDL::getWidth(myres);

  myres = EvtPDL::getId(std::string("rho0"));
  mrho0 = EvtPDL::getMeanMass(myres);
  wrho0 = EvtPDL::getWidth(myres);

  myres = EvtPDL::getId(std::string("rho(3S)0"));
  mrho3s= EvtPDL::getMeanMass(myres);
  wrho3s= EvtPDL::getWidth(myres);


  myres = EvtPDL::getId(std::string("omega(2S)"));
  momega2s= EvtPDL::getMeanMass(myres);
  womega2s= EvtPDL::getWidth(myres);

}

void EvtConExc::showResMass(){
  std::cout<<"J/psi: "<<mjsi<<" "<<wjsi<<std::endl;
  std::cout<<"psipr: "<<mpsip<<" "<<wpsip<<std::endl;
  std::cout<<"psipp: "<<mpsipp<<" "<<wpsipp<<std::endl;
  std::cout<<"phi  : "<<mphi<<" "<<wphi<<std::endl;
  std::cout<<"omega: "<<momega<<" "<<womega<<std::endl;
  std::cout<<"rho0 : "<<mrho0<<" "<<wrho0<<std::endl;
  std::cout<<"rho(3S)0: "<<mrho3s<<" "<<wrho3s<<std::endl;
  std::cout<<"omega(2S): "<<momega2s<<" "<<womega2s<<std::endl;
}

bool EvtConExc::checkdecay(EvtParticle* p){
  int nson=p->getNDaug();
  double massflag=1;
  for(int i=0;i<nson;i++){
    if( EvtPDL::getStdHep(p->getDaug(i)->getId())==22 ) continue;
    massflag *= p->getDaug(i)->mass();
  }
  if(massflag==0){
    std::cout<<"Zero mass!"<<std::endl;
    return 0;
  }else{return 1;}
}


double EvtConExc::sumExc(double mx){//summation all cross section of exclusive decays
  std::vector<int> vmod; vmod.clear();
  int mn[83]={0,1,2,3,4,5,6,7,8,9,10,11,13,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,32,33,34,35,36,37,38,40,41,44,45,46, // 12,14, 30,31,39,42 and 43 is removed
	      50,51,
	      59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,
	      90,91,92,93,94,95,96,
	      74100,74101,74102,74103,74104,74105,74110};
  int mn2[84]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,32,33,34,35,36,37,38,40,41,44,45,46,//mode 14,30,31,42, 43 are removed
	       50,51,
	       59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,
	       90,91,92,93,94,95,96,
	       74100,74101,74102,74103,74104,74105,74110};

  if(_cms > 2.5){
    for(int i=0;i<83;i++){vmod.push_back(mn[i]);}
  }else{
    for(int i=0;i<84;i++){vmod.push_back(mn2[i]);}  
  }
  
 // for(int i=0;i<79;i++){vmod.push_back(mn2[i]);}  

  double xsum=0;
  double uscale = 1;
  for(int i=0;i<vmod.size();i++){
    int mymode = vmod[i];
    if(mymode ==74110) continue;
    delete myxsection;
    myxsection =new EvtXsection (mymode);
    init_mode(mymode);
    if(myxsection->getUnit() =="pb") {uscale =0.001;}else{uscale=1;}
    xsum += uscale*myxsection->getXsection(mx);
    //if(mx==3.0967) {std::cout<<vmod[i]<<" "<<uscale<<" "<<xsum<<std::endl;}
  }
  return xsum;
}

bool EvtConExc::angularSampling(EvtParticle* par){
  bool tagp,tagk;
  tagk=0;
  tagp=0;
  int nds = par->getNDaug();
  for(int i=0;i<par->getNDaug();i++){
    EvtId di=par->getDaug(i)->getId();
    EvtVector4R p4i = par->getDaug(i)->getP4Lab();
    int pdgcode =EvtPDL::getStdHep( di );
    double alpha=1;
    /*
    if(fabs(pdgcode)==2212){//proton and anti-proton
      alpha = baryonAng(p4i.mass());
      cosp = cos(p4i.theta());
      tagp=1;
    }else if(fabs(pdgcode)==321||fabs(pdgcode)==211){
      alpha=1;
      cosk = cos(p4i.theta());
      tagk=1;
    }else continue;
    */
    double angmax = 1+alpha;
    double costheta = cos(p4i.theta());
    double ang=1+alpha*costheta*costheta;
    double xratio = ang/angmax;
    double xi=EvtRandom::Flat(0.,1);
    //std::cout<<"pdgcode "<<pdgcode<<std::endl;
    //std::cout<<ang<<" "<<angmax<<" "<<xi<<" "<<xratio<<std::endl;
    if(xi>xratio) return false;
  }//loop over duaghters
  //if(tagp) std::cout<<" cosp "<<cosp<<std::endl;
  //if(tagk) std::cout<<" cosk "<<cosk<<std::endl;

  return true;
}

double EvtConExc::baryonAng(double mx){
  double mp=0.938;
  double u = 0.938/mx;
  u = u*u;
  double u2 = (1+u)*(1+u);
  double uu = u*(1+6*u);
  double alpha = (u2-uu)/(u2+uu);
  return alpha;
}

bool EvtConExc::photonSampling(EvtParticle* par){
  bool tagp,tagk;
  tagk=0;
  tagp=0;
  int nds = par->getNDaug();
  for(int i=0;i<par->getNDaug();i++){
    EvtId di=par->getDaug(i)->getId();
    EvtVector4R p4i = par->getDaug(i)->getP4Lab();
    int pdgcode =EvtPDL::getStdHep( di );
    double alpha=0;

    if(pdgcode==111 ||pdgcode==221 || pdgcode==331){//for photon 
      alpha = 1;
    }else continue;

    double angmax = 1+alpha;
    double costheta = cos(p4i.theta());
    double ang=1+alpha*costheta*costheta;
    double xratio = ang/angmax;
    double xi=EvtRandom::Flat(0.,1);
    //std::cout<<"pdgcode "<<pdgcode<<std::endl;
    //std::cout<<ang<<" "<<angmax<<" "<<xi<<" "<<xratio<<std::endl;
    if(xi>xratio) return false;
  }//loop over duaghters
  //if(tagp) std::cout<<" cosp "<<cosp<<std::endl;
  //if(tagk) std::cout<<" cosk "<<cosk<<std::endl;

  return true;
}


double EvtConExc::narrowRXS(double mxL,double mxH){
  //br for ee 
  double psippee,psipee,jsiee,phiee,omegaee,rhoee;
  double kev2Gev=0.000001;
  psippee=0.262*kev2Gev;
  psipee =2.36*kev2Gev;
  jsiee  =5.55*kev2Gev;
  phiee=4.266*0.001*2.954*0.0001;
  omegaee=0.6*kev2Gev;
  rhoee=7.04*kev2Gev;
  double s=_cms*_cms;
  double sigv=0;
  double mv=0;
  double widee=0;
  double xpi=12*3.1415926*3.1415926;
  if(mxL<=3.0957 && 3.0957<=mxH || mxL<=3.0983 && 3.0983<=mxH) {
    widee = jsiee;
    mv = 3.096916;
  }else if(mxL<=3.6847 && 3.6847<=mxH || mxL<=3.6873 && 3.6873<=mxH){
    widee = psipee;
    mv = 3.686109;
  }else if(mxL<=1.01906 && 1.01906<=mxH || mxL<= 1.01986 &&  1.01986<=mxH){
    widee = phiee;
    mv = 1.01946;
  }else{return -1.0;}
  
  if(_cms<=mv) return -1.;
  double xv = 1-mv*mv/s;
  sigv += xpi*widee/mv/s*Rad2(s,xv);
  double unic = 3.89379304*100000;  //in unit nb
  return sigv*unic;  //vaccum factor has included in the Rad2
}


double EvtConExc::addNarrowRXS(double mhi,double binwidth){
  double myxs = 0;
  for(int i=0;i<ISRXS.size();i++){ // std::cout<<"ISRM "<<ISRM[i]<<std::endl;
    if( (ISRM[i]-mhi)<binwidth && ISRFLAG[i]){ISRFLAG[i]=0; myxs = ISRXS[i];}
  }
  //std::cout<<mhi<<" "<<binwidth<<" "<<myxs<<std::endl;
  return myxs;
}

double EvtConExc::selectMass(){
  double pm,mhdL,mhds;
  pm   = EvtRandom::Flat(0.,1);
  mhdL =_mhdL; 
  mhds = pm*(_cms - mhdL)+mhdL;  //non narrow resonance
  std::vector<double> sxs;
  for(int i=0;i<ISRID.size();i++){
    double ri=ISRXS[i]/AF[598];
    sxs.push_back(ri);
  }
  for(int j=0;j<sxs.size();j++){
    if(j>0) sxs[j] += sxs[j-1];
  }
  pm  = EvtRandom::Flat(0.,1);
  if(pm<sxs[0]) {
    mhds = EvtPDL::getMass(ISRID[0]); //narrow resonance
  }
  for(int j=1;j<sxs.size();j++){
    double x0 = sxs[j-1];
    double x1 = sxs[j];
    if(x0<pm && pm<=x1)  mhds = EvtPDL::getMass(ISRID[j]); //narrow resonance
  }
  return mhds;
}

double EvtConExc::getVP(double mx){
  double xx,r1,i1;
  double x1,y1,y2;
  xx=0;
  int mytag=1;
  for(int i=0;i<4001;i++){
    x1=vpx[i];
    y1=vpr[i];
    y2=vpi[i];
    if(xx<=mx && mx <x1){xx=x1; r1=y1; i1=y2; mytag=0; break;}
    xx=x1; r1=y1; i1=y2;
  }
  if(mytag==1){std::cout<<"No vacuum polarization value found"<<std::endl;abort();}
  EvtComplex cvp(r1,i1);
  double thevp=abs(1./(1-cvp));
  if(3.0933<mx && mx<3.1035) return 1.; //J/psi peak excluded
  if(3.6810<mx && mx<3.6913) return 1.; //psi(2S) peak excluded
  return thevp*thevp;
}


void EvtConExc::ReadVP(){
  vpx.clear();vpr.clear();vpi.clear();
  int vpsize=4001;
  vpx.resize(vpsize);
  vpr.resize(vpsize);
  vpi.resize(vpsize);
  std::string locvp=getenv("BESEVTGENROOT");
  locvp +="/share/vp.dat";
  ifstream m_inputFile; 
  m_inputFile.open(locvp.c_str());
  double xx,r1,i1;
  double x1,y1,y2;
  xx=0;
  int mytag=1;
  for(int i=0;i<4001;i++){
    m_inputFile>>vpx[i]>>vpr[i]>>vpi[i]; 
  }
  
}

void EvtConExc::mk_VXS(double Esig, double Egamcut, double EgamH, int midx){
  //the mode index is put into vmode
  //initial
  //midx: mode index in vmode[midx] and VXS[midx][bin]
  int mode=vmode[midx];
  double uscale;

  EvtXsection* myxsection = new EvtXsection (mode);
  if(myxsection->getUnit() =="pb") {uscale =0.001;}else{uscale=1;} 
  double s = _cms*_cms;  
  double mlow=myxsection->getXlw();
  if(_cms <= mlow){
    for(int i=0;i<600;i++){VXS[midx][i]=0;}
    return;
  }

  int nint=50;
  int nmc=800;
  //double myxs0 =uscale*gamHXSection(s,Esig,Egamcut,nmc);
  double myxs0 = uscale*trapezoid(s,Esig,Egamcut,nint,myxsection); 
  double xb= 2*Esig/_cms;
  double sig_xs = uscale*SoftPhoton_xs(s,xb)*(myxsection->getXsection(_cms));
  myxs0 += sig_xs;

  int Nstp=598;
  double stp=(EgamH - Egamcut)/Nstp;
  for(int i=0;i<Nstp;i++){//points within Egam(max) ~ Egamcut=25MeV
    double Eg0=EgamH - i*stp;
    double Eg1=EgamH - (i+1)*stp;           
    //double xsi= uscale*gamHXSection(s,Eg1,Eg0,nmc); 
    double xsi= uscale*trapezoid(s,Eg1,Eg0,nint,myxsection); 
    if(i==0) VXS[midx][0]=xsi; 
    if(i>=1) VXS[midx][i]=VXS[midx][i-1]+xsi;
  }
   VXS[midx][598]=VXS[midx][597];
   VXS[midx][599]=VXS[midx][598]+ myxs0;
   //std::cout<<"mode "<<mode<<" "<<uscale<<" "<<VXS[midx][599]<<std::endl; 
}

int EvtConExc::get_mode_index(int mode){
  int i=0;
  for(int j=0;i<vmode.size();j++){
    if(mode==vmode[j]) return j;
  }
  std::cout<<" EvtConExc::get_mode_index:  no index is found in vmode"<<std::endl;
  abort();
}

double EvtConExc::getObsXsection(double mhds,int mode){
  double hwid=(AA[0]-AA[1])/2.;
  double s=_cms*_cms;
  int idx=get_mode_index(mode);
  double Egam=(s-mhds*mhds)/2/_cms;
  for(int i=0;i<600;i++){
    if( (AA[i]-hwid)<=Egam && Egam<(AA[i]+hwid) ) return VXS[idx][i];
  }

 std:cout<<"EvtConExc::getObsXsection : no observed xs is found "<<endl;
  abort();
}

double EvtConExc::Egam2Mhds(double Egam){
  double s=_cms*_cms;
  double mhds = sqrt( s - 2*Egam*_cms);
  return mhds;
}

void EvtConExc::writeDecayTabel(){
  ofstream oa,ob;
  oa.open("_pkhr.dec");
  ob.open("obsxs.dat");
  //
  int im=getModeIndex(74110);

  double xscon=VXS[im][599];
  //std::cout<<"tsize "<<tsize<<" "<<xscon<<" "<<VXS[70][599]<<std::endl;
  std::vector<int> Vmode;
  Vmode.push_back(6);//1:pi+pi-
  Vmode.push_back(13);//2:pi+pi-2pi0
  Vmode.push_back(12);//3:2(pi+pi-)
  Vmode.push_back(0);//4:ppbar
  Vmode.push_back(1);//5:nnbar
  Vmode.push_back(45);//6:K+K-
  Vmode.push_back(46);//7:K0K0bar
  Vmode.push_back(7);//8:pi+pi-pi0
  Vmode.push_back(2);//9:Lambda anti-Lambda
  Vmode.push_back(37);//10: pi+pi- eta
  std::vector<std::string> vmdl;
  vmdl.push_back("PHOKHARA_pipi");
  vmdl.push_back("PHOKHARA_pi0pi0pipi");
  vmdl.push_back("PHOKHARA_4pi");
  vmdl.push_back("PHOKHARA_ppbar");
  vmdl.push_back("PHOKHARA_nnbar");
  vmdl.push_back("PHOKHARA_KK");
  vmdl.push_back("PHOKHARA_K0K0");
  vmdl.push_back("PHOKHARA_pipipi0");
  vmdl.push_back("PHOKHARA_LLB");
  vmdl.push_back("PHOKHARA_pipieta");
  
  ob<<"mode_index  observed cross /nb"<<std::endl;
  for(int i=0;i<vmode.size();i++){
    ob<<vmode[i]<<" "<<VXS[getModeIndex(vmode[i])][599]<<std::endl;
//cout<<"vmode["<<i<<"] = "<<vmode[i]<<", VXS["<<getModeIndex(vmode[i])<<"][599] = "<<XS[getModeIndex(vmode[i])][599]<<std::endl; // wangwp
  }

  for(int i=0;i<Vmode.size();i++){
    //std::cout<<"i: "<<xscon<<" "<<VXS[Vmode[i]][599]<<std::endl;
    xscon -= VXS[ getModeIndex(Vmode[i]) ][599];
  }
  //debugging
  for(int i=0;i<Vmode.size();i++){
    //  std::cout<<Vmode[i]<<"-th mode: "<<VXS[getModeIndex(Vmode[i])][599]<<" "<<std::endl;
  }  

  oa<<"LundaPar PARJ(11)=0.611798"<<std::endl;
  oa<<"LundaPar PARJ(12)=7.92527E-12"<<std::endl;
  oa<<"LundaPar PARJ(14)=0.244495"<<std::endl;
  oa<<"LundaPar PARJ(15)=1.16573E-15"<<std::endl;
  oa<<"LundaPar PARJ(16)=0.436516"<<std::endl;
  oa<<"LundaPar PARJ(17)=0.530517"<<std::endl;
  oa<<"LundaPar PARJ(1)=0.0651577"<<std::endl;
  oa<<"LundaPar PARJ(2)=0.260378"<<std::endl;
  oa<<"LundaPar PARJ(21)=0.0664835"<<std::endl;
  oa<<"LundaPar RALPA(15)=0.576687"<<std::endl;
  oa<<"LundaPar RALPA(16)=0.364796"<<std::endl;
  oa<<"LundaPar RALPA(17)=3.19486E-06"<<std::endl;
  oa<<"noPhotos"<<std::endl;
  oa<<"Particle vpho "<<_cms<<" 0"<<std::endl;
  oa<<"Decay vpho"<<std::endl;
  oa<<"0  pi+ pi-         PHSP ;"<<std::endl;
  oa<<"0  pi0 pi0 pi- pi+ PHSP ;"<<std::endl;
  oa<<"0  pi+ pi- pi- pi+ PHSP ;"<<std::endl;
  oa<<"0  anti-p- p+      PHSP ;"<<std::endl;
  oa<<"0  anti-n0 n0      PHSP ;"<<std::endl;
  oa<<"0  K+ K-           PHSP ;"<<std::endl;
  oa<<"0  K_S0 K_L0       PHSP ;"<<std::endl;
  oa<<"0  pi+ pi- pi0     PHSP ;"<<std::endl;
  oa<<"0  Lambda0 anti-Lambda0 PHSP ;"<<std::endl;
  oa<<"0  pi+ pi- eta    PHSP ;"<<std::endl;
  oa<<"0  gamma phi      PHSP;"<<std::endl;
  oa<<"0  gamma rho0     PHSP;"<<std::endl;
  oa<<"0  gamma omega    PHSP;"<<std::endl;
  oa<<xscon<<" ConExc 74110;"<<std::endl;
  for(int i=0;i<Vmode.size();i++){
    oa<<VXS[ getModeIndex(Vmode[i]) ][599]<<" "<<vmdl[i]<<" ;"<<std::endl;
  }
  oa<<"Enddecay"<<std::endl;
  oa<<"End"<<std::endl;
}



void EvtConExc::checkEvtRatio(){
  ofstream oa;
  oa.open("_evtR.dat");
  //
  int im=getModeIndex(74110);
     double xscon=VXS[im][599];
     double xscon0=xscon;
  oa<<"Ecms= "<<_cms<<" GeV"<<std::endl;
  oa<<"The total observed xs: "<<xscon<<" nb"<<std::endl;
  oa<<"=== mode =========== ratio ==========="<<std::endl;
  for(int i=0;i<vmode.size();i++){
    //std::cout<<"i: "<<xscon<<" "<<VXS[Vmode[i]][599]<<std::endl;
      int themode=getModeIndex(vmode[i]);
      if(vmode[i]==74110) continue;
      xscon -= VXS[themode ][599];
     }
     if(xscon<0) xscon=0;
    //debugging
    for(int i=0;i<vmode.size();i++){
     int themode=getModeIndex(vmode[i]);
     if(vmode[i]==74110) continue;
     oa<<vmode[i]<<"-th mode: "<<100*VXS[themode][599]/xscon0<<" % "<<std::endl;
    }
    oa<<"74110-th mode: "<<100*xscon/xscon0<<" % "<<std::endl;


}

int EvtConExc::getModeIndex(int m){
  for (int i=0;i<vmode.size();i++){
     if(m==vmode[i]) return i;
  }
 std::cout<<"EvtConExc::getModeIndex: no index in vmode found "<<std::endl;
 abort();
}

std::string EvtConExc::commandName(){

  return std::string("ConExcPar");

}

void EvtConExc::command(std::string cmd){
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


void EvtConExc::store(EvtDecayBase* jsdecay){

  if (nconexcdecays==ntable){

    EvtDecayBasePtr* newconexcdecays=new EvtDecayBasePtr[2*ntable+10];
    int i;
    for(i=0;i<ntable;i++){
      newconexcdecays[i]=conexcdecays[i];
    }
    ntable=2*ntable+10;
    delete [] conexcdecays;
    conexcdecays=newconexcdecays;
  }

  conexcdecays[nconexcdecays++]=jsdecay;

}


std::vector<std::string> EvtConExc::split(std::string str,std::string pattern)
{
  std::string::size_type pos;
  std::vector<std::string> result;
  str+=pattern;
  int size=str.size();
  
  for(int i=0; i<size; i++)
    {
      pos=str.find(pattern,i);
      if(pos<size)
        {
	  std::string s=str.substr(i,pos-i);
	  result.push_back(s);
	  i=pos+pattern.size()-1;
        }
    }
  return result;
}


void EvtConExc::InitPars(){
  threshold=0;
  beamEnergySpread=0;
  std::string pattern="=";
  for(int i=0;i<ncommand;i++){
    std::vector<std::string> result=split(commands[i],pattern);
    if(result[0]=="threshold")       { threshold=atof(result[1].c_str());}else
      if(result[0]=="beamEnergySpread"){ beamEnergySpread=atof(result[1].c_str());}
      else{
	std::cout<<"Your parameter in decay card \""<<result[0]<<"\" incorect"<<std::endl;
	std::cout<<"Possible words: threshold, beamEnergySpread"<<std::endl;
	abort();
      }
  }

}


double EvtConExc::energySpread(double mu,double sigma){
  //mu: mean value in Gaussian
  //sigma: variance in Gaussian 
 rloop:
  int n=12;
  double ri=0;
  for(int i=0;i<n;i++){
    double pm= EvtRandom::Flat(0.,1);
    ri += pm;
  }
  double eta=sqrt(n*12.0)*(ri/12-0.5);
  double xsig= sigma*eta+mu;//smapling Gaussion value
  double mx0=staxsection->getXlw();
  double mx1=staxsection->getXup();
  if(xsig<mx0 || xsig>mx1) goto rloop;
  return xsig;
}

void EvtConExc::calAF(double myecms){
  

  double _cms=myecms;
  double Esig = 0.0001;  //sigularity engy 
  double x = 2*Egamcut/_cms;
  double s = _cms*_cms;

  double mhdL=staxsection->getXlw();
  double EgamH = (s-mhdL*mhdL)/2/sqrt(s);

  int nint=50;
  int nmc= 1000; 
  _xs0 = trapezoid(s,Esig,Egamcut,nint);  //   std::cout<<"Int: _xs0= "<<_xs0<<std::endl;
   //--- sigularity point x from 0 to 0.0001GeV
  double xb= 2*Esig/_cms;
  double sig_xs = SoftPhoton_xs(s,xb)*(staxsection->getXsection(_cms));
  _xs0 += sig_xs;

  int Nstp=598;
  double stp=(EgamH - Egamcut)/Nstp;
  for(int i=0;i<Nstp;i++){//points within Egam(max) ~ Egamcut=25MeV
    double Eg0=EgamH - i*stp;
    double Eg1=EgamH - (i+1)*stp;
    double xsi= trapezoid(s,Eg1,Eg0,nint);       //    std::cout<<"Int xsi= " <<xsi<<std::endl;
 
    AA[i]=(Eg1+Eg0)/2;
    double mhi=sqrt(_cms*_cms-2*_cms*AA[i]);
    double mh2=sqrt(_cms*_cms-2*_cms*Eg1);
    double binwidth = mh2-mhi;
    if(i==0) AF[0]=xsi; 
    if(i>=1) AF[i]=AF[i-1]+xsi;
    RadXS[i]=xsi/stp;
  } 
  //add the interval 0~Esig
  AA[598]=Egamcut;   AA[599]=0; //AA[i]: E_gamma 
  AF[598]=AF[597];
  AF[599]=AF[598]+ _xs0;
  //--
  for(int i=0;i<600;i++){
    MH[i]=sqrt(_cms*_cms-2*_cms*AA[i]);
  }
  //--debugging
  double bornXS   = staxsection->getXsection(_cms);
  if(bornXS==0){std::cout<<"EvtConExc::calAF: bad BornXS at "<<_cms<<" GeV"<<std::endl;abort();}
  double fisr=AF[599]/bornXS;
  myFisr.push_back(fisr);
  //std::cout<<"fisr= "<<fisr<<std::endl;
}


void EvtConExc::OutStaISR(){
  int ntot=myFisr.size();
  double mymu=0;
  for(int i=0;i<ntot;i++){mymu += myFisr[i];}
  mymu /= ntot;
  double mysig=0;
  for(int i=0;i<ntot;i++){ mysig += (myFisr[i]-mymu)*(myFisr[i]-mymu);}
  mysig /=ntot;
  mysig = sqrt(mysig);
  std::cout<<"========= Iteration times over ISR factor: "<<ntot<<std::endl;
  std::cout<<" ISR factor * VP factor= observedXS/BornXS: "<<mymu<<" + "<<mysig<<std::endl;
}


double EvtConExc::trapezoid(double s,  double El,double Eh,int n)// integration with trapezoid method
{
  double xL=2*El/sqrt(s);
  double xH=2*Eh/sqrt(s);
  double sum = 0.0;
  double gaps = (xH-xL)/double(n);  //interval
  for (int i = 0; i < n; i++)
    {
      sum += (gaps/2.0) * (Rad2difXs(s,xL + i*gaps, staxsection) + Rad2difXs(s,xL + (i+1)*gaps,staxsection) );
    }
  return sum;
}

double EvtConExc::trapezoid(double s,  double El,double Eh,int n, EvtXsection* myxs)// integration with trapezoid method
{
  double xL=2*El/sqrt(s);
  double xH=2*Eh/sqrt(s);
  double sum = 0.0;
  double gaps = (xH-xL)/double(n);  //interval
  for (int i = 0; i < n; i++)
    {
      sum += (gaps/2.0) * (Rad2difXs(s,xL + i*gaps, myxs) + Rad2difXs(s,xL + (i+1)*gaps,myxs) );
    }
  return sum;
}
