// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "CLHEP/Geometry/Point3D.h"
	#include "CLHEP/Vector/LorentzVector.h"
	#include "CLHEP/Vector/ThreeVector.h"
	#include "CLHEP/Vector/TwoVector.h"
	#include "TrackSelector/rhopi_pipigg.h"
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

	const double gM_pi  = 0.13957061; /// Mass of \f$\pi^\pm\f$.
	const double gM_pi0 = 0.13957061; /// Mass of \f$\pi^0\f$.
	const double gM_rho = 0.770;      /// Mass of \f$\rho\f$.
	const double gEcms  = 3.097;      /// Center-of-mass energy.
	const HepLorentzVector gEcmsVec(0.034, 0, 0, gEcms);



// * =========================== * //
// * ------- CONSTRUCTOR ------- * //
// * =========================== * //

	/**
	 * @brief Constructor for the `rhopi_pipigg` algorithm.
	 * @details Here, you should declare properties: give them a name, assign a parameter (data member of `rhopi_pipigg`), and if required a documentation string. Note that you should define the paramters themselves in the header (rhopi_pipigg/rhopi_pipigg.h) and that you should assign the values in `share/jopOptions_rhopi_pipigg.txt`.
	 */
	rhopi_pipigg::rhopi_pipigg(const std::string &name, ISvcLocator* pSvcLocator) :
		TrackSelector(name, pSvcLocator)
	{

		// * Whether or not to fill a tree/NTuple *
		declareProperty("doFit4c", fDoFit4c);
		declareProperty("doFit6c", fDoFit5c);

		// * Define cuts
		declareProperty("MaxChiSq", fMaxChiSq);
		declareProperty("dM_rho0", fDeltaMrho0 = .150);
		declareProperty("MaxGammaPhi",   fMaxGammaPhi   =  20.0);
		declareProperty("MaxGammaTheta", fMaxGammaTheta =  20.0);
		declareProperty("MaxGammaAngle", fMaxGammaAngle = 200.0);

	}



// * =============================== * //
// * ------- ALGORITHM STEPS ------- * //
// * =============================== * //

	/**
	 * @brief   (Inherited) `initialize` step of `Algorithm`. This function is called only once in the beginning.
	 * @details Define and load NTuples here.
	 */
	StatusCode rhopi_pipigg::initialize_rest()
	{
		// * Book NTuple: dE/dx PID branch * //
			if(fDoDedx) {
				BookNtupleItemsDedx("dedx_pi", fDedx_pi);
			}

		// * Book NTuple: gamma information branch * //
			if(fDoGammaInfo) {
				fGammaInfo["E"];     /// Energy of the photon.
				fGammaInfo["phi"];   /// Smallest angle between the photon and the nearest charged pion.
				fGammaInfo["theta"]; /// Smallest angle between the photon and the nearest charged pion.
				fGammaInfo["angle"]; /// Smallest angle between the photon and the nearest charged pion.
				AddItemsToNTuples("photon", fGammaInfo);
			}

		// * Book NTuple: 4-contraints for Kalman kinematic fit * //
			if(fDoFit4c) {
				fFit4c["mpi0"];  /// Invariant mass for \f$\gamma\gamma\f$ (\f$\pi^0\f$).
				fFit4c["mrho"];  /// Invariant mass for \f$\pi\pi\f$ (\f$\rho\f$).
				fFit4c["chisq"]; /// Chi squared of the Kalman kinematic fit.
				AddItemsToNTuples("fit4c", fFit4c);
			}

		// * Book NTuple: 6-contraints for Kalman kinematic fit * //
			if(fDoFit5c) {
				fFit5c["mpi0"];  /// Invariant mass for \f$\gamma\gamma\f$ (\f$\pi^0\f$).
				fFit5c["mrho"];  /// Invariant mass for \f$\pi\pi\f$ (\f$\rho\f$).
				fFit5c["chisq"]; /// Chi squared of the Kalman kinematic fit.
				AddItemsToNTuples("fit5c", fFit5c);
			}

		fLog << MSG::INFO << "Successfully returned from initialize()" << endmsg;
		return StatusCode::SUCCESS;
	}


	/**
	 * @brief Inherited `execute` method of the `Algorithm`. This function is called *for each event*.
	 */
	StatusCode rhopi_pipigg::execute_rest()
	{

		/// STEP (A): Create selection charged tracks * //

			// * Print log and set counters *
			fLog << MSG::DEBUG << "Starting 'good' charged track selection:" << endmsg;
			ParticleID *pid = ParticleID::instance();

			// * Loop over charged tracks *
			fPionNeg.clear();
			fPionPos.clear();
			for(fTrackIterator = fGoodChargedTracks.begin(); fTrackIterator != fGoodChargedTracks.end(); ++fTrackIterator) {

				/// STEP A.1: Initialise PID

					// * Initialise PID sub-system and set method: probability, likelihood, or neuron network
					pid->init();
					pid->setMethod(pid->methodProbability());
					// pid->setMethod(pid->methodLikelihood());
					// pid->setMethod(pid->methodNeuronNetwork());
					pid->setChiMinCut(4);
					pid->setRecTrack(*fTrackIterator);

					// * Choose ID system and which particles to use
					pid->usePidSys(pid->useDedx() | pid->useTof1() | pid->useTof2() | pid->useTofE()); // use PID sub-system
					pid->identify(pid->onlyPion()); // seperater Pion/Kaon

					// * Perform PID
					pid->calculate();
					if(!(pid->IsPidInfoValid())) continue;

				/// STEP A.2: Identify type of charged particle and add to related vector (this package: kaon and pion)
					RecMdcKalTrack* mdcKalTrk = (*fTrackIterator)->mdcKalTrack(); // after ParticleID, use RecMdcKalTrack substitute RecMdcTrack
					if(pid->probPion() < 0.001) continue; // check if indeed a pion
					// if(pid->pdf(2) < pid->pdf(3)) continue; // for likelihood method (0=electron 1=muon 2=pion 3=kaon 4=proton)
					RecMdcKalTrack::setPidType(RecMdcKalTrack::pion); // PID can be set to electron, muon, pion (default), kaon and proton
					if(mdcKalTrk->charge() < 0) fPionNeg.push_back(*fTrackIterator); // if negative pion
					else                        fPionPos.push_back(*fTrackIterator); // if positive pion

				/// STEP A.3: WRITE PID information
					if(fDoPID) WritePIDInformation(pid);

			}

		/// STEP (B): WRITE dE/dx PID information ("dedx" branch) * //
			if(fDoDedx) {
				WriteDedxInfoForVector(fPionNeg, "dedx_pi",  fDedx_pi);
				WriteDedxInfoForVector(fPionPos, "dedx_pi",  fDedx_pi);
			}

		/// STEP (C): Create selection neutral tracks (photons) * //

			// * Print log and clear vectors of selected particles *
			fLog << MSG::DEBUG << "Starting 'good' neutral track selection:" << endmsg;

			// * Loop over charged tracks *
			fGamma.clear();
			for(fTrackIterator = fGoodNeutralTracks.begin(); fTrackIterator != fGoodNeutralTracks.end(); ++fTrackIterator) {

				/// STEP C.1: Get EM calorimeter info
				fTrackEMC = (*fTrackIterator)->emcShower();
				Hep3Vector emcpos = fTrackEMC->position();

				/// STEP C.2: Find angle differences with nearest charged pion
				fSmallestTheta = 1000.; // start value for difference in theta
				fSmallestPhi   = 1000.; // start value for difference in phi
				fSmallestAngle = 1000.; // start value for difference in angle (?)
				SetSmallestAngles(fPionNegIter, fPionNeg, emcpos); // check all differences with the pions
				SetSmallestAngles(fPionPosIter, fPionPos, emcpos); // check all differences with the pions

				// * Apply angle cut
				if(fSmallestAngle >= fMaxGammaAngle) continue;
				fSmallestTheta = fSmallestTheta * 180 / (CLHEP::pi);
				fSmallestPhi   = fSmallestPhi   * 180 / (CLHEP::pi);
				fSmallestAngle = fSmallestAngle * 180 / (CLHEP::pi);

				// * WRITE photon info ("photon" branch)
				fGammaInfo.at("E")     = fTrackEMC->energy();
				fGammaInfo.at("phi")   = fSmallestTheta;
				fGammaInfo.at("theta") = fSmallestPhi;
				fGammaInfo.at("angle") = fSmallestAngle;
				fNTupleMap.at("photon")->write(); // "photon" branch

				// * Apply photon cuts (energy cut has already been applied in TrackSelector)
				if(
					(fabs(fSmallestTheta) < fMaxGammaTheta) &&
					(fabs(fSmallestPhi)   < fMaxGammaPhi)) continue;
				if(fabs(fSmallestAngle) < fMaxGammaAngle) continue;

				// * Add photon track to vector for gammas
				fGamma.push_back(*fTrackIterator);

			}

			// * Perform strict cut on number of gammas *
			fLog << MSG::DEBUG << "Number of good photons: " << fGamma.size() << endmsq;
			if(fGamma.size() < 2) return StatusCode::SUCCESS;


		/// STEP (C): WRITE Kalman 4-constraint kinematic fit with smallest chi squared ("fit4c" branch) * //
			if(fDoFit4c) {

				// * Loop over all combinations and get the one with the smallest chi square
				fSmallestChiSq = 9999999.;
				KalmanKinematicFit *bestKalmanFit = KalmanKinematicFit::instance();
				for(fGamma1Iter = fGamma.begin(); fGamma1Iter != fGamma.end(); ++fGamma1Iter) {
					for(fGamma2Iter = fGamma1Iter+1; fGamma2Iter != fGamma.end(); ++fGamma2Iter) {
						for(fPionPosIter = fPionPos.begin(); fPionPosIter != fPionPos.end(); ++fPionPosIter) {
							for(fPionPosIter = fPionPos.begin(); fPionPosIter != fPionPos.end(); ++fPionPosIter) {

								// * Get Kalman tracks
									RecMdcKalTrack *KmTrk1 = (*fPionNeg1Iter)->mdcKalTrack();
									RecMdcKalTrack *KmTrk2 = (*fPionNeg2Iter)->mdcKalTrack();
									RecMdcKalTrack *KpTrk  = (*fKaonPosIter )->mdcKalTrack();
									RecMdcKalTrack *pipTrk = (*fPionPosIter )->mdcKalTrack();

								// * Get W-tracks
									WTrackParameter wvKmTrk1(gM_K,  KmTrk1->getZHelix(), KmTrk1->getZError());
									WTrackParameter wvKmTrk2(gM_K,  KmTrk2->getZHelix(), KmTrk2->getZError());
									WTrackParameter wvKpTrk (gM_K,  KpTrk->getZHelix(),  KpTrk->getZError());
									WTrackParameter wvpipTrk(gM_pi, pipTrk->getZHelix(), pipTrk->getZError());

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
									kkmfit->AddFourMomentum(0, gEcmsVec); // 4 constraints: CMS energy and momentum
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


		// * rhopi_pipigg without PID * //
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


			int npip = ipip.size();
			int npim = ipim.size();
			if(npip*npim != 1) return SUCCESS;


		// * Loop over each gamma pair and store total energy * //
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
			if(fElectronst4C) {
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
					}
				}
			}


		// * Apply Kalman kinematic fit * //
			if(fElectronst5C) {
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

				fLog << MSG::INFO << " chisq = " << chisq << endmsg;

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


						// * Measure the photon detection efficiences via J/psi -> rho0 pi0 * //
						if(fabs(prho0.m() - mrho0) < fDeltaMrho0) {
							if(fabs(fcos) < 0.99) {
								// * WRITE photon detection efficiency info ("geff" branch) *
								fFcos = (eg1-eg2) / ppi0.rho();  // E/p ratio for pi^0 candidate
								fElow = (eg1 < eg2) ? eg1 : eg2; // lowest energy of the two gammas
								fTuplePhoton->write(); // "geff" branch
							}
						} // rho0 cut
					}
				}
			}


		return StatusCode::SUCCESS;
	}


	/**
	 * @brief Inherited `finalize` method of `Algorithm`. This function is only called once, after running over all events.
	 * @details Prints the flow chart to the terminal, so make sure you save this output!
	 */
	StatusCode rhopi_pipigg::finalize_rest()
	{
		return StatusCode::SUCCESS;
	}



// * =============================== * //
// * ------- PRIVATE METHODS ------- * //
// * =============================== * //

	/**
	 * @brief Helper function that encapsulates the proces of finding the angle between some photon `
	 */
	void rhopi_pipigg::SetSmallestAngles(std::vector<EvtRecTrack*>::iterator &iter, std::vector<EvtRecTrack*> &vec, Hep3Vector &emcpos)
	{
		for(iter = vec.begin(); iter != vec.end(); ++iter) {
			// * Get the extension object from MDC to EMC
				if(!(*iter)->isExtTrackValid()) continue;
				RecExtTrack *extTrk = (*iter)->extTrack();
				if(extTrk->emcVolumeNumber() == -1) continue;
				Hep3Vector extpos(extTrk->emcPosition());

			// * Get angles
			// double cosTheta = extpos.cosTheta(emcpos);
			double angle  = extpos.angle(emcpos);
			double dTheta = extpos.theta() - emcpos.theta();
			double dPhi   = extpos.deltaPhi(emcpos);
			dTheta = fmod(dTheta + CLHEP::twopi + CLHEP::twopi + pi, CLHEP::twopi) - CLHEP::pi;
			dPhi   = fmod(dPhi   + CLHEP::twopi + CLHEP::twopi + pi, CLHEP::twopi) - CLHEP::pi;
			if(angle < fSmallestAngle){
				fSmallestAngle = angle;
				fSmallestTheta = dTheta;
				fSmallestPhi   = dPhi;
			}
		}
	}