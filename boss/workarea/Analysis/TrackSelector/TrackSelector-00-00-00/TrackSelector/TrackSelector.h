#ifndef Analysis_TrackSelector_H
#define Analysis_TrackSelector_H


// * ========================================= * //
// * ------- DOXYGEN GROUP DEFINITIONS ------- * //
// * ========================================= * //

	/**
	 * @defgroup BOSS BOSS
	 * @brief <b>Classes and packages for the <i>BOSS framework</i></b>. The core of these packages is the `TrackSelector` algorithm, which is the base class for all subalgorithms, such as `D0phi_KpiKK`. Supporting these algorithms are helper classers, such as the `CutObject`.
	 */

	/**
	 * @defgroup BOSS_packages Event selection
	 * @ingroup BOSS
	 * @brief Event selection __packages__ for the BOSS framework. The core of these packages is the `TrackSelector` algorithm, which is the base class for all subalgorithms, such as `D0phi_KpiKK`.
	 */

	/**
	 * @defgroup BOSS_objects Helper objects
	 * @ingroup BOSS
	 * @brief Helper __classes__ for the event selection packages. An example would be the `CutObject`, which is can be used in all packages for applying cuts and storing their paramters in the final ROOT output file.
	 */

	/**
	 * @defgroup BOSS_globals Globals
	 * @ingroup BOSS
	 * @brief Global __parameters__ encapsulated in namespaces.
	 */



// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "CLHEP/Geometry/Point3D.h"
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
	#include "McTruth/McParticle.h"
	#include "McTruth/McEvent.h"
	#include "TH1D.h"
	#include "THStack.h"
	#include "TofRecEvent/RecTofTrack.h"
	#include "TrackSelector/CutObject.h"
	#include "VertexFit/KalmanKinematicFit.h"
	#include <map> /// @todo It would be more efficient to use `unordered_map`, but this requires a newer version of `gcc`.
	#include <string>
	#include <vector>

	#ifndef ENABLE_BACKWARDS_COMPATIBILITY
		typedef HepGeom::Point3D<double> HepPoint3D;
	#endif



// * ==================================== * //
// * ------- GLOBALS AND TYPEDEFS ------- * //
// * ==================================== * //
/// @addtogroup BOSS_globals
/// @{

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
	};

/// @}
// * ================================ * //
// * ------- CLASS DEFINITION ------- * //
// * ================================ * //
/// @addtogroup BOSS_packages
/// @{

	/**
	 * @brief    <b>Base algorithm</b> that creates a selection of charged and/or neutral tracks. Your analysis has to be defined in <i>derived</i> algorithms, making use of the vectors that are filled in this algorithm. See the <a href="https://besiii.gitbook.io/boss/the-boss-afterburner/initial">tutorial pages</a> for more information.
	 *
	 * @author   Remco de Boer 雷穆克 (r.e.deboer@students.uu.nl or remco.de.boer@ihep.ac.cn)
	 * @date     December 14th, 2018
	 */
	class TrackSelector : public Algorithm
	{
	public:
		/// @name Constructors
			///@{
			TrackSelector(const std::string &name, ISvcLocator* pSvcLocator);
			///@}

		/// @name Gaudi `Algorithm` steps
			///@{
			StatusCode initialize();
			StatusCode execute();
			StatusCode finalize();
			///@}


	protected:

		/// @name Derived `Algorithm` steps
			/// @details These steps are `virtual`, because they have to be defined in the derived algorithms.
			///@{
			virtual StatusCode initialize_rest() = 0; //!< This function is executed at the end of `initialize`. It should be further defined in derived subalgorithms.
			virtual StatusCode execute_rest()    = 0; //!< This function is executed at the end of `execute`. It should be further defined in derived subalgorithms.
			virtual StatusCode finalize_rest()   = 0; //!< This function is executed at the end of `finalize`. It should be further defined in derived subalgorithms.
			///@}


		/// @name `NTuple` methods
			///@{
			void AddItemsToNTuples(const char* tupleName, std::map<std::string, NTuple::Item<double> > &map, const char* tupleTitle="ks N-Tuple example");
			void AddItemsToNTuples(NTuplePtr nt, std::map<std::string, NTuple::Item<double> > &map);
			void BookNtupleItems_McTruth(const char* tupleName, std::map<std::string, NTuple::Item<double> > &map, const char* tupleTitle="Monte Carlo truth");
			void BookNtupleItems_Dedx   (const char* tupleName, std::map<std::string, NTuple::Item<double> > &map, const char* tupleTitle="dE/dx info");
			void BookNtupleItems_Tof    (const char* tupleName, std::map<std::string, NTuple::Item<double> > &map, const char* tupleTitle="Time-of-Flight info");
			///@}


		/// @name Write methods
			///@{
			void WriteMcTruth(Event::McParticle* mcTruth, const char* tupleName, std::map<std::string, NTuple::Item<double> > &map);
			void WriteDedxInfo(EvtRecTrack* evtRecTrack, const char* tupleName, std::map<std::string, NTuple::Item<double> > &map);
			void WriteDedxInfoForVector(std::vector<EvtRecTrack*> &vector, const char* tupleName, std::map<std::string, NTuple::Item<double> > &map);
			void WritePIDInformation();
			void WriteTofInformation(SmartRefVector<RecTofTrack>::iterator iter_tof, double ptrk, const char* tupleName, std::map<std::string, NTuple::Item<double> > &map);
			///@}


		/// @name Access to the DST file
			///@{
			SmartDataPtr<Event::EventHeader> fEventHeader;  //!< Data pointer for `"Event/EventHeader"` data. It is set in `execute()` step in each event.
			SmartDataPtr<Event::McParticleCol> fMcParticleCol; //!< Data pointer for `"Event/MC/McParticleCol"` data. It is set in `execute()` step in each event.
			SmartDataPtr<EvtRecEvent> fEvtRecEvent;  //!< Data pointer for `EventModel::EvtRec::EvtRecEvent` which is set in `execute()` in each event.
			SmartDataPtr<EvtRecTrackCol> fEvtRecTrkCol; //!< Data pointer for `EventModel::EvtRec::EvtRecTrackCol` which is set in `execute()` in each event.
			///@}


		/// @name Track collections and iterators
			///@{
			std::vector<Event::McParticle*> fMcParticles; //!< Vector that, in each event, will be filled by a selection of pointers to MC particles that are of interest. @todo Consider defining a `map<int, vector<Event::McParticle*> >`, where the `int` key will be the PDG code. This mapping is to define a subset of the list of `McParticles`.
			std::vector<EvtRecTrack*> fGoodChargedTracks; //!< Vector that, in each event, will be filled by a selection of pointers to 'good' charged tracks.
			std::vector<EvtRecTrack*> fGoodNeutralTracks; //!< Vector that, in each event, will be filled by a selection of pointers to 'good' neutral tracks (photons).
			std::vector<EvtRecTrack*>::iterator fTrackIterator; //!< Iterator for looping over the collection of charged and neutral tracks (`EvtRecTrackCol`).
			///@}


		/// @name Reconstructed track data
			///@{
			RecEmcShower   *fTrackEMC;  //!< Pointer to reconstructed data from the EMC. See more info in <a href="http://bes3.to.infn.it/Boss/7.0.2/html/classRecEmcShower.html">RecEmcShower</a>.
			RecExtTrack    *fTrackExt;  //!< Pointer to reconstructed data from the extension through all detectors. See more info in <a href="http://bes3.to.infn.it/Boss/7.0.2/html/classRecExtTrack.html">RecExtTrack</a>.
			RecMdcDedx     *fTrackDedx; //!< Pointer to reconstructed \f$dE/dx\f$ data from the MDC. See more info in <a href="http://bes3.to.infn.it/Boss/7.0.2/html/classRecMdcDedx.html">RecMdcDedx</a>.
			RecMdcKalTrack *fTrackKal;  //!< Pointer to reconstructed Kalman fit data from the MDC. See more info in <a href="http://bes3.to.infn.it/Boss/7.0.2/html/classRecMdcKalTrack.html">RecMdcKalTrack</a>.
			RecMdcTrack    *fTrackMDC;  //!< Pointer to reconstructed track data from the MDC. See more info in <a href="http://bes3.to.infn.it/Boss/7.0.2/html/classRecMdcTrack.html">RecMdcTrack</a>.
			RecMucTrack    *fTrackMUC;  //!< Pointer to reconstructed data from the MUC. See more info in <a href="http://bes3.to.infn.it/Boss/7.0.2/html/classRecMucTrack.html">RecMucTrack</a>.
			RecTofTrack    *fTrackTOF;  //!< Pointer to reconstructed data from the TOF. See more info in <a href="http://bes3.to.infn.it/Boss/7.0.2/html/classRecTofTrack.html">RecTofTrack</a>.
			///@}


		/// @name Control switches
			/// @details These `bool`eans are used to switch certain features of the `TrackSelector` on or of. For instance, if you do not analyse photons, you should switch of the `fDo_neutral` member. This is done through the declaration of the corresponding property in the constructor and by defining the value in the job options under `share`.
			///@{
			bool fDo_charged; //!< Package property that determines whether or not to make a selection of 'good' charged tracks.
			bool fDo_neutral; //!< Package property that determines whether or not to make a selection of 'good' neutral tracks.
			bool fWrite_mctruth; //!< Package property that determines whether or not to record initial momentum informtation of MC truth.
			bool fWrite_PID;     //!< Package property that determines whether or not to record general PID information (TOF, \f$dE/dx\f$, etc).
			bool fWrite_ToFEC;   //!< Package property that determines whether or not to record data from the Time-of-Flight end cap detector.
			bool fWrite_ToFIB;   //!< Package property that determines whether or not to record data from the Time-of-Flight inner barrel detector.
			bool fWrite_ToFOB;   //!< Package property that determines whether or not to record data from the Time-of-Flight outer barrel detector.
			bool fWrite_charged; //!< Package property that determines whether or not to record charged track vertex info.
			bool fWrite_dedx;    //!< Package property that determines whether or not to record energy loss (\f$dE/dx\f$).
			bool fWrite_mult;    //!< Package property that determines whether or not to record multiplicities.
			bool fWrite_mult_select; //!< Package property that determines whether or not to write the multiplicities <i>of the selected particles</i>.
			bool fWrite_neutral; //!< Package property that determines whether or not to record neutral track info.
			bool fWrite_vertex;  //!< Package property that determines whether or not to record primary vertex info.
			///@}


		// ! NTuple data members ! //
		/// @name `NTuple`s
			/// @details `NTuple`s are like vectors, but its members do not necessarily have to be of the same type. In this package, the NTuples are used to store event-by-event information. Its values are then written to the output ROOT file, creating a ROOT TTree. In that sense, each NTuple here represents one TTree within that output ROOT file, and each `NTuple::Item` represents its leaves. The name of the leaves is determined when calling `NTuple::addItem`.
			/// @warning Note that the `NTuple::Items` have to be added to the NTuple during the `TrackSelector::initialize()` step, otherwise they cannot be used as values! This is also the place where you name these variables, so make sure that the structure here is reflected there!
			///@{
			std::map<std::string, NTuple::Item<double> > fMap_cuts;    //!< Container for the cut parameters.
			std::map<std::string, NTuple::Item<double> > fMap_PID;     //!< Container for the general PID information (TOF, \f$dE/dx\f$, etc) branch. <b>Needs to be filled in the derived class!</b>
			std::map<std::string, NTuple::Item<double> > fMap_TofEC;   //!< Container for the data from the Time-of-Flight end cap detector branch.
			std::map<std::string, NTuple::Item<double> > fMap_TofIB;   //!< Container for the data from the Time-of-Flight inner barrel detector branch.
			std::map<std::string, NTuple::Item<double> > fMap_TofOB;   //!< Container for the data from the Time-of-Flight outer barrel detector branch.
			std::map<std::string, NTuple::Item<double> > fMap_charged; //!< Container for the charged track vertex info charged track vertex branch.
			std::map<std::string, NTuple::Item<double> > fMap_dedx;    //!< Container for the energy loss (\f$dE/dx\f$) branch.
			std::map<std::string, NTuple::Item<double> > fMap_mctruth; //!< Container for MC truth info.
			std::map<std::string, NTuple::Item<double> > fMap_mult;    //!< Container for the multiplicities branch.
			std::map<std::string, NTuple::Item<double> > fMap_mult_select; //!< Container for the `"mult_select"` branch.
			std::map<std::string, NTuple::Item<double> > fMap_neutral; //!< Container for the neutral track info neutral track info branch.
			std::map<std::string, NTuple::Item<double> > fMap_vertex;  //!< Container for the primary vertex info vertex branch.
			///@}


		/// @name Counters and `CutObject`s
			/// The private data members are used to define cuts. The values for these cuts should be set in the `TrackSelector::TrackSelector` constructor (see corresponding `.cxx` file).
			///@{
			NTuple::Tuple* fCutTuples;
			CutObject fCounter_Nevents;   //!< Eventual total number of events.
			CutObject fCounter_Ntracks;   //!< Eventual total number of tracks.
			CutObject fCounter_Ncharged;  //!< Eventual total number of charged tracks.
			CutObject fCounter_Nmdcvalid; //!< Eventual total number of charged tracks that are 'MDC valid'.
			CutObject fCounter_Nneutral;  //!< Eventual total number of neutral tracks.
			CutObject fCut_Vxy; //!< Cut on the radius \f$r\f$ of the primary vertex.
			CutObject fCut_Vz;  //!< Cut on the \f$z\f$ coordinate of the primary vertex.
			CutObject fCut_Rxy; //!< Cut on the distance in the \f$z\f$ direction between the primary vertex and the vertex of the charged track.
			CutObject fCut_Rz;  //!< Cut on the distance in the \f$xy\f$ plane between the primary vertex and the vertex of the charged track.
			CutObject fCut_CosTheta; //!< Cut on \f$\cos(\theta)\f$.
			CutObject fCut_PhotonEnergy; //!< Cut on the photon energy.
			CutObject fCut_PIDChiSq; //!< Cut on the \f$\chi_\mathrm{red}^2\f$ of the kinematic Kalman fits
			CutObject fCut_PIDProb; //!< Cut on the probability that a particle is either a kaon, pion, electron, muon, or proton according to the probability method. See for instance <a href="http://bes3.to.infn.it/Boss/7.0.2/html/classParticleID.html#147bb7be5fa47f275ca3b32e6ae8fbc6">`ParticleID::probPion`</a>.
			///@}


		// * Remaining members and methods * //
			ParticleID* InitializePID(const int method, const int pidsys, const int pidcase, const double chimin=4.);
			MsgStream fLog; //!< Stream object for logging. It needs to be declared as a data member so that it is accessible to all methods of this class.
			ParticleID *fPIDInstance; //!< Pointer to instance of particle identification (PID). Only used in <i>derived subalgorithms</i>.
			std::map<std::string, NTuple::Tuple*> fNTupleMap; //!< Map for `NTuple::Tuple*`s. The string identifier should be the name of the `NTuple` and of the eventual `TTree`.
			HepPoint3D fVertexPoint; //!< Coordinates of the interaction point (primary vertex). Set in each event in `TrackSelector::execute`.
			Int_t fNChargesMDC; //!< Number of charges detected in the MDC.


	private:
		void DeclareCuts();
		void WriteCuts();
		HepLorentzVector ComputeMomentum(EvtRecTrack *track);
		NTuplePtr BookNTuple(const char* tupleName, const char* tupleTitle="ks N-Tuple example");

	};


/// @}
#endif