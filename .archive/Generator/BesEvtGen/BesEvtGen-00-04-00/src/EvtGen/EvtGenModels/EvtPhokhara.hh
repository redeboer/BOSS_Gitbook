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
// Module: EvtPhokhara.cc
//         the necessary file: jetset74.F,lund_crm1_evtgen.F
//                             fist.inc,gen.inc  mix.inc  stdhep.inc
// Description: Modified Lund model at tau-charm energy level, see
//               PHYSICAL REVIEW D, VOLUME 62, 034003
// Modification history:
//   
//   Ping R.-G.    Octo., 2007       Module created    
//
//------------------------------------------------------------------------

#ifndef EVTPHOKHARA_HH
#define EVTPHOKHARA_HH

#include "EvtGenBase/EvtDecayIncoherent.hh"

class EvtParticle;

typedef EvtDecayBase* EvtDecayBasePtr;

#include <iosfwd>
 
class EvtPhokhara:public  EvtDecayIncoherent  {

public:

  EvtPhokhara();
  virtual ~EvtPhokhara();

  void getName(std::string& name);
  EvtDecayBase* clone();
  void decay(EvtParticle *p); 

  std::string commandName();
  void command(std::string cmd);

  void init();
  void init_mode(EvtParticle* p);
  void init_evt(EvtParticle* p);

  void initProbMax();
  int getTotalEvt(){return nevt;}
  void PhokharaInit(int dummy);
  void ExclusiveDecay(EvtParticle* p);
private:

  void store(EvtDecayBase* jsdecay);
 

  static int nphokharadecays;
  static EvtDecayBasePtr* phokharadecays;
  static int ntable;

  static int ncommand;
  static int lcommand;
  static std::string* commands;
  static int nevt;
// jobOption params

 int m_nm;          // # of events to determine the maximum   
 int m_nlo;         // Born(0), NLO(1)
 double m_w;        // soft photon cutoff                
 int m_pion;        // mu+mu-(0),pi+pi-(1),2pi0pi+pi-(2),2pi+2pi-(3),ppbar(4),nnbar(5),
                    // K+K-(6),K0K0bar(7),pi+pi-pi0(8), Lamb Lambbar->pi-pi+ppbar(9) 
 int m_fsr;         // ISR only(0), ISR+FSR(1), ISR+INT+FSR(2)
 int m_fsrnlo;      // yes(1), no(0)
 int m_ivac;        // no(0), yes - Jegerlehner(1), yes - Teubner(2)
 int m_tagged;      // tagged photons(0), untagged photons(1)
 int m_NarrowRes;  // none(0), JPsi (1), Psip(2)
 int m_FF_Kaon;     // KaonFormFactor: constrained (0), unconstrained (1), Kuhn-Khodjamirian-Bruch (2) 
 int m_FF_Pion;     // KS Pionformfactor(0), GS Pionformfactor old(1), GS Pionformfactor new(2)
 int  m_f0_model;   // f0+f0(600): KK model(0), no structure(1), no f0+f0(600)(2), f0 KLOE(3)
 double m_E;        // CMS-energy                        
 double m_q2min;    // minimal  hadrons(muons)-gamma-inv mass squared     
 double m_q2_min_c; // minimal inv. mass squared of the hadrons(muons)
 double m_q2_max_c; // maximal inv. mass squared of the hadrons(muons)
 double m_gmin;     // minimal photon energy/missing energy             
 double m_phot1cut; // minimal photon angle/missing momentum angle
 double m_phot2cut; // maximal photon angle/missing momentum angle
 double m_pi1cut;   // minimal hadrons(muons) angle
 double m_pi2cut;   // maximal hadrons(muons) angle

 double qqmin,qqmax;
 double cos1min,cos1max,cos2min,cos2max,cos3min,cos3max;
 double dsigm1,dsigm2,sigma1,sigma2,sigma,dsigm,Ar[14],Ar_r[14];
 
  int ievent;
  long int m_initSeed;
  std::vector<std::vector<double> > theMmax;
  std::vector<int> nevtgen;
  std::vector<std::string> Vfs;
};

#endif




