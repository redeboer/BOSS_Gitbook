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
// Module: EvtXsection.hh
//
// Description: To define cross section for the continuum exclusive process
// Experimental cross section taken from PRD73,012005, PRD76,092006, also 
// see a review: Rev. Mod. Phys. 83,1545
// Modification history:
//
//    Ping R.-G.       Nov., 2012       Module created
//
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
  44: Sigma+ Sigma- (Sigma0 Sigma0-bar SU(3) extention )
  45: K+K-
  46: K_S K_L

  70: D0D0-bar
  71: D+D-
  72: D+D*-
  73: D-D*+
  74: D*+D*-
  75: D0D-pi+
  76: D0D+pi-
  77: D0D*-pi+
  78: D0D*+pi-
  79: Lambda_c+ Lambda_c-
  

  90: J/psi pi+ pi-
  91: psi(2S)pi+pi-
  92: J/psi K+K- 
  93: D_s+ D_s-
  94: D_s^{*+}D_s^-
  95: D_s^{*-}D_s^+
  
  *************************************/
//------------------------------------------------------------------------
//
#ifndef EVTXSECTION_HH
#define EVTXSECTION_HH

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <ostream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "EvtGenBase/EvtId.hh"
#include "EvtGenBase/EvtComplex.hh"
using namespace std;

class EvtXsection{
 public:
  EvtXsection(std::vector<EvtId> evtdaugs){
    xx.clear();yy.clear();er.clear();
    _mode = getMode(evtdaugs);
    ini_data(_mode);
  
  }
  EvtXsection(int mode){
    xx.clear();yy.clear();er.clear();
    _mode = mode;
    if(mode>=0){
    ini_data(_mode);
    }else if(mode==-1){//calculate the correction factor with Breit-wigner
      _unit="nb";
    }else if(mode==-2){//users provide the cross section list
      //std::cout<<"user diy"<<std::endl;
       ini_data_diy();
    }else if(mode==-100){//multi-exclusive modes
      _unit="nb";
    }
    //std::cout<<"The mode is : "<<_mode<<", "<<xx[0]<<", "<<yy[0]<<std::endl;
  }

  virtual ~EvtXsection();

  int getMode(std::vector<EvtId> evtdaugs);
  void ini_data0(int mode);  // exper. cross section data table
  void ini_data(int mode);   // fitted results on the cross section of expe. xsection
  void ini_data_diy();
  void ini_data_multimode();
  double getXsection(double mx);
  double getErr(double mx);
  double Xsection_a(double mx);//for bin given by up and lower edge
  double Xsection_b(double mx);//for bin given by a center value
  double Xsection_c(double mx);//for xsection calculated with breit-wigner

  double Err_a(double mx);//for bin given by up and lower edge
  double Err_b(double mx);//for bin given by a center value

  int getXBin(double mx,std::vector<double> vy);
  int getXBin_a(double mx );//for bin given by up and lower edge
  int getXBin_b(double mx );//for bin given by a center value
  std::string getUnit(){return _unit;}

  std::vector<double> getXX(){return xx;}
  std::vector<double> getYY(){return yy;}
  std::vector<double> getEr(){return er;}
  double getXup(){return xx[nbins-1];}
  double getXlw(){return xx[0];}
  std::string getMsg(){return msg;}
  void setBW(int pdg);
  void setFile(std::string name){file = name;}
  double getVP(double mx);
  void ReadVP();
  void setModes(std::vector<int> vmd);
 private:
//e+e- --> ppbar mode, xsection taken from PRD73,012005
  std::vector<double> xx,yy,er;
  int nbins,_mode;
  std::string _unit,msg;
  // calculate cross section with Breit-Wigner
  int pdgcode;
  double bree;
  // user provide xsection list
  std::string file;
  std::vector<double> vpx,vpr,vpi;
  std::vector<int> _vmd;
};

#endif
