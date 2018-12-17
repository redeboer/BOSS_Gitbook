#ifndef Physics_Analysis_DzeroOmega_H
#define Physics_Analysis_DzeroOmega_H

/**
 * @brief    Analyse \f$ J/\psi \rightarrow D^0 \phi \f$ events.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     December 17th, 2018
 */



// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "GaudiKernel/AlgFactory.h"
	#include "GaudiKernel/Algorithm.h"
	#include "GaudiKernel/NTuple.h"



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
class DzeroOmega : public Algorithm
{
public:
	// * Constructor and destructors *
	DzeroOmega(const std::string& name, ISvcLocator* pSvcLocator);

	// * Algorithm steps *
	StatusCode initialize();
	StatusCode execute();
	StatusCode finalize();

private:

	// * Reader for beam info * //
	// ReadBeamParFromDb fReader;

	// ! ------- DECLARE CUTS HERE ------- ! //
		// Here, you can define data members that you use to define cuts. The values for these cuts should be set in the `DzeroOmega::DzeroOmega` constructor (see `.cxx` file).

		// * Declare r0, z0 cut for charged tracks *
		double fVr0cut;
		double fVz0cut;
		double fRvz0cut;
		double fRvxy0cut;

		// * Declare energy, dphi, dthe cuts for fake gamma's *
		double fEnergyThreshold;
		double fGammaPhiCut;
		double fGammaThetaCut;
		double fGammaAngleCut;

		// * Declare invariant mass window cut *
		double fDeltaMrho0; // width of inv. mass window around mrho0

		// * Declare whether to test the success of the 4- and 5-constraint fits *
		double fMaxChiSq;


	// ! ------- DECLARE NTUPLES HERE ------- ! //
		// NTuples are like vectors, but its members do not necessarily have to be of the same type. In this package, the NTuples are used to store event-by-event information. Its values are then written to the output ROOT file, creating a ROOT TTree. In that sense, each NTuple here represents one TTree within that output ROOT file, and each NTuple::Item represents its leaves. The name of the leaves is determined when calling NTuple::addItem.
		// Note that the NTuple::Items are added to the NTuple during the DzeroOmega::initialize() step. This is also the place where you name these variables, so make sure that the structure here is reflected there!

		// * Multiplicities *
		NTuple::Tuple* fTupleMult;
			NTuple::Item<int>    fNtotal;   // total number of events per track
			NTuple::Item<int>    fNneutral; // number of neutral tracks
			NTuple::Item<int>    fNcharge;  // number of charged tracks
			NTuple::Item<int>    fNmdc;     // number of charged tracks in MDC
			NTuple::Item<int>    fNgood;    // number of 'good' charged tracks

		// * Vertex information of the charged tracks *
		NTuple::Tuple* fTupleVxyz;
			NTuple::Item<double> fVx0;
			NTuple::Item<double> fVy0;
			NTuple::Item<double> fVz0;
			NTuple::Item<double> fVr0;
			NTuple::Item<double> fRvxy0;
			NTuple::Item<double> fRvz0;
			NTuple::Item<double> fRvphi0;

		// * 'Fake' photon (angles) *
		NTuple::Tuple* fTupleAngles;
			NTuple::Item<double> fDeltaTheta;
			NTuple::Item<double> fDeltaPhi;
			NTuple::Item<double> fDeltaAngle;
			NTuple::Item<double> fEraw;

		// * Raw invariant mass of the two gammas and their total energy *
		NTuple::Tuple* fTupleMgg;
			NTuple::Item<double> fMtoGG;
			NTuple::Item<double> fEtot;

		// * 4-constraint (4C) fit information *
		NTuple::Tuple* fTupleFit4C;
			NTuple::Item<double> fChi1;
			NTuple::Item<double> fMpi0;

		// * 5-constraint (6C) fit information *
		NTuple::Tuple* fTupleFit6C;
			NTuple::Item<double> fChi2;
			NTuple::Item<double> fMrho0;
			NTuple::Item<double> fMrhop;
			NTuple::Item<double> fMrhom;

		// * Photons *
		NTuple::Tuple* fTuplePhoton;
			NTuple::Item<double> fFcos;
			NTuple::Item<double> fElow;

		// * Energy loss dE/dx *
		NTuple::Tuple* fTupleDedx;
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

		// * End cap ToF decector *
		NTuple::Tuple* fTupleTofEC;
			NTuple::Item<double> fPtotTofEC;
			NTuple::Item<double> fPathTofEC;
			NTuple::Item<double> fTofEC;
			NTuple::Item<double> fCntrTofEC;
			NTuple::Item<double> fElectronTofEC;
			NTuple::Item<double> fMuonTofEC;
			NTuple::Item<double> fProtoniTofEC;
			NTuple::Item<double> fKaonTofEC;
			NTuple::Item<double> fProtonTofEC;
			NTuple::Item<double> fPhTofEC;
			NTuple::Item<double> fRhitTofEC;
			NTuple::Item<double> fQualTofEC;

		// * Inner barrel ToF decector *
		NTuple::Tuple* TupleTofIB;
			NTuple::Item<double> fPtotTofIB;
			NTuple::Item<double> fPathTofIB;
			NTuple::Item<double> fTofIB;
			NTuple::Item<double> fCntrTofIB;
			NTuple::Item<double> fElectronTofIB;
			NTuple::Item<double> fMuonTofIB;
			NTuple::Item<double> fProtoniTofIB;
			NTuple::Item<double> fKaonTofIB;
			NTuple::Item<double> fProtonTofIB;
			NTuple::Item<double> fPhTofIB;
			NTuple::Item<double> fZhitTofIB;
			NTuple::Item<double> fQualTofIB;

		// * Outer barrel ToF decector *
		NTuple::Tuple* TupleTofOB;
			NTuple::Item<double> fPtotTofOB;
			NTuple::Item<double> fPathTofOB;
			NTuple::Item<double> fTofOB;
			NTuple::Item<double> fCntrTofOB;
			NTuple::Item<double> fElectronTofOB;
			NTuple::Item<double> fMuonTofOB;
			NTuple::Item<double> fProtoniTofOB;
			NTuple::Item<double> fKaonTofOB;
			NTuple::Item<double> fProtonTofOB;
			NTuple::Item<double> fPhTofOB;
			NTuple::Item<double> fZhitTofOB;
			NTuple::Item<double> fQualTofOB;

		// * Particle ID info *
		NTuple::Tuple* fTuplePID;
			NTuple::Item<double> fPtrackPID;
			NTuple::Item<double> fCostPID;
			NTuple::Item<double> fDedxPID;
			NTuple::Item<double> fTof1PID;
			NTuple::Item<double> fTof2PID;
			NTuple::Item<double> fProbPID;

};

#endif