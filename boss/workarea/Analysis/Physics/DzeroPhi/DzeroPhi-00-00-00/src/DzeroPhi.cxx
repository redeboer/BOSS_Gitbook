// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //
	#include "DstEvent/TofHitStatus.h"
	#include "EventModel/Event.h"
	#include "EventModel/EventHeader.h"
	#include "EventModel/EventModel.h"
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
		const double xmass[] {
			0.000511, // electron
			0.105658, // muon
			0.139570, // charged pion
			0.493677, // charged kaon
			0.938272  // proton
		};
		const int nmass = sizeof(xmass)/sizeof(*xmass);
		const double velc_cm = 29.9792458; // tof_path unit in cm.
		const double velc_mm = 299.792458; // tof path unit in mm
		const double Ecms = 3.097;// center-of-mass energy


	// * Typedefs * //
		typedef std::vector<EvtRecTrackIterator>::iterator Vit;
		typedef std::vector<HepLorentzVector> Vp4;
		typedef std::vector<int> Vint;



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

	// * Whether or not to fill a tree/NTuple *
	declareProperty("doVxyz",  fDoVxyz);
	declareProperty("doFit4c", fDoFit4c);
	declareProperty("doFit6c", fDoFit6c);
	declareProperty("doDedx",  fDoDedx);
	declareProperty("doTofEC", fDoTofEC);
	declareProperty("doTofIB", fDoTofIB);
	declareProperty("doTofOB", fDoTofOB);
	declareProperty("doPID",   fDoPID);

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
	declareProperty("dM_rho0", fDeltaMrho0);

	// * Whether to test the success of the 4- and 5-constraint fits *
	declareProperty("MaxChiSq", fMaxChiSq);

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
	NTuplePtr nt(ntupleSvc(), ""); // temporary NTuplePtr

	// * Book NTuple: Multiplicities * //
		if(fDoVxyz) {
			NTuplePtr nt = BookNTuple("mult");
			if(!nt) return StatusCode::FAILURE;
			nt->addItem("Ntotal",   fEvent_Ntotal);
			nt->addItem("Ncharge",  fEvent_Ncharge);
			nt->addItem("Nneutral", fEvent_Nneutral);
			nt->addItem("vx0",      fEvent_Vx0);
			nt->addItem("vy0",      fEvent_Vy0);
			nt->addItem("vz0",      fEvent_Vz0);
			nt->addItem("Ngood",    fEvent_Ngood);
			nt->addItem("Nmdc",     fEvent_Nmdc); // @todo Check if this makes sense at all
			nt->addItem("fEvent_NKaonPos", fEvent_NKaonPos);
			nt->addItem("fEvent_NKaonNeg", fEvent_NKaonNeg);
			nt->addItem("fEvent_NPionPos", fEvent_NPionPos);
		}

	// * Book NTuple: Vertex position * //
		if(fDoVxyz) {
			nt = BookNTuple("vxyz");
			if(!nt) return StatusCode::FAILURE;
			nt->addItem("vx",    fTrack_Vx);
			nt->addItem("vy",    fTrack_Vy);
			nt->addItem("vz",    fTrack_Vz);
			nt->addItem("vr",    fTrack_Vr);
			nt->addItem("rvxy",  fTrack_Rvxy);
			nt->addItem("rvz",   fTrack_Rvz);
			nt->addItem("rvphi", fTrack_Rvphi);
			nt->addItem("phi",   fTrack_phi);
			nt->addItem("p",     fTrack_p);
		}

	// * Book NTuple: 4-contraints for Kalman kinematic fit * //
		if(fDoFit4c) {
			nt = BookNTuple("fit4c");
			if(!nt) return StatusCode::FAILURE;
			nt->addItem("mD0",   fM_D0);
			nt->addItem("mphi",  fM_phi);
			nt->addItem("mJpsi", fM_Jpsi);
			nt->addItem("chi2",  fChi2sq);
		}

	// * Book NTuple: 6-contraints for Kalman kinematic fit * //
		if(fDoFit6c) {
			nt = BookNTuple("fit6c");
			if(!nt) return StatusCode::FAILURE;
			nt->addItem("mD0",   fM_D0);
			nt->addItem("mphi",  fM_phi);
			nt->addItem("mJpsi", fM_Jpsi);
			nt->addItem("chi2",  fChi2sq);
		}

	// * Book NTuple: dE/dx PID branch * //
		if(fDoDedx) {
			nt = BookNTuple("dedx");
			if(!nt) return StatusCode::FAILURE;
			nt->addItem("ptrk",   fTrack_p);
			nt->addItem("chie",   fChi2e);
			nt->addItem("chimu",  fChi2mu);
			nt->addItem("chipi",  fChi2pi);
			nt->addItem("chik",   fChi2k);
			nt->addItem("chip",   fChi2p);
			nt->addItem("probPH", fProbPH);
			nt->addItem("normPH", fNormPH);
			nt->addItem("ghit",   fGhit);
			nt->addItem("thit",   fThit);
		}

	// * Book NTuple: ToF endcap branch * //
		if(fDoTofEC) {
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
		}

	// * Book NTuple: ToF inner barrel branch * //
		if(fDoTofIB) {
			nt = BookNTuple("tof1");
			if(!nt) return StatusCode::FAILURE;
			nt->addItem("ptrk", fPtotTof);
			nt->addItem("path", fPathTof);
			nt->addItem("tof",  fTof);
			nt->addItem("cntr", fCntrTof);
			nt->addItem("ph",   fPhTof);
			nt->addItem("zhit", fRhitTof);
			nt->addItem("qual", fQualTof);
			nt->addItem("te",   fElectronTof);
			nt->addItem("tmu",  fMuonTof);
			nt->addItem("tpi",  fProtoniTof);
			nt->addItem("tk",   fKaonTof);
			nt->addItem("tp",   fProtonTof);
		}

	// * Book NTuple: ToF outer barrel branch * //
		if(fDoTofOB) {
			nt = BookNTuple("tof2");
			if(!nt) return StatusCode::FAILURE;
			nt->addItem("ptrk", fPtotTof);
			nt->addItem("path", fPathTof);
			nt->addItem("tof",  fTof);
			nt->addItem("cntr", fCntrTof);
			nt->addItem("ph",   fPhTof);
			nt->addItem("zhit", fRhitTof);
			nt->addItem("qual", fQualTof);
			nt->addItem("te",   fElectronTof);
			nt->addItem("tmu",  fMuonTof);
			nt->addItem("tpi",  fProtoniTof);
			nt->addItem("tk",   fKaonTof);
			nt->addItem("tp",   fProtonTof);
		}

	// * Book NTuple: Track PID information * //
		if(fDoPID) {
			nt = BookNTuple("pid");
			if(!nt) return StatusCode::FAILURE;
			nt->addItem("ptrk", fTrack_p);
			nt->addItem("cost", fCostPID);
			nt->addItem("dedx", fDedxPID);
			nt->addItem("tof1", fTof1PID);
			nt->addItem("tof2", fTof2PID);
			nt->addItem("prob", fProbPID);
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

		// * Load event info *
			/*
			http://bes3.to.infn.it/Boss/7.0.2/html/namespaceEventModel_1_1EvtRec.html (namespace)
			http://bes3.to.infn.it/Boss/7.0.2/html/classEvtRecEvent.html (class)
			http://bes3.to.infn.it/Boss/7.0.2/html/EvtRecTrack_8h.html (typedef EvtRecTrackCol)
			*/
		SmartDataPtr<Event::EventHeader> eventHeader (eventSvc(), "/Event/EventHeader");
		SmartDataPtr<EvtRecEvent>        evtRecEvent (eventSvc(), EventModel::EvtRec::EvtRecEvent);
		SmartDataPtr<EvtRecTrackCol>     evtRecTrkCol(eventSvc(), EventModel::EvtRec::EvtRecTrackCol);

		// * Log run number, event number, and number of events *
		fEvent_Ntotal   = evtRecEvent->totalTracks();
		fEvent_Ncharge  = evtRecEvent->totalCharged();
		fEvent_Nneutral = evtRecEvent->totalNeutral();
		log << MSG::DEBUG
			<< "run "           << eventHeader->runNumber()   << ", "
			<< "evtent number " << eventHeader->eventNumber() << endreq;
		log << MSG::DEBUG
			<< "Ncharged = " << fEvent_Ncharge  << ", "
			<< "Nneutral = " << fEvent_Nneutral << ", "
			<< "Ntotal = "   << fEvent_Ntotal   << endreq;


	// * Set vertex origin * //
		IVertexDbSvc* vtxsvc;
		Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);
		if(vtxsvc->isVertexValid()){
			double* dbv = vtxsvc->PrimaryVertex();
			double* vv = vtxsvc->SigmaPrimaryVertex();
			fEvent_Vx0 = dbv[0];
			fEvent_Vy0 = dbv[1];
			fEvent_Vz0 = dbv[2];
		}


	// * LOOP OVER CHARGED TRACKS: select 'good' charged tracks * //
		fEvent_Nmdc = 0; // @todo Check if this makes sense at all
		fGoodChargedTracks.clear();
		for(int i = 0; i < evtRecEvent->totalCharged(); ++i) {
		// Note: the first part of the set of reconstructed tracks are the charged tracks

			// * Check if valid according to MDC
			fTrackIterator = evtRecTrkCol->begin() + i; 
			if(!(*fTrackIterator)->isMdcTrackValid()) continue;

			// * Get track info from Main Drift Chamber
			RecMdcTrack *mdcTrk = (*fTrackIterator)->mdcTrack();
			fTrack_p  = mdcTrk->p();
			fTrack_Vx = mdcTrk->x();
			fTrack_Vy = mdcTrk->y();
			fTrack_Vz = mdcTrk->z();
			fTrack_phi = mdcTrk->helix(1);
			fTrack_Vr =
				(fTrack_Vx - fEvent_Vx0) * cos(fTrack_phi) +
				(fTrack_Vy - fEvent_Vy0) * sin(fTrack_phi);

			// * Get radii of vertex
			HepVector a = mdcTrk->helix();
			HepSymMatrix Ea = mdcTrk->err();
			HepPoint3D point0(0., 0., 0.); // initial point for MDC reconstruction
			HepPoint3D IP(fEvent_Vx0, fEvent_Vy0, fEvent_Vz0);
			VFHelix helixip(point0, a, Ea);
			helixip.pivot(IP);
			HepVector vecipa = helixip.a();
			fTrack_Rvxy  = fabs(vecipa[0]); // nearest distance to IP in xy plane
			fTrack_Rvz   = vecipa[3];       // nearest distance to IP in z direction
			fTrack_Rvphi = vecipa[1]; // angle in the xy-plane (?)

			// * Apply vertex cuts
			if(fabs(fTrack_Vz)   >= fVz0cut)   continue;
			if(fabs(fTrack_Vr)   >= fVr0cut)   continue;
			if(fabs(fTrack_Rvz)  >= fRvz0cut)  continue;
			if(fabs(fTrack_Rvxy) >= fRvxy0cut) continue;

			// * Add charged track to vector
			fGoodChargedTracks.push_back(*fTrackIterator);
			fEvent_Nmdc += mdcTrk->charge(); // @todo Check if this makes sense at all

			// * WRITE primary vertex position info ("vxyz" branch)
			if(fDoVxyz) fNTupleMap["vxyz"]->write();

		}


	// * Store dE/dx PID information * //
		if(fDoDedx) {
			fTrackIterator = fGoodChargedTracks.begin();
			for(; fTrackIterator != fGoodChargedTracks.end(); ++fTrackIterator) {

				// * Check if dE/dx and MDC info exists *
				if(!(*fTrackIterator)->isMdcTrackValid()) continue;
				if(!(*fTrackIterator)->isMdcDedxValid())  continue;
				RecMdcTrack* mdcTrk = (*fTrackIterator)->mdcTrack();
				RecMdcDedx* dedxTrk = (*fTrackIterator)->mdcDedx();

				// * WRITE energy loss PID info ("dedx" branch) *
				fTrack_p = mdcTrk->p();
				fChi2e   = dedxTrk->chiE();
				fChi2mu  = dedxTrk->chiMu();
				fChi2pi  = dedxTrk->chiPi();
				fChi2k   = dedxTrk->chiK();
				fChi2p   = dedxTrk->chiP();
				fProbPH  = dedxTrk->probPH();
				fNormPH  = dedxTrk->normPH();
				fGhit    = dedxTrk->numGoodHits();
				fThit    = dedxTrk->numTotalHits();
				fNTupleMap["dedx"]->write();

			} // end of loop over all charged track
		} // end of fDoDedx


	// * Store Time-of-Flight information * //
		if(fDoTofEC || fDoTofIB || fDoTofOB) {
			fTrackIterator = fGoodChargedTracks.begin();
			for(; fTrackIterator != fGoodChargedTracks.end(); ++fTrackIterator) {

				// * Check if MDC and TOF info for track are valid *
				if(!(*fTrackIterator)->isMdcTrackValid()) continue;
				if(!(*fTrackIterator)->isTofTrackValid()) continue;

				// * Get momentum as determined by MDC *
				RecMdcTrack * mdcTrk = (*fTrackIterator)->mdcTrack();
				double ptrk = mdcTrk->p();
				SmartRefVector<RecTofTrack> tofTrkCol = (*fTrackIterator)->tofTrack();
				SmartRefVector<RecTofTrack>::iterator iter_tof = tofTrkCol.begin();
				for(; iter_tof != tofTrkCol.end(); ++iter_tof) {
					TofHitStatus hitStatus;
					hitStatus.setStatus((*iter_tof)->status());
					if(!hitStatus.is_counter()) continue;
					if(hitStatus.is_barrel()) {
						if(hitStatus.layer() == 1) { // inner barrel
							if(fDoTofIB) GetTofInformation(iter_tof, ptrk, "tof1");
						} else if(hitStatus.layer() == 2) { // outer barrel
							if(fDoTofOB) GetTofInformation(iter_tof, ptrk, "tof2");
						}
					}
					else if(fDoTofEC && hitStatus.layer() == 0) // end cap
						GetTofInformation(iter_tof, ptrk, "tofe");
				}

			} // loop all charged track
		} // end of fDoTofEC, fDoTofIB, fDoTofOB


	// * Create a selection of charged decay products * //

		// * Clear lists of decay products and initialise PID instance *
		fKaonNeg.clear();
		fKaonPos.clear();
		fPionPos.clear();
		ParticleID *pid = ParticleID::instance();

		// * Loop over collection of 'good' charged tracks *
		fTrackIterator = fGoodChargedTracks.begin();
		for(; fTrackIterator != fGoodChargedTracks.end(); ++fTrackIterator) {

			// * Initialise PID sub-system
			// if(pid) delete pid;
			pid->init();
			pid->setMethod(pid->methodProbability());
			// pid->setMethod(pid->methodLikelihood()); // for likelihood Method
			pid->setChiMinCut(4);
			pid->setRecTrack(*fTrackIterator);
			pid->usePidSys(pid->useDedx() | pid->useTof1() | pid->useTof2() | pid->useTofE()); // use PID sub-system
			pid->identify(pid->onlyPion() | pid->onlyKaon()); // seperater Pion/Kaon
			pid->calculate();
			if(!(pid->IsPidInfoValid())) continue;

			// * Get MDC track info *
			RecMdcTrack* mdcTrk = (*fTrackIterator)->mdcTrack();

			// * WRITE particle identification info ("pid" branch) *
			if(fDoPID) {
				fTrack_p = mdcTrk->p();
				fCostPID = cos(mdcTrk->theta());
				fDedxPID = pid->chiDedx(2);
				fTof1PID = pid->chiTof1(2);
				fTof2PID = pid->chiTof2(2);
				fProbPID = pid->probPion();
				fNTupleMap["pid"]->write();
			}

			// * If more likely to be pion (pi+-) *
			if(pid->probPion() > pid->probKaon()) {

				// * Check if indeed a pion
				if(pid->probPion() < 0.001) continue;
				if(pid->pdf(2) < pid->pdf(3)) continue;
					// for likelihood method (0=electron 1=muon 2=pion 3=kaon 4=proton)

				// * Get MDC Kalman track
				RecMdcKalTrack* mdcKalTrk = (*fTrackIterator)->mdcKalTrack();
					// after ParticleID, use RecMdcKalTrack substitute RecMdcTrack
				RecMdcKalTrack::setPidType(RecMdcKalTrack::pion);
					// PID can set to electron, muon, pion (default), kaon and proton

				// * If positive pion
				if(mdcKalTrk->charge() > 0)
					fPionPos.push_back(*fTrackIterator);

			}

			// * If more likely to be kaon (K+-) *
			else {

				// * Check if indeed a kaon
				if(pid->probKaon() < 0.001) continue;
				if(pid->pdf(3) < pid->pdf(2)) continue;
					// for likelihood method (0=electron 1=muon 2=pion 3=kaon 4=proton)

				// * Get MDC Kalman track
				RecMdcKalTrack* mdcKalTrk = (*fTrackIterator)->mdcKalTrack();
					// after ParticleID, use RecMdcKalTrack substitute RecMdcTrack
				RecMdcKalTrack::setPidType(RecMdcKalTrack::kaon);
					// PID can set to electron, muon, pion (default), kaon and proton

				// * If negative kaon
				if(mdcKalTrk->charge() < 0)
					fKaonNeg.push_back(*fTrackIterator);

				// * If positive kaon
				else
					fKaonPos.push_back(*fTrackIterator);

			}
		}

	// * WRITE event info ("mult" branch) * //
		log << MSG::DEBUG << "ngood, totcharge = " << fGoodChargedTracks.size() << " , " << fEvent_Nmdc << endreq;
		if(fDoVxyz) {
			fEvent_Ngood = fGoodChargedTracks.size();
			fEvent_NKaonNeg = fKaonNeg.size();
			fEvent_NKaonPos = fKaonPos.size();
			fEvent_NPionPos = fPionPos.size();
			fNTupleMap["mult"]->write();
		} // end of fDoVxyz


	// ! Cut on number of particles ! //
	// @todo Test if procedure can be made to work for arbitrary number of kaons and pions
		if(fKaonNeg.size() != 2) return SUCCESS; //!< Only select events that contains two \f$K^-\f$'s.
		if(fKaonPos.size() != 1) return SUCCESS; //!< Only select events that contains one \f$\pi^+\f$.
		if(fPionPos.size() != 1) return SUCCESS; //!< Only select events that contains one \f$K^+\f$.


	// * Test all combinations with K- and store the ones closes to D0 and to phi * //

		// * Get Kalman tracks
		RecMdcKalTrack *pipTrk = (*(fPionPos.begin()))  ->mdcKalTrack();
		RecMdcKalTrack *KpTrk  = (*(fKaonPos.begin()))  ->mdcKalTrack();
		RecMdcKalTrack *KmTrk1 = (*(fKaonNeg.begin()))  ->mdcKalTrack();
		RecMdcKalTrack *KmTrk2 = (*(fKaonNeg.begin()+1))->mdcKalTrack();

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

		// * Some local declarations
		HepLorentzVector ecms(0.034, 0, 0, Ecms);
		KalmanKinematicFit *kkmfit1 = KalmanKinematicFit::instance();
		KalmanKinematicFit *kkmfit2 = KalmanKinematicFit::instance();
		double chisq1 = 999999.;
		double chisq2 = 999999.;

		// * Get Kalman fit for first combination with K-
		kkmfit1->init();
		kkmfit1->AddTrack(0, wKm1);  // K- (1st occurrence)
		kkmfit1->AddTrack(1, wpip);  // pi+
		kkmfit1->AddTrack(2, wKm2);  // K- (2nd occurrence)
		kkmfit1->AddTrack(3, wKp);   // K+
		kkmfit1->AddFourMomentum(0, ecms);
		if(kkmfit1->Fit()) chisq1 = kkmfit1->chisq();

		// * Get Kalman fit for second combination with K-
		kkmfit2->init();
		kkmfit2->AddTrack(0, wKm2);  // K- (2nd occurrence)
		kkmfit2->AddTrack(1, wpip);  // pi+
		kkmfit2->AddTrack(2, wKm1);  // K- (1st occurrence)
		kkmfit2->AddTrack(3, wKp);   // K+
		kkmfit2->AddFourMomentum(0, ecms); // 4 constraints: CMS energy and momentum
		if(kkmfit2->Fit()) chisq2 = kkmfit2->chisq();

		// * Get best Kalman fit
		KalmanKinematicFit *kkmfit = nullptr;
		double chisq = 999999.;
		if(chisq1 < chisq2) { kkmfit = kkmfit1; chisq = chisq1; }
		else                { kkmfit = kkmfit2; chisq = chisq2; }

		// * WRITE D0 and phi information from 4-constraint fit ("fit4c" branch)
		if(fDoFit4c && kkmfit && chisq < fMaxChiSq) {
			HepLorentzVector pD0  = kkmfit->pfit(0) + kkmfit->pfit(1);
			HepLorentzVector pphi = kkmfit->pfit(2) + kkmfit->pfit(3);
			HepLorentzVector pJpsi = pD0 + pphi;
			fM_D0   = pD0.m();   // invariant D0 mass according to Kalman kinematic fit
			fM_phi  = pphi.m();  // invariant phi mass according to Kalman kinematic fit
			fM_Jpsi = pJpsi.m(); // invariant Jpsi mass according to Kalman kinematic fit
			fChi2sq = chisq;    // chi square of the Kalman kinematic fit
			fNTupleMap["fit4c"]->write();
		}


	// * Find Kalman 5-constraint kinematic fit with smallest chi squared * //

		// * Some local declarations
		chisq1 = 999999.;
		chisq2 = 999999.;

		// * Get Kalman fit for first combination with K-
		kkmfit1->init();
		kkmfit1->AddTrack(0, wKm1);           // K- (1st occurrence)
		kkmfit1->AddTrack(1, wpip);           // pi+
		kkmfit1->AddTrack(2, wKm2);           // K- (2nd occurrence)
		kkmfit1->AddTrack(3, wKp);            // K+
		kkmfit1->AddResonance(0, mD0,  0, 1); // 5th constraint: D0 resonance
		kkmfit1->AddResonance(1, mphi, 2, 3); // 6th constraint: phi resonance
		kkmfit1->AddFourMomentum(2, ecms); // 4 constraints: CMS energy and momentum
		if(kkmfit1->Fit(0) && kkmfit1->Fit(1) && kkmfit1->Fit(2) && kkmfit1->Fit())
			chisq1 = kkmfit1->chisq();

		// * Get Kalman fit for second combination with K-
		kkmfit2->init();
		kkmfit2->AddTrack(0, wKm2);           // K- (2nd occurrence)
		kkmfit2->AddTrack(1, wpip);           // pi+
		kkmfit2->AddTrack(2, wKm1);           // K- (1st occurrence)
		kkmfit2->AddTrack(3, wKp);            // K+
		kkmfit2->AddResonance(0, mD0,  0, 1); // 5th constraint: D0 resonance
		kkmfit2->AddResonance(1, mphi, 2, 3); // 6th constraint: phi resonance
		kkmfit2->AddFourMomentum(2, ecms); // 4 constraints: CMS energy and momentum
		if(kkmfit2->Fit(0) && kkmfit2->Fit(1) && kkmfit2->Fit(2) && kkmfit2->Fit())
			chisq2 = kkmfit2->chisq();

		// * Get best Kalman fit
		kkmfit = nullptr;
		chisq = 999999.;
		if(chisq1 < chisq2) { kkmfit = kkmfit1; chisq = chisq1; }
		else                { kkmfit = kkmfit2; chisq = chisq2; }

		// * WRITE D0 and phi information from 5-constraint fit ("fit6c" branch)
		if(fDoFit6c && kkmfit && chisq < fMaxChiSq) {
			HepLorentzVector pD0  = kkmfit->pfit(0) + kkmfit->pfit(1);
			HepLorentzVector pphi = kkmfit->pfit(2) + kkmfit->pfit(3);
			HepLorentzVector pJpsi = pD0 + pphi;
			fM_D0   = pD0.m();
			fM_phi  = pphi.m();
			fM_Jpsi = pJpsi.m();
			fChi2sq = chisq;
			fNTupleMap["fit6c"]->write();
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
 * @brief Currently does nothing. Cut flow could be printed in this step.
 * @todo Add log output to `finalize` step.
 */
StatusCode DzeroPhi::finalize() {
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
NTuplePtr DzeroPhi::BookNTuple(const char* tupleName)
{
	const char* bookName = Form("FILE1/%s", tupleName);
	NTuplePtr nt(ntupleSvc(), bookName); // attempt to get from file
	if(!nt) { // if not available in file, book a new one
		nt = ntupleSvc()->book(bookName, CLID_ColumnWiseTuple, "ks N-Tuple example");
		// if(!nt) log << MSG::ERROR << "    Cannot book N-tuple:" << long(nt) << endmsg;
	}
	fNTupleMap[tupleName] = nt;
	return nt;
}

/**
 * @brief
 */
void DzeroPhi::GetTofInformation(SmartRefVector<RecTofTrack>::iterator iter_tof, double ptrk, const char* tupleName)
{
	// * Get ToF info
	fPtotTof = ptrk;
	fPathTof = (*iter_tof)->path();
	fTof     = (*iter_tof)->tof();
	fPhTof   = (*iter_tof)->ph();
	fRhitTof = (*iter_tof)->zrhit();
	fQualTof = 0.+(*iter_tof)->quality();
	fCntrTof = 0.+(*iter_tof)->tofID();
	// * Get ToF for each particle hypothesis
	std::vector<double> texp(nmass);
	for(size_t j = 0; j < texp.size(); ++j) {
		double gb = fPtotTof/xmass[j]; // v = p/m (non-relativistic velocity)
		double beta = gb/sqrt(1+gb*gb);
		texp[j] = 10 * fPathTof /beta/velc_mm; // hypothesis ToF
	}
	// * WRITE ToF info
	fElectronTof = fPathTof - texp[0];
	fMuonTof     = fPathTof - texp[1];
	fProtoniTof  = fPathTof - texp[2];
	fKaonTof     = fPathTof - texp[3];
	fProtonTof   = fPathTof - texp[4];
	fNTupleMap[tupleName]->write();
}