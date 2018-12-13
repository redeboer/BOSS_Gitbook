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
//--- 
  *************************************/
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

