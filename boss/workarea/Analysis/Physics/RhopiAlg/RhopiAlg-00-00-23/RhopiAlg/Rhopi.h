#ifndef Physics_Analysis_Rhopi_H
#define Physics_Analysis_Rhopi_H 

#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/NTuple.h"
//#include "VertexFit/ReadBeamParFromDb.h"


class Rhopi : public Algorithm {

public:
  Rhopi(const std::string& name, ISvcLocator* pSvcLocator);
  StatusCode initialize();
  StatusCode execute();
  StatusCode finalize();  

private:

  //ReadBeamParFromDb fReader;
  // Declare r0, z0 cut for charged tracks
  double fVr0cut;
  double fVz0cut;

  //Declare energy, dphi, dthe cuts for fake gamma's
  double fEnergyThreshold;
  double fGammaPhiCut;
  double fGammaThetaCut;
  double fGammaAngleCut;

  // 
  int fElectronst4C;
  int fElectronst5C;

  // 
  int fCheckDedx;
  int fCheckTof;

  // define Ntuples here

  NTuple::Tuple*  m_tuple1;      // charged track vertex
  NTuple::Item<double>  fVx0;
  NTuple::Item<double>  fVy0;
  NTuple::Item<double>  fVz0;
  NTuple::Item<double>  fVr0;
  NTuple::Item<double>  fRvxy0;
  NTuple::Item<double>  fRvz0;
  NTuple::Item<double>  fRvphi0;

  NTuple::Tuple*  m_tuple2;      // fake photon
  NTuple::Item<double>  fDeltaTheta;
  NTuple::Item<double>  fDeltaPhi;
  NTuple::Item<double>  fDeltaAngle;
  NTuple::Item<double>  fEraw;

  NTuple::Tuple*  m_tuple3;     // rhopi: raw mgg, etot
  NTuple::Item<double>  fMtoGG;
  NTuple::Item<double>  fEtot;

  NTuple::Tuple*  m_tuple4;     // rhopi 4C
  NTuple::Item<double>  fChi1;
  NTuple::Item<double>  fMpi0;

  NTuple::Tuple*  m_tuple5;     // rhopi 5C
  NTuple::Item<double>  fChi2;
  NTuple::Item<double>  fM_rh0;
  NTuple::Item<double>  fM_rhp;
  NTuple::Item<double>  fM_rhm;

  NTuple::Tuple*  m_tuple6;    // photons
  NTuple::Item<double>  fFcos;
  NTuple::Item<double>  fElow;

  NTuple::Tuple* m_tuple7;    // dE/dx
  NTuple::Item<double> fPtrack;
  NTuple::Item<double> fChi2e;
  NTuple::Item<double> fChi2mu;
  NTuple::Item<double> fChi2pi;
  NTuple::Item<double> fChi2k;
  NTuple::Item<double> fChi2p;
  NTuple::Item<double> fProbPH;
  NTuple::Item<double> fNormPH;
  NTuple::Item<double> fGhit;
  NTuple::Item<double> fThit;

  NTuple::Tuple* m_tuple8;   // endcap tof
  NTuple::Item<double> fPtotTofEC;
  NTuple::Item<double> fCntrTofEC;
  NTuple::Item<double> fElectronTofEC;
  NTuple::Item<double> fMuonTofEC;
  NTuple::Item<double> fProtoniTofEC;
  NTuple::Item<double> fKaonTofEC;
  NTuple::Item<double> fProtonTofEC;
  NTuple::Item<double> fPhTofEC;
  NTuple::Item<double> fRhitTofEC;
  NTuple::Item<double> fQualTofEC;

  NTuple::Tuple* m_tuple9;  // barrel inner tof
  NTuple::Item<double> fPtotTofIB;
  NTuple::Item<double> fCntrTofIB;
  NTuple::Item<double> fElectronTofIB;
  NTuple::Item<double> fMuonTofIB;
  NTuple::Item<double> fProtoniTofIB;
  NTuple::Item<double> fKaonTofIB;
  NTuple::Item<double> fProtonTofIB;
  NTuple::Item<double> fPhTofIB;
  NTuple::Item<double> fZhitTofIB;
  NTuple::Item<double> fQualTofIB;

  NTuple::Tuple* m_tuple10;  // barrel outer tof
  NTuple::Item<double> fPtotTofOB;
  NTuple::Item<double> fCntrTofOB;
  NTuple::Item<double> fElectronTofOB;
  NTuple::Item<double> fMuonTofOB;
  NTuple::Item<double> fProtoniTofOB;
  NTuple::Item<double> fKaonTofOB;
  NTuple::Item<double> fProtonTofOB;
  NTuple::Item<double> fPhTofOB;
  NTuple::Item<double> fZhitTofOB;
  NTuple::Item<double> fQualTofOB;
  
  NTuple::Tuple* m_tuple11;  // Particle ID info.
  NTuple::Item<double> fPtrackPID;
  NTuple::Item<double> fCostPID;
  NTuple::Item<double> fDedxPID;
  NTuple::Item<double> fTof1PID;
  NTuple::Item<double> fTof2PID;
  NTuple::Item<double> fProbPID;

};

#endif 
