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
	using namespace TSGlobals;



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
		fLog << MSG::DEBUG << "===>> D0phi_KpiKK::D0phi_KpiKK() <<===" << endmsg;

		/// * The `"do_<treename>"` properties determine whether or not the corresponding `TTree`/`NTuple` will be filled. Default values are set in the constructor as well.
		declareProperty("do_fit4c_all",  fDo_fit4c_all  = false);
		declareProperty("do_fit4c_best", fDo_fit4c_best = false);

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
		fLog << MSG::INFO << "===>> D0phi_KpiKK::initialize_rest() <<===" << endmsg;

		/// <ol type="A">
		/// <li> `"mult_select"`: Multiplicities of selected particles
			/// <ol>
			if(fDo_mult_select) {
				fMap_mult_select["NKaonPos"]; /// <li> `"NKaonPos"`: Number of \f$K^+\f$.
				fMap_mult_select["NKaonNeg"]; /// <li> `"NKaonNeg"`: Number of \f$K^-\f$.
				fMap_mult_select["NPionPos"]; /// <li> `"NPionPos"`: Number of \f$\pi^-\f$.
				AddItemsToNTuples("mult_select", fMap_mult_select, "Multipliciies of selected particles");
			}
			/// </ol>

		/// <li> `"dedx_K"` and `"dedx_pi"`: energy loss \f$dE/dx\f$ PID branch. See `TrackSelector::BookNtupleItemsDedx` for more info.
			if(fDo_dedx) {
				BookNtupleItemsDedx("dedx_K",  fMap_dedx_K);
				BookNtupleItemsDedx("dedx_pi", fMap_dedx_pi);
			}

		/// <li> `"fit4c_all"`: results of the 4-contraints Kalman kinematic fit <i>for all combinations</i>
			/// <ol>
			if(fDo_fit4c_all) {
				fMap_fit4c_all["mD0"];   /// <li> `"mD0"`:   Invariant mass for \f$K^- \pi^+\f$ (\f$D^0\f$).
				fMap_fit4c_all["mphi"];  /// <li> `"mphi"`:  Invariant mass for \f$K^+ K^+  \f$ (\f$\phi\f$).
				fMap_fit4c_all["mJpsi"]; /// <li> `"mJpsi"`: Invariant mass for \f$D^0 \phi \f$ (\f$J/\psi\f$).
				fMap_fit4c_all["chisq"]; /// <li> `"chisq"`: \f$\chi^2\f$ of the Kalman kinematic fit.
				fMap_fit4c_all["relMassDiff"]; /// <li> `"relMassDiff"`: Measure for the best kinematic Kalman fit (see `D0phi_KpiKK::MeasureForBestFit`).
				AddItemsToNTuples("fit4c_all", fMap_fit4c_all, "4-constraint fit information (CMS 4-momentum)");
			}
			/// </ol>

		/// <li> `"fit4c_best"`: results of the 4-contraints Kalman kinematic fit for the combination <i>with the invariant mass closest to the reconstructed particles</i>
			/// <ol>
			if(fDo_fit4c_best) {
				fMap_fit4c_best["mD0"];   /// <li> `"mD0"`:   Invariant mass for \f$K^- \pi^+\f$ (\f$D^0\f$).
				fMap_fit4c_best["mphi"];  /// <li> `"mphi"`:  Invariant mass for \f$K^+ K^+  \f$ (\f$\phi\f$).
				fMap_fit4c_best["mJpsi"]; /// <li> `"mJpsi"`: Invariant mass for \f$D^0 \phi \f$ (\f$J/\psi\f$).
				fMap_fit4c_best["chi2"];  /// <li> `"chisq"`: \f$\chi^2\f$ of the Kalman kinematic fit.
				fMap_fit4c_best["relMassDiff"]; /// <li> `"relMassDiff"`: Measure for the best kinematic Kalman fit (see `D0phi_KpiKK::MeasureForBestFit`).
				AddItemsToNTuples("fit4c_best", fMap_fit4c_best, "4-constraint fit information of the invariant masses closest to the reconstructed particles");
			}
			/// </ol>

		/// </ol>
		fLog << MSG::INFO << "Successfully returned from initialize()" << endmsg;
		return StatusCode::SUCCESS;
	}


	/**
	 * @brief Inherited `execute` method of the `Algorithm`. This function is called *for each event*.
	 */
	StatusCode D0phi_KpiKK::execute_rest()
	{
		fLog << MSG::DEBUG << "===>> D0phi_KpiKK::execute_rest() <<===" << endmsg;

		/// <ol type="A">
		/// <li> Create selection charged tracks

			// * Print log and set counters *
				fLog << MSG::DEBUG << "Starting particle selection:" << endmsg;
				fPIDInstance = ParticleID::instance();

			// * Clear vectors of selected particles *
				fKaonNeg.clear();
				fKaonPos.clear();
				fPionPos.clear();

			// * Loop over charged tracks *
			for(fTrackIterator = fGoodChargedTracks.begin(); fTrackIterator != fGoodChargedTracks.end(); ++fTrackIterator) {
				/// <ol>
				/// <li> Initialise PID and skip if it fails:
					/// <ul>
					if(!InitializePID(
						Probability,
							/// <li> use <b>probability method</b>
						fPIDInstance->useDedx() | fPIDInstance->useTof1() | fPIDInstance->useTof2() | fPIDInstance->useTofE(),
							/// <li> use \f$dE/dx\f$ and the three ToF detectors
						fPIDInstance->onlyPion() | fPIDInstance->onlyKaon(),
							/// <li> identify only pions and kaons
						4.0
							/// <li> use \f$\chi^2 > 4.0\f$
					)) continue;
					/// </ul>

				/// <li> <b>Write</b> Particle Identification information of all tracks
					if(fDo_PID) WritePIDInformation();

				/// <li> Identify type of charged particle and add to related vector: (this package: kaon and pion).
					RecMdcKalTrack* mdcKalTrk = (*fTrackIterator)->mdcKalTrack(); /// `RecMdcKalTrack` (Kalman) is used, but this can be substituted by `RecMdcTrack`.
					if(fPIDInstance->probPion() > fPIDInstance->probKaon()) { /// The particle identification first decides whether the track is more likely to have come from a pion or from a kaon.
						if(fPIDInstance->probPion() < fCut_MinPIDProb) continue; /// A cut is then applied on whether the probability to be a pion (or kaon) is at least `fCut_MinPIDProb` (see eventual settings in `D0phi_KpiKK.txt`).
						if(fPIDInstance->pdf(RecMdcKalTrack::pion) < fPIDInstance->pdf(RecMdcKalTrack::kaon)) continue; /// If, according to the likelihood method, the particle is still more likely to be a kaon than a pion, the track is rejected.
						RecMdcKalTrack::setPidType(RecMdcKalTrack::pion); /// Finally, the particle ID of the `RecMdcKalTrack` object is set to pion
						if(mdcKalTrk->charge() > 0) fPionPos.push_back(*fTrackIterator); /// and the (positive) pion is added to the vector of pions.
					}
					else {
						if(fPIDInstance->probKaon() < fCut_MinPIDProb) continue;
						if(fPIDInstance->pdf(RecMdcKalTrack::kaon) < fPIDInstance->pdf(RecMdcKalTrack::pion)) continue;
						RecMdcKalTrack::setPidType(RecMdcKalTrack::kaon);
						if(mdcKalTrk->charge() < 0) fKaonNeg.push_back(*fTrackIterator);
						else                        fKaonPos.push_back(*fTrackIterator);
					}

				/// </ol>
			}

			// * Finish good photon selection *
			fLog << MSG::DEBUG << "Number of K+:  " << fKaonNeg.size() << endmsg;
			fLog << MSG::DEBUG << "Number of K+:  " << fKaonPos.size() << endmsg;
			fLog << MSG::DEBUG << "Number of K+:  " << fPionPos.size() << endmsg;


		/// <li> <b>Write</b> the multiplicities of the selected particles.
			if(fDo_mult_select) {
				fMap_mult_select.at("NKaonNeg") = fKaonNeg.size();
				fMap_mult_select.at("NKaonPos") = fKaonPos.size();
				fMap_mult_select.at("NPionPos") = fPionPos.size();
				fNTupleMap.at("mult_select")->write();
			}


		/// <li> Apply a strict cut on the number of particles: <i>only 2 negative kaons, 1 positive kaon, and 1 positive pion</i>
			if(fKaonNeg.size() != 2) return StatusCode::SUCCESS;
			if(fKaonPos.size() != 1) return StatusCode::SUCCESS;
			if(fPionPos.size() != 1) return StatusCode::SUCCESS;


		/// <li> <b>Write</b> \f$dE/dx\f$ PID information (`"dedx"` branch)
			if(fDo_dedx) {
				WriteDedxInfoForVector(fKaonNeg, "dedx_K",  fMap_dedx_K);
				WriteDedxInfoForVector(fKaonPos, "dedx_K",  fMap_dedx_K);
				WriteDedxInfoForVector(fPionPos, "dedx_pi", fMap_dedx_pi);
			}


		/// <li> Perform Kalman 4-constraint Kalman kinematic fit for all combinations and decide the combinations that results in the 'best' result. The 'best' result is defined as the combination that has the smallest value of:
			/// \f[
			/// 	\left(\frac{m_{K^-\pi^+}-m_{D^0} }{m_{D^0} }\right) \cdot
			/// 	\left(\frac{m_{K^-K^+}  -m_{\phi}}{m_{\phi}}\right).
			/// \f]
			/// See `D0phi_KpiKK::MeasureForBestFit` for the definition of this measure.
			if(fDo_fit4c_all || fDo_fit4c_best) {
				double bestMeasure = 1e5;
				KalmanKinematicFit *bestKalmanFit = nullptr;
				for(fKaonNeg1Iter = fKaonNeg.begin(); fKaonNeg1Iter != fKaonNeg.end(); ++fKaonNeg1Iter)
				for(fKaonNeg2Iter = fKaonNeg1Iter+1;  fKaonNeg2Iter != fKaonNeg.end(); ++fKaonNeg2Iter)
				for(fKaonPosIter  = fKaonPos.begin(); fKaonPosIter != fKaonPos.end();  ++fKaonPosIter)
				for(fPionPosIter  = fPionPos.begin(); fPionPosIter != fPionPos.end();  ++fPionPosIter)
				{

					// * Get Kalman tracks reconstructed by the MDC
						RecMdcKalTrack* kalTrkKm1 = (*fKaonNeg1Iter)->mdcKalTrack();
						RecMdcKalTrack* kalTrkKm2 = (*fKaonNeg2Iter)->mdcKalTrack();
						RecMdcKalTrack* kalTrkKp  = (*fKaonPosIter) ->mdcKalTrack();
						RecMdcKalTrack* kalTrkpip = (*fPionPosIter) ->mdcKalTrack();

					// * Get W-tracks
						WTrackParameter wvKmTrk1(gM_K,  kalTrkKm1->getZHelix(), kalTrkKm1->getZError());
						WTrackParameter wvKmTrk2(gM_K,  kalTrkKm2->getZHelix(), kalTrkKm2->getZError());
						WTrackParameter wvKpTrk (gM_K,  kalTrkKp ->getZHelix(), kalTrkKp ->getZError());
						WTrackParameter wvpipTrk(gM_pi, kalTrkpip->getZHelix(), kalTrkpip->getZError());

					// * Test vertex fit
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
						KalmanKinematicFit *kkmfit = KalmanKinematicFit::instance();
						kkmfit->init();
						kkmfit->AddTrack(0, vtxfit->wtrk(0)); // K- (1st occurrence)
						kkmfit->AddTrack(1, vtxfit->wtrk(1)); // pi+
						kkmfit->AddTrack(2, vtxfit->wtrk(2)); // K- (2nd occurrence)
						kkmfit->AddTrack(3, vtxfit->wtrk(3)); // K+
						kkmfit->AddFourMomentum(0, gEcmsVec); // 4 constraints: CMS energy and 3-momentum
						if(kkmfit->Fit()) {
							/// Apply max. \f$\chi^2\f$ cut (determined by `fCut_MaxPIDChiSq`).
							if(kkmfit->chisq() > fCut_MaxPIDChiSq) continue;
							/// Compute the measure for the best Kalman kinematic fit and keep a pointer to this result if better than previous.
							ComputeInvariantMasses(kkmfit);
							if(MeasureForBestFit() < bestMeasure) bestKalmanFit = kkmfit;
							/// <b>Write</b> results of the Kalman kinematic fit (all combinations, `"fit4c_all"`).
							if(fDo_fit4c_all) WriteFitResults(kkmfit, fMap_fit4c_all, "fit4c_all");
						}

				}


		/// <li> <b>Write</b> results of the Kalman kitematic fit <i>of the best combination</i> (`"fit4c_best"` branch)
				if(fDo_fit4c_best) {
					if(bestKalmanFit) {
						ComputeInvariantMasses(bestKalmanFit);
						WriteFitResults(bestKalmanFit, fMap_fit4c_best, "fit4c_best");
					}
				}

			} // end of fDo_fit4c_all || fDo_fit4c_best


		/// </ol>
		return StatusCode::SUCCESS;
	}


	/**
	 * @brief Currently does nothing. Cut flow could be printed in this step.
	 * @todo Add log output to `finalize` step.
	 */
	StatusCode D0phi_KpiKK::finalize_rest()
	{
		fLog << MSG::INFO << "===>> D0phi_KpiKK::finalize_rest() <<===" << endmsg;

		return StatusCode::SUCCESS;
	}



// * =============================== * //
// * ------- PRIVATE METHODS ------- * //
// * =============================== * //

	/**
	 * @brief Method that provides a measure for the best Kalman kinematic fit.
	 * @details The closer this value to zero, the better the result.
	 */
	double D0phi_KpiKK::MeasureForBestFit()
	{
		return ((fM_D0-gM_D0)/gM_D0) * ((fM_phi-gM_phi)/gM_phi);
	}


	/**
	 * @brief Encapsulation of the procedure to compute invariant masses of the reconstructed particles.
	 */
	void D0phi_KpiKK::ComputeInvariantMasses(KalmanKinematicFit *kkmfit)
	{
		HepLorentzVector pD0   = kkmfit->pfit(0) + kkmfit->pfit(1); // K^- pi^+
		HepLorentzVector pphi  = kkmfit->pfit(2) + kkmfit->pfit(3); // K^- K^+
		HepLorentzVector pJpsi = pD0 + pphi;                        // D^0 phi
		fM_D0   = pD0.m();
		fM_phi  = pphi.m();
		fM_Jpsi = pJpsi.m();
	}


	/**
	 * @brief Encapsulation of the procedure to write results of the Kalman kinematic fit (no matter how many constrains).
	 */
	void D0phi_KpiKK::WriteFitResults(KalmanKinematicFit *kkmfit, std::map<std::string, NTuple::Item<double> > &map, const char *tupleName)
	{
		fLog << MSG::DEBUG << "Writing fit results \"" << tupleName << "\"" << endmsg;
		map.at("mD0")   = fM_D0;
		map.at("mphi")  = fM_phi;
		map.at("mJpsi") = fM_Jpsi;
		map.at("chisq") = kkmfit->chisq();
		map.at("relMassDiff") = MeasureForBestFit();
		fNTupleMap.at(tupleName)->write();
	}


	/**
	 * @brief This function encapsulates the `addItem` procedure for the fit branches.
	 */ 
	void D0phi_KpiKK::BookNtupleItemsFit(const char* tupleName, std::map<std::string, NTuple::Item<double> > &map, const char* tupleTitle)
	{
		/// <ol>
		map["mD0"];   /// <li> `"mD0"`:   Invariant mass for \f$K^- \pi^+\f$ (\f$D^0\f$).
		map["mphi"];  /// <li> `"mphi"`:  Invariant mass for \f$K^+ K^+  \f$ (\f$\phi\f$).
		map["mJpsi"]; /// <li> `"mJpsi"`: Invariant mass for \f$D^0 \phi \f$ (\f$J/\psi\f$).
		map["chisq"]; /// <li> `"chisq"`: \f$\chi^2\f$ of the Kalman kinematic fit.
		AddItemsToNTuples(tupleName, map, tupleTitle);
		/// </ol>
	}