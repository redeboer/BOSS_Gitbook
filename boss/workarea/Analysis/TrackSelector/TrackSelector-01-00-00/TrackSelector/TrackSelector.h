#ifndef Analysis_TrackSelector_H
#define Analysis_TrackSelector_H

/**
 * @brief    Analyse \f$ J/\psi \rightarrow D^0 \phi \f$ events.
 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
 * @date     December 14th, 2018
 */



// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "EmcRecEventModel/RecEmcShower.h"
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
	#include <map> /// @todo It would be more efficient to use `unordered_map`, but this requires a newer version of `gcc`.
	#include <string>
	#include <vector>



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
		template<typename TYPE> void AddItemsToNTuples  (const char* tupleName, std::map<std::string, NTuple::Item<TYPE> > &map);
		template<typename TYPE> void AddItemsToNTuples(NTuplePtr nt, std::map<std::string, NTuple::Item<TYPE> > &map);
		template<typename TYPE> void BookNtupleItemsDedx(const char* tupleName, std::map<std::string, NTuple::Item<TYPE> > &map);
		template<typename TYPE> void BookNtupleItemsTof (const char* tupleName, std::map<std::string, NTuple::Item<TYPE> > &map);
		template<typename TYPE> void WriteDedxInfo(EvtRecTrack* evtRecTrack, const char* tupleName, std::map<std::string, NTuple::Item<TYPE> > &map);
		template<typename TYPE> void WriteTofInformation(SmartRefVector<RecTofTrack>::iterator iter_tof, double ptrk, const char* tupleName, std::map<std::string, NTuple::Item<TYPE> > &map);
		void ResetSmallestChiSq();
		void ResetSmallestChiSq(double &chisq);
		void WriteDedxInfoForVector(std::vector<EvtRecTrack*> &vector, const char* tupleName, std::map<std::string, NTuple::Item<double> > &map);
		void WritePIDInformation(ParticleID *pid);

	// * Protected data members * //
		HepLorentzVector ComputeMomentum(EvtRecTrack *track);
		MsgStream fLog; //!< Stream object for logging. It needs to be declared as a data member so that it is accessible to all methods of this class.
		RecEmcShower *fTrackEMC; //!< Pointer to the track info from the EM calorimeter. It is a data member to make it accessible to other methods as well.
		RecMdcDedx *fTrackDedx; //!< Pointer to the \f$dE/dx\f$ info from the MDC. It is a data member to make it accessible to other methods as well.
		RecMdcTrack *fTrackMDC; //!< Pointer to the track info from the MDC. It is a data member to make it accessible to other methods as well.
		SmartDataPtr<Event::EventHeader> fEventHeader;  //!< Data pointer for  `Event::EventHeader` which is set in `execute()` in each event.
		SmartDataPtr<EvtRecEvent> fEvtRecEvent;  //!< Data pointer for `EventModel::EvtRec::EvtRecEvent` which is set in `execute()` in each event.
		SmartDataPtr<EvtRecTrackCol> fEvtRecTrkCol; //!< Data pointer for `EventModel::EvtRec::EvtRecTrackCol` which is set in `execute()` in each event.
		double fSmallestChiSq; //!< Current \f$\chi_\mathrm{red}^2\f$ for the Kalman kinematic fit.
		std::map<std::string, NTuple::Tuple*> fNTupleMap; //!< Map for `NTuple::Tuple*`s. The string identifier should be the name of the `NTuple` and of the eventual `TTree`.
		std::map<std::string, std::vector<EvtRecTrack*> > fEvtRecTrackMap; //!< Map of vectors. @todo Decide if this structure is useful.
		std::vector<EvtRecTrack*> fGoodChargedTracks; //!< Vector that, in each event, will be filled by a selection of pointers to 'good' charged tracks.
		std::vector<EvtRecTrack*> fGoodNeutralTracks; //!< Vector that, in each event, will be filled by a selection of pointers to 'good' neutral tracks (photons).
		std::vector<EvtRecTrack*>::iterator fTrackIterator; //!< Iterator for looping over the collection of charged and neutral tracks (`EvtRecTrackCol`).


	// ! NTuple data members ! //
		/// `NTuple`s are like vectors, but its members do not necessarily have to be of the same type. In this package, the NTuples are used to store event-by-event information. Its values are then written to the output ROOT file, creating a ROOT TTree. In that sense, each NTuple here represents one TTree within that output ROOT file, and each `NTuple::Item` represents its leaves. The name of the leaves is determined when calling `NTuple::addItem`.
		/// Note that the `NTuple::Items` have to be added to the NTuple during the `TrackSelector::initialize()` step, otherwise they cannot be used as values! This is also the place where you name these variables, so make sure that the structure here is reflected there!

		// * Maps of Ntuples *
			bool fDo_mult;    //!< Package property that determines whether or not to record multiplicities.
			bool fDo_vertex;  //!< Package property that determines whether or not to record primary vertex info.
			bool fDo_charged; //!< Package property that determines whether or not to record charged track vertex info.
			bool fDo_neutral; //!< Package property that determines whether or not to record neutral track info.
			bool fDo_dedx;    //!< Package property that determines whether or not to record energy loss (\f$dE/dx\f$).
			bool fDo_ToFEC;   //!< Package property that determines whether or not to record data from the Time-of-Flight end cap detector.
			bool fDo_ToFIB;   //!< Package property that determines whether or not to record data from the Time-of-Flight inner barrel detector.
			bool fDo_ToFOB;   //!< Package property that determines whether or not to record data from the Time-of-Flight outer barrel detector.
			bool fDo_pid;     //!< Package property that determines whether or not to record general PID information (TOF, \f$dE/dx\f$, etc).
			std::map<std::string, NTuple::Item<int> >    fMult;    //!< Container for the multiplicities branch.
			std::map<std::string, NTuple::Item<double> > fVertex;  //!< Container for the primary vertex info vertex branch.
			std::map<std::string, NTuple::Item<double> > fCharged; //!< Container for the charged track vertex info charged track vertex branch.
			std::map<std::string, NTuple::Item<double> > fNeutral; //!< Container for the neutral track info neutral track info branch.
			std::map<std::string, NTuple::Item<double> > fDedx;    //!< Container for the energy loss (\f$dE/dx\f$) branch.
			std::map<std::string, NTuple::Item<double> > fTofEC;   //!< Container for the data from the Time-of-Flight end cap detector branch.
			std::map<std::string, NTuple::Item<double> > fTofIB;   //!< Container for the data from the Time-of-Flight inner barrel detector branch.
			std::map<std::string, NTuple::Item<double> > fTofOB;   //!< Container for the data from the Time-of-Flight outer barrel detector branch.
			std::map<std::string, NTuple::Item<double> > fPID;     //!< Container for the general PID information (TOF, \f$dE/dx\f$, etc) branch. <b>Needs to be filled in the derived class!</b>


private:
	/// The private data members are used to define cuts. The values for these cuts should be set in the `TrackSelector::TrackSelector` constructor (see corresponding `.cxx` file).
	double fCut_vr0;
	double fCut_vz0;
	double fCut_rvz0;
	double fCut_rvxy0;
	double fMaxPhotonEnergy;

};

#endif