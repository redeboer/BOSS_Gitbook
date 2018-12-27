#ifndef Analysis_TrackSelector_H
#define Analysis_TrackSelector_H

/**
 * @brief    Base algorithm that creates a selection of charged and/or neutral tracks.
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
	#include "VertexFit/KalmanKinematicFit.h"
	#include <map> /// @todo It would be more efficient to use `unordered_map`, but this requires a newer version of `gcc`.
	#include <string>
	#include <vector>



// * ==================================== * //
// * ------- GLOBALS AND TYPEDEFS ------- * //
// * ==================================== * //

	/**
	 * @brief Namespace that contains some parameters (such as particle masses) that are useful for derived classes of the `TrackSelector` base algorithm.
	 */
	namespace TSGlobals
	{
		const double gM_rho  = 0.77526;    /// Mass of \f$\rho^{0\pm}\f$, see <a href="http://pdg.lbl.gov/2018/listings/rpp2018-list-rho-770.pdf">PDG</a>.
		const double gM_pi0  = 0.1349770;  /// Mass of \f$\pi^0\f$, see <a href="http://pdg.lbl.gov/2018/listings/rpp2018-list-pi-zero.pdf">PDG</a>.
		const double gM_pi   = 0.13957061; /// Mass of \f$\pi^\pm\f$, see <a href="http://pdg.lbl.gov/2018/listings/rpp2018-list-pi-plus-minus.pdf">PDG</a>.
		const double gM_K    = 0.493677;   /// Mass of \f$K^\pm\f$.
		const double gM_D0   = 1.86483;    /// Mass of \f$D^0\f$, see <a href="http://pdg.lbl.gov/2018/listings/rpp2018-list-D-zero.pdf">PDG</a>.
		const double gM_phi  = 1.019461;   /// Mass of \f$\phi\f$.
		const double gM_Jpsi = 3.0969;     /// Mass of \f$J/\psi\f$, see <a href="http://pdg.lbl.gov/2018/listings/rpp2018-list-J-psi-1S.pdf">PDG</a>.
		const double gEcms   = 3.097;      /// Center-of-mass energy.
		const HepLorentzVector gEcmsVec(0.034, 0, 0, gEcms);
		enum PIDMethod {
			Probability,
			Likelihood,
			NeuronNetwork
		};
	};



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
		virtual StatusCode initialize_rest() = 0; //!< This function is executed at the end of `initialize`. It should be further defined in derived subalgorithms.
		virtual StatusCode execute_rest() = 0; //!< This function is executed at the end of `execute`. It should be further defined in derived subalgorithms.
		virtual StatusCode finalize_rest() = 0; //!< This function is executed at the end of `finalize`. It should be further defined in derived subalgorithms.

	// * Protected methods * //
		NTuplePtr BookNTuple(const char* tupleName, const char* tupleTitle = "ks N-Tuple example");
		ParticleID* InitializePID(TSGlobals::PIDMethod method, const int pidsys, const int pidcase, const double chimin=4.);
		void AddItemsToNTuples(const char* tupleName, std::map<std::string, NTuple::Item<double> > &map, const char* tupleTitle="ks N-Tuple example");
		void AddItemsToNTuples(NTuplePtr nt, std::map<std::string, NTuple::Item<double> > &map);
		void BookNtupleItemsDedx(const char* tupleName, std::map<std::string, NTuple::Item<double> > &map, const char* tupleTitle = "dE/dx info");
		void BookNtupleItemsTof (const char* tupleName, std::map<std::string, NTuple::Item<double> > &map, const char* tupleTitle = "Time-of-Flight info");
		void WriteDedxInfo(EvtRecTrack* evtRecTrack, const char* tupleName, std::map<std::string, NTuple::Item<double> > &map);
		void WriteTofInformation(SmartRefVector<RecTofTrack>::iterator iter_tof, double ptrk, const char* tupleName, std::map<std::string, NTuple::Item<double> > &map);
		void WriteDedxInfoForVector(std::vector<EvtRecTrack*> &vector, const char* tupleName, std::map<std::string, NTuple::Item<double> > &map);
		void WritePIDInformation();

	// * Protected data members * //
		HepLorentzVector ComputeMomentum(EvtRecTrack *track);
		MsgStream fLog; //!< Stream object for logging. It needs to be declared as a data member so that it is accessible to all methods of this class.
		ParticleID *fPIDInstance; //!< Pointer to instance of particle identification (PID). Only used in <i>derived subalgorithms</i>.
		RecEmcShower *fTrackEMC; //!< Pointer to the track info from the EM calorimeter. It is a data member to make it accessible to other methods as well.
		RecMdcDedx *fTrackDedx; //!< Pointer to the \f$dE/dx\f$ info from the MDC. It is a data member to make it accessible to other methods as well.
		RecMdcTrack *fTrackMDC; //!< Pointer to the track info from the MDC. It is a data member to make it accessible to other methods as well.
		SmartDataPtr<Event::EventHeader> fEventHeader;  //!< Data pointer for  `Event::EventHeader` which is set in `execute()` in each event.
		SmartDataPtr<EvtRecEvent> fEvtRecEvent;  //!< Data pointer for `EventModel::EvtRec::EvtRecEvent` which is set in `execute()` in each event.
		SmartDataPtr<EvtRecTrackCol> fEvtRecTrkCol; //!< Data pointer for `EventModel::EvtRec::EvtRecTrackCol` which is set in `execute()` in each event.
		std::map<std::string, NTuple::Tuple*> fNTupleMap; //!< Map for `NTuple::Tuple*`s. The string identifier should be the name of the `NTuple` and of the eventual `TTree`.
		std::map<std::string, std::vector<EvtRecTrack*> > fEvtRecTrackMap; //!< Map of vectors. @todo Decide if this structure is useful.
		std::vector<EvtRecTrack*> fGoodChargedTracks; //!< Vector that, in each event, will be filled by a selection of pointers to 'good' charged tracks.
		std::vector<EvtRecTrack*> fGoodNeutralTracks; //!< Vector that, in each event, will be filled by a selection of pointers to 'good' neutral tracks (photons).
		std::vector<EvtRecTrack*>::iterator fTrackIterator; //!< Iterator for looping over the collection of charged and neutral tracks (`EvtRecTrackCol`).


	// ! NTuple data members ! //
		/// `NTuple`s are like vectors, but its members do not necessarily have to be of the same type. In this package, the NTuples are used to store event-by-event information. Its values are then written to the output ROOT file, creating a ROOT TTree. In that sense, each NTuple here represents one TTree within that output ROOT file, and each `NTuple::Item` represents its leaves. The name of the leaves is determined when calling `NTuple::addItem`.
		/// Note that the `NTuple::Items` have to be added to the NTuple during the `TrackSelector::initialize()` step, otherwise they cannot be used as values! This is also the place where you name these variables, so make sure that the structure here is reflected there!

		// * Maps of Ntuples *
			bool fDo_PID;     //!< Package property that determines whether or not to record general PID information (TOF, \f$dE/dx\f$, etc).
			bool fDo_ToFEC;   //!< Package property that determines whether or not to record data from the Time-of-Flight end cap detector.
			bool fDo_ToFIB;   //!< Package property that determines whether or not to record data from the Time-of-Flight inner barrel detector.
			bool fDo_ToFOB;   //!< Package property that determines whether or not to record data from the Time-of-Flight outer barrel detector.
			bool fDo_charged; //!< Package property that determines whether or not to record charged track vertex info.
			bool fDo_dedx;    //!< Package property that determines whether or not to record energy loss (\f$dE/dx\f$).
			bool fDo_mult;    //!< Package property that determines whether or not to record multiplicities.
			bool fDo_mult_select; //!< Package property that determines whether or not to write the multiplicities <i>of the selected particles</i>.
			bool fDo_neutral; //!< Package property that determines whether or not to record neutral track info.
			bool fDo_vertex;  //!< Package property that determines whether or not to record primary vertex info.
			std::map<std::string, NTuple::Item<double> > fMap_PID;     //!< Container for the general PID information (TOF, \f$dE/dx\f$, etc) branch. <b>Needs to be filled in the derived class!</b>
			std::map<std::string, NTuple::Item<double> > fMap_TofEC;   //!< Container for the data from the Time-of-Flight end cap detector branch.
			std::map<std::string, NTuple::Item<double> > fMap_TofIB;   //!< Container for the data from the Time-of-Flight inner barrel detector branch.
			std::map<std::string, NTuple::Item<double> > fMap_TofOB;   //!< Container for the data from the Time-of-Flight outer barrel detector branch.
			std::map<std::string, NTuple::Item<double> > fMap_charged; //!< Container for the charged track vertex info charged track vertex branch.
			std::map<std::string, NTuple::Item<double> > fMap_dedx;    //!< Container for the energy loss (\f$dE/dx\f$) branch.
			std::map<std::string, NTuple::Item<double> > fMap_mult;    //!< Container for the multiplicities branch.
			std::map<std::string, NTuple::Item<double> > fMap_mult_select; //!< Container for the `"mult_select"` branch.
			std::map<std::string, NTuple::Item<double> > fMap_neutral; //!< Container for the neutral track info neutral track info branch.
			std::map<std::string, NTuple::Item<double> > fMap_vertex;  //!< Container for the primary vertex info vertex branch.


	// * Maps, vectors, and iterators * //
		/// The private data members are used to define cuts. The values for these cuts should be set in the `TrackSelector::TrackSelector` constructor (see corresponding `.cxx` file).
		double fCut_MaxVr0; //!< Maximal cut on the radius \f$r\f$ of the primary vertex.
		double fCut_MaxVz0; //!< Maximal cut on the \f$z\f$ coordinate of the primary vertex.
		double fCut_MaxRvz0;
		double fCut_MaxRvxy0;
		double fCut_MinPhotonEnergy; //!< Minimimal cut on the photon energy.
		double fCut_MaxPIDChiSq; //!< Maximum \f$\chi_\mathrm{red}^2\f$ of the kinematic Kalman fits
		double fCut_MinPIDProb; //!< Minimimal probability that a particle is either a kaon, pion, electron, muon, or proton according to the probability method. See e.g. <a href="http://bes3.to.infn.it/Boss/7.0.2/html/classParticleID.html#147bb7be5fa47f275ca3b32e6ae8fbc6">`ParticleID::probPion`</a>.

};

#endif