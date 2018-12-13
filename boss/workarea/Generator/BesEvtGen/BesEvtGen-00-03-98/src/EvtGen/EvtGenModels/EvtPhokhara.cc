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
// Module: EvtPhokhara.cc
//         the necessary file: phokharar.F
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
#include "EvtGenModels/EvtPhokhara.hh"
#include "EvtGenModels/EvtPhokharaDef.hh"
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

#include "GeneratorObject/McGenEvent.h"
#include "BesKernel/IBesRndmGenSvc.h"
#include "CLHEP/Random/RandomEngine.h"
#include "cfortran/cfortran.h"

using namespace std;
using namespace CLHEP;

using std::endl;
using std::fstream;
using std::ios;
using std::ofstream;
using std::resetiosflags;
using std::setiosflags;
using std::setw;

int EvtPhokhara::nphokharadecays=0;
EvtDecayBasePtr* EvtPhokhara::phokharadecays=0; 
int EvtPhokhara::ntable=0;

int EvtPhokhara::ncommand=0;
int EvtPhokhara::lcommand=0;
std::string* EvtPhokhara::commands=0;
int EvtPhokhara::nevt=0;

EvtPhokhara::EvtPhokhara(){}
EvtPhokhara::~EvtPhokhara(){

  //print out message
  // =================================================
  if(flags_.pion == 9)
    maxima_.Mmax[1] = maxima_.Mmax[1] * (1.0 + lambda_par_.alpha_lamb)*(1.0 + lambda_par_.alpha_lamb)
      * lambda_par_.ratio_lamb*lambda_par_.ratio_lamb;

  // --- value of the cross section ------------------
  if (flags_.nlo == 0) {
    sigma = maxima_.Mmax[0]/maxima_.count[0]*maxima_.tr[0];
    dsigm = maxima_.Mmax[0]*sqrt((maxima_.tr[0]/maxima_.count[0]-(maxima_.tr[0]/maxima_.count[0])*(maxima_.tr[0]/maxima_.count[0]))/maxima_.count[0]);
  } else {
    sigma1 = maxima_.Mmax[0]/maxima_.count[0]*maxima_.tr[0];
    sigma2 = maxima_.Mmax[1]/maxima_.count[1]*maxima_.tr[1];
    dsigm1 = maxima_.Mmax[0]*sqrt((maxima_.tr[0]/maxima_.count[0]-(maxima_.tr[0]/maxima_.count[0])*(maxima_.tr[0]/maxima_.count[0]))/maxima_.count[0]);
    dsigm2 = maxima_.Mmax[1]*sqrt((maxima_.tr[1]/maxima_.count[1]-(maxima_.tr[1]/maxima_.count[1])*(maxima_.tr[1]/maxima_.count[1]))/maxima_.count[1]);

    sigma = sigma1+sigma2;
    dsigm = sqrt(dsigm1*dsigm1 + dsigm2*dsigm2);
  }
  // --- output --------------------------------------
  cout << "-------------------------------------------------------------" << endl;
  cout << "     PHOKHARA 7.0 Final Statistics                            " << endl;
  cout << "-------------------------------------------------------------"  << endl;
  cout <<  int(maxima_.tr[0]+maxima_.tr[1]) << " total events accepted of " << endl;
  cout <<  int(ievent)            <<  " total events generated" << endl;
  cout <<  int(maxima_.tr[0])    <<  " one photon events accepted of " << endl;
  cout <<  int(maxima_.count[0]) <<  " events generated" << endl;
  cout <<  int(maxima_.tr[1])    <<  " two photon events accepted of " << endl;
  cout <<  int(maxima_.count[1]) <<  " events generated" << endl;
  cout << endl;
  if (flags_.nlo != 0) cout <<  "sigma1(nbarn) = " << sigma1 << " +- " << dsigm1 << endl;
  if (flags_.nlo != 0) cout <<  "sigma2(nbarn) = " << sigma2 << " +- " << dsigm2 << endl;
  cout <<  "sigma (nbarn) = " << sigma << " +- " << dsigm << endl;
  cout << endl;
  cout <<  "maximum1 = " << maxima_.gross[0] << "  minimum1 = " << maxima_.klein[0] << endl;
  cout <<  "Mmax1    = " << maxima_.Mmax[0]  << endl;
  cout <<  "maximum2 = " << maxima_.gross[1] << "  minimum2 = " << maxima_.klein[1] << endl;
  cout <<  "Mmax2    = " << maxima_.Mmax[1]  << endl;
  cout <<  "-------------------------------------------------------------" << endl;


  int i;
  //the deletion of commands is really uggly!

  if (nphokharadecays==0) {
    delete [] commands;
    commands=0;
    return;
  }

  for(i=0;i<nphokharadecays;i++){
    if (phokharadecays[i]==this){
      phokharadecays[i]=phokharadecays[nphokharadecays-1];
      nphokharadecays--;
      if (nphokharadecays==0) {
	delete [] commands;
	commands=0;
      }
      return;
    }
  }

  report(ERROR,"EvtGen") << "Error in destroying Phokhara model!"<<endl;

}


void EvtPhokhara::getName(std::string& model_name){

  model_name="PHOKHARA";     

}

EvtDecayBase* EvtPhokhara::clone(){

  return new EvtPhokhara;

}


void EvtPhokhara::initProbMax(){

  noProbMax();

}


void EvtPhokhara::init_mode(EvtParticle* p){
 m_pion=getArg(0); 
 // mu+mu-(0),pi+pi-(1),2pi0pi+pi-(2),
 // 2pi+2pi-(3),ppbar(4),nnbar(5),
 // K+K-(6),K0K0bar(7),pi+pi-pi0(8), 
 // Lamb Lambbar->pi-pi+ppbar(9) 
  if(m_pion<0 || m_pion>9){std::cout<<"mode index for phokhar 0~9, but you give "<<m_pion<<std::endl;abort();}
  EvtId myvpho=EvtPDL::getId("vpho");
  m_E = p->mass();//EvtPDL::getMeanMass(myvpho);
///======== list parameters to be initialized
  m_tagged = 0;
  m_nm = 50000 ; // # of events to determine the maximum
  m_nlo = 1;     // Born(0), NLO(1)
  m_w = 0.0001;  // soft photon cutoff
  m_fsr = 1;     // ISR only(0), ISR+FSR(1), ISR+INT+FSR(2)
  m_fsrnlo = 1 ; // yes(1), no(0)
  m_NarrowRes = 1 ;// none(0) jpsi(1) psip(2)
  m_FF_Kaon = 1 ;  // constrained (0), unconstrained (1), Kuhn-Khodjamirian-Bruch (2)
  m_ivac = 0;      // yes(1), no(0)
  m_FF_Pion = 0 ;  // KS Pionformfactor(0), GS Pionformfactor old(1) and new(2)
  m_f0_model = 0 ; // f0+f0(600): KK model(0), no structure(1),  no f0+f0(600)(2), f0 KLOE(3)
  m_q2min = 0.0;   // minimal  hadrons(muons)-gamma-inv mass squared (GeV^2)
  m_q2_min_c = 0.0447 ; // minimal inv. mass squared of the hadrons(muons)(GeV^2)
  m_q2_max_c = m_E*m_E;    // maximal inv. mass squared of the hadrons(muons)(GeV^2)
  m_gmin = 0.001;       // minimal photon energy/missing energy   (GeV)
  m_phot1cut = 0.0;     // maximal photon angle/missing momentum angle (grad)
  m_phot2cut = 180.0;   // maximal photon angle/missing momentum angle (grad)
  m_pi1cut = 0.0 ;      // minimal hadrons(muons) angle (grad)
  m_pi2cut = 180.0;     // maximal hadrons(muons) angle (grad)

  if(!(m_pion==0 || m_pion==1 || m_pion==6)){m_fsr = 0;  m_fsrnlo = 0 ;}
  if( m_pion==9 ){m_nlo = 0 ;}
  // --- input parameter initialization -----------
  m_initSeed = 123456789;
  RLXDINIT(1, m_initSeed);
  maxima_.iprint = 0;
  flags_.nlo = m_nlo;
  flags_.pion = m_pion;
  flags_.fsr = m_fsr;
  flags_.fsrnlo = m_fsrnlo;
  flags_.ivac = m_ivac;
  flags_.FF_pion  = m_FF_Pion;
  flags_.f0_model = m_f0_model;
  flags_.FF_kaon  = m_FF_Kaon;
  flags_.narr_res = m_NarrowRes;

  ctes_.Sp = m_E*m_E;        ;

  cuts_.w = m_w;
  cuts_.q2min = m_q2min;
  cuts_.q2_min_c = m_q2_min_c;
  cuts_.q2_max_c = m_q2_max_c;
  cuts_.gmin = m_gmin;
  cuts_.phot1cut = m_phot1cut;
  cuts_.phot2cut = m_phot2cut;
  cuts_.pi1cut = m_pi1cut;
  cuts_.pi2cut = m_pi2cut;

  INPUT();

  // --- print run data ------------------------------
  cout << "-------------------------------------------------------------" << endl;
  if (flags_.pion == 0)
    cout << "     PHOKHARA 7.0 : e^+ e^- -> mu^+ mu^- gamma" << endl;
  else if (flags_.pion == 1)
    cout << "     PHOKHARA 7.0: e^+ e^- -> pi^+ pi^- gamma" << endl;
  else if (flags_.pion == 2)
    cout << "   PHOKHARA 7.0: e^+ e^- -> pi^+ pi^- 2pi^0 gamma" << endl;
  else if (flags_.pion == 3)
    cout << "   PHOKHARA 7.0: e^+ e^- -> 2pi^+ 2pi^- gamma" << endl;
  else if (flags_.pion == 4)
    cout << "   PHOKHARA 7.0: e^+ e^- -> p pbar gamma" << endl;
  else if (flags_.pion == 5)
    cout << "   PHOKHARA 7.0: e^+ e^- -> n nbar gamma" << endl;
  else if (flags_.pion == 6)
    cout << "   PHOKHARA 7.0: e^+ e^- -> K^+ K^- gamma" << endl;
  else if (flags_.pion == 7)
   cout << "   PHOKHARA 7.0: e^+ e^- -> K_0 K_0bar gamma" << endl;
  else if (flags_.pion == 8)
    cout << "   PHOKHARA 7.0: e^+ e^- -> pi^+ pi^- pi^0 gamma" << endl;
  else if (flags_.pion == 9) {
    cout <<  "PHOKHARA 7.0 : e^+ e^- ->" << endl;
    cout << "  Lambda (-> pi^- p) Lambda bar (-> pi^+ pbar) gamma" << endl;
  } else
    cout <<  "     PHOKHARA 7.0: not yet implemented" << endl;

  // --------------------------------
  cout <<  "--------------------------------------------------------------" << endl;
  printf(" %s %f %s\n","cms total energy                       = ",sqrt(ctes_.Sp)," GeV  ");
  
  //if((cuts_.gmin/2.0/ctes_.ebeam) < 0.000098){
  if(cuts_.gmin<0.001){
    cerr << " minimal missing energy set too small" << endl;
    abort();
  }
  printf(" %s %f %s\n","minimal tagged photon energy           = ",cuts_.gmin," GeV  ");
  printf(" %s %f,%f\n","angular cuts on tagged photon          = ",cuts_.phot1cut,cuts_.phot2cut);
  
  // --------------------------------
  if (flags_.pion == 0)
    printf(" %s %f,%f\n","angular cuts on muons                  = ",cuts_.pi1cut,cuts_.pi2cut);
  else if (flags_.pion == 4)
    printf(" %s %f,%f\n","angular cuts on protons                = ",cuts_.pi1cut,cuts_.pi2cut);
  else if (flags_.pion == 5)
    printf(" %s %f,%f\n","angular cuts on neutrons               = ", cuts_.pi1cut,cuts_.pi2cut);
  else if ((flags_.pion == 6)||(flags_.pion == 7))
    printf(" %s %f,%f\n","angular cuts on kaons                  = ", cuts_.pi1cut,cuts_.pi2cut);
  else if (flags_.pion == 9)
    printf(" %s %f,%f\n","angular cuts on pions and protons      = ", cuts_.pi1cut,cuts_.pi2cut);
  else
    printf(" %s %f,%f\n","angular cuts on pions                  = ", cuts_.pi1cut,cuts_.pi2cut);
    if (flags_.pion == 0)
      printf(" %s %f %s\n","min. muons-tagged photon inv.mass^2    = ", cuts_.q2min," GeV^2");
    else if (flags_.pion == 4)
      printf(" %s %f %s\n","min. protons-tagged photon inv.mass^2  = ", cuts_.q2min," GeV^2" );
    else if (flags_.pion == 5)
      printf(" %s %f %s\n","min. neutrons-tagged photon inv.mass^2 = ", cuts_.q2min," GeV^2" );
    else if ((flags_.pion == 6)||(flags_.pion == 7))
      printf(" %s %f %s\n","min. kaons-tagged photon inv.mass^2    = ", cuts_.q2min," GeV^2" );
    else if (flags_.pion == 9)
      printf(" %s %f %s\n"," min. lambdas-tagged photon inv.mass^2 = ", cuts_.q2min," GeV^2" );
    else
      printf(" %s %f %s\n","min. pions-tagged photon inv.mass^2    = ", cuts_.q2min," GeV^2" );
    cos1min = cos(cuts_.phot2cut*ctes_.pi/180.0);     // photon1 angle cuts in the 
    cos1max = cos(cuts_.phot1cut*ctes_.pi/180.0);     // LAB rest frame   
      cos2min = -1.0;                        // photon2 angle limits      
      cos2max =  1.0;                        //                           
      cos3min = -1.0;                        // hadrons/muons angle limits    
      cos3max =  1.0;                        // in their rest frame            
      if (flags_.pion == 0)                   // virtual photon energy cut 
        qqmin = 4.0*ctes_.mmu*ctes_.mmu;
      else if (flags_.pion == 1)
        qqmin = 4.0*ctes_.mpi*ctes_.mpi;
      else if (flags_.pion == 2)
        qqmin = 4.0*(ctes_.mpi+ctes_.mpi0)*(ctes_.mpi+ctes_.mpi0);
      else if (flags_.pion == 3)
        qqmin = 16.0*ctes_.mpi*ctes_.mpi;
      else if (flags_.pion == 4)
        qqmin = 4.0*ctes_.mp*ctes_.mp;
      else if (flags_.pion == 5)
        qqmin = 4.0*ctes_.mnt*ctes_.mnt;
      else if (flags_.pion == 6)
        qqmin = 4.0*ctes_.mKp*ctes_.mKp;
      else if (flags_.pion == 7)
        qqmin = 4.0*ctes_.mKn*ctes_.mKn;
      else if (flags_.pion == 8)
        qqmin = (2.0*ctes_.mpi+ctes_.mpi0)*(2.0*ctes_.mpi+ctes_.mpi0);
      else if (flags_.pion == 9)
        qqmin = 4.0*ctes_.mlamb*ctes_.mlamb;
      qqmax = ctes_.Sp-2.0*sqrt(ctes_.Sp)*cuts_.gmin;         // if only one photon 
      if (cuts_.q2_max_c < qqmax)
        qqmax=cuts_.q2_max_c;      // external cuts      

      // -------------------
      if ( (cuts_.q2_min_c > qqmin) && (cuts_.q2_min_c < (ctes_.Sp*(1.0-2.0*(cuts_.gmin/sqrt(ctes_.Sp)+cuts_.w))) ) )
        qqmin = cuts_.q2_min_c;
      else {
        cerr << "------------------------------" << endl;
        cerr << " Q^2_min TOO SMALL" << endl;
        cerr << " Q^2_min CHANGED BY PHOKHARA = " << qqmin << " GeV^2" << endl;
        cerr << "------------------------------" << endl;
      }
      // -------------------
      if(qqmax <= qqmin){
        cerr << " Q^2_max to small " << endl;
        cerr << " Q^2_max = " << qqmax << endl;
        cerr << " Q^2_min = " << qqmin << endl;
        abort();
      }

      // -------------------
      if (flags_.pion == 0) {
        printf(" %s %f %s\n", "minimal muon-pair invariant mass^2     = ", qqmin," GeV^2");
        printf(" %s %f %s\n", "maximal muon-pair invariant mass^2     = ", qqmax," GeV^2");
      } else if (flags_.pion == 1) {
        printf(" %s %f %s\n", "minimal pion-pair invariant mass^2     = ", qqmin," GeV^2");
        printf(" %s %f %s\n", "maximal pion-pair invariant mass^2     = ", qqmax," GeV^2");
      } else if (flags_.pion == 4) {
        printf(" %s %f %s\n", "minimal proton-pair invariant mass^2   = ", qqmin," GeV^2");
        printf(" %s %f %s\n", "maximal proton-pair invariant mass^2   = ", qqmax," GeV^2");
      } else if (flags_.pion == 5) {
        printf(" %s %f %s\n", "minimal neutron-pair invariant mass^2  = ", qqmin," GeV^2");
        printf(" %s %f %s\n", "maximal neutron-pair invariant mass^2  = ", qqmax," GeV^2");
      } else if ((flags_.pion == 6) || (flags_.pion == 7)) {
        printf(" %s %f %s\n", "minimal kaon-pair invariant mass^2     = ", qqmin," GeV^2");
        printf(" %s %f %s\n", "maximal kaon-pair invariant mass^2     = ", qqmax," GeV^2");
      } else if(flags_.pion == 8){
        printf(" %s %f %s\n", "minimal three-pion invariant mass^2    = ", qqmin," GeV^2");
        printf(" %s %f %s\n", "maximal three-pion invariant mass^2    = ", qqmax," GeV^2");
      } else if(flags_.pion == 9){
        printf(" %s %f %s\n", "minimal lambda-pair invariant mass^2  = ", qqmin," GeV^2");
        printf(" %s %f %s\n", "maximal lambda-pair invariant mass^2  = ", qqmax," GeV^2");
      } else {
        printf(" %s %f %s\n", "minimal four-pion invariant mass^2     = ", qqmin," GeV^2" );
        printf(" %s %f %s\n", "maximal four-pion invariant mass^2     = ", qqmax," GeV^2");
      }
      // -------------------
      if (flags_.nlo == 0) {
        cout <<  "Born" << endl;
         if(flags_.fsrnlo != 0){
           cerr << "WRONG FSRNLO flag - only fsrnlo=0 allowed for Born" << endl;
           abort();
         }
      }
      // -------------------
      if((flags_.pion == 9) && (flags_.nlo != 0)) {
        cerr << "WRONG NLO flag - only Born allowed for Lambdas"<< endl;
        cerr << "If you feel that you need NLO"<< endl;
        cerr << "please contact the authors"<< endl;
        abort();
      }
      // -------------------
      if (flags_.nlo == 1) printf(" %s %f\n", "NLO:    soft photon cutoff w           = ",cuts_.w);
      if ((flags_.pion <= 1) || (flags_.pion == 6))
        {

          if( ! ((flags_.fsr == 1) || (flags_.fsr == 2) || (flags_.fsrnlo == 0)
              || (flags_.fsr == 1) || (flags_.fsrnlo == 1)
                 || (flags_.fsr == 0) || (flags_.fsrnlo == 0))) {
            cerr << "WRONG combination of FSR, FSRNLO flags" <<endl;
	    abort();
          }
         // ------------------
          if (flags_.fsr == 0)
            cout << "ISR only" << endl;
          else if (flags_.fsr == 1)
            cout << "ISR+FSR" << endl;
          else if (flags_.fsr == 2) {
            if (flags_.nlo == 0)
              cout << "ISR+INT+FSR" << endl;
            else {
              cerr << "WRONG FSR flag: interference is included only for nlo=0" << endl;
              abort();
            }
          }
          else {
            cerr << "WRONG FSR flag" << flags_.fsr << endl;
            abort();
          }
          if(flags_.fsrnlo == 1)
            cout << "IFSNLO included" << endl;
        }
      else
        {
          if((flags_.fsr == 0) && (flags_.fsrnlo == 0))
            cout << "ISR only" << endl;
          else {
            cerr << "FSR is implemented only for pi+pi-, mu+mu- and K+K- modes" << endl;
            abort();
          }
        }
     // ------------------
      if(flags_.ivac == 0){
        cout << "Vacuum polarization is NOT included" << endl;}
        else if(flags_.ivac == 1){
        cout << "Vacuum polarization  by Fred Jegerlehner (http://www-com.physik.hu-berlin.de/fjeger/alphaQEDn.uu)"  << endl;}
        else if(flags_.ivac == 2){
        cout << "Vacuum polarization (VP_HLMNT_v1_3nonr) by Daisuke Nomura and Thomas Teubner" << endl;}
        else {
        cout << "WRONG vacuum polarization switch" << endl;
        abort();
        }

// -----------------
      if(flags_.pion == 1){
        if(flags_.FF_pion == 0)
          cout << "Kuhn-Santamaria PionFormFactor" << endl;
        else if(flags_.FF_pion == 1)
          cout << "Gounaris-Sakurai PionFormFactor old" << endl;
        else if(flags_.FF_pion == 2)
          cout << "Gounaris-Sakurai PionFormFactor new" << endl;
        else {
          cout << "WRONG PionFormFactor switch" << endl;
          abort();
        }
         if(flags_.fsr != 0){
         if(flags_.f0_model == 0)
           cout << "f0+f0(600): K+K- model" << endl;
         else if(flags_.f0_model == 1)
           cout << "f0+f0(600): \"no structure\" model" << endl;
         else if(flags_.f0_model == 2)
           cout << "NO f0+f0(600)" << endl;
         else if(flags_.f0_model == 3)
           cout << "only f0, KLOE: Cesare Bini-private communication" << endl;
         else {
           cout << "WRONG f0+f0(600) switch" << endl;
           abort();
         }
        }
      }
//-------
      if((flags_.pion == 6) || (flags_.pion==7)){
        if(flags_.FF_kaon == 0)
          cout << "constrained KaonFormFactor" << endl;
        else if(flags_.FF_kaon == 1) {
          cout << "unconstrained KaonFormFactor" << endl;}
        else if(flags_.FF_kaon == 2) {
          cout << "Kuhn-Khodjamirian-Bruch KaonFormFactor" << endl;}
        else{
          cout << "WRONG KaonFormFactor switch" << endl;
          abort();
        }
      }
// --------------------
     if((flags_.pion == 0) || (flags_.pion==1) || (flags_.pion == 6) || (flags_.pion == 7)){
        if(flags_.narr_res == 1){
          cout << "THE NARROW RESONANCE J/PSI INCLUDED" << endl;}
        else if(flags_.narr_res == 2){
          cout << "THE NARROW RESONANCE PSI(2S) INCLUDED" << endl;}
        else if(flags_.narr_res != 0){
          cout << "wrong flag narr_res: only 0, 1 or 2 allowed" << endl;
          abort();
        }
       }
// ------
  cout << "--------------------------------------------------------------" << endl;
//
 // =================================================
// --- finding the maximum -------------------------
      for(int i = 0; i<2; i++)
        {
          maxima_.Mmax[i] = 1.0;
          maxima_.gross[i] = 0.0;
          maxima_.klein[i] = 0.0;
        }

      if (flags_.nlo == 0)
        maxima_.Mmax[1]=0.0;   // only 1 photon events generated

      maxima_.tr[0] = 0.0;
      maxima_.tr[1] = 0.0;
      maxima_.count[0] = 0.0;
      maxima_.count[1] = 0.0;

      // =================================================
      // --- beginning the MC loop event generation ------
      for(int j = 1; j <= m_nm; j++)
        {
          RANLXDF(Ar_r,1);
          Ar[1] = Ar_r[0];
          if (Ar[1] <= (maxima_.Mmax[0]/(maxima_.Mmax[0]+maxima_.Mmax[1]))) {
            maxima_.count[0] = maxima_.count[0]+1.0;
            GEN_1PH(1,qqmin,qqmax,cos1min,cos1max,cos3min,cos3max);
          }
          else {
            maxima_.count[1] = maxima_.count[1]+1.0;
            GEN_2PH(1,qqmin,cos1min,cos1max,cos2min,cos2max,cos3min,cos3max);
          }
        }
      // --- end of the MC loop --------------------------
      // =================================================
      // --- for the second run ---
      maxima_.Mmax[0] = maxima_.gross[0]+.05*sqrt(maxima_.gross[0]*maxima_.gross[0]);
      maxima_.Mmax[1] = maxima_.gross[1]+(.03+.02*ctes_.Sp)*sqrt(maxima_.gross[1]*maxima_.gross[1]);
      theMmax[m_pion][0]=maxima_.Mmax[0];
      theMmax[m_pion][1]=maxima_.Mmax[1];

      if((flags_.pion == 1) && (flags_.fsrnlo == 1))
        maxima_.Mmax[1]=maxima_.Mmax[1]*1.5;
      if((flags_.pion == 0) && (flags_.fsrnlo == 1))
        maxima_.Mmax[1]=maxima_.Mmax[1]*1.5;

      if((flags_.pion == 0) && (flags_.fsr == 1) && (flags_.fsrnlo == 0))
        maxima_.Mmax[1]=maxima_.Mmax[1]*1.2;

      if((flags_.pion == 2) || (flags_.pion == 3)){
        maxima_.Mmax[0]=maxima_.Mmax[0]*1.1;
        maxima_.Mmax[1]=maxima_.Mmax[1]*1.1;
      }

      if(flags_.pion == 8){
        maxima_.Mmax[0]=maxima_.Mmax[0]*1.08;
        maxima_.Mmax[1]=maxima_.Mmax[1]*1.1;
      }
// --- end of the second run -----------------------

      maxima_.tr[0] = 0.0;
      maxima_.tr[1] = 0.0;
      maxima_.count[0] = 0.0;
      maxima_.count[1] = 0.0;
}



void EvtPhokhara::init(){
  checkNDaug(0);
  checkNArg(1);
  nevtgen.resize(10);
  theMmax.resize(10);
  for(int i=0;i<=10;i++){ theMmax[i].resize(2); nevtgen[i]=0;}

  Vfs.push_back(" mu+mu-");
  Vfs.push_back(" pi+pi-");
  Vfs.push_back(" 2pi0pi+pi-");
  Vfs.push_back(" 2pi+2pi-");
  Vfs.push_back(" ppbar");
  Vfs.push_back(" nnbar");
  Vfs.push_back(" K+K-");
  Vfs.push_back(" K0K0bar");
  Vfs.push_back(" pi+pi-pi0");
  Vfs.push_back(" Lambda Lambdabar");

  std::string locvp=getenv("BESEVTGENROOT");
  system("cat $BESEVTGENROOT/share/phokhara.param>phokhara.param");

  
  if (getParentId().isAlias()){
    
    report(ERROR,"EvtGen") << "EvtPhokhara finds that you are decaying the"<<endl
			   << " aliased particle "
			   << EvtPDL::name(getParentId()).c_str()
			   << " with the Phokhara model"<<endl
			   << " this does not work, please modify decay table."
			   << endl;
    report(ERROR,"EvtGen") << "Will terminate execution!"<<endl;
    ::abort();

  }

  //store(this);

}


std::string EvtPhokhara::commandName(){
     
  return std::string("PhokharaPar");
  
}

void EvtPhokhara::command(std::string cmd){

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



void EvtPhokhara::decay( EvtParticle *p){
  EvtId myvpho=EvtPDL::getId("vpho");
  if(p->getId()!=myvpho) {std::cout<<"Parent particle is required to be vpho for Phokhara model"<<std::endl;abort();}
  m_pion=getArg(0);
  if(nevtgen[m_pion]==0) {init_mode(p);}
  else{init_evt(p);}
  std::cout<<"PHOKHARA : "<<Vfs[m_pion]<<" mode "<<std::endl;

  int istdheppar=EvtPDL::getStdHep(p->getId());
  int ntrials = 0;
  int tr_old[2];
  tr_old[0] = (int)maxima_.tr[0];
  tr_old[1] = (int)maxima_.tr[1];

  while( ntrials < 10000)
    {
      ievent++;
      RANLXDF(Ar_r,1);
      Ar[1] = Ar_r[0];
      
      if (Ar[1] <= (maxima_.Mmax[0]/(maxima_.Mmax[0]+maxima_.Mmax[1]))) {
        maxima_.count[0] = maxima_.count[0]+1.0;
        GEN_1PH(2,qqmin,qqmax,cos1min,cos1max,cos3min,cos3max);
      }
      else {
        maxima_.count[1] = maxima_.count[1]+1.0;
        GEN_2PH(2,qqmin,cos1min,cos1max,cos2min,cos2max,cos3min,cos3max);
      }
      
      if( ((int)maxima_.tr[0]+(int)maxima_.tr[1]) > (tr_old[0]+tr_old[1]) ) // event accepted after cuts
        {
          goto storedEvents;
        }
      ntrials ++;
    }
  
  std::cout <<"FATAL: Could not satisfy cuts after " << ntrials << "trials. Terminate." <<std::endl;
  //----
 storedEvents:
  int more=0;
  int numstable=0;
  int numparton=0;
  EvtId evtnumstable[100];//
  EvtVector4R p4[20];

  // except ISR photos, products depending on channel
  if (flags_.pion == 0) { // mu+ mu-
    // mu+
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(-13);
    p4[numstable].set(ctes_.momenta[0][5],ctes_.momenta[1][5], ctes_.momenta[2][5], ctes_.momenta[3][5]);     
    numstable++;
    // mu -
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(13);
    p4[numstable].set(ctes_.momenta[0][6],ctes_.momenta[1][6], ctes_.momenta[2][6], ctes_.momenta[3][6]);     
    numstable++;
  }
  if (flags_.pion == 1) { // pi+ pi-
    // pi+
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(211);
    p4[numstable].set(ctes_.momenta[0][5],ctes_.momenta[1][5], ctes_.momenta[2][5], ctes_.momenta[3][5]);     
    numstable++;
    // pi -
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(-211);
    p4[numstable].set(ctes_.momenta[0][6],ctes_.momenta[1][6], ctes_.momenta[2][6], ctes_.momenta[3][6]);     
    numstable++;
  }
  if (flags_.pion == 2) { // pi+ pi-2pi0
    // pi0
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(111);
    p4[numstable].set(ctes_.momenta[0][5],ctes_.momenta[1][5], ctes_.momenta[2][5], ctes_.momenta[3][5]);     
    numstable++;
    // pi0
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(111);
    p4[numstable].set(ctes_.momenta[0][6],ctes_.momenta[1][6], ctes_.momenta[2][6], ctes_.momenta[3][6]);     
    numstable++;
    // pi-
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(-211);
    p4[numstable].set(ctes_.momenta[0][7],ctes_.momenta[1][7], ctes_.momenta[2][7], ctes_.momenta[3][7]);     
    numstable++;
    // pi +
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(211);
    p4[numstable].set(ctes_.momenta[0][8],ctes_.momenta[1][8], ctes_.momenta[2][8], ctes_.momenta[3][8]);     
    numstable++;
  }   
  if (flags_.pion == 3) { // 2(pi+ pi-)
    // pi+
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(211);
    p4[numstable].set(ctes_.momenta[0][5],ctes_.momenta[1][5], ctes_.momenta[2][5], ctes_.momenta[3][5]);     
    numstable++;
    // pi-
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(-211);
    p4[numstable].set(ctes_.momenta[0][6],ctes_.momenta[1][6], ctes_.momenta[2][6], ctes_.momenta[3][6]);     
    numstable++;
    // pi+
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(-211);
    p4[numstable].set(ctes_.momenta[0][7],ctes_.momenta[1][7], ctes_.momenta[2][7], ctes_.momenta[3][7]);     
    numstable++;
    // pi -
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(211);
    p4[numstable].set(ctes_.momenta[0][8],ctes_.momenta[1][8], ctes_.momenta[2][8], ctes_.momenta[3][8]);     
    numstable++;
  }   
  if (flags_.pion == 4) { // ppbar
    // pbar
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(-2212);
    p4[numstable].set(ctes_.momenta[0][5],ctes_.momenta[1][5], ctes_.momenta[2][5], ctes_.momenta[3][5]);     
    numstable++;
    // p
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(2212);
    p4[numstable].set(ctes_.momenta[0][6],ctes_.momenta[1][6], ctes_.momenta[2][6], ctes_.momenta[3][6]);     
    numstable++;
  }
  if (flags_.pion == 5) { // nnbar
    // pbar
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(-2112);
    p4[numstable].set(ctes_.momenta[0][5],ctes_.momenta[1][5], ctes_.momenta[2][5], ctes_.momenta[3][5]);     
    numstable++;
    // p
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(2112);
    p4[numstable].set(ctes_.momenta[0][6],ctes_.momenta[1][6], ctes_.momenta[2][6], ctes_.momenta[3][6]);     
    numstable++;
  }
  if (flags_.pion == 6) { // K+ K-
    // K+
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(321);
    p4[numstable].set(ctes_.momenta[0][5],ctes_.momenta[1][5], ctes_.momenta[2][5], ctes_.momenta[3][5]);     
    numstable++;
    // K -
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(-321);
    p4[numstable].set(ctes_.momenta[0][6],ctes_.momenta[1][6], ctes_.momenta[2][6], ctes_.momenta[3][6]);     
    numstable++;
  }
  if (flags_.pion == 7) { // K0K0bar
    // Kbar
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(310);
    p4[numstable].set(ctes_.momenta[0][5],ctes_.momenta[1][5], ctes_.momenta[2][5], ctes_.momenta[3][5]);     
    numstable++;
    // K0
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(130);
    p4[numstable].set(ctes_.momenta[0][6],ctes_.momenta[1][6], ctes_.momenta[2][6], ctes_.momenta[3][6]);     
    numstable++;
  }
  if (flags_.pion == 8) { // pi+ pi-pi0
    // pi+
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(211);
    p4[numstable].set(ctes_.momenta[0][5],ctes_.momenta[1][5], ctes_.momenta[2][5], ctes_.momenta[3][5]);     
    numstable++;
    // pi-
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(-211);
    p4[numstable].set(ctes_.momenta[0][6],ctes_.momenta[1][6], ctes_.momenta[2][6], ctes_.momenta[3][6]);     
    numstable++;
    // pi0
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(111);
    p4[numstable].set(ctes_.momenta[0][7],ctes_.momenta[1][7], ctes_.momenta[2][7], ctes_.momenta[3][7]);     
    numstable++;
  } 
  if (flags_.pion == 9) { //Lambda Lambdabar-> pi+ pi- ppbar
    // pi+
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(211);
    p4[numstable].set(ctes_.momenta[0][7],ctes_.momenta[1][7], ctes_.momenta[2][7], ctes_.momenta[3][7]);     
    numstable++;
    // pbar
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(-2212);
    p4[numstable].set(ctes_.momenta[0][8],ctes_.momenta[1][8], ctes_.momenta[2][8], ctes_.momenta[3][8]);     
    numstable++;
    // pi-
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(-211);
    p4[numstable].set(ctes_.momenta[0][9],ctes_.momenta[1][9], ctes_.momenta[2][9], ctes_.momenta[3][9]);     
    numstable++; 
    // p
    evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(2212);
    p4[numstable].set(ctes_.momenta[0][10],ctes_.momenta[1][10], ctes_.momenta[2][10], ctes_.momenta[3][10]);     
    numstable++;
  }   
  
 // ISR gamma      
  evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(22);
  p4[numstable].set(ctes_.momenta[0][2],ctes_.momenta[1][2], ctes_.momenta[2][2], ctes_.momenta[3][2]);     
  numstable++;
  if( ctes_.momenta[0][3] != 0 ) // second photon exists
    {
      evtnumstable[numstable]=EvtPDL::evtIdFromStdHep(22);
      p4[numstable].set(ctes_.momenta[0][3],ctes_.momenta[1][3], ctes_.momenta[2][3], ctes_.momenta[3][3]);     
      numstable++;
    }
  
  int channel=EvtDecayTable::inChannelList(p->getId(),numstable,evtnumstable);
  more=(channel!=-1);
  if(more) {std::cout<<"Existence of mode "<<channel<<" in exclusive decay list has the same final state as this one"<<std::endl;abort(); }
  
  p->makeDaughters(numstable,evtnumstable);

  int ndaugFound=0;
  EvtVector4R SUMP4(0,0,0,0);
  for(int i=0;i<numstable;i++){
    p->getDaug(i)->init(evtnumstable[i],p4[i]);
    ndaugFound++;
  }
  if ( ndaugFound == 0 ) {
    report(ERROR,"EvtGen") << "Phokhara has failed to do a decay ";
    report(ERROR,"EvtGen") << EvtPDL::name(p->getId()).c_str() << " " << p->mass()<<endl;
    assert(0);
  }

  nevtgen[m_pion]++;  
  return ;
  
}



void EvtPhokhara::store(EvtDecayBase* jsdecay){

  if (nphokharadecays==ntable){

    EvtDecayBasePtr* newphokharadecays=new EvtDecayBasePtr[2*ntable+10];
    int i;
    for(i=0;i<ntable;i++){
      newphokharadecays[i]=phokharadecays[i];
    }
    ntable=2*ntable+10;
    delete [] phokharadecays;
    phokharadecays=newphokharadecays;
  }

  phokharadecays[nphokharadecays++]=jsdecay;



}


void EvtPhokhara::PhokharaInit(int dummy){
  static int first=1;
  if (first){
    
    first=0;
    //for(int i=0;i<ncommand;i++)
    // lugive0_(commands[i].c_str(),strlen(commands[i].c_str()));
  }
  
}



void EvtPhokhara::init_evt(EvtParticle* p){
 m_pion=getArg(0); 
 // mu+mu-(0),pi+pi-(1),2pi0pi+pi-(2),
 // 2pi+2pi-(3),ppbar(4),nnbar(5),
 // K+K-(6),K0K0bar(7),pi+pi-pi0(8), 
 // Lamb Lambbar->pi-pi+ppbar(9) 
  if(m_pion<0 || m_pion>9){std::cout<<"mode index for phokhar 0~9, but you give "<<m_pion<<std::endl;abort();}
  EvtId myvpho=EvtPDL::getId("vpho");
  m_E = p->mass();//EvtPDL::getMeanMass(myvpho);
///======== list parameters to be initialized
  m_tagged = 0;
  m_nm = 50000 ; // # of events to determine the maximum
  m_nlo = 1;     // Born(0), NLO(1)
  m_w = 0.0001;  // soft photon cutoff
  m_fsr = 1;     // ISR only(0), ISR+FSR(1), ISR+INT+FSR(2)
  m_fsrnlo = 1 ; // yes(1), no(0)
  m_NarrowRes = 1 ;// none(0) jpsi(1) psip(2)
  m_FF_Kaon = 1 ;  // constrained (0), unconstrained (1), Kuhn-Khodjamirian-Bruch (2)
  m_ivac = 0;      // yes(1), no(0)
  m_FF_Pion = 0 ;  // KS Pionformfactor(0), GS Pionformfactor old(1) and new(2)
  m_f0_model = 0 ; // f0+f0(600): KK model(0), no structure(1),  no f0+f0(600)(2), f0 KLOE(3)
  m_q2min = 0.0;   // minimal  hadrons(muons)-gamma-inv mass squared (GeV^2)
  m_q2_min_c = 0.0447 ; // minimal inv. mass squared of the hadrons(muons)(GeV^2)
  m_q2_max_c = m_E*m_E;    // maximal inv. mass squared of the hadrons(muons)(GeV^2)
  m_gmin = 0.001;       // minimal photon energy/missing energy   (GeV)
  m_phot1cut = 0.0;     // maximal photon angle/missing momentum angle (grad)
  m_phot2cut = 180.0;   // maximal photon angle/missing momentum angle (grad)
  m_pi1cut = 0.0 ;      // minimal hadrons(muons) angle (grad)
  m_pi2cut = 180.0;     // maximal hadrons(muons) angle (grad)

  if(!(m_pion==0 || m_pion==1 || m_pion==6)){m_fsr = 0;  m_fsrnlo = 0 ;}
  if( m_pion==9 ){m_nlo = 0 ;}
  // --- input parameter initialization -----------
  maxima_.iprint = 0;
  flags_.nlo = m_nlo;
  flags_.pion = m_pion;
  flags_.fsr = m_fsr;
  flags_.fsrnlo = m_fsrnlo;
  flags_.ivac = m_ivac;
  flags_.FF_pion  = m_FF_Pion;
  flags_.f0_model = m_f0_model;
  flags_.FF_kaon  = m_FF_Kaon;
  flags_.narr_res = m_NarrowRes;

  ctes_.Sp = m_E*m_E;        ;

  cuts_.w = m_w;
  cuts_.q2min = m_q2min;
  cuts_.q2_min_c = m_q2_min_c;
  cuts_.q2_max_c = m_q2_max_c;
  cuts_.gmin = m_gmin;
  cuts_.phot1cut = m_phot1cut;
  cuts_.phot2cut = m_phot2cut;
  cuts_.pi1cut = m_pi1cut;
  cuts_.pi2cut = m_pi2cut;

  INPUT();

    cos1min = cos(cuts_.phot2cut*ctes_.pi/180.0);     // photon1 angle cuts in the 
    cos1max = cos(cuts_.phot1cut*ctes_.pi/180.0);     // LAB rest frame   
      cos2min = -1.0;                        // photon2 angle limits      
      cos2max =  1.0;                        //                           
      cos3min = -1.0;                        // hadrons/muons angle limits    
      cos3max =  1.0;                        // in their rest frame            
      if (flags_.pion == 0)                   // virtual photon energy cut 
        qqmin = 4.0*ctes_.mmu*ctes_.mmu;
      else if (flags_.pion == 1)
        qqmin = 4.0*ctes_.mpi*ctes_.mpi;
      else if (flags_.pion == 2)
        qqmin = 4.0*(ctes_.mpi+ctes_.mpi0)*(ctes_.mpi+ctes_.mpi0);
      else if (flags_.pion == 3)
        qqmin = 16.0*ctes_.mpi*ctes_.mpi;
      else if (flags_.pion == 4)
        qqmin = 4.0*ctes_.mp*ctes_.mp;
      else if (flags_.pion == 5)
        qqmin = 4.0*ctes_.mnt*ctes_.mnt;
      else if (flags_.pion == 6)
        qqmin = 4.0*ctes_.mKp*ctes_.mKp;
      else if (flags_.pion == 7)
        qqmin = 4.0*ctes_.mKn*ctes_.mKn;
      else if (flags_.pion == 8)
        qqmin = (2.0*ctes_.mpi+ctes_.mpi0)*(2.0*ctes_.mpi+ctes_.mpi0);
      else if (flags_.pion == 9)
        qqmin = 4.0*ctes_.mlamb*ctes_.mlamb;
      qqmax = ctes_.Sp-2.0*sqrt(ctes_.Sp)*cuts_.gmin;         // if only one photon 
      if (cuts_.q2_max_c < qqmax)
        qqmax=cuts_.q2_max_c;      // external cuts      

      // -------------------
      if ( (cuts_.q2_min_c > qqmin) && (cuts_.q2_min_c < (ctes_.Sp*(1.0-2.0*(cuts_.gmin/sqrt(ctes_.Sp)+cuts_.w))) ) )
        qqmin = cuts_.q2_min_c;
      else {
      }


 // =================================================
// --- finding the maximum -------------------------
      for(int i = 0; i<2; i++)
        {
          maxima_.Mmax[i] = 1.0;
          maxima_.gross[i] = 0.0;
          maxima_.klein[i] = 0.0;
        }

      if (flags_.nlo == 0)
        maxima_.Mmax[1]=0.0;   // only 1 photon events generated

      maxima_.tr[0] = 0.0;
      maxima_.tr[1] = 0.0;
      maxima_.count[0] = 0.0;
      maxima_.count[1] = 0.0;

      // =================================================
      // --- for the second run ---
      maxima_.Mmax[0] = theMmax[m_pion][0];
      maxima_.Mmax[1] = theMmax[m_pion][1]; 
      if((flags_.pion == 1) && (flags_.fsrnlo == 1))
        maxima_.Mmax[1]=maxima_.Mmax[1]*1.5;
      if((flags_.pion == 0) && (flags_.fsrnlo == 1))
        maxima_.Mmax[1]=maxima_.Mmax[1]*1.5;

      if((flags_.pion == 0) && (flags_.fsr == 1) && (flags_.fsrnlo == 0))
        maxima_.Mmax[1]=maxima_.Mmax[1]*1.2;

      if((flags_.pion == 2) || (flags_.pion == 3)){
        maxima_.Mmax[0]=maxima_.Mmax[0]*1.1;
        maxima_.Mmax[1]=maxima_.Mmax[1]*1.1;
      }

      if(flags_.pion == 8){
        maxima_.Mmax[0]=maxima_.Mmax[0]*1.08;
        maxima_.Mmax[1]=maxima_.Mmax[1]*1.1;
      }
// --- end of the second run -----------------------

      maxima_.tr[0] = 0.0;
      maxima_.tr[1] = 0.0;
      maxima_.count[0] = 0.0;
      maxima_.count[1] = 0.0;
}

