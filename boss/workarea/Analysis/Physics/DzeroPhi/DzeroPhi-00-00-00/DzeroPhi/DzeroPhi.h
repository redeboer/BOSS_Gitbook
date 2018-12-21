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
	#include <map> // would be more efficient to use "unordered_map"
	#include <string>
	#include <utility>
	#include <vector>



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
class DzeroPhi : public Algorithm
{
public:
	// * Constructor and destructors *
	DzeroPhi(const std::string &name, ISvcLocator* pSvcLocator);

	// * Algorithm steps *
	StatusCode initialize();
	StatusCode execute();
	StatusCode finalize();


protected:
	// * Protected methods * //
		NTuplePtr BookNTuple(const char* tupleName);
		template<typename TYPE> void WriteTofInformation(SmartRefVector<RecTofTrack>::iterator iter_tof, double ptrk, const char* tupleName, std::map<const char*, NTuple::Item<TYPE> > &map);
		template<typename TYPE> void WriteDedxInfo(EvtRecTrack* evtRecTrack, const char* tupleName, std::map<const char*, NTuple::Item<TYPE> > &map);
		template<typename TYPE> void WriteDedxInfoForVector(std::vector<EvtRecTrack*> &vector, const char* tupleName, std::map<const char*, NTuple::Item<TYPE> > &map);
		template<typename TYPE> void BookNtupleTofItems (NTuplePtr &nt, std::map<const char*, NTuple::Item<TYPE> > &map);
		template<typename TYPE> void BookNtupleDedxItems(NTuplePtr &nt, std::map<const char*, NTuple::Item<TYPE> > &map);
		template<typename TYPE>
		void AddItemsToNTuples(NTuplePtr &nt, std::map<const char*, NTuple::Item<TYPE> > &map);

		// template<class... ARGS> void DrawAndSaveRecursion(Option_t* option, ARGS&&... args);
		// template<class TYPE, class... ARGS> void DrawAndSaveRecursion(Option_t* option, TYPE first, ARGS... args);
		// template<> void DrawAndSaveRecursion(Option_t* option) {} // end recursion

	// * Protected data members * //
		double fSmallestChiSq;
		MsgStream fLog; //!< Stream object for logging. It needs to be declared as a data member so that it is accessible to all methods of this class.
		std::map<const char*, NTuple::Tuple*> fNTupleMap; //!< Map for `NTuple::Tuple*`s. The string identifier should be the name of the `NTuple` and of the eventual `TTree`.

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

		// * Maps of Ntuples *
			bool fDoMult;    //!< Package property that determines whether or not to record multiplicities.
			bool fDoVertex;  //!< Package property that determines whether or not to record primary vertex info.
			bool fDoTrackVertex; //!< Package property that determines whether or not to record track vertex info.
			bool fDoDedx;    //!< Package property that determines whether or not to record \f$dE/dx\f$.
			bool fDoDedx_K;  //!< Package property that determines whether or not to record \f$dE/dx\f$ of the kaons (\f$K^\pm\f$).
			bool fDoDedx_pi; //!< Package property that determines whether or not to record \f$dE/dx\f$ of the pions (\f$\pi^\pm\f$).
			bool fDoTofEC;   //!< Package property that determines whether or not to record ToF data from the end cap.
			bool fDoTofIB;   //!< Package property that determines whether or not to record ToF data from the inner barrel.
			bool fDoTofOB;   //!< Package property that determines whether or not to record ToF data from the outer barrel.
			bool fDoPID;     //!< Package property that determines whether or not to record PID information.
			bool fDoFit4c;   //!< Package property that determines whether or not to perform and record a 4-constraint Kalman kinematic fit.
			bool fDoFit6c;   //!< Package property that determines whether or not to perform and record a 6-constraint Kalman kinematic fit.
			std::map<const char*, NTuple::Item<int> >    fMult;    //!< Container for the multiplicy branch.
			std::map<const char*, NTuple::Item<double> > fVertex;  //!< Container for the vertex branch.
			std::map<const char*, NTuple::Item<double> > fTrackVertex; //!< Container for the track verex branch.
			std::map<const char*, NTuple::Item<double> > fDedx;    //!< Container for the `"dedx"` branch.
			std::map<const char*, NTuple::Item<double> > fDedx_K;  //!< Container for the `"dedx_k"` branch.
			std::map<const char*, NTuple::Item<double> > fDedx_pi; //!< Container for the `"dedx_pi"` branch.
			std::map<const char*, NTuple::Item<double> > fTofEC;   //!< Container for the `"tofe"` branch.
			std::map<const char*, NTuple::Item<double> > fTofIB;   //!< Container for the `"tof1"` branch.
			std::map<const char*, NTuple::Item<double> > fTofOB;   //!< Container for the `"tof2"` branch.
			std::map<const char*, NTuple::Item<double> > fPID;     //!< Container for the `"pid"` branch.
			std::map<const char*, NTuple::Item<double> > fFit4c;   //!< Container for the fit4c branch.
			std::map<const char*, NTuple::Item<double> > fFit6c;   //!< Container for the fit6c branch.

};

#endif