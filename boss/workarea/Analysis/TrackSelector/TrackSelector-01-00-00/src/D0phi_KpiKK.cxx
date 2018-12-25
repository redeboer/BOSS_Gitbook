// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "CLHEP/Geometry/Point3D.h"
	#include "CLHEP/Vector/LorentzVector.h"
	#include "CLHEP/Vector/ThreeVector.h"
	#include "CLHEP/Vector/TwoVector.h"
	#include "TrackSelector/D0phi_KpiKK.h"
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
	const double gM_K   = 0.493677;   /// Mass of \f$K^\pm\f$.
	const double gM_D0  = 1.86483;    /// Mass of \f$D^0\f$.
	const double gM_phi = 1.019461;   /// Mass of \f$\phi\f$.
	const double gEcms  = 3.097;      /// Center-of-mass energy.
	const HepLorentzVector gEcmsVec(0.034, 0, 0, gEcms);



// * =========================== * //
// * ------- CONSTRUCTOR ------- * //
// * =========================== * //

	/**
	 * @brief Constructor for the `D0phi_KpiKK` algorithm.
	 * @details Here, you should declare properties: give them a name, assign a parameter (data member of `D0phi_KpiKK`), and if required a documentation string. Note that you should define the paramters themselves in the header (D0phi_KpiKK/D0phi_KpiKK.h) and that you should assign the values in `share/D0phi_KpiKK.txt`.
	 */
	D0phi_KpiKK::D0phi_KpiKK(const std::string &name, ISvcLocator* pSvcLocator) :
		TrackSelector(name, pSvcLocator)
	{

		// * Whether or not to fill a tree/NTuple *
		declareProperty("do_fit4c", fDo_fit4c);
		declareProperty("do_fit6c", fDo_fit6c);

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
	StatusCode D0phi_KpiKK::initialize_rest()
	{

		// * Book NTuple: dE/dx PID branch * //
			if(fDo_dedx) {
				BookNtupleItemsDedx("dedx_K", fDedx_K);
				BookNtupleItemsDedx("dedx_pi", fDedx_pi);
			}

		// * Book NTuple: 4-contraints for Kalman kinematic fit * //
			if(fDo_fit4c) {
				fFit4c["mD0"];   /// Invariant mass for \f$K^- \pi^+\f$ (\f$D^0\f$).
				fFit4c["mphi"];  /// Invariant mass for \f$K^+ K^+  \f$ (\f$\phi\f$).
				fFit4c["mJpsi"];   /// Invariant mass for \f$D^0 \phi \f$ (\f$J/\psi\f$).
				fFit4c["chisq"];   /// Chi squared of the Kalman kinematic fit.
				AddItemsToNTuples("fit4c", fFit4c);
			}

		// * Book NTuple: 6-contraints for Kalman kinematic fit * //
			if(fDo_fit6c) {
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
	StatusCode D0phi_KpiKK::execute_rest()
	{

		// * STEP (A): Create selection charged tracks * //

			// * Print log and set counters *
				fLog << MSG::DEBUG << "Starting 'good' charged track selection:" << endmsg;
				ParticleID *pid = ParticleID::instance();

			// * Clear vectors of selected particles *
				fKaonNeg.clear();
				fKaonPos.clear();
				fPionPos.clear();

			// * Loop over charged tracks *
			for(fTrackIterator = fGoodChargedTracks.begin(); fTrackIterator != fGoodChargedTracks.end(); ++fTrackIterator) {

				// * STEP A.1: Initialise PID

					// * Initialise PID sub-system and set method: probability, likelihood, or neuron network
					pid->init();
					pid->setMethod(pid->methodProbability());
					// pid->setMethod(pid->methodLikelihood());
					// pid->setMethod(pid->methodNeuronNetwork());
					pid->setChiMinCut(4);
					pid->setRecTrack(*fTrackIterator);

					// * Choose ID system and which particles to use
					pid->usePidSys(pid->useDedx() | pid->useTof1() | pid->useTof2() | pid->useTofE()); // use PID sub-system
					pid->identify(pid->onlyPion() | pid->onlyKaon()); // seperater Pion/Kaon

					// * Perform PID
					pid->calculate();
					if(!(pid->IsPidInfoValid())) continue;

				// * STEP A.2: Identify type of charged particle and add to related vector (this package: kaon and pion)
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

				// * STEP A.3: WRITE PID information
					if(fDo_pid) WritePIDInformation(pid);

			}

		// * STEP (B): WRITE dE/dx PID information ("dedx" branch) * //
			if(fDo_dedx) {
				WriteDedxInfoForVector(fKaonNeg, "dedx_K",  fDedx_K);
				WriteDedxInfoForVector(fKaonPos, "dedx_K",  fDedx_K);
				WriteDedxInfoForVector(fPionPos, "dedx_pi", fDedx_pi);
			}


		// * STEP (C): WRITE Kalman 4-constraint kinematic fit with smallest chi squared ("fit4c" branch) * //
			if(fDo_fit4c) {

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
					if(fDo_fit4c && bestKalmanFit && fSmallestChiSq < fMaxChiSq) {
						HepLorentzVector pD0   = bestKalmanFit->pfit(0) + bestKalmanFit->pfit(1);
						HepLorentzVector pphi  = bestKalmanFit->pfit(2) + bestKalmanFit->pfit(3);
						HepLorentzVector pJpsi = pD0 + pphi;
						fFit4c.at("mD0")   = pD0.m();
						fFit4c.at("mphi")  = pphi.m();
						fFit4c.at("mJpsi") = pJpsi.m();
						fFit4c.at("chisq") = fSmallestChiSq;
						fNTupleMap.at("fit4c")->write();
					}

			} // end of fDo_fit4c


		// * STEP (D): WRITE Kalman 6-constraint kinematic fit with smallest chi squared ("fit6c" branch) * //
			if(fDo_fit6c) {

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
									kkmfit->AddTrack(0, wKm1);           // K- (1st occurrence)
									kkmfit->AddTrack(1, wpip);           // pi+
									kkmfit->AddTrack(2, wKm2);           // K- (2nd occurrence)
									kkmfit->AddTrack(3, wKp);            // K+
									kkmfit->AddFourMomentum(2, gEcmsVec); // 4 constraints: CMS energy and momentum
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
			} // end of fDo_fit6c


		return StatusCode::SUCCESS;
	}


	/**
	 * @brief Currently does nothing. Cut flow could be printed in this step.
	 * @todo Add log output to `finalize` step.
	 */
	StatusCode D0phi_KpiKK::finalize_rest()
	{
		return StatusCode::SUCCESS;
	}