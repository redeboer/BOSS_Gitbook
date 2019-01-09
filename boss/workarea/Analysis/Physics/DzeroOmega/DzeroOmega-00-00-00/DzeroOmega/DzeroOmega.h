#ifndef Physics_Analysis_DzeroOmega_H
#define Physics_Analysis_DzeroOmega_H

/// @addtogroup BOSS_packages
/// @{

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
	#include <map>
	#include <string>



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
class DzeroOmega : public Algorithm
{
public:
	// * Constructor and destructors * //
	DzeroOmega(const std::string& name, ISvcLocator* pSvcLocator);

	// * Algorithm steps * //
	StatusCode initialize();
	StatusCode execute();
	StatusCode finalize();

private:

	// * Private methods * //
	NTuplePtr BookNTuple(const char* tupleName);

	// * Private data members * //
	std::map<std::string, NTuplePtr> fNTupleMap;
		//!< Map for `NTuplePtr`s. The string identifier should be the name of the `NTuple` and of the eventual `TTree`.

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
			NTuple::Item<int> fNtotal;   //!< Total number of events per track.
			NTuple::Item<int> fNneutral; //!< Number of charged tracks.
			NTuple::Item<int> fNcharge;  //!< Number of charged tracks.
			NTuple::Item<int> fNmdc;     //!< Number of charged tracks in MDC.
			NTuple::Item<int> fNgood;    //!< Number of 'good' charged tracks.

		// * Vertex information of the charged tracks *
			NTuple::Item<double> fVx0;    //!< Primary x-vertex as determined by MDC.
			NTuple::Item<double> fVy0;    //!< Primary y-vertex as determined by MDC.
			NTuple::Item<double> fVz0;    //!< Primary z-vertex as determined by MDC.
			NTuple::Item<double> fVr0;    //!< Distance from origin in xy-plane.
			NTuple::Item<double> fRvxy0;  //!< Nearest distance to IP in xy plane.
			NTuple::Item<double> fRvz0;   //!< Nearest distance to IP in z direction.
			NTuple::Item<double> fRvphi0; //!< Angle in the xy-plane (?).

		// * 'Fake' photon (angles) *
			NTuple::Item<double> fDeltaTheta; //!< Theta difference with nearest charged track (degrees).
			NTuple::Item<double> fDeltaPhi;   //!< Phi difference with nearest charged track (degrees).
			NTuple::Item<double> fDeltaAngle; //!< Angle difference with nearest charged track.
			NTuple::Item<double> fEraw;       //!< Energy of the photon.

		// * Raw invariant mass of the two gammas and their total energy *
			NTuple::Item<double> fMtoGG; //!< Invariant mass of the two gammas.
			NTuple::Item<double> fEtot;  //!< Total energy of pi^+, pi^ and the two gammas.

		// * 4-constraint (4C) fit information *
			NTuple::Item<double> fChi1; //!< Invariant pi0 mass according to Kalman kinematic fit.
			NTuple::Item<double> fMpi0; //!< Chi square of the Kalman kinematic fit.

		// * 5-constraint (6C) fit information *
			NTuple::Item<double> fChi2;  //!< Chi squared of the Kalman kinematic fit.
			NTuple::Item<double> fMrho0; //!< Inv. mass pi^+ pi^- (rho^0).
			NTuple::Item<double> fMrhop; //!< Inv. mass pi^0 pi^+ (rho^+).
			NTuple::Item<double> fMrhom; //!< Inv. mass pi^0 pi^- (rho^-).

		// * Photons *
			NTuple::Item<double> fFcos; //!< E/p ratio for pi^0 candidate.
			NTuple::Item<double> fElow; //!< Lowest energy of the two gammas.

		// * Energy loss dE/dx *
			NTuple::Item<double> fPtrack; //!< Momentum of the track.
			NTuple::Item<double> fChi2e;  //!< Chi2 in case of electron.
			NTuple::Item<double> fChi2mu; //!< Chi2 in case of muon.
			NTuple::Item<double> fChi2pi; //!< Chi2 in case of pion.
			NTuple::Item<double> fChi2k;  //!< Chi2 in case of kaon.
			NTuple::Item<double> fChi2p;  //!< Chi2 in case of proton.
			NTuple::Item<double> fProbPH; //!< Most probable pulse height from truncated mean.
			NTuple::Item<double> fNormPH; //!< Normalized pulse height.
			NTuple::Item<double> fGhit;   //!< Number of good hits.
			NTuple::Item<double> fThit;   //!< Total number of hits.

		// * End cap ToF decector *
			NTuple::Item<double> fPtotTofEC;     //!< Momentum of the track as reconstructed by MDC.
			NTuple::Item<double> fPathTofEC;     //!< Path length.
			NTuple::Item<double> fTofEC;         //!< Time of flight.
			NTuple::Item<double> fCntrTofEC;     //!< ToF counter ID.
			NTuple::Item<double> fElectronTofEC; //!< ToF pulse height.
			NTuple::Item<double> fMuonTofEC;     //!< Track extrapolate Z or R Hit position.
			NTuple::Item<double> fProtoniTofEC;  //!< Data quality of reconstruction.
			NTuple::Item<double> fKaonTofEC;     //!< Difference with ToF in electron hypothesis.
			NTuple::Item<double> fProtonTofEC;   //!< Difference with ToF in muon hypothesis.
			NTuple::Item<double> fPhTofEC;       //!< Difference with ToF in charged pion hypothesis.
			NTuple::Item<double> fRhitTofEC;     //!< Difference with ToF in charged kaon hypothesis.
			NTuple::Item<double> fQualTofEC;     //!< Difference with ToF in proton hypothesis.

		// * Inner barrel ToF decector *
			NTuple::Item<double> fPtotTofIB;     //!< Momentum of the track as reconstructed by MDC.
			NTuple::Item<double> fPathTofIB;     //!< Path length.
			NTuple::Item<double> fTofIB;         //!< Time of flight.
			NTuple::Item<double> fCntrTofIB;     //!< ToF counter ID.
			NTuple::Item<double> fElectronTofIB; //!< ToF pulse height.
			NTuple::Item<double> fMuonTofIB;     //!< Track extrapolate Z or R Hit position.
			NTuple::Item<double> fProtoniTofIB;  //!< Data quality of reconstruction.
			NTuple::Item<double> fKaonTofIB;     //!< Difference with ToF in electron hypothesis.
			NTuple::Item<double> fProtonTofIB;   //!< Difference with ToF in muon hypothesis.
			NTuple::Item<double> fPhTofIB;       //!< Difference with ToF in charged pion hypothesis.
			NTuple::Item<double> fZhitTofIB;     //!< Difference with ToF in charged kaon hypothesis.
			NTuple::Item<double> fQualTofIB;     //!< Difference with ToF in proton hypothesis.

		// * Outer barrel ToF decector *
			NTuple::Item<double> fPtotTofOB;     //!< Momentum of the track as reconstructed by MDC.
			NTuple::Item<double> fPathTofOB;     //!< Path length.
			NTuple::Item<double> fTofOB;         //!< Time of flight.
			NTuple::Item<double> fCntrTofOB;     //!< ToF counter ID.
			NTuple::Item<double> fElectronTofOB; //!< ToF pulse height.
			NTuple::Item<double> fMuonTofOB;     //!< Track extrapolate Z or R Hit position.
			NTuple::Item<double> fProtoniTofOB;  //!< Data quality of reconstruction.
			NTuple::Item<double> fKaonTofOB;     //!< Difference with ToF in electron hypothesis.
			NTuple::Item<double> fProtonTofOB;   //!< Difference with ToF in muon hypothesis.
			NTuple::Item<double> fPhTofOB;       //!< Difference with ToF in charged pion hypothesis.
			NTuple::Item<double> fZhitTofOB;     //!< Difference with ToF in charged kaon hypothesis.
			NTuple::Item<double> fQualTofOB;     //!< Difference with ToF in proton hypothesis.

		// * Particle ID info *
			NTuple::Item<double> fPtrackPID; //!< Momentum of the track.
			NTuple::Item<double> fCostPID;   //!< Theta angle of the track.
			NTuple::Item<double> fDedxPID;   //!< Chi2 of the dedx of the track.
			NTuple::Item<double> fTof1PID;   //!< Chi2 of the inner barrel ToF of the track.
			NTuple::Item<double> fTof2PID;   //!< Chi2 of the outer barrel ToF of the track.
			NTuple::Item<double> fProbPID;   //!< Probability that it is a pion.

};

/// @}


#endif