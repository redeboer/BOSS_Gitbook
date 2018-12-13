//*****************************************************************************
//
// Generators/EvtGen_i/EvtDecay.h
//
// This module takes HepMC events from the transient store and decays
// particles including the secondaries unstable particles
// in EvtGen.
// It is a TopAlg can be used in conjunction Pythia KKMC
//   and SingleParticleGun 
//
// October 2005 adapted to be used in BES3 by A.Zhemchugov
// November 2003: Maria Smizanska 
// August 2002: Malte Muller,   maltemlr@yahoo.co.uk
// Original LHCb code by Witold Pokorski
//
//*****************************************************************************

#ifndef GENERATORMODULESEVTDECAY_H
#define GENERATORMODULESEVTDECAY_H

#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"

#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtDecayBase.hh"
#include "EvtGenBase/EvtDecayTable.hh"
#include "EvtGen.hh"
#include "EvtGenBase/EvtRandomEngine.hh"

#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/NTuple.h"

#include "CLHEP/Random/Ranlux64Engine.h"
#include <stdio.h>
#include <vector>
#include "EvtGenBase/EvtRandom.hh"
#include "EvtGenBase/EvtHis2F.hh"
#include "EvtGenBase/EvtId.hh"

#include "DataInfoSvc/IDataInfoSvc.h"
#include "DataInfoSvc/DataInfoSvc.h"

//#include "TFile.h"
//#include "TTree.h"
//#include "GaudiKernel/NTuple.h"

class IBesRndmGenSvc;

using namespace std;
using namespace CLHEP;

class EvtBesRandom : public EvtRandomEngine
{
public:
 EvtBesRandom(HepRandomEngine* engine);
 double random();
 virtual ~EvtBesRandom();

	
private:
 HepRandomEngine* m_engine;
};

class EvtDecay:public Algorithm 
{	
public:
 EvtDecay(const string& name, ISvcLocator* pSvcLocator);
		
 StatusCode initialize();
 StatusCode execute();
 StatusCode finalize();
 //--
  IDataInfoSvc *tmpInfoSvc;
  DataInfoSvc* dataInfoSvc;

double ampsLenu(vector<double> Vexp,std::vector<double> vpars);
double ampsLbenu(vector<double> Vexp,std::vector<double> vpars);
double HV(double i,double j,vector<double> Vexp, std::vector<double> vpars);
double HA(double i,double j,vector<double> Vexp, std::vector<double> vpars);
double getObsXsection(double mhds,int mode);
double energySpread(double mu,double sigma);


private:
 StatusCode callEvtGen( HepMC::GenEvent* hepMCevt );
 StatusCode callBesEvtGen( HepMC::GenEvent* hepMCevt );
 StatusCode makeHepMC(EvtParticle*, HepMC::GenEvent*, HepMC::GenParticle*);

 double CalAmpsMax( EvtParticle* part );
 double CalAmpsMDIY(EvtParticle* part );

 void MeVToGeV(HepMC::GenEvent* hepMCevt);
 void GeVToMeV(HepMC::GenEvent* hepMCevt);
      
 void ReadTruth(EvtParticle* part,std::vector<std::vector<string> > mylist);
 // data members
 int m_targetID;
 int isNumber(double d);        	
 double m_SetMthr;
//for EvtGen interface
 EvtGen* m_Gen;
 EvtBesRandom* m_RandomEngine;
 
// Local Member Data:-
 IBesRndmGenSvc* p_BesRndmGenSvc;
 vector<long int> m_seeds;
    

// jobOption params
string m_DecayDec, m_PdtTable, userDecFileName,m_DecayTop,m_DecayRec, m_ParentPart;

 std::ofstream outfile,outfile2,truth;
 std::vector<int> m_InSeeds;

 bool _mDIY,m_RdMeasuredEcms;
 double dbEcms, m_beamEnergySpread;
 bool m_ampscalflag;
 bool m_Ncharge,m_NtupleFile, m_Psi4040OpenCharm, m_Psi2openCharm, m_statDecays;
 int m_numberEvent;
 int AllTrk_index,Trk_index[500];
 double px_trk[500];
 double py_trk[500];
 double pz_trk[500];
 double en_trk[500];
 // --- SuperBody3decay
 void SuperBody3decay_make(EvtId ppid,  EvtVector4R p_init);
 bool SuperBody3decay_judge(EvtParticle* part);
 string m_SB3File, m_SB3HT;
 EvtHis2F SuperBody3decay;
 bool first;
 int parentPDGcode;
 double parentMass;
 
 int pdg0,pdg1,pdg2,pdg,multi;
 EvtVector4R son0,son1,son2,son;
 void FinalState_make(EvtId ppid,  EvtVector4R p_init);
 void FinalState_sort(EvtParticle* part);
 void assign_momentum(int pdx, EvtVector4R pv4);
 void assign_momentum2(int pdx, EvtVector4R pv4); // for two identical particle
 bool identical_flag;
 string m_FDPparticle;
 EvtVector4R FDP_init;
 EvtId FDP_id;
 EvtParticle* FDP_part;
 void findPart(EvtParticle* part);
 void countChannel(EvtParticle* part);
 int br[500],vbr[500];
 int totalChannels;
 bool isCharmonium(EvtId xid);
 bool isCharm(EvtId xid);
 bool isRadecay(EvtParticle* par);
 string m_outputp4;
 int decayType(EvtParticle *par);
 std::string getModel(EvtParticle* par, int mode);
 std::string getModel(EvtParticle* par);
 bool m_tagLundModel,_RvalueTag;

 std::vector<std::string> m_mystring;
 std::vector<double>  m_wind0,m_wind1,m_wind2;
 std::vector<int>  m_cluster0,m_cluster1,m_cluster2;
 static int m_runNo;
 std::vector<std::vector<std::string> > m_ReadTruth;

 double m_eBeamPolarization;
 int writeFlag;
 EvtId m_KKMCRes;
 std::vector<double> m_polarization;
 std::string m_truthFile,m_truthPart;
// for Ntuple
NTuple::Tuple*  m_tuple;
NTuple::Item<int> TotNumTrk;
NTuple::Item<int> m_nchr;
NTuple::Item<int> m_nchr_e;
NTuple::Item<int> m_nchr_mu;
NTuple::Item<int> m_nchr_pi;
NTuple::Item<int> m_nchr_k;
NTuple::Item<int> m_nchr_p;  
NTuple::Item<int> m_gamma; 
NTuple::Item<int> m_gammaFSR;
NTuple::Item<int> m_flag1;

NTuple::Array<int> m_Trk_index;  
NTuple::Array<int> m_fst; 
NTuple::Array<double> m_px_trk;  
NTuple::Array<double> m_py_trk; 
NTuple::Array<double> m_pz_trk; 
NTuple::Array<double> m_en_trk; 

NTuple::Tuple*  mass_tuple;	
NTuple::Item<double>  m_m12;   
NTuple::Item<double>  m_m13;
NTuple::Item<double>  m_m23;
NTuple::Item<double>  m_m1;   
NTuple::Item<double>  m_m2;
NTuple::Item<double>  m_m3;
NTuple::Item<double>  m_cos1;   
NTuple::Item<double>  m_cos2;
NTuple::Item<double>  m_cos3;
NTuple::Item<int> m_ich;
NTuple::Tuple*  massgen_tuple;
NTuple::Item<double>   _m12;   
NTuple::Item<double>   _m13;
NTuple::Item<double>   _m23;
NTuple::Item<double>   _m1;   
NTuple::Item<double>   _m2;
NTuple::Item<double>   _m3;
NTuple::Item<double>   _cos1;   
NTuple::Item<double>   _cos2;
NTuple::Item<double>   _cos3;
NTuple::Item<int> _ich;
/////////////
};

#endif
