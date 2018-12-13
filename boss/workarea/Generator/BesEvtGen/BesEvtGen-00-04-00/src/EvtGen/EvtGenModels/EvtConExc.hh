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
// Module:  EvtConExc.hh
//
// Description: To define cross section for the continuum exclusive process
// Experimental cross section taken from PRD73,012005, PRD76,092006, also 
// see a review: Rev. Mod. Phys. 83,1545
  /*******************--- mode definition: also see EvtXsection.cc
  0: ppbar
  1: nnbar
  2: Lambda0 anti-Lambda0
  3: Sigma0 anti-Sigma0
  4: Lambda0 anti-Sigma0
  5: Sigma0 anti-Lambda0
  6: pi+ pi-
  7: pi+ pi- pi0
  8: K+K- pi0
  9: KsK+pi- 
  10: KsK-pi+
  11: K+K-eta
  12: 2(pi+pi-)
  13: pi+pi-2pi0
  14: K+K-pi+pi-
  15: K+K-2pi0
  16: 2(K+K-)
  17: 2(pi+pi-)pi0
  18: 2(pi+pi-)eta
  19: K+K-pi+pi-pi0  		
  20: K+K-pi+pi-eta
  21: 3(pi+pi-)
  22: 2(pi+pi-pi0)
  23: phi eta
  24: phi pi0
  25: K+K*-
  26: K-K*+
  27: K_SK*0-bar
  28: K*0(892)K+pi-
  29: K*0(892)K-pi+
  30: K*+K-pi0
  31: K*-K+pi0
  32: K_2*(1430)0 K+pi-
  33: K_2*(1430)0 K-pi+
  34: K+K-rho
  35: phi pi+pi-
  36: phi f0(980)
  37: eta pi+pi-
  38: omega pi+ pi-
  39: omega f0(980)
  40: eta' pi+ pi-
  41: f_1(1285)pi+pi-
  42: omega K+K-
  43: omega pi+pi-pi0  
  *************************************/
// Modification history:
//
//    Ping R.-G.       Nov., 2012       Module created
//
//------------------------------------------------------------------------
//
#ifndef EVTCONEXC_HH
#define EVTCONEXC_HH 

//#include <string.h>
#include "EvtGenBase/EvtId.hh"
#include "EvtGenBase/EvtDecayIncoherent.hh"
#include "EvtGenModels/EvtXsection.hh"
//------ including root  
#include "TH1.h" 
#include "TH2.h"
#include "TH3.h"
#include "TFile.h"  
#include "TROOT.h" 
#include "TTree.h"
#include "TGraphErrors.h"
#include "TDirectory.h"

class EvtParticle;
typedef EvtDecayBase* EvtDecayBasePtr;
 
class EvtConExc:public  EvtDecayIncoherent  {

public:
  
  EvtConExc() {
  } //constructor
  //---
  virtual ~EvtConExc();

  void getName(std::string& name);

  EvtDecayBase* clone();

  void initProbMax();

  void init();
  void init_Br_ee();

  void decay(EvtParticle *p); 
  void init_mode(int mode);
  double gamHXSection(EvtParticle* p, double El, double Eh, int nmc=100000);
  double gamHXSection(double s, double El, double Eh, int nmc=100000);
  double gamHXSection(double El, double Eh);
  double gamHXSection(double El, double Eh,int mode);
  double gamHXSection_er(double El,double Eh);

  void  findMaxXS(EvtParticle *p);
  double difgamXs(EvtParticle* p); //differential cross section for gamma + hadrons
  double difgamXs(double mhds,double sintheta);
  double Mhad_sampling(double *x,double *y);
  double ISR_ang_integrate(double x,double theta);
  double ISR_ang_sampling(double x);
  bool hadron_angle_sampling(EvtVector4R ppi, EvtVector4R pcm);
  void SetP4(EvtParticle *part, double mhdr,double xeng, double theta); //set the gamma and gamma* momentum according sampled results
  void SetP4Rvalue(EvtParticle *part, double mhdr,double xeng, double theta); //set the gamma and gamma* momentum according sampled results
  bool gam_sampling(EvtParticle *p);
  bool xs_sampling(double xs);
  bool xs_sampling(double xs,double xs1);
  bool baryon_sampling(EvtVector4R pcm, EvtVector4R pi);//baryon angular distri. 1+cos^2\theta
  bool meson_sampling(EvtVector4R pcm, EvtVector4R pi); //meson angular distri. sin^2\theta
  bool VP_sampling(EvtVector4R pcm, EvtVector4R pi); //VP ang. dist. 1+cos^2theta
  bool angularSampling(EvtParticle* part);
  bool photonSampling(EvtParticle* part);
  double baryonAng(double mx);
  double Rad1(double s, double x);
  double Rad2(double s, double x);
  double Rad2difXs(EvtParticle *p);
  double Rad2difXs(double s, double x);
  double Rad2difXs(double s, double x, EvtXsection* myxsection);
  double Rad1difXs(EvtParticle *p);
  double Ros_xs(double mx, double bree,EvtId pid);
  static EvtXsection *myxsection, *staxsection;
  static double _cms; //energy of CMS of ee beam
  static double XS_max;// maxium of cross section in experiment
  static double SetMthr;

  static int getMode;
  double Li2(double x);
  double SoftPhoton_xs(double s,double b);
  double lgr(double *x,double *y,int n,double t);
  bool islgr(double *x,double *y,int n,double t);
  double LLr(double *x,double *y,int n,double t);
  int selectMode(std::vector<int> vmod, double mhds);
  void findMaxXS(double mhds );
  bool gam_sampling(double mhds,double sintheta);
  void resetResMass();
  void getResMass();
  bool checkdecay(EvtParticle* p);
  double sumExc(double mx);
  void showResMass();
  int getNdaugs(){return _ndaugs;}
  EvtId* getDaugId(){return daugs;}
  int getSelectedMode(){return _selectedMode;}
  static int conexcmode, multimode;
  double narrowRXS(double mxL,double mxH);
  double selectMass();
  double addNarrowRXS(double mhi, double binwidth);
  void ReadVP();
  double getVP(double cms);
  void mk_VXS(double Esig,double Egamcut,double EgamH,int midx); //make a set of observed cross section for exclusive processes,including narrow resonance
  int get_mode_index(int mode);
  double getObsXsection(double mhds,int mode);
  double Egam2Mhds(double Egam);
  std::vector<EvtId> get_mode(int mode); 
  void writeDecayTabel();
  void checkEvtRatio();
  int getModeIndex(int m);
  std::string commandName();
  void command(std::string cmd);
  std::vector<std::string> split(std::string str,std::string pattern);
  void InitPars();
  double energySpread(double mu,double sigma);
  void calAF(double myecms);
  void OutStaISR();
  double trapezoid(double s, double a,  double  b, int n);//integration with trapezoid method
  double trapezoid(double s,  double El,double Eh,int n, EvtXsection* myxs);
  static double mlow,mup;
private:

  int _mode,_ndaugs,radflag,testflag;
  EvtId daugs[10],gamId;//daugs[0]~dagus[_ndaugs-1] are hadrons, daugs[_ndaugs] is ISR gamma
  static double _xs0,_xs1; //cross section for 0 and 1-photon processes
  static double _er0,_er1; //cross section for 0 and 1-photon processes
  static int _nevt;
  std::vector<double> ISRXS,ISRM;
  std::vector<bool> ISRFLAG;
  EvtParticle* gamH;
  double maxXS;//maximum of diffrential cross section respective to cos\theta and mhds for ee->gamma hadrons
  double differ,differ2,Rad2Xs;
  std::string _unit;
  std::vector<double> BR_ee;
  std::vector<EvtId > ResId,ISRID;

  double Egamcut;
  //-- for deguggint to make root file
  TFile *myfile;
  Double_t pgam[4],phds[4],ph1[4],ph2[4],mhds,sumxs;
  Double_t mass1,mass2,costheta,selectmode;
  Double_t cosp,cosk;
  Int_t imode;
  TTree *xs;
  bool mydbg; //handler w/o debugging
  TGraphErrors *mygr;
  TH1F* myth,*Xobs,*Xsum;
  // for calulate the correction factor
  int pdgcode;
  std::string file;
  EvtId son[10],pid;
  int nson;
  double vph; //for vaccuam polarization calculation
  static  double AF[600],AA[600],MH[600];
  double RadXS[600],EgamH;
  std::vector<double> myFisr;

  double mjsi,mpsip,mpsipp,mphi,momega,mrho0,mrho3s,momega2s;
  double wjsi,wpsip,wpsipp,wphi,womega,wrho0,wrho3s,womega2s;
  double _mhdL;
  double cmsspread;
  int _selectedMode;
  std::vector<int> _modeFlag;
  bool VISR;
  std::vector<int > vmode, vmd;
  static std::vector<std::vector <double> > VXS;
  std::vector<double> vpx,vpr,vpi;
  double Mthr;
  EvtParticle *theparent;

  static int nconexcdecays;
  static EvtDecayBasePtr* conexcdecays;
  static int ntable;

  static int ncommand;
  static int lcommand;
  static std::string* commands;
  void store(EvtDecayBase* jsdecay);
  //--  parameter list in user's decay card by ConExcPar
  double threshold, beamEnergySpread;
};

#endif

