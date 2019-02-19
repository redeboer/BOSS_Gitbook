// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "CLHEP/Geometry/Point3D.h"
	#include "CLHEP/Vector/LorentzVector.h"
	#include "CLHEP/Vector/ThreeVector.h"
	#include "CLHEP/Vector/TwoVector.h"
	#include "D0phi_KpiKK/D0phi_KpiKK.h"
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

		/// * The `"write_<treename>"` properties determine whether or not the corresponding `TTree`/`NTuple` will be filled. Default values are set in the constructor as well.
		declareProperty("write_fit4c_all",  fWrite_fit4c_all  = false);
		declareProperty("write_fit4c_best", fWrite_fit4c_best = false);
		declareProperty("write_fit_mc",     fWrite_fit_mc     = false);

	}



// * =============================== * //
// * ------- ALGORITHM STEPS ------- * //
// * =============================== * //

	/**
	 * @brief   (Inherited) `initialize` step of `Algorithm`. This function is called only once, when the algorithm is initialised.
	 * @details Define and load `NTuple`s here. Other `NTuple`s have already been defined in the `TrackSelector::initilize` step prior to this this method.
	 */
	StatusCode D0phi_KpiKK::initialize_rest()
	{
		fLog << MSG::INFO << "===>> D0phi_KpiKK::initialize_rest() <<===" << endmsg;

		/// <ol type="A">
		/// <li> `"mult_select"`: Multiplicities of selected particles
			/// <ol>
			if(fWrite_mult_select) {
				fMap_mult_select["NKaonPos"]; /// <li> `"NKaonPos"`: Number of \f$K^+\f$.
				fMap_mult_select["NKaonNeg"]; /// <li> `"NKaonNeg"`: Number of \f$K^-\f$.
				fMap_mult_select["NPionPos"]; /// <li> `"NPionPos"`: Number of \f$\pi^-\f$.
				AddItemsToNTuples("mult_select", fMap_mult_select, "Multipliciies of selected particles");
			}
			/// </ol>

		/// <li> `"dedx_K"` and `"dedx_pi"`: energy loss \f$dE/dx\f$ PID branch. See `TrackSelector::BookNtupleItems_Dedx` for more info.
			if(fWrite_dedx) {
				BookNtupleItems_Dedx("dedx_K",  fMap_dedx_K);
				BookNtupleItems_Dedx("dedx_pi", fMap_dedx_pi);
			}

		/// <li> `"fit4c_*"`: results of the Kalman kinematic fit results. See `TrackSelector::BookNtupleItems_Fit` for more info.
			if(fWrite_fit4c_all)  BookNtupleItems_Fit("fit4c_all",  fMap_fit4c_all,  "4-constraint fit information (CMS 4-momentum)");
			if(fWrite_fit4c_best) BookNtupleItems_Fit("fit4c_best", fMap_fit4c_best, "4-constraint fit information of the invariant masses closest to the reconstructed particles");
			if(fWrite_fit_mc) BookNtupleItems_Fit("fit_mc", fMap_fit_mc, "Fake fit information according to MC truth");

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
						/// <li> use <b>probability method</b>
						fPIDInstance->methodProbability(),
						/// <li> use \f$dE/dx\f$ and the three ToF detectors. Since BOSS 7.0.4, `ParticleID::useTofCorr()` should be used for ToF instead of e.g. `useTof1`.
						fPIDInstance->useDedx() |
						fPIDInstance->useTofCorr(),
						/// <li> identify only pions and kaons
						fPIDInstance->onlyPion() |
						fPIDInstance->onlyKaon(),
						/// <li> use \f$\chi^2 > 4.0\f$
						4.0
					)) continue;
					/// </ul>

				/// <li> <b>Write</b> Particle Identification information of all tracks
					if(fWrite_PID) WritePIDInformation();

				/// <li> Identify type of charged particle and add to related vector: (this package: kaon and pion).
					fTrackKal = (*fTrackIterator)->mdcKalTrack();
					if(fPIDInstance->probPion() > fPIDInstance->probKaon()) { /// The particle identification first decides whether the track is more likely to have come from a pion or from a kaon.
						// if(fPIDInstance->pdf(RecMdcKalTrack::pion) < fPIDInstance->pdf(RecMdcKalTrack::kaon)) continue; /// If, according to the likelihood method, the particle is still more likely to be a kaon than a pion, the track is rejected.
						if(fCut_PIDProb.FailsMin(fPIDInstance->probPion())) continue; /// A cut is then applied on whether the probability to be a pion (or kaon) is at least `fCut_PIDProb_min` (see eventual settings in `D0phi_KpiKK.txt`).
						RecMdcKalTrack::setPidType(RecMdcKalTrack::pion); /// Finally, the particle ID of the `RecMdcKalTrack` object is set to pion
						if(fTrackKal->charge() > 0) fPionPos.push_back(*fTrackIterator); /// and the (positive) pion is added to the vector of pions.
					}
					else {
						// if(fPIDInstance->pdf(RecMdcKalTrack::kaon) < fPIDInstance->pdf(RecMdcKalTrack::pion)) continue;
						if(fCut_PIDProb.FailsMin(fPIDInstance->probKaon())) continue;
						RecMdcKalTrack::setPidType(RecMdcKalTrack::kaon);
						if(fTrackKal->charge() < 0) fKaonNeg.push_back(*fTrackIterator);
						else                        fKaonPos.push_back(*fTrackIterator);
					}

				/// </ol>
			}

			// * Finish good photon selection *
			fLog << MSG::DEBUG
				<< "N_{K^-}  = "  << fKaonNeg.size() << ", "
				<< "N_{K^+}  = "  << fKaonPos.size() << ", "
				<< "N_{\pi^+} = " << fPionPos.size() << endmsg;


		/// <li> <b>Write</b> the multiplicities of the selected particles.
			if(fWrite_mult_select) {
				fMap_mult_select.at("NKaonNeg") = fKaonNeg.size();
				fMap_mult_select.at("NKaonPos") = fKaonPos.size();
				fMap_mult_select.at("NPionPos") = fPionPos.size();
				fNTupleMap.at("mult_select")->write();
			}


		/// <li> Make selection of MC truth particles by looping over the collection of MC particles created in `TrackSelector::execute()`. See <a href="http://home.fnal.gov/~mrenna/lutp0613man2/node44.html">here</a> for a list of PDG codes.
		if(fEventHeader->runNumber()<0 && fWrite_fit_mc) {
			std::vector<Event::McParticle*>::iterator it;
			fMcKaonNeg.clear();
			fMcKaonPos.clear();
			fMcPionPos.clear();
			for(it=fMcParticles.begin(); it!=fMcParticles.end(); ++it) {
				switch((*it)->particleProperty()) {
					case -321 : fMcKaonNeg.push_back(*it); break;
					case  321 : fMcKaonPos.push_back(*it); break;
					case  211 : fMcPionPos.push_back(*it); break;
					default : fLog << MSG::DEBUG << "No switch case defined for McParticle " << (*it)->particleProperty() << endmsg;
				}
			} // end of loop over MC particles



		/// <li> Apply a strict cut on the number of particles: <i>only 2 negative kaons, 1 positive kaon, and 1 positive pion</i>
			if(fMcKaonNeg.size() == 2 && fMcKaonPos.size() == 1 && fMcPionPos.size() == 1)

		/// <li> Loop over MC truth of final decay products.
			for(fMcKaonNeg1Iter = fMcKaonNeg.begin(); fMcKaonNeg1Iter != fMcKaonNeg.end(); ++fMcKaonNeg1Iter)
			for(fMcKaonNeg2Iter = fMcKaonNeg.begin(); fMcKaonNeg2Iter != fMcKaonNeg.end(); ++fMcKaonNeg2Iter)
			for(fMcKaonPosIter  = fMcKaonPos.begin(); fMcKaonPosIter  != fMcKaonPos.end(); ++fMcKaonPosIter)
			for(fMcPionPosIter  = fMcPionPos.begin(); fMcPionPosIter  != fMcPionPos.end(); ++fMcPionPosIter)
			{
				/// <ol>
				/// <li> Only continue if the two kaons are different.
					if(fMcKaonNeg1Iter == fMcKaonNeg2Iter) continue;

				/// <li> Check topology: only consider that combination which comes from \f$J/\psi \rightarrow D^0\phi \rightarrow K^-\pi^+ K^-K^+\f$.
					if((!(*fMcKaonNeg1Iter)->primaryParticle()) && (*fMcKaonNeg1Iter)->mother().particleProperty() != 333) continue; // mother phi
					if((!(*fMcKaonNeg2Iter)->primaryParticle()) && (*fMcKaonNeg2Iter)->mother().particleProperty() != 421) continue; // mother D0
					if((!(*fMcKaonPosIter) ->primaryParticle()) && (*fMcKaonPosIter) ->mother().particleProperty() != 333) continue; // mother phi
					if((!(*fMcPionPosIter) ->primaryParticle()) && (*fMcPionPosIter) ->mother().particleProperty() != 421) continue; // mother D0
				/// <li> Write 'fake' fit results, that is, momenta of the particles reconstructed from MC truth.
					KKFitResult_D0phi_KpiKK fitresult(
						*fMcKaonNeg1Iter,
						*fMcKaonNeg2Iter,
						*fMcKaonPosIter,
						*fMcPionPosIter
					);
					WriteFitResults(fitresult, fMap_fit_mc, "fit_mc");
				/// </ol>
			} // end of loop over particle combinations
		}


		/// <li> <b>Write</b> \f$dE/dx\f$ PID information (`"dedx"` branch)
			if(fWrite_dedx) {
				WriteDedxInfoForVector(fKaonNeg, "dedx_K",  fMap_dedx_K);
				WriteDedxInfoForVector(fKaonPos, "dedx_K",  fMap_dedx_K);
				WriteDedxInfoForVector(fPionPos, "dedx_pi", fMap_dedx_pi);
			}


		/// <li> Perform Kalman 4-constraint Kalman kinematic fit for all combinations and decide the combinations that results in the 'best' result. The 'best' result is defined as the combination that has the smallest value of: \f$m_{K^-K^+}-m_{\phi}\f$ (that is the combination for which the invariant mass of the \f$K^-\pi^+\f$ is closest to \f$\phi\f$). See `D0phi_KpiKK::MeasureForBestFit` for the definition of this measure.
			if(fWrite_fit4c_all || fWrite_fit4c_best) {
				// * Reset best fit parameters * //
				KKFitResult_D0phi_KpiKK bestKalmanFit;
				bestKalmanFit.fBestCompareValue     = 1e9;
				// * Loop over all combinations of K-, K+, and pi+ * //
				for(fKaonNeg1Iter = fKaonNeg.begin(); fKaonNeg1Iter != fKaonNeg.end(); ++fKaonNeg1Iter)
				for(fKaonNeg2Iter = fKaonNeg.begin(); fKaonNeg2Iter != fKaonNeg.end(); ++fKaonNeg2Iter)
				for(fKaonPosIter  = fKaonPos.begin(); fKaonPosIter  != fKaonPos.end(); ++fKaonPosIter)
				for(fPionPosIter  = fPionPos.begin(); fPionPosIter  != fPionPos.end(); ++fPionPosIter)
				{
					if(fKaonNeg1Iter == fKaonNeg2Iter) continue;
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
						vtxfit->AddTrack(0, wvKmTrk1);
						vtxfit->AddTrack(1, wvKmTrk2);
						vtxfit->AddTrack(2, wvKpTrk);
						vtxfit->AddTrack(3, wvpipTrk);
						vtxfit->AddVertex(0, vxpar, 0, 1);
						if(!vtxfit->Fit(0)) continue;
						vtxfit->Swim(0);

					// * Get Kalman kinematic fit for this combination and store if better than previous one
						KalmanKinematicFit *kkmfit = KalmanKinematicFit::instance();
						kkmfit->init();
						kkmfit->AddTrack(0, vtxfit->wtrk(0)); // K- (1st occurrence)
						kkmfit->AddTrack(1, vtxfit->wtrk(1)); // K- (2nd occurrence)
						kkmfit->AddTrack(2, vtxfit->wtrk(2)); // K+
						kkmfit->AddTrack(3, vtxfit->wtrk(3)); // pi+
						kkmfit->AddFourMomentum(0, gEcmsVec); // 4 constraints: CMS energy and 3-momentum
						if(kkmfit->Fit()) {
							/// <ol>
							/// <li> Apply max. \f$\chi^2\f$ cut (determined by `fCut_PIDChiSq_max`).
							if(fCut_PIDChiSq.FailsMax(kkmfit->chisq())) continue;
							/// <li> <b>Write</b> results of the Kalman kinematic fit (all combinations, `"fit4c_all"`).
							KKFitResult_D0phi_KpiKK fitresult(kkmfit);
							if(fWrite_fit4c_all) WriteFitResults(fitresult, fMap_fit4c_all, "fit4c_all");
							/// <li> Decide if this fit is better than the previous
							if(fitresult.IsBetter()) bestKalmanFit = fitresult;
							/// </ol>
						}
				} // end of loop over particle combinations
		/// <li> <b>Write</b> results of the Kalman kitematic fit <i>of the best combination</i> (`"fit4c_best_*"` branches)
				if(fWrite_fit4c_best) WriteFitResults(bestKalmanFit, fMap_fit4c_best, "fit4c_best");

			} // end of fWrite_fit4c_*


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
	 * @brief This function encapsulates the `addItem` procedure for the fit branches.
	 */ 
	void D0phi_KpiKK::BookNtupleItems_Fit(const char* tupleName, std::map<std::string, NTuple::Item<double> > &map, const char* tupleTitle)
	{
		/// <ol>
		map["mD0"];   /// <li> `"mD0"`:   Invariant mass for \f$K^- \pi^+\f$ (\f$D^0\f$).
		map["mphi"];  /// <li> `"mphi"`:  Invariant mass for \f$K^+ K^+  \f$ (\f$\phi\f$).
		map["mJpsi"]; /// <li> `"mJpsi"`: Invariant mass for \f$D^0 \phi \f$ (\f$J/\psi\f$).
		map["pD0"];   /// <li> `"pD0"`:   3-momentum mass for the combination \f$K^- \pi^+\f$ (\f$D^0\f$ candidate).
		map["pphi"];  /// <li> `"pphi"`:  3-momentum mass for the combination \f$K^+ K^+  \f$ (\f$\phi\f$ candidate).
		map["chisq"]; /// <li> `"chisq"`: \f$\chi^2\f$ of the Kalman kinematic fit.
		AddItemsToNTuples(tupleName, map, tupleTitle);
		/// </ol>
	}