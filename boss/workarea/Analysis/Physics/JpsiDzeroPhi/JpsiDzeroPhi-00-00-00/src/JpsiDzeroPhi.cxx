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
	#include "JpsiDzeroPhi/JpsiDzeroPhi.h"
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
 * @brief Constructor for the `JpsiDzeroPhi` algorithm.
 * @details Here, you should declare properties: give them a name, assign a parameter (data member of `JpsiDzeroPhi`), and if required a documentation string. Note that you should define the paramters themselves in the header (JpsiDzeroPhi/JpsiDzeroPhi.h) and that you should assign the values in `share/jopOptions_JpsiDzeroPhi.txt`.
 */
// Algorithms should inherit from Gaudi's `Algorithm` class. See https://dayabay.bnl.gov/dox/GaudiKernel/html/classAlgorithm.html.
JpsiDzeroPhi::JpsiDzeroPhi(const std::string& name, ISvcLocator* pSvcLocator) :
	Algorithm(name, pSvcLocator) {

	// * Define r0, z0 cut for charged tracks *
	declareProperty("Vr0cut",    m_vr0cut);
	declareProperty("Vz0cut",    m_vz0cut);
	declareProperty("Vrvz0cut",  m_rvz0cut);
	declareProperty("Vrvxy0cut", m_rvxy0cut);

	// * Define energy, dphi, dthe cuts for fake gamma's *
	declareProperty("EnergyThreshold", m_energyThreshold);
	declareProperty("GammaPhiCut",     m_gammaPhiCut);
	declareProperty("GammaThetaCut",   m_gammaThetaCut);
	declareProperty("GammaAngleCut",   m_gammaAngleCut);

	// * Define invariant mass window cut *
	declareProperty("dM_rho0", m_dmrho0 = .150);

	// * Whether to test the success of the 4- and 5-constraint fits *
	declareProperty("MaxChiSq", m_maxChiSq); // chisq for both fits should be less

}



// * ========================== * //
// * ------- INITIALIZE ------- * //
// * ========================== * //
/**
 * @brief   (Inherited) `initialize` step of `Algorithm`. This function is called only once in the beginning.
 * @details Define and load NTuples here.
 */
StatusCode JpsiDzeroPhi::initialize(){

	MsgStream log(msgSvc(), name());
	log << MSG::INFO << "In initialize():" << endmsg;

	// * NTuple: Vertex position (vxyz) * //
		NTuplePtr nt_vxyz(ntupleSvc(), "FILE1/vxyz");
		if(nt_vxyz) m_tuple_vxyz = nt_vxyz;
		else {
			m_tuple_vxyz = ntupleSvc()->book("FILE1/vxyz", CLID_ColumnWiseTuple, "ks N-Tuple example");
			if(m_tuple_vxyz) {
				m_tuple_vxyz->addItem("vx0",    m_vx0);    // primary x-vertex as determined by MDC
				m_tuple_vxyz->addItem("vy0",    m_vy0);    // primary y-vertex as determined by MDC
				m_tuple_vxyz->addItem("vz0",    m_vz0);    // primary z-vertex as determined by MDC
				m_tuple_vxyz->addItem("vr0",    m_vr0);    // distance from origin in xy-plane
				m_tuple_vxyz->addItem("rvxy0",  m_rvxy0);  // nearest distance to IP in xy plane
				m_tuple_vxyz->addItem("rvz0",   m_rvz0);   // nearest distance to IP in z direction
				m_tuple_vxyz->addItem("rvphi0", m_rvphi0); // angle in the xy-plane (?)
			}
			else {
				log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple_vxyz) << endmsg;
				return StatusCode::FAILURE;
			}
		}

	// * NTuple: Neutral pion (pi0) fit branch (fit4c) * //
		NTuplePtr nt_fit4c(ntupleSvc(), "FILE1/fit4c");
		if(nt_fit4c) m_tuple_fit4c = nt_fit4c;
		else {
			m_tuple_fit4c = ntupleSvc()->book("FILE1/fit4c", CLID_ColumnWiseTuple, "ks N-Tuple example");
			if(m_tuple_fit4c ) {
				m_tuple_fit4c->addItem("mD0",   m_mD0);   // inv. mass K^- pi^+ (D^0)
				m_tuple_fit4c->addItem("mphi",  m_mphi);  // inv. mass K^+ K^+  (phi)
				m_tuple_fit4c->addItem("mJpsi", m_mJpsi); // inv. mass D^0 phi  (J/psi)
				m_tuple_fit4c->addItem("chi2",  m_chisq); // chi squared of the Kalman kinematic fit
			}
			else {
				log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple_fit4c) << endmsg;
				return StatusCode::FAILURE;
			}
		}

	// * NTuple: Rho fit branch (fit6c) * //
		NTuplePtr nt_fit6c(ntupleSvc(), "FILE1/fit6c");
		if(nt_fit6c) m_tuple_fit6c = nt_fit6c;
		else {
			m_tuple_fit6c = ntupleSvc()->book("FILE1/fit6c", CLID_ColumnWiseTuple, "ks N-Tuple example");
			if(m_tuple_fit6c) {
				m_tuple_fit6c->addItem("mD0",   m_mD0);   // inv. mass K^- pi^+ (D^0)
				m_tuple_fit6c->addItem("mphi",  m_mphi);  // inv. mass K^+ K^+  (phi)
				m_tuple_fit6c->addItem("mJpsi", m_mJpsi); // inv. mass D^0 phi  (J/psi)
				m_tuple_fit6c->addItem("chi2",  m_chisq); // chi squared of the Kalman kinematic fit
			}
			else {
				log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple_fit6c) << endmsg;
				return StatusCode::FAILURE;
			}
		}

	// * NTuple: Import dE/dx PID branch (dedx) * //
		NTuplePtr nt_dedx(ntupleSvc(), "FILE1/dedx");
		if(nt_dedx) m_tuple_dedx = nt_dedx;
		else {
			m_tuple_dedx = ntupleSvc()->book("FILE1/dedx", CLID_ColumnWiseTuple, "ks N-Tuple example");
			if(m_tuple_dedx) {
				m_tuple_dedx->addItem("ptrk",   m_ptrk);   // momentum of the track
				m_tuple_dedx->addItem("chie",   m_chie);   // chi2 in case of electron
				m_tuple_dedx->addItem("chimu",  m_chimu);  // chi2 in case of muon
				m_tuple_dedx->addItem("chipi",  m_chipi);  // chi2 in case of pion
				m_tuple_dedx->addItem("chik",   m_chik);   // chi2 in case of kaon
				m_tuple_dedx->addItem("chip",   m_chip);   // chi2 in case of proton
				m_tuple_dedx->addItem("probPH", m_probPH); // most probable pulse height from truncated mean
				m_tuple_dedx->addItem("normPH", m_normPH); // normalized pulse height
				m_tuple_dedx->addItem("ghit",   m_ghit);   // number of good hits
				m_tuple_dedx->addItem("thit",   m_thit);   // total number of hits
			}
			else {
				log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple_dedx) << endmsg;
				return StatusCode::FAILURE;
			}
		}

	// * NTuple: ToF endcap branch (tofe) * //
		NTuplePtr nt_tofe(ntupleSvc(), "FILE1/tofe");
		if(nt_tofe) m_tuple_tof_ec = nt_tofe;
		else {
			m_tuple_tof_ec = ntupleSvc()->book("FILE1/tofe",CLID_ColumnWiseTuple, "ks N-Tuple example");
			if(m_tuple_tof_ec) {
				m_tuple_tof_ec->addItem("ptrk", m_ptot_etof); // momentum of the track as reconstructed by MDC
				m_tuple_tof_ec->addItem("path", m_path_etof); // path length
				m_tuple_tof_ec->addItem("tof",  m_tof_etof);  // time of flight
				m_tuple_tof_ec->addItem("cntr", m_cntr_etof); // ToF counter ID
				m_tuple_tof_ec->addItem("ph",   m_ph_etof);   // ToF pulse height
				m_tuple_tof_ec->addItem("rhit", m_rhit_etof); // track extrapolate Z or R Hit position
				m_tuple_tof_ec->addItem("qual", m_qual_etof); // data quality of reconstruction
				m_tuple_tof_ec->addItem("te",   m_te_etof);   // difference with ToF in electron hypothesis
				m_tuple_tof_ec->addItem("tmu",  m_tmu_etof);  // difference with ToF in muon hypothesis
				m_tuple_tof_ec->addItem("tpi",  m_tpi_etof);  // difference with ToF in charged pion hypothesis
				m_tuple_tof_ec->addItem("tk",   m_tk_etof);   // difference with ToF in charged kaon hypothesis
				m_tuple_tof_ec->addItem("tp",   m_tp_etof);   // difference with ToF in proton hypothesis
			}
			else {
				log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple_tof_ec) << endmsg;
				return StatusCode::FAILURE;
			}
		}

	// * NTuple: TToF inner barrel branch (tof1) * //
		NTuplePtr nt_tof1(ntupleSvc(), "FILE1/tof1");
		if(nt_tof1) m_tuple_tof_ib = nt_tof1;
		else {
			m_tuple_tof_ib = ntupleSvc()->book("FILE1/tof1", CLID_ColumnWiseTuple, "ks N-Tuple example");
			if(m_tuple_tof_ib) {
				m_tuple_tof_ib->addItem("ptrk", m_ptot_btof1); // momentum of the track as reconstructed by MDC
				m_tuple_tof_ib->addItem("path", m_path_btof1); // path length
				m_tuple_tof_ib->addItem("tof",  m_tof_btof1);  // time of flight
				m_tuple_tof_ib->addItem("cntr", m_cntr_btof1); // ToF counter ID
				m_tuple_tof_ib->addItem("ph",   m_ph_btof1);   // ToF pulse height
				m_tuple_tof_ib->addItem("zhit", m_zhit_btof1); // track extrapolate Z or R Hit position
				m_tuple_tof_ib->addItem("qual", m_qual_btof1); // data quality of reconstruction
				m_tuple_tof_ib->addItem("te",   m_te_btof1);   // difference with ToF in electron hypothesis
				m_tuple_tof_ib->addItem("tmu",  m_tmu_btof1);  // difference with ToF in muon hypothesis
				m_tuple_tof_ib->addItem("tpi",  m_tpi_btof1);  // difference with ToF in charged pion hypothesis
				m_tuple_tof_ib->addItem("tk",   m_tk_btof1);   // difference with ToF in charged kaon hypothesis
				m_tuple_tof_ib->addItem("tp",   m_tp_btof1);   // difference with ToF in proton hypothesis
			}
			else {
				log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple_tof_ib) << endmsg;
				return StatusCode::FAILURE;
			}
		}

	// * NTuple: check ToF outer barrel branch (tof2) * //
		NTuplePtr nt_tof2(ntupleSvc(), "FILE1/tof2");
		if(nt_tof2) m_tuple_tof_ob = nt_tof2;
		else {
			m_tuple_tof_ob = ntupleSvc()->book("FILE1/tof2", CLID_ColumnWiseTuple, "ks N-Tuple example");
			if(m_tuple_tof_ob) {
				m_tuple_tof_ob->addItem("ptrk", m_ptot_btof2); // momentum of the track as reconstructed by MDC
				m_tuple_tof_ob->addItem("path", m_path_btof2); // path length
				m_tuple_tof_ob->addItem("tof",  m_tof_btof2);  // time of flight
				m_tuple_tof_ob->addItem("cntr", m_cntr_btof2); // ToF counter ID
				m_tuple_tof_ob->addItem("ph",   m_ph_btof2);   // ToF pulse height
				m_tuple_tof_ob->addItem("zhit", m_zhit_btof2); // track extrapolate Z or R Hit position
				m_tuple_tof_ob->addItem("qual", m_qual_btof2); // data quality of reconstruction
				m_tuple_tof_ob->addItem("te",   m_te_btof2);   // difference with ToF in electron hypothesis
				m_tuple_tof_ob->addItem("tmu",  m_tmu_btof2);  // difference with ToF in muon hypothesis
				m_tuple_tof_ob->addItem("tpi",  m_tpi_btof2);  // difference with ToF in charged pion hypothesis
				m_tuple_tof_ob->addItem("tk",   m_tk_btof2);   // difference with ToF in charged kaon hypothesis
				m_tuple_tof_ob->addItem("tp",   m_tp_btof2);   // difference with ToF in proton hypothesis
			}
			else {
				log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple_tof_ob) << endmsg;
				return StatusCode::FAILURE;
			}
		}

	// * NTuple: Track PID information (pid) * //
		NTuplePtr nt_pid(ntupleSvc(), "FILE1/pid");
		if(nt_pid) m_tuple_pid = nt_pid;
		else {
			m_tuple_pid = ntupleSvc()->book("FILE1/pid", CLID_ColumnWiseTuple, "ks N-Tuple example");
			if(m_tuple_pid) {
				m_tuple_pid->addItem("ptrk", m_ptrk_pid); // momentum of the track
				m_tuple_pid->addItem("cost", m_cost_pid); // theta angle of the track
				m_tuple_pid->addItem("dedx", m_dedx_pid); // Chi2 of the dedx of the track
				m_tuple_pid->addItem("tof1", m_tof1_pid); // Chi2 of the inner barrel ToF of the track
				m_tuple_pid->addItem("tof2", m_tof2_pid); // Chi2 of the outer barrel ToF of the track
				m_tuple_pid->addItem("prob", m_prob_pid); // probability that it is a pion
			} else {
				log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple_pid) << endmsg;
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
StatusCode JpsiDzeroPhi::execute() {

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
		    << evtRecEvent->totalTracks() << endreq;


	// * Set vertex origin * //
		Hep3Vector xorigin(0,0,0);
		IVertexDbSvc* vtxsvc;
		Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);
		if(vtxsvc->isVertexValid()){
			double* dbv = vtxsvc->PrimaryVertex();
			double* vv = vtxsvc->SigmaPrimaryVertex();
			// HepVector dbv = m_reader.PrimaryVertex(runNo);
			// HepVector vv  = m_reader.SigmaPrimaryVertex(runNo);
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
			m_vx0    = x0;     // primary x-vertex as determined by MDC
			m_vy0    = y0;     // primary y-vertex as determined by MDC
			m_vz0    = z0;     // primary z-vertex as determined by MDC
			m_vr0    = Rxy;    // distance from origin in xy-plane
			m_rvxy0  = Rvxy0;  // nearest distance to IP in xy plane
			m_rvz0   = Rvz0;   // nearest distance to IP in z direction
			m_rvphi0 = Rvphi0; // angle in the xy-plane (?)
			m_tuple_vxyz->write(); // "vxyz" branch

			// * Apply vertex cuts *
			if(fabs(z0)    >= m_vz0cut)   continue;
			if(fabs(Rxy)   >= m_vr0cut)   continue;
			if(fabs(Rvz0)  >= m_rvz0cut)  continue;
			if(fabs(Rvxy0) >= m_rvxy0cut) continue;

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
			m_ptrk   = mdcTrk->p();             // momentum of the track
			m_chie   = dedxTrk->chiE();         // chi2 in case of electron
			m_chimu  = dedxTrk->chiMu();        // chi2 in case of muon
			m_chipi  = dedxTrk->chiPi();        // chi2 in case of pion
			m_chik   = dedxTrk->chiK();         // chi2 in case of kaon
			m_chip   = dedxTrk->chiP();         // chi2 in case of proton
			m_probPH = dedxTrk->probPH();       // most probable pulse height from truncated mean
			m_normPH = dedxTrk->normPH();       // normalized pulse height
			m_ghit   = dedxTrk->numGoodHits();  // number of good hits
			m_thit   = dedxTrk->numTotalHits(); // total number of hits
			m_tuple_dedx->write(); // "dedx" branch
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
					m_ptot_etof = ptrk;          // momentum of the track as reconstructed by MDC
					m_path_etof = path;          // path length
					m_tof_etof  = tof;           // time of flight
					m_cntr_etof = cntr;          // ToF counter ID
					m_ph_etof   = ph;            // ToF pulse height
					m_rhit_etof = rhit;          // track extrapolate Z or R Hit position
					m_qual_etof = qual;          // data quality of reconstruction
					m_te_etof   = tof - texp[0]; // difference with ToF in electron hypothesis
					m_tmu_etof  = tof - texp[1]; // difference with ToF in muon hypothesis
					m_tpi_etof  = tof - texp[2]; // difference with ToF in charged pion hypothesis
					m_tk_etof   = tof - texp[3]; // difference with ToF in charged kaon hypothesis
					m_tp_etof   = tof - texp[4]; // difference with ToF in proton hypothesis
					m_tuple_tof_ec->write(); // "tofe" branch
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
						m_ptot_btof1 = ptrk;          // momentum of the track as reconstructed by MDC
						m_path_btof1 = path;          // path length
						m_tof_btof1  = tof;           // time of flight
						m_cntr_btof1 = cntr;          // ToF counter ID
						m_ph_btof1   = ph;            // ToF pulse height
						m_zhit_btof1 = rhit;          // track extrapolate Z or R Hit position
						m_qual_btof1 = qual;          // data quality of reconstruction
						m_te_btof1   = tof - texp[0]; // difference with ToF in electron hypothesis
						m_tmu_btof1  = tof - texp[1]; // difference with ToF in muon hypothesis
						m_tpi_btof1  = tof - texp[2]; // difference with ToF in charged pion hypothesis
						m_tk_btof1   = tof - texp[3]; // difference with ToF in charged kaon hypothesis
						m_tp_btof1   = tof - texp[4]; // difference with ToF in proton hypothesis
						m_tuple_tof_ib->write(); // "tof1" branch
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
						m_ptot_btof2 = ptrk;          // momentum of the track as reconstructed by MDC
						m_path_btof2 = path;          // path length
						m_tof_btof2  = tof;           // time of flight
						m_cntr_btof2 = cntr;          // ToF counter ID
						m_ph_btof2   = ph;            // ToF pulse height
						m_zhit_btof2 = rhit;          // track extrapolate Z or R Hit position
						m_qual_btof2 = qual;          // data quality of reconstruction
						m_te_btof2   = tof - texp[0]; // difference with ToF in electron hypothesis
						m_tmu_btof2  = tof - texp[1]; // difference with ToF in muon hypothesis
						m_tpi_btof2  = tof - texp[2]; // difference with ToF in charged pion hypothesis
						m_tk_btof2   = tof - texp[3]; // difference with ToF in charged kaon hypothesis
						m_tp_btof2   = tof - texp[4]; // difference with ToF in proton hypothesis
						m_tuple_tof_ob->write(); // "tof2" branch
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
			m_ptrk_pid = mdcTrk->p();          // momentum of the track
			m_cost_pid = cos(mdcTrk->theta()); // theta angle of the track
			m_dedx_pid = pid->chiDedx(2);      // Chi squared of the dedx of the track
			m_tof1_pid = pid->chiTof1(2);      // Chi squared of the inner barrel ToF of the track
			m_tof2_pid = pid->chiTof2(2);      // Chi squared of the outer barrel ToF of the track
			m_prob_pid = pid->probPion();      // probability that it is a pion
			m_tuple_pid->write(); // "pid" branch

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
	// NOTE: Ncut3 -- fit4c passed and ChiSq less than m_maxChiSq

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
		if(kkmfit && chisq < m_maxChiSq) {
			HepLorentzVector pD0  = kkmfit->pfit(0) + kkmfit->pfit(1);
			HepLorentzVector pphi = kkmfit->pfit(2) + kkmfit->pfit(3);
			HepLorentzVector pJpsi = pD0 + pphi;
			m_mD0   = pD0.m();   // invariant D0 mass according to Kalman kinematic fit
			m_mphi  = pphi.m();  // invariant phi mass according to Kalman kinematic fit
			m_mJpsi = pJpsi.m(); // invariant Jpsi mass according to Kalman kinematic fit
			m_chisq = chisq;    // chi square of the Kalman kinematic fit
			m_tuple_fit4c->write(); // "fit4c" branch
			Ncut3++; // ChiSq has to be less than 200 and fit4c has to be passed
		}


	// * Find Kalman 5-constraint kinematic fit with smallest chi squared * //
	// NOTE: Ncut4 -- fit4c passed and ChiSq less than m_maxChiSq

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
		if(kkmfit && chisq < m_maxChiSq) {
			HepLorentzVector pD0  = kkmfit->pfit(0) + kkmfit->pfit(1);
			HepLorentzVector pphi = kkmfit->pfit(2) + kkmfit->pfit(3);
			HepLorentzVector pJpsi = pD0 + pphi;
			m_mD0   = pD0.m();   // invariant D0 mass according to Kalman kinematic fit
			m_mphi  = pphi.m();  // invariant phi mass according to Kalman kinematic fit
			m_mJpsi = pJpsi.m(); // invariant Jpsi mass according to Kalman kinematic fit
			m_chisq = chisq;    // chi square of the Kalman kinematic fit
			m_tuple_fit6c->write(); // "fit6c" branch
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
StatusCode JpsiDzeroPhi::finalize() {

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