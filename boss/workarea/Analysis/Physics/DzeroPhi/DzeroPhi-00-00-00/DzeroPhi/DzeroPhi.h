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
	#include "CLHEP/Geometry/Point3D.h"
	#include "CLHEP/Vector/LorentzVector.h"
	#include "CLHEP/Vector/ThreeVector.h"
	#include "CLHEP/Vector/TwoVector.h"
	#include "EventModel/EventHeader.h"
	#include "EvtRecEvent/EvtRecEvent.h"
	#include "EvtRecEvent/EvtRecTrack.h"
	#include "GaudiKernel/AlgFactory.h"
	#include "GaudiKernel/Algorithm.h"
	#include "GaudiKernel/MsgStream.h"
	#include "GaudiKernel/NTuple.h"
	#include "GaudiKernel/SmartDataPtr.h"
	#include "GaudiKernel/SmartRefVector.h"
	#include "TofRecEvent/RecTofTrack.h"
	#include <map>
	#include <string>
	#include <vector>



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


protected:
	// * Protected methods * //
		NTuplePtr BookNTuple(const char* tupleName);
		void BookNtupleTofItems(NTuplePtr& nt);
		void BookNtupleDedxItems(NTuplePtr& nt);
		void WriteTofInformation(SmartRefVector<RecTofTrack>::iterator iter_tof, double ptrk, const char* tupleName);
		void WriteDedxInfo(EvtRecTrack* evtRecTrack, const char* tupleName);
		void WriteDedxInfoForVector(std::vector<EvtRecTrack*>& vector, const char* tupleName);

	// * Protected data members * //
		double fSmallestChiSq;
		MsgStream fLog; //!< Stream object for logging. It needs to be declared as a data member so that it is accessible to all methods of this class.
		std::map<std::string, NTuple::Tuple*> fNTupleMap; //!< Map for `NTuple::Tuple*`s. The string identifier should be the name of the `NTuple` and of the eventual `TTree`.

		std::vector<EvtRecTrack*> fGoodChargedTracks; //!< Vector that, in each event, will contain a selection of pointers to 'good' charged tracks.
		std::vector<EvtRecTrack*> fKaonNeg; //!< Vector that contains a selection of pointers to charged tracks identified as \f$K^-\f$.
		std::vector<EvtRecTrack*> fKaonPos; //!< Vector that contains a selection of pointers to charged tracks identified as \f$K^+\f$.
		std::vector<EvtRecTrack*> fPionPos; //!< Vector that contains a selection of pointers to charged tracks identified as \f$\pi^+\f$.

		std::vector<EvtRecTrack*>::iterator fTrackIterator; //!< Iterator for looping over the collection of charged and neutral tracks (`EvtRecTrackCol`).
		std::vector<EvtRecTrack*>::iterator fKaonNeg1Iter; //!< Iterator for looping over the collection of negative kaons (1st occurence).
		std::vector<EvtRecTrack*>::iterator fKaonNeg2Iter; //!< Iterator for looping over the collection of negative kaons (2st occurence).
		std::vector<EvtRecTrack*>::iterator fKaonPosIter; //!< Iterator for looping over the collection of positive kaons.
		std::vector<EvtRecTrack*>::iterator fPionPosIter; //!< Iterator for looping over the collection of positive pions.

		SmartDataPtr<Event::EventHeader> fEventHeader;  //!< Data pointer for  `Event::EventHeader` which is set in `execute()` in each event.
		SmartDataPtr<EvtRecEvent>        fEvtRecEvent;  //!< Data pointer for `EventModel::EvtRec::EvtRecEvent` which is set in `execute()` in each event.
		SmartDataPtr<EvtRecTrackCol>     fEvtRecTrkCol; //!< Data pointer for `EventModel::EvtRec::EvtRecTrackCol` which is set in `execute()` in each event.


private:
	// ! Cut parameters ! //
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


	// ! NTuple data members ! //
		// NTuples are like vectors, but its members do not necessarily have to be of the same type. In this package, the NTuples are used to store event-by-event information. Its values are then written to the output ROOT file, creating a ROOT TTree. In that sense, each NTuple here represents one TTree within that output ROOT file, and each NTuple::Item represents its leaves. The name of the leaves is determined when calling NTuple::addItem.
		// Note that the NTuple::Items have to be added to the NTuple during the DzeroPhi::initialize() step, otherwise they cannot be used as values! This is also the place where you name these variables, so make sure that the structure here is reflected there!

		// * Multiplicities *
			bool fDoMult; //!< Package property that determines whether or not to write multiplicities and primary vertex info.
			NTuple::Item<int>    fEvent_Ntotal;   //!< Total number of events per track.
			NTuple::Item<int>    fEvent_Ncharge;  //!< Number of charged tracks.
			NTuple::Item<int>    fEvent_Nneutral; //!< Number of charged tracks.
			NTuple::Item<double> fEvent_Vx0;      //!< Primary \f$x\f$ coordinate of the collision point.
			NTuple::Item<double> fEvent_Vy0;      //!< Primary \f$y\f$ coordinate of the collision point.
			NTuple::Item<double> fEvent_Vz0;      //!< Primary \f$z\f$ coordinate of the collision point.
			NTuple::Item<int>    fEvent_Ngood;    //!< Number of 'good' charged tracks.
			NTuple::Item<int>    fEvent_Nmdc;     //!< Number of charged tracks in MDC. @todo Test whether this parameter really makes sense.
			NTuple::Item<int>    fEvent_NKaonPos; //!< Number of \f$K^+\f$.
			NTuple::Item<int>    fEvent_NKaonNeg; //!< Number of \f$K^-\f$.
			NTuple::Item<int>    fEvent_NPionPos; //!< Number of \f$\pi^-\f$.

		// * Vertex information of the charged tracks *
			bool fDoVxyz; //!< Package property that determines whether or not to write track vertex info.
			NTuple::Item<double> fTrack_Vx;    //!< Primary \f$x\f$ coordinate of the vertex as determined by MDC.
			NTuple::Item<double> fTrack_Vy;    //!< Primary \f$y\f$ coordinate of the vertex as determined by MDC.
			NTuple::Item<double> fTrack_Vz;    //!< Primary \f$z\f$ coordinate of the vertex as determined by MDC.
			NTuple::Item<double> fTrack_Vr;    //!< Distance from origin in \f$xy\f$ plane.
			NTuple::Item<double> fTrack_Rvxy;  //!< Nearest distance to IP in \f$xy\f$ plane.
			NTuple::Item<double> fTrack_Rvz;   //!< Nearest distance to IP in \f$z\f$ direction.
			NTuple::Item<double> fTrack_Rvphi; //!< Angle in the \f$xy\f$plane (?). @todo
			NTuple::Item<double> fTrack_phi;   //!< Helix angle of the particle (?). @todo
			NTuple::Item<double> fTrack_p;     //!< Momentum \f$p\f$ of the track.

		// * 4- and 6-constraint (4C/6C) fit information *
			bool fDoFit4c; //!< Package property that determines whether or not to perform a 4-constraint Kalman kinematic fit.
			bool fDoFit6c; //!< Package property that determines whether or not to perform a 6-constraint Kalman kinematic fit.
			NTuple::Item<double> fInvMass_D0;    //!< Invariant mass for \f$ K^- pi^+ \f$ (\f$ D^0 \f$).
			NTuple::Item<double> fInvMass_phi;   //!< Invariant mass for \f$ K^+ K^+  \f$ (\f$ \phi \f$).
			NTuple::Item<double> fInvMass_Jpsi;  //!< Invariant mass for \f$ D^0 \phi \f$ (\f$ J/\psi \f$).
			NTuple::Item<double> fInvMass_ChiSq; //!< Chi squared of the Kalman kinematic fit.

		// * Energy loss dE/dx *
			bool fDoDedx;      //!< Package property that determines whether or not to write \f$dE/dx\f$.
			bool fDoDedx_Kaon; //!< Package property that determines whether or not to write \f$dE/dx\f$ of the kaons (\f$K^\pm\f$).
			bool fDoDedx_Pion; //!< Package property that determines whether or not to write \f$dE/dx\f$ of the pions (\f$\pi^\pm\f$).
			NTuple::Item<double> fDedx_Chi2e;  //!< Chi squared in case of electron.
			NTuple::Item<double> fDedx_Chi2mu; //!< Chi squared in case of muon.
			NTuple::Item<double> fDedx_Chi2pi; //!< Chi squared in case of pion.
			NTuple::Item<double> fDedx_Chi2k;  //!< Chi squared in case of kaon.
			NTuple::Item<double> fDedx_Chi2p;  //!< Chi squared in case of proton.
			NTuple::Item<double> fDedx_ProbPH; //!< Most probable pulse height from truncated mean.
			NTuple::Item<double> fDedx_NormPH; //!< Normalized pulse height.
			NTuple::Item<double> fDedx_Ghit;   //!< Number of good hits.
			NTuple::Item<double> fDedx_Thit;   //!< Total number of hits.

		// * ToF detector parameters (end cap, inner barrel, and outer barrel) *
			bool fDoTofEC; //!< Package property that determines whether or not to write ToF data from the end cap.
			bool fDoTofIB; //!< Package property that determines whether or not to write ToF data from the inner barrel.
			bool fDoTofOB; //!< Package property that determines whether or not to write ToF data from the outer barrel.
			NTuple::Item<double> fTof;          //!< Time of flight.
			NTuple::Item<double> fTof_Ptot;     //!< Momentum of the track as reconstructed by MDC.
			NTuple::Item<double> fTof_Path;     //!< Path length.
			NTuple::Item<double> fTof_Cntr;     //!< ToF counter ID.
			NTuple::Item<double> fTof_Ph;       //!< ToF pulse height.
			NTuple::Item<double> fTof_Rhit;     //!< Track extrapolate \f$Z\f$ or \f$R\f$ Hit position.
			NTuple::Item<double> fTof_Qual;     //!< Data quality of reconstruction.
			NTuple::Item<double> fTof_Electron; //!< Difference with ToF in electron hypothesis.
			NTuple::Item<double> fTof_Muon;     //!< Difference with ToF in muon hypothesis.
			NTuple::Item<double> fTof_Protoni;  //!< Difference with ToF in charged pion hypothesis.
			NTuple::Item<double> fTof_Kaon;     //!< Difference with ToF in charged kaon hypothesis.
			NTuple::Item<double> fTof_Proton;   //!< Difference with ToF in proton hypothesis.

		// * Particle ID info *
			bool fDoPID; //!< Package property that determines whether or not to write PID information.
			NTuple::Item<double> fPID_Cost;   //!< Theta angle of the track.
			NTuple::Item<double> fPID_Dedx;   //!< Chi squared of the dedx of the track.
			NTuple::Item<double> fPID_Tof1;   //!< Chi squared of the inner barrel ToF of the track.
			NTuple::Item<double> fPID_Tof2;   //!< Chi squared of the outer barrel ToF of the track.
			NTuple::Item<double> fPID_Prob;   //!< Probability that it is a pion.

};

#endif