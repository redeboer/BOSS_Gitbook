// * ========================= * //
// * ------- LIBRARIES ------- * //
// * ========================= * //

	#include "CLHEP/Geometry/Point3D.h"
	#include "CLHEP/Vector/LorentzVector.h"
	#include "CLHEP/Vector/ThreeVector.h"
	#include "CLHEP/Vector/TwoVector.h"
	#include "rhopi_pipigg/rhopi_pipigg.h"
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
	 * @brief Constructor for the `rhopi_pipigg` algorithm.
	 * @details Here, you should declare properties: give them a name, assign a parameter (data member of `rhopi_pipigg`), and if required a documentation string. Note that you should define the paramters themselves in the header (rhopi_pipigg/rhopi_pipigg.h) and that you should assign the values in `share/jopOptions_rhopi_pipigg.txt`.
	 */
	rhopi_pipigg::rhopi_pipigg(const std::string &name, ISvcLocator* pSvcLocator) :
		TrackSelector(name, pSvcLocator)
	{
		fLog << MSG::DEBUG << "===>> rhopi_pipigg::rhopi_pipigg() <<===" << endmsg;

		/// * The `"write_<treename>"` properties determine whether or not the corresponding `TTree`/`NTuple` will be filled. Default values are set in the constructor as well.
		declareProperty("write_fit4c_all",  fWrite_fit4c_all  = false);
		declareProperty("write_fit4c_best", fWrite_fit4c_best = false);
		declareProperty("write_fit5c_all",  fWrite_fit5c_all  = false);
		declareProperty("write_fit5c_best", fWrite_fit5c_best = false);

		/// * The `"cut_<parameter>"` properties determine cuts on certain parameters.
		declareProperty("cut_MaxGammaPhi",   fCut_MaxGammaPhi   =  20.0);
		declareProperty("cut_MaxGammaTheta", fCut_MaxGammaTheta =  20.0);
		declareProperty("cut_MaxGammaAngle", fCut_MaxGammaAngle = 200.0);

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
		fLog << MSG::INFO << "===>> rhopi_pipigg::initialize_rest() <<===" << endmsg;

		/// <ol type="A">
		/// <li> `"mult_select"`: Multiplicities of selected particles
			/// <ol>
			if(fWrite_mult_select) {
				fMap_mult_select["NPhotons"]; /// <li> `"NPhotons"`: Number of \f$\gamma\f$s.
				fMap_mult_select["NPionNeg"]; /// <li> `"NPionNeg"`: Number of \f$\pi^-\f$s.
				fMap_mult_select["NPionPos"]; /// <li> `"NPionPos"`: Number of \f$\pi^+\f$s.
				AddItemsToNTuples("mult_select", fMap_mult_select, "Multipliciies of selected particles");
			}
			/// </ol>

		/// <li> `"dedx_K"` and `"dedx_pi"`: energy loss \f$dE/dx\f$ PID branch. See `TrackSelector::BookNtupleItemsDedx` for more info.
			if(fWrite_dedx) {
				BookNtupleItemsDedx("dedx_pi", fMap_dedx_pi);
			}

		/// <li> `"fit4c_all"`, `"fit4c_best"`, `"fit5c_all"`, and `"fit5c_best"`: results of the Kalman kinematic fit results. See `TrackSelector::BookNtupleItemsDedx` for more info.
			if(fWrite_fit4c_all)  BookNtupleItemsFit("fit4c_all", fMap_fit4c_all, "4-constraint fit information (CMS 4-momentum)");
			if(fWrite_fit4c_best) BookNtupleItemsFit("fit4c_best", fMap_fit4c_best, "4-constraint fit information of the invariant masses closest to the reconstructed particles");
			if(fWrite_fit5c_all)  BookNtupleItemsFit("fit5c_all", fMap_fit5c_all, "5-constraint fit information (CMS 4-momentum)");
			if(fWrite_fit5c_best) BookNtupleItemsFit("fit5c_best", fMap_fit5c_best, "5-constraint fit information of the invariant masses closest to the reconstructed particles");

		/// <li> `"photon"`: information of the selected photons
			/// <ol>
			if(fWrite_photon) {
				fMap_photon["E"];     /// <li> Energy of the photon.
				fMap_photon["phi"];   /// <li> Smallest angle between the photon and the nearest charged pion.
				fMap_photon["theta"]; /// <li> Smallest angle between the photon and the nearest charged pion.
				fMap_photon["angle"]; /// <li> Smallest angle between the photon and the nearest charged pion.
				AddItemsToNTuples("photon", fMap_photon);
			}
			/// </ol>

		/// </ol>

		fLog << MSG::INFO << "Successfully returned from initialize()" << endmsg;
		return StatusCode::SUCCESS;
	}


	/**
	 * @brief Inherited `execute` method of the `Algorithm`. This function is called *for each event*.
	 */
	StatusCode rhopi_pipigg::execute_rest()
	{
		fLog << MSG::DEBUG << "===>> rhopi_pipigg::execute_rest() <<===" << endmsg;

		/// <ol type="A">
		/// <li> Create selection charged tracks

			// * Print log and set counters *
				fLog << MSG::DEBUG << "Starting particle selection:" << endmsg;
				fPIDInstance = ParticleID::instance();

			// * Clear vectors of selected particles *
				fPionNeg.clear();
				fPionPos.clear();

			// * Loop over charged tracks *
			for(fTrackIterator = fGoodChargedTracks.begin(); fTrackIterator != fGoodChargedTracks.end(); ++fTrackIterator) {
				/// <ol>
				/// <li> Initialise PID and skip if it fails:
					/// <ul>
					if(!InitializePID(
						fPIDInstance->methodProbability(),
							/// <li> use <b>probability method</b>
						fPIDInstance->useDedx() | fPIDInstance->useTof1() | fPIDInstance->useTof2() | fPIDInstance->useTofE(),
							/// <li> use \f$dE/dx\f$ and the three ToF detectors
						fPIDInstance->onlyPion(),
							/// <li> identify only pions
						4.0
							/// <li> use \f$\chi^2 > 4.0\f$
					)) continue;
					/// </ul>

				/// <li> <b>Write</b> Particle Identification information of all tracks
					if(fWrite_PID) WritePIDInformation();

				/// <li> Identify type of charged particle and add to related vector: (this package: only pions).
					RecMdcKalTrack* mdcKalTrk = (*fTrackIterator)->mdcKalTrack(); /// `RecMdcKalTrack` (Kalman) is used, but this can be substituted by `RecMdcTrack`.
					if(fPIDInstance->probPion() < fCut_MinPIDProb) continue; /// A cut is then applied on whether the probability to be a pion (or kaon) is at least `fCut_MinPIDProb` (see eventual settings in `rhopi_pipigg.txt`).
					if(fPIDInstance->pdf(RecMdcKalTrack::pion) < fPIDInstance->pdf(RecMdcKalTrack::kaon)) continue; /// If, according to the likelihood method, the particle is still more likely to be a kaon than a pion, the track is rejected.
					RecMdcKalTrack::setPidType(RecMdcKalTrack::pion); /// Finally, the particle ID of the `RecMdcKalTrack` object is set to pion
					if(mdcKalTrk->charge() > 0) fPionPos.push_back(*fTrackIterator); /// and the pions (\f$\pm\f$) are added to the respective vector of pions.
					else                        fPionNeg.push_back(*fTrackIterator);

				/// </ol>
			}


		/// <li> Create selection neutral tracks (photons)

			// * Print log and clear vectors of selected particles *
			fLog << MSG::DEBUG << "Starting 'good' neutral track selection:" << endmsg;

			// * Loop over charged tracks *
			fGamma.clear();
			for(fTrackIterator = fGoodNeutralTracks.begin(); fTrackIterator != fGoodNeutralTracks.end(); ++fTrackIterator) {

				/// <ol>
				/// <li> Get EM calorimeter info
					fTrackEMC = (*fTrackIterator)->emcShower();
					Hep3Vector emcpos = fTrackEMC->position();

				/// <li> Find angle differences with nearest charged pion
					fSmallestTheta = 1000.; // start value for difference in theta
					fSmallestPhi   = 1000.; // start value for difference in phi
					fSmallestAngle = 1000.; // start value for difference in angle (?)
					SetSmallestAngles(fPionNegIter, fPionNeg, emcpos); // check all differences with the pions
					SetSmallestAngles(fPionPosIter, fPionPos, emcpos); // check all differences with the pions

				/// <li> Apply angle cut
					if(fSmallestAngle >= fCut_MaxGammaAngle) continue;
					fSmallestTheta = fSmallestTheta * 180 / (CLHEP::pi);
					fSmallestPhi   = fSmallestPhi   * 180 / (CLHEP::pi);
					fSmallestAngle = fSmallestAngle * 180 / (CLHEP::pi);

				/// <li> <b>Write</b> photon info (`"photon"` branch)
					if(fWrite_photon) {
						fMap_photon.at("E")     = fTrackEMC->energy();
						fMap_photon.at("phi")   = fSmallestTheta;
						fMap_photon.at("theta") = fSmallestPhi;
						fMap_photon.at("angle") = fSmallestAngle;
						fNTupleMap.at("photon")->write();
					}

				/// <li> Apply photon cuts (energy cut has already been applied in TrackSelector)
					if(
						(fabs(fSmallestTheta) < fCut_MaxGammaTheta) &&
						(fabs(fSmallestPhi)   < fCut_MaxGammaPhi)) continue;
					if(fabs(fSmallestAngle) < fCut_MaxGammaAngle) continue;

				/// <li> Add photon track to vector for gammas
					fGamma.push_back(*fTrackIterator);

				/// </ol>
			}

			// * Finish good particle selection *
			fLog << MSG::DEBUG << "Number of photons: " << fGamma.size() << endmsg;
			fLog << MSG::DEBUG << "Number of pi+:     " << fPionPos.size() << endmsg;
			fLog << MSG::DEBUG << "Number of pi+:     " << fPionPos.size() << endmsg;

		/// <li> Apply a cut on the number of particles: <i>only events with more than 2 photons</i>
			fLog << MSG::DEBUG << "Number of good photons: " << fGamma.size() << endmsg;
			if(fGamma.size() < 2) return StatusCode::SUCCESS;


		/// <li> <b>Write</b> \f$dE/dx\f$ PID information (`"dedx_pi"` branch)
			if(fWrite_dedx) {
				WriteDedxInfoForVector(fPionNeg, "dedx_pi", fMap_dedx_pi);
				WriteDedxInfoForVector(fPionPos, "dedx_pi", fMap_dedx_pi);
			}


		/// <li> Perform Kalman 4-constraint Kalman kinematic fit for all combinations
			if(fWrite_fit4c_all || fWrite_fit4c_best) {
				double bestFitMeasure = 1e5;
				KalmanKinematicFit *bestKalmanFit = nullptr;
				for(fGamma1Iter  = fGamma.begin();   fGamma1Iter  != fGamma.end();   ++fGamma1Iter)
				for(fGamma2Iter  = fGamma1Iter+1;    fGamma2Iter  != fGamma.end();   ++fGamma2Iter)
				for(fPionPosIter = fPionPos.begin(); fPionPosIter != fPionPos.end(); ++fPionPosIter)
				for(fPionPosIter = fPionPos.begin(); fPionPosIter != fPionPos.end(); ++fPionPosIter)
				{

					// * Get Kalman tracks
						RecMdcKalTrack *pimTrk = (*fPionNegIter)->mdcKalTrack();
						RecMdcKalTrack *pipTrk = (*fPionPosIter)->mdcKalTrack();

					// * Get W-tracks
						WTrackParameter wvpimTrk(gM_pi, pimTrk->getZHelix(), pimTrk->getZError());
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
						vtxfit->AddTrack(0, wvpimTrk);
						vtxfit->AddTrack(1, wvpipTrk);
						vtxfit->AddVertex(0, vxpar, 0, 1);
						if(!vtxfit->Fit(0)) continue;
						vtxfit->Swim(0);

						WTrackParameter wpim = vtxfit->wtrk(0);
						WTrackParameter wpip = vtxfit->wtrk(1);

					// * Get EMC showers
						RecEmcShower *g1Trk = (*fGamma1Iter)->emcShower();
						RecEmcShower *g2Trk = (*fGamma2Iter)->emcShower();

					// * Get Kalman kinematic fit for this combination and store if better than previous one
						double chisq = 1e6;
						KalmanKinematicFit *kkmfit = KalmanKinematicFit::instance();
						kkmfit->init();
						kkmfit->AddTrack(0, wpim);       // pi-
						kkmfit->AddTrack(1, wpip);       // pi+
						kkmfit->AddTrack(2, 0.0, g1Trk); // gamma (1st occurrence)
						kkmfit->AddTrack(3, 0.0, g2Trk); // gamma (2nd occurence)
						kkmfit->AddFourMomentum(0, gEcmsVec); // 4 constraints: CMS energy and momentum
						if(kkmfit->Fit()) {
							/// Apply max. \f$\chi^2\f$ cut (determined by `fCut_MaxPIDChiSq`).
							if(kkmfit->chisq() > fCut_MaxPIDChiSq) continue;
							/// Compute the measure for the best Kalman kinematic fit and keep a pointer to this result if better than previous.
							ComputeInvariantMasses(kkmfit);
							CompareWithBestFit(MeasureForBestFit5c(), bestFitMeasure, kkmfit, bestKalmanFit);
							/// <b>Write</b> results of the Kalman kinematic fit (all combinations, `"fit4c_all"`).
							if(fWrite_fit4c_all) WriteFitResults(kkmfit, fMap_fit4c_all, "fit4c_all");
						}

				}

				/// <li> <b>Write</b> results of the Kalman kitematic fit <i>of the best combination</i> (`"fit4c_best"` branch)
				if(fWrite_fit4c_best && bestKalmanFit) {
					ComputeInvariantMasses(bestKalmanFit);
					WriteFitResults(bestKalmanFit, fMap_fit4c_best, "fit4c_best");
				}

			} // end of fWrite_fit4c_all || fWrite_fit4c_best


		/// <li> Apply a cut on the number of particles: <i>only events with one \f$\pi^-\f$ and one \f$\pi^+\f$</i>
			if(fPionNeg.size() != 1) return StatusCode::SUCCESS;
			if(fPionPos.size() != 1) return StatusCode::SUCCESS;


		/// <li> Perform Kalman 5-constraint Kalman kinematic fit for all combinations
			if(fWrite_fit5c_all || fWrite_fit5c_best) {
				// * Reset best fit parameters * //
				double bestFitMeasure = 1e5;
				KalmanKinematicFit *bestKalmanFit = nullptr;
				// * Loop over all combinations of pi-, pi+, and gamma * //
				for(fGamma1Iter  = fGamma.begin();   fGamma1Iter  != fGamma.end();   ++fGamma1Iter)
				for(fGamma2Iter  = fGamma1Iter+1;    fGamma2Iter  != fGamma.end();   ++fGamma2Iter)
				for(fPionPosIter = fPionPos.begin(); fPionPosIter != fPionPos.end(); ++fPionPosIter)
				for(fPionPosIter = fPionPos.begin(); fPionPosIter != fPionPos.end(); ++fPionPosIter)
				{

					// * Get Kalman tracks
						RecMdcKalTrack *pimTrk = (*fPionNegIter)->mdcKalTrack();
						RecMdcKalTrack *pipTrk = (*fPionPosIter)->mdcKalTrack();

					// * Get W-tracks
						WTrackParameter wvpimTrk(gM_pi, pimTrk->getZHelix(), pimTrk->getZError());
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
						vtxfit->AddTrack(0, wvpimTrk);
						vtxfit->AddTrack(1, wvpipTrk);
						vtxfit->AddVertex(0, vxpar, 0, 1);
						if(!vtxfit->Fit(0)) continue;
						vtxfit->Swim(0);

						WTrackParameter wpim = vtxfit->wtrk(0);
						WTrackParameter wpip = vtxfit->wtrk(1);

					// * Get EMC showers
						RecEmcShower *g1Trk = (*fGamma1Iter)->emcShower();
						RecEmcShower *g2Trk = (*fGamma2Iter)->emcShower();

					// * Get Kalman kinematic fit for this combination and store if better than previous one
						double chisq = 1e6;
						KalmanKinematicFit *kkmfit = KalmanKinematicFit::instance();
						kkmfit->init();
						kkmfit->AddTrack(0, wpim);       // pi-
						kkmfit->AddTrack(1, wpip);       // pi+
						kkmfit->AddTrack(2, 0.0, g1Trk); // gamma (1st occurrence)
						kkmfit->AddTrack(3, 0.0, g2Trk); // gamma (2nd occurence)
						kkmfit->AddResonance(0, gM_pi0, 2, 3); // 5th constraint: pi0 resonance
						kkmfit->AddFourMomentum(1, gEcmsVec); // 4 constraints: CMS energy and momentum
						if(kkmfit->Fit()) {
							/// Apply max. \f$\chi^2\f$ cut (determined by `fCut_MaxPIDChiSq`).
							if(kkmfit->chisq() > fCut_MaxPIDChiSq) continue;
							/// Compute the measure for the best Kalman kinematic fit and keep a pointer to this result if better than previous.
							ComputeInvariantMasses(kkmfit);
							CompareWithBestFit(MeasureForBestFit5c(), bestFitMeasure, kkmfit, bestKalmanFit);
							/// <b>Write</b> results of the Kalman kinematic fit (all combinations, `"fit5c_all"`).
							if(fWrite_fit5c_all) WriteFitResults(kkmfit, fMap_fit5c_all, "fit5c_all");
						}

				} // end of loop over particle combinations

				/// <li> <b>Write</b> results of the Kalman kitematic fit <i>of the best combination</i> (`"fit5c_best"` branch)
				if(fWrite_fit5c_best && bestKalmanFit) {
					ComputeInvariantMasses(bestKalmanFit);
					WriteFitResults(bestKalmanFit, fMap_fit5c_best, "fit5c_best");
				}

			} // end of fWrite_fit5c_all || fWrite_fit5c_best


		/// </ol>
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
	 * @brief Encapsulation of the procedure to write results of the Kalman kinematic fit (no matter how many constrains).
	 */
	HepLorentzVector rhopi_pipigg::ComputeGammaVector(EvtRecTrack* track)
	{
		RecEmcShower* emcTrk = track->emcShower();
		double eraw  = emcTrk->energy();
		double phi   = emcTrk->phi();
		double theta = emcTrk->theta();
		HepLorentzVector ptrk(
			eraw * sin(theta) * cos(phi),
			eraw * sin(theta) * sin(phi),
			eraw * cos(theta),
			eraw);
		// ptrk = ptrk.boost(-0.011, 0, 0); /// Perform a boost to the CMS
		return ptrk;
	}


	/**
	 * @brief Method that provides a measure for the best Kalman kinematic fit4c.
	 * @details The closer this value to zero, the better the result.
	 */
	double rhopi_pipigg::MeasureForBestFit4c()
	{
		return (fM_pi0-gM_pi0)/gM_pi0;
	}

	/**
	 * @brief Method that provides a measure for the best Kalman kinematic fit5c.
	 * @details The closer this value to zero, the better the result.
	 */
	double rhopi_pipigg::MeasureForBestFit5c()
	{
		return
			((fM_JpsiRho0-gM_Jpsi)/gM_Jpsi) *
			((fM_JpsiRhom-gM_Jpsi)/gM_Jpsi) *
			((fM_JpsiRhop-gM_Jpsi)/gM_Jpsi);
	}


	/**
	 * @brief This function encapsulates the `addItem` procedure for the fit branches.
	 */ 
	void rhopi_pipigg::BookNtupleItemsFit(const char* tupleName, std::map<std::string, NTuple::Item<double> > &map, const char* tupleTitle)
	{
		/// <ol>
		map["Egamma1"];    /// <li> Energy of the first photon.
		map["Egamma2"];    /// <li> Energy of the second photon.
		map["cosAngle"];   /// <li> Cosine of the angle between the two photons.
		map["mpi0"];       /// <li> Invariant mass for \f$\pi^0 \rightarrow \gamma\gamma\f$ candidate.
		map["mrho0"];      /// <li> Invariant mass for \f$\rho^0 \rightarrow \pi^-\pi^+\f$ candidate.
		map["mrho-"];      /// <li> Invariant mass for \f$\rho^- \rightarrow \pi^0\pi^-\f$ candidate.
		map["mrho+"];      /// <li> Invariant mass for \f$\rho^+ \rightarrow \pi^0\pi^+\f$ candidate.
		map["mJpsi_rho0"]; /// <li> Invariant mass for \f$J/\psi \rightarrow \rho^0\pi^0\f$ candidate.
		map["mJpsi_rho-"]; /// <li> Invariant mass for \f$J/\psi \rightarrow \rho^-\pi^+\f$ candidate.
		map["mJpsi_rho+"]; /// <li> Invariant mass for \f$J/\psi \rightarrow \rho^+\pi^-\f$ candidate.
		map["chisq"];      /// <li> \f$\chi^2\f$ of the Kalman kinematic fit.
		AddItemsToNTuples(tupleName, map, tupleTitle);
		/// </ol>
	}


	/**
	 * @brief Encapsulation of the procedure to compute invariant masses of the reconstructed particles.
	 */
	void rhopi_pipigg::ComputeInvariantMasses(KalmanKinematicFit *kkmfit)
	{
		// * Get vectors * //
		HepLorentzVector ppim    = kkmfit->pfit(0);
		HepLorentzVector ppip    = kkmfit->pfit(1);
		HepLorentzVector pgamma1 = kkmfit->pfit(2);
		HepLorentzVector pgamma2 = kkmfit->pfit(3);
		// * Primary candidates * //
		HepLorentzVector ppi0  = pgamma1 + pgamma2; /// Invariant mass for \f$\pi^0 \rightarrow \gamma\gamma\f$ candidate.
		HepLorentzVector prho0 = ppim + ppip;       /// Invariant mass for \f$\rho^0 \rightarrow \pi^-\pi^+\f$ candidate.
		HepLorentzVector prhom = ppi0 + ppim;       /// Invariant mass for \f$\rho^- \rightarrow \pi^0\pi^-\f$ candidate.
		HepLorentzVector prhop = ppi0 + ppip;       /// Invariant mass for \f$\rho^+ \rightarrow \pi^0\pi^+\f$ candidate.
		// * Three J/psi candidates * //
		HepLorentzVector pJpsiRho0 = prho0 + ppi0; /// Invariant mass for \f$J/\psi \rightarrow \rho^0\pi^0\f$ candidate.
		HepLorentzVector pJpsiRhom = prhom + ppip; /// Invariant mass for \f$J/\psi \rightarrow \rho^-\pi^+\f$ candidate.
		HepLorentzVector pJpsiRhop = prhop + ppim; /// Invariant mass for \f$J/\psi \rightarrow \rho^+\pi^-\f$ candidate.
		// * Compute gamma energies and angle between the two photons * //
		fE_gamma1 = pgamma1.e();
		fE_gamma2 = pgamma2.e();
		fCosGamma = abs(fE_gamma1-fE_gamma2) / ppi0.rho();
		// * Compute invariant masses * //
		fM_pi0      = ppi0.m();
		fM_rho0     = prho0.m();
		fM_rhom     = prhom.m();
		fM_rhop     = prhop.m();
		fM_JpsiRho0 = pJpsiRho0.m();
		fM_JpsiRhom = pJpsiRhom.m();
		fM_JpsiRhop = pJpsiRhop.m();
	}


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


	/**
	 * @brief Encapsulation of the procedure to write results of the Kalman kinematic fit (no matter how many constrains).
	 */
	void rhopi_pipigg::WriteFitResults(KalmanKinematicFit *kkmfit, std::map<std::string, NTuple::Item<double> > &map, const char *tupleName)
	{
		fLog << MSG::DEBUG << "Writing fit results \"" << tupleName << "\"" << endmsg;
		map.at("mpi0")       = fM_pi0;
		map.at("mrho0")      = fM_rho0;
		map.at("mrho-")      = fM_rhom;
		map.at("mrho+")      = fM_rhop;
		map.at("mJpsi_rho0") = fM_JpsiRho0;
		map.at("mJpsi_rho-") = fM_JpsiRhom;
		map.at("mJpsi_rho+") = fM_JpsiRhop;
		map.at("chisq")      = kkmfit->chisq();
		fNTupleMap.at(tupleName)->write();
	}