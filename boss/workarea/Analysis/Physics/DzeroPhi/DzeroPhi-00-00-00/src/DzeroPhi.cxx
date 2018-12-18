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
	#include <map>
	#include <string>
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
		const std::vector<const double> xmass {
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

	// * Book NTuple: Vertex position * //
		NTuplePtr nt = BookNTuple("vxyz");
		if(!nt) return StatusCode::FAILURE;
		nt->addItem("vx0",    fVx0);
		nt->addItem("vy0",    fVy0);
		nt->addItem("vz0",    fVz0);
		nt->addItem("vr0",    fVr0);
		nt->addItem("rvxy0",  fRvxy0);
		nt->addItem("rvz0",   fRvz0);
		nt->addItem("rvphi0", fRvphi0);

	// * Book NTuple: Neutral pion (pi0) fit branch * //
		nt = BookNTuple("fit4c");
		if(!nt) return StatusCode::FAILURE;
		nt->addItem("mD0",   fM_D0);
		nt->addItem("mphi",  fM_phi);
		nt->addItem("mJpsi", fM_Jpsi);
		nt->addItem("chi2",  fChi2sq);

	// * Book NTuple: Rho fit branch * //
		nt = BookNTuple("fit6c");
		if(!nt) return StatusCode::FAILURE;
		nt->addItem("mD0",   fM_D0);
		nt->addItem("mphi",  fM_phi);
		nt->addItem("mJpsi", fM_Jpsi);
		nt->addItem("chi2",  fChi2sq);

	// * Book NTuple: Import dE/dx PID branch * //
		nt = BookNTuple("dedx");
		if(!nt) return StatusCode::FAILURE;
		nt->addItem("ptrk",   fPtrack);
		nt->addItem("chie",   fChi2e);
		nt->addItem("chimu",  fChi2mu);
		nt->addItem("chipi",  fChi2pi);
		nt->addItem("chik",   fChi2k);
		nt->addItem("chip",   fChi2p);
		nt->addItem("probPH", fProbPH);
		nt->addItem("normPH", fNormPH);
		nt->addItem("ghit",   fGhit);
		nt->addItem("thit",   fThit);

	// * Book NTuple: ToF endcap branch * //
		nt = BookNTuple("tofe");
		if(!nt) return StatusCode::FAILURE;
		nt->addItem("ptrk", fPtotTof);
		nt->addItem("path", fPathTof);
		nt->addItem("tof",  fTof);
		nt->addItem("cntr", fCntrTof);
		nt->addItem("ph",   fPhTof);
		nt->addItem("rhit", fRhitTof);
		nt->addItem("qual", fQualTof);
		nt->addItem("te",   fElectronTof);
		nt->addItem("tmu",  fMuonTof);
		nt->addItem("tpi",  fProtoniTof);
		nt->addItem("tk",   fKaonTof);
		nt->addItem("tp",   fProtonTof);

	// * Book NTuple: TToF inner barrel branch * //
		nt = BookNTuple("tof1");
		if(!nt) return StatusCode::FAILURE;
		nt->addItem("ptrk", fPtotTof);
		nt->addItem("path", fPathTof);
		nt->addItem("tof",  fTof);
		nt->addItem("cntr", fCntrTof);
		nt->addItem("ph",   fPhTof);
		nt->addItem("zhit", fZhitTof);
		nt->addItem("qual", fQualTof);
		nt->addItem("te",   fElectronTof);
		nt->addItem("tmu",  fMuonTof);
		nt->addItem("tpi",  fProtoniTof);
		nt->addItem("tk",   fKaonTof);
		nt->addItem("tp",   fProtonTof);

	// * Book NTuple: check ToF outer barrel branch * //
		nt = BookNTuple("tof2");
		if(!nt) return StatusCode::FAILURE;
		nt->addItem("ptrk", fPtotTof);
		nt->addItem("path", fPathTof);
		nt->addItem("tof",  fTof);
		nt->addItem("cntr", fCntrTof);
		nt->addItem("ph",   fPhTof);
		nt->addItem("zhit", fZhitTof);
		nt->addItem("qual", fQualTof);
		nt->addItem("te",   fElectronTof);
		nt->addItem("tmu",  fMuonTof);
		nt->addItem("tpi",  fProtoniTof);
		nt->addItem("tk",   fKaonTof);
		nt->addItem("tp",   fProtonTof);

	// * Book NTuple: Track PID information * //
		nt = BookNTuple("pid");
		if(!nt) return StatusCode::FAILURE;
		nt->addItem("ptrk", fPtrackPID);
		nt->addItem("cost", fCostPID);
		nt->addItem("dedx", fDedxPID);
		nt->addItem("tof1", fTof1PID);
		nt->addItem("tof2", fTof2PID);
		nt->addItem("prob", fProbPID);

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
			fNTupleMap["vxyz"]->write();

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
			fPtrack   = mdcTrk->p();
			fChi2e   = dedxTrk->chiE();
			fChi2mu  = dedxTrk->chiMu();
			fChi2pi  = dedxTrk->chiPi();
			fChi2k   = dedxTrk->chiK();
			fChi2p   = dedxTrk->chiP();
			fProbPH = dedxTrk->probPH();
			fNormPH = dedxTrk->normPH();
			fGhit   = dedxTrk->numGoodHits();
			fThit   = dedxTrk->numTotalHits();
			fNTupleMap["dedx"]->write();
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

				// * Test hit status *
				TofHitStatus hitStatus;
				hitStatus.setStatus((*iter_tof)->status());
				if(!hitStatus.is_counter()) continue;

				// * If barrel ToF detector: *
				if(hitStatus.is_barrel()) {

					// * INNER barrel ToF detector
					if(hitStatus.layer() == 1) {
						// * Get ToF info
						fPtotTof = ptrk;
						fPathTof = (*iter_tof)->path();
						fTof     = (*iter_tof)->tof();
						fPhTof   = (*iter_tof)->ph();
						fZhitTof = (*iter_tof)->zrhit();
						fQualTof = 0.+(*iter_tof)->quality();
						fCntrTof = 0.+(*iter_tof)->tofID();
						// * Get ToF for each particle hypothesis
						std::vector<double> texp(xmass.size());
						for(auto j = 0; j < texp.size(); j++) {
							double gb = fPtotTof/xmass[j]; // v = p/m (non-relativistic velocity)
							double beta = gb/sqrt(1+gb*gb);
							texp[j] = 10 * fPathTof /beta/velc_mm; // hypothesis ToF
						}
						// * WRITE ToF inner barrel info ("tof1" branch)
						fElectronTof = tof - texp[0];
						fMuonTof     = tof - texp[1];
						fProtoniTof  = tof - texp[2];
						fKaonTof     = tof - texp[3];
						fProtonTof   = tof - texp[4];
						fNTupleMap["tof1"]->write();
					}

					// * Outer barrel ToF detector
					if(hitStatus.layer() == 2) {
						// * Get ToF info
						fPtotTof = ptrk;
						fPathTof = (*iter_tof)->path();
						fTof     = (*iter_tof)->tof();
						fPhTof   = (*iter_tof)->ph();
						fZhitTof = (*iter_tof)->zrhit();
						fQualTof = 0.+(*iter_tof)->quality();
						fCntrTof = 0.+(*iter_tof)->tofID();
						// * Get ToF for each particle hypothesis
						std::vector<double> texp(xmass.size());
						for(auto j = 0; j < texp.size(); j++) {
							double gb = fPtotTof/xmass[j]; // v = p/m (non-relativistic velocity)
							double beta = gb/sqrt(1+gb*gb);
							texp[j] = 10 * fPathTof /beta/velc_mm; // hypothesis ToF
						}
						// * WRITE ToF outer barrel info ("tof2" branch)
						fElectronTof = tof - texp[0];
						fMuonTof     = tof - texp[1];
						fProtoniTof  = tof - texp[2];
						fKaonTof     = tof - texp[3];
						fProtonTof   = tof - texp[4];
						fNTupleMap["tof2"]->write();
					}

				}

				// * If end cap ToF detector: *
				else if(hitStatus.layer() == 0) {
					// * Get ToF info
					fPtotTof = ptrk;
					fPathTof = (*iter_tof)->path();
					fTof     = (*iter_tof)->tof();
					fPhTof   = (*iter_tof)->ph();
					fZhitTof = (*iter_tof)->zrhit();
					fQualTof = 0.+(*iter_tof)->quality();
					fCntrTof = 0.+(*iter_tof)->tofID();
					// * Get ToF for each particle hypothesis
					std::vector<double> texp(xmass.size());
					for(auto j = 0; j < texp.size(); j++) {
						double gb = fPtotTof/xmass[j]; // v = p/m (non-relativistic velocity)
						double beta = gb/sqrt(1+gb*gb);
						texp[j] = 10 * fPathTof /beta/velc_mm; // hypothesis ToF
					}
					// * WRITE ToF end cap info ("tofe" branch)
					fElectronTof = tof - texp[0];
					fMuonTof     = tof - texp[1];
					fProtoniTof  = tof - texp[2];
					fKaonTof     = tof - texp[3];
					fProtonTof   = tof - texp[4];
					fNTupleMap["tofe"]->write();
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
			fNTupleMap["pid"]->write();

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
			fNTupleMap["fit4c"]->write();
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
			fNTupleMap["fit6c"]->write();
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



// * ================================= * //
// * -------- PRIVATE METHODS -------- * //
// * ================================= * //
/**
 * @brief   Helper function that allows you to create pair of a string with a `NTuplePtr`.
 * @details This function first attempts to see if there is already an `NTuple` in the output file. If not, it will book an `NTuple` of 
 *
 * @param tupleName This will not only be the name of your `NTuple`, but also the name of the `TTree` in the output ROOT file when you use `NTuple::write()`. The name used here is also used as identifier in `NTuplePtr` map `fNTupleMap`. In other words, you can get any of the `NTuplePtr`s in this map by using `fNTupleMap[<tuple name>]`. If there is no `NTuple` of this name, it will return a `nullptr`.
 */
NTuplePtr NTupleMap::BookNTuple(const char* tupleName) {
	const char* bookName = Form("FILE1/%s", tupleName);
	NTuplePtr nt(ntupleSvc(), bookName); // attempt to get from file
	if(!nt) { // if not available in file, book a new one
		nt = ntupleSvc()->book(bookName, CLID_ColumnWiseTuple, "ks N-Tuple example");
		// if(!nt) log << MSG::ERROR << "    Cannot book N-tuple:" << long(fNTupleMap[tupleName]) << endmsg;
	}
	fNTupleMap[tupleName] = nt;
	return nt;
}