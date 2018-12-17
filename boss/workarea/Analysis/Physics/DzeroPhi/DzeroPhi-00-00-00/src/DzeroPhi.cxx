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
	#include "DzeroPhi/DzeroPhi.h"
	#include "TMath.h"
	#include "VertexFit/Helix.h"
	#include "VertexFit/IVertexDbSvc.h"
	#include "VertexFit/KalmanKinematicFit.h"
	#include "VertexFit/VertexFit.h"
	#include <vector>
	// #include "VertexFit/KinematicFit.h"
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
		const double mpi  = 0.13957061; // mass of pi+-
		const double mpi0 = 0.1349770;  // mass of pi0
		const double mK   = 0.493677;   // mass of K+-
		const double mD0  = 1.86483;    // mass of D0
		const double mphi = 1.019461;   // mass of phi
		const double mrho = 0.77526;    // mass of rho0
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
		int Ncut1; // vertex cuts
		int Ncut2; // pass PID
		int Ncut3; // pass fit4c
		int Ncut4; // pass fit6c



// * =========================== * //
// * ------- CONSTRUCTOR ------- * //
// * =========================== * //
/**
 * @brief Constructor for the `DzeroPhi` algorithm.
 * @details Here, you should declare properties: give them a name, assign a parameter (data member of `DzeroPhi`), and if required a documentation string. Note that you should define the paramters themselves in the header (DzeroPhi/DzeroPhi.h) and that you should assign the values in `share/jopOptions_DzeroPhi.txt`.
 */
// Algorithms should inherit from Gaudi's `Algorithm` class. See https://dayabay.bnl.gov/dox/GaudiKernel/html/classAlgorithm.html.
DzeroPhi::DzeroPhi(const std::string& name, ISvcLocator* pSvcLocator) :
	Algorithm(name, pSvcLocator) {

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
	declareProperty("MaxChiSq", fMaxChiSq); // chisq for both fits should be less

}



// * ========================== * //
// * ------- INITIALIZE ------- * //
// * ========================== * //
/**
 * @brief   (Inherited) `initialize` step of `Algorithm`. This function is called only once in the beginning.
 * @details Define and load NTuples here.
 */
StatusCode DzeroPhi::initialize(){

	MsgStream log(msgSvc(), name());
	log << MSG::INFO << "In initialize():" << endmsg;

	// * NTuple: Vertex position (vxyz) * //
		NTuplePtr nt_vxyz(ntupleSvc(), "FILE1/vxyz");
		if(nt_vxyz) fTupleVxyz = nt_vxyz;
		else {
			fTupleVxyz = ntupleSvc()->book("FILE1/vxyz", CLID_ColumnWiseTuple, "ks N-Tuple example");
			if(fTupleVxyz) {
				fTupleVxyz->addItem("vx0",    fVx0);    // primary x-vertex as determined by MDC
				fTupleVxyz->addItem("vy0",    fVy0);    // primary y-vertex as determined by MDC
				fTupleVxyz->addItem("vz0",    fVz0);    // primary z-vertex as determined by MDC
				fTupleVxyz->addItem("vr0",    fVr0);    // distance from origin in xy-plane
				fTupleVxyz->addItem("rvxy0",  fRvxy0);  // nearest distance to IP in xy plane
				fTupleVxyz->addItem("rvz0",   fRvz0);   // nearest distance to IP in z direction
				fTupleVxyz->addItem("rvphi0", fRvphi0); // angle in the xy-plane (?)
			}
			else {
				log << MSG::ERROR << "    Cannot book N-tuple:" << long(fTupleVxyz) << endmsg;
				return StatusCode::FAILURE;
			}
		}

	// * NTuple: Neutral pion (pi0) fit branch (fit4c) * //
		NTuplePtr nt_fit4c(ntupleSvc(), "FILE1/fit4c");
		if(nt_fit4c) fTupleFit4C = nt_fit4c;
		else {
			fTupleFit4C = ntupleSvc()->book("FILE1/fit4c", CLID_ColumnWiseTuple, "ks N-Tuple example");
			if(fTupleFit4C ) {
				fTupleFit4C->addItem("mD0",   fM_D0);   // inv. mass K^- pi^+ (D^0)
				fTupleFit4C->addItem("mphi",  fM_phi);  // inv. mass K^+ K^+  (phi)
				fTupleFit4C->addItem("mJpsi", fM_Jpsi); // inv. mass D^0 phi  (J/psi)
				fTupleFit4C->addItem("chi2",  fChi2sq); // chi squared of the Kalman kinematic fit
			}
			else {
				log << MSG::ERROR << "    Cannot book N-tuple:" << long(fTupleFit4C) << endmsg;
				return StatusCode::FAILURE;
			}
		}

	// * NTuple: Rho fit branch (fit6c) * //
		NTuplePtr nt_fit6c(ntupleSvc(), "FILE1/fit6c");
		if(nt_fit6c) fTupleFit6C = nt_fit6c;
		else {
			fTupleFit6C = ntupleSvc()->book("FILE1/fit6c", CLID_ColumnWiseTuple, "ks N-Tuple example");
			if(fTupleFit6C) {
				fTupleFit6C->addItem("mD0",   fM_D0);   // inv. mass K^- pi^+ (D^0)
				fTupleFit6C->addItem("mphi",  fM_phi);  // inv. mass K^+ K^+  (phi)
				fTupleFit6C->addItem("mJpsi", fM_Jpsi); // inv. mass D^0 phi  (J/psi)
				fTupleFit6C->addItem("chi2",  fChi2sq); // chi squared of the Kalman kinematic fit
			}
			else {
				log << MSG::ERROR << "    Cannot book N-tuple:" << long(fTupleFit6C) << endmsg;
				return StatusCode::FAILURE;
			}
		}

	// * NTuple: Import dE/dx PID branch (dedx) * //
		NTuplePtr nt_dedx(ntupleSvc(), "FILE1/dedx");
		if(nt_dedx) fTupleDedx = nt_dedx;
		else {
			fTupleDedx = ntupleSvc()->book("FILE1/dedx", CLID_ColumnWiseTuple, "ks N-Tuple example");
			if(fTupleDedx) {
				fTupleDedx->addItem("ptrk",   fPtrack);   // momentum of the track
				fTupleDedx->addItem("chie",   fChi2e);   // chi2 in case of electron
				fTupleDedx->addItem("chimu",  fChi2mu);  // chi2 in case of muon
				fTupleDedx->addItem("chipi",  fChi2pi);  // chi2 in case of pion
				fTupleDedx->addItem("chik",   fChi2k);   // chi2 in case of kaon
				fTupleDedx->addItem("chip",   fChi2p);   // chi2 in case of proton
				fTupleDedx->addItem("probPH", fProbPH); // most probable pulse height from truncated mean
				fTupleDedx->addItem("normPH", fNormPH); // normalized pulse height
				fTupleDedx->addItem("ghit",   fGhit);   // number of good hits
				fTupleDedx->addItem("thit",   fThit);   // total number of hits
			}
			else {
				log << MSG::ERROR << "    Cannot book N-tuple:" << long(fTupleDedx) << endmsg;
				return StatusCode::FAILURE;
			}
		}

	// * NTuple: ToF endcap branch (tofe) * //
		NTuplePtr nt_tofe(ntupleSvc(), "FILE1/tofe");
		if(nt_tofe) fTupleTofEC = nt_tofe;
		else {
			fTupleTofEC = ntupleSvc()->book("FILE1/tofe",CLID_ColumnWiseTuple, "ks N-Tuple example");
			if(fTupleTofEC) {
				fTupleTofEC->addItem("ptrk", fPtotTofEC); // momentum of the track as reconstructed by MDC
				fTupleTofEC->addItem("path", fPathTofEC); // path length
				fTupleTofEC->addItem("tof",  fTofEC);  // time of flight
				fTupleTofEC->addItem("cntr", fCntrTofEC); // ToF counter ID
				fTupleTofEC->addItem("ph",   fPhTofEC);   // ToF pulse height
				fTupleTofEC->addItem("rhit", fRhitTofEC); // track extrapolate Z or R Hit position
				fTupleTofEC->addItem("qual", fQualTofEC); // data quality of reconstruction
				fTupleTofEC->addItem("te",   fElectronTofEC);   // difference with ToF in electron hypothesis
				fTupleTofEC->addItem("tmu",  fMuonTofEC);  // difference with ToF in muon hypothesis
				fTupleTofEC->addItem("tpi",  fProtoniTofEC);  // difference with ToF in charged pion hypothesis
				fTupleTofEC->addItem("tk",   fKaonTofEC);   // difference with ToF in charged kaon hypothesis
				fTupleTofEC->addItem("tp",   fProtonTofEC);   // difference with ToF in proton hypothesis
			}
			else {
				log << MSG::ERROR << "    Cannot book N-tuple:" << long(fTupleTofEC) << endmsg;
				return StatusCode::FAILURE;
			}
		}

	// * NTuple: TToF inner barrel branch (tof1) * //
		NTuplePtr nt_tof1(ntupleSvc(), "FILE1/tof1");
		if(nt_tof1) TupleTofIB = nt_tof1;
		else {
			TupleTofIB = ntupleSvc()->book("FILE1/tof1", CLID_ColumnWiseTuple, "ks N-Tuple example");
			if(TupleTofIB) {
				TupleTofIB->addItem("ptrk", fPtotTofIB); // momentum of the track as reconstructed by MDC
				TupleTofIB->addItem("path", fPathTofIB); // path length
				TupleTofIB->addItem("tof",  fTofIB);  // time of flight
				TupleTofIB->addItem("cntr", fCntrTofIB); // ToF counter ID
				TupleTofIB->addItem("ph",   fPhTofIB);   // ToF pulse height
				TupleTofIB->addItem("zhit", fZhitTofIB); // track extrapolate Z or R Hit position
				TupleTofIB->addItem("qual", fQualTofIB); // data quality of reconstruction
				TupleTofIB->addItem("te",   fElectronTofIB);   // difference with ToF in electron hypothesis
				TupleTofIB->addItem("tmu",  fMuonTofIB);  // difference with ToF in muon hypothesis
				TupleTofIB->addItem("tpi",  fProtoniTofIB);  // difference with ToF in charged pion hypothesis
				TupleTofIB->addItem("tk",   fKaonTofIB);   // difference with ToF in charged kaon hypothesis
				TupleTofIB->addItem("tp",   fProtonTofIB);   // difference with ToF in proton hypothesis
			}
			else {
				log << MSG::ERROR << "    Cannot book N-tuple:" << long(TupleTofIB) << endmsg;
				return StatusCode::FAILURE;
			}
		}

	// * NTuple: check ToF outer barrel branch (tof2) * //
		NTuplePtr nt_tof2(ntupleSvc(), "FILE1/tof2");
		if(nt_tof2) TupleTofOB = nt_tof2;
		else {
			TupleTofOB = ntupleSvc()->book("FILE1/tof2", CLID_ColumnWiseTuple, "ks N-Tuple example");
			if(TupleTofOB) {
				TupleTofOB->addItem("ptrk", fPtotTofOB); // momentum of the track as reconstructed by MDC
				TupleTofOB->addItem("path", fPathTofOB); // path length
				TupleTofOB->addItem("tof",  fTofOB);  // time of flight
				TupleTofOB->addItem("cntr", fCntrTofOB); // ToF counter ID
				TupleTofOB->addItem("ph",   fPhTofOB);   // ToF pulse height
				TupleTofOB->addItem("zhit", fZhitTofOB); // track extrapolate Z or R Hit position
				TupleTofOB->addItem("qual", fQualTofOB); // data quality of reconstruction
				TupleTofOB->addItem("te",   fElectronTofOB);   // difference with ToF in electron hypothesis
				TupleTofOB->addItem("tmu",  fMuonTofOB);  // difference with ToF in muon hypothesis
				TupleTofOB->addItem("tpi",  fProtoniTofOB);  // difference with ToF in charged pion hypothesis
				TupleTofOB->addItem("tk",   fKaonTofOB);   // difference with ToF in charged kaon hypothesis
				TupleTofOB->addItem("tp",   fProtonTofOB);   // difference with ToF in proton hypothesis
			}
			else {
				log << MSG::ERROR << "    Cannot book N-tuple:" << long(TupleTofOB) << endmsg;
				return StatusCode::FAILURE;
			}
		}

	// * NTuple: Track PID information (pid) * //
		NTuplePtr nt_pid(ntupleSvc(), "FILE1/pid");
		if(nt_pid) fTuplePID = nt_pid;
		else {
			fTuplePID = ntupleSvc()->book("FILE1/pid", CLID_ColumnWiseTuple, "ks N-Tuple example");
			if(fTuplePID) {
				fTuplePID->addItem("ptrk", fPtrackPID); // momentum of the track
				fTuplePID->addItem("cost", fCostPID); // theta angle of the track
				fTuplePID->addItem("dedx", fDedxPID); // Chi2 of the dedx of the track
				fTuplePID->addItem("tof1", fTof1PID); // Chi2 of the inner barrel ToF of the track
				fTuplePID->addItem("tof2", fTof2PID); // Chi2 of the outer barrel ToF of the track
				fTuplePID->addItem("prob", fProbPID); // probability that it is a pion
			} else {
				log << MSG::ERROR << "    Cannot book N-tuple:" << long(fTuplePID) << endmsg;
				return StatusCode::FAILURE;
			}
		}

	log << MSG::INFO << "Successfully returned from initialize()" << endmsg;
	return StatusCode::SUCCESS;
}



// * ========================= * //
// * -------- EXECUTE -------- * //
// * ========================= * //
/**
 * @brief Inherited `execute` method of the `Algorithm`. This function is called *for each event*.
 */
StatusCode DzeroPhi::execute() {

	MsgStream log(msgSvc(), name());
	log << MSG::INFO << "In execute():" << endreq;

	// * Load next event from DST file * //
	// NOTE: Ncut0 -- no cut yet

		// * Load DST file info *
		SmartDataPtr<Event::EventHeader> eventHeader(eventSvc(), "/Event/EventHeader");
		int runNo = eventHeader->runNumber();
		int evtNo = eventHeader->eventNumber();
		log << MSG::DEBUG << "run, evtnum = "
		    << runNo << " , "
		    << evtNo << endreq;
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
		    << evtRecEvent->totalTracks() << endreq;


	// * Set vertex origin * //
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


	// * LOOP OVER CHARGED TRACKS: select 'good' charged tracks * //
	// NOTE: Ncut1 -- vertex cuts
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
			double Rxy  =
				(x0 - xorigin.x()) * cos(phi0) +
				(y0 - xorigin.y()) * sin(phi0);

			// * Get radii of vertex
			HepVector a = mdcTrk->helix();
			HepSymMatrix Ea = mdcTrk->err();
			HepPoint3D point0(0., 0., 0.); // the initial point for MDC recosntruction
			HepPoint3D IP(xorigin[0], xorigin[1], xorigin[2]);
			VFHelix helixip(point0, a, Ea);
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
			// fTupleVxyz->write(); // "vxyz" branch

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
		log << MSG::DEBUG << "ngood, totcharge = " << iGood.size() << " , " << nCharge << endreq;
		Ncut1++; // vertex cuts


	// * Store dE/dx PID information * //
		for(int i = 0; i < iGood.size(); ++i) {

			// * Get track *
			EvtRecTrackIterator  itTrk = evtRecTrkCol->begin() + iGood[i];
			if(!(*itTrk)->isMdcTrackValid()) continue;
			if(!(*itTrk)->isMdcDedxValid())  continue;
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


	// * Store Time-of-Flight information * //
		for(int i = 0; i < iGood.size(); ++i) {
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
				if(!hitStatus.is_counter()) continue;

				// * If end cap ToF detector: *
				if(!hitStatus.is_barrel()) { // is not barrel
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

				// * If barrel ToF detector: *
				else {

					// * Inner barrel ToF detector
					if(hitStatus.layer() == 1) {
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

					// * Outer barrel ToF detector
					if(hitStatus.layer() == 2) {
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


	// * Get 4-momentum for each kaon and each pi+ * //
		Vint iKm, iKp, ipip; // serves as collection of pointers to which tracks in the collection are good
		Vp4  pKm, pKp, ppip; // vector of momenta
		ParticleID *pid = ParticleID::instance();
		for(int i = 0; i < iGood.size(); ++i) {

			// * Get 'good' track
			EvtRecTrackIterator itTrk = evtRecTrkCol->begin() + iGood[i];

			// * Initialise PID sub-system
			// if(pid) delete pid;
			pid->init();
			pid->setMethod(pid->methodProbability());
			// pid->setMethod(pid->methodLikelihood()); // for Likelihood Method
			pid->setChiMinCut(4);
			pid->setRecTrack(*itTrk);
			pid->usePidSys(pid->useDedx() | pid->useTof1() | pid->useTof2() | pid->useTofE()); // use PID sub-system
			pid->identify(pid->onlyPion() | pid->onlyKaon()); // seperater Pion/Kaon
			pid->calculate();
			if(!(pid->IsPidInfoValid())) continue;

			// * Get MDC track info *
			RecMdcTrack* mdcTrk = (*itTrk)->mdcTrack();

			// * WRITE particle identification info ("pid" branch) *
			fPtrackPID = mdcTrk->p();          // momentum of the track
			fCostPID = cos(mdcTrk->theta()); // theta angle of the track
			fDedxPID = pid->chiDedx(2);      // Chi squared of the dedx of the track
			fTof1PID = pid->chiTof1(2);      // Chi squared of the inner barrel ToF of the track
			fTof2PID = pid->chiTof2(2);      // Chi squared of the outer barrel ToF of the track
			fProbPID = pid->probPion();      // probability that it is a pion
			fTuplePID->write(); // "pid" branch

			// * If more likely to be pion (pi+-) *
			if(pid->probPion() > pid->probKaon()) {

				// * Check if indeed a pion
				if(pid->probPion() < 0.001) continue;
				if(pid->pdf(2) < pid->pdf(3)) continue;
					// for Likelihood Method (0=electron 1=muon 2=pion 3=kaon 4=proton)

				// * Get MDC Kalman track
				RecMdcKalTrack* mdcKalTrk = (*itTrk)->mdcKalTrack();
					// after ParticleID, use RecMdcKalTrack substitute RecMdcTrack
				RecMdcKalTrack::setPidType(RecMdcKalTrack::pion);
					// PID can set to electron, muon, pion (default), kaon and proton

				// * If positive pion
				if(mdcKalTrk->charge() > 0) {
					HepLorentzVector ptrk;
					ptrk.setPx(mdcKalTrk->px());
					ptrk.setPy(mdcKalTrk->py());
					ptrk.setPz(mdcKalTrk->pz());
					double p3 = ptrk.mag();
					ptrk.setE(sqrt(p3*p3 + mpi*mpi));
					// ptrk = ptrk.boost(-0.011,0,0); // boost to cms
					ipip.push_back(iGood[i]);
					ppip.push_back(ptrk);
				}

			}
			// * If more likely to be kaon (K+-)
			else {

				// * Check if indeed a kaon
				if(pid->probPion() < 0.001) continue;
				if(pid->pdf(3) < pid->pdf(2)) continue;
					// for Likelihood Method (0=electron 1=muon 2=pion 3=kaon 4=proton)

				// * Get MDC Kalman track
				RecMdcKalTrack* mdcKalTrk = (*itTrk)->mdcKalTrack();
					// after ParticleID, use RecMdcKalTrack substitute RecMdcTrack
				RecMdcKalTrack::setPidType(RecMdcKalTrack::kaon);
					// PID can set to electron, muon, pion (default), kaon and proton

				// * If positive kaon
				if(mdcKalTrk->charge() > 0) {
					HepLorentzVector ptrk;
					ptrk.setPx(mdcKalTrk->px());
					ptrk.setPy(mdcKalTrk->py());
					ptrk.setPz(mdcKalTrk->pz());
					double p3 = ptrk.mag();
					ptrk.setE(sqrt(p3*p3 + mK*mK));
					// ptrk = ptrk.boost(-0.011,0,0); // boost to cms
					iKp.push_back(iGood[i]);
					pKp.push_back(ptrk);
				}
				// * If negative kaon
				else {
					HepLorentzVector ptrk;
					ptrk.setPx(mdcKalTrk->px());
					ptrk.setPy(mdcKalTrk->py());
					ptrk.setPz(mdcKalTrk->pz());
					double p3 = ptrk.mag();
					ptrk.setE(sqrt(p3*p3 + mK*mK));
					// ptrk = ptrk.boost(-0.011,0,0); // boost to cms
					iKm.push_back(iGood[i]);
					pKm.push_back(ptrk);
				}
			}
		}


	// * Cut on number of particles * //
	// ! TODO Test if procedure can be made to work for arbitrary number of kaons and pions
	// NOTE: Ncut2 -- only events that numerically agree with Jpsi --> K- pi+ K- K+ (D0 phi)
		if(iKm.size()  != 2) return SUCCESS; // only two K-'s
		if(ipip.size() != 1) return SUCCESS; // only one pi+
		if(iKp.size()  != 1) return SUCCESS; // only one K+
		Ncut2++;


	// * Test all combinations with K- and store the ones closes to D0 and to phi * //

		// * Get Kalman tracks
		RecMdcKalTrack *pipTrk = (*(evtRecTrkCol->begin()+ipip[0]))->mdcKalTrack();
		RecMdcKalTrack *KpTrk  = (*(evtRecTrkCol->begin()+iKp[0])) ->mdcKalTrack();
		RecMdcKalTrack *KmTrk1 = (*(evtRecTrkCol->begin()+iKm[0])) ->mdcKalTrack();
		RecMdcKalTrack *KmTrk2 = (*(evtRecTrkCol->begin()+iKm[1])) ->mdcKalTrack();

		// * Get W-tracks
		WTrackParameter wvpipTrk(mpi, pipTrk->getZHelix(), pipTrk->getZError());
		WTrackParameter wvKpTrk (mK,  KpTrk->getZHelix(),  KpTrk->getZError());
		WTrackParameter wvKmTrk1(mK,  KmTrk1->getZHelix(), KmTrk1->getZError());
		WTrackParameter wvKmTrk2(mK,  KmTrk2->getZHelix(), KmTrk2->getZError());


	// * Test vertex fit * //
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
		vtxfit->AddTrack(1, wvKpTrk);
		vtxfit->AddTrack(2, wvKmTrk1);
		vtxfit->AddTrack(3, wvKmTrk2);
		vtxfit->AddVertex(0, vxpar, 0, 1);
		if(!vtxfit->Fit(0)) return SUCCESS;
		vtxfit->Swim(0);

		WTrackParameter wpip = vtxfit->wtrk(0);
		WTrackParameter wKp  = vtxfit->wtrk(1);
		WTrackParameter wKm1 = vtxfit->wtrk(2);
		WTrackParameter wKm2 = vtxfit->wtrk(3);


	// * Find Kalman 4-constraint kinematic fit with smallest chi squared * //
	// NOTE: Ncut3 -- fit4c passed and ChiSq less than fMaxChiSq

		// * Some local declarations
		HepLorentzVector ecms(0.034, 0, 0, Ecms);
		KalmanKinematicFit *kkmfit1 = KalmanKinematicFit::instance();
		KalmanKinematicFit *kkmfit2 = KalmanKinematicFit::instance();
		double chisq1 = 999999.;
		double chisq2 = 999999.;

		// * Get Kalman fit for first combination with K-
		kkmfit1->init();
		kkmfit1->AddTrack(0, wKm1);  // 1c: K- (1st occurrence)
		kkmfit1->AddTrack(1, wpip);  // 2c: pi+
		kkmfit1->AddTrack(2, wKm2);  // 3c: K- (2nd occurrence)
		kkmfit1->AddTrack(3, wKp);   // 4c: K+
		kkmfit1->AddFourMomentum(0, ecms);
		if(kkmfit1->Fit()) chisq1 = kkmfit1->chisq();

		// * Get Kalman fit for second combination with K-
		kkmfit2->init();
		kkmfit2->AddTrack(0, wKm2);  // 1c: K- (2nd occurrence)
		kkmfit2->AddTrack(1, wpip);  // 2c: pi+
		kkmfit2->AddTrack(2, wKm1);  // 3c: K- (1st occurrence)
		kkmfit2->AddTrack(3, wKp);   // 4c: K+
		kkmfit2->AddFourMomentum(0, ecms);
		if(kkmfit2->Fit()) chisq2 = kkmfit2->chisq();

		// * Get best Kalman fit
		KalmanKinematicFit *kkmfit = nullptr;
		double chisq = 999999.;
		if(chisq1 < chisq2) { kkmfit = kkmfit1; chisq = chisq1; }
		else                { kkmfit = kkmfit2; chisq = chisq2; }

		// * WRITE D0 and phi information from 4-constraint fit ("fit4c" branch)
		if(kkmfit && chisq < fMaxChiSq) {
			HepLorentzVector pD0  = kkmfit->pfit(0) + kkmfit->pfit(1);
			HepLorentzVector pphi = kkmfit->pfit(2) + kkmfit->pfit(3);
			HepLorentzVector pJpsi = pD0 + pphi;
			fM_D0   = pD0.m();   // invariant D0 mass according to Kalman kinematic fit
			fM_phi  = pphi.m();  // invariant phi mass according to Kalman kinematic fit
			fM_Jpsi = pJpsi.m(); // invariant Jpsi mass according to Kalman kinematic fit
			fChi2sq = chisq;    // chi square of the Kalman kinematic fit
			fTupleFit4C->write(); // "fit4c" branch
			Ncut3++; // ChiSq has to be less than 200 and fit4c has to be passed
		}


	// * Find Kalman 5-constraint kinematic fit with smallest chi squared * //
	// NOTE: Ncut4 -- fit4c passed and ChiSq less than fMaxChiSq

		// * Some local declarations
		chisq1 = 999999.;
		chisq2 = 999999.;

		// * Get Kalman fit for first combination with K-
		kkmfit1->init();
		kkmfit1->AddTrack(0, wKm1);           // 1c: K- (1st occurrence)
		kkmfit1->AddTrack(1, wpip);           // 2c: pi+
		kkmfit1->AddTrack(2, wKm2);           // 3c: K- (2nd occurrence)
		kkmfit1->AddTrack(3, wKp);            // 4c: K+
		kkmfit1->AddResonance(0, mD0,  0, 1); // 5c: D0 resonance
		kkmfit1->AddResonance(1, mphi, 2, 3); // 6c: phi resonance
		kkmfit1->AddFourMomentum(2, ecms);
		if(kkmfit1->Fit(0) && kkmfit1->Fit(1) && kkmfit1->Fit(2) && kkmfit1->Fit())
			chisq1 = kkmfit1->chisq();

		// * Get Kalman fit for second combination with K-
		kkmfit2->init();
		kkmfit2->AddTrack(0, wKm2);           // 1c: K- (2nd occurrence)
		kkmfit2->AddTrack(1, wpip);           // 2c: pi+
		kkmfit2->AddTrack(2, wKm1);           // 3c: K- (1st occurrence)
		kkmfit2->AddTrack(3, wKp);            // 4c: K+
		kkmfit2->AddResonance(0, mD0,  0, 1); // 5c: D0 resonance
		kkmfit2->AddResonance(1, mphi, 2, 3); // 6c: phi resonance
		kkmfit2->AddFourMomentum(2, ecms);
		if(kkmfit2->Fit(0) && kkmfit2->Fit(1) && kkmfit2->Fit(2) && kkmfit2->Fit())
			chisq2 = kkmfit2->chisq();

		// * Get best Kalman fit
		kkmfit = nullptr;
		chisq = 999999.;
		if(chisq1 < chisq2) { kkmfit = kkmfit1; chisq = chisq1; }
		else                { kkmfit = kkmfit2; chisq = chisq2; }

		// * WRITE D0 and phi information from 5-constraint fit ("fit6c" branch)
		if(kkmfit && chisq < fMaxChiSq) {
			HepLorentzVector pD0  = kkmfit->pfit(0) + kkmfit->pfit(1);
			HepLorentzVector pphi = kkmfit->pfit(2) + kkmfit->pfit(3);
			HepLorentzVector pJpsi = pD0 + pphi;
			fM_D0   = pD0.m();   // invariant D0 mass according to Kalman kinematic fit
			fM_phi  = pphi.m();  // invariant phi mass according to Kalman kinematic fit
			fM_Jpsi = pJpsi.m(); // invariant Jpsi mass according to Kalman kinematic fit
			fChi2sq = chisq;    // chi square of the Kalman kinematic fit
			fTupleFit6C->write(); // "fit6c" branch
			Ncut4++; // Kalman kinematic fit 6c is successful
		}

	// * Clean objects for this event * //
		if(kkmfit)  delete kkmfit;
		if(kkmfit1) delete kkmfit1;
		if(kkmfit2) delete kkmfit2;

	return StatusCode::SUCCESS;
}



// * ========================== * //
// * -------- FINALIZE -------- * //
// * ========================== * //
/**
 * @brief Inherited `finalize` method of `Algorithm`. This function is only called once, after running over all events.
 * @details Prints the flow chart to the terminal, so make sure you save this output!
 */
StatusCode DzeroPhi::finalize() {

	MsgStream log(msgSvc(), name());
	log << MSG::INFO << "in finalize()" << endmsg;

	// * Print flow chart * //
		cout << "Resulting FLOW CHART:" << endl;
		cout << "  (0) Total number of events: " << Ncut0 << endl;
		cout << "  (1) Vertex cuts:            " << Ncut1 << endl;
		cout << "  (3) Pass PID:               " << Ncut2 << endl;
		cout << "  (4) Pass 4C Kalman fit:     " << Ncut3 << endl;
		cout << "  (5) Pass 6C Kalman fit:     " << Ncut4 << endl;
		cout << endl ;

	log << MSG::INFO << "Successfully returned from finalize()" << endmsg;
	return StatusCode::SUCCESS;
}