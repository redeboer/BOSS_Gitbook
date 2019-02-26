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
	#include <float.h> // for DBL_MAX
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


	/// Constructor for the `D0phi_KpiKK` algorithm, derived from `TrackSelector`.
	/// Here, you should declare properties: give them a name, assign a parameter (data member of `D0phi_KpiKK`), and if required a documentation string. Note that you should define the paramters themselves in the header (D0phi_KpiKK/D0phi_KpiKK.h) and that you should assign the values in `share/D0phi_KpiKK.txt`.
	D0phi_KpiKK::D0phi_KpiKK(const std::string &name, ISvcLocator* pSvcLocator) :
		/// * Construct base algorithm `TrackSelector`.
			TrackSelector(name, pSvcLocator),
		/// * Construct `NTuple::Tuple` containers used in derived classes.
			fNTuple_dedx_K    ("dedx_K",     "dE/dx of the kaons"),
			fNTuple_dedx_pi   ("dedx_pi",    "dE/dx of the pions"),
			fNTuple_fit4c_all ("fit4c_all",  "4-constraint fit information (CMS 4-momentum)"),
			fNTuple_fit4c_best("fit4c_best", "4-constraint fit information of the invariant masses closest to the reconstructed particles"),
			fNTuple_fit_mc    ("fit_mc",     "Fake fit information according to MC truth")
	{ PrintFunctionName("D0phi_KpiKK", __func__); PostConstructor();
		fCreateChargedCollection = true; /// @remark Set `fCreateChargedCollection` to `true` to ensure that the preselection of charged tracks is made. The neutral tracks are not necessary.
	}



// * =============================== * //
// * ------- ALGORITHM STEPS ------- * //
// * =============================== * //


	/// (Inherited) `initialize` step of `Algorithm`.
	/// This function is called only once, when the algorithm is initialised.
	/// @remark Define and load `NTuple`s here. Other `NTuple`s have already been defined in the `TrackSelector::initilize` step prior to this this method.
	StatusCode D0phi_KpiKK::initialize_rest()
	{ PrintFunctionName("D0phi_KpiKK", __func__);
		/// <ol type="A">
		/// <li> `"mult_select"`: Multiplicities of selected particles
			/// <ol>
			fNTuple_mult_sel.AddItem("NKaonPos"); /// <li> `"NKaonPos"`: Number of \f$K^+\f$.
			fNTuple_mult_sel.AddItem("NKaonNeg"); /// <li> `"NKaonNeg"`: Number of \f$K^-\f$.
			fNTuple_mult_sel.AddItem("NPionPos"); /// <li> `"NPionPos"`: Number of \f$\pi^-\f$.
			/// </ol>

		/// <li> `"dedx_K"` and `"dedx_pi"`: energy loss \f$dE/dx\f$ PID branch. See `TrackSelector::AddNTupleItems_Dedx` for more info.
			AddNTupleItems_Dedx(fNTuple_dedx_K);
			AddNTupleItems_Dedx(fNTuple_dedx_pi);

		/// <li> `"fit4c_*"`: results of the Kalman kinematic fit results. See `TrackSelector::AddNTupleItems_Fit` for more info.
			AddNTupleItems_Fit(fNTuple_fit4c_all);
			AddNTupleItems_Fit(fNTuple_fit4c_best);
			AddNTupleItems_Fit(fNTuple_fit_mc);

		/// </ol>
		return StatusCode::SUCCESS;
	}


	/// Inherited `execute` method of the `Algorithm`.
	/// This function is called *for each event*.
	StatusCode D0phi_KpiKK::execute_rest()
	{ PrintFunctionName("D0phi_KpiKK", __func__);
		/// <ol type="A">
		/// <li> Create specific charged track selections
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

				/// <li> @b Write Particle Identification information of all tracks
					WritePIDInformation();

				/// <li> Identify type of charged particle and add to related vector: (this package: kaon and pion).
					fTrackKal = (*fTrackIterator)->mdcKalTrack();
					if(fPIDInstance->probPion() > fPIDInstance->probKaon()) { /// The particle identification first decides whether the track is more likely to have come from a pion or from a kaon.
						// if(fPIDInstance->pdf(RecMdcKalTrack::pion) < fPIDInstance->pdf(RecMdcKalTrack::kaon)) continue; /// If, according to the likelihood method, the particle is still more likely to be a kaon than a pion, the track is rejected.
						if(fCut_PIDProb.FailsMin(fPIDInstance->probPion())) continue; /// A cut is then applied on whether the probability to be a pion (or kaon) is at least `fCut_PIDProb_min` (see eventual settings in `D0phi_KpiKK.txt`).
						RecMdcKalTrack::setPidType(RecMdcKalTrack::pion); /// Finally, the particle ID of the `RecMdcKalTrack` object is set to pion
						if(fTrackKal->charge()>0) fPionPos.push_back(*fTrackIterator); /// and the (positive) pion is added to the vector of pions.
					}
					else {
						// if(fPIDInstance->pdf(RecMdcKalTrack::kaon) < fPIDInstance->pdf(RecMdcKalTrack::pion)) continue;
						if(fCut_PIDProb.FailsMin(fPIDInstance->probKaon())) continue;
						RecMdcKalTrack::setPidType(RecMdcKalTrack::kaon);
						if(fTrackKal->charge()<0) fKaonNeg.push_back(*fTrackIterator);
						else                      fKaonPos.push_back(*fTrackIterator);
					}

				/// </ol>
			}


		/// <li> Create selection of MC truth particles by looping over the collection of MC particles created in `TrackSelector::execute()`. See <a href="http://home.fnal.gov/~mrenna/lutp0613man2/node44.html">here</a> for a list of PDG codes.
			if(fEventHeader->runNumber()<0 && fNTuple_fit_mc.DoWrite()) {
				fMcKaonNeg.clear();
				fMcKaonPos.clear();
				fMcPionPos.clear();
				std::vector<Event::McParticle*>::iterator it;
				for(it = fMcParticles.begin(); it != fMcParticles.end(); ++it) {
					switch((*it)->particleProperty()) {
						case -321 : fMcKaonNeg.push_back(*it); break;
						case  321 : fMcKaonPos.push_back(*it); break;
						case  211 : fMcPionPos.push_back(*it); break;
						default : fLog << MSG::DEBUG << "No switch case defined for McParticle " << (*it)->particleProperty() << endmsg;
					}
				}
			}


		/// <li> @b Write the multiplicities of the selected particles.
			fLog << MSG::DEBUG
				<< "N_{K^-} = "   << fKaonNeg.size() << ", "
				<< "N_{K^+} = "   << fKaonPos.size() << ", "
				<< "N_{\pi^+} = " << fPionPos.size() << endmsg;
			if(fNTuple_mult_sel.DoWrite()) {
				fNTuple_mult_sel.at("NKaonNeg") = fKaonNeg.size();
				fNTuple_mult_sel.at("NKaonPos") = fKaonPos.size();
				fNTuple_mult_sel.at("NPionPos") = fPionPos.size();
				fNTuple_mult_sel.Write();
			}


		/// <li> Apply a strict cut on the number of particles: <i>only 2 negative kaons, 1 positive kaon, and 1 positive pion</i>
			if(fKaonNeg.size() != 2) return StatusCode::SUCCESS;
			if(fKaonPos.size() != 1) return StatusCode::SUCCESS;
			if(fPionPos.size() != 1) return StatusCode::SUCCESS;
			if(fEventHeader->runNumber()<0 && fNTuple_fit_mc.DoWrite()) {
				if(
					fMcKaonNeg.size() != 2 || 
					fMcKaonPos.size() != 1 || 
					fMcPionPos.size() != 1
				) {
					fLog << MSG::DEBUG << "Number cut passed, but MC truth disagrees:" << endmsg;
					fLog << MSG::DEBUG << "  nMcKaonNeg = " << fMcKaonNeg.size() << endmsg;
					fLog << MSG::DEBUG << "  nMcKaonPos = " << fMcKaonPos.size() << endmsg;
					fLog << MSG::DEBUG << "  nMcPionPos = " << fMcPionPos.size() << endmsg;
					return StatusCode::SUCCESS;
				}
			}


		/// <li> @b Write Monte Carlo truth for `topoana` package
			fNTuple_mctruth.Write();


		/// <li> @b Write \f$dE/dx\f$ PID information (`"dedx_*"` branchs)
			WriteDedxInfoForVector(fKaonNeg, fNTuple_dedx_K);
			WriteDedxInfoForVector(fKaonPos, fNTuple_dedx_K);
			WriteDedxInfoForVector(fPionPos, fNTuple_dedx_pi);


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
					if(!IsDecay(*fMcKaonNeg2Iter, 421, -321)) continue; // D0  --> K-
					if(!IsDecay(*fMcPionPosIter,  421,  211)) continue; // D0  --> pi+
					if(!IsDecay(*fMcKaonNeg1Iter, 333, -321)) continue; // phi --> K-
					if(!IsDecay(*fMcKaonPosIter,  333,  321)) continue; // phi --> K+
				/// <li> Write 'fake' fit results, that is, momenta of the particles reconstructed from MC truth.
					KKFitResult_D0phi_KpiKK fitresult(
						*fMcKaonNeg1Iter,
						*fMcKaonNeg2Iter,
						*fMcKaonPosIter,
						*fMcPionPosIter
					);
					WriteFitResults(&fitresult, fNTuple_fit_mc);
				/// </ol>
			}


		/// <li> Perform Kalman 4-constraint Kalman kinematic fit for all combinations and decide the combinations that results in the 'best' result. The 'best' result is defined as the combination that has the smallest value of: \f$m_{K^-K^+}-m_{\phi}\f$ (that is the combination for which the invariant mass of the \f$K^-\pi^+\f$ is closest to \f$\phi\f$). See `D0phi_KpiKK::MeasureForBestFit` for the definition of this measure.
			if(fNTuple_fit4c_all.DoWrite() || fNTuple_fit4c_best.DoWrite()) {
				// * Reset best fit parameters * //
				KKFitResult_D0phi_KpiKK bestKalmanFit;
				bestKalmanFit.ResetBestCompareValue();
				// * Loop over all combinations //
				for(fKaonNeg1Iter = fKaonNeg.begin(); fKaonNeg1Iter != fKaonNeg.end(); ++fKaonNeg1Iter)
				for(fKaonNeg2Iter = fKaonNeg.begin(); fKaonNeg2Iter != fKaonNeg.end(); ++fKaonNeg2Iter)
				for(fKaonPosIter  = fKaonPos.begin(); fKaonPosIter  != fKaonPos.end(); ++fKaonPosIter)
				for(fPionPosIter  = fPionPos.begin(); fPionPosIter  != fPionPos.end(); ++fPionPosIter)
				{
					// * Only continue if we are not dealing with the same kaon
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

					// * Initiate vertex fit * //
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

					// * Test vertex fit * //
						VertexFit* vtxfit = VertexFit::instance();
						vtxfit->init();
						vtxfit->AddTrack(0, wvKmTrk1);
						vtxfit->AddTrack(1, wvKmTrk2);
						vtxfit->AddTrack(2, wvKpTrk);
						vtxfit->AddTrack(3, wvpipTrk);
						vtxfit->AddVertex(0, vxpar, 0, 1);
						if(!vtxfit->Fit(0)) {
							fLog << MSG::WARNING << "vertex fit failed" << endmsg;
							continue;
						}
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
							/// <li> Construct fit result object for this combintation.
							KKFitResult_D0phi_KpiKK fitresult(kkmfit);
							/// <li> @b Write results of the Kalman kinematic fit (all combinations, `"fit4c_all"`).
							WriteFitResults(&fitresult, fNTuple_fit4c_all);
							/// <li> Decide if this fit is better than the previous.
							if(fitresult.IsBetter()) bestKalmanFit = fitresult;
							/// </ol>
						} else {
							fLog << MSG::DEBUG << "Fit failed for event "
								<< fEventHeader->runNumber() << ":"
								<< fEventHeader->eventNumber() << endmsg;
						}
				}
				/// After loop over combintations:
				/// @b Write results of the Kalman kitematic fit <i>of the best combination</i> (`"fit4c_best"` branches).
				WriteFitResults(&bestKalmanFit, fNTuple_fit4c_best);
			}


		/// </ol>
		return StatusCode::SUCCESS;
	}


	/// Currently does nothing.
	/// See `TrackSelector::finalize` for what else is done when finalising.
	StatusCode D0phi_KpiKK::finalize_rest()
	{ PrintFunctionName("D0phi_KpiKK", __func__);
		return StatusCode::SUCCESS;
	}



// * =============================== * //
// * ------- PRIVATE METHODS ------- * //
// * =============================== * //


	/// Specification of what should be written to the fit `NTuple`.
	/// This function is called in `TrackSelector::WriteFitResults`.
	void D0phi_KpiKK::SetFitNTuple(KKFitResult *fitresults, NTupleContainer &tuple)
	{
		/// -# Convert to the derived object of `KKFitResult` designed for this package. @remark This cast is required and cannot be solved using virtual methods, because of the specific structure of each analysis.
			KKFitResult_D0phi_KpiKK* fit = dynamic_cast<KKFitResult_D0phi_KpiKK*>(fitresults);

		/// -# @warning Terminate if cast failed.
			if(!fit) {
				std::cout << "FATAL ERROR: Dynamic cast failed" << std::endl;
				std::terminate();
			}

		/// -# Set the `NTuple::Item`s.
			tuple.at("mD0")   = fit->fM_D0;
			tuple.at("mJpsi") = fit->fM_Jpsi;
			tuple.at("mphi")  = fit->fM_phi;
			tuple.at("pD0")   = fit->fP_D0;
			tuple.at("pphi")  = fit->fP_phi;
			tuple.at("chisq") = fit->fChiSquared;
	}


	/// This function encapsulates the `addItem` procedure for the fit branches.
	void D0phi_KpiKK::AddNTupleItems_Fit(NTupleContainer &tuple)
	{
		if(!tuple.DoWrite()) return;
		tuple.AddItem("mD0");   /// * `"mD0"`:   Invariant mass for \f$K^- \pi^+\f$ (\f$D^0\f$).
		tuple.AddItem("mphi");  /// * `"mphi"`:  Invariant mass for \f$K^+ K^+  \f$ (\f$\phi\f$).
		tuple.AddItem("mJpsi"); /// * `"mJpsi"`: Invariant mass for \f$D^0 \phi \f$ (\f$J/\psi\f$).
		tuple.AddItem("pD0");   /// * `"pD0"`:   3-momentum mass for the combination \f$K^- \pi^+\f$ (\f$D^0\f$ candidate).
		tuple.AddItem("pphi");  /// * `"pphi"`:  3-momentum mass for the combination \f$K^+ K^+  \f$ (\f$\phi\f$ candidate).
		tuple.AddItem("chisq"); /// * `"chisq"`: \f$\chi^2\f$ of the Kalman kinematic fit.
	}