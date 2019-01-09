#ifndef Analysis_TrackSelector_H
#define Analysis_TrackSelector_H

/// @addtogroup BOSS_Afterburner
/// @{

/**
 * @brief    Analyse \f$ J/\psi \rightarrow D^0 \phi \f$ events.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     December 14th, 2018
 */



// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "EventModel/EventHeader.h"
	#include "EvtRecEvent/EvtRecEvent.h"
	#include "EvtRecEvent/EvtRecTrack.h"
	#include "GaudiKernel/Algorithm.h"
	#include "GaudiKernel/MsgStream.h"
	#include "GaudiKernel/NTuple.h"
	#include "GaudiKernel/SmartDataPtr.h"
	#include "GaudiKernel/SmartRefVector.h"
	#include "ParticleID/ParticleID.h"
	#include "TofRecEvent/RecTofTrack.h"
	#include <map> /// Note: It would be more efficient to use `unordered_map`.
	#include <string>
	#include <vector>
	// #include "GaudiKernel/AlgFactory.h"



// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
class TrackSelector : public Algorithm
{
public:
	// * Constructor and destructors *
	TrackSelector(const std::string &name, ISvcLocator* pSvcLocator);

	// * Algorithm steps *
	StatusCode initialize();
	StatusCode execute();
	StatusCode finalize();


protected:

	// * Algorithm steps that have to be defined in derived algorithm classes *
		virtual StatusCode initialize_rest() {}
		virtual StatusCode execute_rest() {}
		virtual StatusCode finalize_rest() {}

	// * Protected methods * //
		NTuplePtr BookNTuple(const char* tupleName, const char* tupleTitle = "ks N-Tuple example");
		template<typename TYPE> void AddItemsToNTuples(NTuplePtr nt, std::map<std::string, NTuple::Item<TYPE> > &map);
		template<typename TYPE> void AddItemsToNTuples  (const char* tupleName, std::map<std::string, NTuple::Item<TYPE> > &map);
		template<typename TYPE> void BookNtupleItemsDedx(const char* tupleName, std::map<std::string, NTuple::Item<TYPE> > &map);
		template<typename TYPE> void BookNtupleItemsTof (const char* tupleName, std::map<std::string, NTuple::Item<TYPE> > &map);
		template<typename TYPE> void WriteDedxInfo(EvtRecTrack* evtRecTrack, const char* tupleName, std::map<std::string, NTuple::Item<TYPE> > &map);
		void WriteDedxInfoForVector(std::vector<EvtRecTrack*> &vector, const char* tupleName, std::map<std::string, NTuple::Item<double> > &map);
		template<typename TYPE> void WriteTofInformation(SmartRefVector<RecTofTrack>::iterator iter_tof, double ptrk, const char* tupleName, std::map<std::string, NTuple::Item<TYPE> > &map);
		void WritePIDInformation(ParticleID *pid);

	// * Protected data members * //
		MsgStream fLog; //!< Stream object for logging. It needs to be declared as a data member so that it is accessible to all methods of this class.
		RecMdcDedx* fTrackDedx;
		RecMdcTrack *fTrackMDC;
		SmartDataPtr<Event::EventHeader> fEventHeader;  //!< Data pointer for  `Event::EventHeader` which is set in `execute()` in each event.
		SmartDataPtr<EvtRecEvent>        fEvtRecEvent;  //!< Data pointer for `EventModel::EvtRec::EvtRecEvent` which is set in `execute()` in each event.
		SmartDataPtr<EvtRecTrackCol>     fEvtRecTrkCol; //!< Data pointer for `EventModel::EvtRec::EvtRecTrackCol` which is set in `execute()` in each event.
		std::map<std::string, NTuple::Tuple*> fNTupleMap; //!< Map for `NTuple::Tuple*`s. The string identifier should be the name of the `NTuple` and of the eventual `TTree`.
		std::map<std::string, std::vector<EvtRecTrack*> > fEvtRecTrackMap; //!< Map of vectors. @todo Decide if this structure is useful.
		std::vector<EvtRecTrack*> fGoodChargedTracks; //!< Vector that, in each event, will contain a selection of pointers to 'good' charged tracks.
		std::vector<EvtRecTrack*>::iterator fTrackIterator; //!< Iterator for looping over the collection of charged and neutral tracks (`EvtRecTrackCol`).


	// ! NTuple data members ! //
		/// `NTuple`s are like vectors, but its members do not necessarily have to be of the same type. In this package, the NTuples are used to store event-by-event information. Its values are then written to the output ROOT file, creating a ROOT TTree. In that sense, each NTuple here represents one TTree within that output ROOT file, and each `NTuple::Item` represents its leaves. The name of the leaves is determined when calling `NTuple::addItem`.
		/// Note that the `NTuple::Items` have to be added to the NTuple during the `TrackSelector::initialize()` step, otherwise they cannot be used as values! This is also the place where you name these variables, so make sure that the structure here is reflected there!

		// * Maps of Ntuples *
			bool fDoMult;    //!< Package property that determines whether or not to record multiplicities.
			bool fDoVertex;  //!< Package property that determines whether or not to record primary vertex info.
			bool fDoTrackVertex; //!< Package property that determines whether or not to record track vertex info.
			bool fDoDedx;    //!< Package property that determines whether or not to record \f$dE/dx\f$.
			bool fDoTofEC;   //!< Package property that determines whether or not to record ToF data from the end cap.
			bool fDoTofIB;   //!< Package property that determines whether or not to record ToF data from the inner barrel.
			bool fDoTofOB;   //!< Package property that determines whether or not to record ToF data from the outer barrel.
			bool fDoPID;     //!< Package property that determines whether or not to record PID information.
			std::map<std::string, NTuple::Item<int> >    fMult;    //!< Container for the multiplicy branch.
			std::map<std::string, NTuple::Item<double> > fVertex;  //!< Container for the vertex branch.
			std::map<std::string, NTuple::Item<double> > fTrackVertex; //!< Container for the track verex branch.
			std::map<std::string, NTuple::Item<double> > fDedx;    //!< Container for the `"dedx"` branch.
			std::map<std::string, NTuple::Item<double> > fTofEC;   //!< Container for the `"tofe"` branch.
			std::map<std::string, NTuple::Item<double> > fTofIB;   //!< Container for the `"tof1"` branch.
			std::map<std::string, NTuple::Item<double> > fTofOB;   //!< Container for the `"tof2"` branch.
			std::map<std::string, NTuple::Item<double> > fPID;     //!< Container for the `"pid"` branch. <b>Needs to be filled in the derived class!</b>


private:
	// ! Cut parameters ! //
		/// Here, you can define data members that you use to define cuts. The values for these cuts should be set in the `TrackSelector::TrackSelector` constructor (see `.cxx` file).
		
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

};

/// @}


#endif