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
	#include "TrackSelector/TrackSelector.h"
	#include "VertexFit/Helix.h"
	#include "VertexFit/IVertexDbSvc.h"

	#ifndef ENABLE_BACKWARDS_COMPATIBILITY
		typedef HepGeom::Point3D<double> HepPoint3D;
	#endif

	using CLHEP::Hep2Vector;
	using CLHEP::Hep3Vector;
	using CLHEP::HepLorentzVector;



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
	 * @details Here, you should declare properties: give them a name, assign a parameter (data member of `TrackSelector`), and if required a documentation string. Note that you should define the paramters themselves in the header (TrackSelector/TrackSelector.h) and that you should assign the values in `share/jopOptions_TrackSelector.txt`.
	 */
	// Algorithms should inherit from Gaudi's `Algorithm` class. See https://dayabay.bnl.gov/dox/GaudiKernel/html/classAlgorithm.html.
	TrackSelector::TrackSelector(const std::string &name, ISvcLocator* pSvcLocator) :
		Algorithm(name, pSvcLocator),
		fLog(msgSvc(), name),
		fEventHeader (eventSvc(), "/Event/EventHeader"),
		fEvtRecEvent (eventSvc(), EventModel::EvtRec::EvtRecEvent),
		fEvtRecTrkCol(eventSvc(), EventModel::EvtRec::EvtRecTrackCol)
	{

		// * Whether or not to fill a tree/NTuple *
		declareProperty("doMult",        fDoMult);
		declareProperty("doVertex",      fDoVertex);
		declareProperty("doTrackVertex", fDoCharged);
		declareProperty("doDedx",        fDoDedx);
		declareProperty("doTofEC",       fDoTofEC);
		declareProperty("doTofIB",       fDoTofIB);
		declareProperty("doTofOB",       fDoTofOB);
		declareProperty("doPID",         fDoPID);

		// * Define cuts *
		declareProperty("Vr0cut",    fVr0cut   =  1.);
		declareProperty("Vz0cut",    fVz0cut   =  5.);
		declareProperty("Vrvz0cut",  fRvz0cut  = 10.);
		declareProperty("Vrvxy0cut", fRvxy0cut =  1.);
		declareProperty("MaxPhotonEnergy", fMaxPhotonEnergy = 0.04);

	}



// * =============================== * //
// * ------- ALGORITHM STEPS ------- * //
// * =============================== * //

	/**
	 * @brief   (Inherited) `initialize` step of `Algorithm`. This function is called only once in the beginning.
	 * @details Define and load NTuples here.
	 */
	StatusCode TrackSelector::initialize()
	{

		// * Book NTuple: Multiplicities * //
			if(fDoMult) {
				fMult["Ntotal"];       /// Total number of events per track.
				fMult["Ncharge"];      /// Number of charged tracks.
				fMult["Nneutral"];     /// Number of charged tracks.
				fMult["NgoodCharged"]; /// Number of 'good' charged tracks.
				fMult["NgoodNeutral"]; /// Number of 'good' neutral tracks.
				fMult["Nmdc"];         /// Number of charged tracks in MDC.
				fMult["NKaonPos"];     /// Number of \f$K^+\f$.
				fMult["NKaonNeg"];     /// Number of \f$K^-\f$.
				fMult["NPionPos"];     /// Number of \f$\pi^-\f$.
				AddItemsToNTuples("mult", fMult); /// Branch for multiplicies.
			}

		// * Book NTuple: Vertex info * //
			if(fDoVertex) {
				fVertex["vx0"]; /// Primary \f$x\f$ coordinate of the collision point.
				fVertex["vy0"]; /// Primary \f$y\f$ coordinate of the collision point.
				fVertex["vz0"]; /// Primary \f$z\f$ coordinate of the collision point.
				AddItemsToNTuples("vertex", fVertex); /// Branch for primary vertex info.
			}

		// * Book NTuple: dE/dx PID branch * //
			if(fDoDedx) {
				BookNtupleItemsDedx("dedx", fDedx);
			}

		// * Book NTuple: ToF branch * //
			if(fDoTofEC) BookNtupleItemsTof("tofe", fTofEC);
			if(fDoTofIB) BookNtupleItemsTof("tof1", fTofIB);
			if(fDoTofOB) BookNtupleItemsTof("tof2", fTofOB);

		// * Book NTuple: Track PID information * //
			if(fDoPID) {
				fPID["ptrk"];    /// Momentum of the track as reconstructed by MDC.
				fPID["cost"];    /// Theta angle of the track.
				fPID["dedx"];    /// Chi squared of the dedx of the track.
				fPID["tof1"];    /// Chi squared of the inner barrel ToF of the track.
				fPID["tof2"];    /// Chi squared of the outer barrel ToF of the track.
				fPID["prob_K"];  /// Probability that it is a kaon.
				fPID["prob_e"];  /// Probability that it is a electron.
				fPID["prob_mu"]; /// Probability that it is a muon.
				fPID["prob_p"];  /// Probability that it is a proton.
				fPID["prob_pi"]; /// Probability that it is a pion.
				AddItemsToNTuples("pid", fPID);
			}

		// * Book NTuple: Charged track info * //
			if(fDoCharged) {
				fCharged["vx"];    /// Primary \f$x\f$ coordinate of the vertex as determined by MDC.
				fCharged["vy"];    /// Primary \f$y\f$ coordinate of the vertex as determined by MDC.
				fCharged["vz"];    /// Primary \f$z\f$ coordinate of the vertex as determined by MDC.
				fCharged["vr"];    /// Distance from origin in \f$xy\f$ plane.
				fCharged["rvxy"];  /// Nearest distance to IP in \f$xy\f$ plane.
				fCharged["rvz"];   /// Nearest distance to IP in \f$z\f$ direction.
				fCharged["rvphi"]; /// Angle in the \f$xy\f$plane (?). @todo
				fCharged["phi"];   /// Helix angle of the particle (?). @todo
				fCharged["p"];     /// Momentum \f$p\f$ of the track.
				AddItemsToNTuples("charged", fCharged);
			}

		// * Book NTuple: Neutral track info * //
			if(fDoNeutral) {
				fNeutral["E"];     /// Energy of the neutral track as determined by the EM calorimeter.
				fNeutral["x"];     /// \f$x\f$-coordinate of the neutral track according to the EMC.
				fNeutral["y"];     /// \f$y\f$-coordinate of the neutral track according to the EMC.
				fNeutral["z"];     /// \f$z\f$-coordinate of the neutral track according to the EMC.
				fNeutral["phi"];   /// \f$\phi\f$-angle of the neutral track according to the EMC.
				fNeutral["theta"]; /// \f$\theta\f$-angle of the neutral track according to the EMC.
				fNeutral["time"];  /// Time of the neutral track according to the EMC. @todo Investigate what this parameter precisely means.
				AddItemsToNTuples("neutral", fNeutral);
			}

		return initialize_rest();
		fLog << MSG::INFO << "Successfully returned from initialize()" << endmsg;
		return StatusCode::SUCCESS;
	}


	/**
	 * @brief Inherited `execute` method of the `Algorithm`. This function is called *for each event*.
	 */
	StatusCode TrackSelector::execute()
	{
		/// STEP (A): Load next event from DST file

			// * Load event info *
				/*
				http://bes3.to.infn.it/Boss/7.0.2/html/namespaceEventModel_1_1EvtRec.html (namespace)
				http://bes3.to.infn.it/Boss/7.0.2/html/classEvtRecEvent.html (class)
				http://bes3.to.infn.it/Boss/7.0.2/html/EvtRecTrack_8h.html (typedef EvtRecTrackCol)
				*/
			fLog << MSG::DEBUG << "Loading EventHeader, EvtRecEvent, and EvtRecTrackCol" << endreq;
			fEventHeader  = SmartDataPtr<Event::EventHeader>(eventSvc(), "/Event/EventHeader");
			fEvtRecEvent  = SmartDataPtr<EvtRecEvent>       (eventSvc(), EventModel::EvtRec::EvtRecEvent);
			fEvtRecTrkCol = SmartDataPtr<EvtRecTrackCol>    (eventSvc(), EventModel::EvtRec::EvtRecTrackCol);

			// * Log run number, event number, and number of events *
			fLog << MSG::DEBUG
				<< "RUN "          << fEventHeader->runNumber()   << ", "
				<< "event number " << fEventHeader->eventNumber() << endreq;
			fLog << MSG::DEBUG
				<< "Ncharged = " << fEvtRecEvent->totalCharged() << ", "
				<< "Nneutral = " << fEvtRecEvent->totalNeutral() << ", "
				<< "Ntotal = "   << fEvtRecEvent->totalTracks()  << endreq;

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


		/// STEP (B): Create selection charged tracks and write track info

			// * Print log and set counters *
			fLog << MSG::DEBUG << "Starting 'good' charged track selection:" << endreq;
			int nChargesMDC = 0;
			ParticleID *pid = ParticleID::instance();

			// * Loop over charged tracks *
			fGoodChargedTracks.clear();
			for(int i = 0; i < fEvtRecEvent->totalCharged(); ++i) {
			// Note: the first part of the set of reconstructed tracks are the charged tracks
				/// STEP 1: Get MDC information

					// * Get track info from Main Drift Chamber
					fLog << MSG::DEBUG << "   charged track " << i << "/" << fEvtRecEvent->totalCharged() << endreq;
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

				/// STEP 2: Apply vertex cuts, store 

					// * Apply vertex cuts
					if(fTrackMDC->z() >= fVz0cut)   continue;
					if(vr          >= fVr0cut)   continue;
					if(rvz         >= fRvz0cut)  continue;
					if(rvxy        >= fRvxy0cut) continue;

					// * Add charged track to vector
					fGoodChargedTracks.push_back(*fTrackIterator);
					nChargesMDC += fTrackMDC->charge(); // @todo Check if this makes sense at all

				/// STEP 3: WRITE charged track vertex position info ("charged" branch)
					if(fDoCharged) {
						fCharged.at("vx")    = fTrackMDC->x();
						fCharged.at("vy")    = fTrackMDC->y();
						fCharged.at("vz")    = fTrackMDC->z();
						fCharged.at("vr")    = vr;
						fCharged.at("rvxy")  = rvxy;
						fCharged.at("rvz")   = rvz;
						fCharged.at("rvphi") = rvphi;
						fCharged.at("phi")   = phi;
						fCharged.at("p")     = fTrackMDC->p();
						fNTupleMap.at("charged")->write();
					}

				/// STEP 4: WRITE dE/dx PID information ("dedx" branch)
					if(fDoDedx) WriteDedxInfo(*fTrackIterator, "dedx", fDedx);

				/// STEP 5: WRITE Time-of-Flight PID information ("tof*" branch)
					if(fDoTofEC || fDoTofIB || fDoTofOB) {

						// * Check if MDC and TOF info for track are valid *
						if(!(*fTrackIterator)->isMdcTrackValid()) continue;
						if(!(*fTrackIterator)->isTofTrackValid()) continue;

						// * Get momentum as determined by MDC *
						fTrackMDC = (*fTrackIterator)->mdcTrack();
						double ptrk;
						if(fTrackMDC) fTrackMDC->p();
						SmartRefVector<RecTofTrack> tofTrkCol = (*fTrackIterator)->tofTrack();
						SmartRefVector<RecTofTrack>::iterator iter_tof = tofTrkCol.begin();
						for(; iter_tof != tofTrkCol.end(); ++iter_tof) {
							TofHitStatus hitStatus;
							hitStatus.setStatus((*iter_tof)->status());
							if(!hitStatus.is_counter()) continue;
							if(hitStatus.is_barrel()) {
								if(hitStatus.layer() == 1) { // inner barrel
									if(fDoTofIB) WriteTofInformation(iter_tof, ptrk, "tof1", fTofIB);
								} else if(hitStatus.layer() == 2) { // outer barrel
									if(fDoTofOB) WriteTofInformation(iter_tof, ptrk, "tof2", fTofOB);
								}
							}
							else if(fDoTofEC && hitStatus.layer() == 0) // end cap
								WriteTofInformation(iter_tof, ptrk, "tofe", fTofEC);
						}

					} // if(fDoTofEC || fDoTofIB || fDoTofOB)

			}


		/// STEP (C): Create selection of neutral tracks and write track info
			// Note: The second part of the set of reconstructed events consists of the neutral tracks, that is, the photons detected by the EMC (by clustering EMC crystal energies). Each neutral track is paired with each charged track and if their angle is smaller than a certain value (here, 200), the photon track is stored as 'good photon' (added to `iGam`).
			fGoodNeutralTracks.clear();
			for(int i = fEvtRecEvent->totalCharged(); i < fEvtRecEvent->totalTracks(); ++i) {
				/// STEP 1: Get MDC information

					// * Get track and test if available
					fLog << MSG::DEBUG << "   neutral track " << i-fEvtRecEvent->totalCharged() << "/" << fEvtRecEvent->totalNeutral() << endreq;
					fTrackIterator = fEvtRecTrkCol->begin() + i; 
					if(!(*fTrackIterator)->isEmcShowerValid()) continue;
					fTrackEMC = (*fTrackIterator)->emcShower();
					if(!fTrackEMC) continue;

					// * Apply photon cuts
					if(fTrackEMC->energy() < fMaxPhotonEnergy) continue;

					// * WRITE photon info ("photon" branch)
					fNeutral.at("E")     = fTrackEMC->energy();
					fNeutral.at("x")     = fTrackEMC->x();
					fNeutral.at("y")     = fTrackEMC->y();
					fNeutral.at("z")     = fTrackEMC->z();
					fNeutral.at("phi")   = fTrackEMC->phi();
					fNeutral.at("theta") = fTrackEMC->theta();
					fNeutral.at("time")  = fTrackEMC->time();
					fNTupleMap.at("neutral")->write();

					// * Add photon track to vector
					fGoodNeutralTracks.push_back(*fTrackIterator);

			}

			// * Finish Good Photon Selection *
			fLog << MSG::DEBUG << "Number of good photons: " << fGoodNeutralTracks.size() << endreq;


		/// STEP (D): WRITE event info ("mult" and "vertex" branch)
			fLog << MSG::DEBUG << "ngood, totcharge = " << fGoodChargedTracks.size() << " , " << nChargesMDC << endreq;
			if(fDoMult) {
				fMult.at("Ntotal")       = fEvtRecEvent->totalTracks();
				fMult.at("Ncharge")      = fEvtRecEvent->totalCharged();
				fMult.at("Nneutral")     = fEvtRecEvent->totalNeutral();
				fMult.at("NgoodCharged") = fGoodChargedTracks.size();
				fMult.at("NgoodNeutral") = fGoodNeutralTracks.size();
				fMult.at("Nmdc")         = nChargesMDC;
				fNTupleMap.at("vertex")->write();
			}
			if(fDoVertex) {
				fVertex.at("vx0") = v0x;
				fVertex.at("vy0") = v0y;
				fVertex.at("vz0") = v0z;
				fNTupleMap.at("vertex")->write();
			}

		/// STEP (E): Perform derived algoritm
		return execute_rest();
		return StatusCode::SUCCESS;
	}


	/**
	 * @brief Currently does nothing. Cut flow could be printed in this step.
	 * @todo Add log output to `finalize` step.
	 */
	StatusCode TrackSelector::finalize()
	{
		return finalize_rest();
		return StatusCode::SUCCESS;
	}



// * ================================= * //
// * -------- PRIVATE METHODS -------- * //
// * ================================= * //

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
			nt = ntupleSvc()->book(bookName, CLID_ColumnWiseTuple, tupleTitle);
			if(!nt) fLog << MSG::ERROR << "    Cannot book N-tuple:" << long(nt) << " (" << tupleName << ")" << endmsg;
		}
		fNTupleMap[tupleName] = nt.ptr(); /// Use `map::operator[]` if you want to book an `NTuple::Item` and use `map::at` if you want to access the `NTuple` by key value. This ensures that the programme throws an exception if you ask for the wrong key later.
		return nt;
	}


	/**
	 * @brief This function encapsulates the `addItem` procedure for the ToF branch. This allows to standardize the loading of the end cap, inner barrel, and outer barrel ToF branches.
	 */ 
	template<typename TYPE>
	void TrackSelector::BookNtupleItemsTof(const char* tupleName, std::map<std::string, NTuple::Item<TYPE> > &map)
	{
		map["ptrk"];  /// Momentum of the track as reconstructed by MDC.
		map["tof"];   /// Time of flight.
		map["path"];  /// Path length.
		map["cntr"];  /// ToF counter ID.
		map["ph"];    /// ToF pulse height.
		map["zrhit"]; /// Track extrapolate \f$Z\f$ or \f$R\f$ Hit position.
		map["qual"];  /// Data quality of reconstruction.
		map["te"];    /// Difference with ToF in electron hypothesis.
		map["tmu"];   /// Difference with ToF in muon hypothesis.
		map["tpi"];   /// Difference with ToF in charged pion hypothesis.
		map["tk"];    /// Difference with ToF in charged kaon hypothesis.
		map["tp"];    /// Difference with ToF in proton hypothesis.
		AddItemsToNTuples(tupleName, map);
	}


	/**
	 * @brief This function encapsulates the `addItem` procedure for the \f$dE/dx\f$ energy loss branch (`"dedx"`).
	 * @details This method allows you to perform the same booking method for different types of charged particles (for instance 'all charged particles', kaons, and pions).
	 */
	template<typename TYPE>
	void TrackSelector::BookNtupleItemsDedx(const char* tupleName, std::map<std::string, NTuple::Item<TYPE> > &map)
	{
		map["ptrk"];   /// Momentum of the track as reconstructed by MDC.
		map["chie"];   /// Chi squared in case of electron.
		map["chimu"];  /// Chi squared in case of muon.
		map["chipi"];  /// Chi squared in case of pion.
		map["chik"];   /// Chi squared in case of kaon.
		map["chip"];   /// Chi squared in case of proton.
		map["probPH"]; /// Most probable pulse height from truncated mean.
		map["normPH"]; /// Normalized pulse height.
		map["ghit"];   /// Number of good hits.
		map["thit"];   /// Total number of hits.
		AddItemsToNTuples(tupleName, map);
	}


	/**
	 * @brief
	 */
	template<typename TYPE>
	void TrackSelector::WriteTofInformation(SmartRefVector<RecTofTrack>::iterator iter_tof, double ptrk, const char* tupleName, std::map<std::string, NTuple::Item<TYPE> > &map)
	{

		// * Get ToF for each particle hypothesis
		double path = (*iter_tof)->path();
		std::vector<double> texp(gNMasses);
		for(size_t j = 0; j < texp.size(); ++j) {
			double gb = ptrk/gMasses[j]; // v = p/m (non-relativistic velocity)
			double beta = gb/sqrt(1+gb*gb);
			texp[j] = 10 * path /beta/gSpeedOfLight; // hypothesis ToF
		}

		// * WRITE ToF info
		map.at("ptrk")  = ptrk;
		map.at("tof")   = (*iter_tof)->tof();
		map.at("path")  = (*iter_tof)->path();
		map.at("cntr")  = (*iter_tof)->tofID();
		map.at("ph")    = (*iter_tof)->ph();
		map.at("zrhit") = (*iter_tof)->zrhit();
		map.at("qual")  = (*iter_tof)->quality();
		map.at("te")    = path - texp[0];
		map.at("tmu")   = path - texp[1];
		map.at("tpi")   = path - texp[2];
		map.at("tk")    = path - texp[3];
		map.at("tp")    = path - texp[4];
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
	template<typename TYPE>
	void TrackSelector::WriteDedxInfo(EvtRecTrack* evtRecTrack, const char* tupleName, std::map<std::string, NTuple::Item<TYPE> > &map)
	{

		// * Check if dE/dx and MDC info exists *
		if(!evtRecTrack->isMdcTrackValid()) return;
		if(!evtRecTrack->isMdcDedxValid())  return;
		fTrackMDC  = evtRecTrack->mdcTrack();
		fTrackDedx = evtRecTrack->mdcDedx();

		// * WRITE energy loss PID info ("dedx" branch) *
		map.at("ptrk")   = fTrackMDC->p();
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
	template<typename TYPE>
	void TrackSelector::AddItemsToNTuples(NTuplePtr nt, std::map<std::string, NTuple::Item<TYPE> > &map)
	{
		if(!nt) return;
		typename std::map<std::string, NTuple::Item<TYPE> >::iterator it = map.begin();
		for(; it != map.end(); ++it) nt->addItem(it->first, it->second);
	}

	/**
	 * @brief Automatically assign all <i>mapped values</i> of `map` as items to an `NTuplePtr`. The `NTuplePtr` is booked automatically.
	 * 
	 * @param tupleName Name of the `NTuplePtr` to which you want to add the <i>mapped values</i> of `map`.
	 * @param map The `map` from which you want to load the <i>mapped values</i>.
	 */
	template<typename TYPE>
	void TrackSelector::AddItemsToNTuples(const char* tupleName, std::map<std::string, NTuple::Item<TYPE> > &map)
	{
		AddItemsToNTuples(BookNTuple(tupleName), map);
	}

	/**
	 * @brief Encapsulates the proces of writing PID info. This allows you to write the PID information after the particle selection as well.
	 * @param pid Instance of particle ID (`ParticleID::instance()`).
	 */
	void TrackSelector::WritePIDInformation(ParticleID *pid)
	{
		fTrackMDC = (*fTrackIterator)->mdcTrack();
		if(fTrackMDC) {
			fPID.at("ptrk") = fTrackMDC->p();
			fPID.at("cost") = cos(fTrackMDC->theta());
		}
		fPID.at("dedx") = pid->chiDedx(2);
		fPID.at("tof1") = pid->chiTof1(2);
		fPID.at("tof2") = pid->chiTof2(2);
		fPID.at("prob_K")  = pid->probKaon();
		fPID.at("prob_e")  = pid->probElectron();
		fPID.at("prob_mu") = pid->probMuon();
		fPID.at("prob_p")  = pid->probProton();
		fPID.at("prob_pi") = pid->probPion();
		fNTupleMap.at("pid")->write();
	}