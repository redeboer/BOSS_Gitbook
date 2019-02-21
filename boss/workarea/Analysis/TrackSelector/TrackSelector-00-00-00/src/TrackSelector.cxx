// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

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
	#include <cmath>

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
	const double gSpeedOfLight = 299.792458; // tof path unit in mm
	const int gNMasses = sizeof(gMasses)/sizeof(*gMasses);



// * =========================== * //
// * ------- CONSTRUCTOR ------- * //
// * =========================== * //


	/// Constructor for the `TrackSelector` algorithm.
	/// Here, you should declare properties: give them a name, assign a parameter (data member of `TrackSelector`), and if required a documentation string. Note that you should define the paramters themselves in the header (TrackSelector/TrackSelector.h) and that you should assign the values in `share/jopOptions_TrackSelector.txt`. Algorithms should inherit from Gaudi's `Algorithm` class. See https://dayabay.bnl.gov/dox/GaudiKernel/html/classAlgorithm.html for more details.
	TrackSelector::TrackSelector(const std::string &name, ISvcLocator* pSvcLocator) :
		/// * Construct `Algorithm` objects.
			Algorithm(name, pSvcLocator),
			fLog(msgSvc(), name),
		/// * Construct objects for file access.
			fEventHeader  (eventSvc(), "/Event/EventHeader"),
			fMcParticleCol(eventSvc(), "/Event/MC/McParticleCol"),
			fEvtRecEvent  (eventSvc(), EventModel::EvtRec::EvtRecEvent),
			fEvtRecTrkCol (eventSvc(), EventModel::EvtRec::EvtRecTrackCol),
		/// * Construct `NTuple::Tuple`s containers used in base class
			fNTuple_mult   ("mult",    "Event multiplicities"),
			fNTuple_vertex ("vertex",  "Primary vertex (collision point)"),
			fNTuple_charged("charged", "Charged track info"),
			fNTuple_neutral("neutral", "Charged track info"),
			fNTuple_dedx   ("dedx",    "dE/dx of all charged tracks"),
			fNTuple_TofEC  ("TofEC",   "End cap ToF of all tracks"),
			fNTuple_TofIB  ("TofIB",   "Inner barrel ToF of all tracks"),
			fNTuple_TofOB  ("TofOB",   "Outer barrel ToF of all tracks"),
			fNTuple_PID    ("PID",     "Particle Identification parameters"),
			fNTuple_cuts   ("_cutvalues", "1st entry: min value, 2nd entry: max value, 3rd entry: number passed"),
		/// * Construct `NTuple::Tuple`s containers used in derived classes.
			fNTuple_mult_sel("mult_select", "Multiplicities of selected particles"),
			fNTuple_mctruth ("mctruth",     "Monte Carlo truth for TopoAna package"),
		/// * Construct counters (in essence a `CutObject` without cuts).
			fCounter_Nevents  ("N_events"),
			fCounter_Ntracks  ("N_tracks"),
			fCounter_Ncharged ("N_charged"),
			fCounter_Nneutral ("N_neutral"),
			fCounter_Nmdcvalid("N_MDCvalid"),
		/// * Construct `CutObject`s. The `"cut_<parameter>_min/max"` properties determine cuts on certain parameters.
			fCut_Vxy         ("vertex_xy"),
			fCut_Vz          ("vertex_z"),
			fCut_Rxy         ("declen_xy"),
			fCut_Rz          ("declen_z"),
			fCut_CosTheta    ("costheta"),
			fCut_PhotonEnergy("PhotonEnergy"),
			fCut_PIDChiSq    ("PIDChiSq"),
			fCut_PIDProb     ("PIDProb")
	{
		fLog << MSG::DEBUG << "===>> TrackSelector::TrackSelector() <<===" << endmsg;

		/// @todo Check if `DeclareCuts` should be moved to `TrackSelector::initialize` in order to also declare the cuts that are defined in the derived algorithm. This depends on whether the constructor of the base or of the derived class is called first.
		DeclareSwitches();
		DeclareCuts();
	}



// * =============================== * //
// * ------- ALGORITHM STEPS ------- * //
// * =============================== * //


	/// (Inherited) `initialize` step of `Algorithm`. This function is called once in the beginning <i>of each run</i>. Define and load NTuples here. The `NTuples` will become the `TTree`s in the eventual ROOT file, the added `NTuple::Item`s will be the branches of those trees.
	StatusCode TrackSelector::initialize()
	{
		fLog << MSG::INFO << "===>> TrackSelector::initialize() <<===" << endmsg;

		/// <ol type="A">
		BookNTuples();
		/// <li> `"mult"`: Multiplicities of the total event
			/// <ol>
			fNTuple_mult.AddItem("Ntotal");       /// <li> `"Ntotal"`:       Total number of events per track.
			fNTuple_mult.AddItem("Ncharge");      /// <li> `"Ncharge"`:      Number of charged tracks.
			fNTuple_mult.AddItem("Nneutral");     /// <li> `"Nneutral"`:     Number of charged tracks.
			if(fNTuple_neutral) fNTuple_mult.AddItem("NgoodNeutral"); /// <li> `"NgoodNeutral"`: Number of 'good' neutral tracks.
			if(fNTuple_charged) {
				fNTuple_mult.AddItem("NgoodCharged"); /// <li> `"NgoodCharged"`: Number of 'good' charged tracks.
				fNTuple_mult.AddItem("Nmdc");         /// <li> `"Nmdc"`:         Number of charged tracks in MDC.
			}
			/// </ol>

		/// <li> `"vertex"`: Vertex info
			/// <ol>
			fNTuple_vertex.AddItem("vx0"); /// <li> `"vx0"`: \f$x\f$ coordinate of the collision point.
			fNTuple_vertex.AddItem("vy0"); /// <li> `"vy0"`: \f$y\f$ coordinate of the collision point.
			fNTuple_vertex.AddItem("vz0"); /// <li> `"vz0"`: \f$z\f$ coordinate of the collision point.
			/// </ol>

		/// <li> `"charged"`: Charged track info.
			/// <ol>
			fNTuple_charged.AddItem("vx");    /// <li> `"vx"`:    \f$x\f$ coordinate of the secondary vertex as determined by MDC.
			fNTuple_charged.AddItem("vy");    /// <li> `"vy"`:    \f$y\f$ coordinate of the secondary vertex as determined by MDC.
			fNTuple_charged.AddItem("vz");    /// <li> `"vz"`:    \f$z\f$ coordinate of the secondary vertex as determined by MDC.
			fNTuple_charged.AddItem("vr");    /// <li> `"vr"`:    Distance from origin in \f$xy\f$ plane.
			fNTuple_charged.AddItem("rvxy");  /// <li> `"rvxy"`:  Nearest distance to interaction point in \f$xy\f$ plane.
			fNTuple_charged.AddItem("rvz");   /// <li> `"rvz"`:   Nearest distance to interaction point in \f$z\f$ direction.
			fNTuple_charged.AddItem("rvphi"); /// <li> `"rvphi"`: Angle in the \f$xy\f$plane. @todo Get explanation of geometry (angle) definitions in an event.
			fNTuple_charged.AddItem("phi");   /// <li> `"phi"`:   Helix angle of the particle.
			fNTuple_charged.AddItem("p");     /// <li> `"p"`:     Momentum \f$p\f$ of the track.
			/// </ol>

		/// <li> `"neutral"`: Neutral track info.
			/// <ol>
			fNTuple_neutral.AddItem("E");     /// <li> `"E"`: Energy of the neutral track as determined by the EM calorimeter.
			fNTuple_neutral.AddItem("x");     /// <li> `"x"`: \f$x\f$-coordinate of the neutral track according to the EMC.
			fNTuple_neutral.AddItem("y");     /// <li> `"y"`: \f$y\f$-coordinate of the neutral track according to the EMC.
			fNTuple_neutral.AddItem("z");     /// <li> `"z"`: \f$z\f$-coordinate of the neutral track according to the EMC.
			fNTuple_neutral.AddItem("phi");   /// <li> `"phi"`: \f$\phi\f$-angle of the neutral track according to the EMC.
			fNTuple_neutral.AddItem("theta"); /// <li> `"theta"`: \f$\theta\f$-angle of the neutral track according to the EMC.
			fNTuple_neutral.AddItem("time");  /// <li> `"time"`: Time of the neutral track according to the EMC. @todo Investigate what this parameter precisely means.
			/// </ol>

		/// <li> `"dedx"`: energy loss \f$dE/dx\f$ PID branch. See `TrackSelector::AddNTupleItems_Dedx` for more info.
			AddNTupleItems_Dedx(fNTuple_dedx);

		/// <li> `"ToFEC"`, `"ToFIB"`, and `"ToFOB"`: information from the three Time-of-Flight detectors. See `TrackSelector::AddNTupleItems_Tof` for more info.
			AddNTupleItems_Tof(fNTuple_TofEC);
			AddNTupleItems_Tof(fNTuple_TofIB);
			AddNTupleItems_Tof(fNTuple_TofOB);

		/// <li> `"mctruth"`: Monte Carlo truth for TopoAna package.
			AddNTupleItems_McTruth();

		/// <li> `"PID"`: Track PID information.
			/// <ol>
			fNTuple_PID.AddItem("p");        /// <li> `"p"`:        Momentum of the track as reconstructed by MDC.
			fNTuple_PID.AddItem("cost");     /// <li> `"cost"`:     Theta angle of the track.
			fNTuple_PID.AddItem("chiToFIB"); /// <li> `"chiToFIB"`: \f$\chi^2\f$ of the inner barrel ToF of the track.
			fNTuple_PID.AddItem("chiToFEC"); /// <li> `"chiToFEC"`: \f$\chi^2\f$ of the end cap ToF of the track.
			fNTuple_PID.AddItem("chiToFOB"); /// <li> `"chiToFOB"`: \f$\chi^2\f$ of the outer barrel ToF of the track.
			fNTuple_PID.AddItem("chidEdx");  /// <li> `"chidEdx"`:  \f$\chi^2\f$ of the energy loss \f$dE/dx\f$ of the identified track.
			fNTuple_PID.AddItem("prob_K");   /// <li> `"prob_K"`:   Probability that the track is from a kaon according to the probability method.
			fNTuple_PID.AddItem("prob_e");   /// <li> `"prob_e"`:   Probability that the track is from a electron according to the probability method.
			fNTuple_PID.AddItem("prob_mu");  /// <li> `"prob_mu"`:  Probability that the track is from a muon according to the probability method.
			fNTuple_PID.AddItem("prob_p");   /// <li> `"prob_p"`:   Probability that the track is from a proton according to the probability method.
			fNTuple_PID.AddItem("prob_pi");  /// <li> `"prob_pi"`:  Probability that the track is from a pion according to the probability method.
			/// </ol>
		/// </ol>

		initialize_rest();
		// BookNTupleForCuts();
		fLog << MSG::INFO << "Successfully returned from TrackSelector::initialize()" << endmsg;
		return StatusCode::SUCCESS;
	}


	/// This method is called <b>for each event</b>.
	StatusCode TrackSelector::execute()
	{
		fLog << MSG::DEBUG << "===>> TrackSelector::execute() <<===" << endmsg;

		/// <ol type="A">
		/// <li> Load next event from DST file. For more info see:
			/// <ul>
				/// <li> <a href="http://bes3.to.infn.it/Boss/7.0.2/html/namespaceEventModel_1_1EvtRec.html">Namespace `EventModel`</a>
				/// <li> <a href="http://bes3.to.infn.it/Boss/7.0.2/html/classEvtRecEvent.html">Class `EvtRecEvent`</a>
				/// <li> <a href="http://bes3.to.infn.it/Boss/7.0.2/html/EvtRecTrack_8h.html">Type definition `EvtRecTrackCol`</a>
				/// <li> <a href="http://bes3.to.infn.it/Boss/7.0.0/html/namespaceEvent.html#b6a28637c54f890ed93d8fd13d5021ed">Type definition `Event::McParticleCol`</a>
			/// </ul>

			// * Load headers
			fLog << MSG::DEBUG << "Loading EventHeader, EvtRecEvent, and EvtRecTrackCol" << endmsg;
			fEventHeader   = SmartDataPtr<Event::EventHeader>(eventSvc(), "/Event/EventHeader");
			fEvtRecEvent   = SmartDataPtr<EvtRecEvent>       (eventSvc(), EventModel::EvtRec::EvtRecEvent);
			fEvtRecTrkCol  = SmartDataPtr<EvtRecTrackCol>    (eventSvc(), EventModel::EvtRec::EvtRecTrackCol);

			// * Log run number, event number, and number of events *
			fLog << MSG::DEBUG
				<< "RUN "          << fEventHeader->runNumber()   << ", "
				<< "event number " << fEventHeader->eventNumber() << endmsg;
			fLog << MSG::DEBUG
				<< "Ncharged = " << fEvtRecEvent->totalCharged() << ", "
				<< "Nneutral = " << fEvtRecEvent->totalNeutral() << ", "
				<< "Ntotal = "   << fEvtRecEvent->totalTracks()  << endmsg;

			// * Increase counters *
			++fCounter_Nevents;
			fCounter_Ncharged += fEvtRecEvent->totalCharged();
			fCounter_Nneutral += fEvtRecEvent->totalNeutral();
			fCounter_Ntracks  += fEvtRecEvent->totalTracks();

			// * Set vertex origin *
				IVertexDbSvc* vtxsvc;
				Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);
				if(vtxsvc->isVertexValid()){
					double* dbv = vtxsvc->PrimaryVertex();
					// double* vv  = vtxsvc->SigmaPrimaryVertex();
					fVertexPoint.set(dbv[0], dbv[1], dbv[2]);
				}

		/// <li> Get Monte Carlo truth if available (that is, if the run number is negative).
			if(fEventHeader->runNumber()<0 && fNTuple_mctruth) {
				/// <ol>
				/// <li> Load `McParticelCol` from `"/Event/MC/McParticleCol"` directory in `"FILE1"`.
				fMcParticleCol = SmartDataPtr<Event::McParticleCol>(eventSvc(), "/Event/MC/McParticleCol");

				/// <li> @b Abort if does not exist.
				///@todo Might have to make this aborting behaviour less strict.
				if(!fMcParticleCol) {
					fLog << MSG::ERROR << "Could not retrieve McParticelCol" << endmsg;
					return(StatusCode::FAILURE);
				}

				/// <li> Loop over collection of MC particles (`Event::McParticleCol`). For more info on the data available in `McParticle`, see <a href="http://bes3.to.infn.it/Boss/7.0.2/html/McParticle_8h-source.html">here</a>. Only add to `fMcParticles` if the `McParticle` satisfies:
				bool doNotRecord(true);
				for(Event::McParticleCol::iterator it = fMcParticleCol->begin(); it!=fMcParticleCol->end(); ++it) {
					/// <ul>
					/// <li> It is not a primary particle.
					if( (*it)->primaryParticle()) continue;
					/// <li> Only add if the decay has been generated. @todo Why? What does this mean precisely?
					if(!(*it)->decayFromGenerator()) continue;
					/// <li> Only start recording after we have passed the initial cluster (e.g. \f$J/\psi\f$). Note that `91` is the PDG code of cluster and `92` is the PDG code of string.
					if(
						(*it)->particleProperty() == 91 ||  /// <li> `91` is the PDG code of cluster.
						(*it)->particleProperty() == 92 ) { /// <li> `92` is the PDG code of string.
						doNotRecord = false;
					}
					/// <li> Skip if this particle is the initial cluster itself.
					if(doNotRecord) continue;
					/// <li> Add the pointer to the `fMcParticles` vector.
					fMcParticles.push_back(*it);
					/// </ul>
				} // end of for loop
				/// </ol>

			} // end of if runNumber<0

		/// <li> Create selection charged tracks and write track info:
			/// Note: the first part of the set of reconstructed tracks are the charged tracks.

			// * Reset counters and initialize PID instance *
			fGoodChargedTracks.clear();
			fPIDInstance = ParticleID::instance();

			// * Only perform if there are charged tracks *
			if(fNTuple_charged && fEvtRecEvent->totalCharged()) {

				fNChargesMDC = 0;
				fLog << MSG::DEBUG << "Starting 'good' charged track selection:" << endmsg;
				for(int i = 0; i < fEvtRecEvent->totalCharged(); ++i) {
					/// <ol>
					/// <li> Get MDC information

						// * Get track info from Main Drift Chamber
						fLog << MSG::DEBUG << "   charged track " << i << "/" << fEvtRecEvent->totalCharged() << endmsg;
						fTrackIterator = fEvtRecTrkCol->begin() + i; 
						if(!(*fTrackIterator)->isMdcTrackValid()) continue;
						++fCounter_Nmdcvalid;
						fTrackMDC = (*fTrackIterator)->mdcTrack();

						// * Get kinematics of track
						double phi  = fTrackMDC->helix(1);
						double vr =
							(fTrackMDC->x() - fVertexPoint.x()) * cos(phi) +
							(fTrackMDC->y() - fVertexPoint.y()) * sin(phi);

						// * Get radii of track vertex
						HepVector a = fTrackMDC->helix();
						HepSymMatrix Ea = fTrackMDC->err();
						HepPoint3D point0(0., 0., 0.); // initial point for MDC reconstruction
						VFHelix helixip(point0, a, Ea);
						helixip.pivot(fVertexPoint); /// @todo Check if `helixip.pivot(fVertexPoint)` doesn't affect `fVertexPoint`.
						HepVector vecipa = helixip.a();
						double rvxy  = fabs(vecipa[0]); // nearest distance to IP in xy plane
						double rvz   = vecipa[3];       // nearest distance to IP in z direction
						double rvphi = vecipa[1];       // angle in the xy-plane (?)

					/// <li> Apply charged track cuts:
						/// <ul>
						if(fCut_Vz .FailsMax(fabs(fTrackMDC->z()))) continue; /// <li> \f$z\f$ coordinate of the collision point has to be within `cut_Vz0_max`
						if(fCut_Vxy.FailsMax(vr)                  ) continue; /// <li> radius in \f$xy\f$ plane has to be less than `cut_Vr0_max`
						if(fCut_Rz .FailsMax(fabs(rvz))           ) continue; /// <li> \f$z\f$ coordinate of the distance to the interaction point has to be within `cut_Rvz0_max`
						if(fCut_Rxy.FailsMax(rvxy)                ) continue; /// <li> distance to the interaction point in \f$xy\f$ plane has to be within `cut_Rvxy0_max`
						if(fCut_CosTheta.FailsMax(fabs(cos(fTrackMDC->theta())))) continue; /// <li> distance to the interaction point in \f$xy\f$ plane has to be within `cut_costheta_max`
						/// </ul>

						// * Add charged track to vector
						fGoodChargedTracks.push_back(*fTrackIterator);
						fNChargesMDC += fTrackMDC->charge(); // @todo Check if this makes sense at all

					/// <li> @b Write charged track vertex position info ("charged" branch)
						if(fNTuple_charged.DoWrite()) {
							fNTuple_charged.at("vx")    = fTrackMDC->x();
							fNTuple_charged.at("vy")    = fTrackMDC->y();
							fNTuple_charged.at("vz")    = fTrackMDC->z();
							fNTuple_charged.at("vr")    = vr;
							fNTuple_charged.at("rvxy")  = rvxy;
							fNTuple_charged.at("rvz")   = rvz;
							fNTuple_charged.at("rvphi") = rvphi;
							fNTuple_charged.at("phi")   = phi;
							fNTuple_charged.at("p")     = fTrackMDC->p();
							fNTuple_charged.Write();
						}

					/// <li> @b Write dE/dx PID information ("dedx" branch)
						WriteDedxInfo(*fTrackIterator, fNTuple_dedx);

					/// <li> @b Write Time-of-Flight PID information ("tof*" branch)
						if(fNTuple_TofEC.DoWrite() || fNTuple_TofIB.DoWrite() || fNTuple_TofOB.DoWrite()) {

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
										if(fNTuple_TofIB.DoWrite()) WriteTofInformation(iter_tof, ptrk, fNTuple_TofIB);
									} else if(hitStatus.layer() == 2) { // outer barrel
										if(fNTuple_TofOB.DoWrite()) WriteTofInformation(iter_tof, ptrk, fNTuple_TofOB);
									}
								}
								else if(fNTuple_TofEC.DoWrite() && hitStatus.layer() == 0) // end cap
									WriteTofInformation(iter_tof, ptrk, fNTuple_TofEC);
							}

						} // if(fNTuple_tofec.DoWrite() || fNTuple_tofib.DoWrite() || fNTuple_tofob.DoWrite())

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
			if(fNTuple_neutral && fEvtRecEvent->totalNeutral()) {

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
						if(fCut_PhotonEnergy.FailsMin(fTrackEMC->energy())) continue;

					/// <li> @b Write neutral track information (if `write_neutral` is set to `true`).
						if(fNTuple_neutral.DoWrite()) {
							fNTuple_neutral.at("E")     = fTrackEMC->energy();
							fNTuple_neutral.at("x")     = fTrackEMC->x();
							fNTuple_neutral.at("y")     = fTrackEMC->y();
							fNTuple_neutral.at("z")     = fTrackEMC->z();
							fNTuple_neutral.at("phi")   = fTrackEMC->phi();
							fNTuple_neutral.at("theta") = fTrackEMC->theta();
							fNTuple_neutral.at("time")  = fTrackEMC->time();
							NTupleContainer::Get("neutral").Write();
						}

					/// <li> Add photon track to vector of neutral tracks (`fGoodNeutralTracks`).
						fGoodNeutralTracks.push_back(*fTrackIterator);

					/// </ol>
				}

				// * Finish good photon selection *
				fLog << MSG::DEBUG << "Number of 'good' photons: " << fGoodNeutralTracks.size() << endmsg;
			}


		/// <li> @b Write event info (`"mult"` branch)
			if(fNTuple_mult.DoWrite()) {
				fNTuple_mult.at("Ntotal")       = fEvtRecEvent->totalTracks();
				fNTuple_mult.at("Ncharge")      = fEvtRecEvent->totalCharged();
				fNTuple_mult.at("Nneutral")     = fEvtRecEvent->totalNeutral();
				if(fNTuple_charged) {
					fNTuple_mult.at("NgoodCharged") = fGoodChargedTracks.size();
					fNTuple_mult.at("Nmdc")         = fNChargesMDC;
				}
				if(fNTuple_neutral) fNTuple_mult.at("NgoodNeutral") = fGoodNeutralTracks.size();
				NTupleContainer::Get("mult").Write();
			}

		/// <li> @b Write event info (`"vertex"` branch)
			if(fNTuple_vertex.DoWrite()) {
				fNTuple_vertex.at("vx0") = fVertexPoint.x();
				fNTuple_vertex.at("vy0") = fVertexPoint.y();
				fNTuple_vertex.at("vz0") = fVertexPoint.z();
				NTupleContainer::Get("vertex").Write();
			}

		/// <li> Perform derived algoritm as defined in `TrackSelector::execute_rest`.
			if(execute_rest() == StatusCode::SUCCESS) return StatusCode::SUCCESS;
			if(fPIDInstance) delete fPIDInstance;

		/// </ol>
		return StatusCode::SUCCESS;
	}


	/// Is called at the end <i>of the entire process</i>. Writes total cut flow to terminal and to the output file.
	StatusCode TrackSelector::finalize()
	{
		fLog << MSG::INFO << "===>> TrackSelector::finalize() <<===" << endmsg;

		finalize_rest();

		WriteCuts();
		CutObject::PrintAll();

		fLog << MSG::INFO << "Successfully returned from TrackSelector::finalize()" << endmsg;
		return StatusCode::SUCCESS;
	}



// * ================================ * //
// * -------- NTUPLE METHODS -------- * //
// * ================================ * //


	/// This function encapsulates the `addItem` procedure for the MC truth branches for the TopoAna package.
	void TrackSelector::AddNTupleItems_McTruth()
	{
		if(!fNTuple_mctruth) return;
		// fNTuple_mctruth.GetNTuple()->addItem("iEvt",   fNTuple_mctruth.iEvt);           /// * `"iEvt"`: @b Counter for number of events (not the ID!) @todo Check event counter if required for `topoana`
		fNTuple_mctruth.GetNTuple()->addItem("runID",  fNTuple_mctruth.runID);          /// * `"runID"`: Run number ID.
		fNTuple_mctruth.GetNTuple()->addItem("evtID",  fNTuple_mctruth.evtID);          /// * `"evtID"`: Rvent number ID.
		fNTuple_mctruth.GetNTuple()->addItem("nItems", fNTuple_mctruth.nItems, 0, 100); /// * `"nItems"`: Number of MC particles stored for this event. This one is necessary for loading following two items, because they are arrays.
		fNTuple_mctruth.GetNTuple()->addIndexedItem("particle", fNTuple_mctruth.nItems, fNTuple_mctruth.particle); /// * `"particle"`: PDG code for the particle in this array.
		fNTuple_mctruth.GetNTuple()->addIndexedItem("mother",   fNTuple_mctruth.nItems, fNTuple_mctruth.mother);   /// * `"mother"`: Track index of the mother particle.
	}


	/// This function encapsulates the `addItem` procedure for the \f$dE/dx\f$ energy loss branch (`"dedx"`). This method allows you to perform the same booking method for different types of charged particles (for instance 'all charged particles', kaons, and pions).
	void TrackSelector::AddNTupleItems_Dedx(NTupleContainer &tuple)
	{
		if(!tuple) return;
		// tuple.AddItem("dedx_K");     /// * `"dedx_K"`      Expected value of \f$dE/dx\f$ in case of kaon hypothesis.
		// tuple.AddItem("dedx_P");     /// * `"dedx_P"`      Expected value of \f$dE/dx\f$ in case of proton hypothesis.
		// tuple.AddItem("dedx_e");     /// * `"dedx_e"`      Expected value of \f$dE/dx\f$ in case of electron hypothesis.
		// tuple.AddItem("dedx_mom");   /// * `"dedx_mom"`    "\f$dE/dx\f$ calib used momentum".
		// tuple.AddItem("dedx_mu");    /// * `"dedx_mu"`     Expected value of \f$dE/dx\f$ in case of muon hypothesis.
		// tuple.AddItem("dedx_pi");    /// * `"dedx_pi"`     Expected value of \f$dE/dx\f$ in case of pion hypothesis.
		tuple.AddItem("Ngoodhits");  /// * `"Ngoodhits"`:  Number of good \f$dE/dx\f$ hits (excluding overflow).
		tuple.AddItem("Ntotalhits"); /// * `"Ntotalhits"`: Number of good \f$dE/dx\f$ hits (including overflow).
		tuple.AddItem("chie");       /// * `"chie"`:       \f$\chi^2\f$ in case of electron ("number of \f$\sigma\f$ from \f$e^\pm\f$").
		tuple.AddItem("chik");       /// * `"chik"`:       \f$\chi^2\f$ in case of kaon ("number of \f$\sigma\f$ from \f$K^\pm\f$").
		tuple.AddItem("chimu");      /// * `"chimu"`:      \f$\chi^2\f$ in case of muon ("number of \f$\sigma\f$ from \f$\mu^\pm\f$").
		tuple.AddItem("chip");       /// * `"chip"`:       \f$\chi^2\f$ in case of proton ("number of \f$\sigma\f$ from \f$p^\pm\f$").
		tuple.AddItem("chipi");      /// * `"chipi"`:      \f$\chi^2\f$ in case of pion ("number of \f$\sigma\f$ from \f$\pi^\pm\f$").
		tuple.AddItem("normPH");     /// * `"normPH"`:     Normalized pulse height.
		tuple.AddItem("p");          /// * `"p"`:          Momentum of the track as reconstructed by MDC.
		tuple.AddItem("probPH");     /// * `"probPH"`:     Most probable pulse height from truncated mean.
	}


	/// This function encapsulates the `addItem` procedure for the ToF branch. This allows to standardize the loading of the end cap, inner barrel, and outer barrel ToF branches.
	void TrackSelector::AddNTupleItems_Tof(NTupleContainer &tuple)
	{
		if(!tuple) return;
		tuple.AddItem("p");      /// * `"p"`:      Momentum of the track as reconstructed by MDC.
		tuple.AddItem("tof");    /// * `"tof"`:    Time of flight.
		tuple.AddItem("path");   /// * `"path"`:   Path length.
		tuple.AddItem("cntr");   /// * `"cntr"`:   ToF counter ID.
		tuple.AddItem("ph");     /// * `"ph"`:     ToF pulse height.
		tuple.AddItem("zrhit");  /// * `"zrhit"`:  Track extrapolate \f$Z\f$ or \f$R\f$ Hit position.
		tuple.AddItem("qual");   /// * `"qual"`:   Data quality of reconstruction.
		tuple.AddItem("tof_e");  /// * `"tof_e"`:  Difference with ToF in electron hypothesis.
		tuple.AddItem("tof_mu"); /// * `"tof_mu"`: Difference with ToF in muon hypothesis.
		tuple.AddItem("tof_pi"); /// * `"tof_pi"`: Difference with ToF in charged pion hypothesis.
		tuple.AddItem("tof_K");  /// * `"tof_K"`:  Difference with ToF in charged kaon hypothesis.
		tuple.AddItem("tof_p");  /// * `"tof_p"`:  Difference with ToF in proton hypothesis.
	}


	/// Helper function that allows you to relate the `NTupleContainer` argument `tuple` to the output file (i.e. to 'book' it).
	void TrackSelector::BookNTuple(NTupleContainer &tuple)
	{
		/// -# @b Abort if 'do `JobSwitch'` is set to `false`.
			if(!tuple) return;

		/// -# Form a string for booking in the file. Typically: `"FILE1/<tree name>"`.
			const char* bookName = Form("FILE1/%s", tuple.Name());

		/// -# Attempt to get this `NTuple::Tuple` from file the file.
			NTuplePtr nt(ntupleSvc(), bookName);

		/// -# If not available in file, book a new one.
			if(!nt) {
				fLog << MSG::INFO << "Booked NTuple \"" << tuple.Name() << "\"" << endmsg;
				nt = ntupleSvc()->book(bookName, CLID_ColumnWiseTuple, tuple.Description());
				if(!nt) fLog << MSG::ERROR << "    Cannot book N-tuple:" << long(nt) << " (" << tuple.Name() << ")" << endmsg;
			}

		/// -# Import this NTuplePtr to the `tuple` object.
			tuple.SetTuplePtr(nt);
	}


	/// Go over all instances of `NTupleContainer` and book them using `BookNTuple`.
	void TrackSelector::BookNTuples()
	{
		std::map<std::string, NTupleContainer*>::iterator it = NTupleContainer::instances.begin();
		for(it; it != NTupleContainer::instances.end(); ++it) BookNTuple(*it->second);
	}


	/// Declare properties for each `JobSwitch`. This method has been added to the `TrackSelector`, and not to the `JobSwitch` class, because it requires the `Algorithm::decalareProperty` method.
	void TrackSelector::DeclareSwitches()
	{
		std::list<JobSwitch*>::iterator it = JobSwitch::gJobSwitches.begin();
		for(; it != JobSwitch::gJobSwitches.end(); ++it) {
			declareProperty((*it)->Name(), (*it)->value);
			fLog << MSG::INFO << "  added property \"" << (*it)->Name() << "\"" << endmsg;
		}
	}



// * =============================== * //
// * -------- WRITE METHODS -------- * //
// * =============================== * //


	/// Encapsulates of the writing procedure for \f$dE/dx\f$ energy loss information <i>for one track</i>.
	/// Here, you should use `map::at` to access the `NTuple::Item`s and `NTuplePtr`, because you want your package to throw an exception if the element does not exist. See http://bes3.to.infn.it/Boss/7.0.2/html/TRecMdcDedx_8h-source.html#l00115 for available data members of `RecMdcDedx`
	/// @param evtRecTrack Pointer to the reconstructed track of which you want to write the \f$dE/dx\f$ data.
	/// @param tuple `NTupleContainer` to which you want to write the \f$dE/dx\f$ data.
	void TrackSelector::WriteDedxInfo(EvtRecTrack* evtRecTrack, NTupleContainer &tuple)
	{
		/// -# Abort if the 'write `JobSwitch`' has been set to `false`.
			if(!tuple.DoWrite()) return;

		/// -# Abort if \f$dE/dx\f$ and MDC info does not exist
			if(!evtRecTrack->isMdcTrackValid()) return;
			if(!evtRecTrack->isMdcDedxValid())  return;
			fTrackMDC  = evtRecTrack->mdcTrack();
			fTrackDedx = evtRecTrack->mdcDedx();

		/// -# Set \f$dE/dx\f$ info and set the `NTuple::Item`s.
			// tuple.at("dedx_K")     = fTrackDedx->getDedxExpect(3);
			// tuple.at("dedx_P")     = fTrackDedx->getDedxExpect(4);
			// tuple.at("dedx_e")     = fTrackDedx->getDedxExpect(0);
			// tuple.at("dedx_mom")   = fTrackDedx->getDedxMoment();
			// tuple.at("dedx_mu")    = fTrackDedx->getDedxExpect(1);
			// tuple.at("dedx_pi")    = fTrackDedx->getDedxExpect(2);
			tuple.at("Ngoodhits")  = fTrackDedx->numGoodHits();
			tuple.at("Ntotalhits") = fTrackDedx->numTotalHits();
			tuple.at("chie")       = fTrackDedx->chiE();
			tuple.at("chik")       = fTrackDedx->chiK();
			tuple.at("chimu")      = fTrackDedx->chiMu();
			tuple.at("chip")       = fTrackDedx->chiP();
			tuple.at("chipi")      = fTrackDedx->chiPi();
			tuple.at("normPH")     = fTrackDedx->normPH();
			tuple.at("p")          = fTrackMDC->p();
			tuple.at("probPH")     = fTrackDedx->probPH();
		
		/// -# @b Write \f$dE/dx\f$ info.
			tuple.Write();

	}


	/// Encapsulates a `for` loop of the writing procedure for \f$dE/dx\f$ energy loss information.
	/// This method allows you to write \f$dE/dx\f$ information for any selection of charged tracks. Just feed it a vector that contains such a collection of `EvtRecTrack` pointers.
	void TrackSelector::WriteDedxInfoForVector(std::vector<EvtRecTrack*> &vector, NTupleContainer &tuple)
	{
		/// -# Abort if the 'write `JobSwitch`' has been set to `false`.
			if(!tuple.DoWrite()) return;

		/// -# Loop over the vector of `EvtRecTrack` you fed and perform `WriteDedxInfo`.
			fLog << MSG::DEBUG << "Writing \"" << tuple.Name() << "\" info" << endmsg;
			for(fTrackIterator = vector.begin(); fTrackIterator != vector.end(); ++fTrackIterator)
				WriteDedxInfo(*fTrackIterator, tuple);
	}


	/// This `virtual` method has been declared in the base algorithm to standardise the writing of a fit tuple.
	/// @remark You should redefine the virtual `SetFitNTuple` method in your derived algorithm to specify what should be stored to this fit `NTuple`.
	void TrackSelector::WriteFitResults(KKFitResult *fitresults, NTupleContainer &tuple)
	{
		/// -# Abort if `fitresults` is not available.
			if(!fitresults || !*fitresults) {
				fLog << MSG::DEBUG << "KalmanKinematicFit for \"" << tuple.Name() << "\" is empty" << endmsg;
				return;
			}
			
		/// -# Performed derived version of the `virtual` `SetFitNTuple` method.
			SetFitNTuple(fitresults, tuple);

		/// -# @b Write \f$dE/dx\f$ info.
			tuple.Write();
			fLog << MSG::DEBUG << "Writing fit results \"" << tuple.Name() << "\"" << endmsg;
	}



	/// Encapsulates the proces of writing PID info. This allows you to write the PID information after the particle selection as well.
	void TrackSelector::WritePIDInformation()
	{
		/// -# Abort if the 'write `JobSwitch`' has been set to `false`.
			if(!fNTuple_PID.DoWrite()) return;

		/// -# Abort if there is no PID instance.
			if(!fPIDInstance) return;

		/// -# Get PID info and set the `NTuple::Item`s.
			fLog << MSG::DEBUG << "Writing PID information" << endmsg;
			fTrackMDC = (*fTrackIterator)->mdcTrack();
			if(fTrackMDC) {
				fNTuple_PID.at("p")    = fTrackMDC->p();
				fNTuple_PID.at("cost") = cos(fTrackMDC->theta());
			}
			fNTuple_PID.at("chiToFEC") = fPIDInstance->chiTofE(2);
			fNTuple_PID.at("chiToFIB") = fPIDInstance->chiTof1(2);
			fNTuple_PID.at("chiToFOB") = fPIDInstance->chiTof2(2);
			fNTuple_PID.at("chidEdx")  = fPIDInstance->chiDedx(2);
			fNTuple_PID.at("prob_K")   = fPIDInstance->probKaon();
			fNTuple_PID.at("prob_e")   = fPIDInstance->probElectron();
			fNTuple_PID.at("prob_mu")  = fPIDInstance->probMuon();
			fNTuple_PID.at("prob_p")   = fPIDInstance->probProton();
			fNTuple_PID.at("prob_pi")  = fPIDInstance->probPion();

		/// -# @b Write PID info.
			fNTuple_PID.Write();

	}


	/// Helper method for writing Time-of-Flight information. This function has be created to enable you to write TOF information for different collections of tracks.
	void TrackSelector::WriteTofInformation(SmartRefVector<RecTofTrack>::iterator iter_tof, double ptrk, NTupleContainer &tuple)
	{
		/// -# Abort if the 'write `JobSwitch`' has been set to `false`.
			if(!tuple.DoWrite()) return;

		/// -# Compute ToF for each particle hypothesis
			double path = (*iter_tof)->path();
			std::vector<double> texp(gNMasses);
			for(size_t j = 0; j < texp.size(); ++j) {
				double gb = ptrk/gMasses[j]; // v = p/m (non-relativistic velocity)
				double beta = gb/sqrt(1+gb*gb);
				texp[j] = 10 * path /beta/gSpeedOfLight; // hypothesis ToF
			}

		/// -# Set the `NTuple::Item`s.
			tuple.at("p")      = ptrk;
			tuple.at("tof")    = (*iter_tof)->tof();
			tuple.at("path")   = path;
			tuple.at("cntr")   = (*iter_tof)->tofID();
			tuple.at("ph")     = (*iter_tof)->ph();
			tuple.at("zrhit")  = (*iter_tof)->zrhit();
			tuple.at("qual")   = (*iter_tof)->quality();
			tuple.at("tof_e")  = path - texp[0];
			tuple.at("tof_mu") = path - texp[1];
			tuple.at("tof_pi") = path - texp[2];
			tuple.at("tof_K")  = path - texp[3];
			tuple.at("tof_p")  = path - texp[4];

		/// -# @b Write ToF info
			tuple.Write();

	}



// * ========================================= * //
// * -------- PARTICLE IDENTIFICATION -------- * //
// * ========================================= * //


	/// Method that standardizes the initialisation of the particle identification system. Define here <i>as general as possible</i>, but use in the derived subalgorithms.
	/// See http://bes3.to.infn.it/Boss/7.0.2/html/classParticleID.html for more info.
	/// @todo Since BOSS 7.0.4, `ParticleID::useTofCorr()` should be used for ToF instead of e.g. `useTof1`. See talk by Liu Huanhuan on 2019/01/10.
	/// @param method Which method to use: probability, likelihood, or neuron network (see `TSGlobals::PIDMethod`). You can also combine using e.g. `pid->methodLikelihood() | pid->methodProbability()`.
	/// @param pidsys PID systems you want to call. Can combined using bit seperators (`|`), e.g. `pid->useDedx() | pid->useTof1() | pid->useTof2() | pid->useTofE()` for \f$dE/dx\f$ plus all ToF detectors.
	/// @param pidcase Which particles to identify. For instance, `pid->onlyPion() | pid->onlyKaon()` in the case of pions and kaons.
	/// @param chimin Minimal \f$\chi^2\f$ of the resulting particle identification.
	ParticleID* TrackSelector::InitializePID(const int method, const int pidsys, const int pidcase, const double chimin)
	{

		// * Initialise PID sub-system and set method: probability, likelihood, or neuron network
		fPIDInstance->init();
		fPIDInstance->setMethod(method);
		fPIDInstance->setChiMinCut(chimin);
		fPIDInstance->setRecTrack(*fTrackIterator);

		// * Choose ID system and which particles to use
		fPIDInstance->usePidSys(pidsys);
		fPIDInstance->identify(pidcase);

		// * Perform PID
		fPIDInstance->calculate();
		if(!(fPIDInstance->IsPidInfoValid())) return nullptr;
		return fPIDInstance;
	}



// * ============================= * //
// * -------- CUT METHODS -------- * //
// * ============================= * //

	/// Helper function for `WriteCuts` that allows you to write one entry (usually: `min`, `max`, `counter`).
	template<typename TYPE>
	void TrackSelector::WriteCuts_entry(const TYPE &value)
	{
		std::list<CutObject*>::iterator cut = CutObject::gCutObjects.begin();
		for(cut; cut != CutObject::gCutObjects.end(); ++cut)
			fNTuple_cuts[(*cut)->Name()] = value;
		fNTuple_cuts.Write();
	}


	/// Declare properties for each `CutObject`. Each `CutObject` has two properties: a `min` and a `max`. This method has been added to the `TrackSelector`, and not to the `CutObject` class, because it requires the `Algorithm::decalareProperty` method.
	void TrackSelector::DeclareCuts()
	{
		std::list<CutObject*>::iterator cut = CutObject::gCutObjects.begin();
		for(; cut != CutObject::gCutObjects.end(); ++cut) {
			declareProperty((*cut)->NameMin(), (*cut)->min);
			declareProperty((*cut)->NameMax(), (*cut)->max);
			fLog << MSG::INFO << "  added cut \"" << (*cut)->Name() << "\"" << endmsg;
		}
	}


	/// Write all cuts (`name`, `value`, and `count` of accepted) to a branch called "_cutvalues".
	void TrackSelector::WriteCuts()
	{
		/// -# For each cut name, create an `NTuple::Item<double>` in the map `fNTuple_cuts`.
		std::list<CutObject*>::iterator cut;
		for(cut = CutObject::gCutObjects.begin(); cut != CutObject::gCutObjects.end(); ++cut) {
			fNTuple_cuts.AddItem((*cut)->Name());
		}
		/// -# Write `min` values to the first entry.
		WriteCuts_entry((*cut)->min);
		/// -# Write `max` values to the second entry.
		WriteCuts_entry((*cut)->max);
		/// -# Write the `counter` values to the third entry.
		WriteCuts_entry((*cut)->counter);
	}



// * ======================================= * //
// * -------- COMPUTATIONAL METHODS -------- * //
// * ======================================= * //


	/// Compute a 'momentum' for a neutral track.
	/// The momentum is computed from the neutral track (photon) energy and from the location (angles) where it was detected in the EMC.
	HepLorentzVector TrackSelector::ComputeMomentum(EvtRecTrack *track)
	{
		fTrackEMC = track->emcShower();
		double eraw = fTrackEMC->energy();
		double phi  = fTrackEMC->phi();
		double the  = fTrackEMC->theta();
		HepLorentzVector ptrk(
			eraw * sin(the) * cos(phi), // px
			eraw * sin(the) * sin(phi), // py
			eraw * cos(the),            // pz
			eraw);
		// ptrk = ptrk.boost(-0.011, 0, 0); // boost to center-of-mass frame
		return ptrk;
	}