// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "DstEvent/TofHitStatus.h"
	#include "EventModel/Event.h"
	#include "EventModel/EventModel.h"
	#include "GaudiKernel/AlgFactory.h"
	#include "GaudiKernel/Bootstrap.h"
	#include "GaudiKernel/IDataProviderSvc.h"
	#include "GaudiKernel/IHistogramSvc.h"
	#include "GaudiKernel/INTupleSvc.h"
	#include "GaudiKernel/ISvcLocator.h"
	#include "GaudiKernel/NTuple.h"
	#include "GaudiKernel/PropertyMgr.h"
	#include "ParticleID/ParticleID.h"
	#include "DzeroPhi/DzeroPhi.h"
	#include "TMath.h"
	#include "VertexFit/Helix.h"
	#include "VertexFit/IVertexDbSvc.h"
	#include "VertexFit/KalmanKinematicFit.h"
	#include "VertexFit/VertexFit.h"
	#include <string>
	#include <utility>

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
		const HepLorentzVector ecms(0.034, 0, 0, Ecms);


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
		Algorithm(name, pSvcLocator),
		fLog(msgSvc(), name),
		fEventHeader (eventSvc(), "dummy"),
		fEvtRecEvent (eventSvc(), EventModel::EvtRec::EvtRecEvent),
		fEvtRecTrkCol(eventSvc(), EventModel::EvtRec::EvtRecTrackCol)
	{

		// * Whether or not to fill a tree/NTuple *
		declareProperty("doMult",    fDoMult);
		declareProperty("doVxyz",    fDoVxyz);
		declareProperty("doFit4c",   fDoFit4c);
		declareProperty("doFit6c",   fDoFit6c);
		declareProperty("doDedx",    fDoDedx);
		declareProperty("doDedx_K",  fDoDedx_Kaon);
		declareProperty("doDedx_pi", fDoDedx_Pion);
		declareProperty("doTofEC",   fDoTofEC);
		declareProperty("doTofIB",   fDoTofIB);
		declareProperty("doTofOB",   fDoTofOB);
		declareProperty("doPID",     fDoPID);

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



// * =============================== * //
// * ------- ALGORITHM STEPS ------- * //
// * =============================== * //

	/**
	 * @brief   (Inherited) `initialize` step of `Algorithm`. This function is called only once in the beginning.
	 * @details Define and load NTuples here.
	 */
	StatusCode DzeroPhi::initialize()
	{
		NTuplePtr nt(ntupleSvc(), ""); // temporary NTuplePtr
		// * Book NTuple: Multiplicities * //
			nt = BookNTuple("mult");
			if(!nt) return StatusCode::FAILURE;
			nt->addItem("Ntotal",   fEvent_Ntotal);
			nt->addItem("Ncharge",  fEvent_Ncharge);
			nt->addItem("Nneutral", fEvent_Nneutral);
			nt->addItem("vx0",      fEvent_Vx0);
			nt->addItem("vy0",      fEvent_Vy0);
			nt->addItem("vz0",      fEvent_Vz0);
			nt->addItem("Ngood",    fEvent_Ngood);
			nt->addItem("Nmdc",     fEvent_Nmdc); // @todo Check if this makes sense at all
			nt->addItem("NKaonPos", fEvent_NKaonPos);
			nt->addItem("NKaonNeg", fEvent_NKaonNeg);
			nt->addItem("NPionPos", fEvent_NPionPos);

		// * Book NTuple: Vertex position * //
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

		// * Book NTuple: 4-contraints for Kalman kinematic fit * //
			nt = BookNTuple("fit4c");
			if(!nt) return StatusCode::FAILURE;
			nt->addItem("mD0",   fInvMass_D0);
			nt->addItem("mphi",  fInvMass_phi);
			nt->addItem("mJpsi", fInvMass_Jpsi);
			nt->addItem("chi2",  fInvMass_ChiSq);

		// * Book NTuple: 6-contraints for Kalman kinematic fit * //
			nt = BookNTuple("fit6c");
			if(!nt) return StatusCode::FAILURE;
			nt->addItem("mD0",   fInvMass_D0);
			nt->addItem("mphi",  fInvMass_phi);
			nt->addItem("mJpsi", fInvMass_Jpsi);
			nt->addItem("chi2",  fInvMass_ChiSq);

		// * Book NTuple: dE/dx PID branch * //
			nt = BookNTuple("dedx");
			if(!nt) return StatusCode::FAILURE;
			BookNtupleDedxItems(nt);

		// * Book NTuple: dE/dx PID branch for kaons * //
			nt = BookNTuple("dedx_K");
			if(!nt) return StatusCode::FAILURE;
			BookNtupleDedxItems(nt);

		// * Book NTuple: dE/dx PID branch for pions * //
			nt = BookNTuple("dedx_pi");
			if(!nt) return StatusCode::FAILURE;
			BookNtupleDedxItems(nt);

		// * Book NTuple: ToF endcap branch * //
			nt = BookNTuple("tofe");
			if(!nt) return StatusCode::FAILURE;
			BookNtupleTofItems(nt);

		// * Book NTuple: ToF inner barrel branch * //
			nt = BookNTuple("tof1");
			if(!nt) return StatusCode::FAILURE;
			BookNtupleTofItems(nt);

		// * Book NTuple: ToF outer barrel branch * //
			nt = BookNTuple("tof2");
			if(!nt) return StatusCode::FAILURE;
			BookNtupleTofItems(nt);

		// * Book NTuple: Track PID information * //
			nt = BookNTuple("pid");
			if(!nt) return StatusCode::FAILURE;
			nt->addItem("ptrk", fTrack_p);
			nt->addItem("cost", fPID_Cost);
			nt->addItem("dedx", fPID_Dedx);
			nt->addItem("tof1", fPID_Tof1);
			nt->addItem("tof2", fPID_Tof2);
			nt->addItem("prob", fPID_Prob);

		fLog << MSG::INFO << "Successfully returned from initialize()" << endmsg;
		return StatusCode::SUCCESS;
	}


	/**
	 * @brief Inherited `execute` method of the `Algorithm`. This function is called *for each event*.
	 */
	StatusCode DzeroPhi::execute()
	{
		// * STEP (A): Load next event from DST file * //

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
			fEvent_Ntotal   = fEvtRecEvent->totalTracks();
			fEvent_Ncharge  = fEvtRecEvent->totalCharged();
			fEvent_Nneutral = fEvtRecEvent->totalNeutral();
			fLog << MSG::DEBUG
				<< "RUN "          << fEventHeader->runNumber()   << ", "
				<< "event number " << fEventHeader->eventNumber() << endreq;
			fLog << MSG::DEBUG
				<< "Ncharged = " << fEvent_Ncharge  << ", "
				<< "Nneutral = " << fEvent_Nneutral << ", "
				<< "Ntotal = "   << fEvent_Ntotal   << endreq;

			// * Set vertex origin *
				IVertexDbSvc* vtxsvc;
				Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);
				if(vtxsvc->isVertexValid()){
					double* dbv = vtxsvc->PrimaryVertex();
					double* vv = vtxsvc->SigmaPrimaryVertex();
					fEvent_Vx0 = dbv[0];
					fEvent_Vy0 = dbv[1];
					fEvent_Vz0 = dbv[2];
				}


		// * STEP (B): Create selection charged tracks * //

			// * Print log and set counters *
				fLog << MSG::DEBUG << "Starting 'good' charged track selection:" << endreq;
				fEvent_Nmdc = 0; // @todo Check if this makes sense at all
				ParticleID *pid = ParticleID::instance();

			// * Clear vectors of selected particles *
				fGoodChargedTracks.clear();
				fKaonNeg.clear();
				fKaonPos.clear();
				fPionPos.clear();

			// * Loop over charged tracks *
			for(int i = 0; i < fEvtRecEvent->totalCharged(); ++i) {
			// Note: the first part of the set of reconstructed tracks are the charged tracks

				// * STEP 1: Get MDC information

					// * Get track info from Main Drift Chamber
					fLog << MSG::DEBUG << "   charged track " << i << "/" << fEvtRecEvent->totalCharged() << endreq;
					fTrackIterator = fEvtRecTrkCol->begin() + i; 
					if(!(*fTrackIterator)->isMdcTrackValid()) continue;
					RecMdcTrack *mdcTrk = (*fTrackIterator)->mdcTrack();

					// * Get kinematics of track
					fTrack_p   = mdcTrk->p();
					fTrack_Vx  = mdcTrk->x();
					fTrack_Vy  = mdcTrk->y();
					fTrack_Vz  = mdcTrk->z();
					fTrack_phi = mdcTrk->helix(1);
					fTrack_Vr =
						(fTrack_Vx - fEvent_Vx0) * cos(fTrack_phi) +
						(fTrack_Vy - fEvent_Vy0) * sin(fTrack_phi);

					// * Get radii of track vertex
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

				// * STEP 2: Apply vertex cuts, store track, and write info

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

				// * STEP 3: Distinguish and store types of particles

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

					// * WRITE particle identification info ("pid" branch)
					if(fDoPID) {
						fTrack_p = mdcTrk->p();
						fPID_Cost = cos(mdcTrk->theta());
						fPID_Dedx = pid->chiDedx(2);
						fPID_Tof1 = pid->chiTof1(2);
						fPID_Tof2 = pid->chiTof2(2);
						fPID_Prob = pid->probPion();
						fNTupleMap["pid"]->write();
					}

					// * Identify type of charged particle and add to related vector (this package: kaon and pion) *
					RecMdcKalTrack* mdcKalTrk = (*fTrackIterator)->mdcKalTrack(); // after ParticleID, use RecMdcKalTrack substitute RecMdcTrack
					if(pid->probPion() > pid->probKaon()) { // NOTE: if more likely to be pion
						if(pid->probPion() < 0.001) continue; // check if indeed a pion
						if(pid->pdf(2) < pid->pdf(3)) continue; // for likelihood method (0=electron 1=muon 2=pion 3=kaon 4=proton)
						RecMdcKalTrack::setPidType(RecMdcKalTrack::pion); // PID can be set to electron, muon, pion (default), kaon and proton
						if(mdcKalTrk->charge() > 0) fPionPos.push_back(*fTrackIterator); // if positive pion
					}
					else { // NOTE: if more likely to be kaon (K+-)
						if(pid->probKaon() < 0.001) continue; // check if indeed a kaon
						if(pid->pdf(3) < pid->pdf(2)) continue; // for likelihood method (0=electron 1=muon 2=pion 3=kaon 4=proton)
						RecMdcKalTrack::setPidType(RecMdcKalTrack::kaon); // PID can be set to electron, muon, pion (default), kaon and proton
						if(mdcKalTrk->charge() < 0) fKaonNeg.push_back(*fTrackIterator); // if negative kaon
						else                        fKaonPos.push_back(*fTrackIterator); // if positive kaon
					}

			}


		// * STEP (C): WRITE event info ("mult" branch) * //
			fLog << MSG::DEBUG << "ngood, totcharge = " << fGoodChargedTracks.size() << " , " << fEvent_Nmdc << endreq;
			if(fDoMult) {
				fEvent_Ngood = fGoodChargedTracks.size();
				fEvent_NKaonNeg = fKaonNeg.size();
				fEvent_NKaonPos = fKaonPos.size();
				fEvent_NPionPos = fPionPos.size();
				fNTupleMap["mult"]->write();
			} // end of fDoVxyz


		// * STEP (D): WRITE dE/dx PID information ("dedx" branch) * //
			if(fDoDedx)      WriteDedxInfoForVector(fGoodChargedTracks, "dedx");
			if(fDoDedx_Kaon) WriteDedxInfoForVector(fGoodChargedTracks, "dedx_K");
			if(fDoDedx_Kaon) WriteDedxInfoForVector(fGoodChargedTracks, "dedx_K");
			if(fDoDedx_Pion) WriteDedxInfoForVector(fGoodChargedTracks, "dedx_pi");


		// * STEP (E): WRITE Time-of-Flight PID information ("tof*" branch) * //
			if(fDoTofEC || fDoTofIB || fDoTofOB) for(fTrackIterator = fGoodChargedTracks.begin(); fTrackIterator != fGoodChargedTracks.end(); ++fTrackIterator) {

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
							if(fDoTofIB) WriteTofInformation(iter_tof, ptrk, "tof1");
						} else if(hitStatus.layer() == 2) { // outer barrel
							if(fDoTofOB) WriteTofInformation(iter_tof, ptrk, "tof2");
						}
					}
					else if(fDoTofEC && hitStatus.layer() == 0) // end cap
						WriteTofInformation(iter_tof, ptrk, "tofe");
				}

			} // loop all charged tracks


		// * STEP (F): WRITE Kalman 4-constraint kinematic fit with smallest chi squared ("fit4c" branch) * //
			if(fDoFit4c) {

				// * Loop over all combinations and get the one with the smallest chi square
				fSmallestChiSq = 9999999.;
				KalmanKinematicFit *bestKalmanFit = KalmanKinematicFit::instance();
				for(fKaonNeg1Iter = fKaonNeg.begin(); fKaonNeg1Iter != fKaonNeg.end(); ++fKaonNeg1Iter) {
					for(fKaonNeg2Iter = fKaonNeg1Iter+1; fKaonNeg2Iter != fKaonNeg.end(); ++fKaonNeg2Iter) {
						for(fKaonPosIter = fKaonPos.begin(); fKaonPosIter != fKaonPos.end(); ++fKaonPosIter) {
							for(fPionPosIter = fPionPos.begin(); fPionPosIter != fPionPos.end(); ++fPionPosIter) {

								// * Get Kalman tracks
									RecMdcKalTrack *KmTrk1 = (*fKaonNeg1Iter)->mdcKalTrack();
									RecMdcKalTrack *KmTrk2 = (*fKaonNeg2Iter)->mdcKalTrack();
									RecMdcKalTrack *KpTrk  = (*fKaonPosIter )->mdcKalTrack();
									RecMdcKalTrack *pipTrk = (*fPionPosIter )->mdcKalTrack();

								// * Get W-tracks
									WTrackParameter wvKmTrk1(mK,  KmTrk1->getZHelix(), KmTrk1->getZError());
									WTrackParameter wvKmTrk2(mK,  KmTrk2->getZHelix(), KmTrk2->getZError());
									WTrackParameter wvKpTrk (mK,  KpTrk->getZHelix(),  KpTrk->getZError());
									WTrackParameter wvpipTrk(mpi, pipTrk->getZHelix(), pipTrk->getZError());

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
									if(!vtxfit->Fit(0)) continue;
									vtxfit->Swim(0);

									WTrackParameter wpip = vtxfit->wtrk(0);
									WTrackParameter wKp  = vtxfit->wtrk(1);
									WTrackParameter wKm1 = vtxfit->wtrk(2);
									WTrackParameter wKm2 = vtxfit->wtrk(3);

								// * Get Kalman kinematic fit for this combination and store if better than previous one
									double chisq = 999999.;
									KalmanKinematicFit *kkmfit = KalmanKinematicFit::instance();
									kkmfit->init();
									kkmfit->AddTrack(0, wKm1);  // K- (1st occurrence)
									kkmfit->AddTrack(1, wpip);  // pi+
									kkmfit->AddTrack(2, wKm2);  // K- (2nd occurrence)
									kkmfit->AddTrack(3, wKp);   // K+
									kkmfit->AddFourMomentum(0, ecms); // 4 constraints: CMS energy and momentum
									if(kkmfit->Fit()) {
										chisq = kkmfit->chisq();
										if(chisq < fSmallestChiSq) {
											bestKalmanFit = kkmfit;
											fSmallestChiSq = chisq;
										}
									}

							} // end of loop over pi^+
						} // end of loop over K^+
					} // end of loop over second K^-
				} // end of loop over first K^-

				// * WRITE D0 and phi information from 4-constraint fit ("fit4c" branch)
					if(fDoFit4c && bestKalmanFit && fSmallestChiSq < fMaxChiSq) {
						HepLorentzVector pD0   = bestKalmanFit->pfit(0) + bestKalmanFit->pfit(1);
						HepLorentzVector pphi  = bestKalmanFit->pfit(2) + bestKalmanFit->pfit(3);
						HepLorentzVector pJpsi = pD0 + pphi;
						fInvMass_D0   = pD0.m();   // invariant D0 mass according to Kalman kinematic fit
						fInvMass_phi  = pphi.m();  // invariant phi mass according to Kalman kinematic fit
						fInvMass_Jpsi = pJpsi.m(); // invariant Jpsi mass according to Kalman kinematic fit
						fInvMass_ChiSq = fSmallestChiSq; // chi square of the Kalman kinematic fit
						fNTupleMap["fit4c"]->write();
					}

			} // end of fDoFit4c


		// * STEP (G): WRITE Kalman 6-constraint kinematic fit with smallest chi squared ("fit6c" branch) * //
			if(fDoFit6c) {

				// * Loop over all combinations and get the one with the smallest chi square
				fSmallestChiSq = 9999999.;
				KalmanKinematicFit *bestKalmanFit = KalmanKinematicFit::instance();
				for(fKaonNeg1Iter = fKaonNeg.begin(); fKaonNeg1Iter != fKaonNeg.end(); ++fKaonNeg1Iter) {
					for(fKaonNeg2Iter = fKaonNeg1Iter+1; fKaonNeg2Iter != fKaonNeg.end(); ++fKaonNeg2Iter) {
						for(fKaonPosIter = fKaonPos.begin(); fKaonPosIter != fKaonPos.end(); ++fKaonPosIter) {
							for(fPionPosIter = fPionPos.begin(); fPionPosIter != fPionPos.end(); ++fPionPosIter) {

								// * Get Kalman tracks
									RecMdcKalTrack *KmTrk1 = (*fKaonNeg1Iter)->mdcKalTrack();
									RecMdcKalTrack *KmTrk2 = (*fKaonNeg2Iter)->mdcKalTrack();
									RecMdcKalTrack *KpTrk  = (*fKaonPosIter )->mdcKalTrack();
									RecMdcKalTrack *pipTrk = (*fPionPosIter )->mdcKalTrack();

								// * Get W-tracks
									WTrackParameter wvKmTrk1(mK,  KmTrk1->getZHelix(), KmTrk1->getZError());
									WTrackParameter wvKmTrk2(mK,  KmTrk2->getZHelix(), KmTrk2->getZError());
									WTrackParameter wvKpTrk (mK,  KpTrk->getZHelix(),  KpTrk->getZError());
									WTrackParameter wvpipTrk(mpi, pipTrk->getZHelix(), pipTrk->getZError());

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
									if(!vtxfit->Fit(0)) continue;
									vtxfit->Swim(0);

									WTrackParameter wpip = vtxfit->wtrk(0);
									WTrackParameter wKp  = vtxfit->wtrk(1);
									WTrackParameter wKm1 = vtxfit->wtrk(2);
									WTrackParameter wKm2 = vtxfit->wtrk(3);

								// * Get Kalman kinematic fit for this combination and store if better than previous one
									double chisq = 999999.;
									KalmanKinematicFit *kkmfit = KalmanKinematicFit::instance();
									kkmfit->init();
									kkmfit->AddTrack(0, wKm1);           // K- (1st occurrence)
									kkmfit->AddTrack(1, wpip);           // pi+
									kkmfit->AddTrack(2, wKm2);           // K- (2nd occurrence)
									kkmfit->AddTrack(3, wKp);            // K+
									kkmfit->AddFourMomentum(2, ecms); // 4 constraints: CMS energy and momentum
									if(kkmfit->Fit(0) && kkmfit->Fit(1) && kkmfit->Fit(2) && kkmfit->Fit()) {
										chisq = kkmfit->chisq();
										if(chisq < fSmallestChiSq) {
											bestKalmanFit = kkmfit;
											fSmallestChiSq = chisq;
										}
									}

							} // end of loop over pi^+
						} // end of loop over K^+
					} // end of loop over second K^-
				} // end of loop over first K^-

				// * WRITE D0 and phi information from 5-constraint fit ("fit6c" branch)
				if(bestKalmanFit && fSmallestChiSq < fMaxChiSq) {
					HepLorentzVector pD0   = bestKalmanFit->pfit(0) + bestKalmanFit->pfit(1);
					HepLorentzVector pphi  = bestKalmanFit->pfit(2) + bestKalmanFit->pfit(3);
					HepLorentzVector pJpsi = pD0 + pphi;
					fInvMass_D0   = pD0.m();
					fInvMass_phi  = pphi.m();
					fInvMass_Jpsi = pJpsi.m();
					fInvMass_ChiSq = fSmallestChiSq;
					fNTupleMap["fit6c"]->write();
				}
			} // end of fDoFit6c


		return StatusCode::SUCCESS;
	}


	/**
	 * @brief Currently does nothing. Cut flow could be printed in this step.
	 * @todo Add log output to `finalize` step.
	 */
	StatusCode DzeroPhi::finalize()
	{
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
			if(!nt) fLog << MSG::ERROR << "    Cannot book N-tuple:" << long(nt) << " (" << tupleName << ")" << endmsg;
		}
		// fNTupleMap[tupleName] = nt.ptr();
		fNTupleMap.insert(make_pair(tupleName, nt.ptr()));
		return nt;
	}


	/**
	 * @brief This function encapsulates the `addItem` procedure for the ToF branch. This allows to standardize the loading of the end cap, inner barrel, and outer barrel ToF branches.
	 */
	void DzeroPhi::BookNtupleTofItems(NTuplePtr& nt)
	{
		if(!nt) return;
		nt->addItem("tof",  fTof);
		nt->addItem("ptrk", fTof_Ptot);
		nt->addItem("path", fTof_Path);
		nt->addItem("cntr", fTof_Cntr);
		nt->addItem("ph",   fTof_Ph);
		nt->addItem("rhit", fTof_Rhit);
		nt->addItem("qual", fTof_Qual);
		nt->addItem("te",   fTof_Electron);
		nt->addItem("tmu",  fTof_Muon);
		nt->addItem("tpi",  fTof_Proton);
		nt->addItem("tk",   fTof_Kaon);
		nt->addItem("tp",   fTof_Proton);
	}


	/**
	 * @brief This function encapsulates the `addItem` procedure for the \f$dE/dx\f$ energy loss branch (`"dedx"`).
	 * @details This method allows you to perform the same booking method for different types of charged particles (for instance 'all charged particles', kaons, and pions).
	 */
	void DzeroPhi::BookNtupleDedxItems(NTuplePtr& nt)
	{
		if(!nt) return;
		nt->addItem("ptrk",   fTrack_p);
		nt->addItem("chie",   fDedx_Chi2e);
		nt->addItem("chimu",  fDedx_Chi2mu);
		nt->addItem("chipi",  fDedx_Chi2pi);
		nt->addItem("chik",   fDedx_Chi2k);
		nt->addItem("chip",   fDedx_Chi2p);
		nt->addItem("probPH", fDedx_ProbPH);
		nt->addItem("normPH", fDedx_NormPH);
		nt->addItem("ghit",   fDedx_Ghit);
		nt->addItem("thit",   fDedx_Thit);
	}


	/**
	 * @brief
	 */
	void DzeroPhi::WriteTofInformation(SmartRefVector<RecTofTrack>::iterator iter_tof, double ptrk, const char* tupleName)
	{

		// * Get ToF info
		fTof_Ptot = ptrk;
		fTof      = (*iter_tof)->tof();
		fTof_Path = (*iter_tof)->path();
		fTof_Ph   = (*iter_tof)->ph();
		fTof_Rhit = (*iter_tof)->zrhit();
		fTof_Qual = 0.+(*iter_tof)->quality();
		fTof_Cntr = 0.+(*iter_tof)->tofID();

		// * Get ToF for each particle hypothesis
		std::vector<double> texp(nmass);
		for(size_t j = 0; j < texp.size(); ++j) {
			double gb = fTof_Ptot/xmass[j]; // v = p/m (non-relativistic velocity)
			double beta = gb/sqrt(1+gb*gb);
			texp[j] = 10 * fTof_Path /beta/velc_mm; // hypothesis ToF
		}

		// * WRITE ToF info
		fTof_Electron = fTof_Path - texp[0];
		fTof_Muon     = fTof_Path - texp[1];
		fTof_Proton  = fTof_Path - texp[2];
		fTof_Kaon     = fTof_Path - texp[3];
		fTof_Proton   = fTof_Path - texp[4];
		fNTupleMap[tupleName]->write();

	}


	/**
	 * @brief Encapsulates a `for` loop of the writing procedure for \f$dE/dx\f$ energy loss information.
	 * @details This method allows you to write \f$dE/dx\f$ information for any selection of charged tracks. Just feed it a vector that contains such a collection of `EvtRecTrack` pointers.
	 * 
	 * @param vector The selection of charged tracks that of which you want to write the \f$dE/dx\f$ data.
	 * @param tupleName The name of the tuple to which you want to write the information.
	 */
	void DzeroPhi::WriteDedxInfoForVector(std::vector<EvtRecTrack*>& vector, const char* tupleName)
	{
		for(fTrackIterator = vector.begin(); fTrackIterator != vector.end(); ++fTrackIterator) {
			WriteDedxInfo(*fTrackIterator, tupleName);
		}
	}


	/**
	 * @brief Encapsulates of the writing procedure for \f$dE/dx\f$ energy loss information <i>for one track</i>.
	 * 
	 * @param evtRecTrack Pointer to the reconstructed track of which you want to write the \f$dE/dx\f$ data.
	 * @param tupleName The name of the tuple to which you want to write the information.
	 */
	void DzeroPhi::WriteDedxInfo(EvtRecTrack* evtRecTrack, const char* tupleName)
	{

		// * Check if dE/dx and MDC info exists *
		if(!evtRecTrack->isMdcTrackValid()) return;
		if(!evtRecTrack->isMdcDedxValid())  return;
		RecMdcTrack* mdcTrk = evtRecTrack->mdcTrack();
		RecMdcDedx* dedxTrk = evtRecTrack->mdcDedx();

		// * WRITE energy loss PID info ("dedx" branch) *
		fTrack_p = mdcTrk->p();
		fDedx_Chi2e   = dedxTrk->chiE();
		fDedx_Chi2mu  = dedxTrk->chiMu();
		fDedx_Chi2pi  = dedxTrk->chiPi();
		fDedx_Chi2k   = dedxTrk->chiK();
		fDedx_Chi2p   = dedxTrk->chiP();
		fDedx_ProbPH  = dedxTrk->probPH();
		fDedx_NormPH  = dedxTrk->normPH();
		fDedx_Ghit    = dedxTrk->numGoodHits();
		fDedx_Thit    = dedxTrk->numTotalHits();
		fNTupleMap[tupleName]->write();

	}