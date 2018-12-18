#ifndef Physics_Analysis_DzeroPhi_H
#define Physics_Analysis_DzeroPhi_H

/**
 * @brief    Analyse \f$ J/\psi \rightarrow D^0 \phi \f$ events.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     December 14th, 2018
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
class DzeroPhi : public Algorithm
{
public:
	// * Constructor and destructors *
	DzeroPhi(const std::string& name, ISvcLocator* pSvcLocator);

	// * Algorithm steps *
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
		// Here, you can define data members that you use to define cuts. The values for these cuts should be set in the `DzeroPhi::DzeroPhi` constructor (see `.cxx` file).

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
		// Note that the NTuple::Items are added to the NTuple during the DzeroPhi::initialize() step. This is also the place where you name these variables, so make sure that the structure here is reflected there!

		// * Vertex information of the charged tracks *
			NTuple::Item<double> fVx0;    //!< Primary x-vertex as determined by MDC.
			NTuple::Item<double> fVy0;    //!< Primary y-vertex as determined by MDC.
			NTuple::Item<double> fVz0;    //!< Primary z-vertex as determined by MDC.
			NTuple::Item<double> fVr0;    //!< Distance from origin in xy-plane.
			NTuple::Item<double> fRvxy0;  //!< Nearest distance to IP in xy plane.
			NTuple::Item<double> fRvz0;   //!< Nearest distance to IP in z direction.
			NTuple::Item<double> fRvphi0; //!< Angle in the xy-plane (?).

		// * 4- and 6-constraint (4C/6C) fit information *
			NTuple::Item<double> fM_D0;   //!< Invvariant mass K^- pi^+ (D^0).
			NTuple::Item<double> fM_phi;  //!< Invvariant mass K^+ K^+  (phi).
			NTuple::Item<double> fM_Jpsi; //!< Invvariant mass D^0 phi  (J/psi).
			NTuple::Item<double> fChi2sq; //!< Chi squared of the Kalman kinematic fit.

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

		// * ToF detector parameters (end cap, inner barrel, and outer barrel) *
			NTuple::Item<double> fPtotTof;     //!< Momentum of the track as reconstructed by MDC.
			NTuple::Item<double> fPathTof;     //!< Path length.
			NTuple::Item<double> fTof;         //!< Time of flight.
			NTuple::Item<double> fCntrTof;     //!< ToF counter ID.
			NTuple::Item<double> fElectronTof; //!< ToF pulse height.
			NTuple::Item<double> fMuonTof;     //!< Track extrapolate Z or R Hit position.
			NTuple::Item<double> fProtoniTof;  //!< Data quality of reconstruction.
			NTuple::Item<double> fKaonTof;     //!< Difference with ToF in electron hypothesis.
			NTuple::Item<double> fProtonTof;   //!< Difference with ToF in muon hypothesis.
			NTuple::Item<double> fPhTof;       //!< Difference with ToF in charged pion hypothesis.
			NTuple::Item<double> fRhitTof;     //!< Difference with ToF in charged kaon hypothesis.
			NTuple::Item<double> fQualTof;     //!< Difference with ToF in proton hypothesis.

		// * Particle ID info *
			NTuple::Item<double> fPtrackPID; //!< Momentum of the track.
			NTuple::Item<double> fCostPID;   //!< Theta angle of the track.
			NTuple::Item<double> fDedxPID;   //!< Chi2 of the dedx of the track.
			NTuple::Item<double> fTof1PID;   //!< Chi2 of the inner barrel ToF of the track.
			NTuple::Item<double> fTof2PID;   //!< Chi2 of the outer barrel ToF of the track.
			NTuple::Item<double> fProbPID;   //!< Probability that it is a pion.

};

#endif