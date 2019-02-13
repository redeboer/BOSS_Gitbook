// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "CLHEP/Geometry/Point3D.h"
	#include "CLHEP/Vector/LorentzVector.h"
	#include "CLHEP/Vector/ThreeVector.h"
	#include "CLHEP/Vector/TwoVector.h"
	#include "DstEvent/TofHitStatus.h"
	#include "EventModel/Event.h"
	#include "EventModel/EventHeader.h"
	#include "EventModel/EventModel.h"
	#include "EvtRecEvent/EvtRecEvent.h"
	#include "EvtRecEvent/EvtRecTrack.h"
	#include "GaudiKernel/AlgFactory.h"
	#include "GaudiKernel/Bootstrap.h"
	#include "GaudiKernel/IDataProviderSvc.h"
	#include "GaudiKernel/IHistogramSvc.h"
	#include "GaudiKernel/INTupleSvc.h"
	#include "GaudiKernel/ISvcLocator.h"
	#include "GaudiKernel/ISvcLocator.h"
	#include "GaudiKernel/MsgStream.h"
	#include "GaudiKernel/NTuple.h"
	#include "GaudiKernel/PropertyMgr.h"
	#include "GaudiKernel/SmartDataPtr.h"
	#include "ParticleID/ParticleID.h"
	#include "RhopiAlg/RhopiAlg.h"
	#include "TMath.h"
	#include "VertexFit/Helix.h"
	#include "VertexFit/IVertexDbSvc.h"
	#include "VertexFit/KalmanKinematicFit.h"
	#include "VertexFit/VertexFit.h"
	#include <vector>
	#ifndef ENABLE_BACKWARDS_COMPATIBILITY
		typedef HepGeom::Point3D<double> HepPoint3D;
	#endif
	using CLHEP::Hep2Vector;
	using CLHEP::Hep3Vector;
	using CLHEP::HepLorentzVector;



// * ==================================== * //
// * ------- GLOBALS AND TYPEDEFS ------- * //
// * ==================================== * //

	// * Constants * //
		const double mpi0  = 0.13957; // mass of pi0
		const double mrho0 = 0.770;   // mass of rho0
		const double xmass[5] = {
			0.000511, // electron
			0.105658, // muon
			0.139570, // charged pion
			0.493677, // charged kaon
			0.938272  // proton
		};
		const double velc_cm = 29.9792458; // tof_path unit in cm.
		const double velc_mm = 299.792458; // tof path unit in mm
		const double Ecms = 3.097;// center-of-mass energy


	// * Typedefs * //
		typedef std::vector<int> Vint;
		typedef std::vector<HepLorentzVector> Vp4;


	// * Counters for generating cut flow * //
		int Ncut0; // counter for all events (no cuts)
		int Ncut1; // nGood!=2 or nCharge!=0
		int Ncut2; // number of photons < 2
		int Ncut3; // pass PID
		int Ncut4; // pass fit4c
		int Ncut5; // pass fit5c
		int Ncut6; // "geff" branch



// * =========================== * //
// * ------- CONSTRUCTOR ------- * //
// * =========================== * //
	/**
	 * @brief Constructor for the `RhopiAlg` algorithm.
	 * @details Here, you should declare properties: give them a name, assign a parameter (data member of `RhopiAlg`), and if required a documentation string. Note that you should define the paramters themselves in the header (RhopiAlg/RhopiAlg.h) and that you should assign the values in `share/jopOptions_RhopiAlg.txt`.
	 */
	// Algorithms should inherit from Gaudi's `Algorithm` class. See https://dayabay.bnl.gov/dox/GaudiKernel/html/classAlgorithm.html.
	RhopiAlg::RhopiAlg(const std::string& name, ISvcLocator* pSvcLocator) :
		Algorithm(name, pSvcLocator)
	{

		// * Define r0, z0 cut for charged tracks *
		declareProperty("Vr0cut",    fVr0cut);
		declareProperty("Vz0cut",    fVz0cut);
		declareProperty("Vrvz0cut",  fRvz0cut);
		declareProperty("Vrvxy0cut", fRvxy0cut);

		// * Define energy, dphi, dthe cuts for fake gamma's *
		declareProperty("EnergyThreshold", fEnergyThreshold);
		declareProperty("GammaPhiCut",     fGammaPhiCut);
		declareProperty("GammaThetaCut",   fGammaThetaCut);
		declareProperty("GammaAngleCut",   fGammaAngleCut);

		// * Define invariant mass window cut *
		declareProperty("dM_rho0", fDeltaMrho0 = .150);

		// * Whether to test the success of the 4- and 5-constraint fits *
		declareProperty("Test4C",   fDo_fit4c); // write fit4c
		declareProperty("Test5C",   fDo_fit5c); // write fit5c and geff
		declareProperty("MaxChiSq", fMaxChiSq); // chisq for both fits should be less

		// * Whether or not to check success of Particle Identification *
		declareProperty("CheckDedx", fCheckDedx);
		declareProperty("CheckTof",  fCheckTof);

	}



// * ========================== * //
// * ------- INITIALIZE ------- * //
// * ========================== * //
	/**
	 * @brief   (Inherited) `initialize` step of `Algorithm`. This function is called only once in the beginning.
	 * @details Define and load `NTuple`s here.
	 */
	StatusCode RhopiAlg::initialize()
	{
		MsgStream log(msgSvc(), name());
		log << MSG::INFO << "In initialize():" << endmsg;
		/// <ul>
		/// <li> <b>`NTuple "vxyz"`</b>:   Vertex position
			/// <ul>
			NTuplePtr nt1(ntupleSvc(), "FILE1/vxyz");
			if ( nt1 ) fTupleVxyz = nt1;
			else {
				fTupleVxyz = ntupleSvc()->book("FILE1/vxyz", CLID_ColumnWiseTuple, "ks N-Tuple example");
				if(fTupleVxyz) {
					fTupleVxyz->addItem("vx0",    fVx0);    /// <li> `"vx0"`:    primary \f$x\f$-vertex as determined by MDC
					fTupleVxyz->addItem("vy0",    fVy0);    /// <li> `"vy0"`:    primary \f$y\f$-vertex as determined by MDC
					fTupleVxyz->addItem("vz0",    fVz0);    /// <li> `"vz0"`:    primary \f$z\f$-vertex as determined by MDC
					fTupleVxyz->addItem("vr0",    fVr0);    /// <li> `"vr0"`:    distance from origin in \f$xy\f$-plane
					fTupleVxyz->addItem("rvxy0",  fRvxy0);  /// <li> `"rvxy0"`:  nearest distance to IP in \f$xy\f$ plane
					fTupleVxyz->addItem("rvz0",   fRvz0);   /// <li> `"rvz0"`:   nearest distance to IP in \f$z\f$ direction
					fTupleVxyz->addItem("rvphi0", fRvphi0); /// <li> `"rvphi0"`: angle in the \f$xy\f$-plane (?) @todo Check definition of `"rvphi0"`
				}
				else {
					log << MSG::ERROR << "    Cannot book N-tuple:" << long(fTupleVxyz) << endmsg;
					return StatusCode::FAILURE;
				}
			}
			/// </ul>

		/// <li> <b>`NTuple "photon"`</b>: Photon kinematics
			/// <ul>
			NTuplePtr nt2(ntupleSvc(), "FILE1/photon");
			if ( nt2 ) fTupleAngles = nt2;
			else {
				fTupleAngles = ntupleSvc()->book("FILE1/photon", CLID_ColumnWiseTuple, "ks N-Tuple example");
				if(fTupleAngles) {
					fTupleAngles->addItem("dthe", fDeltaTheta); /// <li> `"dthe"` branch: theta difference with nearest charged track (\f$^\circ\f$)
					fTupleAngles->addItem("dphi", fDeltaPhi);   /// <li> `"dphi"` branch: phi difference with nearest charged track (\f$^\circ\f$)
					fTupleAngles->addItem("dang", fDeltaAngle); /// <li> `"dang"` branch: angle difference with nearest charged track
					fTupleAngles->addItem("eraw", fEraw);       /// <li> `"eraw"` branch: energy of the photon
				}
				else {
					log << MSG::ERROR << "    Cannot book N-tuple:" << long(fTupleAngles) << endmsg;
					return StatusCode::FAILURE;
				}
			}
			/// </ul>

		/// <li> <b>`NTuple "etot"`</b>:   Energy branch
			/// <ul>
			NTuplePtr nt3(ntupleSvc(), "FILE1/etot");
			if ( nt3 ) fTupleMgg = nt3;
			else {
				fTupleMgg = ntupleSvc()->book("FILE1/etot", CLID_ColumnWiseTuple, "ks N-Tuple example");
				if(fTupleMgg) {
					fTupleMgg->addItem("m2gg", fMtoGG); /// <li> `"m2gg"` branch: invariant mass of the two gammas
					fTupleMgg->addItem("etot", fEtot);  /// <li> `"etot"` branch: total energy of \f$\pi^+\f$, \f$\pi^-\f$ and the two gammas
				}
				else {
					log << MSG::ERROR << "    Cannot book N-tuple:" << long(fTupleMgg) << endmsg;
					return StatusCode::FAILURE;
				}
			}
			/// </ul>

		/// <li> <b>`NTuple "fit4c"`</b>:  Neutral pion (pi0) fit branch
			/// <ul>
			if(fDo_fit4c) {
				NTuplePtr nt4(ntupleSvc(), "FILE1/fit4c");
				if ( nt4 ) fTupleFit4C = nt4;
				else {
					fTupleFit4C = ntupleSvc()->book("FILE1/fit4c", CLID_ColumnWiseTuple, "ks N-Tuple example");
					if(fTupleFit4C ) {
						fTupleFit4C->addItem("mpi0", fMpi0); /// <li> `"mpi0"` branch: invariant \f$\pi^0\f$ mass according to Kalman kinematic fit
						fTupleFit4C->addItem("chi2", fChi1); /// <li> `"chi2"` branch: \f$\chi^2\f$ of the Kalman kinematic fit
					}
					else {
						log << MSG::ERROR << "    Cannot book N-tuple:" << long(fTupleFit4C) << endmsg;
						return StatusCode::FAILURE;
					}
				}
			}
			/// </ul>

		/// <li> <b>`NTuple "fit5c"`</b>:  Rho fit branch
			/// <ul>
			if(fDo_fit5c) {
				NTuplePtr nt5(ntupleSvc(), "FILE1/fit5c");
				if ( nt5 ) fTupleFit5C = nt5;
				else {
					fTupleFit5C = ntupleSvc()->book("FILE1/fit5c", CLID_ColumnWiseTuple, "ks N-Tuple example");
					if(fTupleFit5C) {
						fTupleFit5C->addItem("chi2",  fChi2);  /// <li> `"chi2"` branch:  \f$\chi^2\f$ of the Kalman kinematic fit
						fTupleFit5C->addItem("mrho0", fMrho0); /// <li> `"mrho0"` branch: invariant mass for \f$\pi^+\pi^-\f$ combination (\f$\rho^0\f$)
						fTupleFit5C->addItem("mrhop", fMrhop); /// <li> `"mrhop"` branch: invariant mass for \f$\pi^0\pi^+\f$ combination (\f$\rho^+\f$)
						fTupleFit5C->addItem("mrhom", fMrhom); /// <li> `"mrhom"` branch: invariant mass for \f$\pi^0\pi^-\f$ combination (\f$\rho^-\f$)
					}
					else {
						log << MSG::ERROR << "    Cannot book N-tuple:" << long(fTupleFit5C) << endmsg;
						return StatusCode::FAILURE;
					}
				}
			}
			/// </ul>

		/// <li> <b>`NTuple "geff"`</b>:   Photon detection efficiences
			/// <ul>
			if(fDo_fit5c) {
				NTuplePtr nt6(ntupleSvc(), "FILE1/geff");
				if ( nt6 ) fTuplePhoton = nt6;
				else {
					fTuplePhoton = ntupleSvc()->book("FILE1/geff", CLID_ColumnWiseTuple, "ks N-Tuple example");
					if(fTuplePhoton) {
						fTuplePhoton->addItem("fcos", fFcos); /// <li> `"fcos"` branch: \f$E/|\vec{p}|\f$ ratio for \f$\pi^0\f$ candidate
						fTuplePhoton->addItem("elow", fElow); /// <li> `"elow"` branch: lowest energy of the two photons
					}
					else {
						log << MSG::ERROR << "    Cannot book N-tuple:" << long(fTuplePhoton) << endmsg;
						return StatusCode::FAILURE;
					}
				}
			}
			/// </ul>

		/// <li> <b>`NTuple "dedx"`</b>:   Import dE/dx PID branch
			/// <ul>
			if(fCheckDedx) {
				NTuplePtr nt7(ntupleSvc(), "FILE1/dedx");
				if ( nt7 ) fTupleDedx = nt7;
				else {
					fTupleDedx = ntupleSvc()->book("FILE1/dedx", CLID_ColumnWiseTuple, "ks N-Tuple example");
					if(fTupleDedx) {
						fTupleDedx->addItem("ptrk",   fPtrack); /// <li> `"ptrk"` branch:   momentum of the track
						fTupleDedx->addItem("chie",   fChi2e);  /// <li> `"chie"` branch:   chi2 in case of electron
						fTupleDedx->addItem("chimu",  fChi2mu); /// <li> `"chimu"` branch:  chi2 in case of muon
						fTupleDedx->addItem("chipi",  fChi2pi); /// <li> `"chipi"` branch:  chi2 in case of pion
						fTupleDedx->addItem("chik",   fChi2k);  /// <li> `"chik"` branch:   chi2 in case of kaon
						fTupleDedx->addItem("chip",   fChi2p);  /// <li> `"chip"` branch:   chi2 in case of proton
						fTupleDedx->addItem("probPH", fProbPH); /// <li> `"probPH"` branch: most probable pulse height from truncated mean
						fTupleDedx->addItem("normPH", fNormPH); /// <li> `"normPH"` branch: normalized pulse height
						fTupleDedx->addItem("ghit",   fGhit);   /// <li> `"ghit"` branch:   number of good hits
						fTupleDedx->addItem("thit",   fThit);   /// <li> `"thit"` branch:   total number of hits
					}
					else {
						log << MSG::ERROR << "    Cannot book N-tuple:" << long(fTupleDedx) << endmsg;
						return StatusCode::FAILURE;
					}
				}
			}
			/// </ul>

		/// <li> <b>`NTuple "tofe"`</b>:   ToF endcap branch
			/// <ul>
			if(fCheckTof) {
				NTuplePtr nt8(ntupleSvc(), "FILE1/tofe");
				if ( nt8 ) fTupleTofEC = nt8;
				else {
					fTupleTofEC = ntupleSvc()->book("FILE1/tofe",CLID_ColumnWiseTuple, "ks N-Tuple example");
					if(fTupleTofEC) {
						fTupleTofEC->addItem("ptrk", fPtotTofEC);     /// <li> `"ptrk"` branch: momentum of the track as reconstructed by MDC
						fTupleTofEC->addItem("path", fPathTofEC);     /// <li> `"path"` branch: path length
						fTupleTofEC->addItem("tof",  fTofEC);         /// <li> `"tof"` branch:  time of flight
						fTupleTofEC->addItem("cntr", fCntrTofEC);     /// <li> `"cntr"` branch: ToF counter ID
						fTupleTofEC->addItem("ph",   fPhTofEC);       /// <li> `"ph"` branch:   ToF pulse height
						fTupleTofEC->addItem("rhit", fRhitTofEC);     /// <li> `"rhit"` branch: track extrapolate Z or R Hit position
						fTupleTofEC->addItem("qual", fQualTofEC);     /// <li> `"qual"` branch: data quality of reconstruction
						fTupleTofEC->addItem("te",   fElectronTofEC); /// <li> `"te"` branch:   difference with ToF in electron hypothesis
						fTupleTofEC->addItem("tmu",  fMuonTofEC);     /// <li> `"tmu"` branch:  difference with ToF in muon hypothesis
						fTupleTofEC->addItem("tpi",  fProtoniTofEC);  /// <li> `"tpi"` branch:  difference with ToF in charged pion hypothesis
						fTupleTofEC->addItem("tk",   fKaonTofEC);     /// <li> `"tk"` branch:   difference with ToF in charged kaon hypothesis
						fTupleTofEC->addItem("tp",   fProtonTofEC);   /// <li> `"tp"` branch:   difference with ToF in proton hypothesis
					}
					else {
						log << MSG::ERROR << "    Cannot book N-tuple:" << long(fTupleTofEC) << endmsg;
						return StatusCode::FAILURE;
					}
				}
			}
			/// </ul>

		/// <li> <b>`NTuple "tof1"`</b>:   ToF <i>inner</i> barrel branch
			/// <ul>
			if(fCheckTof) {
				NTuplePtr nt9(ntupleSvc(), "FILE1/tof1");
				if ( nt9 ) TupleTofIB = nt9;
				else {
					TupleTofIB = ntupleSvc()->book("FILE1/tof1", CLID_ColumnWiseTuple, "ks N-Tuple example");
					if(TupleTofIB) {
						TupleTofIB->addItem("ptrk", fPtotTofIB);     /// <li> `"ptrk"` branch: momentum of the track as reconstructed by MDC
						TupleTofIB->addItem("path", fPathTofIB);     /// <li> `"path"` branch: path length
						TupleTofIB->addItem("tof",  fTofIB);         /// <li> `"tof"` branch:  time of flight
						TupleTofIB->addItem("cntr", fCntrTofIB);     /// <li> `"cntr"` branch: ToF counter ID
						TupleTofIB->addItem("ph",   fPhTofIB);       /// <li> `"ph"` branch:   ToF pulse height
						TupleTofIB->addItem("zhit", fZhitTofIB);     /// <li> `"zhit"` branch: track extrapolate Z or R Hit position
						TupleTofIB->addItem("qual", fQualTofIB);     /// <li> `"qual"` branch: data quality of reconstruction
						TupleTofIB->addItem("te",   fElectronTofIB); /// <li> `"te"` branch:   difference with ToF in electron hypothesis
						TupleTofIB->addItem("tmu",  fMuonTofIB);     /// <li> `"tmu"` branch:  difference with ToF in muon hypothesis
						TupleTofIB->addItem("tpi",  fProtoniTofIB);  /// <li> `"tpi"` branch:  difference with ToF in charged pion hypothesis
						TupleTofIB->addItem("tk",   fKaonTofIB);     /// <li> `"tk"` branch:   difference with ToF in charged kaon hypothesis
						TupleTofIB->addItem("tp",   fProtonTofIB);   /// <li> `"tp"` branch:   difference with ToF in proton hypothesis
					}
					else {
						log << MSG::ERROR << "    Cannot book N-tuple:" << long(TupleTofIB) << endmsg;
						return StatusCode::FAILURE;
					}
				}
			}
			/// </ul>

		/// <li> <b>`NTuple "tof2"`</b>:   ToF <i>outer</i> barrel branch
			/// <ul>
			if(fCheckTof) {
				NTuplePtr nt10(ntupleSvc(), "FILE1/tof2");
				if ( nt10 ) TupleTofOB = nt10;
				else {
					TupleTofOB = ntupleSvc()->book("FILE1/tof2", CLID_ColumnWiseTuple, "ks N-Tuple example");
					if(TupleTofOB) {
						TupleTofOB->addItem("ptrk", fPtotTofOB);     /// <li> `"ptrk"` branch: momentum of the track as reconstructed by MDC
						TupleTofOB->addItem("path", fPathTofOB);     /// <li> `"path"` branch: path length
						TupleTofOB->addItem("tof",  fTofOB);         /// <li> `"tof"` branch:  time of flight
						TupleTofOB->addItem("cntr", fCntrTofOB);     /// <li> `"cntr"` branch: ToF counter ID
						TupleTofOB->addItem("ph",   fPhTofOB);       /// <li> `"ph"` branch:   ToF pulse height
						TupleTofOB->addItem("zhit", fZhitTofOB);     /// <li> `"zhit"` branch: track extrapolate Z or R Hit position
						TupleTofOB->addItem("qual", fQualTofOB);     /// <li> `"qual"` branch: data quality of reconstruction
						TupleTofOB->addItem("te",   fElectronTofOB); /// <li> `"te"` branch:   difference with ToF in electron hypothesis
						TupleTofOB->addItem("tmu",  fMuonTofOB);     /// <li> `"tmu"` branch:  difference with ToF in muon hypothesis
						TupleTofOB->addItem("tpi",  fProtoniTofOB);  /// <li> `"tpi"` branch:  difference with ToF in charged pion hypothesis
						TupleTofOB->addItem("tk",   fKaonTofOB);     /// <li> `"tk"` branch:   difference with ToF in charged kaon hypothesis
						TupleTofOB->addItem("tp",   fProtonTofOB);   /// <li> `"tp"` branch:   difference with ToF in proton hypothesis
					}
					else {
						log << MSG::ERROR << "    Cannot book N-tuple:" << long(TupleTofOB) << endmsg;
						return StatusCode::FAILURE;
					}
				}
			}
			/// </ul>

		/// <li> <b>`NTuple "pid"`</b>:    Track PID information
			/// <ul>
			NTuplePtr nt11(ntupleSvc(), "FILE1/pid");
			if ( nt11 ) fTuplePID = nt11;
			else {
				fTuplePID = ntupleSvc()->book("FILE1/pid", CLID_ColumnWiseTuple, "ks N-Tuple example");
				if(fTuplePID) {
					fTuplePID->addItem("ptrk", fPtrackPID); /// <li> `"ptrk"` branch: momentum of the track
					fTuplePID->addItem("cost", fCostPID);   /// <li> `"cost"` branch: theta angle of the track
					fTuplePID->addItem("dedx", fDedxPID);   /// <li> `"dedx"` branch: Chi2 of the dedx of the track
					fTuplePID->addItem("tof1", fTof1PID);   /// <li> `"tof1"` branch: Chi2 of the inner barrel ToF of the track
					fTuplePID->addItem("tof2", fTof2PID);   /// <li> `"tof2"` branch: Chi2 of the outer barrel ToF of the track
					fTuplePID->addItem("prob", fProbPID);   /// <li> `"prob"` branch: probability that it is a pion
				} else {
					log << MSG::ERROR << "    Cannot book N-tuple:" << long(fTuplePID) << endmsg;
					return StatusCode::FAILURE;
				}
			}
			/// </ul>

		/// </ul>
		log << MSG::INFO << "Successfully returned from initialize()" << endmsg;
		return StatusCode::SUCCESS;
	}



// * ========================= * //
// * -------- EXECUTE -------- * //
// * ========================= * //
	/**
	 * @brief Inherited `execute` method of the `Algorithm`. This function is called *for each event*.
	 */
	StatusCode RhopiAlg::execute()
	{
		/// <ol>
		/// <li> Create log stream (`MsgStream` class)
			MsgStream log(msgSvc(), name());
			log << MSG::INFO << "In execute():" << endmsg;

		/// <li> Load next event from DST file
			/// <b> Uses `Ncut0` counter</b>: no cut applied yet.

			// * Load DST file info *
			SmartDataPtr<Event::EventHeader> eventHeader(eventSvc(), "/Event/EventHeader");
			int runNo = eventHeader->runNumber();
			int evtNo = eventHeader->eventNumber();
			log << MSG::DEBUG << "run, evtnum = "
				<< runNo << " , "
				<< evtNo << endmsg;
			cout << "event number: " << evtNo << endl;
			Ncut0++; // counter for all events

			// * Load event information and track collection *
				/*
				http://bes3.to.infn.it/Boss/7.0.2/html/namespaceEventModel_1_1EvtRec.html (namespace)
				http://bes3.to.infn.it/Boss/7.0.2/html/classEvtRecEvent.html (class)
				http://bes3.to.infn.it/Boss/7.0.2/html/EvtRecTrack_8h.html (typedef EvtRecTrackCol)
				*/
			SmartDataPtr<EvtRecEvent>    evtRecEvent (eventSvc(), EventModel::EvtRec::EvtRecEvent);
			SmartDataPtr<EvtRecTrackCol> evtRecTrkCol(eventSvc(), EventModel::EvtRec::EvtRecTrackCol);

			// * Log number of events *
			log << MSG::DEBUG << "Ncharged, Nneutral, Ntotal = "
				<< evtRecEvent->totalCharged() << ", "
				<< evtRecEvent->totalNeutral() << ", "
				<< evtRecEvent->totalTracks() << endmsg;


		/// <li> Set vertex origin
			Hep3Vector xorigin(0,0,0);
			IVertexDbSvc* vtxsvc;
			Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);
			if(vtxsvc->isVertexValid()){
				double* dbv = vtxsvc->PrimaryVertex();
				double* vv = vtxsvc->SigmaPrimaryVertex();
				// HepVector dbv = fReader.PrimaryVertex(runNo);
				// HepVector vv  = fReader.SigmaPrimaryVertex(runNo);
				xorigin.setX(dbv[0]);
				xorigin.setY(dbv[1]);
				xorigin.setZ(dbv[2]);
			}


		/// <li> LOOP OVER CHARGED TRACKS: select charged tracks (eventual pions)
			/// <b> Uses `Ncut1` counter</b>: nGood has to be 2, mdcTrk->charge() has to be more than 0.
			// The first part of the set of reconstructed tracks are the charged tracks
			int nCharge = 0;
				//!< Number of charged tracks as identified by the MDC.
			Vint iGood;
				//!< vector of integers that give the position of tracks in the `evtRecEvent` marked good.
			for(int i = 0; i < evtRecEvent->totalCharged(); ++i) {

				// * Get track: beginning of all tracks + event number
				EvtRecTrackIterator itTrk = evtRecTrkCol->begin() + i; 
				if(!(*itTrk)->isMdcTrackValid()) continue;

				// * Get track info from Main Drift Chamber
				RecMdcTrack *mdcTrk = (*itTrk)->mdcTrack();
				double pch  = mdcTrk->p();
				double x0   = mdcTrk->x();
				double y0   = mdcTrk->y();
				double z0   = mdcTrk->z();
				double phi0 = mdcTrk->helix(1);

				// * Get vertex origin
				double xv   = xorigin.x();
				double yv   = xorigin.y();
				double Rxy  = (x0-xv)*cos(phi0)+(y0-yv)*sin(phi0);

				// * Get radii of vertex
				HepVector a = mdcTrk->helix();
				HepSymMatrix Ea = mdcTrk->err();
				HepPoint3D point0(0.,0.,0.); // the initial point for MDC recosntruction
				HepPoint3D IP(xorigin[0],xorigin[1],xorigin[2]);
				VFHelix helixip(point0,a,Ea);
				helixip.pivot(IP);
				HepVector vecipa = helixip.a();
				double Rvxy0  = fabs(vecipa[0]); // nearest distance to IP in xy plane
				double Rvz0   = vecipa[3];       // nearest distance to IP in z direction
				double Rvphi0 = vecipa[1];

				// * WRITE primary vertex position info ("vxyz" branch) *
				fVx0    = x0;     // primary x-vertex as determined by MDC
				fVy0    = y0;     // primary y-vertex as determined by MDC
				fVz0    = z0;     // primary z-vertex as determined by MDC
				fVr0    = Rxy;    // distance from origin in xy-plane
				fRvxy0  = Rvxy0;  // nearest distance to IP in xy plane
				fRvz0   = Rvz0;   // nearest distance to IP in z direction
				fRvphi0 = Rvphi0; // angle in the xy-plane (?)
				fTupleVxyz->write(); // "vxyz" branch

				// * Apply vertex cuts *
				if(fabs(z0)    >= fVz0cut)   continue;
				if(fabs(Rxy)   >= fVr0cut)   continue;
				if(fabs(Rvz0)  >= fRvz0cut)  continue;
				if(fabs(Rvxy0) >= fRvxy0cut) continue;

				// * Add charged track to vector *
				iGood.push_back(i);
				nCharge += mdcTrk->charge();

			}

			// * Finish Good Charged Track Selection * //
			int nGood = iGood.size();
			log << MSG::DEBUG << "ngood, totcharge = " << nGood << " , " << nCharge << endmsg;
			if((nGood != 2)||(nCharge!=0)){
				return StatusCode::SUCCESS;
			}
			Ncut1++; // nGood!=2 or nCharge!=0


		/// <li> LOOP OVER NEUTRAL TRACKS: select photons
			/// <b> Uses `Ncut2` counter</b>: number of good photons has to be 2 at least.
			// The second part of the set of reconstructed events consists of the neutral tracks, that is, the photons detected by the EMC (by clustering EMC crystal energies). Each neutral track is paired with each charged track and if their angle is smaller than a certain value (here, 200), the photon track is stored as 'good photon' (added to `iGam`).
			Vint iGam;
			for(int i = evtRecEvent->totalCharged(); i < evtRecEvent->totalTracks(); ++i) {

				// * Get track
				EvtRecTrackIterator itTrk = evtRecTrkCol->begin() + i;
				if(!(*itTrk)->isEmcShowerValid()) continue;
				RecEmcShower *emcTrk = (*itTrk)->emcShower();
				Hep3Vector emcpos(emcTrk->x(), emcTrk->y(), emcTrk->z());

				// * Find the theta, phi, and angle difference with nearest charged track
				double dthe = 200.; // start value for difference in theta
				double dphi = 200.; // start value for difference in phi
				double dang = 200.; // start value for difference in angle (?)
				for(int j = 0; j < evtRecEvent->totalCharged(); j++) {
					EvtRecTrackIterator jtTrk = evtRecTrkCol->begin() + j;
					if(!(*jtTrk)->isExtTrackValid()) continue;
					RecExtTrack *extTrk = (*jtTrk)->extTrack();
					if(extTrk->emcVolumeNumber() == -1) continue;
					Hep3Vector extpos = extTrk->emcPosition();
					// double ctht = extpos.cosTheta(emcpos);
					double angd = extpos.angle(emcpos);
					double thed = extpos.theta() - emcpos.theta();
					double phid = extpos.deltaPhi(emcpos);
					thed = fmod(thed + CLHEP::twopi + CLHEP::twopi + pi, CLHEP::twopi) - CLHEP::pi;
					phid = fmod(phid + CLHEP::twopi + CLHEP::twopi + pi, CLHEP::twopi) - CLHEP::pi;
					if(angd < dang){
						dang = angd;
						dthe = thed;
						dphi = phid;
					}
				}

				// * Apply angle cut
				if(dang>=200) continue;
				double eraw = emcTrk->energy();
				dthe = dthe * 180 / (CLHEP::pi);
				dphi = dphi * 180 / (CLHEP::pi);
				dang = dang * 180 / (CLHEP::pi);

				// * WRITE photon info ("photon" branch)
				fDeltaTheta = dthe; // theta difference with nearest charged track (degrees)
				fDeltaPhi = dphi; // phi difference with nearest charged track (degrees)
				fDeltaAngle = dang; // angle difference with nearest charged track
				fEraw = eraw; // energy of the photon
				fTupleAngles->write(); // "photon" branch

				// * Apply photon cuts
				if(eraw < fEnergyThreshold) continue;
				if((fabs(dthe) < fGammaThetaCut) && (fabs(dphi) < fGammaPhiCut) ) continue;
				if(fabs(dang) < fGammaAngleCut) continue;

				// * Add photon track to vector
				iGam.push_back(i);

			}

			// * Finish Good Photon Selection *
			int nGam = iGam.size();
			log << MSG::DEBUG << "Number of good photons: " << nGam << " , " << evtRecEvent->totalNeutral() << endmsg;
			if(nGam<2) {
				return StatusCode::SUCCESS;
			}
			Ncut2++; // number of photons < 2


		/// <li> Check charged track dEdx PID information
			if(fCheckDedx) {
				for(int i = 0; i < nGood; ++i) {

					// * Get track *
					EvtRecTrackIterator  itTrk = evtRecTrkCol->begin() + iGood[i];
					if(!(*itTrk)->isMdcTrackValid()) continue;
					if(!(*itTrk)->isMdcDedxValid())continue;
					RecMdcTrack* mdcTrk = (*itTrk)->mdcTrack();
					RecMdcDedx* dedxTrk = (*itTrk)->mdcDedx();

					// * WRITE energy loss PID info ("dedx" branch) *
					fPtrack   = mdcTrk->p();             // momentum of the track
					fChi2e   = dedxTrk->chiE();         // chi2 in case of electron
					fChi2mu  = dedxTrk->chiMu();        // chi2 in case of muon
					fChi2pi  = dedxTrk->chiPi();        // chi2 in case of pion
					fChi2k   = dedxTrk->chiK();         // chi2 in case of kaon
					fChi2p   = dedxTrk->chiP();         // chi2 in case of proton
					fProbPH = dedxTrk->probPH();       // most probable pulse height from truncated mean
					fNormPH = dedxTrk->normPH();       // normalized pulse height
					fGhit   = dedxTrk->numGoodHits();  // number of good hits
					fThit   = dedxTrk->numTotalHits(); // total number of hits
					fTupleDedx->write(); // "dedx" branch
				}
			}


		/// <li> Check charged track ToF PID information
			if(fCheckTof) {
				for(int i = 0; i < nGood; ++i) {
					EvtRecTrackIterator  itTrk = evtRecTrkCol->begin() + iGood[i];
					if(!(*itTrk)->isMdcTrackValid()) continue;
					if(!(*itTrk)->isTofTrackValid()) continue;

					RecMdcTrack * mdcTrk = (*itTrk)->mdcTrack();
					SmartRefVector<RecTofTrack> tofTrkCol = (*itTrk)->tofTrack();

					double ptrk = mdcTrk->p();

					SmartRefVector<RecTofTrack>::iterator iter_tof = tofTrkCol.begin();
					for(;iter_tof != tofTrkCol.end(); ++iter_tof) {
						TofHitStatus hitStatus;
						hitStatus.setStatus((*iter_tof)->status());

						// * If end cap ToF detector: *
						if(!(hitStatus.is_barrel())) {
							// *  *
							if( !(hitStatus.is_counter()) ) continue; // ?
							if(hitStatus.layer() != 0) continue; // abort if not end cap

							// * Get ToF info *
							double path = (*iter_tof)->path();       // distance of flight
							double tof  = (*iter_tof)->tof();        // time of flight
							double ph   = (*iter_tof)->ph();         // ToF pulse height
							double rhit = (*iter_tof)->zrhit();      // Track extrapolate Z or R Hit position
							double qual = 0.+(*iter_tof)->quality(); // data quality of reconstruction
							double cntr = 0.+(*iter_tof)->tofID();   // ToF counter ID

							// * Get ToF for each particle hypothesis *
							double texp[5];
							for(int j = 0; j < 5; j++) {
								double gb = ptrk/xmass[j]; // v = p/m (non-relativistic velocity)
								double beta = gb/sqrt(1+gb*gb);
								texp[j] = 10 * path /beta/velc_mm; // hypothesis ToF
							}

							// * WRITE ToF end cap info ("tofe" branch) *
							fPtotTofEC = ptrk;          // momentum of the track as reconstructed by MDC
							fPathTofEC = path;          // path length
							fTofEC  = tof;           // time of flight
							fCntrTofEC = cntr;          // ToF counter ID
							fPhTofEC   = ph;            // ToF pulse height
							fRhitTofEC = rhit;          // track extrapolate Z or R Hit position
							fQualTofEC = qual;          // data quality of reconstruction
							fElectronTofEC   = tof - texp[0]; // difference with ToF in electron hypothesis
							fMuonTofEC  = tof - texp[1]; // difference with ToF in muon hypothesis
							fProtoniTofEC  = tof - texp[2]; // difference with ToF in charged pion hypothesis
							fKaonTofEC   = tof - texp[3]; // difference with ToF in charged kaon hypothesis
							fProtonTofEC   = tof - texp[4]; // difference with ToF in proton hypothesis
							fTupleTofEC->write(); // "tofe" branch
						}

						// * If ebarrel ToF ToF detector: *
						else {
							if(!hitStatus.is_counter()) continue;
							if(hitStatus.layer() == 1) { // * inner barrel ToF detector
								double path = (*iter_tof)->path();       // distance of flight
								double tof  = (*iter_tof)->tof();        // time of flight
								double ph   = (*iter_tof)->ph();         // ToF pulse height
								double rhit = (*iter_tof)->zrhit();      // Track extrapolate Z or R Hit position
								double qual = 0.+(*iter_tof)->quality(); // data quality of reconstruction
								double cntr = 0.+(*iter_tof)->tofID();   // ToF counter ID
								double texp[5];
								for(int j = 0; j < 5; j++) {
									double gb = ptrk/xmass[j]; // v = p/m (non-relativistic velocity)
									double beta = gb/sqrt(1+gb*gb);
									texp[j] = 10 * path /beta/velc_mm; // hypothesis ToF
								}

								// * WRITE ToF inner barrel info ("tof1" branch) *
								fPtotTofIB = ptrk;          // momentum of the track as reconstructed by MDC
								fPathTofIB = path;          // path length
								fTofIB  = tof;           // time of flight
								fCntrTofIB = cntr;          // ToF counter ID
								fPhTofIB   = ph;            // ToF pulse height
								fZhitTofIB = rhit;          // track extrapolate Z or R Hit position
								fQualTofIB = qual;          // data quality of reconstruction
								fElectronTofIB   = tof - texp[0]; // difference with ToF in electron hypothesis
								fMuonTofIB  = tof - texp[1]; // difference with ToF in muon hypothesis
								fProtoniTofIB  = tof - texp[2]; // difference with ToF in charged pion hypothesis
								fKaonTofIB   = tof - texp[3]; // difference with ToF in charged kaon hypothesis
								fProtonTofIB   = tof - texp[4]; // difference with ToF in proton hypothesis
								TupleTofIB->write(); // "tof1" branch
							}

							if(hitStatus.layer() == 2) { // * outer barrel ToF detector
								double path = (*iter_tof)->path();       // distance of flight
								double tof  = (*iter_tof)->tof();        // ToF pulse height
								double ph   = (*iter_tof)->ph();         // ToF pulse height
								double rhit = (*iter_tof)->zrhit();      // track extrapolate Z or R Hit position
								double qual = 0.+(*iter_tof)->quality(); // data quality of reconstruction
								double cntr = 0.+(*iter_tof)->tofID();   // ToF counter ID
								double texp[5];
								for(int j = 0; j < 5; j++) {
									double gb = ptrk/xmass[j]; // v = p/m (non-relativistic velocity)
									double beta = gb/sqrt(1+gb*gb);
									texp[j] = 10 * path/beta/velc_mm; // hypothesis ToF
								}

								// * WRITE ToF outer barrel info ("tof2" branch) *
								fPtotTofOB = ptrk;          // momentum of the track as reconstructed by MDC
								fPathTofOB = path;          // path length
								fTofOB  = tof;           // time of flight
								fCntrTofOB = cntr;          // ToF counter ID
								fPhTofOB   = ph;            // ToF pulse height
								fZhitTofOB = rhit;          // track extrapolate Z or R Hit position
								fQualTofOB = qual;          // data quality of reconstruction
								fElectronTofOB   = tof - texp[0]; // difference with ToF in electron hypothesis
								fMuonTofOB  = tof - texp[1]; // difference with ToF in muon hypothesis
								fProtoniTofOB  = tof - texp[2]; // difference with ToF in charged pion hypothesis
								fKaonTofOB   = tof - texp[3]; // difference with ToF in charged kaon hypothesis
								fProtonTofOB   = tof - texp[4]; // difference with ToF in proton hypothesis
								TupleTofOB->write(); // "tof2" branch
							}
						}
					}
				} // loop all charged track
			}


		/// <li> Get 4-momentum for each photon
			Vp4 pGam;
			for(int i = 0; i < nGam; ++i) {
				EvtRecTrackIterator itTrk = evtRecTrkCol->begin() + iGam[i];
				RecEmcShower* emcTrk = (*itTrk)->emcShower();
				double eraw = emcTrk->energy();
				double phi = emcTrk->phi();
				double the = emcTrk->theta();
				HepLorentzVector ptrk;
				ptrk.setPx(eraw*sin(the)*cos(phi));
				ptrk.setPy(eraw*sin(the)*sin(phi));
				ptrk.setPz(eraw*cos(the));
				ptrk.setE(eraw);
				// ptrk = ptrk.boost(-0.011, 0, 0); // boost to cms
				pGam.push_back(ptrk);
			}
			// cout << "before pid" << endl;


		/// <li> Get 4-momentum for each charged track
			Vint ipip, ipim; // vector of number of good tracks
			Vp4  ppip, ppim; // vector of momenta
			ParticleID *pid = ParticleID::instance();
			for(int i = 0; i < nGood; ++i) {
				EvtRecTrackIterator itTrk = evtRecTrkCol->begin() + iGood[i];
				// if(pid) delete pid;
				pid->init();
				pid->setMethod(pid->methodProbability());
				// pid->setMethod(pid->methodLikelihood()); // for Likelihood Method

				pid->setChiMinCut(4);
				pid->setRecTrack(*itTrk);
				pid->usePidSys(pid->useDedx() | pid->useTof1() | pid->useTof2() | pid->useTofE()); // use PID sub-system
				pid->identify(pid->onlyPion() | pid->onlyKaon()); // seperater Pion/Kaon
				// pid->identify(pid->onlyPion());
				// pid->identify(pid->onlyKaon());
				pid->calculate();
				if(!(pid->IsPidInfoValid())) continue;
				RecMdcTrack* mdcTrk = (*itTrk)->mdcTrack();

				// * WRITE particle identification info ("pid" branch) *
				fPtrackPID = mdcTrk->p();          // momentum of the track
				fCostPID = cos(mdcTrk->theta()); // theta angle of the track
				fDedxPID = pid->chiDedx(2);      // Chi squared of the dedx of the track
				fTof1PID = pid->chiTof1(2);      // Chi squared of the inner barrel ToF of the track
				fTof2PID = pid->chiTof2(2);      // Chi squared of the outer barrel ToF of the track
				fProbPID = pid->probPion();      // probability that it is a pion
				fTuplePID->write(); // "pid" branch

				if(pid->probPion() < 0.001 || (pid->probPion() < pid->probKaon())) continue;
				if(pid->probPion() < 0.001) continue;
				if(pid->pdf(2)<pid->pdf(3)) continue; // for Likelihood Method (0=electron 1=muon 2=pion 3=kaon 4=proton)

				RecMdcKalTrack* mdcKalTrk = (*itTrk)->mdcKalTrack(); // After ParticleID, use RecMdcKalTrack substitute RecMdcTrack
				RecMdcKalTrack::setPidType(RecMdcKalTrack::pion); // PID can set to electron, muon, pion, kaon and proton;The default setting is pion

				if(mdcKalTrk->charge() >0) {
					ipip.push_back(iGood[i]);
					HepLorentzVector ptrk;
					ptrk.setPx(mdcKalTrk->px());
					ptrk.setPy(mdcKalTrk->py());
					ptrk.setPz(mdcKalTrk->pz());
					double p3 = ptrk.mag();
					ptrk.setE(sqrt(p3*p3+mpi0*mpi0));
					// ptrk = ptrk.boost(-0.011,0,0); // boost to cms
					ppip.push_back(ptrk);
				} else {
					ipim.push_back(iGood[i]);
					HepLorentzVector ptrk;
					ptrk.setPx(mdcKalTrk->px());
					ptrk.setPy(mdcKalTrk->py());
					ptrk.setPz(mdcKalTrk->pz());
					double p3 = ptrk.mag();
					ptrk.setE(sqrt(p3*p3+mpi0*mpi0));
					// ptrk = ptrk.boost(-0.011,0,0); // boost to cms
					ppim.push_back(ptrk);
				}
			}


		/// <li> RhopiAlg without PID
			// for(int i = 0; i < nGood; ++i) { 
				// EvtRecTrackIterator itTrk = evtRecTrkCol->begin() + iGood[i];
				// RecMdcTrack* mdcTrk = (*itTrk)->mdcTrack();
				// if(mdcTrk->charge() >0) {
					// ipip.push_back(iGood[i]);
					// HepLorentzVector ptrk;
					// ptrk.setPx(mdcTrk->px());
					// ptrk.setPy(mdcTrk->py());
					// ptrk.setPz(mdcTrk->pz());
					// double p3 = ptrk.mag();
					// ptrk.setE(sqrt(p3*p3+mpi0*mpi0));
					// ppip.push_back(ptrk);
				// } else {
					// ipim.push_back(iGood[i]);
					// HepLorentzVector ptrk;
					// ptrk.setPx(mdcTrk->px());
					// ptrk.setPy(mdcTrk->py());
					// ptrk.setPz(mdcTrk->pz());
					// double p3 = ptrk.mag();
					// ptrk.setE(sqrt(p3*p3+mpi0*mpi0));
					// ppim.push_back(ptrk);
				// }
			// } // without PID


			/// <b> Uses `Ncut3` counter</b>: ipip.size() * ipim.size() cannot be 1.
			int npip = ipip.size();
			int npim = ipim.size();
			if(npip*npim != 1) return SUCCESS;
			Ncut3++; // ipip.size() * ipim.size() cannot be 1


		/// <li> Loop over each gamma pair and store total energy
			HepLorentzVector pTot;
			for(int i = 0; i < nGam-1; ++i) {
				for(int j = i+1; j < nGam; j++) {
					HepLorentzVector p2g = pGam[i] + pGam[j];
					pTot = ppip[0] + ppim[0];
					pTot += p2g;

					// * WRITE total energy and pi^0 candidate inv. mass ("etot" branch) *
					fMtoGG = p2g.m();  // invariant mass of the two gammas
					fEtot = pTot.e(); // total energy of pi^+, pi^ and the two gammas
					fTupleMgg->write(); // "etot" branch
				}
			}

			RecMdcKalTrack *pipTrk = (*(evtRecTrkCol->begin()+ipip[0]))->mdcKalTrack();
			RecMdcKalTrack *pimTrk = (*(evtRecTrkCol->begin()+ipim[0]))->mdcKalTrack();

			WTrackParameter wvpipTrk, wvpimTrk;
			wvpipTrk = WTrackParameter(mpi0, pipTrk->getZHelix(), pipTrk->getZError());
			wvpimTrk = WTrackParameter(mpi0, pimTrk->getZHelix(), pimTrk->getZError());


		/// <li> Default is pion, for other particles:
			// wvppTrk = WTrackParameter(mp, pipTrk->getZHelixP(), pipTrk->getZErrorP()); // proton
			// wvmupTrk = WTrackParameter(mmu, pipTrk->getZHelixMu(), pipTrk->getZErrorMu()); // muon
			// wvepTrk = WTrackParameter(me, pipTrk->getZHelixE(), pipTrk->getZErrorE()); // electron
			// wvkpTrk = WTrackParameter(mk, pipTrk->getZHelixK(), pipTrk->getZErrorK()); // kaon


		/// <li> Test vertex fit
			HepPoint3D vx(0., 0., 0.);
			HepSymMatrix Evx(3, 0);
			double bx = 1E+6;
			double by = 1E+6;
			double bz = 1E+6;
			Evx[0][0] = bx*bx;
			Evx[1][1] = by*by;
			Evx[2][2] = bz*bz;

			VertexParameter vxpar;
			vxpar.setVx(vx);
			vxpar.setEvx(Evx);

			VertexFit* vtxfit = VertexFit::instance();
			vtxfit->init();
			vtxfit->AddTrack(0, wvpipTrk);
			vtxfit->AddTrack(1, wvpimTrk);
			vtxfit->AddVertex(0, vxpar,0, 1);
			if(!vtxfit->Fit(0)) return SUCCESS;
			vtxfit->Swim(0);

			WTrackParameter wpip = vtxfit->wtrk(0);
			WTrackParameter wpim = vtxfit->wtrk(1);

			// KinematicFit * kmfit = KinematicFit::instance();
			KalmanKinematicFit * kkmfit = KalmanKinematicFit::instance();


		/// <li> Apply Kalman 4-constrain kinematic fit
			if(fDo_fit4c) {
				HepLorentzVector ecms(0.034, 0, 0, Ecms);
				double chisq = 9999.;
				int ig1 = -1;
				int ig2 = -1;
				// * Run over all gamma pairs and find the pair with the best chi2
				for(int i = 0; i < nGam-1; ++i) {
					RecEmcShower *g1Trk = (*(evtRecTrkCol->begin()+iGam[i]))->emcShower();
					for(int j = i+1; j < nGam; j++) {
						RecEmcShower *g2Trk = (*(evtRecTrkCol->begin()+iGam[j]))->emcShower();
						kkmfit->init();
						kkmfit->AddTrack(0, wpip);       // pos. pion
						kkmfit->AddTrack(1, wpim);       // neg. pion
						kkmfit->AddTrack(2, 0.0, g1Trk); // first gamma track
						kkmfit->AddTrack(3, 0.0, g2Trk); // seconnd gamma track
						kkmfit->AddFourMomentum(0, ecms); // 4 constraints: CMS energy and momentum
						if(kkmfit->Fit()) {
							double chi2 = kkmfit->chisq();
							if(chi2 < chisq) {
								chisq = chi2;
								ig1 = iGam[i];
								ig2 = iGam[j];
							}
						}
					}
				}


			/// <b> Uses `Ncut4` counter</b>: fit4c passed and ChiSq less than fMaxChiSq.
				if(chisq < fMaxChiSq) {
					RecEmcShower *g1Trk = (*(evtRecTrkCol->begin()+ig1))->emcShower();
					RecEmcShower *g2Trk = (*(evtRecTrkCol->begin()+ig2))->emcShower();
					kkmfit->init();
					kkmfit->AddTrack(0, wpip);      // pos. pion
					kkmfit->AddTrack(1, wpim);      // neg. pion
					kkmfit->AddTrack(2, 0., g1Trk); // first gamma track
					kkmfit->AddTrack(3, 0., g2Trk); // second gamma track
					kkmfit->AddFourMomentum(0, ecms); // 4 constraints: CMS energy and momentum
					if(kkmfit->Fit()) {
						HepLorentzVector ppi0 = kkmfit->pfit(2) + kkmfit->pfit(3);

						// * WRITE pi^0 information from EMCal ("fit4c" branch) *
						fMpi0 = ppi0.m();        // invariant pi0 mass according to Kalman kinematic fit
						fChi1 = kkmfit->chisq(); // chi square of the Kalman kinematic fit
						fTupleFit4C->write(); // "fit4c" branch
						Ncut4++; // ChiSq has to be less than 200 and fit4c has to be passed
					}
				}
			}


		/// <li> Apply Kalman kinematic fit
			/// <b> Uses `Ncut5` counter</b>: Kalman kinematic fit 5c is successful.
			/// <b> Uses `Ncut6` counter</b>: \f$J/\psi \rightarrow \rho^0\pi^0\f$ (cut on invariant mass window).
			if(fDo_fit5c) {
				HepLorentzVector ecms(0.034, 0, 0, Ecms);
				double chisq = 9999.;
				int ig1 = -1;
				int ig2 = -1;
				// * Find the best combination over all possible pi+ pi- gamma gamma pair
				for(int i = 0; i < nGam-1; ++i) {
					RecEmcShower *g1Trk = (*(evtRecTrkCol->begin()+iGam[i]))->emcShower();
					for(int j = i+1; j < nGam; j++) {
						RecEmcShower *g2Trk = (*(evtRecTrkCol->begin()+iGam[j]))->emcShower();
						kkmfit->init();
						kkmfit->AddTrack(0, wpip);            // pos. pion
						kkmfit->AddTrack(1, wpim);            // neg. pion
						kkmfit->AddTrack(2, 0., g1Trk);       // first gamma track
						kkmfit->AddTrack(3, 0., g2Trk);       // second gamma track
						kkmfit->AddResonance(0, 0.135, 2, 3); // 5th constraint: pi0 resonance
						kkmfit->AddFourMomentum(1, ecms); // 4 constraints: CMS energy and momentum
						if(!kkmfit->Fit(0)) continue;
						if(!kkmfit->Fit(1)) continue;
						if(kkmfit->Fit()) {
							double chi2 = kkmfit->chisq();
							if(chi2 < chisq) {
								chisq = chi2;
								ig1 = iGam[i];
								ig2 = iGam[j];
							}
						}
					}
				}

				log << MSG::INFO << " chisq = " << chisq << endmsg;

				if(chisq < fMaxChiSq) {
					RecEmcShower* g1Trk = (*(evtRecTrkCol->begin()+ig1))->emcShower();
					RecEmcShower* g2Trk = (*(evtRecTrkCol->begin()+ig2))->emcShower();
					kkmfit->init();
					kkmfit->AddTrack(0, wpip);            // pi^+ track
					kkmfit->AddTrack(1, wpim);            // pi^- track
					kkmfit->AddTrack(2, 0., g1Trk);       // first gamma track
					kkmfit->AddTrack(3, 0., g2Trk);       // second gamma track
					kkmfit->AddResonance(0, 0.135, 2, 3); // 5th constraint: pi0 resonance
					kkmfit->AddFourMomentum(1, ecms); // 4 constraints: CMS energy and momentum

					// * Kalman kinematic fit5c * //
					if(kkmfit->Fit()){
						HepLorentzVector ppi0  = kkmfit->pfit(2) + kkmfit->pfit(3); // inv. mass Gamma Gamma (pi^0)
						HepLorentzVector prho0 = kkmfit->pfit(0) + kkmfit->pfit(1); // inv. mass pi^+ pi^- (rho^0)
						HepLorentzVector prhop = ppi0 + kkmfit->pfit(0);            // inv. mass pi^0 pi^+ (rho^+)
						HepLorentzVector prhom = ppi0 + kkmfit->pfit(1);            // inv. mass pi^0 pi^- (rho^-)
						double eg1 = (kkmfit->pfit(2)).e();
						double eg2 = (kkmfit->pfit(3)).e();
						double fcos = abs(eg1-eg2)/ppi0.rho();

						// * WRITE inv. mass from EMCal info ("fit5c" branch) *
						fChi2 = kkmfit->chisq(); // chi squared of the Kalman kinematic fit
						fMrho0 = prho0.m();      // inv. mass pi^+ pi^- (rho^0)
						fMrhop = prhop.m();      // inv. mass pi^0 pi^+ (rho^+)
						fMrhom = prhom.m();      // inv. mass pi^0 pi^- (rho^-)
						fTupleFit5C->write(); // "fit5c" branch
						Ncut5++; // Kalman kinematic fit 5c is successful


						// * Measure the photon detection efficiences via J/psi -> rho0 pi0 * //
						if(fabs(prho0.m() - mrho0) < fDeltaMrho0) {
							if(fabs(fcos) < 0.99) {
								// * WRITE photon detection efficiency info ("geff" branch) *
								fFcos = (eg1-eg2) / ppi0.rho();  // E/p ratio for pi^0 candidate
								fElow = (eg1 < eg2) ? eg1 : eg2; // lowest energy of the two gammas
								fTuplePhoton->write(); // "geff" branch
								Ncut6++; // 0.620 < mass of rho0 < 0.920
							}
						} // rho0 cut
					}
				}
			}


		/// </ol>
		return StatusCode::SUCCESS;
	}



// * ========================== * //
// * -------- FINALIZE -------- * //
// * ========================== * //
	/**
	 * @brief Inherited `finalize` method of `Algorithm`. This function is only called once, after running over all events.
	 * @details Prints the flow chart to the terminal, so <b>make sure you save this output!</b>
	 */
	StatusCode RhopiAlg::finalize()
	{
		MsgStream log(msgSvc(), name());
		log << MSG::INFO << "in finalize()" << endmsg;

		cout << "Resulting FLOW CHART:" << endl;
		cout << "  (0) Total number of events: " << Ncut0 << endl;
		cout << "  (1) nGood==2, nCharge==0:   " << Ncut1 << endl;
		cout << "  (2) nGam>=2:                " << Ncut2 << endl;
		cout << "  (3) Pass PID:               " << Ncut3 << endl;
		cout << "  (4) Pass 4C Kalman fit:     " << Ncut4 << endl;
		cout << "  (5) Pass 5C Kalman fit:     " << Ncut5 << endl;
		cout << "  (6) J/psi -> rho0 pi0:      " << Ncut6 << endl;
		cout << endl ;

		log << MSG::INFO << "Successfully returned from finalize()" << endmsg;
		return StatusCode::SUCCESS;
	}