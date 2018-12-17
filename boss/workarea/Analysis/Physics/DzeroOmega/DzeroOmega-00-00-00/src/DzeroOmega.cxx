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
	#include "DzeroOmega/DzeroOmega.h"
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
 * @brief Constructor for the `DzeroOmega` algorithm.
 * @details Here, you should declare properties: give them a name, assign a parameter (data member of `DzeroOmega`), and if required a documentation string. Note that you should define the paramters themselves in the header (DzeroOmega/DzeroOmega.h) and that you should assign the values in `share/jopOptions_DzeroOmega.txt`.
 */
// Algorithms should inherit from Gaudi's `Algorithm` class. See https://dayabay.bnl.gov/dox/GaudiKernel/html/classAlgorithm.html.
DzeroOmega::DzeroOmega(const std::string& name, ISvcLocator* pSvcLocator) :
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
	declareProperty("Test4C",   m_test4C); // write fit4c
	declareProperty("Test5C",   m_test5C); // write fit5c and geff
	declareProperty("MaxChiSq", m_maxChiSq); // chisq for both fits should be less

	// * Whether or not to check success of Particle Identification *
	declareProperty("CheckDedx", m_checkDedx);
	declareProperty("CheckTof",  m_checkTof);

}



// * ========================== * //
// * ------- INITIALIZE ------- * //
// * ========================== * //
/**
 * @brief   (Inherited) `initialize` step of `Algorithm`. This function is called only once in the beginning.
 * @details Define and load NTuples here.
 */
StatusCode DzeroOmega::initialize(){

	MsgStream log(msgSvc(), name());
	log << MSG::INFO << "In initialize():" << endmsg;

	// * NTuple: Vertex position (vxyz) * //
		NTuplePtr nt1(ntupleSvc(), "FILE1/vxyz");
		if ( nt1 ) m_tuple_vxyz = nt1;
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

	// * NTuple: Photon kinematics (photon) * //
		NTuplePtr nt2(ntupleSvc(), "FILE1/photon");
		if ( nt2 ) m_tuple_ang = nt2;
		else {
			m_tuple_ang = ntupleSvc()->book("FILE1/photon", CLID_ColumnWiseTuple, "ks N-Tuple example");
			if(m_tuple_ang) {
				m_tuple_ang->addItem("dthe", m_dthe); // theta difference with nearest charged track (degrees)
				m_tuple_ang->addItem("dphi", m_dphi); // phi difference with nearest charged track (degrees)
				m_tuple_ang->addItem("dang", m_dang); // angle difference with nearest charged track
				m_tuple_ang->addItem("eraw", m_eraw); // energy of the photon
			}
			else {
				log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple_ang) << endmsg;
				return StatusCode::FAILURE;
			}
		}

	// * NTuple: Energy branch (etot) * //
		NTuplePtr nt3(ntupleSvc(), "FILE1/etot");
		if ( nt3 ) m_tuple_mgg = nt3;
		else {
			m_tuple_mgg = ntupleSvc()->book("FILE1/etot", CLID_ColumnWiseTuple, "ks N-Tuple example");
			if(m_tuple_mgg) {
				m_tuple_mgg->addItem("m2gg", m_m2gg); // invariant mass of the two gammas
				m_tuple_mgg->addItem("etot", m_etot); // total energy of pi^+, pi^ and the two gammas
			}
			else {
				log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple_mgg) << endmsg;
				return StatusCode::FAILURE;
			}
		}

	// * NTuple: Neutral pion (pi0) fit branch (fit4c) * //
		if(m_test4C) {
			NTuplePtr nt4(ntupleSvc(), "FILE1/fit4c");
			if ( nt4 ) m_tuple_fit4c = nt4;
			else {
				m_tuple_fit4c = ntupleSvc()->book("FILE1/fit4c", CLID_ColumnWiseTuple, "ks N-Tuple example");
				if(m_tuple_fit4c ) {
					m_tuple_fit4c->addItem("mpi0", m_mpi0); // invariant pi0 mass according to Kalman kinematic fit
					m_tuple_fit4c->addItem("chi2", m_chi1); // chi square of the Kalman kinematic fit
				}
				else {
					log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple_fit4c) << endmsg;
					return StatusCode::FAILURE;
				}
			}
		}

	// * NTuple: Rho fit branch (fit5c) * //
		if(m_test5C) {
			NTuplePtr nt5(ntupleSvc(), "FILE1/fit5c");
			if ( nt5 ) m_tuple_fit5c = nt5;
			else {
				m_tuple_fit5c = ntupleSvc()->book("FILE1/fit5c", CLID_ColumnWiseTuple, "ks N-Tuple example");
				if(m_tuple_fit5c) {
					m_tuple_fit5c->addItem("chi2",  m_chi2);  // chi squared of the Kalman kinematic fit
					m_tuple_fit5c->addItem("mrho0", m_mrho0); // inv. mass pi^+ pi^- (rho^0)
					m_tuple_fit5c->addItem("mrhop", m_mrhop); // inv. mass pi^0 pi^+ (rho^+)
					m_tuple_fit5c->addItem("mrhom", m_mrhom); // inv. mass pi^0 pi^- (rho^-)
				}
				else {
					log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple_fit5c) << endmsg;
					return StatusCode::FAILURE;
				}
			}

	// * NTuple: Photon detection efficiences (geff) * //
			NTuplePtr nt6(ntupleSvc(), "FILE1/geff");
			if ( nt6 ) m_tuple_photon = nt6;
			else {
				m_tuple_photon = ntupleSvc()->book("FILE1/geff", CLID_ColumnWiseTuple, "ks N-Tuple example");
				if(m_tuple_photon) {
					m_tuple_photon->addItem("fcos", m_fcos); // E/p ratio for pi^0 candidate
					m_tuple_photon->addItem("elow", m_elow); // lowest energy of the two gammas
				}
				else {
					log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple_photon) << endmsg;
					return StatusCode::FAILURE;
				}
			}
		}

	// * NTuple: Import dE/dx PID branch (dedx) * //
		if(m_checkDedx) {
			NTuplePtr nt7(ntupleSvc(), "FILE1/dedx");
			if ( nt7 ) m_tuple_dedx = nt7;
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
		}

	// * NTuple: ToF endcap branch (tofe) * //
		if(m_checkTof) {
			NTuplePtr nt8(ntupleSvc(), "FILE1/tofe");
			if ( nt8 ) m_tuple_tof_ec = nt8;
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
		}

	// * NTuple: TToF inner barrel branch (tof1) * //
		if(m_checkTof) {
			NTuplePtr nt9(ntupleSvc(), "FILE1/tof1");
			if ( nt9 ) m_tuple_tof_ib = nt9;
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
		}

	// * NTuple: check ToF outer barrel branch (tof2) * //
		if(m_checkTof) {
			NTuplePtr nt10(ntupleSvc(), "FILE1/tof2");
			if ( nt10 ) m_tuple_tof_ob = nt10;
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
		}

	// * NTuple: Track PID information (pid) * //
		NTuplePtr nt11(ntupleSvc(), "FILE1/pid");
		if ( nt11 ) m_tuple_pid = nt11;
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
StatusCode DzeroOmega::execute() {

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


	// * LOOP OVER CHARGED TRACKS: select charged tracks (eventual pions) * //
	// NOTE: Ncut1 -- nGood has to be 2, mdcTrk->charge() has to be more than 0
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
		int nGood = iGood.size();
		log << MSG::DEBUG << "ngood, totcharge = " << nGood << " , " << nCharge << endreq;
		if((nGood != 2)||(nCharge!=0)){
			return StatusCode::SUCCESS;
		}
		Ncut1++; // nGood!=2 or nCharge!=0


	// * LOOP OVER NEUTRAL TRACKS: select photons * //
	// NOTE: Ncut2 -- number of good photons has to be 2 at least
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
			m_dthe = dthe; // theta difference with nearest charged track (degrees)
			m_dphi = dphi; // phi difference with nearest charged track (degrees)
			m_dang = dang; // angle difference with nearest charged track
			m_eraw = eraw; // energy of the photon
			m_tuple_ang->write(); // "photon" branch

			// * Apply photon cuts
			if(eraw < m_energyThreshold) continue;
			if((fabs(dthe) < m_gammaThetaCut) && (fabs(dphi) < m_gammaPhiCut) ) continue;
			if(fabs(dang) < m_gammaAngleCut) continue;

			// * Add photon track to vector
			iGam.push_back(i);

		}

		// * Finish Good Photon Selection *
		int nGam = iGam.size();
		log << MSG::DEBUG << "Number of good photons: " << nGam << " , " << evtRecEvent->totalNeutral() << endreq;
		if(nGam<2) {
			return StatusCode::SUCCESS;
		}
		Ncut2++; // number of photons < 2


	// * Check charged track dEdx PID information * //
		if(m_checkDedx) {
			for(int i = 0; i < nGood; ++i) {

				// * Get track *
				EvtRecTrackIterator  itTrk = evtRecTrkCol->begin() + iGood[i];
				if(!(*itTrk)->isMdcTrackValid()) continue;
				if(!(*itTrk)->isMdcDedxValid())continue;
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
		}


	// * Check charged track ToF PID information * //
		if(m_checkTof) {
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
		}


	// * Get 4-momentum for each photon * //
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


	// * Get 4-momentum for each charged track * //
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
			m_ptrk_pid = mdcTrk->p();          // momentum of the track
			m_cost_pid = cos(mdcTrk->theta()); // theta angle of the track
			m_dedx_pid = pid->chiDedx(2);      // Chi squared of the dedx of the track
			m_tof1_pid = pid->chiTof1(2);      // Chi squared of the inner barrel ToF of the track
			m_tof2_pid = pid->chiTof2(2);      // Chi squared of the outer barrel ToF of the track
			m_prob_pid = pid->probPion();      // probability that it is a pion
			m_tuple_pid->write(); // "pid" branch

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


	// * DzeroOmega without PID * //
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


	// NOTE: Ncut3 -- ipip.size() * ipim.size() cannot be 1
		int npip = ipip.size();
		int npim = ipim.size();
		if(npip*npim != 1) return SUCCESS;
		Ncut3++; // ipip.size() * ipim.size() cannot be 1


	// * Loop over each gamma pair and store total energy * //
		HepLorentzVector pTot;
		for(int i = 0; i < nGam-1; ++i) {
			for(int j = i+1; j < nGam; j++) {
				HepLorentzVector p2g = pGam[i] + pGam[j];
				pTot = ppip[0] + ppim[0];
				pTot += p2g;

				// * WRITE total energy and pi^0 candidate inv. mass ("etot" branch) *
				m_m2gg = p2g.m();  // invariant mass of the two gammas
				m_etot = pTot.e(); // total energy of pi^+, pi^ and the two gammas
				m_tuple_mgg->write(); // "etot" branch
			}
		}

		RecMdcKalTrack *pipTrk = (*(evtRecTrkCol->begin()+ipip[0]))->mdcKalTrack();
		RecMdcKalTrack *pimTrk = (*(evtRecTrkCol->begin()+ipim[0]))->mdcKalTrack();

		WTrackParameter wvpipTrk, wvpimTrk;
		wvpipTrk = WTrackParameter(mpi0, pipTrk->getZHelix(), pipTrk->getZError());
		wvpimTrk = WTrackParameter(mpi0, pimTrk->getZHelix(), pimTrk->getZError());


	// * Default is pion, for other particles: * //
		// wvppTrk = WTrackParameter(mp, pipTrk->getZHelixP(), pipTrk->getZErrorP()); // proton
		// wvmupTrk = WTrackParameter(mmu, pipTrk->getZHelixMu(), pipTrk->getZErrorMu()); // muon
		// wvepTrk = WTrackParameter(me, pipTrk->getZHelixE(), pipTrk->getZErrorE()); // electron
		// wvkpTrk = WTrackParameter(mk, pipTrk->getZHelixK(), pipTrk->getZErrorK()); // kaon


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
		vtxfit->AddTrack(1, wvpimTrk);
		vtxfit->AddVertex(0, vxpar,0, 1);
		if(!vtxfit->Fit(0)) return SUCCESS;
		vtxfit->Swim(0);

		WTrackParameter wpip = vtxfit->wtrk(0);
		WTrackParameter wpim = vtxfit->wtrk(1);

		// KinematicFit * kmfit = KinematicFit::instance();
		KalmanKinematicFit * kkmfit = KalmanKinematicFit::instance();


	// * Apply Kalman 4-constrain kinematic fit * //
		if(m_test4C) {
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
					kkmfit->AddTrack(0, wpip);       // 1c: pos. pion
					kkmfit->AddTrack(1, wpim);       // 2c: neg. pion
					kkmfit->AddTrack(2, 0.0, g1Trk); // 3c: first gamma track
					kkmfit->AddTrack(3, 0.0, g2Trk); // 4c: seconnd gamma track
					kkmfit->AddFourMomentum(0, ecms);
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


	// NOTE: Ncut4 -- fit4c passed and ChiSq less than m_maxChiSq
			if(chisq < m_maxChiSq) {
				RecEmcShower *g1Trk = (*(evtRecTrkCol->begin()+ig1))->emcShower();
				RecEmcShower *g2Trk = (*(evtRecTrkCol->begin()+ig2))->emcShower();
				kkmfit->init();
				kkmfit->AddTrack(0, wpip);      // 1c: pos. pion
				kkmfit->AddTrack(1, wpim);      // 2c: neg. pion
				kkmfit->AddTrack(2, 0., g1Trk); // 3c: first gamma track
				kkmfit->AddTrack(3, 0., g2Trk); // 4c: second gamma track
				kkmfit->AddFourMomentum(0, ecms);
				if(kkmfit->Fit()) {
					HepLorentzVector ppi0 = kkmfit->pfit(2) + kkmfit->pfit(3);

					// * WRITE pi^0 information from EMCal ("fit4c" branch) *
					m_mpi0 = ppi0.m();        // invariant pi0 mass according to Kalman kinematic fit
					m_chi1 = kkmfit->chisq(); // chi square of the Kalman kinematic fit
					m_tuple_fit4c->write(); // "fit4c" branch
					Ncut4++; // ChiSq has to be less than 200 and fit4c has to be passed
				}
			}
		}


	// * Apply Kalman kinematic fit * //
	// NOTE: Ncut5 -- Kalman kinematic fit 5c is successful
	// NOTE: Ncut6 -- J/psi -> rho0 pi0 (cut on invariant mass window)
		if(m_test5C) {
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
					kkmfit->AddTrack(0, wpip);            // 1c: pos. pion
					kkmfit->AddTrack(1, wpim);            // 2c: neg. pion
					kkmfit->AddTrack(2, 0., g1Trk);       // 3c: first gamma track
					kkmfit->AddTrack(3, 0., g2Trk);       // 4c: second gamma track
					kkmfit->AddResonance(0, 0.135, 2, 3); // 5c: pi0 resonance
					kkmfit->AddFourMomentum(1, ecms);
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

			log << MSG::INFO << " chisq = " << chisq << endreq;

			if(chisq < m_maxChiSq) {
				RecEmcShower* g1Trk = (*(evtRecTrkCol->begin()+ig1))->emcShower();
				RecEmcShower* g2Trk = (*(evtRecTrkCol->begin()+ig2))->emcShower();
				kkmfit->init();
				kkmfit->AddTrack(0, wpip);            // 1c: pi^+ track
				kkmfit->AddTrack(1, wpim);            // 2c: pi^- track
				kkmfit->AddTrack(2, 0., g1Trk);       // 3c: first gamma track
				kkmfit->AddTrack(3, 0., g2Trk);       // 4c: second gamma track
				kkmfit->AddResonance(0, 0.135, 2, 3); // 5c: pi0 resonance
				kkmfit->AddFourMomentum(1, ecms);

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
					m_chi2 = kkmfit->chisq(); // chi squared of the Kalman kinematic fit
					m_mrho0 = prho0.m();      // inv. mass pi^+ pi^- (rho^0)
					m_mrhop = prhop.m();      // inv. mass pi^0 pi^+ (rho^+)
					m_mrhom = prhom.m();      // inv. mass pi^0 pi^- (rho^-)
					m_tuple_fit5c->write(); // "fit5c" branch
					Ncut5++; // Kalman kinematic fit 5c is successful


					// * Measure the photon detection efficiences via J/psi -> rho0 pi0 * //
					if(fabs(prho0.m() - mrho0) < m_dmrho0) {
						if(fabs(fcos) < 0.99) {
							// * WRITE photon detection efficiency info ("geff" branch) *
							m_fcos = (eg1-eg2) / ppi0.rho();  // E/p ratio for pi^0 candidate
							m_elow = (eg1 < eg2) ? eg1 : eg2; // lowest energy of the two gammas
							m_tuple_photon->write(); // "geff" branch
							Ncut6++; // 0.620 < mass of rho0 < 0.920
						}
					} // rho0 cut
				}
			}
		}


	return StatusCode::SUCCESS;
}



// * ========================== * //
// * -------- FINALIZE -------- * //
// * ========================== * //
/**
 * @brief Inherited `finalize` method of `Algorithm`. This function is only called once, after running over all events.
 * @details Prints the flow chart to the terminal, so make sure you save this output!
 */
StatusCode DzeroOmega::finalize() {

	MsgStream log(msgSvc(), name());
	log << MSG::INFO << "in finalize()" << endmsg;

	// * Print flow chart * //
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