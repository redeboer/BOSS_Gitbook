// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "CLHEP/Geometry/Point3D.h"
	#include "CLHEP/Vector/LorentzVector.h"
	#include "CLHEP/Vector/ThreeVector.h"
	#include "CLHEP/Vector/TwoVector.h"
	#include "DstEvent/TofHitStatus.h"
	#include "EventModel/Event.h"
	#include "EventModel/EventModel.h"
	#include "GaudiKernel/Bootstrap.h"
	#include "TMath.h"
	#include "TString.h"
	#include "TrackSelector/TrackSelector.h"
	#include "VertexFit/Helix.h"
	#include "VertexFit/IVertexDbSvc.h"

	#ifndef ENABLE_BACKWARDS_COMPATIBILITY
		typedef HepGeom::Point3D<double> HepPoint3D;
	#endif

	using CLHEP::Hep2Vector;
	using CLHEP::Hep3Vector;
	using CLHEP::HepLorentzVector;
	using namespace TSGlobals;



// * ==================================== * //
// * ------- GLOBALS AND TYPEDEFS ------- * //
// * ==================================== * //

	const double gMasses[] {
		0.000511, // electron
		0.105658, // muon
		0.139570, // charged pion
		0.493677, // charged kaon
		0.938272  // proton
	};
	const int gNMasses = sizeof(gMasses)/sizeof(*gMasses);
	const double gSpeedOfLight = 299.792458; // tof path unit in mm



// * =========================== * //
// * ------- CONSTRUCTOR ------- * //
// * =========================== * //
	/**
	 * @brief Constructor for the `TrackSelector` algorithm.
	 * @details Here, you should declare properties: give them a name, assign a parameter (data member of `TrackSelector`), and if required a documentation string. Note that you should define the paramters themselves in the header (TrackSelector/TrackSelector.h) and that you should assign the values in `share/jopOptions_TrackSelector.txt`. Algorithms should inherit from Gaudi's `Algorithm` class. See https://dayabay.bnl.gov/dox/GaudiKernel/html/classAlgorithm.html for more details.
	 */
	TrackSelector::TrackSelector(const std::string &name, ISvcLocator* pSvcLocator) :
		Algorithm(name, pSvcLocator),
		fLog(msgSvc(), name),
		fEventHeader (eventSvc(), "/Event/EventHeader"),
		fEvtRecEvent (eventSvc(), EventModel::EvtRec::EvtRecEvent),
		fEvtRecTrkCol(eventSvc(), EventModel::EvtRec::EvtRecTrackCol)
	{
		fLog << MSG::DEBUG << "===>> TrackSelector::TrackSelector() <<===" << endmsg;

		/// * The `"do_<treename>"` properties determine whether or not the corresponding `TTree`/`NTuple` will be filled. Default values are set in the constructor as well.
		declareProperty("do_mult",        fDo_mult        = false);
		declareProperty("do_vertex",      fDo_vertex      = false);
		declareProperty("do_charged",     fDo_charged     = false);
		declareProperty("do_neutral",     fDo_neutral     = false);
		declareProperty("do_dedx",        fDo_dedx        = false);
		declareProperty("do_ToFEC",       fDo_ToFEC       = false);
		declareProperty("do_ToFIB",       fDo_ToFIB       = false);
		declareProperty("do_ToFOB",       fDo_ToFOB       = false);
		declareProperty("do_PID",         fDo_PID         = false);
		declareProperty("do_mult_select", fDo_mult_select = false);

		/// * The `"cut_<parameter>"` properties determine cuts on certain parameters.
		declareProperty("cut_MaxVr0",          fCut_MaxVr0          = 1.);
		declareProperty("cut_MaxVz0",          fCut_MaxVz0          = 5.);
		declareProperty("cut_MaxVrvz0",        fCut_MaxRvz0         = 10.);
		declareProperty("cut_MaxVrvxy0",       fCut_MaxRvxy0        = 1.);
		declareProperty("cut_MinPhotonEnergy", fCut_MinPhotonEnergy = 0.04);
		declareProperty("cut_MaxPIDChiSq",     fCut_MaxPIDChiSq     = 200.);
		declareProperty("cut_MinPIDProb",      fCut_MinPIDProb      = 0.001);

	}



// * =============================== * //
// * ------- ALGORITHM STEPS ------- * //
// * =============================== * //

	/**
	 * @brief   (Inherited) `initialize` step of `Algorithm`. This function is called only once in the beginning.
	 * @details Define and load NTuples here. The `NTuples` will become the `TTree`s in the eventual ROOT file, the added `NTuple::Item`s will be the branches of those trees.
	 */
	StatusCode TrackSelector::initialize()
	{
		fLog << MSG::INFO << "===>> TrackSelector::initialize() <<===" << endmsg;

		/// <ol type="A">
		/// <li> `"mult"`: Multiplicities of the total event
			/// <ol>
			if(fDo_mult) {
				fMap_mult["Ntotal"];       /// <li> `"Ntotal"`:       Total number of events per track.
				fMap_mult["Ncharge"];      /// <li> `"Ncharge"`:      Number of charged tracks.
				fMap_mult["Nneutral"];     /// <li> `"Nneutral"`:     Number of charged tracks.
				fMap_mult["NgoodCharged"]; /// <li> `"NgoodCharged"`: Number of 'good' charged tracks.
				fMap_mult["NgoodNeutral"]; /// <li> `"NgoodNeutral"`: Number of 'good' neutral tracks.
				fMap_mult["Nmdc"];         /// <li> `"Nmdc"`:         Number of charged tracks in MDC.
				AddItemsToNTuples("mult", fMap_mult, "Event multiplicities");
			}
			/// </ol>

		/// <li> `"vertex"`: Vertex info
			/// <ol>
			if(fDo_vertex) {
				fMap_vertex["vx0"]; /// <li> `"vx0"`: Primary \f$x\f$ coordinate of the collision point.
				fMap_vertex["vy0"]; /// <li> `"vy0"`: Primary \f$y\f$ coordinate of the collision point.
				fMap_vertex["vz0"]; /// <li> `"vz0"`: Primary \f$z\f$ coordinate of the collision point.
				AddItemsToNTuples("vertex", fMap_vertex, "Primary vertex");
			}
			/// </ol>

		/// <li> `"charged"`: Charged track info.
			/// <ol>
			if(fDo_charged) {
				fMap_charged["vx"];    /// <li> `"vx"`:    Primary \f$x\f$ coordinate of the vertex as determined by MDC.
				fMap_charged["vy"];    /// <li> `"vy"`:    Primary \f$y\f$ coordinate of the vertex as determined by MDC.
				fMap_charged["vz"];    /// <li> `"vz"`:    Primary \f$z\f$ coordinate of the vertex as determined by MDC.
				fMap_charged["vr"];    /// <li> `"vr"`:    Distance from origin in \f$xy\f$ plane.
				fMap_charged["rvxy"];  /// <li> `"rvxy"`:  Nearest distance to IP in \f$xy\f$ plane.
				fMap_charged["rvz"];   /// <li> `"rvz"`:   Nearest distance to IP in \f$z\f$ direction.
				fMap_charged["rvphi"]; /// <li> `"rvphi"`: Angle in the \f$xy\f$plane. @todo Get explanation of geometry (angle) definitions in an event.
				fMap_charged["phi"];   /// <li> `"phi"`:   Helix angle of the particle.
				fMap_charged["p"];     /// <li> `"p"`:     Momentum \f$p\f$ of the track.
				AddItemsToNTuples("charged", fMap_charged, "Charged track info");
			}
			/// </ol>

		/// <li> `"neutral"`: Neutral track info.
			/// <ol>
			if(fDo_neutral) {
				fMap_neutral["E"];     /// <li> `"E"`: Energy of the neutral track as determined by the EM calorimeter.
				fMap_neutral["x"];     /// <li> `"x"`: \f$x\f$-coordinate of the neutral track according to the EMC.
				fMap_neutral["y"];     /// <li> `"y"`: \f$y\f$-coordinate of the neutral track according to the EMC.
				fMap_neutral["z"];     /// <li> `"z"`: \f$z\f$-coordinate of the neutral track according to the EMC.
				fMap_neutral["phi"];   /// <li> `"phi"`: \f$\phi\f$-angle of the neutral track according to the EMC.
				fMap_neutral["theta"]; /// <li> `"theta"`: \f$\theta\f$-angle of the neutral track according to the EMC.
				fMap_neutral["time"];  /// <li> `"time"`: Time of the neutral track according to the EMC. @todo Investigate what this parameter precisely means.
				AddItemsToNTuples("neutral", fMap_neutral, "Neutral track info");
			}
			/// </ol>

		/// <li> `"dedx"`: energy loss \f$dE/dx\f$ PID branch. See `TrackSelector::BookNtupleItemsDedx` for more info.
			if(fDo_dedx) {
				BookNtupleItemsDedx("dedx", fMap_dedx, "dE/dx of all charged tracks");
			}

		/// <li> `"ToFEC"`, `"ToFIB"`, and `"ToFOB"`: information from the three Time-of-Flight detectors. See `TrackSelector::BookNtupleItemsTof` for more info.
			if(fDo_ToFEC) BookNtupleItemsTof("ToFEC", fMap_TofEC, "End cap ToF of all tracks");
			if(fDo_ToFIB) BookNtupleItemsTof("ToFIB", fMap_TofIB, "Inner barrel ToF of all tracks");
			if(fDo_ToFOB) BookNtupleItemsTof("ToFOB", fMap_TofOB, "Outer barrel ToF of all tracks");

		/// <li> `"PID"`: Track PID information.
			/// <ul>
			if(fDo_PID) {
				fMap_PID["p"];        /// <li> `"p"]; `:       Momentum of the track as reconstructed by MDC.
				fMap_PID["cost"];     /// <li> `"cost"`:    Theta angle of the track.
				fMap_PID["chiToFIB"]; /// <li> `"chiToFIB"`:   \f$\chi^2\f$ of the inner barrel ToF of the track.
				fMap_PID["chiToFEC"]; /// <li> `"chiToFEC"`:   \f$\chi^2\f$ of the end cap ToF of the track.
				fMap_PID["chiToFOB"]; /// <li> `"chiToFOB"`:   \f$\chi^2\f$ of the outer barrel ToF of the track.
				fMap_PID["chidEdx"];  /// <li> `"chidEdx"`: \f$\chi^2\f$ of the energy loss \f$dE/dx\f$ of the identified track.
				fMap_PID["prob_K"];   /// <li> `"prob_K"`:  Probability that the track is from a kaon according to the probability method.
				fMap_PID["prob_e"];   /// <li> `"prob_e"`:  Probability that the track is from a electron according to the probability method.
				fMap_PID["prob_mu"];  /// <li> `"prob_mu"`: Probability that the track is from a muon according to the probability method.
				fMap_PID["prob_p"];   /// <li> `"prob_p"`:  Probability that the track is from a proton according to the probability method.
				fMap_PID["prob_pi"];  /// <li> `"prob_pi"`: Probability that the track is from a pion according to the probability method.
				AddItemsToNTuples("PID", fMap_PID, "Particle Identification parameters");
			}
			/// </ul>

		/// </ol>
		if(initialize_rest() == StatusCode::SUCCESS) return StatusCode::SUCCESS;
		fLog << MSG::INFO << "Successfully returned from TrackSelector::initialize()" << endmsg;
		return StatusCode::SUCCESS;
	}


	/**
	 * @brief Inherited `execute` method of the `Algorithm`. This function is called *for each event*.
	 */
	StatusCode TrackSelector::execute()
	{
		fLog << MSG::DEBUG << "===>> TrackSelector::execute() <<===" << endmsg;

		/// <ol type="A">
		/// <li> Load next event from DST file. For more info see:
			/// <ul>
				/// <li> <a href="http://bes3.to.infn.it/Boss/7.0.2/html/namespaceEventModel_1_1EvtRec.html">Namespace `EventModel`</a>
				/// <li> <a href="http://bes3.to.infn.it/Boss/7.0.2/html/classEvtRecEvent.html">Class `EvtRecEvent`</a>
				/// <li> <a href="http://bes3.to.infn.it/Boss/7.0.2/html/EvtRecTrack_8h.html">Type definition `EvtRecTrackCol`</a>
			/// </ul>

			// * Load headers
			fLog << MSG::DEBUG << "Loading EventHeader, EvtRecEvent, and EvtRecTrackCol" << endmsg;
			fEventHeader  = SmartDataPtr<Event::EventHeader>(eventSvc(), "/Event/EventHeader");
			fEvtRecEvent  = SmartDataPtr<EvtRecEvent>       (eventSvc(), EventModel::EvtRec::EvtRecEvent);
			fEvtRecTrkCol = SmartDataPtr<EvtRecTrackCol>    (eventSvc(), EventModel::EvtRec::EvtRecTrackCol);

			// * Log run number, event number, and number of events *
			fLog << MSG::DEBUG
				<< "RUN "          << fEventHeader->runNumber()   << ", "
				<< "event number " << fEventHeader->eventNumber() << endmsg;
			fLog << MSG::DEBUG
				<< "Ncharged = " << fEvtRecEvent->totalCharged() << ", "
				<< "Nneutral = " << fEvtRecEvent->totalNeutral() << ", "
				<< "Ntotal = "   << fEvtRecEvent->totalTracks()  << endmsg;

			// * Set vertex origin *
				IVertexDbSvc* vtxsvc;
				Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);
				double v0x, v0y, v0z;
				if(vtxsvc->isVertexValid()){
					double* dbv = vtxsvc->PrimaryVertex();
					double* vv = vtxsvc->SigmaPrimaryVertex();
					v0x = dbv[0];
					v0y = dbv[1];
					v0z = dbv[2];
				}


		/// <li> Create selection charged tracks and write track info:
			/// Note: the first part of the set of reconstructed tracks are the charged tracks.

			// * Set counters and data members *
			int nChargesMDC = 0;
			fGoodChargedTracks.clear();
			fPIDInstance = ParticleID::instance();

			// * Only perform if there are charged tracks *
			if(fEvtRecEvent->totalCharged()) {

				// * Loop over charged tracks *
				fLog << MSG::DEBUG << "Starting 'good' charged track selection:" << endmsg;
				for(int i = 0; i < fEvtRecEvent->totalCharged(); ++i) {
					/// <ol>
					/// <li> Get MDC information

						// * Get track info from Main Drift Chamber
						fLog << MSG::DEBUG << "   charged track " << i << "/" << fEvtRecEvent->totalCharged() << endmsg;
						fTrackIterator = fEvtRecTrkCol->begin() + i; 
						if(!(*fTrackIterator)->isMdcTrackValid()) continue;
						fTrackMDC = (*fTrackIterator)->mdcTrack();

						// * Get kinematics of track
						double phi  = fTrackMDC->helix(1);
						double vr =
							(fTrackMDC->x() - v0x) * cos(phi) +
							(fTrackMDC->y() - v0y) * sin(phi);

						// * Get radii of track vertex
						HepVector a = fTrackMDC->helix();
						HepSymMatrix Ea = fTrackMDC->err();
						HepPoint3D point0(0., 0., 0.); // initial point for MDC reconstruction
						HepPoint3D IP(v0x, v0y, v0z);
						VFHelix helixip(point0, a, Ea);
						helixip.pivot(IP);
						HepVector vecipa = helixip.a();
						double rvxy  = fabs(vecipa[0]); // nearest distance to IP in xy plane
						double rvz    = vecipa[3];       // nearest distance to IP in z direction
						double rvphi = vecipa[1];       // angle in the xy-plane (?)

					/// <li> Apply vertex cuts, store 

						// * Apply vertex cuts
						if(fTrackMDC->z() >= fCut_MaxVz0)   continue;
						if(vr             >= fCut_MaxVr0)   continue;
						if(rvz            >= fCut_MaxRvz0)  continue;
						if(rvxy           >= fCut_MaxRvxy0) continue;

						// * Add charged track to vector
						fGoodChargedTracks.push_back(*fTrackIterator);
						nChargesMDC += fTrackMDC->charge(); // @todo Check if this makes sense at all

					/// <li> <b>Write</b> charged track vertex position info ("charged" branch)
						if(fDo_charged) {
							fMap_charged.at("vx")    = fTrackMDC->x();
							fMap_charged.at("vy")    = fTrackMDC->y();
							fMap_charged.at("vz")    = fTrackMDC->z();
							fMap_charged.at("vr")    = vr;
							fMap_charged.at("rvxy")  = rvxy;
							fMap_charged.at("rvz")   = rvz;
							fMap_charged.at("rvphi") = rvphi;
							fMap_charged.at("phi")   = phi;
							fMap_charged.at("p")     = fTrackMDC->p();
							fNTupleMap.at("charged")->write();
						}

					/// <li> <b>Write</b> dE/dx PID information ("dedx" branch)
						if(fDo_dedx) WriteDedxInfo(*fTrackIterator, "dedx", fMap_dedx);

					/// <li> <b>Write</b> Time-of-Flight PID information ("tof*" branch)
						if(fDo_ToFEC || fDo_ToFIB || fDo_ToFOB) {

							// * Check if MDC and TOF info for track are valid *
							if(!(*fTrackIterator)->isMdcTrackValid()) continue;
							if(!(*fTrackIterator)->isTofTrackValid()) continue;

							// * Get momentum as determined by MDC *
							fTrackMDC = (*fTrackIterator)->mdcTrack();
							double ptrk;
							if(fTrackMDC) ptrk = fTrackMDC->p();
							SmartRefVector<RecTofTrack> tofTrkCol = (*fTrackIterator)->tofTrack();
							SmartRefVector<RecTofTrack>::iterator iter_tof = tofTrkCol.begin();
							for(; iter_tof != tofTrkCol.end(); ++iter_tof) {
								TofHitStatus hitStatus;
								hitStatus.setStatus((*iter_tof)->status());
								if(!hitStatus.is_counter()) continue;
								if(hitStatus.is_barrel()) {
									if(hitStatus.layer() == 1) { // inner barrel
										if(fDo_ToFIB) WriteTofInformation(iter_tof, ptrk, "ToFIB", fMap_TofIB);
									} else if(hitStatus.layer() == 2) { // outer barrel
										if(fDo_ToFOB) WriteTofInformation(iter_tof, ptrk, "ToFOB", fMap_TofOB);
									}
								}
								else if(fDo_ToFEC && hitStatus.layer() == 0) // end cap
									WriteTofInformation(iter_tof, ptrk, "ToFEC", fMap_TofEC);
							}

						} // if(fDo_tofec || fDo_tofib || fDo_tofob)

					/// </ol>
				}

				// * Finish good charged track selection *
				fLog << MSG::DEBUG << "Number of 'good' charged tracks: " << fGoodChargedTracks.size() << endmsg;
			}


		/// <li> Create selection of neutral tracks and write track info.
			/// Note: The second part of the set of reconstructed events consists of the neutral tracks, that is, the photons detected by the EMC (by clustering EMC crystal energies). Each neutral track is paired with each charged track and if their angle is smaller than a certain value (here, 200), the photon track is stored as 'good photon' (added to `iGam`).

			// * Set counters and data members *
			fGoodNeutralTracks.clear();

			// * Only perform if there are neutral tracks *
			if(fEvtRecEvent->totalNeutral()) {

				// * Loop over neutral tracks *
				fLog << MSG::DEBUG << "Starting 'good' neutral track selection:" << endmsg;
				for(int i = fEvtRecEvent->totalCharged(); i < fEvtRecEvent->totalTracks(); ++i) {
					/// <ol>
					/// <li> Get EMC information
						fLog << MSG::DEBUG << "   neutral track " << i-fEvtRecEvent->totalCharged() << "/" << fEvtRecEvent->totalNeutral() << endmsg;
						fTrackIterator = fEvtRecTrkCol->begin() + i; 
						if(!(*fTrackIterator)->isEmcShowerValid()) continue;
						fTrackEMC = (*fTrackIterator)->emcShower();
						if(!fTrackEMC) continue;

					/// <li> Apply photon energy cut (set by `TrackSelector.cut_PhotonEnergy`).
						if(fTrackEMC->energy() < fCut_MinPhotonEnergy) continue;

					/// <li> <b>Write</b> neutral track information (if `do_neutral` is set to `true`).
						if(fDo_neutral) {
							fMap_neutral.at("E")     = fTrackEMC->energy();
							fMap_neutral.at("x")     = fTrackEMC->x();
							fMap_neutral.at("y")     = fTrackEMC->y();
							fMap_neutral.at("z")     = fTrackEMC->z();
							fMap_neutral.at("phi")   = fTrackEMC->phi();
							fMap_neutral.at("theta") = fTrackEMC->theta();
							fMap_neutral.at("time")  = fTrackEMC->time();
							fNTupleMap.at("neutral")->write();
						}

					/// <li> Add photon track to vector of neutral tracks (`fGoodNeutralTracks`).
						fGoodNeutralTracks.push_back(*fTrackIterator);

					/// </ol>
				}

				// * Finish good photon selection *
				fLog << MSG::DEBUG << "Number of 'good' photons: " << fGoodNeutralTracks.size() << endmsg;
			}


		/// <li> <b>write</b> event info (`"mult"` branch)
			if(fDo_mult) {
				fMap_mult.at("Ntotal")       = fEvtRecEvent->totalTracks();
				fMap_mult.at("Ncharge")      = fEvtRecEvent->totalCharged();
				fMap_mult.at("Nneutral")     = fEvtRecEvent->totalNeutral();
				fMap_mult.at("NgoodCharged") = fGoodChargedTracks.size();
				fMap_mult.at("NgoodNeutral") = fGoodNeutralTracks.size();
				fMap_mult.at("Nmdc")         = nChargesMDC;
				fNTupleMap.at("mult")->write();
			}

		/// <li> <b>write</b> event info (`"vertex"` branch)
			if(fDo_vertex) {
				fMap_vertex.at("vx0") = v0x;
				fMap_vertex.at("vy0") = v0y;
				fMap_vertex.at("vz0") = v0z;
				fNTupleMap.at("vertex")->write();
			}

		/// <li> Perform derived algoritm as defined in `TrackSelector::execute_rest`.
			if(execute_rest() == StatusCode::SUCCESS) return StatusCode::SUCCESS;

		/// </ol>
		return StatusCode::SUCCESS;
	}


	/**
	 * @brief Currently does nothing. Cut flow could be printed in this step.
	 * @todo Add log output to `finalize` step.
	 */
	StatusCode TrackSelector::finalize()
	{
		fLog << MSG::INFO << "===>> TrackSelector::finalize() <<===" << endmsg;

		if(finalize_rest() == StatusCode::SUCCESS) return StatusCode::SUCCESS;
		fLog << MSG::INFO << "Successfully returned from TrackSelector::finalize()" << endmsg;
		return StatusCode::SUCCESS;
	}



// * ================================= * //
// * -------- PRIVATE METHODS -------- * //
// * ================================= * //

	/**
	 * @brief Compute a 'momentum' for a neutral track.
	 * @details The momentum is computed from the neutral track (photon) energy and from the location (angles) where it was detected in the EMC.
	 */
	HepLorentzVector TrackSelector::ComputeMomentum(EvtRecTrack *track)
	{
		RecEmcShower *emcTrk = track->emcShower();
		double eraw = emcTrk->energy();
		double phi  = emcTrk->phi();
		double the  = emcTrk->theta();
		HepLorentzVector ptrk(
			eraw * sin(the) * cos(phi), // px
			eraw * sin(the) * sin(phi), // py
			eraw * cos(the),            // pz
			eraw);
		// ptrk = ptrk.boost(-0.011, 0, 0); // boost to center-of-mass frame
		return ptrk;
	}

	/**
	 * @brief   Helper function that allows you to create pair of a string with a `NTuplePtr`.
	 * @details This function first attempts to see if there is already an `NTuple` in the output file. If not, it will book an `NTuple` of 
	 *
	 * @param tupleName This will not only be the name of your `NTuple`, but also the name of the `TTree` in the output ROOT file when you use `NTuple::write()`. The name used here is also used as identifier in `NTuplePtr` map `fNTupleMap`. In other words, you can get any of the `NTuplePtr`s in this map by using `fNTupleMap[<tuple name>]`. If there is no `NTuple` of this name, it will return a `nullptr`.
	 * @param tupleTitle Description of the `NTuple`. Has a default value if you don't want to think about this.
	 */
	NTuplePtr TrackSelector::BookNTuple(const char* tupleName, const char* tupleTitle)
	{
		const char* bookName = Form("FILE1/%s", tupleName);
		NTuplePtr nt(ntupleSvc(), bookName); // attempt to get from file
		if(!nt) { // if not available in file, book a new one
			fLog << MSG::INFO << "Booked NTuple \"" << tupleName << "\"" << endmsg;
			nt = ntupleSvc()->book(bookName, CLID_ColumnWiseTuple, tupleTitle);
			if(!nt) fLog << MSG::ERROR << "    Cannot book N-tuple:" << long(nt) << " (" << tupleName << ")" << endmsg;
		}
		fNTupleMap[tupleName] = nt.ptr(); /// Use `map::operator[]` if you want to book an `NTuple::Item` and use `map::at` if you want to access the `NTuple` by key value. This ensures that the programme throws an exception if you ask for the wrong key later.
		return nt;
	}


	/**
	 * @brief This function encapsulates the `addItem` procedure for the ToF branch. This allows to standardize the loading of the end cap, inner barrel, and outer barrel ToF branches.
	 */ 
	void TrackSelector::BookNtupleItemsTof(const char* tupleName, std::map<std::string, NTuple::Item<double> > &map, const char* tupleTitle)
	{
		/// <ol>
		map["p"];      /// <li> `"p"`:      Momentum of the track as reconstructed by MDC.
		map["tof"];    /// <li> `"tof"`:    Time of flight.
		map["path"];   /// <li> `"path"`:   Path length.
		map["cntr"];   /// <li> `"cntr"`:   ToF counter ID.
		map["ph"];     /// <li> `"ph"`:     ToF pulse height.
		map["zrhit"];  /// <li> `"zrhit"`:  Track extrapolate \f$Z\f$ or \f$R\f$ Hit position.
		map["qual"];   /// <li> `"qual"`:   Data quality of reconstruction.
		map["tof_e"];  /// <li> `"tof_e"`:  Difference with ToF in electron hypothesis.
		map["tof_mu"]; /// <li> `"tof_mu"`: Difference with ToF in muon hypothesis.
		map["tof_pi"]; /// <li> `"tof_pi"`: Difference with ToF in charged pion hypothesis.
		map["tof_K"];  /// <li> `"tof_K"`:  Difference with ToF in charged kaon hypothesis.
		map["tof_p"];  /// <li> `"tof_p"`:  Difference with ToF in proton hypothesis.
		AddItemsToNTuples(tupleName, map, tupleTitle);
		/// </ol>
	}


	/**
	 * @brief This function encapsulates the `addItem` procedure for the \f$dE/dx\f$ energy loss branch (`"dedx"`).
	 * @details This method allows you to perform the same booking method for different types of charged particles (for instance 'all charged particles', kaons, and pions).
	 */
	void TrackSelector::BookNtupleItemsDedx(const char* tupleName, std::map<std::string, NTuple::Item<double> > &map, const char* tupleTitle)
	{
		/// <ol>
		map["p"];      /// <li> `"p"`:      Momentum of the track as reconstructed by MDC.
		map["chie"];   /// <li> `"chie"`:   Chi squared in case of electron.
		map["chimu"];  /// <li> `"chimu"`:  Chi squared in case of muon.
		map["chipi"];  /// <li> `"chipi"`:  Chi squared in case of pion.
		map["chik"];   /// <li> `"chik"`:   Chi squared in case of kaon.
		map["chip"];   /// <li> `"chip"`:   Chi squared in case of proton.
		map["probPH"]; /// <li> `"probPH"`: Most probable pulse height from truncated mean.
		map["normPH"]; /// <li> `"normPH"`: Normalized pulse height.
		map["ghit"];   /// <li> `"ghit"`:   Number of good hits.
		map["thit"];   /// <li> `"thit"`:   Total number of hits.
		AddItemsToNTuples(tupleName, map, tupleTitle);
		/// </ol>
	}


	/**
	 * @brief
	 */
	void TrackSelector::WriteTofInformation(SmartRefVector<RecTofTrack>::iterator iter_tof, double ptrk, const char* tupleName, std::map<std::string, NTuple::Item<double> > &map)
	{

		// * Get ToF for each particle hypothesis
		double path = (*iter_tof)->path();
		std::vector<double> texp(gNMasses);
		for(size_t j = 0; j < texp.size(); ++j) {
			double gb = ptrk/gMasses[j]; // v = p/m (non-relativistic velocity)
			double beta = gb/sqrt(1+gb*gb);
			texp[j] = 10 * path /beta/gSpeedOfLight; // hypothesis ToF
		}

		// * <b>write</b> ToF info
		map.at("p")     = ptrk;
		map.at("tof")   = (*iter_tof)->tof();
		map.at("path")  = path;
		map.at("cntr")  = (*iter_tof)->tofID();
		map.at("ph")    = (*iter_tof)->ph();
		map.at("zrhit") = (*iter_tof)->zrhit();
		map.at("qual")  = (*iter_tof)->quality();
		map.at("tof_e")    = path - texp[0];
		map.at("tof_mu")   = path - texp[1];
		map.at("tof_pi")   = path - texp[2];
		map.at("tof_K")    = path - texp[3];
		map.at("tof_p")    = path - texp[4];
		fNTupleMap.at(tupleName)->write();

	}


	/**
	 * @brief Encapsulates a `for` loop of the writing procedure for \f$dE/dx\f$ energy loss information.
	 * @details This method allows you to write \f$dE/dx\f$ information for any selection of charged tracks. Just feed it a vector that contains such a collection of `EvtRecTrack` pointers.
	 * 
	 * @param vector The selection of charged tracks that of which you want to write the \f$dE/dx\f$ data.
	 * @param tupleName The name of the tuple to which you want to write the information.
	 * @param map The `map` from which you want to get the `NTuple::Item`s.
	 */
	void TrackSelector::WriteDedxInfoForVector(std::vector<EvtRecTrack*> &vector, const char* tupleName, std::map<std::string, NTuple::Item<double> > &map)
	{
		for(fTrackIterator = vector.begin(); fTrackIterator != vector.end(); ++fTrackIterator) {
			WriteDedxInfo(*fTrackIterator, tupleName, map);
		}
	}


	/**
	 * @brief Encapsulates of the writing procedure for \f$dE/dx\f$ energy loss information <i>for one track</i>.
	 * @details Here, you should use `map::at` to access the `NTuple::Item`s and `NTuplePtr`, because you want your package to throw an exception if the element does not exist.
	 * @param evtRecTrack Pointer to the reconstructed track of which you want to write the \f$dE/dx\f$ data.
	 * @param tupleName The name of the tuple to which you want to write the information.
	 * @param map The `map` from which you want to get the `NTuple::Item`s.
	 */
	void TrackSelector::WriteDedxInfo(EvtRecTrack* evtRecTrack, const char* tupleName, std::map<std::string, NTuple::Item<double> > &map)
	{

		// * Check if dE/dx and MDC info exists *
		if(!evtRecTrack->isMdcTrackValid()) return;
		if(!evtRecTrack->isMdcDedxValid())  return;
		fTrackMDC  = evtRecTrack->mdcTrack();
		fTrackDedx = evtRecTrack->mdcDedx();

		// * <b>write</b> energy loss PID info ("dedx" branch) *
		map.at("p")   = fTrackMDC->p();
		map.at("chie")   = fTrackDedx->chiE();
		map.at("chimu")  = fTrackDedx->chiMu();
		map.at("chipi")  = fTrackDedx->chiPi();
		map.at("chik")   = fTrackDedx->chiK();
		map.at("chip")   = fTrackDedx->chiP();
		map.at("probPH") = fTrackDedx->probPH();
		map.at("normPH") = fTrackDedx->normPH();
		map.at("ghit")   = fTrackDedx->numGoodHits();
		map.at("thit")   = fTrackDedx->numTotalHits();
		fNTupleMap.at(tupleName)->write();

	}

	/**
	 * @brief Automatically assign all <i>mapped values</i> of `map` as items to an `NTuplePtr`.
	 * 
	 * @param nt The `NTuplePtr` to which you want to add the <i>mapped values</i> of `map`.
	 * @param map The `map` from which you want to load the <i>mapped values</i>.
	 */
	void TrackSelector::AddItemsToNTuples(NTuplePtr nt, std::map<std::string, NTuple::Item<double> > &map)
	{
		if(!nt) return;
		std::map<std::string, NTuple::Item<double> >::iterator it = map.begin();
		for(; it != map.end(); ++it) {
			nt->addItem(it->first, it->second);
			fLog << MSG::DEBUG << "  added \"" << it->first << "\"" << endmsg;
		}
		fLog << MSG::INFO << "  total: " << map.size() << " items" << endmsg;
	}

	/**
	 * @brief Automatically assign all <i>mapped values</i> of `map` as items to an `NTuplePtr`. The `NTuplePtr` is booked automatically.
	 * 
	 * @param tupleName Name of the `NTuplePtr` to which you want to add the <i>mapped values</i> of `map`.
	 * @param map The `map` from which you want to load the <i>mapped values</i>.
	 * @param tupleTitle Title of the `NTuplePtr`. You can later use this title as a more detailed description in your analysis of the resulting histograms.
	 */
	void TrackSelector::AddItemsToNTuples(const char* tupleName, std::map<std::string, NTuple::Item<double> > &map, const char* tupleTitle)
	{
		AddItemsToNTuples(BookNTuple(tupleName, tupleTitle), map);
	}

	/**
	 * @brief Encapsulates the proces of writing PID info. This allows you to write the PID information after the particle selection as well.
	 */
	void TrackSelector::WritePIDInformation()
	{
		if(!fDo_PID) return;
		if(!fPIDInstance) return;
		fLog << MSG::DEBUG << "Writing PID information" << endmsg;
		fTrackMDC = (*fTrackIterator)->mdcTrack();
		if(fTrackMDC) {
			fMap_PID.at("p")    = fTrackMDC->p();
			fMap_PID.at("cost") = cos(fTrackMDC->theta());
		}
		fMap_PID.at("chiToFEC") = fPIDInstance->chiTofE(2);
		fMap_PID.at("chiToFIB") = fPIDInstance->chiTof1(2);
		fMap_PID.at("chiToFOB") = fPIDInstance->chiTof2(2);
		fMap_PID.at("chidEdx")  = fPIDInstance->chiDedx(2);
		fMap_PID.at("prob_K")   = fPIDInstance->probKaon();
		fMap_PID.at("prob_e")   = fPIDInstance->probElectron();
		fMap_PID.at("prob_mu")  = fPIDInstance->probMuon();
		fMap_PID.at("prob_p")   = fPIDInstance->probProton();
		fMap_PID.at("prob_pi")  = fPIDInstance->probPion();
		fNTupleMap.at("PID")->write();
	}

	/**
	 * @brief Method that standardizes the initialisation of the particle identification system. Define here <i>as general as possible</i>, but use in the derived subalgorithms.
	 * @details See http://bes3.to.infn.it/Boss/7.0.2/html/classParticleID.html for more info.
	 * 
	 * @param method Which method to use: probability, likelihood, or neuron network (see `TSGlobals::PIDMethod`).
	 * @param pidsys PID systems you want to call. Can combined using bit seperators (`|`), e.g. `pid->useDedx() | pid->useTof1() | pid->useTof2() | pid->useTofE()` for \f$dE/dx\f$ plus all ToF detectors.
	 * @param pidcase Which particles to identify. For instance, `pid->onlyPion() | pid->onlyKaon()` in the case of pions and kaons.
	 * @param chimin Minimal \f$\chi^2\f$ of the resulting particle identification.
	 */
	ParticleID* TrackSelector::InitializePID(PIDMethod method, const int pidsys, const int pidcase, const double chimin)
	{
		// * Initialise PID sub-system and set method: probability, likelihood, or neuron network
		fPIDInstance->init();
		switch(method) {
			case Probability:   fPIDInstance->setMethod(fPIDInstance->methodProbability());   break;
			case Likelihood:    fPIDInstance->setMethod(fPIDInstance->methodLikelihood());    break;
			case NeuronNetwork: fPIDInstance->setMethod(fPIDInstance->methodNeuronNetwork()); break;
			default: return nullptr;
		}
		fPIDInstance->setChiMinCut(chimin);
		fPIDInstance->setRecTrack(*fTrackIterator);

		// * Choose ID system and which particles to use
		fPIDInstance->usePidSys(pidsys);
		fPIDInstance->identify(pidcase);

		// * Perform PID
		fPIDInstance->calculate();
		if(!(fPIDInstance->IsPidInfoValid())) return nullptr;
	}