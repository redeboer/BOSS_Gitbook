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
	#include "DzeroPhiDer/DzeroPhiDer.h"
	#include "TrackSelector/TrackSelector.h"
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
	 * @brief Constructor for the `DzeroPhiDer` algorithm.
	 * @details Here, you should declare properties: give them a name, assign a parameter (data member of `DzeroPhiDer`), and if required a documentation string. Note that you should define the paramters themselves in the header (DzeroPhiDer/DzeroPhiDer.h) and that you should assign the values in `share/jopOptions_DzeroPhiDer.txt`.
	 */
	// Algorithms should inherit from Gaudi's `Algorithm` class. See https://dayabay.bnl.gov/dox/GaudiKernel/html/classAlgorithm.html.
	DzeroPhiDer::DzeroPhiDer(const std::string &name, ISvcLocator* pSvcLocator) :
		Algorithm(name, pSvcLocator),
		fLog(msgSvc(), name),
		fEventHeader (eventSvc(), "dummy"),
		fEvtRecEvent (eventSvc(), EventModel::EvtRec::EvtRecEvent),
		fEvtRecTrkCol(eventSvc(), EventModel::EvtRec::EvtRecTrackCol)
	{

		// * Whether or not to fill a tree/NTuple *
		declareProperty("doFit4c",       fDoFit4c);
		declareProperty("doFit6c",       fDoFit6c);

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
	StatusCode DzeroPhiDer::initialize_rest()
	{

		// * Book NTuple: dE/dx PID branch * //
			if(fDoDedx) {
				BookNtupleItemsDedx("dedx_K", fDedx_K);
				BookNtupleItemsDedx("dedx_pi", fDedx_pi);
			}

		// * Book NTuple: 4-contraints for Kalman kinematic fit * //
			if(fDoFit4c) {
				fFit4c["mD0"];   /// Invariant mass for \f$ K^- pi^+ \f$ (\f$ D^0 \f$).
				fFit4c["mphi"];  /// Invariant mass for \f$ K^+ K^+  \f$ (\f$ \phi \f$).
				fFit4c["mJpsi"]; /// Invariant mass for \f$ D^0 \phi \f$ (\f$ J/\psi \f$).
				fFit4c["chisq"];  /// Chi squared of the Kalman kinematic fit.
				AddItemsToNTuples("fit4c", fFit4c);
			}

		// * Book NTuple: 6-contraints for Kalman kinematic fit * //
			if(fDoFit6c) {
				fFit6c["mD0"];   /// Invariant mass for \f$ K^- pi^+ \f$ (\f$ D^0 \f$).
				fFit6c["mphi"];  /// Invariant mass for \f$ K^+ K^+  \f$ (\f$ \phi \f$).
				fFit6c["mJpsi"]; /// Invariant mass for \f$ D^0 \phi \f$ (\f$ J/\psi \f$).
				fFit6c["chi2"];  /// Chi squared of the Kalman kinematic fit.
				AddItemsToNTuples("fit6c", fFit6c);
			}

		fLog << MSG::INFO << "Successfully returned from initialize()" << endmsg;
		return StatusCode::SUCCESS;
	}


	/**
	 * @brief Inherited `execute` method of the `Algorithm`. This function is called *for each event*.
	 */
	StatusCode DzeroPhiDer::execute_rest()
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


		// * STEP (B): Create selection charged tracks * //

			// * Print log and set counters *
				fLog << MSG::DEBUG << "Starting 'good' charged track selection:" << endreq;
				int nChargesMDC = 0;
				ParticleID *pid = ParticleID::instance();

			// * Clear vectors of selected particles *
				fGoodChargedTracks.clear();
				fKaonNeg.clear();
				fKaonPos.clear();
				fPionPos.clear();

			// * Loop over charged tracks *
			for(int i = 0; i < fEvtRecEvent->totalCharged(); ++i) {
			// Note: the first part of the set of reconstructed tracks are the charged tracks

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
						fPID.at("ptrk") = mdcTrk->p();
						fPID.at("cost") = cos(mdcTrk->theta());
						fPID.at("dedx") = pid->chiDedx(2);
						fPID.at("tof1") = pid->chiTof1(2);
						fPID.at("tof2") = pid->chiTof2(2);
						fPID.at("prob") = pid->probPion();
						fNTupleMap.at("pid")->write();
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

		// * STEP (D): WRITE dE/dx PID information ("dedx" branch) * //
			if(fDoDedx) {
				WriteDedxInfoForVector(fKaonNeg,           "dedx_K",  fDedx_K);
				WriteDedxInfoForVector(fKaonPos,           "dedx_K",  fDedx_K);
				WriteDedxInfoForVector(fPionPos,           "dedx_pi", fDedx_pi);
			}


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
						fFit4c.at("mD0")   = pD0.m();
						fFit4c.at("mphi")  = pphi.m();
						fFit4c.at("mJpsi") = pJpsi.m();
						fFit4c.at("chisq") = fSmallestChiSq;
						fNTupleMap.at("fit4c")->write();
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
					fFit6c.at("mD0")   = pD0.m();
					fFit6c.at("mphi")  = pphi.m();
					fFit6c.at("mJpsi") = pJpsi.m();
					fFit6c.at("chisq") = fSmallestChiSq;
					fNTupleMap.at("fit6c")->write();
				}
			} // end of fDoFit6c


		return StatusCode::SUCCESS;
	}


	/**
	 * @brief Currently does nothing. Cut flow could be printed in this step.
	 * @todo Add log output to `finalize` step.
	 */
	StatusCode DzeroPhiDer::finalize_rest()
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
	 * @param tupleTitle Description of the `NTuple`. Has a default value if you don't want to think about this.
	 */
	NTuplePtr DzeroPhiDer::BookNTuple(const char* tupleName, const char* tupleTitle)
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
	void DzeroPhiDer::BookNtupleItemsTof(const char* tupleName, std::map<std::string, NTuple::Item<TYPE> > &map)
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
	void DzeroPhiDer::BookNtupleItemsDedx(const char* tupleName, std::map<std::string, NTuple::Item<TYPE> > &map)
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
	void DzeroPhiDer::WriteTofInformation(SmartRefVector<RecTofTrack>::iterator iter_tof, double ptrk, const char* tupleName, std::map<std::string, NTuple::Item<TYPE> > &map)
	{

		// * Get ToF for each particle hypothesis
		double path = (*iter_tof)->path();
		std::vector<double> texp(nmass);
		for(size_t j = 0; j < texp.size(); ++j) {
			double gb = ptrk/xmass[j]; // v = p/m (non-relativistic velocity)
			double beta = gb/sqrt(1+gb*gb);
			texp[j] = 10 * path /beta/velc_mm; // hypothesis ToF
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
	template<typename TYPE>
	void DzeroPhiDer::WriteDedxInfoForVector(std::vector<EvtRecTrack*> &vector, const char* tupleName, std::map<std::string, NTuple::Item<TYPE> > &map)
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
	void DzeroPhiDer::WriteDedxInfo(EvtRecTrack* evtRecTrack, const char* tupleName, std::map<std::string, NTuple::Item<TYPE> > &map)
	{

		// * Check if dE/dx and MDC info exists *
		if(!evtRecTrack->isMdcTrackValid()) return;
		if(!evtRecTrack->isMdcDedxValid())  return;
		RecMdcTrack* mdcTrk = evtRecTrack->mdcTrack();
		RecMdcDedx* dedxTrk = evtRecTrack->mdcDedx();

		// * WRITE energy loss PID info ("dedx" branch) *
		map.at("ptrk")   = mdcTrk->p();
		map.at("chie")   = dedxTrk->chiE();
		map.at("chimu")  = dedxTrk->chiMu();
		map.at("chipi")  = dedxTrk->chiPi();
		map.at("chik")   = dedxTrk->chiK();
		map.at("chip")   = dedxTrk->chiP();
		map.at("probPH") = dedxTrk->probPH();
		map.at("normPH") = dedxTrk->normPH();
		map.at("ghit")   = dedxTrk->numGoodHits();
		map.at("thit")   = dedxTrk->numTotalHits();
		fNTupleMap.at(tupleName)->write();

	}

	/**
	 * @brief Automatically assign all <i>mapped values</i> of `map` as items to an `NTuplePtr`.
	 * 
	 * @param nt The `NTuplePtr` to which you want to add the <i>mapped values</i> of `map`.
	 * @param map The `map` from which you want to load the <i>mapped values</i>.
	 */
	template<typename TYPE>
	void DzeroPhiDer::AddItemsToNTuples(NTuplePtr nt, std::map<std::string, NTuple::Item<TYPE> > &map)
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
	void DzeroPhiDer::AddItemsToNTuples(const char* tupleName, std::map<std::string, NTuple::Item<TYPE> > &map)
	{
		AddItemsToNTuples(BookNTuple(tupleName), map);
	}

	// /**
	//  * @brief Function that allows you to draw and save any set of `TObject`s.
	//  */
	// template<typename TYPE, class ...ARGS>
	// void AddItemsToNTuples(std::map<std::string, NTuple::Item<TYPE> > &map, Option_t* option, const char* logScale, ARGS... args)
	// {
	// 	// * Create canvas * //
	// 		TCanvas c;
	// 		SetLogScale(c, logScale);
	// 		c.SetBatch();
	// 	// * Draw objects * //
	// 		DrawAndSaveRecursion(option, args...);
	// 	// * Save canvas * //
	// 		const TString outputDir = Form("%s/%s", Settings::Output::PlotOutputDir.Data(), __BASE_FILE__);
	// 		gSystem->mkdir(outputDir.Data());
	// 		c.SaveAs(Form("%s/%s.%s", outputDir.Data(), filename, Settings::Output::Extension));
	// }

	// /**
	//  * @brief The `CreateNTuples` functions are necessary for `AddItemsToNTuples`, which is a variadic template function.
	//  */
	// template<class... ARGS> void AddItemsToNTuples(Option_t* option, ARGS&&... args); // start recursion
	// template<class TYPE, class... ARGS>
	// void AddItemsToNTuples(Option_t* option, TYPE first, ARGS... args)
	// {
	// 	auto obj = dynamic_cast<TObject*>(first);
	// 	if(obj) obj->Draw(option);
	// 	AddItemsToNTuples(option, args...); // continue recursion
	// }
	// template<> void AddItemsToNTuples(Option_t* option) {} // end recursion